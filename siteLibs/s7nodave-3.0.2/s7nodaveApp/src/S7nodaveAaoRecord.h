#ifndef S7nodaveAaoRecord_h
#define S7nodaveAaoRecord_h

#include <aaoRecord.h>
#include <cantProceed.h>
// There is a bug in dbAccessDefs.h (EPICS R3.14.12): dbAccessDefs.h depends on
// dbBase.h but does not include it. Therefore we have to include dbBase.h
// before dbAccessDefs.h
#include <dbBase.h>
#include <dbAccessDefs.h>
#include <dbFldTypes.h>

#include "ArraySupport.h"
#include "S7nodaveOutputRecord.h"
#include "s7nodave.h"
#include "s7nodaveAsyn.h"

namespace s7nodave {

/**
 * Device support for aao record.
 */
class S7nodaveAaoRecord : public S7nodaveOutputRecord {
public:
    /**
     * Constructor. The passed record pointer is stored and used by all methods,
     * which need to access record fields.
     */
    S7nodaveAaoRecord(dbCommon *record) :
        S7nodaveOutputRecord(record, aaoRecordType) {
    };

protected:

    virtual long initRecord() override {
        // In contrast to the waveform record, the aao record does not allocate
        // the memory for BPTR before calling the device support's init method.
        // It will only check after device support initialization whether BPTR
        // has been initialized by the device support and allocate memory if
        // not.
        aaoRecord *aaoRec = reinterpret_cast<aaoRecord *>(this->record);
        if (!aaoRec->bptr) {
            // Please note that we never free the memory allocated here.
            // However, the record routine itself does the same thing and as
            // there is no method for removing records from the IOC during
            // run-time, this is probably okay.
            aaoRec->bptr = callocMustSucceed(aaoRec->nelm, dbValueSize(aaoRec->ftvl), "aao: buffer calloc failed");
        }
        return S7nodaveOutputRecord::initRecord();
    };

    virtual Optional<s7nodavePlcDataType> getPlcDataType(PlcAddress plcAddress, Optional<s7nodavePlcDataType> suggestion) override {
        aaoRecord *aaoRec = reinterpret_cast<aaoRecord *>(this->record);
        return ArraySupport::getPlcDataTypeOut(plcAddress, suggestion, aaoRec->ftvl);
    };

    virtual DBLINK getDeviceAddress() const override {
        aaoRecord *aaoRec = reinterpret_cast<aaoRecord *>(this->record);
        return aaoRec->out;
    };

    virtual void readFromRecord(void *buffer, int bufferSize) const override {
        ArraySupport::write<aaoRecord>(this->myAsynUser, this->record, buffer, bufferSize, this->recordAddress->getPlcDataType());
    };

    virtual unsigned long int getIoBufferSizeInBits() const override {
        aaoRecord *aaoRec = reinterpret_cast<aaoRecord *>(this->record);
        if (aaoRec->ftvl == DBF_STRING) {
            // String is special because each element takes 40 bytes, while
            // the corresponding PLC type is only one byte.
            return MAX_STRING_SIZE * aaoRec->nelm * S7nodaveOutputRecord::getIoBufferSizeInBits();
        } else {
            // For all other data-types, we can just multiply by the number of
            // elements.
            return aaoRec->nelm * S7nodaveOutputRecord::getIoBufferSizeInBits();
        }
    };

    virtual long writeToRecord(void *buffer, int bufferSize) override {
        return ArraySupport::read<aaoRecord>(this->myAsynUser, this->record, buffer, bufferSize, this->recordAddress->getPlcDataType());
    };
};

} // namespace s7nodave

#endif // S7nodaveAaoRecord_h
