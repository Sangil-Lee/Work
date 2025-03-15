#ifndef s7nodave_AnalogSupport_h
#define s7nodave_AnalogSupport_h

#include <dbCommon.h>

#include "Optional.h"
#include "PlcAddress.h"
#include "S7nodaveRecordAddress.h"
#include "s7nodaveAsyn.h"

namespace s7nodave {

/**
 * This class provides functions that are common to ai and ao records.
 */
class AnalogSupport {
public:
    /**
     * Reads data from buffer into the corresponding record fields (RVAL or
     * VAL). This function assumes that the buffer already contains the right
     * number of elements and has the correct (host architecture) byte order.
     */
    template <class recordType>
    static long read(asynUser *pasynUser, dbCommon *record, const void *buffer, int bufferSize, s7nodavePlcDataType plcDataType) {
        recordType *typedRecord = reinterpret_cast<recordType *>(record);
        return readInternal(pasynUser, record, typedRecord->rval, typedRecord->val, buffer, bufferSize, plcDataType);
    };

    /**
     * Writes data from the corresponding record fields (RVAL or VAL) into the
     * buffer. This function assumes that the buffer has sufficient space for
     * the value and writes the value in host architecture byte order.
     */
    template <class recordType>
    static void write(asynUser *pasynUser, dbCommon *record, void* buffer, int bufferSize, s7nodavePlcDataType plcDataType) {
        recordType *typedRecord = reinterpret_cast<recordType *>(record);
        writeInternal(pasynUser, record, typedRecord->rval, typedRecord->val, buffer, bufferSize, plcDataType);
    };

    /**
     * Returns the PLC data-type used for the specified address. If the address
     * string requests a PLC data-type that is not supported by this record
     * type, an empty result is returned.
     */
    static Optional<s7nodavePlcDataType> getPlcDataType(PlcAddress plcAddress, Optional<s7nodavePlcDataType> userRequestedType, Optional<s7nodavePlcDataType> defaultType);

    /**
     * Extracts the device parameters (DLV and DHV) supported by analog records
     * and returns the read values or default values in the passed references.
     */
    static void extractDeviceParameters(asynUser *pasynuser, dbCommon *record, S7nodaveRecordAddress::DeviceParameters& deviceParameters, double& deviceLowValue, double& deviceHighValue);
    static void initDeviceLimits(S7nodaveRecordAddress recordAddress, double& deviceLowValue, double& deviceHighValue);

    /**
     * Initializes the records ESLO and EOFF fields using the record field
     * EGUL and EGUF and the given limits for the raw values. ESLO and EOFF
     * are calculated, so that a raw value of deviceLowValue corresponds to an
     * engineering-unit value of EGUL and an engineering-unit value of
     * deviceHighValue corresponds to an engineering-unit value of EGUF.
     */
    template <class recordType>
    static long convert(dbCommon *record, double deviceLowValue, double deviceHighValue) {
        recordType *typedRecord = reinterpret_cast<recordType *>(record);
        return convertInternal(record, typedRecord->eslo, typedRecord->eoff, typedRecord->eguf, typedRecord->egul, deviceLowValue, deviceHighValue);
    };

private:
    /**
     * The constructor is private because all public methods of this class are
     * static.
     */
    AnalogSupport() {
    };

    static long convertInternal(dbCommon *record, epicsFloat64& eslo, epicsFloat64& eoff, epicsFloat64 eguf, epicsFloat64 egul, double deviceLowValue, double deviceHighValue);

    static long readInternal(asynUser *pasynUser, dbCommon *record, epicsInt32& rval, epicsFloat64& val, const void *buffer, int bufferSize, s7nodavePlcDataType plcDataType);

    static void writeInternal(asynUser *pasynUser, dbCommon *commonRecord, epicsInt32 rval, epicsFloat64 val, void* buffer, int bufferSize, s7nodavePlcDataType plcDataType);
};

} // namespace s7nodave

#endif // s7nodave_AnalogSupport_h
