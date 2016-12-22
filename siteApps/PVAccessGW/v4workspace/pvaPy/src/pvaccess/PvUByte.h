// Copyright information and license terms for this software can be
// found in the file LICENSE that is included with the distribution

#ifndef PV_UBYTE_H
#define PV_UBYTE_H

#include "pv/pvData.h"
#include "PvScalar.h"

class PvUByte : public PvScalar
{
public:
    PvUByte();
    PvUByte(unsigned char uc);
    virtual ~PvUByte();

    void set(unsigned char uc);
    unsigned char get() const;
private:
    static boost::python::dict createStructureDict();
};

#endif
