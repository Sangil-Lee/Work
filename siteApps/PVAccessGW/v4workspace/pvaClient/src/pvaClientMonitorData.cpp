/* pvaClientMonitorData.cpp */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/**
 * @author mrk
 * @date 2015.02
 */

#include <typeinfo>
#include <sstream>

#include <pv/createRequest.h>
#include <pv/convert.h>

#define epicsExportSharedSymbols

#include <pv/pvaClient.h>

using std::tr1::static_pointer_cast;
using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace std;

namespace epics { namespace pvaClient {


typedef std::tr1::shared_ptr<PVArray> PVArrayPtr;

static ConvertPtr convert = getConvert();

static StructureConstPtr nullStructure;
static PVStructurePtr nullPVStructure;

static string noStructure("no pvStructure ");
static string noValue("no value field");
static string noScalar("value is not a scalar");
static string notCompatibleScalar("value is not a compatible scalar");
static string noArray("value is not an array");
static string noScalarArray("value is not a scalarArray");
static string notDoubleArray("value is not a doubleArray");
static string notStringArray("value is not a stringArray");
static string noAlarm("no alarm");
static string noTimeStamp("no timeStamp");


PvaClientMonitorDataPtr PvaClientMonitorData::create(StructureConstPtr const & structure)
{
    PvaClientMonitorDataPtr epv(new PvaClientMonitorData(structure));
    return epv;
}

PvaClientMonitorData::PvaClientMonitorData(StructureConstPtr const & structure)
: structure(structure)
{
    messagePrefix = "";
}

void PvaClientMonitorData::setData(MonitorElementPtr const & monitorElement)
{
   pvStructure = monitorElement->pvStructurePtr;
   changedBitSet = monitorElement->changedBitSet;
   overrunBitSet = monitorElement->overrunBitSet;
   pvValue = pvStructure->getSubField("value");
}

void PvaClientMonitorData::checkValue()
{
    if(pvValue) return;
    throw std::runtime_error(messagePrefix + noValue);
}

void PvaClientMonitorData::setMessagePrefix(std::string const & value)
{
    messagePrefix = value + " ";
}

StructureConstPtr PvaClientMonitorData::getStructure()
{return structure;}

PVStructurePtr PvaClientMonitorData::getPVStructure()
{
    if(pvStructure) return pvStructure;
    throw std::runtime_error(messagePrefix + noStructure);
}

BitSetPtr PvaClientMonitorData::getChangedBitSet()
{
    if(!changedBitSet) throw std::runtime_error(messagePrefix + noStructure);
    return changedBitSet;
}

BitSetPtr PvaClientMonitorData::getOverrunBitSet()
{
    if(!overrunBitSet) throw std::runtime_error(messagePrefix + noStructure);
    return overrunBitSet;
}

std::ostream & PvaClientMonitorData::showChanged(std::ostream & out)
{
    if(!changedBitSet) throw std::runtime_error(messagePrefix + noStructure);
    size_t nextSet = changedBitSet->nextSetBit(0);
    PVFieldPtr pvField;
    while(nextSet!=string::npos) {
        if(nextSet==0) {
             pvField = pvStructure;
        } else {
              pvField = pvStructure->getSubField(nextSet);
        }
        string name = pvField->getFullName();
        out << name << " = " << pvField << endl;
        nextSet = changedBitSet->nextSetBit(nextSet+1);
    }
    return out;
}

std::ostream & PvaClientMonitorData::showOverrun(std::ostream & out)
{
    if(!overrunBitSet) throw std::runtime_error(messagePrefix + noStructure);
    size_t nextSet = overrunBitSet->nextSetBit(0);
    PVFieldPtr pvField;
    while(nextSet!=string::npos) {
        if(nextSet==0) {
             pvField = pvStructure;
        } else {
              pvField = pvStructure->getSubField(nextSet);
        }
        string name = pvField->getFullName();
        out << name << " = " << pvField << endl;
        nextSet = overrunBitSet->nextSetBit(nextSet+1);
    }
    return out;
}

bool PvaClientMonitorData::hasValue()
{
    if(!pvValue) return false;
    return true;
}

bool PvaClientMonitorData::isValueScalar()
{
    if(!pvValue) return false;
    if(pvValue->getField()->getType()==scalar) return true;
    return false;
}

bool PvaClientMonitorData::isValueScalarArray()
{
    if(!pvValue) return false;
    if(pvValue->getField()->getType()==scalarArray) return true;
    return false;
}

PVFieldPtr  PvaClientMonitorData::getValue()
{
   checkValue();
   return pvValue;
}

PVScalarPtr  PvaClientMonitorData::getScalarValue()
{
    checkValue();
    PVScalarPtr pv = pvStructure->getSubField<PVScalar>("value");
    if(!pv) {
        throw std::runtime_error(messagePrefix + noScalar);
    }
    return pv;
}

PVArrayPtr  PvaClientMonitorData::getArrayValue()
{
    checkValue();
    PVArrayPtr pv = pvStructure->getSubField<PVArray>("value");
    if(!pv) {
        throw std::runtime_error(messagePrefix + noArray);
    }
    return pv;
}

PVScalarArrayPtr  PvaClientMonitorData::getScalarArrayValue()
{
    checkValue();
    PVScalarArrayPtr pv = pvStructure->getSubField<PVScalarArray>("value");
    if(!pv) {
        throw std::runtime_error(messagePrefix + noScalarArray);
    }
    return pv;
}

double PvaClientMonitorData::getDouble()
{
    PVScalarPtr pvScalar = getScalarValue();
    ScalarType scalarType = pvScalar->getScalar()->getScalarType();
    if(scalarType==pvDouble) {
        PVDoublePtr pvDouble = static_pointer_cast<PVDouble>(pvScalar);
        return pvDouble->get();
    }
    if(!ScalarTypeFunc::isNumeric(scalarType)) {
        throw std::runtime_error(messagePrefix + notCompatibleScalar);
    }
    return convert->toDouble(pvScalar);
}

string PvaClientMonitorData::getString()
{
    PVScalarPtr pvScalar = getScalarValue();
    return convert->toString(pvScalar);
}

shared_vector<const double> PvaClientMonitorData::getDoubleArray()
{
    checkValue();
    PVDoubleArrayPtr pv = pvStructure->getSubField<PVDoubleArray>("value");
    if(!pv) {
        throw std::runtime_error(messagePrefix + notDoubleArray);
    }
    return pv->view();   
}

shared_vector<const string> PvaClientMonitorData::getStringArray()
{
    checkValue();
    PVStringArrayPtr pv = pvStructure->getSubField<PVStringArray>("value");
    if(!pv) {
        throw std::runtime_error(messagePrefix + notStringArray);
    }
    return pv->view();   

}

Alarm PvaClientMonitorData::getAlarm()
{
   if(!pvStructure) {
        throw std::runtime_error(messagePrefix + noAlarm);
   }
   PVStructurePtr pvs = pvStructure->getSubField<PVStructure>("alarm");
   if(!pvs) throw std::runtime_error(messagePrefix + noAlarm);
   pvAlarm.attach(pvs);
   if(pvAlarm.isAttached()) {
       Alarm alarm;
       pvAlarm.get(alarm);
       pvAlarm.detach();
       return alarm;
   }
   throw std::runtime_error(messagePrefix + noAlarm);
}

TimeStamp PvaClientMonitorData::getTimeStamp()
{
   if(!pvStructure) {
        throw std::runtime_error(messagePrefix + noTimeStamp);
   }
   PVStructurePtr pvs = pvStructure->getSubField<PVStructure>("timeStamp");
   if(!pvs) throw std::runtime_error(messagePrefix + noTimeStamp);
   pvTimeStamp.attach(pvs);
   if(pvTimeStamp.isAttached()) {
       TimeStamp timeStamp;
       pvTimeStamp.get(timeStamp);
       pvTimeStamp.detach();
       return timeStamp;
   }
   throw std::runtime_error(messagePrefix + noTimeStamp);
}

}}
