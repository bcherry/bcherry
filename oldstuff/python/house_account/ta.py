from datetime import date

class trans:
	def __init__(self,d,a,b,n):
		self.date = d
		self.amt = a
		self.bal = b
		self.note = n
	def getDate(self):	return self.date
	def getAmt(self):	return self.amt
	def getBal(self):	return self.bal
	def getNote(self):	return self.note
	
	def printIt(self):
		print '%s %s %s %s' % (self.date.isoformat(),('$%.2f' % (self.amt)).rjust(10),('$%.2f' % (self.bal)).rjust(10),self.note)
			
class account:
	def __init__(self,n,r):
		self.name = n
		self.rent = r
		self.bal = 0
		self.trans_list = []
		self.trans_list.append(trans(date.today(),0,0,'Account Creation'))
	
	def add(self,amt,note):
		self.bal += amt
		self.trans_list.append(trans(date.today(),amt,self.bal,note))
	def printHistory(self):
		print '%s %s %s Notes' % ('Date'.center(10),'Amount'.center(10),'Balance'.center(10))
		for t in self.trans_list:
			t.printIt()
	
	def getBal(self):
		return self.bal
	
	def getRent(self):
		return self.rent
	
	def setRent(self,r):
		self.rent = r
	
	def getName(self):
		return self.name
		
	
