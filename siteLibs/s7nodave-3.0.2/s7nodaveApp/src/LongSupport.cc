#include <cassert>

#include <alarm.h>
#include <epicsTypes.h>
#include <longinRecord.h>
#include <longoutRecord.h>
#include <recGbl.h>

#include "LongSupport.h"

namespace s7nodave {

void LongSupport::readInternal(asynUser *pasynUser, dbCommon *record, epicsInt32& val, const void *buffer, int bufferSize, s7nodavePlcDataType plcDataType) {
    switch (plcDataType) {
    case plcDataTypeBool:
        assert(bufferSize >= 1);
        val = *(static_cast<const unsigned char *>(buffer));
        break;
    case plcDataTypeInt8:
        assert(bufferSize >= 1);
        val = *(static_cast<const epicsInt8 *>(buffer));
        break;
    case plcDataTypeUint8:
        assert(bufferSize >= 1);
        val = *(static_cast<const epicsUInt8 *>(buffer));
        break;
    case plcDataTypeInt16:
        assert(bufferSize >= 2);
        val = *(static_cast<const epicsInt16 *>(buffer));
        break;
    case plcDataTypeUint16:
        assert(bufferSize >= 2);
        val = *(static_cast<const epicsUInt16 *>(buffer));
        break;
    case plcDataTypeInt32:
        assert(bufferSize >= 4);
        val = *(static_cast<const epicsInt32 *>(buffer));
        break;
    default:
        // Data type (float, uint32) is not supported
        recGblSetSevr(record, READ_ALARM, INVALID_ALARM);
        asynPrint(pasynUser, ASYN_TRACE_ERROR, "%s LongSupport::readInternal Got unexpected PLC data-type.\n", record->name);
        return;
    }
    asynPrint(pasynUser, ASYN_TRACEIO_DEVICE, "%s LongSupport::readInternal read long %d\n", record->name, val);
}

void LongSupport::writeInternal(asynUser *pasynUser, dbCommon *record, epicsInt32 recordVal, void* buffer, int bufferSize, s7nodavePlcDataType plcDataType) {
    switch (plcDataType) {
    case plcDataTypeBool:
    {
        assert(bufferSize >= 1);
        unsigned char *val = static_cast<unsigned char *>(buffer);
        *val = (recordVal != 0) ? 1 : 0;
        asynPrint(pasynUser, ASYN_TRACEIO_DEVICE, "%s LongSupport::writeInternal writing bit %hhu\n", record->name, *val);
        break;
    }
    case plcDataTypeInt8:
    {
        assert(bufferSize >= 1);
        epicsInt8 *val = static_cast<epicsInt8 *>(buffer);
        *val = recordVal;
        asynPrint(pasynUser, ASYN_TRACEIO_DEVICE, "%s LongSupport::writeInternal writing bit %hhd\n", record->name, *val);
        break;
    }
    case plcDataTypeUint8:
    {
        assert(bufferSize >= 1);
        epicsUInt8 *val = static_cast<epicsUInt8 *>(buffer);
        *val = recordVal;
        asynPrint(pasynUser, ASYN_TRACEIO_DEVICE, "%s LongSupport::writeInternal writing bit %hhu\n", record->name, *val);
        break;
    }
    case plcDataTypeInt16:
    {
        assert(bufferSize >= 2);
        epicsInt16 *val = static_cast<epicsInt16 *>(buffer);
        *val = recordVal;
        asynPrint(pasynUser, ASYN_TRACEIO_DEVICE, "%s LongSupport::writeInternal writing bit %hd\n", record->name, *val);
        break;
    }
    case plcDataTypeUint16:
    {
        assert(bufferSize >= 2);
        epicsUInt16 *val = static_cast<epicsUInt16 *>(buffer);
        *val = recordVal;
        asynPrint(pasynUser, ASYN_TRACEIO_DEVICE, "%s LongSupport::writeInternal writing bit %hu\n", record->name, *val);
        break;
    }
    case plcDataTypeInt32:
    {
        assert(bufferSize >= 4);
        epicsInt32 *val = static_cast<epicsInt32 *>(buffer);
        *val = recordVal;
        asynPrint(pasynUser, ASYN_TRACEIO_DEVICE, "%s LongSupport::writeInternal writing bit %d\n", record->name, *val);
        break;
    }
    default:
        // Data type (float, uint32) is not supported
        recGblSetSevr(record, WRITE_ALARM, INVALID_ALARM);
        asynPrint(pasynUser, ASYN_TRACE_ERROR, "%s LongSupport::readInternal Got unexpected PLC data-type.\n", record->name);
        break;
    }
}

Optional<s7nodavePlcDataType> LongSupport::getPlcDataType(PlcAddress, Optional<s7nodavePlcDataType> userRequestedType, Optional<s7nodavePlcDataType> defaultType) {
    bool userRequested;
    Optional<s7nodavePlcDataType> dataType;
    if (userRequestedType) {
        userRequested = true;
        dataType = userRequestedType;
    } else {
        userRequested = false;
        dataType = defaultType;
    }
    if (dataType && (*dataType == plcDataTypeUint32 || *dataType == plcDataTypeFloat)) {
        // These data types are not supported
        if (userRequested) {
            return Optional<s7nodavePlcDataType>();
        } else {
            // Suggest int32
            return plcDataTypeInt32;
        }
    } else {
        return dataType;
    }
}

} // namespace s7nodave
