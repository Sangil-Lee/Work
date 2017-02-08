/* pvaClientPutData.cpp */
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

class PvaClientPostHandlerPvt: public PostHandler
{
    PvaClientPutData * easyData;
    size_t fieldNumber;
public:
    PvaClientPostHandlerPvt(PvaClientPutData *easyData,size_t fieldNumber)
    : easyData(easyData),fieldNumber(fieldNumber){}
    void postPut() { easyData->postPut(fieldNumber);}
};


typedef std::tr1::shared_ptr<PVArray> PVArrayPtr;
static ConvertPtr convert = getConvert();
static string noValue("no value field");
static string notScalar("value is not a scalar");
static string notCompatibleScalar("value is not a compatible scalar");
static string notArray("value is not an array");
static string notScalarArray("value is not a scalarArray");
static string notDoubleArray("value is not a doubleArray");
static string notStringArray("value is not a stringArray");

PvaClientPutDataPtr PvaClientPutData::create(StructureConstPtr const & structure)
{
    PvaClientPutDataPtr epv(new PvaClientPutData(structure));
    return epv;
}

PvaClientPutData::PvaClientPutData(StructureConstPtr const & structure)
: structure(structure),
  pvStructure(getPVDataCreate()->createPVStructure(structure)),
  bitSet(BitSetPtr(new BitSet(pvStructure->getNumberFields())))
{
    messagePrefix = "";
    size_t nfields = pvStructure->getNumberFields();
    postHandler.resize(nfields);
    PVFieldPtr pvField;
    for(size_t i =0; i<nfields; ++i)
    {
        postHandler[i] = PostHandlerPtr(new PvaClientPostHandlerPvt(this, i));
        if(i==0) {
            pvField = pvStructure;
        } else {
            pvField = pvStructure->getSubField(i);
        }
        pvField->setPostHandler(postHandler[i]);
    }
    pvValue = pvStructure->getSubField("value");
}

void PvaClientPutData::checkValue()
{
    if(pvValue) return;
    throw std::runtime_error(messagePrefix + noValue);
}

void PvaClientPutData::postPut(size_t fieldNumber)
{
    bitSet->set(fieldNumber);
}

void PvaClientPutData::setMessagePrefix(std::string const & value)
{
    messagePrefix = value + " ";
}

StructureConstPtr PvaClientPutData::getStructure()
{return structure;}

PVStructurePtr PvaClientPutData::getPVStructure()
{return pvStructure;}

BitSetPtr PvaClientPutData::getChangedBitSet()
{return bitSet;}

std::ostream & PvaClientPutData::showChanged(std::ostream & out)
{
    size_t nextSet = bitSet->nextSetBit(0);
    PVFieldPtr pvField;
    while(nextSet!=string::npos) {
        if(nextSet==0) {
             pvField = pvStructure;
        } else {
              pvField = pvStructure->getSubField(nextSet);
        }
        string name = pvField->getFullName();
        out << name << " = " << pvField << endl;
        nextSet = bitSet->nextSetBit(nextSet+1);
    }
    return out;
}

bool PvaClientPutData::hasValue()
{
    if(!pvValue) return false;
    return true;
}

bool PvaClientPutData::isValueScalar()
{
    if(!pvValue) return false;
    if(pvValue->getField()->getType()==scalar) return true;
    return false;
}

bool PvaClientPutData::isValueScalarArray()
{
    if(!pvValue) return false;
    if(pvValue->getField()->getType()==scalarArray) return true;
    return false;
}

PVFieldPtr  PvaClientPutData::getValue()
{
   checkValue();
   return pvValue;
}

PVScalarPtr  PvaClientPutData::getScalarValue()
{
    checkValue();
    PVScalarPtr pv = pvStructure->getSubField<PVScalar>("value");
    if(!pv) throw std::runtime_error(messagePrefix + notScalar);
    return pv;
}

PVArrayPtr  PvaClientPutData::getArrayValue()
{
    checkValue();
    PVArrayPtr pv = pvStructure->getSubField<PVArray>("value");
    if(!pv) throw std::runtime_error(messagePrefix + notArray);
    return pv;
}

PVScalarArrayPtr  PvaClientPutData::getScalarArrayValue()
{
    checkValue();
    PVScalarArrayPtr pv = pvStructure->getSubField<PVScalarArray>("value");
    if(!pv) throw std::runtime_error(messagePrefix + notScalarArray);
    return pv;
}

double PvaClientPutData::getDouble()
{
    PVScalarPtr pvScalar = getScalarValue();
    ScalarType scalarType = pvScalar->getScalar()->getScalarType();
    if(scalarType==pvDouble) {
        PVDoublePtr pvDouble = static_pointer_cast<PVDouble>(pvScalar);
        return pvDouble->get();
    }
    if(!ScalarTypeFunc::isNumeric(scalarType)) {
        throw std::runtime_error(messagePrefix  + notCompatibleScalar);
    }
    return convert->toDouble(pvScalar);
}

string PvaClientPutData::getString()
{
    PVScalarPtr pvScalar = getScalarValue();
    return convert->toString(pvScalar);
}

shared_vector<const double> PvaClientPutData::getDoubleArray()
{
    checkValue();
    PVDoubleArrayPtr pv = pvStructure->getSubField<PVDoubleArray>("value");
    if(!pv) {
        throw std::runtime_error(messagePrefix + notDoubleArray);
    }
    return pv->view();   
}

shared_vector<const string> PvaClientPutData::getStringArray()
{
    checkValue();
    PVStringArrayPtr pv = pvStructure->getSubField<PVStringArray>("value");
    if(!pv) {
        throw std::runtime_error(messagePrefix + notStringArray);
    }
    return pv->view();   

}

void PvaClientPutData::putDouble(double value)
{
    PVScalarPtr pvScalar = getScalarValue();
    ScalarType scalarType = pvScalar->getScalar()->getScalarType();
    if(scalarType==pvDouble) {
        PVDoublePtr pvDouble = static_pointer_cast<PVDouble>(pvScalar);
         pvDouble->put(value);
         return;
    }
    if(!ScalarTypeFunc::isNumeric(scalarType)) {
        throw std::runtime_error(messagePrefix + notCompatibleScalar);
    }
    convert->fromDouble(pvScalar,value);
}

void PvaClientPutData::putString(std::string const & value)
{
    PVScalarPtr pvScalar = getScalarValue();
    convert->fromString(pvScalar,value);
}



void PvaClientPutData::putDoubleArray(shared_vector<const double> const & value)
{
    checkValue();
    PVDoubleArrayPtr pv = pvStructure->getSubField<PVDoubleArray>("value");
    if(!pv) {
        throw std::runtime_error(messagePrefix + notDoubleArray);
    }
    pv->replace(value);
}

void PvaClientPutData::putStringArray(shared_vector<const std::string> const & value)
{
    checkValue();
    PVStringArrayPtr pv = pvStructure->getSubField<PVStringArray>("value");
    if(!pv) {
        throw std::runtime_error(messagePrefix + notStringArray);
    }
    pv->replace(value);
}

void PvaClientPutData::putStringArray(std::vector<std::string> const & value)
{
    checkValue();
    PVScalarArrayPtr pv = pvStructure->getSubField<PVScalarArray>("value");
    if(!pv) {
        throw std::runtime_error(messagePrefix + notScalarArray);
    }
    convert->fromStringArray(pv,0,value.size(),value,0);
}

}}
