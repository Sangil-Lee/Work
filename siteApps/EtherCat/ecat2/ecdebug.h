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
 * Any changes to this file preclude further distribution.
 * Bugs/errors/patches/feature requests can be sent directly to the author,
 * and if they are accepted, will be included in the next version or release.
 *
 * Disclaimer:
 * The software (code, tools) is provided "as is", without warranty of any kind.
 * Author makes no warranties, express or implied, that the code is free of
 * errors, or are consistent with anyu particular standard, or that it
 * will meet your requirements for any particular application.
 * It should not be relied on for solving a problem whose correct or incorrect
 * solution could result in injury to a person or a loss of property.
 * The author disclaim all liability for direct, indirect or
 * consequential damage resulting from your use of the code or tools.
 * If you do use it, it is at your own risk.
 *
 */

#ifndef ECDEBUG_H
#define ECDEBUG_H

#define dbgprinterr  printf( "%s:%d (C%02d): ", __FILE__, __LINE__, card->cardnumber )
#define printerr     errlogSevPrintf(errlogFatal, "%s:%d (C%02d): ", __FILE__, __LINE__, card->cardnumber )
#define printerr2    errlogSevPrintf(errlogFatal, drvethercatNAME " (card no. %d): ", card->cardnumber )
#define printerr3    errlogSevPrintf(errlogFatal, drvethercatNAME )

#define dmmARGCHECK( cond, errmsg ) \
            do { if( cond ) { printf( drvicsNAME ": " errmsg "\n"); return S_dev_badArgument; } } while(0)

#define dmmCHECKerrno( cond, errmsg ) \
            do { if( cond ) { printerr2; dmmdebug errmsg; return errno; } } while(0)

#define dmmCHECKret( cond, errmsg, retv ) \
            do { if( cond ) { printerr2; dmmdebug errmsg; return retv; } } while(0)

#define dmmCHECK( cond, errmsg, retv ) \
            do { if( cond ) { printerr3; dmmdebug errmsg; return retv; } } while(0)

#define dmmERR( errmsg, retv ) \
            do { printerr3; dmmdebug errmsg; return retv; } while(0)


#define dmmDEBUG(debuglevel, errmsg) \
        do { if( dmmDEBUGVAR >= debuglevel || dmmDEBUGVAR == DBG_all )  { dbgprinterr; dmmdebug errmsg;}  } while (0)

#define dmmDEBUGeq(debuglevel, errmsg)  \
        do { if( (dmmDEBUGVAR & debuglevel) || dmmDEBUGVAR == DBG_all ) { dbgprinterr; dmmdebug errmsg;}  } while (0)

#define dmmDEBUGblk(debuglevel, execblock)  \
        do { if( (dmmDEBUGVAR & debuglevel) || dmmDEBUGVAR == DBG_all ) { dbgprinterr; execblock;}  } while (0)

/*------------------------------------*/
/*                                    */
/* Debug                              */
/*                                    */
/*------------------------------------*/
enum dmmDBG {
    DBG_irqwt2 = 0x00000002,     /* irq wt pass, count    */
    DBG_irqwt1 = 0x00000001,     /* irq wt channels       */
    DBG_init   = 0x00000004,     /*                       */
    DBG_4      = 0x00000008,     /*                       */
    DBG_dma1   = 0x00000010,     /* DMA done              */
    DBG_dma2   = 0x00000020,     /* DMA transfer info     */
    DBG_dma3   = 0x00000040,     /* DMA error             */
    DBG_8      = 0x00000080,     /*                       */
    DBG_9      = 0x00000100,     /*                       */
    DBG_10     = 0x00000200,     /*                       */
    DBG_11     = 0x00000400,     /*                       */
    DBG_12     = 0x00000800,     /*                       */
    DBG_write  = 0x00001000,     /* write                 */
    DBG_read   = 0x00002000,     /* read                  */
    DBG_15     = 0x00004000,     /*                       */
    DBG_16     = 0x00008000,     /*                       */

};


/*--------------------------------- */

/*#define DEBUG_FNCALLED */
/*#define DEBUG_FNCALLED_2 */
/*#define DEBUG_FNCALLED_3 */

#ifdef DEBUG_FNCALLED
	#define FN_CALLED   { printf( PPREFIX "%s called (%s, line %d)\n", __func__, __FILE__, __LINE__ ); }
#else
	#define FN_CALLED 	{ (void)0; }
#endif

#ifdef DEBUG_FNCALLED_2
	#define FN_CALLED2   { printf( PPREFIX "%s called (%s, line %d)\n", __func__, __FILE__, __LINE__ ); }
#else
	#define FN_CALLED2 	{ (void)0; }
#endif

#ifdef DEBUG_FNCALLED_3
	#define FN_CALLED3   { printf( PPREFIX "%s called (%s, line %d)\n", __func__, __FILE__, __LINE__ ); }
#else
	#define FN_CALLED3 	{ (void)0; }
#endif


/*--------------------------------- */
void st_start( int no );
void st_end( int no );
void st_print( int no );
void st_print2( int no );

#endif


















