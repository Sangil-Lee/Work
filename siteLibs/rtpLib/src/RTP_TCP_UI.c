#include <ansi_c.h>
#include <cvirte.h>		
#include <userint.h>
#include "RTP_TCP_UI.h"
#include "RTP_TCP.h"

static int mainPanel;

int main (int argc, char *argv[])
{
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
	if ((mainPanel = LoadPanel (0, "RTP_TCP_UI.uir", MAIN_PANEL)) < 0)
		return -1;
	DisplayPanel (mainPanel);
	RunUserInterface ();
	DiscardPanel (mainPanel);
	return 0;
}

int CVICALLBACK Panel_Proc (int panel, int event, void *callbackData,
		int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			closesocket(rtpSocket);
			WSACleanup();
			QuitUserInterface (0);
			break;
	}
	return 0;
}

int CVICALLBACK ReadBool (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	unsigned char _maskBit[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80}; // 비트별 마스킹 
	unsigned short resultBool[NUMBER_TO_READ] = {0, 0, 0, 0, 0};
	unsigned short bool_Number_to_read= 0;
	unsigned short result = 0;
	int numeric[5] = {MAIN_PANEL_BOOL_PRINT_1, MAIN_PANEL_BOOL_PRINT_2, MAIN_PANEL_BOOL_PRINT_3, MAIN_PANEL_BOOL_PRINT_4, MAIN_PANEL_BOOL_PRINT_5};
	int i;
	
	switch (event)
	{
		case EVENT_COMMIT:
			ReadBoolData();
			
			GetCtrlVal(MAIN_PANEL, MAIN_PANEL_BOOL_NUMBER_TO_READ, &bool_Number_to_read);
			
			for(i = 0; i < 5; i++)
			{
				SetCtrlVal(MAIN_PANEL, numeric[i], 0);
				
			}
			
			
			for(i = 0; i < bool_Number_to_read; i++)
			{
				result = bool_Read_Response[5] & _maskBit[i];
				
				if(result > 0)
				{
					resultBool[i] = 1;
				}
				else if(result == 0)
				{
					resultBool[i] = 0;
				}
				
				 SetCtrlVal(MAIN_PANEL, numeric[i], resultBool[i]); 
			}
			
			break;
	}
	return 0;
}

int CVICALLBACK ReadInt (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	short int resultInt[NUMBER_TO_READ] = {0, 0, 0, 0, 0};
	unsigned short int_Number_to_read = 0;
	int numeric[5] = {MAIN_PANEL_INT_PRINT_1, MAIN_PANEL_INT_PRINT_2, MAIN_PANEL_INT_PRINT_3, MAIN_PANEL_INT_PRINT_4, MAIN_PANEL_INT_PRINT_5};
	int i, j;
	
	switch (event)
	{
		case EVENT_COMMIT:
			ReadIntData();
			
			GetCtrlVal(MAIN_PANEL, MAIN_PANEL_INT_NUMBER_TO_READ, &int_Number_to_read);
			
			for(i = 0; i < 5; i++)
			{
				SetCtrlVal(MAIN_PANEL, numeric[i], 0);
				
			}
			
			i = 5;
			j = 0;
			while(i <= (((int_Number_to_read - 1) * sizeof(short int)) + 5))
			{
				memcpy(&resultInt[j], &int_Read_Response[i], sizeof(short int));
				SetCtrlVal(MAIN_PANEL, numeric[j], resultInt[j]);
				
				j++;
				i += sizeof(short int);
			}
			break;
	}
	return 0;
}

int CVICALLBACK ReadFloat (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	float resultFloat[NUMBER_TO_READ] = {0.0, 0.0, 0.0, 0.0, 0.0};
	unsigned short float_Number_to_read = 0;
	int numeric[5] = {MAIN_PANEL_FLOAT_PRINT_1, MAIN_PANEL_FLOAT_PRINT_2, MAIN_PANEL_FLOAT_PRINT_3, MAIN_PANEL_FLOAT_PRINT_4, MAIN_PANEL_FLOAT_PRINT_5};
	int i, j;
	
	switch (event)
	{
		case EVENT_COMMIT:
			ReadFloatData();
			
			GetCtrlVal(MAIN_PANEL, MAIN_PANEL_FLOAT_NUMBER_TO_READ, &float_Number_to_read);
			
			for(i = 0; i < 5; i++)
			{
				SetCtrlVal(MAIN_PANEL, numeric[i], 0.0);
				
			}
			
			i = 5;
			j = 0;
			while(i <= (((float_Number_to_read - 1) * sizeof(float)) + 5))
			{
				memcpy(&resultFloat[j], &float_Read_Response[i], sizeof(float));
				SetCtrlVal(MAIN_PANEL, numeric[j], resultFloat[j]); 
				j++;
				i += sizeof(float);
			}
			break;
	}
	return 0;
}

int CVICALLBACK WriteBool (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	unsigned short writeIndex;
	char writeBool;
	
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(MAIN_PANEL, MAIN_PANEL_BOOL_WRITE_INDEX, &writeIndex);
			GetCtrlVal(MAIN_PANEL, MAIN_PANEL_BOOL_WRITE_VALUE, &writeBool);
			
			WriteBoolData(writeIndex, writeBool);

			break;
	}
	return 0;
}

int CVICALLBACK WriteInt (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	unsigned short writeIndex;
	int writeInt;
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(MAIN_PANEL, MAIN_PANEL_INT_WRITE_INDEX, &writeIndex);
			GetCtrlVal(MAIN_PANEL, MAIN_PANEL_INT_WRITE_VALUE, &writeInt);
			
			WriteIntData(writeIndex, &writeInt);
			break;
	}
	return 0;
}

int CVICALLBACK WriteFloat (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	unsigned short writeIndex;
	float writeFloat;
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(MAIN_PANEL, MAIN_PANEL_FLOAT_WRITE_INDEX, &writeIndex);
			GetCtrlVal(MAIN_PANEL, MAIN_PANEL_FLOAT_WRITE_VALUE, &writeFloat);
			
			WriteFloatData(writeIndex, &writeFloat);
			break;
	}
	return 0;
}

int CVICALLBACK InitRTPConnection (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			char *ipAddress = "255.255.255.255";	    
			
			GetCtrlVal(MAIN_PANEL, MAIN_PANEL_IP_ADDRESS, ipAddress);
			
			InitConnection(ipAddress);

			break;
	}
	return 0;
}
