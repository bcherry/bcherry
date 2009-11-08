text = open('ocr.txt','r').read()
ans = ""
for c in text:
	if c.isalpha():
		ans = ans + c
		
print ans
