/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
 */

/**
 * @author mrk
 * @date 2013.07.24
 */

/* Author: Marty Kraimer */
#include <pv/pvDatabase.h>
#include <pv/standardField.h>
#include <pv/standardPVField.h>
#include <pv/ntscalar.h>
#include <pv/ntscalarArray.h>
#include <pv/ntunion.h>
#include <pv/ntenum.h>

#include <pv/channelProviderLocal.h>

#define epicsExportSharedSymbols
#include "pvaRecord.h"

using namespace std;
using namespace epics::pvData;
using namespace epics::nt;
using namespace epics::pvAccess;
using namespace epics::pvDatabase;

using namespace epics::pvarecord::database;

static FieldCreatePtr fieldCreate = getFieldCreate();
static StandardFieldPtr standardField = getStandardField();
static PVDataCreatePtr pvDataCreate = getPVDataCreate();
static StandardPVFieldPtr standardPVField = getStandardPVField();

static void createStructureArrayRecord(
    PVDatabasePtr const &master,
    string const &recordName)
{
    StructureConstPtr top = fieldCreate->createFieldBuilder()->
         addNestedStructureArray("value")->
             add("name",pvString)->
             add("value",pvString)->
             endNested()->
         createStructure();
    PVStructurePtr pvStructure = pvDataCreate->createPVStructure(top);
    PVRecordPtr pvRecord = PVRecord::create(recordName,pvStructure);
    bool result = master->addRecord(pvRecord); 
    if(!result) cout<< "record " << recordName << " not added" << endl;
}

static void createRestrictedUnionRecord(
    PVDatabasePtr const &master,
    string const &recordName)
{
    StructureConstPtr point = fieldCreate->createFieldBuilder()->
         add("x",pvDouble)->
         add("y",pvDouble)->
         createStructure();
    
   UnionConstPtr union_t = getFieldCreate()->createFieldBuilder()->
        add("string",pvString)->
        addArray("stringArray",pvString)->
        add("point",point) ->
        createUnion();

    UnionConstPtr u = getFieldCreate()->createFieldBuilder()->
        add("string",pvString)->
        addArray("stringArray",pvString)->
        add("point",point) ->
        add("union_t",union_t) ->
        createUnion();

    PVStructurePtr pvStructure= NTUnion::createBuilder()->
        value(u)->
        addTimeStamp()->
        createPVStructure();
    PVRecordPtr pvRecord = PVRecord::create(recordName,pvStructure);
    bool result = master->addRecord(pvRecord);
    if(!result) cout<< "record " << recordName << " not added" << endl;
}

static void createVariantUnionRecord(
    PVDatabasePtr const &master,
    string const &recordName)
{
    
    PVStructurePtr pvStructure= NTUnion::createBuilder()->
        addTimeStamp()->
        createPVStructure();
    PVRecordPtr pvRecord = PVRecord::create(recordName,pvStructure);
    bool result = master->addRecord(pvRecord);
    if(!result) cout<< "record " << recordName << " not added" << endl;
}

static void createRestrictedUnionArrayRecord(
    PVDatabasePtr const &master,
    string const &recordName)
{
    StructureConstPtr top = fieldCreate->createFieldBuilder()->
         add("timeStamp",standardField->timeStamp()) ->
         addNestedUnionArray("value")->
             add("string",pvString)->
             addArray("stringArray",pvString)->
             endNested()->
         createStructure();
    PVStructurePtr pvStructure = pvDataCreate->createPVStructure(top);
    PVRecordPtr pvRecord = PVRecord::create(recordName,pvStructure);
    bool result = master->addRecord(pvRecord);
    if(!result) cout<< "record " << recordName << " not added" << endl;
}

static void createVariantUnionArrayRecord(
    PVDatabasePtr const &master,
    string const &recordName)
{
    StructureConstPtr top = fieldCreate->createFieldBuilder()->
         add("timeStamp",standardField->timeStamp()) ->
         addArray("value",fieldCreate->createVariantUnion())->
         createStructure();
    PVStructurePtr pvStructure = pvDataCreate->createPVStructure(top);
    PVRecordPtr pvRecord = PVRecord::create(recordName,pvStructure);
    bool result = master->addRecord(pvRecord);
    if(!result) cout<< "record " << recordName << " not added" << endl;
}

static void createBigRecord(
    PVDatabasePtr const &master,
    string const &recordName)
{
    StructureConstPtr top = fieldCreate->createFieldBuilder()->
        add("timeStamp",standardField->timeStamp()) ->
        addNestedStructure("scalar") ->
            addNestedStructure("boolean") ->
                add("value",pvBoolean) ->
            endNested()->
            addNestedStructure("byte") ->
                add("value",pvByte) ->
            endNested()->
            addNestedStructure("long") ->
                add("value",pvLong) ->
            endNested()->
            addNestedStructure("double") ->
                add("value",pvDouble) ->
            endNested()->
            addNestedStructure("string") ->
                add("value",pvString) ->
            endNested()->
        endNested()->
        addNestedStructure("scalarArray") ->
            addNestedStructure("boolean") ->
                addArray("value",pvBoolean) ->
            endNested()->
            addNestedStructure("byte") ->
                addArray("value",pvByte) ->
            endNested()->
            addNestedStructure("long") ->
                addArray("value",pvLong) ->
            endNested()->
            addNestedStructure("double") ->
                addArray("value",pvDouble) ->
            endNested()->
            addNestedStructure("string") ->
                addArray("value",pvString) ->
            endNested()->
        endNested()->
        addNestedStructureArray("structureArray")->
            add("name",pvString)->
            add("value",pvString)->
        endNested()->
        addNestedUnion("restrictedUnion")->
            add("string",pvString)->
            addArray("stringArray",pvString)->
        endNested()->
        add("variantUnion",fieldCreate->createVariantUnion())->
        createStructure();
    PVStructurePtr pvStructure = pvDataCreate->createPVStructure(top);
    PVRecordPtr pvRecord = PVRecord::create(recordName,pvStructure);
    bool result = master->addRecord(pvRecord); 
    if(!result) cout<< "record " << recordName << " not added" << endl;
}

void PVARecord::create()
{
    PVDatabasePtr master = PVDatabase::getMaster();
    bool result(false);
    NTEnumBuilderPtr ntEnumBuilder = NTEnum::createBuilder();
    PVStructurePtr pvStructure = ntEnumBuilder->
        addAlarm()->
        addTimeStamp()->
        createPVStructure();
    shared_vector<string> choices(2);
    choices[0] = "zero";
    choices[1] = "one";
    PVStringArrayPtr pvChoices = pvStructure->getSubField<PVStringArray>("value.choices");
    pvChoices->replace(freeze(choices));
    result = master->addRecord(PVRecord::create("PVRenum",pvStructure));
    if(!result) cout<< "record PVRenum not added\n";

    createStructureArrayRecord(master,"PVRstructureArray");
    createRestrictedUnionRecord(master,"PVRrestrictedUnion");
    createVariantUnionRecord(master,"PVRvariantUnion");
    createRestrictedUnionArrayRecord(master,"PVRrestrictedUnionArray");
    createVariantUnionArrayRecord(master,"PVRvariantUnionArray");
    createBigRecord(master,"PVRBigRecord");

}

