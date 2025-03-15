#include <epicsExport.h>
#include <initHooks.h>
#include <iocsh.h>

#include "PollGroup.h"
#include "s7nodaveAsyn.h"

/*
 * This file registers functions for iocsh and also creates an IOC init hook.
 */


/*
 * Structures needed for the s7nodaveConfigureIsoTcpPort command.
 */
static const iocshArg iocshS7nodaveConfigureIsoTcpPortArg0 = {
    "port name",
    iocshArgString
};

static const iocshArg iocshS7nodaveConfigureIsoTcpPortArg1 = {
    "PLC host[:port number]",
    iocshArgString
};

static const iocshArg iocshS7nodaveConfigureIsoTcpPortArg2 = {
    "PLC rack number",
    iocshArgInt
};

static const iocshArg iocshS7nodaveConfigureIsoTcpPortArg3 = {
    "PLC slot number",
    iocshArgInt
};

static const iocshArg iocshS7nodaveConfigureIsoTcpPortArg4 = {
    "priority",
    iocshArgInt
};

static const iocshArg *const iocshS7nodaveConfigureIsoTcpPortArgs[] = {
    &iocshS7nodaveConfigureIsoTcpPortArg0,
    &iocshS7nodaveConfigureIsoTcpPortArg1,
    &iocshS7nodaveConfigureIsoTcpPortArg2,
    &iocshS7nodaveConfigureIsoTcpPortArg3,
    &iocshS7nodaveConfigureIsoTcpPortArg4
};

static const iocshFuncDef iocshS7nodaveConfigureIsoTcpPortFuncDef = {
    "s7nodaveConfigureIsoTcpPort",
    5,
    iocshS7nodaveConfigureIsoTcpPortArgs
};

static void iocshS7nodaveConfigureIsoTcpPortFunc(const iocshArgBuf *args) {
    char *portName = args[0].sval;
    char *plcHostname = args[1].sval;
    int plcRack = args[2].ival;
    int plcSlot = args[3].ival;
    unsigned int priority = args[4].ival;
    s7nodaveConfigureIsoTcpPort(portName, plcHostname, plcRack, plcSlot, priority);
}

/*
 * Structures needed for the s7nodaveConfigurePollGroup command.
 */
static const iocshArg iocshS7nodaveConfigurePollGroupArg0 = {
    "port name",
    iocshArgString
};

static const iocshArg iocshS7nodaveConfigurePollGroupArg1 = {
    "poll-group name",
    iocshArgString
};

static const iocshArg iocshS7nodaveConfigurePollGroupArg2 = {
    "polling interval (seconds)",
    iocshArgDouble
};

static const iocshArg iocshS7nodaveConfigurePollGroupArg3 = {
    "priority",
    iocshArgInt
};

static const iocshArg *const iocshS7nodaveConfigurePollGroupArgs[] = {
    &iocshS7nodaveConfigurePollGroupArg0,
    &iocshS7nodaveConfigurePollGroupArg1,
    &iocshS7nodaveConfigurePollGroupArg2,
    &iocshS7nodaveConfigurePollGroupArg3
};

static const iocshFuncDef iocshS7nodaveConfigurePollGroupFuncDef = {
    "s7nodaveConfigurePollGroup",
    3,
    iocshS7nodaveConfigurePollGroupArgs
};

static void iocshS7nodaveConfigurePollGroupFunc(const iocshArgBuf *args) {
    char *portName = args[0].sval;
    char *pollGroupName = args[1].sval;
    double pollingInterval = args[2].dval;
    int priority = args[3].ival;
    s7nodaveConfigurePollGroup(portName, pollGroupName, pollingInterval, priority);
}

/*
 * Function that is executed each time the initialization state of the IOC
 * changes.
 */
static void initHookFunc(initHookState state) {
    if (state == initHookAfterIocRunning) {
        // Start the poll groups.
        s7nodaveStartPollGroups();
    }
}

/*
 * Registrar that registers the iocsh commands and the init hook function.
 */
static void s7nodaveRegistrar() {
    iocshRegister(&iocshS7nodaveConfigureIsoTcpPortFuncDef, iocshS7nodaveConfigureIsoTcpPortFunc);
    iocshRegister(&iocshS7nodaveConfigurePollGroupFuncDef, iocshS7nodaveConfigurePollGroupFunc);
    initHookRegister(initHookFunc);
}

epicsExportRegistrar(s7nodaveRegistrar);
