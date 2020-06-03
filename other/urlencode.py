import urllib.parse
import sys
s=urllib.parse.quote(sys.argv[1])
print(s, end="")
