#
#  Space Tennis
#  Implementation (c) SenseGraphics AB
#  Erik Lovquist
#
# A tennis game taking place in space. The player controls a racket with the 
# stylus and uses it to bounce a ball in direction of the opponent. There are
# three different game modes: 
#  1. Bounce with the opponent, every bounce give a point. 
#  2. Hit the ball beside the computer controlled opponent to score points. 
#  3. The opponent acts as a target, points are achieved by hitting the target.

from H3DInterface import *
import H3DUtils
import random
import math
import sys
import os.path
import thread

bonustime=15

path = ""

# get the first haptics device and put it into the hdev variable.
hdev = getHapticsDevice(0)
if hdev:
  hdev.stylus.setValue( None )

# TimerCallback used for delayed events.
timer_callback = H3DUtils.TimerCallback()

# Class that is used to show the message HIT! or GOAL! for a while
# when a goal occurs
class TargetScore:
  def __init__( self ):
    self.node, self.dn = createX3DNodeFromURL( path + "score.x3d" )
    
  def showScore( self, pos, score ):
    self.dn["TRANS"].translation.setValue( Vec3f(0, 0, -0.1) )
    self.dn["TIME"].startTime.setValue( time.getValue() )
    if score < 0:
      self.dn["TEXT"].string.setValue( [ "HIT!"] )
    else:
      self.dn["TEXT"].string.setValue( [ "GOAL!"] )
      
         
# to see if the main button on the haptic device is pressed, if so, 
# serve the ball   
class ButtonPressed( AutoUpdate( SFBool ) ):
  def __init__( self, parent ):
    AutoUpdate( SFBool ).__init__( self )
    self.game = parent

  def update( self, event ):
    b = event.getValue()
    if b==1:
      if not self.game.started and self.game.lock:
        self.game.started = 1
        self.game.dn["TX_BALL"].momentum.setValue( self.game.initial_momentum )
      return 1
    else:
      return 0

# The main class for the game.
class SpaceTennisGame:
  def __init__( self, parent, resistance, initial_momentum,
                game_style, mom_effect ):

    self.parent = parent
    self.initial_momentum = initial_momentum
    self.resistance = resistance
    self.game_style = game_style
    self.mom_effect = mom_effect

    self.button_pressed = ButtonPressed( self )
    if hdev:
      hdev.mainButton.route( self.button_pressed )

    self.node, self.dn = createX3DNodeFromURL( path + "enclosure.x3d" )
    if not hdev:
      self.dn["INSTRUCTIONS_SWITCH"].whichChoice.setValue( 1 )
    self.parent = parent
    self.game_running = 0
    self.started = 0
    
    # make the node visible by adding it to the parent
    self.parent.children.push_back( self.node )

    # parameters for the game play
    self.total_score = 0
    self.amountLevels = 4
    self.radius = 0.03
    self.momIncrease = 0.1
    self.momIndex = 0.1
    self.lock = 1
    self.oldTime = 0
    self.time = 0

    self.dt = 0.00001
    self.oppIndex = 0.00001

    if self.game_style:
      self.opponentSpeed = 0.9
    elif self.initial_momentum.z == 1.2:
      self.opponentSpeed = 0.07 
    elif self.initial_momentum.z == 0.6:
      self.opponentSpeed = 0.05 
    else:
      self.opponentSpeed = 0.025 
      
    if self.game_style == 2:
      self.dn["TEXT_RACKET"].string.setValue( [" TARGET " ] )

    self.target_score = TargetScore()
    self.node.children.push_back( self.target_score.node )
   
    self.levelIndex = 0
    
  def setup( self ):
    bonuses = 2
    self.time = time.getValue()
    self.dn["TRANS_REF4"].translation.setValue( Vec3f(0, -0.2, -0.3 )) 

    
  def updateScore( self, points ):
    self.total_score += points
    self.dn["SCORE_TEXT"].string.setValue( [str( self.total_score ) ] )

  def update( self, s=None ):

    #tweak the speed of the ball depending of the speed of computer    
    self.time = time.getValue()
    dt = (self.time - self.oldTime)*75 
    self.oldTime = self.time

    pos = self.dn["TX_BALL"].position.getValue()
    rad = self.dn["BALL"].radius.getValue()
    mom=self.dn["TX_BALL"].momentum.getValue(  )
    opponentPos = self.dn["R_T2"].position.getValue()
    playerPos = self.dn["R_T"].position.getValue()

    try:
      #the racket follows the stylus
      self.dn["R_T"].position.setValue( hdev.trackerPosition.getValue() )

      if not self.game_running:
        return
      
      if not self.started:
        self.dn["TX_BALL"].momentum.setValue( Vec3f( 0, 0, 0 ) )
        trackPos = hdev.trackerPosition.getValue()
        self.dn["R_T"].position.setValue( Vec3f( trackPos.x, trackPos.y, 0.05 ) )
        self.dn["TX_BALL"].angularMomentum.setValue( Vec3f( 0, 0, 0 ))
        self.dn["GRAVITY"].force.setValue( Vec3f(0,0,0))
      else:
        self.dn["INSTRUCTIONS_TOGGLE"].graphicsOn.setValue( False )
        trackPos = hdev.trackerPosition.getValue()
        self.dn["R_T"].position.setValue( Vec3f( trackPos.x, trackPos.y, 0.05 ) )
        pos = self.dn["TX_BALL"].position.getValue()
        #Resistance applied to the racket
        self.dn["GRAVITY"].force.setValue( Vec3f(-trackPos.x*self.resistance,-trackPos.y*self.resistance,0))
        
        #move the opponent, depending on gamestyle, AI of the racket
        if self.game_style == 2:
          self.dn["R_T2"].position.setValue( Vec3f(math.sin(self.oppIndex * 2 * math.pi) * 0.1 , 0,0))
          self.oppIndex -= 0.001
        elif mom.z < 0:
          rackPos = self.dn["R_T2"].position.getValue()
          ballPos = pos
          ballDiff = ballPos  - rackPos
          xPos = rackPos.x + ballDiff.x*self.opponentSpeed*0.5 
          yPos = rackPos.y + ballDiff.y*self.opponentSpeed
          
          #make sure the opponent doesnt move outside of the play area
          if xPos < -0.175:
            xPos = -0.175
          elif xPos > 0.175:
            xPos = 0.175
          if yPos < -0.125:
            yPos = -0.125
          elif yPos > 0.125:
            yPos = 0.125
          
          self.dn["R_T2"].position.setValue( Vec3f(xPos, yPos ,0 ))         
        else:
          rackPos = self.dn["R_T2"].position.getValue() 
          ballPos = pos
          ballDiff = ballPos - rackPos
          xPos = rackPos.x + ballDiff.x*self.opponentSpeed/3*0.5 
          yPos = rackPos.y + ballDiff.y*self.opponentSpeed/3
          
          #make sure the opponent doesnt move outside of the play area
          if xPos< -0.175:
            xPos = -0.175
          elif xPos > 0.175:
            xPos = 0.175
          if yPos < -0.125:
            yPos = -0.125
          elif yPos > 0.125:
            yPos = 0.125
        
          self.dn["R_T2"].position.setValue( Vec3f(xPos, yPos, 0 ))        
        
        opponentPos = self.dn["R_T2"].position.getValue()

        #move the references
        self.dn["TRANS_REF3"].translation.setValue( Vec3f(opponentPos.x, -0.2, -0.7 ))
        self.dn["TRANS_REF4"].translation.setValue( Vec3f(pos.x, -0.2, pos.z ))

        # if the ball has moved passed a wall, flip the momentum in order for it
        # to bounce, and play the bounce sound.
        if ( ( pos.x+rad >  0.25 ) and mom.x > 0):
          # right wall
          self.dn["TX_BALL"].momentum.setValue( Vec3f(-mom.x, mom.y, mom.z ) )
          self.dn["WALL"].startTime.setValue( time.getValue() )
          self.dn["SOUND3"].location.setValue( pos )
        if ( ( pos.x-rad < -0.25 ) and mom.x < 0):
          # left wall
          self.dn["TX_BALL"].momentum.setValue( Vec3f(-mom.x, mom.y, mom.z ) )
          self.dn["WALL"].startTime.setValue( time.getValue() )
          self.dn["SOUND3"].location.setValue( pos )      
        if ( ( pos.y+rad >  0.2 ) and mom.y > 0):
          # top wall
          self.dn["TX_BALL"].momentum.setValue( Vec3f(mom.x, -mom.y, mom.z ) )
          self.dn["WALL"].startTime.setValue( time.getValue() )
          self.dn["SOUND3"].location.setValue( pos )
        if ( ( pos.y-rad < -0.2 ) and mom.y < 0):
          # bottom wall
          self.dn["TX_BALL"].momentum.setValue( Vec3f(mom.x, -mom.y, mom.z ) )
          self.dn["WALL"].startTime.setValue( time.getValue() )
          self.dn["SOUND3"].location.setValue( pos )
        if ( ( pos.z-rad < -0.7 ) and (mom.z < 0 ) ):
          # opponent side wall
          self.dn["TX_BALL"].momentum.setValue( Vec3f(mom.x, mom.y, -mom.z ) )

          # wall was hit, check if goal or opponent hit
          opponentDiff = opponentPos - pos
          if( (opponentDiff.x < -0.065) or (opponentDiff.x > 0.065) or
              (opponentDiff.y < -0.065) or (opponentDiff.y > 0.065) ):
            # opponent wall was hit
            if self.game_style == 0:
              self.target_score.showScore( Vec3f(0, 0, 0.05), 100 )
              self.momIncrease = 0.01
              self.updateScore(1)
              self.dn["CROWD"].startTime.setValue( time.getValue() )
            else:
              self.dn["WALL"].startTime.setValue( time.getValue() )
              self.dn["SOUND3"].location.setValue( pos )        
          else:
            self.dn["BALL1"].startTime.setValue( time.getValue() )
            self.dn["SOUND"].location.setValue( opponentPos )
            self.dn["SOUND"].intensity.setValue( 0.5 )
            if self.game_style == 1:
              self.updateScore(1)
            elif self.game_style == 2:
              self.updateScore(100)
              self.dn["CROWD"].startTime.setValue( time.getValue() )
              self.target_score.showScore( Vec3f(0, 0, 0.05), -1 )          


        #calculate the normal of the plane and the distance between normal and the ball
        trackPos = hdev.trackerPosition.getValue() 
        norm=Matrix3f(hdev.trackerOrientation.getValue())* Vec3f(0,0,1)
        posDiff = trackPos - pos
        distance =(norm) * (posDiff)

        #to see if ball has passed players racket
        if pos.z > 0.29:
          self.restart()
                    
        #to see if ball has passed opponents racket
        playerDiff = pos.z - playerPos.z 

        #handles collision between racket and ball
        if((playerDiff-rad < 0) and (playerDiff-rad*1.5 > -0.1) and 
           (posDiff.x < 0.08) and (posDiff.x > -0.08) and
           (posDiff.y < 0.06) and (posDiff.y > -0.06)):
          #play sound
          self.dn["BALL1"].startTime.setValue( time.getValue() )
          self.dn["SOUND"].location.setValue( playerPos )
          self.dn["SOUND"].intensity.setValue( 1 )
          #haptic feedback when hitting the ball
          self.momIndex += 0.00005
          self.dn["FORCE"].force.setValue( Vec3f(0,0,1))
          timer_callback.addCallback( time.getValue() + 0.25, self.neutralizeForce, () )
          self.dn["R_T"].position.setValue( Vec3f(playerPos.x, playerPos.y, playerPos.z + 0.03))
          #self.bluestar.dn["TX"].momentum.setValue( Vec3f(mom.x, mom.y, -mom.z ) )
          posDiff = posDiff*-1
          #how to move the ball when it hits players racket
          # Hit areas:  1  2  3
          #                   4  5  6
          #                   7  8  9 
          #upper left corner: 1
          if (posDiff.x > -0.08) and (posDiff.x < -0.02) and (posDiff.y < 0.06) and (posDiff.y > 0.01):
            #moves the ball on impact
            self.dn["TX_BALL"].position.setValue( Vec3f(pos.x, pos.y, pos.z - 0.01 ) )
            self.dn["TX_BALL"].momentum.setValue( Vec3f(-self.mom_effect*random.random(), 
                                                        self.mom_effect*random.random(), 
                                                        -mom.z ) )
            #self.momX = 1
            self.dn["TX_BALL"].angularMomentum.setValue( Vec3f(playerPos.x + self.momIndex*75, 
                                                               playerPos.y - self.momIndex*75,
                                                               playerPos.z ))
          #upper middle: 2  
          elif (posDiff.x > -0.02) and (posDiff.x < 0.02) and (posDiff.y < 0.06) and (posDiff.y > 0.01):
            self.dn["TX_BALL"].momentum.setValue( Vec3f(mom.x, self.mom_effect*random.random(), -mom.z ) )
            self.dn["TX_BALL"].position.setValue( Vec3f(pos.x, pos.y, pos.z - 0.01 ) )
            self.dn["TX_BALL"].angularMomentum.setValue( Vec3f(playerPos.x, playerPos.y - self.momIndex*75, playerPos.z ))
          #upper right corner: 3
          elif (posDiff.x > 0.02) and (posDiff.x < 0.08) and (posDiff.y < 0.06) and (posDiff.y > 0.01):
            self.dn["TX_BALL"].momentum.setValue( Vec3f(self.mom_effect*random.random(), self.mom_effect*random.random(), -mom.z ) )
            self.dn["TX_BALL"].position.setValue( Vec3f(pos.x, pos.y, pos.z - 0.01 ) )
            self.dn["TX_BALL"].angularMomentum.setValue( Vec3f(playerPos.x - self.momIndex*75, playerPos.y - self.momIndex*75, playerPos.z ))
          #middle left: 4
          elif (posDiff.x > -0.08) and (posDiff.x < -0.02) and (posDiff.y < 0.01) and (posDiff.y > -0.01):
            self.dn["TX_BALL"].momentum.setValue( Vec3f(-self.mom_effect*random.random(), mom.y, -mom.z ) )
            self.dn["TX_BALL"].position.setValue( Vec3f(pos.x, pos.y, pos.z - 0.01 ) )
            self.dn["TX_BALL"].angularMomentum.setValue( Vec3f(playerPos.x + self.momIndex*75, playerPos.y, playerPos.z ))
          #middle : 5
          elif (posDiff.x > -0.02) and (posDiff.x < 0.02) and (posDiff.y < 0.01) and (posDiff.y > -0.01):
            self.dn["TX_BALL"].momentum.setValue( Vec3f(mom.x*0.5+random.random()*0.1, mom.y*0.5+random.random()*0.1, -mom.z ) )
            self.dn["TX_BALL"].position.setValue( Vec3f(pos.x, pos.y, pos.z - 0.01 ) )
            self.dn["TX_BALL"].angularMomentum.setValue( Vec3f(playerPos.x - self.momIndex*75, playerPos.y, playerPos.z ))                    
          #middle right: 6
          elif (posDiff.x > 0.02) and (posDiff.x < 0.08) and (posDiff.y < 0.01) and (posDiff.y > -0.01):
            self.dn["TX_BALL"].momentum.setValue( Vec3f(self.mom_effect*random.random(), mom.y, -mom.z ) )
            self.dn["TX_BALL"].position.setValue( Vec3f(pos.x, pos.y, pos.z - 0.01 ) )
            self.dn["TX_BALL"].angularMomentum.setValue( Vec3f(playerPos.x - self.momIndex*75, playerPos.y, playerPos.z ))
          #lower left: 7
          elif (posDiff.x > -0.08) and (posDiff.x < -0.02) and (posDiff.y < -0.01) and (posDiff.y > -0.06):
            self.dn["TX_BALL"].momentum.setValue( Vec3f(-self.mom_effect*random.random(), -self.mom_effect*random.random(), -mom.z ) )
            self.dn["TX_BALL"].position.setValue( Vec3f(pos.x, pos.y, pos.z - 0.01 ) )
            self.dn["TX_BALL"].angularMomentum.setValue( Vec3f(playerPos.x + self.momIndex*75, playerPos.x + self.momIndex*75, playerPos.z ))           
          #lower middle: 8
          elif (posDiff.x > -0.02) and (posDiff.x < 0.02) and (posDiff.y < -0.01) and (posDiff.y > -0.06):
            self.dn["TX_BALL"].momentum.setValue( Vec3f(mom.x, -self.mom_effect*random.random(), -mom.z ) )
            self.dn["TX_BALL"].position.setValue( Vec3f(pos.x, pos.y, pos.z - 0.01 ) )
            self.dn["TX_BALL"].angularMomentum.setValue( Vec3f(playerPos.x, playerPos.x + self.momIndex*75, playerPos.z ))           
          #lower middle: 9
          elif (posDiff.x > 0.02) and (posDiff.x < 0.08) and (posDiff.y < -0.01) and (posDiff.y > -0.06):
            self.dn["TX_BALL"].momentum.setValue( Vec3f(self.mom_effect*random.random(), -self.mom_effect*random.random(), -mom.z ) )
            self.dn["TX_BALL"].position.setValue( Vec3f(pos.x, pos.y, pos.z - 0.01 ) )
            self.dn["TX_BALL"].angularMomentum.setValue( Vec3f(playerPos.x - self.momIndex*75, playerPos.x + self.momIndex*75, playerPos.z ))
          self.momIncrease += 0.00001
    except Exception, e:
      pass

  # Sets the ball to the start position
  def restart( self ):
    self.dn["TX_BALL"].position.setValue( Vec3f(0, 0, -0.3 ))
    self.momIncrease = 0.1
    self.momIndex = 0.1    
    self.started=0
    self.lock = 0
    self.dn["FINALSCORE_TOGGLE"].graphicsOn.setValue( False )

    self.updateScore( 0 )
    
    if self.levelIndex < self.amountLevels - 1:
      if self.levelIndex == 0 :
        self.levelIndex += 1
        self.dn["LIFE1_TOGGLE"].graphicsOn.setValue( False )
      elif self.levelIndex == 1 :
        self.dn["LIFE2_TOGGLE"].graphicsOn.setValue( False )
        self.levelIndex += 1
      elif self.levelIndex == 2 :
        self.dn["LIFE3_TOGGLE"].graphicsOn.setValue( False )
        self.levelIndex += 1

      self.lock = 1
      self.setup()    
    else:
      # no more lives, show final score
      self.dn["LIFE1_TOGGLE"].graphicsOn.setValue( True )
      self.dn["LIFE2_TOGGLE"].graphicsOn.setValue( True )
      self.dn["LIFE3_TOGGLE"].graphicsOn.setValue( True )

      self.dn["FINALSCORE_TOGGLE"].graphicsOn.setValue( True )
      self.dn["TRANS_REF4"].translation.setValue( Vec3f(0, -0.2, -0.3 )) 
      self.dn["FINAL_TEXT"].string.setValue( ["Final Score: "+ str( self.total_score ) ] )
      timer_callback.addCallback( time.getValue() + 8, self.restart, () )
      self.total_score = 0
      self.levelIndex = 0
    
  # set the force to zero force
  def neutralizeForce( self ):
    playerPos = self.dn["R_T"].position.getValue()
    self.dn["FORCE"].force.setValue( Vec3f(0,0,0))
    self.dn["R_T"].position.setValue( Vec3f(playerPos.x, playerPos.y, playerPos.z + 0.03))

  # start game
  def restartGame( self ):    
    self.started=0
    self.total_score = 0
    self.updateScore(0)
    self.setup()
    self.game_running = 1

  # Call this function to clean up circular references
  def cleanUp( self ):
    self.parent = None
    self.dn = None
    self.target_score = None
    self.button_pressed = None


