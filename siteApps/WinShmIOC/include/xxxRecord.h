/** @file xxxRecord.h
 * @brief Declarations for the @ref xxxRecord "xxx" record type.
 *
 * This header was generated from xxxRecord.dbd
 */

#ifndef INC_xxxRecord_H
#define INC_xxxRecord_H

#include "epicsTypes.h"
#include "link.h"
#include "epicsMutex.h"
#include "ellLib.h"
#include "devSup.h"
#include "epicsTime.h"

/** @brief Declaration of xxx record type. */
typedef struct xxxRecord {
    char                name[61];   /**< @brief Record Name */
    char                desc[41];   /**< @brief Descriptor */
    char                asg[29];    /**< @brief Access Security Group */
    epicsEnum16         scan;       /**< @brief Scan Mechanism */
    epicsEnum16         pini;       /**< @brief Process at iocInit */
    epicsInt16          phas;       /**< @brief Scan Phase */
    char                evnt[40];   /**< @brief Event Name */
    epicsInt16          tse;        /**< @brief Time Stamp Event */
    DBLINK              tsel;       /**< @brief Time Stamp Link */
    epicsEnum16         dtyp;       /**< @brief Device Type */
    epicsInt16          disv;       /**< @brief Disable Value */
    epicsInt16          disa;       /**< @brief Disable */
    DBLINK              sdis;       /**< @brief Scanning Disable */
    epicsMutexId        mlok;       /**< @brief Monitor lock */
    ELLLIST             mlis;       /**< @brief Monitor List */
    ELLLIST             bklnk;      /**< @brief Backwards link tracking */
    epicsUInt8          disp;       /**< @brief Disable putField */
    epicsUInt8          proc;       /**< @brief Force Processing */
    epicsEnum16         stat;       /**< @brief Alarm Status */
    epicsEnum16         sevr;       /**< @brief Alarm Severity */
    char                amsg[40];   /**< @brief Alarm Message */
    epicsEnum16         nsta;       /**< @brief New Alarm Status */
    epicsEnum16         nsev;       /**< @brief New Alarm Severity */
    char                namsg[40];  /**< @brief New Alarm Message */
    epicsEnum16         acks;       /**< @brief Alarm Ack Severity */
    epicsEnum16         ackt;       /**< @brief Alarm Ack Transient */
    epicsEnum16         diss;       /**< @brief Disable Alarm Sevrty */
    epicsUInt8          lcnt;       /**< @brief Lock Count */
    epicsUInt8          pact;       /**< @brief Record active */
    epicsUInt8          putf;       /**< @brief dbPutField process */
    epicsUInt8          rpro;       /**< @brief Reprocess  */
    struct asgMember    *asp;       /**< @brief Access Security Pvt */
    struct processNotify *ppn;      /**< @brief pprocessNotify */
    struct processNotifyRecord *ppnr; /**< @brief pprocessNotifyRecord */
    struct scan_element *spvt;      /**< @brief Scan Private */
    struct typed_rset   *rset;      /**< @brief Address of RSET */
    unambiguous_dset    *dset;      /**< @brief DSET address */
    void                *dpvt;      /**< @brief Device Private */
    struct dbRecordType *rdes;      /**< @brief Address of dbRecordType */
    struct lockRecord   *lset;      /**< @brief Lock Set */
    epicsEnum16         prio;       /**< @brief Scheduling Priority */
    epicsUInt8          tpro;       /**< @brief Trace Processing */
    epicsUInt8          bkpt;       /**< @brief Break Point */
    epicsUInt8          udf;        /**< @brief Undefined */
    epicsEnum16         udfs;       /**< @brief Undefined Alarm Sevrty */
    epicsTimeStamp      time;       /**< @brief Time */
    epicsUInt64         utag;       /**< @brief Time Tag */
    DBLINK              flnk;       /**< @brief Forward Process Link */
    epicsFloat64        val;        /**< @brief Current EGU Value */
    DBLINK              inp;        /**< @brief Input Specification */
    epicsInt16          prec;       /**< @brief Display Precision */
    char                egu[16];    /**< @brief Engineering Units */
    epicsFloat32        hopr;       /**< @brief High Operating Range */
    epicsFloat32        lopr;       /**< @brief Low Operating Range */
    epicsFloat32        hihi;       /**< @brief Hihi Alarm Limit */
    epicsFloat32        lolo;       /**< @brief Lolo Alarm Limit */
    epicsFloat32        high;       /**< @brief High Alarm Limit */
    epicsFloat32        low;        /**< @brief Low Alarm Limit */
    epicsEnum16         hhsv;       /**< @brief Hihi Severity */
    epicsEnum16         llsv;       /**< @brief Lolo Severity */
    epicsEnum16         hsv;        /**< @brief High Severity */
    epicsEnum16         lsv;        /**< @brief Low Severity */
    epicsFloat64        hyst;       /**< @brief Alarm Deadband */
    epicsFloat64        adel;       /**< @brief Archive Deadband */
    epicsFloat64        mdel;       /**< @brief Monitor Deadband */
    epicsFloat64        lalm;       /**< @brief Last Value Alarmed */
    epicsFloat64        alst;       /**< @brief Last Value Archived */
    epicsFloat64        mlst;       /**< @brief Last Val Monitored */
} xxxRecord;

typedef enum {
	xxxRecordNAME = 0,
	xxxRecordDESC = 1,
	xxxRecordASG = 2,
	xxxRecordSCAN = 3,
	xxxRecordPINI = 4,
	xxxRecordPHAS = 5,
	xxxRecordEVNT = 6,
	xxxRecordTSE = 7,
	xxxRecordTSEL = 8,
	xxxRecordDTYP = 9,
	xxxRecordDISV = 10,
	xxxRecordDISA = 11,
	xxxRecordSDIS = 12,
	xxxRecordMLOK = 13,
	xxxRecordMLIS = 14,
	xxxRecordBKLNK = 15,
	xxxRecordDISP = 16,
	xxxRecordPROC = 17,
	xxxRecordSTAT = 18,
	xxxRecordSEVR = 19,
	xxxRecordAMSG = 20,
	xxxRecordNSTA = 21,
	xxxRecordNSEV = 22,
	xxxRecordNAMSG = 23,
	xxxRecordACKS = 24,
	xxxRecordACKT = 25,
	xxxRecordDISS = 26,
	xxxRecordLCNT = 27,
	xxxRecordPACT = 28,
	xxxRecordPUTF = 29,
	xxxRecordRPRO = 30,
	xxxRecordASP = 31,
	xxxRecordPPN = 32,
	xxxRecordPPNR = 33,
	xxxRecordSPVT = 34,
	xxxRecordRSET = 35,
	xxxRecordDSET = 36,
	xxxRecordDPVT = 37,
	xxxRecordRDES = 38,
	xxxRecordLSET = 39,
	xxxRecordPRIO = 40,
	xxxRecordTPRO = 41,
	xxxRecordBKPT = 42,
	xxxRecordUDF = 43,
	xxxRecordUDFS = 44,
	xxxRecordTIME = 45,
	xxxRecordUTAG = 46,
	xxxRecordFLNK = 47,
	xxxRecordVAL = 48,
	xxxRecordINP = 49,
	xxxRecordPREC = 50,
	xxxRecordEGU = 51,
	xxxRecordHOPR = 52,
	xxxRecordLOPR = 53,
	xxxRecordHIHI = 54,
	xxxRecordLOLO = 55,
	xxxRecordHIGH = 56,
	xxxRecordLOW = 57,
	xxxRecordHHSV = 58,
	xxxRecordLLSV = 59,
	xxxRecordHSV = 60,
	xxxRecordLSV = 61,
	xxxRecordHYST = 62,
	xxxRecordADEL = 63,
	xxxRecordMDEL = 64,
	xxxRecordLALM = 65,
	xxxRecordALST = 66,
	xxxRecordMLST = 67
} xxxFieldIndex;

#ifdef GEN_SIZE_OFFSET

#include <epicsExport.h>
#include <cantProceed.h>
#ifdef __cplusplus
extern "C" {
#endif
static int xxxRecordSizeOffset(dbRecordType *prt)
{
    xxxRecord *prec = 0;

    if (prt->no_fields != 68) {
        cantProceed("IOC build or installation error:\n"
            "    The xxxRecord defined in the DBD file has %d fields,\n"
            "    but the record support code was built with 68.\n",
            prt->no_fields);
    }
    prt->papFldDes[xxxRecordNAME]->size = sizeof(prec->name);
    prt->papFldDes[xxxRecordNAME]->offset = (unsigned short)((char *)&prec->name - (char *)prec);
    prt->papFldDes[xxxRecordDESC]->size = sizeof(prec->desc);
    prt->papFldDes[xxxRecordDESC]->offset = (unsigned short)((char *)&prec->desc - (char *)prec);
    prt->papFldDes[xxxRecordASG]->size = sizeof(prec->asg);
    prt->papFldDes[xxxRecordASG]->offset = (unsigned short)((char *)&prec->asg - (char *)prec);
    prt->papFldDes[xxxRecordSCAN]->size = sizeof(prec->scan);
    prt->papFldDes[xxxRecordSCAN]->offset = (unsigned short)((char *)&prec->scan - (char *)prec);
    prt->papFldDes[xxxRecordPINI]->size = sizeof(prec->pini);
    prt->papFldDes[xxxRecordPINI]->offset = (unsigned short)((char *)&prec->pini - (char *)prec);
    prt->papFldDes[xxxRecordPHAS]->size = sizeof(prec->phas);
    prt->papFldDes[xxxRecordPHAS]->offset = (unsigned short)((char *)&prec->phas - (char *)prec);
    prt->papFldDes[xxxRecordEVNT]->size = sizeof(prec->evnt);
    prt->papFldDes[xxxRecordEVNT]->offset = (unsigned short)((char *)&prec->evnt - (char *)prec);
    prt->papFldDes[xxxRecordTSE]->size = sizeof(prec->tse);
    prt->papFldDes[xxxRecordTSE]->offset = (unsigned short)((char *)&prec->tse - (char *)prec);
    prt->papFldDes[xxxRecordTSEL]->size = sizeof(prec->tsel);
    prt->papFldDes[xxxRecordTSEL]->offset = (unsigned short)((char *)&prec->tsel - (char *)prec);
    prt->papFldDes[xxxRecordDTYP]->size = sizeof(prec->dtyp);
    prt->papFldDes[xxxRecordDTYP]->offset = (unsigned short)((char *)&prec->dtyp - (char *)prec);
    prt->papFldDes[xxxRecordDISV]->size = sizeof(prec->disv);
    prt->papFldDes[xxxRecordDISV]->offset = (unsigned short)((char *)&prec->disv - (char *)prec);
    prt->papFldDes[xxxRecordDISA]->size = sizeof(prec->disa);
    prt->papFldDes[xxxRecordDISA]->offset = (unsigned short)((char *)&prec->disa - (char *)prec);
    prt->papFldDes[xxxRecordSDIS]->size = sizeof(prec->sdis);
    prt->papFldDes[xxxRecordSDIS]->offset = (unsigned short)((char *)&prec->sdis - (char *)prec);
    prt->papFldDes[xxxRecordMLOK]->size = sizeof(prec->mlok);
    prt->papFldDes[xxxRecordMLOK]->offset = (unsigned short)((char *)&prec->mlok - (char *)prec);
    prt->papFldDes[xxxRecordMLIS]->size = sizeof(prec->mlis);
    prt->papFldDes[xxxRecordMLIS]->offset = (unsigned short)((char *)&prec->mlis - (char *)prec);
    prt->papFldDes[xxxRecordBKLNK]->size = sizeof(prec->bklnk);
    prt->papFldDes[xxxRecordBKLNK]->offset = (unsigned short)((char *)&prec->bklnk - (char *)prec);
    prt->papFldDes[xxxRecordDISP]->size = sizeof(prec->disp);
    prt->papFldDes[xxxRecordDISP]->offset = (unsigned short)((char *)&prec->disp - (char *)prec);
    prt->papFldDes[xxxRecordPROC]->size = sizeof(prec->proc);
    prt->papFldDes[xxxRecordPROC]->offset = (unsigned short)((char *)&prec->proc - (char *)prec);
    prt->papFldDes[xxxRecordSTAT]->size = sizeof(prec->stat);
    prt->papFldDes[xxxRecordSTAT]->offset = (unsigned short)((char *)&prec->stat - (char *)prec);
    prt->papFldDes[xxxRecordSEVR]->size = sizeof(prec->sevr);
    prt->papFldDes[xxxRecordSEVR]->offset = (unsigned short)((char *)&prec->sevr - (char *)prec);
    prt->papFldDes[xxxRecordAMSG]->size = sizeof(prec->amsg);
    prt->papFldDes[xxxRecordAMSG]->offset = (unsigned short)((char *)&prec->amsg - (char *)prec);
    prt->papFldDes[xxxRecordNSTA]->size = sizeof(prec->nsta);
    prt->papFldDes[xxxRecordNSTA]->offset = (unsigned short)((char *)&prec->nsta - (char *)prec);
    prt->papFldDes[xxxRecordNSEV]->size = sizeof(prec->nsev);
    prt->papFldDes[xxxRecordNSEV]->offset = (unsigned short)((char *)&prec->nsev - (char *)prec);
    prt->papFldDes[xxxRecordNAMSG]->size = sizeof(prec->namsg);
    prt->papFldDes[xxxRecordNAMSG]->offset = (unsigned short)((char *)&prec->namsg - (char *)prec);
    prt->papFldDes[xxxRecordACKS]->size = sizeof(prec->acks);
    prt->papFldDes[xxxRecordACKS]->offset = (unsigned short)((char *)&prec->acks - (char *)prec);
    prt->papFldDes[xxxRecordACKT]->size = sizeof(prec->ackt);
    prt->papFldDes[xxxRecordACKT]->offset = (unsigned short)((char *)&prec->ackt - (char *)prec);
    prt->papFldDes[xxxRecordDISS]->size = sizeof(prec->diss);
    prt->papFldDes[xxxRecordDISS]->offset = (unsigned short)((char *)&prec->diss - (char *)prec);
    prt->papFldDes[xxxRecordLCNT]->size = sizeof(prec->lcnt);
    prt->papFldDes[xxxRecordLCNT]->offset = (unsigned short)((char *)&prec->lcnt - (char *)prec);
    prt->papFldDes[xxxRecordPACT]->size = sizeof(prec->pact);
    prt->papFldDes[xxxRecordPACT]->offset = (unsigned short)((char *)&prec->pact - (char *)prec);
    prt->papFldDes[xxxRecordPUTF]->size = sizeof(prec->putf);
    prt->papFldDes[xxxRecordPUTF]->offset = (unsigned short)((char *)&prec->putf - (char *)prec);
    prt->papFldDes[xxxRecordRPRO]->size = sizeof(prec->rpro);
    prt->papFldDes[xxxRecordRPRO]->offset = (unsigned short)((char *)&prec->rpro - (char *)prec);
    prt->papFldDes[xxxRecordASP]->size = sizeof(prec->asp);
    prt->papFldDes[xxxRecordASP]->offset = (unsigned short)((char *)&prec->asp - (char *)prec);
    prt->papFldDes[xxxRecordPPN]->size = sizeof(prec->ppn);
    prt->papFldDes[xxxRecordPPN]->offset = (unsigned short)((char *)&prec->ppn - (char *)prec);
    prt->papFldDes[xxxRecordPPNR]->size = sizeof(prec->ppnr);
    prt->papFldDes[xxxRecordPPNR]->offset = (unsigned short)((char *)&prec->ppnr - (char *)prec);
    prt->papFldDes[xxxRecordSPVT]->size = sizeof(prec->spvt);
    prt->papFldDes[xxxRecordSPVT]->offset = (unsigned short)((char *)&prec->spvt - (char *)prec);
    prt->papFldDes[xxxRecordRSET]->size = sizeof(prec->rset);
    prt->papFldDes[xxxRecordRSET]->offset = (unsigned short)((char *)&prec->rset - (char *)prec);
    prt->papFldDes[xxxRecordDSET]->size = sizeof(prec->dset);
    prt->papFldDes[xxxRecordDSET]->offset = (unsigned short)((char *)&prec->dset - (char *)prec);
    prt->papFldDes[xxxRecordDPVT]->size = sizeof(prec->dpvt);
    prt->papFldDes[xxxRecordDPVT]->offset = (unsigned short)((char *)&prec->dpvt - (char *)prec);
    prt->papFldDes[xxxRecordRDES]->size = sizeof(prec->rdes);
    prt->papFldDes[xxxRecordRDES]->offset = (unsigned short)((char *)&prec->rdes - (char *)prec);
    prt->papFldDes[xxxRecordLSET]->size = sizeof(prec->lset);
    prt->papFldDes[xxxRecordLSET]->offset = (unsigned short)((char *)&prec->lset - (char *)prec);
    prt->papFldDes[xxxRecordPRIO]->size = sizeof(prec->prio);
    prt->papFldDes[xxxRecordPRIO]->offset = (unsigned short)((char *)&prec->prio - (char *)prec);
    prt->papFldDes[xxxRecordTPRO]->size = sizeof(prec->tpro);
    prt->papFldDes[xxxRecordTPRO]->offset = (unsigned short)((char *)&prec->tpro - (char *)prec);
    prt->papFldDes[xxxRecordBKPT]->size = sizeof(prec->bkpt);
    prt->papFldDes[xxxRecordBKPT]->offset = (unsigned short)((char *)&prec->bkpt - (char *)prec);
    prt->papFldDes[xxxRecordUDF]->size = sizeof(prec->udf);
    prt->papFldDes[xxxRecordUDF]->offset = (unsigned short)((char *)&prec->udf - (char *)prec);
    prt->papFldDes[xxxRecordUDFS]->size = sizeof(prec->udfs);
    prt->papFldDes[xxxRecordUDFS]->offset = (unsigned short)((char *)&prec->udfs - (char *)prec);
    prt->papFldDes[xxxRecordTIME]->size = sizeof(prec->time);
    prt->papFldDes[xxxRecordTIME]->offset = (unsigned short)((char *)&prec->time - (char *)prec);
    prt->papFldDes[xxxRecordUTAG]->size = sizeof(prec->utag);
    prt->papFldDes[xxxRecordUTAG]->offset = (unsigned short)((char *)&prec->utag - (char *)prec);
    prt->papFldDes[xxxRecordFLNK]->size = sizeof(prec->flnk);
    prt->papFldDes[xxxRecordFLNK]->offset = (unsigned short)((char *)&prec->flnk - (char *)prec);
    prt->papFldDes[xxxRecordVAL]->size = sizeof(prec->val);
    prt->papFldDes[xxxRecordVAL]->offset = (unsigned short)((char *)&prec->val - (char *)prec);
    prt->papFldDes[xxxRecordINP]->size = sizeof(prec->inp);
    prt->papFldDes[xxxRecordINP]->offset = (unsigned short)((char *)&prec->inp - (char *)prec);
    prt->papFldDes[xxxRecordPREC]->size = sizeof(prec->prec);
    prt->papFldDes[xxxRecordPREC]->offset = (unsigned short)((char *)&prec->prec - (char *)prec);
    prt->papFldDes[xxxRecordEGU]->size = sizeof(prec->egu);
    prt->papFldDes[xxxRecordEGU]->offset = (unsigned short)((char *)&prec->egu - (char *)prec);
    prt->papFldDes[xxxRecordHOPR]->size = sizeof(prec->hopr);
    prt->papFldDes[xxxRecordHOPR]->offset = (unsigned short)((char *)&prec->hopr - (char *)prec);
    prt->papFldDes[xxxRecordLOPR]->size = sizeof(prec->lopr);
    prt->papFldDes[xxxRecordLOPR]->offset = (unsigned short)((char *)&prec->lopr - (char *)prec);
    prt->papFldDes[xxxRecordHIHI]->size = sizeof(prec->hihi);
    prt->papFldDes[xxxRecordHIHI]->offset = (unsigned short)((char *)&prec->hihi - (char *)prec);
    prt->papFldDes[xxxRecordLOLO]->size = sizeof(prec->lolo);
    prt->papFldDes[xxxRecordLOLO]->offset = (unsigned short)((char *)&prec->lolo - (char *)prec);
    prt->papFldDes[xxxRecordHIGH]->size = sizeof(prec->high);
    prt->papFldDes[xxxRecordHIGH]->offset = (unsigned short)((char *)&prec->high - (char *)prec);
    prt->papFldDes[xxxRecordLOW]->size = sizeof(prec->low);
    prt->papFldDes[xxxRecordLOW]->offset = (unsigned short)((char *)&prec->low - (char *)prec);
    prt->papFldDes[xxxRecordHHSV]->size = sizeof(prec->hhsv);
    prt->papFldDes[xxxRecordHHSV]->offset = (unsigned short)((char *)&prec->hhsv - (char *)prec);
    prt->papFldDes[xxxRecordLLSV]->size = sizeof(prec->llsv);
    prt->papFldDes[xxxRecordLLSV]->offset = (unsigned short)((char *)&prec->llsv - (char *)prec);
    prt->papFldDes[xxxRecordHSV]->size = sizeof(prec->hsv);
    prt->papFldDes[xxxRecordHSV]->offset = (unsigned short)((char *)&prec->hsv - (char *)prec);
    prt->papFldDes[xxxRecordLSV]->size = sizeof(prec->lsv);
    prt->papFldDes[xxxRecordLSV]->offset = (unsigned short)((char *)&prec->lsv - (char *)prec);
    prt->papFldDes[xxxRecordHYST]->size = sizeof(prec->hyst);
    prt->papFldDes[xxxRecordHYST]->offset = (unsigned short)((char *)&prec->hyst - (char *)prec);
    prt->papFldDes[xxxRecordADEL]->size = sizeof(prec->adel);
    prt->papFldDes[xxxRecordADEL]->offset = (unsigned short)((char *)&prec->adel - (char *)prec);
    prt->papFldDes[xxxRecordMDEL]->size = sizeof(prec->mdel);
    prt->papFldDes[xxxRecordMDEL]->offset = (unsigned short)((char *)&prec->mdel - (char *)prec);
    prt->papFldDes[xxxRecordLALM]->size = sizeof(prec->lalm);
    prt->papFldDes[xxxRecordLALM]->offset = (unsigned short)((char *)&prec->lalm - (char *)prec);
    prt->papFldDes[xxxRecordALST]->size = sizeof(prec->alst);
    prt->papFldDes[xxxRecordALST]->offset = (unsigned short)((char *)&prec->alst - (char *)prec);
    prt->papFldDes[xxxRecordMLST]->size = sizeof(prec->mlst);
    prt->papFldDes[xxxRecordMLST]->offset = (unsigned short)((char *)&prec->mlst - (char *)prec);
    prt->rec_size = sizeof(*prec);
    return 0;
}
epicsExportRegistrar(xxxRecordSizeOffset);

#ifdef __cplusplus
}
#endif
#endif /* GEN_SIZE_OFFSET */

#endif /* INC_xxxRecord_H */
