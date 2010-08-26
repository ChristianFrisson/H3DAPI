#
# H3D Terrain demo
# Copyright 2005 SenseGraphics AB
#

# import the H3D libraries
from H3D import *
from H3DInterface import *

# import python libraries
import math
import random


# Seed the Python random number generator
random.seed()

# Shut off followViewpoint for devices so trackerposition does not
# give wrong values when viewpoint changes.
di = getActiveDeviceInfo()
if( di ):
  devices = di.device.getValue()
  for d in devices:
    d.followViewpoint.setValue( 0 )

# Terrain clas
# A simple implementation of the diamond-square fractal algorithm for
# random terrain generation.
#   depth - level of detail of terrain, where grid size is 2^depth+1
#   roughness - a roughness constant for how rough the terrain should be
#   waterlevel - the height level to render water at
#   sizex - the x size of the geometry
#   sizey - the z size of the geometry
#   scale - a scaling factor for the maximum y height of the geometry
class Terrain:
  def __init__( self, depth=6, roughness=0.5, waterlevel=0.0,
                sizex=10.0, sizey=10.0, scale=1.0 ):
    # initialise arguments to constructor
    self.depth = int(depth)
    self.roughness = float(roughness)
    self.waterlevel = float(waterlevel)
    self.sizex=float(sizex)
    self.sizey=float(sizey)
    self.scale=float(scale)

    # grid size, 2^depth+1
    self.size = (1 << self.depth)+1

    # initialise max (the maximum height) to be at the waterlevel
    self.max = float(waterlevel)
    self.BuildTerrain()
    self.DeformTerrain()
    self.Flood( )


  # BuildTerrain()
  # Allocates python array data and seeds an initial random altitude for the
  # four corners of the dataset.
  def BuildTerrain( self ):
    self.heightmap=[]
    for i in range(self.size):
      self.heightmap.append( [] )
      for j in range(self.size):
        self.heightmap[i].append( 0 )
    self.heightmap[0][0]                     = 0 #self.scale*(random.random()-0.5)
    self.heightmap[0][self.size-1]           = self.scale*(random.random()-0.5)
    self.heightmap[self.size-1][0]           = self.scale*(random.random())
    self.heightmap[self.size-1][self.size-1] = self.scale*(random.random()-0.5)

  # diamond()
  # Internal function used by DeformTerrain() for performing the 
  # diamond-square algorithm.
  def diamond( self, c, x, y, step ):
    if step > 1:
      half=step/2
      avg = ( self.heightmap[x][y] +
              self.heightmap[x][y+step] +
              self.heightmap[x+step][y] +
              self.heightmap[x+step][y+step] ) / 4
      h = avg + self.scale * c * (random.random()-0.5)
      self.heightmap[x+half][y+half] = h
      if h > self.max:
        self.max = h

  # square()
  # Internal function used by DeformTerrain() for performing the 
  # diamond-square algorithm.
  def square( self, c, x, y, step ):
    num=0
    avg=0.0
    half=step/2
    if ( x >= 0 ):
      avg=avg+self.heightmap[x][y+half]
      num=num+1
    if ( y >= 0 ):
      avg=avg+self.heightmap[x+half][y]
      num=num+1
    if ( x+step < self.size ):
      avg=avg+self.heightmap[x+step][y+half]
      num=num+1
    if ( y+step < self.size ):
      avg=avg+self.heightmap[x+half][y+step]
      num=num+1
    
    h = avg/num + self.scale * c * (random.random()-0.5) 
    self.heightmap[x+half][y+half] = h
    if h > self.max:
      self.max = h

  # DeformTerrain()
  # Itterative terrain deformer using classic diamond-square algorithm.
  # For more information see http://www.gameprogrammer.com/fractal.html
  def DeformTerrain( self ):
    rough = self.roughness
    r = self.size-1

    # Diamond-square iterative implementation:
    for i in range(self.depth):
      s = r >> 1
      x = 0
      while ( x < self.size-1 ):
        y = 0
        while ( y < self.size-1 ):
          self.diamond( rough, x, y, r )
          y = y + r
        x = x + r

      if s>0:
        x = 0
        while ( x < self.size ):
          y = (x + s) % r
          while ( y < self.size ):
            self.square( rough, x-s, y-s, r )
            y = y + r
          x = x + s
      rough = rough * self.roughness #0.5
      r = r >> 1

  # Flood():
  # Fill the terrain with water at the given waterlevel.
  def Flood( self ):
    for i in range(self.size):
      for j in range(self.size):
        if self.heightmap[i][j] < self.waterlevel:
          self.heightmap[i][j] = self.waterlevel

  # GenerateCoords():
  # Generates a list of vertices for an IndexedTriangleSet.
  def GenerateCoords( self ):
    points=[]
    stepx = self.sizex/self.size
    stepy = self.sizey/self.size
    for i in range(self.size):
      for j in range(self.size):
        points.append( Vec3f( (i-self.size/2)*stepx,
                              self.heightmap[i][j],
                              (j-self.size/2)*stepy ) )
    return points

  # GenerateIndex():
  # Generates an index list for an IndexedTriangleSet.
  def GenerateIndex( self ):
    indices=[]
    for i in range( self.size-1 ):
      for j in range( self.size-1 ):
        indices.append( i*self.size + j )
        indices.append( (i+1)*self.size + j )
        indices.append( i*self.size + j + 1 )
        indices.append( i*self.size + j + 1 )
        indices.append( (i+1)*self.size + j )
        indices.append( (i+1)*self.size + j + 1 )
    return indices

  # GenerateColors():
  # Generate vertex colours based on their height to give a simple 
  # but realistic Earth-based terrain appearance.
  def GenerateColors( self ):
    colors=[]
    for i in range(self.size):
      for j in range(self.size):
        height = self.heightmap[i][j]
        if height <= self.waterlevel:
          # water
          col = RGB(0,0.2,0.4)
        elif height <= 0.05*self.max:
          # beach
          col = RGB(0.8,0.8,0.3)
        elif height <= 0.7*self.max:
          # grass
          col = RGB(0.05,0.25+0.5*height/self.max,0.05)
        elif height <= 0.85*self.max:
          # mountain
          col = RGB(0.8,0.9,0.6)
        else:
          # snowy peaks
          col = RGB(1,1,1)
        colors.append( col )
    return colors

  # GetHeight():
  # get an interpolated height at a given x/y point in global coordinates
  # used by the navigator node to maintain camera position a fixed height 
  # above the ground
  def GetHeight( self, x, y ):
    width=self.sizex
    height=self.sizey
    sz = 1 << self.depth

    # bilinear interpolation
    posx = sz * ((x+width/2)/width)
    posy = sz * ((y+height/2)/height)
    intx = int( posx )
    inty = int( posy )
    if intx < 0 or inty < 0 or intx>=sz-1 or inty>=sz-1:
      return self.max

    diffx = posx - float( intx )
    diffy = posy - float( inty )
    height =  ( (1-diffx)*(1-diffy)*self.heightmap[intx  ][inty  ] +  
                (1-diffx)*(  diffy)*self.heightmap[intx  ][inty+1] +
                (  diffx)*(1-diffy)*self.heightmap[intx+1][inty  ] + 
                (  diffx)*(  diffy)*self.heightmap[intx+1][inty+1] )

    return height



# get a reference to the IndexedTriangleSet node
shape = references.getValue()[0]
pod = references.getValue()[1]
its = shape.geometry.getValue()

# create an instance of the terrain generator class
terrain = Terrain( depth=8, roughness=0.55, waterlevel=0.0,
                   sizex=50.0, sizey=50.0, scale=8 )

# generate and set the ITS coordinates
coord = its.coord.getValue()
coord.point.setValue( terrain.GenerateCoords() )

# generate ITS indices
its.index.setValue( terrain.GenerateIndex() )

# set ITS terrain colours
color = its.color.getValue()
color.color.setValue( terrain.GenerateColors( ) )



## Navigation 

# create and initialise two fields for controlling the camera
# these fields are routed in the X3D file.
podvel = 0
podangvel = 0
podalt = 0.15
podPos = SFVec3f()
podOrientation = SFRotation()
podOrientation.setValue( Rotation( 0, 1, 0, 0 ) )
podpos = podPos.getValue()
podorn = podOrientation.getValue()
vpPosition=SFVec3f()
vpOrientation=SFRotation()
vpPosition.setValue( Vec3f( 0, 0.2, 0 ) )
#vpOrientation.setValue( Rotation( 1, 0, 0, -0.250 ) )
vpOrientation.setValue( Rotation( 0, 1, 0, 0 ) )

# Navigation key manager - move the camera using the arrow keys.
# Left / right will turn the camera, up / down will move the
# camera forwards or backwards respectively.
class KeyManager( AutoUpdate( SFInt32 ) ):
  def update( self, event ):
    global podvel, podangvel, podalt
    key = event.getValue()
    if key == 19: #left
      podangvel = podangvel + 0.001
      #pos.x = pos.x - 0.01
    elif key == 20: #right
      podangvel = podangvel - 0.001
      #pos.x = pos.x + 0.01
    elif key == 17: #up
      podvel = podvel + 0.01
    elif key == 18: #down
      # breaking only
      podvel = podvel - 0.01
      if podvel < 0:
        podvel = 0
    elif key == 1:
      podalt = podalt + 0.033
    elif key == 2:
      podalt = podalt - 0.033
      if podalt < 0.5:
        podalt = 0.5

    # H3D requires a return value if the Field type is an SF or MF container
    return 0

# create an instance ofthe key manager to be routed to from the X3D file
keyManager = KeyManager()


podOrigin = SFVec3f()
# Phantom navigation
class ProxyNavigator( AutoUpdate( SFVec3f ) ):
  def update( self, event ):
    global podvel, podangvel, podalt, origin, podOrigin
    pos = event.getValue()
    try:
      diff = origin - pos
    except:
      origin = pos
      podOrigin.setValue( origin )
      diff = origin - pos

    podangvel = 0.2*diff.x
    podvel = diff.z
    if podvel < 0:
      podvel = 0
    podalt = -diff.y + 0.25

    # H3D requires a return value if the Field type is an SF or MF container
    return Vec3f(0, 0, 0)

proxyNavigator = ProxyNavigator()


class PodUpdater( AutoUpdate( SFTime ) ):
  def update( self, event ):
    global podorn, podpos, terrain, podvel, podangvel, podalt

    podorn.a = podorn.a + podangvel

    # damping
    podangvel = 0.99*podangvel
    podvel = 0.99*podvel

    mx = Matrix3f( podorn )
    podpos = podpos + mx * Vec3f( 0, 0, -podvel )
    
    # determine height from heightmap
    newalt = terrain.GetHeight( podpos.x, podpos.z ) + podalt
    podpos.y = 0.2*newalt + 0.8*podpos.y
    deltay = newalt-podpos.y

    # set camera just behind the pod
    camerapos = vpPosition.getValue()
    newcpos = podpos + mx*Vec3f(0, 0, 0.05) + Vec3f(0, 0.01, 0 )
    vpPosition.setValue( 0.75*newcpos + 0.25*camerapos )
    cameraorn = vpOrientation.getValue()
    newcorn = podorn * Rotation( 0, 0, 1, 25*podangvel )
    vpOrientation.setValue( Rotation( 0.75*Matrix3f(newcorn) + 0.25*Matrix3f(cameraorn) ) )


    # set pod position / orientation
    podPos.setValue( podpos )
    podOrientation.setValue( podorn * 
                             Rotation( 0, 0, 1, 50*podangvel ) * 
                             Rotation( 1, 0, 0, deltay ) )


    return 0

podUpdater = PodUpdater()
time.route( podUpdater )






