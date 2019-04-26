import numpy
import ctypes

class PatchNetwork:

	model=0

	def __init__(self):            
		self.model=numpy.random.rand(2,2)*255
		print "PatchNetwork instantiated"
		print self.model        
	def __del__(self):
		print "PatchNetwork deleted"
	def Sum(self):          
		return numpy.sum(self.model)
	def Avr(self, count):      
		return numpy.sum(self.model)/count
	def Predict(self, markingPatch):
		print("Predict arg", markingPatch)     
		data = numpy.asarray( markingPatch)
		print("as array", data)
		return data
