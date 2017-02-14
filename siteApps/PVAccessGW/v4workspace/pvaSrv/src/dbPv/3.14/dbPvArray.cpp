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

#include <cantProceed.h>
#include <epicsStdio.h>
#include <epicsMutex.h>
#include <epicsEvent.h>
#include <epicsThread.h>
#include <epicsExit.h>
#include <dbAccess.h>
#include <dbCommon.h>
#include <recSup.h>
#include <dbBase.h>
#include <dbEvent.h>

#include <pv/noDefaultMethods.h>
#include <pv/pvIntrospect.h>
#include <pv/pvData.h>
#include <pv/pvAccess.h>
#include <pv/standardField.h>
#include <pv/standardPVField.h>
#include <pv/alarm.h>
#include <pv/control.h>
#include <pv/display.h>
#include <pv/timeStamp.h>
#include <pv/pvAlarm.h>
#include <pv/pvControl.h>
#include <pv/pvDisplay.h>
#include <pv/pvEnumerated.h>
#include <pv/pvTimeStamp.h>

#define epicsExportSharedSymbols
#include <epicsExport.h>
#include "dbPv.h"

using namespace epics::pvData;
using namespace epics::pvAccess;
using std::tr1::static_pointer_cast;
using std::cout;
using std::endl;
using std::string;

namespace epics { namespace pvaSrv { 

static PVArray::shared_pointer nullPVArray;
static Status illegalOffsetStatus(
    Status::STATUSTYPE_ERROR,
    "count must be >0"
);
static Status illegalCountStatus(
    Status::STATUSTYPE_ERROR,
    "count must be >0"
);
static Status illegalStrideStatus(
    Status::STATUSTYPE_ERROR,
    "stride must be >0"
);

extern "C" {
typedef long (*get_array_info) (DBADDR *,long *,long *);
typedef long (*put_array_info) (DBADDR *,long );
}

DbPvArray::DbPvArray(
    DbPvPtr const &dbPv,
    ChannelArrayRequester::shared_pointer const &channelArrayRequester,
    DbAddr &dbAddr)
: dbPv(dbPv),
  channelArrayRequester(channelArrayRequester),
  dbAddr(dbAddr),
  beingDestroyed(false)
{
    if(DbPvDebug::getLevel()>0)printf("dbPvArray::dbPvArray\n");
}

DbPvArray::~DbPvArray()
{
    if(DbPvDebug::getLevel()>0) printf("dbPvArray::~dbPvArray\n");
}

bool DbPvArray::init(PVStructure::shared_pointer const &pvRequest)
{
    if(!(dbAddr.no_elements>1)) {
        channelArrayRequester.get()->message("field in V3 record is not an array",errorMessage);
        return false;
    }
    ScalarType scalarType(pvBoolean);
    // Note that pvBoolean is not a supported type
    switch(dbAddr.field_type) {
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
    default:
      break;
    }
    if(scalarType==pvBoolean) {
        channelArrayRequester.get()->message("unsupported field in DB record",errorMessage);
        return false;
    }
    pvScalarArray = getPVDataCreate()->createPVScalarArray(scalarType);
    pvScalarArray->setCapacity(dbAddr.no_elements);
    channelArrayRequester->channelArrayConnect(
        Status::Ok,
        getPtrSelf(),
        pvScalarArray->getArray());
    return true;
}

void DbPvArray::destroy() {
    if(DbPvDebug::getLevel()>0) printf("dbPvArray::destroy beingDestroyed %s\n",
         (beingDestroyed ? "true" : "false"));
     {
        Lock xx(mutex);
        if(beingDestroyed) return;
        beingDestroyed = true;
    }
}

void DbPvArray::getArray(size_t offset,size_t count,size_t stride)
{
    if(offset<0) {
         channelArrayRequester->getArrayDone(illegalOffsetStatus,getPtrSelf(),nullPVArray);
         return;
    }
    if(stride<0) {
         channelArrayRequester->getArrayDone(illegalStrideStatus,getPtrSelf(),nullPVArray);
         return;
    }
    dbScanLock(dbAddr.precord);
    long v3length = 0;
    long v3offset = 0;
    struct rset *prset = dbGetRset(&dbAddr);
    if(prset && prset->get_array_info) {
        get_array_info get_info;
        get_info = (get_array_info)(prset->get_array_info);
        get_info(&dbAddr, &v3length, &v3offset);
        if(v3offset!=0) {
            dbScanUnlock(dbAddr.precord);
            Status status(Status::STATUSTYPE_ERROR,
                   "v3offset not supported");
            channelArrayRequester->getArrayDone(
                status,
                getPtrSelf(),
                nullPVArray);
            return;
        }
    }
    bool ok = false;
    while(true) {
        size_t length = v3length;
        if(length<=0) break;
        if(count<=0) {
             count = -offset + length/stride;
             if(count>0) ok = true;
             break;
        }
        size_t maxcount = -offset + length/stride;
        if(count>maxcount) count = maxcount;
        ok = true;
        break;
    }
    if(!ok) {
        dbScanUnlock(dbAddr.precord);
        pvScalarArray->setLength(0);
        channelArrayRequester->getArrayDone(
            Status::Ok,
            getPtrSelf(),
            pvScalarArray);
        return;
    }
    size_t capacity = pvScalarArray->getCapacity();
    {
    Lock lock(dataMutex);
    switch(dbAddr.field_type) {
    case DBF_CHAR: {
        shared_vector<int8> xxx(capacity);
        int8 *from = static_cast<int8 *>(dbAddr.pfield);
        for(size_t i= 0;i< count; ++i) xxx[i] = from[i*stride+offset];
        shared_vector<const int8> data(freeze(xxx));
        PVByteArrayPtr pva = static_pointer_cast<PVByteArray>(pvScalarArray);
        pva->replace(data);
        break;
   }
    case DBF_UCHAR: {
        shared_vector<uint8> xxx(count);
        uint8 *from = static_cast<uint8 *>(dbAddr.pfield);
        for(size_t i= 0;i< count; ++i) xxx[i] = from[i*stride+offset];
        shared_vector<const uint8> data(freeze(xxx));
        PVUByteArrayPtr pva = static_pointer_cast<PVUByteArray>(pvScalarArray);
        pva->replace(data);
        break;
    }
    case DBF_SHORT: {
        shared_vector<int16> xxx(count);
        int16 *from = static_cast<int16 *>(dbAddr.pfield);
        for(size_t i= 0;i< count; ++i) xxx[i] = from[i*stride+offset];
        shared_vector<const int16> data(freeze(xxx));
        PVShortArrayPtr pva = static_pointer_cast<PVShortArray>(pvScalarArray);
        pva->replace(data);
        break;
    }
    case DBF_USHORT: {
        shared_vector<uint16> xxx(count);
        uint16 *from = static_cast<uint16 *>(dbAddr.pfield);
        for(size_t i= 0;i< count; ++i) xxx[i] = from[i*stride+offset];
        shared_vector<const uint16> data(freeze(xxx));
        PVUShortArrayPtr pva = static_pointer_cast<PVUShortArray>(pvScalarArray);
        pva->replace(data);
        break;
    }
    case DBF_LONG: {
        shared_vector<int32> xxx(count);
        int32 *from = static_cast<int32 *>(dbAddr.pfield);
        for(size_t i= 0;i< count; ++i) xxx[i] = from[i*stride+offset];
        shared_vector<const int32> data(freeze(xxx));
        PVIntArrayPtr pva = static_pointer_cast<PVIntArray>(pvScalarArray);
        pva->replace(data);
        break;
    }
    case DBF_ULONG: {
        shared_vector<uint32> xxx(count);
        uint32 *from = static_cast<uint32 *>(dbAddr.pfield);
        for(size_t i= 0;i< count; ++i) xxx[i] = from[i*stride+offset];
        shared_vector<const uint32> data(freeze(xxx));
        PVUIntArrayPtr pva = static_pointer_cast<PVUIntArray>(pvScalarArray);
        pva->replace(data);
        break;
    }
    case DBF_FLOAT: {
        shared_vector<float> xxx(count);
        float *from = static_cast<float *>(dbAddr.pfield);
        for(size_t i= 0;i< count; ++i) xxx[i] = from[i*stride+offset];
        shared_vector<const float> data(freeze(xxx));
        PVFloatArrayPtr pva = static_pointer_cast<PVFloatArray>(pvScalarArray);
        pva->replace(data);
        break;
    }
    case DBF_DOUBLE: {
        shared_vector<double> xxx(count);
        double *from = static_cast<double *>(dbAddr.pfield);
        for(size_t i= 0;i< count; ++i) xxx[i] = from[i*stride+offset];
        shared_vector<const double> data(freeze(xxx));
        PVDoubleArrayPtr pva = static_pointer_cast<PVDoubleArray>(pvScalarArray);
        pva->replace(data);
        break;
    }
    case DBF_STRING: {
        shared_vector<string> xxx(count);
        char *from = static_cast<char *>(dbAddr.pfield);
        from += offset*dbAddr.field_size;
        for(size_t i= 0;i< count; ++i) {
             xxx[i] = from;
             from += stride*dbAddr.field_size;
        }
        shared_vector<const string> data(freeze(xxx));
        PVStringArrayPtr pva = static_pointer_cast<PVStringArray>(pvScalarArray);
        pva->replace(data);
        break;
    }
    }
    pvScalarArray->setLength(offset + count*stride);
    }
    dbScanUnlock(dbAddr.precord);
    channelArrayRequester->getArrayDone(
        Status::Ok,
        getPtrSelf(),
        pvScalarArray);
}

void DbPvArray::putArray(
    PVArray::shared_pointer const &pvArray,
    size_t offset, size_t count, size_t stride)
{
    if(offset<0) {
         channelArrayRequester->putArrayDone(illegalOffsetStatus,getPtrSelf());
         return;
    }
    if(count<0) {
         channelArrayRequester->putArrayDone(illegalCountStatus,getPtrSelf());
         return;
    }
    if(stride<0) {
         channelArrayRequester->putArrayDone(illegalStrideStatus,getPtrSelf());
         return;
    }
    dbScanLock(dbAddr.precord);
    long no_elements = dbAddr.no_elements;
    size_t length = no_elements;
    if((offset + count*stride)>length) {
        count = - offset + (length + stride -1)/stride; 
    }
    if(count<=0) {
        dbScanUnlock(dbAddr.precord);
        channelArrayRequester->putArrayDone(
            Status::Ok,
            getPtrSelf());
        return;
    }
    long newLength = offset + count*stride;
    struct rset *prset = dbGetRset(&dbAddr);
    if(prset && prset->get_array_info) {
        long oldLength = 0;
        long v3offset = 0;
        get_array_info get_info;
        get_info = (get_array_info)(prset->get_array_info);
        get_info(&dbAddr, &oldLength, &v3offset);
        if(newLength>oldLength) {
           if(prset && prset->put_array_info) {
               put_array_info put_info;
               put_info = (put_array_info)(prset->put_array_info);
               put_info(&dbAddr, newLength);
           }
        }
    }
    {
    Lock lock(dataMutex);
    switch(dbAddr.field_type) {
    case DBF_CHAR: {
        PVByteArrayPtr pva = static_pointer_cast<PVByteArray>(pvArray);
        shared_vector<const int8> xxx(pva->view());
        int8 *to = static_cast<int8 *>(dbAddr.pfield);
        for(size_t i= 0;i< count; i++) to[i*stride + offset] = xxx[i];
        break;
    }
    case DBF_UCHAR: {
        PVUByteArrayPtr pva = static_pointer_cast<PVUByteArray>(pvArray);
        shared_vector<const uint8> xxx(pva->view());
        uint8 *to = static_cast<uint8 *>(dbAddr.pfield);
        for(size_t i= 0;i< count; i++) to[i*stride + offset] = xxx[i];
        break;
    }
    case DBF_SHORT: {
        PVShortArrayPtr pva = static_pointer_cast<PVShortArray>(pvArray);
        shared_vector<const int16> xxx(pva->view());
        int16 *to = static_cast<int16 *>(dbAddr.pfield);
        for(size_t i= 0;i< count; i++) to[i*stride + offset] = xxx[i];
        break;
    }
    case DBF_USHORT: {
        PVUShortArrayPtr pva = static_pointer_cast<PVUShortArray>(pvArray);
        shared_vector<const uint16> xxx(pva->view());
        uint16 *to = static_cast<uint16 *>(dbAddr.pfield);
        for(size_t i= 0;i< count; i++) to[i*stride + offset] = xxx[i];
        break;
    }
    case DBF_LONG: {
        PVIntArrayPtr pva = static_pointer_cast<PVIntArray>(pvArray);
        shared_vector<const int32> xxx(pva->view());
        int32 *to = static_cast<int32 *>(dbAddr.pfield);
        for(size_t i= 0;i< count; i++) to[i*stride + offset] = xxx[i];
        break;
    }
    case DBF_ULONG: {
        PVUIntArrayPtr pva = static_pointer_cast<PVUIntArray>(pvArray);
        shared_vector<const uint32> xxx(pva->view());
        uint32 *to = static_cast<uint32 *>(dbAddr.pfield);
        for(size_t i= 0;i< count; i++) to[i*stride + offset] = xxx[i];
        break;
    }
    case DBF_FLOAT: {
        PVFloatArrayPtr pva = static_pointer_cast<PVFloatArray>(pvArray);
        shared_vector<const float> xxx(pva->view());
        float *to = static_cast<float *>(dbAddr.pfield);
        for(size_t i= 0;i< count; i++) to[i*stride + offset] = xxx[i];
        break;
    }
    case DBF_DOUBLE: {
        PVDoubleArrayPtr pva = static_pointer_cast<PVDoubleArray>(pvArray);
        shared_vector<const double> xxx(pva->view());
        double *to = static_cast<double *>(dbAddr.pfield);
        for(size_t i= 0;i< count; i++) {
            to[i*stride + offset] = xxx[i];
        }
        break;
    }
    case DBF_STRING: {
        PVStringArrayPtr pva = static_pointer_cast<PVStringArray>(pvArray);
        shared_vector<const string> xxx(pva->view());
        char *to = static_cast<char *>(dbAddr.pfield);
        to += offset*dbAddr.field_size;
        for(size_t i= 0;i< count; i+= stride) {
             const char *from = xxx[i].c_str();
             long nchar = xxx[i].size();
             if(nchar>dbAddr.field_size) nchar = dbAddr.field_size;
             for(long j=0; j< nchar; j++) to[j] = from[j];
             to[nchar] = 0;
             to += stride*dbAddr.field_size;
        }
        break;
    }
    }
    }
    db_post_events(dbAddr.precord,dbAddr.pfield,DBE_VALUE | DBE_LOG);
    dbScanUnlock(dbAddr.precord);
    channelArrayRequester->putArrayDone(Status::Ok,getPtrSelf());
}

void DbPvArray::getLength()
{
    long v3offset = 0;
    long v3length = 0;
    dbScanLock(dbAddr.precord);
    struct rset *prset = dbGetRset(&dbAddr);
    if(prset && prset->get_array_info) {
        get_array_info get_info;
        get_info = (get_array_info)(prset->get_array_info);
        get_info(&dbAddr, &v3length, &v3offset);
    }
    dbScanUnlock(dbAddr.precord);
    channelArrayRequester->getLengthDone(
       Status::Ok,getPtrSelf(),v3length);
}

void DbPvArray::setLength(size_t length)
{
    dbScanLock(dbAddr.precord);
    long no_elements = dbAddr.no_elements;
    if(length>static_cast<size_t>(no_elements)) length = no_elements;
    long v3length = length;
    struct rset *prset = dbGetRset(&dbAddr);
    if(prset && prset->put_array_info) {
        put_array_info put_info;
        put_info = (put_array_info)(prset->put_array_info);
        put_info(&dbAddr, v3length);
    }
    dbScanUnlock(dbAddr.precord);
    channelArrayRequester->setLengthDone(Status::Ok,getPtrSelf());
}

void DbPvArray::lock()
{
    dataMutex.lock();
}

void DbPvArray::unlock()
{
    dataMutex.unlock();
}

}}
