class textfile:
	ntfiles = 0 # count of number of textfile objects
	def __init__(self,fname):
		textfile.ntfiles += 1
		self.name = fname	#name
		self.fh = open(fname)	#handle for the file
		self.lines = self.fh.readlines()
		self.nlines = len(self.lines)	#number of lines
		self.nwords = 0	#number of words
		self.wordcount()
	def wordcount(self):
		"finds the number of words in the file"
		for l in self.lines:
			w = l.split()
			self.nwords += len(w)
	def grep(self,target):
		"prints out all lines containing target"
		for l in self.lines:
			if l.find(target) >= 0:
				print l

a = textfile('x')
b = textfile('y')
print "the number of text files open is", textfile.ntfiles
print "here is some information about them (name, lines, words):"
for f in [a,b]:
	print f.name,f.nlines,f.nwords
	a.grep('example')
	