#ifndef INCsnmpH
#define INCsnmpH
#include "epicsTypes.h"
#include "link.h"
#include "epicsMutex.h"
#include "ellLib.h"
#include "epicsTime.h"
typedef struct snmpRecord {
	char		name[61];	/* Record Name */
	char		desc[41];	/* Descriptor */
	char		asg[29];	/* Access Security Group */
	epicsEnum16	scan;	/* Scan Mechanism */
	epicsEnum16	pini;	/* Process at iocInit */
	epicsInt16	phas;	/* Scan Phase */
	epicsInt16	evnt;	/* Event Number */
	epicsInt16	tse;	/* Time Stamp Event */
	DBLINK		tsel;	/* Time Stamp Link */
	epicsEnum16	dtyp;	/* Device Type */
	epicsInt16	disv;	/* Disable Value */
	epicsInt16	disa;	/* Disable */
	DBLINK		sdis;	/* Scanning Disable */
	epicsMutexId	mlok;	/* Monitor lock */
	ELLLIST		mlis;	/* Monitor List */
	epicsUInt8	disp;	/* Disable putField */
	epicsUInt8	proc;	/* Force Processing */
	epicsEnum16	stat;	/* Alarm Status */
	epicsEnum16	sevr;	/* Alarm Severity */
	epicsEnum16	nsta;	/* New Alarm Status */
	epicsEnum16	nsev;	/* New Alarm Severity */
	epicsEnum16	acks;	/* Alarm Ack Severity */
	epicsEnum16	ackt;	/* Alarm Ack Transient */
	epicsEnum16	diss;	/* Disable Alarm Sevrty */
	epicsUInt8	lcnt;	/* Lock Count */
	epicsUInt8	pact;	/* Record active */
	epicsUInt8	putf;	/* dbPutField process */
	epicsUInt8	rpro;	/* Reprocess  */
	struct asgMember *asp;	/* Access Security Pvt */
	struct putNotify *ppn;	/* addr of PUTNOTIFY */
	struct putNotifyRecord *ppnr;	/* pputNotifyRecord */
	struct scan_element *spvt;	/* Scan Private */
	struct rset	*rset;	/* Address of RSET */
	struct dset	*dset;	/* DSET address */
	void		*dpvt;	/* Device Private */
	struct dbRecordType *rdes;	/* Address of dbRecordType */
	struct lockRecord *lset;	/* Lock Set */
	epicsEnum16	prio;	/* Scheduling Priority */
	epicsUInt8	tpro;	/* Trace Processing */
	char bkpt;	/* Break Point */
	epicsUInt8	udf;	/* Undefined */
	epicsTimeStamp	time;	/* Time */
	DBLINK		flnk;	/* Forward Process Link */
	epicsFloat64	val;	/* Current Value */
	char		host[81];	/* IP Address */
	char		comm[81];	/* Community */
	char		oids[81];	/* OID Name */
	char		auth[81];	/* auth password */
	char		priv[81];	/* priv password */
	char		vers[81];	/* SNMP Version */
	DBLINK		inp;	/* Input Specification */
	epicsInt16	prec;	/* Display Precision */
	epicsEnum16	linr;	/* Linearization */
	epicsFloat64	eguf;	/* Engineer Units Full */
	epicsFloat64	egul;	/* Engineer Units Low */
	char		egu[16];	/* Engineering Units */
	epicsFloat64	hopr;	/* High Operating Range */
	epicsFloat64	lopr;	/* Low Operating Range */
	epicsFloat64	aoff;	/* Adjustment Offset */
	epicsFloat64	aslo;	/* Adjustment Slope */
	epicsFloat64	smoo;	/* Smoothing */
	epicsFloat64	hihi;	/* Hihi Alarm Limit */
	epicsFloat64	lolo;	/* Lolo Alarm Limit */
	epicsFloat64	high;	/* High Alarm Limit */
	epicsFloat64	low;	/* Low Alarm Limit */
	epicsEnum16	hhsv;	/* Hihi Severity */
	epicsEnum16	llsv;	/* Lolo Severity */
	epicsEnum16	hsv;	/* High Severity */
	epicsEnum16	lsv;	/* Low Severity */
	epicsFloat64	hyst;	/* Alarm Deadband */
	epicsFloat64	adel;	/* Archive Deadband */
	epicsFloat64	mdel;	/* Monitor Deadband */
	epicsFloat64	lalm;	/* Last Value Alarmed */
	epicsFloat64	alst;	/* Last Value Archived */
	epicsFloat64	mlst;	/* Last Val Monitored */
	epicsFloat64	eslo;	/* Raw to EGU Slope */
	epicsFloat64	eoff;	/* Raw to EGU Offset */
	epicsInt32	roff;	/* Raw Offset, obsolete */
	void *	pbrk;	/* Ptrto brkTable */
	epicsInt16	init;	/* Initialized? */
	epicsInt16	lbrk;	/* LastBreak Point */
	epicsInt32	rval;	/* Current Raw Value */
	epicsInt32	oraw;	/* Previous Raw Value */
	DBLINK		siol;	/* Sim Input Specifctn */
	epicsFloat64	sval;	/* Simulation Value */
	DBLINK		siml;	/* Sim Mode Location */
	epicsEnum16	simm;	/* Simulation Mode */
	epicsEnum16	sims;	/* Sim mode Alarm Svrty */
	DBLINK		out;	/* Output Specification */
	epicsFloat64	oval;	/* Old Value */
	epicsFloat64	drvh;	/* Drive High Limit */
	epicsFloat64	drvl;	/* Drive Low Limit */
} snmpRecord;
#define snmpRecordNAME	0
#define snmpRecordDESC	1
#define snmpRecordASG	2
#define snmpRecordSCAN	3
#define snmpRecordPINI	4
#define snmpRecordPHAS	5
#define snmpRecordEVNT	6
#define snmpRecordTSE	7
#define snmpRecordTSEL	8
#define snmpRecordDTYP	9
#define snmpRecordDISV	10
#define snmpRecordDISA	11
#define snmpRecordSDIS	12
#define snmpRecordMLOK	13
#define snmpRecordMLIS	14
#define snmpRecordDISP	15
#define snmpRecordPROC	16
#define snmpRecordSTAT	17
#define snmpRecordSEVR	18
#define snmpRecordNSTA	19
#define snmpRecordNSEV	20
#define snmpRecordACKS	21
#define snmpRecordACKT	22
#define snmpRecordDISS	23
#define snmpRecordLCNT	24
#define snmpRecordPACT	25
#define snmpRecordPUTF	26
#define snmpRecordRPRO	27
#define snmpRecordASP	28
#define snmpRecordPPN	29
#define snmpRecordPPNR	30
#define snmpRecordSPVT	31
#define snmpRecordRSET	32
#define snmpRecordDSET	33
#define snmpRecordDPVT	34
#define snmpRecordRDES	35
#define snmpRecordLSET	36
#define snmpRecordPRIO	37
#define snmpRecordTPRO	38
#define snmpRecordBKPT	39
#define snmpRecordUDF	40
#define snmpRecordTIME	41
#define snmpRecordFLNK	42
#define snmpRecordVAL	43
#define snmpRecordHOST	44
#define snmpRecordCOMM	45
#define snmpRecordOIDS	46
#define snmpRecordAUTH	47
#define snmpRecordPRIV	48
#define snmpRecordVERS	49
#define snmpRecordINP	50
#define snmpRecordPREC	51
#define snmpRecordLINR	52
#define snmpRecordEGUF	53
#define snmpRecordEGUL	54
#define snmpRecordEGU	55
#define snmpRecordHOPR	56
#define snmpRecordLOPR	57
#define snmpRecordAOFF	58
#define snmpRecordASLO	59
#define snmpRecordSMOO	60
#define snmpRecordHIHI	61
#define snmpRecordLOLO	62
#define snmpRecordHIGH	63
#define snmpRecordLOW	64
#define snmpRecordHHSV	65
#define snmpRecordLLSV	66
#define snmpRecordHSV	67
#define snmpRecordLSV	68
#define snmpRecordHYST	69
#define snmpRecordADEL	70
#define snmpRecordMDEL	71
#define snmpRecordLALM	72
#define snmpRecordALST	73
#define snmpRecordMLST	74
#define snmpRecordESLO	75
#define snmpRecordEOFF	76
#define snmpRecordROFF	77
#define snmpRecordPBRK	78
#define snmpRecordINIT	79
#define snmpRecordLBRK	80
#define snmpRecordRVAL	81
#define snmpRecordORAW	82
#define snmpRecordSIOL	83
#define snmpRecordSVAL	84
#define snmpRecordSIML	85
#define snmpRecordSIMM	86
#define snmpRecordSIMS	87
#define snmpRecordOUT	88
#define snmpRecordOVAL	89
#define snmpRecordDRVH	90
#define snmpRecordDRVL	91
#endif /*INCsnmpH*/
#ifdef GEN_SIZE_OFFSET
#ifdef __cplusplus
extern "C" {
#endif
#include <epicsAssert.h>
#include <epicsExport.h>
static int snmpRecordSizeOffset(dbRecordType *pdbRecordType)
{
    snmpRecord *prec = 0;
  assert(pdbRecordType->no_fields==92);
  pdbRecordType->papFldDes[0]->size=sizeof(prec->name);
  pdbRecordType->papFldDes[0]->offset=(short)((char *)&prec->name - (char *)prec);
  pdbRecordType->papFldDes[1]->size=sizeof(prec->desc);
  pdbRecordType->papFldDes[1]->offset=(short)((char *)&prec->desc - (char *)prec);
  pdbRecordType->papFldDes[2]->size=sizeof(prec->asg);
  pdbRecordType->papFldDes[2]->offset=(short)((char *)&prec->asg - (char *)prec);
  pdbRecordType->papFldDes[3]->size=sizeof(prec->scan);
  pdbRecordType->papFldDes[3]->offset=(short)((char *)&prec->scan - (char *)prec);
  pdbRecordType->papFldDes[4]->size=sizeof(prec->pini);
  pdbRecordType->papFldDes[4]->offset=(short)((char *)&prec->pini - (char *)prec);
  pdbRecordType->papFldDes[5]->size=sizeof(prec->phas);
  pdbRecordType->papFldDes[5]->offset=(short)((char *)&prec->phas - (char *)prec);
  pdbRecordType->papFldDes[6]->size=sizeof(prec->evnt);
  pdbRecordType->papFldDes[6]->offset=(short)((char *)&prec->evnt - (char *)prec);
  pdbRecordType->papFldDes[7]->size=sizeof(prec->tse);
  pdbRecordType->papFldDes[7]->offset=(short)((char *)&prec->tse - (char *)prec);
  pdbRecordType->papFldDes[8]->size=sizeof(prec->tsel);
  pdbRecordType->papFldDes[8]->offset=(short)((char *)&prec->tsel - (char *)prec);
  pdbRecordType->papFldDes[9]->size=sizeof(prec->dtyp);
  pdbRecordType->papFldDes[9]->offset=(short)((char *)&prec->dtyp - (char *)prec);
  pdbRecordType->papFldDes[10]->size=sizeof(prec->disv);
  pdbRecordType->papFldDes[10]->offset=(short)((char *)&prec->disv - (char *)prec);
  pdbRecordType->papFldDes[11]->size=sizeof(prec->disa);
  pdbRecordType->papFldDes[11]->offset=(short)((char *)&prec->disa - (char *)prec);
  pdbRecordType->papFldDes[12]->size=sizeof(prec->sdis);
  pdbRecordType->papFldDes[12]->offset=(short)((char *)&prec->sdis - (char *)prec);
  pdbRecordType->papFldDes[13]->size=sizeof(prec->mlok);
  pdbRecordType->papFldDes[13]->offset=(short)((char *)&prec->mlok - (char *)prec);
  pdbRecordType->papFldDes[14]->size=sizeof(prec->mlis);
  pdbRecordType->papFldDes[14]->offset=(short)((char *)&prec->mlis - (char *)prec);
  pdbRecordType->papFldDes[15]->size=sizeof(prec->disp);
  pdbRecordType->papFldDes[15]->offset=(short)((char *)&prec->disp - (char *)prec);
  pdbRecordType->papFldDes[16]->size=sizeof(prec->proc);
  pdbRecordType->papFldDes[16]->offset=(short)((char *)&prec->proc - (char *)prec);
  pdbRecordType->papFldDes[17]->size=sizeof(prec->stat);
  pdbRecordType->papFldDes[17]->offset=(short)((char *)&prec->stat - (char *)prec);
  pdbRecordType->papFldDes[18]->size=sizeof(prec->sevr);
  pdbRecordType->papFldDes[18]->offset=(short)((char *)&prec->sevr - (char *)prec);
  pdbRecordType->papFldDes[19]->size=sizeof(prec->nsta);
  pdbRecordType->papFldDes[19]->offset=(short)((char *)&prec->nsta - (char *)prec);
  pdbRecordType->papFldDes[20]->size=sizeof(prec->nsev);
  pdbRecordType->papFldDes[20]->offset=(short)((char *)&prec->nsev - (char *)prec);
  pdbRecordType->papFldDes[21]->size=sizeof(prec->acks);
  pdbRecordType->papFldDes[21]->offset=(short)((char *)&prec->acks - (char *)prec);
  pdbRecordType->papFldDes[22]->size=sizeof(prec->ackt);
  pdbRecordType->papFldDes[22]->offset=(short)((char *)&prec->ackt - (char *)prec);
  pdbRecordType->papFldDes[23]->size=sizeof(prec->diss);
  pdbRecordType->papFldDes[23]->offset=(short)((char *)&prec->diss - (char *)prec);
  pdbRecordType->papFldDes[24]->size=sizeof(prec->lcnt);
  pdbRecordType->papFldDes[24]->offset=(short)((char *)&prec->lcnt - (char *)prec);
  pdbRecordType->papFldDes[25]->size=sizeof(prec->pact);
  pdbRecordType->papFldDes[25]->offset=(short)((char *)&prec->pact - (char *)prec);
  pdbRecordType->papFldDes[26]->size=sizeof(prec->putf);
  pdbRecordType->papFldDes[26]->offset=(short)((char *)&prec->putf - (char *)prec);
  pdbRecordType->papFldDes[27]->size=sizeof(prec->rpro);
  pdbRecordType->papFldDes[27]->offset=(short)((char *)&prec->rpro - (char *)prec);
  pdbRecordType->papFldDes[28]->size=sizeof(prec->asp);
  pdbRecordType->papFldDes[28]->offset=(short)((char *)&prec->asp - (char *)prec);
  pdbRecordType->papFldDes[29]->size=sizeof(prec->ppn);
  pdbRecordType->papFldDes[29]->offset=(short)((char *)&prec->ppn - (char *)prec);
  pdbRecordType->papFldDes[30]->size=sizeof(prec->ppnr);
  pdbRecordType->papFldDes[30]->offset=(short)((char *)&prec->ppnr - (char *)prec);
  pdbRecordType->papFldDes[31]->size=sizeof(prec->spvt);
  pdbRecordType->papFldDes[31]->offset=(short)((char *)&prec->spvt - (char *)prec);
  pdbRecordType->papFldDes[32]->size=sizeof(prec->rset);
  pdbRecordType->papFldDes[32]->offset=(short)((char *)&prec->rset - (char *)prec);
  pdbRecordType->papFldDes[33]->size=sizeof(prec->dset);
  pdbRecordType->papFldDes[33]->offset=(short)((char *)&prec->dset - (char *)prec);
  pdbRecordType->papFldDes[34]->size=sizeof(prec->dpvt);
  pdbRecordType->papFldDes[34]->offset=(short)((char *)&prec->dpvt - (char *)prec);
  pdbRecordType->papFldDes[35]->size=sizeof(prec->rdes);
  pdbRecordType->papFldDes[35]->offset=(short)((char *)&prec->rdes - (char *)prec);
  pdbRecordType->papFldDes[36]->size=sizeof(prec->lset);
  pdbRecordType->papFldDes[36]->offset=(short)((char *)&prec->lset - (char *)prec);
  pdbRecordType->papFldDes[37]->size=sizeof(prec->prio);
  pdbRecordType->papFldDes[37]->offset=(short)((char *)&prec->prio - (char *)prec);
  pdbRecordType->papFldDes[38]->size=sizeof(prec->tpro);
  pdbRecordType->papFldDes[38]->offset=(short)((char *)&prec->tpro - (char *)prec);
  pdbRecordType->papFldDes[39]->size=sizeof(prec->bkpt);
  pdbRecordType->papFldDes[39]->offset=(short)((char *)&prec->bkpt - (char *)prec);
  pdbRecordType->papFldDes[40]->size=sizeof(prec->udf);
  pdbRecordType->papFldDes[40]->offset=(short)((char *)&prec->udf - (char *)prec);
  pdbRecordType->papFldDes[41]->size=sizeof(prec->time);
  pdbRecordType->papFldDes[41]->offset=(short)((char *)&prec->time - (char *)prec);
  pdbRecordType->papFldDes[42]->size=sizeof(prec->flnk);
  pdbRecordType->papFldDes[42]->offset=(short)((char *)&prec->flnk - (char *)prec);
  pdbRecordType->papFldDes[43]->size=sizeof(prec->val);
  pdbRecordType->papFldDes[43]->offset=(short)((char *)&prec->val - (char *)prec);
  pdbRecordType->papFldDes[44]->size=sizeof(prec->host);
  pdbRecordType->papFldDes[44]->offset=(short)((char *)&prec->host - (char *)prec);
  pdbRecordType->papFldDes[45]->size=sizeof(prec->comm);
  pdbRecordType->papFldDes[45]->offset=(short)((char *)&prec->comm - (char *)prec);
  pdbRecordType->papFldDes[46]->size=sizeof(prec->oids);
  pdbRecordType->papFldDes[46]->offset=(short)((char *)&prec->oids - (char *)prec);
  pdbRecordType->papFldDes[47]->size=sizeof(prec->auth);
  pdbRecordType->papFldDes[47]->offset=(short)((char *)&prec->auth - (char *)prec);
  pdbRecordType->papFldDes[48]->size=sizeof(prec->priv);
  pdbRecordType->papFldDes[48]->offset=(short)((char *)&prec->priv - (char *)prec);
  pdbRecordType->papFldDes[49]->size=sizeof(prec->vers);
  pdbRecordType->papFldDes[49]->offset=(short)((char *)&prec->vers - (char *)prec);
  pdbRecordType->papFldDes[50]->size=sizeof(prec->inp);
  pdbRecordType->papFldDes[50]->offset=(short)((char *)&prec->inp - (char *)prec);
  pdbRecordType->papFldDes[51]->size=sizeof(prec->prec);
  pdbRecordType->papFldDes[51]->offset=(short)((char *)&prec->prec - (char *)prec);
  pdbRecordType->papFldDes[52]->size=sizeof(prec->linr);
  pdbRecordType->papFldDes[52]->offset=(short)((char *)&prec->linr - (char *)prec);
  pdbRecordType->papFldDes[53]->size=sizeof(prec->eguf);
  pdbRecordType->papFldDes[53]->offset=(short)((char *)&prec->eguf - (char *)prec);
  pdbRecordType->papFldDes[54]->size=sizeof(prec->egul);
  pdbRecordType->papFldDes[54]->offset=(short)((char *)&prec->egul - (char *)prec);
  pdbRecordType->papFldDes[55]->size=sizeof(prec->egu);
  pdbRecordType->papFldDes[55]->offset=(short)((char *)&prec->egu - (char *)prec);
  pdbRecordType->papFldDes[56]->size=sizeof(prec->hopr);
  pdbRecordType->papFldDes[56]->offset=(short)((char *)&prec->hopr - (char *)prec);
  pdbRecordType->papFldDes[57]->size=sizeof(prec->lopr);
  pdbRecordType->papFldDes[57]->offset=(short)((char *)&prec->lopr - (char *)prec);
  pdbRecordType->papFldDes[58]->size=sizeof(prec->aoff);
  pdbRecordType->papFldDes[58]->offset=(short)((char *)&prec->aoff - (char *)prec);
  pdbRecordType->papFldDes[59]->size=sizeof(prec->aslo);
  pdbRecordType->papFldDes[59]->offset=(short)((char *)&prec->aslo - (char *)prec);
  pdbRecordType->papFldDes[60]->size=sizeof(prec->smoo);
  pdbRecordType->papFldDes[60]->offset=(short)((char *)&prec->smoo - (char *)prec);
  pdbRecordType->papFldDes[61]->size=sizeof(prec->hihi);
  pdbRecordType->papFldDes[61]->offset=(short)((char *)&prec->hihi - (char *)prec);
  pdbRecordType->papFldDes[62]->size=sizeof(prec->lolo);
  pdbRecordType->papFldDes[62]->offset=(short)((char *)&prec->lolo - (char *)prec);
  pdbRecordType->papFldDes[63]->size=sizeof(prec->high);
  pdbRecordType->papFldDes[63]->offset=(short)((char *)&prec->high - (char *)prec);
  pdbRecordType->papFldDes[64]->size=sizeof(prec->low);
  pdbRecordType->papFldDes[64]->offset=(short)((char *)&prec->low - (char *)prec);
  pdbRecordType->papFldDes[65]->size=sizeof(prec->hhsv);
  pdbRecordType->papFldDes[65]->offset=(short)((char *)&prec->hhsv - (char *)prec);
  pdbRecordType->papFldDes[66]->size=sizeof(prec->llsv);
  pdbRecordType->papFldDes[66]->offset=(short)((char *)&prec->llsv - (char *)prec);
  pdbRecordType->papFldDes[67]->size=sizeof(prec->hsv);
  pdbRecordType->papFldDes[67]->offset=(short)((char *)&prec->hsv - (char *)prec);
  pdbRecordType->papFldDes[68]->size=sizeof(prec->lsv);
  pdbRecordType->papFldDes[68]->offset=(short)((char *)&prec->lsv - (char *)prec);
  pdbRecordType->papFldDes[69]->size=sizeof(prec->hyst);
  pdbRecordType->papFldDes[69]->offset=(short)((char *)&prec->hyst - (char *)prec);
  pdbRecordType->papFldDes[70]->size=sizeof(prec->adel);
  pdbRecordType->papFldDes[70]->offset=(short)((char *)&prec->adel - (char *)prec);
  pdbRecordType->papFldDes[71]->size=sizeof(prec->mdel);
  pdbRecordType->papFldDes[71]->offset=(short)((char *)&prec->mdel - (char *)prec);
  pdbRecordType->papFldDes[72]->size=sizeof(prec->lalm);
  pdbRecordType->papFldDes[72]->offset=(short)((char *)&prec->lalm - (char *)prec);
  pdbRecordType->papFldDes[73]->size=sizeof(prec->alst);
  pdbRecordType->papFldDes[73]->offset=(short)((char *)&prec->alst - (char *)prec);
  pdbRecordType->papFldDes[74]->size=sizeof(prec->mlst);
  pdbRecordType->papFldDes[74]->offset=(short)((char *)&prec->mlst - (char *)prec);
  pdbRecordType->papFldDes[75]->size=sizeof(prec->eslo);
  pdbRecordType->papFldDes[75]->offset=(short)((char *)&prec->eslo - (char *)prec);
  pdbRecordType->papFldDes[76]->size=sizeof(prec->eoff);
  pdbRecordType->papFldDes[76]->offset=(short)((char *)&prec->eoff - (char *)prec);
  pdbRecordType->papFldDes[77]->size=sizeof(prec->roff);
  pdbRecordType->papFldDes[77]->offset=(short)((char *)&prec->roff - (char *)prec);
  pdbRecordType->papFldDes[78]->size=sizeof(prec->pbrk);
  pdbRecordType->papFldDes[78]->offset=(short)((char *)&prec->pbrk - (char *)prec);
  pdbRecordType->papFldDes[79]->size=sizeof(prec->init);
  pdbRecordType->papFldDes[79]->offset=(short)((char *)&prec->init - (char *)prec);
  pdbRecordType->papFldDes[80]->size=sizeof(prec->lbrk);
  pdbRecordType->papFldDes[80]->offset=(short)((char *)&prec->lbrk - (char *)prec);
  pdbRecordType->papFldDes[81]->size=sizeof(prec->rval);
  pdbRecordType->papFldDes[81]->offset=(short)((char *)&prec->rval - (char *)prec);
  pdbRecordType->papFldDes[82]->size=sizeof(prec->oraw);
  pdbRecordType->papFldDes[82]->offset=(short)((char *)&prec->oraw - (char *)prec);
  pdbRecordType->papFldDes[83]->size=sizeof(prec->siol);
  pdbRecordType->papFldDes[83]->offset=(short)((char *)&prec->siol - (char *)prec);
  pdbRecordType->papFldDes[84]->size=sizeof(prec->sval);
  pdbRecordType->papFldDes[84]->offset=(short)((char *)&prec->sval - (char *)prec);
  pdbRecordType->papFldDes[85]->size=sizeof(prec->siml);
  pdbRecordType->papFldDes[85]->offset=(short)((char *)&prec->siml - (char *)prec);
  pdbRecordType->papFldDes[86]->size=sizeof(prec->simm);
  pdbRecordType->papFldDes[86]->offset=(short)((char *)&prec->simm - (char *)prec);
  pdbRecordType->papFldDes[87]->size=sizeof(prec->sims);
  pdbRecordType->papFldDes[87]->offset=(short)((char *)&prec->sims - (char *)prec);
  pdbRecordType->papFldDes[88]->size=sizeof(prec->out);
  pdbRecordType->papFldDes[88]->offset=(short)((char *)&prec->out - (char *)prec);
  pdbRecordType->papFldDes[89]->size=sizeof(prec->oval);
  pdbRecordType->papFldDes[89]->offset=(short)((char *)&prec->oval - (char *)prec);
  pdbRecordType->papFldDes[90]->size=sizeof(prec->drvh);
  pdbRecordType->papFldDes[90]->offset=(short)((char *)&prec->drvh - (char *)prec);
  pdbRecordType->papFldDes[91]->size=sizeof(prec->drvl);
  pdbRecordType->papFldDes[91]->offset=(short)((char *)&prec->drvl - (char *)prec);
    pdbRecordType->rec_size = sizeof(*prec);
    return(0);
}
epicsExportRegistrar(snmpRecordSizeOffset);
#ifdef __cplusplus
}
#endif
#endif /*GEN_SIZE_OFFSET*/
