# This file sets up a GUI for choosing settings for a SpeceTennis game
# using TKinter and then starts a SpaceTennis game.

from H3DInterface import *
import sys
import SpaceTennis
from Tkinter import *

g, = references.getValue()

# Settings GUI
#global variables for GUI
resistance = 5
initial_momentum = Vec3f( 0, 0, 0.3 )
closeUI=0
gameStyle=1
momEffect = 0.2  
dual = 0

class StartButton:
  def __init__( self, root ):
    self.f = Frame( root )
    self.f.pack( side=TOP, padx=5, pady=5 )
    self.sb = Button( self.f, text="Start!", command=self.__start )
    self.sb.pack( side=LEFT, padx=10 )

  def __start( self ):
    global closeUI
    closeUI=1
  def __quit( self ):
    global closeUI
    closeUI=2

class StyleButton:
  def __init__( self, root ):
    self.f = Frame( root )
    self.f.pack( side=TOP, padx=5, pady=5 )
    self.l = Label( self.f, text="Set game style:" )
    self.l.pack( side=LEFT )
    self.bb = Button( self.f, text="Bounce (Beginner)", command=self.__bounce )
    self.bb.pack( side=LEFT, padx=10 )
    self.sb = Button( self.f, text="Score", command=self.__score )
    self.sb.pack( side=LEFT, padx=10 )
    self.sb = Button( self.f, text="Precision", command=self.__target )
    self.sb.pack( side=LEFT, padx=10 )    
  def __bounce( self ):
    global gameStyle
    gameStyle=1
  def __score( self ):
    global gameStyle
    gameStyle=0
  def __target( self ):
    global gameStyle
    gameStyle=2     

class DiffButton:
  def __init__( self, root ):
    global momEffect
    self.f = Frame( root )
    self.f.pack( side=TOP, padx=5, pady=5 )
    self.l = Label( self.f, text="Set degree of ball movment:" )
    self.l.pack( side=LEFT )
    self.eb = Button( self.f, text="  +  ", command=self.__plus )
    self.eb.pack( side=LEFT, padx=5 )
    self.mb = Button( self.f, text="  -  ", command=self.__minus )
    self.mb.pack( side=LEFT, padx=5 )
    self.c = StringVar()
    self.l2 = Label( self.f, textvariable=self.c, text=str(momEffect) )
    self.l2.pack( side=LEFT )
  def __plus( self ):
    global momEffect
    if momEffect < 0.99:
      momEffect += 0.1
      self.c.set(str(momEffect))
  def __minus( self ):
    global momEffect
    if momEffect > 0.1:
      momEffect -= 0.1
      self.c.set(str(momEffect))
    
class SpeedButton:
  def __init__( self, root ):
    global initial_momentum
    self.f = Frame( root )
    self.f.pack( side=TOP, padx=5, pady=5 )
    self.c = StringVar()
    self.l = Label( self.f, text="Choose speed of ball:" )
    self.l.pack( side=LEFT )
    self.eb = Button( self.f, text="Slow", command=self.__easy )
    self.eb.pack( side=LEFT, padx=5 )
    self.mb = Button( self.f, text="Medium", command=self.__med )
    self.mb.pack( side=LEFT, padx=5 )
    self.hb = Button( self.f, text="Fast", command=self.__hard )
    self.hb.pack( side=LEFT, padx=5 )

  def __easy( self ):
    global initial_momentum
    initial_momentum.z = 0.3

  def __med( self ):
    global initial_momentum
    initial_momentum.z = 0.6
  def __hard( self ):
    global initial_momentum
    initial_momentum.z = 1.2

class ResistanceButton:
  def __init__( self, root ):
    self.f = Frame( root )
    self.f.pack( side=TOP, padx=5, pady=5 )
    self.c = StringVar()
    self.l = Label( self.f, text="Set degree of resistance:" )
    self.l.pack( side=LEFT )
    self.eb = Button( self.f, text="None", command=self.__s )
    self.eb.pack( side=LEFT, padx=5 )
    self.mb = Button( self.f, text="Medium", command=self.__m )
    self.mb.pack( side=LEFT, padx=5 )
    self.hb = Button( self.f, text="Large", command=self.__l )
    self.hb.pack( side=LEFT, padx=5 )
    self.l2 = Label( self.f, textvariable=self.c, text=str(resistance) )
    self.l2.pack( side=LEFT )
  def __s( self ):
    global resistance
    resistance = 0
    self.c.set(str(resistance))    
  def __m( self ):
    global resistance
    resistance = 5
    self.c.set(str(resistance))
  def __l( self ):
    global resistance
    resistance = 10
    self.c.set(str(resistance))


    
def BuildGUI( root ):
  global sb, sp, mb, stb, sd
  sp = SpeedButton( root )
  sd = DiffButton( root )
  mb = ResistanceButton( root ) 
  stb =StyleButton( root )
  sb = StartButton( root )
   

root = Tk( None, "" )
root.geometry("+100+100")
fr = Frame( root )
fr.pack( padx=50, pady=10 )
info= Label( fr, text="SPACE TENNIS - SenseGraphics" )
info.pack( side=TOP,pady=10 )
info2= Label( fr, text="Set the speed of the ball. Resistance decides the \n power of the force opposite to movement \n of the racket \n\n Bounce: Keep the ball in play \n Score: Bounce ball past opponent \n Precision: Hit the moving target " )
info2.pack( side=TOP,pady=10 )

BuildGUI( fr )

# wait until UI is closed
while not closeUI:
  root.update()

# create a new SpaceTennisGame
spacetennis = SpaceTennis.SpaceTennisGame( g, resistance, initial_momentum, 
                                           gameStyle, momEffect )
# start the game
spacetennis.restartGame()

# update the spacetennis game once per scenegraph loop. traverseSG is called
# once per scenegraph loop when traversing the PythonScript node in the 
# scene graph.
def traverseSG():
  spacetennis.update()

# Class needed to clean up circula references. When this global class
# is destroyed it means that the PythonScript is unloaded which means
# that there should be no circular references to the scene graph.
class CleanUpClass:
  # classes_to_clean should contain a list of classes which has the cleanUp
  # function.
  def __init__(self, classes_to_clean ):
    self.classes_to_clean = classes_to_clean

  # Call cleanUp for all contained classes. The cleanUp functions should
  # remove circular references in the class.
  def __del__(self):
    for i in self.classes_to_clean:
      i.cleanUp()

cleanUpClass = CleanUpClass( [spacetennis] )

