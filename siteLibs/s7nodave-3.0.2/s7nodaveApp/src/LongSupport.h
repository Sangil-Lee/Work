#ifndef s7nodave_LongSupport_h
#define s7nodave_LongSupport_h

#include <dbCommon.h>

#include "Optional.h"
#include "PlcAddress.h"
#include "s7nodave.h"
#include "S7nodaveRecordAddress.h"
#include "s7nodaveAsyn.h"

namespace s7nodave {

/*
 * This class provides functions that are common to longin and longout records.
 */
class LongSupport {
public:
    /**
     * Reads data from buffer into the VAL field of the record. This function
     * assumes that the buffer already contains the right number of elements
     * and has the correct (host architecture) byte order.
     */
    template <class recordType>
    static long read(asynUser *pasynUser, dbCommon *record, const void *buffer, int bufferSize, s7nodavePlcDataType plcDataType) {
        recordType *typedRecord = reinterpret_cast<recordType *>(record);
        readInternal(pasynUser, record, typedRecord->val, buffer, bufferSize, plcDataType);
        return RECORD_STATUS_OK;
    };

    /**
     * Writes data from the RVAL record field into the buffer. This function
     * assumes that the buffer has sufficient space for the value and writes the
     * value in host architecture byte order.
     */
    template <class recordType>
    static void write(asynUser *pasynUser, dbCommon *record, void* buffer, int bufferSize, s7nodavePlcDataType plcDataType) {
        recordType *typedRecord = reinterpret_cast<recordType *>(record);
        writeInternal(pasynUser, record, typedRecord->val, buffer, bufferSize, plcDataType);
    };

    /**
     * Returns the PLC data-type used for the specified address. If the address
     * string requests a PLC data-type that is not supported by this record
     * type, an empty result is returned.
     */
    static Optional<s7nodavePlcDataType> getPlcDataType(PlcAddress plcAddress, Optional<s7nodavePlcDataType> userRequestedType, Optional<s7nodavePlcDataType> defaultType);

private:
    /**
     * The constructor is private because all public methods of this class are
     * static.
     */
    LongSupport() {
    };
    static void readInternal(asynUser *pasynUser, dbCommon *record, epicsInt32& val, const void *buffer, int bufferSize, s7nodavePlcDataType plcDataType);
    static void writeInternal(asynUser *pasynUser, dbCommon *record, epicsInt32 val, void* buffer, int bufferSize, s7nodavePlcDataType plcDataType);
};

} // namespace s7nodave

#endif // s7nodave_LongSupport_h
