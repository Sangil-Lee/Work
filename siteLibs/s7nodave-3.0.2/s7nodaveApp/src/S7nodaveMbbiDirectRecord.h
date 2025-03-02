#ifndef S7nodaveMbbiDirectRecord_h
#define S7nodaveMbbiDirectRecord_h

#include <mbbiDirectRecord.h>

#include "MultiBinarySupport.h"
#include "S7nodaveInputRecord.h"
#include "s7nodave.h"
#include "s7nodaveAsyn.h"

namespace s7nodave {

/**
 * Device support for mbbiDirect record.
 */
class S7nodaveMbbiDirectRecord : public S7nodaveInputRecord {
public:
    /**
     * Constructor. The passed record pointer is stored and used by all methods,
     * which need to access record fields.
     */
    S7nodaveMbbiDirectRecord(dbCommon *record) :
        S7nodaveInputRecord(record, mbbiDirectRecordType) {

    };

    virtual long initRecord() override {
        long status;
        status = S7nodaveInputRecord::initRecord();
        if (status == RECORD_STATUS_OK) {
            MultiBinarySupport::initMask<mbbiDirectRecord>(this->record, this->recordAddress->getPlcAddress().getDataSize());
        }
        return status;
    };

protected:
    virtual Optional<s7nodavePlcDataType> getPlcDataType(PlcAddress plcAddress, Optional<s7nodavePlcDataType> suggestion) override {
        auto defaultType = S7nodaveInputRecord::getPlcDataType(plcAddress, suggestion);
        return MultiBinarySupport::getPlcDataType(plcAddress, suggestion, defaultType);
    };

    virtual DBLINK getDeviceAddress() const override {
        mbbiDirectRecord *mbbiDRec = reinterpret_cast<mbbiDirectRecord *>(this->record);
        return mbbiDRec->inp;
    };

    virtual long writeToRecord(void *buffer, int bufferSize) override {
        MultiBinarySupport::read<mbbiDirectRecord>(this->myAsynUser, this->record, buffer, bufferSize, this->recordAddress->getPlcDataType());
        return RECORD_STATUS_OK;
    };
};

} // namespace s7nodave

#endif // S7nodaveMbbiDirectRecord_h
