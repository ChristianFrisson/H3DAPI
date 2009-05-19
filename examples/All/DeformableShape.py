from H3DInterface import *

di = getActiveDeviceInfo()
if( di ):
  devices = di.device.getValue()
  for d in devices:
    d.proxyWeighting.setValue( 0 )