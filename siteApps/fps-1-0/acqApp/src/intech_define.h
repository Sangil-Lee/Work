/**
 * Project Name : FPS - Fast Protection System
 * 
 * Copyright 2018 by Intech Telecom Corporation, Inc
 * All right reserved.
 * 
 * Project Description :    To protect the accelerators from sudden beam loss during RAON accelerator operation, 
 *                          we want to develop a Fast Protection System (FPS) based on Field Programmable Gate Array (FPGA).
 *                          Prior to the fabrication of the FPS including the entire range of the RAON accelerator, 
 *                          one chain consisting of one mitigation node and seven acquisition nodes is to be tested.
 *                          Monitoring and operation of this system is performed in the CS-Studio (CSS) Operator Interface (OPI) 
 *                          environment and the data is transferred to the central control system through the EPICS IOC.
 */

/** 
 * File name : intech_define.h
 *
 * File Description : Describes define values used in functions created by INTEC Telecom.
 *
 * Author : Kim Jong In (jongink@imcu.co.kr)
 * Dept : Software
 * Created Date : 2018.07.xx
 * Version	: .
 */

#ifndef __INTECH_DEFINE_H
#define __INTECH_DEFINE_H

/* basee address */
#define XPAR_BRAM_0_BASEADDR    0x40000000
#define XPAR_BRAM_20_BASEADDR   0x40040000
#define XPAR_BRAM_1_BASEADDR    0x42000000
//#define XPAR_BRAM_2_BASEADDR  0x46000000

#define XPAR_GPIO_0_BASEADDR    0x41000000
#define XPAR_GPIO_1_BASEADDR    0x41001000

/* bram type */ 
#define Bram0_Idx 0 // 4000_0000 
#define Bram1_Idx 1 // 4200_0000
//#define Bram2_Idx 2 // 4600_0000

/* 4200_0000 address offset */
#define GTX_RESET		(0x00)
#define PMS_FLAG        (0x00)
#define INTERLOCK_STATE (0x03)
#define OFFSET_BOARDID	(0x0A)
#define PMS_CLEAR       (0x0D)
#define VALID_BOARDID	(0x1A)	// set 1 -> set 0  BoardID Set

/* file */
#define FILEMODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
#define FILE_MAX_SIZE 200   //Array Size

#define PREV_MIN	(	0	)
#define PREV_MAX	(	20	)
#define PREV_SIZE	(	21	)
#define CURR_MIN	(	21	)
#define CURR_MAX	(	40	)
#define CURR_SIZE	(	20	)

#endif
