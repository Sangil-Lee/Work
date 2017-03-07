/*
 *
 * (c) 2013 Dragutin Maier-Manojlovic     dragutin.maier-manojlovic@psi.ch
 * Date: 01.10.2013
 *
 * Paul Scherrer Institute (PSI)
 * Switzerland
 *
 * This file and the package it belongs to have to be redistributed as is,
 * with no changes.
 * Any changes to this file mean that further distribution is not allowed.
 * Bugs/errors/patches/feature requests can be sent directly to the author,
 * and if they are accepted, will be included in the next version or release.
 *
 * Disclaimer:
 * The software (code, tools) is provided "as is", without warranty of any kind.
 * Author makes no warranties, express or implied, that the code is free of
 * errors, or is consistent with any particular standard, or that it
 * will meet your requirements for any particular application.
 * It should not be relied on for solving a problem whose correct or incorrect
 * solution could result in injury to a person or a loss of property.
 * The author disclaim all liability for direct, indirect or
 * consequential damage resulting from your use of the code or tools.
 * If you do use it, it is at your own risk.
 *
 */



#ifndef ECCOMMON_H
#define ECCOMMON_H

#define FREQ_1Hz	1000000
#define FREQ_2Hz	500000
#define FREQ_3Hz	333333
#define FREQ_4Hz	250000
#define FREQ_5Hz	200000

#define FREQ_10Hz	100000
#define FREQ_100Hz	10000
#define FREQ_1000Hz	1000

#define FREQ_2000Hz	500
#define FREQ_3000Hz	333
#define FREQ_4000Hz	250
#define FREQ_5000Hz	200

#define FREQ_10000Hz 100

#define FREQ_1KHz	FREQ_1000Hz
#define FREQ_2KHz	FREQ_2000Hz
#define FREQ_3KHz	FREQ_3000Hz
#define FREQ_4KHz	FREQ_4000Hz
#define FREQ_5KHz	FREQ_5000Hz
#define FREQ_10KHz	FREQ_10000Hz



#define FREQ_MIN		10
#define FREQ_MAX		2000000001
/*#define FREQ_DEFAULT	FREQ_1Hz */
/*#define FREQ_DEFAULT	FREQ_100Hz */
#define FREQ_DEFAULT	FREQ_1000Hz
/*#define FREQ_DEFAULT	FREQ_2000Hz */

#define DEVICE_NAME "psi_ethercat"
#define BUF_LEN 80
#define LBUF_LEN 256
#define MAX_SBUF	1024
#define SBUF_SAFETY	(MAX_SBUF-64)

#define M_LOCK              down( &ecatm->master->master_sem )
#define M_UNLOCK            up( &ecatm->master->master_sem )

#define PSI_ECAT_VERSION_MAGIC        0xdd

#define IOCTL_MSG_R  _IOWR(PSI_ECAT_VERSION_MAGIC, 1, int)
#define IOCTL_MSG_W  _IOWR(PSI_ECAT_VERSION_MAGIC, 2, int)
#define IOCTL_MSG_RW  _IOWR(PSI_ECAT_VERSION_MAGIC, 10, int)
#define IOCTL_MSG_DREG  _IOWR(PSI_ECAT_VERSION_MAGIC, 20, int)
#define IOCTL_MSG_DOMEXISTS _IOWR(PSI_ECAT_VERSION_MAGIC, 21, int)

/*------------------------------------------ */
typedef struct _ecd_master ecd_master;
typedef struct _ecd_domain ecd_domain;
typedef struct _ecnode ecnode;


/*------------------------------- */


/* */
/* */
typedef enum  {
	DCT_NOT_CONFIGURED = 0,
	DCT_CONFIGURED = 0x01,
	DCT_AUTOCONFIG = 0x10
} DOMAIN_CONFIG_TYPE;


typedef enum {
	REC_ERROR = 0,

	REC_AI,
	REC_AO,
	REC_BI,
	REC_BO,
	REC_MBBI,
	REC_MBBO,
	REC_MBBIDIRECT,
	REC_MBBODIRECT,
	REC_LONGIN,
	REC_LONGOUT,
	REC_STRINGIN,
	REC_STRINGOUT,
	REC_WAVEFORM,
	REC_FANOUT,
	REC_DFANOUT,
	REC_CALC,
	REC_CALCOUT,
	REC_CPID,
	REC_AAI,
	REC_AAO,

	REC_LAST,
} RECTYPE;
/*------------------------------------------ */

typedef struct {
	int offs;
	int bit;
	int bitlen;

	int byteoffs; /* for .Onn */
	int bytelen; /* for .Lnn */

	int bitspec;
	epicsType typespec;
	char *typename;

	int rw_dir;
	int nobt;
	int shft;
	int mask;
} domain_register;

typedef struct _conn_rec {
	struct _conn_rec *next;

	int ix; /* index in rectypes, to speed up printing */
	RECTYPE rectype;
	dbCommon *rec;
	domain_register *dreg_info;
	/* aai & aao */
	int ftvl_len;
	int ftvl_type;
} conn_rec;



typedef struct _domain_reginfo {
	ecnode *master;
	ecnode *slave;
	ecnode *sync;
	ecnode *pdo;
	ecnode *pdo_entry;

	ecnode *domain;
	ecnode *domain_entry;

	unsigned int byte;
	unsigned int bit;
	unsigned int bit_length;


} domain_reg_info;

typedef struct _sts_entry {
	struct _sts_entry *next;

	ecnode *pdo_entry_from;
	ecnode *pdo_entry_to;
	domain_register from;
	domain_register to;

} sts_entry;


struct _ecd_domain {

    ecnode *nmaster;
    struct task_struct *domain_thread;
    struct dentry *mmfile;


    int is_running;
    int rate; /* in microseconds */

    char *dmem;    /* domain mem */
    char *rmem;    /* R mem */
    char *wmem;    /* W mem */
    int dallocated;
    int dsize;

    /* sts */
	epicsMutexId sts_lock;
    int num_of_sts_entries;
    sts_entry *sts;

	/* cfg */
    DOMAIN_CONFIG_TYPE dcfgtype;
	int num_of_regs;
	ec_pdo_entry_reg_t *regs;
	domain_reg_info *reginfos;

};

typedef struct _ecd_slave {

	int check;

    int health;
    ec_slave_info_t config_slave_info;

} ecd_slave;


/*------------------------------------------ */


struct _ecd_master {
	ec_master_t *master;

	ec_master_info_t master_info;
    ec_master_state_t master_state;

	ec_master_info_t master_info_at_start;
	int health;
	int link_up;
	int slave_aggr_health;

    struct task_struct *master_thread;

};


/*------------------------------------- */
typedef enum {
	ECNT_ERROR = 0,
	ECNT_ROOT,            /* 1 */
	ECNT_MASTER,          /* 2 */
	ECNT_SLAVE,           /* 3 */
	ECNT_SYNC,            /* 4 */
	ECNT_PDO,             /* 5 */
	ECNT_PDO_ENTRY,       /* 6 */

	ECNT_DOMAIN,          /* 7 */
	ECNT_DOMAIN_ENTRY     /* 8 */
} ECN_TYPE;

/* very flat node structure for */
/* easier access to members in code */
struct _ecnode {
	ECN_TYPE	type;
	struct _ecnode *parent;
	struct _ecnode *next;
	struct _ecnode *child;

	/* common */
	int nr;
	struct proc_dir_entry *pdire;
	char *path;


	ec_pdo_entry_info_t pdo_entry_t;
	ec_pdo_info_t 		pdo_t;
	ec_sync_info_t 		sync_t;
	ec_slave_info_t 	slave_t;

	union
	{
		ec_master_t *master_t;
		ec_domain_t *domain_t;
	};

	union
	{
		ecd_master mdata;
		ecd_domain ddata;
		ecd_slave sdata;
	};


	/* if pdo entry belongs to a domain */
	ecnode *de_master;
	ecnode *de_slave;
	ecnode *de_sync;
	ecnode *de_pdo;
	ecnode *de_pdo_entry;
	ecnode *de_domain;

	ecnode *domain_entry;

	int id;


	conn_rec *cr;

};



/*----------------------------------------- */

typedef enum {
	ERR_ERROR = 0,
	ERR_NO_ERROR = 1,

	ERR_OUT_OF_MEMORY 		= 0x100,
	ERR_BAD_REQUEST 		= 0x101,
	ERR_DOES_NOT_EXIST		= 0x102,
	ERR_ALREADY_EXISTS 		= 0x103,
	ERR_PREREQ_FAIL 		= 0x104,
	ERR_OUT_OF_RANGE        = 0x105,

	ERR_BAD_ARGUMENT 		= 0x1a0,
	ERR_BAD_ARGUMENT_1 		= 0x1a1,
	ERR_BAD_ARGUMENT_2 		= 0x1a2,
	ERR_BAD_ARGUMENT_3 		= 0x1a3,
	ERR_BAD_ARGUMENT_4 		= 0x1a4,
	ERR_BAD_ARGUMENT_5 		= 0x1a5,
	ERR_BAD_ARGUMENT_6 		= 0x1a6,
	ERR_BAD_ARGUMENT_7 		= 0x1a7,
	ERR_BAD_ARGUMENT_8 		= 0x1a8,
	ERR_BAD_ARGUMENT_9 		= 0x1a9,

	ERR_OPERATION_FAILED 	= 0x200,
	ERR_OPERATION_FAILED_1 	= 0x201,
	ERR_OPERATION_FAILED_2 	= 0x202,
	ERR_OPERATION_FAILED_3 	= 0x203,
	ERR_OPERATION_FAILED_4 	= 0x204,
	ERR_OPERATION_FAILED_5 	= 0x205,
	ERR_OPERATION_FAILED_6 	= 0x206,
	ERR_OPERATION_FAILED_7 	= 0x207,
	ERR_OPERATION_FAILED_8 	= 0x208,
	ERR_OPERATION_FAILED_9 	= 0x209,


/*
	ERR_ = 10,
	ERR_ = 10,
	ERR_ = 10,
	ERR_ = 10,
	ERR_ = 10,
	ERR_ = 10,
	ERR_ = 10,
	ERR_ = 10,
*/



} EC_ERR;

typedef enum {
	S_NUM = 0,
	SM_NUM,
	P_NUM,
	E_NUM,
	B_NUM,
	L_NUM,
	O_NUM,
	D_NUM,
	R_NUM,
	LR_NUM,
	M_NUM,
	T_NUM
} TOKNUM;


typedef struct ecat {
	struct ecat *next;


	ecnode *m;
	ecnode *d;
	long rate;
	double freq;

	IOSCANPVT r_scan;
	IOSCANPVT w_scan;

	int dnr; /* domain nr */

	char *mmap_fname;

	char *r_data;
	char *w_data;

	char *w_mask;

	char *irq_r_mask;
	epicsMutexId irq_lock;

	epicsMutexId rw_lock;
	epicsMutexId health_lock;

	int dsize;
	epicsThreadId dthread; /* domain worker thread */

	epicsEventId irq;
	epicsThreadId irqthread; /* domain irq thread */

	epicsThreadId scthread; /* slave check thread */


	int test;

} ethcat;


typedef enum {
	SRT_ERROR = 0,
	SRT_M_STATUS,
	SRT_S_STATUS,
	SRT_L_STATUS,
	SRT_S_OP_STATUS,


} SYSTEM_REC_TYPE;

typedef struct {
	int system;                   /* a system record? */
	SYSTEM_REC_TYPE sysrectype;
	int nr;
	int master_nr;
} system_rec_data;


#define PPREFIX "===== ecat2: "
#define EPT_MAX_TOKENS 16

int parse_str( char *s, ethcat **e, ecnode **pe, int *dreg_nr, domain_register *dreg, system_rec_data *srdata );
int slave_has_static_config( int slave_nr );
#endif


















