/* mitiMain.cpp */
/* Author:  Marty Kraimer Date:    17MAR2000 */

#include "intech_global.h"

int main(int argc,char *argv[])
{
    if(argc>=2) {    
        iocsh(argv[1]);
        epicsThreadSleep(.2);
    }
    iocsh(NULL);
    epicsExit(0);
    return(0);
}
