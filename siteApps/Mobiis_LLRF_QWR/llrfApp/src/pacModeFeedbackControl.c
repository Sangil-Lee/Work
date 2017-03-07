#include <stdio.h>
#include <string.h>

#include <dbDefs.h>
#include <registryFunction.h>
#include <aSubRecord.h>
#include <epicsExport.h>

static long pacModeFeedbackCtrlInit(aSubRecord *precord)
{
//	printf("%s record init routine called(%p)!!\n",
//		precord->name, (void*)precord);
	return 0;
}

static long pacModeFeedbackCtrlProc(aSubRecord *precord)
{
	unsigned short pac_mode = *((unsigned short *)(precord->a));
	unsigned short prev_pac_mode = *((unsigned short *)(precord->b));
    unsigned short phase_fb = *((unsigned short *)(precord->c));
    unsigned short amplitude_fb = *((unsigned short *)(precord->d));
    
    unsigned short res_prev_pac_mode;
    unsigned short res_phase_fb;
    unsigned short res_amplitude_fb;

    if (pac_mode != prev_pac_mode) {
        res_prev_pac_mode = pac_mode;
        res_phase_fb = 0;
        res_amplitude_fb = 0;
    } else {
        res_phase_fb = phase_fb;
        res_amplitude_fb = amplitude_fb;
    }

	*((unsigned short *)precord->vala) = res_prev_pac_mode;
    *((unsigned short *)precord->valb) = res_phase_fb;
    *((unsigned short *)precord->valc) = res_amplitude_fb;

	return 0;
}

epicsRegisterFunction(pacModeFeedbackCtrlInit);
epicsRegisterFunction(pacModeFeedbackCtrlProc);
