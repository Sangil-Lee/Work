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

        arrsize = 2048
        #arrsize = 1024 * 2  

        #step = 0.0009765625

        amplitude1 = 20
        amplitude2 = 50
        amplitude3 = 75

        FreqA = 201
        FreqB = 305
        FreqC = 999

        data = amplitude1*np.cos(FreqA*2 * np.pi * np.arange(arrsize) / arrsize) + amplitude2*np.sin(FreqB * 2 * np.pi * np.arange(arrsize) / arrsize) + amplitude3*np.sin(FreqC * 2 * np.pi * np.arange(arrsize) / arrsize)

        inp = PV('ctrlslab-fftWave:Wave')
        inp.put(data, wait=True)

        asubproc = PV('ctrlslab-fftWave:ASubFFT.PROC')
        asubproc.put('1', wait=True)

if __name__ == '__main__':
    unittest.main()
