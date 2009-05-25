from H3DInterface import *

def info():
  info, = references.getValue()
  for n in info.getFieldList():
    f = info.getField( n ) 
    print n + ": " + `f.getValue()`
    
def initialize():
  info()
