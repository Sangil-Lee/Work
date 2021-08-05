import sys
import re
import os

#python3.8 GenSubRVFM.py pv/hePAK/hePAK.pv
argc = len(sys.argv)
print(argc)
if argc != 2:
    print('Usage:python3.8 '+str(sys.argv[0])+' PVlistFile')
    raise SystemExit('EX)python3.8 '+str(sys.argv[0])+' pv/hePAK/hePAK.pv')

filename = str(sys.argv[1])

pvlist = []
arglist = []
f = open(filename, 'r')
for line in f:
    if(line[0] == '#'): continue
    liststr = line.rstrip('\n')
    liststr = liststr.replace(' ','')
    if(liststr == ''): continue
    liststr = re.split('[=]',liststr)
    #print(liststr[1])
    pvlist.append(liststr[0])
    arglist.append(liststr[1])
f.close()

inplist = [] 
for lst in arglist:
    unitlist = []
    lst = re.split('[(, \t)]',lst)
    unitlist = list(filter(None,lst))
    inplist.append(unitlist)


filename = os.path.basename(filename)
subfilename   = 'sub'+filename.rsplit('.', 1)[0]
templfilename = 'templ'+filename.rsplit('.', 1)[0]
templExt = ".template"
subExt = ".sub"
templ = open('../Db/'+templfilename+templExt, 'w')
sub   = open('../Db/'+subfilename+subExt, "w")

#########################################################
nl = '\n'
tab = '\t'
open = '{'
close = '}'
charsize = 60
prefix = '$(SYS)$(SUBSYS)$(DEV)$(SUBDEV)'
signal = '$(SIGNAL)'
charnull = "'\\0'"
comma =','
#########################################################
fieldlist   = ["DESC", "SCAN",       "FLNK", "INAM",            "SNAM",             "INPA", "INPB", "INPC"]
paddinglist = ["13",   "13",         "10",   "18",              "18",               "33",   "33",   "4"]
valuelist   = ["Desc", "1 second",   "",     "InitDnstVisco",   "ProcDnstVisco",    "",     "",     "3"] #INPC 3=Density, 25=Viscosity

fieldlist   = ['\"\"' if value =='' else value for value in fieldlist]
valuelist   = ['\"\"' if value =='' else value for value in valuelist]
paddinglist = ['10' if value =='' else value for value in paddinglist]

text = f'{nl}\
record(sub, "{prefix}{signal}"){nl}\
{open}{nl}\
  field({fieldlist[0]}, "$({fieldlist[0]})"){nl}\
  field({fieldlist[1]}, "$({fieldlist[1]})"){nl}\
  field({fieldlist[2]}, "$({fieldlist[2]})"){nl}\
  field({fieldlist[3]}, "InitDnstVisco"){nl}\
  field({fieldlist[4]}, "ProcDnstVisco"){nl}\
  field({fieldlist[5]}, "$({fieldlist[5]})"){nl}\
  field({fieldlist[6]}, "$({fieldlist[6]})"){nl}\
  field({fieldlist[7]}, "$({fieldlist[7]})"){nl}\
  field(PINI, "YES"){nl}\
{close}{nl}'

templ.write(text)
text = f'file "db/{templfilename}{templExt}" {open} pattern{nl}'
sub.write(text)
signal = re.split('[$()]',prefix+signal)
signal = ' '.join(signal).split()
pvname = ''
for e in signal:
    e = '{message: <{padcnt}}'.format(message = e+',', padcnt=int(11))
    pvname = pvname + e
text = '{'+pvname
sub.write(text)

for idx, field in enumerate(fieldlist):
    idx = idx+1
    if(idx == len(fieldlist)):
        text = field
    else:
        text = field+','
        text = '{message: <{padcnt}}'.format(message = text, padcnt=int(str(paddinglist[idx-1])))
    sub.write(text)

text = '}\n'
sub.write(text)

#for ix, lst in enumerate(inplist):
    #print(lst[0], lst[1])
for cnt in range(len(pvlist)):
    pvnamelist = pvlist[cnt]
    if(pvnamelist.find('DEN') != -1):
        pvnamelist = pvlist[cnt] + ':Dens'
    elif(pvnamelist.find('VIS') != -1):
        pvnamelist = pvlist[cnt] + ':Visco'
        
    inpname = pvnamelist
    pvnamelist = re.split('[-:]', pvnamelist)
    pvname =''
    for indx, e in enumerate(pvnamelist):
        if(signal[indx] == 'SUBSYS' or signal[indx] == 'SUBDEV'):
            e = '-' + e + ':'
        e = '{message: <{padcnt}}'.format(message = e+',', padcnt=int(11))
        pvname = pvname + e
    sncText = '{'+pvname
    sub.write(sncText)
    lst = inplist[cnt]
    for orgidx, value in enumerate(valuelist):
        if(value == "\"\""): value = ""
        if(orgidx == len(valuelist)-1):
            if(fieldlist[orgidx] == 'INPA'):
                pvname = lst[0]+':Press'
                sncText = "\""+ pvname + value + "\""
            elif(fieldlist[orgidx] == 'INPB'):
                pvname = lst[1]+':Temp'
                sncText = "\""+ pvname + value + "\""
            elif(fieldlist[orgidx] == 'INPC'):
                #print(inpname)
                if(inpname.find('DEN') != -1):
                    value = '3'
                elif(inpname.find('VIS') != -1):
                    value = '25'
                sncText = "\""+ value + "\""
            else:
                sncText = "\""+ value + "\""
        else:
            if(fieldlist[orgidx] == 'INPA'):
                pvname = lst[0]+':Press'
                sncText = "\""+ pvname + value + "\","
            elif(fieldlist[orgidx] == 'INPB'):
                pvname = lst[1]+':Temp'
                sncText = "\""+ pvname + value + "\","
            elif(fieldlist[orgidx] == 'INPC'):
                if(inpname.find('DEN') != -1):
                    value = '3'
                elif(inpname.find('VIS') != -1):
                    value = '25'
                sncText = "\""+ value + "\""
            else:
                sncText = "\"" + value+'\",'
        sncText = '{message: <{padcnt}}'.format(message = sncText, padcnt=int(str(paddinglist[orgidx])))
        sub.write(sncText)
    sncText = '}\n'
    sub.write(sncText)
sncText = '}\n'
sub.write(sncText)

templ.close()
sub.close()

#print('Successfully Generated File:', templfilename+templExt, seqfilename+seqExt)
