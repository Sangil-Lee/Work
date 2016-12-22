// Copyright information and license terms for this software can be
// found in the file LICENSE that is included with the distribution

#ifndef PY_PV_DATA_UTILITY_H
#define PY_PV_DATA_UTILITY_H

#include <string>
#include "pv/pvData.h"
#include "boost/python/str.hpp"
#include "boost/python/extract.hpp"
#include "boost/python/object.hpp"
#include "boost/python/list.hpp"
#include "boost/python/dict.hpp"
#include "boost/python/tuple.hpp"

#include "PyUtility.h"
#include "InvalidDataType.h"

namespace PyPvDataUtility
{

//
// Checks
//
void checkFieldExists(const std::string& fieldName, const epics::pvData::PVStructurePtr& pvStructurePtr);

//
// Field retrieval
//
std::string getValueOrSingleFieldName(const epics::pvData::PVStructurePtr& pvStructurePtr);
std::string getValueOrSelectedUnionFieldName(const epics::pvData::PVStructurePtr& pvStructurePtr);

epics::pvData::PVFieldPtr getSubField(const std::string& fieldName, const epics::pvData::PVStructurePtr& pvStructurePtr);

epics::pvData::FieldConstPtr getField(const std::string& fieldName, const epics::pvData::PVStructurePtr& pvStructurePtr);

epics::pvData::ScalarConstPtr getScalarField(const std::string& fieldName, const epics::pvData::PVStructurePtr& pvStructurePtr);

epics::pvData::PVScalarArrayPtr getScalarArrayField(const std::string& fieldName, epics::pvData::ScalarType scalarType, const epics::pvData::PVStructurePtr& pvStructurePtr);

epics::pvData::StructureConstPtr getStructure(const std::string& fieldName, const epics::pvData::PVStructurePtr& pvStructurePtr);

epics::pvData::PVStructurePtr getStructureField(const std::string& fieldName, const epics::pvData::PVStructurePtr& pvStructurePtr);

epics::pvData::PVStructureArrayPtr getStructureArrayField(const std::string& fieldName, const epics::pvData::PVStructurePtr& pvStructurePtr);

epics::pvData::PVUnionPtr getUnionField(const std::string& fieldName, const epics::pvData::PVStructurePtr& pvStructurePtr);

void setUnionField(const epics::pvData::PVFieldPtr& pvFrom, epics::pvData::PVUnionPtr pvUnion);

epics::pvData::PVUnionArrayPtr getUnionArrayField(const std::string& fieldName, const epics::pvData::PVStructurePtr& pvStructurePtr);

epics::pvData::PVBooleanPtr getBooleanField(const std::string& fieldName, const epics::pvData::PVStructurePtr& pvStructurePtr);

epics::pvData::PVBytePtr getByteField(const std::string& fieldName, const epics::pvData::PVStructurePtr& pvStructurePtr);

epics::pvData::PVUBytePtr getUByteField(const std::string& fieldName, const epics::pvData::PVStructurePtr& pvStructurePtr);

epics::pvData::PVShortPtr getShortField(const std::string& fieldName, const epics::pvData::PVStructurePtr& pvStructurePtr);

epics::pvData::PVUShortPtr getUShortField(const std::string& fieldName, const epics::pvData::PVStructurePtr& pvStructurePtr);

epics::pvData::PVIntPtr getIntField(const std::string& fieldName, const epics::pvData::PVStructurePtr& pvStructurePtr);

epics::pvData::PVUIntPtr getUIntField(const std::string& fieldName, const epics::pvData::PVStructurePtr& pvStructurePtr);

epics::pvData::PVLongPtr getLongField(const std::string& fieldName, const epics::pvData::PVStructurePtr& pvStructurePtr);

epics::pvData::PVULongPtr getULongField(const std::string& fieldName, const epics::pvData::PVStructurePtr& pvStructurePtr);

epics::pvData::PVFloatPtr getFloatField(const std::string& fieldName, const epics::pvData::PVStructurePtr& pvStructurePtr);

epics::pvData::PVDoublePtr getDoubleField(const std::string& fieldName, const epics::pvData::PVStructurePtr& pvStructurePtr);

epics::pvData::PVStringPtr getStringField(const std::string& fieldName, const epics::pvData::PVStructurePtr& pvStructurePtr);

//
// Field type retrieval
//
epics::pvData::Type getFieldType(const std::string& fieldName, const epics::pvData::PVStructurePtr& pvStructurePtr);

epics::pvData::ScalarType getScalarType(const std::string& fieldName, const epics::pvData::PVStructurePtr& pvStructurePtr);

epics::pvData::ScalarType getScalarArrayType(const std::string& fieldName, const epics::pvData::PVStructurePtr& pvStructurePtr);

//
// Conversion PY object => PV Field
//
void pyObjectToField(const boost::python::object& pyObject, const std::string& fieldName, epics::pvData::PVStructurePtr& pvStructurePtr);

//
// Conversion PY object => PV Scalar
//
void pyObjectToScalarField(const boost::python::object& pyObject, const std::string& fieldName, epics::pvData::PVStructurePtr& pvStructurePtr);

//
// Conversion PY object => PV Scalar Array
//
void pyObjectToScalarArrayField(const boost::python::object& pyObject, const std::string& fieldName, epics::pvData::PVStructurePtr& pvStructurePtr);

//
// Conversion PY object => PV Structure
//
void pyObjectToStructureField(const boost::python::object& pyObject, const std::string& fieldName, epics::pvData::PVStructurePtr& pvStructurePtr);

//
// Conversion PY object => PV Structure Array
//
void pyObjectToStructureArrayField(const boost::python::object& pyObject, const std::string& fieldName, epics::pvData::PVStructurePtr& pvStructurePtr);

//
// Conversion PY object => PV Union
//
void pyObjectToUnionField(const boost::python::object& pyObject, const std::string& fieldName, epics::pvData::PVStructurePtr& pvStructurePtr);

//
// Conversion PY object => PV Union Array
//
void pyObjectToUnionArrayField(const boost::python::object& pyObject, const std::string& fieldName, epics::pvData::PVStructurePtr& pvStructurePtr);

//
// Conversion PY [] => PV Scalar Array
//
void pyListToScalarArrayField(const boost::python::list& pyList, const std::string& fieldName, epics::pvData::PVStructurePtr& pvStructurePtr);

//
// Conversion PV Scalar Array => PY []
//
void scalarArrayFieldToPyList(const std::string& fieldName, const epics::pvData::PVStructurePtr& pvStructurePtr, boost::python::list& pyList);

//
// Conversion PV String Array => PY []
//
void stringArrayToPyList(const epics::pvData::StringArray& stringArray, boost::python::list& pyList);

//
// Conversion PY {} => PV Structure
//
void pyDictToStructure(const boost::python::dict& pyDict, epics::pvData::PVStructurePtr& pvStructurePtr);

void pyDictToStructureField(const boost::python::dict& pyDict, const std::string& fieldName, epics::pvData::PVStructurePtr& pvStructurePtr);

//
// Conversion PY () => PV Union
//
void pyTupleToUnionField(const boost::python::tuple& pyTuple, const std::string& fieldName, epics::pvData::PVStructurePtr& pvStructurePtr);

//
// Conversion PY {} => PV Union
//
void pyDictToUnion(const boost::python::dict& pyDict, epics::pvData::PVUnionPtr& pvUnionPtr);

//
// Conversion PY {} => PV Union Field
//
void pyDictToUnionField(const boost::python::dict& pyDict, const std::string& fieldName, epics::pvData::PVStructurePtr& pvStructurePtr);

//
// Conversion PY [{}] => PV Union Array
//
void pyListToUnionArrayField(const boost::python::list& pyList, const std::string& fieldName, epics::pvData::PVStructurePtr& pvStructurePtr);

//
//
// Conversion PY [{}] => PV Structure Array
//
void pyListToStructureArrayField(const boost::python::list& pyList, const std::string& fieldName, epics::pvData::PVStructurePtr& pvStructurePtr);

//
// Conversion PV Structure Array => PY [{}] 
//
void structureArrayFieldToPyList(const std::string& fieldName, const epics::pvData::PVStructurePtr& pvStructurePtr, boost::python::list& pyList);

//
// Conversion PV Structure => PY {}
//
void structureToPyDict(const epics::pvData::PVStructurePtr& pvStructurePtr, boost::python::dict& pyDict);

void structureFieldToPyDict(const std::string& fieldName, const epics::pvData::PVStructurePtr& pvStructurePtr, boost::python::dict& pyDict);

//
//
// Add PV Scalar => PY {}
// 
void addScalarFieldToDict(const std::string& fieldName, epics::pvData::ScalarType scalarType, const epics::pvData::PVStructurePtr& pvStructurePtr, boost::python::dict& pyDict);

//
// Add PV Scalar Array => PY {}
// 
void addScalarArrayFieldToDict(const std::string& fieldName, epics::pvData::ScalarType scalarType, const epics::pvData::PVStructurePtr& pvStructurePtr, boost::python::dict& pyDict);

//
// Add PV Structure => PY {}
// 
void addStructureFieldToDict(const std::string& fieldName, const epics::pvData::PVStructurePtr& pvStructurePtr, boost::python::dict& pyDict);

//
// Add PV Structure Array => PY {}
// 
void addStructureArrayFieldToDict(const std::string& fieldName, const epics::pvData::PVStructurePtr& pvStructurePtr, boost::python::dict& pyDict);

//
// Add PV Union => PY {}
// 
void addUnionFieldToDict(const std::string& fieldName, const epics::pvData::PVStructurePtr& pvStructurePtr, boost::python::dict& pyDict);

//
// Add PV Union Array => PY {}
// 
void addUnionArrayFieldToDict(const std::string& fieldName, const epics::pvData::PVStructurePtr& pvStructurePtr, boost::python::dict& pyDict);

//
// Conversion Structure => PY {}
//
void structureToPyDict(const epics::pvData::StructureConstPtr& structurePtr, boost::python::dict& pyDict);

//
// Conversion Union => PY ()
//
boost::python::tuple unionToPyTuple(const epics::pvData::UnionConstPtr& unionPtr);

//
// Conversion Field => PY {}
//
void fieldToPyDict(const epics::pvData::FieldConstPtr& fieldPtr, const std::string& fieldName, boost::python::dict& pyDict);

//
// Copy PV Structure => PV Structure
//
void copyStructureToStructure(const epics::pvData::PVStructurePtr& srcPvStructurePtr, epics::pvData::PVStructurePtr& destPvStructurePtr);
void copyStructureToStructure(const std::string& fieldName, const epics::pvData::PVStructurePtr& srcPvStructurePtr, epics::pvData::PVStructurePtr& destPvStructurePtr);

//
// Copy PV Structure Array => PV Structure
//
void copyStructureArrayToStructure(const std::string& fieldName, const epics::pvData::PVStructurePtr& srcPvStructurePtr, epics::pvData::PVStructurePtr& destPvStructurePtr);

// 
// Copy PV Scalar to PV Structure
//
void copyScalarToStructure(const std::string& fieldName, epics::pvData::ScalarType scalarType, const epics::pvData::PVStructurePtr& srcPvStructurePtr, epics::pvData::PVStructurePtr& destPvStructurePtr);

// 
// Copy PV Scalar Array to PV Structure
//
void copyScalarArrayToStructure(const std::string& fieldName, epics::pvData::ScalarType scalarType, const epics::pvData::PVStructurePtr& srcPvStructurePtr, epics::pvData::PVStructurePtr& destPvStructurePtr);

// 
// Methods for creating structure
//
epics::pvData::StructureConstPtr createStructureFromDict(const boost::python::dict& pyDict, const std::string& structureId="");
epics::pvData::UnionConstPtr createUnionFromDict(const boost::python::dict& pyDict, const std::string& structureId="");
void updateFieldArrayFromDict(const boost::python::dict& pyDict, epics::pvData::FieldConstPtrArray& fields, epics::pvData::StringArray& names);
epics::pvData::PVStructurePtr createUnionPvStructure(const epics::pvData::PVUnionPtr& pvUnion, const std::string& fieldName); 
epics::pvData::PVStructurePtr createUnionFieldPvStructure(epics::pvData::UnionConstPtr unionPtr, const std::string& fieldName);

void addScalarField(const std::string& fieldName, epics::pvData::ScalarType scalarType, epics::pvData::FieldConstPtrArray& fields, epics::pvData::StringArray& names);
void addScalarArrayField(const std::string& fieldName, epics::pvData::ScalarType scalarType, epics::pvData::FieldConstPtrArray& fields, epics::pvData::StringArray& names);
void addStructureField(const std::string& fieldName, const boost::python::dict& pyDict, epics::pvData::FieldConstPtrArray& fields, epics::pvData::StringArray& names);
void addStructureArrayField(const std::string& fieldName, const boost::python::dict& pyDict, epics::pvData::FieldConstPtrArray& fields, epics::pvData::StringArray& names);
void addUnionField(const std::string& fieldName, const boost::python::dict& pyDict, epics::pvData::FieldConstPtrArray& fields, epics::pvData::StringArray& names);
void addUnionArrayField(const std::string& fieldName, const boost::python::dict& pyDict, epics::pvData::FieldConstPtrArray& fields, epics::pvData::StringArray& names);
void addVariantUnionField(const std::string& fieldName, epics::pvData::FieldConstPtrArray& fields, epics::pvData::StringArray& names);
void addVariantUnionArrayField(const std::string& fieldName, epics::pvData::FieldConstPtrArray& fields, epics::pvData::StringArray& names);

bool updateFieldArrayFromInt(const boost::python::object& pyObject, const std::string& fieldName, epics::pvData::FieldConstPtrArray& fields, epics::pvData::StringArray& names);
bool updateFieldArrayFromIntList(const boost::python::object& pyObject, const std::string& fieldName, epics::pvData::FieldConstPtrArray& fields, epics::pvData::StringArray& names);
bool updateFieldArrayFromDict(const boost::python::object& pyObject, const std::string& fieldName, epics::pvData::FieldConstPtrArray& fields, epics::pvData::StringArray& names);
bool updateFieldArrayFromDictList(const boost::python::object& pyObject, const std::string& fieldName, epics::pvData::FieldConstPtrArray& fields, epics::pvData::StringArray& names);
bool updateFieldArrayFromTuple(const boost::python::object& pyObject, const std::string& fieldName, epics::pvData::FieldConstPtrArray& fields, epics::pvData::StringArray& names);
bool updateFieldArrayFromTupleList(const boost::python::object& pyObject, const std::string& fieldName, epics::pvData::FieldConstPtrArray& fields, epics::pvData::StringArray& names);
bool updateFieldArrayFromPvObject(const boost::python::object& pyObject, const std::string& fieldName, epics::pvData::FieldConstPtrArray& fields, epics::pvData::StringArray& names);
bool updateFieldArrayFromPvObjectList(const boost::python::object& pyObject, const std::string& fieldName, epics::pvData::FieldConstPtrArray& fields, epics::pvData::StringArray& names);

//
// Extract union structure dict 
//
boost::python::dict extractUnionStructureDict(const boost::python::dict& pyDict);

//
// Template implementations
//

template<typename PvArrayType, typename CppType, typename PyType>
void pyListToScalarArrayField(const boost::python::list& pyList, const std::string& fieldName, epics::pvData::PVStructurePtr pvStructurePtr)
{
    int listSize = boost::python::len(pyList);
    std::tr1::shared_ptr<PvArrayType> valueArray = pvStructurePtr->getSubField<PvArrayType>(fieldName);
    typename PvArrayType::svector v(listSize);
    for (int i = 0; i < listSize; i++) {
        boost::python::extract<PyType> valueExtract(pyList[i]);
        if (valueExtract.check()) {
            v[i] = valueExtract();
        }
        else {
            throw InvalidDataType("Invalid data type for element %d", i);
        }
    }
    valueArray->setCapacity(listSize);
    valueArray->replace(freeze(v));
}

template<typename PvArrayType, typename CppType>
void scalarArrayToPyList(const epics::pvData::PVScalarArrayPtr& pvScalarArrayPtr, boost::python::list& pyList) 
{
    int nDataElements = pvScalarArrayPtr->getLength();
    typename PvArrayType::const_svector data;
    pvScalarArrayPtr->PVScalarArray::template getAs<CppType>(data);
    for (int i = 0; i < nDataElements; ++i) {
        pyList.append(data[i]);
    }
}


template<typename PvArrayType, typename CppType>
void copyScalarArrayToScalarArray(const epics::pvData::PVScalarArrayPtr& srcPvScalarArrayPtr, epics::pvData::PVScalarArrayPtr& destPvScalarArrayPtr)
{
    int nDataElements = srcPvScalarArrayPtr->getLength();
    typename PvArrayType::const_svector data;
    srcPvScalarArrayPtr->PVScalarArray::template getAs<CppType>(data);

    destPvScalarArrayPtr->setCapacity(nDataElements);
    destPvScalarArrayPtr->putFrom(data);
}

} // namespace PyPvDataUtility

#endif

