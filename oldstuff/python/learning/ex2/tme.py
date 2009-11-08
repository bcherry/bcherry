import sys

def checkline():
	global l
	global wordcount
	w = l.split()
	wordcount += len(w)	

wordcount = 0
f = open(sys.argv[1])
flines = f.readlines()
linecount = len(flines)
for l in flines:
	checkline()
print linecount, wordcount
