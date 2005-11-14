import math
from H3D import *
from H3DInterface import *
import random


class SS:
  def __init__( self, n1, n2, n3, m, a, b  ):
    self.n1 = n1
    self.n2 = n2
    self.n3 = n3
    self.m  = m
    self.a  = a
    self.b  = b

src_ss1 = SS( 100, 100, 100, 4, 1, 1 )
src_ss2 = SS( 100, 100, 100, 4, 1, 1 )
dst_ss1 = SS( 100, 100, 100, 3, 1, 1 )
dst_ss2 = SS( 100, 100, 100, 5, 1, 1 )

ss1_m  = SFFloat()
ss1_n1 = SFFloat()
ss1_n2 = SFFloat()
ss1_n3 = SFFloat()
ss1_a  = SFFloat()
ss1_b  = SFFloat()

ss2_m  = SFFloat()
ss2_n1 = SFFloat()
ss2_n2 = SFFloat()
ss2_n3 = SFFloat()
ss2_a  = SFFloat()
ss2_b  = SFFloat()

tick=0

def traverseSG():
  global src_ss1, src_ss2, dst_ss1, dst_ss2
  global tick

  ss1_m.setValue ( src_ss1.m  * (1-tick) + dst_ss1.m  * tick )
  ss1_n1.setValue( src_ss1.n1 * (1-tick) + dst_ss1.n1 * tick )
  ss1_n2.setValue( src_ss1.n2 * (1-tick) + dst_ss1.n2 * tick )
  ss1_n3.setValue( src_ss1.n3 * (1-tick) + dst_ss1.n3 * tick )
  ss1_a.setValue ( src_ss1.a  * (1-tick) + dst_ss1.a  * tick )
  ss1_b.setValue ( src_ss1.b  * (1-tick) + dst_ss1.b  * tick )

  ss2_m.setValue ( src_ss2.m  * (1-tick) + dst_ss2.m  * tick )
  ss2_n1.setValue( src_ss2.n1 * (1-tick) + dst_ss2.n1 * tick )
  ss2_n2.setValue( src_ss2.n2 * (1-tick) + dst_ss2.n2 * tick )
  ss2_n3.setValue( src_ss2.n3 * (1-tick) + dst_ss2.n3 * tick )
  ss2_a.setValue ( src_ss2.a  * (1-tick) + dst_ss2.a  * tick )
  ss2_b.setValue ( src_ss2.b  * (1-tick) + dst_ss2.b  * tick )

  tick = tick + 0.01
  if tick > 1.0:
    tick = 0
    #generate new src/dst
    src_ss1 = dst_ss1
    src_ss2 = dst_ss2 
    dst_ss1 = SS( 10*random.random(),
                  2*random.random()+1,
                  2*random.random()+1,
                  2*random.random()+1,
                  10*random.random()+1,
                  10*random.random()+1 )
    dst_ss2 = SS( 10*random.random(),
                  2*random.random()+1,
                  2*random.random()+1,
                  2*random.random()+1,
                  10*random.random()+1,
                  10*random.random()+1 )
