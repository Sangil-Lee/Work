/* ntndarray.cpp */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * This software is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */

#include <algorithm>

#define epicsExportSharedSymbols
#include <pv/ntndarray.h>
#include <pv/ntndarrayAttribute.h>
#include <pv/ntutils.h>

using namespace std;
using namespace epics::pvData;

namespace epics { namespace nt {

static NTFieldPtr ntField = NTField::get();


namespace detail {

static FieldCreatePtr fieldCreate = getFieldCreate();
static PVDataCreatePtr pvDataCreate = getPVDataCreate();

static Mutex mutex;

StructureConstPtr NTNDArrayBuilder::createStructure()
{
    enum
    {
        DISCRIPTOR_INDEX,
        TIMESTAMP_INDEX,
        ALARM_INDEX,
        DISPLAY_INDEX
    };

    const size_t NUMBER_OF_INDICES = DISPLAY_INDEX+1;
    const size_t NUMBER_OF_STRUCTURES = 1 << NUMBER_OF_INDICES;

    Lock xx(mutex);

    static StructureConstPtr ntndarrayStruc[NUMBER_OF_STRUCTURES];
    static UnionConstPtr valueType;
    static StructureConstPtr codecStruc;
    static StructureConstPtr dimensionStruc;
    static StructureConstPtr attributeStruc;

    StructureConstPtr returnedStruc;

    size_t index = 0;
    if (descriptor) index  |= 1 << DISCRIPTOR_INDEX;
    if (timeStamp)  index  |= 1 << TIMESTAMP_INDEX;
    if (alarm)      index  |= 1 << ALARM_INDEX;
    if (display)    index  |= 1 << DISPLAY_INDEX;

    bool isExtended = !extraFieldNames.empty();

    if (isExtended || !ntndarrayStruc[index])
    {
        StandardFieldPtr standardField = getStandardField();
        FieldBuilderPtr fb = fieldCreate->createFieldBuilder();

        if (!valueType)
        {
            for (int i = pvBoolean; i < pvString; ++i)
            {
                ScalarType st = static_cast<ScalarType>(i);
                fb->addArray(std::string(ScalarTypeFunc::name(st)) + "Value", st);
            }
            valueType = fb->createUnion();                
        }

        if (!codecStruc)
        {
            codecStruc = fb->setId("codec_t")->
                add("name", pvString)->
                add("parameters", fieldCreate->createVariantUnion())->
                createStructure();
        }

        if (!dimensionStruc)
        {
            dimensionStruc = fb->setId("dimension_t")->
                add("size", pvInt)->
                add("offset",  pvInt)->
                add("fullSize",  pvInt)->
                add("binning",  pvInt)->
                add("reverse",  pvBoolean)->
                createStructure();
        }

        if (!attributeStruc)
        {
            attributeStruc = NTNDArrayAttribute::createBuilder()->createStructure();
        }

        fb->setId(NTNDArray::URI)->
            add("value", valueType)->
            add("codec", codecStruc)->
            add("compressedSize", pvLong)->
            add("uncompressedSize", pvLong)->
            addArray("dimension", dimensionStruc)->
            add("uniqueId", pvInt)->
            add("dataTimeStamp", standardField->timeStamp())->
            addArray("attribute", attributeStruc);

        if (descriptor)
            fb->add("descriptor", pvString);

        if (alarm)
            fb->add("alarm", standardField->alarm());

        if (timeStamp)
            fb->add("timeStamp", standardField->timeStamp());

        if (display)
            fb->add("display", standardField->display());

        size_t extraCount = extraFieldNames.size();
        for (size_t i = 0; i< extraCount; i++)
            fb->add(extraFieldNames[i], extraFields[i]);

        returnedStruc = fb->createStructure();

        if (!isExtended)
            ntndarrayStruc[index] = returnedStruc; 
    }
    else
    {
        return ntndarrayStruc[index];
    }

    return returnedStruc;
}

NTNDArrayBuilder::shared_pointer NTNDArrayBuilder::addDescriptor()
{
    descriptor = true;
    return shared_from_this();
}

NTNDArrayBuilder::shared_pointer NTNDArrayBuilder::addAlarm()
{
    alarm = true;
    return shared_from_this();
}

NTNDArrayBuilder::shared_pointer NTNDArrayBuilder::addTimeStamp()
{
    timeStamp = true;
    return shared_from_this();
}

NTNDArrayBuilder::shared_pointer NTNDArrayBuilder::addDisplay()
{
    display = true;
    return shared_from_this();
}

PVStructurePtr NTNDArrayBuilder::createPVStructure()
{
    return getPVDataCreate()->createPVStructure(createStructure());
}

NTNDArrayPtr NTNDArrayBuilder::create()
{
    return NTNDArrayPtr(new NTNDArray(createPVStructure()));
}

NTNDArrayBuilder::NTNDArrayBuilder()
{
    reset();
}

void NTNDArrayBuilder::reset()
{
    descriptor = false;
    timeStamp = false;
    alarm = false;
    display = false;
    extraFieldNames.clear();
    extraFields.clear();
}

NTNDArrayBuilder::shared_pointer NTNDArrayBuilder::add(string const & name, FieldConstPtr const & field)
{
    extraFields.push_back(field); extraFieldNames.push_back(name);
    return shared_from_this();
}


}

const std::string NTNDArray::URI("epics:nt/NTNDArray:1.0");
const std::string ntAttrStr("epics:nt/NTAttribute:1.0");

static FieldCreatePtr fieldCreate = getFieldCreate();
static PVDataCreatePtr pvDataCreate = getPVDataCreate();

class NTValueType
{
public:
    static bool isCompatible(UnionConstPtr const &u)
    {
        if(!u.get()) return false;

        if (u->getID() != Union::defaultId()) return false;
        if (u->isVariant()) return false;

        for (int i = pvBoolean; i != pvString; ++i)
        {
            ScalarType scalarType = static_cast<ScalarType>(i);
            std::string name(ScalarTypeFunc::name(scalarType));
            name += "Value";
            ScalarArrayConstPtr scalarField = u->getField<ScalarArray>(name);
            if (scalarField.get() == 0 ||
                    scalarField->getElementType() != scalarType)
                return false;
        }

        return true;
    }
};

class NTCodec
{
public:
    static bool isCompatible(StructureConstPtr const &structure)
    {
        if(!structure.get()) return false;

        if (structure->getID() != "codec_t") return false;

        ScalarConstPtr scalarField = structure->getField<Scalar>("name");
        if (scalarField.get() == 0 || scalarField->getScalarType() != pvString)
            return false;

        UnionConstPtr paramField = structure->getField<Union>("parameters");
        if (paramField.get() == 0 || !paramField->isVariant())
            return false;

        return true;
    }
};


class NTDimension
{
public:
    static bool isCompatible(StructureConstPtr const &structure)
    {
        if(!structure.get()) return false;

        if (structure->getID() != "dimension_t") return false;

        ScalarConstPtr scalarField = structure->getField<Scalar>("size");
        if (scalarField.get() == 0 || scalarField->getScalarType() != pvInt)
            return false;

        scalarField = structure->getField<Scalar>("offset");
        if (scalarField.get() == 0 || scalarField->getScalarType() != pvInt)
            return false;

        scalarField = structure->getField<Scalar>("fullSize");
        if (scalarField.get() == 0 || scalarField->getScalarType() != pvInt)
            return false;

        scalarField = structure->getField<Scalar>("binning");
        if (scalarField.get() == 0 || scalarField->getScalarType() != pvInt)
            return false;

        scalarField = structure->getField<Scalar>("reverse");
        if (scalarField.get() == 0 || scalarField->getScalarType() != pvBoolean)
            return false;

        return true;
    }
};

NTNDArray::shared_pointer NTNDArray::wrap(PVStructurePtr const & pvStructure)
{
    if(!isCompatible(pvStructure)) return shared_pointer();
    return wrapUnsafe(pvStructure);
}

NTNDArray::shared_pointer NTNDArray::wrapUnsafe(PVStructurePtr const & pvStructure)
{
    return shared_pointer(new NTNDArray(pvStructure));
}

bool NTNDArray::is_a(StructureConstPtr const & structure)
{
    return NTUtils::is_a(structure->getID(), URI);
}

bool NTNDArray::isCompatible(StructureConstPtr const &structure)
{
    if(!structure.get()) return false;

    UnionConstPtr valueField = structure->getField<Union>("value");
    if(!NTValueType::isCompatible(valueField)) return false;

    StructureConstPtr codecField = structure->getField<Structure>("codec");
    if(!NTCodec::isCompatible(codecField)) return false;

    ScalarConstPtr compressedSizeField = structure->getField<Scalar>("compressedSize");
    if (compressedSizeField.get() == 0)
        return false;

    if (compressedSizeField->getScalarType() != pvLong)
        return false;


    ScalarConstPtr uncompressedSizeField = structure->getField<Scalar>("uncompressedSize");
    if (uncompressedSizeField.get() == 0)
        return false;

    if (uncompressedSizeField->getScalarType() != pvLong)
        return false;

    StructureArrayConstPtr dimensionField = structure->getField<StructureArray>("dimension");
    if (dimensionField.get() == 0)
        return false;
    StructureConstPtr dimElementStruc = dimensionField->getStructure();

    if(!NTDimension::isCompatible(dimElementStruc))
       return false;

    NTFieldPtr ntField = NTField::get();

    StructureConstPtr dataTimeStampField = structure->getField<Structure>(
        "dataTimeStamp");
    if (dataTimeStampField.get() == 0 || !ntField->isTimeStamp(dataTimeStampField))
        return false;


    ScalarConstPtr uniqueIdField = structure->getField<Scalar>("uniqueId");
    if (uniqueIdField.get() == 0)
        return false;

    if (uniqueIdField->getScalarType() != pvInt)
        return false;


    StructureArrayConstPtr attributeField = structure->getField<StructureArray>( "attribute");

    StructureConstPtr attributeElementStruc = attributeField->getStructure();

    if (!NTNDArrayAttribute::isCompatible(attributeElementStruc))
        return false;


    FieldConstPtr field = structure->getField("descriptor");
    if (field.get())
    {
        ScalarConstPtr descriptorField = structure->getField<Scalar>("descriptor");
        if (!descriptorField.get() || descriptorField->getScalarType() != pvString)
            return false;
    }

    field = structure->getField("alarm");
    if (field.get() && !ntField->isAlarm(field))
        return false;

    field = structure->getField("timeStamp");
    if (field.get() && !ntField->isTimeStamp(field))
        return false;

    field = structure->getField("display");
    if (field.get() && !ntField->isDisplay(field))
        return false;

    return true;
}


bool NTNDArray::isCompatible(PVStructurePtr const & pvStructure)
{
    if(!pvStructure.get()) return false;

    return isCompatible(pvStructure->getStructure());
}


bool NTNDArray::isValid()
{
    int64 valueSize = getValueSize();
    int64 compressedSize = getCompressedDataSize()->get();
    if (valueSize != compressedSize)
        return false;

    long expectedUncompressed = getExpectedUncompressedSize();
    long uncompressedSize = getUncompressedDataSize()->get();
    if (uncompressedSize != expectedUncompressed)
        return false;

    std::string codecName = getCodec()->getSubField<PVString>("name")->get();
    if (codecName == "" && valueSize < uncompressedSize)
        return false;

    return true;
}

int64 NTNDArray::getExpectedUncompressedSize()
{
    int64 size = 0;
    PVStructureArrayPtr pvDim = getDimension();

    if (pvDim->getLength() != 0)
    {
        PVStructureArray::const_svector data = pvDim->view();
        size = getValueTypeSize();
        for (PVStructureArray::const_svector::const_iterator it = data.begin();
        it != data.end(); ++it )
        {
            PVStructurePtr dim = *it;
            size *= dim->getSubField<PVInt>("size")->get();
        }
    }

    return size;
}

int64 NTNDArray::getValueSize()
{
    int64 size = 0;
    PVScalarArrayPtr storedValue = getValue()->get<PVScalarArray>();
    if (!storedValue.get())
    {
        size = storedValue->getLength()*getValueTypeSize();
    }
    return size;
}

int64 NTNDArray::getValueTypeSize()
{
    int64 typeSize = 0;
    PVScalarArrayPtr storedValue = getValue()->get<PVScalarArray>();
    if (storedValue.get())
    {
        switch (storedValue->getScalarArray()->getElementType())
        {
        case pvBoolean:
        case pvByte:
        case pvUByte:
            typeSize = 1;
            break;

        case pvShort:
        case pvUShort:
            typeSize = 2;
            break;

        case pvInt:
        case pvUInt:
        case pvFloat:
            typeSize = 4;
            break;

        case pvLong:
        case pvULong:
        case pvDouble:
            typeSize = 8;
            break;

        default:
            break;
        }
    }
    return typeSize;
}

NTNDArrayBuilderPtr NTNDArray::createBuilder()
{
    return NTNDArrayBuilderPtr(new detail::NTNDArrayBuilder());
}


bool NTNDArray::attachTimeStamp(PVTimeStamp &pvTimeStamp) const
{
    PVStructurePtr ts = getTimeStamp();
    if (ts)
        return pvTimeStamp.attach(ts);
    else
        return false;
}

bool NTNDArray::attachDataTimeStamp(PVTimeStamp &pvTimeStamp) const
{
    PVStructurePtr ts = getDataTimeStamp();
    if (ts)
        return pvTimeStamp.attach(ts);
    else
        return false;
}

bool NTNDArray::attachAlarm(PVAlarm &pvAlarm) const
{
    PVStructurePtr al = getAlarm();
    if (al)
        return pvAlarm.attach(al);
    else
        return false;
}

bool NTNDArray::attachDisplay(PVDisplay &pvDisplay) const
{
    PVStructurePtr dp = getDisplay();
    if (dp)
        return pvDisplay.attach(dp);
    else
        return false;
}

PVStructurePtr NTNDArray::getPVStructure() const
{
    return pvNTNDArray;
}

PVUnionPtr NTNDArray::getValue() const
{
    return pvNTNDArray->getSubField<PVUnion>("value");
}

PVStructurePtr NTNDArray::getCodec() const
{
    return pvNTNDArray->getSubField<PVStructure>("codec");
}

PVLongPtr NTNDArray::getCompressedDataSize() const
{
    return pvNTNDArray->getSubField<PVLong>("compressedSize");
}

PVLongPtr NTNDArray::getUncompressedDataSize() const
{
    return pvNTNDArray->getSubField<PVLong>("uncompressedSize");
}

PVStructureArrayPtr NTNDArray::getDimension() const
{
    return pvNTNDArray->getSubField<PVStructureArray>("dimension");
}

PVIntPtr NTNDArray::getUniqueId() const
{
    return pvNTNDArray->getSubField<PVInt>("uniqueId");
}

PVStructurePtr NTNDArray::getDataTimeStamp() const
{
    return pvNTNDArray->getSubField<PVStructure>("dataTimeStamp");
}

PVStructureArrayPtr NTNDArray::getAttribute() const
{
    return pvNTNDArray->getSubField<PVStructureArray>("attribute");
}

PVStringPtr NTNDArray::getDescriptor() const
{
    return pvNTNDArray->getSubField<PVString>("descriptor");
}

PVStructurePtr NTNDArray::getTimeStamp() const
{
    return pvNTNDArray->getSubField<PVStructure>("timeStamp");
}

PVStructurePtr NTNDArray::getAlarm() const
{
    return pvNTNDArray->getSubField<PVStructure>("alarm");
}

PVStructurePtr NTNDArray::getDisplay() const
{
    return pvNTNDArray->getSubField<PVStructure>("display");
}


NTNDArray::NTNDArray(PVStructurePtr const & pvStructure) :
    pvNTNDArray(pvStructure)
{}


}}
