#ifndef INCicmpH
#define INCicmpH
#include "epicsTypes.h"
#include "link.h"
#include "epicsMutex.h"
#include "ellLib.h"
#include "epicsTime.h"
typedef struct icmpRecord {
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
	char		val[40];	/* Current value */
	char		oval[40];	/* Previous Value */
	char		egu[16];	/* Units name */
	epicsInt32	hopr;	/* High Operating Range */
	epicsInt32	lopr;	/* Low Operating Range */
	epicsInt32	hihi;	/* Hihi Alarm Limit */
	epicsInt32	lolo;	/* Lolo Alarm Limit */
	epicsInt32	high;	/* High Alarm Limit */
	epicsInt32	low;	/* Low Alarm Limit */
	epicsEnum16	hhsv;	/* Hihi Severity */
	epicsEnum16	llsv;	/* Lolo Severity */
	epicsEnum16	hsv;	/* High Severity */
	epicsEnum16	lsv;	/* Low Severity */
	epicsInt32	hyst;	/* Alarm Deadband */
	epicsInt32	adel;	/* Archive Deadband */
	epicsInt32	mdel;	/* Monitor Deadband */
	epicsInt32	lalm;	/* Last Value Alarmed */
	epicsInt32	alst;	/* Last Value Archived */
	epicsInt32	mlst;	/* Last Val Monitored */
	DBLINK		siol;	/* Sim Input Specifctn */
	epicsInt32	sval;	/* Simulation Value */
	DBLINK		siml;	/* Sim Mode Location */
	epicsEnum16	simm;	/* Simulation Mode */
	epicsEnum16	sims;	/* Sim mode Alarm Svrty */
	DBLINK		inp;	/* Input Specification */
	DBLINK		inpa;	/* Input A */
	DBLINK		inpb;	/* Input B */
	DBLINK		inpc;	/* Input C */
	DBLINK		inpd;	/* Input D */
	DBLINK		inpe;	/* Input E */
	DBLINK		inpf;	/* Input F */
	DBLINK		inpg;	/* Input G */
	DBLINK		inph;	/* Input H */
	DBLINK		inpi;	/* Input I */
	DBLINK		inpj;	/* Input J */
	DBLINK		inpk;	/* Input K */
	DBLINK		inpl;	/* Input L */
	DBLINK		inpm;	/* Input M */
	DBLINK		inpn;	/* Input N */
	DBLINK		inpo;	/* Input O */
	DBLINK		inpp;	/* Input P */
	DBLINK		inpq;	/* Input Q */
	DBLINK		inpr;	/* Input R */
	DBLINK		inps;	/* Input S */
	DBLINK		inpt;	/* Input T */
	DBLINK		inpu;	/* Input U */
	DBLINK		inpv;	/* Input V */
	DBLINK		inpw;	/* Input W */
	DBLINK		inpx;	/* Input X */
	DBLINK		inpy;	/* Input Y */
	DBLINK		inpz;	/* Input Z */
	DBLINK		inpaa;	/* Input A */
	DBLINK		inpab;	/* Input B */
	DBLINK		inpac;	/* Input C */
	DBLINK		inpad;	/* Input D */
	DBLINK		inpae;	/* Input E */
	DBLINK		inpaf;	/* Input F */
	DBLINK		inpag;	/* Input G */
	DBLINK		inpah;	/* Input H */
	DBLINK		inpai;	/* Input I */
	DBLINK		inpaj;	/* Input J */
	DBLINK		inpak;	/* Input K */
	DBLINK		inpal;	/* Input L */
	DBLINK		inpam;	/* Input M */
	DBLINK		inpan;	/* Input N */
} icmpRecord;
#define icmpRecordNAME	0
#define icmpRecordDESC	1
#define icmpRecordASG	2
#define icmpRecordSCAN	3
#define icmpRecordPINI	4
#define icmpRecordPHAS	5
#define icmpRecordEVNT	6
#define icmpRecordTSE	7
#define icmpRecordTSEL	8
#define icmpRecordDTYP	9
#define icmpRecordDISV	10
#define icmpRecordDISA	11
#define icmpRecordSDIS	12
#define icmpRecordMLOK	13
#define icmpRecordMLIS	14
#define icmpRecordDISP	15
#define icmpRecordPROC	16
#define icmpRecordSTAT	17
#define icmpRecordSEVR	18
#define icmpRecordNSTA	19
#define icmpRecordNSEV	20
#define icmpRecordACKS	21
#define icmpRecordACKT	22
#define icmpRecordDISS	23
#define icmpRecordLCNT	24
#define icmpRecordPACT	25
#define icmpRecordPUTF	26
#define icmpRecordRPRO	27
#define icmpRecordASP	28
#define icmpRecordPPN	29
#define icmpRecordPPNR	30
#define icmpRecordSPVT	31
#define icmpRecordRSET	32
#define icmpRecordDSET	33
#define icmpRecordDPVT	34
#define icmpRecordRDES	35
#define icmpRecordLSET	36
#define icmpRecordPRIO	37
#define icmpRecordTPRO	38
#define icmpRecordBKPT	39
#define icmpRecordUDF	40
#define icmpRecordTIME	41
#define icmpRecordFLNK	42
#define icmpRecordVAL	43
#define icmpRecordOVAL	44
#define icmpRecordEGU	45
#define icmpRecordHOPR	46
#define icmpRecordLOPR	47
#define icmpRecordHIHI	48
#define icmpRecordLOLO	49
#define icmpRecordHIGH	50
#define icmpRecordLOW	51
#define icmpRecordHHSV	52
#define icmpRecordLLSV	53
#define icmpRecordHSV	54
#define icmpRecordLSV	55
#define icmpRecordHYST	56
#define icmpRecordADEL	57
#define icmpRecordMDEL	58
#define icmpRecordLALM	59
#define icmpRecordALST	60
#define icmpRecordMLST	61
#define icmpRecordSIOL	62
#define icmpRecordSVAL	63
#define icmpRecordSIML	64
#define icmpRecordSIMM	65
#define icmpRecordSIMS	66
#define icmpRecordINP	67
#define icmpRecordINPA	68
#define icmpRecordINPB	69
#define icmpRecordINPC	70
#define icmpRecordINPD	71
#define icmpRecordINPE	72
#define icmpRecordINPF	73
#define icmpRecordINPG	74
#define icmpRecordINPH	75
#define icmpRecordINPI	76
#define icmpRecordINPJ	77
#define icmpRecordINPK	78
#define icmpRecordINPL	79
#define icmpRecordINPM	80
#define icmpRecordINPN	81
#define icmpRecordINPO	82
#define icmpRecordINPP	83
#define icmpRecordINPQ	84
#define icmpRecordINPR	85
#define icmpRecordINPS	86
#define icmpRecordINPT	87
#define icmpRecordINPU	88
#define icmpRecordINPV	89
#define icmpRecordINPW	90
#define icmpRecordINPX	91
#define icmpRecordINPY	92
#define icmpRecordINPZ	93
#define icmpRecordINPAA	94
#define icmpRecordINPAB	95
#define icmpRecordINPAC	96
#define icmpRecordINPAD	97
#define icmpRecordINPAE	98
#define icmpRecordINPAF	99
#define icmpRecordINPAG	100
#define icmpRecordINPAH	101
#define icmpRecordINPAI	102
#define icmpRecordINPAJ	103
#define icmpRecordINPAK	104
#define icmpRecordINPAL	105
#define icmpRecordINPAM	106
#define icmpRecordINPAN	107
#endif /*INCicmpH*/
#ifdef GEN_SIZE_OFFSET
#ifdef __cplusplus
extern "C" {
#endif
#include <epicsAssert.h>
#include <epicsExport.h>
static int icmpRecordSizeOffset(dbRecordType *pdbRecordType)
{
    icmpRecord *prec = 0;
  assert(pdbRecordType->no_fields==108);
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
  pdbRecordType->papFldDes[44]->size=sizeof(prec->oval);
  pdbRecordType->papFldDes[44]->offset=(short)((char *)&prec->oval - (char *)prec);
  pdbRecordType->papFldDes[45]->size=sizeof(prec->egu);
  pdbRecordType->papFldDes[45]->offset=(short)((char *)&prec->egu - (char *)prec);
  pdbRecordType->papFldDes[46]->size=sizeof(prec->hopr);
  pdbRecordType->papFldDes[46]->offset=(short)((char *)&prec->hopr - (char *)prec);
  pdbRecordType->papFldDes[47]->size=sizeof(prec->lopr);
  pdbRecordType->papFldDes[47]->offset=(short)((char *)&prec->lopr - (char *)prec);
  pdbRecordType->papFldDes[48]->size=sizeof(prec->hihi);
  pdbRecordType->papFldDes[48]->offset=(short)((char *)&prec->hihi - (char *)prec);
  pdbRecordType->papFldDes[49]->size=sizeof(prec->lolo);
  pdbRecordType->papFldDes[49]->offset=(short)((char *)&prec->lolo - (char *)prec);
  pdbRecordType->papFldDes[50]->size=sizeof(prec->high);
  pdbRecordType->papFldDes[50]->offset=(short)((char *)&prec->high - (char *)prec);
  pdbRecordType->papFldDes[51]->size=sizeof(prec->low);
  pdbRecordType->papFldDes[51]->offset=(short)((char *)&prec->low - (char *)prec);
  pdbRecordType->papFldDes[52]->size=sizeof(prec->hhsv);
  pdbRecordType->papFldDes[52]->offset=(short)((char *)&prec->hhsv - (char *)prec);
  pdbRecordType->papFldDes[53]->size=sizeof(prec->llsv);
  pdbRecordType->papFldDes[53]->offset=(short)((char *)&prec->llsv - (char *)prec);
  pdbRecordType->papFldDes[54]->size=sizeof(prec->hsv);
  pdbRecordType->papFldDes[54]->offset=(short)((char *)&prec->hsv - (char *)prec);
  pdbRecordType->papFldDes[55]->size=sizeof(prec->lsv);
  pdbRecordType->papFldDes[55]->offset=(short)((char *)&prec->lsv - (char *)prec);
  pdbRecordType->papFldDes[56]->size=sizeof(prec->hyst);
  pdbRecordType->papFldDes[56]->offset=(short)((char *)&prec->hyst - (char *)prec);
  pdbRecordType->papFldDes[57]->size=sizeof(prec->adel);
  pdbRecordType->papFldDes[57]->offset=(short)((char *)&prec->adel - (char *)prec);
  pdbRecordType->papFldDes[58]->size=sizeof(prec->mdel);
  pdbRecordType->papFldDes[58]->offset=(short)((char *)&prec->mdel - (char *)prec);
  pdbRecordType->papFldDes[59]->size=sizeof(prec->lalm);
  pdbRecordType->papFldDes[59]->offset=(short)((char *)&prec->lalm - (char *)prec);
  pdbRecordType->papFldDes[60]->size=sizeof(prec->alst);
  pdbRecordType->papFldDes[60]->offset=(short)((char *)&prec->alst - (char *)prec);
  pdbRecordType->papFldDes[61]->size=sizeof(prec->mlst);
  pdbRecordType->papFldDes[61]->offset=(short)((char *)&prec->mlst - (char *)prec);
  pdbRecordType->papFldDes[62]->size=sizeof(prec->siol);
  pdbRecordType->papFldDes[62]->offset=(short)((char *)&prec->siol - (char *)prec);
  pdbRecordType->papFldDes[63]->size=sizeof(prec->sval);
  pdbRecordType->papFldDes[63]->offset=(short)((char *)&prec->sval - (char *)prec);
  pdbRecordType->papFldDes[64]->size=sizeof(prec->siml);
  pdbRecordType->papFldDes[64]->offset=(short)((char *)&prec->siml - (char *)prec);
  pdbRecordType->papFldDes[65]->size=sizeof(prec->simm);
  pdbRecordType->papFldDes[65]->offset=(short)((char *)&prec->simm - (char *)prec);
  pdbRecordType->papFldDes[66]->size=sizeof(prec->sims);
  pdbRecordType->papFldDes[66]->offset=(short)((char *)&prec->sims - (char *)prec);
  pdbRecordType->papFldDes[67]->size=sizeof(prec->inp);
  pdbRecordType->papFldDes[67]->offset=(short)((char *)&prec->inp - (char *)prec);
  pdbRecordType->papFldDes[68]->size=sizeof(prec->inpa);
  pdbRecordType->papFldDes[68]->offset=(short)((char *)&prec->inpa - (char *)prec);
  pdbRecordType->papFldDes[69]->size=sizeof(prec->inpb);
  pdbRecordType->papFldDes[69]->offset=(short)((char *)&prec->inpb - (char *)prec);
  pdbRecordType->papFldDes[70]->size=sizeof(prec->inpc);
  pdbRecordType->papFldDes[70]->offset=(short)((char *)&prec->inpc - (char *)prec);
  pdbRecordType->papFldDes[71]->size=sizeof(prec->inpd);
  pdbRecordType->papFldDes[71]->offset=(short)((char *)&prec->inpd - (char *)prec);
  pdbRecordType->papFldDes[72]->size=sizeof(prec->inpe);
  pdbRecordType->papFldDes[72]->offset=(short)((char *)&prec->inpe - (char *)prec);
  pdbRecordType->papFldDes[73]->size=sizeof(prec->inpf);
  pdbRecordType->papFldDes[73]->offset=(short)((char *)&prec->inpf - (char *)prec);
  pdbRecordType->papFldDes[74]->size=sizeof(prec->inpg);
  pdbRecordType->papFldDes[74]->offset=(short)((char *)&prec->inpg - (char *)prec);
  pdbRecordType->papFldDes[75]->size=sizeof(prec->inph);
  pdbRecordType->papFldDes[75]->offset=(short)((char *)&prec->inph - (char *)prec);
  pdbRecordType->papFldDes[76]->size=sizeof(prec->inpi);
  pdbRecordType->papFldDes[76]->offset=(short)((char *)&prec->inpi - (char *)prec);
  pdbRecordType->papFldDes[77]->size=sizeof(prec->inpj);
  pdbRecordType->papFldDes[77]->offset=(short)((char *)&prec->inpj - (char *)prec);
  pdbRecordType->papFldDes[78]->size=sizeof(prec->inpk);
  pdbRecordType->papFldDes[78]->offset=(short)((char *)&prec->inpk - (char *)prec);
  pdbRecordType->papFldDes[79]->size=sizeof(prec->inpl);
  pdbRecordType->papFldDes[79]->offset=(short)((char *)&prec->inpl - (char *)prec);
  pdbRecordType->papFldDes[80]->size=sizeof(prec->inpm);
  pdbRecordType->papFldDes[80]->offset=(short)((char *)&prec->inpm - (char *)prec);
  pdbRecordType->papFldDes[81]->size=sizeof(prec->inpn);
  pdbRecordType->papFldDes[81]->offset=(short)((char *)&prec->inpn - (char *)prec);
  pdbRecordType->papFldDes[82]->size=sizeof(prec->inpo);
  pdbRecordType->papFldDes[82]->offset=(short)((char *)&prec->inpo - (char *)prec);
  pdbRecordType->papFldDes[83]->size=sizeof(prec->inpp);
  pdbRecordType->papFldDes[83]->offset=(short)((char *)&prec->inpp - (char *)prec);
  pdbRecordType->papFldDes[84]->size=sizeof(prec->inpq);
  pdbRecordType->papFldDes[84]->offset=(short)((char *)&prec->inpq - (char *)prec);
  pdbRecordType->papFldDes[85]->size=sizeof(prec->inpr);
  pdbRecordType->papFldDes[85]->offset=(short)((char *)&prec->inpr - (char *)prec);
  pdbRecordType->papFldDes[86]->size=sizeof(prec->inps);
  pdbRecordType->papFldDes[86]->offset=(short)((char *)&prec->inps - (char *)prec);
  pdbRecordType->papFldDes[87]->size=sizeof(prec->inpt);
  pdbRecordType->papFldDes[87]->offset=(short)((char *)&prec->inpt - (char *)prec);
  pdbRecordType->papFldDes[88]->size=sizeof(prec->inpu);
  pdbRecordType->papFldDes[88]->offset=(short)((char *)&prec->inpu - (char *)prec);
  pdbRecordType->papFldDes[89]->size=sizeof(prec->inpv);
  pdbRecordType->papFldDes[89]->offset=(short)((char *)&prec->inpv - (char *)prec);
  pdbRecordType->papFldDes[90]->size=sizeof(prec->inpw);
  pdbRecordType->papFldDes[90]->offset=(short)((char *)&prec->inpw - (char *)prec);
  pdbRecordType->papFldDes[91]->size=sizeof(prec->inpx);
  pdbRecordType->papFldDes[91]->offset=(short)((char *)&prec->inpx - (char *)prec);
  pdbRecordType->papFldDes[92]->size=sizeof(prec->inpy);
  pdbRecordType->papFldDes[92]->offset=(short)((char *)&prec->inpy - (char *)prec);
  pdbRecordType->papFldDes[93]->size=sizeof(prec->inpz);
  pdbRecordType->papFldDes[93]->offset=(short)((char *)&prec->inpz - (char *)prec);
  pdbRecordType->papFldDes[94]->size=sizeof(prec->inpaa);
  pdbRecordType->papFldDes[94]->offset=(short)((char *)&prec->inpaa - (char *)prec);
  pdbRecordType->papFldDes[95]->size=sizeof(prec->inpab);
  pdbRecordType->papFldDes[95]->offset=(short)((char *)&prec->inpab - (char *)prec);
  pdbRecordType->papFldDes[96]->size=sizeof(prec->inpac);
  pdbRecordType->papFldDes[96]->offset=(short)((char *)&prec->inpac - (char *)prec);
  pdbRecordType->papFldDes[97]->size=sizeof(prec->inpad);
  pdbRecordType->papFldDes[97]->offset=(short)((char *)&prec->inpad - (char *)prec);
  pdbRecordType->papFldDes[98]->size=sizeof(prec->inpae);
  pdbRecordType->papFldDes[98]->offset=(short)((char *)&prec->inpae - (char *)prec);
  pdbRecordType->papFldDes[99]->size=sizeof(prec->inpaf);
  pdbRecordType->papFldDes[99]->offset=(short)((char *)&prec->inpaf - (char *)prec);
  pdbRecordType->papFldDes[100]->size=sizeof(prec->inpag);
  pdbRecordType->papFldDes[100]->offset=(short)((char *)&prec->inpag - (char *)prec);
  pdbRecordType->papFldDes[101]->size=sizeof(prec->inpah);
  pdbRecordType->papFldDes[101]->offset=(short)((char *)&prec->inpah - (char *)prec);
  pdbRecordType->papFldDes[102]->size=sizeof(prec->inpai);
  pdbRecordType->papFldDes[102]->offset=(short)((char *)&prec->inpai - (char *)prec);
  pdbRecordType->papFldDes[103]->size=sizeof(prec->inpaj);
  pdbRecordType->papFldDes[103]->offset=(short)((char *)&prec->inpaj - (char *)prec);
  pdbRecordType->papFldDes[104]->size=sizeof(prec->inpak);
  pdbRecordType->papFldDes[104]->offset=(short)((char *)&prec->inpak - (char *)prec);
  pdbRecordType->papFldDes[105]->size=sizeof(prec->inpal);
  pdbRecordType->papFldDes[105]->offset=(short)((char *)&prec->inpal - (char *)prec);
  pdbRecordType->papFldDes[106]->size=sizeof(prec->inpam);
  pdbRecordType->papFldDes[106]->offset=(short)((char *)&prec->inpam - (char *)prec);
  pdbRecordType->papFldDes[107]->size=sizeof(prec->inpan);
  pdbRecordType->papFldDes[107]->offset=(short)((char *)&prec->inpan - (char *)prec);
    pdbRecordType->rec_size = sizeof(*prec);
    return(0);
}
epicsExportRegistrar(icmpRecordSizeOffset);
#ifdef __cplusplus
}
#endif
#endif /*GEN_SIZE_OFFSET*/
