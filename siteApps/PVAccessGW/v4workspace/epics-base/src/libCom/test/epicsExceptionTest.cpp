/*************************************************************************\
* Copyright (c) 2006 UChicago Argonne LLC, as Operator of Argonne
*     National Laboratory.
* Copyright (c) 2002 The Regents of the University of California, as
*     Operator of Los Alamos National Laboratory.
* EPICS BASE is distributed subject to a Software License Agreement found
* in file LICENSE that is included with this distribution.
\*************************************************************************/

//
// Verify that the local c++ exception mechanism matches the ANSI/ISO standard.
// Author: Jeff Hill
//

#include <new>
#include <iostream>
#include <cstdio>
#if defined(__GNUC__) && (__GNUC__<2 || (__GNUC__==2 && __GNUC_MINOR__<=96))
#include <climits>
#else
#include <limits>
#endif

#include "epicsUnitTest.h"
#include "epicsThread.h"
#include "testMain.h"

using namespace std;

#if defined ( _MSC_VER )
    // some interesting bugs found in the MS implementation of new
#   if _MSC_VER >= 1900
        static size_t unsuccessfulNewSize = numeric_limits < size_t > :: max ();
#   elif _MSC_VER > 1310  /* this gets fixed some release after visual studio 7 we hope */
        static const size_t unsuccessfulNewSize = numeric_limits < size_t > :: max ();
#   else
        static const size_t unsuccessfulNewSize = numeric_limits < size_t > :: max () - 100;
#   endif
    // passing a size_t to printf() needs "%zu" on some platforms
#elif defined(vxWorks)
    // Neither vxWorks 5 or 6 supply true ANSI C++
    static const size_t unsuccessfulNewSize = UINT_MAX - 15u;
#else
    static const size_t unsuccessfulNewSize = numeric_limits < size_t > :: max ();
#endif

class exThread : public epicsThreadRunable {
public:
    exThread ();
    void waitForCompletion ();
    ~exThread() {};
private:
    epicsThread thread;
    bool done;
    void run ();
};

static void epicsExceptionTestPrivate ()
{
    try {
        char * p = new char [unsuccessfulNewSize];
        testFail("new char[%lu] returned %p",
            (unsigned long) unsuccessfulNewSize, p);
    }
    catch ( const bad_alloc & ) {
        testPass("new char[%lu] threw",
            (unsigned long) unsuccessfulNewSize);
    }
    catch ( ... ) {
        testFail("new: threw wrong type");
    }
    try {
        char * p = new ( nothrow ) 
            char [unsuccessfulNewSize];
        testOk(p == 0, "new (nothrow) returned %p", p);
    }
    catch( ... ) {
        testFail("new (nothrow): threw");
    }
}

exThread::exThread () :
    thread ( *this, "testExceptions", epicsThreadGetStackSize(epicsThreadStackSmall) ),
        done ( false )
{
    this->thread.start ();
}

void exThread::run ()
{
    epicsExceptionTestPrivate ();
    this->done = true;
}

void exThread::waitForCompletion ()
{
    while ( ! this->done ) {
        epicsThreadSleep ( 0.1 );
    }
}

MAIN(epicsExceptionTest)
{
    testPlan(4);
    epicsExceptionTestPrivate ();
    
    exThread athread;
    athread.waitForCompletion ();
    return testDone();
}
