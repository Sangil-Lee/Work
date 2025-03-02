#include <stdlib.h>
#include <string.h>

#include <alarm.h>
#include <callback.h>
#include <epicsEndian.h>
#include <epicsGuard.h>
#include <recGbl.h>

#include "PollGroup.h"
#include "s7nodave.h"

#include "S7nodaveInputRecord.h"

namespace s7nodave {

void S7nodaveInputRecord::asynProcessCallback() {
    bool success = true;
    int bufferSize = this->getIoBufferSizeInBytes();
    void *buffer = pasynManager->memMalloc(bufferSize);
    if (!buffer) {
        asynPrint(this->myAsynUser, ASYN_TRACE_ERROR, "%s S7nodaveInputRecord::asynProcessCallback Allocation of %d bytes of memory failed.\n", record->name, bufferSize);
        success = false;
    }
    if (success) {
        success = this->readFromPlc(buffer, bufferSize);
    }
    if (!success) {
        if (buffer) {
            pasynManager->memFree(buffer, bufferSize);
        }
        buffer = nullptr;
        bufferSize = 0;
    }
    // We have to lock the mutex in order to synchronize with the get data
    // process in processRecord().
    {
        epicsGuard<epicsMutex> guard(this->newDataMutex);
        // Check that newData is empty, otherwise we have to free it in order
        // to avoid a memory leak.
        if (this->newData) {
            pasynManager->memFree(this->newData, this->newDataLength);
        }
        this->newData = buffer;
        this->newDataLength = bufferSize;
        this->processCallbackError = !success;
    }

    // Although it is not obvious, we can check record->pact here without having
    // to lock a mutex. A mutex in asynManager ensures, that the thread
    // executing this code will see a change of record->pact performed by the
    // thread executing processRecord().
    if (record->pact) {
        asynPrint(this->myAsynUser, ASYN_TRACE_FLOW, "%s S7nodaveInputRecord::asynProcessCallback queuing request for process callback to be run again.\n", this->record->name);
        callbackRequestProcessCallback(&this->processCallback, this->record->prio, this->record);
    }
}

long S7nodaveInputRecord::initRecord() {
    long status = S7nodaveRecord::initRecord();
    if (status == RECORD_STATUS_OK) {
        // Initialize "I/O Intr" scan support
        scanIoInit(&this->ioScanPvt);
    }
    return status;
}

long S7nodaveInputRecord::processRecord() {
    bool haveNewData = false;
    bool processCallbackError = false;
    void *newData = nullptr;
    size_t newDataLength = 0;
    // We have to lock a mutex in order to synchronize with
    // asynProccessCallback(), which places the new data in the corresponding
    // attributes.
    {
        epicsGuard<epicsMutex> guard(this->newDataMutex);
        newData = this->newData;
        newDataLength = this->newDataLength;
        haveNewData = (newData != nullptr);
        processCallbackError = this->processCallbackError;
        this->processCallbackError = false;
        this->newData = nullptr;
        this->newDataLength = 0;
    }
    if (processCallbackError) {
        // An error occurred while trying to read data for this record.
        if (newData) {
            pasynManager->memFree(newData, newDataLength);
            newData = nullptr;
        }
        recGblSetSevr(this->record, READ_ALARM, INVALID_ALARM);
        // We still return RECORD_STATUS_OK, because it was not this method
        // where the error happened.
        return RECORD_STATUS_OK;
    }
    if (!haveNewData && !this->record->pact) {
        // Process has been called without new data present or the PACT flag
        // set. This means that an asynchronous read should be performed.
        // We have to set PACT in order to signal that the record is being
        // processed asynchronously and to avoid processRecord being called a
        // second time, before the asynchronous read finished.
        this->record->pact = 1;
        // Now we queue a callback request that will be executed in the port
        // thread.
        asynPrint(this->myAsynUser, ASYN_TRACE_FLOW, "%s S7nodaveInputRecord::processRecord calling queueRequest\n", this->record->name);
        asynStatus status = pasynManager->queueRequest(this->myAsynUser, asynQueuePriorityMedium, 0);
        if (status == asynSuccess) {
            return RECORD_STATUS_OK;
        } else {
            // The queue request failed. With have to reset PACT to give this
            // method the chance of being called again later.
            this->record->pact = 0;
            asynPrint(this->myAsynUser, ASYN_TRACE_ERROR, "%s S7nodaveRecord::processRecord queueRequest %s\n", record->name, this->myAsynUser->errorMessage);
            recGblSetSevr(this->record, READ_ALARM, INVALID_ALARM);
        }
    }
    long status = RECORD_STATUS_OK;
    if (haveNewData) {
        // We received new data, so we should update the record's value.
        status = this->writeToRecord(newData, newDataLength);
        // Unset UDF flag, because this might have been the first time that
        // the record has been read.
        this->record->udf = 0;
        pasynManager->memFree(newData, newDataLength);
        newData = nullptr;
    }
    return status;
}

long S7nodaveInputRecord::getIoIntInfoRecord(int command, IOSCANPVT *iopvt) {
    // Unlike processRecord, getIoIntInfoRecord might be called even if the
    // device support for the record has not been initialized properly. In this
    // case myAsynUser will be null and the following code will lead to a
    // segmentation fault, so we check for this condition and return in this
    // case.
    if (!this->myAsynUser) {
        return RECORD_STATUS_ERROR;
    }
    std::shared_ptr<PollGroup> pollGroup = PollGroup::find(this->recordAddress->getPortName(), this->pollGroupName);
    if (!pollGroup) {
        asynPrint(this->myAsynUser, ASYN_TRACE_ERROR, "%s S7nodaveInputRecord::getIoIntInfoRecord Could not find poll group \"%s\".\n", this->record->name, this->pollGroupName.c_str());
        return RECORD_STATUS_ERROR;
    }
    if (command == 0) {
        // Add to poll group
        pollGroup->registerRequester(&this->pollRequester);
    } else {
        // Remove from poll group
        pollGroup->unregisterRequester(&this->pollRequester);
    }
    *iopvt = this->ioScanPvt;
    return RECORD_STATUS_OK;
}

void S7nodaveInputRecord::extractDeviceParameters(S7nodaveRecordAddress::DeviceParameters& deviceParameters) {
    // Let super classes extract their parameters
    S7nodaveRecord::extractDeviceParameters(deviceParameters);

    // Extract poll-group name (PG parameter)
    auto pg = deviceParameters.find("PG");
    if (pg != deviceParameters.end()) {
        Optional<std::string> value = pg->second;
        if (value) {
            this->pollGroupName = *value;
            deviceParameters.erase(pg);
        } else {
            asynPrint(this->myAsynUser, ASYN_TRACE_ERROR, "%s S7nodaveRecord::extractDeviceParameters Parameter\"PG\" must specify value.\n", record->name);
        }
    }
}

void S7nodaveInputRecord::preparePollRequest(PollService& pollService) {
    PlcAddress plcAddress = this->recordAddress->getPlcAddress();
    s7nodavePlcDataType plcDataType = this->recordAddress->getPlcDataType();
    unsigned long bufferSizeInBits = getIoBufferSizeInBits();
    unsigned int bufferSizeInBytes = bufferSizeInBits / 8 + ((bufferSizeInBits % 8 == 0) ? 0 : 1);
    // Bools do not have to start on byte boundaries.
    if (plcDataType == plcDataTypeBool && plcAddress.getStartBit() != 0) {
        // We have to use a different buffer for reading
        bufferSizeInBytes = (bufferSizeInBits + plcAddress.getStartBit()) / 8;
        if ((bufferSizeInBits + plcAddress.getStartBit()) % 8 != 0) {
            bufferSizeInBytes += 1;
        }
    }
    void *buffer = pasynManager->memMalloc(bufferSizeInBytes);
    if (!buffer) {
        asynPrint(this->myAsynUser, ASYN_TRACE_ERROR, "%s S7nodaveRecord::preparePollRequest Allocation of %d bytes of memory failed.\n", record->name, bufferSizeInBytes);
        return;
    }
    pollService.requestRead(plcAddress, bufferSizeInBytes, buffer);
}

void S7nodaveInputRecord::processPollResponse(bool success, int bufferSize, void *buffer) {
    PlcAddress plcAddress = this->recordAddress->getPlcAddress();
    s7nodavePlcDataType plcDataType = this->recordAddress->getPlcDataType();
    unsigned long bufferSizeInBits = getIoBufferSizeInBits();
    if (plcDataType == plcDataTypeBool && plcAddress.getStartBit() != 0) {
        // We have to rearrange the bits in order to start with the first
        // requested bit.
        if (success) {
            int buffer2Size = bufferSizeInBits / 8 + ((bufferSizeInBits % 8 == 0) ? 0 : 1);
            void *buffer2 = pasynManager->memMalloc(buffer2Size);
            if (!buffer) {
                asynPrint(this->myAsynUser, ASYN_TRACE_ERROR, "%s S7nodaveRecord::processPollResponse Allocation of %d bytes of memory failed.\n", record->name, buffer2Size);
                success = false;
            }
            if (success) {
                // Make buffer sizes are as expected
                assert(bufferSize >= buffer2Size);
                int startBit = plcAddress.getStartBit();
                unsigned char *bufferAsChar = static_cast<unsigned char *>(buffer);
                unsigned char *buffer2AsChar = static_cast<unsigned char *>(buffer2);
                for (int pos = 0; pos < buffer2Size; pos++) {
                    int b = bufferAsChar[pos] >> startBit;
                    b &= bitMasks[8-startBit];
                    if (pos + 1 < bufferSize) {
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
                // Replace buffer with buffer2
                pasynManager->memFree(buffer, bufferSize);
                buffer = buffer2;
                bufferSize = buffer2Size;
            }
        }
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

    if (!success) {
        // If the read process was not successful, we do not have to
        // preserve the buffer, so we can free it now instead of doing this
        // during record processing.
        pasynManager->memFree(buffer, bufferSize);
        buffer = nullptr;
        bufferSize = 0;
    }
    // We have to lock the mutex in order to synchronize with the get data
    // process in processRecord().
    {
        epicsGuard<epicsMutex> guard(this->newDataMutex);
        // Check that newData is empty, otherwise we have to free it in order
        // to avoid a memory leak.
        if (this->newData) {
            pasynManager->memFree(this->newData, this->newDataLength);
        }
        this->newData = buffer;
        this->newDataLength = bufferSize;
        this->processCallbackError = !success;
    }
    // Queue a request for processRecord to be called.
    scanIoRequest(this->ioScanPvt);
}

} // namespace s7nodave
