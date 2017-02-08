/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/**
 * @author mrk
 */
/* Marty Kraimer 2011.03 */
/* This connects to a DB record and presents the data as a PVStructure
 * It provides access to  value, alarm, display, and control.
 */

#include <cstddef>
#include <cstdlib>
#include <cstddef>
#include <string>
#include <cstdio>

#include <cadef.h>
#include <db_access.h>
#include <dbDefs.h>
#include <alarmString.h>

#include <pv/caStatus.h>

#include "dbPvDebug.h"
#include "caMonitor.h"
#include "caContext.h"

using namespace epics::pvData;
using namespace epics::pvaSrv;
using std::string;
using epics::pvAccess::ca::dbrStatus2alarmMessage;
using epics::pvAccess::ca::dbrStatus2alarmStatus;

CaData::CaData()
: doubleValue(0.0),
  sevr(0), stat(0), status("")
{}

CaData::~CaData()
{}

class CaMonitorPvt {
public:
    CaMonitorPvt(CaMonitorRequesterPtr const &requester,
        string pvName,CaType caType);
    ~CaMonitorPvt();
    CaData & getData();
    void connect();
    void start();
    void stop();

    CaMonitorRequesterPtr requester;
    string pvName;
    CaType caType;
    CaData data;
    chanId chid;
    evid myevid;
    caContextPtr context;
};

extern "C" {

static void connectionCallback(struct connection_handler_args args)
{
    if(DbPvDebug::getLevel()>0) printf("connectionCallback\n");
    CaMonitorPvt *pvt = static_cast<CaMonitorPvt *>(ca_puser(args.chid));
    pvt->requester->connectionCallback();
}

static void accessRightsCallback(struct access_rights_handler_args args)
{
    CaMonitorPvt *pvt = static_cast<CaMonitorPvt *>(ca_puser(args.chid));
    pvt->requester->accessRightsCallback();
}

static void eventCallback(struct event_handler_args eha)
{
    if(DbPvDebug::getLevel()>0) printf("eventCallback\n");
    CaMonitorPvt *pvt = static_cast<CaMonitorPvt *>(ca_puser(eha.chid));
    if(eha.status!=ECA_NORMAL) {
        pvt->requester->eventCallback(ca_message(eha.status));
        return;
    }
    switch(pvt->caType) {
        case CaEnum: {
            const struct dbr_time_enum *from =
                static_cast<const struct dbr_time_enum*>(eha.dbr);
            pvt->data.sevr = from->severity;
            pvt->data.stat = dbrStatus2alarmStatus[from->status];
            pvt->data.status = dbrStatus2alarmMessage[from->status];
            pvt->data.intValue = from->value;
            pvt->data.timeStamp = from->stamp;
            break;
        }
        case CaByte: {
            const struct dbr_time_char *from =
                static_cast<const struct dbr_time_char*>(eha.dbr);
            pvt->data.sevr = from->severity;
            pvt->data.stat = dbrStatus2alarmStatus[from->status];
            pvt->data.status = dbrStatus2alarmMessage[from->status];
            pvt->data.byteValue = from->value;
            pvt->data.timeStamp = from->stamp;
            break;
        }
        case CaUByte: {
            const struct dbr_time_char *from =
                static_cast<const struct dbr_time_char*>(eha.dbr);
            pvt->data.sevr = from->severity;
            pvt->data.stat = dbrStatus2alarmStatus[from->status];
            pvt->data.status = dbrStatus2alarmMessage[from->status];
            pvt->data.ubyteValue = static_cast<uint8>(from->value);
            pvt->data.timeStamp = from->stamp;
            break;
        }
        case CaShort: {
            const struct dbr_time_short *from =
                static_cast<const struct dbr_time_short*>(eha.dbr);
            pvt->data.sevr = from->severity;
            pvt->data.stat = dbrStatus2alarmStatus[from->status];
            pvt->data.status = dbrStatus2alarmMessage[from->status];
            pvt->data.shortValue = from->value;
            pvt->data.timeStamp = from->stamp;
            break;
        }
        case CaUShort: {
            const struct dbr_time_short *from =
                static_cast<const struct dbr_time_short*>(eha.dbr);
            pvt->data.sevr = from->severity;
            pvt->data.stat = dbrStatus2alarmStatus[from->status];
            pvt->data.status = dbrStatus2alarmMessage[from->status];
            pvt->data.ushortValue = static_cast<uint16>(from->value);
            pvt->data.timeStamp = from->stamp;
            break;
        }
        case CaInt: {
            const struct dbr_time_long *from =
                static_cast<const struct dbr_time_long*>(eha.dbr);
            pvt->data.sevr = from->severity;
            pvt->data.stat = dbrStatus2alarmStatus[from->status];
            pvt->data.status = dbrStatus2alarmMessage[from->status];
            pvt->data.intValue = from->value;
            pvt->data.timeStamp = from->stamp;
            break;
        }
        case CaUInt: {
            const struct dbr_time_long *from =
                static_cast<const struct dbr_time_long*>(eha.dbr);
            pvt->data.sevr = from->severity;
            pvt->data.stat = dbrStatus2alarmStatus[from->status];
            pvt->data.status = dbrStatus2alarmMessage[from->status];
            pvt->data.uintValue = static_cast<uint32>(from->value);
            pvt->data.timeStamp = from->stamp;
            break;
        }
        case CaFloat: {
            const struct dbr_time_float *from =
                static_cast<const struct dbr_time_float*>(eha.dbr);
            pvt->data.sevr = from->severity;
            pvt->data.stat = dbrStatus2alarmStatus[from->status];
            pvt->data.status = dbrStatus2alarmMessage[from->status];
            pvt->data.floatValue = from->value;
            pvt->data.timeStamp = from->stamp;
            break;
        }
        case CaDouble: {
            const struct dbr_time_double *from =
                static_cast<const struct dbr_time_double*>(eha.dbr);
            pvt->data.sevr = from->severity;
            pvt->data.stat = dbrStatus2alarmStatus[from->status];
            pvt->data.status = dbrStatus2alarmMessage[from->status];
            pvt->data.doubleValue = from->value;
            pvt->data.timeStamp = from->stamp;
            break;
        }
        case CaString: {
            const struct dbr_time_string *from =
                static_cast<const struct dbr_time_string*>(eha.dbr);
            pvt->data.sevr = from->severity;
            pvt->data.stat = dbrStatus2alarmStatus[from->status];
            pvt->data.status = dbrStatus2alarmMessage[from->status];
            pvt->data.timeStamp = from->stamp;
            // client will get value from record
            break;
        }
    }
    if(DbPvDebug::getLevel()>0) printf("eventCallback calling requester->eventCallback\n");
    pvt->requester->eventCallback(0);
    if(DbPvDebug::getLevel()>0) printf("eventCallback after calling requester->eventCallback\n");
}

} //extern "C"

CaMonitorPvt::CaMonitorPvt(
    CaMonitorRequesterPtr const &requester,
    string pvName, CaType caType)
: requester(requester), pvName(pvName), caType(caType),
  data(), chid(0), myevid(0), context(caContextCreate::get(requester))
{
    if(DbPvDebug::getLevel()>0) printf("caMonitorPvt::caMonitorPvt\n");
}

CaMonitorPvt::~CaMonitorPvt()
{
    if(DbPvDebug::getLevel()>0) printf("caMonitorPvt::~caMonitorPvt\n");
    if(chid!=0) {
        context->checkContext();
        ca_clear_channel(chid);
        chid = 0;
    }
    context->release();
}

void CaMonitorPvt::connect()
{
    if(DbPvDebug::getLevel()>0) printf("caMonitorPvt::connect\n");
    int status = 0;
    context->checkContext();
    status = ca_create_channel(
        pvName.c_str(),connectionCallback,this,20,&chid);
    if(status!=ECA_NORMAL) {
        requester->message("ca_create_channel failed",errorMessage);
        if(chid!=0) ca_clear_channel(chid);
        return;
    }
    status = ca_replace_access_rights_event(chid,accessRightsCallback);
    if(status!=ECA_NORMAL) {
        requester->message("ca_replace_access_rights_event failed",warningMessage);
    }
}

void CaMonitorPvt::start()
{
    if(DbPvDebug::getLevel()>0) printf("caMonitorPvt::start\n");
    chtype catype = DBR_STRING;
    switch(caType) {
        case CaEnum: catype = DBR_TIME_ENUM; break;
        case CaByte: catype = DBR_TIME_CHAR; break;
        case CaUByte: catype = DBR_TIME_CHAR; break;
        case CaShort: catype = DBR_TIME_SHORT; break;
        case CaUShort: catype = DBR_TIME_SHORT; break;
        case CaInt: catype = DBR_TIME_LONG; break;
        case CaUInt: catype = DBR_TIME_LONG; break;
        case CaFloat: catype = DBR_TIME_FLOAT; break;
        case CaDouble: catype = DBR_TIME_DOUBLE; break;
        case CaString: catype = DBR_TIME_STRING; break;
    }
    context->checkContext();
    int status = ca_create_subscription(
        catype, 1, chid, DBE_VALUE|DBE_ALARM,
        eventCallback, this, &myevid);
    if(status!=ECA_NORMAL) {
        requester->message("ca_create_subscription failed",warningMessage);
    }
}

void CaMonitorPvt::stop()
{
    if(DbPvDebug::getLevel()>0) printf("caMonitorPvt::stop\n");
    context->checkContext();
    ca_clear_subscription(myevid);
}

CaMonitor::CaMonitor(
    CaMonitorRequesterPtr const &requester,
    string const &pvName,CaType caType)
: pImpl(new CaMonitorPvt(requester, pvName, caType))
{
    if(DbPvDebug::getLevel()>0) printf("caMonitor::caMonitor\n");
}

CaMonitor::~CaMonitor()
{
    if(DbPvDebug::getLevel()>0) printf("caMonitor::~caMonitor\n");
    delete pImpl;
}

CaData & CaMonitor::getData()
{
    return pImpl->data;
}

void CaMonitor::connect()
{
    pImpl->connect();
}

void CaMonitor::start()
{
    pImpl->start();
}

void CaMonitor::stop()
{
    pImpl->stop();
}

const char *CaMonitor::getStatusString(long status)
{
    return ca_message(status);
}

bool CaMonitor::hasReadAccess()
{
    if(pImpl->chid==0) return false;
    if(ca_read_access(pImpl->chid)) return true;
    return false;
}

bool CaMonitor::hasWriteAccess()
{
    if(pImpl->chid==0) return false;
    if(ca_write_access(pImpl->chid)) return true;
    return false;
}

bool CaMonitor::isConnected()
{
    if(pImpl->chid==0) return false;
    enum channel_state state = ca_state(pImpl->chid);
    if(state==cs_conn) return true;
    return false;
}
