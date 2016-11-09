program scopeAutoArmPrint
option +r;

/* Written by:
 *
 *	Nick DiMonte
 *	Argonne National Labratory
 *	April 07, 2003
 *
 *
 * Modifications:
 * ==============
 * .01	04-07-2003	npd	initial release
 * .02	06-16-2003	npd	Added SR current and User mode
 *				to prevent unwanted triggers.
 * .03	06-19-2003	npd	Added a time stamp pv, when a
 *				trigger is detected, this script 
 *				will process the timestamp pv.
 * .04	01-19-2004	npd	Modified to work with 3014B network
 *				scope.
 * .05	02-17-2004	npd	Selected network printer and format
 *				type. Also added event status to check
 *				printer communications. This to try
 *				resolve the issue when a printer ggoes
 *				into power save mode and takes longer to
 *				power-up than the scope is willing to
 *				wait.
 *
 */

%%#include <stdio.h>	/* Standard I?O Functions */
%%#include <string.h>	/* String Library Functions */
%%#include <tsDefs.h>	/* Time Stamp Valuse */
%%#include <stdlib.h>	/* Library Functions */
%%#include <epicsThread.h>  /* New Library Functions replace taskLib.h */

#define REMOTE 0
#define LOCAL 1
#define LOCKED 1
#define UNLOCK 0
#define DONE 0
#define OFF 0
#define ON 1
#define MEM1 0
#define SEQ 1
#define RUNSTOP 0
#define RUN 1
#define BUSY 1
#define ENABLE 1
#define DISABLE 0
#define NOUPDATE 1
#define UPDATE 0
#define USER 1
#define ETHERNET 4
#define EPSCOLOR 1

short RemoteCtl;
short RecallSetup;
short FP_LockCtl;
short FP_LockStat;
short AutoPrintArm;
short ScopeBusy;
short ScopeReset;
short AcquireStat;
short SingleSeqMode;
short SingleSeqStat;
short AcquireRun;
short AcquireStop;
short PrintSetup;
short PrintDisplay;
short ClearMenu;
short MsgBoxText;
short MsgBoxTextTMP;
short MsgBoxCtl;
short MsgBoxSize;
short ProcessCtl;
short WF_ScanCtl;
short ScriptScanCtl;
short PrinterPort;
short PrinterFormat;
short RunScanCtl;
assign RemoteCtl      to "{scope}GPIBenableBO";
assign RecallSetup    to "{scope}recallMO";
assign FP_LockCtl     to "{scope}frontPanelLockBO";
assign FP_LockStat    to "{scope}frontPanelLockBI";
assign AutoPrintArm   to "{scope}armPrintAutoBO";
assign ScopeBusy      to "{scope}scopeBusyBI";
assign ScopeReset     to "{scope}resetBO";
assign AcquireStat    to "{scope}acquireStatBI";
assign SingleSeqMode  to "{scope}singleSeqBO";
assign SingleSeqStat  to "{scope}singleSeqBI";
assign AcquireRun     to "{scope}runBO";
assign AcquireStop    to "{scope}stopBO";
assign PrintSetup     to "{scope}printerSetupBO";
assign PrintDisplay   to "{scope}printDisplayBO";
assign ClearMenu      to "{scope}clearMenuBO";
assign MsgBoxText     to "{scope}RF_MessageBoxMO";
assign MsgBoxTextTMP  to "{scope}RF_MessageBoxMO.FFVL";
assign MsgBoxCtl      to "{scope}messageBoxBO";
assign MsgBoxSize     to "{scope}RF_MsgBoxSizeBO";
assign ProcessCtl     to "{scope}scopeInUseMO";
assign WF_ScanCtl     to "{scope}readScaledWfsCC.DISV";
assign ScriptScanCtl  to "{scope}ScriptUpdateCtlBO";
assign PrinterPort    to "{scope}printerPortMO";
assign PrinterFormat  to "{scope}printerFormatMO";
assign RunScanCtl     to "{scope}stopReadRunSQ.SCAN";
monitor RemoteCtl;
monitor FP_LockCtl;
monitor FP_LockStat;
monitor ScopeBusy;
monitor AcquireStat;
monitor AutoPrintArm;
monitor SingleSeqStat;

float SRcurrent;
short UserMode;
short MachineMode;
short TimeStampPROC;
assign SRcurrent     to "S:SRcurrentAI";
assign UserMode      to "S:DesiredMode";
assign MachineMode   to "{scope}MachineModeBO";
assign TimeStampPROC to "{scope}WF_timeStampTS.PROC";
monitor SRcurrent;
monitor UserMode;
monitor MachineMode;

float DESER_Ctl;
float DESER_Stat;
float SESR_Stat;
float EventQ;
short ClrEventQ;
short PrintError;
assign DESER_Ctl      to "{scope}DESERegAO";
assign DESER_Stat     to "{scope}DESERegAI";
assign SESR_Stat      to "{scope}SESRegAI";
assign EventQ         to "{scope}EventQueueAI";
assign ClrEventQ      to "{scope}ClearEventsBO";
assign PrintError     to "{scope}printErrBO";

short RFSYS;
short printCnt;
short printErr;
char rfSys[8];

%%
ss ArmPrintTds3054 {
  state init {
    when( ) {
      printf("\n\nEntering Tds3054 Print & Acquire Seq !!!!\n");
      pvGet(MsgBoxTextTMP);
      RFSYS = MsgBoxTextTMP - 1; /* Sub 1 to match table */
      if ( RFSYS+1 < 5 ) 
	sprintf(rfSys, "RF_HV%d",RFSYS+1);
      else 
	sprintf(rfSys, "RF_LL%d",RFSYS-4);
      printf("Configuring Tds3000 for %s \n",rfSys);
      ScopeReset = 1;
      pvPut(ScopeReset);
%%    epicsThreadSleep(5);       /* 5 second delay */
      MsgBoxSize = 1;
      pvPut(MsgBoxSize);
      MsgBoxText = RFSYS;
      pvPut(MsgBoxText);
      ProcessCtl = 0;  /* Set to AVAILABLE */
      pvPut(ProcessCtl);
      ScriptScanCtl = UPDATE;
      pvPut(ScriptScanCtl);
    } state remoteModeChk
  }
  state remoteModeChk {
    when ( RemoteCtl == REMOTE && AutoPrintArm == ON &&
		((UserMode==USER && MachineMode==USER) || 
		MachineMode == !USER) ) {
      /* WF_ScanCtl = DISABLE;
      pvPut(WF_ScanCtl); */
      RunScanCtl = DISABLE;
      pvPut(RunScanCtl); 
      printf("%s: Auto Print set to ON !!!!\n",rfSys);
    } state configScopeAuto
    when( RemoteCtl == LOCAL || AutoPrintArm == OFF ||
		(UserMode == !USER && MachineMode == USER)) {
      FP_LockCtl = UNLOCK;
      pvPut(FP_LockCtl);
      MsgBoxCtl = OFF;
      pvPut(MsgBoxCtl);
      ProcessCtl = 0;  /* Set to AVAILABLE */
      pvPut(ProcessCtl);
      ScriptScanCtl = UPDATE;
      pvPut(ScriptScanCtl);
      /* WF_ScanCtl = ENABLE;
      pvPut(WF_ScanCtl); */
      printf("%s: Auto Print set to OFF !!!!\n",rfSys); 
    } state remoteModeWait
  }
  state remoteModeWait {
    when ( RemoteCtl == REMOTE && AutoPrintArm == ON &&
		((UserMode==USER && MachineMode==USER) || 
		MachineMode == !USER) ) {
      /* change detected..... */
    } state remoteModeChk
  }
  state configScopeAuto {
    when ( delay(3.0) ) {
      printf("%s: .... configuring scope \n",rfSys); 
      ScriptScanCtl = NOUPDATE;
      pvPut(ScriptScanCtl);
%%    epicsThreadSleep(5); 
      RecallSetup = MEM1;
      pvPut(RecallSetup);
%%    epicsThreadSleep(5); 
      SingleSeqMode = RUNSTOP;
      pvPut(SingleSeqMode);
      ClearMenu = 1;	/* clear menu before locking F.P. controls */
      pvPut(ClearMenu);
      MsgBoxCtl = ON;
      pvPut(MsgBoxCtl);
      MsgBoxSize = 1;
      pvPut(MsgBoxSize);
      MsgBoxText = RFSYS;
      pvPut(MsgBoxText);
      ProcessCtl = 1;  /* Set to Auto Print mode */
      pvPut(ProcessCtl);
%%    epicsThreadSleep(5); 
      FP_LockCtl = LOCKED;
      pvPut(FP_LockCtl);
%%    epicsThreadSleep(5); 
      /* WF_ScanCtl = ENABLE;
      pvPut(WF_ScanCtl); */
    } state chkFP_lock
  } 
  state chkFP_lock {
    when ( FP_LockStat == LOCKED ) {
    } state AcquireStop
    when ( FP_LockStat == UNLOCK ) {
      printf("%s: ERROR: scope F.P. not locked.....\n",rfSys);
    } state remoteModeChk
  } 
  state AcquireStop {
    when ( delay(3.0) ) {
      printf("%s: .... set scope acquire to stop \n",rfSys); 
      AcquireStop = 1;
      pvPut(AcquireStop);
    } state configPrinterPort
  } 
  state configPrinterPort {
    when ( delay(3.0) ) {
      printf("%s: .... configuring scope printer port \n",rfSys); 
      PrintSetup = 1;
      pvPut(PrintSetup);
      PrinterPort = ETHERNET;
      pvPut(PrinterPort);
      PrinterFormat = EPSCOLOR;
      pvPut(PrinterFormat);
      printf("%s: .... checking SR current is > 10mA \n",rfSys); 
    } state chkSRcurrent
  } 
  state chkSRcurrent {
    when ( RemoteCtl == REMOTE && AutoPrintArm == ON &&
		((UserMode==USER && MachineMode==USER) || 
		MachineMode == !USER) && SRcurrent > 10 ) {
      printf("%s: SR current is above 10mA ....\n",rfSys); 
    } state chkAcquireStat
    when ( RemoteCtl == LOCAL || AutoPrintArm == OFF ||
		(UserMode == !USER && MachineMode == USER)) {
      printf("%s: Auto print canceled ....\n",rfSys); 
    } state remoteModeChk
  } 
  state chkAcquireStat {
    when ( RemoteCtl == REMOTE && AutoPrintArm == ON 
		&& AcquireStat == DONE && ScopeBusy == DONE 
		&& ((UserMode==USER && MachineMode==USER) || 
		MachineMode == !USER) ) {
      printf("%s: Scope is NOT busy (good) ....\n",rfSys); 
    } state armForSingleSeq
    when ( RemoteCtl == REMOTE && AutoPrintArm == ON 
		&& AcquireStat == RUN && ScopeBusy == BUSY 
		&& ((UserMode==USER && MachineMode==USER) || 
		MachineMode == !USER) ) {
      printf("%s: Scope is STILL busy  acquiring ....\n",rfSys); 
    } state AcquireStop
    when ( RemoteCtl == REMOTE && AutoPrintArm == ON 
		&& AcquireStat == DONE && ScopeBusy == BUSY 
		&& ((UserMode==USER && MachineMode==USER) || 
		MachineMode == !USER) ) {
      printf("%s: Scope is STILL busy  printing ....\n",rfSys); 
    } state configPrinterPort
    when ( RemoteCtl == REMOTE && AutoPrintArm == ON 
		&& AcquireStat == RUN && ScopeBusy == DONE 
		&& ((UserMode==USER && MachineMode==USER) || 
		MachineMode == !USER) ) {
      printf("%s: Error: scope is in an invalid mode !!!!\n",rfSys); 
      printf("       resetting scope !!!!\n"); 
      /* ScopeReset = 1;
      pvPut(ScopeReset); */
      SingleSeqMode = RUNSTOP;
      pvPut(SingleSeqMode);
      AcquireStop = 1;
      pvPut(AcquireStop);
    } state remoteModeChk
    when ( RemoteCtl == LOCAL || AutoPrintArm == OFF ||
		(UserMode == !USER && MachineMode == USER)) {
      printf("%s: .... chkAcquireStat: auto print canceled \n",rfSys); 
      FP_LockCtl = UNLOCK;
      pvPut(FP_LockCtl);
      ProcessCtl = 0;  /* Set to AVAILABLE */
      pvPut(ProcessCtl);
      MsgBoxCtl = OFF;
      pvPut(MsgBoxCtl);
    } state remoteModeChk
  } 
  state armForSingleSeq {
    when ( delay(3.4) ) {
      printf("%s: .... arming scope for single seq \n",rfSys); 
      SingleSeqMode = SEQ;
      pvPut(SingleSeqMode);
      AcquireRun = RUN;
      pvPut(AcquireRun);
%%    epicsThreadSleep(5);
    } state chkSglSeq
  } 
  state chkSglSeq {
    when ( SingleSeqStat == SEQ) {
    } state waitForSglSeq
    when ( SingleSeqStat == RUNSTOP) {
      printf("%s: ERROR: scope not set for sequence mode \n",rfSys);
    } state remoteModeChk
  } 
  state waitForSglSeq {
    when ( RemoteCtl == REMOTE && AutoPrintArm == ON 
		&& AcquireStat == RUN && ScopeBusy == BUSY 
		&& SingleSeqStat == SEQ && FP_LockStat == LOCKED
		&& ((UserMode==USER && MachineMode==USER) || 
		MachineMode == !USER) ) {
      printf("%s: .... waiting for scope to be armed \n",rfSys); 
    } state waitForTriggerDly
    when ( RemoteCtl == LOCAL || AutoPrintArm == OFF ||
		SingleSeqStat == RUNSTOP || FP_LockStat == UNLOCK
		|| (UserMode == !USER && MachineMode == USER)) {
      printf("%s: .... waitForSglSeq: auto print canceled \n",rfSys); 
      FP_LockCtl = UNLOCK;
      pvPut(FP_LockCtl);
      MsgBoxCtl = OFF;
      pvPut(MsgBoxCtl);
      ProcessCtl = 0;  /* Set to AVAILABLE */
      pvPut(ProcessCtl);
    } state remoteModeChk
  } 
  state waitForTriggerDly {
    when ( delay(3.0) ) {
      printf("%s: .... waiting for scope to trigger \n",rfSys); 
    } state waitForTrigger
  }
  state waitForTrigger {
    when ( RemoteCtl == REMOTE && AutoPrintArm == ON  
		&& AcquireStat == DONE && ScopeBusy == DONE 
		&& SingleSeqStat == SEQ && FP_LockStat == LOCKED
		&& ((UserMode==USER && MachineMode==USER) || 
		MachineMode == !USER) ) {
      printf("%s: .... trigger has been received \n",rfSys); 
      ScriptScanCtl = UPDATE;
      pvPut(ScriptScanCtl);
      TimeStampPROC = 1;
      pvPut(TimeStampPROC);
      printCnt=0;
      PrintError = 0; /* OK */
      pvPut(PrintError);
    } state printDisplayDly
    when ( RemoteCtl == LOCAL || AutoPrintArm == OFF ||
		SingleSeqStat == RUNSTOP || FP_LockStat == UNLOCK
		|| (UserMode == !USER && MachineMode == USER)) {
      printf("%s: .... waitForTrigger: auto print canceled \n",rfSys); 
      printf("     RemoteCtl     = %d\n",RemoteCtl);
      printf("     AutoPrintArm  = %d\n",AutoPrintArm);
      printf("     AcquireStat   = %d\n",AcquireStat);
      printf("     ScopeBusy     = %d\n",ScopeBusy);
      printf("     SingleSeqStat = %d\n",SingleSeqStat);
      printf("     FP_LockStat   = %d\n",FP_LockStat);
      printf("     UserMode      = %d\n",UserMode);
      printf("     MachineMode   = %d\n",MachineMode);
      FP_LockCtl = UNLOCK;
      pvPut(FP_LockCtl);
      ProcessCtl = 0;  /* Set to AVAILABLE */
      pvPut(ProcessCtl);
      MsgBoxCtl = OFF;
      pvPut(MsgBoxCtl);
    } state remoteModeChk
  } 
  state printDisplayDly {
    when ( delay(10.0) ) {
      /* WF_ScanCtl = DISABLE;
      pvPut(WF_ScanCtl); */
      DESER_Ctl = 0;
      pvPut(DESER_Ctl); 
%%    epicsThreadSleep(5);       /* 5 second delay */
      ClrEventQ = 1;
      pvPut(ClrEventQ); 
%%    epicsThreadSleep(5);       /* 5 second delay */
      DESER_Ctl = 16;
      pvPut(DESER_Ctl); 
    } state printDisplay
  }
  state printDisplay {
    when ( delay(2.0) ) {
      PrintDisplay = 1;
      pvPut(PrintDisplay);
      printCnt++;
      printf("%s: Printing Tds3054 display !!!!\n",rfSys);
    } state waitForPrintJobDly
  } 
  state waitForPrintJobDly {
    when ( delay(10.0) ) {
      printf("%s: .... waiting for print to finish \n",rfSys); 
    } state waitForPrintJob
  } 
  state waitForPrintJob {
    when ( RemoteCtl == REMOTE && AutoPrintArm == ON && ScopeBusy == DONE ) {
      printErr=0;
      while ( (pvGet(SESR_Stat) & 0x10) > 0 ){
        pvGet(EventQ);
	printf("%s: Event error #= %g\n",rfSys,EventQ);
        if ( EventQ >= 270 && EventQ <= 287 ){
	  printErr++;
	}	
      }
      printf("%s: .... waiting for print to finish \n",rfSys); 
    } state chkPrintErrors
    when ( RemoteCtl == LOCAL || AutoPrintArm == OFF ||
		(UserMode == !USER && MachineMode == USER)) {
    } state configScopeAuto
  } 
  state chkPrintErrors {
    when ( printErr > 0 && printCnt < 6 ) {
%%    epicsThreadSleep(60);       /* 60 second delay */
      printf("%s: .... error printing, attempting print again \n",rfSys); 
      ClearMenu = 1;	
      pvPut(ClearMenu);
    } state printDisplayDly
    when ( printErr == 0 ) {
      printf("%s: .... print was successful. \n",rfSys); 
      PrintError = 0; /* OK */
      pvPut(PrintError);
    } state PrintJobDone
    when ( printCnt == 6 ) {
      printf("%s: .... printing failed, aborting.... \n",rfSys); 
      PrintError = 1; /* Fault */
      pvPut(PrintError);
    } state PrintJobDone
  } 
  state PrintJobDone {
    when ( RemoteCtl == REMOTE && AutoPrintArm == ON && ScopeBusy == DONE ) {
      ScriptScanCtl = NOUPDATE;
      pvPut(ScriptScanCtl);
      /* WF_ScanCtl = ENABLE;
      pvPut(WF_ScanCtl); */
      printf("%s: Printing Tds3054 DONE !!!!\n",rfSys);
    } state configScopeAuto
    when ( RemoteCtl == LOCAL || AutoPrintArm == OFF ||
		(UserMode == !USER && MachineMode == USER)) {
      printf("%s: .... waitForPrintJob: auto print canceled \n",rfSys); 
      ScriptScanCtl = UPDATE;
      pvPut(ScriptScanCtl);
      /* WF_ScanCtl = ENABLE;
      pvPut(WF_ScanCtl); */
      FP_LockCtl = UNLOCK;
      pvPut(FP_LockCtl);
      ProcessCtl = 0;  /* Set to AVAILABLE */
      pvPut(ProcessCtl);
    } state remoteModeChk
  } 
}

