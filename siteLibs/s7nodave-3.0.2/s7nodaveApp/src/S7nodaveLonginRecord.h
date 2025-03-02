#ifndef S7nodaveLonginRecord_h
#define S7nodaveLonginRecord_h

#include <longinRecord.h>

#include "LongSupport.h"
#include "S7nodaveInputRecord.h"
#include "s7nodave.h"
#include "s7nodaveAsyn.h"

namespace s7nodave {

/**
 * Device support for longin record.
 */
class S7nodaveLonginRecord : public S7nodaveInputRecord {
public:
    /**
     * Constructor. The passed record pointer is stored and used by all methods,
     * which need to access record fields.
     */
    S7nodaveLonginRecord(dbCommon *record) :
        S7nodaveInputRecord(record, longinRecordType) {
    };

protected:
    virtual Optional<s7nodavePlcDataType> getPlcDataType(PlcAddress plcAddress, Optional<s7nodavePlcDataType> suggestion) override {
        auto defaultType = S7nodaveInputRecord::getPlcDataType(plcAddress, suggestion);
        return LongSupport::getPlcDataType(plcAddress, suggestion, defaultType);
    };

    virtual DBLINK getDeviceAddress() const override {
        longinRecord *liRec = reinterpret_cast<longinRecord *>(this->record);
        return liRec->inp;
    };

    virtual long writeToRecord(void *buffer, int bufferSize) override {
        LongSupport::read<longinRecord>(this->myAsynUser, this->record, buffer, bufferSize, this->recordAddress->getPlcDataType());
        return RECORD_STATUS_OK;
    };
};

} // namespace s7nodave

#endif // S7nodaveLonginRecord_h
