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
 * File name : intech_function.cpp
 *
 * File Description : Describe functions created by Intec Telecom.
 *
 * Author : Kim Jong In (jongink@imcu.co.kr)
 * Dept : Software
 * Created Date : 2018.07.xx
 * Version	: .
 */

// #define CHECK_BIT
// #define FPS_SYSTEM

#include "intech_global.h"

#include "acqFPSAsynEpics.h"

/**
 * Function Name : StartFPS()
 *
 * Function Description : It is the first function to perform PMS and additional functions.
 *                        - Read the Bram data of Zynq PL and store it in DDR.
 *                          When the interlock occurs, the function stops the iterative 
 *                          operation and then activates the Event function to generate the PMS data file.
 * Input : none
 * Output : none
 * Version : .
 */

int lo =0;
int debug_cnt = 0;
int Store_Data_Index =0;
int Interlock_Pos = 0;
int Interlock_Num = 0;
int acqFPSAsynEpics::StartFPS()
{
	int i;
	int result = 0;
	unsigned int ready_mode;
	unsigned int mode_offset;
	unsigned int interlock_flag;
	unsigned int PMS_Unlock = 0;

	ready_mode = (pacqFPSDriver->ReadUInt32GPIO());	// 0 or 1 or 2
	interlock_flag = (ready_mode & 0x4)>>2;	// PL 에서 Bram Data 를 분석하여 해당 Bram에서 Interlock 발생했는지 알려주는 Flag
	ready_mode = ready_mode & 0x3;			// Bram Read에 대한 Mode 0 : read 안함, 1 : 0x4004_0000 주소 Read , 2 : 0x4000_0000

	PMS_Unlock = ((pacqFPSDriver->ReadUInt32Data(PMS_FLAG,Bram1_Idx) >> 15) & 0x1 );	// PV에서 PMS 제어를 위한 Trigger

	if(ready_mode != 0 )
	{
		mode_offset = (ready_mode == 1)? 0 : 0x40000;

		pacqFPSDriver->WriteUInt32_GPIO(1); // interrupt값 1 셋팅
		
		// 16 * 10000 Byte Read
		pacqFPSDriver->ReadUInt32Array(mode_offset, (unsigned int *)&Store_Data[Store_Data_Index][0]);	// 460 use

		// memcpy(&test_Time, &Store_Data[Store_Data_Index][0].Time_Data, sizeof(test_Time));
		if( (interlock_flag == 1) && (PMS_Unlock == 1) )
		{
			for( i = 0; i< 10000 ; i++)
			{
				if( Store_Data[Store_Data_Index][i].Time_Data.interlock == 1){
					//printf("interlock %d\n", Store_Data_Index);
					Interlock_Pos = Store_Data_Index;
					Interlock_Num = i;
					break;
				}
			}

			// File 이름에 사용되는 시간정보변수
			Interlock_Time_Data = Store_Data[Store_Data_Index][i].Time_Data;

			printf("Interlock Idx : (%d) 10000 Count Number : %d \n", Interlock_Pos, Interlock_Num);
			// printf("111 interlock_time_H (0x%x) interlock_time_L (0x%x)\n", interlock_time_H, interlock_time_L);

			result = 1;

		}else{
			result = 0;
		}

		if(Store_Data_Index < PREV_MAX){
			Store_Data_Index++;
		}else{
			Store_Data_Index = 0;
		}

		pacqFPSDriver->WriteUInt32_GPIO(0); // interrupt값 0 셋팅

	}else{
		//ready_mode == 0 : Store 안함.
		result = 0;
	}
	return result;
}


/**
 * Function Name : EventPMS()
 *
 * Function Description : It is operated by 'StartFPS' function and it carries out the function 
 *                        of writing the remaining BRAM data to the DDR after interlocking.
 *                        In addition, data of 200 msec is written from BRAM data to DDR from the time of interlock occurrence.
 * Input : none
 * Output : none
 * Version : .
 */
void acqFPSAsynEpics::EventPMS()
{
	unsigned int mode_offset = 0;
	unsigned int ready_mode = 0;
	int PMS_Index = CURR_MIN;
    //Current_Offset = temp_index;            //나머지 bram데이터를 쓰고난 data의 인덱스 위치를 옵셋으로 지정

    //printf("400ms Store Start \n");
    while(PMS_Index < CURR_MIN + CURR_SIZE)       //400ms까지 데이터를 쓰고나면 루프를 빠져나감.
    {
		// ready_mode = pacqFPSDriver->ReadUInt32Data(0x1000000,0);	// 0 or 1 or 2
		ready_mode = (pacqFPSDriver->ReadUInt32GPIO() & 0x3) ;	// 0 or 1 or 2
        if( ready_mode != 0)
		{
			pacqFPSDriver->WriteUInt32_GPIO(1); // interrupt값 1 셋팅
				mode_offset = (ready_mode == 1) ? 0 : 40000;

				// 16 * 10000 Byte Read
				pacqFPSDriver->ReadUInt32Array(mode_offset, (unsigned int *)&Store_Data[PMS_Index][0]);	// 460 use

				PMS_Index ++;
			pacqFPSDriver->WriteUInt32_GPIO(0); // interrupt값 0 셋팅
		}else{
		}
	}

	Get_PMS_FileName(pmsdata_filename);
	Create_Log(pmsdata_filename);      //인터락이 걸린 위치값부터 파일을 생성
}


/**
 * Function Name : Create_Log(int Position, unsigned int *data, char *FileName)
 * Function Description : Call the 'Get_PMS_FileName' function to determine the name of the PMS Data file to be created, 
 *                          and call the 'Create_Log' function to create the file.
 * Input : Position - When interlock occurs , *data - Address of variable to load in DDR , FileName - Name of the file to be created
 * Output : none
 * Version : .
 */
// int acqFPSAsynEpics::Create_Log(int Position, unsigned int *data, char *FileName)
int acqFPSAsynEpics::Create_Log( char *FileName)
{
	int fd = 0;
	int current_pms_val = 0;
	int line_cnt = 0;
	char buf[60];

	//file write param
	int i,j;
	int index 		= Interlock_Pos;
	int num 		= Interlock_Num;

    fd = open(FileName, O_RDWR| O_CREAT, FILEMODE);
	if(fd < 0){
		perror("Error : fd open ");
	}

    printf("\nStart File Write...\n");
    printf("interlock Position : %d , Num : %d\n", index,num);
	
	sprintf(buf, "line_Num,index,interlock,TimeStamp,1us Count,dry,wet\n");
    lseek(fd, 0, SEEK_CUR);
    lseek(fd, 0, SEEK_END);
    if( write(fd, buf, strlen(buf)) == 0){
		perror("PMS FILE WRITE ERROR \n");
	}
	line_cnt++;

	//////////////////////////////////////////////////////////////////
	//				-200ms ~ 0ms 
	//////////////////////////////////////////////////////////////////
	if ( index == PREV_MAX){
		i = 0;
	}else{
		i = index + 1;
	}

	j = num;

	while (1)
	{
		if(j < 10000)
		{
			{	// Write File
				file_wr(line_cnt,Store_Data[i][j], &fd, i);
				line_cnt++;
			}
			j++;
		}
		else
		{
			if( i == index){
				break;
			}else if(i == PREV_MAX){
				i = 0;
			}else{
				i++;
			}

			j = 0;
		}
	}

	//////////////////////////////////////////////////////////////////
	//				0ms ~ 200ms 
	//////////////////////////////////////////////////////////////////
	i = CURR_MIN;
	j = 0;
	while(1)
	{
		if( j < 10000 )
		{
			if( (j == num) && (i == CURR_MAX) )
			{
				break;
			}

			{
				file_wr(line_cnt,Store_Data[i][j], &fd, i);
				line_cnt++;
			}
			j++;
		}
		else
		{
			i++;
			j = 0;
		}
	}

    printf("Saved PMS File\n\n");

    //PMS Flag Off
    current_pms_val = pacqFPSDriver->ReadUInt32Data(PMS_FLAG,Bram1_Idx);
    current_pms_val &= ~(1<<15);
	// printf(" set PMS_Flag Value = 0x%x\n",current_pms_val);
    pacqFPSDriver->WriteUInt32Data(PMS_FLAG, current_pms_val);

    return 0;
}

void acqFPSAsynEpics::file_wr(unsigned int Line_Num, PMS_Data_t acqNode, int *fd, int interlockIdx)
{
	char buf[100] = {0,};
	memset(buf, 0 , sizeof(buf));

	sprintf(buf, "%06d,%02d,%01d,%02d-%03d-%02d-%02d-%02d-%04d,%05d,%#x,%#x\n",
				Line_Num,
				interlockIdx,
				acqNode.Time_Data.interlock, 
				acqNode.Time_Data.year, 
				acqNode.Time_Data.day, 
				acqNode.Time_Data.hour, 
				acqNode.Time_Data.min, 
				acqNode.Time_Data.sec, 
				acqNode.Time_Data.msec, 
				acqNode.Time_Data.count, 
				acqNode.Dry_Data, 
				acqNode.Wet_Data
				);

    lseek(*fd, 0, SEEK_CUR);
    // write(fd, buf, strlen(buf));
    lseek(*fd, 0, SEEK_END);
    if (write(*fd, buf, strlen(buf)) == 0)
	{
		perror("PMS FILE WRITE ERROR \n");
	}
}


/**
 * Function Name : acqFPSAsynEpics::Get_PMS_FileName(char *str, int size)
 * Function Description : Call the 'Get_PMS_FileName' function to determine the name of the PMS Data file to be created, 
 *                          and call the 'Create_Log' function to create the file.
 * Input : Position - When interlock occurs , *data - Address of variable to load in DDR , FileName - Name of the file to be created
 * Output : none
 * Version : .
 */
void acqFPSAsynEpics::Get_PMS_FileName(char *str, int size)
{
    char AN_Number[10] = { 0,};

	sprintf(str,"y-%d_d-%d_h-%d_m-%d_s-%d_ms-%d_",
		Interlock_Time_Data.year,
		Interlock_Time_Data.day,
		Interlock_Time_Data.hour,
		Interlock_Time_Data.min,
		Interlock_Time_Data.sec,
		Interlock_Time_Data.msec
		);

	Get_NodeName(AN_Number);
	strcat(str, AN_Number);	// Interloc Time + AN_Number

    strcat(str, ".fps"); 	//Get_NodeName(nodename); 함수에서 처리하도록 변경
	// printf("Get_PMS_FileName str : %s \n", str);

    Get_Absolute_Path(str, size);
}

/**
 * Function Name : acqFPSAsynEpics::Get_NodeName(char *str)
 * Function Description : 
 * Input : 
 * Output : none
 * Version : .
 */
void acqFPSAsynEpics::Get_NodeName(char *str)
{
	char buf[8];
	#if 1
		sprintf(buf, "AN%d",Board_Name_env);
	#else
		int value;
		int Link;
		int Node;
		value = pacqFPSDriver->ReadUInt32Data(OFFSET_BOARDID);     //Get a board ID
		Link = (value >> 28) & 0xf;                 //상위 4비트를 추출
		Node = value & 0xf;                         //하위 4비트를 추출
		sprintf(buf, "AN%d%d",Link,Node);        // Link 1~2 Node 1~7까지
	#endif

	strcpy(str, buf);
}

/**
 * Function Name : CheckFile()
 * Function Description : 
 * Input : 
 * Output : none
 * Version : .
 */
void acqFPSAsynEpics::CheckFile()
{
    int value = 0;
    char filename[FILE_MAX_SIZE] = {0,};  //include path

    Get_PMS_FileName(filename); // included file path

    value = access(filename, F_OK);   // Check file existence?    existence == 0 / don't existence == Positive integer

    if(value == 0)
    {
        setParamValue("FileCheck", 1);         //pv write 1 ..... LED on
    }
    else
    {
        setParamValue("FileCheck", 0);         //pv write 0 ..... LED off
    }
}

/**
 * Function Name : Get_Absolute_Path(char *filename, int size)
 * Function Description : 
 * Input : 
 * Output : none
 * Version : .
 */
void acqFPSAsynEpics::Get_Absolute_Path(char *filename, int size)
{
    char *filepath;

    filepath = (char *)malloc(sizeof(filename)* size);

//    strcpy(filepath, "/home/debian/epics/R3.14.12.5/siteApps/Ctrl_IOC/fps-1-0/iocBoot/iocacq/");
    strcpy(filepath, "/mnt/sdcard/Log/");
    strcat(filepath, filename);		// /mnt/sdcard/Log/filename
    strcpy(filename, filepath);

    free(filepath);
}