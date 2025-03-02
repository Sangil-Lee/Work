#include <stdlib.h>

#include <epicsEndian.h>

#include "s7nodave.h"

#include "S7nodaveRecord.h"

namespace s7nodave {

S7nodaveRecord::S7nodaveRecord(dbCommon *record, s7nodaveRecordType recordType) {
    this->record = record;
    this->recordType = recordType;
    this->deviceAddress = 0;

    // Buffers, etc. for new data
    this->processCallbackError = false;
    this->newDataLength = 0;
    this->newData = nullptr;

    // Initialize structures for asyn
    this->myAsynUser = nullptr;
    this->myAsynCommonInterface = nullptr;
    this->myAsynCommon = nullptr;
    this->myAsynS7nodaveInterface = nullptr;
    this->myAsynS7nodave = nullptr;
}

S7nodaveRecord::~S7nodaveRecord() {
}

long S7nodaveRecord::initRecordError() {
    // Set PACT to 1 so that record will not be processed.
    this->record->pact = 1;
    if (this->myAsynUser) {
        int connected = 0;
        pasynManager->isConnected(this->myAsynUser, &connected);
        if (connected) {
            pasynManager->disconnect(this->myAsynUser);
        }
        pasynManager->freeAsynUser(this->myAsynUser);
        this->myAsynUser = nullptr;
    }
    this->myAsynCommonInterface = nullptr;
    this->myAsynCommon = nullptr;
    this->myAsynS7nodaveInterface = nullptr;
    this->myAsynS7nodave = nullptr;
    return RECORD_STATUS_ERROR;
}

long S7nodaveRecord::initRecord() {
    // createAsynUser always succeeds (otherwise the thread is suspended)
    this->myAsynUser = pasynManager->createAsynUser(asynProcessCallbackStatic, nullptr);
    this->myAsynUser->userPvt = this;

    // Initialize device address
    DBLINK addr = this->getDeviceAddress();
    if (addr.type != INST_IO) {
        asynPrint(this->myAsynUser, ASYN_TRACE_ERROR, "%s S7nodaveRecord::initRecord Invalid device address. Maybe mixed up INP/OUT or forgot '@'?\n", this->record->name);
        return initRecordError();
    }
    char *deviceAddressCharString = addr.value.instio.string;
    std::string deviceAddressString = (deviceAddressCharString == nullptr) ? "" : deviceAddressCharString;
    if (deviceAddressString.size() == 0) {
        asynPrint(this->myAsynUser, ASYN_TRACE_ERROR, "%s S7nodaveRecord::initRecord Invalid device address. Maybe mixed up INP/OUT or forgot '@'?\n", this->record->name);
        return initRecordError();
    }
    // Parse device address string
    auto parserResult = S7nodaveRecordAddress::parseRecordAddress(deviceAddressString);
    // Check results
    std::string portName;
    Optional<S7nodaveRecordAddress::DeviceParameters> deviceParameters;
    Optional<PlcAddress> plcAddress;
    Optional<s7nodavePlcDataType> plcDataType;
    bool parserResultValid;
    std::tie(portName, deviceParameters, plcAddress, plcDataType, parserResultValid) = parserResult;
    // plcDataType might be empty even if the specified address is valid, so we
    // do not check it in the if clause.
    if (!parserResultValid || portName.size() == 0 || !deviceParameters || !plcAddress) {
        // Find out which part caused the problem. The order of the checks
        // matters because if one part is empty because of an error, the other
        // parts will be empty as well, even if they did not cause the problem.
        // plcDataType is special because it being empty is not an error by
        // itself, but if parserResultValid is not set and it is the only empty
        // return value, it must be the cause of the problem.
        if (portName.size() == 0) {
            asynPrint(this->myAsynUser, ASYN_TRACE_ERROR, "%s S7nodaveRecord::initRecord Invalid port name in device address \"@%s\".\n", this->record->name, deviceAddressCharString);
        } else if (!deviceParameters) {
            asynPrint(this->myAsynUser, ASYN_TRACE_ERROR, "%s S7nodaveRecord::initRecord Invalid device parameters in device address \"@%s\".\n", this->record->name, deviceAddressCharString);
        } else if (!plcAddress) {
            asynPrint(this->myAsynUser, ASYN_TRACE_ERROR, "%s S7nodaveRecord::initRecord Invalid PLC memory address in device address \"@%s\".\n", this->record->name, deviceAddressCharString);
        } else if (!plcDataType) {
            asynPrint(this->myAsynUser, ASYN_TRACE_ERROR, "%s S7nodaveRecord::initRecord Invalid PLC data type in device address \"@%s\".\n", this->record->name, deviceAddressCharString);
        }
        return initRecordError();
    }

    // Extract device parameters. Make a copy first so that we can still append
    // them to the record address.
    S7nodaveRecordAddress::DeviceParameters originalDeviceParameters = *deviceParameters;
    this->extractDeviceParameters(*deviceParameters);
    if (!deviceParameters->empty()) {
        std::string firstParameterName = deviceParameters->begin()->first;
        asynPrint(this->myAsynUser, ASYN_TRACE_ERROR, "%s S7nodaveRecord::initRecord Invalid device parameter \"%s\" in device address \"@%s\".\n", this->record->name, firstParameterName.c_str(), deviceAddressCharString);
        return initRecordError();
    }

    // Enable child classes to narrow down the allowed data-types or suggest
    // a data type if none was specified.
    plcDataType = getPlcDataType(*plcAddress, plcDataType);
    if (!plcDataType) {
        asynPrint(this->myAsynUser, ASYN_TRACE_ERROR, "%s S7nodaveRecord::initRecord No data-type specified in device address  \"@%s\" or specified data-type not allowed for this record.\n", this->record->name, deviceAddressCharString);
        return initRecordError();
    }
    Optional<S7nodaveRecordAddress> recordAddress = S7nodaveRecordAddress::create(portName, *plcAddress, *plcDataType, originalDeviceParameters);
    if (!recordAddress) {
        asynPrint(this->myAsynUser, ASYN_TRACE_ERROR, "%s S7nodaveRecord::initRecord Invalid device address \"@%s\". Maybe the type of the specified memory address and the data type do not match?\n", this->record->name, deviceAddressCharString);
        return initRecordError();
    }
    this->recordAddress = recordAddress;

    // Connect asynUser to device
    asynStatus status = pasynManager->connectDevice(this->myAsynUser, this->recordAddress->getPortName().c_str(), this->deviceAddress);
    if (status != asynSuccess) {
        asynPrint(this->myAsynUser, ASYN_TRACE_ERROR, "%s S7nodaveRecord::initRecord pasynManager->connectDevice %s", this->record->name, this->myAsynUser->errorMessage);
        return initRecordError();
    }
    // Find needed interfaces
    this->myAsynCommonInterface = pasynManager->findInterface(this->myAsynUser, asynCommonType, 1);
    if (!this->myAsynCommonInterface) {
        asynPrint(this->myAsynUser, ASYN_TRACE_ERROR, "pasynManager->findInterface() did not return interface %s for port %s and record %s.\n", asynCommonType, this->recordAddress->getPortName().c_str(), this->record->name);
        return initRecordError();
    }
    this->myAsynCommon = static_cast<asynCommon *>(this->myAsynCommonInterface->pinterface);
    this->myAsynS7nodaveInterface = pasynManager->findInterface(this->myAsynUser, asynS7nodaveType, 1);
    if (!this->myAsynS7nodaveInterface) {
        asynPrint(this->myAsynUser, ASYN_TRACE_ERROR, "pasynManager->findInterface() did not return interface %s for port %s and record %s.\n", asynCommonType, this->recordAddress->getPortName().c_str(), this->record->name);
        return initRecordError();
    }
    this->myAsynS7nodave = static_cast<asynS7nodave *>(this->myAsynS7nodaveInterface->pinterface);

    return RECORD_STATUS_OK;
}

long S7nodaveRecord::convertRecord(int) {
    // No conversion
    return RECORD_STATUS_OK;
}

long S7nodaveRecord::getIoIntInfoRecord(int, IOSCANPVT *) {
    return RECORD_STATUS_ERROR;
}

void S7nodaveRecord::asynProcessCallbackStatic(asynUser *pasynUser) {
    S7nodaveRecord *s7nodaveRecord = static_cast<S7nodaveRecord *>(pasynUser->userPvt);
    s7nodaveRecord->asynProcessCallback();
}

Optional<s7nodavePlcDataType> S7nodaveRecord::getPlcDataType(PlcAddress plcAddress, Optional<s7nodavePlcDataType>) {
    switch (PlcAddress::dataSizeInBits(plcAddress.getDataSize())) {
    case 1:
        return plcDataTypeBool;
    case 8:
        return plcDataTypeInt8;
    case 16:
        return plcDataTypeInt16;
    case 32:
        return plcDataTypeInt32;
    default:
        return Optional<s7nodavePlcDataType>();
    }
}

void S7nodaveRecord::extractDeviceParameters(S7nodaveRecordAddress::DeviceParameters&) {
    // There are no device parameters supported by all record types (yet).
}

unsigned long int S7nodaveRecord::getIoBufferSizeInBits() const {
    return static_cast<unsigned long int>(PlcAddress::dataSizeInBits(this->recordAddress->getPlcAddress().getDataSize()));
}

int S7nodaveRecord::getIoBufferSizeInBytes() const {
    // Calculate buffer size
    // The driver interface only allows for reading complete bytes.
    // Therefore we have to account for the bits in the last partly used byte.
    unsigned long bufferSizeInBits = getIoBufferSizeInBits();
    int bufferSize = bufferSizeInBits / 8;
    if (bufferSizeInBits % 8 != 0) {
        bufferSize += 1;
    }
    return bufferSize;
}

void S7nodaveRecord::convertFromOrToPlcByteOrder(void *buffer, int bufferSize, s7nodavePlcDataType plcDataType) {
    unsigned char *bufferAsChar = static_cast<unsigned char *>(buffer);
    switch (plcDataType) {
    case plcDataTypeBool:
    case plcDataTypeInt8:
    case plcDataTypeUint8:
        // Bool and byte do not need conversion
        break;
    case plcDataTypeInt16:
    case plcDataTypeUint16:
#if EPICS_BYTE_ORDER == EPICS_ENDIAN_BIG
        // No conversion needed for big endian architectures
        break;
#elif EPICS_BYTE_ORDER == EPICS_ENDIAN_LITTLE
        for (int pos = 0; pos < bufferSize; pos += 2) {
            unsigned char c = bufferAsChar[pos];
            bufferAsChar[pos] = bufferAsChar[pos + 1];
            bufferAsChar[pos + 1] = c;
        }
        break;
#else
#error EPICS_BYTE_ORDER is neither EPICS_ENDIAN_BIG nor EPICS_ENDIAN_LITTLE
#endif
    case plcDataTypeInt32:
    case plcDataTypeUint32:
#if EPICS_BYTE_ORDER == EPICS_ENDIAN_BIG
        // No conversion needed for big endian architectures
        break;
#elif EPICS_BYTE_ORDER == EPICS_ENDIAN_LITTLE
        for (int pos = 0; pos < bufferSize; pos += 4) {
            unsigned char c = bufferAsChar[pos];
            bufferAsChar[pos] = bufferAsChar[pos + 3];
            bufferAsChar[pos + 3] = c;
            c = bufferAsChar[pos + 1];
            bufferAsChar[pos + 1] = bufferAsChar[pos + 2];
            bufferAsChar[pos + 2] = c;
        }
        break;
#else
#error EPICS_BYTE_ORDER is neither EPICS_ENDIAN_BIG nor EPICS_ENDIAN_LITTLE
#endif
    case plcDataTypeFloat:
#if EPICS_FLOAT_WORD_ORDER == EPICS_ENDIAN_BIG
        // No conversion needed for big endian architectures
        break;
#elif EPICS_FLOAT_WORD_ORDER == EPICS_ENDIAN_LITTLE
        for (int pos = 0; pos < bufferSize; pos += 4) {
            unsigned char c = bufferAsChar[pos];
            bufferAsChar[pos] = bufferAsChar[pos + 3];
            bufferAsChar[pos + 3] = c;
            c = bufferAsChar[pos + 1];
            bufferAsChar[pos + 1] = bufferAsChar[pos + 2];
            bufferAsChar[pos + 2] = c;
        }
        break;
#else
#error EPICS_FLOAT_WORD_ORDER is neither EPICS_ENDIAN_BIG nor EPICS_ENDIAN_LITTLE
#endif
    }
}

const unsigned char S7nodaveRecord::bitMasks[] = {0, 1, 3, 7, 15, 31, 63, 127, 255};

bool S7nodaveRecord::readFromPlc(void *buffer, int bufferSize) const {
    bool success = true;
    PlcAddress plcAddress = this->recordAddress->getPlcAddress();
    unsigned long bufferSizeInBits = getIoBufferSizeInBits();
    int bytesRead;
    if (!buffer) {
        asynPrint(this->myAsynUser, ASYN_TRACE_ERROR, "%s S7nodaveRecord::readFromPlc Got NULL pointer instead of buffer.\n", record->name);
        return false;
    }
    // If needIntermediateBuffer is set (see below) we might have to use a
    // different buffer. Otherwise we just set these variables to use the buffer
    // we already have.
    void *bufferUsed;
    int bufferUsedSize;
    s7nodavePlcDataType plcDataType = this->recordAddress->getPlcDataType();
    bool needIntermediateBuffer = (plcDataType == plcDataTypeBool && plcAddress.getStartBit() != 0);
    if (needIntermediateBuffer) {
        // We have to use a different buffer for reading
        bufferUsedSize = (bufferSizeInBits + plcAddress.getStartBit()) / 8;
        if ((bufferSizeInBits + plcAddress.getStartBit()) % 8 != 0) {
            bufferUsedSize += 1;
        }
        bufferUsed = pasynManager->memMalloc(bufferUsedSize);
        if (!bufferUsed) {
            asynPrint(this->myAsynUser, ASYN_TRACE_ERROR, "%s S7nodaveRecord::readFromPlc Allocation of %d bytes of memory failed.\n", record->name, bufferUsedSize);
            return false;
        }
    } else {
        // We can directly use the given buffer
        bufferUsed = buffer;
        bufferUsedSize = bufferSize;
    }
    asynStatus status = this->myAsynS7nodave->plcRead(this->myAsynS7nodaveInterface->drvPvt, this->myAsynUser, plcAddress.getArea(), plcAddress.getAreaNumber(), plcAddress.getStartByte(), bufferUsedSize, bufferUsed, &bytesRead);
    if (status != asynSuccess || bytesRead != bufferSize) {
        success = false;
    }
    if (needIntermediateBuffer) {
        if (success) {
            // We have to rearrange the bits in order to start with the first
            // requested bit.
            int startBit = plcAddress.getStartBit();
            unsigned char *bufferAsChar = static_cast<unsigned char *>(bufferUsed);
            unsigned char *buffer2AsChar = static_cast<unsigned char *>(buffer);
            for (int pos = 0; pos < bufferSize; pos++) {
                int b = bufferAsChar[pos] >> startBit;
                b &= bitMasks[8-startBit];
                if (pos + 1 < bufferUsedSize) {
                    int b2 = bufferAsChar[pos + 1] << (8 - startBit);
                    b2 &= ~bitMasks[8-startBit];
                    buffer2AsChar[pos] = b | b2;
                } else {
                    // Because of the number of bits that have been
                    // requested, we do not have to fill up the last
                    // byte.
                    buffer2AsChar[pos] = b;
                }
            }
            // Make sure unused bits of last byte are zero
            if (bufferSizeInBits % 8 != 0) {
                int lastByteBits = bufferSizeInBits % 8;
                buffer2AsChar[bufferSize - 1] = buffer2AsChar[bufferSize - 1] & bitMasks[lastByteBits];
            }
        }
        // Always free the allocated memory.
        if (bufferUsed) {
            pasynManager->memFree(bufferUsed, bufferUsedSize);
        }
        bufferUsed = nullptr;
    } else if (plcDataType == plcDataTypeBool && plcAddress.getStartBit() == 0 && bufferSizeInBits % 8 != 0) {
        // If we requested a boolean starting at bit zero, we do not have to
        // realign the data. However, we might have to mask the last byte, if
        // the number of bits requested is not an integer multiple of eight.
        if (success) {
            if (bufferSize > 0) {
                unsigned char *bufferAsChar = static_cast<unsigned char *>(buffer);
                bufferAsChar[bufferSize - 1] &= bitMasks[bufferSizeInBits % 8];
            }
        }
    }
    if (success) {
        convertFromOrToPlcByteOrder(buffer, bufferSize, plcDataType);
    }
    return success;
}

bool S7nodaveRecord::writeToPlc(void *buffer, int bufferSize) const {
    bool success = true;
    PlcAddress plcAddress = this->recordAddress->getPlcAddress();
    s7nodavePlcDataType plcDataType = this->recordAddress->getPlcDataType();
    unsigned long bufferSizeInBits = getIoBufferSizeInBits();
    convertFromOrToPlcByteOrder(buffer, bufferSize, plcDataType);
    int startBit = plcAddress.getStartBit();
    // If we have an address that is not starting at the beginning of a byte
    // we have to rearrange the data.
    bool needIntermediateBuffer = (plcDataType == plcDataTypeBool && startBit != 0);
    void *bufferUsed = nullptr;
    int bufferUsedSize = 0;
    if (needIntermediateBuffer) {
        // The intermediate buffer might be one byte bigger
        bufferUsedSize = (bufferSizeInBits + startBit) / 8;
        if ((bufferSizeInBits + startBit) % 8 != 0) {
            bufferUsedSize += 1;
        }
        bufferUsed = pasynManager->memMalloc(bufferUsedSize);
        if (!bufferUsed) {
            asynPrint(this->myAsynUser, ASYN_TRACE_ERROR, "%s S7nodaveRecord::writeToPlc Allocation of %d bytes of memory failed.\n", record->name, bufferUsedSize);
            return false;
        }
        // We have to rearrange the bits in order to start with the first
        // bit of the first byte (partially) written.
        unsigned char *bufferAsChar = static_cast<unsigned char *>(buffer);
        unsigned char *buffer2AsChar = static_cast<unsigned char *>(bufferUsed);
        for (int pos = 0; pos < bufferUsedSize; pos++) {
            int b = 0;
            if (pos < bufferSize) {
                // The last byte is only a partial byte, so we do not
                // have to run this part.
                b = bufferAsChar[pos] << startBit;
                b &= ~bitMasks[startBit];
            }
            if (pos > 0) {
                // The first byte is only a partial byte, so we do not
                // have to run this part.
                int b2 = bufferAsChar[pos - 1] >> (8 - startBit);
                b2 &= bitMasks[8 - startBit];
                buffer2AsChar[pos] = b | b2;
            } else {
                buffer2AsChar[pos] = b;
            }
        }
    } else {
        bufferUsed = buffer;
        bufferUsedSize = bufferSize;
    }

    asynStatus status;
    unsigned long int numBitsWritten = 0;
    int bytesOffset = 0;
    int bytesWriteLength = bufferUsedSize;
    if ((plcDataType == plcDataTypeBool) && ((bufferSizeInBits + startBit) % 8 != 0)) {
        // The last byte is a partial byte, so we should write one byte less.
        bytesWriteLength--;
    }
    if (needIntermediateBuffer) {
        // We have to write the bits of the partial byte at the start
        // individually.
        for (int bitNumber = startBit; bitNumber < 8 && bitNumber; bitNumber ++) {
            unsigned char bitValue = (static_cast<unsigned char *>(bufferUsed))[0] & (0x01 << bitNumber);
            status = this->myAsynS7nodave->plcWriteBit(this->myAsynS7nodaveInterface->drvPvt, this->myAsynUser, plcAddress.getArea(), plcAddress.getAreaNumber(), plcAddress.getStartByte(), bitNumber, bitValue);
            if (status != asynSuccess) {
                success = false;
                break;
            }
            numBitsWritten++;
            if (numBitsWritten == bufferSizeInBits) {
                break;
            }
        }
        // Do not write first byte of buffer
        bytesOffset = 1;
        bytesWriteLength--;
    }
    if (success && (bufferSizeInBits - numBitsWritten) >= 8) {
        int bytesWritten;
        status = this->myAsynS7nodave->plcWrite(this->myAsynS7nodaveInterface->drvPvt, this->myAsynUser, plcAddress.getArea(), plcAddress.getAreaNumber(), plcAddress.getStartByte() + bytesOffset, bytesWriteLength, bufferUsed, &bytesWritten);
        if (status != asynSuccess || bytesWritten != bytesWriteLength) {
            success = false;
        }
        numBitsWritten += 8 * bytesWriteLength;
    }
    if (success && numBitsWritten < bufferSizeInBits) {
        // The last byte is a partial byte
        unsigned char lastByte = (static_cast<unsigned char *>(buffer))[bytesOffset + bytesWriteLength];
        int endBit = startBit + (bufferSizeInBits % 8);
        for (int bitNumber = 0; bitNumber < endBit; bitNumber++) {
            unsigned char bitValue = lastByte & (0x01 << bitNumber);
            status = this->myAsynS7nodave->plcWriteBit(this->myAsynS7nodaveInterface->drvPvt, this->myAsynUser, plcAddress.getArea(), plcAddress.getAreaNumber(), plcAddress.getStartByte() + bytesOffset + bytesWriteLength, bitNumber, bitValue);
            if (status != asynSuccess) {
                success = false;
                break;
            }
        }
    }
    if (needIntermediateBuffer) {
        // Free buffer memory
        if (bufferUsed) {
            pasynManager->memFree(bufferUsed, bufferUsedSize);
            bufferUsed = nullptr;
        }
    }
    return success;
}

} // namespace s7nodave
