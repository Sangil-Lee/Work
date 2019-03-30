#!/usr/bin/python
import epics
import time, sys
import numpy
import subprocess
num_cbs = 0
timestamps = []
samples = 1000

def cb_pv(timestamp=None, **kw):
    global num_cbs, timestamps, samples
    if num_cbs == 0:
        num_cbs = 1
        return
    if num_cbs == samples:
        return
    timestamps.append(timestamp)
    num_cbs += 1


def main():
    freqs = [3000, 2000, 1000, 500, 100, 50, 40, 10]
    global num_cbs, timestamps, samples

    for freq in freqs:
        proc = 'FREQ=%d iocsh ../startup/ecat2training.cmd' % freq
        iocoutlog = open('ecat2test-stdout-%d.log' % freq,'w+')
        iocerrlog = open('ecat2test-stderr-%d.log' % freq,'w+')
        ioc = subprocess.Popen(proc, subprocess.PIPE, stdout=iocoutlog, stderr=iocerrlog, shell=True)     
        time.sleep(5)

        pv = epics.PV(sys.argv[1], form='time')
        pv.add_callback(cb_pv)
    
        while num_cbs < samples:
            time.sleep(0.5)
        pv.clear_callbacks()
        pv.disconnect()
        ioc.terminate()
        ioc.kill()
        subprocess.call(["killall","/opt/epics/bases/base-3.15.4/bin/centos7-x86_64/softIoc"])
        iocoutlog.close()
        iocerrlog.close()

        

        tdiff = numpy.diff(timestamps)
        print 'Statistics running bus cycle loop in %d Hz, over %d samples' % (freq, num_cbs)	
        print 'TIME -- Max: %.6f ms, Min: %.6f ms, Mean: %.6f ms, Sdev: %.6f ms' % (numpy.amax(tdiff)*1000, numpy.amin(tdiff)*1000, numpy.mean(tdiff)*1000, numpy.std(tdiff)*1000)	
        print 'FREQ -- Mean: %.6f Hz' % (1/numpy.mean(tdiff))	
        timestamps = []
        tdiff = []
        num_cbs = 0

if __name__ == '__main__':
    main()
