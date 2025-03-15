#ifndef S7nodaveAaiRecord_h
#define S7nodaveAaiRecord_h

#include <aaiRecord.h>
#include <dbFldTypes.h>

#include "ArraySupport.h"
#include "S7nodaveInputRecord.h"
#include "s7nodave.h"
#include "s7nodaveAsyn.h"

namespace s7nodave {

/**
 * Device support for aai record.
 */
class S7nodaveAaiRecord : public S7nodaveInputRecord {
public:
    /**
     * Constructor. The passed record pointer is stored and used by all methods,
     * which need to access record fields.
     */
    S7nodaveAaiRecord(dbCommon *record) :
        S7nodaveInputRecord(record, aaiRecordType) {

    };

protected:
    virtual Optional<s7nodavePlcDataType> getPlcDataType(PlcAddress plcAddress, Optional<s7nodavePlcDataType> suggestion) override {
        aaiRecord *aaiRec = reinterpret_cast<aaiRecord *>(this->record);
        return ArraySupport::getPlcDataTypeIn(plcAddress, suggestion, aaiRec->ftvl);
    };

    virtual DBLINK getDeviceAddress() const override {
        aaiRecord *aaiRec = reinterpret_cast<aaiRecord *>(this->record);
        return aaiRec->inp;
    };

    virtual unsigned long int getIoBufferSizeInBits() const override {
        aaiRecord *aaiRec = reinterpret_cast<aaiRecord *>(this->record);
        if (aaiRec->ftvl == DBF_STRING) {
            // String is special because each element takes 40 bytes, while
            // the corresponding PLC type is only one byte.
            return MAX_STRING_SIZE * aaiRec->nelm * S7nodaveInputRecord::getIoBufferSizeInBits();
        } else {
            // For all other data-types, we can just multiply by the number of
            // elements.
            return aaiRec->nelm * S7nodaveInputRecord::getIoBufferSizeInBits();
        }
    };

    virtual long writeToRecord(void *buffer, int bufferSize) override {
        return ArraySupport::read<aaiRecord>(this->myAsynUser, this->record, buffer, bufferSize, this->recordAddress->getPlcDataType());
    };
};

} // namespace s7nodave

#endif // S7nodaveAaiRecord_h
