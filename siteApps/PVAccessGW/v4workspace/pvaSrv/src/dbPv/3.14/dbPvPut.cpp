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

#include <dbAccess.h>
#include <dbEvent.h>
#include <dbNotify.h>
#include <dbCommon.h>

#include <pv/pvIntrospect.h>
#include <pv/pvData.h>
#include <pv/pvAccess.h>

#include "dbPv.h"
#include "dbUtil.h"

using namespace epics::pvData;
using namespace epics::pvAccess;
using std::string;

namespace epics { namespace pvaSrv { 

DbPvPut::DbPvPut(
    DbPvPtr const &dbPv,
    ChannelPutRequester::shared_pointer const &channelPutRequester,
    DbAddr &dbAddr)
: dbUtil(DbUtil::getDbUtil()),
  dbPv(dbPv),
  channelPutRequester(channelPutRequester),
  dbAddr(dbAddr),
  propertyMask(0),
  process(false),
  block(false),
  firstTime(true),
  beingDestroyed(false)
{
    if(DbPvDebug::getLevel()>0) printf("dbPvPut::dbPvPut()\n");
}

DbPvPut::~DbPvPut()
{
    if(DbPvDebug::getLevel()>0) printf("dbPvPut::~dbPvPut()\n");
}

bool DbPvPut::init(PVStructure::shared_pointer const &pvRequest)
{
    propertyMask = dbUtil->getProperties(
        channelPutRequester,
        pvRequest,
        dbAddr,
        true);
    if(propertyMask==dbUtil->noAccessBit) return false;
    if(propertyMask==dbUtil->noModBit) {
        channelPutRequester->message(
             "field not allowed to be changed",errorMessage);
        return 0;
    }
    pvStructure = PVStructure::shared_pointer(
        dbUtil->createPVStructure(
            channelPutRequester,
            propertyMask,
            dbAddr));
    if (!pvStructure.get()) return false;
    if (propertyMask & dbUtil->dbPutBit) {
        if (propertyMask & dbUtil->processBit) {
            channelPutRequester->message(
                        "process determined by dbPutField", errorMessage);
        }
    } else if (propertyMask&dbUtil->processBit) {
        process = true;
        pNotify.reset(new (struct putNotify)());
        notifyAddr.reset(new DbAddr());
        memcpy(notifyAddr.get(), &dbAddr, sizeof(DbAddr));
        DbAddr *paddr = notifyAddr.get();
        struct dbCommon *precord = paddr->precord;
        char buffer[sizeof(precord->name) + 10];
        strcpy(buffer, precord->name);
        strcat(buffer, ".PROC");
        if (dbNameToAddr(buffer, paddr)) {
            throw std::logic_error("dbNameToAddr failed");
        }
        struct putNotify *pn = pNotify.get();
        pn->userCallback = this->notifyCallback;
        pn->paddr        = paddr;
        pn->nRequest     = 1;
        pn->dbrType      = DBR_CHAR;
        pn->usrPvt       = this;
        if (propertyMask & dbUtil->blockBit) block = true;
    }
    int numFields = pvStructure->getNumberFields();
    bitSet.reset(new BitSet(numFields));
    channelPutRequester->channelPutConnect(
       Status::Ok,
       getPtrSelf(),
       pvStructure->getStructure());
    return true;
}

string DbPvPut::getRequesterName() {
    return channelPutRequester->getRequesterName();
}

void DbPvPut::message(string const &message,MessageType messageType)
{
    channelPutRequester->message(message,messageType);
}

void DbPvPut::destroy() {
    if(DbPvDebug::getLevel()>0) printf("dbPvPut::destroy beingDestroyed %s\n",
         (beingDestroyed ? "true" : "false"));
    {
        Lock xx(mutex);
        if (beingDestroyed) return;
        beingDestroyed = true;
        if (pNotify) dbNotifyCancel(pNotify.get());
    }
}

void DbPvPut::put(PVStructurePtr const &pvStructure, BitSetPtr const & bitSet)
{
    if(DbPvDebug::getLevel()>0) printf("dbPvPut::put()\n");
    Lock lock(dataMutex);
    this->pvStructure = pvStructure;
    this->bitSet = bitSet;
    PVFieldPtr pvField = pvStructure.get()->getPVFields()[0];
    if(propertyMask&dbUtil->dbPutBit) {
        Status status = dbUtil->putField(
            channelPutRequester,propertyMask,dbAddr,pvField);
        lock.unlock();
        channelPutRequester->putDone(status,getPtrSelf());
        return;
    }
    dbScanLock(dbAddr.precord);
    Status status = dbUtil->put(
                channelPutRequester, propertyMask, dbAddr, pvField);
    if (process && !block) dbProcess(dbAddr.precord);
    dbScanUnlock(dbAddr.precord);
    lock.unlock();
    if (block && process) {
        epicsUInt8 value = 1;
        pNotify.get()->pbuffer = &value;
        dbPutNotify(pNotify.get());
    } else {
        channelPutRequester->putDone(status, getPtrSelf());
    }
}

void DbPvPut::notifyCallback(struct putNotify *pn)
{
    DbPvPut * pdp = static_cast<DbPvPut *>(pn->usrPvt);
    pdp->channelPutRequester->putDone(Status::Ok, pdp->getPtrSelf());
}

void DbPvPut::get()
{
    if(DbPvDebug::getLevel()>0) printf("dbPvPut::get()\n");
    {
    Lock lock(dataMutex);
    bitSet->clear();
    dbScanLock(dbAddr.precord);
    Status status = dbUtil->get(
        channelPutRequester,
        propertyMask,dbAddr,
        pvStructure,
        bitSet,
        0);
    if(firstTime) {
        firstTime = false;
        bitSet->set(pvStructure->getFieldOffset());
    }
    dbScanUnlock(dbAddr.precord);
    }
    channelPutRequester->getDone(
        Status::Ok,
        getPtrSelf(),
        pvStructure,
        bitSet);
}

void DbPvPut::lock()
{
    dataMutex.lock();
}

void DbPvPut::unlock()
{
    dataMutex.unlock();
}

}}
