#ifndef s7nodave_MultiBinarySupport_h
#define s7nodave_MultiBinarySupport_h

#include <dbCommon.h>
#include <mbboDirectRecord.h>
#include <mbboRecord.h>

#include "Optional.h"
#include "PlcAddress.h"
#include "S7nodaveRecordAddress.h"
#include "s7nodave.h"
#include "s7nodaveAsyn.h"

namespace s7nodave {

/**
 * This class provides functions that are common to mbbi, mbbo, mbbiDirect and
 * mbboDirect records.
 */
class MultiBinarySupport {
public:
    /**
     * The MASK field of the record should have been initialized by the record,
     * so that the lowest NOBT bits are set. If NOBT is negative or bigger than
     * the size of the selected PLC data-type, MASK is set to be zero.
     * Otherwise, MASK is shifted to the left (higher bits) by SHFT bits.
     */
    template <class recordType>
    static void initMask(dbCommon *record, s7nodavePlcDataSize plcDataSize) {
        recordType *typedRecord = reinterpret_cast<recordType *>(record);
        // Since EPICS 3.15.4, the NOBT field of the mbbi and mbbo record is of
        // type epicsUInt16, but the NOBT field of the mbbiDirect and mbboDirect
        // records is of type epicsInt16. In earlier versions, all four record
        // types used epicsInt16. We simply cast the parameter to epicsUInt16.
        // We never read this parameter and only set it to zero if it is greater
        // than the number of bits of the device data-type. Signed and unsigned
        // zero is the same, so this should be safe.
        initMaskInternal(record, typedRecord->mask, reinterpret_cast<epicsUInt16&>(typedRecord->nobt), typedRecord->shft, plcDataSize);
    }

    /**
     * Reads data from buffer into the RVAL field of the record. This function
     * assumes that the buffer already contains the right number of elements
     * and has the correct (host architecture) byte order.
     */
    template <class recordType>
    static long read(asynUser *pasynUser, dbCommon *record, const void *buffer, int bufferSize, s7nodavePlcDataType plcDataType) {
        recordType *typedRecord = reinterpret_cast<recordType *>(record);
        readInternal(pasynUser, record, typedRecord->rval, typedRecord->mask, buffer, bufferSize, plcDataType);
        return RECORD_STATUS_OK;
    };

    /**
     * Writes data from the RVAL record field into the buffer. This function
     * assumes that the buffer has sufficient space for the value and writes the
     * value in host architecture byte order.
     *
     * For this method, there are actually two specialized implementations for
     * the mbbo and mbboDirect record-type.
     */
    template <class recordType>
    static void write(asynUser *pasynUser, dbCommon *record, void* buffer, int bufferSize, s7nodavePlcDataType plcDataType);

    /**
     * Returns the PLC data-type used for the specified address. If the address
     * string requests a PLC data-type that is not supported by this record
     * type, an empty result is returned.
     */
    static Optional<s7nodavePlcDataType> getPlcDataType(PlcAddress plcAddress, Optional<s7nodavePlcDataType> userRequestedType, Optional<s7nodavePlcDataType> defaultType);

private:
    static void initMaskInternal(dbCommon *record, epicsUInt32& mask, epicsUInt16& nobt, epicsUInt16 shft, s7nodavePlcDataSize plcDataSize);
    static void readInternal(asynUser *pasynUser, dbCommon *record, epicsUInt32& rval, epicsUInt32 mask, const void *buffer, int bufferSize, s7nodavePlcDataType plcDataType);
};

} // namespace s7nodave

#endif // s7nodave_MultiBinarySupport_h
