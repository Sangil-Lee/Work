/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/**
 * @author mrk
 */
#ifndef DBPVDEBUG_H
#define DBPVDEBUG_H

namespace epics { namespace pvaSrv { 

class DbPvDebug {
public:
    static void setLevel(int level);
    static int getLevel();
};

}}

#endif  /* DBPVDEBUG_H */
