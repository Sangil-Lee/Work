/*
 * Device support for Tektronix TDS3000/TDS5000-series digital oscilloscopes
 *
 ************************************************************************
 * Copyright (c) 2003 The University of Chicago, as Operator of Argonne
 * National Laboratory, and the Regents of the University of
 * California, as Operator of Los Alamos National Laboratory.
 * TDS3000/TDS5000 support is distributed subject to a Software License
 * Agreement found in file LICENSE that is included with this distribution.
 ************************************************************************
 *
 * Original Author: Nick Di Monte <npd@aps.anl.gov>
 *   Modifications for TDS5000 support by : S. Joshua Stein and W. Eric Norum
 */

#include    <stdlib.h>
#include    <stdio.h>
#include    <string.h>
#include    <alarm.h>
#include    <devCommonGpib.h>
#include    <menuFtype.h>

/*
 * The TDS3000 series and TDS5000 series oscilloscopes share an almost
 * identical set of commands.  Perform some C preprocessor trickery to
 * allow this code to support both families.
 */
#define tcat(x,y,z) x ## y ## z
#define cat(x,y,z) tcat(x,y,z)

#define DSET_AI         cat(devAiTDS, FAMILY, Gpib)
#define DSET_AO         cat(devAoTDS, FAMILY, Gpib)
#define DSET_LI         cat(devLiTDS, FAMILY, Gpib)
#define DSET_LO         cat(devLoTDS, FAMILY, Gpib)
#define DSET_BI         cat(devBiTDS, FAMILY, Gpib)
#define DSET_BO         cat(devBoTDS, FAMILY, Gpib)
#define DSET_MBBO       cat(devMbboTDS, FAMILY, Gpib)
#define DSET_MBBI       cat(devMbbiTDS, FAMILY, Gpib)
#define DSET_SI         cat(devSiTDS, FAMILY, Gpib)
#define DSET_SO         cat(devSoTDS, FAMILY, Gpib)
#define DSET_WF         cat(devWfTDS, FAMILY, Gpib)
#define NAME(FAMILY) "devTDS" #FAMILY "Gpib"

#include    <devGpib.h>

#define TIMEWINDOW     2.00      /* 2 second delay before reconnect */
#define TIMEOUT        12.0      /* 12 second limit for I/O operation */

/******************************************************************************
 *
 * String arrays for EFAST operations.  Note that the last entry must be 
 * NULL.
 *
 * On input operations, only as many bytes as are found in the string array
 * elements are compared.  If there are more bytes than that in the input
 * message, they are ignored.  The first matching string found (starting
 * from the 0'th element) will be used as a match.
 *
 * NOTE: For the input operations, the strings are compared literally!  This
 * can cause problems if the instrument is returning things like \r and \n
 * characters.  You must take care when defining input strings so you include
 * them as well.
 *
 ******************************************************************************/

static char     *(timeRefCmd[])    = {"HOR:TRIG:POS 10",
                                      "HOR:TRIG:POS 50",
                                      "HOR:TRIG:POS 90",
                                      NULL};

static char     *(trigModeCmd[])   = {"TRIG:A:MODE NORMAL",
                                      "TRIG:A:MODE AUTO",
                                      NULL};

static char     *(trigModeTable[])   = {"NORMAL" EFASTI_TERM,
                                        "AUTO"   EFASTI_TERM,
                                        NULL};

static char     *(trigSourceCmd[]) = {"TRIG:A:EDGE:SOU CH1",
                                      "TRIG:A:EDGE:SOU CH2",
                                      "TRIG:A:EDGE:SOU CH3",
                                      "TRIG:A:EDGE:SOU CH4",
                                      "TRIG:A:EDGE:SOU LINE",
                                      "TRIG:A:EDGE:SOU VERTICAL",
                                      "TRIG:A:EDGE:SOU EXT10",
                                      "TRIG:A:EDGE:SOU EXT",
                                      NULL};

static char     *(trigSourceTable[]) = {"CH1"      EFASTI_TERM,
                                        "CH2"      EFASTI_TERM,
                                        "CH3"      EFASTI_TERM,
                                        "CH4"      EFASTI_TERM,
                                        "LINE"     EFASTI_TERM,
                                        "VERTICAL" EFASTI_TERM,
                                        "EXT10"    EFASTI_TERM,
                                        "EXT"      EFASTI_TERM,
                                        NULL};

static char     *(trigSlopeCmd[])  = {"TRIG:A:EDGE:SLO FALL",
                                      "TRIG:A:EDGE:SLO RISE",
                                      NULL};

static char     *(trigSlopeTable[])  = {"FALL" EFASTI_TERM,
                                        "RISE" EFASTI_TERM,
                                        NULL};

static char     *trigStat[] = {"AUTO",
                               "ARMED",
                               "READY",
                               "SAVE",
                               "TRIGGER",
                                NULL};

static char     *(chanVdivCmd[])  = {"SCALE 1.0E-3",
                                     "SCALE 2.0E-3",
                                     "SCALE 5.0E-3",
                                     "SCALE 10.0E-3",
                                     "SCALE 20.0E-3",
                                     "SCALE 50.0E-3",
                                     "SCALE 100.0E-3",
                                     "SCALE 200.0E-3",
                                     "SCALE 500.0E-3",
                                     "SCALE 1",
                                     "SCALE 2",
                                     "SCALE 5",
                                     "SCALE 10",
                                      NULL};

static char     *(chanVdivTable[])   = { FAMILY_VDIV_TABLE
                                        NULL};


static char     *(chanCoupCmd[]) = {"COUPLING DC",
                                    "COUPLING AC",
                                    "COUPLING GND",
                                     NULL};

static char     *(chanCoupleTable[])  = {"DC"  EFASTI_TERM,
                                         "AC"  EFASTI_TERM,
                                         "GND" EFASTI_TERM,
                                         NULL};


static char     *(chan1OnOffCmd[]) = {"SELECT:CH1 OFF",
                                      "SELECT:CH1 ON",
                                      NULL};

static char     *(chan2OnOffCmd[]) = {"SELECT:CH2 OFF",
                                      "SELECT:CH2 ON",
                                      NULL};

static char     *(chan3OnOffCmd[]) = {"SELECT:CH3 OFF",
                                      "SELECT:CH3 ON",
                                      NULL};

static char     *(chan4OnOffCmd[]) = {"SELECT:CH4 OFF",
                                      "SELECT:CH4 ON",
                                      NULL};

static char     *(chanImpedCmd[]) = {"IMP FIFTY",
                                     "IMP MEG",
                                      NULL};

static char     *(chanImpedTable[]) = {"FIFTY" EFASTI_TERM,
                                       "MEG" EFASTI_TERM,
                                       NULL};

static char     *(acquireStopAfterCmd[]) = {"ACQUIRE:STOPAFTER RUNSTOP",
                                            "ACQUIRE:STOPAFTER SEQUENCE",
                                            NULL};

static char     *(acquireStopAfterTable[])  = {"RUNSTOP"  EFASTI_TERM,
                                               "SEQUENCE" EFASTI_TERM,
                                               NULL};

static char     *(frontPanelLockCmd[])  = {"LOCK NONE",
                                           "LOCK ALL",
                                           NULL};

static char     *(frontPanelLockTable[])  = {"NONE" EFASTI_TERM,
                                             "ALL"  EFASTI_TERM,
                                             NULL};

static char     *(recallCmd[])    = {"RECALL:SETUP 1",
                                     "RECALL:SETUP 2",
                                     "RECALL:SETUP 3",
                                     "RECALL:SETUP 4",
                                     "RECALL:SETUP 5",
                                     "RECALL:SETUP 6",
                                     "RECALL:SETUP 7",
                                     "RECALL:SETUP 8",
                                     "RECALL:SETUP 9",
                                     "RECALL:SETUP 10",
                                     "RECALL:SETUP FACTORY",
                                     NULL};

static char     *(saveCmd[])      = {"SAVE:SETUP 1",
                                     "SAVE:SETUP 2",
                                     "SAVE:SETUP 3",
                                     "SAVE:SETUP 4",
                                     "SAVE:SETUP 5",
                                     "SAVE:SETUP 6",
                                     "SAVE:SETUP 7",
                                     "SAVE:SETUP 8",
                                     "SAVE:SETUP 9",
                                     "SAVE:SETUP 10",
                                     NULL};

static char     *printerPortTable[] = {"CENTRONICS" EFASTI_TERM,
                                       "FILE"       EFASTI_TERM,
                                       "GIPB",      EFASTI_TERM 
                                       "RS232"      EFASTI_TERM,
                                       "ETHERNET"   EFASTI_TERM,
                                        NULL};

static char     *printerPortCmd[] = {"HARDC:PORT CENTRONICS",
                                     "HARDC:PORT FILE",
                                     "HARDC:PORT GIPB",
                                     "HARDC:PORT RS232",
                                     "HARDC:PORT ETHERNET",
                                      NULL};

static char     *printerFormatTable[] = {"DESKJETC"  EFASTI_TERM,
                                         "EPSCOLOR"  EFASTI_TERM,
                                         "LASERJET", EFASTI_TERM 
                                         "PCXCOLOR"  EFASTI_TERM,
                                          NULL};

static char     *printerFormatCmd[] = {"HARDCOPY:FORMAT DESKJETC",
                                       "HARDCOPY:FORMAT EPSCOLOR",
                                       "HARDCOPY:FORMAT LASERJET",
                                       "HARDCOPY:FORMAT PCXCOLOR",
                                        NULL};

static char     *(messageBoxCmd[])  = 
                        {"MESSAGE:SHOW 'RF1HV\n\nFront\nControls\nLOCKED'",
                         "MESSAGE:SHOW 'RF2HV\n\nFront\nControls\nLOCKED'",
                         "MESSAGE:SHOW 'RF3HV\n\nFront\nControls\nLOCKED'",
                         "MESSAGE:SHOW 'RF4HV\n\nFront\nControls\nLOCKED'",
                         "MESSAGE:SHOW 'RF1LL\n\nFront\nControls\nLOCKED'",
                         "MESSAGE:SHOW 'RF2LL\n\nFront\nControls\nLOCKED'",
                         "MESSAGE:SHOW 'RF3LL\n\nFront\nControls\nLOCKED'",
                         "MESSAGE:SHOW 'RF4LL\n\nFront\nControls\nLOCKED'",
                         NULL};

static char     *(messageBoxTable[])  = 
                        {"'RF1HV\nFront\nControls\nLOCKED'" EFASTI_TERM,
                         "'RF2HV\nFront\nControls\nLOCKED'" EFASTI_TERM,
                         "'RF3HV\nFront\nControls\nLOCKED'" EFASTI_TERM,
                         "'RF4HV\nFront\nControls\nLOCKED'" EFASTI_TERM,
                         "'RF1LL\nFront\nControls\nLOCKED'" EFASTI_TERM,
                         "'RF2LL\nFront\nControls\nLOCKED'" EFASTI_TERM,
                         "'RF3LL\nFront\nControls\nLOCKED'" EFASTI_TERM,
                         "'RF4LL\nFront\nControls\nLOCKED'" EFASTI_TERM,
                         NULL};

/****************************************************************************** *
 * Array of structures that define all GPIB messages
 * supported for this type of instrument.
 *
 ******************************************************************************/

/* forward declarations of some custom convert routines */
static int convertData();
static int scaleData();
static int preamblePoints();
static int preambleCount();
static int writeSetupData();
static int readSetupData();
static int writeStringWf();
static int getEthernetName();


static struct gpibCmd gpibCmds[] = 
{
/* Param */
/*  0 */ FILL,

/* Timebase */
/*  1 : read horizontal scale : AI */
         {&DSET_AI, GPIBREAD, IB_Q_LOW, "HOR:MAIN:SCALE?", "%lf", 0, 20,
          NULL, 0, 0, NULL, NULL, NULL},
/*  2 : read delay : AI */
         {&DSET_AI, GPIBREAD, IB_Q_LOW, "HOR:DEL:TIM?", "%lf", 0, 20,
          NULL, 0, 0, NULL, NULL, NULL},
/*  3 : set horizontal scale : AO */
         {&DSET_AO, GPIBWRITE, IB_Q_LOW, NULL, "HOR:MAIN:SCALE %e", 0, 30,
          NULL, 0, 0, NULL, NULL, 0},
/*  4 : set delay : AO */
         {&DSET_AO, GPIBWRITE, IB_Q_LOW, NULL,"HOR:DEL:TIM %e", 0, 30,
          NULL, 0, 0, NULL, NULL, 0},
/*  5 : set display reference : MBBO */
         {&DSET_MBBO, GPIBEFASTO, IB_Q_HIGH, NULL, NULL, 0, 0,
          NULL, 0, 0, timeRefCmd, NULL, 0},
/*  6 : read display reference : AI */
         {&DSET_AI, GPIBREAD, IB_Q_LOW, "HOR:TRIG:POS?", "%lf", 0, 20,
          NULL, 0, 0, NULL, NULL, NULL},
/*  7 - 9 :            */
         FILL,FILL,FILL,

/* Channel 1 */ 
/* 10 : read vertical range : AI */
         {&DSET_AI, GPIBREAD, IB_Q_LOW, "CH1:SCALE?", "%lf", 0, 20,
          NULL, 0 ,0, NULL, NULL, NULL},
/* 11 : set vertical scale : MO */
         {&DSET_MBBO, GPIBEFASTO, IB_Q_HIGH, "CH1:", NULL, 0, 30,
          NULL, 0, 0, chanVdivCmd, NULL, 0},
/* 12 : read offset range : AI */
         {&DSET_AI, GPIBREAD, IB_Q_LOW, "CH1:OFFS?", "%lf", 0, 20,
          NULL, 0 ,0, NULL, NULL, NULL},
/* 13 : set offset : AO */
         {&DSET_AO, GPIBWRITE, IB_Q_LOW, NULL,"CH1:OFFS %e", 0, 30,
          NULL, 0, 0, NULL, NULL, 0},
/* 14 : set coulping : MO */
         {&DSET_MBBO, GPIBEFASTO, IB_Q_HIGH, "CH1:", NULL, 0, 30,
          NULL, 0, 0, chanCoupCmd, NULL, 0},
/* 15 : enable channel : BO */
         {&DSET_BO, GPIBEFASTO, IB_Q_HIGH, NULL, NULL, 0, 0,
          NULL, 0, 0, chan1OnOffCmd, NULL, 0},
/* 16 : read vertical range : MBBI */
         {&DSET_MBBI, GPIBEFASTI, IB_Q_HIGH, "CH1:SCALE?", NULL, 0, 50,
          NULL, 0, 0, chanVdivTable, NULL, NULL},
/* 17 : read coupling : MBBI */
         {&DSET_MBBI, GPIBEFASTI, IB_Q_HIGH, "CH1:COUPLING?", NULL, 0, 50,
          NULL, 0, 0, chanCoupleTable, NULL, NULL},
/* 18 : read channel enable : BI  */
         {&DSET_BI, GPIBREAD, IB_Q_HIGH, "SEL:CH1?", "%d", 0, 20,
          NULL, 0, 0, NULL, NULL, NULL},
/* 19 : read waveform : WF */
        {&DSET_WF, GPIBCVTIO, IB_Q_LOW, "DAT:SOU CH1;:WAVFRM?", 
            "%lf", 0, 32000, convertData, 0 ,0, NULL, NULL, NULL},
/* 20 : read channel impedance : BI */
         {&DSET_BI, GPIBEFASTI, IB_Q_HIGH, "CH1:IMP?", NULL, 0, 50,
          NULL, 0, 0, chanImpedTable, NULL, NULL},
/* 21 : set channel impedance : BO */
         {&DSET_BO, GPIBEFASTO, IB_Q_HIGH, "CH1:", NULL, 0, 30,
          NULL, 0, 0, chanImpedCmd, NULL, 0},
/* 22 - 29 :            */
         FILL,FILL,FILL,FILL,FILL,FILL,FILL,FILL,

/* Channel 2 */
/* 30 : read vertical range : AI */
         {&DSET_AI, GPIBREAD, IB_Q_LOW, "CH2:SCALE?", "%lf", 0, 20,
          NULL, 0 ,0, NULL, NULL, NULL},
/* 31 : set vertical scale : MO */
         {&DSET_MBBO, GPIBEFASTO, IB_Q_HIGH, "CH2:", NULL, 0, 30,
          NULL, 0, 0, chanVdivCmd, NULL, 0},
/* 32 : read offset range : AI */
         {&DSET_AI, GPIBREAD, IB_Q_LOW, "CH2:OFFS?", "%lf", 0, 20,
          NULL, 0 ,0, NULL, NULL, NULL},
/* 33 : set offset : AO */
         {&DSET_AO, GPIBWRITE, IB_Q_LOW, NULL,"CH2:OFFS %e", 0, 30,
          NULL, 0, 0, NULL, NULL, 0},
/* 34 : set coulping : MO */
         {&DSET_MBBO, GPIBEFASTO, IB_Q_HIGH, "CH2:", NULL, 0, 30,
          NULL, 0, 0, chanCoupCmd, NULL, 0},
/* 35 : enable channel : BO */
         {&DSET_BO, GPIBEFASTO, IB_Q_HIGH, NULL, NULL, 0, 0,
          NULL, 0, 0, chan2OnOffCmd, NULL, 0},
/* 36 : read vertical range : MBBI */
         {&DSET_MBBI, GPIBEFASTI, IB_Q_HIGH, "CH2:SCALE?", NULL, 0, 50,
          NULL, 0, 0, chanVdivTable, NULL, NULL},
/* 37 : read coupling : MBBI */
         {&DSET_MBBI, GPIBEFASTI, IB_Q_HIGH, "CH2:COUPLING?", NULL, 0, 50,
          NULL, 0, 0, chanCoupleTable, NULL, NULL},
/* 38 : read channel enable : BI  */
         {&DSET_BI, GPIBREAD, IB_Q_HIGH, "SEL:CH2?", "%d", 0, 20,
          NULL, 0, 0, NULL, NULL, NULL},
/* 39 : read waveform : WF */
        {&DSET_WF, GPIBCVTIO, IB_Q_LOW, "DAT:SOU CH2;:WAVFRM?", 
            "%lf", 0, 32000, convertData, 0 ,0, NULL, NULL, NULL},
/* 40 : read channel impedance : BI */
         {&DSET_BI, GPIBEFASTI, IB_Q_HIGH, "CH2:IMP?", NULL, 0, 50,
          NULL, 0, 0, chanImpedTable, NULL, NULL},
/* 41 : set channel impedance : BO */
         {&DSET_BO, GPIBEFASTO, IB_Q_HIGH, "CH2:", NULL, 0, 30,
          NULL, 0, 0, chanImpedCmd, NULL, 0},
/* 42 - 49 :            */
         FILL,FILL,FILL,FILL,FILL,FILL,FILL,FILL,

/* Channel 3 */
/* 50 : read vertical range : AI */
         {&DSET_AI, GPIBREAD, IB_Q_LOW, "CH3:SCALE?", "%lf", 0, 20,
          NULL, 0 ,0, NULL, NULL, NULL},
/* 51 : set vertical scale : MO */
         {&DSET_MBBO, GPIBEFASTO, IB_Q_HIGH, "CH3:", NULL, 0, 30,
          NULL, 0, 0, chanVdivCmd, NULL, 0},
/* 52 : read offset range : AI */
         {&DSET_AI, GPIBREAD, IB_Q_LOW, "CH3:OFFS?", "%lf", 0, 20,
          NULL, 0 ,0, NULL, NULL, NULL},
/* 53 : set offset : AO */
         {&DSET_AO, GPIBWRITE, IB_Q_LOW, NULL,"CH3:OFFS %e", 0, 30,
          NULL, 0, 0, NULL, NULL, 0},
/* 54 : set coulping : MO */
         {&DSET_MBBO, GPIBEFASTO, IB_Q_HIGH, "CH3:", NULL, 0, 30,
          NULL, 0, 0, chanCoupCmd, NULL, 0},
/* 55 : enable channel : BO */
         {&DSET_BO, GPIBEFASTO, IB_Q_HIGH, NULL, NULL, 0, 0,
          NULL, 0, 0, chan3OnOffCmd, NULL, 0},
/* 56 : read vertical range : MBBI */
         {&DSET_MBBI, GPIBEFASTI, IB_Q_HIGH, "CH3:SCALE?", NULL, 0, 50,
          NULL, 0, 0, chanVdivTable, NULL, NULL},
/* 57 : read coupling : MBBI */
         {&DSET_MBBI, GPIBEFASTI, IB_Q_HIGH, "CH3:COUPLING?", NULL, 0, 50,
          NULL, 0, 0, chanCoupleTable, NULL, NULL},
/* 58 : read channel enable : BI  */
         {&DSET_BI, GPIBREAD, IB_Q_HIGH, "SEL:CH3?", "%d", 0, 20,
          NULL, 0, 0, NULL, NULL, NULL},
/* 59 : read waveform : WF */
        {&DSET_WF, GPIBCVTIO, IB_Q_LOW, "DAT:SOU CH3;:WAVFRM?", 
            "%lf", 0, 32000, convertData, 0 ,0, NULL, NULL, NULL},
/* 60 : read channel impedance : BI */
         {&DSET_BI, GPIBEFASTI, IB_Q_HIGH, "CH3:IMP?", NULL, 0, 50,
          NULL, 0, 0, chanImpedTable, NULL, NULL},
/* 61 : set channel impedance : BO */
         {&DSET_BO, GPIBEFASTO, IB_Q_HIGH, "CH3:", NULL, 0, 30,
          NULL, 0, 0, chanImpedCmd, NULL, 0},
/* 62 - 69 :            */
         FILL,FILL,FILL,FILL,FILL,FILL,FILL,FILL,
 
/* Channel 4 */
/* 70 : read vertical range : AI */
         {&DSET_AI, GPIBREAD, IB_Q_LOW, "CH4:SCALE?", "%lf", 0, 20,
          NULL, 0 ,0, NULL, NULL, NULL},
/* 71 : set vertical scale : MO */
         {&DSET_MBBO, GPIBEFASTO, IB_Q_HIGH, "CH4:", NULL, 0, 30,
          NULL, 0, 0, chanVdivCmd, NULL, 0},
/* 72 : read offset range : AI */
         {&DSET_AI, GPIBREAD, IB_Q_LOW, "CH4:OFFS?", "%lf", 0, 20,
          NULL, 0 ,0, NULL, NULL, NULL},
/* 73 : set offset : AO */
         {&DSET_AO, GPIBWRITE, IB_Q_LOW, NULL,"CH4:OFFS %e", 0, 30,
          NULL, 0, 0, NULL, NULL, 0},
/* 74 : set coulping : MO */
         {&DSET_MBBO, GPIBEFASTO, IB_Q_HIGH, "CH4:", NULL, 0, 30,
          NULL, 0, 0, chanCoupCmd, NULL, 0},
/* 75 : enable channel : BO */
         {&DSET_BO, GPIBEFASTO, IB_Q_HIGH, NULL, NULL, 0, 0,
          NULL, 0, 0, chan4OnOffCmd, NULL, 0},
/* 76 : read vertical range : MBBI */
         {&DSET_MBBI, GPIBEFASTI, IB_Q_HIGH, "CH4:SCALE?", NULL, 0, 50,
          NULL, 0, 0, chanVdivTable, NULL, NULL},
/* 77 : read coupling : MBBI */
         {&DSET_MBBI, GPIBEFASTI, IB_Q_HIGH, "CH4:COUPLING?", NULL, 0, 50,
          NULL, 0, 0, chanCoupleTable, NULL, NULL},
/* 78 : read channel enable : BI  */
         {&DSET_BI, GPIBREAD, IB_Q_HIGH, "SEL:CH4?", "%d", 0, 20,
          NULL, 0, 0, NULL, NULL, NULL},
/* 79 : read waveform : WF */
        {&DSET_WF, GPIBCVTIO, IB_Q_LOW, "DAT:SOU CH4;:WAVFRM?", 
            "%lf", 0, 32000, convertData, 0 ,0, NULL, NULL, NULL},
/* 80 : read channel impedance : BI */
         {&DSET_BI, GPIBEFASTI, IB_Q_HIGH, "CH4:IMP?", NULL, 0, 50,
          NULL, 0, 0, chanImpedTable, NULL, NULL},
/* 81 : set channel impedance : BO */
         {&DSET_BO, GPIBEFASTO, IB_Q_HIGH, "CH4:", NULL, 0, 30,
          NULL, 0, 0, chanImpedCmd, NULL, 0},
/* 82 - 89 :            */
         FILL,FILL,FILL,FILL,FILL,FILL,FILL,FILL,

/* Instrument Parameters: Part 1  */
/* 90 : init GPIB Parameters */
         {&DSET_BO, GPIBCMD, IB_Q_HIGH, 
         "*CLS;:WFMP:ENC BIN; BN_F RI; BYT_N 2; BYT_O MSB;:HOR:RECORD 10000;:HEAD OFF;:VERBOSE ON;:DATA:START 1;:DATA:STOP 10000",
          NULL, 0, 0, NULL, 0 ,0, NULL, NULL, 0},
/* 91 : recall memory setting : MBBO */
         {&DSET_MBBO, GPIBEFASTO, IB_Q_HIGH, NULL, NULL, 0, 0,
          NULL, 0, 0, recallCmd, NULL, 0},
/* 92 : save memory setting : MBBO */
         {&DSET_MBBO, GPIBEFASTO, IB_Q_HIGH, NULL, NULL, 0, 0,
          NULL, 0, 0, saveCmd, NULL, 0},
/* 93 : string output only : SO */
         {&DSET_SO, GPIBWRITE, IB_Q_HIGH, NULL, "%s", 0, 41,
          NULL, 0 ,0, NULL, NULL, 0},
/* 94 : autoscale : BO */
         {&DSET_BO, GPIBCMD, IB_Q_HIGH, "AUTOSET", NULL, 0, 0,
          NULL, 0 ,0, NULL, NULL, 0},
/* 95 - 96 */
         FILL,FILL,
/* 97 : read setup : WF */
         {&DSET_WF, GPIBREAD, IB_Q_LOW, "SET?", "%lf", 0, 2060,
          readSetupData, 0 ,0, NULL, NULL, NULL},
/* 98 : write setup : WF */
         {&DSET_WF, GPIBWRITE, IB_Q_LOW, NULL, NULL, 0, 2068,
          writeSetupData, 0 ,0, NULL, NULL, 0},
/* 99 : reset scope : BO  */
         {&DSET_BO, GPIBCMD, IB_Q_HIGH, "*RST",
          NULL, 0, 0, NULL, 0 ,0, NULL, NULL, 0},

/* Trigger */
/* 100 : set time mode : MBBO */
         {&DSET_MBBO, GPIBEFASTO, IB_Q_HIGH, NULL, NULL, 0, 0,
          NULL, 0, 0, trigModeCmd, NULL, 0},
/* 101 : set trigger source : MBBO */
         {&DSET_MBBO, GPIBEFASTO, IB_Q_HIGH, NULL, NULL, 0, 0,
          NULL, 0, 0, trigSourceCmd, NULL, 0},
/* 102 : set trigger level : AO */
         {&DSET_AO, GPIBWRITE, IB_Q_LOW, NULL,"TRIG:A:LEV %e", 0, 30,
          NULL, 0, 0, NULL, NULL, 0},
/* 103 : set trigger slope : MBBO */
         {&DSET_MBBO, GPIBEFASTO, IB_Q_HIGH, NULL, NULL, 0, 0,
          NULL, 0, 0, trigSlopeCmd, NULL, 0},
/* 104 : set holdoff : AO */
         {&DSET_AO, GPIBWRITE, IB_Q_HIGH, NULL,"TRIG:A:HOLD:TIME %e", 0, 50,
          NULL, 0, 0, NULL, NULL, 0},
/* 105 : read holdoff : AI */
         {&DSET_AI, GPIBREAD, IB_Q_HIGH, "TRIG:A:HOLD:TIME?","%lf", 0, 20,
          NULL, 0, 0, NULL, NULL, NULL},
/* 106 : read trigger level : AI */
         {&DSET_AI, GPIBREAD, IB_Q_HIGH, "TRIG:A:LEV?","%lf", 0, 20,
          NULL, 0, 0, NULL, NULL, NULL},
/* 107 : read time mode : MBBI */
         {&DSET_MBBI, GPIBEFASTI, IB_Q_HIGH, "TRIG:A:MODE?", NULL, 0, 50,
          NULL, 0, 0, trigModeTable, NULL, NULL},
/* 108 : read trigger source mode : MBBI */
         {&DSET_MBBI, GPIBEFASTI, IB_Q_HIGH, "TRIG:A:EDGE:SOU?", NULL, 0, 50,
          NULL, 0, 0, trigSourceTable, NULL, NULL},
/* 109 : read trigger slope : MBBI */
         {&DSET_MBBI, GPIBEFASTI, IB_Q_HIGH, "TRIG:A:EDGE:SLO?", NULL, 0, 50,
          NULL, 0, 0, trigSlopeTable, NULL, NULL},
/* 110 : Trigger Status */
         {&DSET_MBBI, GPIBEFASTI,  IB_Q_HIGH, "TRIGGER:STATE?", NULL, 0, 50,
          NULL, 0, 0, trigStat, NULL, NULL},
/* 111 - 119 */
         FILL,FILL,FILL,FILL,FILL,FILL,FILL,FILL,FILL,

/* Instrument Parameters Continued */
/* 120 : read GPIB device response : SI  */
         {&DSET_SI, GPIBREAD, IB_Q_HIGH, "", "%s", 0, 100,
          NULL, 0, 0, NULL, NULL, NULL},
/* 121 : XINCrement : AI */
         {&DSET_AI, GPIBREAD, IB_Q_HIGH, "WFMPRE:XINCR?","%lf", 0, 100,
          NULL, 0, 0, NULL, NULL, NULL},
/* 122 : XORigin : AI */
         {&DSET_AI, GPIBREAD, IB_Q_HIGH, "WFMPRE:PT_OFF?","%lf", 0, 100,
          NULL, 0, 0, NULL, NULL, NULL},
/* 123 : XREFerence : AI */
         {&DSET_AI, GPIBREAD, IB_Q_HIGH, "WFMPRE:XZERO?","%lf", 0, 100,
          NULL, 0, 0, NULL, NULL, NULL},
/* 124 : YINCrement : AI */
         {&DSET_AI, GPIBREAD, IB_Q_HIGH, "WFMPRE:YMULT?","%lf", 0, 100,
          NULL, 0, 0, NULL, NULL, NULL},
/* 125 : YORigin : AI */
         {&DSET_AI, GPIBREAD, IB_Q_HIGH, "WFMPRE:YZERO?","%lf", 0, 100,
          NULL, 0, 0, NULL, NULL, NULL},
/* 126 : YREFerence : AI */
         {&DSET_AI, GPIBREAD, IB_Q_HIGH, "WFMPRE:YOFF?","%lf", 0, 100,
          NULL, 0, 0, NULL, NULL, NULL},
/* 127 - 129 */
         FILL,FILL,FILL,

/* Waveform memory */
/* 130 : read waveform mem 1: WF */
        {&DSET_WF, GPIBCVTIO, IB_Q_LOW, "DAT:SOU REF1;:WAVFRM?", 
            "%lf", 0, 32000, convertData, 0 ,0, NULL, NULL, NULL},
/* 131 : read waveform mem 2: WF */
        {&DSET_WF, GPIBCVTIO, IB_Q_LOW, "DAT:SOU REF2;:WAVFRM?", 
            "%lf", 0, 32000, convertData, 0 ,0, NULL, NULL, NULL},
/* 132 : read waveform mem 3: WF */
        {&DSET_WF, GPIBCVTIO, IB_Q_LOW, "DAT:SOU REF3;:WAVFRM?", 
            "%lf", 0, 32000, convertData, 0 ,0, NULL, NULL, NULL},
/* 133 : read waveform mem 4: WF */
        {&DSET_WF, GPIBCVTIO, IB_Q_LOW, "DAT:SOU REF4;:WAVFRM?", 
            "%lf", 0, 32000, convertData, 0 ,0, NULL, NULL, NULL},
/* 134 : NPOInts (num points per segment from preamble): AI */
         {&DSET_AI, GPIBREAD, IB_Q_HIGH, "WFMPRE?","%lf", 0, 300,
          preamblePoints, 0, 0, NULL, NULL, NULL},
/* 135 : NCOUnt (number of segments from preamble): AI */
         {&DSET_AI, GPIBREAD, IB_Q_HIGH, "WFMPRE?","%lf", 0, 300,
          preambleCount, 0, 0, NULL, NULL, NULL},

/* Waveforms for medm display */
/* 136 : read waveform : WF */
        {&DSET_WF, GPIBCVTIO, IB_Q_LOW, "DAT:SOU CH1;:WAVFRM?", 
            "%lf", 0, 32000, scaleData, 0 ,0, NULL, NULL, NULL},
/* 137 : read waveform : WF */
        {&DSET_WF, GPIBCVTIO, IB_Q_LOW, "DAT:SOU CH2;:WAVFRM?", 
            "%lf", 0, 32000, scaleData, 0 ,0, NULL, NULL, NULL},
/* 138 : read waveform : WF */
        {&DSET_WF, GPIBCVTIO, IB_Q_LOW, "DAT:SOU CH3;:WAVFRM?", 
            "%lf", 0, 32000, scaleData, 0 ,0, NULL, NULL, NULL},
/* 139 : read waveform : WF */
        {&DSET_WF, GPIBCVTIO, IB_Q_LOW, "DAT:SOU CH4;:WAVFRM?", 
            "%lf", 0, 32000, scaleData, 0 ,0, NULL, NULL, NULL},
/* 140 : X axis in time units  */
#if FAMILY==3000
        {&DSET_WF, GPIBREAD, IB_Q_LOW, "DAT:SOU CH1;:WFMPRE?", 
#else
        {&DSET_WF, GPIBREAD, IB_Q_LOW, "DAT:SOU CH1;:WFMOUTPRE?", 
#endif
            "%lf", 0, 32000, scaleData, 1 ,0, NULL, NULL, NULL},
/* 141 - 149:            */
         FILL,FILL,FILL,FILL,FILL,FILL,FILL,FILL,FILL,

/* waveform records to increase the size of "stringin/stringout" capabilities */
/* 150 : read a long response string */
         {&DSET_WF, GPIBREAD, IB_Q_HIGH, "", NULL, 0, 258,
          readSetupData, 256, 0, NULL, NULL, NULL},
/* 151 : write a long response string */
         {&DSET_WF, GPIBWRITE, IB_Q_HIGH, NULL, NULL, 0, 258,
          writeStringWf, 256, 0, NULL, NULL, 0},

/* printer functions for a DeskJet color printer */
/* 152 - setup Hard copy to output to printer BO */
        {&DSET_BO, GPIBCMD,  IB_Q_HIGH, 
        "HARDC:INKS ON;LAY LAN;PALE NORM", 
        NULL, 0, 0, NULL, 0, 0, NULL, NULL , 0},
/* 153 - print to printer BO */
        {&DSET_BO, GPIBCMD, IB_Q_HIGH, ":CLEARMENU;:DIS:CLOCK ON;:HARDC START", 
        NULL, 0, 0, NULL, 0, 0, NULL, NULL , 0},
/* 154 - read printer port configuration MBBI */
         {&DSET_MBBI, GPIBEFASTI, IB_Q_LOW, "HARDCOPY:PORT?", NULL, 0, 50,
          NULL, 0, 0, printerPortTable, NULL, NULL},
/* 155 - abort print job BO */
        {&DSET_BO, GPIBCMD, IB_Q_HIGH, "HARDCOPY ABORT", 
        NULL, 0, 0, NULL, 0, 0, NULL, NULL , 0},
/* 156 - clear menu on display BO */
        {&DSET_BO, GPIBCMD, IB_Q_HIGH, ":CLEARMENU", 
        NULL, 0, 0, NULL, 0, 0, NULL, NULL , 0},
/* 157 - set printer port : MBBO */
         {&DSET_MBBO, GPIBEFASTO, IB_Q_HIGH, NULL, NULL, 0, 0,
          NULL, 0, 0, printerPortCmd, NULL, 0},
/* 158 - set printer format : MBBO */
         {&DSET_MBBO, GPIBEFASTO, IB_Q_HIGH, NULL, NULL, 0, 0,
          NULL, 0, 0, printerFormatCmd, NULL, 0},
/* 159 - read printer format MBBI */
         {&DSET_MBBI, GPIBEFASTI, IB_Q_LOW, "HARDCOPY:FORMAT?", NULL, 0, 50,
          NULL, 0, 0, printerFormatTable, NULL, NULL},

/* Acquire Functions */
/* 160 : set acquire stop after : BO */
         {&DSET_BO, GPIBEFASTO, IB_Q_HIGH, NULL, NULL, 0, 0,
          NULL, 0, 0, acquireStopAfterCmd, NULL, 0},
/* 161 : read acquire stop after status BI */
         {&DSET_BI, GPIBEFASTI, IB_Q_HIGH, "ACQUIRE:STOPAFTER?", NULL, 0, 50,
          NULL, 0, 0, acquireStopAfterTable, NULL, NULL},
/* 162 : start acquisition : BO */
         {&DSET_BO, GPIBWRITE, IB_Q_HIGH, NULL, "ACQUIRE:STATE RUN", 0, 20,
          NULL, 0 ,0, NULL, NULL, 0},
/* 163 : stop acquisition : BO */
         {&DSET_BO, GPIBWRITE, IB_Q_HIGH, NULL, "ACQUIRE:STATE STOP", 0, 20,
          NULL, 0 ,0, NULL, NULL, 0},
/* 164 : read acquire status BI */
         {&DSET_BI, GPIBREAD, IB_Q_HIGH, "ACQ:STATE?", "%d", 0, 20,
          NULL, 0, 0, NULL, NULL, NULL},

/* 165 - 169:            */
         FILL,FILL,FILL,FILL,FILL,

/* Miscellaneous Functions */
/* 170 : Front panel lock control : BO */
         {&DSET_BO, GPIBEFASTO, IB_Q_HIGH, NULL, NULL, 0, 0,
          NULL, 0, 0, frontPanelLockCmd, NULL, 0},
/* 171 : Front panel lock status BI */
         {&DSET_BI, GPIBEFASTI, IB_Q_HIGH, "LOCK?", NULL, 0, 50,
          NULL, 0, 0, frontPanelLockTable, NULL, NULL},
/* 172 : read BUSY status, for printing or acquire status BI */
         {&DSET_BI, GPIBREAD, IB_Q_HIGH, "BUSY?", "%d", 0, 20,
          NULL, 0, 0, NULL, NULL, NULL},
/* 173 : read content of message box SI */
         {&DSET_SI, GPIBREAD, IB_Q_HIGH, "MESSAGE:SHOW?", "%s", 0, 20,
          NULL, 0, 0, NULL, NULL, NULL},
/* 174 : write content to message box SO */
         {&DSET_SO, GPIBWRITE, IB_Q_HIGH, NULL, "MESSAGE:SHOW %s", 0, 20,
          NULL, 0, 0, NULL, NULL, 0},
/* 175 : config message box BO */
         {&DSET_BO, GPIBWRITE, IB_Q_HIGH, NULL, "MESSAGE:STATE %d", 0, 20,
          NULL, 0, 0, NULL, NULL, 0},
/* 176 : message box status BI */
         {&DSET_BI, GPIBREAD, IB_Q_HIGH, "MESSAGE:STATE?", "%d", 0, 20,
          NULL, 0, 0, NULL, NULL, NULL},
/* 177 : read message box MI */
         {&DSET_MBBI, GPIBEFASTI, IB_Q_HIGH, "MESSAGE:SHOW?", NULL, 0, 50,
          NULL, 0, 0, messageBoxTable, NULL, NULL},
/* 178 : write message box : MO */
         {&DSET_MBBO, GPIBEFASTO, IB_Q_HIGH, NULL, NULL, 0, 0,
          NULL, 0, 0, messageBoxCmd, NULL, 0},
/* 179 : config message box size BO */
         {&DSET_BO, GPIBWRITE, IB_Q_HIGH, NULL, 
	 "MESSAGE:BOX 540,300,630,400", 0, 50, NULL, 0, 0, NULL, NULL, 0},
/* 180 : read ethernet name SI */
         {&DSET_SI, GPIBCVTIO, IB_Q_HIGH, NULL, NULL, 0, 100, 
          getEthernetName, 0, 0, NULL, NULL, NULL},
/* 181 : write SO */
         {&DSET_SO, GPIBWRITE, IB_Q_HIGH, NULL, "%s", 0, 100,
          NULL, 0, 0, NULL, NULL, 0},
/* 182 : read Device Event Status Enable Register: AI */
         {&DSET_AI, GPIBREAD, IB_Q_LOW, "DESE?", "%lf", 0, 20,
          NULL, 0, 0, NULL, NULL, NULL},
/* 183 : read Standard Event Status Register: AI */
         {&DSET_AI, GPIBREAD, IB_Q_LOW, "*ESR?", "%lf", 0, 20,
          NULL, 0, 0, NULL, NULL, NULL},
/* 184 : read Event Queue: AI */
         {&DSET_AI, GPIBREAD, IB_Q_LOW, "EVENT?", "%lf", 0, 20,
          NULL, 0, 0, NULL, NULL, NULL},
/* 185 : set Device Event Status Enable Register : AO */
         {&DSET_AO, GPIBWRITE, IB_Q_LOW, NULL, "DESE %f", 0, 30,
          NULL, 0, 0, NULL, NULL, 0},
/* 186 : clear Event Status : BO  */
         {&DSET_BO, GPIBCMD, IB_Q_HIGH, "*CLS",
          NULL, 0, 0, NULL, 0 ,0, NULL, NULL, 0}

};


/* The following is the number of elements in the command array above.  */
#define NUMPARAMS       sizeof(gpibCmds)/sizeof(struct gpibCmd)

/*****************************************************************************
 *
 * Structure containing the user's functions and operating parameters needed
 * by the gpib library functions.
 *
 *****************************************************************************/
static long init_ai(int parm)
{
   if (parm == 0) {
     devSupParms.name = NAME(FAMILY);     /* device support module type name */
     devSupParms.gpibCmds = gpibCmds;     /* GPIB command array */
     devSupParms.numparams = NUMPARAMS;   /* number of supported parameters */
     devSupParms.timeout = TIMEOUT;       /* I/O time limit (msec) */
     devSupParms.timeWindow = TIMEWINDOW; /* # of milliseconds to skip after a device times out */
     devSupParms.respond2Writes = -1;     /* set pos if the device responds to writes */
   }
   return(0);
}


/****************************************************************************
 *
 * 
 * Custom convert routines for TDS3000/5000 
 *
 *
 ***************************************************************************/

/* Use waveform preamble data to interpret waveform */
static int
convertData( struct gpibDpvt *pdpvt, int P1, int P2, char **P3)
{ 
  asynUser *pasynUser = pdpvt->pasynUser;
  double xIncr, xZero, yMult, yZero, yOff;
  int byte, bits, length, ptOff, width;
  char encode[20], binFmt[20], byteOr[20], idString[80], ptFmt[20], xUnit[20], yUnit[20];
  int len, i;
  size_t ntrans;
  struct waveformRecord *pwf= (struct waveformRecord *)pdpvt->precord;
  gpibCmd *pgpibCmd = gpibCmdGet(pdpvt);
  asynOctet *pasynOctet = pdpvt->pasynOctet;
  void *asynOctetPvt = pdpvt->asynOctetPvt;

  if (pasynOctet->write(asynOctetPvt,pasynUser,pgpibCmd->cmd,
                             strlen(pgpibCmd->cmd),&ntrans) != asynSuccess) {
    epicsSnprintf(pasynUser->errorMessage,pasynUser->errorMessageSize,
                                                "convertData write failed");
    return -1;
  }
  if (pdevSupportGpib->readArbitraryBlockProgramData(pdpvt) <= 0) {
    epicsSnprintf(pasynUser->errorMessage,pasynUser->errorMessageSize,
                            "convertData readArbitraryBlockProgramData failed");
    return -1;
  }

  if (sscanf(pdpvt->msg, "%d;%d;%19[^;];%19[^;];%19[^;];%d;%79[^;];%19[^;];"
                         "%lg;%d;%lg;%19[^;];%lg;%lg;%lg;%19[^;];#%1d%n",
        &byte,&bits,encode,binFmt,byteOr,&length,idString,ptFmt,
        &xIncr,&ptOff,&xZero,xUnit,&yMult,&yZero,&yOff,yUnit,&width,&i) != 17) {
    epicsSnprintf(pasynUser->errorMessage,pasynUser->errorMessageSize,
                                                "convertData sscanf failed");
    return -1;
  }
  asynPrint(pasynUser, ASYN_TRACE_FLOW, "%p, index=%d\n", (unsigned char *)&pdpvt->msg[0],i);
  asynPrint(pasynUser, ASYN_TRACE_FLOW, "byte- %d, bits- %d, encode- %s, binFmt- %s, byteOr-  %s\n",
          byte, bits, encode, binFmt, byteOr);
  asynPrint(pasynUser, ASYN_TRACE_FLOW, "length- %d, ptFmt- %s\n",
          length, ptFmt);
  asynPrint(pasynUser, ASYN_TRACE_FLOW, "xIncr- %g\nptOff- %d\nxZero- %g\nyMult- %g\nyZero- %g\nyOff- %g\n"              , xIncr, ptOff, xZero, yMult, yZero, yOff);
  i += width; /* I should be pointing to the first data byte now */

  /* Copy waveform data from response buffer to waveform record */
  if(P1) {
      asynPrint(pasynUser, ASYN_TRACE_FLOW, "%p, index=%d\n", (void *)&pdpvt->msg[0], i);
      asynPrint(pasynUser, ASYN_TRACE_FLOW, "ptFmt - %s, length - %d\n", ptFmt, length);
      len = P1;   /* if P1 is non-zero, use it as length */
  }
  else {
      len = length;
  }
  memcpy((char *)pwf->bptr, (char *)&pdpvt->msg[i], len*byte);
  pwf->nord=len;
  return 0;
}

/* Use waveform preamble to scale waveforms to volts waveform */
/* Also, read only the first .NELM values */
static int
scaleData(struct gpibDpvt *pdpvt, int P1, int P2, char **P3)
{
  asynUser *pasynUser = pdpvt->pasynUser;
  signed char    *pByte;
  signed short   *pShort;
  float          *pFArray, *pFTime;
  double        xIncr, xZero, yMult, yZero, yOff;
  int byte, bits, length, ptOff, width,nrFrames;
  char encode[20], binFmt[20], byteOr[20], idString[80], ptFmt[20], xUnit[20], yUnit[20];
  int i, j;
  struct waveformRecord *pwf= (struct waveformRecord *)pdpvt->precord;
  gpibCmd *pgpibCmd = gpibCmdGet(pdpvt);
  asynOctet *pasynOctet = pdpvt->pasynOctet;
  void *asynOctetPvt = pdpvt->asynOctetPvt;
  size_t ntrans;

  if (P1 == 0) {
    if (pasynOctet->write(asynOctetPvt,pasynUser,pgpibCmd->cmd,
                               strlen(pgpibCmd->cmd),&ntrans) != asynSuccess) {
      epicsSnprintf(pasynUser->errorMessage,pasynUser->errorMessageSize,
                                                  "scaleData write failed");
      return -1;
    }
    if (pdevSupportGpib->readArbitraryBlockProgramData(pdpvt) <= 0) {
      epicsSnprintf(pasynUser->errorMessage,pasynUser->errorMessageSize,
                              "scaleData readArbitraryBlockProgramData failed");
      return -1;
    }
  }
  i = 0;
  //printf("Message: %s\n", pdpvt->msg);
#if FAMILY==3000
  if (sscanf(pdpvt->msg,"%d;%d;%19[^;];%19[^;];%19[^;];%d;%79[^;];"
                        "%19[^;];%lg;%d;%lg;%19[^;];%lg;%lg;"
                        "%lg;%19[^;];%n",
                    &byte,&bits,encode,binFmt,byteOr,&length,idString,
                    ptFmt,&xIncr,&ptOff,&xZero,xUnit,&yMult,&yZero,
                    &yOff,yUnit,&i) != 16) {
#else
  if (sscanf(pdpvt->msg,"%d;%d;%19[^;];%19[^;];%19[^;];%79[^;];%d;"
						"%19[^;];%19[^;];%lg;%lg;%d;%19[^;];%lg;"
						"%lg;%lg;%d;%n",
         			&byte,&bits,encode,binFmt,byteOr,idString,&length,
                    ptFmt,xUnit,&xIncr,&xZero,&ptOff,yUnit,&yMult,
                    &yOff,&yZero,&nrFrames,&i) != 17) {
#endif
#if 0
//"1;8;ASCII;RI;MSB;"Ch1, DC coupling, 1.000mV/div, 4.000us/div, 10000 points, Sample mode";10000;Y;LINEAR;"s";4.0000E-9;-20.0000E-6;0;"
//"1;8;ASCII;RI;MSB;"Ch1, DC coupling, 1.000mV/div, 4.000us/div, 10000 points, Sample mode";10000;
//Y;"s";4.0000E-9;-20.0000E-6;0;"V";40.0
"Ch1, DC coupling, 1.000mV/div, 4.000us/div, 10000 points, Sample mode"

"1; &byte
8; &bits
ASCII; encode
RI; binFmt
MSB;byteOr
"Ch1, DC coupling, 1.000mV/div, 4.000us/div, 10000 points, Sample mode"; idString
10000; length
Y; ptFmt
"s"; xUnit
4.0000E-9;xIncr
-20.0000E-6; xZero
0; ptOff
"V"; yUnit
40.0
#endif

    epicsSnprintf(pasynUser->errorMessage,pasynUser->errorMessageSize,
                                            "scaleData sscanf failed (\"%s\")",
                                                                pdpvt->msg);

    return -1;
  }
  asynPrint(pasynUser, ASYN_TRACE_FLOW, "%s\n", pwf->name);
  asynPrint(pasynUser, ASYN_TRACE_FLOW, "%p, index=%d\n", (unsigned char *)&pdpvt->msg[0],i);
  asynPrint(pasynUser, ASYN_TRACE_FLOW, "byte- %d, bits- %d, encode- %s, binFmt- %s, byteOr-  %s\n",
         byte, bits, encode, binFmt, byteOr);
  asynPrint(pasynUser, ASYN_TRACE_FLOW, "length- %d, ptFmt- %s\n",
         length, ptFmt);
  asynPrint(pasynUser, ASYN_TRACE_FLOW, "xIncr- %g\nptOff- %d\nxZero- %g\nyMult- %g\nyZero- %g\nyOff- %g\n",                    xIncr, ptOff, xZero, yMult, yZero, yOff);

  switch(P1) {
    case 0: /* Channel Data */
      /* Confirm wf record is expecting floats */
      if(pwf->ftvl != menuFtypeFLOAT) {
        epicsSnprintf(pasynUser->errorMessage,pasynUser->errorMessageSize,
                      "%s.FTVL must be FLOAT\n", pwf->name);
        return -1;
      }
      /*
       * Our TDS5054 sometimes returns just the waveform preamble
       * even though the full waveform was requested.  Give it
       * another chance to return the data.
       */
      if ((i == 0) && (P2 == 0)) {
        printf("scaleData RETRY for %s\n", pwf->name);
        return scaleData(pdpvt, P1, 1, P3);
      }
      if ((i < 60) || (i > 200)) {
        epicsSnprintf(pasynUser->errorMessage,pasynUser->errorMessageSize,
                            "scaleData bad scanf i:%d (\"%s\")", i, pdpvt->msg);
        return -1;
      }
      if (sscanf(pdpvt->msg+i, "#%1d", &width) != 1) {
        epicsSnprintf(pasynUser->errorMessage,pasynUser->errorMessageSize,
                  "Failed to scan width at offset %d (\"%.3s\")\n",
                                                             i, pdpvt->msg+i);
        return -1;
      }
      i += 2 + width; /* I should be pointing to the first data byte now */
        asynPrint(pasynUser, ASYN_TRACE_FLOW, "width- %d    i- %d\n", width, i);
      /* initialize byte pointer and short pointer */
      pByte  = (signed char *)&pdpvt->msg[i];
      pShort= (short *)&pdpvt->msg[i];
      pFArray = (float *)pwf->bptr;

      /* Fill in waveform */
      switch(byte) {
        case 1: /* data type = BYTE */
          for(j=0; ((j<length) && (j<pwf->nelm)); j++, pByte++ ) {
              /* The following conversion yields VOLTS */
              pFArray[j] = (float)(((*pByte - yOff) * yMult) + yZero); 
              /* The following conversion yields DIVISIONS 
              pFArray[j] = (float)(((*pByte - yOff)/yOff * 5.0)); */
          }
          pwf->nord=j;
          break;
        case 2: /* data type = WORD  */
          for(j=0; ((j<length) && (j<pwf->nelm)); j++, pShort++) {
              unsigned char *c1 = (unsigned char *)pShort; 
              unsigned char *c2 = c1 + 1;
              short stmp = (*c1 << 8) | *c2; /* WFMPre:BYT_OR==MSB */
              /* The following conversion yields VOLTS */
              pFArray[j] = (float)(((stmp - yOff) * yMult) + yZero);
              /* The following conversion yields DIVISIONS 
              pFArray[j] = (float)(((stmp - yOff)/yOff * 5.0)); */
          }
          pwf->nord=j;
          break;

        default : /* invalid size */
          epicsSnprintf(pasynUser->errorMessage,pasynUser->errorMessageSize,
                      "%s bad WFRMPre:BYT_Nr.\n",pwf->name);
          return -1;
      }
      break;
      
    case 1: /* Time axis data, in microseconds */
      if(pwf->ftvl != menuFtypeFLOAT) {
        epicsSnprintf(pasynUser->errorMessage,pasynUser->errorMessageSize,
                      "%s.FTVL must be FLOAT\n", pwf->name);
        return -1;
      }
      pFTime = (float *)pwf->bptr; 
      for(j=0; ((j<length) && (j<pwf->nelm)); j++)
          pFTime[j] = (((j-ptOff) * xIncr) + xZero) * 1.0e6; 
      asynPrint(pasynUser, ASYN_TRACE_FLOW, "\nTotal count = %d\n ", j);
      pwf->nord=j;
      break;
  }
  return 0;
}

static int
preamblePoints(struct gpibDpvt *pdpvt, int P1, int P2, char **P3)
{ 
  char preamble[255];
  char *token;
  int byte, bits, length;
  char *binFmt, *byteOr;
  long i;
  struct aiRecord *pai= (struct aiRecord *)pdpvt->precord;

  /* Get waveform preamble  */
  i = 0;
  while (pdpvt->msg[i] != '\0') {
    preamble[i] = pdpvt->msg[i];
    i++;
    if (i == 254) break;
  }
  preamble[i] = '\0';
  token = strtok(&(preamble[0]),";");
  byte = atoi(token);           /* 1 or 2 byte per point */
  token = strtok(NULL,";");
  bits = atoi(token);           /* 8 or 16 bits per point */
  token = strtok(NULL,";");
  binFmt = token;               /* ASCII or Binary */
  token = strtok(NULL,";");
  byteOr = token;               /* byte order */
  token = strtok(NULL,";");
  length = atoi(token);         /* 500 or 10k points per WF */

  /* Copy length (points) value to val field of ai record */
  pai->val = (double)length;

  return 0;
}

static int
preambleCount(struct gpibDpvt *pdpvt, int P1, int P2, char **P3)
{ 
  int count;
  struct aiRecord *pai= (struct aiRecord *)pdpvt->precord;

  /* This Tek scope doesn't have a count value */
  count = 1;

  /* Copy count value to val field of ai record */
  pai->val = (double)count;

  return(0);
}

static int
readSetupData(struct gpibDpvt *pdpvt, int P1, int P2, char **P3)
{ 
  struct waveformRecord *pwf= (struct waveformRecord *)pdpvt->precord;
  int ncopy;

  if(pdpvt->msgInputLen <= pwf->nelm) {
    ncopy = pdpvt->msgInputLen;
  }
  else {
    ncopy = pwf->nelm;
    errlogPrintf("%s readSetupData() truncating %d bytes to %lu byte .NELM\n",
                                    pwf->name, pdpvt->msgInputLen, pwf->nelm);
  }
  memcpy(pwf->bptr,&pdpvt->msg[0],ncopy);
  pwf->nord=ncopy;
  return(0);
}

static int
writeSetupData(struct gpibDpvt *pdpvt, int P1, int P2, char **P3)
{ 
  struct waveformRecord *pwf= (struct waveformRecord *)pdpvt->precord;

  strncpy(&pdpvt->msg[0],":SYST:SET ",10);
  memcpy(&pdpvt->msg[10],pwf->bptr,pwf->nord);
  return(pwf->nord+10);
}

static int
writeStringWf(struct gpibDpvt *pdpvt, int P1, int P2, char **P3)
{ 
  struct waveformRecord *pwf= (struct waveformRecord *)pdpvt->precord;
  int nelm;

  nelm = (pwf->nord < P1)?pwf->nord:P1;
  memcpy(&pdpvt->msg[0],pwf->bptr,nelm);
  return(nelm);
}

static int
getEthernetName(struct gpibDpvt *pdpvt, int P1, int P2, char **P3)
{
  asynUser *pasynUser = pdpvt->pasynUser;
  struct stringinRecord *psi= (struct stringinRecord *)pdpvt->precord;

  asynOctet *pasynOctet = pdpvt->pasynOctet;
  void *asynOctetPvt = pdpvt->asynOctetPvt;
  int j;
  size_t ntrans;
  char cbuf[300];
  char make[12], model[12];
  char netName[40];

  if ((pasynOctet->write(asynOctetPvt,pasynUser,"*IDN?",
                sizeof"*IDN?" - 1,&ntrans) != asynSuccess)
   || (pasynOctet->read(asynOctetPvt,pasynUser,cbuf,
                sizeof cbuf - 1,&ntrans,NULL) != asynSuccess)) {
    epicsSnprintf(pasynUser->errorMessage,pasynUser->errorMessageSize,
                  "Scope ID I/O failed\n");
    return -1;
  }
  cbuf[ntrans] = '\0';   /* Null-terminate reply message */
  /* do the checks in cbuf and optionally send the "ETHERNET:NAME?"
     using the write/read commands like above */

  if (( j = sscanf(cbuf, "%11[^,],%11[^,]", make, model)) != 2) {
    epicsSnprintf(pasynUser->errorMessage,pasynUser->errorMessageSize,
                  "Scope ID scanned %d. ID string: \"%s\"\n", j, cbuf);
    return -1;
  }

  asynPrint(pasynUser, ASYN_TRACE_FLOW, "cbuf = %s\n",cbuf);
  asynPrint(pasynUser, ASYN_TRACE_FLOW, "make = %s\n",make);
  asynPrint(pasynUser, ASYN_TRACE_FLOW, "model = %s\n",model);
  asynPrint(pasynUser, ASYN_TRACE_FLOW, "model[8] = %c\n",model[8]);

  if ((model[8] == 'B')
   || (strstr(cbuf,"TDS3EM") != NULL)) { /* has ethernet option */
    if ((pasynOctet->write(asynOctetPvt,pdpvt->pasynUser,"ETHER:NAME?",
                sizeof"ETHER:NAME?" - 1,&ntrans) != asynSuccess)
     || (pasynOctet->read(asynOctetPvt,pdpvt->pasynUser,cbuf,
                  sizeof cbuf - 1,&ntrans,NULL) != asynSuccess)) {
      epicsSnprintf(pasynUser->errorMessage,pasynUser->errorMessageSize,
                    "Scope ID I/O failed\n");
      return -1;
    }
    cbuf[ntrans] = '\0';   /* Null-terminate reply message */
    sscanf(cbuf, "%39[^\n\r]",netName);
  }
  else {
    sscanf(cbuf, "%18[^\n\r]",netName);
  }
  strcpy(psi->val,netName);
  return 0;
} 
