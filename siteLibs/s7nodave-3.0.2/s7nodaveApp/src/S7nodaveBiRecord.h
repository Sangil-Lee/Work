#ifndef S7nodaveBiRecord_h
#define S7nodaveBiRecord_h

#include <biRecord.h>

#include "BinarySupport.h"
#include "S7nodaveInputRecord.h"
#include "s7nodave.h"
#include "s7nodaveAsyn.h"

namespace s7nodave {

/**
 * Device support for ai record.
 */
class S7nodaveBiRecord : public S7nodaveInputRecord {
public:
    /**
     * Constructor. The passed record pointer is stored and used by all methods,
     * which need to access record fields.
     */
    S7nodaveBiRecord(dbCommon *record) :
        S7nodaveInputRecord(record, biRecordType) {
    };

protected:
    virtual Optional<s7nodavePlcDataType> getPlcDataType(PlcAddress plcAddress, Optional<s7nodavePlcDataType> suggestion) override {
        auto defaultType = S7nodaveInputRecord::getPlcDataType(plcAddress, suggestion);
        return BinarySupport::getPlcDataType(plcAddress, suggestion, defaultType);
    };

    virtual DBLINK getDeviceAddress() const override {
        biRecord *biRec = reinterpret_cast<biRecord *>(this->record);
        return biRec->inp;
    };

    virtual long writeToRecord(void *buffer, int bufferSize) override {
        BinarySupport::read<biRecord>(this->myAsynUser, this->record, buffer, bufferSize, this->recordAddress->getPlcDataType());
        return RECORD_STATUS_OK;
    };
};

} // namespace s7nodave

#endif // S7nodaveBiRecord_h
