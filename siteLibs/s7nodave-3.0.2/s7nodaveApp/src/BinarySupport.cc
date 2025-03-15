#include <cassert>
#include <limits>

#include <alarm.h>
#include <epicsTypes.h>
#include <biRecord.h>
#include <boRecord.h>
#include <recGbl.h>

#include "BinarySupport.h"

namespace s7nodave {

void BinarySupport::readInternal(asynUser *pasynUser, dbCommon *record, epicsUInt32& rval, const void *buffer, int bufferSize, s7nodavePlcDataType plcDataType) {
    switch (plcDataType) {
    case plcDataTypeBool:
    {
        assert(bufferSize >= 1);
        const unsigned char val = *(static_cast<const unsigned char *>(buffer));
        rval = (val != 0) ? 1 : 0;
        break;
    }
    case plcDataTypeInt8:
    {
        assert(bufferSize >= 1);
        const epicsInt8 val = *(static_cast<const epicsInt8 *>(buffer));
        rval = (val != 0) ? 1 : 0;
        break;
    }
    case plcDataTypeUint8:
    {
        assert(bufferSize >= 1);
        const epicsUInt8 val = *(static_cast<const epicsUInt8 *>(buffer));
        rval = (val != 0) ? 1 : 0;
        break;
    }
    case plcDataTypeInt16:
    {
        assert(bufferSize >= 2);
        const epicsInt16 val = *(static_cast<const epicsInt16 *>(buffer));
        rval = (val != 0) ? 1 : 0;
        break;
    }
    case plcDataTypeUint16:
    {
        assert(bufferSize >= 2);
        const epicsUInt16 val = *(static_cast<const epicsUInt16 *>(buffer));
        rval = (val != 0) ? 1 : 0;
        break;
    }
    case plcDataTypeInt32:
    {
        assert(bufferSize >= 4);
        const epicsInt32 val = *(static_cast<const epicsInt32 *>(buffer));
        rval = (val != 0) ? 1 : 0;
        break;
    }
    case plcDataTypeUint32:
    {
        assert(bufferSize >= 4);
        const epicsUInt32 val = *(static_cast<const epicsUInt32 *>(buffer));
        rval = (val != 0) ? 1 : 0;
        break;
    }
    case plcDataTypeFloat:
    {
        assert(bufferSize >= 4);
        const epicsFloat32 val = *(static_cast<const epicsFloat32 *>(buffer));
        std::numeric_limits<epicsFloat32> floatLimits;
        if (val != 0.0 && val != floatLimits.quiet_NaN() && val != floatLimits.signaling_NaN() && val != floatLimits.infinity() && val != -floatLimits.infinity()) {
            rval = 1;
        } else {
            rval = 0;
        }
        break;
    }
    }
    asynPrint(pasynUser, ASYN_TRACEIO_DEVICE, "%s BinarySupport::readInternal read boolean %s\n", record->name, (rval != 0) ? "TRUE" : "FALSE");
}

void BinarySupport::writeInternal(asynUser *pasynUser, dbCommon *commonRecord, epicsUInt32 rval, void* buffer, int bufferSize, s7nodavePlcDataType plcDataType) {
    switch (plcDataType) {
    case plcDataTypeBool:
    {
        assert(bufferSize >= 1);
        unsigned char *val = static_cast<unsigned char *>(buffer);
        *val = (rval != 0) ? 1 : 0;
        asynPrint(pasynUser, ASYN_TRACEIO_DEVICE, "%s BinarySupport::writeInternal writing bit %hhu\n", commonRecord->name, *val);
        break;
    }
    case plcDataTypeInt8:
    {
        assert(bufferSize >= 1);
        epicsInt8 *val = static_cast<epicsInt8 *>(buffer);
        *val = (rval != 0) ? 1 : 0;
        asynPrint(pasynUser, ASYN_TRACEIO_DEVICE, "%s BinarySupport::writeInternal writing int8 %hhd\n", commonRecord->name, *val);
        break;
    }
    case plcDataTypeUint8:
    {
        assert(bufferSize >= 1);
        epicsUInt8 *val = static_cast<epicsUInt8 *>(buffer);
        *val = (rval != 0) ? 1 : 0;
        asynPrint(pasynUser, ASYN_TRACEIO_DEVICE, "%s BinarySupport::writeInternal writing uint8 %hhu\n", commonRecord->name, *val);
        break;
    }
    case plcDataTypeInt16:
    {
        assert(bufferSize >= 2);
        epicsInt16 *val = static_cast<epicsInt16 *>(buffer);
        *val = (rval != 0) ? 1 : 0;
        asynPrint(pasynUser, ASYN_TRACEIO_DEVICE, "%s BinarySupport::writeInternal writing int16 %hd\n", commonRecord->name, *val);
        break;
    }
    case plcDataTypeUint16:
    {
        assert(bufferSize >= 2);
        epicsUInt16 *val = static_cast<epicsUInt16 *>(buffer);
        *val = (rval != 0) ? 1 : 0;
        asynPrint(pasynUser, ASYN_TRACEIO_DEVICE, "%s BinarySupport::writeInternal writing uint16 %hu\n", commonRecord->name, *val);
        break;
    }
    case plcDataTypeInt32:
    {
        assert(bufferSize >= 4);
        epicsInt32 *val = static_cast<epicsInt32 *>(buffer);
        *val = (rval != 0) ? 1 : 0;
        asynPrint(pasynUser, ASYN_TRACEIO_DEVICE, "%s BinarySupport::writeInternal writing int32 %d\n", commonRecord->name, *val);
        break;
    }
    case plcDataTypeUint32:
    {
        assert(bufferSize >= 4);
        epicsUInt32 *val = static_cast<epicsUInt32 *>(buffer);
        *val = (rval != 0) ? 1 : 0;
        asynPrint(pasynUser, ASYN_TRACEIO_DEVICE, "%s BinarySupport::writeInternal writing uint32 %u\n", commonRecord->name, *val);
        break;
    }
    case plcDataTypeFloat:
    {
        assert(bufferSize >= 4);
        epicsFloat32 *val = static_cast<epicsFloat32 *>(buffer);
        *val = (rval != 0) ? 1.0 : 0.0;
        asynPrint(pasynUser, ASYN_TRACEIO_DEVICE, "%s BinarySupport::writeInternal writing float %f\n", commonRecord->name, *val);
        break;
    }
    }
}

Optional<s7nodavePlcDataType> BinarySupport::getPlcDataType(PlcAddress, Optional<s7nodavePlcDataType> userRequestedType, Optional<s7nodavePlcDataType> defaultType) {
    // All data types are supported
    Optional<s7nodavePlcDataType> dataType;
    if (userRequestedType) {
        dataType = userRequestedType;
    } else {
        dataType = defaultType;
    }
    return dataType;
}

} // namespace s7nodave
