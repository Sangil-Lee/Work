#ifndef S7nodaveBoRecord_h
#define S7nodaveBoRecord_h

#include <boRecord.h>

#include "BinarySupport.h"
#include "S7nodaveOutputRecord.h"
#include "s7nodave.h"
#include "s7nodaveAsyn.h"

namespace s7nodave {

/**
 * Device support for bo record.
 */
class S7nodaveBoRecord : public S7nodaveOutputRecord {
public:
    /**
     * Constructor. The passed record pointer is stored and used by all methods,
     * which need to access record fields.
     */
    S7nodaveBoRecord(dbCommon *record) :
        S7nodaveOutputRecord(record, boRecordType) {

    };

protected:
    virtual Optional<s7nodavePlcDataType> getPlcDataType(PlcAddress plcAddress, Optional<s7nodavePlcDataType> suggestion) override {
        auto defaultType = S7nodaveOutputRecord::getPlcDataType(plcAddress, suggestion);
        return BinarySupport::getPlcDataType(plcAddress, suggestion, defaultType);
    };

    virtual DBLINK getDeviceAddress() const override {
        boRecord *boRec = reinterpret_cast<boRecord *>(this->record);
        return boRec->out;
    };

    virtual void readFromRecord(void *buffer, int bufferSize) const override {
        BinarySupport::write<boRecord>(this->myAsynUser, this->record, buffer, bufferSize, this->recordAddress->getPlcDataType());
    };

    virtual long writeToRecord(void *buffer, int bufferSize) override {
        BinarySupport::read<boRecord>(this->myAsynUser, this->record, buffer, bufferSize, this->recordAddress->getPlcDataType());
        return RECORD_STATUS_OK;
    };
};

} // namespace s7nodave

#endif // S7nodaveBoRecord_h
