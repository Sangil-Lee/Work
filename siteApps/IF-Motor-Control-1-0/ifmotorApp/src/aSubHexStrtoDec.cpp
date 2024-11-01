/*
 * Convertion Hexa String to Decimal
const char *hexstring = "abcdef0";
// 16의 의미는 hexstring 이 16진수라는 의미입니다.
int number = (int)strtol(hexstring, NULL, 16);

//String 문자열에 0x 의 16진수 기호가 붙어 있을 경우는 아래와 같이 사용합니다. 
const char *hexstring = "0xabcdef0";
int number = (int)strtol(hexstring, NULL, 0);
*/

#include <aSubRecord.h>
#include <epicsExport.h>

#include <dbAccess.h>
#include <link.h>
#include <dbDefs.h>
#include <dbLock.h>
#include <dbAddr.h>
#include <epicsThread.h>
#include <epicsMessageQueue.h>
#include <registryFunction.h>
#include <recSup.h>
#include <stringinRecord.h>
#include <longinRecord.h>
#include <epicsExport.h>

#include <stdio.h>
#include <string.h>
#include <cstdio>
#include <string>
#include <iostream>
#include <bitset>
#include <algorithm>

using namespace std;

static long InitHexaStr2Dec(aSubRecord *precord)
{
    printf("aSub Initialize\n");
    return(0);
}

static long ProcHexaStr2Dec(aSubRecord *precord)
{
	long status = 0;

	const char* hexstring = (const char*)precord->a;
	// 16의 의미는 hexstring 이 16진수라는 의미입니다.
	int number = (int)strtol(hexstring, NULL, 16);

	long *uval = (long*)precord->val;
	uval[0] = number;

    return (status);
}

/* Register these symbols for use by IOC code: */
epicsRegisterFunction(InitHexaStr2Dec);
epicsRegisterFunction(ProcHexaStr2Dec);
