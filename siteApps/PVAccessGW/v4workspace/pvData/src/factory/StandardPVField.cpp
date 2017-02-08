/* StandardPVField.cpp */
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
#include <pv/lock.h>
#include <pv/pvIntrospect.h>
#include <pv/pvData.h>
#include <pv/standardField.h>
#include <pv/standardPVField.h>

using std::string;

namespace epics { namespace pvData { 

StandardPVField::StandardPVField()
: standardField(getStandardField()),
  fieldCreate(getFieldCreate()),
  pvDataCreate(getPVDataCreate()),
  notImplemented("not implemented")
{}

StandardPVField::~StandardPVField(){}

PVStructurePtr StandardPVField::scalar(
    ScalarType type,string const & properties)
{
    StructureConstPtr field = standardField->scalar(type,properties);
    PVStructurePtr pvStructure = pvDataCreate->createPVStructure(field);
    return pvStructure;
}

PVStructurePtr StandardPVField::scalarArray(
    ScalarType elementType, string const & properties)
{
    StructureConstPtr field = standardField->scalarArray(elementType,properties);
    PVStructurePtr pvStructure = pvDataCreate->createPVStructure(field);
    return pvStructure;
}

PVStructurePtr StandardPVField::structureArray(
    StructureConstPtr const & structure,string const & properties)
{
    StructureConstPtr field = standardField->structureArray(structure,properties);
    PVStructurePtr pvStructure = pvDataCreate->createPVStructure(field);
    return pvStructure;
}

PVStructurePtr StandardPVField::unionArray(
    UnionConstPtr const & punion,string const & properties)
{
    StructureConstPtr field = standardField->unionArray(punion,properties);
    PVStructurePtr pvStructure = pvDataCreate->createPVStructure(field);
    return pvStructure;
}

PVStructurePtr StandardPVField::enumerated(StringArray const &choices)
{
    StructureConstPtr field = standardField->enumerated();
    PVStructurePtr pvStructure = pvDataCreate->createPVStructure(field);
    PVStringArray::svector cdata(choices.size());
    std::copy(choices.begin(), choices.end(), cdata.begin());
    pvStructure->getSubFieldT<PVStringArray>("choices")->replace(freeze(cdata));
    return pvStructure;
}

PVStructurePtr StandardPVField::enumerated(
    StringArray const &choices,string const & properties)
{
    StructureConstPtr field = standardField->enumerated(properties);
    PVStructurePtr pvStructure =  pvDataCreate->createPVStructure(field);
    PVStringArray::svector cdata(choices.size());
    std::copy(choices.begin(), choices.end(), cdata.begin());
    pvStructure->getSubFieldT<PVStringArray>("value.choices")->replace(freeze(cdata));
    return pvStructure;
}

StandardPVFieldPtr StandardPVField::getStandardPVField()
{
    static StandardPVFieldPtr standardPVField;
    static Mutex mutex;
    Lock xx(mutex);

    if(standardPVField.get()==NULL) {
        standardPVField= StandardPVFieldPtr(new StandardPVField());
    }
    return standardPVField;
}

StandardPVFieldPtr getStandardPVField() {
    return StandardPVField::getStandardPVField();
}

}}
