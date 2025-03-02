#ifndef S7nodaveMbboDirectRecord_h
#define S7nodaveMbboDirectRecord_h

#include <mbboDirectRecord.h>

#include "MultiBinarySupport.h"
#include "S7nodaveOutputRecord.h"
#include "s7nodave.h"
#include "s7nodaveAsyn.h"

namespace s7nodave {

/**
 * Device support for mbboDirect record.
 */
class S7nodaveMbboDirectRecord : public S7nodaveOutputRecord {
public:
    /**
     * Constructor. The passed record pointer is stored and used by all methods,
     * which need to access record fields.
     */
    S7nodaveMbboDirectRecord(dbCommon *record) :
        S7nodaveOutputRecord(record, mbboDirectRecordType) {

    };

    virtual long initRecord() override {
        long status = S7nodaveOutputRecord::initRecord();
        if (status != RECORD_STATUS_OK) {
            return status;
        }

        mbboDirectRecord *r = reinterpret_cast<mbboDirectRecord *>(this->record);
        epicsUInt32 rval = r->rval;
        if (r->shft > 0) {
            rval >>= r->shft;
        }
        if (r->mask != 0) {
            rval &= r->mask;
        }
        epicsUInt8 *bits[] = {&r->b0, &r->b1, &r->b2, &r->b3, &r->b4, &r->b5, &r->b6, &r->b7, &r->b8, &r->b9, &r->ba, &r->bb, &r->bc, &r->bd, &r->be, &r->bf};
        for (int i = 0; i < 16; i++) {
            if ((rval & (0x01 << i)) != 0) {
                *bits[i] = 1;
            } else {
                *bits[i] = 0;
            }
        }

        return status;
    }

protected:
    virtual long interceptInitRecordBeforeValueInit() override {
        MultiBinarySupport::initMask<mbboDirectRecord>(this->record, this->recordAddress->getPlcAddress().getDataSize());
        return RECORD_STATUS_OK;
    };

    virtual Optional<s7nodavePlcDataType> getPlcDataType(PlcAddress plcAddress, Optional<s7nodavePlcDataType> suggestion) override {
        auto defaultType = S7nodaveOutputRecord::getPlcDataType(plcAddress, suggestion);
        return MultiBinarySupport::getPlcDataType(plcAddress, suggestion, defaultType);
    };

    virtual DBLINK getDeviceAddress() const override {
        mbboDirectRecord *mbboDRec = reinterpret_cast<mbboDirectRecord *>(this->record);
        return mbboDRec->out;
    };

    virtual void readFromRecord(void *buffer, int bufferSize) const override {
        MultiBinarySupport::write<mbboDirectRecord>(this->myAsynUser, this->record, buffer, bufferSize, this->recordAddress->getPlcDataType());
    };

    virtual long writeToRecord(void *buffer, int bufferSize) override {
        MultiBinarySupport::read<mbboDirectRecord>(this->myAsynUser, this->record, buffer, bufferSize, this->recordAddress->getPlcDataType());
        return RECORD_STATUS_OK;
    };
};

} // namespace s7nodave

#endif // S7nodaveMbboDirectRecord_h
