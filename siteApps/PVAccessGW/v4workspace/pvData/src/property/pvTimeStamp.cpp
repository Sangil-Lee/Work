/* pvTimeStamp.cpp */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/**
 *  @author mrk
 */
#include <string>
#include <stdexcept>

#define epicsExportSharedSymbols
#include <pv/pvType.h>
#include <pv/timeStamp.h>
#include <pv/pvData.h>
#include <pv/pvTimeStamp.h>

using std::tr1::static_pointer_cast;
using std::string;

namespace epics { namespace pvData { 

string PVTimeStamp::noTimeStamp("No timeStamp structure found");
string PVTimeStamp::notAttached("Not attached to a timeStamp structure");

bool PVTimeStamp::attach(PVFieldPtr const & pvField)
{
    if(pvField->getField()->getType()!=structure) return false;
    PVStructurePtr xxx = static_pointer_cast<PVStructure>(pvField);
    PVStructure* pvStructure = xxx.get();
    while(true) {
        PVLongPtr pvLong = pvStructure->getSubField<PVLong>("secondsPastEpoch");
        if(pvLong.get()!=NULL) {
            pvSecs = pvLong;
            pvNano = pvStructure->getSubField<PVInt>("nanoseconds");
            pvUserTag = pvStructure->getSubField<PVInt>("userTag");
        }
        if(pvSecs.get()!=NULL
        && pvNano.get()!=NULL
        && pvUserTag.get()!=NULL) return true;
        detach();
        // look up the tree for a timeSyamp
        pvStructure = pvStructure->getParent();
        if(pvStructure==NULL) break;
    }
    return false;
}

void PVTimeStamp::detach()
{
    pvSecs.reset();
    pvUserTag.reset();
    pvNano.reset();
}

bool PVTimeStamp::isAttached() {
    if(pvSecs.get()==NULL) return false;
    return true;
}

void PVTimeStamp::get(TimeStamp & timeStamp) const
{
    if(pvSecs.get()==NULL) {
        throw std::logic_error(notAttached);
    }
    timeStamp.put(pvSecs->get(),pvNano->get());
    timeStamp.setUserTag(pvUserTag->get());
}

bool PVTimeStamp::set(TimeStamp const & timeStamp)
{
    if(pvSecs.get()==NULL) {
        throw std::logic_error(notAttached);
    }
    if(pvSecs->isImmutable() || pvNano->isImmutable()) return false;
    pvSecs->put(timeStamp.getSecondsPastEpoch());
    pvUserTag->put(timeStamp.getUserTag());
    pvNano->put(timeStamp.getNanoseconds());
    return true;
}
    
}}
