/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/**
 * @author mrk
 */
/* Marty Kraimer 2011.03 */
/* This connects to a V3 DB record and calls CA to monitor data.
 * Only numeric scalar, timeStamp, and alarm are provided.
 */

#ifndef CAMONITOR_H
#define CAMONITOR_H

#include <epicsTime.h>

#include <pv/pvIntrospect.h>
#include <pv/requester.h>
#include <pv/lock.h>

enum CaType {
    CaEnum,
    CaByte,
    CaUByte,
    CaShort,
    CaUShort,
    CaInt,
    CaUInt,
    CaFloat,
    CaDouble,
    CaString
};

struct CaData {
    CaData();
    ~CaData();
    /* The following have new values after each data event*/
    union { //only used for scalar values
        epics::pvData::int8   byteValue;
        epics::pvData::uint8  ubyteValue;
        epics::pvData::uint16 ushortValue;
        epics::pvData::int16  shortValue;
        epics::pvData::int32  intValue;
        epics::pvData::uint32 uintValue;
        float                 floatValue;
        double                doubleValue;
    };
    epicsTimeStamp  timeStamp;
    int             sevr;
    int             stat;
    std::string     status;
};

class CaMonitorRequester : public virtual epics::pvData::Requester {
public:
    POINTER_DEFINITIONS(CaMonitorRequester);
    virtual ~CaMonitorRequester(){}
    virtual void exceptionCallback(long status,long op) = 0;
    virtual void connectionCallback() = 0;
    virtual void accessRightsCallback() = 0;
    virtual void eventCallback(const char *status) = 0;
};

typedef std::tr1::shared_ptr<CaMonitorRequester> CaMonitorRequesterPtr;

class CaMonitor : private epics::pvData::NoDefaultMethods {
public:
    CaMonitor(
        CaMonitorRequesterPtr const &requester,
        std::string const &pvName,
        CaType caType);
    ~CaMonitor();
    CaData & getData();
    void connect();
    void start();
    void stop();
    const char * getStatusString(long status);
    bool hasReadAccess();
    bool hasWriteAccess();
    bool isConnected();
private:
    class CaMonitorPvt *pImpl;
};

#endif  /* CAMONITOR_H */
