#include <dbCommon.h>
#include <devSup.h>
#include <epicsExport.h>

#include "s7nodave.h"
#include "s7nodaveAsyn.h"

/*
 * This file contains all the device support structure definitions needed to be
 * registered. It acts as a wrapper around the rest of the code, which is C++.
 */

/*
 * aai record type
 */
static long initAaiRecord (dbCommon *record) {
    return s7nodaveInitRecord(record, aaiRecordType);
}

struct {
    long numberOfFunctionPointers;
    DEVSUPFUN report;
    DEVSUPFUN init;
    DEVSUPFUN init_record;
    DEVSUPFUN get_ioint_info;
    DEVSUPFUN read;
} devAaiS7nodave = {
    5,
    NULL,
    NULL,
    initAaiRecord,
    s7nodaveGetIoIntInfoRecord,
    s7nodaveProcessRecord
};

epicsExportAddress(dset, devAaiS7nodave);

/*
 * aao record type
 */
static long initAaoRecord (dbCommon *record) {
    return s7nodaveInitRecord(record, aaoRecordType);
}

struct {
    long numberOfFunctionPointers;
    DEVSUPFUN report;
    DEVSUPFUN init;
    DEVSUPFUN init_record;
    DEVSUPFUN get_ioint_info;
    DEVSUPFUN read;
} devAaoS7nodave = {
    5,
    NULL,
    NULL,
    initAaoRecord,
    NULL,
    s7nodaveProcessRecord,
};

epicsExportAddress(dset, devAaoS7nodave);

/*
 * ai record type
 */
static long initAiRecord (dbCommon *record) {
    return s7nodaveInitRecord(record, aiRecordType);
}

struct {
    long numberOfFunctionPointers;
    DEVSUPFUN report;
    DEVSUPFUN init;
    DEVSUPFUN init_record;
    DEVSUPFUN get_ioint_info;
    DEVSUPFUN read;
    DEVSUPFUN convert;
} devAiS7nodave = {
    6,
    NULL,
    NULL,
    initAiRecord,
    s7nodaveGetIoIntInfoRecord,
    s7nodaveProcessRecord,
    s7nodaveConvertRecord
};

epicsExportAddress(dset, devAiS7nodave);

/*
 * ao record type
 */
static long initAoRecord (dbCommon *record) {
    return s7nodaveInitRecord(record, aoRecordType);
}

struct {
    long numberOfFunctionPointers;
    DEVSUPFUN report;
    DEVSUPFUN init;
    DEVSUPFUN init_record;
    DEVSUPFUN get_ioint_info;
    DEVSUPFUN read;
    DEVSUPFUN convert;
} devAoS7nodave = {
    6,
    NULL,
    NULL,
    initAoRecord,
    NULL,
    s7nodaveProcessRecord,
    s7nodaveConvertRecord
};

epicsExportAddress(dset, devAoS7nodave);

/*
 * bi record type
 */
static long initBiRecord (dbCommon *record) {
    return s7nodaveInitRecord(record, biRecordType);
}

struct {
    long numberOfFunctionPointers;
    DEVSUPFUN report;
    DEVSUPFUN init;
    DEVSUPFUN init_record;
    DEVSUPFUN get_ioint_info;
    DEVSUPFUN read;
} devBiS7nodave = {
    5,
    NULL,
    NULL,
    initBiRecord,
    s7nodaveGetIoIntInfoRecord,
    s7nodaveProcessRecord
};

epicsExportAddress(dset, devBiS7nodave);

/*
 * bo record type
 */
static long initBoRecord (dbCommon *record) {
    return s7nodaveInitRecord(record, boRecordType);
}

struct {
    long numberOfFunctionPointers;
    DEVSUPFUN report;
    DEVSUPFUN init;
    DEVSUPFUN init_record;
    DEVSUPFUN get_ioint_info;
    DEVSUPFUN read;
} devBoS7nodave = {
    5,
    NULL,
    NULL,
    initBoRecord,
    NULL,
    s7nodaveProcessRecord,
};

epicsExportAddress(dset, devBoS7nodave);

/*
 * longin record type
 */
static long initLiRecord (dbCommon *record) {
    return s7nodaveInitRecord(record, longinRecordType);
}

struct {
    long numberOfFunctionPointers;
    DEVSUPFUN report;
    DEVSUPFUN init;
    DEVSUPFUN init_record;
    DEVSUPFUN get_ioint_info;
    DEVSUPFUN read;
} devLonginS7nodave = {
    5,
    NULL,
    NULL,
    initLiRecord,
    s7nodaveGetIoIntInfoRecord,
    s7nodaveProcessRecord
};

epicsExportAddress(dset, devLonginS7nodave);

/*
 * longout record type
 */
static long initLoRecord (dbCommon *record) {
    return s7nodaveInitRecord(record, longoutRecordType);
}

struct {
    long numberOfFunctionPointers;
    DEVSUPFUN report;
    DEVSUPFUN init;
    DEVSUPFUN init_record;
    DEVSUPFUN get_ioint_info;
    DEVSUPFUN read;
} devLongoutS7nodave = {
    5,
    NULL,
    NULL,
    initLoRecord,
    NULL,
    s7nodaveProcessRecord
};

epicsExportAddress(dset, devLongoutS7nodave);

/*
 * mbbi record type
 */
static long initMbbiRecord (dbCommon *record) {
    return s7nodaveInitRecord(record, mbbiRecordType);
}

struct {
    long numberOfFunctionPointers;
    DEVSUPFUN report;
    DEVSUPFUN init;
    DEVSUPFUN init_record;
    DEVSUPFUN get_ioint_info;
    DEVSUPFUN read;
} devMbbiS7nodave = {
    5,
    NULL,
    NULL,
    initMbbiRecord,
    s7nodaveGetIoIntInfoRecord,
    s7nodaveProcessRecord
};

epicsExportAddress(dset, devMbbiS7nodave);

/*
 * mbbo record type
 */
static long initMbboRecord (dbCommon *record) {
    return s7nodaveInitRecord(record, mbboRecordType);
}

struct {
    long numberOfFunctionPointers;
    DEVSUPFUN report;
    DEVSUPFUN init;
    DEVSUPFUN init_record;
    DEVSUPFUN get_ioint_info;
    DEVSUPFUN read;
} devMbboS7nodave = {
    5,
    NULL,
    NULL,
    initMbboRecord,
    NULL,
    s7nodaveProcessRecord,
};

epicsExportAddress(dset, devMbboS7nodave);

/*
 * mbbiDirect record type
 */
static long initMbbiDirectRecord (dbCommon *record) {
    return s7nodaveInitRecord(record, mbbiDirectRecordType);
}

struct {
    long numberOfFunctionPointers;
    DEVSUPFUN report;
    DEVSUPFUN init;
    DEVSUPFUN init_record;
    DEVSUPFUN get_ioint_info;
    DEVSUPFUN read;
} devMbbiDirectS7nodave = {
    5,
    NULL,
    NULL,
    initMbbiDirectRecord,
    s7nodaveGetIoIntInfoRecord,
    s7nodaveProcessRecord
};

epicsExportAddress(dset, devMbbiDirectS7nodave);

/*
 * mbbo record type
 */
static long initMbboDirectRecord (dbCommon *record) {
    return s7nodaveInitRecord(record, mbboDirectRecordType);
}

struct {
    long numberOfFunctionPointers;
    DEVSUPFUN report;
    DEVSUPFUN init;
    DEVSUPFUN init_record;
    DEVSUPFUN get_ioint_info;
    DEVSUPFUN read;
} devMbboDirectS7nodave = {
    5,
    NULL,
    NULL,
    initMbboDirectRecord,
    NULL,
    s7nodaveProcessRecord,
};

epicsExportAddress(dset, devMbboDirectS7nodave);

/*
 * stringin record type
 */
static long initSiRecord (dbCommon *record) {
    return s7nodaveInitRecord(record, stringinRecordType);
}

struct {
    long numberOfFunctionPointers;
    DEVSUPFUN report;
    DEVSUPFUN init;
    DEVSUPFUN init_record;
    DEVSUPFUN get_ioint_info;
    DEVSUPFUN read;
} devStringinS7nodave = {
    5,
    NULL,
    NULL,
    initSiRecord,
    s7nodaveGetIoIntInfoRecord,
    s7nodaveProcessRecord
};

epicsExportAddress(dset, devStringinS7nodave);

/*
 * stringout record type
 */
static long initSoRecord (dbCommon *record) {
    return s7nodaveInitRecord(record, stringoutRecordType);
}

struct {
    long numberOfFunctionPointers;
    DEVSUPFUN report;
    DEVSUPFUN init;
    DEVSUPFUN init_record;
    DEVSUPFUN get_ioint_info;
    DEVSUPFUN read;
} devStringoutS7nodave = {
    5,
    NULL,
    NULL,
    initSoRecord,
    NULL,
    s7nodaveProcessRecord
};

epicsExportAddress(dset, devStringoutS7nodave);

/*
 * waveform record type - input
 */
static long initWaveformInRecord (dbCommon *record) {
    return s7nodaveInitRecord(record, waveformInRecordType);
}

struct {
    long numberOfFunctionPointers;
    DEVSUPFUN report;
    DEVSUPFUN init;
    DEVSUPFUN init_record;
    DEVSUPFUN get_ioint_info;
    DEVSUPFUN read;
} devWaveformInS7nodave = {
    5,
    NULL,
    NULL,
    initWaveformInRecord,
    s7nodaveGetIoIntInfoRecord,
    s7nodaveProcessRecord
};

epicsExportAddress(dset, devWaveformInS7nodave);

/*
 * waveform record type - output
 */
static long initWaveformOutRecord (dbCommon *record) {
    return s7nodaveInitRecord(record, waveformOutRecordType);
}

struct {
    long numberOfFunctionPointers;
    DEVSUPFUN report;
    DEVSUPFUN init;
    DEVSUPFUN init_record;
    DEVSUPFUN get_ioint_info;
    DEVSUPFUN read;
} devWaveformOutS7nodave = {
    5,
    NULL,
    NULL,
    initWaveformOutRecord,
    NULL,
    s7nodaveProcessRecord,
};

epicsExportAddress(dset, devWaveformOutS7nodave);
