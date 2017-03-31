#ifndef _requireNetSNMPversion_h_
#define _requireNetSNMPversion_h_

/*
   We need the numeric devSnmp_NETSNMP_VERSION defined to tell older NET-SNMP
   versions from newer where conditional compilation is needed.

   Unfortunately NET-SNMP (at least up through 5.7.1) only seems to provides a
   define for PACKAGE_VERSION, which is a string.

   We should get this define as a command line -D argument that our Makefile
   got from getNetSNMPversion.pl.  We need to flag an error if it is missing.
*/
#ifndef devSnmp_NETSNMP_VERSION
  #error devSnmp_NETSNMP_VERSION not defined (should have been via getNetSNMPversion.pl)
#endif

#endif  /* _requireNetSNMPversion_h_ */
