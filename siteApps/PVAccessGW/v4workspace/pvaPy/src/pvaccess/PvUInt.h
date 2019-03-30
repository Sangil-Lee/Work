// Copyright information and license terms for this software can be
// found in the file LICENSE that is included with the distribution

#ifndef PV_UINT_H
#define PV_UINT_H

#include "pv/pvData.h"
#include "PvScalar.h"

class PvUInt : public PvScalar
{
public:
    PvUInt();
    PvUInt(unsigned int ui);
    virtual ~PvUInt();

    void set(unsigned int ui);
    unsigned int get() const;
private:
    static boost::python::dict createStructureDict();
};

#endif
