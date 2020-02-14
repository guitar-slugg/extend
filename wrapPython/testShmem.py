#wrap a so file and call some method
import sys, platform
import ctypes, ctypes.util
import time
 
mylib_path = ctypes.util.find_library("./lib/libextend")
if not mylib_path:
    print("Unable to find the specified library.")
    sys.exit()

mylib = ctypes.CDLL(mylib_path)

mylib.readShMem()

