#include <iostream>
#include <cantProceed.h>
#include <osiSock.h>
#include <epicsExit.h>
#include <epicsExport.h>

#include <ocrasynepics.h>

using namespace std;

extern "C" {
}


class OCRAsynDriver : public asynPortDriver
{

};
