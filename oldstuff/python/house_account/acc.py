from ta import *
import pickle

class glb:
	fname = 'data.acc'
	f = open(fname)
	accounts = pickle.load(f)
	f.close()

def main():
	print 'Welcome to The House Manager'
	print 'What would you like to do?'
	opt = ['File Rent Check','Utilities Bill','Manage Existing Account','Create New Account']
	funcs = [rentCheck,util,manage,newAcc]
	for i in range(len(opt)):
		print '(%d) %s' % (i+1,opt[i])
	funcs[int(raw_input('? '))-1]()
	
	
		
	f = open(glb.fname,'w')
	pickle.dump(glb.accounts,f)
	f.close()
	
def rentCheck():
	print 'rent'

def util():
	print 'Please enter the utility bill:'
	amt = float(raw_input('$'))
	glb.accounts[0].add(-1*amt,'Utilities bill')
	amt = round(amt / (float(len(glb.accounts) - 1)),2)
	print 'Bill is $%.2f per person' % (amt)
	for acc in glb.accounts[1:]:
		acc.add(-1*amt,'Utilities bill')
	

def manage():
	print 'Which account would you like to manage?'
	for i in range(len(glb.accounts)):
		print '(%d) %s' % (i+1,glb.accounts[i].getName())
	acc = glb.accounts[int(raw_input('? ')) - 1]
	choices = ['Print History','Report Balance','Report Deposit','Report Withdrawal','Change Name']
	func = [printHistory,repBal,repDep,repWith,changeName]
	for i in range(len(choices)):
		print '(%d) %s' % (i+1,choices[i])
	func[int(raw_input('? ')) - 1](acc)
		
def printHistory(acc):
	acc.printHistory()
def repBal(acc):
	print 'Current Balance is $%.2f' % (acc.getBal())
def repDep(acc):
	print 'What amount is the deposit?'
	amt = round(float(raw_input('$')),2)
	print 'Any Notes?'
	note = raw_input()
	acc.add(amt,note)
	glb.accounts[0].add(amt,'%s-- %s' % (acc.getName(),note))
def repWith(acc):
	print 'What amount is the withdrawal?'
	amt = round(float(raw_input('$')),2)
	print 'Any Notes?'
	note = raw_input()
	acc.add(-1*amt,note)
	glb.accounts[0].add(-1*amt,'%s-- %s' % (acc.getName(),note))
def changeName(acc):
	print 'What is the new name?'
	acc.setName(raw_input())


def newAcc():
	print 'new'

if __name__ == '__main__':
	main()
