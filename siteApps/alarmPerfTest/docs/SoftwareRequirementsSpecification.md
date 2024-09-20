# EPICS IOC __ Software Requirements Specification

-------- snip snip -------

This is the SRS template MD file. 

This document is based on the IEEE Recommended Practice for Software Requirements Specifications (IEEE Std 830-1998(R2009)) and Systems and software engineering Life cycle processes - Requirements engineering (ISO/IEC/IEEE 29148:2018(E)).

Not all contents are unnecessary to define due to the ALS-U Software Management Plan. However, please prepare this document first before writing an EPICS IOC.

-------- snip snip ------

Author: 

Status:

##### Table of Contents  
[Introduction](#introduction)  

[Overall description](#overalldescription)  

[Specific requirements](#specificrequirements)  

[Appendixes](#appendixes)

[Index](#index)


<a name="introduction"/>

## Introduction

### Purpose

* the purpose of the SRS
* the intended audience for the SRS

### Scope

* Identify the EPICS IOC Application Name;
* Explain what the EPICS IOC will, and if necessary, will not do;
* Describe the EPICS IOC being specificed, including objectives and goals.

### Definitions, acronyms, and abbreviations

This subsection should provide the definitions of all terms, acronyms, and abbreviations required to properly interpret the SRT.

### References

### Overview



<a name="overalldescription"/>

## Overall Description

### Production perspective

### Product functions

### User characteristics

### Constraints

### Assumptions and dependencies



<a name="specificrequirements"/>

## Specific requirements

### External interface requirements

#### User interfaces

#### Hardware interfaces

* Cable
* Pinout
* Serial settings

#### Software interface: EPICS base 

* EPICS Base: _/home/ctrluser/epics/debian-11/7.0.7/base_
** Source: source code url or ALS-U EPICS environment

#### Software interface: Dependent Modules

* asyn:
** Source:

* StreamDevice: 

#### Software interface: Coexist Modules

* iocStats:

#### Communications interfaces

This should specify the various interfaces to communications such as local network protocoles, etc.

#### Memory constraints

Specify any applicable characteristics and limits on system memories

#### Operations

Specify the normal and special operations required by the user such as:

1) the various modes of operations according to user types
2) periods of interactive operations and periods of unattneded operations
3) data processing support functions
4) recovery operations

#### Site adaptation requirements


#### Interfaces with services

Specify interactions with Controls Services

* Channel Finder
* Archiver Appliance
* RecSync
* .....

### Functional requirements

#### User class 1 (Controls Engineer/Scientist)

* Functional requirement 1
* Functional requirement 2
* Functional requirement 3

#### User class 2 (Technical subsystem Engineer/Scientist)

* Functional requirement 1
* Functional requirement 2

#### User class 3 (Accelerator operator)

* Functional requirement 1
* Functional requirement 2

### Performance requirements

### Design constraints

### Software system attributes

Specify the required attributes of the software product.

a) Reliability - specify the factors required to establish the required reliability level for the software system at teh time of production release
b) Availability - specify the factors required to guarantee a defined availability level for the entire system
c) Security - specify the requirements to protect the software from accidental or malicious access, use modification, destruction or disclosure.


### Other requirements



<a name="Appendixes"/>

## Appendixes

<a name="index"/>

