from H3DInterface import *
import random
from helper import *
import navigation

MAIN_GROUP, MAIN_TRANSFORM, ROTATION_TRANSFORM, KS, TS, DIRT_G, DI, BALLS_G, STATUS_S = references.getValue()
device = DI.device.getValue()[0]
objs = {}

mousemanager = navigation.MouseManager(MAIN_TRANSFORM, ROTATION_TRANSFORM)

class WALL:
  x_min, x_max = -0.15, 0.15
  y_min, y_max = -0.15, 0.15
  z_min, z_max = -0.15, 0.15

class Fish:
  BALL_RADIUS = 0.01
  HEAD_DIRECTION = Vec3f(0, 0, -1)

  def __init__(self, game):
    self.game = game  # circular reference
    
    self.position = SFVec3f()

    self.fish_x3d, dn = createX3DNodeFromURL("fish.x3d")
    self.TRANSFORM = dn['FISH_T']
    MAIN_GROUP.children.push_back(self.fish_x3d)
    self.position.route(self.TRANSFORM.translation)

  def moveOn(self):
    " move Fish to next position "

    delta = device.trackerPosition.getValue()
    pos = self.position.getValue() + delta / 3

    delta.normalizeSafe()
    self.TRANSFORM.rotation.setValue( Rotation( self.HEAD_DIRECTION, delta ) )

    # boundary check
    if pos.x < WALL.x_min: pos.x = WALL.x_min
    elif pos.x > WALL.x_max: pos.x = WALL.x_max
    if pos.y < WALL.y_min: pos.y = WALL.y_min
    elif pos.y > WALL.y_max: pos.y = WALL.y_max
    if pos.z < WALL.z_min: pos.z = WALL.z_min
    elif pos.z > WALL.z_max: pos.z = WALL.z_max    

    self.position.setValue( pos )

  def dirtEatable(self, dirt):
    if not dirt.dead:
      if (self.position.getValue() - dirt.position.getValue()).length() <= 0.05:
        return True
    return False

class Dirt:
  " Class to represent a dirt"

  DIRT_RADIUS = 0.005

  def __init__(self, pos=None):
    self.dead = 0 # dirt reach bottom. cannot eat anymore.
    self.position = SFVec3f(Vec3f())
    if pos is not None: self.position.setValue(pos)

    self.transform, dn = createX3DNodeFromString("""\
      <Transform>
        <Shape>
          <Sphere radius='%s' />
          <Appearance shadow='true'><Material diffuseColor='0 0 1' /></Appearance>
        </Shape>
      </Transform>""" % Dirt.DIRT_RADIUS)
    self.material = self.transform.children.getValue()[0].appearance.getValue().material.getValue()
    self.position.routeNoEvent(self.transform.translation)
    DIRT_G.children.push_back(self.transform)

  def moveOn(self):
    v = self.position.getValue()
    if v.y <= -0.15:
      self.dead = 1
      self.material.diffuseColor.setValue(RGB(0.384, 0.153, 0.153))
      return
    v.y -= 0.001 #TODO replace with level's value
    self.position.setValue(v)

  def __del__(self):
    DIRT_G.removeChildren.push_back(self.transform)
    DIRT_G.removeChildren.pop_back()

########################################
# 
########################################
class Strings2Status( AutoUpdate( TypedField( MFString, (SFInt32) ) ) ):
  # (level, time, lives)
  def update(self, event):
    ins = getRoutesIn(self)
    if len(ins) < 1: return []
    ans = []
    ans.append( "Eats:  " + str(ins[0].getValue()) )
    return ans    

class Game:

  def __init__(self):
  
    self.level = SFInt32(0)
    self.lives = SFInt32(20)
    self.dirts_caught = SFInt32(0)

    self.stats2board = Strings2Status()
    self.dirts_caught.route(self.stats2board)
    self.stats2board.route(STATUS_S.string)

    self.started, self.paused = 0, 1
    self.timer_cb = CallbackEventField( self.onTimeCycle, () )
    TS.cycleTime.routeNoEvent(self.timer_cb)

    # list of dirts
    self.dirts = list()
    self.fish = Fish(self)


  def pause(self):
    " pause/resume game "
    if self.started:
      self.paused = 1 - self.paused
      TS.enabled.setValue(not self.paused)

  def start(self):
    self.started = 1
    self.paused = 0

  def gameover(self):
    self.started = 0
    self.paused = 1

  def onTimeCycle(self):
    if not self.started: return False
    if self.paused: return False

    # Move fish to next position
    self.fish.moveOn()

    # check if Fish hits dirt
    dirts_to_remove = []
    for dirt in self.dirts:
      if self.fish.dirtEatable(dirt):
        self.dirts_caught.setValue( self.dirts_caught.getValue() + 1 )
        dirts_to_remove.append(dirt)  # remove dirt if eaten
    for dirt in dirts_to_remove:
      self.dirts.remove(dirt)

    # if generating more dirt?
    self.checkAndGenDirt()
    
    # move dirts
    for dirt in self.dirts:
      dirt.moveOn()

    return True

  def checkAndGenDirt(self):
    # generate dirt if not already
    if random.randint(0, 20) == 1:
      dirt = Dirt()
      dirt.position.setValue( Vec3f( random.uniform(WALL.x_min + Dirt.DIRT_RADIUS, WALL.x_max - Dirt.DIRT_RADIUS),
                                       WALL.y_max,
                                       random.uniform(WALL.z_min + Dirt.DIRT_RADIUS, WALL.z_max - Dirt.DIRT_RADIUS)) )
      self.dirts.append(dirt)

#######################################
# GAME
#######################################
game = Game()

#######################################
# KEY HANDLING
#######################################
def handleKeyPress():
  global KS, game
  key = KS.keyPress.getValue()
  if key == ' ':
    if not game.started:
      game.start()
    else:
      game.pause()

keypress_cb = CallbackEventField( handleKeyPress, () )
KS.keyPress.route(keypress_cb)




