/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/* Copyright (c) National Instruments 2012. All Rights Reserved.          */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  MAIN_PANEL                       1       /* callback function: Panel_Proc */
#define  MAIN_PANEL_CONNECTION_BUTTON     2       /* callback function: InitRTPConnection */
#define  MAIN_PANEL_Float_1               3
#define  MAIN_PANEL_Float_2               4
#define  MAIN_PANEL_Float_3               5
#define  MAIN_PANEL_Float_5               6
#define  MAIN_PANEL_Float_4               7
#define  MAIN_PANEL_FLOAT_VARIABLE        8
#define  MAIN_PANEL_INT_VARIABLE          9
#define  MAIN_PANEL_INT_1                 10
#define  MAIN_PANEL_INT_3                 11
#define  MAIN_PANEL_INT_4                 12
#define  MAIN_PANEL_INT_5                 13
#define  MAIN_PANEL_INT_2                 14
#define  MAIN_PANEL_BOOL_1                15
#define  MAIN_PANEL_BOOL_2                16
#define  MAIN_PANEL_BOOL_3                17
#define  MAIN_PANEL_BOOL_4                18
#define  MAIN_PANEL_BOOL_READ_BUTTON      19      /* callback function: ReadBool */
#define  MAIN_PANEL_INT_READ_BUTTON       20      /* callback function: ReadInt */
#define  MAIN_PANEL_FLOAT_READ_BUTTON     21      /* callback function: ReadFloat */
#define  MAIN_PANEL_BOOL_5                22
#define  MAIN_PANEL_SPLITTER              23
#define  MAIN_PANEL_BOOL_VARIABLE         24
#define  MAIN_PANEL_BOOL_WRITE            25      /* callback function: WriteBool */
#define  MAIN_PANEL_INT_WRITE             26      /* callback function: WriteInt */
#define  MAIN_PANEL_FLOAT_WRITE           27      /* callback function: WriteFloat */
#define  MAIN_PANEL_FLOAT_WRITE_VARIABLE  28
#define  MAIN_PANEL_INT_WRITE_VARIABLE    29
#define  MAIN_PANEL_BOOL_WRITE_VARIABLE   30
#define  MAIN_PANEL_IP_ADDRESS            31
#define  MAIN_PANEL_INT_NUMBER_TO_READ    32
#define  MAIN_PANEL_BOOL_NUMBER_TO_READ   33
#define  MAIN_PANEL_FLOAT_NUMBER_TO_READ  34
#define  MAIN_PANEL_BOOL_PRINT_5          35
#define  MAIN_PANEL_BOOL_PRINT_4          36
#define  MAIN_PANEL_BOOL_PRINT_3          37
#define  MAIN_PANEL_BOOL_PRINT_2          38
#define  MAIN_PANEL_BOOL_PRINT_1          39
#define  MAIN_PANEL_INT_PRINT_5           40
#define  MAIN_PANEL_INT_PRINT_4           41
#define  MAIN_PANEL_INT_PRINT_3           42
#define  MAIN_PANEL_INT_PRINT_2           43
#define  MAIN_PANEL_INT_PRINT_1           44
#define  MAIN_PANEL_FLOAT_PRINT_5         45
#define  MAIN_PANEL_FLOAT_PRINT_4         46
#define  MAIN_PANEL_FLOAT_PRINT_3         47
#define  MAIN_PANEL_FLOAT_PRINT_2         48
#define  MAIN_PANEL_FLOAT_PRINT_1         49
#define  MAIN_PANEL_BOOL_WRITE_INDEX      50
#define  MAIN_PANEL_BOOL_WRITE_VALUE      51
#define  MAIN_PANEL_INT_WRITE_INDEX       52
#define  MAIN_PANEL_INT_WRITE_VALUE       53
#define  MAIN_PANEL_FLOAT_WRITE_INDEX     54
#define  MAIN_PANEL_FLOAT_WRITE_VALUE     55


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK InitRTPConnection(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Panel_Proc(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK ReadBool(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK ReadFloat(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK ReadInt(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK WriteBool(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK WriteFloat(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK WriteInt(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
