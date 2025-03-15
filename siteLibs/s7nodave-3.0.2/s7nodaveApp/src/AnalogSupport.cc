#include <cassert>

#include <alarm.h>
#include <epicsTypes.h>
#include <aiRecord.h>
#include <aoRecord.h>
#include <recGbl.h>

#include "s7nodave.h"

#include "AnalogSupport.h"

namespace s7nodave {

long AnalogSupport::readInternal(asynUser *pasynUser, dbCommon *record, epicsInt32& rval, epicsFloat64& val, const void *buffer, int bufferSize, s7nodavePlcDataType plcDataType) {
    switch (plcDataType) {
    case plcDataTypeBool:
        assert(bufferSize >= 1);
        rval = *(static_cast<const unsigned char *>(buffer));
        break;
    case plcDataTypeInt8:
        assert(bufferSize >= 1);
        rval = *(static_cast<const epicsInt8 *>(buffer));
        break;
    case plcDataTypeUint8:
        assert(bufferSize >= 1);
        rval = *(static_cast<const epicsUInt8 *>(buffer));
        break;
    case plcDataTypeInt16:
        assert(bufferSize >= 2);
        rval = *(static_cast<const epicsInt16 *>(buffer));
        break;
    case plcDataTypeUint16:
        assert(bufferSize >= 2);
        rval = *(static_cast<const epicsUInt16 *>(buffer));
        break;
    case plcDataTypeInt32:
        assert(bufferSize >= 4);
        rval = *(static_cast<const epicsInt32 *>(buffer));
        break;
    case plcDataTypeFloat:
        assert(bufferSize >= 4);
        val = *(static_cast<const epicsFloat32 *>(buffer));
        asynPrint(pasynUser, ASYN_TRACEIO_DEVICE, "%s AnalogSupport:readInternal Read value %f\n", record->name, val);
        return RECORD_STATUS_OK;
    default:
        // Data type (uint32) is not supported
        asynPrint(pasynUser, ASYN_TRACE_ERROR, "%s AnalogSupport:readInternal Got value with unsupported PLC data-type.\n", record->name);
        recGblSetSevr(record, READ_ALARM, INVALID_ALARM);
        break;
    }
    asynPrint(pasynUser, ASYN_TRACEIO_DEVICE, "%s AnalogSupport:readInternal Read raw value %d\n", record->name, rval);
    return RECORD_STATUS_OK;
}

void AnalogSupport::writeInternal(asynUser *pasynUser, dbCommon *record, epicsInt32 rval, epicsFloat64 recordVal, void* buffer, int bufferSize, s7nodavePlcDataType plcDataType) {
    switch (plcDataType) {
    case plcDataTypeBool:
    {
        assert(bufferSize >= 1);
        unsigned char *val = static_cast<unsigned char *>(buffer);
        *val = (rval != 0) ? 1 : 0;
        break;
    }
    case plcDataTypeInt8:
    {
        assert(bufferSize >= 1);
        epicsInt8 *val = static_cast<epicsInt8 *>(buffer);
        *val = rval;
        asynPrint(pasynUser, ASYN_TRACEIO_DEVICE, "%s AnalogSupport:readInternal Writing raw value %hhd\n", record->name, *val);
        break;
    }
    case plcDataTypeUint8:
    {
        assert(bufferSize >= 1);
        epicsUInt8 *val = static_cast<epicsUInt8 *>(buffer);
        *val = rval;
        asynPrint(pasynUser, ASYN_TRACEIO_DEVICE, "%s AnalogSupport:writeInternal Writing raw value %hhu\n", record->name, *val);
        break;
    }
    case plcDataTypeInt16:
    {
        assert(bufferSize >= 2);
        epicsInt16 *val = static_cast<epicsInt16 *>(buffer);
        *val = rval;
        asynPrint(pasynUser, ASYN_TRACEIO_DEVICE, "%s AnalogSupport:writeInternal Writing raw value %hd\n", record->name, *val);
        break;
    }
    case plcDataTypeUint16:
    {
        assert(bufferSize >= 2);
        epicsUInt16 *val = static_cast<epicsUInt16 *>(buffer);
        *val = rval;
        asynPrint(pasynUser, ASYN_TRACEIO_DEVICE, "%s AnalogSupport:writeInternal Writing raw value %hu\n", record->name, *val);
        break;
    }
    case plcDataTypeInt32:
    {
        assert(bufferSize >= 4);
        epicsInt32 *val = static_cast<epicsInt32 *>(buffer);
        *val = rval;
        asynPrint(pasynUser, ASYN_TRACEIO_DEVICE, "%s AnalogSupport:writeInternal Writing raw value %d\n", record->name, *val);
        break;
    }
    case plcDataTypeFloat:
    {
        assert(bufferSize >= 4);
        epicsFloat32 *val = static_cast<epicsFloat32 *>(buffer);
        // If the PLC data-type is float, we use VAL instead of RVAL.
        *val = recordVal;
        asynPrint(pasynUser, ASYN_TRACEIO_DEVICE, "%s AnalogSupport:writeInternal Writing raw value %f\n", record->name, *val);
        break;
    }
    default:
        // Data type (uint32) is not supported
        asynPrint(pasynUser, ASYN_TRACE_ERROR, "%s AnalogSupport:writeInternal Cannot write value with unsupported PLC data-type.\n", record->name);
        recGblSetSevr(record, WRITE_ALARM, INVALID_ALARM);
        break;
    }
}

Optional<s7nodavePlcDataType> AnalogSupport::getPlcDataType(PlcAddress, Optional<s7nodavePlcDataType> userRequestedType, Optional<s7nodavePlcDataType> defaultType) {
    bool userRequested;
    Optional<s7nodavePlcDataType> dataType;
    if (userRequestedType) {
        userRequested = true;
        dataType = userRequestedType;
    } else {
        userRequested = false;
        dataType = defaultType;
    }
    if (dataType && *dataType == plcDataTypeUint32) {
        // This data type is not supported
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

void AnalogSupport::extractDeviceParameters(asynUser *pasynUser, dbCommon *record, S7nodaveRecordAddress::DeviceParameters& deviceParameters, double& deviceLowValue, double& deviceHighValue) {
    S7nodaveRecordAddress::DeviceParameters::iterator dlv = deviceParameters.find("DLV");
    S7nodaveRecordAddress::DeviceParameters::iterator dhv = deviceParameters.find("DHV");
    // Either both or none of these optional parameters has to be specified.
    if (dlv != deviceParameters.end() && dhv != deviceParameters.end()) {
        // Check that parameters have values
        if (!dlv->second) {
            asynPrint(pasynUser, ASYN_TRACE_ERROR, "%s AnalogSupport::extractDeviceParameters Parameter DLV has no value.\n", record->name);
            return;
        }
        if (!dhv->second) {
            asynPrint(pasynUser, ASYN_TRACE_ERROR, "%s AnalogSupport::extractDeviceParameters Parameter DHV has no value.\n", record->name);
            return;
        }
        deviceLowValue = strtod(dlv->second->c_str(), nullptr);
        deviceHighValue = strtod(dhv->second->c_str(), nullptr);
        deviceParameters.erase(dlv);
        deviceParameters.erase(dhv);
    } else if (dlv != deviceParameters.end()) {
        asynPrint(pasynUser, ASYN_TRACE_ERROR, "%s AnalogSupport::extractDeviceParameters Parameter DLV is specified but parameter DHV is missing.\n", record->name);
    } else if (dhv != deviceParameters.end()) {
        asynPrint(pasynUser, ASYN_TRACE_ERROR, "%s AnalogSupport::extractDeviceParameters Parameter DHV is specified but parameter DLV is missing.\n", record->name);
    }
}

void AnalogSupport::initDeviceLimits(S7nodaveRecordAddress recordAddress, double& deviceLowValue, double& deviceHighValue) {
    S7nodaveRecordAddress::DeviceParameters deviceParameters = recordAddress.getDeviceParameters();
    S7nodaveRecordAddress::DeviceParameters::iterator dlv = deviceParameters.find("DLV");
    // If one of the two parameters was specified and we got here, the fields
    // have already been set.
    if (dlv == deviceParameters.end()) {
        // Initialize parameters from PLC data size
        switch(recordAddress.getPlcDataType()) {
        case plcDataTypeBool:
        case plcDataTypeUint8:
        case plcDataTypeUint16:
        case plcDataTypeUint32:
            // Unsigned data types
            deviceLowValue = 0;
            deviceHighValue = (1LL << (PlcAddress::dataSizeInBits(recordAddress.getPlcAddress().getDataSize()))) - 1LL;
            break;
        case plcDataTypeInt8:
        case plcDataTypeInt16:
        case plcDataTypeInt32:
            // Signed data types
            deviceLowValue = -(1LL << (PlcAddress::dataSizeInBits(recordAddress.getPlcAddress().getDataSize()) - 1));
            deviceHighValue = (1LL << (PlcAddress::dataSizeInBits(recordAddress.getPlcAddress().getDataSize()) - 1)) - 1LL;
            break;
        case plcDataTypeFloat:
            // Float does not use conversion
            break;
        }
    }
}

long AnalogSupport::convertInternal(dbCommon *, epicsFloat64& eslo, epicsFloat64& eoff, epicsFloat64 eguf, epicsFloat64 egul, double deviceLowValue, double deviceHighValue) {
    if (deviceLowValue != deviceHighValue) {
        eslo = (eguf - egul)/(deviceHighValue - deviceLowValue);
        eoff = (deviceHighValue * egul - deviceLowValue * eguf) / (deviceHighValue - deviceLowValue);
    }
    return RECORD_STATUS_OK;
}

} // namespace s7nodave
