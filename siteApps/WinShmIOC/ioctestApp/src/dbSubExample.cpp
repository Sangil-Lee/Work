#include <stdio.h>

#include <dbDefs.h>
#include <registryFunction.h>
#include <subRecord.h>
#include <aSubRecord.h>
#include <epicsExport.h>

#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/io.hpp>

#include <cstdlib> //std::system
#include <sstream>
#include <iostream>

using namespace boost::interprocess;
using std::vector;
//using namespace boost::numeric::ublas;

typedef allocator<float, managed_shared_memory::segment_manager>  ShmemAllocator;
typedef vector<float, ShmemAllocator> MyVector;

managed_shared_memory segment(open_only, "MySharedMemory");

int mySubDebug;

static long mySubInit(subRecord *precord)
{
    if (mySubDebug)
        printf("Record %s called mySubInit(%p)\n",
               precord->name, (void*) precord);
    return 0;
}

static long mySubProcess(subRecord *precord)
{
    if (mySubDebug)
        printf("Record %s called mySubProcess(%p)\n",
               precord->name, (void*) precord);
    return 0;
}

static long myAsubInit(aSubRecord *precord)
{
    if (mySubDebug)
        printf("Record %s called myAsubInit(%p)\n",
               precord->name, (void*) precord);
    return 0;
}
static long myAsubProcess(aSubRecord *precord)
{
    if (mySubDebug)
        printf("Record %s called myAsubProcess(%p)\n",
               precord->name, (void*) precord);

    //Find the vector using the c-string name
    MyVector *myvector = segment.find<MyVector>("MyVector").first;
    myvector->resize(precord->nova);

    float *outval = (float*)precord->vala;
    for(size_t i = 0; i < myvector->size(); ++i)  //Insert data in the vector
    {
       //std::cout <<"Shared Mem Read:" << myvector->at(i) << std::endl;
       outval[i] = myvector->at(i);
    } 
    return 0;
}

/* Register these symbols for use by IOC code: */

epicsExportAddress(int, mySubDebug);
epicsRegisterFunction(mySubInit);
epicsRegisterFunction(mySubProcess);
epicsRegisterFunction(myAsubInit);
epicsRegisterFunction(myAsubProcess);
