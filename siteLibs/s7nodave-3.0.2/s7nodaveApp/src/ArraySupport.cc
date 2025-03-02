#include <cstring>
#include <string>

#include <alarm.h>
#include <epicsTypes.h>
#include <dbFldTypes.h>
#include <recGbl.h>

#include "s7nodave.h"

#include "ArraySupport.h"
#include "utilities.h"

namespace s7nodave {

namespace {

void checkBufferSize(int bufferSize, epicsUInt32 numElements, s7nodavePlcDataType dataType) {
    // Buffer size should always be positive
    assert(bufferSize >= 0);
    unsigned int uBufferSize = static_cast<int>(bufferSize);
    switch (dataType) {
    case plcDataTypeBool:
        assert(uBufferSize >= numElements / 8 + ((numElements % 8 == 0) ? 0 : 1));
        break;
    case plcDataTypeInt8:
    case plcDataTypeUint8:
        assert(uBufferSize >= numElements);
        break;
    case plcDataTypeInt16:
    case plcDataTypeUint16:
        assert(uBufferSize >= numElements * 2);
        break;
    case plcDataTypeInt32:
    case plcDataTypeUint32:
    case plcDataTypeFloat:
        assert(uBufferSize >= numElements * 4);
        break;
    }
}

/*
 * We use a function template with specializations in order to get the format
 * string for a numeric data type.
 */
template<class numberType>
inline const char* formatStringForType();

template<> inline const char* formatStringForType<epicsInt8>() { return "%hhd"; }
template<> inline const char* formatStringForType<epicsUInt8>() { return "%hhu"; }
template<> inline const char* formatStringForType<epicsInt16>() { return "%hd"; }
template<> inline const char* formatStringForType<epicsUInt16>() { return "%hu"; }
template<> inline const char* formatStringForType<epicsInt32>() { return "%d"; }
template<> inline const char* formatStringForType<epicsUInt32>() { return "%u"; }
template<> inline const char* formatStringForType<epicsFloat32>() { return "%f"; }
template<> inline const char* formatStringForType<epicsFloat64>() { return "%f"; }

/*
 * Converts a number to a string representation using snprintf.
 */
template<class numberType>
inline std::string numberToString(numberType number) {
    char buffer[100];
    snprintf(buffer, 100, formatStringForType<numberType>(), number);
    return std::string(buffer);
}

/*
 * Converts an array of numbers to a string representation.
 */
template<class numberType>
std::string numberArrayToString(const numberType *numberArray, size_t numElements, const char *separator) {
    std::string result;
    for (size_t i = 0; i < numElements; i++) {
        if (i != 0) {
            result.append(separator);
        }
        result.append(numberToString<numberType>(numberArray[i]));
    }
    return result;
}

/*
 * Converts an array of numbers to a string representation. Accepts a void
 * pointer buffer, that is automatically casted to the specified type.
 */
template<class numberType>
inline std::string numberArrayToString(const void *numberArray, size_t numElements, const char *separator) {
    const numberType *numberTypedArray = static_cast<const numberType *>(numberArray);
    return numberArrayToString(numberTypedArray, numElements, separator);
}


/*
 * Converts an array of numbers of one type to an array of numbers of a
 * different type.
 */
template <class sourceType, class targetType>
void convertBuffer(const void *sourceBuffer, void *targetBuffer, epicsUInt32 numElements) {
    const sourceType *source = static_cast<const sourceType *>(sourceBuffer);
    targetType *target = static_cast<targetType *>(targetBuffer);

    for (epicsUInt32 i = 0; i < numElements; i++) {
        target[i] = source[i];
    }
}

/*
 * Checks whether the ASYN_TRACEIO_DEVICE bit is set in the I/O trace mask.
 */
inline bool shouldTraceIO(asynUser *pasynUser) {
    return ((pasynTrace->getTraceMask(pasynUser) & ASYN_TRACEIO_DEVICE) != 0);
}

template <class numberType>
inline void asynPrintConvertBufferToRecord(asynUser *pasynUser, dbCommon *record, void *numberArray, epicsUInt32 numElements) {
    // Building the log line is quite expensive. Therefore we first check whether
    // the
    if (shouldTraceIO(pasynUser)) {
        asynPrint(pasynUser, ASYN_TRACEIO_DEVICE, "%s ArraySupport::convertBufferToRecord read array with %u elements: [%s]\n", record->name, numElements, numberArrayToString<numberType>(numberArray, numElements, ", ").c_str());
    }
}

/*
 * Concatenates an array of strings to a single string, separated by commas and
 * spaces.
 */
 std::string concatenateStringArray(const void *stringArray, epicsUInt32 numElements) {
    std::string result;
    const char *stringTypedArray = static_cast<const char *>(stringArray);
    for (epicsUInt32 i = 0; i < numElements; i++) {
        if (i != 0) {
            result.append(", ");
        }
        result.append("\"");
        result.append(stringTypedArray + i * MAX_STRING_SIZE);
        result.append(stringTypedArray + i * MAX_STRING_SIZE);
        result.append("\"");
    }
    return result;
}

/*
 * Writes the content of a buffer to a record (not for boolean buffers).
 */
template <class sourceType>
void convertBufferToRecord(asynUser *pasynUser, dbCommon *record, const void *sourceBuffer, void *targetBuffer, epicsUInt32 numElements, epicsEnum16 targetType) {
    switch (targetType) {
    case DBF_STRING:
    {
        // For a string we can just copy the whole buffer
        memcpy(targetBuffer, sourceBuffer, numElements * MAX_STRING_SIZE);
        // Make sure string is null-terminated.
        char *charBuffer = static_cast<char *>(targetBuffer);
        for (epicsUInt32 i = 0; i < numElements; i++) {
            charBuffer[(i + 1) * MAX_STRING_SIZE - 1] = 0;
        }
        if (shouldTraceIO(pasynUser)) {
            asynPrint(pasynUser, ASYN_TRACEIO_DEVICE, "%s ArraySupport::convertBufferToRecord read array with %u elements: [%s]\n", record->name, numElements, concatenateStringArray(targetBuffer, numElements).c_str());
        }
        break;
    }
    case DBF_CHAR:
        convertBuffer<sourceType, epicsInt8>(sourceBuffer, targetBuffer, numElements);
        asynPrintConvertBufferToRecord<epicsInt8>(pasynUser, record, targetBuffer, numElements);
        break;
    case DBF_UCHAR:
        convertBuffer<sourceType, epicsUInt8>(sourceBuffer, targetBuffer, numElements);
        asynPrintConvertBufferToRecord<epicsUInt8>(pasynUser, record, targetBuffer, numElements);
        break;
    case DBF_SHORT:
        convertBuffer<sourceType, epicsInt16>(sourceBuffer, targetBuffer, numElements);
        asynPrintConvertBufferToRecord<epicsInt16>(pasynUser, record, targetBuffer, numElements);
        break;
    case DBF_USHORT:
    case DBF_ENUM:
        convertBuffer<sourceType, epicsUInt16>(sourceBuffer, targetBuffer, numElements);
        asynPrintConvertBufferToRecord<epicsUInt16>(pasynUser, record, targetBuffer, numElements);
        break;
    case DBF_LONG:
        convertBuffer<sourceType, epicsInt32>(sourceBuffer, targetBuffer, numElements);
        asynPrintConvertBufferToRecord<epicsInt32>(pasynUser, record, targetBuffer, numElements);
        break;
    case DBF_ULONG:
        convertBuffer<sourceType, epicsUInt32>(sourceBuffer, targetBuffer, numElements);
        asynPrintConvertBufferToRecord<epicsUInt32>(pasynUser, record, targetBuffer, numElements);
        break;
    case DBF_FLOAT:
        convertBuffer<sourceType, epicsFloat32>(sourceBuffer, targetBuffer, numElements);
        asynPrintConvertBufferToRecord<epicsFloat32>(pasynUser, record, targetBuffer, numElements);
        break;
    case DBF_DOUBLE:
        convertBuffer<sourceType, epicsFloat64>(sourceBuffer, targetBuffer, numElements);
        asynPrintConvertBufferToRecord<epicsFloat64>(pasynUser, record, targetBuffer, numElements);
        break;
    }
}

/**
 * Writes the content of a boolean buffer to a record.
 */
template <class targetType>
void convertBoolBufferToRecord(asynUser *pasynUser, dbCommon *record, const void *sourceBuffer, void *targetBuffer, epicsUInt32 numElements, epicsEnum16 targetTypeCode) {
    const unsigned char *source = static_cast<const unsigned char *>(sourceBuffer);
    targetType *target = static_cast<targetType *>(targetBuffer);
    // Check whether IO tracing is enabled
    bool traceIO = shouldTraceIO(pasynUser);
    std::string arrayString;
    for (epicsUInt32 i = 0; i < numElements; i++) {
        unsigned char bitVal = source[i / 8] & (0x01 << (i % 8));
        if (targetTypeCode == DBF_STRING) {
            char *charTarget = static_cast<char *>(targetBuffer);
            const char *boolString;
            const char *trueString = "TRUE";
            const char *falseString = "FALSE";
            if (bitVal != 0) {
                boolString = trueString;
            } else {
                boolString = falseString;
            }
            strncpy(charTarget, boolString, MAX_STRING_SIZE);
            if (traceIO) {
                if (i != 0) {
                    arrayString.append(", ");
                }
                arrayString.append("\"");
                arrayString.append(boolString);
                arrayString.append("\"");
            }
        } else {
            if (bitVal != 0) {
                bitVal = 1;
            }
            target[i] = bitVal;
            if (traceIO) {
                arrayString.append((bitVal != 0) ? "1" : "0");
            }
        }
    }
    if (traceIO) {
        asynPrint(pasynUser, ASYN_TRACEIO_DEVICE, "%s ArraySupport::convertBoolBufferToRecord read array with %u elements: [%s]\n", record->name, numElements, arrayString.c_str());
    }
}

/*
 * Writes the content of a boolean buffer to a record (for all record element
 * types).
 */
void convertBoolBufferToRecord(asynUser *pasynUser, dbCommon *record, const void *sourceBuffer, void *targetBuffer, epicsUInt32 numElements, epicsEnum16 targetType) {
    switch (targetType) {
    case DBF_STRING:
        convertBoolBufferToRecord<char>(pasynUser, record, sourceBuffer, targetBuffer, numElements, targetType);
        break;
    case DBF_CHAR:
        convertBoolBufferToRecord<epicsInt8>(pasynUser, record, sourceBuffer, targetBuffer, numElements, targetType);
        break;
    case DBF_UCHAR:
        convertBoolBufferToRecord<epicsUInt8>(pasynUser, record, sourceBuffer, targetBuffer, numElements, targetType);
        break;
    case DBF_SHORT:
        convertBoolBufferToRecord<epicsInt16>(pasynUser, record, sourceBuffer, targetBuffer, numElements, targetType);
        break;
    case DBF_USHORT:
    case DBF_ENUM:
        convertBoolBufferToRecord<epicsUInt16>(pasynUser, record, sourceBuffer, targetBuffer, numElements, targetType);
        break;
    case DBF_LONG:
        convertBoolBufferToRecord<epicsInt32>(pasynUser, record, sourceBuffer, targetBuffer, numElements, targetType);
        break;
    case DBF_ULONG:
        convertBoolBufferToRecord<epicsUInt32>(pasynUser, record, sourceBuffer, targetBuffer, numElements, targetType);
        break;
    case DBF_FLOAT:
        convertBoolBufferToRecord<epicsFloat32>(pasynUser, record, sourceBuffer, targetBuffer, numElements, targetType);
        break;
    case DBF_DOUBLE:
        convertBoolBufferToRecord<epicsFloat64>(pasynUser, record, sourceBuffer, targetBuffer, numElements, targetType);
        break;
    }
}

} // anonymous namesapce

long ArraySupport::readInternal(asynUser *pasynUser, dbCommon *record, void *bptr, epicsUInt32 nelm, epicsEnum16 ftvl, epicsUInt32& nord, const void *buffer, int bufferSize, s7nodavePlcDataType plcDataType) {
    // Make extra sure that buffer has correct size. record->nelm should not
    // have changed, but who knows...
    checkBufferSize(bufferSize, nelm, plcDataType);
    // Set number of elements read to number of elements. We will change this
    // to zero on error.
    nord = nelm;
    // Check whether extended logging is enabled.
    switch (plcDataType) {
    case plcDataTypeBool:
        convertBoolBufferToRecord(pasynUser, record, buffer, bptr, nelm, ftvl);
        break;
    case plcDataTypeInt8:
        convertBufferToRecord<epicsInt8>(pasynUser, record, buffer, bptr, nelm, ftvl);
        break;
    case plcDataTypeUint8:
        convertBufferToRecord<epicsUInt8>(pasynUser, record, buffer, bptr, nelm, ftvl);
        break;
    case plcDataTypeInt16:
        convertBufferToRecord<epicsInt16>(pasynUser, record, buffer, bptr, nelm, ftvl);
        break;
    case plcDataTypeUint16:
        convertBufferToRecord<epicsUInt16>(pasynUser, record, buffer, bptr, nelm, ftvl);
        break;
    case plcDataTypeInt32:
        convertBufferToRecord<epicsInt32>(pasynUser, record, buffer, bptr, nelm, ftvl);
        break;
    case plcDataTypeUint32:
        convertBufferToRecord<epicsUInt32>(pasynUser, record, buffer, bptr, nelm, ftvl);
        break;
    case plcDataTypeFloat:
        convertBufferToRecord<epicsFloat32>(pasynUser, record, buffer, bptr, nelm, ftvl);
        break;
    default:
        // Data type is not supported (should never happen).
        recGblSetSevr(record, READ_ALARM, INVALID_ALARM);
        nord = 0;
        break;
    }
    return RECORD_STATUS_OK;
}

namespace {

template <class numberType>
inline void asynPrintConvertRecordToBuffer(asynUser *pasynUser, dbCommon *record, void *numberArray, epicsUInt32 numElements) {
    // Building the log line is quite expensive. Therefore we first check
    // whether IO tracing is enabled.
    if (shouldTraceIO(pasynUser)) {
        asynPrint(pasynUser, ASYN_TRACEIO_DEVICE, "%s ArraySupport::convertRecordToBuffer writing array with %u elements: [%s]\n", record->name, numElements, numberArrayToString<numberType>(numberArray, numElements, ", ").c_str());
    }
}

/**
 * Writes the content of a record to a buffer (not for boolean buffers).
 */
template <class targetType>
void convertRecordToBuffer(asynUser *pasynUser, dbCommon *record, const void *sourceBuffer, void *targetBuffer, epicsUInt32 numElements, epicsEnum16 sourceType) {
    switch (sourceType) {
    case DBF_STRING:
    {
        // For a string we can just copy the whole buffer
        memcpy(targetBuffer, sourceBuffer, numElements * MAX_STRING_SIZE);
        // Make sure strings are null-terminated
        char *charBuffer = static_cast<char *>(targetBuffer);
        for (epicsUInt32 i = 0; i < numElements; i++) {
            charBuffer[(i + 1) * MAX_STRING_SIZE - 1] = 0;
        }
        if (shouldTraceIO(pasynUser)) {
            asynPrint(pasynUser, ASYN_TRACEIO_DEVICE, "%s ArraySupport::convertBufferToRecord read array with %u elements: [%s]\n", record->name, numElements, concatenateStringArray(targetBuffer, numElements).c_str());
        }
        return;
    }
    case DBF_CHAR:
        convertBuffer<epicsInt8, targetType>(sourceBuffer, targetBuffer, numElements);
        break;
    case DBF_UCHAR:
        convertBuffer<epicsUInt8, targetType>(sourceBuffer, targetBuffer, numElements);
        break;
    case DBF_SHORT:
        convertBuffer<epicsInt16, targetType>(sourceBuffer, targetBuffer, numElements);
        break;
    case DBF_USHORT:
    case DBF_ENUM:
        convertBuffer<epicsUInt16, targetType>(sourceBuffer, targetBuffer, numElements);
        break;
    case DBF_LONG:
        convertBuffer<epicsInt32, targetType>(sourceBuffer, targetBuffer, numElements);
        break;
    case DBF_ULONG:
        convertBuffer<epicsUInt32, targetType>(sourceBuffer, targetBuffer, numElements);
        break;
    case DBF_FLOAT:
        convertBuffer<epicsFloat32, targetType>(sourceBuffer, targetBuffer, numElements);
        break;
    case DBF_DOUBLE:
        convertBuffer<epicsFloat64, targetType>(sourceBuffer, targetBuffer, numElements);
        break;
    }
    asynPrintConvertRecordToBuffer<targetType>(pasynUser, record, targetBuffer, numElements);
}

/**
 * Writes the content of a record to a boolean buffer.
 */
template <class sourceType>
void convertRecordToBoolBuffer(asynUser *pasynUser, dbCommon *record, const void *sourceBuffer, void *targetBuffer, epicsUInt32 numElements, epicsEnum16 sourceTypeCode) {
    unsigned char *target = static_cast<unsigned char *>(targetBuffer);
    const sourceType *source = static_cast<const sourceType *>(sourceBuffer);
    // Check whether IO tracing is enabled
    bool traceIO = shouldTraceIO(pasynUser);
    std::string arrayString;
    for (epicsUInt32 i = 0; i < numElements; i++) {
        unsigned char bitVal;
        if (sourceTypeCode == DBF_STRING) {
            const char *charSource = static_cast<const char *>(sourceBuffer) + i * MAX_STRING_SIZE;
            std::string boolString = charSource;
            trim(boolString);
            toUpper(boolString);
            if (boolString == "1" || boolString == "TRUE") {
                bitVal = 1;
            } else {
                bitVal = 0;
            }
        } else {
            if (source[i] != 0) {
                bitVal = 1;
            } else {
                bitVal = 0;
            }
        }
        if (i % 8 == 0) {
            // First bit in byte
            target[i / 8] = bitVal;
        } else {
            target[i / 8] |= bitVal << (i % 8);
        }
        if (traceIO) {
            arrayString.append((bitVal != 0) ? "1" : "0");
        }
    }
    if (traceIO) {
        asynPrint(pasynUser, ASYN_TRACEIO_DEVICE, "%s ArraySupport::convertRecordToBoolBuffer writing array with %u elements: [%s]\n", record->name, numElements, arrayString.c_str());
    }
}

/**
 * Writes the content of a record to a boolean buffer (for all record element
 * types).
 */
void convertRecordToBoolBuffer(asynUser *pasynUser, dbCommon *record, const void *sourceBuffer, void *targetBuffer, epicsUInt32 numElements, epicsEnum16 targetType) {
    switch (targetType) {
    case DBF_STRING:
        convertRecordToBoolBuffer<char>(pasynUser, record, sourceBuffer, targetBuffer, numElements, targetType);
        break;
    case DBF_CHAR:
        convertRecordToBoolBuffer<epicsInt8>(pasynUser, record, sourceBuffer, targetBuffer, numElements, targetType);
        break;
    case DBF_UCHAR:
        convertRecordToBoolBuffer<epicsUInt8>(pasynUser, record, sourceBuffer, targetBuffer, numElements, targetType);
        break;
    case DBF_SHORT:
        convertRecordToBoolBuffer<epicsInt16>(pasynUser, record, sourceBuffer, targetBuffer, numElements, targetType);
        break;
    case DBF_USHORT:
    case DBF_ENUM:
        convertRecordToBoolBuffer<epicsUInt16>(pasynUser, record, sourceBuffer, targetBuffer, numElements, targetType);
        break;
    case DBF_LONG:
        convertRecordToBoolBuffer<epicsInt32>(pasynUser, record, sourceBuffer, targetBuffer, numElements, targetType);
        break;
    case DBF_ULONG:
        convertRecordToBoolBuffer<epicsUInt32>(pasynUser, record, sourceBuffer, targetBuffer, numElements, targetType);
        break;
    case DBF_FLOAT:
        convertRecordToBoolBuffer<epicsFloat32>(pasynUser, record, sourceBuffer, targetBuffer, numElements, targetType);
        break;
    case DBF_DOUBLE:
        convertRecordToBoolBuffer<epicsFloat64>(pasynUser, record, sourceBuffer, targetBuffer, numElements, targetType);
        break;
    }
}

} // anonymous namesapce

void ArraySupport::writeInternal(asynUser *pasynUser, dbCommon *record, void *bptr, epicsUInt32 nelm, epicsEnum16 ftvl, epicsUInt32& nord, void* buffer, int bufferSize, s7nodavePlcDataType plcDataType) {
    // Make extra sure that buffer has correct size. record->nelm should not
    // have changed, but who knows...
    checkBufferSize(bufferSize, nelm, plcDataType);
    switch (plcDataType) {
    case plcDataTypeBool:
        convertRecordToBoolBuffer(pasynUser, record, bptr, buffer, nelm, ftvl);
        break;
    case plcDataTypeInt8:
        convertRecordToBuffer<epicsInt8>(pasynUser, record, bptr, buffer, nelm, ftvl);
        break;
    case plcDataTypeUint8:
        convertRecordToBuffer<epicsUInt8>(pasynUser, record, bptr, buffer, nelm, ftvl);
        break;
    case plcDataTypeInt16:
        convertRecordToBuffer<epicsInt16>(pasynUser, record, bptr, buffer, nelm, ftvl);
        break;
    case plcDataTypeUint16:
        convertRecordToBuffer<epicsUInt16>(pasynUser, record, bptr, buffer, nelm, ftvl);
        break;
    case plcDataTypeInt32:
        convertRecordToBuffer<epicsInt32>(pasynUser, record, bptr, buffer, nelm, ftvl);
        break;
    case plcDataTypeUint32:
        convertRecordToBuffer<epicsUInt32>(pasynUser, record, bptr, buffer, nelm, ftvl);
        break;
    case plcDataTypeFloat:
        convertRecordToBuffer<epicsFloat32>(pasynUser, record, bptr, buffer, nelm, ftvl);
        break;
    default:
        // Data type is not supported (should never happen).
        asynPrint(pasynUser, ASYN_TRACE_ERROR, "%s ArraySupport::writeInternal Got unexpected PLC data type.\n", record->name);
        recGblSetSevr(record, READ_ALARM, INVALID_ALARM);
        nord = 0;
        return;
    }
    nord = nelm;
}

Optional<s7nodavePlcDataType> ArraySupport::getPlcDataTypeIn(PlcAddress plcAddress, Optional<s7nodavePlcDataType> suggestion, epicsEnum16 ftvl) {
    Optional<s7nodavePlcDataType> emptyResult;
    if (suggestion) {
        switch(ftvl) {
        case DBF_STRING:
            // For a string we read a sequence of 40 bytes each, thus
            // data types which are not bytes do not make sense.
            switch (*suggestion) {
            case plcDataTypeInt8:
            case plcDataTypeUint8:
                return suggestion;
            default:
                break;
            }
            break;
        case DBF_CHAR:
            switch (*suggestion) {
            case plcDataTypeBool:
            case plcDataTypeInt8:
                return suggestion;
            default:
                break;
            }
            break;
        case DBF_UCHAR:
            switch (*suggestion) {
            case plcDataTypeBool:
            case plcDataTypeUint8:
                return suggestion;
            default:
                break;
            }
            break;
        case DBF_SHORT:
            switch (*suggestion) {
            case plcDataTypeBool:
            case plcDataTypeInt8:
            case plcDataTypeUint8:
            case plcDataTypeInt16:
                return suggestion;
            default:
                break;
            }
            break;
        case DBF_USHORT:
        case DBF_ENUM:
            // An enum is also represented by a uint16.
            switch (*suggestion) {
            case plcDataTypeBool:
            case plcDataTypeUint8:
            case plcDataTypeUint16:
                return suggestion;
            default:
                break;
            }
            break;
        case DBF_LONG:
            switch (*suggestion) {
            case plcDataTypeBool:
            case plcDataTypeInt8:
            case plcDataTypeUint8:
            case plcDataTypeInt16:
            case plcDataTypeUint16:
            case plcDataTypeInt32:
                return suggestion;
            default:
                break;
            }
            break;
        case DBF_ULONG:
            switch (*suggestion) {
            case plcDataTypeBool:
            case plcDataTypeUint8:
            case plcDataTypeUint16:
            case plcDataTypeUint32:
                return suggestion;
            default:
                break;
            }
            break;
        case DBF_FLOAT:
        case DBF_DOUBLE:
            // Everything can be converted to a float or double.
            return suggestion;
        }
        return emptyResult;
    } else {
        // We ignore the default and decide based on the EPICS data-type
        // and PLC data size.
        s7nodavePlcDataSize dataSize = plcAddress.getDataSize();
        switch (ftvl) {
        case DBF_STRING:
            if (dataSize == plcDataSizeByte) {
                return plcDataTypeInt8;
            }
            break;
        case DBF_CHAR:
            switch (dataSize) {
            case plcDataSizeBit:
                return plcDataTypeBool;
            case plcDataSizeByte:
                return plcDataTypeInt8;
            default:
                break;
            }
            break;
        case DBF_UCHAR:
            switch (dataSize) {
            case plcDataSizeBit:
                return plcDataTypeBool;
            case plcDataSizeByte:
                return plcDataTypeUint8;
            default:
                break;
            }
            break;
        case DBF_SHORT:
            switch (dataSize) {
            case plcDataSizeBit:
                return plcDataTypeBool;
            case plcDataSizeByte:
                return plcDataTypeInt8;
            case plcDataSizeWord:
                return plcDataTypeInt16;
            default:
                break;
            }
            break;
        case DBF_USHORT:
        case DBF_ENUM:
            // enum is an unsigned short
            switch (dataSize) {
            case plcDataSizeBit:
                return plcDataTypeBool;
            case plcDataSizeByte:
                return plcDataTypeUint8;
            case plcDataSizeWord:
                return plcDataTypeUint16;
            default:
                break;
            }
            break;
        case DBF_LONG:
            switch (dataSize) {
            case plcDataSizeBit:
                return plcDataTypeBool;
            case plcDataSizeByte:
                return plcDataTypeInt8;
            case plcDataSizeWord:
                return plcDataTypeInt16;
            case plcDataSizeDword:
                return plcDataTypeInt32;
            default:
                break;
            }
            break;
        case DBF_ULONG:
            switch (dataSize) {
            case plcDataSizeBit:
                return plcDataTypeBool;
            case plcDataSizeByte:
                return plcDataTypeUint8;
            case plcDataSizeWord:
                return plcDataTypeUint16;
            case plcDataSizeDword:
                return plcDataTypeUint32;
            default:
                break;
            }
            break;
        case DBF_FLOAT:
        case DBF_DOUBLE:
            switch (dataSize) {
            case plcDataSizeBit:
                return plcDataTypeBool;
            case plcDataSizeByte:
                return plcDataTypeUint8;
            case plcDataSizeWord:
                return plcDataTypeUint16;
            case plcDataSizeDword:
                return plcDataTypeFloat;
            default:
                break;
            }
            break;
        }
        return emptyResult;
    }
}

Optional<s7nodavePlcDataType> ArraySupport::getPlcDataTypeOut(PlcAddress plcAddress, Optional<s7nodavePlcDataType> suggestion, epicsEnum16 ftvl) {
    Optional<s7nodavePlcDataType> emptyResult;
    if (suggestion) {
        switch(ftvl) {
        case DBF_STRING:
            // For a string we write a sequence of 40 bytes each, thus
            // data types which are not bytes do not make sense.
            switch (*suggestion) {
            case plcDataTypeInt8:
            case plcDataTypeUint8:
                return suggestion;
            default:
                break;
            }
            break;
        case DBF_CHAR:
            switch (*suggestion) {
            case plcDataTypeBool:
            case plcDataTypeInt8:
            case plcDataTypeInt16:
            case plcDataTypeInt32:
            case plcDataTypeFloat:
                return suggestion;
            default:
                break;
            }
            break;
        case DBF_UCHAR:
            switch (*suggestion) {
            case plcDataTypeBool:
            case plcDataTypeUint8:
            case plcDataTypeInt16:
            case plcDataTypeUint16:
            case plcDataTypeInt32:
            case plcDataTypeUint32:
            case plcDataTypeFloat:
                return suggestion;
            default:
                break;
            }
            break;
        case DBF_SHORT:
            switch (*suggestion) {
            case plcDataTypeBool:
            case plcDataTypeInt16:
            case plcDataTypeInt32:
            case plcDataTypeFloat:
                return suggestion;
            default:
                break;
            }
            break;
        case DBF_USHORT:
        case DBF_ENUM:
            // An enum is also represented by a uint16.
            switch (*suggestion) {
            case plcDataTypeBool:
            case plcDataTypeUint16:
            case plcDataTypeInt32:
            case plcDataTypeUint32:
            case plcDataTypeFloat:
                return suggestion;
            default:
                break;
            }
            break;
        case DBF_LONG:
            switch (*suggestion) {
            case plcDataTypeBool:
            case plcDataTypeInt32:
            case plcDataTypeFloat:
                return suggestion;
            default:
                break;
            }
            break;
        case DBF_ULONG:
            switch (*suggestion) {
            case plcDataTypeBool:
            case plcDataTypeUint32:
            case plcDataTypeFloat:
                return suggestion;
            default:
                break;
            }
            break;
        case DBF_FLOAT:
        case DBF_DOUBLE:
            if (*suggestion == plcDataTypeFloat) {
                return suggestion;
            }
            break;
        }
        return emptyResult;
    } else {
        // We ignore the default and decide based on the EPICS data-type
        // and PLC data size.
        s7nodavePlcDataSize dataSize = plcAddress.getDataSize();
        switch (ftvl) {
        case DBF_STRING:
            if (dataSize == plcDataSizeByte) {
                return plcDataTypeInt8;
            }
            break;
        case DBF_CHAR:
            switch (dataSize) {
            case plcDataSizeBit:
                return plcDataTypeBool;
            case plcDataSizeByte:
                return plcDataTypeInt8;
            case plcDataSizeWord:
                return plcDataTypeInt16;
            case plcDataSizeDword:
                return plcDataTypeInt32;
            }
            break;
        case DBF_UCHAR:
            switch (dataSize) {
            case plcDataSizeBit:
                return plcDataTypeBool;
            case plcDataSizeByte:
                return plcDataTypeUint8;
            case plcDataSizeWord:
                return plcDataTypeUint16;
            case plcDataSizeDword:
                return plcDataTypeUint32;
            }
            break;
        case DBF_SHORT:
            switch (dataSize) {
            case plcDataSizeBit:
                return plcDataTypeBool;
            case plcDataSizeWord:
                return plcDataTypeInt16;
            case plcDataSizeDword:
                return plcDataTypeInt32;
            default:
                break;
            }
            break;
        case DBF_USHORT:
        case DBF_ENUM:
            // enum is an unsigned short
            switch (dataSize) {
            case plcDataSizeBit:
                return plcDataTypeBool;
            case plcDataSizeWord:
                return plcDataTypeUint16;
            case plcDataSizeDword:
                return plcDataTypeUint32;
            default:
                break;
            }
            break;
        case DBF_LONG:
            switch (dataSize) {
            case plcDataSizeBit:
                return plcDataTypeBool;
            case plcDataSizeDword:
                return plcDataTypeInt32;
            default:
                break;
            }
            break;
        case DBF_ULONG:
            switch (dataSize) {
            case plcDataSizeBit:
                return plcDataTypeBool;
            case plcDataSizeDword:
                return plcDataTypeUint32;
            default:
                break;
            }
            break;
        case DBF_FLOAT:
        case DBF_DOUBLE:
            switch (dataSize) {
            case plcDataSizeBit:
                return plcDataTypeBool;
            case plcDataSizeDword:
                return plcDataTypeFloat;
            default:
                break;
            }
            break;
        }
        return emptyResult;
    }
}

} // namespace s7nodave
