/*
 * Agilent E364XA device support
 *
 * This device is truly horrible.  It absolutely requires working flow
 * control -- and then uses DSR/DTR handshaking instead of RTS/CTS.
 *
 * The E364XA serial port:
 *     Requires TWO stop bits.
 *     Receives on pin 2.
 *     Transmits on pin 3.
 *     Drives DTR (pin 4) for for handshaking.
 *         The device to which an E364xA is connected must monitor this
 *         pin and inhibit transmission until it has been asserted.  The
 *         E364xA will experience communication errors after every command
 *         which sets and then reads back a value if this is not done.
 *     Monitors DSR (pin 6) for handshaking.
 *         The device to which an E364xA is connected must drive DSR high
 *         to allow the E364xA to transmit.  It is not sufficient to simply
 *         leave this line disconnected.
 *
 */

#include <epicsStdio.h>
#include <devCommonGpib.h>

/******************************************************************************
 *
 * The following define statements are used to declare the names to be used
 * for the dset tables.   
 *
 * A DSET_AI entry must be declared here and referenced in an application
 * database description file even if the device provides no AI records.
 *
 ******************************************************************************/
#define DSET_AI     devAie364xa
#define DSET_AO     devAoe364xa
#define DSET_BI     devBie364xa
#define DSET_BO     devBoe364xa
#define DSET_EV     devEve364xa
#define DSET_LI     devLie364xa
#define DSET_LO     devLoe364xa
#define DSET_MBBI   devMbbie364xa
#define DSET_MBBID  devMbbide364xa
#define DSET_MBBO   devMbboe364xa
#define DSET_MBBOD  devMbbode364xa
#define DSET_SI     devSie364xa
#define DSET_SO     devSoe364xa
#define DSET_WF     devWfe364xa

#include <devGpib.h> /* must be included after DSET defines */

#define TIMEOUT     2.0    /* I/O must complete within this time */
#define TIMEWINDOW  5.0    /* Wait this long after device timeout */

/******************************************************************************
 * Strings used by the init routines to fill in the znam,onam,...
 * fields in BI and BO record types.
 ******************************************************************************/

static char  *offOnList[] = { "Off","On" };
static struct devGpibNames   offOn = { 2,offOnList,0,1 };

static char  *initNamesList[] = { "Init","Init" };
static struct devGpibNames initNames = { 2,initNamesList,0,1 };

static char  *voltageRangeNamesList[] = { "Low","High" };
static struct devGpibNames voltageRangeNames = { 2,voltageRangeNamesList,0,1 };

/******************************************************************************
 * Structures used by the init routines to fill in the onst,twst,... and the
 * onvl,twvl,... fields in MBBI and MBBO record types.
 *
 * Note that the intExtSsBm and intExtSsBmStop structures use the same
 * intExtSsBmStopList and intExtSsBmStopVal lists but have a different number
 * of elements in them that they use... The intExtSsBm structure only represents
 * 4 elements,while the intExtSsBmStop structure represents 5.
 ******************************************************************************/


/******************************************************************************
 * String arrays for EFAST operations. The last entry must be 0.
 *
 * On input operations,only as many bytes as are found in the string array
 * elements are compared.  Additional bytes are ignored.
 * The first matching string  will be used as a match.
 *
 * For the input operations,the strings are compared literally!  This
 * can cause problems if the instrument is returning things like \r and \n
 * characters.  When defining input strings so you include them as well.
 ******************************************************************************/

static char *voltageRange[] = {"LOW","HIGH",0};

/******************************************************************************
 * Array of structures that define all GPIB messages
 * supported for this type of instrument.
 ******************************************************************************/

#if 0
static struct gpibCmd gpibCmds[] = {
    /* Param 0 - Enable remote control */
    /*{&DSET_BO,GPIBCMD,IB_Q_HIGH,"SYST:REM",0,0,80,NULL,0,0,NULL,&initNames,0},*/
	FILL,
    /* Param 1 - Read SCPI identification string */
    /*{&DSET_SI,GPIBREAD,IB_Q_HIGH,"*IDN?","%39[^\r\n]",0,80,0,0,0,NULL,NULL,0},*/
	FILL,
    /* Param 2 - Read SCPI status byte */
    /*{&DSET_LI,GPIBREAD,IB_Q_HIGH,"*STB?","%d",0,80,0,0,0,NULL,NULL,0},*/
	FILL,
    /* Param 3 - Read SCPI event register */
    /*{&DSET_LI,GPIBREAD,IB_Q_HIGH,"*ESR?","%d",0,80,0,0,0,NULL,NULL,0},*/
	FILL,
    /* Param 4 - Read output completion status */
    /*{&DSET_BI,GPIBREAD,IB_Q_HIGH,"*OPC?","%d",0,80,0,0,0,NULL,&offOn,0},*/
	FILL,
    FILL, FILL, FILL,
    /* Param 8 - Turn output off/on */
    /*{&DSET_BO,GPIBWRITE,IB_Q_HIGH,NULL,"OUTP %d",0,80,0,0,0,NULL,&offOn,0},*/
	FILL,
    /* Param 9 - Read on/off status */
    /*{&DSET_BI,GPIBREAD,IB_Q_HIGH,"OUTP?","%d",0,80,0,0,0,NULL,&offOn,0},*/
	FILL,

    /* Param 10 - Set voltage range */
    /*{&DSET_BO,GPIBEFASTO,IB_Q_HIGH,"VOLT:RANG ",NULL,0,80,0,0,0,voltageRange,&voltageRangeNames,0},*/
	FILL,
    /* Param 11 - Read voltage range string */
    /*{&DSET_SI,GPIBREAD,IB_Q_HIGH,"VOLT:RANG?","%39[^\n]",0,80,0,0,0,NULL,NULL,0},*/
	FILL,
    /* Param 12 - Set voltage */
    {&DSET_AO,GPIBWRITE,IB_Q_HIGH,NULL,"VOLT %g",0,80,0,0,0,NULL,NULL,0},
    /* Param 13 - Read voltage setpoint */
    /*{&DSET_AI,GPIBREAD,IB_Q_HIGH,"VOLT?","%lf",0,80,0,0,0,NULL,NULL,0},*/
    /* Param 14 - Measure voltage */
    /*{&DSET_AI,GPIBREAD,IB_Q_HIGH,"MEAS:VOLT?","%lf",0,80,0,0,0,NULL,NULL,0},*/
	FILL,
    FILL, FILL, FILL, FILL, FILL,

    /* Param 20 - Set current */
    {&DSET_AO,GPIBWRITE,IB_Q_HIGH,NULL,"CURR %g",0,80,0,0,0,NULL,NULL,0},
    /* Param 21 - Read current setpoint */
    /*{&DSET_AI,GPIBREAD,IB_Q_HIGH,"CURR?","%lf",0,80,0,0,0,NULL,NULL,0},*/
	FILL,
    /* Param 22 - Measure current */
    /*{&DSET_AI,GPIBREAD,IB_Q_HIGH,"MEAS:CURR?","%lf",0,80,0,0,0,NULL,NULL,0},*/
	FILL,
    FILL, FILL, FILL, FILL, FILL, FILL, FILL,
};
#else
static struct gpibCmd gpibCmds[] = {
    /* Param 0 - Enable remote control */
    {&DSET_BO,GPIBCMD,IB_Q_HIGH,"SYST:REM",0,0,80,NULL,0,0,NULL,&initNames,0},
    /* Param 1 - Set voltage */
    {&DSET_AO,GPIBWRITE,IB_Q_HIGH,NULL,"VOLT %g",0,80,0,0,0,NULL,NULL,0},
    /* Param 2 - Set current */
    {&DSET_AO,GPIBWRITE,IB_Q_HIGH,NULL,"CURR %g",0,80,0,0,0,NULL,NULL,0},
    /* Param 3 - Turn output off/on */
    {&DSET_BO,GPIBWRITE,IB_Q_HIGH,NULL,"OUTP %d",0,80,0,0,0,NULL,&offOn,0},
    /* Param 4 - Read voltage setpoint */
	{&DSET_AI,GPIBREAD,IB_Q_HIGH,"VOLT?","%lf",0,80,0,0,0,NULL,NULL,0},
    /*{&DSET_SI,GPIBREAD,IB_Q_HIGH,"VOLT?","%39[^+]%s",0,80,0,0,0,NULL,NULL,0},*/
    /* Param 5 - Read current setpoint */
    {&DSET_AI,GPIBREAD,IB_Q_HIGH,"CURR?","%lf",0,80,0,0,0,NULL,NULL,0},
	/*FILL,*/
};

#endif

/* The following is the number of elements in the command array above.  */
#define NUMPARAMS sizeof(gpibCmds)/sizeof(struct gpibCmd)

/******************************************************************************
 * Initialize device support parameters
 *
 *****************************************************************************/
static long init_ai(int parm)
{
    if(parm==0) {
        devSupParms.name = "deve364xa";
        devSupParms.gpibCmds = gpibCmds;
        devSupParms.numparams = NUMPARAMS;
        devSupParms.timeout = TIMEOUT;
        devSupParms.timeWindow = TIMEWINDOW;
        devSupParms.respond2Writes = -1;
    }
    return(0);
}
