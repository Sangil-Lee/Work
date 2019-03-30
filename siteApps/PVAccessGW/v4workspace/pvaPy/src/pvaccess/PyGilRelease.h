// Copyright information and license terms for this software can be
// found in the file LICENSE that is included with the distribution

#ifndef PY_GIL_RELEASE_H
#define PY_GIL_RELEASE_H

#include "boost/python.hpp"

class PyGilRelease 
{
public:
    PyGilRelease();
    ~PyGilRelease();
private:
    PyThreadState* threadState;
};

inline PyGilRelease::PyGilRelease() :
    threadState(PyEval_SaveThread())
{
}

inline PyGilRelease::~PyGilRelease()
{
    PyEval_RestoreThread(threadState); 
    threadState = NULL; 
}

#endif // #ifndef PY_GIL_RELEASE_H
