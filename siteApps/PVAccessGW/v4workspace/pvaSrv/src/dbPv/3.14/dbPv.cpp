/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/**
 * @author mrk
 */

#include <string>
#include <stdexcept>
#include <memory>

#include <dbFldTypes.h>
#include <dbDefs.h>

#include <pv/pvIntrospect.h>
#include <pv/pvData.h>
#include <pv/noDefaultMethods.h>
#include <pv/standardField.h>

#include "dbPv.h"

using namespace epics::pvData;
using namespace epics::pvAccess;
using std::string;

namespace epics { namespace pvaSrv { 

static int dbPvDebugLevel = 0;
void DbPvDebug::setLevel(int level)
{
    dbPvDebugLevel = level;
}

int DbPvDebug::getLevel()
{
    return dbPvDebugLevel;
}

DbPv::DbPv(
    DbPvProviderPtr const &provider,
    ChannelRequester::shared_pointer const & requester,
    string const &name,
    std::tr1::shared_ptr<DbAddr> dbAddr
)
:  provider(provider),
   requester(requester),
   name(name),
   dbAddr(dbAddr),
   recordField()
{
//printf("dbPv::dbPv\n");
}

void DbPv::init()
{
    // this requires valid existance of dbPv::shared_pointer instance
    StandardFieldPtr standardField = getStandardField();
    ScalarType scalarType = pvBoolean;
    DbAddr *paddr = dbAddr.get();
    switch(paddr->field_type) {
        case DBF_CHAR:
            scalarType = pvByte; break;
        case DBF_UCHAR:
            scalarType = pvUByte; break;
        case DBF_SHORT:
            scalarType = pvShort; break;
        case DBF_USHORT:
            scalarType = pvUShort; break;
        case DBF_LONG:
            scalarType = pvInt; break;
        case DBF_ULONG:
            scalarType = pvUInt; break;
        case DBF_FLOAT:
            scalarType = pvFloat; break;
        case DBF_DOUBLE:
            scalarType = pvDouble; break;
        case DBF_STRING:
            scalarType = pvString; break;

        case DBF_ENUM:
        case DBF_MENU:
        case DBF_DEVICE:
            recordField = standardField->enumerated("value,timeStamp,alarm");
            return;
        case DBF_INLINK:
        case DBF_OUTLINK:
        case DBF_FWDLINK:
            scalarType = pvString; break;
        default:
          break;
    }
    if(scalarType!=pvBoolean) {
        bool isArray = (paddr->no_elements>1) ? true : false;
        if(isArray) {
            recordField = standardField->scalarArray(scalarType,
                "value,timeStamp,alarm,display");
        } else {
            recordField = standardField->scalar(scalarType,
                "value,timeStamp,alarm,display,control");
        }
    }
}

DbPv::~DbPv()
{
//printf("dbPv::~dbPv\n");
}

void DbPv::getField(GetFieldRequester::shared_pointer const &requester,
        string const &subField)
{
    if(recordField) {
        requester->getDone(Status::Ok,recordField);
        return;
    }
    Status status(Status::STATUSTYPE_ERROR,"client asked for illegal V3 field");
    requester->getDone(status,FieldConstPtr());
}

ChannelProcess::shared_pointer DbPv::createChannelProcess(
        ChannelProcessRequester::shared_pointer const & channelProcessRequester,
        PVStructure::shared_pointer const & pvRequest)
{
    DbPvProcess::shared_pointer dbPvProcess(
          new DbPvProcess(getPtrSelf(),channelProcessRequester,*(dbAddr.get())));
    dbPvProcess->init(pvRequest);
    return dbPvProcess;
}

ChannelGet::shared_pointer DbPv::createChannelGet(
        ChannelGetRequester::shared_pointer const &channelGetRequester,
        PVStructure::shared_pointer const &pvRequest)
{
    DbPvGet::shared_pointer dbPvGet(
        new DbPvGet(getPtrSelf(),channelGetRequester,*(dbAddr.get())));
    if(!dbPvGet->init(pvRequest)) {
        Status createFailed(Status::STATUSTYPE_ERROR, "create dbPvGet failed");
        channelGetRequester->channelGetConnect(
            createFailed,
            dbPvGet,
            nullStructure);
    }
    return dbPvGet;
}

ChannelPut::shared_pointer DbPv::createChannelPut(
        ChannelPutRequester::shared_pointer const &channelPutRequester,
        PVStructure::shared_pointer const &pvRequest)
{
    DbPvPut::shared_pointer dbPvPut(
          new DbPvPut(getPtrSelf(),channelPutRequester,*(dbAddr.get())));
    if(!dbPvPut->init(pvRequest)) {
        Status createFailed(Status::STATUSTYPE_ERROR, "create dbPvPut failed");
        channelPutRequester->channelPutConnect(
            createFailed,
            dbPvPut,
            nullStructure);
    }
    return dbPvPut;
}

Monitor::shared_pointer DbPv::createMonitor(
        MonitorRequester::shared_pointer const &monitorRequester,
        PVStructure::shared_pointer const &pvRequest)
{
    DbPvMonitor::shared_pointer dbPvMonitor(
         new DbPvMonitor(getPtrSelf(),monitorRequester,*(dbAddr.get())));
    if(!dbPvMonitor->init(pvRequest)) {
        Status createFailed(Status::STATUSTYPE_ERROR, "create dbPvMonitor failed");
        StructureConstPtr xxx;
        monitorRequester->monitorConnect(
            createFailed,
            dbPvMonitor,
            xxx);
    }
    return dbPvMonitor;
}

ChannelArray::shared_pointer DbPv::createChannelArray(
        ChannelArrayRequester::shared_pointer const &channelArrayRequester,
        PVStructure::shared_pointer const &pvRequest)
{
    DbPvArray::shared_pointer dbPvArray(
         new DbPvArray(getPtrSelf(),channelArrayRequester,*(dbAddr.get())));
    if(!dbPvArray->init(pvRequest)) {
        Status createFailed(Status::STATUSTYPE_ERROR, "create dbPvArray failed");
        ScalarArrayConstPtr xxx;
        channelArrayRequester->channelArrayConnect(
            createFailed,
            dbPvArray,
            xxx);
    }
    return dbPvArray;
}

void DbPv::printInfo()
{
    printInfo(std::cout);
}

void DbPv::printInfo(std::ostream& out)
{
    out << "dbPv provides access to DB records";
}

}}
