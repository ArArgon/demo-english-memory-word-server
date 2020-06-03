import urllib.parse
import sys
s=urllib.parse.unquote(sys.argv[1])
print(s, end="")
