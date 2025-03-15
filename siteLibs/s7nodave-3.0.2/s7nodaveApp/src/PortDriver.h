#ifndef s7nodave_PortDriver_h
#define s7nodave_PortDriver_h

#include <string>

#include <asynDriver.h>
#include <epicsMutex.h>

#include "s7_micro_client.h"
#include "s7nodaveAsyn.h"

namespace s7nodave {

/**
 * Provides access to a PLC through an asyn port.
 */
class PortDriver {
public:
    /**
     * Constructor. Called by s7nodaveConfigureIsoTcpPort to create a driver for
     * a port.
     */
    PortDriver(const char *portName, const char *plcHostname, const int plcPort, const int plcRack, const int plcSlot, const unsigned int priority);

    /**
     * Destructor.
     */
    ~PortDriver();

    /**
     * Must be called after construction in order to register the port with the
     * asyn manager. This is separated from construction because it is not
     * guaranteed to succeed.
     */
    asynStatus registerPortDriver();

private:
    /**
     * TRUE if registerPortDriver has been called.
     */
    bool registered;

    /**
     * Name of the port
     */
    std::string portName;

    /**
     * Host name or IP address of PLC (without port number).
     */
    std::string plcHostname;

    /**
     * TCP port number.
     */
    int plcPort;

    /**
     * PLC rack number (needed for connection).
     */
    int plcRack;

    /**
     * PLC slot number (needed for connection).
     */
    int plcSlot;

    /**
     * Priority of the port thread.
     */
    unsigned int priority;

    /**
     * Mutex for synchronizing connect/disconnect requests.
     */
    epicsMutex connectMutex;

    /**
     * Snap7 client object.
     */
    TSnap7MicroClient client;

    /**
     * Tells whether the port is currently connected.
     */
    bool connected;

    /**
     * asynCommon structure registered with the asyn manager.
     */
    asynCommon myAsynCommon;

    /**
     * Interface for the asynCommon structure registered with the asyn manager.
     */
    asynInterface myAsynCommonInterface;

    /**
     * asynS7nodave (driver-specific) structure registered with the asyn
     * manager.
     */
    asynS7nodave myAsynS7nodave;

    /**
     * Interface for the asynS7nodave (driver-specific) structure registered
     * with the asyn manager.
     */
    asynInterface myAsynS7nodaveInterface;


    static void asynReportStatic(void *drvPvt, FILE *fp, int details);

    /**
     * Called by asyn to request a report about the driver status.
     */
    void asynReport(FILE *fp, int details);

    static asynStatus asynConnectStatic(void *drvPvt, asynUser *pasynUser);

    /**
     * Called by asyn to connect the port.
     */
    asynStatus asynConnect(asynUser *pasynUser);

    static asynStatus asynDisconnectStatic(void *drvPvt, asynUser *pasynUser);

    /**
     * Called by asyn to disconnect the port.
     */
    asynStatus asynDisconnect(asynUser *pasynUser);

    static asynStatus asynPlcReadStatic(void *drvPvt, asynUser *pasynUser, s7nodavePlcArea area, int areaNumber, int startByte, int readLength, void *buffer, int *bytesRead);

    /**
     * Used by device support code to read from the PLC.
     */
    asynStatus asynPlcRead(asynUser *paysnUser, s7nodavePlcArea area, int areaNumber, int startByte, int readLength, void *buffer, int *bytesRead);

    static asynStatus asynPlcReadMultipleItemsStatic(void *drvPvt, asynUser *pasynUser, asynS7nodaveReadItem *firstItem);

    /**
     * Used by device support code to read multiple items from the PLC.
     */
    asynStatus asynPlcReadMultipleItems(asynUser *pasynUser, asynS7nodaveReadItem *firstItem);

    static asynStatus asynPlcWriteStatic(void *drvPvt, asynUser *pasynUser, s7nodavePlcArea area, int areaNumber, int startByte, int writeLength, void *buffer, int *bytesWritten);

    /**
     * Used by device support code to write to the PLC.
     */
    asynStatus asynPlcWrite(asynUser *pasynUser, s7nodavePlcArea area, int areaNumber, int startByte, int writeLength, void *buffer, int *bytesWritten);

    static asynStatus asynPlcWriteBitStatic(void *drvPvt, asynUser *pasynUser, s7nodavePlcArea area, int areaNumber, int startByte, int startBit, unsigned char bitValue);

    /**
     * Used by device support code to write a single bit to the PLC.
     */
    asynStatus asynPlcWriteBit(asynUser *pasynUser, s7nodavePlcArea area, int areaNumber, int startByte, int startBit, unsigned char bitValue);
};

} // namespace s7nodave

#endif // s7nodave_PortDriver_h
