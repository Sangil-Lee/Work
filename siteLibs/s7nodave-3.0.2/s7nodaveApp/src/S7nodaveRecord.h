#ifndef S7nodaveRecord_h
#define S7nodaveRecord_h

#include <string>

#include <asynDriver.h>
#include <callback.h>
#include <dbCommon.h>
#include <epicsMutex.h>

#include "Optional.h"
#include "PlcAddress.h"
#include "S7nodaveRecordAddress.h"
#include "s7nodaveAsyn.h"

namespace s7nodave {

/**
 * Base class for all device support classes for all record types.
 */
class S7nodaveRecord {
public:
    /**
     * Constructor. The record is stored in an attribute of this class and
     * is used by all methods which want to access record fields. The
     * recordType must correspond to the class inheriting from this base class
     * and calling the constructor.
     */
    S7nodaveRecord(dbCommon *record, s7nodaveRecordType recordType);

    /**
     * Destructor.
     */
    virtual ~S7nodaveRecord();

    /**
     * Called on record initialization in order to initialize the record
     * (e.g. create and connect asynUser, read initial value, etc.).
     */
    virtual long initRecord();

    /**
     * Called each time the record is processed. Used for reading (input
     * records) or writing (output records) data from or to the hardware. The
     * processRecord method might asynchronously by setting the record's
     * PACT field to 1 before returning and then later queuing a request
     * for processRecord to be run again.
     * This method has to be implemented by child classes in order to implement
     * the synchronous part of the read or write process.
     */
    virtual long processRecord() = 0;

    /**
     * For record types which support conversion between hardware (raw) and
     * engineering units, this method is called in order to avoid the record
     * fields used for conversion. Child classes which want to support
     * conversion have to implement this method.
     */
    virtual long convertRecord(int pass);

    /**
     * Called when a record's SCAN field is set to "I/O Intr" (command == 0)
     * or when it was set to "I/O Intr" and now has been changed to a different
     * setting (command == 1). Child classes which want to support interrupt
     * scan mode have to implement this method.
     */
    virtual long getIoIntInfoRecord(int command, IOSCANPVT *iopvt);

protected:
    /**
     * This method is called by initRecord() if an error has occurred during
     * record initialization and the initialization that has already been
     * performed (e.g. creating an asynUser) shall be cleaned up.
     */
    virtual long initRecordError();

    /**
     * Static wrapper around asynProcessCallback.
     */
    static void asynProcessCallbackStatic(asynUser *pasynUser);

    /**
     * Method registered with asyn for being called in the port thread's context
     * when a request has been queued using asynManager->queueRequest.
     * This method has to be implemented by child classes which want to perform
     * asynchronous I/O.
     */
    virtual void asynProcessCallback() = 0;

    /**
     * Returns the record field representing the device address. Usually, for
     * input records this field is named INP and for output records this field
     * is named OUT.
     */
    virtual DBLINK getDeviceAddress() const = 0;

    /**
     * Returns the PLC data-type that is used for communication with the device.
     * Child classes should overload this method, because the default
     * implementation simply makes suggestions based on the PLC item's data
     * size, ignoring the data-type that has been requested in the device
     * address string.
     */
    virtual Optional<s7nodavePlcDataType> getPlcDataType(PlcAddress plcAddress, Optional<s7nodavePlcDataType> suggestion);

    /**
     * Extracts device parameters from the device parameter map. The parameters,
     * that are understood must be removed from the map passed. This method
     * must be overloaded by child classes which support device parameters.
     * Otherwise, this class's initRecord method will complain if there any
     * entries left in the map.
     */
    virtual void extractDeviceParameters(S7nodaveRecordAddress::DeviceParameters& deviceParameters);

    /**
     * Determines the size of the buffer needed for a read or write request.
     * This can be used by a method wanting to call readFromPlc or writeToPlc
     * to allocate the buffer memory. This method is actually just a wrapper
     * around getIoBufferSizeInBits().
     */
    virtual int getIoBufferSizeInBytes() const;
    /**
     * Returns the size needed for the I/O buffer in bits. The default
     * implementation just returns the length of the configured PLC data type.
     * Subclasses, which want to read or write multiple elements from or to the
     * PLC have to override this method.
     */
    virtual unsigned long getIoBufferSizeInBits() const;

    /**
     * Converts between host and PLC byte order. The data type is needed because
     * the kind of byte rearrangement performed depends on the type and width
     * of the items.
     */
    static void convertFromOrToPlcByteOrder(void *buffer, int bufferSize, s7nodavePlcDataType plcDataType);

    /**
     * Array of bit masks. The array index equals the number of bits set
     * (starting with the lowest bit. This set of masks is very useful for
     * performing bit arithmetics.
     */
    static const unsigned char bitMasks[];

    /**
     * Actually reads from PLC. This method is mainly used by input records, but
     * also by output records for initialization. If called outside the driver's
     * port thread, asynManager->lockPort must be called before calling this
     * function.
     */
    virtual bool readFromPlc(void *buffer, int bufferSize) const;

    /**
     * Actually writes data to the PLC. This method is only used by output
     * records, but we still put it here for symmetry reasons and because some
     * functionality is common with readFromPlc. This method must only be called
     * from within the driver's port thread.
     */
    virtual bool writeToPlc(void *buffer, int bufferSize) const;

    /**
     * Read the value from the buffer into the record. The buffer must have the
     * correct size (as returned by getIoBufferSize).
     * This method must only be called while holding this record's mutex.
     * Usually this is assured by calling this method from within initRecord
     * or processRecord.
     */
    virtual long writeToRecord(void *buffer, int bufferSize) = 0;

    /**
     * Record this device support has been instantiated for.
     */
    dbCommon *record;

    /**
     * Type of the record.
     */
    s7nodaveRecordType recordType;

    /**
     * asyn device address. For the driver support we have so far, this is
     * always zero.
     */
    int deviceAddress;

    /**
     * Stored the record address (content of device address field).
     * The record address is not available on construction but does not have a
     * default constructor. Therefore we wrap it in an optional.
     */
    Optional<S7nodaveRecordAddress> recordAddress;

    /**
     * Mutex that must be locked before accessing the newData, newDataLength or
     * processCallbackError attributes.
     */
    epicsMutex newDataMutex;

    /**
     * Length of data in newData buffer.
     */
    size_t newDataLength;

    /**
     * Buffer containing new data or NULL if no new data is available.
     */
    void *newData;

    /**
     * Flag that is set to true, if an error occurred within
     * asynProcessCallback.
     */
    bool processCallbackError;

    /**
     * Callback needed to queue a request for processRecord to be run again.
     */
    CALLBACK processCallback;

    /**
     * asynUser structure. Needed for nearly any methods of asynManager.
     */
    asynUser *myAsynUser;

    /**
     * Interface for asynCommon.
     */
    asynInterface *myAsynCommonInterface;

    /**
     * Actual interface the common asyn driver functions.
     */
    asynCommon *myAsynCommon;

    /**
     * Interface for asynS7nodave.
     */
    asynInterface *myAsynS7nodaveInterface;

    /**
     * Actual interface for s7nodave-specific driver functions.
     */
    asynS7nodave *myAsynS7nodave;
};

} // namespace s7nodave

#endif // S7nodaveRecord_h
