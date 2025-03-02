#ifndef S7nodaveAoRecord_h
#define S7nodaveAoRecord_h

#include <aoRecord.h>

#include "AnalogSupport.h"
#include "S7nodaveOutputRecord.h"
#include "s7nodave.h"
#include "s7nodaveAsyn.h"

namespace s7nodave {

/**
 * Device support for ao record.
 */
class S7nodaveAoRecord : public S7nodaveOutputRecord {
public:
    /**
     * Constructor. The passed record pointer is stored and used by all methods,
     * which need to access record fields.
     */
    S7nodaveAoRecord(dbCommon *record) :
        S7nodaveOutputRecord(record, aoRecordType) {
        // Conversion parameters
        this->deviceLowValue = 0;
        this->deviceHighValue = 0;
    };

    virtual long initRecord() override {
        long status = S7nodaveOutputRecord::initRecord();
        if (status == RECORD_STATUS_OK) {
            AnalogSupport::initDeviceLimits(*this->recordAddress, this->deviceLowValue, this->deviceHighValue);
            this->convertRecord(1);
            // If the PLC is using the float data-type, we do not want the
            // VAL field of this record to be overwritten with a value
            // calculated using the RVAL field. Instead we use the VAL field
            // directly. We signal this to the record's init_record routine by
            // returning the special value RECORD_STATUS_OK_NO_CONVERT instead
            // of RECORD_STATUS_OK.
            // As initRecord was successful, this->recordAddress should be
            // initialized and thus we can safely access it here.
            if (this->recordAddress->getPlcDataType() == plcDataTypeFloat) {
                return RECORD_STATUS_OK_NO_CONVERT;
            } else {
                return RECORD_STATUS_OK;
            }
        } else {
            return status;
        }
    };

    virtual long convertRecord(int pass) override {
        if (pass == 0) {
            return RECORD_STATUS_OK;
        }
        return AnalogSupport::convert<aoRecord>(this->record, this->deviceLowValue, this->deviceHighValue);
    };

protected:
    /**
     * Low device value. This raw value is equivalent to a engineering-units
     * value of EGUL.
     */
    double deviceLowValue;

    /**
     * High device value. This raw value is equivalent to a engineering-units
     * value of EGUF.
     */
    double deviceHighValue;

    virtual void extractDeviceParameters(S7nodaveRecordAddress::DeviceParameters& deviceParameters) override {
        // Let super classes extract their parameters
        S7nodaveRecord::extractDeviceParameters(deviceParameters);

        // Extract deviceLowValue (DLV) and deviceHighValue(DHV) parameters,
        // if they have been given in the address string.
        AnalogSupport::extractDeviceParameters(this->myAsynUser, this->record, deviceParameters, this->deviceLowValue, this->deviceHighValue);
    }

    virtual Optional<s7nodavePlcDataType> getPlcDataType(PlcAddress plcAddress, Optional<s7nodavePlcDataType> suggestion) override {
        auto defaultType = S7nodaveOutputRecord::getPlcDataType(plcAddress, suggestion);
        return AnalogSupport::getPlcDataType(plcAddress, suggestion, defaultType);
    };

    virtual DBLINK getDeviceAddress() const override {
        aoRecord *aoRec = reinterpret_cast<aoRecord *>(this->record);
        return aoRec->out;
    };

    virtual void readFromRecord(void *buffer, int bufferSize) const override {
        AnalogSupport::write<aoRecord>(this->myAsynUser, this->record, buffer, bufferSize, this->recordAddress->getPlcDataType());
    };

    virtual long writeToRecord(void *buffer, int bufferSize) override {
        return AnalogSupport::read<aoRecord>(this->myAsynUser, this->record, buffer, bufferSize, this->recordAddress->getPlcDataType());
    };
};

} // namespace s7nodave

#endif // S7nodaveAoRecord_h
