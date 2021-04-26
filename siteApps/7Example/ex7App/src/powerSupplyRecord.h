/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
 */

/**
 * @author mrk
 * @date 2013.04.02
 */
#ifndef POWERSUPPLYRECORD_H
#define POWERSUPPLYRECORD_H


#include <pv/timeStamp.h>
#include <pv/alarm.h>
#include <pv/pvTimeStamp.h>
#include <pv/pvAlarm.h>
#include <pv/pvDatabase.h>

#include <shareLib.h>

namespace epics { namespace example { namespace powerSupply {

class PowerSupplyRecord;
typedef std::tr1::shared_ptr<PowerSupplyRecord> PowerSupplyRecordPtr;

class epicsShareClass PowerSupplyRecord :
    public epics::pvDatabase::PVRecord
{
public:
    POINTER_DEFINITIONS(PowerSupplyRecord);
    static PowerSupplyRecordPtr create(
        std::string const & recordName);
    virtual ~PowerSupplyRecord() {}
    virtual bool init() {return false;}
    virtual void process();
    
private:
    PowerSupplyRecord(std::string const & recordName,
        epics::pvData::PVStructurePtr const & pvStructure);
    void initPvt();

    epics::pvData::PVDoublePtr pvCurrent;
    epics::pvData::PVDoublePtr pvPower;
    epics::pvData::PVDoublePtr pvVoltage;
    epics::pvData::PVAlarm pvAlarm;
    epics::pvData::Alarm alarm;
};


}}}

#endif  /* POWERSUPPLYRECORD_H */
