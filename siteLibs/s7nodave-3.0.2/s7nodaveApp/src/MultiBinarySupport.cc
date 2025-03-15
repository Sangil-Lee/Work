#include <cassert>
#include <limits>

#include <alarm.h>
#include <epicsTypes.h>
#include <mbbiRecord.h>
#include <mbbiDirectRecord.h>
#include <mbboRecord.h>
#include <mbboDirectRecord.h>
#include <recGbl.h>

#include "PlcAddress.h"

#include "MultiBinarySupport.h"

namespace s7nodave {

namespace {

void writeCommon(asynUser *pasynUser, dbCommon *record, epicsUInt32 rval, void *buffer, int bufferSize, s7nodavePlcDataType plcDataType) {
    switch (plcDataType) {
    case plcDataTypeInt8:
    case plcDataTypeUint8:
    {
        assert(bufferSize >= 1);
        epicsUInt8 *val = static_cast<epicsUInt8 *>(buffer);
        *val = rval & 0xff;
        asynPrint(pasynUser, ASYN_TRACEIO_DEVICE, "%s MultiBinarySupport::writeCommon writing byte %u\n", record->name, *val);
        break;
    }
    case plcDataTypeInt16:
    case plcDataTypeUint16:
    {
        assert(bufferSize >= 2);
        epicsUInt16 *val = static_cast<epicsUInt16 *>(buffer);
        *val = rval & 0xffff;
        asynPrint(pasynUser, ASYN_TRACEIO_DEVICE, "%s MultiBinarySupport::writeCommon writing word %u\n", record->name, *val);
        break;
    }
    case plcDataTypeInt32:
    case plcDataTypeUint32:
    {
        assert(bufferSize >= 4);
        epicsUInt32 *val = static_cast<epicsUInt32 *>(buffer);
        *val = rval;
        asynPrint(pasynUser, ASYN_TRACEIO_DEVICE, "%s MultiBinarySupport::writeCommon writing byte %u\n", record->name, *val);
        break;
    }
    default:
        // Unsupported type, that should never happen
        asynPrint(pasynUser, ASYN_TRACE_ERROR, "%s MultiBinarySupport::writeCommon Got unexpected PLC data-type.\n", record->name);
        return;
    }
}

} // anonymous namesapce

template <>
void MultiBinarySupport::write<mbboRecord>(asynUser *pasynUser, dbCommon *record, void* buffer, int bufferSize, s7nodavePlcDataType plcDataType) {
    mbboRecord *typedRecord = reinterpret_cast<mbboRecord *>(record);
    epicsUInt32 mask;
    epicsUInt32 rval;
    rval = typedRecord->val;
    mask = typedRecord->mask;
    mbboRecord *r = typedRecord;
    epicsUInt32 *vals[] = {&r->zrvl, &r->onvl, &r->twvl, &r->thvl, &r->frvl, &r->fvvl, &r->sxvl, &r->svvl, &r->eivl, &r->nivl, &r->tevl, &r->elvl, &r->tvvl, &r->ttvl, &r->ftvl, &r->ffvl};
    for (int i = 0; i < 16; i++) {
        if (*vals[i] != 0) {
            // If at least one value is defined, use RVAL
            rval = typedRecord->rval;
            if (mask != 0) {
                rval &= mask;
            }
            break;
        }
    }
    writeCommon(pasynUser, record, rval, buffer, bufferSize, plcDataType);
};

template <>
void MultiBinarySupport::write<mbboDirectRecord>(asynUser *pasynUser, dbCommon *record, void* buffer, int bufferSize, s7nodavePlcDataType plcDataType) {
    mbboDirectRecord *typedRecord = reinterpret_cast<mbboDirectRecord *>(record);
    epicsUInt32 mask;
    epicsUInt32 rval;
    rval = typedRecord->rval;
    mask = typedRecord->mask;
    if (mask != 0) {
        // Use VAL instead of RVAL
        rval = typedRecord->val;
        rval &= mask;
    }
    writeCommon(pasynUser, record, rval, buffer, bufferSize, plcDataType);
};

void MultiBinarySupport::initMaskInternal(dbCommon *, epicsUInt32& mask, epicsUInt16& nobt, epicsUInt16 shft, s7nodavePlcDataSize plcDataSize) {
    int dataSizeInBits = PlcAddress::dataSizeInBits(plcDataSize);
    if (nobt > dataSizeInBits) {
        nobt = 0;
        mask = 0;
    }
    if (shft > 0) {
        mask <<= shft;
    }
}

void MultiBinarySupport::readInternal(asynUser *pasynUser, dbCommon *record, epicsUInt32& rval, epicsUInt32 mask, const void *buffer, int bufferSize, s7nodavePlcDataType plcDataType) {
    // We treat all numbers as unsigned, because we are only interested in the
    // bits.
    switch (plcDataType) {
    case plcDataTypeInt8:
    case plcDataTypeUint8:
        assert(bufferSize >= 1);
        rval = *(static_cast<const epicsUInt8 *>(buffer));
        break;
    case plcDataTypeInt16:
    case plcDataTypeUint16:
        assert(bufferSize >= 2);
        rval = *(static_cast<const epicsUInt16 *>(buffer));
        break;
    case plcDataTypeInt32:
    case plcDataTypeUint32:
        assert(bufferSize >= 4);
        rval = *(static_cast<const epicsUInt32 *>(buffer));
        break;
    default:
        // Unsupported type, should never happen
        asynPrint(pasynUser, ASYN_TRACE_ERROR, "%s MultiBinarySupport::readInternal Got unexpected PLC data-type.\n", record->name);
        return;
    }
    if (mask != 0) {
        rval &= mask;
    }
    asynPrint(pasynUser, ASYN_TRACEIO_DEVICE, "%s MultiBinarySupport::readInternal read long %u\n", record->name, rval);
}

Optional<s7nodavePlcDataType> MultiBinarySupport::getPlcDataType(PlcAddress, Optional<s7nodavePlcDataType> userRequestedType, Optional<s7nodavePlcDataType> defaultType) {
    Optional<s7nodavePlcDataType> dataType;
    if (userRequestedType) {
        dataType = userRequestedType;
    } else {
        dataType = defaultType;
    }
    if (dataType && *dataType == plcDataTypeBool) {
        // bool is not supported
        return Optional<s7nodavePlcDataType>();
    } else if (dataType && *dataType == plcDataTypeFloat) {
        if (userRequestedType) {
            // float is not supported
            return Optional<s7nodavePlcDataType>();
        } else {
            // int32 has the same length as float
            return plcDataTypeUint32;
        }
    }
    return dataType;
}

} // namespace s7nodave
