/* nturi.cpp */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * This software is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */

#include <algorithm>

#define epicsExportSharedSymbols
#include <pv/nturi.h>
#include <pv/ntutils.h>

using namespace std;
using namespace epics::pvData;

namespace epics { namespace nt {

static NTFieldPtr ntField = NTField::get();

namespace detail {


NTURIBuilder::shared_pointer NTURIBuilder::addQueryString(std::string const & name)
{
    if (std::find(queryFieldNames.begin(), queryFieldNames.end(), name) != queryFieldNames.end())
        throw std::runtime_error("duplicate query field name");

    queryFieldNames.push_back(name);
    queryTypes.push_back(pvString);

    return shared_from_this();
}

NTURIBuilder::shared_pointer NTURIBuilder::addQueryDouble(std::string const & name)
{
    if (std::find(queryFieldNames.begin(), queryFieldNames.end(), name) != queryFieldNames.end())
        throw std::runtime_error("duplicate query field name");

    queryFieldNames.push_back(name);
    queryTypes.push_back(pvDouble);

    return shared_from_this();
}

NTURIBuilder::shared_pointer NTURIBuilder::addQueryInt(std::string const & name)
{
    if (std::find(queryFieldNames.begin(), queryFieldNames.end(), name) != queryFieldNames.end())
        throw std::runtime_error("duplicate query field name");

    queryFieldNames.push_back(name);
    queryTypes.push_back(pvInt);

    return shared_from_this();
}

StructureConstPtr NTURIBuilder::createStructure()
{
    FieldBuilderPtr builder = getFieldCreate()->
        createFieldBuilder()->
        setId(NTURI::URI)->
        add("scheme", pvString);

    if (authority)
        builder->add("authority", pvString);

    builder->add("path", pvString);

    if (!queryFieldNames.empty())
    {
        FieldBuilderPtr nestedBuilder = builder->
             addNestedStructure("query");

        vector<string>::size_type len = queryFieldNames.size();
        for (vector<string>::size_type i = 0; i < len; i++)
            nestedBuilder->add(queryFieldNames[i], queryTypes[i]);

        builder = nestedBuilder->endNested();
    }

    size_t extraCount = extraFieldNames.size();
    for (size_t i = 0; i< extraCount; i++)
        builder->add(extraFieldNames[i], extraFields[i]);

    StructureConstPtr s = builder->createStructure();

    reset();
    return s;
}

NTURIBuilder::shared_pointer NTURIBuilder::addAuthority()
{
    authority = true;
    return shared_from_this();
}

PVStructurePtr NTURIBuilder::createPVStructure()
{
    return getPVDataCreate()->createPVStructure(createStructure());
}

NTURIPtr NTURIBuilder::create()
{
    return NTURIPtr(new NTURI(createPVStructure()));
}

NTURIBuilder::NTURIBuilder()
{
    reset();
}

void NTURIBuilder::reset()
{
    queryFieldNames.clear();
    queryTypes.clear();
    authority = false;
}

NTURIBuilder::shared_pointer NTURIBuilder::add(string const & name, FieldConstPtr const & field)
{
    extraFields.push_back(field);
    extraFieldNames.push_back(name);
    return shared_from_this();
}

}

const std::string NTURI::URI("epics:nt/NTURI:1.0");

NTURI::shared_pointer NTURI::wrap(PVStructurePtr const & pvStructure)
{
    if(!isCompatible(pvStructure)) return shared_pointer();
    return wrapUnsafe(pvStructure);
}

NTURI::shared_pointer NTURI::wrapUnsafe(PVStructurePtr const & pvStructure)
{
    return shared_pointer(new NTURI(pvStructure));
}

bool NTURI::is_a(StructureConstPtr const & structure)
{
    return NTUtils::is_a(structure->getID(), URI);
}

bool NTURI::isCompatible(StructureConstPtr const & structure)
{
    if (!structure.get()) return false;

    ScalarConstPtr schemeField = structure->getField<Scalar>("scheme");
    if (schemeField.get() == 0 || schemeField->getScalarType() != pvString)
        return false;

    ScalarConstPtr pathField = structure->getField<Scalar>("path");
    if (pathField.get() == 0 || pathField->getScalarType() != pvString)
        return false;

    FieldConstPtr field = structure->getField("authority");
    if (field.get())
    {
        ScalarConstPtr authorityField = structure->getField<Scalar>("authority");
        if (!authorityField.get() || authorityField->getScalarType() != pvString)
            return false;
    }

    field = structure->getField("query");
    if (field.get())
    {
        StructureConstPtr queryField = structure->getField<Structure>("query");
        if (!queryField.get())
            return false;

        FieldConstPtrArray const & fields = queryField->getFields();
        for (FieldConstPtrArray::const_iterator it = fields.begin();
             it != fields.end(); ++it)
        {
            if ((*it)->getType() != scalar) return false;
            ScalarType scalarType = std::tr1::dynamic_pointer_cast<const Scalar>(
               (*it))->getScalarType();
            if (scalarType != pvString &&
                scalarType != pvInt &&
                scalarType != pvDouble) return false;
        }
    }

    return true;
}


bool NTURI::isCompatible(PVStructurePtr const & pvStructure)
{
    if(!pvStructure) return false;

    return isCompatible(pvStructure->getStructure());
}

bool NTURI::isValid()
{
    return true;
}

NTURIBuilderPtr NTURI::createBuilder()
{
    return NTURIBuilderPtr(new detail::NTURIBuilder());
}


PVStructurePtr NTURI::getPVStructure() const
{
    return pvNTURI;
}


PVStringPtr NTURI::getScheme() const
{
    return pvNTURI->getSubField<PVString>("scheme");
}

PVStringPtr NTURI::getAuthority() const
{
    return pvNTURI->getSubField<PVString>("authority");
}

PVStringPtr NTURI::getPath() const
{
    return pvNTURI->getSubField<PVString>("path");
}

PVStructurePtr NTURI::getQuery() const
{
    return pvNTURI->getSubField<PVStructure>("query");
}

StringArray const & NTURI::getQueryNames() const
{
    return pvNTURI->getSubField<PVStructure>("query")->getStructure()->getFieldNames();
}

PVFieldPtr NTURI::getQueryField(std::string const & name) const
{
    return pvNTURI->getSubField("query." + name);
}

NTURI::NTURI(PVStructurePtr const & pvStructure) :
    pvNTURI(pvStructure)
{}


}}
