/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
 */

/**
 * @author mrk
 * @date 2013.07.24
 */
#ifndef EXAMPLEDATABASE_H
#define EXAMPLEDATABASE_H

#include <pv/pvDatabase.h>

#include <shareLib.h>

namespace epics { namespace pvarecord { namespace database {

class epicsShareClass  PVARecord{
public:
    static void create();
};


}}}

#endif  /* EXAMPLEDATABASE_H */
