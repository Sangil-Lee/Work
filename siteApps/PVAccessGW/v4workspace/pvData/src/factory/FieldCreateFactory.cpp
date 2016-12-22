/*FieldCreateFactory.cpp*/
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/**
 *  @author mrk
 */

#ifdef _WIN32
#define NOMINMAX
#endif

#include <cstddef>
#include <cstdlib>
#include <string>
#include <cstdio>
#include <stdexcept>
#include <sstream>

#include <errlog.h>

#define epicsExportSharedSymbols
#include <pv/lock.h>
#include <pv/pvIntrospect.h>
#include <pv/factory.h>
#include <pv/serializeHelper.h>

using std::tr1::static_pointer_cast;
using std::size_t;
using std::string;

static volatile unsigned largest_bitset;

// check for definitions affected by a BitSet serialization issue
// https://github.com/epics-base/pvDataCPP/issues/24
static
void bugCheck(size_t nfields)
{
    if(nfields>largest_bitset) {
        largest_bitset = nfields;
        errlogPrintf("Info: new max Structure/Union size %u\n", (unsigned)nfields);
    }
    if((nfields%64)<56) return;
    errlogPrintf("Warning: PVStructure/Union with %u fields,\n"
                 "BitSet serialization compatibility as per to https://github.com/epics-base/pvDataCPP/issues/24\n",
                 (unsigned)nfields);
}

namespace epics { namespace pvData {

static DebugLevel debugLevel = lowDebug;

Field::Field(Type type)
    : m_fieldType(type)
{
}

Field::~Field() {
}


std::ostream& operator<<(std::ostream& o, const Field& f)
{
	return f.dump(o);
};

Scalar::Scalar(ScalarType scalarType)
       : Field(scalar),scalarType(scalarType)
{
    if(scalarType<0 || scalarType>MAX_SCALAR_TYPE)
        THROW_EXCEPTION2(std::invalid_argument, "Can't construct Scalar from invalid ScalarType");
}

Scalar::~Scalar(){}

std::ostream& Scalar::dump(std::ostream& o) const
{
    return o << format::indent() << getID();
}

string Scalar::getID() const
{
    static const string idScalarLUT[] = {
	"boolean", // pvBoolean
	"byte",    // pvByte
	"short",   // pvShort
	"int",     // pvInt
	"long",    // pvLong
	"ubyte",   // pvUByte
	"ushort",  // pvUShort
	"uint",    // pvUInt
	"ulong",   // pvULong
	"float",   // pvFloat
	"double",  // pvDouble
	"string"   // pvString
    };
    return idScalarLUT[scalarType];
}

int8 Scalar::getTypeCodeLUT(ScalarType scalarType)
{
    static const int8 typeCodeLUT[] = {
        0x00, // pvBoolean
        0x20, // pvByte
        0x21, // pvShort
        0x22, // pvInt
        0x23, // pvLong
        0x24, // pvUByte
        0x25, // pvUShort
        0x26, // pvUInt
        0x27, // pvULong
        0x42, // pvFloat
        0x43, // pvDouble
        0x60  // pvString
    };
   return typeCodeLUT[scalarType];
}


void Scalar::serialize(ByteBuffer *buffer, SerializableControl *control) const {
    control->ensureBuffer(1);
    buffer->putByte(getTypeCodeLUT(scalarType));
}

void Scalar::deserialize(ByteBuffer* /*buffer*/, DeserializableControl* /*control*/) {
    // must be done via FieldCreate
    throw std::runtime_error("not valid operation, use FieldCreate::deserialize instead");
}




std::string BoundedString::getID() const
{
    std::ostringstream id;
    id << Scalar::getID() << '(' << maxLength << ')';
    return id.str();
}

void BoundedString::serialize(ByteBuffer *buffer, SerializableControl *control) const
{
    control->ensureBuffer(1);
    buffer->putByte(0x83);
    SerializeHelper::writeSize(maxLength, buffer, control);
}

std::size_t BoundedString::getMaximumLength() const
{
    return maxLength;
}

BoundedString::BoundedString(std::size_t maxStringLength) :
    Scalar(pvString), maxLength(maxStringLength)
{
    if (maxLength == 0)
        THROW_EXCEPTION2(std::invalid_argument, "maxLength == 0");
}

BoundedString::~BoundedString() {}


static string emptyStringtring;

static void serializeStructureField(const Structure* structure, ByteBuffer* buffer, SerializableControl* control)
{
	// to optimize default (non-empty) IDs optimization
	// empty IDs are not allowed
	string id = structure->getID();
	if (id == Structure::DEFAULT_ID)	// TODO slow comparison
		SerializeHelper::serializeString(emptyStringtring, buffer, control);
	else
	    SerializeHelper::serializeString(id, buffer, control);

    FieldConstPtrArray const & fields = structure->getFields();
    StringArray const & fieldNames = structure->getFieldNames();
    std::size_t len = fields.size();
    SerializeHelper::writeSize(len, buffer, control);
    for (std::size_t i = 0; i < len; i++)
    {
        SerializeHelper::serializeString(fieldNames[i], buffer, control);
        control->cachedSerialize(fields[i], buffer);
    }
}

static StructureConstPtr deserializeStructureField(const FieldCreate* fieldCreate, ByteBuffer* buffer, DeserializableControl* control)
{
    string id = SerializeHelper::deserializeString(buffer, control);
    const std::size_t size = SerializeHelper::readSize(buffer, control);
    FieldConstPtrArray fields; fields.reserve(size);
    StringArray fieldNames; fieldNames.reserve(size);
    for (std::size_t i = 0; i < size; i++)
    {
        fieldNames.push_back(SerializeHelper::deserializeString(buffer, control));
        fields.push_back(control->cachedDeserialize(buffer));
    }

    if (id.empty())
        return fieldCreate->createStructure(fieldNames, fields);
    else
    	return fieldCreate->createStructure(id, fieldNames, fields);
}

static void serializeUnionField(const Union* punion, ByteBuffer* buffer, SerializableControl* control)
{
	// to optimize default (non-empty) IDs optimization
	// empty IDs are not allowed
	string id = punion->getID();
	if (id == Union::DEFAULT_ID)	// TODO slow comparison
		SerializeHelper::serializeString(emptyStringtring, buffer, control);
	else
	    SerializeHelper::serializeString(id, buffer, control);

    FieldConstPtrArray const & fields = punion->getFields();
    StringArray const & fieldNames = punion->getFieldNames();
    std::size_t len = fields.size();
    SerializeHelper::writeSize(len, buffer, control);
    for (std::size_t i = 0; i < len; i++)
    {
        SerializeHelper::serializeString(fieldNames[i], buffer, control);
        control->cachedSerialize(fields[i], buffer);
    }
}

static UnionConstPtr deserializeUnionField(const FieldCreate* fieldCreate, ByteBuffer* buffer, DeserializableControl* control)
{
    string id = SerializeHelper::deserializeString(buffer, control);
    const std::size_t size = SerializeHelper::readSize(buffer, control);
    FieldConstPtrArray fields; fields.reserve(size);
    StringArray fieldNames; fieldNames.reserve(size);
    for (std::size_t i = 0; i < size; i++)
    {
        fieldNames.push_back(SerializeHelper::deserializeString(buffer, control));
        fields.push_back(control->cachedDeserialize(buffer));
    }

    if (id.empty())
        return fieldCreate->createUnion(fieldNames, fields);
    else
    	return fieldCreate->createUnion(id, fieldNames, fields);
}

Array::Array(Type type)
    : Field(type)
{
}

Array::~Array() {}

ScalarArray::ScalarArray(ScalarType elementType)
    : Array(scalarArray),
      elementType(elementType)
{
    if(elementType<0 || elementType>MAX_SCALAR_TYPE)
        throw std::invalid_argument("Can't construct ScalarArray from invalid ScalarType");
}

ScalarArray::~ScalarArray() {}

const string ScalarArray::getIDScalarArrayLUT() const
{
    static const string idScalarArrayLUT[] = {
    "boolean[]", // pvBoolean
    "byte[]",    // pvByte
    "short[]",   // pvShort
	"int[]",     // pvInt
	"long[]",    // pvLong
	"ubyte[]",   // pvUByte
	"ushort[]",  // pvUShort
	"uint[]",    // pvUInt
	"ulong[]",   // pvULong
	"float[]",   // pvFloat
	"double[]",  // pvDouble
	"string[]"   // pvString
    };
    return idScalarArrayLUT[elementType];
}

string ScalarArray::getID() const
{
    return getIDScalarArrayLUT();
}

std::ostream& ScalarArray::dump(std::ostream& o) const
{
    return o << format::indent() << getID();
}

void ScalarArray::serialize(ByteBuffer *buffer, SerializableControl *control) const {
    control->ensureBuffer(1);
    buffer->putByte((int8)0x08 | Scalar::getTypeCodeLUT(elementType));
}

void ScalarArray::deserialize(ByteBuffer* /*buffer*/, DeserializableControl* /*control*/) {
    throw std::runtime_error("not valid operation, use FieldCreate::deserialize instead");
}


BoundedScalarArray::~BoundedScalarArray() {}

BoundedScalarArray::BoundedScalarArray(ScalarType elementType, size_t size)
    : ScalarArray(elementType),
      size(size)
{
}

string BoundedScalarArray::getID() const
{
    char buffer[32];
    sprintf(buffer, "%s<%zu>", ScalarTypeFunc::name(getElementType()), size);
    return string(buffer);
}

void BoundedScalarArray::serialize(ByteBuffer *buffer, SerializableControl *control) const {
    control->ensureBuffer(1);
    buffer->putByte((int8)0x10 | Scalar::getTypeCodeLUT(getElementType()));
    SerializeHelper::writeSize(size, buffer, control);
}


FixedScalarArray::~FixedScalarArray() {}

FixedScalarArray::FixedScalarArray(ScalarType elementType, size_t size)
    : ScalarArray(elementType),
      size(size)
{
}

string FixedScalarArray::getID() const
{
    char buffer[32];
    sprintf(buffer, "%s[%zu]", ScalarTypeFunc::name(getElementType()), size);
    return string(buffer);
}

void FixedScalarArray::serialize(ByteBuffer *buffer, SerializableControl *control) const {
    control->ensureBuffer(1);
    buffer->putByte((int8)0x18 | Scalar::getTypeCodeLUT(getElementType()));
    SerializeHelper::writeSize(size, buffer, control);
}



StructureArray::StructureArray(StructureConstPtr const & structure)
: Array(structureArray),pstructure(structure)
{
}

StructureArray::~StructureArray() {
    if(debugLevel==highDebug) printf("~StructureArray\n");
}

string StructureArray::getID() const
{
	return pstructure->getID() + "[]";
}

std::ostream& StructureArray::dump(std::ostream& o) const
{
    o << format::indent() << getID() << std::endl;
    {
        format::indent_scope s(o);
        o << *pstructure;
    }
    return o;
}

void StructureArray::serialize(ByteBuffer *buffer, SerializableControl *control) const {
    control->ensureBuffer(1);
    buffer->putByte((int8)0x88);
    control->cachedSerialize(pstructure, buffer);
}

void StructureArray::deserialize(ByteBuffer* /*buffer*/, DeserializableControl* /*control*/) {
    throw std::runtime_error("not valid operation, use FieldCreate::deserialize instead");
}

UnionArray::UnionArray(UnionConstPtr const & _punion)
: Array(unionArray),punion(_punion)
{
}

UnionArray::~UnionArray() {
    if(debugLevel==highDebug) printf("~UnionArray\n");
}

string UnionArray::getID() const
{
	return punion->getID() + "[]";
}

std::ostream& UnionArray::dump(std::ostream& o) const
{
    o << format::indent() << getID() << std::endl;
    {
        format::indent_scope s(o);
        o << *punion;
    }
    return o;
}

void UnionArray::serialize(ByteBuffer *buffer, SerializableControl *control) const {
    control->ensureBuffer(1);
    if (punion->isVariant())
    {
        // unrestricted/variant union
        buffer->putByte((int8)0x8A);
    }
    else
    {
        buffer->putByte((int8)0x89);
        control->cachedSerialize(punion, buffer);
    }
}

void UnionArray::deserialize(ByteBuffer* /*buffer*/, DeserializableControl* /*control*/) {
    throw std::runtime_error("not valid operation, use FieldCreate::deserialize instead");
}

const string Structure::DEFAULT_ID = Structure::defaultId();

const string & Structure::defaultId()
{
    static const string id = "structure";
    return id;
}

Structure::Structure (
    StringArray const & fieldNames,
    FieldConstPtrArray const & infields,
    string const & inid)
: Field(structure),
      fieldNames(fieldNames),
      fields(infields),
      id(inid)
{
    if(inid.empty()) {
        THROW_EXCEPTION2(std::invalid_argument, "Can't construct Structure, id is empty string");
    }
    if(fieldNames.size()!=fields.size()) {
        THROW_EXCEPTION2(std::invalid_argument, "Can't construct Structure, fieldNames.size()!=fields.size()");
    }
    size_t number = fields.size();
    bugCheck(number);
    for(size_t i=0; i<number; i++) {
        const string& name = fieldNames[i];
        if(name.empty()) {
            THROW_EXCEPTION2(std::invalid_argument, "Can't construct Structure, empty string in fieldNames");
        }
        if(fields[i].get()==NULL)
            THROW_EXCEPTION2(std::invalid_argument, "Can't construct Structure, NULL in fields");
        // look for duplicates
        for(size_t j=i+1; j<number; j++) {
            string otherName = fieldNames[j];
            int result = name.compare(otherName);
            if(result==0) {
                string  message("Can't construct Structure, duplicate fieldName ");
                message += name;
                THROW_EXCEPTION2(std::invalid_argument, message);
            }
        }
    }
}

Structure::~Structure() { }


string Structure::getID() const
{
	return id;
}

FieldConstPtr  Structure::getField(string const & fieldName) const {
    size_t numberFields = fields.size();
    for(size_t i=0; i<numberFields; i++) {
        FieldConstPtr pfield = fields[i];
        int result = fieldName.compare(fieldNames[i]);
        if(result==0) return pfield;
    }
    return FieldConstPtr();
}

size_t Structure::getFieldIndex(string const &fieldName) const {
    size_t numberFields = fields.size();
    for(size_t i=0; i<numberFields; i++) {
        FieldConstPtr pfield = fields[i];
        int result = fieldName.compare(fieldNames[i]);
        if(result==0) return i;
    }
    return -1;
}

std::ostream& Structure::dump(std::ostream& o) const
{
    o << format::indent() << getID() << std::endl;
    {
        format::indent_scope s(o);
        dumpFields(o);
    }
    return o;
}

void Structure::dumpFields(std::ostream& o) const
{
    size_t numberFields = fields.size();
    for(size_t i=0; i<numberFields; i++) {
        FieldConstPtr pfield = fields[i];
        o << format::indent() << pfield->getID() << ' ' << fieldNames[i] << std::endl;
        switch(pfield->getType()) {
            case scalar:
            case scalarArray:
                break;
            case structure:
            {
                Field const *xxx = pfield.get();
                Structure const *pstruct = static_cast<Structure const*>(xxx);
                format::indent_scope s(o);
                pstruct->dumpFields(o);
                break;
            }
            case structureArray:
            {
                format::indent_scope s(o);
                Field const *xxx = pfield.get();
                StructureArray const *pstructureArray = static_cast<StructureArray const*>(xxx);
                o << *pstructureArray->getStructure();
                break;
            }
            case union_:
            {
                Field const *xxx = pfield.get();
                Union const *punion = static_cast<Union const*>(xxx);
                format::indent_scope s(o);
                punion->dumpFields(o);
                break;
            }
            case unionArray:
            {
                format::indent_scope s(o);
                Field const *xxx = pfield.get();
                UnionArray const *punionArray = static_cast<UnionArray const*>(xxx);
                o << *punionArray->getUnion();
                break;
            }
        }
    }
}

void Structure::serialize(ByteBuffer *buffer, SerializableControl *control) const {
    control->ensureBuffer(1);
    buffer->putByte((int8)0x80);
    serializeStructureField(this, buffer, control);
}

void Structure::deserialize(ByteBuffer* /*buffer*/, DeserializableControl* /*control*/) {
    throw std::runtime_error("not valid operation, use FieldCreate::deserialize instead");
}

const string Union::DEFAULT_ID = Union::defaultId();

const string & Union::defaultId()
{
    static const string id = "union";
    return id;
}

const string Union::ANY_ID = Union::anyId();

const string & Union::anyId()
{
    static const string id = "any";
    return id;
}

Union::Union ()
: Field(union_),
      fieldNames(),
      fields(),
      id(anyId())
{
}


Union::Union (
    StringArray const & fieldNames,
    FieldConstPtrArray const & infields,
    string const & inid)
: Field(union_),
      fieldNames(fieldNames),
      fields(infields),
      id(inid)
{
    if(inid.empty()) {
        THROW_EXCEPTION2(std::invalid_argument, "Can't construct Union, id is empty string");
    }
    if(fieldNames.size()!=fields.size()) {
        THROW_EXCEPTION2(std::invalid_argument, "Can't construct Union, fieldNames.size()!=fields.size()");
    }
    if(fields.size()==0 && inid!=ANY_ID) {
        THROW_EXCEPTION2(std::invalid_argument, "Can't construct Union, no fields only allowed when id = " + ANY_ID);
    }

    size_t number = fields.size();
    bugCheck(number);
    for(size_t i=0; i<number; i++) {
        const string& name = fieldNames[i];
        if(name.empty()) {
            THROW_EXCEPTION2(std::invalid_argument, "Can't construct Union, empty string in fieldNames");
        }
        if(fields[i].get()==NULL)
            THROW_EXCEPTION2(std::invalid_argument, "Can't construct Union, NULL in fields");
        // look for duplicates
        for(size_t j=i+1; j<number; j++) {
            string otherName = fieldNames[j];
            int result = name.compare(otherName);
            if(result==0) {
                string  message("Can't construct Union, duplicate fieldName ");
                message += name;
                THROW_EXCEPTION2(std::invalid_argument, message);
            }
        }
    }
}

Union::~Union() { }


string Union::getID() const
{
	return id;
}

FieldConstPtr  Union::getField(string const & fieldName) const {
    size_t numberFields = fields.size();
    for(size_t i=0; i<numberFields; i++) {
        FieldConstPtr pfield = fields[i];
        int result = fieldName.compare(fieldNames[i]);
        if(result==0) return pfield;
    }
    return FieldConstPtr();
}

size_t Union::getFieldIndex(string const &fieldName) const {
    size_t numberFields = fields.size();
    for(size_t i=0; i<numberFields; i++) {
        FieldConstPtr pfield = fields[i];
        int result = fieldName.compare(fieldNames[i]);
        if(result==0) return i;
    }
    return -1;
}

std::ostream& Union::dump(std::ostream& o) const
{
    o << format::indent() << getID() << std::endl;
    {
        format::indent_scope s(o);
        dumpFields(o);
    }
    return o;
}

void Union::dumpFields(std::ostream& o) const
{
    size_t numberFields = fields.size();
    for(size_t i=0; i<numberFields; i++) {
        FieldConstPtr pfield = fields[i];
        o << format::indent() << pfield->getID() << ' ' << fieldNames[i] << std::endl;
        switch(pfield->getType()) {
            case scalar:
            case scalarArray:
                break;
            case structure:
            {
                Field const *xxx = pfield.get();
                Structure const *pstruct = static_cast<Structure const*>(xxx);
                format::indent_scope s(o);
                pstruct->dumpFields(o);
                break;
            }
            case structureArray:
            {
                format::indent_scope s(o);
                o << *pfield;
                break;
            }
            case union_:
            {
                Field const *xxx = pfield.get();
                Union const *punion = static_cast<Union const*>(xxx);
                format::indent_scope s(o);
                punion->dumpFields(o);
                break;
            }
            case unionArray:
            {
                format::indent_scope s(o);
                o << *pfield;
                break;
            }
        }
    }
}

void Union::serialize(ByteBuffer *buffer, SerializableControl *control) const {
    control->ensureBuffer(1);
    if (fields.size() == 0)
    {
        // unrestricted/variant union
        buffer->putByte((int8)0x82);
    }
    else
    {
        buffer->putByte((int8)0x81);
        serializeUnionField(this, buffer, control);
    }
}

void Union::deserialize(ByteBuffer* /*buffer*/, DeserializableControl* /*control*/) {
    throw std::runtime_error("not valid operation, use FieldCreate::deserialize instead");
}


FieldBuilder::FieldBuilder() : fieldCreate(getFieldCreate()), idSet(false) {}

FieldBuilder::FieldBuilder(FieldBuilderPtr const & _parentBuilder,
			string const & _nestedName,
			Type _nestedClassToBuild, bool _nestedArray) :
		fieldCreate(getFieldCreate()),
		idSet(false),
		parentBuilder(_parentBuilder),
		nestedClassToBuild(_nestedClassToBuild),
		nestedName(_nestedName),
		nestedArray(_nestedArray)
{}

void FieldBuilder::reset()
{
	id.erase();
    idSet = false;
	fieldNames.clear();
	fields.clear();
}

FieldBuilderPtr FieldBuilder::setId(string const & id)
{
    this->id = id;
    idSet = true; 
    return shared_from_this();
}

FieldBuilderPtr FieldBuilder::add(string const & name, ScalarType scalarType)
{
    fields.push_back(fieldCreate->createScalar(scalarType)); fieldNames.push_back(name); 
	return shared_from_this();
}

FieldBuilderPtr FieldBuilder::addBoundedString(std::string const & name, std::size_t maxLength)
{
    fields.push_back(fieldCreate->createBoundedString(maxLength)); fieldNames.push_back(name);
    return shared_from_this();
}

FieldBuilderPtr FieldBuilder::add(string const & name, FieldConstPtr const & field)
{
    fields.push_back(field); fieldNames.push_back(name);
	return shared_from_this();
}

FieldBuilderPtr FieldBuilder::addArray(string const & name, ScalarType scalarType)
{
    fields.push_back(fieldCreate->createScalarArray(scalarType)); fieldNames.push_back(name);
	return shared_from_this();
}

FieldBuilderPtr FieldBuilder::addFixedArray(string const & name, ScalarType scalarType, size_t size)
{
    fields.push_back(fieldCreate->createFixedScalarArray(scalarType, size)); fieldNames.push_back(name);
    return shared_from_this();
}

FieldBuilderPtr FieldBuilder::addBoundedArray(string const & name, ScalarType scalarType, size_t size)
{
    fields.push_back(fieldCreate->createBoundedScalarArray(scalarType, size)); fieldNames.push_back(name);
    return shared_from_this();
}

FieldBuilderPtr FieldBuilder::addArray(string const & name, FieldConstPtr const & element)
{
    switch (element->getType())
    {
        case structure:
            fields.push_back(fieldCreate->createStructureArray(static_pointer_cast<const Structure>(element)));
            break;
        case union_:
            fields.push_back(fieldCreate->createUnionArray(static_pointer_cast<const Union>(element)));
            break;
        case scalar:

            if (std::tr1::dynamic_pointer_cast<const BoundedString>(element).get())
                THROW_EXCEPTION2(std::invalid_argument, "bounded string arrays are not supported");

            fields.push_back(fieldCreate->createScalarArray(static_pointer_cast<const Scalar>(element)->getScalarType()));
            break;
        // scalarArray?
        default:
            std::ostringstream msg("unsupported array element type: ");
            msg << element->getType();
            THROW_EXCEPTION2(std::invalid_argument, msg.str());
    }
    
    fieldNames.push_back(name);
	return shared_from_this();
}

FieldConstPtr FieldBuilder::createFieldInternal(Type type)
{
    // minor optimization
    if (fieldNames.size() == 0 && type == union_)
        return fieldCreate->createVariantUnion();

    if (type == structure)
    {
        return (idSet) ?
            fieldCreate->createStructure(id, fieldNames, fields) :
            fieldCreate->createStructure(fieldNames, fields);
    }
    else if (type == union_)
    {
        return (idSet) ?
            fieldCreate->createUnion(id, fieldNames, fields) :
            fieldCreate->createUnion(fieldNames, fields);
    }
    else
    {
        std::ostringstream msg("unsupported type: ");
        msg << type;
        THROW_EXCEPTION2(std::invalid_argument, msg.str());
    }
}


StructureConstPtr FieldBuilder::createStructure()
{
    if (parentBuilder.get())
        THROW_EXCEPTION2(std::runtime_error, "createStructure() called in nested FieldBuilder");
	
    StructureConstPtr field(static_pointer_cast<const Structure>(createFieldInternal(structure)));
    reset();
    return field;
}

UnionConstPtr FieldBuilder::createUnion()
{
    if (parentBuilder.get())
        THROW_EXCEPTION2(std::runtime_error, "createUnion() called in nested FieldBuilder");
	
    UnionConstPtr field(static_pointer_cast<const Union>(createFieldInternal(union_)));
    reset();
    return field;
}

FieldBuilderPtr FieldBuilder::addNestedStructure(string const & name)
{
    return FieldBuilderPtr(new FieldBuilder(shared_from_this(), name, structure, false));
}


FieldBuilderPtr FieldBuilder::addNestedUnion(string const & name)
{
    return FieldBuilderPtr(new FieldBuilder(shared_from_this(), name, union_, false));
}


FieldBuilderPtr FieldBuilder::addNestedStructureArray(string const & name)
{
    return FieldBuilderPtr(new FieldBuilder(shared_from_this(), name, structure, true));
}

FieldBuilderPtr FieldBuilder::addNestedUnionArray(string const & name)
{
    return FieldBuilderPtr(new FieldBuilder(shared_from_this(), name, union_, true));
}

FieldBuilderPtr FieldBuilder::endNested()
{
    if (!parentBuilder.get())
        THROW_EXCEPTION2(std::runtime_error, "this method can only be called to create nested fields");
        
    FieldConstPtr nestedField = createFieldInternal(nestedClassToBuild);
    if (nestedArray)
        parentBuilder->addArray(nestedName, nestedField);
    else
        parentBuilder->add(nestedName, nestedField);
        
    return parentBuilder;
}


FieldBuilderPtr FieldCreate::createFieldBuilder() const
{
    return FieldBuilderPtr(new FieldBuilder());
}

ScalarConstPtr FieldCreate::createScalar(ScalarType scalarType) const
{
    if(scalarType<0 || scalarType>MAX_SCALAR_TYPE) {
        std::ostringstream strm("Can't construct Scalar from invalid ScalarType ");
        strm << scalarType;
        THROW_EXCEPTION2(std::invalid_argument, strm.str());
    }

    return scalars[scalarType];
}

BoundedStringConstPtr FieldCreate::createBoundedString(std::size_t maxLength) const
{
    // TODO use std::make_shared
    std::tr1::shared_ptr<BoundedString> s(new BoundedString(maxLength), Field::Deleter());
    BoundedStringConstPtr sa = s;
    return sa;
}

ScalarArrayConstPtr FieldCreate::createScalarArray(ScalarType elementType) const
{
    if(elementType<0 || elementType>MAX_SCALAR_TYPE) {
        std::ostringstream strm("Can't construct ScalarArray from invalid ScalarType ");
        strm << elementType;
        THROW_EXCEPTION2(std::invalid_argument, strm.str());
    }
        
    return scalarArrays[elementType];
}

ScalarArrayConstPtr FieldCreate::createFixedScalarArray(ScalarType elementType, size_t size) const
{
    if(elementType<0 || elementType>MAX_SCALAR_TYPE) {
        std::ostringstream strm("Can't construct fixed ScalarArray from invalid ScalarType ");
        strm << elementType;
        THROW_EXCEPTION2(std::invalid_argument, strm.str());
    }

    // TODO use std::make_shared
    std::tr1::shared_ptr<ScalarArray> s(new FixedScalarArray(elementType, size), Field::Deleter());
    ScalarArrayConstPtr sa = s;
    return sa;
}

ScalarArrayConstPtr FieldCreate::createBoundedScalarArray(ScalarType elementType, size_t size) const
{
    if(elementType<0 || elementType>MAX_SCALAR_TYPE) {
        std::ostringstream strm("Can't construct bounded ScalarArray from invalid ScalarType ");
        strm << elementType;
        THROW_EXCEPTION2(std::invalid_argument, strm.str());
    }

    // TODO use std::make_shared
    std::tr1::shared_ptr<ScalarArray> s(new BoundedScalarArray(elementType, size), Field::Deleter());
    ScalarArrayConstPtr sa = s;
    return sa;
}

StructureConstPtr FieldCreate::createStructure () const
{
      StringArray fieldNames;
      FieldConstPtrArray fields;
      return createStructure(fieldNames,fields);
}

namespace {
bool xisalnum(char c)
{
    return (c>='a' && c<='z') || (c>='A' && c<='Z') || (c>='0' && c<='9');
}

void validateFieldName(const std::string& n)
{
    // enforce [A-Za-z_][A-Za-z0-9_]*
    if(n.size()==0)
        throw std::invalid_argument("zero length field names not allowed");
    if(n[0]>='0' && n[0]<='9') {
        std::ostringstream msg;
        msg<<"Field name \""<<n<<"\" must begin with A-Z, a-z, or '_'";
        throw std::invalid_argument(msg.str());
    }
    for(size_t i=0, N=n.size(); i<N; i++)
    {
        char c = n[i];
        if(xisalnum(c)) {}
        else {
            switch(c){
            case '_':
                break;
            default:
            {
                std::ostringstream msg;
                msg<<"Invalid charactor '"<<c<<"' ("<<(int)c<<") in field name \""<<n<<"\" "
                     "must be A-Z, a-z, 0-9, or '_'";
                throw std::invalid_argument(msg.str());
            }
            }
        }
    }
}

void validateFieldNames(const StringArray& l)
{
    for(StringArray::const_iterator it=l.begin(), end=l.end(); it!=end; ++it)
        validateFieldName(*it);
}
}

StructureConstPtr FieldCreate::createStructure (
    StringArray const & fieldNames,FieldConstPtrArray const & fields) const
{
      validateFieldNames(fieldNames);
      // TODO use std::make_shared
      std::tr1::shared_ptr<Structure> sp(new Structure(fieldNames,fields), Field::Deleter());
      StructureConstPtr structure = sp;
      return structure;
}

StructureConstPtr FieldCreate::createStructure (
    string const & id,
    StringArray const & fieldNames,
    FieldConstPtrArray const & fields) const
{
      validateFieldNames(fieldNames);
      // TODO use std::make_shared
      std::tr1::shared_ptr<Structure> sp(new Structure(fieldNames,fields,id), Field::Deleter());
      StructureConstPtr structure = sp;
      return structure;
}

StructureArrayConstPtr FieldCreate::createStructureArray(
    StructureConstPtr const & structure) const
{
     // TODO use std::make_shared
     std::tr1::shared_ptr<StructureArray> sp(new StructureArray(structure), Field::Deleter());
     StructureArrayConstPtr structureArray = sp;
     return structureArray;
}

UnionConstPtr FieldCreate::createUnion (
    StringArray const & fieldNames,FieldConstPtrArray const & fields) const
{
      validateFieldNames(fieldNames);
      // TODO use std::make_shared
      std::tr1::shared_ptr<Union> sp(new Union(fieldNames,fields), Field::Deleter());
      UnionConstPtr punion = sp;
      return punion;
}

UnionConstPtr FieldCreate::createUnion (
    string const & id,
    StringArray const & fieldNames,
    FieldConstPtrArray const & fields) const
{
      validateFieldNames(fieldNames);
      // TODO use std::make_shared
      std::tr1::shared_ptr<Union> sp(new Union(fieldNames,fields,id), Field::Deleter());
      UnionConstPtr punion = sp;
      return punion;
}

UnionConstPtr FieldCreate::createVariantUnion () const
{
    return variantUnion;
}

UnionArrayConstPtr FieldCreate::createUnionArray(
    UnionConstPtr const & punion) const
{
     // TODO use std::make_shared 
     std::tr1::shared_ptr<UnionArray> sp(new UnionArray(punion), Field::Deleter());
     UnionArrayConstPtr unionArray = sp;
     return unionArray;
}

UnionArrayConstPtr FieldCreate::createVariantUnionArray () const
{
    return variantUnionArray;
}

StructureConstPtr FieldCreate::appendField(
    StructureConstPtr const & structure,
    string const & fieldName,
    FieldConstPtr const & field) const
{
    StringArray const & oldNames = structure->getFieldNames();
    FieldConstPtrArray const & oldFields = structure->getFields();
    size_t oldLen = oldNames.size();
    StringArray newNames(oldLen+1);
    FieldConstPtrArray newFields(oldLen+1);
    for(size_t i = 0; i<oldLen; i++) {
        newNames[i] = oldNames[i];
        newFields[i] = oldFields[i];
    }
    newNames[oldLen] = fieldName;
    newFields[oldLen] = field;
    return createStructure(structure->getID(),newNames,newFields);
}

StructureConstPtr FieldCreate::appendFields(
    StructureConstPtr const & structure,
    StringArray const & fieldNames,
    FieldConstPtrArray const & fields) const
{
    validateFieldNames(fieldNames);
    StringArray const & oldNames = structure->getFieldNames();
    FieldConstPtrArray const & oldFields = structure->getFields();
    size_t oldLen = oldNames.size();
    size_t extra = fieldNames.size();
    StringArray newNames(oldLen+extra);
    FieldConstPtrArray newFields(oldLen+extra);
    for(size_t i = 0; i<oldLen; i++) {
        newNames[i] = oldNames[i];
        newFields[i] = oldFields[i];
    }
    for(size_t i = 0; i<extra; i++) {
        newNames[oldLen +i] = fieldNames[i];
        newFields[oldLen +i] = fields[i];
    }
    return createStructure(structure->getID(),newNames,newFields);
}


static int decodeScalar(int8 code)
{
    static const int integerLUT[] =
    {
        pvByte,   // 8-bits
        pvShort,  // 16-bits
        pvInt,    // 32-bits
        pvLong,   // 64-bits
        pvUByte,  // unsigned 8-bits
        pvUShort, // unsigned 16-bits
        pvUInt,   // unsigned 32-bits
        pvULong   // unsigned 64-bits
    };
    
    static const int floatLUT[] =
    {
        -1, // reserved
        -1, // 16-bits
        pvFloat,   // 32-bits
        pvDouble,  // 64-bits
        -1,
        -1,
        -1,
        -1
    };
    // bits 7-5
    switch (code >> 5)
    {
    case 0: return pvBoolean;
    case 1: return integerLUT[code & 0x07];
    case 2: return floatLUT[code & 0x07];
    case 3: return pvString;
    default: return -1;
    }
}

FieldConstPtr FieldCreate::deserialize(ByteBuffer* buffer, DeserializableControl* control) const
{
    control->ensureData(1);
    int8 code = buffer->getByte();
    if (code == -1)
        return FieldConstPtr();

    int typeCode = code & 0xE7;
    int scalarOrArray = code & 0x18;
    bool notArray = (scalarOrArray == 0);
    if (notArray)
    {
        if (typeCode < 0x80)
        {
            // Type type = Type.scalar;
            int scalarType = decodeScalar(code);
            if (scalarType == -1)
                throw std::invalid_argument("invalid scalar type encoding");
            return scalars[scalarType];
        }
        else if (typeCode == 0x80)
        {
            // Type type = Type.structure;
            return deserializeStructureField(this, buffer, control);
        }
        else if (typeCode == 0x81)
        {
            // Type type = Type.union;
            return deserializeUnionField(this, buffer, control);
        }
        else if (typeCode == 0x82)
        {
            // Type type = Type.union; variant union (aka any type)
            return variantUnion;
        }
        else if (typeCode == 0x83)
        {
            // TODO cache?
            // bounded string

            size_t size = SerializeHelper::readSize(buffer, control);

            // TODO use std::make_shared
            std::tr1::shared_ptr<Field> sp(
                        new BoundedString(size),
                        Field::Deleter());
            FieldConstPtr p = sp;
            return p;
        }
        else
            throw std::invalid_argument("invalid type encoding");
    }
    else // array
    {
        bool isVariable = (scalarOrArray == 0x08);
        // bounded == 0x10;
        bool isFixed = (scalarOrArray == 0x18);

        size_t size = (isVariable ? 0 : SerializeHelper::readSize(buffer, control));

        if (typeCode < 0x80)
        {
            // Type type = Type.scalarArray;
            int scalarType = decodeScalar(code);
            if (scalarType == -1)
                throw std::invalid_argument("invalid scalarArray type encoding");
            if (isVariable)
                return scalarArrays[scalarType];
            else if (isFixed)
            {
                // TODO use std::make_shared
                std::tr1::shared_ptr<Field> sp(
                            new FixedScalarArray(static_cast<epics::pvData::ScalarType>(scalarType), size),
                            Field::Deleter());
                FieldConstPtr p = sp;
                return p;
            }
            else
            {
                // TODO use std::make_shared
                std::tr1::shared_ptr<Field> sp(
                            new BoundedScalarArray(static_cast<epics::pvData::ScalarType>(scalarType), size),
                            Field::Deleter());
                FieldConstPtr p = sp;
                return p;
            }
        }
        else if (typeCode == 0x80)
        {
            // TODO fixed and bounded array support
            if (!isVariable)
                throw std::invalid_argument("fixed and bounded structure array not supported");

            // Type type = Type.structureArray;
            StructureConstPtr elementStructure = std::tr1::static_pointer_cast<const Structure>(control->cachedDeserialize(buffer));
            // TODO use std::make_shared
            std::tr1::shared_ptr<Field> sp(new StructureArray(elementStructure), Field::Deleter());
            FieldConstPtr p = sp;
            return p;
        }
        else if (typeCode == 0x81)
        {
            // TODO fixed and bounded array support
            if (!isVariable)
                throw std::invalid_argument("fixed and bounded structure array not supported");

            // Type type = Type.unionArray;
            UnionConstPtr elementUnion = std::tr1::static_pointer_cast<const Union>(control->cachedDeserialize(buffer));
            // TODO use std::make_shared
            std::tr1::shared_ptr<Field> sp(new UnionArray(elementUnion), Field::Deleter());
            FieldConstPtr p = sp;
            return p;
        }
        else if (typeCode == 0x82)
        {
            // TODO fixed and bounded array support
            if (!isVariable)
                throw std::invalid_argument("fixed and bounded structure array not supported");

            // Type type = Type.unionArray; variant union (aka any type)
            return variantUnionArray;
        }
        else
            throw std::invalid_argument("invalid type encoding");
    }
}

// TODO replace with non-locking singleton pattern
FieldCreatePtr FieldCreate::getFieldCreate()
{
	LOCAL_STATIC_LOCK;
	static FieldCreatePtr fieldCreate;
	static Mutex mutex;

	Lock xx(mutex);
    if(fieldCreate.get()==0) fieldCreate = FieldCreatePtr(new FieldCreate());
    return fieldCreate;
}

FieldCreate::FieldCreate()
{
    for (int i = 0; i <= MAX_SCALAR_TYPE; i++)
    {
        // TODO use std::make_shared
        std::tr1::shared_ptr<Scalar> sp(new Scalar(static_cast<ScalarType>(i)), Field::Deleter());
        ScalarConstPtr p = sp;
        scalars.push_back(p);

        // TODO use std::make_shared
        std::tr1::shared_ptr<ScalarArray> spa(new ScalarArray(static_cast<ScalarType>(i)), Field::Deleter());
        ScalarArrayConstPtr pa = spa;
        scalarArrays.push_back(spa);
    }

    // TODO use std::make_shared
    std::tr1::shared_ptr<Union> su(new Union(), Field::Deleter());
    variantUnion = su;

    // TODO use std::make_shared
    std::tr1::shared_ptr<UnionArray> sua(new UnionArray(variantUnion), Field::Deleter());
    variantUnionArray = sua;
}

FieldCreatePtr getFieldCreate() {
    return FieldCreate::getFieldCreate();
}

}}

namespace std{
    std::ostream& operator<<(std::ostream& o, const epics::pvData::Field *ptr)
    {
        if(ptr) return o << *ptr;
        return o << "nullptr";
    }
}

