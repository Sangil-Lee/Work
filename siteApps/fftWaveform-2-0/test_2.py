import time
import unittest

import numpy as np
from epics import PV


class TestFFTW(unittest.TestCase):

    def test_1ke_asub_1sine(self):
        """
        Test a 1k array with a single sine wave - using aSub
        """
        real_is_in = False
        imag_is_in = False

        def data_callback(pvname=None, **kwargs):
            nonlocal real_is_in, imag_is_in
            if pvname.endswith('real'):
                real_is_in = True
            elif pvname.endswith('imag'):
                imag_is_in = True

        arrsize = 1000
        #arrsize = 1024 * 2  

        #step = 0.0009765625

        amplitude1 = 20
        amplitude2 = 50
        amplitude3 = 75

        FreqA = 201
        FreqB = 305
        FreqC = 427

        times = 1
        #times = 1/(arrsize/2)

        data = amplitude1*times*np.cos(FreqA*2 * np.pi * np.arange(arrsize) / arrsize) + amplitude2*times*np.sin(FreqB * 2 * np.pi * np.arange(arrsize) / arrsize) + amplitude3*times*np.sin(FreqC * 2 * np.pi * np.arange(arrsize) / arrsize)
        #data = amplitude1*times*np.cos(FreqA*2 * np.pi * (np.arange(arrsize)/arrsize)/2) + amplitude2*times*np.sin(FreqB * 2 * np.pi * (np.arange(arrsize) / arrsize)/2) + amplitude3*times*np.sin(FreqC * 2 * np.pi * (np.arange(arrsize)/arrsize)/2)

        #arrsize = 500
        #Fs = 2*arrsize
        #Ts = 1./Fs

        #phase = 0.
        #ampA = 20
        #ampB = 50
        #ampC = 75
        #freqA = 30
        #freqB = 60
        #freqC = 90

        #data = []
        #for i in range(0,Fs):
        #    phase += Ts
        #    if phase > 2.*np.pi:
        #        phase = 0.
        #    data.append(ampA*np.cos(2*np.pi*phase*freqA) + ampB*np.sin(2*np.pi*phase*freqB) + ampC*np.sin(2*np.pi*phase*freqC))

        wintype = PV('ctrlslab-fftWave:wintype')
        inp = PV('ctrlslab-fftWave:inp-real')
        outr = PV('ctrlslab-fftWave:out-real', callback=data_callback)
        outi = PV('ctrlslab-fftWave:out-imag', callback=data_callback)
        while not all([real_is_in, imag_is_in]):
            time.sleep(0.001)
        real_is_in = False
        imag_is_in = False

        wintype.put('None', wait=True)
        inp.put(data, wait=True)

        result = np.fft.rfft(data)

        while not all([real_is_in, imag_is_in]):
            time.sleep(0.001)

        self.assertTrue(np.allclose(result.imag, outi.get()))
        self.assertTrue(np.allclose(result.real, outr.get()))

if __name__ == '__main__':
    unittest.main()
