from epics import PV, caget, caput
import argparse
import csv
import re

def str2bool(s):
    return s.lower() in ('yes', 'true', 't', '1')

def setAlarms():
    readfile = args.file
    try:
        with open(readfile, 'r', encoding='utf-8') as file:
            csv_reader = csv.reader(file)
            for i, row in enumerate(csv_reader):
                print(i, '===>>>')
                pvname = ' '.join(row)

                print('[Before]')
                pv_hihi = PV(pvname+'.HIHI')
                pv_high = PV(pvname+'.HIGH')
                pv_low  = PV(pvname+'.LOW')
                pv_lolo = PV(pvname+'.LOLO')

                if pv_hihi.get() == None:
                    print('can not connect with C.A: -->', pvname)
                    break
                print(pvname+'.HIHI=', pv_hihi.get(), ' --> Set.HIHI = [', args.hihi, ']')
                print(pvname+'.HIGH=', pv_high.get(), ' --> Set.HIGH = [', args.high, ']')
                print(pvname+'.LOW=',  pv_low.get(),  ' --> Set.LOW  = [', args.low,  ']')
                print(pvname+'.LOLO=', pv_lolo.get(), ' --> Set.LOLO = [', args.lolo, ']')

                pv_hihi.put(float(args.hihi), wait=True)
                pv_high.put(float(args.high), wait=True)
                pv_low.put(float(args.low), wait=True)
                pv_lolo.put(float(args.lolo), wait=True)

                print('[After]')
                print(pvname+'.HIHI=', pv_hihi.get())
                print(pvname+'.HIGH=', pv_high.get())
                print(pvname+'.LOW=',  pv_low.get())
                print(pvname+'.LOLO=', pv_lolo.get())

                print('<<<===')
            file.close()
    except IOError:
        print('Exception:could not read file:', readfile)

def checkAlarms():
    readfile = args.file
    try:
        with open(args.file, 'r', encoding='utf-8') as file:
            csv_reader = csv.reader(file)
            for i, row in enumerate(csv_reader):
                print(i, '===>>>')
                pvname = ' '.join(row)

                pv_hihi = PV(pvname+'.HIHI')
                pv_high = PV(pvname+'.HIGH')
                pv_low  = PV(pvname+'.LOW')
                pv_lolo = PV(pvname+'.LOLO')

                hihi_val = pv_hihi.get()
                high_val = pv_high.get()
                low_val  = pv_low.get()
                lolo_val = pv_lolo.get()

                bok = True
                if hihi_val != args.hihi:
                    bok = False
                    print(pvname + ".HIHI[{0}] --> Args.HIHI[{1}]".format(hihi_val, args.hihi) )
                if high_val != args.high:
                    bok = False
                    print(pvname + ".HIGH[{0}] --> Args.HIGH[{1}]".format(high_val, args.high) )
                if low_val  != args.low:
                    bok = False
                    print(pvname + ".LOW[{0}]  --> Args.LOW[{1}]".format(low_val, args.low) )
                if lolo_val != args.lolo :
                    bok = False
                    print(pvname + ".LOLO[{0}] --> Args.LOLO[{1}]".format(lolo_val, args.lolo) )

                if bok == True:
                    print("OK")

                print('<<<===')
            file.close()
    except IOError:
        print('Exception:could not read file:', readfile)

def setAlarmPVs(s, bcheck):
    print(s,',bcheck:',bcheck)
    pvnames = s
    pvnames = re.split(r'[\s,]+', pvnames)
    for i, pvname in enumerate(pvnames):
        print(i, '===>>>')
        print(pvname)
        if bcheck == True:
            pv_hihi = PV(pvname+'.HIHI')
            pv_high = PV(pvname+'.HIGH')
            pv_low  = PV(pvname+'.LOW')
            pv_lolo = PV(pvname+'.LOLO')
            if pv_hihi.get() == None:
                print('can not connect with C.A: -->', pvname)
                break

            hihi_val = pv_hihi.get()
            high_val = pv_high.get()
            low_val  = pv_low.get()
            lolo_val = pv_lolo.get()

            bok = True
            if hihi_val != args.hihi:
                bok = False
                print(pvname + ".HIHI[{0}] --> Args.HIHI[{1}]".format(hihi_val, args.hihi) )
            if high_val != args.high:
                bok = False
                print(pvname + ".HIGH[{0}] --> Args.HIGH[{1}]".format(high_val, args.high) )
            if low_val  != args.low:
                bok = False
                print(pvname + ".LOW[{0}]  --> Args.LOW[{1}]".format(low_val, args.low) )
            if lolo_val != args.lolo :
                bok = False
                print(pvname + ".LOLO[{0}] --> Args.LOLO[{1}]".format(lolo_val, args.lolo) )

            if bok == True:
                print("OK")
        else:
            print('[Before]')
            pv_hihi = PV(pvname+'.HIHI')
            pv_high = PV(pvname+'.HIGH')
            pv_low  = PV(pvname+'.LOW')
            pv_lolo = PV(pvname+'.LOLO')

            if pv_hihi.get() == None:
                print('can not connect with C.A: -->', pvname)
                break
            print(pvname+'.HIHI=', pv_hihi.get(), ' --> Set.HIHI = [', args.hihi, ']')
            print(pvname+'.HIGH=', pv_high.get(), ' --> Set.HIGH = [', args.high, ']')
            print(pvname+'.LOW=',  pv_low.get(),  ' --> Set.LOW  = [', args.low,  ']')
            print(pvname+'.LOLO=', pv_lolo.get(), ' --> Set.LOLO = [', args.lolo, ']')

            pv_hihi.put(float(args.hihi), wait=True)
            pv_high.put(float(args.high), wait=True)
            pv_low.put(float(args.low), wait=True)
            pv_lolo.put(float(args.lolo), wait=True)

            print('[After]')
            print(pvname+'.HIHI=', pv_hihi.get())
            print(pvname+'.HIGH=', pv_high.get())
            print(pvname+'.LOW=',  pv_low.get())
            print(pvname+'.LOLO=', pv_lolo.get())

        print('<<<===')



parser = argparse.ArgumentParser(description='Alarm Setpoint Configure')

parser.add_argument('-check', type=bool, default=argparse.SUPPRESS)
parser.add_argument('-file',  help='Alarm list for changing alarm setpoints')
parser.add_argument('-pvs',   help='PV names:e.g., pvname-1 pvname-2')
parser.add_argument('-lolo',  type= float, help='LOLO alarm setpoint to change')
parser.add_argument('-low',   type= float, help='LOW alarm setpoint to change')
parser.add_argument('-high',  type= float, help='HIGH alarm setpoint to change')
parser.add_argument('-hihi',  type= float, help='HIHI alarm setpoint to change')

args = parser.parse_args()
bcheck = str2bool('{}'.format('check' in args))
print("BPVs: ", args.pvs)

print('Parsed arguments: {}'.format(args))
print('-file', args.file)
print('-pvs',  args.pvs)
print('-lolo', args.lolo)
print('-low',  args.low)
print('-high', args.high)
print('-hihi', args.hihi)

if __name__ == "__main__":

    if args.pvs != None:
        setAlarmPVs(args.pvs, bcheck)
    elif bcheck == True:
        checkAlarms()
    else:
        setAlarms()
