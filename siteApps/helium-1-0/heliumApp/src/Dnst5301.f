C This is a program to calculate the density of GHe in the dewar.
C The following assignments are required.
C
      Real*8 function Dnst5301(Press)
			REAL*8 Press
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
      JOUT = 00000
C
C-------------------------------------- INPUT PARAMETER IDENTIFICATION
C 
C Two input variables are required to locate a thermodynamic state point.
C     They are specified by: JIN1, VALU1, JIN2, VALU2. 
C JIN1 is an input variable for the saturated vapor.
C JIN2 is an input variable for the pressure of GHe in the dewar.

C ------------------------------------- INPUT DATA FOR TEST
C
C Variables can be in either order.
C
C         JIN1 = 9
C         VALU1= 1.
C         JIN2 = 1
C         VALU2= 100.
C
C ------------------------------------- INPUT DATA
C
         JIN1 = 9
         VALU1= 1.
         JIN2 = 1
C
C Pressure unit shall be changed from "bara" to "kPa" 
C
         VALU2= Press*100
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
C Note: PROP(3,j) is the density of GHe in the dewar. 
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
C------------------------------------- OUTPUT PARAMETER LABELS
C
C Note: PROP(3,j) is the density of GHe in the dewar. 
C The unit is "g/L".
C
C------------------------------------- OUTPUT DATA					   
C

      IF (IDID .GE.1)  THEN
C
C If the calculation was finished successfully,
C the output data is inputted into the result variable, "Dnst5301".
C
	   Dnst5301 = PROP(3,0)
C
C Print selected results after a successful calculation for test. 
C 
C         WRITE (1, 10) Dnst5301, IDID
      ELSE
C
C  Otherwise print error message for test.
C 
C         CALL ERRMSG (MESSAG, IDID)
C          WRITE (1, '(1X,A60)') MESSAG
C
C If the calculation was failed, 
C the Dnst5301 will be zero for EPICS to notice the failure.
C
	   Dnst5301 = 0
      ENDIF

C      CLOSE (1, STATUS='KEEP')

C
C------------------------------------- END THE PROGRAM 
C  10  FORMAT (G13.7, I4)
      END
