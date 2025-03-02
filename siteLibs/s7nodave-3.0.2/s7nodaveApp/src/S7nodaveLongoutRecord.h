#ifndef S7nodaveLongoutRecord_h
#define S7nodaveLongoutRecord_h

#include <longoutRecord.h>

#include "LongSupport.h"
#include "S7nodaveOutputRecord.h"
#include "s7nodave.h"
#include "s7nodaveAsyn.h"

namespace s7nodave {

/**
 * Device support for longout record.
 */
class S7nodaveLongoutRecord : public S7nodaveOutputRecord {
public:
    /**
     * Constructor. The passed record pointer is stored and used by all methods,
     * which need to access record fields.
     */
    S7nodaveLongoutRecord(dbCommon *record) :
        S7nodaveOutputRecord(record, longoutRecordType) {
    };

protected:
    virtual Optional<s7nodavePlcDataType> getPlcDataType(PlcAddress plcAddress, Optional<s7nodavePlcDataType> suggestion) override {
        auto defaultType = S7nodaveOutputRecord::getPlcDataType(plcAddress, suggestion);
        return LongSupport::getPlcDataType(plcAddress, suggestion, defaultType);
    };

    virtual DBLINK getDeviceAddress() const override {
        longoutRecord *loRec = reinterpret_cast<longoutRecord *>(this->record);
        return loRec->out;
    };

    virtual void readFromRecord(void *buffer, int bufferSize) const override {
        LongSupport::write<longoutRecord>(this->myAsynUser, this->record, buffer, bufferSize, this->recordAddress->getPlcDataType());
    };

    virtual long writeToRecord(void *buffer, int bufferSize) override {
        LongSupport::read<longoutRecord>(this->myAsynUser, this->record, buffer, bufferSize, this->recordAddress->getPlcDataType());
        return RECORD_STATUS_OK;
    };
};

} // namespace s7nodave

#endif // S7nodaveLongoutRecord_h
