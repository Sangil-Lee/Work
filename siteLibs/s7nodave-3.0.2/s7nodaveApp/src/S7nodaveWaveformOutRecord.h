#ifndef S7nodaveWaveformOutRecord_h
#define S7nodaveWaveformOutRecord_h

#include <dbFldTypes.h>
#include <waveformRecord.h>

#include "ArraySupport.h"
#include "S7nodaveOutputRecord.h"
#include "s7nodave.h"
#include "s7nodaveAsyn.h"

namespace s7nodave {

/**
 * Device support for waveform record, input direction. The waveform record
 * type is special, because it can be used as an input or output record.
 * The two different device supports are implemented by using a different
 * value in the DTYP field (s7nodaveWfIn and s7nodaveWfOut respectively).
 */
class S7nodaveWaveformOutRecord : public S7nodaveOutputRecord {
public:
    /**
     * Constructor. The passed record pointer is stored and used by all methods,
     * which need to access record fields.
     */
    S7nodaveWaveformOutRecord(dbCommon *record) :
        S7nodaveOutputRecord(record, waveformOutRecordType) {
    };

protected:

    virtual Optional<s7nodavePlcDataType> getPlcDataType(PlcAddress plcAddress, Optional<s7nodavePlcDataType> suggestion) override {
        waveformRecord *wfRec = reinterpret_cast<waveformRecord *>(this->record);
        return ArraySupport::getPlcDataTypeOut(plcAddress, suggestion, wfRec->ftvl);
   };

    virtual DBLINK getDeviceAddress() const override {
        waveformRecord *wfRec = reinterpret_cast<waveformRecord *>(this->record);
        return wfRec->inp;
    };

    virtual void readFromRecord(void *buffer, int bufferSize) const override {
        ArraySupport::write<waveformRecord>(this->myAsynUser, this->record, buffer, bufferSize, this->recordAddress->getPlcDataType());
    };

    virtual unsigned long int getIoBufferSizeInBits() const override {
        waveformRecord *wfRec = reinterpret_cast<waveformRecord *>(this->record);
        if (wfRec->ftvl == DBF_STRING) {
            // String is special because each element takes 40 bytes, while
            // the corresponding PLC type is only one byte.
            return MAX_STRING_SIZE * wfRec->nelm * S7nodaveOutputRecord::getIoBufferSizeInBits();
        } else {
            return wfRec->nelm * S7nodaveOutputRecord::getIoBufferSizeInBits();
        }
        return wfRec->nelm * S7nodaveOutputRecord::getIoBufferSizeInBits();
    };

    virtual long writeToRecord(void *buffer, int bufferSize) override {
        return ArraySupport::read<waveformRecord>(this->myAsynUser, this->record, buffer, bufferSize, this->recordAddress->getPlcDataType());
    };
};

} // namespace s7nodave

#endif // S7nodaveWaveformOutRecord_h
