from H3DInterface import *
import random
import time as python_time
random.seed( python_time.time() )

DRAW_TOGGLE, TEXT_TOGGLE, GEOM_TYPE_TEXT = references.getValue()

class DrawGeometry:
  def __init__( self, name, color = RGB( 1, 1, 0 ) ):
    node = None
    self.coordDestination = None
    field_list = None
    self.dist_between_coords = None
    self.min_nr_of_coords = 1
    if name == "Extrusion":
      extrusion_strings = []
      extrusion_strings.append( """<Extrusion crossSection='0.01 0.01, 0.01 -0.01, -0.01 -0.01, -0.01 0.01, 0.01 0.01' />""" )
      extrusion_strings.append( """<Extrusion crossSection='0.01 0.004, 0.01 -0.004, 0.004 -0.01, -0.004 -0.01, -0.01 -0.004, -0.01 0.004, -0.004 0.01, 0.004 0.01, 0.01 0.004' />""" )
      extrusion_strings.append( """<Extrusion crossSection='0 0, 0 0.01, 0.01 0.01, 0.01 -0.01, -0.01 -0.01, -0.01 0, 0 0' />""" )
      extrusion_strings.append( """<Extrusion crossSection='0.005 0.01, 0.005 -0.01, -0.005 -0.01, -0.005 0.01, 0.005 0.01' />""" )
      extrusion_strings.append( """<Extrusion crossSection='0 -0.005, 0 0, 0.004 0.005, 0.01 0.007' solid="false" endCap="false" beginCap="false"/>""" )
      node, dn = createX3DNodeFromString( extrusion_strings[random.randint( 0, len(extrusion_strings)-1 )] )
      self.coordDestination = node.spine
      self.dist_between_coords = 0.01
      self.min_nr_of_coords = 2
    else:
      node = createNode( name )
      field_list = node.getFieldList()
      coord = createNode( "Coordinate" )
      if "coord" in field_list:
        node.coord.setValue( coord )
      elif "controlPoint" in field_list:
        node.controlPoint.setValue( coord )
      else:
        raise Exception( "Can not handle node of type " + name )
      self.coordDestination = coord.point
    
    global DRAW_TOGGLE
    self.tg, self.dn = createX3DNodeFromString( """<ToggleGroup graphicsOn="false" hapticsOn="false">
                                                      <Shape DEF='THE_SHAPE'>
                                                        <Appearance DEF='APP'>
                                                          <Material DEF='MAT' />
                                                          <SmoothSurface/>
                                                        </Appearance>
                                                      </Shape>
                                                    </ToggleGroup>""" )
    self.dn['THE_SHAPE'].geometry.setValue( node )

    if name == "PointSet" or name == "LineSet" or name == "NurbsCurve":
      self.dn['MAT'].emissiveColor.setValue( color )
      if name == "LineSet":
        self.min_nr_of_coords = 2
      elif name == "NurbsCurve":
        self.min_nr_of_coords = 4
        self.dist_between_coords = 0.01
    else:
      self.dn['MAT'].diffuseColor.setValue( color )

    self.coordField = MFVec3f()
    
    if name.startswith( "Triangle" ):
      self.min_nr_of_coords = 3
      self.dist_between_coords = 0.01
      if field_list and "solid" in field_list:
        node.solid.setValue( False )
      
    self.vertexCount = None
    self.vertexCountDestination = None
    if field_list:
      if "vertexCount" in field_list:
        self.vertexCountDestination = node.vertexCount
      elif "stripCount" in field_list:
        self.vertexCountDestination = node.stripCount
      elif "fanCount" in field_list:
        self.vertexCountDestination = node.fanCount
    if self.vertexCountDestination:
      self.vertexCount = MFInt32()

    self.weight = None
    if field_list and "weight" in field_list:
      self.weight = MFDouble()
      self.weight.route( node.weight )
    self.tessellation = None
    if field_list and "tessellation" in field_list:
      self.tessellation = SFInt32()
      self.tessellation.route( node.tessellation )
    DRAW_TOGGLE.children.push_back( self.tg )
  
  def pushPos( self, pos ):
    self.coordField.push_back( pos )
    if self.weight:
      self.weight.push_back( 1 )
    if self.vertexCount:
      self.vertexCount.setValue( [self.coordField.size()] )
    if self.tessellation:
      self.tessellation.setValue( self.coordField.size() * 10 )

  def addPos( self, pos ):
    if not self.dist_between_coords:
      self.pushPos(pos)
    else:
      coord_field = self.coordField.getValue()
      coord_len = len( coord_field )
      if coord_len == 0:
        self.pushPos(pos)
      elif coord_len > 0 and ( coord_field[ coord_len-1] - pos ).length() > self.dist_between_coords:
        self.pushPos(pos)
      
    if self.min_nr_of_coords != -1 and self.coordField.size() >= self.min_nr_of_coords:
      self.coordField.route( self.coordDestination )
      self.tg.graphicsOn.setValue( True )
      self.min_nr_of_coords = -1
      if self.vertexCountDestination:
        self.vertexCount.route( self.vertexCountDestination )
    
      
  def toggleHaptics( self, haptics_on ):
    if self.tg.hapticsOn.getValue() != haptics_on:
      self.tg.hapticsOn.setValue( haptics_on )

class DevicePositionChangeField( AutoUpdate(TypedField(SFBool, (SFBool, SFVec3f))) ):
  def __init__( self ):
    AutoUpdate(TypedField(SFBool, (SFBool, SFVec3f))).__init__(self)
    self.draw_geom_index = 0
    self.draw_geoms = []
    self.draw_geoms.append( "PointSet" )
    self.draw_geoms.append( "LineSet" )
    self.draw_geoms.append( "Extrusion" )
    self.draw_geoms.append( "TriangleSet" )
    self.draw_geoms.append( "TriangleStripSet" )
    self.draw_geoms.append( "TriangleFanSet" )
    self.draw_geoms.append( "NurbsCurve" )
    #add trianglefanset here.
    self.geom_to_draw = None
    self.color = RGB( 1, 1, 0 )
    self.is_drawing = False
    
  def update(self, event):
    click, pos = self.getRoutesIn()
    if click.getValue():
      if not self.geom_to_draw:
        self.geom_to_draw = DrawGeometry( self.draw_geoms[self.draw_geom_index], self.color )
        self.geom_to_draw.toggleHaptics( False )
      if not self.is_drawing:
        # disable haptics
        self.is_drawing = True
        self.geom_to_draw.toggleHaptics( False )
      self.geom_to_draw.addPos( pos.getValue() )
    elif self.is_drawing:
      self.geom_to_draw.toggleHaptics( True )
      self.is_drawing = False
    return True
  
  def nextGeometry( self ):
    global DRAW_TOGGLE, GEOM_TYPE_TEXT
    self.draw_geom_index = ( self.draw_geom_index + 1 ) % len( self.draw_geoms )
    GEOM_TYPE_TEXT.string.setValue( ["Geometry is: " + self.draw_geoms[self.draw_geom_index]] )
    if self.geom_to_draw:
      self.geom_to_draw.toggleHaptics( DRAW_TOGGLE.hapticsOn.getValue() )
      self.geom_to_draw = None
  
  def setColor( self, color ):
    global DRAW_TOGGLE
    self.color = color
    if self.geom_to_draw:
      self.geom_to_draw.toggleHaptics( DRAW_TOGGLE.hapticsOn.getValue() )
      self.geom_to_draw = None
  
  def clear( self ):
    global DRAW_TOGGLE
    DRAW_TOGGLE.children.clear()
    if self.geom_to_draw:
      self.geom_to_draw = None
  
  def toggleHaptics( self ):
    global DRAW_TOGGLE
    DRAW_TOGGLE.hapticsOn.setValue( not DRAW_TOGGLE.hapticsOn.getValue() )

position_change = DevicePositionChangeField()

class KeyPress( AutoUpdate(SFString) ):    
  def update(self, event):
    global position_change
    k = event.getValue()
    if k == ' ':
      print 'Clear board'
      position_change.clear()
    elif k == 't':
      TEXT_TOGGLE.graphicsOn.setValue(not TEXT_TOGGLE.graphicsOn.getValue())
    elif k == 'n':
      position_change.nextGeometry()
    elif k == 'c':
      position_change.setColor( RGB( random.random(), random.random(), random.random() ) )
    elif k == 'h':
      position_change.toggleHaptics()
    return ''
    
keypress = KeyPress()

device = getHapticsDevice(0)
if not device:
  di = createX3DNodeFromString( """<DeviceInfo><AnyDevice/></DeviceInfo>""" )[0]
  device = getHapticsDevice(0)
if device:
  device.mainButton.routeNoEvent( position_change )
  device.weightedProxyPosition.route( position_change )
  device.stylus.setValue( createX3DNodeFromString( """<Shape containerField='stylus'>
                                                        <Appearance>
                                                          <Material/>
                                                        </Appearance>
                                                        <Sphere radius='0.0025'/>
                                                      </Shape>""" )[0] )
  device.hapticsRenderer.setValue( createNode( "RuspiniRenderer" ) )