#ifndef S7nodaveStringinRecord_h
#define S7nodaveStringinRecord_h

#include <stringinRecord.h>
#include <dbFldTypes.h>

#include "S7nodaveInputRecord.h"
#include "s7nodave.h"
#include "s7nodaveAsyn.h"
#include "StringSupport.h"

namespace s7nodave {

/**
 * Device support for stringin record.
 */
class S7nodaveStringinRecord : public S7nodaveInputRecord {
public:
    /**
     * Constructor. The passed record pointer is stored and used by all methods,
     * which need to access record fields.
     */
    S7nodaveStringinRecord(dbCommon *record) :
        S7nodaveInputRecord(record, stringinRecordType) {
    };

protected:
    virtual Optional<s7nodavePlcDataType> getPlcDataType(PlcAddress plcAddress, Optional<s7nodavePlcDataType> suggestion) override {
        return StringSupport::getPlcDataType(plcAddress, suggestion);
    };

    virtual DBLINK getDeviceAddress() const override {
        stringinRecord *siRec = reinterpret_cast<stringinRecord *>(this->record);
        return siRec->inp;
    };

    virtual unsigned long int getIoBufferSizeInBits() const override {
        return StringSupport::getIoBufferSizeInBits(this->recordAddress->getPlcDataType());
    };

    virtual long writeToRecord(void *buffer, int bufferSize) override {
        return StringSupport::read<stringinRecord>(this->myAsynUser, this->record, buffer, bufferSize, this->recordAddress->getPlcDataType());
    };
};

} // namespace s7nodave

#endif // S7nodaveStringinRecord_h
