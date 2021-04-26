/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
 */

/**
 * @author mrk
 * @date 2013.04.02
 */
#include <iocsh.h>
#include <pv/pvDatabase.h>
#include <pv/pvStructureCopy.h>
#include <pv/timeStamp.h>
#include <pv/standardField.h>
#include <pv/pvAlarm.h>
#include <pv/channelProviderLocal.h>

// The following must be the last include for code database uses
#include <epicsExport.h>
#define epicsExportSharedSymbols
//#include "powerSupply/powerSupplyRecord.h"
#include "powerSupplyRecord.h"

using namespace epics::pvData;
using namespace epics::pvDatabase;
using std::string;
using std::cout;
using std::cerr;
using std::endl;

namespace epics { namespace example { namespace powerSupply {

PowerSupplyRecordPtr PowerSupplyRecord::create(
    string const & recordName)
{
    FieldCreatePtr fieldCreate = getFieldCreate();
    StandardFieldPtr standardField = getStandardField();
    PVDataCreatePtr pvDataCreate = getPVDataCreate();

    StructureConstPtr  topStructure = fieldCreate->createFieldBuilder()->
            add("alarm",standardField->alarm()) ->
            add("timeStamp",standardField->timeStamp()) ->
            addNestedStructure("power") ->
               add("value",pvDouble) ->
               endNested()->
            addNestedStructure("voltage") ->
               add("value",pvDouble) ->
               endNested()->
            addNestedStructure("current") ->
               add("value",pvDouble) ->
               endNested()->
            createStructure();
    PVStructurePtr pvStructure = pvDataCreate->createPVStructure(topStructure);
    PowerSupplyRecordPtr pvRecord(
        new PowerSupplyRecord(recordName,pvStructure));
    pvRecord->initPvt();
    return pvRecord;
}

PowerSupplyRecord::PowerSupplyRecord(
    string const & recordName,
    PVStructurePtr const & pvStructure)
: PVRecord(recordName,pvStructure)
{
}


void PowerSupplyRecord::initPvt()
{
    initPVRecord();
    PVStructurePtr pvStructure = getPVStructure();
    PVFieldPtr pvField;
    pvField = pvStructure->getSubField("alarm");
    pvAlarm.attach(pvField);
    pvCurrent = pvStructure->getSubField<PVDouble>("current.value");
    pvVoltage = pvStructure->getSubField<PVDouble>("voltage.value");
    pvPower = pvStructure->getSubField<PVDouble>("power.value");
    alarm.setMessage("bad voltage");
    alarm.setSeverity(majorAlarm);
    pvAlarm.set(alarm);
}

void PowerSupplyRecord::process()
{
    double voltage = pvVoltage->get();
    double power = pvPower->get();
    if(voltage<1e-3 && voltage>-1e-3) {
        alarm.setMessage("bad voltage");
        alarm.setSeverity(majorAlarm);
        pvAlarm.set(alarm);
        throw std::runtime_error("bad voltage exception");
    }
    double current = power/voltage;
    pvCurrent->put(current);
    pvAlarm.get(alarm);
    if(alarm.getSeverity()!=noAlarm) {
        alarm.setMessage("");
        alarm.setSeverity(noAlarm);
        pvAlarm.set(alarm);
    }
    PVRecord::process();
}
}}}

static const iocshArg testArg0 = { "recordName", iocshArgString };
static const iocshArg *testArgs[] = {
    &testArg0};

static const iocshFuncDef powerSupplyFuncDef = {
    "powerSupplyRecord", 1, testArgs};
static void powerSupplyCallFunc(const iocshArgBuf *args)
{
    char *recordName = args[0].sval;
    if(!recordName) {
        throw std::runtime_error("powerSupplyRecord invalid number of arguments");
    }
    epics::example::powerSupply::PowerSupplyRecordPtr record
       = epics::example::powerSupply::PowerSupplyRecord::create(recordName);
    bool result = PVDatabase::getMaster()->addRecord(record);
    if(!result) cout << "recordname" << " not added" << endl;
}

static void powerSupplyRecord(void)
{
    static int firstTime = 1;
    if (firstTime) {
        firstTime = 0;
        iocshRegister(&powerSupplyFuncDef, powerSupplyCallFunc);
    }
}

extern "C" {
    epicsExportRegistrar(powerSupplyRecord);
}
