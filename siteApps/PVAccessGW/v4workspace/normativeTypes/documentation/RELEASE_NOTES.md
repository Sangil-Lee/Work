Release 5.0
===========

This release adds support through wrapper classes and builders for the
remaining Normative Types:

* NTEnum
* NTMatrix
* NTURI
* NTAttribute
* NTContinuum
* NTHistogram
* NTAggregate
* NTUnion
* NTScalarMultiChannel

Release 5.0 therefore implements fully the
[16 Mar 2015 version](http://epics-pvdata.sourceforge.net/alpha/normativeTypes/normativeTypes_20150316.html)
 of the normativeTypes specification.

Each wrapper class has an extended API:

* is_a now has a convenience overload taking a PVStructure.
* isCompatible, reporting introspection type compatibility, now has an overload
  taking a Structure. The PVStructure version is retained as a convenience
  method and for backwards compatibility.
* An isValid function now reports validity of a compatible PVStructure's data
  with respect to the specification.

Other changes are:

* Support for NTAttributes extended as required by NTNDArray
  (NTNDArrayAttributes).
* A new class for parsing NT IDs (NTID).
* Resolution of the confusion between column names and labels in NTTable and
  improved API. Function for adding columns is now addColumn rather than add.
  New getColumnNames function provided.
* isConnected is treated as an optional rather than a required field in
  NTMultiChannelArray. isConnected() and addIsConnected() functions added to
  wrapper and builder respectively.
* Unit tests for all new classes.

Release 4.0
===========

This is the first release of normativeTypesCPP that is part of an official
EPICS V4 release.
It is a major rewrite of the previous versions of normativeTypesCPP.

This release provides support through wrapper classes and builders for the
following Normative Types:

* NTScalar
* NTScalarArray
* NTNameValue
* NTTable
* NTMultiChannel
* NTNDArray

Each type has a wrapper class of the same name which has functions for checking 
compatibility of existing PVStructures (isCompatible) and the reported types of 
Structures (is_a), wraps existing PVStructures (wrap, wrapUnsafe) and provides
a convenient interface to all required and optional fields.

Each type has a builder which can create a Structure, a PVStructure or a
wrapper around a new PVStructure. In each case optional or extra fields can be
added and options such as choice of scalar type can be made.


Additional features are:

* Utility classes NTField and NTPVField for standard structure fields and
  NTUtils for type IDs.
* Unit tests for the implemented classes.

