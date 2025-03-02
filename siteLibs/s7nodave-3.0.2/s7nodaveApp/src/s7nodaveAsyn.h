#ifndef s7nodaveAsyn_h
#define s7nodaveAsyn_h

#include <asynDriver.h>
#include <dbCommon.h>
#include <dbScan.h>

/*
 * String used to identify the driver interface, when registering the interface
 * or looking for it.
 */
#define asynS7nodaveType "asynS7nodave"

// This header file is referenced from C and C++ source files. Therefore we have
// to mark the commonly used declarations and definitions as 'extern "C"'.
#ifdef __cplusplus
extern "C" {
#endif

/**
 * Identfies the various areas of the PLC memory. The area is the first part
 * of the device address string (e.g. "I" for input or "DB" for data blocks).
 */
typedef enum s7nodavePlcArea {
    plcAreaDb,
    plcAreaFlags,
    plcAreaInputs,
    plcAreaOutputs,
    plcAreaTimer,
    plcAreaCounter
} s7nodavePlcArea;

/**
 * Identifies the size of an element in the PLC memory. This is usally the
 * second part of the device address string (e.g. "B" for byte or "W" for word).
 */
typedef enum s7nodavePlcDataSize {
    plcDataSizeBit,
    plcDataSizeByte,
    plcDataSizeWord,
    plcDataSizeDword
} s7nodavePlcDataSize;

/**
 * Identifies the data type used by the PLC. Bools are special because they only
 * use a single bit. The size of the integer types correspond to their names.
 * Floats are always 32 bit.
 */
typedef enum s7nodavePlcDataType {
    plcDataTypeBool,
    plcDataTypeInt8,
    plcDataTypeUint8,
    plcDataTypeInt16,
    plcDataTypeUint16,
    plcDataTypeInt32,
    plcDataTypeUint32,
    plcDataTypeFloat
} s7nodavePlcDataType;

/**
 * Structure storing a request to read an item from the PLC memory.
 */
typedef struct asynS7nodaveReadItem {
    /**
     * Memory area type to read from.
     */
    s7nodavePlcArea area;
    /**
     * Number of the memory area. Must be zero unless are is plcAreaDb.
     */
    int areaNumber;
    /**
     * First byte to read from the memory area.
     */
    int startByte;
    /**
     * Number of bytes to read from the memory area.
     */
    int readBytes;
    /**
     * Buffer to store read data in. Must have at least readBytes bytes.
     */
    void *buffer;
    /**
     * Set to the number of bytes read after the read function finished.
     */
    int bytesRead;
    /**
     * Status code associated with the result of the read process.
     */
    asynStatus status;
    /**
     * Next item to read or NULL if this is the last item in the list.
     */
    struct asynS7nodaveReadItem *next;
} asynS7nodaveReadItem;

/**
 * Interface used by device support to communicate with the driver.
 */
typedef struct asynS7nodave {
    /**
     * Reads an item from the PLC memory. The number of bytes read is returned
     * in the bytesRead variable. The driver might split the item into several
     * requests if it is too big for a single request.
     */
    asynStatus (*plcRead)(void *drvPvt, asynUser *pasynUser, s7nodavePlcArea area, int areaNumber, int startByte, int readBytes, void *buffer, int *bytesRead);

    /**
     * Reads multiple items from the PLC at once. The result of the read
     * operation is returned in the passed asynS7nodaveReadItem structure.
     * The items might be split into several requests if they are too big
     * for being processed in a single request.
     */
    asynStatus (*plcReadMultipleItems)(void *drvPvt, asynUser *pasynUser, asynS7nodaveReadItem *firstItem);

    /**
     * Writes an item to the PLC. The number of bytes written is returned in
     * the bytesWritten variable. The driver might split the item into several
     * requests if it is too big for a single request.
     */
    asynStatus (*plcWrite)(void *drvPvt, asynUser *pasynUser, s7nodavePlcArea area, int areaNumber, int startByte, int writeLength, void *buffer, int *bytesWritten);

    /**
     * Writes to a single bit in the PLC. Other bits of the byte will not be
     * modified. Sets the bit to one if bitVal is not zero and to zero if bitVal
     * is zero.
     */
    asynStatus (*plcWriteBit)(void *drvPvt, asynUser *pasynUser, s7nodavePlcArea area, int areaNumber, int startByte, int startBit, unsigned char bitValue);
} asynS7nodave;

/**
 * Record types supported by device support.
 */
typedef enum s7nodaveRecordType {
    aaiRecordType,
    aaoRecordType,
    aiRecordType,
    aoRecordType,
    biRecordType,
    boRecordType,
    longinRecordType,
    longoutRecordType,
    mbbiRecordType,
    mbboRecordType,
    mbbiDirectRecordType,
    mbboDirectRecordType,
    stringinRecordType,
    stringoutRecordType,
    waveformInRecordType,
    waveformOutRecordType
} s7nodaveRecordType;

/**
 * Called from iocsh in order to create a port running the s7nodave driver.
 * The portName must be unique within the IOC and identifies the connection.
 * The plcHostname must be a numeric IP address or host name that can be
 * resolved. They can be followed by an optional port number (separated by a
 * colon). If not specified, the TCP port 102 is used. The rack and slot number
 * depend on the PLC configuration. In most cases it is rack 0, slot 0, but some
 * configurations might have rack 0, slot 2 or (even rarer) other settings.
 * The priority parameter specifies the priority of the port thread. If zero,
 * a priority of epicsThreadPriorityMedium is used.
 */
void s7nodaveConfigureIsoTcpPort(const char *portName, const char* plcHostname, const int plcRack, const int plcSlot, const unsigned int priority);

/**
 * Called from the corresponding functions in s7nodaveRecordDefinitions.c in
 * order to initialize the device support for a record. Creates an instance of
 * the class corresponding to recordType (S7nodave...Record) and calls its
 * initRecord method. The class's instance is stored in the DPVT field of the
 * record.
 */
long s7nodaveInitRecord(dbCommon *record, s7nodaveRecordType recordType);

/**
 * Device support function listed in the device support structure and called
 * when a record is processed. Delegates to the S7nodaveRecord::processRecord
 * method.
 */
long s7nodaveProcessRecord(dbCommon *record);

/**
 * Device support function listed in the device support structure of records
 * that support value conversion. Called by the record to initialize the
 * records conversion fields. Delegates to the S7nodaveRecord::convertRecord
 * method.
 */
long s7nodaveConvertRecord(dbCommon *record, int pass);


/**
 * Device support function listed in the device support structure of records
 * that support scan mode "I/O Intr". Called when scan mode is changed to or
 * from "I/O Intr". Delegates to S7nodaveRecord::getIoIntInfoRecord method.
 */
long s7nodaveGetIoIntInfoRecord(int command, dbCommon *record, IOSCANPVT *iopvt);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // s7nodaveAsyn_h
