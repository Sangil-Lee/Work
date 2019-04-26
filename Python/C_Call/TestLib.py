#!/usr/bin/python

import ctypes

def main():
	TestLib = ctypes.cdll.LoadLibrary('./TestLib.so')
#TestLib.SampleMultiple.argtypes=[c_float, c_float]
	print TestLib.SampleAddInt(1, 2)

#print TestLib.SampleMultiple(c_float(2.3), c_float(3.4))
	TestLib.SampleMultiple.argtypes=[ctypes.c_float, ctypes.c_float,]
	TestLib.SampleMultiple.restype=ctypes.c_float
	print TestLib.SampleMultiple(20.3, 3.4)

if __name__ == '__main__':
	main()
