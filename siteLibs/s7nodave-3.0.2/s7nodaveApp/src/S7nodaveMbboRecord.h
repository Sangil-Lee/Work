#ifndef S7nodaveMbboRecord_h
#define S7nodaveMbboRecord_h

#include <mbboRecord.h>

#include "MultiBinarySupport.h"
#include "S7nodaveOutputRecord.h"
#include "s7nodave.h"
#include "s7nodaveAsyn.h"

namespace s7nodave {

/**
 * Device support for mbbo record.
 */
class S7nodaveMbboRecord : public S7nodaveOutputRecord {
public:
    /**
     * Constructor. The passed record pointer is stored and used by all methods,
     * which need to access record fields.
     */
    S7nodaveMbboRecord(dbCommon *record) :
        S7nodaveOutputRecord(record, mbboRecordType) {
    };

protected:
    virtual long interceptInitRecordBeforeValueInit() override {
        MultiBinarySupport::initMask<mbboRecord>(this->record, this->recordAddress->getPlcAddress().getDataSize());
        return RECORD_STATUS_OK;
    };

    virtual Optional<s7nodavePlcDataType> getPlcDataType(PlcAddress plcAddress, Optional<s7nodavePlcDataType> suggestion) override {
        auto defaultType = S7nodaveOutputRecord::getPlcDataType(plcAddress, suggestion);
        return MultiBinarySupport::getPlcDataType(plcAddress, suggestion, defaultType);
    };

    virtual DBLINK getDeviceAddress() const override {
        mbboRecord *mbboRec = reinterpret_cast<mbboRecord *>(this->record);
        return mbboRec->out;
    };

    virtual void readFromRecord(void *buffer, int bufferSize) const override {
        MultiBinarySupport::write<mbboRecord>(this->myAsynUser, this->record, buffer, bufferSize, this->recordAddress->getPlcDataType());
    };

    virtual long writeToRecord(void *buffer, int bufferSize) override {
        MultiBinarySupport::read<mbboRecord>(this->myAsynUser, this->record, buffer, bufferSize, this->recordAddress->getPlcDataType());
        return RECORD_STATUS_OK;
    };
};

} // namespace s7nodave

#endif // S7nodaveMbboRecord_h
