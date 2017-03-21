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
#include <stdexcept>
#include <memory>

#include <epicsThread.h>

#include <dbAccess.h>
#include <dbNotify.h>
#include <dbCommon.h>

#include <pv/pvData.h>
#include <pv/convert.h>
#include <pv/monitor.h>
#include <pv/pvAccess.h>

#define epicsExportSharedSymbols

#include <epicsExport.h>
#include "dbPv.h"
#include "caContext.h"
#include "caMonitor.h"
#include "dbUtil.h"

using namespace epics::pvData;
using namespace epics::pvAccess;
using std::tr1::dynamic_pointer_cast;
using namespace std;

namespace epics { namespace pvaSrv { 

static ConvertPtr convert = getConvert();

DbPvMonitor::DbPvMonitor(
    DbPvPtr const &dbPv,
    MonitorRequester::shared_pointer const &monitorRequester,
    DbAddr &dbAddr)
: dbUtil(DbUtil::getDbUtil()),
  dbPv(dbPv),
  monitorRequester(monitorRequester),
  dbAddr(dbAddr),
  event(),
  propertyMask(0),
  firstTime(true),
  gotEvent(false),
  caType(CaByte),
  queueSize(2),
  caMonitor(),
  numberFree(queueSize),
  numberUsed(0),
  nextGetFree(0),
  nextGetUsed(0),
  nextReleaseUsed(0),
  beingDestroyed(false),
  isStarted(false)
{
    if(DbPvDebug::getLevel()>0) printf("dbPvMonitor::dbPvMonitor\n");
}

DbPvMonitor::~DbPvMonitor() {
    if(DbPvDebug::getLevel()>0) printf("dbPvMonitor::~dbPvMonitor\n");
}

bool DbPvMonitor::init(
    PVStructure::shared_pointer const &pvRequest)
{
    string queueSizeString("record._options.queueSize");
    {
        PVStringPtr pvString = pvRequest->getSubField<PVString>(queueSizeString);
        if(pvString) {
             string value = pvString->get();
             queueSize = atoi(value.c_str());
             numberFree = queueSize;
        }
    }
    propertyMask = dbUtil->getProperties(
        monitorRequester,
        pvRequest,
        dbAddr,
        false);
    if(propertyMask==dbUtil->noAccessBit) return false;
    if(propertyMask&dbUtil->isLinkBit) {
        monitorRequester->message("can not monitor a link field",errorMessage);
        return 0;
    }
    elements.reserve(queueSize);
    for(int i=0; i<queueSize; i++) {
        PVStructurePtr pvStructure(dbUtil->createPVStructure(
                monitorRequester,
                propertyMask,
                dbAddr));
        if(!pvStructure) return false;
        MonitorElementPtr element(new MonitorElement(pvStructure));
        elements.push_back(element);
    }
    MonitorElementPtr element = elements[0];
    StructureConstPtr saveStructure = element->pvStructurePtr->getStructure();
    if((propertyMask&dbUtil->enumValueBit)!=0) {
        caType = CaEnum;
    } else {
        ScalarType scalarType = dbUtil->getScalarType(
            monitorRequester,
            dbAddr);
        switch(scalarType) {
        case pvByte: caType = CaByte; break;
        case pvUByte: caType = CaUByte; break;
        case pvShort: caType = CaShort; break;
        case pvUShort: caType = CaUShort; break;
        case pvInt: caType = CaInt; break;
        case pvUInt: caType = CaUInt; break;
        case pvFloat: caType = CaFloat; break;
        case pvDouble: caType = CaDouble; break;
        case pvString: caType = CaString; break;
        default:
            throw std::logic_error("bad scalarType");
        }
    }
    string pvName = dbPv->getChannelName();
    caMonitor.reset(
        new CaMonitor(getPtrSelf(), pvName, caType));
    caMonitor->connect();
    event.wait();
    Monitor::shared_pointer thisPointer = dynamic_pointer_cast<Monitor>(getPtrSelf());
    monitorRequester->monitorConnect(
       Status::Ok,
       thisPointer,
       saveStructure);
    return true;
}

string DbPvMonitor::getRequesterName() {
    return monitorRequester.get()->getRequesterName();
}

void DbPvMonitor::message(string const &message,MessageType messageType)
{
    monitorRequester->message(message,messageType);
}

void DbPvMonitor::destroy() {
    if(DbPvDebug::getLevel()>0) printf("dbPvMonitor::destroy beingDestroyed %s\n",
         (beingDestroyed ? "true" : "false"));
    {
        Lock xx(mutex);
        if(beingDestroyed) return;
        beingDestroyed = true;
    }
    stop();
    caMonitor.reset();
    dbPv.reset();
}

Status DbPvMonitor::start()
{
    if(DbPvDebug::getLevel()>0) printf("dbPvMonitor::start\n");
    {
        Lock xx(mutex);
        if(beingDestroyed) {
             Status status(Status::STATUSTYPE_ERROR,"beingDestroyed");
             return status;
        }
        if(isStarted) return Status::Ok;
        isStarted = true;
        firstTime = true;
    }
    if(!currentElement) {
        currentElement = getFree();
    }
    if(!currentElement) {
        printf("dbPvMonitor::start will throw\n");
        throw std::logic_error(
            "dbPvMonitor::start no free queue element");
    }

    caMonitor.get()->start();
    return Status::Ok;
}

Status DbPvMonitor::stop()
{
    {
        Lock xx(mutex);
        if(!isStarted) return Status::Ok;
        isStarted = false;
    }
    if(DbPvDebug::getLevel()>0) printf("dbPvMonitor::stop\n");
    caMonitor->stop();
    return Status::Ok;
}

MonitorElementPtr DbPvMonitor::poll()
{
    if (DbPvDebug::getLevel() > 0) printf("dbPvMonitor::poll\n");
    if (beingDestroyed) return nullElement;
    Lock xx(mutex);
    if (numberUsed == 0) return nullElement;
    int ind = nextGetUsed;
    nextGetUsed++;
    if (nextGetUsed >= queueSize) nextGetUsed = 0;
    return elements[ind];
}

void DbPvMonitor::release(MonitorElementPtr const & element)
{
    if (DbPvDebug::getLevel() > 0) printf("dbPvMonitor::release\n");
    if (beingDestroyed) return;
    Lock xx(mutex);
    if (element != elements[nextReleaseUsed++]) {
        throw std::logic_error(
                    "not queueElement returned by last call to getUsed");
    }
    if (nextReleaseUsed >= queueSize) nextReleaseUsed = 0;
    numberUsed--;
    numberFree++;
}

void DbPvMonitor::exceptionCallback(long status,long op)
{}

void DbPvMonitor::connectionCallback()
{
    if(DbPvDebug::getLevel()>0) printf("dbPvMonitor::connectionCallback\n");
    event.signal();
}

void DbPvMonitor::accessRightsCallback()
{}

void DbPvMonitor::eventCallback(const char *status)
{
    if(DbPvDebug::getLevel()>0) printf("dbPvMonitor::eventCallback\n");
    if(beingDestroyed) return;
    if(status!=0) {
         monitorRequester->message(status,errorMessage);
    }
    MonitorElementPtr nextElement = nullElement;
    PVStructure::shared_pointer pvStructure = currentElement->pvStructurePtr;
    BitSet::shared_pointer bitSet = currentElement->changedBitSet;
    dbScanLock(dbAddr.precord);
    CaData &caData = caMonitor.get()->getData();
    BitSet::shared_pointer overrunBitSet = currentElement->overrunBitSet;
    Status stat = dbUtil->get(
       monitorRequester,
       propertyMask,
       dbAddr,
       pvStructure,
       overrunBitSet,
       &caData);

    if(firstTime) {
        firstTime = false;
        bitSet->clear();
        overrunBitSet->clear();
        bitSet->set(0);
    } else {
        int index = overrunBitSet->nextSetBit(0);
        while(index>=0) {
            bool wasSet = bitSet->get(index);
            if(!wasSet) {
                bitSet->set(index);
                overrunBitSet->clear(index);
            }
            index = overrunBitSet->nextSetBit(index+1);
        }
    }
    
    if(bitSet->nextSetBit(0)>=0) {
        nextElement =getFree();
        if(nextElement) {
            PVStructure::shared_pointer pvNext = nextElement->pvStructurePtr;
            convert->copy(pvStructure,pvNext);
            nextElement->changedBitSet->clear();
            nextElement->overrunBitSet->clear();
        }
    }
    dbScanUnlock(dbAddr.precord);
    if(bitSet->nextSetBit(0)<0) return;
    if(!nextElement) return;
    numberUsed++;
    currentElement = nextElement;
    monitorRequester->monitorEvent(getPtrSelf());
}

void DbPvMonitor::lock()
{}

void DbPvMonitor::unlock()
{}

MonitorElementPtr &DbPvMonitor::getFree()
{
    if(numberFree==0) return nullElement;
    numberFree--;
    int ind = nextGetFree;
    nextGetFree++;
    if(nextGetFree>=queueSize) nextGetFree = 0;
    return elements[ind];

}

}}
