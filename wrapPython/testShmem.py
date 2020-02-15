#wrap a so file and call some method
import sys, platform
import ctypes, ctypes.util
import time
from ctypes import *
 
mylib_path = ctypes.util.find_library("./lib/libextend")
if not mylib_path:
    print("Unable to find the specified library.")
    sys.exit()

mylib = ctypes.CDLL(mylib_path)

charptr = POINTER(c_char)
mylib.readSharedMem.restype = charptr

shId = mylib.createSharedMem("hello.txt", 1024)
buf = mylib.readSharedMem(shId, 1024)

print(cast(buf, c_char_p).value)
