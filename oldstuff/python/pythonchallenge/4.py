import urllib
import time
nothing = '46059'
for i in range(300):
    url = "http://www.pythonchallenge.com/pc/def/linkedlist.php?nothing=%s" % nothing
    print url
    page = urllib.urlopen(url).read()
    nothing = page.split()[-1]
    if page.split()[:-1] != "and the next nothing is".split():
        # akward sentence
        print i, page
    time.sleep(.5)