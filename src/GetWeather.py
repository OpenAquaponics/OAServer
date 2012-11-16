#
#Have an admin packet that the operator can send to configure all the different set points (water temp alerts, high/low alerts, etc)
#

import re
import pycurl
import cStringIO

buf = cStringIO.StringIO()

c = pycurl.Curl()
c.setopt(c.URL, 'http://www.wunderground.com/cgi-bin/findweather/hdfForecast?query=35802')
c.setopt(c.WRITEFUNCTION, buf.write)
c.perform()

#str = '<div id="tempActual"><span id="rapidtemp" class="pwsrt" pwsid="KALHUNTS15" pwsunit="english" pwsvariable="tempf" english="&deg;F" metric="&deg;C" value="55.2">'

#infile = open('./website.txt', 'r')
#str = infile.read()
#infile.close()

str = buf.getvalue()
buf.close()

restr = re.compile('(?:<div id="tempActual">.*value=")(.*)(?:">)')
m = restr.findall(str)
print "%s" % (m[0])

#m = restr.search(str)
#print len(m.groups())
#for i in range(len(m.groups())):
#  print i, " - ", m.group(i)
#print i+1, " - ", m.group(i+1)



