#ifndef S7nodaveOutputRecord_h
#define S7nodaveOutputRecord_h

#include "S7nodaveRecord.h"

namespace s7nodave {

/**
 * Base class of all device support classes for output records.
 */
class S7nodaveOutputRecord : public S7nodaveRecord {
protected:
    /**
     * Constructor. The record is stored in an attribute of this class and
     * is used by all methods which want to access record fields. The
     * recordType must correspond to the class inheriting from this base class
     * and calling the constructor.
     */
    S7nodaveOutputRecord(dbCommon *record, s7nodaveRecordType recordType) :
        S7nodaveRecord(record, recordType) {
    };

    virtual long initRecord() override;

    virtual void asynProcessCallback() override;

    virtual long processRecord() override;

    /**
     * Reads the value from the record into the buffer. The buffer must have
     * the correct size (as returned by getIoBufferSize). This method must
     * be implemented by child classes in order to read the value from the
     * corresponding record field and write into the buffer that will then
     * be sent to the device.
     */
    virtual void readFromRecord(void *buffer, int bufferSize) const = 0;

    /**
     * Method that can be implemented by child classes, which want to run code
     * that executes AFTER configuration initialization but BEFORE the
     * current value is read from the hardware. The default implementation
     * just returns RECORD_STATUS_OK.
     */
    virtual long interceptInitRecordBeforeValueInit();
};

} // namespace s7nodave

#endif // S7nodaveOutputRecord_h
