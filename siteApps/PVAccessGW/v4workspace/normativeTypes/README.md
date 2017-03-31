normativeTypesCPP
=================

normativeTypesCPP is a C++ module containing helper classes which implement
and provide support for the EPICS V4 Normative Types.

The latter are a set of standard high-level data types to aid interoperability
of EPICS V4 applications and are specified in the
[NormativeTypes Specification](http://epics-pvdata.sourceforge.net/alpha/normativeTypes/normativeTypes.html).


Status
------

The current release (5.0) implements fully the
[16 Mar 2015 version](http://epics-pvdata.sourceforge.net/alpha/normativeTypes/normativeTypes_20150316.html)
of the Normative Types Specification.

The module status is alpha and the API and behaviour may change in future
versions.

The previous release (4.0) supported only 6 types. 
There is no documentation yet for the new types added in 5.0.


Further Info
------------

Consult the documents in the documentation directory, in particular

* normativeTypesCPP.html
* RELEASE_NOTES.md

Also see the [EPICS Version 4 website](http://epics-pvdata.sourceforge.net)


Prerequisites
-------------

normativeTypesCPP requires recent versions of the following software:

1. EPICS Base (v3.14.12.3 or later)
2. EPICS4 pvCommonCPP (4.1.0 or later)
2. pvDataCPP (5.0.0 or later)

(pvCommonCPP may not be needed depending on host/compiler.)


Building
--------

Building uses the make utility and the EPICS base build system.

The build system needs the location of the prerequisites, e.g. by placing the
lines of the form

    PVDATA = /home/install/epicsV4/pvDataCPP
    PVCOMMON = /home/install/epicsV4/pvCommonCPP
    EPICS_BASE = /home/install/epics/base

pointing to the locations in a file called RELEASE.local
in the configure directory or the parent directory of normativeTypesCPP.

With this in place, to build type make

    make

To perform a clean build type

    make clean uninstall

To run the unit tests type

    make runtests

For more information on the EPICS build system consult the
[Application Development guide](http://www.aps.anl.gov/epics/base/R3-14/12-docs/AppDevGuide.pdf).

