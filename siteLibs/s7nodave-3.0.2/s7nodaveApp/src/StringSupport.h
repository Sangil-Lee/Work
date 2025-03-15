#ifndef s7nodave_StringSupport_h
#define s7nodave_StringSupport_h

#include <dbCommon.h>

#include "Optional.h"
#include "PlcAddress.h"
#include "s7nodave.h"
#include "S7nodaveRecordAddress.h"
#include "s7nodaveAsyn.h"

namespace s7nodave {

/**
 * This class provides functions that are common to stringin and stringout
 * records.
 */
class StringSupport {
public:
    /**
     * Reads data from buffer into the VAL field of the record. This function
     * will always copy exactly MAX_STRING_SIZE bytes of data, regardless of how
     * long the string stored in the record actually is. It will ensure that the
     * copied string is always null-terminated.
     */
    template <class recordType>
    static long read(asynUser *pasynUser, dbCommon *record, const void *buffer, int bufferSize, s7nodavePlcDataType plcDataType) {
        recordType *typedRecord = reinterpret_cast<recordType *>(record);
        readInternal(pasynUser, record, typedRecord->val, buffer, bufferSize, plcDataType);
        return RECORD_STATUS_OK;
    };

    /**
     * Writes data from the VAL record field into the buffer. This function will
     * always copy exactly MAX_STRING_SIZE bytes of data, regardless of how long
     * the string stored in the buffer actually is. It will ensure that the
     * copied string is always null-terminated.
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
    static Optional<s7nodavePlcDataType> getPlcDataType(PlcAddress plcAddress, Optional<s7nodavePlcDataType> userRequestedType);


    /**
     * Returns the size needed for the I/O buffer. This is always the maximum
     * string length (MAX_STRING_SIZE, usually 40 bytes) in bits.
     */
    static unsigned long getIoBufferSizeInBits(s7nodavePlcDataType plcDataType);

private:
    /**
     * The constructor is private because all public methods of this class are
     * static.
     */
    StringSupport() {
    };

    static void readInternal(asynUser *pasynUser, dbCommon *record, char *val, const void *buffer, int bufferSize, s7nodavePlcDataType plcDataType);
    static void writeInternal(asynUser *pasynUser, dbCommon *record, char *val, void* buffer, int bufferSize, s7nodavePlcDataType plcDataType);
};

} // namespace s7nodave

#endif // s7nodave_StringSupport_h
