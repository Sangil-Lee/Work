#include <cstdio>
#include <cstring>
#include <iostream>
#include <dbDefs.h>
#include <registryFunction.h>
#include <aSubRecord.h>
#include <epicsExport.h>

using namespace std;

static long subInitCheckDisk(aSubRecord *precord)
{
    return 0;
}

static long subProcCheckDisk(aSubRecord *precord)
{
    char cmd[]="df -k --output=used,avail /home/ctrluser/elasticsearch/data | tail -n 1" ;
    FILE* apipe = popen(cmd, "r");
    // if the popen succeeds read the commands output into the program with
    char line[132];
    unsigned long *a = (unsigned long*)precord->vala;
    unsigned long *b = (unsigned long*)precord->valb;
    while (  fgets( line, 132 , apipe) )
    {
       //cout << line <<endl;
    	int index = 0;
        char *ptr = strtok(line, " ");
        while(ptr != NULL)
        {
	    switch (index)
	    {
		case 0:
			a[0] = stol(ptr);
			//printf("%lu\n", *a);
			break;
		case 1:
			ptr[strlen(ptr)-1]='\0';
			b[0] = stol(ptr);
			//printf("%lu\n",*b);
			break;
	    };
            ptr = strtok(NULL, " ");
	    index++;
        };
    }
    pclose(apipe);

    return 0;
}

/* Register these symbols for use by IOC code: */
epicsRegisterFunction(subInitCheckDisk);
epicsRegisterFunction(subProcCheckDisk);
