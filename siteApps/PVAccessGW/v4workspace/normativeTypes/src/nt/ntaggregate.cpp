/* ntaggregate.cpp */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * This software is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */

#define epicsExportSharedSymbols
#include <pv/ntaggregate.h>
#include <pv/ntutils.h>

using namespace std;
using namespace epics::pvData;

namespace epics { namespace nt {

static NTFieldPtr ntField = NTField::get();

namespace detail {

StructureConstPtr NTAggregateBuilder::createStructure()
{
    FieldBuilderPtr builder =
            getFieldCreate()->createFieldBuilder()->
               setId(NTAggregate::URI)->
               add("value", pvDouble)->
               add("N", pvLong);

    if (dispersion)
        builder->add("dispersion", pvDouble);

    if (first)
        builder->add("first", pvDouble);

    if (firstTimeStamp)
        builder->add("firstTimeStamp", ntField->createTimeStamp());

    if (last)
        builder->add("last" , pvDouble);

    if (lastTimeStamp)
        builder->add("lastTimeStamp", ntField->createTimeStamp());

    if (max)
        builder->add("max", pvDouble);

    if (min)
        builder->add("min", pvDouble);

    if (descriptor)
        builder->add("descriptor", pvString);

    if (alarm)
        builder->add("alarm", ntField->createAlarm());

    if (timeStamp)
        builder->add("timeStamp", ntField->createTimeStamp());

    size_t extraCount = extraFieldNames.size();
    for (size_t i = 0; i< extraCount; i++)
        builder->add(extraFieldNames[i], extraFields[i]);


    StructureConstPtr s = builder->createStructure();

    reset();
    return s;
}

NTAggregateBuilder::shared_pointer NTAggregateBuilder::addDispersion()
{
    dispersion = true;
    return shared_from_this();
}

NTAggregateBuilder::shared_pointer NTAggregateBuilder::addFirst()
{
    first = true;
    return shared_from_this();
}

NTAggregateBuilder::shared_pointer NTAggregateBuilder::addFirstTimeStamp()
{
    firstTimeStamp = true;
    return shared_from_this();
}

NTAggregateBuilder::shared_pointer NTAggregateBuilder::addLast()
{
    last = true;
    return shared_from_this();
}

NTAggregateBuilder::shared_pointer NTAggregateBuilder::addLastTimeStamp()
{
    lastTimeStamp = true;
    return shared_from_this();
}

NTAggregateBuilder::shared_pointer NTAggregateBuilder::addMax()
{
    max = true;
    return shared_from_this();
}

NTAggregateBuilder::shared_pointer NTAggregateBuilder::addMin()
{
    min = true;
    return shared_from_this();
}

NTAggregateBuilder::shared_pointer NTAggregateBuilder::addDescriptor()
{
    descriptor = true;
    return shared_from_this();
}

NTAggregateBuilder::shared_pointer NTAggregateBuilder::addAlarm()
{
    alarm = true;
    return shared_from_this();
}

NTAggregateBuilder::shared_pointer NTAggregateBuilder::addTimeStamp()
{
    timeStamp = true;
    return shared_from_this();
}

PVStructurePtr NTAggregateBuilder::createPVStructure()
{
    return getPVDataCreate()->createPVStructure(createStructure());
}

NTAggregatePtr NTAggregateBuilder::create()
{
    return NTAggregatePtr(new NTAggregate(createPVStructure()));
}

NTAggregateBuilder::NTAggregateBuilder()
{
    reset();
}

void NTAggregateBuilder::reset()
{
    dispersion = false;
    first = false;
    firstTimeStamp = false;
    last = false;
    lastTimeStamp = false;
    max = false;
    min = false;

    descriptor = false;
    alarm = false;
    timeStamp = false;

    extraFieldNames.clear();
    extraFields.clear();
}

NTAggregateBuilder::shared_pointer NTAggregateBuilder::add(string const & name, FieldConstPtr const & field)
{
    extraFields.push_back(field); extraFieldNames.push_back(name);
    return shared_from_this();
}


}

const std::string NTAggregate::URI("epics:nt/NTAggregate:1.0");

NTAggregate::shared_pointer NTAggregate::wrap(PVStructurePtr const & pvStructure)
{
    if(!isCompatible(pvStructure)) return shared_pointer();
    return wrapUnsafe(pvStructure);
}

NTAggregate::shared_pointer NTAggregate::wrapUnsafe(PVStructurePtr const & pvStructure)
{
    return shared_pointer(new NTAggregate(pvStructure));
}

bool NTAggregate::is_a(StructureConstPtr const & structure)
{
    return NTUtils::is_a(structure->getID(), URI);
}

bool NTAggregate::isCompatible(StructureConstPtr const &structure)
{
    if (structure.get() == 0) return false;

    ScalarConstPtr valueField = structure->getField<Scalar>("value");
    if (valueField.get() == 0 || valueField->getScalarType() != pvDouble)
        return false;

    ScalarConstPtr nField = structure->getField<Scalar>("N");
    if (nField.get() == 0 || nField->getScalarType() != pvLong)
        return false;

    FieldConstPtr field = structure->getField("dispersion");
    if (field.get())
    {
        ScalarConstPtr dispersionField = structure->getField<Scalar>("dispersion");
        if (!dispersionField.get() || dispersionField->getScalarType() != pvDouble)
            return false;
    }

    field = structure->getField("first");
    if (field.get())
    {
        ScalarConstPtr firstField = structure->getField<Scalar>("first");
        if (!firstField.get() || firstField->getScalarType() != pvDouble)
            return false;
    }

    field = structure->getField("firstTimeStamp");
    if (field.get() && !ntField->isTimeStamp(field))
        return false;

    field = structure->getField("last");
    if (field.get())
    {
        ScalarConstPtr lastField = structure->getField<Scalar>("last");
        if (!lastField.get() || lastField->getScalarType() != pvDouble)
            return false;
    }

    field = structure->getField("lastTimeStamp");
    if (field.get() && !ntField->isTimeStamp(field))
        return false;

    field = structure->getField("max");
    if (field.get())
    {
        ScalarConstPtr maxField = structure->getField<Scalar>("max");
        if (!maxField.get() || maxField->getScalarType() != pvDouble)
            return false;
    }

    field = structure->getField("min");
    if (field.get())
    {
        ScalarConstPtr minField = structure->getField<Scalar>("min");
        if (!minField.get() || minField->getScalarType() != pvDouble)
            return false;
    }

    field = structure->getField("descriptor");
    if (field.get())
    {
        ScalarConstPtr descriptorField = structure->getField<Scalar>("descriptor");
        if (!descriptorField.get() || descriptorField->getScalarType() != pvString)
            return false;
    }

    NTFieldPtr ntField = NTField::get();

    field = structure->getField("alarm");
    if (field.get() && !ntField->isAlarm(field))
        return false;

    field = structure->getField("timeStamp");
    if (field.get() && !ntField->isTimeStamp(field))
        return false;

    return true;
}

bool NTAggregate::isCompatible(PVStructurePtr const & pvStructure)
{
    if(!pvStructure) return false;

    return isCompatible(pvStructure->getStructure());
}

bool NTAggregate::isValid()
{
    return true;
}

NTAggregateBuilderPtr NTAggregate::createBuilder()
{
    return NTAggregateBuilderPtr(new detail::NTAggregateBuilder());
}

bool NTAggregate::attachTimeStamp(PVTimeStamp &pvTimeStamp) const
{
    PVStructurePtr ts = getTimeStamp();
    if (ts)
        return pvTimeStamp.attach(ts);
    else
        return false;
}

bool NTAggregate::attachAlarm(PVAlarm &pvAlarm) const
{
    PVStructurePtr al = getAlarm();
    if (al)
        return pvAlarm.attach(al);
    else
        return false;
}

PVStructurePtr NTAggregate::getPVStructure() const
{
    return pvNTAggregate;
}

PVStringPtr NTAggregate::getDescriptor() const
{
    return pvNTAggregate->getSubField<PVString>("descriptor");
}

PVStructurePtr NTAggregate::getTimeStamp() const
{
    return pvNTAggregate->getSubField<PVStructure>("timeStamp");
}

PVStructurePtr NTAggregate::getAlarm() const
{
    return pvNTAggregate->getSubField<PVStructure>("alarm");
}

PVDoublePtr NTAggregate::getValue() const
{
    return pvValue;
}

PVLongPtr NTAggregate::getN() const
{
    return pvNTAggregate->getSubField<PVLong>("N");
}

PVDoublePtr NTAggregate::getDispersion() const
{
    return pvNTAggregate->getSubField<PVDouble>("dispersion");
}

PVDoublePtr NTAggregate::getFirst() const
{
    return pvNTAggregate->getSubField<PVDouble>("first");
}

PVStructurePtr NTAggregate::getFirstTimeStamp() const
{
    return pvNTAggregate->getSubField<PVStructure>("firstTimeStamp");
}

PVDoublePtr NTAggregate::getLast() const
{
    return pvNTAggregate->getSubField<PVDouble>("last");
}

PVStructurePtr NTAggregate::getLastTimeStamp() const
{
    return pvNTAggregate->getSubField<PVStructure>("lastTimeStamp");
}

PVDoublePtr NTAggregate::getMax() const
{
    return pvNTAggregate->getSubField<PVDouble>("max");
}

PVDoublePtr NTAggregate::getMin() const
{
    return pvNTAggregate->getSubField<PVDouble>("min");
}

NTAggregate::NTAggregate(PVStructurePtr const & pvStructure) :
    pvNTAggregate(pvStructure), pvValue(pvNTAggregate->getSubField<PVDouble>("value"))
{}


}}
