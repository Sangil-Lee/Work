#ifndef S7nodaveAiRecord_h
#define S7nodaveAiRecord_h

#include <aiRecord.h>

#include "AnalogSupport.h"
#include "S7nodaveInputRecord.h"
#include "s7nodave.h"
#include "s7nodaveAsyn.h"

namespace s7nodave {

/**
 * Device support for ai record.
 */
class S7nodaveAiRecord : public S7nodaveInputRecord {
public:
    /**
     * Constructor. The passed record pointer is stored and used by all methods,
     * which need to access record fields.
     */
    S7nodaveAiRecord(dbCommon *record) :
        S7nodaveInputRecord(record, aiRecordType) {
        // Conversion parameters
        this->deviceLowValue = 0;
        this->deviceHighValue = 0;
    };

    virtual long initRecord() override {
        long status = S7nodaveInputRecord::initRecord();
        if (status == RECORD_STATUS_OK) {
            // For an analog record, we have to initialize the device limits,
            // so that conversion will work.
            AnalogSupport::initDeviceLimits(*this->recordAddress, this->deviceLowValue, this->deviceHighValue);
            // By calling convertRecord(1), we make sure that the record fields
            // used for conversion are initialized correctly.
            this->convertRecord(1);
        }
        return status;
    };

    virtual long convertRecord(int pass) override {
        if (pass == 0) {
            return RECORD_STATUS_OK;
        }
        return AnalogSupport::convert<aiRecord>(this->record, this->deviceLowValue, this->deviceHighValue);
    };

    virtual long processRecord() override {
        long status = S7nodaveInputRecord::processRecord();
        if (status == RECORD_STATUS_OK) {
            // If the PLC is using the float data-type, we do not want the
            // VAL field of this record to be overwritten with a value
            // calculated using the RVAL field. Instead we use the VAL field
            // directly. We signal this to the record's readValue routine by
            // returning the special value RECORD_STATUS_OK_NO_CONVERT instead
            // of RECORD_STATUS_OK.
            if (this->recordAddress->getPlcDataType() == plcDataTypeFloat) {
                return RECORD_STATUS_OK_NO_CONVERT;
            } else {
                return RECORD_STATUS_OK;
            }
        } else {
            return status;
        }
    };

protected:
    /*
     * Low device value. This raw value is equivalent to a engineering-units
     * value of EGUL.
     */
    double deviceLowValue;

    /*
     * High device value. This raw value is equivalent to a engineering-units
     * value of EGUF.
     */
    double deviceHighValue;

    virtual void extractDeviceParameters(S7nodaveRecordAddress::DeviceParameters& deviceParameters) override {
        // Let super classes extract their parameters
        S7nodaveInputRecord::extractDeviceParameters(deviceParameters);

        // Extract deviceLowValue (DLV) and deviceHighValue(DHV) parameters,
        // if they have been given in the address string.
        AnalogSupport::extractDeviceParameters(this->myAsynUser, this->record, deviceParameters, this->deviceLowValue, this->deviceHighValue);
    }

    virtual Optional<s7nodavePlcDataType> getPlcDataType(PlcAddress plcAddress, Optional<s7nodavePlcDataType> suggestion) override {
        auto defaultType = S7nodaveInputRecord::getPlcDataType(plcAddress, suggestion);
        return AnalogSupport::getPlcDataType(plcAddress, suggestion, defaultType);
    };

    virtual DBLINK getDeviceAddress() const override {
        aiRecord *aiRec = reinterpret_cast<aiRecord *>(this->record);
        return aiRec->inp;
    };

    virtual long writeToRecord(void *buffer, int bufferSize) override {
        return AnalogSupport::read<aiRecord>(this->myAsynUser, this->record, buffer, bufferSize, this->recordAddress->getPlcDataType());
    };
};

} // namespace s7nodave

#endif // S7nodaveAiRecord_h
