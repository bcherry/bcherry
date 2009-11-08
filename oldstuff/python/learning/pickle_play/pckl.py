import pickle

class transaction:
	def __init__(self,n,a,d):
		self.name = n	#name
		self.amt = a
		self.date = d
	def getName(self):
		return self.name
	def getAmt(self):
		return self.amt
	def getDate(self):
		return self.date
	

