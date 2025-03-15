#ifndef S7nodaveStringoutRecord_h
#define S7nodaveStringoutRecord_h

#include <stringoutRecord.h>
#include <dbFldTypes.h>

#include "S7nodaveOutputRecord.h"
#include "s7nodave.h"
#include "s7nodaveAsyn.h"
#include "StringSupport.h"

namespace s7nodave {

/**
 * Device support for stringout record.
 */
class S7nodaveStringoutRecord : public S7nodaveOutputRecord {
public:
    /**
     * Constructor. The passed record pointer is stored and used by all methods,
     * which need to access record fields.
     */
    S7nodaveStringoutRecord(dbCommon *record) :
        S7nodaveOutputRecord(record, stringoutRecordType) {
    };

protected:

    virtual Optional<s7nodavePlcDataType> getPlcDataType(PlcAddress plcAddress, Optional<s7nodavePlcDataType> suggestion) override {
        return StringSupport::getPlcDataType(plcAddress, suggestion);
    };

    virtual DBLINK getDeviceAddress() const override {
        stringoutRecord *soRec = reinterpret_cast<stringoutRecord *>(this->record);
        return soRec->out;
    };

    virtual void readFromRecord(void *buffer, int bufferSize) const override {
        StringSupport::write<stringoutRecord>(this->myAsynUser, this->record, buffer, bufferSize, this->recordAddress->getPlcDataType());
    };

    virtual unsigned long int getIoBufferSizeInBits() const override {
        return StringSupport::getIoBufferSizeInBits(this->recordAddress->getPlcDataType());
    };

    virtual long writeToRecord(void *buffer, int bufferSize) override {
        return StringSupport::read<stringoutRecord>(this->myAsynUser, this->record, buffer, bufferSize, this->recordAddress->getPlcDataType());
    };
};

} // namespace s7nodave

#endif // S7nodaveStringoutRecord_h
