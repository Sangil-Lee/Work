/* ntfield.cpp */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * This software is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */

#include <pv/lock.h>

#define epicsExportSharedSymbols
#include <pv/ntfield.h>

using namespace epics::pvData;
using std::tr1::static_pointer_cast;

namespace epics { namespace nt {

NTFieldPtr NTField::get()
{
    static Mutex mutex;
    static NTFieldPtr ntstructureField;
    Lock xx(mutex);
    if(ntstructureField.get()==NULL) {
         ntstructureField = NTFieldPtr(new NTField());
    }
    return ntstructureField;
}

NTField::NTField()
: fieldCreate(getFieldCreate()),
  standardField(getStandardField())
{
}

bool NTField::isEnumerated(FieldConstPtr const & field)
{
   if(field->getType()!=structure) return false;
   StructureConstPtr structurePtr = static_pointer_cast<const Structure>(field);
   FieldConstPtrArray fields = structurePtr->getFields();
   StringArray names = structurePtr->getFieldNames();
   size_t n = structurePtr->getNumberFields();
   if(n!=2) return false;
   FieldConstPtr f = fields[0];
   if(names[0].compare("index")!=0) return false;
   if(f->getType()!=scalar) return false;
   ScalarConstPtr s = static_pointer_cast<const Scalar>(f);
   if(s->getScalarType()!=pvInt) return false;
   f = fields[1];
   if(names[1].compare("choices")!=0) return false;
   if(f->getType()!=scalarArray) return false;
   ScalarConstPtr sa = static_pointer_cast<const Scalar>(f);
   if(sa->getScalarType()!=pvString) return false;
   return true;
}

bool NTField::isTimeStamp(FieldConstPtr const & field)
{
   if(field->getType()!=structure) return false;
   StructureConstPtr structurePtr = static_pointer_cast<const Structure>(field);
   FieldConstPtrArray fields = structurePtr->getFields();
   StringArray names = structurePtr->getFieldNames();
   size_t  n = structurePtr->getNumberFields();
   if(n!=3) return false;
   FieldConstPtr f = fields[0];
   if(names[0].compare("secondsPastEpoch")!=0) return false;
   if(f->getType()!=scalar) return false;
   ScalarConstPtr s = static_pointer_cast<const Scalar>(f);
   if(s->getScalarType()!=pvLong) return false;
   f = fields[1];
   if(names[1].compare("nanoseconds")!=0) return false;
   if(f->getType()!=scalar) return false;
   s = static_pointer_cast<const Scalar>(f);
   if(s->getScalarType()!=pvInt) return false;
   f = fields[2];
   if(names[2].compare("userTag")!=0) return false;
   if(f->getType()!=scalar) return false;
   s = static_pointer_cast<const Scalar>(f);
   if(s->getScalarType()!=pvInt) return false;
   return true;
}

bool NTField::isAlarm(FieldConstPtr const & field)
{
   if(field->getType()!=structure) return false;
   StructureConstPtr structurePtr = static_pointer_cast<const Structure>(field);
   FieldConstPtrArray fields = structurePtr->getFields();
   StringArray names = structurePtr->getFieldNames();
   size_t n = structurePtr->getNumberFields();
   if(n!=3) return false;
   FieldConstPtr f = fields[0];
   if(names[0].compare("severity")!=0) return false;
   if(f->getType()!=scalar) return false;
   ScalarConstPtr s = static_pointer_cast<const Scalar>(f);
   if(s->getScalarType()!=pvInt) return false;
   f = fields[1];
   if(names[1].compare("status")!=0) return false;
   if(f->getType()!=scalar) return false;
   s = static_pointer_cast<const Scalar>(f);
   if(s->getScalarType()!=pvInt) return false;
   f = fields[2];
   if(names[2].compare("message")!=0) return false;
   if(f->getType()!=scalar) return false;
   s = static_pointer_cast<const Scalar>(f);
   if(s->getScalarType()!=pvString) return false;
   return true;
}

bool NTField::isDisplay(FieldConstPtr const & field)
{
   if(field->getType()!=structure) return false;
   StructureConstPtr structurePtr = static_pointer_cast<const Structure>(field);
   FieldConstPtrArray fields = structurePtr->getFields();
   StringArray names = structurePtr->getFieldNames();
   size_t n = structurePtr->getNumberFields();
   if(n!=5) return false;
   FieldConstPtr f = fields[0];
   if(names[0].compare("limitLow")!=0) return false;
   if(f->getType()!=scalar) return false;
   ScalarConstPtr s = static_pointer_cast<const Scalar>(f);
   if(s->getScalarType()!=pvDouble) return false;
   f = fields[1];
   if(names[1].compare("limitHigh")!=0) return false;
   if(f->getType()!=scalar) return false;
   s = static_pointer_cast<const Scalar>(f);
   if(s->getScalarType()!=pvDouble) return false;
   f = fields[2];
   if(names[2].compare("description")!=0) return false;
   if(f->getType()!=scalar) return false;
   s = static_pointer_cast<const Scalar>(f);
   if(s->getScalarType()!=pvString) return false;
   f = fields[3];
   if(names[3].compare("format")!=0) return false;
   if(f->getType()!=scalar) return false;
   s = static_pointer_cast<const Scalar>(f);
   if(s->getScalarType()!=pvString) return false;
   f = fields[4];
   if(names[4].compare("units")!=0) return false;
   if(f->getType()!=scalar) return false;
   s = static_pointer_cast<const Scalar>(f);
   if(s->getScalarType()!=pvString) return false;
   return true;
}

bool NTField::isAlarmLimit(FieldConstPtr const & field)
{
   if(field->getType()!=structure) return false;
   StructureConstPtr structurePtr = static_pointer_cast<const Structure>(field);
   FieldConstPtrArray fields = structurePtr->getFields();
   StringArray names = structurePtr->getFieldNames();
   size_t n = structurePtr->getNumberFields();
   if(n!=10) return false;
   FieldConstPtr f = fields[0];
   if(names[0].compare("active")!=0) return false;
   if(f->getType()!=scalar) return false;
   ScalarConstPtr s = static_pointer_cast<const Scalar>(f);
   if(s->getScalarType()!=pvBoolean) return false;
   f = fields[1];
   if(names[1].compare("lowAlarmLimit")!=0) return false;
   if(f->getType()!=scalar) return false;
   s = static_pointer_cast<const Scalar>(f);
   if(s->getScalarType()!=pvDouble) return false;
   f = fields[2];
   if(names[2].compare("lowWarningLimit")!=0) return false;
   if(f->getType()!=scalar) return false;
   s = static_pointer_cast<const Scalar>(f);
   if(s->getScalarType()!=pvDouble) return false;
   f = fields[3];
   if(names[3].compare("highWarningLimit")!=0) return false;
   if(f->getType()!=scalar) return false;
   s = static_pointer_cast<const Scalar>(f);
   if(s->getScalarType()!=pvDouble) return false;
   f = fields[4];
   if(names[4].compare("highAlarmLimit")!=0) return false;
   if(f->getType()!=scalar) return false;
   s = static_pointer_cast<const Scalar>(f);
   if(s->getScalarType()!=pvDouble) return false;
   f = fields[5];
   if(names[5].compare("lowAlarmSeverity")!=0) return false;
   if(f->getType()!=scalar) return false;
   s = static_pointer_cast<const Scalar>(f);
   if(s->getScalarType()!=pvInt) return false;
   f = fields[6];
   if(names[6].compare("lowWarningSeverity")!=0) return false;
   if(f->getType()!=scalar) return false;
   s = static_pointer_cast<const Scalar>(f);
   if(s->getScalarType()!=pvInt) return false;
   f = fields[7];
   if(names[7].compare("highWarningSeverity")!=0) return false;
   if(f->getType()!=scalar) return false;
   s = static_pointer_cast<const Scalar>(f);
   if(s->getScalarType()!=pvInt) return false;
   f = fields[8];
   if(names[8].compare("highAlarmSeverity")!=0) return false;
   if(f->getType()!=scalar) return false;
   s = static_pointer_cast<const Scalar>(f);
   if(s->getScalarType()!=pvInt) return false;
   f = fields[9];
   if(names[9].compare("hysteresis")!=0) return false;
   if(f->getType()!=scalar) return false;
   return true;
}

bool NTField::isControl(FieldConstPtr const & field)
{
   if(field->getType()!=structure) return false;
   StructureConstPtr structurePtr = static_pointer_cast<const Structure>(field);
   FieldConstPtrArray fields = structurePtr->getFields();
   StringArray names = structurePtr->getFieldNames();
   size_t n = structurePtr->getNumberFields();
   if(n!=3) return false;
   FieldConstPtr f = fields[0];
   if(names[0].compare("limitLow")!=0) return false;
   if(f->getType()!=scalar) return false;
   ScalarConstPtr s = static_pointer_cast<const Scalar>(f);
   if(s->getScalarType()!=pvDouble) return false;
   f = fields[1];
   if(names[1].compare("limitHigh")!=0) return false;
   if(f->getType()!=scalar) return false;
   s = static_pointer_cast<const Scalar>(f);
   if(s->getScalarType()!=pvDouble) return false;
   f = fields[2];
   if(names[2].compare("minStep")!=0) return false;
   if(f->getType()!=scalar) return false;
   s = static_pointer_cast<const Scalar>(f);
   if(s->getScalarType()!=pvDouble) return false;
   return true;
}

StructureConstPtr NTField::createEnumerated()
{
    return standardField->enumerated();
}

StructureConstPtr NTField::createTimeStamp()
{
    return standardField->timeStamp();
}

StructureConstPtr NTField::createAlarm()
{
    return standardField->alarm();
}

StructureConstPtr NTField::createDisplay()
{
    return standardField->display();
}

StructureConstPtr NTField::createControl()
{
    return standardField->control();
}

StructureArrayConstPtr NTField::createEnumeratedArray()
{
    return fieldCreate->createStructureArray(createEnumerated());
}

StructureArrayConstPtr NTField::createTimeStampArray()
{
    StructureConstPtr st = createTimeStamp();
    return fieldCreate->createStructureArray(st);
}

StructureArrayConstPtr NTField::createAlarmArray()
{
    StructureConstPtr st = createAlarm();
    return fieldCreate->createStructureArray(st);
}

PVNTFieldPtr PVNTField::get()
{
    static Mutex mutex;
    static PVNTFieldPtr pvntstructureField;
    Lock xx(mutex);
    if(pvntstructureField.get()==NULL) {
         pvntstructureField = PVNTFieldPtr(new PVNTField());
    }
    return pvntstructureField;
}

PVNTField::PVNTField()
: pvDataCreate(getPVDataCreate()),
  standardField(getStandardField()),
  standardPVField(getStandardPVField()),
  ntstructureField(NTField::get())
{
}


PVStructurePtr PVNTField::createEnumerated(
    StringArray const & choices)
{
    return standardPVField->enumerated(choices);
}

PVStructurePtr PVNTField::createTimeStamp()
{
    StructureConstPtr timeStamp = standardField->timeStamp();
    return pvDataCreate->createPVStructure(timeStamp);
}

PVStructurePtr PVNTField::createAlarm()
{
    StructureConstPtr alarm = standardField->alarm();
    return pvDataCreate->createPVStructure(alarm);
}

PVStructurePtr PVNTField::createDisplay()
{
    StructureConstPtr display = standardField->display();
    return pvDataCreate->createPVStructure(display);
}


PVStructurePtr PVNTField::createControl()
{
    StructureConstPtr control = standardField->control();
    return pvDataCreate->createPVStructure(control);
}

PVStructureArrayPtr PVNTField::createEnumeratedArray()
{
    StructureArrayConstPtr sa =
        ntstructureField->createEnumeratedArray();
    return pvDataCreate->createPVStructureArray(sa);
}

PVStructureArrayPtr PVNTField::createTimeStampArray()
{
    StructureArrayConstPtr sa =
         ntstructureField->createTimeStampArray();
    return pvDataCreate->createPVStructureArray(sa);
}

PVStructureArrayPtr PVNTField::createAlarmArray()
{
    StructureArrayConstPtr sa = ntstructureField->createAlarmArray();
    return pvDataCreate->createPVStructureArray(sa);
}

}}

