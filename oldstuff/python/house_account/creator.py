from ta import *
import pickle

fname = 'data.acc'

accounts = [account('main',4200),account('ben',500),account('toby',500),account('joe',500),account('steo',600),account('james',600),account('chris',750),account('colin',750)]

f = open(fname,'w')
pickle.dump(accounts,f)
f.close()
