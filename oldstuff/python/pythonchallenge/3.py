import re

text = open("equality.txt",'r').read()
regex = '[a-z][A-Z][A-Z][A-Z]([a-z])[A-Z][A-Z][A-Z][a-z]'
p = re.compile(regex)
ans = p.findall(text)
print ''.join(ans)

