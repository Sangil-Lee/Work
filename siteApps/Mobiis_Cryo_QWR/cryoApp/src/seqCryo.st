/*************************************************************************\
This file is distributed subject to a Software License Agreement found
in the file LICENSE that is included with this distribution.
\*************************************************************************/
program seqCryo
option +r;

short ccv1;
assign ccv1 to "{sysname}CCV001:SetPB";

monitor ccv1;

short ccv2;
assign ccv2 to "{sysname}CCV002:SetPB";

monitor ccv2;

short tm[7];
assign tm[0] to "{sysname}TM001:state";
assign tm[1] to "{sysname}TM002:state";
assign tm[2] to "{sysname}TM003:state";
assign tm[3] to "{sysname}TM004:state";
assign tm[4] to "{sysname}TM005:state";
assign tm[5] to "{sysname}TM006:state";
assign tm[6] to "{sysname}TM007:state";

short vs;
assign vs to "{sysname}IOC:vacumm";

short v[6];

assign v[0] to "{sysname}Cath001:Dest.SEVR";
assign v[1] to "{sysname}Cath002:Dest.SEVR";
assign v[2] to "{sysname}Cath003:Dest.SEVR";
assign v[3] to "{sysname}Cath004:Dest.SEVR";
assign v[4] to "{sysname}Cath005:Dest.SEVR";
assign v[5] to "{sysname}Cath006:Dest.SEVR";

monitor v;

short cs;
assign cs to "{sysname}IOC:cryogenic";

short c[5];

assign c[0] to "{sysname}2KL001:Dest.SEVR";
assign c[1] to "{sysname}2KL002:Dest.SEVR";
assign c[2] to "{sysname}4KL001:Dest.SEVR";
assign c[3] to "{sysname}4KL002:Dest.SEVR";
assign c[4] to "{sysname}PG001:Dest.SEVR";

monitor c;

short ts;
assign ts to "{sysname}IOC:temperature";

short t[40];
assign t[0] to "{sysname}TM001:CX001.SEVR";
assign t[1] to "{sysname}TM001:CX002.SEVR";
assign t[2] to "{sysname}TM001:CX003.SEVR";
assign t[3] to "{sysname}TM001:CX004.SEVR";
assign t[4] to "{sysname}TM001:CX005.SEVR";
assign t[5] to "{sysname}TM001:CX006.SEVR";
assign t[6] to "{sysname}TM001:CX007.SEVR";
assign t[7] to "{sysname}TM001:CX008.SEVR";
assign t[8] to "{sysname}TM001:CX009.SEVR";
assign t[9] to "{sysname}TM001:CX010.SEVR";
assign t[10] to "{sysname}TM001:CX011.SEVR";
assign t[11] to "{sysname}TM001:CX012.SEVR";
assign t[12] to "{sysname}TM002:CX013.SEVR";
assign t[13] to "{sysname}TM002:CX014.SEVR";
assign t[14] to "{sysname}TM002:PT001.SEVR";
assign t[15] to "{sysname}TM002:PT002.SEVR";
assign t[16] to "{sysname}TM002:PT003.SEVR";
assign t[17] to "{sysname}TM002:PT004.SEVR";
assign t[18] to "{sysname}TM003:CX021.SEVR";
assign t[19] to "{sysname}TM003:CX022.SEVR";
assign t[20] to "{sysname}TM004:CX023.SEVR";
assign t[21] to "{sysname}TM004:CX024.SEVR";
assign t[22] to "{sysname}TM004:CX025.SEVR";
assign t[23] to "{sysname}TM004:CX026.SEVR";
assign t[24] to "{sysname}TM005:CX035.SEVR";
assign t[25] to "{sysname}TM005:CX036.SEVR";
assign t[26] to "{sysname}TM005:PT015.SEVR";
assign t[27] to "{sysname}TM005:PT016.SEVR";
assign t[28] to "{sysname}TM005:PT017.SEVR";
assign t[29] to "{sysname}TM005:PT018.SEVR";
assign t[30] to "{sysname}TM006:PT020.SEVR";
assign t[31] to "{sysname}TM006:PT021.SEVR";
assign t[32] to "{sysname}TM006:PT022.SEVR";
assign t[33] to "{sysname}TM006:PT023.SEVR";
assign t[34] to "{sysname}TM006:PT024.SEVR";
assign t[35] to "{sysname}TM006:PT025.SEVR";
assign t[36] to "{sysname}TM006:PT026.SEVR";
assign t[37] to "{sysname}TM006:PT027.SEVR";
assign t[38] to "{sysname}TM007:PT030.SEVR";
assign t[39] to "{sysname}TM007:PT031.SEVR";

monitor t;

short rs;
assign rs to "{sysname}IOC:rfdevice";

short ns;
assign ns to "{sysname}IOC:network";

short as;
assign as to "{sysname}IOC:alarm";

monitor vs;
monitor cs;
monitor ts;
monitor rs;
monitor ns;

ss CCV1_pid_state
{
  state CCV1pidSet
	{
      	when (ccv1 == 1)
	    	{
//printf("CCV1 SET\n");
	    	} state CCV1pidReset
 	}
   	
  state CCV1pidReset
	{
		when (delay(5)) {
//printf("CCV1 RESET\n");
		  	ccv1 = 0;
		  	pvPut(ccv1);
	    } state CCV1pidSet
   	}
}

ss CCV2_pid_state
{
  state CCV2pidSet
	{
      	when (ccv2 == 1)
	    	{
//printf("CCV2 SET\n");
	    	} state CCV2pidReset
 	}
   	
  state CCV2pidReset
	{
		when (delay(5)) {
//printf("CCV2 RESET\n");
		  	ccv2 = 0;
		  	pvPut(ccv2);
	    } state CCV2pidSet
   	}
}

ss alarm_state
{
	state alarmNoAlarm
	{
	    when ((vs != 0) || (cs != 0) || (ts != 0) || (rs != 0) || (ns != 0))
	    {
	    	as = 1;
//	printf("changing alarm to Alarm\n");
		pvPut(as);
	    } state alarmYesAlarm
	}
	state alarmYesAlarm
	{
	
	    when ((vs == 0) && (cs == 0) && (ts == 0) && (rs == 0) && (ns == 0))
	    {
//printf("changing vacumm to No Alarm\n");
	    	as = 0;
	    	pvPut(as);
	    } state alarmNoAlarm
	}
}

ss vacumm_state
{
	state vacummNoAlarm
	{
	    when ((v[0] != 0) || (v[1] != 0) || (v[2] != 0) || (v[3] != 0) || (v[4] != 0) || (v[5] != 0))
	    {
	    	vs = 1;
//	printf("changing vacumm to Alarm\n");
		pvPut(vs);
	    } state vacummYesAlarm
	}
	state vacummYesAlarm
	{
	
	    when ((v[0] == 0) && (v[1] == 0) && (v[2] == 0) && (v[3] == 0) && (v[4] == 0) && (v[5] == 0))
	    {
//printf("changing vacumm to No Alarm\n");
	    	vs = 0;
	    	pvPut(vs);
	    } state vacummNoAlarm
	}
}

ss cryogenic_state
{
	state cryogenicNoAlarm
	{
	    when ((c[0] != 0) || (c[1] != 0) || (c[2] != 0) || (c[3] != 0) || (c[4] != 0) ||
	          (t[0] != 0) || (t[1] != 0) || (t[2] != 0) || (t[3] != 0) || (t[4] != 0) || (t[5] != 0) || (t[6] != 0)|| (t[7] != 0) || (t[8] != 0) || (t[9] != 0) ||
	          (t[18] != 0) || (t[19] != 0) || (t[20] != 0) || (t[21] != 0) || (t[22] != 0) || (t[23] != 0) || (t[24] != 0) || (t[25] != 0))
	    {
	    	cs = 1;
//	printf("changing cryogenic to Alarm\n");
		pvPut(cs);
	    } state cryogenicYesAlarm
	}
	state cryogenicYesAlarm
	{
	
      when ((c[0] == 0) && (c[1] == 0) && (c[2] == 0) && (c[3] == 0) && (c[4] == 0) &&
	          (t[0] == 0) && (t[1] == 0) && (t[2] == 0) && (t[3] == 0) && (t[4] == 0) && (t[5] == 0) && (t[6] == 0)&& (t[7] == 0) && (t[8] == 0) && (t[9] == 0) &&
	          (t[18] == 0) && (t[19] == 0) && (t[20] == 0) && (t[21] == 0) && (t[22] == 0) && (t[23] == 0) && (t[24] == 0) && (t[25] == 0))
	    {
//printf("changing cryogenic to No Alarm\n");
	    	cs = 0;
	    	pvPut(cs);
	    } state cryogenicNoAlarm
	}
}

ss temp_state
{
	state tempNoAlarm
	{
	    when ((t[0] != 0) || (t[1] != 0) || (t[2] != 0) || (t[3] != 0) || (t[4] != 0) || (t[5] != 0) || (t[6] != 0)|| (t[7] != 0) || (t[8] != 0) || (t[9] != 0) ||     
	          (t[10] != 0) || (t[11] != 0) || (t[12] != 0) || (t[13] != 0) || (t[14] != 0) || (t[15] != 0) || (t[16] != 0)|| (t[17] != 0) || (t[18] != 0) || (t[19] != 0) || 
	          (t[20] != 0) || (t[21] != 0) || (t[22] != 0) || (t[23] != 0) || (t[24] != 0) || (t[25] != 0) || (t[26] != 0)|| (t[27] != 0) || (t[28] != 0) || (t[29] != 0) ||  
	          (t[30] != 0) || (t[31] != 0) || (t[32] != 0) || (t[33] != 0) || (t[34] != 0) || (t[35] != 0) || (t[36] != 0)|| (t[37] != 0) || (t[38] != 0) || (t[39] != 0))
	    {
	    	ts = 1;
//	printf("changing temp to Alarm\n");
		pvPut(ts);
	    } state tempYesAlarm
	}
	state tempYesAlarm
	{
	    when ((t[0] == 0) && (t[1] == 0) && (t[2] == 0) && (t[3] == 0) && (t[4] == 0) && (t[5] == 0) && (t[6] == 0)&& (t[7] == 0) && (t[8] == 0) && (t[9] == 0) &&     
	          (t[10] == 0) && (t[11] == 0) && (t[12] == 0) && (t[13] == 0) && (t[14] == 0) && (t[15] == 0) && (t[16] == 0)&& (t[17] == 0) && (t[18] == 0) && (t[19] == 0) && 
	          (t[20] == 0) && (t[21] == 0) && (t[22] == 0) && (t[23] == 0) && (t[24] == 0) && (t[25] == 0) && (t[26] == 0)&& (t[27] == 0) && (t[28] == 0) && (t[29] == 0) &&  
	          (t[30] == 0) && (t[31] == 0) && (t[32] == 0) && (t[33] == 0) && (t[34] == 0) && (t[35] == 0) && (t[36] == 0)&& (t[37] == 0) && (t[38] == 0) && (t[39] == 0))	
	    {
//printf("changing temp to No Alarm\n");
	    	ts = 0;
	    	pvPut(ts);
	    } state tempNoAlarm
	}
}

ss rf_state
{
	state rfNoAlarm
	{
	    when ((t[0] != 0) || (t[1] != 0) || (t[2] != 0) || (t[3] != 0) ||     
	          (t[10] != 0) || (t[11] != 0) || (t[12] != 0) || (t[13] != 0) || (t[14] != 0) || (t[15] != 0) || (t[16] != 0)|| (t[17] != 0) ||
	          (t[20] != 0) || (t[21] != 0) || (t[22] != 0) || (t[23] != 0))
	    {
	    	rs = 1;
//	printf("changing rf to Alarm\n");
		pvPut(rs);
	    } state rfYesAlarm
	}
	state rfYesAlarm
	{
	    when ((t[0] == 0) && (t[1] == 0) && (t[2] == 0) && (t[3] == 0) &&     
	          (t[10] == 0) && (t[11] == 0) && (t[12] == 0) && (t[13] == 0) && (t[14] == 0) && (t[15] == 0) && (t[16] == 0)&& (t[17] == 0) &&
	          (t[20] == 0) && (t[21] == 0) && (t[22] == 0) && (t[23] == 0))
	    {
//printf("changing rf to No Alarm\n");
	    	rs = 0;
	    	pvPut(rs);
	    } state rfNoAlarm
	}
}

ss plc_state
{
	state plcNoAlarm
	{
	    when ((v[0] == 3) && (v[1] == 3) && (c[0] == 3) && (c[1] == 3)) /* all point is INVALID	*/
	    {
//printf("changing plc network to Alarm\n");
	    	ns = 1;
		pvPut(ns);
	    } state plcYesAlarm
	}
	state plcYesAlarm
	{
	    when ((v[0] != 3) || (v[1] != 3) || (c[0] != 3) || (c[1] != 3))
	    {
//printf("changing plc network to No Alarm\n");
	    	ns = 0;
	    	pvPut(ns);
	    } state plcNoAlarm
	}
}

ss tm1_state
{
	state tm1NoAlarm
	{
	    when ((t[0] == 3) && (t[1] == 3) && (t[2] == 3) && (t[3] == 3)) /* all point is INVALID	*/
	    {
//printf("changing tm1 to Alarm\n");
	    	tm[0] = 1;
        	pvPut(tm[0]);
	    } state tm1YesAlarm
	}
	state tm1YesAlarm
	{
	    when ((t[0] != 3) || (t[1] != 3) || (t[2] != 3) || (t[3] != 3))
	    {
//printf("changing tm1 to No Alarm\n");
	    	tm[0] = 0;
	    	pvPut(tm[0]);
	    } state tm1NoAlarm
	}
}

ss tm2_state
{
	state tm2NoAlarm
	{
	    when ((t[12] == 3) && (t[13] == 3) && (t[14] == 3) && (t[15] == 3)) /* all point is INVALID	*/
	    {
//printf("changing tm2 to Alarm\n");
	    	tm[1] = 1;
        	pvPut(tm[1]);
	    } state tm2YesAlarm
	}
	state tm2YesAlarm
	{
	    when ((t[12] != 3) || (t[13] != 3) || (t[14] != 3) || (t[15] != 3))
	    {
//printf("changing tm2 to No Alarm\n");
	    	tm[1] = 0;
	    	pvPut(tm[1]);
	    } state tm2NoAlarm
	}
}

ss tm3_state
{
	state tm3NoAlarm
	{
	    when ((t[18] == 3) && (t[19] == 3)) /* all point is INVALID	*/
	    {
//printf("changing tm3 to Alarm\n");
	    	tm[2] = 1;
        	pvPut(tm[2]);
	    } state tm3YesAlarm
	}
	state tm3YesAlarm
	{
	    when ((t[18] != 3) || (t[19] != 3))
	    {
//printf("changing tm3 to No Alarm\n");
	    	tm[2] = 0;
	    	pvPut(tm[2]);
	    } state tm3NoAlarm
	}
}

ss tm4_state
{
	state tm4NoAlarm
	{
	    when ((t[20] == 3) && (t[21] == 3) && (t[22] == 3) && (t[23] == 3)) /* all point is INVALID	*/
	    {
//printf("changing tm4 to Alarm\n");
	    	tm[3] = 1;
        	pvPut(tm[3]);
	    } state tm4YesAlarm
	}
	state tm4YesAlarm
	{
	    when ((t[20] != 3) || (t[21] != 3) || (t[22] != 3) || (t[23] != 3))
	    {
//printf("changing tm4 to No Alarm\n");
	    	tm[3] = 0;
	    	pvPut(tm[3]);
	    } state tm4NoAlarm
	}
}

ss tm5_state
{
	state tm5NoAlarm
	{
	    when ((t[24] == 3) && (t[25] == 3) && (t[26] == 3) && (t[27] == 3)) /* all point is INVALID	*/
	    {
//printf("changing tm5 to Alarm\n");
	    	tm[4] = 1;
        	pvPut(tm[4]);
	    } state tm5YesAlarm
	}
	state tm5YesAlarm
	{
	    when ((t[24] != 3) || (t[25] != 3) || (t[26] != 3) || (t[27] != 3))
	    {
//printf("changing tm5 to No Alarm\n");
	    	tm[4] = 0;
	    	pvPut(tm[4]);
	    } state tm5NoAlarm
	}
}

ss tm6_state
{
	state tm6NoAlarm
	{
	    when ((t[30] == 3) && (t[31] == 3) && (t[32] == 3) && (t[33] == 3)) /* all point is INVALID	*/
	    {
//printf("changing tm6 to Alarm\n");
	    	tm[5] = 1;
        	pvPut(tm[5]);
	    } state tm6YesAlarm
	}
	state tm6YesAlarm
	{
	    when ((t[30] != 3) || (t[31] != 3) || (t[32] != 3) || (t[33] != 3))
	    {
//printf("changing tm6 to No Alarm\n");
	    	tm[5] = 0;
	    	pvPut(tm[5]);
	    } state tm6NoAlarm
	}
}

ss tm7_state
{
	state tm7NoAlarm
	{
	    when ((t[38] == 3) && (t[39] == 3)) /* all point is INVALID	*/
	    {
//printf("changing tm7 to Alarm\n");
	    	tm[6] = 1;
        	pvPut(tm[6]);
	    } state tm7YesAlarm
	}
	state tm7YesAlarm
	{
	    when ((t[38] != 3) || (t[39] != 3))
	    {
//printf("changing tm7 to No Alarm\n");
	    	tm[6] = 0;
	    	pvPut(tm[6]);
	    } state tm7NoAlarm
	}
}
