#ifndef S7nodaveInputRecord_h
#define S7nodaveInputRecord_h

#include "PollGroup.h"
#include "S7nodaveRecord.h"

namespace s7nodave {

/**
 * Base class of all device support classes for input records.
 */
class S7nodaveInputRecord : public S7nodaveRecord {
protected:
    /**
     * Constructor. The record is stored in an attribute of this class and
     * is used by all methods which want to access record fields. The
     * recordType must correspond to the class inheriting from this base class
     * and calling the constructor.
     */
    S7nodaveInputRecord(dbCommon *record, s7nodaveRecordType recordType) :
        S7nodaveRecord(record, recordType) {
        this->pollGroupName = "default";
        this->pollRequester.record = this;
    };

    virtual void asynProcessCallback() override;

    virtual long initRecord() override;

    virtual long processRecord() override;

    virtual long getIoIntInfoRecord(int command, IOSCANPVT *iopvt) override;

    virtual void extractDeviceParameters(S7nodaveRecordAddress::DeviceParameters& deviceParameters) override;

    /**
     * Registers a read request when the record is part of a poll group. Calls
     * the requestRead method of the passed pollService in order to read the
     * memory area corresponding to this record's value.
     */
    virtual void preparePollRequest(PollService& pollService);

    /**
     * Called by poll group support when a request previously queued by the
     * preparePollRequest method has been sent and the response can now be
     * processed. Will also be called when the request failed, but with
     * requestSucceeded set to false.
     * Similar to the asynProcessCallback method, this method will store the
     * returned data in the newData... fields of this class and queue a process
     * request for this record.
     */
    virtual void processPollResponse(bool requestSucceeded, int bufferSize, void *buffer);

    /**
     * Name of the poll group of this record. If not configured explicitly, this
     * will be the string "default".
     */
    std::string pollGroupName;

    /**
     * Needed in order to queue a request for the record to be processed by the
     * scan engines I/O interrupt process thread.
     */
    IOSCANPVT ioScanPvt;

    /**
     * Class implementing the interface provided by the poll group component.
     * This implementation just delegates calls to the preparePollRequest and
     * processPollReponse methods of the enclosing class.
     */
    class PollRequesterImpl : public PollRequester {
    public:
        S7nodaveInputRecord *record;
        virtual void prepareRequest(PollService& pollService) override {
            this->record->preparePollRequest(pollService);
        };

        virtual void processResponse(bool requestSucceeded, int bufferSize, void *buffer) override {
            this->record->processPollResponse(requestSucceeded, bufferSize, buffer);
        };
    };

    /**
     * Instance of the PollRequester class.
     */
    PollRequesterImpl pollRequester;
};

} // namespace s7nodave

#endif  // S7nodaveInputRecord_h
