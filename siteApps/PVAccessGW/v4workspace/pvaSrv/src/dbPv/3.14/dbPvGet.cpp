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

#include <pv/pvData.h>
#include <pv/pvAccess.h>

#include "dbPv.h"
#include "dbUtil.h"

using namespace epics::pvData;
using namespace epics::pvAccess;
using std::string;

namespace epics { namespace pvaSrv { 

DbPvGet::DbPvGet(
    DbPvPtr const &dbPv,
    ChannelGetRequester::shared_pointer const &channelGetRequester,
    DbAddr &dbAddr)
: dbUtil(DbUtil::getDbUtil()),
  dbPv(dbPv),
  channelGetRequester(channelGetRequester),
  dbAddr(dbAddr),
  process(false),
  block(false),
  firstTime(true),
  propertyMask(0),
  beingDestroyed(false)
{
    if(DbPvDebug::getLevel()>0)printf("dbPvGet::dbPvGet\n");
}

DbPvGet::~DbPvGet()
{
    if(DbPvDebug::getLevel()>0)printf("dbPvGet::~dbPvGet\n");
}

bool DbPvGet::init(PVStructure::shared_pointer const &pvRequest)
{
    propertyMask = dbUtil->getProperties(
        channelGetRequester,
        pvRequest,
        dbAddr,
        false);
    if(propertyMask==dbUtil->noAccessBit) return false;
    pvStructure = PVStructure::shared_pointer(
                dbUtil->createPVStructure(
                    channelGetRequester,
                    propertyMask,
                    dbAddr));
    if (!pvStructure.get()) return false;
    dbUtil->getPropertyData(channelGetRequester,propertyMask,dbAddr,pvStructure);
    int numFields = pvStructure->getNumberFields();
    bitSet.reset(new BitSet(numFields));
    if (propertyMask&dbUtil->processBit) {
        process = true;
        pNotify.reset(new (struct putNotify)());
        notifyAddr.reset(new DbAddr());
        memcpy(notifyAddr.get(), &dbAddr, sizeof(DbAddr));
        DbAddr *paddr = notifyAddr.get();
        struct dbCommon *precord = paddr->precord;
        char buffer[sizeof(precord->name) + 10];
        strcpy(buffer,precord->name);
        strcat(buffer,".PROC");
        if (dbNameToAddr(buffer, paddr)) {
            throw std::logic_error("dbNameToAddr failed");
        }
        struct putNotify *pn = pNotify.get();
        pn->userCallback = this->notifyCallback;
        pn->paddr = paddr;
        pn->nRequest = 1;
        pn->dbrType = DBR_CHAR;
        pn->usrPvt = this;
        if (propertyMask & dbUtil->blockBit) block = true;
    }
    channelGetRequester->channelGetConnect(
                Status::Ok,
                getPtrSelf(),
                pvStructure->getStructure());
    return true;
}

string DbPvGet::getRequesterName() {
    return channelGetRequester->getRequesterName();
}

void DbPvGet::message(string const &message,MessageType messageType)
{
    channelGetRequester->message(message,messageType);
}

void DbPvGet::destroy() {
    if(DbPvDebug::getLevel()>0) printf("dbPvGet::destroy beingDestroyed %s\n",
         (beingDestroyed ? "true" : "false"));
    {
        Lock xx(mutex);
        if(beingDestroyed) return;
        beingDestroyed = true;
        if (pNotify) dbNotifyCancel(pNotify.get());
    }
}

void DbPvGet::get()
{
    if (block && process) {
        epicsUInt8 value = 1;
        pNotify.get()->pbuffer = &value;
        dbPutNotify(pNotify.get());
    } else {
        dbScanLock(dbAddr.precord);
        if (process) dbProcess(dbAddr.precord);

        Lock lock(dataMutex);
        bitSet->clear();
        Status status = dbUtil->get(
                    channelGetRequester,
                    propertyMask,
                    dbAddr,
                    pvStructure,
                    bitSet,
                    0);
        dbScanUnlock(dbAddr.precord);
        if (firstTime) {
            firstTime = false;
            bitSet->clear();
            bitSet->set(0);
        }
        lock.unlock();
        channelGetRequester->getDone(
            status,
            getPtrSelf(),
            pvStructure,
            bitSet);
    }
}

void DbPvGet::notifyCallback(struct putNotify *pn)
{
    DbPvGet * pdp = static_cast<DbPvGet *>(pn->usrPvt);

    dbScanLock(pdp->dbAddr.precord);
    Lock lock(pdp->dataMutex);
    pdp->bitSet->clear();
    Status status = pdp->dbUtil->get(
                pdp->channelGetRequester,
                pdp->propertyMask,
                pdp->dbAddr,
                pdp->pvStructure,
                pdp->bitSet,
                0);
    dbScanUnlock(pdp->dbAddr.precord);
    if (pdp->firstTime) {
        pdp->firstTime = false;
        pdp->bitSet->clear();
        pdp->bitSet->set(0);
    }
    lock.unlock();
    pdp->channelGetRequester->getDone(
                status,
                pdp->getPtrSelf(),
                pdp->pvStructure,
                pdp->bitSet);
}

void DbPvGet::lock()
{
    dataMutex.lock();
}

void DbPvGet::unlock()
{
    dataMutex.unlock();
}

}}
