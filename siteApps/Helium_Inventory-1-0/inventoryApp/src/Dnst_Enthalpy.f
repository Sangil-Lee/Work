C   PROGRAM Dnst
C This is a program to calculate the density of GHe in the cryogenic blower.
C The following assignments are required.
C Code, 3 = Density, 9 = Enthalpy
C      Real*16 function Dnst_Enthalpy(Press, Temp)
      Real*8 function Dnst_Enthalpy(Code, Press, Temp)
	INTEGER Code
	REAL*8 Press, Temp 
	REAL*8 VALU1, VALU2, PROP(0:41,0:2)
      CHARACTER MESSAG*60
      INTEGER IDID, NUNITS, NPRICS, JIN1, JIN2, JOUT

C    
C-------------------------------------- OPEN AN OUTPUT FILE
C Open a file for receiving output in this program.
C
C      OPEN (UNIT=1, FILE='USER.OUT')
C
C    
C-------------------------------------- ACCURACY & PRECISION
C
C ACCURACY:
C Accuracy relates to the difference between HEPAKs
C calculated value and the correctly measured value of a
C thermodynamic variable at a given state point.  Residual
C errors or uncertainties in the fitted experimental data are
C often a major consideration in estimating the accuracy of
C the HEPAK equation.  Accuracies much better than one percent
C are not common, and when the fluid state is near the critical
C point, accuracies are generally much lower.  The user should
C consult the HEPAK manual for additional information.
C
C PRECISION:
C Almost all HEPAK calculations involve numerical iteration.
C Criteria for terminating the iteration must balance
C computer calculation time against precision of the results.
C If the computer prints out a pressure (or other parameter)
C of 4.999 when you had specified an input value 5.000 for
C this same parameter, low iterative precision is the problem.
C You may select from three precision criteria:
C     Modest precision, fast calculations  ........  (1)
C     Normal precision and speed           ........  (2)
C     High precision, slow calculations    ........  (3)
C
      NPRCIS = 2
C
C
C-------------------------------------- UNITS SELECTION
C
C Select any one of 4 units sets for both input and output variables
C     Choices are:  (1) = SI units
C                   (2) = Mixed SI and cgs units
C                   (3) = Mixed SI and molar units
C                   (4) = Engineering units (psi, R, lb, ft, etc.)
C
C     In this example, mixed SI-cgs units are selected.
C
      NUNITS = 2
C
C     The unit of the quality shall be non-dimension
C     The unit of the pressure shall be "kPa"
C     The unit of the temperature shall be "K"
C     The unit of the density shall be "g/L" 
C
C-------------------------------------- DEFINE PARAMETERS TO BE CALCULATED
C
C Output from the calculation is an index IDID and an array PROP,
C which contains 41 different thermodynamic parameters. 
C Pressure, temperature, density, volume, quality and PV/RT are always 
C calculated.
C So JOUT is 00000.
C
      JOUT = 10000
C
C-------------------------------------- INPUT PARAMETER IDENTIFICATION
C 
C Two input variables are required to locate a thermodynamic state point.
C     They are specified by: JIN1, VALU1, JIN2, VALU2. 
C JIN1 is an input variable for the temperature of GHe in the cryogenic blower.
C JIN2 is an input variable for the pressure of GHe in the cryogenic blowers.

C ------------------------------------- INPUT DATA FOR TEST
C
C Variables can be in either order.

C         JIN1 = 2
C         VALU1= 300.
C         JIN2 = 1
C         VALU2= 100.

C ------------------------------------- INPUT DATA
C
         IF (Temp .LT. 0) THEN
            JIN1 = 13
            JIN2 = 13
            IDID = 4
        ELSE
            JIN1 = 2
            JIN2 = 1
        ENDIF

         VALU1= Temp
C Pressure unit shall be changed from "barg" to "kPa" 
C
         VALU2= (Press)*100
C         IDID = 1
C         PROP = (9,0)
C
C-------------------------------------- PERFORM THE CALCULATIONS
C
C Calculations are performed by CRYODATA's thermodynamic code.
C Subroutine CALC orchestrates the calculation:
C
      CALL CALC (IDID, PROP, JOUT, JIN1, VALU1, JIN2, VALU2,
     +           NPRCIS, NUNITS)
C
C The variables JOUT, JIN1, VALU1, JIN2, VALU2, NUNITS, and
C NPRCIS are input parameters, unchanged by the call to CALC.
C The calling program can "reuse" them in subsequent operations
C if desired.
C
C-------------------------------------- TEST FOR SUCCESS OR FAILURE
C
C First, test for success or failure of the calculation.
C If IDID < 0, the calculation failed.  
C When IDID < -200, the reason for the failure is ambiguous.
C When the input datum is near the limit of the range of validity,
C it is possible to fail when HEPAK's starting guess for the iterative
C routines is too far from the correct value.  
C
C-------------------------------------- LOCATION OF OUTPUT VARIABLES
C
C All numerical results are returned in the 2-dimensioned array PROP(i,j)
C     and the integer parameter IDID.
C The first dimension (i) of PROP runs from 0 to 41, and specifies a
C    unique thermodynamic property, tabulated below.  
C The second dimension (j) of PROP runs from 0 to 2, and specifies a
C    single phase fluid or liquid-vapor mixture, saturated liquid, or
C    saturated vapor, also tabulated below.
C
C Note: PROP(3,j) is the density of GHe in the buffer tanks. 
C
C-------- Fluid phase information associated with PROP (i,j) and IDID
C
C IDID      ------ location of output variables -------
C    1      single phase fluid in PROP(i,0) only
C    2      sat liquid and vapor in PROP(i,1) and PROP(i,2) only; 
C    3      same as IDID=2 + mixture properties in PROP(i,0)
C    4      sat liquid in PROP(i,1) only (JINn=13, see below);
C    5      sat vapor  in PROP(i,2) only (JINn=14, see below);
C    <0     indicates that an error has occurred, results are invalid.
C
C Thus, on output:
C
C PROP (i,0) refers to single phase fluid,   when IDID = 1
C                      liquid-vapor mixtures when IDID = 3
C            will be zero for other values of IDID
C PROP (i,1) refers to saturated liquid when IDID = 2, 3, or 4
C            will be zero for other values of IDID
C PROP (i,2) refers to saturated vapor when IDID = 2, 3, or 5
C            will be zero for other values of IDID
C
C------------------------------------- OUTPUT PARAMETER
C
C Note: PROP(3,j) is the density of GHe in the cryogenic blower. 
C The unit is "Kg/m3".
C
C------------------------------------- OUTPUT DATA					   
C
C

C      IF (IDID .GE.1)  THEN
       IF (IDID .EQ. 1)  THEN
C
C If the calculation was finished successfully,
C the output data is inputted into the result variable, "Dnst".
C
C	   silee modified, avoiding for compile link(function naming conflict)
C	   Dnst = PROP(3,0) 
C	   PRINT *,'In Fortran:','Code:',Code,'Press:',Press,'Temp:',Temp
	   Dnst_Enthalpy = PROP(Code,0)
C       CALL ERRMSG (MESSAG, IDID)
C	   PRINT *,'Enthalpy:',Dnst_Enthalpy,'MSG',MESSAG
C
C Print selected results after a successful calculation for test. 
C 
C         WRITE (1, 10) Dnst_Enthalpy, IDID
      ELSE IF (IDID .EQ. 4) THEN
C Liquid saturation 
	   Dnst_Enthalpy = PROP(Code,1)

      ELSE
C
C  Otherwise print error message for test.
C 
C         CALL ERRMSG (MESSAG, IDID)
C          WRITE (1, '(1X,A60)') MESSAG
C
C If the calculation was failed, 
C the Dnst will be zero for EPICS to notice the failure.
C
C	   Dnst = 0
C	   silee modified, avoiding for compile link(function naming conflict)
	   Dnst_Enthalpy = 0
      ENDIF

C      CLOSE (1, STATUS='KEEP')

C
C------------------------------------- END THE PROGRAM 
C  10  FORMAT (G13.7, I4)
      END

C
C------------------------------------- HePAK SUBROUTINE

      SUBROUTINE CALC (IDID, PRP, JOUT, J1, VALU1, J2, VALU2,
     +      JPRECS, JUNITS)
* (C) Copyright (1992), Cryodata Inc.; all rights reserved.
C-----Version 3.40, Nov. 26, 1996
C
C Please update the Heprop version #, in block data HEINIT, as new
C     versions are developed!
*
* This subroutine performs all the calculations.
* !! See file USER.FOR for properties definitions and locations !!
* -------------------------------- AFTER ANY SUCCESSFUL CALL TO CALC
* the following properties will have been calculated:
*    PRP (0:5) + (if IDID > 1) PRP(6:7)
* Additional output is controlled by the 5-digit parameter JOUT.
C Each non-zero digit specifies additional parameters.
*   If the 10000's digit > 0:   PRP(8:13)  = State Variables
*   If the 1000's digit > 0 :   PRP(14:24) = Derivatives
*   If the 100's digit > 0  :   PRP(25:26) = Thermal conductivity and
C                               Viscosity and (if IDID > 1) PRP(29) =
C                               surface tension.
C                               Also, if Derivatives have been calculated,
C                               PRP(27:28) = Diffusivity & Prandtl No.
C                               will be calculated.
*   If the 10's digit > 0   :   PRP(30:31) = Dielectric constant and
C                               refractive index.
*       Note! PRP(40,0) = wavelength (micrometers) is a required input for
*       the refractive index calculation!
*   If the 1's digit > 0    :  PRP(8:24) + PRP(32:39) =  HeII properties
C       State and derivative properties will automatically be calculated
C       when HeII properties are requested.
* ------------------------------------------------------
      IMPLICIT DOUBLE PRECISION (A-H, O-Z)
      DOUBLE PRECISION LAMVAR
C !! Caution: NIV, NIP, NCF must agree with values in other subroutines !!
      PARAMETER (NIV=10, NIP=15, NCF=76)
*
      CHARACTER   FLNAME*32, LBL1*1
      DIMENSION PRP(0:41, 0:2), AIN(NIV), KIN(NIV)
      COMMON /SUBLAM/ LAMVAR(4)
      COMMON /FLNAM/  FLNAME
      COMMON /PRINIT/ COMINI(31)
      COMMON /MC1/    GMC1(34), VP(9), GMC2(2), PTP, TCC, TTP, GMC3(4)
      COMMON /UCONVF/ CONVF (NCF)
      COMMON /UUNITS/ KUNITS(0:40)
      COMMON /PRECSN/ E(0:15)
C-----Version 3.4 references to UIDLOK and UIDTF added, following 2 lines
      LOGICAL LATCH, UIDTF
      COMMON /UIDLOK/ UIDTF
      SAVE LATCH, TCRIT, PLAML, TLAML, DLAML
*-----Initialization of constants:
      DATA DLAMH, PLAMH, TLAMH /179.83, 3013400., 1.7673/
      DATA LATCH /.TRUE./
      DATA KIN /1, 2, 3, 4, 8, 9, 11, 12, 5, 33/
      IF (LATCH) THEN
         LATCH = .FALSE.
C-----Version 3.4, following line          
         IF (.NOT. UIDTF) CALL HEUID (KDID)
         LAMVAR( 4) = -1.
         TCRIT= COMINI(4)
         PLAML= COMINI(9)
         TLAML= COMINI(10)
         DLAML= COMINI(11)
      ENDIF
C-----Version 3.4, add the following if block          
      IF (.NOT. UIDTF) THEN
         IDID = -1
         RETURN
      ENDIF
*
* Check for valid input parameters
*
      IF (((J1-1)*(J1-NIP).GT.0).OR.((J2-1)*(J2-NIP).GT.0)) THEN
         IDID = -11
         RETURN
      ENDIF
*
* Check for valid thermodynamic parameter pair
*
      M = JM (J1, J2)
      IF (M .LE. 0) THEN
         IDID = -21
         RETURN
      ENDIF
*
* Reset precision and units as needed
*
      IF (JPRECS .NE. NINT(E(0))) CALL PRCIS (JPRECS)
      IF (JUNITS .NE. KUNITS(40)) CALL UNILIB (JUNITS)
C
C Place input values into the array and convert to HEPAK units
C (Note: input temperatures must be K or R, not C or F)
C
      IF (J1 .LE. NIV) AIN(J1) = VALU1 * CONVF (KUNITS(KIN(J1)))
      IF (J2 .LE. NIV) AIN(J2) = VALU2 * CONVF (KUNITS(KIN(J2)))
C
C-----Version 3.21B: Add one line for use by spreadsheets.
      IF (PRP (41,2) .EQ. -262144.D0) GO TO 200
C
*
C If Volume was an input parameter, calculate density.
*
      IF (J1 .EQ. 4) THEN
         IF (VALU1 .LE. 0.D0) THEN
            IDID = -170
            RETURN
         ENDIF
         AIN(3) = 1.D0/VALU1
      ELSE IF (J2 .EQ. 4) THEN
         IF (VALU2 .LE. 0.D0) THEN
            IDID = -170
            RETURN
         ENDIF
         AIN(3) = 1.D0/VALU2
      ENDIF
*
* Set the output array to signify "not a valid answer"
*     Properties 40 & 41 are not used for output (in this version),
*     but may be used for various internal communications, or
*     for input.  Do not erase.
      DO 404 K1 = 0, 39
         PRP (K1,0) = 0.D0
         PRP (K1,1) = 0.D0
         PRP (K1,2) = 0.D0
404   CONTINUE
      IDID = 0
*-----Obtain temperature, density and quality from input parameters
      GO TO (101, 102, 103, 104, 105, 106, 107, 108, 109, 110,
     &       111, 112, 113, 114, 115, 116, 117, 118, 119, 120,
     &       121, 122, 123, 124, 125, 126, 1125) M
*-----Pressure, temperature input
  101 CALL DFPT (IDID, PRP(3,0), PRP(0,0), AIN(1), AIN(2))
      PRP(1,0) = AIN(1)
      PRP(2,0) = AIN(2)
      GO TO 200
*
*-----(D,T) Input
  102 CALL FDT (IDID, PRP(0,0), PRP(1,0), PRP(6,1), PRP(3,1),
     +      PRP(3,2), AIN(3), AIN(2))
      PRP(2,0) = AIN(2)
      PRP(3,0) = AIN(3)
      GO TO 200
*-----Pressure and density input
  103 CALL TFDP (IDID, PRP(2,0), PRP(0,0), PRP(3,1), PRP(3,2),
     +      AIN(3), AIN(1))
      IF (IDID .EQ. 3) CALL PSATFT (PRP(1,0), PRP(6,1), PRP(2,0))
      PRP(3,0) = AIN(3)
      GO TO 200
*-----Pressure and enthalpy input
  104 CALL DTFPX (IDID, PRP(3,0), PRP(2,0), PRP(0,0), PRP(3,1),
     +      PRP(3,2), DUM1, AIN(1), AIN(6), 'PH')
      IF (IDID .EQ. 3) CALL PSATFT (PRP(1,0), PRP(6,1), PRP(2,0))
      GOTO 200
*-----Density and enthalpy input
  105 CALL TFDY (IDID, PRP(2,0), PRP(0,0), PRP(1,0), PRP(6,1),
     +      PRP(3,1), PRP(3,2), AIN(3), AIN(6), 'H')
      PRP(3,0) = AIN(3)
      GOTO 200
*-----Pressure and entropy input
  106 CALL DTFPX (IDID, PRP(3,0), PRP(2,0), PRP(0,0), PRP(3,1),
     +      PRP(3,2), DUM1, AIN(1), AIN(5), 'PS')
      IF (IDID .EQ. 3) CALL PSATFT (PRP(1,0), PRP(6,1), PRP(2,0))
      GO TO 200
*-----Temperature and entropy input
  107 CALL DFST (IDID, PRP(3,0), PRP(0,0), PRP(3,1), PRP(3,2),
     +      PRP(1,0), PRP(6,1), AIN(5), AIN(2))
      PRP(2,0) = AIN(2)
      GO TO 200
*-----Density and entropy input
  108 CALL TFDY (IDID, PRP(2,0), PRP(0,0), PRP(1,0), PRP(6,1),
     +      PRP(3,1), PRP(3,2), AIN(3), AIN(5), 'S')
      PRP(3,0) = AIN(3)
      GO TO 200
*-----Enthalpy and entropy input
  109 CALL DTFHS (IDID, PRP(3,0), PRP(2,0), PRP(0,0), PRP(3,1),
     +      PRP(3,2), PRP(1,0), PRP(6,1), AIN(6), AIN(5))
      GO TO 200
*-----Pressure and internal energy input
  110 CALL DTFPX (IDID, PRP(3,0), PRP(2,0), PRP(0,0), PRP(3,1),
     +      PRP(3,2), DUM1, AIN(1), AIN(7), 'PU')
      IF (IDID .EQ. 3) CALL PSATFT (PRP(1,0), PRP(6,1), PRP(2,0))
      GO TO 200
*-----Density and internal energy input
  111 CALL TFDY (IDID, PRP(2,0), PRP(0,0), PRP(1,0), PRP(12,0),
     +      PRP(3,1), PRP(3,2), AIN(3), AIN(7), 'U')
      PRP(3,0) = AIN(3)
      GO TO 200
*-----Saturation pressure input, possibly quality and pressure
  112 AIN(2) = TSATFP (AIN(1))
      IF (AIN(2) .LT. 0.D0) THEN
         IDID = -121
         GO TO 200
      ENDIF
      GO TO 1135
*-----delta T to the lambda line along the saturation line
 1125 AIN(2) = AIN(10) + TLAML
*-----Saturation temperature input, possibly quality and temperature
  113 IF ((AIN(2) .LT. 0.7999) .OR. (AIN(2) .GT. TCRIT)) THEN
         IDID = -122
         GO TO 200
      ENDIF
 1135 PRP(2,0) = AIN(2)
      CALL PSATFT (PRP(1,0), PRP(6,1), AIN(2))
      PRP(3,1) = D2LFPT (PRP(1,0), AIN(2))
      PRP(3,2) = D2VFPT (PRP(1,0), AIN(2))
* If quality has been specified, check that it is between 0 and 1;
* Else with satL or satV specification, set quality = 0
* so that saturated superfluid properties will be calculated correctly.
      IF ((J1 .EQ. 9) .OR. (J2 .EQ. 9)) THEN
         IF ((AIN(9).LT.0.D0) .OR. (AIN(9).GT. 1.D0)) THEN
            IDID= -129
            GO TO 200
         ENDIF
         IDID = 3
      ELSE
         AIN(9) = 0.D0
         IDID = 2
      ENDIF
      PRP(3,0) = 1.D0/((1.D0-AIN(9))/PRP(3,1) + AIN(9)/PRP(3,2))
      PRP(0,0) = AIN(9)
      GO TO 200
*-----Saturation S, H, U, or G input
  114 IF ((J1 .EQ. 5) .OR. (J2 .EQ. 5)) THEN
         LBL1 = 'S'
         DUM1 = AIN(5)
      ELSE IF ((J1 .EQ. 6) .OR. (J2 .EQ. 6)) THEN
         LBL1 = 'H'
         DUM1 = AIN(6)
      ELSE IF ((J1 .EQ. 7) .OR. (J2 .EQ. 7)) THEN
         LBL1 = 'U'
         DUM1 = AIN(7)
      ELSE
         LBL1 = 'G'
         DUM1 = AIN(8)
      ENDIF
      CALL SATFY (IDID, PRP(0,0), PRP(1,0), PRP(6,1), PRP(2,0),
     +         PRP(3,1), PRP(3,2), DUM1, LBL1)
      GO TO 200
*-----Saturation density input (double-valued region excluded).
  115 CALL SATFD (IDID, PRP(0,0), PRP(1,0), PRP(6,1), PRP(2,0),
     $               PRP(3,1), PRP(3,2), AIN(3))
      PRP(3,0) = AIN(3)
      IF (IDID .GT. 0) IDID = 2
      GO TO 200
*-----Melting pressure input
  116 PRP(2,0) = TMFP (AIN(1))
      IF (PRP(2,0) .LE. 0.) THEN
         IDID = -131
      ELSE
         CALL DFPT (IDID, PRP(3,0), DUM1, AIN(1), PRP(2,0))
         PRP(0,0) = -1.
      ENDIF
      GO TO 200
*-----Melting temperature input
  117 IF ((AIN(2)-0.8)*(AIN(2)-14.05) .GT. 0.) THEN
         IDID = -132
      ELSE
         AIN(1) = PMFT (AIN(2))
         CALL DFPT (IDID, PRP(3,0), DUM1, AIN(1), AIN(2))
         PRP(1,0) = AIN(1)
         PRP(2,0) = AIN(2)
         PRP(0,0) = -1.
      ENDIF
      GO TO 200
*-----Pressure and Gibbs energy input
  118 CONTINUE
      CALL DTFPG (IDID, PRP(3,0), PRP(2,0), AIN(1), AIN(8))
      PRP(0,0) = -1.
      GO TO 200
*-----Temperature and Gibbs energy input
  119 CONTINUE
      CALL DFTG (IDID, PRP(3,0), AIN(2), AIN(8))
      PRP(2,0) = AIN(2)
      PRP(0,0) = -1.
      GO TO 200
*-----Delta-T to the lambda line at a given density
  120 CONTINUE
      IF (ABS(AIN(10)) .GE. 0.50D0) THEN
         IDID = -144
      ELSE IF ((AIN(3) .GT. DLAMH) .OR. (AIN(3) .LT. 145.56D0)) THEN
         IDID = -143
      ELSE
         PRP(1,0) = PRESSA (AIN(3), AIN(10))
         PRP(2,0) = LAMVAR(1) + AIN(10)
         PRP(3,0) = AIN(3)
* Quality will be adjusted following statement 230
         PRP(0,0) = -1.
         IDID = 1
* Possible out-or-range near the saturation line
         CALL PSATFT (DUM1, DUM2, PRP(2,0))
         IF (PRP(1,0) .LT. DUM1) IDID = -145
      ENDIF
      GO TO 200
*-----Delta-T to the lambda line at a given pressure
  121 CONTINUE
      IF (ABS(AIN(10)) .GE. 0.50D0) THEN
         IDID = -144
      ELSE IF ((AIN(1) .LT. 0.D0) .OR. (AIN(1) .GT. PLAMH)) THEN
         IDID = -141
      ELSE
         IDID = 1
         PRP(2,0) = TLFP (AIN(1)) + AIN(10)
         CALL DF2PT (IDID, PRP(3,0), AIN(1), PRP(2,0))
         PRP(1,0) = AIN(1)
         PRP(0,0) = -1.
      ENDIF
      GO TO 200
*---- LAMBDA line pressure input
  122 CONTINUE
      IF ((AIN(1).LT.PLAML-2.).OR.(AIN(1).GT.PLAMH*1.0001)) THEN
         IDID = -141
      ELSE
         PRP(1,0) = AIN(1)
         PRP(2,0) = TLFP (AIN(1))
         PRP(3,0) = DLFT (PRP(2,0))
         PRP(0,0) = -2.
         IDID = 1
      ENDIF
      GO TO 200
*-----LAMBDA line temperature input
  123 CONTINUE
      IF ((AIN(2).GT.TLAML+.0001).OR.(AIN(2).LT.TLAMH-.001)) THEN
         IDID = -142
      ELSE
         PRP(1,0) = PLFT (AIN(2))
         PRP(2,0) = AIN(2)
         PRP(3,0) = DLFT (AIN(2))
         PRP(0,0) = -2.
         IDID = 1
      ENDIF
      GO TO 200
*-----LAMBDA line density input
  124 CONTINUE
      IF ((AIN(3).LT.DLAML-0.1).OR.(AIN(3).GT.DLAMH+0.1)) THEN
         IDID = -143
      ELSE
         PRP(2,0) = TLFD (AIN(3))
         PRP(1,0) = PLFT (PRP(2,0))
         PRP(3,0) = AIN(3)
         PRP(0,0) = -2.
         IDID = 1
      ENDIF
      GO TO 200
*---- Lower LAMBDA point
  125 CONTINUE
      PRP(1,0)= PLAML
      PRP(2,0)= TLAML
      PRP(0,0)= 0.
      IDID = 2
      PRP(3,1)= DLAML
C     PRP(3,2)= D2VFPT (PRP(1,0), PRP(2,0))
      PRP(3,2)= 1.192378D0
* Following point is defined by the T76 temperature scale
      PRP(12,0)= 12407.9D0
      GO TO 200
*-----Upper LAMBDA point
  126 CONTINUE
      PRP(1,0) = PLAMH
      PRP(2,0) = TLAMH
      PRP(3,0) = DLAMH
      PRP(0,0) = -2.
      IDID = 1
*     GO TO 200
*-----All input goes through this point;  check for error.
C     Valid properties at this point are:
C     If IDID=1: density, temperature, and quality
C                = prp(3,0), prp(2,0), prp(0,0)
C                  (In HeII, prp(0,0) will be reset from -1 to -2 later)
C                If M >= 20, pressure = prp(1,0) has been calculated
C     If IDID=3: Mixture, liquid, vapor densities, temperature,
C                saturation pressure, mixture quality
C                = prp(3,0), prp(3,1), prp(3,2), prp(2,0),
C                  prp(1,0), prp(0,0)
C     If IDID=2: Liquid and Vapor densities, temperature, pressure,
C                = prp(3,1), prp(3,2), prp(2,0), prp(1,0)
C                  (note: quality = prp(0,0) will be set = 0)
 200  CONTINUE
      IF (IDID .GE. 1) THEN
         IF (PRP(2,0) .LT. 0.7999) THEN
            IDID = -161
         ELSE IF (PRP(2,0) .GT. 1500.1) THEN
            IDID = -162
         ELSE IF (M .LE. 11) THEN
* Check for valid density if not already checked
            IF (PRP(3,0) .GT. DENMAX (PRP(2,0))) THEN
               IDID = -163
            ELSE
               DUM2 = MAX (PRP(3,0), PRP(3,1))
               IF (DUM2 .LE. 0.) IDID = -164
            ENDIF
         ENDIF
      ENDIF
      IF (IDID .EQ. 0) IDID=-300
      IF (IDID .LE. 0) RETURN
*-----Calculate pressure if not already done.
      IF (IDID .EQ. 1) THEN
         IF (M .LT. 20) PRP(1,0) = PRESS (PRP(3,0), PRP(2,0))
      ELSE
C-----   In two-phase or mixture, transfer P and T to 2-phase locations
         PRP(1,1) = PRP(1,0)
         PRP(1,2) = PRP(1,0)
         PRP(2,1) = PRP(2,0)
         PRP(2,2) = PRP(2,0)
C-----Version 3.23: one line added
         PRP(6,0) = PRP(6,1)
         PRP(6,2) = PRP(6,1)
         PRP(0,1) = 0.D0
         PRP(0,2) = 1.D0
      ENDIF
C
*-----P, T, RHO, and X have been evaluated.
*     Also calculate specific volume, PV/RT, and latent heat if at saturation.
      IF (IDID .NE. 2) THEN
         PRP(4,0) = 1./PRP(3,0)
         PRP(5,0) = PRP(1,0) /(2077.226D0*PRP(2,0)*PRP(3,0))
      ENDIF
      IF (IDID .GE. 2) THEN
         PRP(5,1) = PRP(1,1) /(2077.226D0*PRP(2,1)*PRP(3,1))
         PRP(5,2) = PRP(1,2) /(2077.226D0*PRP(2,2)*PRP(3,2))
         PRP(4,1) = 1./PRP(3,1)
         PRP(4,2) = 1./PRP(3,2)
C-----Version 3.211:
C Clausius-Clapyron value in PRP(7,1) and PRP(7,2);  PRP(7,2) may be
C overwritten later with delH (if delH is calculated).
         PRP(7,1) = PRP(6,1)*PRP(2,0)*(PRP(4,2)-PRP(4,1))
         PRP(7,2) = PRP(7,1)
C-----Version 3.23: following line
         PRP(7,0) = PRP(7,1)
      ENDIF
      QAL = PRP(0,0)
      OMQ = 1.D0-QAL
*-----Set output mode
      IF (IDID .EQ. 2) THEN
         IF ((J1 .EQ. 13) .OR. (J2 .EQ. 13)) THEN
            IDID = 4
         ELSE IF ((J1 .EQ. 14) .OR. (J2 .EQ. 14)) THEN
            IDID = 5
         ENDIF
      ENDIF
      JO1 = JOUT/10000
      JO2 = MOD (JOUT, 10000)/1000
      JO3 = MOD (JOUT, 1000)/100
      JO4 = MOD (JOUT, 100)/10
      JO5 = MOD (JOUT, 10)
      IF (JO5 .GE. 1) THEN
         JO1=1
         JO2=1
      ENDIF
*-----OUTPUT
C-----Version 3.23: non-functional numbered CONTINUEs removed below.
*-----State properties
      IF (JO1 .GT. 0) THEN
         IF (IDID .GE. 2) THEN
            IF (IDID .NE. 5) CALL SHAUG (PRP(1,1))
            IF (IDID .NE. 4) CALL SHAUG (PRP(1,2))
C-----Version 3.23:
            IF (IDID .LE. 3) THEN
               PRP(7,0) = PRP(9,2) - PRP(9,1)
               PRP(7,2) = PRP(7,0)
            ENDIF
            PRP( 8,0) = QAL*PRP( 8,2) + OMQ*PRP( 8,1)
            PRP( 9,0) = QAL*PRP( 9,2) + OMQ*PRP( 9,1)
            PRP(10,0) = QAL*PRP(10,2) + OMQ*PRP(10,1)
            PRP(11,0) = QAL*PRP(11,2) + OMQ*PRP(11,1)
            PRP(12,0) = QAL*PRP(12,2) + OMQ*PRP(12,1)
         ELSE
            CALL SHAUG (PRP(1,0))
         ENDIF
      ENDIF
*-----Derivative properties
      IF (JO2 .GT. 0) THEN
         IF (IDID .GE. 2) THEN
            IF (IDID .NE. 5) CALL DERIV (PRP(14,1), PRP(3,1), PRP(2,1))
            IF (IDID .NE. 4) CALL DERIV (PRP(14,2), PRP(3,2), PRP(2,2))
         ELSE
            CALL DERIV (PRP(14,0), PRP(3,0), PRP(2,0))
         ENDIF
      ENDIF
*-----Transport properties
      IF (JO3 .GT. 0) THEN
         IF (IDID .GE. 2) THEN
            PRP(29,1) = STEN (PRP(2,1))
            PRP(29,2) = PRP(29,1)
            IF (IDID .NE. 5) THEN
               PRP(25,1) = VISCOS (PRP(3,1), PRP(2,1))
               PRP(26,1) = TCON (PRP(3,1), PRP(2,1))
               IF (PRP(26,1) .GT. 0.D0)
     +            PRP(27,1) = PRP(25,1) *  PRP(14,1) / PRP(26,1)
               IF (PRP(14,1) .GT. 0.D0)
     +            PRP(28,1) = PRP(26,1) / (PRP(3,1) * PRP(14,1))
            ENDIF
            IF (IDID .NE. 4) THEN
               PRP(25,2) = VISCOS (PRP(3,2), PRP(2,2))
               PRP(26,2) = TCON (PRP(3,2), PRP(2,2))
               IF (PRP(26,2) .GT. 0.D0)
     +            PRP(27,2) = PRP(25,2) *  PRP(14,2) / PRP(26,2)
               IF (PRP(14,2) .GT. 0.D0)
     +            PRP(28,2) = PRP(26,2) / (PRP(3,2) * PRP(14,2))
            ENDIF
         ELSE IF (PRP(1,0) .LT. 1.01D+08) THEN
* The transport functions are not fitted at pressures above 100 MPa.
            PRP(25,0) = VISCOS (PRP(3,0), PRP(2,0))
            PRP(26,0) = TCON (PRP(3,0), PRP(2,0))
            IF (PRP(26,0) .GT. 0.D0)
     +         PRP(27,0) = PRP(25,0) * PRP(14,0) / PRP(26,0)
            IF (PRP(14,0) .GT. 0.D0)
     +         PRP(28,0) = PRP(26,0) / (PRP(3,0) * PRP(14,0))
         ENDIF
      ENDIF
*-----Dielectric contant and/or refractive index
      IF (JO4 .GT. 0) THEN
C-----Version 3.23: following IF logic changed.
         WAVELN = PRP(40,0)
         IF ((IDID-1)*(IDID-3) .EQ. 0) THEN
            PRP(30,0) = DIELHE (PRP(3,0))
            PRP(31,0) = REFR (PRP(3,0), WAVELN)
         ENDIF
         IF ((IDID-2)*(IDID-4) .LE. 0) THEN
            PRP(30,1) = DIELHE (PRP(3,1))
            PRP(31,1) = REFR (PRP(3,1), WAVELN)
         ENDIF
         IF ((IDID-2)*(IDID-3)*(IDID-5) .EQ. 0) THEN
            PRP(30,2) = DIELHE (PRP(3,2))
            PRP(31,2) = REFR (PRP(3,2), WAVELN)
         ENDIF
      ENDIF
*-----Unique superfluid properties
* The array HII(1:5) is evaluated by SUPERF
C-----Version 3.23: minor change in Fortran; no functional change.
      IF ((JO5 .GT. 0) .AND. (IDID .LT. 5)) THEN
         J = MIN (1, (IDID-1))
C Proceed only if A-equations have been used.
         CALL AMLAP (TMINM, TMAXA, PRP(3,J))
         IF (PRP(2,J) .GE. TMAXA) GO TO 400
         PRP(39,J) = LAMVAR(1)
         PRP(33,J) = PRP(2,J) - LAMVAR(1)
         IF (ABS(PRP(33,J)) .LT. 1.D-06) PRP(33,J) = 0.D0
         PRP(32,J) = PRP(2,J) - TLFP (PRP(1,J))
         IF (ABS(PRP(32,J)) .LT. 1.D-06) PRP(32,J) = 0.D0
         IF (PRP(33,J) .LE. -1.D-06) THEN
            PRP(0,J) = -2.D0
C-----Version 3.23: accidently created an index error in following line
C-----Version 3.24: index error corrected: PRP(2,j) corrected to PRP(1,j)
            CALL SUPERF (PRP(34,J), PRP(1,J), PRP(3,J), PRP(2,J),
     +            PRP(33,J), PRP(8,J), PRP(14,J))
         ENDIF
      ENDIF
400   CONTINUE
*-----Units conversion.
      IF (KUNITS(40) .GT. 1) THEN
         DO 420 J = 1, 38
            CVFJ = CONVF(KUNITS(J))
            PRP(J,0) = PRP(J,0)/CVFJ
            PRP(J,1) = PRP(J,1)/CVFJ
            PRP(J,2) = PRP(J,2)/CVFJ
  420    CONTINUE
C Note: PRP(39,k), = Tlambda, remains in Kelvin regardless of units.
      ENDIF
      END
C
C
C
      INTEGER FUNCTION JM (J1, J2)
* (C) Copyright (1992), Cryodata Inc.; all rights reserved.
* Version 3.21, Jan. 5, 1992
*
* Symmetric matrix of response to input variables
      DIMENSION KM (15, 15)
*       PP  TT  DD  VV  SS  HH  UU  GG  XX  dt  MM   2  SL  SV  LL
      DATA KM /
     P   0,  1,  3,  3,  6,  4, 10, 18, 12, 21, 16, 12, 12, 12, 22,
     T   1,  0,  2,  2,  7,  0,  0, 19, 13,  0, 17, 13, 13, 13, 23,
     D   3,  2,  0,  0,  8,  5, 11,  0,  0, 20,  0, 15, 15, 15, 24,
     V   3,  2,  0,  0,  8,  5, 11,  0,  0, 20,  0, 15, 15, 15, 24,
     S   6,  7,  8,  8,  0,  9,  0,  0,  0,  0,  0, 14, 14, 14,  0,
     H   4,  0,  5,  5,  9,  0,  0,  0,  0,  0,  0, 14, 14, 14,  0,
     U  10,  0, 11, 11,  0,  0,  0,  0,  0,  0,  0, 14, 14, 14,  0,
     G  18, 19,  0,  0,  0,  0,  0,  0,  0,  0,  0, 14, 14, 14,  0,
     X  12, 13,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     t  21,  0, 20, 20,  0,  0,  0,  0,  0,  0,  0, 27, 27,  0,  0,
     M  16, 17,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 26,
     2  12, 13, 15, 15, 14, 14, 14, 14,  0, 27,  0,  0,  0,  0, 25,
     4  12, 13, 15, 15, 14, 14, 14, 14,  0, 27,  0,  0,  0,  0, 25,
     5  12, 13, 15, 15, 14, 14, 14, 14,  0,  0,  0,  0,  0,  0,  0,
     L  22, 23, 24, 24,  0,  0,  0,  0,  0,  0, 26, 25, 25,  0,  0/
      JM = KM (J1, J2)
      END
C
C
C
      DOUBLE PRECISION FUNCTION ROUN01 (X)
* (C) Copyright (1990), Cryodata Inc.; all rights reserved.
      IMPLICIT DOUBLE PRECISION (A-H, O-Z)
* Correction of roundoff errors on X in the range 0 to 1
      IF (X .LT. 3.D-05) THEN
         ROUN01 = 0.D0
      ELSE IF (X .GT. 0.9999D0) THEN
         ROUN01 = 1.D0
      ELSE
         ROUN01 = X
      ENDIF
      END
C
C
C
C-----Version 3.4, subroutine added
      SUBROUTINE HEUID (IDID)
C Copyright notice, ownership
C !Following NUID must not conflict with other file assignments!
      PARAMETER (NUID=17)
      CHARACTER*60 TEXT, BUFF
      LOGICAL UIDTF
      COMMON /UIDLOK/ UIDTF
      COMMON /UIDHE/  NLINES, TEXT(6)
C
C Temporarily disable UID checking
      IDID = 0
      UIDTF = .TRUE.
      RETURN
C
      OPEN (UNIT=NUID, FILE='HEPAK.UID', STATUS='OLD', 
     + ACCESS='DIRECT', FORM='UNFORMATTED', RECL=60, ERR=4)
      READ (NUID, ERR=4) NLINES
      READ (NUID, ERR=4) NSUM
      KSUM = 0
C Hepak.uid generation guarantees that NLINES will be 5 or less.
      DO 2 J = 1, NLINES
         READ (NUID, ERR=4) BUFF
         TEXT(J) = BUFF
         DO 1 K = 1, 32
            KSUM = KSUM + MOD (K*(ICHAR(BUFF(K:K))-32), 256)
    1    CONTINUE
    2 CONTINUE
      READ (NUID, ERR=4) BUFF
      DO 3 K = 1, 12
         KSUM = KSUM + MOD (K*(ICHAR(BUFF(K:K))-32), 256)
    3 CONTINUE
      IF (NSUM .NE. KSUM) GO TO 4
      IF (BUFF(1:5) .NE. 'HEPAK') GO TO 4
      CLOSE (NUID)
      IDID = 0
      UIDTF = .TRUE.
      RETURN
C
  4   CONTINUE
C Hepak user unidentified or invalid, program blocked.
      IDID = -1
      NLINES = 1
      TEXT(1) = 'Hepak user unidentified or invalid; program blocked.'
      UIDTF = .FALSE.
      RETURN  
      END
C
C
C
C-----Version 3.4, subroutine added
      SUBROUTINE UIDMSG (LINES, MESSAG)
      CHARACTER*60 MESSAG(5), TEXT
      COMMON /UIDHE/  NLINES, TEXT(6)
C NLINES is never more than 5      
      DO 10 K = 1, NLINES
         MESSAG(K) = TEXT(K)
   10 CONTINUE
      LINES = NLINES
      RETURN
      END
C
C
C      
      SUBROUTINE ERRMSG (MESSAG, IDID)
* (C) Copyright (1992), Cryodata Inc.; all rights reserved.
* Error messages, when the program returns with IDID < zero
C-----Version 3.23
C Input values of IDID:
C   -1        : unidentified or invalid user. (Version 3.4+) 
C  -10 TO  -99: external coding errors.  User should investigate.
C -100 to -199: input is outside of the valid EOS range.
C -200 to -299: unexpected iteration convergence failure.  Probably
C               due to input out of range (which should have been
C               caught elsewhere) or, possible programming error.
C               Cryodata should investigate.
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
C-----Version 3.4 Nlines increased by 1
      PARAMETER (NLINES=55, NLM1=NLINES-1)
      CHARACTER*60 LINE (NLINES), MESSAG
      DIMENSION JIDID(NLM1)
      DATA (LINE(J), J=1,19) /
     1' Invalid input parameter(s).                                ',
     2' Invalid combination of input parameters.                   ',
     3' Input Pressure <= zero.                                    ',
     4' Input Pressure too high; out of range.                     ',
     5' Input Temperature < 0.8 K; out of range.                   ',
     6' Input Temperature > 1500 K; out of range.                  ',
     7' Input Density <= zero.                                     ',
     8' Input Density is outside of valid range.                   ',
     9' Solid phase encountered.                                   ',
     A' Entropy out of range.                                      ',
     1' Enthalpy out of range.                                     ',
     2' Internal Energy out of range.                              ',
     3' Input Gibbs energy must be in liquid between 0.8 and 3.0 K.',
     4' Gibbs energy iteration requires 0.8 <= Temperature <=3.0 K.',
     5' Pressure out of range for Gibbs energy iteration.          ',
     6' Saturation pressure out of range (1.48 to 226463 Pa).      ',
     7' Saturation temperature out of range (0.8 to 5.1953 K).     ',
     8' Saturation density out of range (0.000888 to 146.16 Kg/m3).',
     9' Saturation entropy out of range (0.00471 to 23.936 J/g-K). '/
      DATA (LINE(J), J=20, 38) /
     B' Saturation enthalpy out of range (0.00187 to 30.744 J/g).  ',
     1' Saturation Helmholz E out of range (-.00191 to -11.287 J/g)',
     2' Saturation energy out of range (0.00186 to 24.724 J/g).    ',
     3' Saturation Gibbs E out of range (-0.00189 to -8.0212 J/g). ',
     4' Saturation quality must be between 0 and 1 (inclusive).    ',
     5' Melting pressure out of range (25.328 to 1013.25 bars)     ',
     6' Melting temperature > 14.0 K, pressure exceeds valid range.',
     7' Lambda pressure out of range (.050418 to 30.134 bars).     ',
     8' Lambda temperature out of range (2.1768 to 1.7673 K).      ',
     9' Lambda density out of range (146.15 to 179.83 Kg/m3).      ',
     C' On input, absolute value of delta-T must be < 0.5 K        ',
     1' Liquid-vapor mixture state point.                          ',
     2' Calculated temperature < 0.8 K                             ',
     3' Calculated temperature > 1500. K                           ',
     4' Calculated pressure > maximum valid pressure.              ',
     5' Calculated density <= zero.                                ',
     6' Input specific volume <= zero.                             ',
     7' Indeterminant function in compressed liquid.               ',
     8' Iteration failure with (P,T) input.  Out of range?         '/
      DATA (LINE(J), J=39, 55) /
     9' Iteration failure with (H,S) input.  Out of range?         ',
     D' Iteration failure with (P,G) input.  Out of range?         ',
     1' Iteration failure with (D,P) input; compressed liquid?     ',
     2' Failure with (D,S), (D,H) or (D,U) input.  Out of range?   ',
     3' Failure with (P,S), (P,H) or (P,U) input.  Out of range?   ',
     4' Unexpected iteration failure near the lambda line.         ',
     5' (C) Copyright (1992), Cryodata, Inc.; all rights reserved. ',
     6' Successful calculation: single phase fluid.                ',
     7' Successful calculation: sat. liquid and vapor.             ',
     8' Successful calculation: liquid-vapor mixture.              ',
     9' Successful calculation: sat. liquid only.                  ',
     E' Successful calculation: sat. vapor only.                   ',
     1' Invalid property or units or phase index in HEFUN.         ',
     2' HEPAK has not performed any calculations.                  ',
     3' Calculated state point was not in specified fluid phase.   ',
     4' Hepak user unidentified; program blocked.                  ',
     5' Cryodata program error: invalid IDID index.                '/
C-----Version 3.4 LINE(54) defined above. 
C-----Version 3.23
      DATA JIDID / - 11, - 12, -101, -102, -103, -104, -105, -106,
     A -107, -108, -109, -110, -111, -112, -113, -121, -122, -123,
     A -124, -125, -126, -127, -128, -129, -131, -132, -141, -142,
     C -143, -144, -145, -161, -162, -163, -164, -170, -180, -201,
     D -202, -203, -204, -205, -206, -207,  0,  1,  2,  3,  4,  5,
     E  -13, -20, -15, -1/
C-----Version 3.4 JIDID(54) = -1 added to above data statement.
C Note: HEPAK does not assign IDID values of -13, -15, or -20.
C However, function HEFUN can return these values.
      DO 414 J = 1, NLM1
         IF (JIDID (J) .EQ. IDID) THEN
            MESSAG = LINE (J)
            RETURN
         ENDIF
  414 CONTINUE
      MESSAG = LINE(NLINES)
      RETURN
      END
C
C
C
      SUBROUTINE PRCIS (J)
* (C) Copyright (1992), Cryodata Inc.; all rights reserved.
* Set precision for use in iterative routines.
      IMPLICIT DOUBLE PRECISION (A-H, O-Z)
      PARAMETER (NF=3, NE=15)
      DIMENSION A(0:NE), FACT(NF)
      COMMON/PRECSN/ E(0:NE)
      DATA A /0.D0, 3.D-2, 1.D-2, 3.D-3, 1.D-3, 3.D-4, 1.D-4, 3.D-5,
     +       1.D-5, 3.D-6, 1.D-6, 3.D-7, 1.D-7, 3.D-8, 1.D-8, 3.D-9/
      DATA FACT /20.D0, 0.5D0, 2.D-3/
      IF (J .GT. NF) THEN
         N = NF
      ELSE IF (J .LT. 1) THEN
         N = 1
      ELSE
         N = J
      ENDIF
      R    = FACT(N)
      E(0) = N
      DO 416 K = 1, NE
         E(K) = R*A(K)
  416 CONTINUE
      RETURN
      END
C
C
C
      BLOCK DATA HEINIT
* (C) Copyright (1992), Cryodata Inc.; all rights reserved.
* Replacement for subroutine PRDATA in earlier versions of HEPAK
C Sept. 13, 1992
      IMPLICIT DOUBLE PRECISION (A-H, O-Z)
      CHARACTER FLNAME*32
      COMMON/PRNAME/FLNAME
C     COMMON/PRINIT/R,AMW,PCRIT,TCRIT,DCRIT,HCRIT,UCRIT,SCRIT,
C    1   PTR,TTR,DTRF,DTRG,HTRF,HTRG,UTRF,UTRG,STRF,STRG,THI,DHI,
C    2   HHI,UHI,SHI,CPHI,CVHI,HMAX2P,UMAX2P,PMAX,PMIN,TMAX,TMIN
      COMMON /PRINIT/ AAA(31)
      COMMON/PRECSN/ E(0:15)
      COMMON /UUNITS/ KUNITS(0:40)
C-----Version 3.40 /UIDHE/ added, following 4 lines
      LOGICAL UIDTF
      CHARACTER*60 TEXT
      COMMON /UIDLOK/ UIDTF
      COMMON /UIDHE/  NLINES, TEXT(6)
C-----Version 3.23: two lines added (Heprop version #)
      COMMON /HEPRVR/ HEPRVS
      DATA HEPRVS /3.31D0/
      DATA FLNAME /'HELIUM                          '/
C-----Version 3.40 Uidtf defined, next line.
      DATA UIDTF /.FALSE./
      DATA NLINES /1/
      DATA TEXT /'Heprop is unitialized; user i.d. not known',
     + 5*'                                                   '/ 
C-----Version 3.21  HMAX2P and UMAX2P changed to "exact" values
      DATA AAA /
     A  2077.23476D0, 4.0026D0, 227462.3D0, 5.1953D0, 69.6412374D0,
     B  21932.D0,     18682.D0,    5768.6D0,   5041.8D0,  2.1768D0,
     C  146.15D0,  1.1923727D0,  2961.33D0,  25757.05D0, 2926.83D0,
     D  21528.67D0,  1579.97D0,   11991.95D0,    300.D0,  0.16040D0,
     E  1573.5D+3,  950.1D+3,    31.612D+3,    5193.D0,    3117.D0,
     F  30744.D0,   24724.D0,   20280.D+5,   0.1D0,       1500.D0,
     G  0.8D0/
      DATA E/0.D0, 3.D-2, 1.D-2, 3.D-3, 1.D-3, 3.D-4, 1.D-4, 3.D-5,
     +      1.D-5, 3.D-6, 1.D-6, 3.D-7, 1.D-7, 3.D-8, 1.D-8, 3.D-9/
      DATA KUNITS/
     A  1, 57, 12, 16, 20,  1, 69, 28, 24, 28, 28, 28, 28, 60, 24, 24,
     B  1, 65,  1, 53, 32, 36, 73, 69, 28, 58, 38,  1, 43, 46,  1,  1,
     C 12, 12,  1, 32, 32, 49, 50, 12,  1/
      END
C
C
C
      BLOCK DATA ACON
* (C) Copyright (1990), Cryodata Inc.; all rights reserved.
* This subroutine written by V. Arp.
*
      IMPLICIT DOUBLE PRECISION (A-H, O-Z)
      COMMON /SUBHEC/ C1(39),S01,A01, C2(39),S02,A02, CL(8,8), V0, T0
      DATA C1 /
     A -0.55524231D+00, 0.18333872D+00,-0.42819388D-01,-0.49962336D-01,
     B -0.83818656D-01,-0.14081863D+00, 0.89901300D+00, 0.66841845D+01,
     C  0.98899347D+01, 0.73876336D+01, 0.20130513D+01,-0.25251119D-01,
     D -0.10649342D+01,-0.35520547D+01,-0.58465160D+01,-0.42352097D+01,
     E -0.12206228D+01, 0.16905918D-01, 0.21835833D+00, 0.74548499D+00,
     F  0.11932777D+01, 0.86121784D+00, 0.25519882D+00,-0.13224602D-02,
     G -0.20161157D-01,-0.65799115D-01,-0.10330654D+00,-0.75388298D-01,
     H -0.23788871D-01, 0.52810077D-04, 0.66823412D-03, 0.21297216D-02,
     I  0.32541554D-02, 0.24421110D-02, 0.82971274D-03,-0.19221178D+01,
     J  0.11203003D+01,-0.16430199D+00,-0.34805651D-02/
      DATA C2 /        -0.14629809D+01, 0.76365652D+00,-0.11943389D+00,
     A  0.30525707D-02,-0.10405828D+00,-0.14748127D+00,-0.96308013D+00,
     B  0.67943869D+00,-0.55276320D+00, 0.19535989D-01,-0.47928840D-01,
     C  0.75410775D-01,-0.80609070D-01,-0.76641800D+00,-0.35260824D+01,
     D -0.51006607D+01,-0.20845765D+01,-0.12991643D-01,-0.14392344D-01,
     E  0.72407645D+00, 0.35487925D+01, 0.49508203D+01, 0.20014366D+01,
     F  0.12086022D-02, 0.10894370D-01,-0.21841979D+00,-0.10697343D+01,
     G -0.14541070D+01,-0.56844527D+00,-0.82701229D-04,-0.10295381D-02,
     H  0.20777040D-01, 0.10085164D+00, 0.13267276D+00, 0.49701540D-01,
     I  0.95829687D+00,-0.14025436D+01, 0.63551829D+00,-0.55978553D-01/
      DATA CL/1.,  0.,   0.,   0.,   0.,    0.,    0.,    0.,
     A        1., -1.,   0.,   0.,   0.,    0.,    0.,    0.,
     B        1., -2.,   2.,   0.,   0.,    0.,    0.,    0.,
     C        1., -3.,   6.,  -6.,   0.,    0.,    0.,    0.,
     D        1., -4.,  12., -24.,  24.,    0.,    0.,    0.,
     E        1., -5.,  20., -60., 120., -120.,    0.,    0.,
     F        1., -6.,  30.,-120., 360., -720.,  720.,    0.,
     G        1., -7.,  42.,-210., 840.,-2520., 5040.,-5040./
      DATA V0 ,T0 /6.842285D0, 2.1768D0/
      DATA S01,S02 / 3.63345D0, -.044009D0/
      DATA A01,A02 /-4.32500D0, -.787770D0/
      END
C
C
C
      BLOCK DATA HEMC
* (C) Copyright (1990), Cryodata Inc.; all rights reserved.
* This subroutine written by R.D. McCarty and V. Arp.
* Helium I coefficients; version March 14, 1989
      IMPLICIT DOUBLE PRECISION(A-H, O-Z)
      DIMENSION G(32),VP(9),GV(9),GT(9),FV(4),FT(4),EV(8),ET(8),A(20)
      COMMON /MC1/ G,R,GAMMA,VP,DTP,PCC,PTP,TCC,TTP,TUL,TLL,PUL,DCC
      COMMON /MC2/ EM,EOK,RM,TC,DC,X,PC,SIG
      COMMON /MC3/ A,DTPV,EG
      COMMON /MC4/ GI(11)
      COMMON /MC5/ BETA,XO,DELTA,E1,E2,AGAM
      COMMON /MC6/ GV,GT,FV,FT,EV,ET
      COMMON /MC8/ T0,S0,H0
      COMMON /MC9/ BX(6),PX(6)
      DATA SIG,XO,BETA,DELTA,E1,E2,AGAM /
     A .0D+00, .0D+00, .0D+00, .0D+00, .0D+00, .0D+00, .0D+00/
      DATA EM,EOK,RM,TC,DC,X,PC /
     A .400260D+01, .0D+00, .0D+00, .519530D+01, .173990D+02,
     B .0D+00, .227460D+00 /
      DATA G /               .4558980227431D-04,  .1260692007853D-02,
     A -.7139657549318D-02,  .9728903861441D-02, -.1589302471562D-01,
     B  .1454229259623D-05, -.4708238429298D-04,  .1132915223587D-02,
     C  .2410763742104D-02, -.5093547838381D-08,  .2699726927900D-05,
     D -.3954146691114D-04,  .1551961438127D-08,  .1050712335785D-07,
     E -.5501158366750D-07, -.1037673478521D-09,  .6446881346448D-12,
     F  .3298960057071D-10, -.3555585738784D-12, -.6885401367690D-02,
     G  .9166109232806D-02, -.6544314242937D-05, -.3315398880031D-04,
     H -.2067693644676D-07,  .3850153114958D-07, -.1399040626999D-10,
     I -.1888462892389D-11, -.4595138561035D-14,  .6872567403738D-14,
     J -.6097223119177D-18, -.7636186157005D-17,  .3848665703556D-17/
      DATA A/                -.5618978079000D+03,  .3300736785000D+01,
     B  -.6031200561000D+02,  .6129901560000D+03, -.2718577178000D+04,
     C   .1285185020000D+04, -.4406873907000D+03,  .7163145577000D+02,
     D   .0000000000000D+00,  .0000000000000D+00,  .0000000000000D+00,
     E   .0000000000000D+00,  .0000000000000D+00, -.3434500882000D+02,
     F   .4501470666000D+00, -.6895460170000D+01,  .5442223002000D+02,
     G  -.9716273774000D+02,  .1785061367000D+02,  .0000000000000D+00/
      DATA GI /  .00D+00, .00D+00, .00D+00, .25D+01, .00D+00,
     A  .00D+00, .00D+00, .00D+00, .25D+01, .00D+00, .00D+00/
      DATA VP /
     A  .1723358273506D+01, .2355572223663D+01, -.1899616867304D+00,
     B -.6145205348730D-01, .1394346356392D+01,  .1500000000000D+01,
     C  .2176800000000D+01, .5195300000000D+01,  .5041800000000D-02/
C Following line added Sept. 13, 1992
      DATA TTP, TCC, PTP /  .21768D+01, .51953D+01,  .50418D-02/
      DATA GV, GT  /9*0.D0, 9*0.D0/
      DATA EV, ET  /8*0.D0, 8*0.D0/
      DATA FV, FT  / 4*0.D0, 4*0.D0/
      DATA DTP,DTPV /.3651377D+02, .294058864D+00/
      DATA T0,S0,H0 / 0.D0,  .0078616D0, .0611315D0/
      DATA R, GAMMA, TUL, TLL, PUL, DCC, PCC /
     A .00831430, -.33033259E-02, 1500.00, 2.17, 100.00, 17.3990, .2275/
      DATA BX /6*0.0/, PX / .29D+02, .40D+01, .15D+01, 3*0.D0/
      END
C
C
C
      BLOCK DATA UULIB
* (C) Copyright (1992), Cryodata Inc.; all rights reserved.
      IMPLICIT DOUBLE PRECISION(A-H, O-Z)
      PARAMETER (NCF=76)
      CHARACTER*11 UNITS
      COMMON /UCONVF/ CONVF (NCF)
      COMMON /UNLABL/ UNITS (NCF)
C-----Version 3.22  SFTC units corrected
      DATA UNITS /
     1   ' [-]       ', ' [KPa]     ', ' [MPa]     ', ' [bar]     ',
     2   ' [atmos]   ', ' [psi]     ', ' [1/KPa]   ', ' [1/MPa]   ',
     3   ' [1/bar]   ', ' [1/atmos] ', ' [1/psi]   ', ' [K]       ',
     4   ' [R]       ', ' [1/K]     ', ' [1/F]     ', ' [Kg/m3]   ',
     5   ' [g/cm3]   ', ' [mol/L]   ', ' [lb/ft3]  ', ' [m3/Kg]   ',
     6   ' [cm3/g]   ', ' [L/mol]   ', ' [ft3/lb]  ', ' [J/Kg-K]  ',
     7   ' [J/g-K]   ', ' [J/mol-K] ', ' [BTU/lb-R]', ' [J/Kg]    ',
     8   ' [J/g]     ', ' [J/mol]   ', ' [BTU/lb]  ', ' [m/s]     ',
     9   ' [ft/s]    ', ' [K/KPa]   ', ' [K/bar]   ', ' [K/Pa]    ',
     A   ' [R/psi]   ', ' [W/m-K]   ', ' BTU/hrftR ', ' [uPa-s]   ',
     1   ' [upoise]  ', ' [lbm/ft-s]', ' [m2/s]    ', ' [cm2/s]   ',
     2   ' [ft2/s]   ', ' [N/m]     ', ' [dyne/cm] ', ' [lbF/ft]  ',
     3   ' [m-s/Kg]  ', ' [W3/m5-K] ', ' [cm-s/g]  ', ' [W3/cm5-K]',
     4   ' [1/Pa]    ', ' [V/Vc]    ', ' [F]       ', ' [C]       ',
     5   ' [Pa]      ', ' [Pa-s]    ', ' [P/Pc]    ', ' [T/Tc]    ',
     6   ' [D/Dc]    ', ' [TdP/PdT] ', ' [../R]    ', ' [../RTc]  ',
     7   ' [TdV/VdT] ', ' [PdD/DdP] ', ' [PdT/TdP] ', ' [psi/R]   ',
     8   ' [Pa/K]    ', ' [KPa/K]   ', ' [MPa/K]   ', ' [bar/K]   ',
     9   ' [Pa-m3/Kg]', ' [KPaL/mol]', ' [KPam3/Kg]', ' PSI-ft3/lb'/
C-----Version 3.31  Conversion factor #52 changed 10^13 to 10^10
      DATA CONVF /
     1     1.D0,          1.D+03,        1.D+06,        1.D+05,
     2     1.01325D+05,   6894.757D0,    1.D-03,        1.D-06,
     3     1.D-05,        9.86923D-06,   1.45037D-04,   1.D0,
     4     0.555555556D0, 1.D0,          1.8D0,         1.D0,
     5     1.D+03,        4.0026D0,      16.01846D0,    1.D0,
     6     1.D-03,        0.2498376D0,   6.24279D-02,   1.D0,
     7     1.D+03,        249.8376D0,    4186.8D0,      1.D0,
     8     1.D+03,        249.8376D0,    2326.D0,       1.D0,
     9     0.3048,        1.D-03,        1.D-05,        1.D0,
     A     8.05765D-05,   1.D0,          1.730735D0,    1.D-06,
     1     1.D-07,        1.488164D0,    1.D0,          1.D-04,
     2     9.290304D-2,   1.D0,          1.D-03,        14.59390,
     3     1.D0,          1.D0,          10.D0,         1.D+10,
     4     1.D0,          1.43593D-02,   0.555555556D0, 1.D0,
     5     1.D0,          1.D0,          227462.3D0,    5.1953D0,
     6     69.6412D0,     1.D0,          2077.226D0,    10791.812D0,
     7     1.D0,          1.D0,          1.D0,          12410.56D0,
     8     1.D0,          1.D+03,        1.D+06,        1.D+05,
     9     1.D0,          249.8376D0,    1.D+03,        430.4257D0/
      END
C
C
C
      SUBROUTINE UNILIB (NK)
* (C) Copyright (1992), Cryodata Inc.; all rights reserved.
C Output is the array KUNITS in common /UUNITS/
C    the library array KULIB (j,NK) is copied onto KUNITS.
C KUNITS is used in subroutine CALC.  Sequence is that of PROP array.
C KUNITS may be overwritten in order to customize units.
      PARAMETER (NKMAX=4)
      DIMENSION KULIB(0:40,NKMAX)
      COMMON /UUNITS/ KUNITS(0:40)
      DATA KULIB /
     A  1, 57, 12, 16, 20,  1, 69, 28, 24, 28, 28, 28, 28, 60, 24, 24,
     B  1, 65,  1, 53, 32, 36, 73, 69, 28, 58, 38,  1, 43, 46,  1,  1,
     C 12, 12,  1, 32, 32, 49, 50, 12,  1,
     D  1,  2, 12, 16, 21,  1, 70, 29, 25, 29, 29, 29, 29,  4, 25, 25,
     E  1, 65,  1,  7, 32, 34, 75, 70, 29, 40, 38,  1, 44, 47,  1,  1,
     F 12, 12,  1, 32, 32, 51, 52, 12,  2,
     G  1,  2, 12, 18, 22,  1, 70, 30, 26, 30, 30, 30, 30,  2, 26, 26,
     H  1, 65,  1,  7, 32, 34, 74, 70, 30, 40, 38,  1, 43, 47,  1,  1,
     I 12, 12,  1, 32, 32, 51, 52, 12,  3,
     J  1,  6, 13, 19, 23,  1, 68, 31, 27, 31, 31, 31, 31,  6, 27, 27,
     K  1, 65,  1, 11, 33, 37, 76, 68, 31, 42, 39,  1, 45, 48,  1,  1,
     L 13, 13,  1, 33, 33, 49, 50, 12,  4/
C Note: (j,40) is the set number, equivalent to NK
C If NK > NKmax, customized units have been set in an external program:
C     do not overwrite.
      IF (NK .GT. NKMAX) RETURN
      N = MAX (1, NK)
      DO 20 J = 0, 40
   20 KUNITS(J) = KULIB (J, N)
      END
C
C
C
C-----Version 3.301: local variable DPDT renamed XDPDT
      SUBROUTINE FDT (IDID, Q, PS, XDPDT, DL, DV, DD, TT)
* (C) Copyright (1992), Cryodata Inc.; all rights reserved.
C Check of (density, tmeperature) input variables for out-of-range
C or liquid-vapor mixtures.
      IMPLICIT DOUBLE PRECISION (A-H, O-Z)
      DATA TCC, DSATM /5.1953D0, 146.16D0/
      D = DD
      T = TT
      IDID = 1
      IF (T .LT. 0.8D0) THEN
         IDID = -103
      ELSE IF (T .GT. 1501.) THEN
         IDID = -104
      ELSE IF (D .LE. 0.D0) THEN
         IDID = -105
      ELSE IF (D .GT. DENMAX (T)) THEN
         IDID = -106
      ENDIF
      IF (IDID .LT. 0) RETURN
      IF (D .GT. 69.64D0) THEN
         Q = -1.
      ELSE
         Q = 2.
      ENDIF
      IF ((T .LT. TCC) .AND. (D .LT. DSATM)) THEN
         XDL = DFSAT (T)
         IF (D .LT. XDL*1.05) THEN
            XDV = DGSAT (T)
            IF (D .GT. XDV*0.95) THEN
C Close to or within 2-phase region
C-----Version 3.301: local variable DPDT renamed XDPDT
               CALL PSATFT (PS, XDPDT, T)
               DL = D2LFPT (PS, T)
               IF (D .LT. DL) THEN
                  DV = D2VFPT (PS, T)
                  IF (D .GT. DV) THEN
                     IDID = 3
                     IF (DL .EQ. DV) THEN
                        Q = 0.5
                     ELSE
                        Q  = (DL/D - 1.D0) / (DL/DV - 1.D0)
                     ENDIF
                  ENDIF
               ENDIF
            ENDIF
         ENDIF
      ENDIF
      RETURN
      END
C
C
C
      DOUBLE PRECISION FUNCTION PRESS (D, T)
* (C) Copyright (1990), Cryodata Inc.; all rights reserved.
* This function written by V. Arp.
*
* Pressure [Pa] as a function of density [kg/m3] and temperature [K]
* Valid for temperatures 0.8 to 1500 K, but excluding the 2-phase region.
      IMPLICIT DOUBLE PRECISION (A-H, O-Z)
      CALL AMLAP (TL, TH, D)
      IF (T .GT. TL) THEN
*        He I equation (above 2.5 K)
         PRESS = PRESSM ((D/4.0026D0), T) * 1.D+06
         IF (T .GE. TH) RETURN
      ENDIF
*     He II and lambda line vicinity
      PA = PRESSA (D, T)
      IF (T .LE. TL) THEN
         PRESS = PA
      ELSE
         WT    = (T - TL) / (TH - TL)
         PRESS = WT*PRESS + (1. - WT)*PA
      ENDIF
      END
C
C
C
      DOUBLE PRECISION FUNCTION DPDT (D, T)
* (C) Copyright (1990), Cryodata Inc.; all rights reserved.
*
* dP/dT [kG/K] at constant V as a function of density [kg/m3]
* and temperature [K]
* Valid for temperatures 0.8 to 1500 K, but excluding the 2-phase region.
      IMPLICIT DOUBLE PRECISION (A-H, O-Z)
      CALL AMLAP (TL, TH, D)
      IF (T .GT. TL) THEN
         DPDT = DPDTM ((D/4.0026D0), T) * 1.D+06
         IF (T .GE. TH) RETURN
      ENDIF
      DA = DPDTA (D, T)
      IF (T .LE. TL) THEN
         DPDT = DA
      ELSE
         WT   = (T - TL) / (TH - TL)
         DPDT = WT*DPDT + (1. - WT)*DA
      ENDIF
      END
C
C
C
      DOUBLE PRECISION FUNCTION DPDD (D, T)
* (C) Copyright (1990), Cryodata Inc.; all rights reserved.
*
* dP/dD [Pa-m3/kg] at constant T as a function of density [kg/m3]
* and temperature [K]
* Valid for temperatures 0.8 to 1500 K, but excluding the 2-phase region.
      IMPLICIT DOUBLE PRECISION (A-H, O-Z)
      CALL AMLAP (TL, TH, D)
      IF (T .GT. TL) THEN
         DPDD = DPDDM ((D/4.0026D0), T) * 249837.6D0
         IF (T .GE. TH) RETURN
      ENDIF
      IF (T .GE. TH) RETURN
      DA = DPDDA (D, T)
      IF (T .LE. TL) THEN
         DPDD = DA
      ELSE
         WT   = (T - TL) / (TH - TL)
         DPDD = WT*DPDD + (1. - WT)*DA
      ENDIF
      END
C
C
C
      DOUBLE PRECISION FUNCTION CV (D, T)
* (C) Copyright (1990), Cryodata Inc.; all rights reserved.
*
* Cv [J/(kg-K)] as a function of density [kg/m3] and temperature [K]
* Valid for temperatures 0.8 to 1500 K, but excluding the 2-phase region.
      IMPLICIT DOUBLE PRECISION (A-H, O-Z)
      DATA GMWT /4.0026D0/
      CALL AMLAP (TL, TH, D)
      IF (T .GT. TL) THEN
         CV = 1000. * CVM ((D/GMWT), T) / GMWT
         IF (T .GE. TH) RETURN
      ENDIF
      CA = CVA (D, T)
      IF (T .LE. TL) THEN
         CV = CA
      ELSE
         WT = (T - TL) / (TH - TL)
         CV = WT*CV + (1. - WT)*CA
      ENDIF
      END
C
C
C
      DOUBLE PRECISION FUNCTION ENTROP (D, T)
* (C) Copyright (1990), Cryodata Inc.; all rights reserved.
*
* Entropy [J/(kg-K)] as a function of density [kg/m3] and temperature [K]
* Valid for temperatures 0.8 to 1500 K, but excluding the 2-phase region.
      IMPLICIT DOUBLE PRECISION (A-H, O-Z)
      DATA GMWT /4.0026D0/
      CALL AMLAP (TL, TH, D)
      IF (T .GT. TL) THEN
         ENTROP = 1000.D0 * ENTRM ((D/GMWT), T) / GMWT
         IF (T .GE. TH) RETURN
      ENDIF
      SA = ENTRA (D, T)
      IF (T .LE. TL) THEN
         ENTROP = SA
      ELSE
         WT   = (T - TL) / (TH - TL)
         ENTROP = WT*ENTROP + (1. - WT)*SA
      ENDIF
      END
C
C
C
      SUBROUTINE SHAUG (PRP)
* (C) Copyright (1992), Cryodata Inc.; all rights reserved.
*
* Output: S, H, A, U, G;  Input: P, D, T ; all SI units
*    PRP: 1=P, 2=T, 3=D, 8=S, 9=H, 10=A, 11=U, 12=G, 13=Fugacity
* Valid for temperatures 0.8 to 1500 K, but excluding the 2-phase region.
      IMPLICIT DOUBLE PRECISION (A-H, O-Z)
      DIMENSION PRP (*)
C Note: the first index is 1, not 0
      DATA GMWT /4.0026D0/
      CALL AMLAP (TL, TH, PRP(3))
      PV = PRP(1)/PRP(3)
      IF (PRP(2) .GT. TL) THEN
         PRP(8) = 1.D+03 * ENTRM ((PRP(3)/GMWT), PRP(2)) / GMWT
         PRP(11)= 1.D+03 * ENERM ((PRP(3)/GMWT), PRP(2)) / GMWT
         PRP(9) = PRP(11) + PV
         PRP(10)= PRP(11) - PRP(2)*PRP(8)
         PRP(12)= PRP(10) + PV
         IF (PRP(2) .GE. TH) RETURN
      ENDIF
      SA = ENTRA (PRP(3), PRP(2))
      AA = HELMA (PRP(3), PRP(2))
      UA = AA + PRP(2)*SA
      HA = UA + PV
      GA = AA + PV
      IF (PRP(2) .LE. TL) THEN
         PRP(8) = SA
         PRP(9) = HA
         PRP(10)= AA
         PRP(11)= UA
         PRP(12)= GA
      ELSE
         WT   = (PRP(2) - TL) / (TH - TL)
         OMWT = 1.D0 - WT
         PRP( 8) = WT*PRP( 8) + OMWT*SA
         PRP( 9) = WT*PRP( 9) + OMWT*HA
         PRP(10) = WT*PRP(10) + OMWT*AA
         PRP(11) = WT*PRP(11) + OMWT*UA
         PRP(12) = WT*PRP(12) + OMWT*GA
      ENDIF
      END
C
C
C
      DOUBLE PRECISION FUNCTION CVM (RHO,T)
* (C) Copyright (1994), Cryodata Inc.; all rights reserved.
*
* Output: CVM [J/mol-K] = Cv = specific heat at constant volume
* Input:  RHO [mol/L]   = density (2-phase region excluded)
*         T [K]         = temperature, range 2 to 1500 K
*
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
C-----Version 3.3; function rewritten for greater efficiency
      PARAMETER (CPIG0=2.5D0*0.00831430D0)
      PARAMETER (ONE3RD=1.D0/3.D0, TWO3RD=2.D0/3.D0, TWO7TH=2.D0/7.D0,
     +           SIX7TH=6.D0/7.D0, F1OP3=1.D0/0.3D0)
      DIMENSION G(32), VP(9)
      COMMON/MC1/ G,R,GM,VP,DTP,PCC,PTP,TCC,TTP,TUL,TLL,PUL,DCC
      TT=T
      D =RHO
      D2=D*D
      D4=D2*D2
      TI=1.D0/TT
      TI2=TI*TI
      F=EXP(GM*D2)
      GMI = 0.5D0/GM
      GG2=-2.D0*GMI*GMI
      GG3=-4.D0*GG2*GMI
      GG4=-6.D0*GG3*GMI
      GG5=-8.D0*GG4*GMI
      GG6=-10.D0*GG5*GMI
      GD1=F*GMI
      GD2=(F*D2-2.D0*GD1)*GMI
      GD3=(F*D4-4.D0*GD2)*GMI
      GD4=(F*D4*D2-6.D0*GD3)*GMI
      GD5=(F*D4*D4-8.D0*GD4)*GMI
      GD6=(F*D4*D4*D2-10.D0*GD5)*GMI
      CINT = D*(-0.25D0*G(2)/SQRT(TT)+TI2*(2.D0*G(4)+6.D0*G(5)*TI+D
     A *(G(8)+3.D0*G(9)*TI+TWO3RD*G(12)*D))+D4*TI2*((((0.75D0*G(19)
     B *D+SIX7TH*G(18))*TI+TWO7TH*G(17))*D+ONE3RD*G(16))*D+1.2D0*TI
     C *G(15)+0.4D0*G(14)))+6.D0*TI2*TI*((GD1-GMI)*(G(20)+2.D0*TI
     D *G(21))+(GD2-GG2)*(G(22)+F1OP3*TI2*G(23))+(GD3-GG3)*(G(24)
     E +2.D0*TI*G(25))+(GD4-GG4)*(G(26)+F1OP3*TI2*G(27))+(GD5-GG5)
     F *(G(28)+2.D0*TI*G(29))+(GD6-GG6)*(G(30)+TI*(2.D0*G(31)+F1OP3
     G *TI*G(32))))
      CVM = 1.D+03 * (CPIG0 - (R+CINT))
      END
C
C
C
        DOUBLE PRECISION FUNCTION DPDDM (RR,TT)
* (C) Copyright (1994), Cryodata Inc.; all rights reserved.
*
* Output: DPDDM [MPa-L/mol] = dPressure/dDensity at constant temperature
* Input:  RR [mol/L]        = density (2-phase region excluded)
*         TT [K]            = temperature, range 2 to 1500 K
*
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
C-----Version 3.3; function rewritten for greater efficiency
      PARAMETER (C53=5.D0/3.D0, C73=7.D0/3.D0, C113=11.D0/3.D0,
     +           C133=13.D0/3.D0)
      DIMENSION G(32), VP(9)
      COMMON/MC1/ G,R,GM,VP,DTP,PCC,PTP,TCC,TTP,TUL,TLL,PUL,DCC
      D=RR
      T = TT
      D2=D*D
      TI=1.D0/TT
      TI2=TI*TI
      F=EXP(GM*D2)
      F1=2.0D0*F*GM*D
      FZ=F1*D2*D
      FY=3.0D0*F*D2
      DPDDM = D*(2.D0*(G(3)+G(2)*SQRT(T)+G(1)*T+TI*(G(4)+TI*G(5)))
     A +D*(3.D0*(T*G(6)+G(7)+TI*(G(8)+TI*G(9)))+D*(4.D0*(T*G(10)
     B +G(11)+TI*G(12)+D*1.25D0*G(13))+D2*TI*((6.D0*(G(14)+TI*G(15))
     C +D*7.D0*G(16)+D2*(8.D0*(G(17)+TI*G(18))+9.D0*TI*D*G(19)))))))
     E +TI2*((FY+FZ)*(G(20)+TI*G(21))+D2*((C53*FY+FZ)*(G(22)+TI2*G(23))
     F +D2*((C73*FY+FZ)*(G(24)+TI*G(25))+D2*((3.D0*FY+FZ)*(G(26)+TI2
     G *G(27))+D2*((C113*FY+FZ)*(G(28)+TI*G(29))+D2*((C133*FY+FZ)
     H *(G(30)+TI*G(31)+TI2*G(32))))))))+R*T
        END
C
C
C
        DOUBLE PRECISION FUNCTION DPDTM (RR,TT)
* (C) Copyright (1994), Cryodata Inc.; all rights reserved.
*
* Output: DPDTM [MPa/K] = dPressure/dTemperature at constant density
* Input:  RR [mol/L]    = density (2-phase region excluded)
*         TT [K]        = temperature, range 2 to 1500 K
*
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
C-----Version 3.3; function rewritten for greater efficiency
      DIMENSION G(32), VP(9)
      COMMON/MC1/ G,R,GM,VP,DTP,PCC,PTP,TCC,TTP,TUL,TLL,PUL,DCC
      D = RR
      T = TT
      D2= D*D
      TI= 1.D0/T
      TI2=TI*TI
      DPDTM = D2*(G(1)+0.5D0*G(2)/SQRT(T)-TI2*(G(4)+2.D0*TI*G(5))
     A  + D*(G(6)-TI2*(G(8)+TI*(2.D0*G(9)+EXP(GM*D2)*(2.D0*G(20)
     B  + 3.D0*TI*G(21)+D2*(2.D0*G(22)+4.D0*TI2*G(23)+D2*(2.D0*G(24)
     C  + 3.D0*TI*G(25)+D2*(2.D0*G(26)+4.D0*TI2*G(27)+D2*(2.D0*G(28) 
     D  + 3.D0*TI*G(29)+D2*(2.D0*G(30)+3.D0*TI*G(31)+4.D0*TI2*G(32)
     E    ))))))))+D*(G(10)-TI2*G(12)-D2*TI2*(G(14)+2.D0*TI*G(15)
     F  + D*G(16)+D2*(G(17)+2.D0*TI*(G(18)+D*G(19)))))))+R*D
        END
C
C
C
        DOUBLE PRECISION FUNCTION ENERM (RR,TT)
* (C) Copyright (1994), Cryodata Inc.; all rights reserved.
*
*  Output: ENERM [J/mol] = Internal energy
*  Input:  RR [mol/L]    = density (2-phase region excluded)
*          TT [K]        = temperature, range 2 to 1500 K
*
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
C-----Version 3.3; function rewritten for greater efficiency
      PARAMETER (ONE3RD=1.D0/3.D0, TWO3RD=2.D0/3.D0, TWO7TH=2.D0/7.D0,
     1 THREE7=3.D0/7.D0)
      DIMENSION G(32), VP(9)
      COMMON/MC1/ G,R,GM,VP,DTP,PCC,PTP,TCC,TTP,TUL,TLL,PUL,DCC
      COMMON/MC8/ T0,S0,H0
      T = TT
      TI=1.D0/TT
      TI2=TI*TI
      D=RR
      D2=D*D
      D3=D2*D
      D4=D3*D
      F=EXP(GM*D2)
      GMI=0.5D0/GM
      GG2=-2.D0*GMI*GMI
      GG3=-4.D0*GG2*GMI
      GG4=-6.D0*GG3*GMI
      GG5=-8.D0*GG4*GMI
      GG6=-10.D0*GG5*GMI
      GD1=F*GMI
      GD2=(F*D2-2.D0*GD1)*GMI
      GD3=(F*D4-4.D0*GD2)*GMI
      GD4=(F*D4*D2-6.D0*GD3)*GMI
      GD5=(F*D4*D4-8.D0*GD4)*GMI
      GD6=(F*D4*D4*D2-10.D0*GD5)*GMI
      E = 2.5D0*R*T+D*(G(2)*0.5D0*SQRT(T)+G(3)+TI*(G(4)*2.D0+TI*G(5)
     1 *3.0)+D*(G(7)*0.5D0+TI*(G(8)+TI*G(9)*1.5D0)+D*(G(11)*ONE3RD+TI
     2 *G(12)*TWO3RD+D*(G(13)*0.25D0+D*TI*(G(14)*0.4D0+TI*G(15)*0.6D0
     3 +D*(G(16)*ONE3RD+D*(G(17)*TWO7TH+TI*(G(18)*THREE7+D*G(19)
     4 *0.375D0))))))))+TI2*((GD1-GMI)*(3.D0*G(20)+TI*4.D0*G(21))
     5 +(GD2-GG2)*(3.D0*G(22)+TI2*5.D0*G(23))+(GD3-GG3)*(3.D0*G(24)
     6 +TI*4.D0*G(25))+(GD4-GG4)*(3.D0*G(26)+TI2*5.D0*G(27))+(GD5-GG5)
     7 *(3.D0*G(28)+TI*4.D0*G(29))+(GD6-GG6)*(3.D0*G(30)+TI*(4.D0
     8 *G(31)+TI*5.D0*G(32))))
        ENERM = (E - R*T + H0) * 1.D+03
        END
C
C
C
        DOUBLE PRECISION FUNCTION ENTRM (RR,TT)
* (C) Copyright (1994), Cryodata Inc.; all rights reserved.
*
*
* Output: ENTRM [J/mol-K] = entropy
* Input:  RR [mol/L]      = density (2-phase region excluded)
*         TT [K]          = temperature, range 2 to 1500 K
*
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
C-----Version 3.3; function rewritten for greater efficiency
      PARAMETER (ONE3=1.D0/3.D0, ONE6=1.D0/6.D0, ONE7=1.D0/7.D0,
     +  TWO7=2.D0/7.D0)
      DIMENSION G(32), VP(9)
      COMMON/MC1/ G,R,GM,VP,DTP,PCC,PTP,TCC,TTP,TUL,TLL,PUL,DCC
      COMMON/MC8/ T0,S0,H0
      DATA P0 /0.101325D0/
      T = TT
      TI=1.D0/T
      TI2=TI*TI
      D=RR
      D2=D*D
      D4=D2*D2
      F=EXP(GM*D2)
      GG1=0.5D0/GM
      GG2=-2.D0*GG1*GG1
      GG3=-4.D0*GG2*GG1
      GG4=-6.D0*GG3*GG1
      GG5=-8.D0*GG4*GG1
      GG6=-10.D0*GG5*GG1
      GD1=F*GG1
      GD2=(F*D2-2.D0*GD1)*GG1
      GD3=(F*D4-4.D0*GD2)*GG1
      GD4=(F*D4*D2-6.D0*GD3)*GG1
      GD5=(F*D4*D4-8.D0*GD4)*GG1
      GD6=(F*D4*D4*D2-10.D0*GD5)*GG1
Cvda  Z(4) = 2.5
      SINT = 2.5D0*R*LOG(T)+D*(-G(1)-0.5*G(2)/SQRT(T)-D*(0.5D0*G(6)
     A + ONE3*D*G(10))+TI2*(G(4)+D*(0.5*G(8)+D*(ONE3*G(12)+D2*(0.2D0
     B *G(14)+D*(ONE6*G(16)+D*ONE7*G(17)))))+TI*(2.D0*G(5)+D*(G(9)
     C +D2*D*(0.4D0*G(15)+D2*(TWO7*G(18)+D*0.25D0*G(19)))))))+2.D0
     D *TI2*TI*((F-1.D0)*GG1*(G(20)+G(21)*TI*1.5D0)+(GD2-GG2)*(G(22)
     E +G(23)*TI2*2.D0)+(GD3-GG3)*(G(24)+G(25)*TI*1.5D0)+(GD4-GG4)
     F *(G(26)+G(27)*TI2*2.D0)+(GD5-GG5)*(G(28)+G(29)*TI*1.5D0)
     G +(GD6-GG6)*(G(30)+G(31)*TI*1.5D0+G(32)*TI2*2.D0))
        ENTRM = (SINT + S0 - R*LOG(D*R*TT/P0)) * 1.D+03
        END
C
C
C
      DOUBLE PRECISION FUNCTION PRESSM (RR,TT)
* (C) Copyright (1994), Cryodata Inc.; all rights reserved.
*
* Output: PRESSM [MPa] = pressure
* Input:  RR [mol/L]   = density (2-phase region excluded)
*         TT [K]       = temperature, range 2 to 1500 K
*
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
C-----Version 3.3; function rewritten for greater efficiency
      DIMENSION G(32), VP(9)
      COMMON/MC1/ G,R,GM,VP,DTP,PCC,PTP,TCC,TTP,TUL,TLL,PUL,DCC
      T = TT
      D = RR
      D2= D*D
      TI= 1.D0/TT
      TI2=TI*TI
      PRESSM = (((((((((((TI*G(32)+G(31))*TI+G(30))*D2+G(29)*TI+G(28))
     A *D2+G(27)*TI2+G(26))*D2+G(25)*TI+G(24))*D2+G(23)*TI2+G(22))*D2
     B +G(21)*TI+G(20))*EXP(GM*D2)+G(09))*TI+G(08))*TI+G(07))*D+
     C ((((((D*G(19)+G(18))*TI+G(17))*D+G(16))*D+TI*G(15)+G(14))*D2
     D +G(12))*TI+D*G(13)+G(11))*D2+(TI*G(05)+G(04))*TI+SQRT(T)*G(02)
     E +((D*G(10)+G(06))*D+G(01))*T+G(03))*D2+R*D*T
      END
C
C
C
      DOUBLE PRECISION FUNCTION PRESSA (D, TT)
* (C) Copyright (1990), Cryodata Inc.; all rights reserved.
*
* Pressure [Pa] as a function of density [kg/m3] and temperature [K]
* Valid in compressed liquid from 0.8 to about 3 K.
* If TT is less than 0.8 K, TT is assumed to be the isochoric distance
*    to the lambda line (negative in HeII, positive in HeI).
      IMPLICIT DOUBLE PRECISION (A-H, O-Z)
      DIMENSION PVTCSA(6)
      COMMON /SUBHEC/ C(41,2), CL(8,8), V0, T0
      COMMON /SUBLAM/ TL, DTLDV, D2TDV2, VSAVE
      V  = 1000.D0/D
      X  = V - V0
      IF (V .NE. VSAVE) CALL LAMDER (V)
      IF (TT .GT. 0.7999D0) THEN
         T  = TT
         DT = T - TL
      ELSE
         DT = TT
         T  = TL + DT
      ENDIF
      IF (DT .GT. 0.D0) THEN
         M = 1
      ELSE
         M = 2
      ENDIF
      CALL LOGFUN (PVTCSA, T, DT, M)
      PRESSA  = (PVTCSA(1) + PRESA2 (X, T, M)) * 1.D+06
      END
C
C
C
      DOUBLE PRECISION FUNCTION PRESA2 (X, T, M)
* (C) Copyright (1990), Cryodata Inc.; all rights reserved.
*
* "background" pressure [MPa] as a function of V-V0 [cm3/gm], and T [K].
* M specifies HeI or HeII.  V0 is the volume at the lower lambda point.
* Called from PRESSA.
      IMPLICIT DOUBLE PRECISION (A-H, O-Z)
      DIMENSION F(7), Q(5)
      COMMON /SUBHEC/ C(41,2), CL(8,8), V0, T0
      T2      = T*T
      F(1)    = 1.D0
      DO 40 J = 1, 5
   40 Q(J)    = 0.D0
      DO 60 K = 1, 6
      DO 50 J = 1, 5
   50 Q(J)    = Q(J) + F(K)*C(6*J+K-1, M)
   60 F(K+1)  = F(K)*X
      PRESA2  = Q(1)+T2*(Q(2)+T2*(Q(3)+T2*(Q(4)+T2*Q(5))))
      END
C
C
C
      DOUBLE PRECISION FUNCTION DPDTA (D, TT)
* (C) Copyright (1990), Cryodata Inc.; all rights reserved.
*
* dP/dT as a function of density and temperature [SI units]
* Valid in compressed liquid from 0.8 to about 3 K.
* If TT is less than 0.8 K, TT is assumed to be the isochoric distance
*    to the lambda line (negative in HeII, positive in HeI).
      IMPLICIT DOUBLE PRECISION (A-H, O-Z)
      DIMENSION F(7), Q(4), PVTCSA(6)
      COMMON /SUBHEC/ C(41,2), CL(8,8), V0, T0
      COMMON /SUBLAM/ TL, DTLDV, D2TDV2, VSAVE
      V  = 1000./D
      X  = V - V0
      IF (V .NE. VSAVE) CALL LAMDER (V)
      IF (TT .GT. 0.7999D0) THEN
         T  = TT
         DT = T - TL
      ELSE
         DT = TT
         T  = TL + DT
      ENDIF
      IF (DT .GT. 0.) THEN
         M = 1
      ELSE
         M = 2
      ENDIF
      CALL LOGFUN (PVTCSA, T, DT, M)
      T2      = T*T
      F(1)    = 1.
      DO 40 J = 1, 4
   40 Q(J)    = 0.
      DO 60 K = 1, 6
      DO 50 J = 1, 4
   50 Q(J)    = Q(J) + F(K)*C(6*J+K+5, M)
   60 F(K+1)  = F(K)*X
      DPDTA   = (PVTCSA(3) + T*(2.D0*Q(1) + T2*(4.D0*Q(2) + T2*
     A           (6.D0*Q(3) + T2*8.D0*Q(4))))) * 1.D+06
      END
C
C
C
      DOUBLE PRECISION FUNCTION DPDDA (D, TT)
* (C) Copyright (1990), Cryodata Inc.; all rights reserved.
*
* dP/dD as a function of density and temperature [SI units]
* Valid in compressed liquid from 0.8 to about 3 K.
* If TT is less than 0.8 K, TT is assumed to be the isochoric distance
*    to the lambda line (negative in HeII, positive in HeI).
      IMPLICIT DOUBLE PRECISION (A-H, O-Z)
      DIMENSION F(6), Q(5), PVTCSA(6)
      COMMON /SUBHEC/ C(41,2), CL(8,8), V0, T0
      COMMON /SUBLAM/ TL, DTLDV, D2TDV2, VSAVE
      V  = 1000./D
      X  = V - V0
      IF (V .NE. VSAVE) CALL LAMDER (V)
      IF (TT .GT. 0.7999D0) THEN
         T  = TT
         DT = T - TL
      ELSE
         DT = TT
         T  = TL + DT
      ENDIF
      IF (DT .GT. 0.) THEN
         M = 1
      ELSE
         M = 2
      ENDIF
      CALL LOGFUN (PVTCSA, T, DT, M)
      T2      = T*T
      F(1)    = 1.
      DO 40 J = 1, 5
   40 Q(J)    = 0.
      DO 60 K = 1, 5
      FF      = F(K)*DBLE(K)
      DO 50 J = 1, 5
   50 Q(J)    = Q(J) + FF*C(6*J+K, M)
   60 F(K+1)  = F(K)*X
      DPDV    = PVTCSA(2) + Q(1) + T2*(Q(2) + T2*(Q(3) + T2*
     A           (Q(4) + T2*Q(5))))
      DPDDA   = -1.D+09 * DPDV/(D*D)
      END
C
C
C
      DOUBLE PRECISION FUNCTION CVA (D, TT)
* (C) Copyright (1990), Cryodata Inc.; all rights reserved.
*
* Cv as a function of density and temperature [SI units]
* Valid in compressed liquid from 0.8 to about 3 K.
* If TT is less than 0.8 K, TT is assumed to be the isochoric distance
*    to the lambda line (negative in HeII, positive in HeI).
      IMPLICIT DOUBLE PRECISION (A-H, O-Z)
      DIMENSION F(7), Q(4), PVTCSA(6)
      COMMON /SUBHEC/ C(41,2), CL(8,8), V0, T0
      COMMON /SUBLAM/ TL, DTLDV, D2TDV2, VSAVE
      V  = 1000./D
      X  = V - V0
      IF (V .NE. VSAVE) CALL LAMDER (V)
      IF (TT .GT. 0.7999D0) THEN
         T  = TT
         DT = T - TL
      ELSE
         DT = TT
         T  = TL + DT
      ENDIF
      IF (DT .GT. 0.) THEN
         M = 1
      ELSE
         M = 2
      ENDIF
      CALL LOGFUN (PVTCSA, T, DT, M)
      T2      = T*T
      F(1)    = X
      DO 10 J = 1, 4
   10 Q(J)    = 0.D0
      DO 30 K = 1, 6
      FF      = F(K)/DBLE(K)
      DO 20 J = 1, 4
   20 Q(J)    = Q(J) + FF*C(6*J+K+5, M)
   30 F(K+1)  = F(K)*X
      CVA     = (PVTCSA(4) + T*(2.D0*Q(1) + T2*(12.D0*Q(2) + T2*
     A          (30.D0*Q(3) + T2*56.D0*Q(4))))
     B          + T*(C(36,M) + T2*(C(37,M) + T2*(C(38,M)
     C          + T2*C(39,M))))) * 1.D+03
      END
C
C
C
      DOUBLE PRECISION FUNCTION ENTRA (D, TT)
* (C) Copyright (1990), Cryodata Inc.; all rights reserved.
*
* Entropy as a function of density and temperature [SI units]
* Valid in compressed liquid from 0.8 to about 3 K.
* If TT is less than 0.8 K, TT is assumed to be the isochoric distance
*    to the lambda line (negative in HeII, positive in HeI).
      IMPLICIT DOUBLE PRECISION (A-H, O-Z)
      DIMENSION PVTCSA(6)
      COMMON /SUBHEC/ C(41,2), CL(8,8), V0, T0
      COMMON /SUBLAM/ TL, DTLDV, D2TDV2, VSAVE
      V  = 1000./D
      X  = V - V0
      IF (V .NE. VSAVE) CALL LAMDER (V)
      IF (TT .GT. 0.7999D0) THEN
         T  = TT
         DT = T - TL
      ELSE
         DT = TT
         T  = TL + DT
      ENDIF
      IF (DT .GT. 0.) THEN
         M = 1
      ELSE
         M = 2
      ENDIF
      CALL LOGFUN (PVTCSA, T, DT, M)
      ENTRA  = (PVTCSA(5) + ENTRA2 (X, T, M)) * 1.D+03
      END
C
C
C
      DOUBLE PRECISION FUNCTION ENTRA2 (X, T, M)
* (C) Copyright (1990), Cryodata Inc.; all rights reserved.
*
* "background" entropy [J/gm-K] as a function of V-V0 [cm3/gm], and T [K].
* M specifies HeI or HeII.  V0 is the volume at the lower lambda point.
* Called from ENTRA.
      IMPLICIT DOUBLE PRECISION (A-H, O-Z)
      DIMENSION F(7), Q(4)
      COMMON /SUBHEC/ C(41,2), CL(8,8), V0, T0
      T2   = T*T
      T5   = T2*T2*T
      F(1) = X
      DO 40 J = 1, 4
   40 Q(J) = 0.
      DO 60 K = 1, 6
         FF = F(K)/DBLE(K)
         DO 50 J = 1, 4
   50    Q(J) = Q(J) + FF*C(6*J+K+5,M)
   60 F(K+1) = F(K) * X
      ENTRA2 =  T*(2.D0*Q(1) + T2*(4.D0*Q(2) + T2*(6.D0*Q(3)
     A        + T2*8.D0*Q(4)))) + C(36,M)*T  + C(37,M)*T2*T/3.D0
     B        + C(38,M)*T5/5.D0 + C(39,M)*T5*T2/7.D0 + C(40,M)
      END
C
C
C
      DOUBLE PRECISION FUNCTION HELMA (D, TT)
* (C) Copyright (1990), Cryodata Inc.; all rights reserved.
*
* Helmholtz energy as a function of density and temperature [SI units]
* Valid in compressed liquid from 0.8 to about 3 K.
* If TT is less than 0.8 K, TT is assumed to be the isochoric distance
*    to the lambda line (negative in HeII, positive in HeI).
      IMPLICIT DOUBLE PRECISION (A-H, O-Z)
      DIMENSION PVTCSA(6)
      COMMON /SUBHEC/ C(41,2), CL(8,8), V0, T0
      COMMON /SUBLAM/ TL, DTLDV, D2TDV2, VSAVE
      V  = 1000./D
      X  = V - V0
      IF (V .NE. VSAVE) CALL LAMDER (V)
      IF (TT .GT. 0.7999D0) THEN
         T  = TT
         DT = T - TL
      ELSE
         DT = TT
         T  = TL + DT
      ENDIF
      IF (DT .GT. 0.) THEN
         M = 1
      ELSE
         M = 2
      ENDIF
      CALL LOGFUN (PVTCSA, T, DT, M)
      HELMA  = -1.D+03 * (PVTCSA(6) + HELMA2 (X, T, M))
      END
C
C
C
      DOUBLE PRECISION FUNCTION HELMA2 (X, T, M)
* (C) Copyright (1990), Cryodata Inc.; all rights reserved.
*
* "background" Helmholtz energy [J/gm] as a function of V-V0 [cm3/gm]
C  and T [K].
* M specifies HeI or HeII.  V0 is the volume at the lower lambda point.
* Called from HELMA.
      IMPLICIT DOUBLE PRECISION (A-H, O-Z)
      DIMENSION F(7), Q(5)
      COMMON /SUBHEC/ C(41,2), CL(8,8), V0, T0
      T2 = T*T
      T4 = T2*T2
      F(1)    = X
      DO 40 J = 1, 5
   40 Q(J)    = 0.
      DO 60 K = 1, 6
      FF      = F(K)/DBLE(K)
      DO 50 J = 1, 5
   50 Q(J)    = Q(J) + FF*C(6*J+K-1, M)
   60 F(K+1)  = F(K)*X
      HELMA2  = Q(1)+T2*(Q(2)+T2*(Q(3)+T2*(Q(4)+T2*Q(5))))
     &    + C(36,M)*T2/2.D0 + C(37,M)*T4/12.D0
     &    + C(38,M)*T4*T2/30.D0+ C(39,M)*T4*T4/56.D0
     &    + C(40,M)*T + C(41,M)
      END
C
C
C
      SUBROUTINE LAMDER (V)
* (C) Copyright (1990), Cryodata Inc.; all rights reserved.
*
*-----OUTPUT
* T      = Lambda temperature [K] [T76 scale]
* DTDV   = dT/dV              [(K-gm)/cm3]
* D2TDV2 = d2T/dV2            [(K-gm2)/cm6]
*-----INPUT
* V = Specific volume [cm3/gm]
*
      IMPLICIT DOUBLE PRECISION (A-H, O-Z)
      COMMON /SUBLAM/ T, DTDV, D2TDV2, VSAVE
      COMMON /SUBHEC/ C(41,2), CL(8,8), V0, T0
      DIMENSION A(5)
      DATA A /           0.91672438D-01, -0.82840336D-01,
     &  0.71832749D-01,  0.48395170D-01,  0.39159012D-01/
      X    = V - V0
      T    = T0 + X*(A(1) + X*(A(2) + X*(A(3) + X*(A(4) + X*A(5)))))
      DTDV = A(1)+ X*(2.D0*A(2) + X*(3.D0*A(3) + X*(4.D0*A(4)
     A       + X*5.D0*A(5))))
      D2TDV2 = 2.D0*A(2)+X*(6.D0*A(3)+X*(12.D0*A(4)+X*20.D0*A(5)))
      END
C
C
C
      SUBROUTINE LOGFUN (PVTCSA, TT, DTT, M)
* (C) Copyright (1994), Cryodata Inc.; all rights reserved.
*
* quasi-logarithmic thermodynamic functions
* Output:
* PVTCSA(1) = pressure [MPa]
* PVTCSA(2) = dP/dV [MPa-kg/m3]
* PVTCSA(3) = dP/dT [Mpa/K]
* PVTCSA(4) = Cv [J/kg -K]
* PVTCSA(5) = entropy [J/kg-K]
* PVTCSA(6) = Helmholz energy [J/kg]
* Input:
* T  = Temperature
* DT = T - Tlambda = isochoric distance to the lambda line
* (Note: parameters in common SUBLAM must have evaluated prior to this call
*  to LOGFUN, by a call to subroutine LAMDER.)
*
      PARAMETER (MAX=9)
      IMPLICIT DOUBLE PRECISION (A-H, O-Z)
      DIMENSION PVTCSA(6), Y(MAX), CL5(5)
      COMMON /SUBLAM/ TL, DTLDV, D2TDV2, VSAVE
      COMMON /SUBHEC/ C(41,2), CL(8,8), V0, T0
      SAVE AP, AD, AT, AC, AS, AA, TSAVE, DTSAVE
      DATA ZERO /0.D0/,  DTSAVE, TSAVE /2* 98765.432D0/
      DATA CL5 /1.D0, -4.D0, 12.D0, -24.D0, 24.D0/
*
      T  = TT
      DT = DTT
      IF ((DT .NE. DTSAVE) .OR. (T .NE. TSAVE)) THEN
* Calculation is skipped if input parameters are unchanged
      DTSAVE = DT
      TSAVE  = T
*
* Y(1) = quasi-logarithmic singularity
* Y(i), i>1, = successive indefinite integrals
*
      IF (DT .EQ. ZERO) THEN
         Y(1)    = -100.D0
         DO 10 I = 2, MAX
   10    Y(I)    = ZERO
      ELSE
         Z = ABS (DT)
         Y(1) = LOG (Z)
         FACT = 1.D0
         DO 20 I = 2, MAX
         IF ((ABS(Y(I-1)) .LT. 1.D-25) .AND. (I .GT. 2)) THEN
            Y(I) = ZERO
         ELSE
            Y(I) = (DT*Y(I-1) - DT**(I-1)/FACT) / DBLE(I-1)
         ENDIF
   20    FACT = FACT*DBLE(I)
      ENDIF
*
* Thermodynamic function evaluation
C-----Version 3.3; function rewritten for greater efficiency
*
      T2 = T*T
      R0 = T2*T2
      AP = C(1,M)*Y(2) + C(2,M)*(T2*Y(2) - 4.D0*T*Y(3) + 6.D0*Y(4))
      AT = C(1,M)*Y(1) + C(2,M)*(T2*Y(1) - 2.D0*T*Y(2) + 2.D0*Y(3))
      AD = C(1,M)*Y(1) + C(2,M)*(T2*Y(1) - 4.D0*T*Y(2) + 6.D0*Y(3))
      AC = C(1,M)*T*Y(1) + C(2,M)*T2*T*Y(1)
      AS = C(1,M)*Y(2) + C(2,M)*(T2*Y(2) - 2.D0*T*Y(3) + 2.D0*Y(4))      
      AA = C(1,M)*Y(3) + C(2,M)*(T2*Y(3) - 4.D0*T*Y(4) + 6.D0*Y(5)) 
      DO 40 I = 1, 3
         CKM= C((I+2),M)
         XP = R0*Y(I+1)
         XT = R0*Y(I)
         XD = R0*Y(I)
         XS = R0*Y(I+1)
         XA = R0*Y(I+2)
         R  = R0/T
         DO 30 J = 2, 5
            E  = CL5(J)*R
            EJ = DBLE(J)*E
            XP = XP + EJ*Y(I+J)
            XT = XT + E *Y(I+J-1)
            XD = XD + EJ*Y(I+J-1)
            XS = XS + E* Y(I+J)
            XA = XA + EJ*Y(I+J+1)
            R  = R/T
30       CONTINUE
         AP = AP + CKM*XP
         AT = AT + CKM*XT
         AD = AD + CKM*XD
         AC = AC + CKM*T*R0*Y(I)
         AS = AS + CKM*XS
         AA = AA + CKM*XA
   40 CONTINUE
      AD = DTLDV*DTLDV*AD - D2TDV2*AP
      AP = -DTLDV*AP
      AT = -DTLDV*AT
      ENDIF
* Output
      PVTCSA(1) = AP
      PVTCSA(2) = AD
      PVTCSA(3) = AT
      PVTCSA(4) = AC
      PVTCSA(5) = AS
      PVTCSA(6) = AA
      RETURN
      END
C
C
C
      SUBROUTINE DERIV (F, DI, TI)
* (C) Copyright (1992), Cryodata Inc.; all rights reserved.
*
*-----OUTPUT
* F( 1) = Cp    = Specific heat at constant P    [J/(kG-K)]
* F( 2) = Cv    = Specific heat at constant V    [J/(kG-K)]
* F( 3) = Gamma = Cp/Cv                          [-]
* F( 4) = Alpha = (T/V)(dV/dT)  at constant P    [-]
* F( 5) = Grun  = (V/Cv)(dP/dT) at constant V    [-]
* F( 6) = Kt    = (1/D)(dD/dP)  at constant T    [1/Pa]
* F( 7) = C     = velocity of sound              [m/s]
* F( 8) = JT    = Joule-Thomson coefficient      [K/Pa]
*                 (= dT/dP at constant H)
* F( 9) = DPDD  = dP/dD at constant T            [Pa-m3/kG]
* F(10) = DPDT  = dP/dT at constant D            [Pa/K]
* F(11) = V*(DH/DV) at constant P                [J/kG]
*-----INPUT
* D     = DI = Density                           [kG/M3]
* T     = TI = Temperature                       [K]
*
      IMPLICIT DOUBLE PRECISION (A-H, O-Z)
      DIMENSION F(11)
      SAVE D, T, A, B, CCV
      DATA D, T /-10., -10./
* A, B, CCV, D, and T are local variables (SAVEd from the previous call)
      IF ((DI .NE. D) .OR. (TI .NE. T)) THEN
         D=DI
         T=TI
         A = DPDT (D,T)
         B = DPDD (D,T)
         CCV=CV (D,T)
      ENDIF
      F(2) = CCV
      F(6) = 1.D0/(B*D)
      F(4) = T*A*F(6)
      F(5) = A/(D*CCV)
      F(3) = 1.D0+F(4)*F(5)
      F(7) = MAX ((F(3)*B), 1.D0)
      F(11)= F(7)/F(5)
      F(7) = SQRT (F(7))
      F(1) = F(3)*F(2)
      F(8) = (F(4)-1.D0)/(D*F(1))
      F(9) = B
      F(10)= A
      END
C
C
C
      SUBROUTINE SUPERF (CHEII, PI, D, T, DTV, S, F)
* (C) Copyright (1990, 1992), Cryodata Inc.; all rights reserved.
*  This subroutine returns parameters from the two-fluid model
*  of superfluidity:
*  RSR  = superfluid density fraction                   [-]
*  V2S  = velocity of second sound                      [m/s]
*  V4S  = velocity of fourth sound                      [m/s]
*  GM   = Gorter-Mellink mutual friction parameter      [m-s/kg]
*  COND = conductivity parameter                        [(W/m2)3/(m/K)]
*         (The defining equation for COND is that used by
*          Srinivasan and Hofmann, Cryogenics 25, 652 (1985), i.e.,
*          Q**3 = COND * (dT/dX)
*                 where Q     = heat flux               [W/m2]
*                       dT/dX = temperature gradient    [K/m]
*          ! This equation assumes turbulent counterflow.
*  VISC = viscosity of the normal fluid component       [kg/(m-s)]
*-----INPUT
*  P, D, T, S are pressure, density,  temperature & entropy (SI units)
*  F(11) is the derivative array (see subroutine DERIV for definitions).
*  DTV is T - Tlambda(V), ie, the isochoric distance from Tlambda
*  Version July 12, 1992; for HEPAK v3.2
      IMPLICIT DOUBLE PRECISION (A-H, O-Z)
      DIMENSION F(11), CHEII(5)
*
* Validity very close to the lambda line is unknown.
      IF (DTV .LT. -0.0009) THEN
        RSR = RSRFPT (PI, T)
        DS  = D*RSR
        DN  = D-DS
*  Maynard's equations (2), (3), and (5); assume small corrections.
        C2  = F(7)*F(7)
        V2S = (D/DN-1.)*T*S*S/F(1)
        V2S = V2S*(1. + F(4)*F(5)*V2S/(C2-V2S))
        ZZ  = (V2S/C2)*(1. - 2.*F(4)*C2/(F(3)*S*T))
        V4S = C2*(1. - (DN/D)*(1. - ZZ))
        V2S = SQRT(V2S)
        V4S = SQRT(V4S)
*  GM is fitted to the data of Srinivasan and Hofmann
        GM  = 383.7 + 10.649*DN
*  Asymtotic form approaching TL is given by Frederking
        IF (GM .GT. (1413.+12.4*(D-160.))) GM = 558.9/RSR
        COND = S * ((DS*S*T)**3) / (GM*DN)
* Output data
        CHEII(1) = RSR
        CHEII(2) = V2S
        CHEII(3) = V4S
        CHEII(4) = GM
        CHEII(5) = COND
      ELSE
        CHEII(1) = 0.D0
        CHEII(2) = 0.D0
        CHEII(3) = 0.D0
        CHEII(4) = 0.D0
        CHEII(5) = 0.D0
      ENDIF
      END
C
C
C
      DOUBLE PRECISION FUNCTION RSRFPT (P, T)
* (C) Copyright (1990), Cryodata Inc.; all rights reserved.
*
* Rhos/Rho = superfluid density fraction, from Maynard, PR B14, 3868 (1976)
* as a function of P [Pascal] and T [K].
* Maynard's data includes the range T/Tlambda(P) > 0.55
      IMPLICIT DOUBLE PRECISION (A-H, O-Z)
      DIMENSION A(3)
      DATA A / -0.2084035690D+01, 0.1760235312D+01, -0.1469764627D+01/
      X = 1.D0 - T / TLFP (P)
      IF (X .LE. 0.D0) THEN
         RSRFPT = 0.
      ELSE IF (X .LE. 0.45D0) THEN
         RSRFPT = A(1)*X*LOG(X) + X*X*(A(2) + X*A(3))
      ELSE IF (X .LT. 1.D0) THEN
* The following is an approximate empirical fit beyond Maynard's data
         RSRFPT = 1.D0 - 0.02863D0 * ((1.D0-X)/0.55D0)**6
      ELSE
         RSRFPT = 1.D0
      ENDIF
      END
C
C
C
      SUBROUTINE AMLAP (TMINM, TMAXA, D)
* (C) Copyright (1990), Cryodata Inc.; all rights reserved.
*
* Computes the overlap temperatures between Arp and McCarty equations
* as a function of density [kg/m3].
* TMINM is the minimum temperature for McCarty (in compressed liquid).
* TMAXA is the maximum temperature for Arp
      IMPLICIT DOUBLE PRECISION (A-H, O-Z)
      DATA TM, TA /2.53, 2.98/, DMIN, DMAX /140., 190./
      IF ((D .LT. DMIN) .OR. (D .GT. DMAX)) THEN
         TMINM = 0.
         TMAXA = 0.1
      ELSE
*        Note: -0.28 [K] / (DMAX-DMIN) = -0.0056 = slope of boundary line
         DT = -0.0056 * (D-DMIN)
         TMINM = TM + DT
         TMAXA = TA + DT
         IF (D .GT. 180.) THEN
            DT = -0.035 * (D-180.)
            TMINM = TMINM + DT
            TMAXA = TMAXA + DT
         ENDIF
      ENDIF
      END
C
C
C
      DOUBLE PRECISION FUNCTION D2LFPT (PSAT, TSAT)
C (C) Copyright (1990), Cryodata Inc.; all rights reserved.
C
C Saturated liquid density, 0.8 to 5.1953 K, T76 scale.
C Output is the density such that PRESS(D,T) = PSAT; will differ slightly
C from the density DFSAT, except when T > TNRC.
C Valid input of both PSAT and TSAT must be made by the calling program.
C Version Nov. 8, 1990
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      PARAMETER (TNRC=5.1060D0, TLOW=4.804D0, DELT=TNRC-TLOW)
      COMMON /PRECSN/ E(0:15)
      DATA P0 /5000.D0/
      DL   = DFSAT (TSAT)
      IF (TSAT .LT. TNRC) THEN
         D   = DL
         DPD = DPDD  (D,TSAT)
         DO 10 K = 1, 8
            DELP = PRESS (D,TSAT) - PSAT
            D = D - DELP/DPD
            IF (ABS (DELP/(PSAT+P0)) .LT. E(12)) GO TO 20
   10    CONTINUE
   20    CONTINUE
         IF (TSAT .GT. TLOW) THEN
            DL = (D*(TNRC-TSAT) + DL*(TSAT-TLOW))/DELT
         ELSE
            DL = D
         ENDIF
      ENDIF
      D2LFPT = DL
      END
C
C
C
      DOUBLE PRECISION FUNCTION DFSAT (T)
C (C) Copyright (1990), Cryodata Inc.; all rights reserved.
C
C Density of saturated liquid [kg/m3]; 0.8 < T < 5.1953; T76 scale.
C This is the best independent estimate of saturation density;
C However, it is not exactly consistent with function D2LFPT.
C HeI range above TNRC:  theoretical form, fitted for continuity at TNRC
C HeI range above 3.2 K: R.D.McCarty equation May 4, 1988.
C HeI range below 3.2 K: Van Degrift' s equation, quoted by Barenghi
C     Lucas and Donnelly, J. Low Temp. Physics 44, 491 (1981),
C     shifted to the T76 temperature scale.
C
C HeII range: V.Arp equation Dec 16, 1987.
C Version Nov. 8, 1990
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      PARAMETER (ONE3RD=1.D0/3.D0)
      DIMENSION A(6), B(4), C(5)
      DATA TTP, TBB, TCC, DCC /2.1768D0, 3.1853D0, 5.1953D0, 17.399D0/,
     &    DTPL /36.514D0/, DEN0 /145.188D0/,   DENL /146.15D0/,
     &    TNRC /5.1060D0/, DNRC /93.3275D0/
      DATA A / -.3434500882D+02,  .4501470666D+00, -.6895460170D+01,
     A          .5442223002D+02, -.9716273774D+02,  .1785061367D+02/
      DATA B / -0.1297822470D+01, -0.4911239491D+01,
     B          0.1021276082D+01, -0.2626058542D+01/
      DATA C / -0.2433560558D+03, 0.1814476908D+04, 0.2425346403D+04,
     C         -0.4604869802D+03, 0.2453561962D+03/
      IF (T .GE. TBB) THEN
         IF (T .GT. TNRC) THEN
            P = (MAX (1.D-20, ((TCC-T)/(TCC-TNRC))))**ONE3RD
            D = DCC + P*(DNRC/4.0026D0-DCC)
         ELSE
            X = (T-TCC)/(TTP-TCC)
            Y = X**ONE3RD
            Z = 1.D0/X
            P = A(1) * LOG(X)
            DO 10 J = 2, 6
            P = P + A(J)*(1.D0-Z)
            IF (J .EQ. 4) THEN
               Z = Y
            ELSE
               Z = Z*Y
            ENDIF
   10       CONTINUE
            D = DCC + EXP(P)*(DTPL-DCC)
         ENDIF
         DFSAT = D * 4.0026D0
      ELSE
         Z = T - TTP
         IF (Z .GE. 0.) THEN
            DFSAT = DENL + Z*(B(2) + Z*(B(3) +  Z*B(4)))
            IF (Z .NE. 0.D0) DFSAT = DFSAT + B(1)*Z*LOG(Z)
         ELSE
            Y = Z * (LOG(-Z) - 1.D0)
            P = T*T
            DFSAT = ((P*P*(C(1)*Y + C(2)*Z + C(3) + C(4)*P)
     &               +C(5)*P) * 1.D-06 + 1.D0) * DEN0
         ENDIF
      ENDIF
      END
C
C
C
      DOUBLE PRECISION FUNCTION D2VFPT (PSAT, TSAT)
C (C) Copyright (1990), Cryodata Inc.; all rights reserved.
C
C Saturated vapor density, 0.8 to 5.1953 K, T76 scale.
C Output is the density such that PRESS(D,T) = PSAT; may differ slightly
C from the density DGSAT, except when T > TNRC.
C Valid input of both PSAT and TSAT must be made by the calling program.
C Version Nov. 6, 1990
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      PARAMETER (TNRC=5.1060D0, TLOW=4.804D0, DELT=TNRC-TLOW)
      COMMON /PRECSN/ E(0:15)
      DATA P0 /5000.D0/
      DV = DGSAT (TSAT)
      IF (TSAT .LT. TNRC) THEN
         D   = DV
         DPD = DPDD  (D,TSAT)
         DO 10 K = 1, 8
            DELP = PRESS (D,TSAT) - PSAT
            D = D - DELP/DPD
            IF (ABS (DELP/(PSAT+P0)) .LT. E(12)) GO TO 20
   10    CONTINUE
   20    CONTINUE
         IF (TSAT .GT. TLOW) THEN
            DV = (D*(TNRC-TSAT) + DV*(TSAT-TLOW))/DELT
         ELSE
            DV = D
         ENDIF
      ENDIF
      D2VFPT = DV
      END
C
C
C
      DOUBLE PRECISION FUNCTION DGSAT (T)
C (C) Copyright (1990), Cryodata Inc.; all rights reserved.
C
C Saturated vapor density, 0.8 < T < 5.1953.
C This is the best independent estimate of saturation density;
C Theoretical form above TNRC, fitted for continuity at TNRC.
C Version Nov. 8, 1990
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      PARAMETER (ONE3RD=1.D0/3.D0)
      DIMENSION A(8), C(8)
      DATA A /-.5618978079D+03,  .3300736785D+01, -.6031200561D+02,
     A         .6129901560D+03, -.2718577178D+04,  .1285185020D+04,
     B        -.4406873907D+03,  .7163145577D+02/
      DATA C /-7.41816D0,   5.42128D0,   9.903203D0,  -9.617095D0,
     C         6.804602D0, -3.0154606D0, 0.7461357D0, -0.0791791D0/
      DATA TTP /2.1768D0/, TCC /5.1953D0/, DCC /17.399D0/,
     &    DTPV /.294058864D0/, TNRC /5.106D00/, DNRC /46.5087D0/
      IF (T .GT. TTP) THEN
         IF (T .GT. TNRC) THEN
            P = (MAX (1.D-20, ((TCC-T)/(TCC-TNRC))))**ONE3RD
            D = DCC + P*(DNRC/4.0026D0 - DCC)
         ELSE
            X = (T-TCC)/(TTP-TCC)
            Y = X**ONE3RD
            Z = 1.D0/X
            P = A(1) * LOG(X)
            DO 10 J = 2, 8
            P = P + A(J)*(1.D0-Z)
            IF (J .EQ. 4) THEN
               Z = Y
            ELSE
               Z = Z*Y
            ENDIF
   10       CONTINUE
            D=DCC + EXP(P)*(DTPV-DCC)
         ENDIF
         DGSAT = D * 4.0026D0
      ELSE IF (T .GT. 0.799) THEN
         P = C(1)/T + C(2)
         Y = 1.D0
         DO 20 J = 3, 8
         Y = Y*T
   20    P = P + C(J)*Y
         D = EXP(P) / (2077.2258D0*T)
         Z = (0.0537D0 - 0.514D0/T) / 4.0026D0
         DGSAT = D /(1.+Z*D) + 0.00164D0*D**3
      ELSE
         DGSAT = -1.
      ENDIF
      END
C
C
C
C-----Version 3.301: local variable DPDT renamed XDPDT
      SUBROUTINE KIERST (P, XDPDT, D, DDDT, T)
* (C) Copyright (1990), Cryodata Inc.; all rights reserved.
*
*-----OUTPUT
* P    = Lambda-line pressure          [Pa]
*      = 5041.8 at T=2.1768
* DPDT = dP/dT along the lambda line   [Pa/K]
* D    = Density at the lambda line    [kG/M3]
*      = 146.15 at T=2.1768
* DDDT = dD/dT along the lambda line   [kG/(M3-K)]
*-----INPUT
* T    = temperature  (T76 scale)      [K]
*        valid range is 2.1768 to 1.7673
*-----REFERENCE
* H.A. Kierstead, Phys Rev 162, 153 (1967); constants revised for T76 scale
* Note: Kierstead's density equation disagrees slightly with HEPAK,
* and is not used by HEPAK.
*-----
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      DIMENSION A(7), B(7)
      DATA A / .42774167, -94.820469, -85.817089, -102.39597,
     1        -76.735240, -.37798315,  42.148155/
      DATA B / .14841216,  -.15036724, -.32811465, -.52635312,
     2        -.37937084,  -.00226216, 36.645230/
      X    = T - 2.1768
      EX   = EXP (A(7)*X)
      P    = (A(1) + X*(A(2) + X*(A(3) + X*(A(4) + X*A(5))))
     A       + A(6)*EX) * 101325.
C-----Version 3.301: local variable DPDT renamed XDPDT
      XDPDT = (A(2) + X*(2.*A(3) + X*(3.*A(4) + X*4.*A(5)))
     A       + A(6)*A(7)*EX) * 101325.
      EX   = EXP (B(7)*X)
      D    = (B(1) + X*(B(2) + X*(B(3) + X*(B(4) + X*B(5))))
     B       + B(6)*EX) * 1000.
      DDDT = (B(2) + X*(2.*B(3) + X*(3.*B(4) + X*4.*B(5)))
     B       + B(6)*B(7)*EX) * 1000.
      RETURN
      END
C
C
C
      DOUBLE PRECISION FUNCTION D175K (PASCAL)
* (C) Copyright (1990), Cryodata Inc.; all rights reserved.
*
* density [kg/m3] of HeII as a function of P [Pa] at T(58)=1.75
* accuracy probably about 0.2%.
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      DIMENSION C(6)
      DATA C/-0.8129582813E+00,  0.1859901260E+02, -0.6344068036E+01,
     A        0.2844939685E+01, -0.8216755033E+00,  0.1044392629E+00/
      DATA DEN0 /146.081/
      P = 1.E-06*PASCAL
      D175K = DEN0 + C(1) + P*(C(2) + P*(C(3) + P*(C(4) + P*
     1        (C(5) + P*C(6)))))
      END
C
C
C
      DOUBLE PRECISION FUNCTION PLFT (T)
* (C) Copyright (1990), Cryodata Inc.; all rights reserved.
*
* P = Lambda-line pressure = 5041.8 Pa    at T = 2.1768 K
*                          = 30.134E+5 Pa at T = 1.7673 K
* reference:
* H.A. Kierstead, Phys Rev 162, 153 (1967) (T58 scale)
* refitted to T76 scale.
      IMPLICIT DOUBLE PRECISION (A-H, O-Z)
      DIMENSION B(7)
      DATA B / .42774167, -94.820469, -85.817089, -102.39597,
     1        -76.735240, -.37798315,  42.148155/
      X = T - 2.1768D0
      P=B(1)+(B(2)+(B(3)+(B(4)+B(5)*X)*X)*X)*X+B(6)*EXP(B(7)*X)
      PLFT=P*101325.
      END
C
C
C
      DOUBLE PRECISION FUNCTION PMFT (TT)
* (C) Copyright (1992), Cryodata Inc.; all rights reserved.
*
*  melting pressure as a function of temperature, T76 scale below 5.1953
*  F1 to F3 have been refitted to T58 equations of Grilly and Mills.
*  F4 and F5 have been fitted to Grilly and Mills data.
      IMPLICIT DOUBLE PRECISION (A-H, O-Z)
      DATA CONV /98066.5D0/, CON2 /101325.D0/, DELT /0.005D0/
      F1 (X) = (-17.80+17.31457*X**1.555414)*CONV
      F2 (X) = (34.2097 + X*(-45.31231 + X*(32.26926 + X*
     A          (-4.91282 + X*0.310795)))) * CONV
      F3 (X) = (17.8537 + X*(-15.49444 + X*12.57562)) * CON2
      F4 (X) = (99.3328 + X*(-101.44970 + X*35.1175)) * CON2
      F5 (X) = (24.997 + 3.36930 * (X-0.725)**4) * CON2
      T = TT
* Cut off at the upper limit of HEPAK
      IF (T .GT. 14.0104D0) THEN
         PMFT = 1013.25E+05
      ELSE IF (T .GE. 5.1953 + DELT) THEN
         PMFT = F1 (T)
      ELSE IF (T .GT. 5.1953 - DELT) THEN
         W    = (T - 5.1953)/(2.*DELT) + 0.5
         PMFT = W*F1(T) + (1.-W)*F2(T)
      ELSE IF (T .GE. 2.0044 + DELT) THEN
         PMFT = F2 (T)
      ELSE IF (T .GT. 2.0044 - DELT) THEN
         W    = (T - 2.0044)/(2.*DELT) + 0.5
         PMFT = W*F2(T) + (1.-W)*F3(T)
      ELSE IF (T .GE. 1.7660 + DELT) THEN
         PMFT = F3 (T)
      ELSE IF (T .GT. 1.7660 - DELT) THEN
         W    = (T - 1.7660)/(2.*DELT) + 0.5
         PMFT = W*F3(T) + (1.-W)*F4(T)
      ELSE IF (T .GE. 1.4676 + DELT) THEN
         PMFT = F4 (T)
      ELSE IF (T .GT. 1.4676 - DELT) THEN
         W    = (T - 1.4676)/(2.*DELT) + 0.5
         PMFT = W*F4(T) + (1.-W)*F5(T)
      ELSE IF (T .GE. 0.8D0) THEN
         PMFT = F5 (T)
      ELSE
         PMFT = -1.
      ENDIF
      END
C
C
C
      SUBROUTINE PSATFT (P, DPDTS, T)
* (C) Copyright (1992), Cryodata Inc.; all rights reserved.
*
*  saturation pressure and dP/dT as a function of temperature
*  on the T76 scale, from 0.5 to 5.1953 K; Equation is given by
*  Durieux and Rusby, Metrologia 19, 67 (1983).
C
* Note: c(11,1) has been corrected in this version, August 19, 1992.
C Earlier versions of this routine had c(11,1)=14.5333d0 (missing a "3");
C the corresponding error in pressure is never more than 0.01 pct.
C
      IMPLICIT DOUBLE PRECISION (A-H, O-Z)
      DIMENSION C(11,2)
      DATA C/             -30.93285D0,   392.47361D0,  -2328.04587D0,
     1   8111.30347D0, -17809.80901D0, 25766.52747D0, -24601.4D0,
     2  14944.65142D0,  -5240.36518D0,   807.93168D0,     14.53333D0,
     3     -7.41816D0,      5.42128D0,    9.903203D0,     -9.617095D0,
     4     6.804602D0,   -3.0154606D0,   0.7461357D0,     -0.0791791D0,
     5     0.D0,          0.D0,          0.D0/
* at TL, P=5041.8 and dPdT=12407.9; fixed points on the T76 scale
C Note: Durieux and Rusby state Pc=227.46 kPa at 5.1953 K
C However, this (corrected) equation gives Pc=227.4623 kPa at 5.1953 K.
      DATA TL, TC, TMIN /2.1768D0, 5.1953D0, 0.5D0/
      IF ((T .GT. TC+1.D-05) .OR. (T .LT. TMIN)) THEN
         P     = -1.
         DPDTS = -1.
         RETURN
      ELSE IF (T .GT. TL) THEN
         X  = MIN (T/TC, 1.D0)
         M  = 1
         MX = 10
      ELSE
         X  = T
         M  = 2
         MX = 8
      ENDIF
      Q0 = C(1,M)/X + C(2,M)
      Q1 = -C(1,M)/(X*X)
      TN = 1.D0
      DO 10 J = 3, MX
      Q1 = Q1 + C(J,M)*TN*DBLE(J-2)
      TN = TN*X
   10 Q0 = Q0 + C(J,M)*TN
      X  = 1.D0 - X
      IF ((M .EQ. 1) .AND. (X .GT. 0.D0)) THEN
         Y  = X**0.9D0
         Q0 = Q0 +     X*C(11,M)*Y
         Q1 = Q1 - 1.9D0*C(11,M)*Y
      ENDIF
      P     = EXP (Q0)
      DPDTS = P * Q1
      IF (M .EQ. 1) DPDTS = DPDTS / TC
      RETURN
      END
C
C
C
      DOUBLE PRECISION FUNCTION PSAT (T)
      IMPLICIT DOUBLE PRECISION (A-H, O-Z)
      TT   = T
C-----Version 3.301: local variable DPDT renamed XDPDT
      CALL PSATFT (P, XDPDT, TT)
      PSAT = P
      END
C
C
C
      DOUBLE PRECISION FUNCTION DLFT (TT)
* (C) Copyright (1992), Cryodata Inc.; all rights reserved.
*
* Lambda line density as a function of temperature, T76 scale
* valid range is 2.1768 to 1.7673
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      COMMON /PRECSN/ E(0:15)
      EXTERNAL TLFD
      T = TT
      CALL ROOTAZ (D, 146.14D0, 147.D0, 155.D0, 182.D0, E(8), E(9),
     $            E(11), E(13), TLFD, T, JX)
      IF (JX .LE. 0) THEN
         DLFT = -1.
      ELSE
         DLFT = D
      ENDIF
      END
C
C
C
      DOUBLE PRECISION FUNCTION DLFP (P)
*  lambda line density [kg/m3] as a function of pressure [Pa]
      IMPLICIT DOUBLE PRECISION (A-H, O-Z)
      T = TLFP (P)
      DLFP = DLFT (T)
      END
C
C
C
      DOUBLE PRECISION FUNCTION DMFP (P)
*-----melting density as a function of pressure
      IMPLICIT DOUBLE PRECISION (A-H, O-Z)
      T = TMFP (P)
      DMFP = DMFT (T)
      END
C
C
C
      DOUBLE PRECISION FUNCTION DMFT (T)
* (C) Copyright (1990), Cryodata Inc.; all rights reserved.
*
* Liquid density at the melting line [kg/m3] as a function of T [K]
* Range 0.8 to 14.0 K; accuracy generally better than 0.3 kg/m3
* Version March 22, 1989.
      IMPLICIT DOUBLE PRECISION (A-H, O-Z)
      DIMENSION CL(3), CM(3), CH(4)
      DATA CL / 0.1679361577D+03, 0.6728283584D+01, -0.1018218341D+02/
      DATA CM / 0.1469940814D+03, 0.1858136626D+02, -0.1497696476D+01/
      DATA CH / 0.1542809083D+03, 0.1890207406D+02,
     H         -0.8746341757D+00, 0.2235656147D-01/
      DATA T1, T2 /1.7673D0, 3.5300D0/
      IF (T .GT. T2) THEN
         DMFT = CH(1) + T*(CH(2) + T*(CH(3) + T*CH(4)))
      ELSE
         Z = T - T1
         IF (ABS (Z) .LT. 1.D-04) THEN
            DMFT = 0.D0
         ELSE
            DMFT = Z * LOG (ABS(Z))
         ENDIF
         IF (Z .GE. 0.D0) THEN
            DMFT = CM(3)*DMFT + CM(1) + T*CM(2)
         ELSE
            DMFT = CL(3)*DMFT + CL(1) + T*CL(2)
         ENDIF
      ENDIF
      END
C
C
C
      DOUBLE PRECISION FUNCTION TLFD (D)
* (C) Copyright (1990), Cryodata Inc.; all rights reserved.
* lambda line temperature as a function of density
      IMPLICIT DOUBLE PRECISION (A-H, O-Z)
      COMMON /SUBLAM/ TL, DUM(3)
      VCGS = 1000./D
      CALL LAMDER (VCGS)
      TLFD = TL
      END
C
C
C
      DOUBLE PRECISION FUNCTION TLFP (P)
* (C) Copyright (1992), Cryodata Inc.; all rights reserved.
* lambda line temperature as a function of pressure [Pa]
      IMPLICIT DOUBLE PRECISION (A-H, O-Z)
      COMMON /PRECSN/ E(0:15)
      EXTERNAL PLFT
      PP = P
C T=2.17725 corresponds to P=0
      CALL ROOTAZ (TT, 2.17725D0, 2.1768D0, 2.D0, 1.7672D0,
     +      E(9), 0.D0, E(2), E(11), PLFT, PP, JR)
      TLFP = TT
      END
C
C
C
      FUNCTION TMFD (DD)
* (C) Copyright (1992), Cryodata Inc.; all rights reserved.
      IMPLICIT DOUBLE PRECISION (A-H, O-Z)
      COMMON /PRECSN/ E(0:15)
      EXTERNAL DMFT
      D = DD
      X = (D-171.2)/136.
      TA = MAX (1.D0, 0.8+13.2*X)
      TB = 2.D0 + MIN (TA, 12.D0)
      TA = TB - 2.D0
      CALL ROOTAZ (T, 0.8D0, TA, TB, 14.011D0, E(9), 0.D0, E(5), E(10),
     +      DMFT, D, JX)
      IF (JX .LT. 0) T=0.8D0
      TMFD = T
      END
C
C
C
      DOUBLE PRECISION FUNCTION TMFP (PP)
* (C) Copyright (1992), Cryodata Inc.; all rights reserved.
* melting temperature as a function of pressure [Pa]
      IMPLICIT DOUBLE PRECISION (A-H, O-Z)
      COMMON /PRECSN/ E(0:15)
      EXTERNAL PMFT
      P = PP
      CALL ROOTAZ (TT, 0.8D0, 2.D0, 8.D0, 14.0104D0,
     +      E(8), E(10), E(2), E(10), PMFT, P, JX)
      IF (JX .LE. 0) THEN
         TMFP = -1.
      ELSE
         TMFP = TT
      ENDIF
      END
C
C
C
      DOUBLE PRECISION FUNCTION TSATFP (PP)
* (C) Copyright (1992), Cryodata Inc.; all rights reserved.
* saturation temperature as a function of pressure
      IMPLICIT DOUBLE PRECISION (A-H, O-Z)
      COMMON /PRECSN/ E(0:15)
      EXTERNAL PSAT
      DATA TA, TZ /0.8D0, 5.1953D0/
      P = MAX (PP, 1.475154D0)
      PL = LOG(P)
      TB = 0.6985201046D0 + PL*(0.2654401757D0 + PL*
     +      (-0.5434555937D-01 + PL*0.5045477179D-02))
      TB = MAX (TA, TB-0.04D0)
      TC = MIN (TZ, TB+0.08D0)
      CALL ROOTAZ (T, TA, TB, TC, TZ,
     +      E(10), E(13), E(3), E(10), PSAT, P, JR)
      IF (JR .LE. 0) THEN
         TSATFP = -1.
      ELSE
         TSATFP = T
      ENDIF
      END
C
C
C
      DOUBLE PRECISION FUNCTION DENMAX (T)
* (C) Copyright (1990), Cryodata Inc.; all rights reserved.
*
*-----OUTPUT
* DENMAX = Density [kG/m3] ; maximum density for which
*          the equations are valid (melting pressure to 14 K;
*          1000 atmos to 75 K, 20000 bars to 300 K, 1000 atmos to 1500 K)
*-----INPUT
* T  = Temperature [K] ; 0.8 < T < 1500.
* Accuracy of fit: better than 0.3 kg/m3 for all T
*-----
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      DIMENSION A(4), B(4), C(3)
      DATA A / -0.1670040531E+02,  0.4857565167E+03,
     A         -0.2585274317E+03,  0.5161716546E+02/
      DATA B /  0.2924157352E+03,  0.1202253290E+04,
     B         -0.1859861872E+04,  0.1060045153E+04/
      DATA C /  0.9714767056E+00,  0.4745050407E+03, -0.3028968312E+03/
      IF (T .LT. 14.05) THEN
         D = DMFT (T)
      ELSE
         X = 100./(T+50.)
         IF (T .LT. 74.99) THEN
            D = A(1) + X*(A(2) + X*(A(3) + X*A(4)))
         ELSE IF (T .LT. 300.01) THEN
            D = B(1) + X*(B(2) + X*(B(3) + X*B(4)))
         ELSE
            D = C(1) + X*(C(2) + X*C(3))
         ENDIF
      ENDIF
* Add a little for error tolerance.
      DENMAX = D + 0.5
      END
C
C
C
      DOUBLE PRECISION FUNCTION H3K (P)
* (C) Copyright (1990), Cryodata Inc.; all rights reserved.
*
*-----OUTPUT
* H3K = Enthalpy [J/kG] at 3 K, for P > P(sat)
*-----INPUT
* P   = Pressure [Pa] ; fitted to 70 atmospheres
*-----ACCURACY
* Accuracy about +/- 4.  J/kG for pressures below 2 bars
*          about +/- 25. J/kG for pressures above 2 bars
*-----Version Sept 18, 1987
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      DIMENSION C(4)
      DATA C / 0.4954376389E+04, -0.1302167945E+03, 0.6571877607E+03,
     A        -0.1382310121E+02/
      IF (P .LE. 0.) THEN
         H3K = -1.
      ELSE
         Z   = SQRT(1.E-05*P)
         H3K = C(1)+Z*(C(2)+Z*(C(3)+Z*C(4)))
      ENDIF
      END
C
C
C
      DOUBLE PRECISION FUNCTION S3K (P)
* (C) Copyright (1990), Cryodata Inc.; all rights reserved.
*
*-----OUTPUT
* S3K = Entropy [J/(kG-K)] at 3 K, for P > P(sat)
*-----INPUT
* P   = Pressure [Pa] ; fitted to 70 atmospheres
*-----ACCURACY
* Accuracy about +/- 1. J/(kG-K) for pressures below 2 bars
*          about +/- 8. J/(kG-K) for pressures above 2 bars
*-----Version Sept 18, 1987
      IMPLICIT DOUBLE PRECISION (A-H, O-Z)
      DIMENSION C(5)
      DATA C /0.2365990305E+04, -0.3012076857E+02, -0.3492403215E+02,
     A        0.5141720002E+01, -0.2612678040E+00/
      IF (P .LE. 0.) THEN
         S3K = -1.
      ELSE
         Z   = SQRT(1.E-05*P)
         S3K = C(1)+Z*(C(2)+Z*(C(3)+Z*(C(4)+Z*C(5))))
      ENDIF
      END
C
C
C
      DOUBLE PRECISION FUNCTION U3K (P)
* (C) Copyright (1990), Cryodata Inc.; all rights reserved.
* This function written by V. Arp.
*
*-----OUTPUT
* U3K = Internal Energy [J/kG] at 3 K, for P > P(sat)
*-----INPUT
* P   = Pressure [Pa] ; fitted to 70 atmospheres
*-----ACCURACY
* Accuracy about +/- 4.  J/kG for pressures below 2 bars
*          about +/- 10. J/kG for pressures above 2 bars
*-----Version Sept 18, 1987
      IMPLICIT DOUBLE PRECISION (A-H, O-Z)
      DIMENSION C(5)
      DATA C/ 0.4947998498E+04, -0.1008309679E+03, -0.1066139445E+03,
     A        0.2995380841E+02, -0.1536198940E+01/
      IF (P .LE. 0.) THEN
         U3K = -1.
      ELSE
         Z   = SQRT(1.E-05*P)
         U3K = C(1)+Z*(C(2)+Z*(C(3)+Z*(C(4)+Z*C(5))))
      ENDIF
      END
C
C
C
      DOUBLE PRECISION FUNCTION D3K (P)
* (C) Copyright (1990), Cryodata Inc.; all rights reserved.
*
*-----OUTPUT
* D3K = Density [kG/m3] at 3 K, for P > P(sat)
*-----INPUT
* P   = Pressure [Pa] ; fitted to 74 bars
*-----ACCURACY
* Accuracy about +/- 0.1 kG/m3 for pressures below 15 bars
*          about +/- 0.3 kG/m3 for pressures above 15 bars
*-----Version Sept 18, 1987
      IMPLICIT DOUBLE PRECISION (A-H, O-Z)
      DIMENSION C(5)
      DATA C/ 0.1398535389E+03, 0.1496220385E+01, 0.2334289084E+01,
     A       -0.3419086874E+00, 0.1716145720E-01/
      IF (P .LE. 0.) THEN
         D3K = -1.
      ELSE
         Z   = SQRT(1.E-05*P)
         D3K = C(1)+Z*(C(2)+Z*(C(3)+Z*(C(4)+Z*C(5))))
      ENDIF
      END
C
C
C
C-----Version 3.301: local variable DPDT renamed XDPDT
      SUBROUTINE SATFS (IDID, QUAL, PS, XDPDT, TS, DL, DV, SS)
* (C) Copyright (1992), Cryodata Inc.; all rights reserved.
*
* Quality, saturated pressure, temperature, and densities as a function
* of (saturated) entropy.  SI units.  IDID=2 when successful.
C     Range 0.8 to 5.1953 K
*-----Version Oct. 17, 1992
      IMPLICIT DOUBLE PRECISION (A-H, O-Z)
      COMMON /SUBRT/ V(13), JV
      COMMON /PRECSN/ E(0:15)
      EXTERNAL SATS
      DATA SC /5768.4D0/, TC /5.1953D0/, TMIN /0.8D0/
      S = SS
      IF (S .GE. SC) THEN
         JV = 2
         TX = TSATSV (S)
      ELSE
         JV = 1
         TX = TSATSL (S)
      ENDIF
      TA = MIN (MAX (TX, 0.81D0), 5.12D0)
      TB = TA + 0.06D0
      CALL ROOTAZ (TT, TMIN, TA, TB, TC, E(9), 0.D0,
     +      E(5), E(8), SATS, S, JX)
      IF (JX .LT. 0) THEN
         IDID = -124
      ELSE
         IDID = 2
         TS   = TT
         PS   = V(1)
C-----Version 3.301: local variable DPDT renamed XDPDT
         XDPDT = V(2)
         IF (JV .EQ. 2) THEN
            DV = V(3)
            DL = D2LFPT (PS, TS)
            QUAL = 1.
         ELSE
            DL = V(3)
            DV = D2VFPT (PS, TS)
            QUAL = 0.
         ENDIF
      ENDIF
      END
C
C
C
      DOUBLE PRECISION FUNCTION SATS (T)
* (C) Copyright (1992), Cryodata Inc.; all rights reserved.
* function used by SATFS
      IMPLICIT DOUBLE PRECISION (A-H, O-Z)
      COMMON /SUBRT/ V(13), JV
      CALL PSATFT (P, XDPDT, T)
      IF (JV .EQ. 1) THEN
         D = D2LFPT (P, T)
      ELSE
         D = D2VFPT (P, T)
      ENDIF
      SATS = ENTROP (D, T)
      V(1) = P
      V(2) = XDPDT
      V(3) = D
      END
C
C
C
      FUNCTION TSATSL (SJKG)
C Saturated liquid Temperature [K] as a function of entropy [J/kg-K]
C Valid for 4.7128 < SJKG < 5768.4 J/kg-K, corresponding to
C           0.8 < T < 5.1953 K
C Fitted to HEPAK data, Oct 17, 1992; rms accuracy (+-) 0.035 K
      IMPLICIT DOUBLE PRECISION (A-H, O-Z)
      DIMENSION C(6)
      DATA C / 0.2017414715E+01, -0.1192464994E+01,  0.2441002711E+01,
     +        -0.1804761829E+01,  0.5800360350E+00, -0.6892288428E-01/
      DATA E/0.1736D0/
      S = MAX ((SJKG*1.D-3), 0.0047D0)
      S2=SQRT (S)
      TSATSL = C(1)*(S**E) + S**4*(C(2) + S2*(C(3) + S2*(C(4) +
     +      S2*(C(5) + S2*C(6)))))
      END
C
C
C
      FUNCTION TSATSV (SJKG)
C Saturated vapor Temperature [K] as a function of entropy [J/kg-K]
C Valid for 5768.4 < SJKG < 23936 J/kg-K, corresponding to
C           5.1953 > T > 0.8 K
C Fitted to HEPAK data, Oct 17, 1992; rms accuracy (+-) 0.035 K
      IMPLICIT DOUBLE PRECISION (A-H, O-Z)
      DIMENSION C(6)
      DATA C/-0.2621235672E+02,  0.2431127551E+02, -0.1377701570E+02,
     +        0.3157246354E+01, -0.3331317936E+00,  0.1342914271E-01/
      DATA E/0.6D0/
      S = (SJKG*1.D-3)**E
      TSATSV = C(1)+S*S*(C(2)+S*(C(3)+S*(C(4)+S*(C(5)+S*C(6)))))
      END
C
C
C
      SUBROUTINE SATFD (IDID, QUAL, PSAT, DPDT, TSAT, DL, DV, RHO)
* (C) Copyright (1992), Cryodata Inc.; all rights reserved.
* This subroutine written by V. Arp.
*
* Output: saturation properties: temperature, pressure, dP/dT,
*         liquid and vapor densities (one of these will be a repeat
*         of the input parameter, RHO, all SI units)
* Input:  RHO = saturated fluid density.
* NOTE: The maximum saturated liquid density of 146.1603 occurs at
C       T = 2.1852 K, according the HEPAK v3.2.  In the liquid, SATFD
C       always returns a temperature T > 2.1852; the saturated superfluid
C       branch is not found in the following iteration.
C Version August 29, 1992
      IMPLICIT DOUBLE PRECISION (A-H, O-Z)
      COMMON /SUBRT/ R(13), JR
      COMMON /PRECSN/ E(0:15)
      EXTERNAL SATD, SATD1
      DATA DELT, DNRCL, DNRCV /0.0893D0, 93.3275D0, 46.5087D0/
C          DELT = TB - TNRC;  TNRC is defined in DFSAT and DGSAT
      DATA TA, TZ /0.8D0, 5.1953D0/, DC /69.6412D0/
      IF ((RHO .GT. 146.1603D0) .OR. (RHO .LT. 0.888D-03)) THEN
         IDID = -123
         RETURN
      ENDIF
      IF (RHO .GT. DNRCL) THEN
         JR = 0
         TB = 2.1852D0
         TC = 4.5D0
      ELSE IF (RHO .GT. DNRCV) THEN
         IF (RHO .GE. DC) THEN
            DREF = DNRCL
C           FACT = (DNRCV - DC) / (DNRCL - DC)
            FACT = -0.9766194D0
            QUAL = 0.D0
         ELSE
            DREF = DNRCV
C           FACT = (DNRCL - DC) / (DREF - DC)
            FACT = -1.0239403D0
            QUAL = 1.D0
         ENDIF
         TERM = MAX (ABS((RHO-DC)/(DREF-DC)), 1.D-08)
         TSAT = TZ - DELT*(TERM**3)
         CALL PSATFT (PSAT, DPDT, TSAT)
         TERM = DC + FACT*(RHO - DC)
         IF (QUAL .LT. 0.5D0) THEN
            DL = RHO
            DV = TERM
         ELSE
            DL = TERM
            DV = RHO
         ENDIF
         IDID = 2
         RETURN
      ELSE
         JR = 1
         TB = TA + 0.06D0*RHO
         TC = 0.50D0*(TB + TZ)
      ENDIF
      CALL ROOTAZ (T, TA, TB, TC, TZ, 3.D-02, 0.D0,
     +      1.D-08, 3.D-04, SATD1, RHO, JX)
      IF (JX .LE. 0) THEN
         IDID = -123
      ELSE
         TA1 = MIN (T, TZ-0.005D0)
         TB1 = MIN (T+0.005D0, TZ)
         CALL ROOTAZ (T, TA, TA1, TB1, TZ, E(8), E(10),
     +         E(15), E(9), SATD, RHO, JX)
         IF (JX .LT. 0) THEN
            IDID = -180
            RETURN
         ENDIF
         IDID = 2
           PSAT = R(1)
           DPDT = R(2)
         TSAT = T
         IF (JR .EQ. 0) THEN
            DL = RHO
            DV = D2VFPT (PSAT, TSAT)
            QUAL = 0.
         ELSE
            DV = RHO
            DL = D2LFPT (PSAT, TSAT)
            QUAL = 1.
         ENDIF
      ENDIF
      END
C
C
C
      DOUBLE PRECISION FUNCTION SATD (T)
* (C) Copyright (1992), Cryodata Inc.; all rights reserved.
* function used by SATFD
      IMPLICIT DOUBLE PRECISION (A-H, O-Z)
      COMMON /SUBRT/ R(13), JR
      CALL PSATFT (R(1), R(2), T)
      IF (JR .EQ. 0) THEN
         SATD = D2LFPT (R(1), T)
      ELSE
         SATD = D2VFPT (R(1), T)
      ENDIF
      END
C
C
C
      DOUBLE PRECISION FUNCTION SATD1 (T)
* (C) Copyright (1992), Cryodata Inc.; all rights reserved.
* function used by SATFD
      IMPLICIT DOUBLE PRECISION (A-H, O-Z)
      COMMON /SUBRT/ R(13), JR
      IF (JR .EQ. 0) THEN
         SATD1 = DFSAT (T)
      ELSE
         SATD1 = DGSAT (T)
      ENDIF
      END
C
C
C
      SUBROUTINE SATFY (IDID, QUAL, PS, DPDT, TS, DL, DV, YY, LABEL)
      IMPLICIT DOUBLE PRECISION (A-H, O-Z)
      CHARACTER*1 LABEL
      COMMON/PRINIT/RR,AMW,PCRIT,TCRIT,DCRIT,HCRIT,UCRIT,SCRIT,
     1   PTR,TTR,DTRF,DTRG,HTRF,HTRG,UTRF,UTRG,STRF,STRG,THI,DHI,
     2   HHI,UHI,SHI,CPHI,CVHI,HMAX2P,UMAX2P,PMAX,PMIN,TMAX,TMIN
      COMMON /PRECSN/ E(0:15)
      COMMON /SUBRT/ R(13), JR
      EXTERNAL SATLY, SATVY
      Y = YY
      IF (LABEL .EQ. 'S') THEN
         CALL SATFS (IDID, QUAL, PS, DPDT, TS, DL, DV, Y)
         RETURN
      ENDIF
      IF (LABEL .EQ. 'H') THEN
         YMIN  = 1.8766
         YCRIT = HCRIT
         YMAX  = HMAX2P
         TYMIN = 4.18
         JR    = 9
         IDID  = -125
      ELSE IF (LABEL .EQ. 'U') THEN
         YMIN  = 1.8665
         YCRIT = UCRIT
         YMAX  = UMAX2P
         TYMIN = 4.21
         JR    = 11
         IDID  = -127
      ELSE IF (LABEL .EQ. 'G') THEN
C Following Gibbs energies are calculated from HEPAK v3.2
         YMAX = -8021.2
         YMIN = -1.8963
         YCRIT= YMAX
         JR   = 12
         IDID = -128
      ELSE
         IDID = -299
         RETURN
      ENDIF
      IF ((Y-YCRIT)*(Y-YMIN) .LE. 0.D0) THEN
         CALL ROOTAZ (T, TMIN, 2.0D0, 4.5D0, TCRIT, E(8), 0.D0,
     +      E(11), E(9), SATLY, Y, ITER)
         IF (ITER .GT. 0) THEN
            IDID  = 2
            PS  = R(1)
            DPDT = R(6)
            TS  = T
            DL  = R(3)
            DV  = D2VFPT (R(1), T)
            QUAL= 0.
         ENDIF
      ELSE IF (((Y-YCRIT)*(Y-YMAX) .LE. 0.D0) .AND. (JR .NE. 12)) THEN
         CALL ROOTAZ (T, TYMIN, 4.5D0, 5.D0, TCRIT, E(8), 0.D0,
     +      E(11), E(9), SATVY, Y, ITER)
         IF (ITER .GT. 0) THEN
            IDID  = 2
            PS  = R(1)
            DPDT = R(6)
            TS  = T
            DV  = R(3)
            DL  = D2LFPT (R(1), T)
            QUAL= 1.
         ENDIF
      ENDIF
      END
C
C
C
      DOUBLE PRECISION FUNCTION SATLY (T)
* (C) Copyright (1992), Cryodata Inc.; all rights reserved.
*
* External function for SATFY; see it for definitions; v 1/5/93
      IMPLICIT DOUBLE PRECISION (A-H, O-Z)
      COMMON /SUBRT/ R(13), JR
      CALL PSATFT (R(1), R(6), T)
      R(3) = D2LFPT (R(1), T)
      R(2) = T
      CALL SHAUG (R)
      SATLY = R(JR)
      END
C
C
C
      DOUBLE PRECISION FUNCTION SATVY (T)
* (C) Copyright (1992), Cryodata Inc.; all rights reserved.
*
* External function for SATFY; see it for definitions; v 1/5/93
      IMPLICIT DOUBLE PRECISION (A-H, O-Z)
      COMMON /SUBRT/ R(13), JR
      CALL PSATFT (R(1), R(6), T)
      R(3) = D2VFPT (R(1), T)
      R(2) = T
      CALL SHAUG (R)
      SATVY = R(JR)
      END
C
C
C
      DOUBLE PRECISION FUNCTION T7658 (T58)
*
* T76 as a function of T58; valid from 0.8 to Tc
* RMS fitting error = 66 microdegrees.
* constrained for 0 error at T58 = 2.172 and 5.190
C This function is not used by HEPAK.  It is listed here for reference only.
      IMPLICIT DOUBLE PRECISION (A-H, O-Z)
      DIMENSION A(4), B(3), C(3)
      DATA A /-0.1891260993E-02, 0.8508937840E-02, -0.4836428596E-02,
     A         0.1076075419E-02/
*     DATA A / 0.1498489151E-02, 0.1152508836E-02,  0.1692099224E-03/
      DATA B / 0.7530201452E-02,-0.2681259464E-02,  0.9784628670E-03/
      DATA C / 0.6115602751E-02, 0.1146742367E-02, -0.2767840460E-03/
      IF (T58 .LE. 2.1720) THEN
         T7658 = T58 + A(1) + T58*(A(2) + T58*(A(3) + T58*A(4)))
      ELSE IF (T58 .LE. 4.082) THEN
         X = 1./(T58 - 1.70)
         T7658 = T58 + B(1) + X*X*(B(2) + X*B(3))
      ELSE IF (T58 .LE. 4.619) THEN
         T7658 = T58 + 0.00713
      ELSE IF (T58 .LE. 5.190) THEN
         X = 1./(5.4 - T58)
         T7658 = T58 + C(1) + X*(C(2) + X*C(3))
      ELSE
         T7658 = -1.
      ENDIF
      END
C
C
C
      DOUBLE PRECISION FUNCTION RHOBB(P,T)
* (C) Copyright (1990), Cryodata Inc.; all rights reserved.
*
      IMPLICIT DOUBLE PRECISION (A-H, O-Z)
*  Beattie-Bridgeman equation used for approximate density
      DATA A0/0.0216/ASM/.05984/B0/.014/BSM/0.0/CSM/40./
     &     RBB/.08206/AMW/4.0026/
      PI=RBB*T*101325./P
      RHOBB=AMW/((PI+B0*(1.-BSM/PI))*(1.-CSM/PI*T**(-3))
     1     -A0*(1.-ASM/PI)/(RBB*T))
      END
C
C
C
      SUBROUTINE DF2PT (IDID, D, PP, TT)
* (C) Copyright (1992), Cryodata Inc.; all rights reserved.
C This subroutine iterates for density D [Kg/m3], given P [Pa] and T [K].
C It is valid only in the compressed liquid near the lambda line, where
C it is more precise than subroutine DFPT
      IMPLICIT DOUBLE PRECISION (A-H, O-Z)
      T = TT
      P = PP
      D = D175K (T)
      IDID = 1
      DO 20 J = 1, 5
         DELP = P - PRESSA(D,T)
         DELD = DPDDA (D,T)
         IF (ABS (DELD) .LT. 1.D-08) THEN
            IDID = -207
            RETURN
         ENDIF
         DELD = DELP/DELD
         D = D + DELD
         IF (ABS(DELD) .LT. 0.001D0) RETURN
20    CONTINUE
      IDID = -207
      RETURN
      END
C
C
C
      SUBROUTINE DFPT (IDID, D, X, P, T)
* (C) Copyright (1992), Cryodata Inc.; all rights reserved.
*
*  density given pressure and temperature [SI units].
      IMPLICIT DOUBLE PRECISION (A-H, O-Z)
      COMMON/SUBRT/TSUB,RSUB(12),JR
      COMMON/PRINIT/R,AMW,PCRIT,TCRIT,DCRIT,HCRIT,UCRIT,SCRIT,
     1   PTR,TTR,DTRF,DTRG,HTRF,HTRG,UTRF,UTRG,STRF,STRG,THI,DHI,
     2   HHI,UHI,SHI,CPHI,CVHI,HMAX2P,UMAX2P,PMAX,PMIN,TMAX,TMIN
      COMMON /PRECSN/ E(0:15)
      EXTERNAL DPT
      DATA P20 /20.D+05/
      TSUB=T
      IF (P .GT. PMAX) THEN
         IDID = -102
         RETURN
      ELSE IF (P .LE. 0.) THEN
         IDID = -101
         RETURN
      ELSE IF (T .LT. 0.7999D0) THEN
         IDID = -103
         RETURN
      ENDIF
      TLINE=11.+P*1.E-6
      IF (T.GT.TLINE) THEN
* close to ideal gas
         IF (T .GT. TMAX) THEN
            IDID = -104
            RETURN
         ENDIF
         RHO=RHOBB(P,T)
         DMAX = DENMAX(T)
         DMIN = 1.D-08
         DA = MIN (RHO, DMAX)
         DB = DA*0.9
* check saturation line
      ELSE IF (T .LT. TCRIT) THEN
         IF (P .GE. P20) THEN
            DMAX = DMFT (T) + 0.3
            DA = 170.D0
            DB = 0.5D0*(DA+DMAX)
            DMIN = 150.D0
         ELSE IF (P .LT. PSAT(T)) THEN
            DA =  P / (2200. * T)
            DB = DGSAT (T)
            DMIN = 1.D-08
            DMAX = DB*1.03D0
         ELSE
            DMIN = 0.97D0*DFSAT (T)
            DB = D175K (P) + 1.
            DA = 0.5D0*(DB+DMIN)
            DMAX = 180.D0
         ENDIF
      ELSE IF (P .LT. PCRIT) THEN
         DA = P / (2077.*T)
         DMAX = 70.D0
         DB = MIN (DMAX, (2.D0*DA))
         DMIN = 1.D-08
      ELSE
         DMAX = DENMAX(T)
         DMIN = 5.
         IF (P .LT. P20) THEN
            DA = 320./T + P*2.D-05
            DB = 1.5*DA
         ELSE
            DA = MIN ((80. + P*0.4D-06), (0.7*DMAX))
            DB = 0.5*(DA+DMAX)
         ENDIF
      ENDIF
      CALL ROOTAZ (RHO, DMIN, DA, DB, DMAX, E(15), E(10), E(15),
     +      E(10), DPT, P, JX)
      IF (JX.LE.0) THEN
         IDID = -201
         IF (T .LT. 14.05) IDID = -107
      ELSE
         D = RHO
         IDID = 1
         IF (RHO .GT. DCRIT) THEN
            X = -1.
         ELSE
            X = 2.
         ENDIF
      ENDIF
      END
C
      DOUBLE PRECISION FUNCTION DPT(D)
* Function used by DFPT
      IMPLICIT DOUBLE PRECISION (A-H, O-Z)
      COMMON/SUBRT/T,R(12),JR
      DPT= PRESS (D,T)
      END
C
C
C
      SUBROUTINE DFST (IDID, RHO, X, RF, RG, P, DPDTS, S, T)
* (C) Copyright (1992), Cryodata Inc.; all rights reserved.
*
*  density given entropy and temperature [SI units].
      IMPLICIT DOUBLE PRECISION (A-H, O-Z)
      COMMON/SUBRT/TT,RSUB(12),JR
      COMMON/PRINIT/R,AMW,PCRIT,TCRIT,DCRIT,HCRIT,UCRIT,SCRIT,
     1   PTR,TTR,DTRF,DTRG,HTRF,HTRG,UTRF,UTRG,STRF,STRG,THI,DHI,
     2   HHI,UHI,SHI,CPHI,CVHI,HMAX2P,UMAX2P,PMAX,PMIN,TMAX,TMIN
      COMMON /PRECSN/ E(0:15)
      EXTERNAL DST
      IF (S*(S-64000.) .GE. 0.D0) THEN
         IDID = -108
         RETURN
      ENDIF
      IF (T .LT. TMIN) THEN
         IDID = -103
         RETURN
      ENDIF
      IF (T .GT. TMAX) THEN
         IDID = -104
         RETURN
      ENDIF
      X=2.
      TT=T
      DMAX = DENMAX (T)
      IF (T .LT. TCRIT) THEN
C-----Version 3.301: local variable PSAT renamed XPSAT (2 lines)
         CALL PSATFT (XPSAT, DUM, T)
         RG = D2VFPT (XPSAT, T)
         SG = ENTROP (RG, T)
         IF (S .GE. SG) THEN
            DMAX = 1.01*RG
            DA = RG * EXP ((SG-S)/R)
            DB = SQRT (DA*DMAX)
            DMIN = 1.D-08
         ELSE
C-----Version 3.301: local variable PSAT renamed XPSAT
            RF = D2LFPT (XPSAT, T)
            SF = ENTROP (RF, T)
            IF (S .GT. SF) THEN
               X    = (S-SF)/(SG-SF)
               RHO  = RF / (1. + X*(RF/RG - 1.))
C-----Version 3.301: local variable PSAT renamed XPSAT
               P    = XPSAT
               DPDTS= DUM
               IDID = 3
               RETURN
            ELSE
               DMIN = 0.99*RF
               DA = 0.75D0*DMIN + 0.25D0*DMAX
               DB = 0.25D0*DMIN + 0.75D0*DMAX
            ENDIF
         ENDIF
      ELSE
         DA = 1.2 * DHI*EXP((SHI-S+CVHI*LOG(T/THI))/R)
         DA = MIN (DA, DMAX)
         DMIN = 1.D-08
         DB = MAX ((0.67*DA), DMIN)
      ENDIF
      EYA = CVHI * E(10)
      CALL ROOTAZ (RHO, DMIN, DA, DB, DMAX, E(15), E(10),
     +      EYA, E(10), DST, S, JX)
      IF (JX.LE.0) THEN
         IDID = -108
      ELSE
         IF (RHO.GT.DCRIT) X=-1.
         IDID = 1
      ENDIF
      END
C
C
C
      DOUBLE PRECISION FUNCTION DST(Y)
* (C) Copyright (1990), Cryodata Inc.; all rights reserved.
* Function used by DFST
      IMPLICIT DOUBLE PRECISION (A-H, O-Z)
      COMMON/SUBRT/T,R(12),JR
      DST=ENTROP(Y,T)
      END
C
C
C
      SUBROUTINE DTFHS (IDID, D, T, X, DL, DV, P, DPDT, H, S)
* (C) Copyright (1992), Cryodata Inc.; all rights reserved.
*
* Density, Temperature, and Quality as a function of Enthalpy and Entropy.
* In the two phase region (quality between 0 and 1), saturation densities,
* pressure, and dP/dT are also returned. SI units. Valid from 0.8 to 1500 K.
*
      IMPLICIT DOUBLE PRECISION (A-H, O-Z)
      PARAMETER (ZERO=0.D0)
      COMMON /PRINIT/ R,AMW,PCRIT,TCRIT,DCRIT,HCRIT,UCRIT,SCRIT,
     1   PTR,TTR,DTRF,DTRG,HTRF,HTRG,UTRF,UTRG,STRF,STRG,THI,DHI,
     2   HHI,UHI,SHI,CPHI,CVHI,HMAX2P,UMAX2P,PMAX,PMIN,TMAX,TMIN
      EXTERNAL DTHS
      COMMON /SUBRT/  RR(13), JR
      COMMON /PRECSN/ E(0:15)
* Following are entropy and temperature when H(sat vap) is maximum
* Version August 29, 1992
      DATA SHMAX2 /8545.D0/, THMAX2 /4.17D0/
C Following 2 elements of RR are not modified by the call to ROOT
      RR(4) = S
      RR(5) = H
      X=2.
      IF (S*(S-64000.) .GE. 0.D0) THEN
         IDID = -108
         RETURN
      ENDIF
      IF (H*(H-8100000.) .GE. 0.D0) THEN
         IDID = -109
         RETURN
      ENDIF
      IF (H .LT. HMAX2P) THEN
* If in the two-phase region, then we can find T such that H-TS = Gsat(T)
         CALL ROOTAZ (T1, TMIN, 1.2D0, 3.6D0, TCRIT, E(8), 0.D0,
     +         E(9), E(10), DTHS, ZERO, JX)
         IF (JX .GT. 0) THEN
* Two phase
            T  = T1
            CALL PSATFT (P, DPDT, T1)
            DL = D2LFPT (P, T1)
            DV = D2VFPT (P, T1)
            SL = ENTROP (DL, T1)
            SV = ENTROP (DV, T1)
            IF (SV .NE. SL) THEN
               X  = (S-SL) / (SV-SL)
               D  = 1.D0/((1.D0/DL)*(1.D0-X) + (1.D0/DV)*X)
            ELSE
               X = 0.5
               D = DCRIT
            ENDIF
            IF (X .LT. ZERO) THEN
               D1 = DL
            ELSE IF (X .GT. 1.D0) THEN
               D1 = DV
            ELSE
               IDID = 3
               RETURN
            ENDIF
         ELSE IF (S .LE. ZERO) THEN
            IDID = -108
            RETURN
         ELSE IF (S .LT. SCRIT) THEN
* Compressed liquid
            T1 = TMIN + (S/SCRIT) * (TCRIT-TMIN)
            D1 = 1.005D0 * DFSAT (T1)
         ELSE IF (S .LT. SHMAX2) THEN
* Near saturated vapor, T > THMAX2
            T1 = (S-SCRIT)/(SHMAX2-SCRIT)
            T1 = TCRIT*(1.D0-T1) + THMAX2*T1
            D1 = 0.95D0 * DGSAT (T1)
         ELSE IF (H .LE. ZERO) THEN
            IDID = -109
            RETURN
         ELSE
* Vapor, T < TCRIT
* Latent heat at T = 0 is about 21000 J/KG
            T1 = MAX (TMIN, ((H-21000.D0)*THMAX2/HMAX2P))
            IF (T1 .GE. TCRIT) T1 = TCRIT - 0.04
            D1 = DGSAT (T1)
         ENDIF
      ELSE
* use ideal gas approximation
         T1 = MAX (THI+(H-HHI)/CPHI, TCRIT+2.D0)
         T1 = MIN (T1, 1500.D0)
         D1 = DHI*EXP((SHI-S-CVHI*LOG(THI/T1))/R)
         D1 = MIN (D1, DENMAX(T1))
         D1 = MAX (D1, 1.D-05)
      ENDIF
      CALL DTILXY (D1, T1, I, 'HS', H, S)
      IF (I .LE. 0) THEN
         IDID = -202
      ELSE
         D  = D1
         T  = T1
         DL = ZERO
         DV = ZERO
         IF (D.GT.DCRIT) X=-1.
         IDID = 1
      ENDIF
      END
C
C
C
      DOUBLE PRECISION FUNCTION DTHS (T)
* (C) Copyright (1992), Cryodata Inc.; all rights reserved.
* External function in DTFHS; Version Sept 9, 1989
      IMPLICIT DOUBLE PRECISION (A-H, O-Z)
      COMMON /SUBRT/ RR(13), JR
      RR(2)= T
      GSAT = RR(5) - RR(2)*RR(4)
      CALL PSATFT (RR(1), RR(6), RR(2))
      RR(3)= D2LFPT (RR(1), RR(2))
      CALL SHAUG (RR)
      DTHS = GSAT - RR(12)
      END
C
C
C
      SUBROUTINE DTFPX (IDID, RHO, TT, Q, DL, DV, DPDTS, P, X, LABEL)
* (C) Copyright (1992), Cryodata Inc.; all rights reserved.
*
* Output: Density (RHO), Temperature (TT), and Quality (Q)
*         If Q is between 0 and 1, the saturation pressure P, its
*         derivative dP/dT, and saturation densitites DL and DV
*         are also evaluated. SI units.
* Input:  Pressure (P) and parameter X, where X can be entropy (S),
*         enthalpy (H) or internal energy (U), depending on LABEL.
* Range:  valid over the full state equation, including 2-phase region
C Version 3.2; indices changed to match PROP array
      IMPLICIT DOUBLE PRECISION (A-H, O-Z)
      CHARACTER*2 LABEL
      DIMENSION FL(13), FV(13), FDER(11), FHIP1(8:11), FHIP2(8:11)
      COMMON/PRINIT/R,AMW,PCRIT,TCRIT,DCRIT,HCRIT,UCRIT,SCRIT,
     1   PTR,TTR,DTRF,DTRG,HTRF,HTRG,UTRF,UTRG,STRF,STRG,THI,DHI,
     2   HHI,UHI,SHI,CPHI,CVHI,HMAX2P,UMAX2P,PMAX,PMIN,TMAX,TMIN
      DATA FHIP1 /2.90D+3,  712.D+3, 0.D0, 270.D+3/
      DATA FHIP2 /17.4D+3, 5450.D+3, 0.D0, 1510.D+3/
      IF (P .LE. 0.) THEN
         IDID = -101
         RETURN
      ELSE IF (P .GT. PMAX) THEN
         IDID = -102
         RETURN
      ELSE IF (LABEL .EQ. 'PS') THEN
         IDID  = -108
         IF (X*(X-64000.) .GE. 0.D0) RETURN
         SPHT  = CPHI
         JF    = 8
         TA    = THI * EXP ((X-SHI+R*LOG(1.D-05*P))/CPHI)
      ELSE IF (LABEL .EQ. 'PH') THEN
         IDID  = -109
         IF (X*(X-8100000.) .GE. 0.D0) RETURN
         SPHT  = CPHI
         JF    = 9
         TA    = THI + (X-HHI)/CPHI
      ELSE IF (LABEL .EQ. 'PU') THEN
         IDID  = -110
         IF (X*(X-4730000.) .GE. 0.D0) RETURN
         SPHT  = CVHI
         JF    = 11
         TA    = THI + (X-UHI)/CVHI
C      ELSE
C         STOP ' LABEL ERROR IN DTFPX'
      ENDIF
      IF (X .LT. 1.) RETURN
      T    = -1.
      IF ((TA .GT. 15.) .AND. (P .LT. 1.02D+08)) THEN
* Check for approximate ideal gas
         D = RHOBB (P, TA)
         IF ((P .LT. 20.D+05) .OR. (D .LT. DCRIT)) T = TA
      ENDIF
      IF (T .LT. 0.) THEN
      IF (P .LT. PCRIT) THEN
         IF (P .LT. 1.47) THEN
*           PSAT (0.8) = 1.4752 PA; low pressure vapor
            T = MAX (TMIN, TA)
            D = P /(R*T)
         ELSE
* Compare with saturated vapor
            TS = TSATFP (P)
            DV = D2VFPT (P, TS)
            IF (JF .EQ. 8) THEN
               FV(8) = ENTROP (DV, TS)
               IF (X .GT. FV(8))  T = TS * EXP((X-FV(8))/SPHT)
            ELSE
               FV(1) = P
               FV(2) = TS
               FV(3) = DV
               CALL SHAUG (FV)
               IF (X .GT. FV(JF)) T = TS + (X-FV(JF))/SPHT
            ENDIF
            IF (T .GT. 0) THEN
* Single phase vapor
               D = DV * TS / T
            ELSE
* Not vapor; compare with saturated liquid
               DL = D2LFPT (P, TS)
               IF (JF .EQ. 8) THEN
                  FL(8) = ENTROP (DL, TS)
               ELSE
                  FL(1) = P
                  FL(2) = TS
                  FL(3) = DL
                  CALL SHAUG(FL)
               ENDIF
               IF (X .GT. FL(JF)) THEN
* Two phase mixture
                  IF (FL(JF) .EQ. FV(JF)) THEN
                     Q = 0.5D0
                  ELSE
                     Q   = ROUN01 ((X-FL(JF))/(FV(JF)-FL(JF)))
                  ENDIF
                  CALL PSATFT (DUM, DPDTS, TS)
                  RHO = DL / (1. + Q*(DL/DV - 1.))
                  TT  = TS
                  IDID= 3
                  RETURN
               ELSE
* Compressed liquid
                  CALL DERIV (FDER, DL, TS)
                  DELT = (X-FL(JF))/FDER(1)
                  IF (JF .EQ. 8) DELT = DELT * T
                  T = TS + DELT
                  IF (T .LT. TMIN) T = TMIN
                  D = MAX (D3K(P), DL)
               ENDIF
             ENDIF
         ENDIF
      ELSE IF (P .LT. 25.4E+05) THEN
* Compressed liquid
         D = D3K (P)
         IF (JF .EQ. 8) THEN
            X3 = S3K (P)
            IF (X .GT. X3) THEN
               T = 3. * EXP ((X-X3)/SPHT)
               D = D * 3./T
            ELSE
               T = MAX (TMIN, (3. + (X-X3)/SPHT))
            ENDIF
         ELSE
            IF (JF .EQ. 9) THEN
               X3 = H3K (P)
            ELSE
               X3 = U3K (P)
            ENDIF
              T = MAX (TMIN, (3. + (X-X3)/SPHT))
            IF (X .GT. X3) D = D * 3./T
         ENDIF
      ELSE IF (P .GT. 1000.D+05) THEN
         IF ((X-FHIP1(JF))*(X-FHIP2(JF)) .GT. 0.D0) RETURN
         D = 150.D0 + 1.8D-7*P
         T = 180.D0
      ELSE
* Possible solid
         TM = TMFP (P)
         DM = DMFP (P)
         FL(1) = P
         FL(2) = TM
         FL(3) = DM
         CALL SHAUG (FL)
         IF (X .LT. FL(JF)*0.999) THEN
            IDID = -107
            RETURN
         ENDIF
         T = TM + (X-FL(JF))/SPHT
         IF (JF .EQ. 8) T = TM * EXP ((X-FL(JF))/SPHT)
         D = DM * SQRT (TM / T)
      ENDIF
      ENDIF
      CALL DTILXY (D, T, I, LABEL, P, X)
      IF (I .LT. 0) THEN
         IDID = -206
      ELSE
         IDID = 1
         DL = 0.
         DV = 0.
         IF (D .GT. DCRIT) THEN
            Q = -1.D0
         ELSE
            Q = 2.
         ENDIF
         RHO = D
         TT  = T
      ENDIF
      END
C
C
C
      SUBROUTINE DTFPG (IDID, DCALC, TCALC, P, G)
* (C) Copyright (1992), Cryodata Inc.; all rights reserved.
*
* Density and temperature as a function of pressure and Gibbs energy
* This subroutine valid only in the compressed liquid between 0.8 & 3 K
      IMPLICIT DOUBLE PRECISION (A-H, O-Z)
      DIMENSION PRP(13)
      IF ((P .LT. 1.46) .OR. (P .GT. 3020000.)) THEN
         IDID = -113
         RETURN
      ENDIF
      PCALC = P
      G3K = H3K(PCALC) - 3.D0*S3K(PCALC)
      D   = D175K(PCALC)
      T   = 0.8
      PRP(1) = PCALC
      PRP(2) = T
      PRP(3) = D
      CALL SHAUG (PRP)
      GCALC = PRP(12)
      IF ((G-GCALC)*(G-G3K) .GT. 0.) THEN
         IDID = -111
         RETURN
      ENDIF
      T = 0.8 + (G-GCALC)/(G3K-GCALC)*2.2D0
      CALL DTILXY (D, T, J, 'PG', P, G)
      IF (J .LE. 0) THEN
         IDID = -203
      ELSE
         IDID  = 1
         DCALC = D
         TCALC = T
      ENDIF
      END
C
C
C
      SUBROUTINE DTILXY (D, T, J, LABEL, X, Y)
* (C) Copyright (1992), Cryodata Inc.; all rights reserved.
*
*-----OUTPUT
* D = Density     [kG/m3]
* T = Temperature [K]
* J = Iteration index, which should be a positive number
*     If J is negative, it signals convergence failure.
*-----INPUT
* D = an initial estimate for density
* T = an initial estimate for temperature
* LABEL = a character variable specifying the pair (X,Y)
* X = Thermodynamic variable identified by LABEL
* Y = Thermodynamic variable identified by LABEL
*----- Version August 29, 1992
      IMPLICIT DOUBLE PRECISION (A-H, O-Z)
      CHARACTER*2 LABEL
      LOGICAL CALLDR
      COMMON /PRECSN/ E(0:15)
      DIMENSION F(11), PRP(13)
      J   = 0
      FRACT = 0.3D0
      CALLDR= .TRUE.
      DO 60 I = 1, 30
      PCALC = PRESS (D, T)
      PRP(1) = PCALC
      PRP(2) = T
      PRP(3) = D
      CALL SHAUG (PRP)
      IF (CALLDR) CALL DERIV (F, D, T)
      C2 = F(7)*F(7)
      IF (LABEL .EQ. 'PH') THEN
*-----X is Pressure, Y is Enthalpy
         DELP = X - PCALC
         DELH = Y - PRP(9)
         IF ((ABS(DELP) .LT. E(9)*X+E(1)) .AND.
     $       (ABS(DELH) .LT. E(9)*Y+E(1))) J=I
         DELD = ((1.D0+F(5))*DELP - D*F(5)*DELH) / C2
         DELT = F(8) * DELP + DELH / F(1)
      ELSE IF (LABEL .EQ. 'PS') THEN
*-----X is Pressure, Y is Entropy
         DELP = X - PCALC
         DELS = Y - PRP(8)
         IF ((ABS(DELP/X) .LT. E(9)) .AND.
     $       (ABS(T*DELS) .LT. E(8)*F(1))) J = I
         DELD = (DELP - F(5)*D*T*DELS) / C2
         DELT = (F(4)*DELP/D + T*DELS) / F(1)
      ELSE IF (LABEL .EQ. 'PU') THEN
*-----X is Pressure, Y is Energy
         DELP  = X - PCALC
         DELU  = Y - PRP(11)
         IF ((ABS(DELP) .LT. E(9)*X + E(1)) .AND.
     $       (ABS(DELU) .LT. E(9)*Y + E(2))) J = I
         CKT   = PCALC * F(6)
         OMPKS = 1. - F(5)*CKT/F(3)
         DELD  = (DELP - F(5)*D*DELU)/(C2*OMPKS)
         DELT  = ((F(4)-CKT)*DELP/D + DELU)/(F(1)*OMPKS)
      ELSE IF (LABEL .EQ. 'PG') THEN
*-----X is Pressure, Y is Gibbs energy
         DELP = X - PCALC
         DELG = Y - PRP(12)
         IF (((ABS(DELP/X).LT.E(10)) .OR. (ABS(DELP) .LT. E(1)))
     &      .AND. (ABS(DELG/PRP(8)).LT.E(6))) J = I
         DELD = (F(4)/(PRP(8)*T)) * ((PRP(8)/(F(5)*F(2)) - 1.D0)*DELP
     &          + D*DELG)
         DELT = (DELP/D - DELG) / PRP(8)
      ELSE IF (LABEL .EQ. 'HS') THEN
*-----X is Enthalpy, Y is Entropy
         DELH = X - PRP(9)
         DELS = Y - PRP(8)
         IF ((ABS(DELH) .LT. E(10)*ABS(X)+E(2)) .AND.
     $       (ABS(T*DELS) .LT. E(10)*ABS(Y)+E(2))) J=I
         DELD = (DELH - (1.+F(5))*T*DELS) * D / C2
         DELT = (F(5)*DELH/C2 - D*F(8)*DELS) * T
      ENDIF
*-----iteration
      IF ((ABS(DELD/D) + ABS(DELT/T) .LT. 0.0004D0) .AND.
     $    (I .GT. 3)) THEN
         CALLDR = .FALSE.
         FRACT  = 0.D0
      ENDIF
*-----Limit large steps
      IF (FRACT .GT. 0.D0) THEN
         IF ((I .GT. 7) .AND. ((I/4)*4 .EQ. I)) THEN
            DELD = DELD*0.5D0
            DELT = DELT*0.5D0
         ENDIF
         DELD = SIGN (MIN (ABS(DELD), ABS(FRACT*D)), DELD)
         DELT = SIGN (MIN (ABS(DELT), ABS(FRACT*T)), DELT)
      ENDIF
      D  = D + DELD
      T  = T + DELT
      IF ((ABS(DELD).LT.E(12)*D) .AND. (ABS(DELT).LT.E(12)*T)) J = I
      IF (J .GT. 0) RETURN
   60 CONTINUE
      IF (FRACT .LT. 0.01D0) THEN
* Probable oscillation around the true answer
         D = D - 0.5D0*DELD
         T = T - 0.5D0*DELT
         J = 61
         RETURN
      ENDIF
   70 J  = -200
      RETURN
      END
C
C
C
      SUBROUTINE TFDP (IDID, T, X, DL, DV, D, P)
* (C) Copyright (1992), Cryodata Inc.; all rights reserved.
*
* Output: Temperature, quality, and saturation densities
* Input:  Density and pressure  (All SI units)
      IMPLICIT DOUBLE PRECISION (A-H, O-Z)
      COMMON /SUBRT/ RT(13), JR
      COMMON /SUBLAM/ TLAM, OTHER(3)
      COMMON /PRECSN/ E(0:15)
      EXTERNAL DMFT, TDP
* GRUNCV = Gruneisen parameter * Cv, and is reasonably constant.
* Used for integration w.r to P along isochores.
      DATA DC, GRUNCV, PMAX /69.64D0, 2078.D0, 1.0133D+08/
      X    = -1.
      IDID = 1
      TMAX = 0.D0
      IF (D .LE. 0.) THEN
         IDID = -105
         RETURN
      ELSE IF (P .LE. 0.) THEN
         IDID = -101
         RETURN
      ELSE IF (P .GT. PMAX) THEN
         IDID = -102
         RETURN
      ENDIF
      IF ((P .LT. 16.D+05) .OR. (D .LT. DC)) THEN
         TA = MIN ((P/(2077.*D)), 1400.D0)
         TMIN = 0.8D0
         IF (TA .GT. 30.) THEN
            TB = 1.05*TA
            TMAX = 1500.D0
         ELSE IF ((TA .GT. 6.) .OR. (P .LT. 1.48)) THEN
            TB = TA + 3.
            TMAX = 1500.D0
         ENDIF
      ENDIF
      IF (TMAX .EQ. 0.D0) THEN
         IF (D .LT. 146.16D0) THEN
            CALL SATFD (JX, XX, PS, DPDT, TS, DL, DV, D)
            IF (JX .LT. 0) THEN
               IDID = -203
               RETURN
            ENDIF
            IF (P .LT. PS) THEN
               TT = TSATFP (P)
               DL = D2LFPT (P, TT)
               DV = D2VFPT (P, TT)
               IF (DL .EQ. DV) THEN
                  X = 0.5D0
               ELSE
                  X = (DL/D - 1.D0) / (DL/DV - 1.D0)
                  X = ROUN01 (X)
               ENDIF
               T  = TT
               IDID = 3
               RETURN
            ELSE
               IF (D .LT. DC) X  = 2.D0
               TMIN = TS - 1.D-03
               TMAX = 1500.D0
               TA = MIN ((TS+0.75D0*(P-PS)/(D*GRUNCV)), 1400.D0)
               TB = MIN ((TA*1.25D0), TMAX)
            ENDIF
         ELSE
* 179.84 = density at upper lambda point, where T = 1.7673
            IF (D .LT. 179.84) THEN
               TMIN = TLFD (D) + 0.2
               JX = -1
            ELSE
               TMIN = TMFD (D)
               JX = 0
            ENDIF
            PM = PRESS (D, TMIN)
            TA = 0.75D0*(P-PM) / (D*GRUNCV)
            IF (TA .LT. 0.D0) THEN
               IF (JX .LT. 0) THEN
                  IDID = -180
               ELSE
                  IDID = -107
               ENDIF
               RETURN
            ENDIF
            TA = TA + TMIN
            TB = TA*1.25D0
            TMAX = 400.D0
         ENDIF
      ENDIF
      RT(1) = D
      CALL ROOTAZ (TT, TMIN, TA, TB, TMAX, E(8), E(11),
     +      E(2), E(11), TDP, P, JX)
      IF (JX .LT. 0) THEN
         IDID = -106
      ELSE
         T = TT
      ENDIF
      END
C
      DOUBLE PRECISION FUNCTION TDP(Y)
* (C) Copyright (1992), Cryodata Inc.; all rights reserved.
* Function used by TFDP
      IMPLICIT DOUBLE PRECISION (A-H, O-Z)
      COMMON/SUBRT/RHO,R(12),JR
      TDP = PRESS (RHO, Y)
      END
C
C
C-----Version 3.301: local variable DPDT renamed XDPDT
      SUBROUTINE TFDY (IDID, T, Q, PS, XDPDT, DL, DV, DD, YY, LABEL)
* (C) Copyright (1992), Cryodata Inc.; all rights reserved.
*
* Output: Temperature (T) and Quality (Q)
*         If Q is between 0 and 1, the saturation pressure P, its
*         derivative dP/dT, and saturation densitites DL and DV
*         are also evaluated.  SI units.
* Input:  Density D and parameter X, where Y can be entropy (S),
*         enthalpy (H) or internal energy (U), depending on LABEL.
* Range:  valid over the full state equation, including 2-phase region
* Version Aug. 5, 1992; for HEPAK v3.2
      IMPLICIT DOUBLE PRECISION (A-H, O-Z)
      CHARACTER*1 LABEL
      COMMON /SUBRT/ RT(13), JR
      COMMON /PRECSN/ E(0:15)
      EXTERNAL YJFDT
      DATA TZ /1500./
      Y = YY
      D = DD
      IF (D*(D-599.) .GE. 0.) THEN
         IDID = -105
         RETURN
      ENDIF
      IF (D .GT. 171.7) THEN
         TA = TMFD (D)
      ELSE
         TA = 0.8D0
      ENDIF
      IF (LABEL .EQ. 'S') THEN
         IDID = -108
         IF (Y*(Y-64000.) .GE. 0.D0) RETURN
         TB = EXP(((1.D-03*Y    - 10.037)/2.07723       + DLOG(D))/1.5)
         TB = MAX (TA, TB)
         JR    = 8
      ELSE IF (LABEL .EQ. 'H') THEN
         IDID = -109
         IF (Y*(Y-8100000.) .GE. 0.D0) RETURN
         TB = MAX ((Y/5193.), TA)
         JR    = 9
      ELSE IF (LABEL .EQ. 'U') THEN
         IDID = -110
         IF (Y*(Y-4730000.) .GE. 0.D0) RETURN
         TB = MAX ((Y/3117.), TA)
         JR    = 11
      ELSE
         IDID = -299
         RETURN
      ENDIF
      TB = MAX (TB, 1.D0)
      IF (TB .GT. 1200.) THEN
         TB = 1200.
         TC = 1400.
      ELSE
         TC = 2.D0*TB
         TC = MIN (TC, (TB+50.))
      ENDIF
      RT(3) = D
      CALL ROOTAZ (T, TA, TB, TC, TZ, E(9), E(11),
     +             E(3), E(10), YJFDT, Y, JX)
      IF (JX .LT. 0) RETURN
      IDID = NINT (RT(7))
      IF (IDID .EQ. 3) THEN
         Q  = RT(8)
         PS = RT(1)
         DL = RT(4)
         DV = RT(5)
C-----Version 3.301: local variable DPDT renamed XDPDT
         XDPDT = RT(6)
      ELSE IF   (D .LT. 69.64) THEN
         Q = 2.
      ELSE
         Q = -1.
      ENDIF
      END
C
C
C
      FUNCTION YJFDT (T)
* (C) Copyright (1992), Cryodata Inc.; all rights reserved.
C
C External function in TFDY.
C Output:
C     YJFDT = entropy, enthlpy, or energy [SI units]
C     (if IDID = 3) saturation properties P, DPDT, Q, DL, DV, via /SUBRT/
C Input:  T = temperature (called from ROOTAZ)
C         D = density, from TFDY via common /SUBRT/
C Version Sept 13, 1992
      IMPLICIT DOUBLE PRECISION (A-H, O-Z)
      COMMON /SUBRT/ RT(13), JR
      DIMENSION F(13)
      D = RT(3)
C-----Version 3.301: local variable DPDT renamed XDPDT
      CALL FDT (IDID, Q, PS, XDPDT, DL, DV, D, T)
      RT(7) = DBLE(IDID)
      IF (IDID .LT. 0) IDID = 1
      IF (IDID .EQ. 1) THEN
         DL = D
         PS = PRESS (D,T)
      ENDIF
      IF (JR .EQ. 8) THEN
         YJFDT = ENTROP (DL, T)
      ELSE
         F(1) = PS
         F(2) = T
         F(3) = DL
         CALL SHAUG (F)
         YJFDT = F(JR)
      ENDIF
      IF (IDID .EQ. 3) THEN
         IF (JR .EQ. 8) THEN
            YJFDT = Q*ENTROP(DV, T) + (1.D0 - Q)*YJFDT
         ELSE
            F(3) = DV
            CALL SHAUG (F)
            YJFDT = Q*F(JR) + (1.D0 - Q)*YJFDT
         ENDIF
         RT(8) = Q
         RT(1) = PS
         RT(4) = DL
         RT(5) = DV
C-----Version 3.301: local variable DPDT renamed XDPDT
         RT(6) = XDPDT
      ENDIF
      END
C
C
C
      SUBROUTINE DFTG (IDID, D, T, G)
* (C) Copyright (1992), Cryodata Inc.; all rights reserved.
*
* Density as a function of Temperature and Gibbs Energy (all SI units).
* This function operates only in the compressed liquid between 0.8 and
* 3 K (applicable to the fountain effect in superfluid).
      IMPLICIT DOUBLE PRECISION (A-H, O-Z)
      COMMON /SUBRT/ R(13), JR
      COMMON /PRECSN/ E(0:15)
      EXTERNAL DTG
      IF (T .LT. 0.8D0) THEN
         IDID = -103
         RETURN
      ELSE IF (T .GT. 3.0) THEN
         IDID = -112
         RETURN
      ENDIF
      R(2) = T
      DMIN = 0.995 * DFSAT (T)
      DMAX = DMFT (T)
      D1   = 0.90D0*DMIN + 0.1D0*DMAX
      D2   = 0.5D0*(DMIN+DMAX)
      CALL ROOTAZ (D, DMIN, D1, D2, DMAX, E(7), E(11),
     +      E(6), E(11), DTG, G, JX)
      IF (JX .LT. 0) THEN
         IDID = -111
      ELSE
         IDID = 1
      ENDIF
      END
C
      DOUBLE PRECISION FUNCTION DTG (D)
* (C) Copyright (1992), Cryodata Inc.; all rights reserved.
*
* Gibbs energy as a function of density and (from /SUBRT/) T.
* This is an external function to subroutine DFTG.
      IMPLICIT DOUBLE PRECISION (A-H, O-Z)
      COMMON /SUBRT/ R(13), JR
      R(3) = D
      R(1) = PRESSA (R(3), R(2))
      CALL SHAUG (R)
      DTG = R(12)
      END
C
C
C
      DOUBLE PRECISION FUNCTION DIELHE (RHO)
C    (Dielectric constant - 1.0) as a function of density [Kg/m3].
C-----Version 3.23: Output changed to (Dielectric constant - 1.0)
C                   instead of just the dielectric constant.
C    Equation from D. Friend, NIST, 12/20/90.  Includes values from Gugan
C    and Michel,  Metrologia 1980, 1984 with 1986 CODATA constants;
C    temperature dependence of B is eliminated.
C
      IMPLICIT DOUBLE PRECISION (A-H, O-Z)
      P = (.123493D0-5.86D-6*RHO)*1.046516792D0
      DIELHE = (2.D0*P*RHO+1000.D0)/(1000.D0-P*RHO) - 1.D0
      END
C
C
C
      DOUBLE PRECISION FUNCTION REFR (RHO, W)
*     (Refractive index -1.0) as a function of density and wavelength [um].
C-----Version 3.23: Output changed to (refr. index - 1.0)
C                   instead of just the refractive index. 
C     Uses new dielectric constant correlation for infinite wavelength.
C     Uses Sellmeier fit from Peck (App. Opt. 22, 2906, 1983) for dispersion
C     between 0.09 and 2 micrometer [= um] wavelength.
C     For larger wavelengths, linearly interpolates between 2 um value and
C     dielectric constant value.  The dispersion at 298.15 k, 0.101325 mpa
C     is assumed to be unchanged at other state points.
C     Equation from D. Friend (NIST), Jan 3 1991.
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      DATA RINF, R2MIN /31.704D-6, 34.640D-6/
      IF (W .LE. 0.D0) THEN
         W2I = 1.0
      ELSE
         W2I=1.D0/(W*W)
      ENDIF
      RININF=DSQRT (1.D0 + DIELHE (RHO))
      IF (W2I .LT. 0.25) THEN
         RDISP=W2I*(R2MIN-RINF)/0.25D0
      ELSE
         RDISP=7.3123D-6 + 9279.7D-6/(339.82D0-W2I) - RINF
      ENDIF
      REFR=RININF + RDISP - 1.D0
      RETURN
      END
C
C
C
      DOUBLE PRECISION FUNCTION STEN (T)
*  Surface tension [N/m] as a function of temperature [K].
C  Equation from D. Friend, NIST, Jan 8, 1991.  Fit of data from
C  Iino, Suzuki, Ikushima, JLTP 61, 155 (1985).
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      DATA TC/5.1953D0/
      DATA A1,A2,A3/389.057D-6, 521.410D-6, -579.737D-6/
      IF (T.GE.TC) THEN
         STEN=0.
      ELSE
         TSTR=(TC-T)/TC
         STEN=TSTR*(A1+TSTR*(A2+TSTR*A3))
      ENDIF
      END
C
C
C
      DOUBLE PRECISION FUNCTION TCON (RHO,T)
* (C) Copyright (1990), Cryodata Inc.; all rights reserved.
* This function written by B.A. Hands.
*
*  thermal conductivity as a function of density and temperature
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      IMPLICIT DOUBLE PRECISION  (K)
      DIMENSION F(12),C(5)
      DATA F/3.726229668,.186297053E-3,-.7275964435E-6,-.1427549651E-3,
     1.3290833592E-4,-.5213335363E-7,.4492659933E-7,-.5924416513E-8,
     2.7087321137E-5,-.6013335678E-5,.8067145814E-6,.3995125013E-6/
      DATA X0/.392/,E1/2.8461/,E2/.27156/,BETA/.3554/,GAMMA/1.1743/,
     1DELTA/4.304/,DC/69.58/,TC/5.18992/,PC/227460./
      DATA CONST/-5.882788298/,CON/3.4685233E-17/
      DATA C/.7034007057,3.739232544,-26.20316969,59.82252246,
     1      -49.26397634/
      D = RHO
      IF (T .LT. 3.5) THEN
         IF (D .LT. DC) THEN
            TCON = CONLPT (RHO, T)
         ELSE
            TCON = 0.
         ENDIF
         RETURN
      ENDIF
      T1=T**(1./3.)
      T2=T1*T1
      D2=RHO*RHO
      D3=RHO*D2
      IF(RHO.LT.1.E-8) GO TO 10
      DL=D2*LOG(RHO/69.64)
      GO TO 11
10    DL=0.
* Calculate critical enhancement
11    IF(T.GT.12..OR.T.LT.3.5) GO TO 5
* Calculate compressibility
      KT=1./DPDD(RHO,T)/RHO
      DELD=ABS((RHO-DC)/DC)
      DELT=ABS((T-TC)/TC)
      R2=(DELT/.2)**2+(DELD/.25)**2
      IF(R2.GE.1) GO TO 20
      W=DELT/(DELD**(1./BETA))
      X1=(W+X0)/X0
      XX2B=X1**(2.*BETA)
      XX2BE=(1.+E2*XX2B)**((GAMMA-1.)/2./BETA)
      H=E1*X1*XX2BE
      DHDX=E1*XX2BE/X0+E1*E2/X0*XX2B*XX2BE/(1.+E2*XX2B)*(GAMMA-1.)
      D2KT=(DELTA*H-W*DHDX/BETA)*(DELD**(DELTA-1.))
      KT1=DC*DC/D2/D2KT/PC
      KT=R2*KT+(1.-R2)*KT1
* Calculate excess conductivity
20    PDT=DPDT(RHO,T)
      KCRIT=T*T*SQRT(KT)/RHO/VISCOS(RHO,T)*PDT*PDT*EXP(-18.66*DELT**2
     1-4.25*DELD**4)
      KCRIT=KCRIT*CON
      GO TO 6
5     KCRIT=0
6     A=0
      TT=T
      DO 100 I=2,5
      A=C(I)/TT+A
100   TT=TT*T
      K0=T**(C(1))*EXP(A+CONST)
      DL=D2*LOG(RHO/68.)
      TCON=K0+F(1)*KCRIT+RHO*(F(2)+F(3)*T +F(4)*T1 +F(5)*T2)
     1+D3*(F(6)+F(7)*T1 +F(8)*T2 )+DL*(F(9)+F(10)*T1 +F(11)*T2
     2+F(12)/T)
      RETURN
      END
C
C
C
      FUNCTION CONLPT (D, T)
* (C) Copyright (1992), Cryodata Inc.; all rights reserved.
C Conductivity of vapor, T < 3.5 K; extrapolation of HEPAK v3.0 data.
C The accuracy of this extrapolation is unknown.  Existing data from
C 3.5 to 20 K suggests anomalous behavior in the low T limit.
C The assigned lower limit of 1.2 K is arbitrary.
C
C Units: CONLPT [W/m-K], Density [Kg/m3], Temperature [K]
C V. Arp; August, 1992
      IMPLICIT DOUBLE PRECISION (A-H, O-Z)
      DATA A,B /0.1916377113E-02, 0.4654575892E-04/
      IF (T .GT. 1.1999999D0) THEN
         CONLPT = A*T + B*D
      ELSE
         CONLPT = 0.D0
      ENDIF
      RETURN
      END
C
C
      DOUBLE PRECISION FUNCTION VISCOS (DKGM3,TK)
* (C) Copyright (1992), Cryodata Inc.; all rights reserved.
C Output: Viscosity [SI units = Pa-s]
C Input:  Density [Kg/m3] (single phase region only!), and Temperature [K]
C-----Version 3.2; 7/2/92
C
      IMPLICIT DOUBLE PRECISION (A-H, O-Z)
      PARAMETER (TL=3.5D0, TH=3.75D0, DT=TH-TL)
      COMMON/PRINIT/PARAM(31)
      D = DKGM3
      T = TK
      IF (T .GT. TL) THEN
         VISCOS = VISCHI (D, T)
         IF (T .GE. TH) RETURN
      ENDIF
C Param(5) is the critical density
      IF (D .GT. PARAM(5)) THEN
         VX = VISLAM (D, T)
      ELSE
         VX = VISLPT (D, T)
      ENDIF
      IF (T .LE. TL) THEN
         VISCOS = VX
         RETURN
      ENDIF
      W = (T-TL)/DT
      VISCOS = W*VISCOS + (1.D0-W)*VX
      RETURN
      END
C
C
C
      DOUBLE PRECISION FUNCTION VISCHI (DKGM3,T)
C Output: Viscosity [SI units = Pa-s]
C Input:  Density [Kg/m3], and Temperature [K] (valid only for T>3.5K)
C
C This is consistent with NIST's viscosity function, January, 1991,
C by D.G.Friend, V. Arp and R.D. McCarty.
C
      IMPLICIT DOUBLE PRECISION (A-H, O-Z)
      PARAMETER (T1=100.D0, T2=110.D0, DELT=T2-T1, TMAX=300.D0)
      IF (T .LE. TMAX) THEN
         TL = DLOG(T)
      ELSE
C        TL = DLOG(TMAX)
         TL = 5.7037825D0
      ENDIF
      VISCL = EXP (-0.135311743D0/TL+1.00347841D0+TL*(1.20654649D0
     2             +TL*(-0.149564551D0+TL*0.0125208416D0)))
      VISC0 = VISCL
      IF (T .GT. T1) THEN
         VISCH  = 196.D0*T**0.71938D0 *
     +            EXP((12.451D0-295.67D0/T)/T-4.1249D0)
         IF (T .LT. T2) THEN
            VISC0 = VISCL + (VISCH-VISCL)*(T-T1)/DELT
         ELSE
            VISC0 = VISCH
         ENDIF
      ENDIF
      R = 1.D-03 * DKGM3
      B = -47.5295259D0/TL+87.6799309D0+TL*(-42.0741589D0
     1    +TL*(8.33128289D0-0.589252385D0*TL))
      C =  547.309267D0/TL-904.870586D0+TL*(431.404928D0
     1    +TL*(-81.4504854D0+TL*5.37008433D0))
      D = -1684.39324D0/TL+3331.08630D0+TL*(-1632.19172D0
     1    +TL*(308.804413D0-20.2936367D0*TL))
      VISCHI = 1.D-07*(VISC0 + VISCL*(DEXP(R*(B + R*(C+ R*D)))-1.D0))
      RETURN
      END
C
C
C
      FUNCTION VISLAM (DD, TT)
* (C) Copyright (1992), Cryodata Inc.; all rights reserved.
C Liquid helium viscosity, 1.2 to 3.8 K
C     Fitted to HMeyer's data along the saturation line.
C     Extended to higher densities by using Hoffman's (Brewer and Edwards)
C     data, shifted by 6.2 pct; joined smoothly to Steward's data
C     at T = 3.6 to 3.8 K.
C     The calling program must make certain that the input density
C     is within the liquid range (nominally 132 to 180+ Kg/m3).
C Units: VISLAM [Pa-s], DD [Kg/m3], TT [K]
      IMPLICIT DOUBLE PRECISION (A-H, O-Z)
      DIMENSION A(3), E0(2), B(8), C(8)
      DATA E0/1.80D0, -4.79D0/
      DATA A / 0.2505885162E+01,  0.5230553382E+00,  0.5607799718E+00/
      DATA B /-0.1127424846E+03,  0.2095894826E+03, -0.1286503418E+03,
     B        -0.7951538104E+02,  0.2015521019E+03, -0.1231199069E+03,
     B        -0.6460724357E+02,  0.5424829902E+02/
      DATA C / 0.8772148954E+03, -0.2515234338E+04,  0.2679676294E+04,
     C         0.3469587682E+03, -0.1509946785E+04,  0.2056348276E+04,
     C         0.2886724375E+03, -0.3831832082E+03/
      DENS = DD
      TL= TLFD (DENS)
      T = TT
      IF (T .LT. 1.1999999D0) THEN
         VISLAM = 0.
         RETURN
      ENDIF
      X = ABS (1. - T/TL)
      D = 10.*(DENS/146.15D0 - 1.D0)
      IF (T .LE. TL) THEN
         J = 2
         V = X*X*(C(1)+X*(C(2)+X*C(3)) + D*(C(4)+X*(C(5)+X*C(6))
     +       + D*X*(C(7)+X*C(8))))
      ELSE
         J = 1
         V = X*X*(B(1)+X*(B(2)+X*B(3)) + D*(B(4)+X*(B(5)+X*B(6))
     +       + D*(B(7)+X*B(8))))
      ENDIF
      E = 10.*(1. + E0(J)*((X+1.D-08)**0.84D0))
      V = V + E*(A(1) + D*(A(2) + D*A(3)))
      VISLAM = V*1.D-07
      RETURN
      END
C
C
C
      FUNCTION VISLPT (D, T)
* (C) Copyright (1992), Cryodata Inc.; all rights reserved.
C Viscosity of vapor, T < 3.5 K; extrapolation of HEPAK v3.0 data.
C The accuracy of this extrapolation is unknown.  Existing data from
C 3.5 to 20 K suggests anomalous behavior in the low T limit.
C The assigned lower limit of 1.2 K is arbitrary.
C
C Units: VISLPT [Pa-s], Density [Kg/m3], Temperature [K]
      IMPLICIT DOUBLE PRECISION (A-H, O-Z)
      DATA A,B /0.2539133848E+00, 0.1024277783E-01/
      IF (T .GT. 1.1999999D0) THEN
         VISLPT = (A*T + B*D)*1.D-06
      ELSE
         VISLPT = 0.D0
      ENDIF
      RETURN
      END
C
C
C
      SUBROUTINE ROOTAZ (R, A, B, C, Z, EXA, EXR, EYA, EYR, F, F0, JX)
* (C) Copyright (1992), Cryodata Inc.; all rights reserved.
*
*-----DESCRIPTION
* This subroutine finds the root, R, such that F(R) = F0,
* where F is an external function, and F0 is a specified constant.
*
*-----EXTERNAL VARIABLES
* This version of ROOTAZ assumes that all external floating-point
* variables are DOUBLE PRECISION
* OUTPUT VARIABLES
*   R           = the desired root,     if JX is positive;
*               = an approximate root,  if JX is zero;
*               = an invalid root,      if JX is negative.
*   JX          = number of calls to F, if ROOTAZ was successful.
*                 If JX=0, input error tolerances may be invalid.
*                 If JX = JMAX+4 = 54, iteration has been terminated without
*                 satisfying any convergence criterion.
*               = -1 if input values of A, B, C, and Z are invalid
*               = -2 if input values of EXA, EXR, EYA, or EYR are invalid.
*               = -3 if extrapolation beyond [B, C] is indeterminant.
*               = -4 if extrapolation beyond A or Z was attempted.
*               = -5 if extrapolation of a non-monotonic function
*                     was attempted.
* INPUT VARIABLES
*   A and Z     = limiting values of the independent variable which must
*                 bracket the root R.  A must not equal Z.  ROOTAZ will
*                 not extrapolate beyond these limits.
*   B and C     = Initial guesses for the root R.  B must not equal C,
*                 and both must be within or at the limits of the
*                 range [A, Z].
*   EXA and EXR = absolute and relative tolerances on the independent
*                 variable.  R is accepted as the root if it is
*                 bracketed within the range EXA + EXR*R
*                 EXA must be > zero.  EXR must be >= zero.
*   EYA and EYR = absolute and relative tolerances on the dependent
*                 variable.  R is accepted as the root if the absolute
*                 value of F(R) - F0 is less than EYA + EYR*F0
*                 EYA must be > zero.  EYR must be >= zero.
*   F0          = the specified value of the function F
C
C Note:  ROOTAZ does not modify any of these input variables
C
*   F           = the external function.  It must be named in an EXTERNAL
*                 statement in the calling program.  It can be non-monotonic
*                 if B and C bracket R, but must be monotonic if
*                 extrapolation is required..
*-----INTERNAL VARIABLES
* All internal floating point variables are DOUBLE PRECISION
* X(1)...X(4)     are successive approximations to R.
* Y(1)...Y(4)     are successive values of F(X(i)) - F0.
* ESYS            specifies the machine precision of the external variables.
*                 It must be larger than the smallest number EPS such that
*                 1 + EPS is not equal to 1 in double precision arithmetic.
* JMAX            should be chosen such that 2**JMAX > (1/ESYS)**2
*                 in order to assure convergence for any F.
* Other variables are used in intermediate steps.
C
*-----VERSION Sept. 13, 1992;  V. Arp
      IMPLICIT DOUBLE PRECISION (A-H, O-Z)
      PARAMETER (ESYS=1.D-08, OME=1.D0-ESYS, JMAX=50)
      LOGICAL LIMIT
      DIMENSION X(4), Y(4)
      EXTERNAL F
C Statement function
      EX(J1,J2) = EXA + (EXR+ESYS)*ABS(X(J1)) + (EXR+ESYS)*ABS(X(J2))
C
C Check for valid parameters
C
      IF ((A .EQ. Z) .OR. (B .EQ. C) .OR. ((B-A)*(B-Z) .GT. 0.D0) .OR.
     +   ((C-A)*(C-Z) .GT. 0.D0)) THEN
         JX = -1
         RETURN
      ENDIF
      IF ((EXA .LE. 0.D0) .OR. (EXR .LT. 0.D0) .OR. (EYA .LE. 0.D0)
     +      .OR. (EYR .LT. 0.D0)) THEN
         JX = -2
         RETURN
      ENDIF
      LIMIT  = .FALSE.
      EY     = EYA + (ESYS + EYR)*ABS(F0)
C
C First guess
C
      Y(1) = F(B) - F0
      IF (ABS (Y(1)) .LT. EY) THEN
         JX = 1
         R = B
         RETURN
      ENDIF
C
C Second guess
C
      Y(2) = F(C) - F0
      IF (ABS (Y(2)) .LT. EY) THEN
         JX = 2
         R = C
         RETURN
      ENDIF
      R21 = Y(2)/Y(1)
C
C Order such that Y(2) is smaller than Y(1).  This is required for
C extrapolation, where monotonic F(x) is assumed.
C If extrapolation is not required, F(x) can be non-monotonic.
C
      IF (ABS(R21) .GT. 1.D0+ESYS) THEN
         R32  = Y(2)
         Y(2) = Y(1)
         Y(1) = R32
         X(1) = C
         X(2) = B
      ELSE IF (R21 .LT. OME) THEN
         X(1) = B
         X(2) = C
      ELSE
C Failure: Constant function F; indeterminant extrapolation.
         JX = -3
         RETURN
      ENDIF
C
C Third try: linear interpolation or extrapolation
C
      STEP = X(2) - X(1)
      X(3) = Y(2)*STEP/(Y(1)-Y(2)) + X(2)
      IF (R21 .GT. 0.D0) THEN
C
C Extrapolation; check limits.
C First, find which limit applies.
C
         IF (((A-X(2))/STEP) .GE. 0.D0) THEN
            XLIM = A
         ELSE
            XLIM = Z
         ENDIF
         IF (ABS(X(2)-XLIM) .LT. 0.5D0*EX(2,2)) THEN
C Failure: A=B or A=C or Z=B or Z=C; extrapolation is not permitted.
            JX = -4
            RETURN
         ENDIF
C Do not extrapolate beyond XLIM.
         IF ((X(3)-XLIM)*(X(3)-X(1)) .GE. 0.D0) THEN
            X(3) = XLIM
            LIMIT = .TRUE.
         ENDIF
      ELSE IF (ABS(STEP) .LE. EX(1,2)) THEN
C B and C straddle the root.
C Return if their separation is is less than the minimum acceptable
C separation specified by EXA, EXR, and ESYS.   User should check for
C input error.
         JX = 0
         R = X(3)
         RETURN
      ENDIF
      Y(3) = F(X(3)) - F0
      IF (ABS(Y(3)) .LT. EY) THEN
         R = X(3)
         JX = 3
         RETURN
      ENDIF
      JF = 0
      JG = 0
C
C-----Do loop
C
      DO 480 JJ = 0, JMAX
      R21 = Y(2)/Y(1)
      R32 = Y(3)/Y(2)
      IF ((R32 .GT. 0.D0) .AND. (R21 .GT. 0.D0)) THEN
C
C Root not bracketed; extrapolation still required.
C
         IF (LIMIT) THEN
C    Failure; further extrpolation is prohibited.
            JX = -4
            RETURN
         ENDIF
         IF (R32 .GE. OME) THEN
C    Extrapolation failure: constant or non-monotonic function.
            JX = -5
            RETURN
         ENDIF
         XI = Y(3)*(X(3)-X(2))/(Y(2)-Y(3))
         IF (R21 .GT. R32) THEN
C    Use inverse quadratic extrapolation if abs(slope) is decreasing.
            XE = Y(3)*(X(3)-X(1))/(Y(1)-Y(3))
            W  = Y(1)/(Y(1)-Y(2))
            XI = W*XI + (1.D0-W)*XE
         ENDIF
         X(4) = XI + X(3)
         IF (R32 .GT. 0.1D0) THEN
C    If extrapolation convergence is slow, give an artificial boost.
            IF (JJ .GE. 2) THEN
               X(4) = X(4) + REAL(JJ-1)*STEP
            ELSE
               X(4) = X(4) + REAL(JJ+1)*(X(3) - X(2))
            ENDIF
         ENDIF
C    But do not extrapolate beyond the limit.
         IF ((X(4)-XLIM)*(X(4)-X(1)) .GT. 0.D0) THEN
            X(4) = XLIM
            LIMIT = .TRUE.
         ENDIF
         JF = 0
         JS = 2
      ELSE
C
C Root has been bracketed.
C    Define the geometry.
         IF (R32 .LT. 0.D0) THEN
            JO = 1
            JF = 0
            IF (R21 .LT. 0.D0) THEN
               JS = 2
               JD = 3
               JG = JG + 1
            ELSE
               JS = 3
               JD = 2
               JG = 0
            ENDIF
         ELSE
            JS = 1
            JD = 3
            JO = 2
            JF = JF+1
            JG = 0
         ENDIF
         XJD = X(JD)
         XJS = X(JS)
         XJO = X(JO)
         YJD = Y(JD)
C    Exit if the bracket width is less than EX
         IF (ABS(XJS-XJD) .LE. EX(JS,JD)) THEN
            JX = JJ+3
            R = X(3)
            RETURN
         ENDIF
         IF ((ABS(R32) .GE. OME) .OR.
     +       ((JG .GE. 2) .AND. (R32 .LT. -0.7D0)))  THEN
C    Not converging: bisect.
            X(4) = 0.5D0*(XJS+XJD)
            JF = 0
         ELSE
C
C    The following approximates quadratic interpolation when converging
C    nicely.  Otherwise it can accelerate the search when large changes
C    in slope and/or slow movement toward a distant root are found.
C
            XI  = YJD*(XJD-XJS)/(Y(JS)-YJD) + XJD
            IF (ABS (Y(JO)-YJD) .LT. EY) THEN
               XE = XJS
            ELSE
               XE  = YJD*(XJD-XJO)/(Y(JO)-YJD) + XJD
               IF ((XE-XJS)*(XE-XJD) .GT. 0.D0) XE = XJS
            ENDIF
            W = (XJD-XJO)/(XJS-XJO)
            IF ((JF .GE. 2) .AND. (W .LT. 0.3D0) .AND.
     +          (R32 .GT. 0.25D0)) THEN
               X(4) = X(3) + (X(1) - X(3))*SQRT(W)
            ELSE
               X(4) = W*XI + (1.D0-W)*XE
            ENDIF
         ENDIF
      ENDIF
C X(4), Y(4) are the new estimate.
      Y(4) = F(X(4)) - F0
      IF (ABS (Y(4)) .LT. EY) THEN
         JX = JJ+4
         R = X(4)
         RETURN
      ENDIF
C Update indices.
      IF (JS .GE. 2) THEN
         X(1) = X(2)
         Y(1) = Y(2)
      ENDIF
      X(2) = X(3)
      X(3) = X(4)
      Y(2) = Y(3)
      Y(3) = Y(4)
  480 CONTINUE
C This statement should never be reached
      R = X(4)
      JX = JMAX+4
      RETURN
      END
