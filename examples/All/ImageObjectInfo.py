from H3DInterface import *

def info():
  for i, info in enumerate( references.getValue() ):
    for n in info.getFieldList():
      f = info.getField( n ) 
      print n + ": " + `f.getValue()`
    print "\n"
    
def initialize():
  info()
