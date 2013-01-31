#!/usr/bin/env python

import os
import wx
import ConfigParser
import string
import sys

settings_path = "."
H3D_ROOT = os.getenv( "H3D_ROOT" )
if H3D_ROOT and os.path.exists(H3D_ROOT):
  settings_path = H3D_ROOT + "/settings/"

h3dload_ini_path = settings_path + "/h3dload.ini"
if sys.platform == "win32":
  HOMEDRIVE = os.getenv("HOMEDRIVE")
  HOMEPATH = os.getenv( "HOMEPATH" )
  tmp_h3dload_ini_path = "%s%s\\h3dload.ini"%(HOMEDRIVE,HOMEPATH)
  if HOMEDRIVE and HOMEPATH and os.path.exists(tmp_h3dload_ini_path):
    h3dload_ini_path = tmp_h3dload_ini_path
else:
  HOME = os.getenv( "HOME" )
  tmp_h3dload_ini_path = "%s/.h3dload.ini"%HOME
  if HOME and os.path.exists(tmp_h3dload_ini_path):
    h3dload_ini_path = tmp_h3dload_ini_path
    
cp = ConfigParser.ConfigParser()
cp.read(h3dload_ini_path)

stereo_mode_box_options = ["No stereo", "Quad buffered", "Horizontal split", "Horizontal split keep ratio", "Vertical split", "Vertical split keep ratio", "Horizontal interlaced", "Vertical interlaced", "Checker Interlaced", "Sharp display", "Red-Blue stereo", "Red-Green stereo", "Red-Cyan stereo" ]

stereo_mode_strings = [ "MONO", "QUAD_BUFFERED_STEREO", "HORIZONTAL_SPLIT", "HORIZONTAL_SPLIT_KEEP_RATIO", "VERTICAL_SPLIT", "VERTICAL_SPLIT_KEEP_RATIO", "HORIZONTAL_INTERLACED", "VERTICAL_INTERLACED", "CHECKER_INTERLACED", "VERTICAL_INTERLACED_GREEN_SHIFT", "RED_BLUE_STEREO", "RED_GREEN_STEREO", "RED_CYAN_STEREO" ]

#----------------------------------------------------------------------
class MainFrame(wx.Frame):
    """
    Main window/frame.
    """
    title = "Stroke Assessment Prototype"
    def __init__(self, app, parent):
      wx.Frame.__init__(self, parent, -1, self.title, size=(1024,700),
                         style=wx.DEFAULT_FRAME_STYLE | wx.NO_FULL_REPAINT_ON_RESIZE)
      self.app = app

        
      self.frame = MyFrame(self, None)
      #


class TestDialog(wx.Dialog):
  def getCommonDevices( self ):
    try:
      root, dirs, files  = os.walk(settings_path + "/common/device").next()
      return files
    except:
      return []


  def isCommonStylus( self, stylus ):
    try:
      root, dirs, files  = os.walk(settings_path + "/common/stylus").next()
      for f in files:
        if( stylus == f ): 
          return 1
      return 0
    except:
      return 0

  def isCommonDevice( self, device ):
    try:
      root, dirs, files  = os.walk(settings_path + "/common/device").next()
      for f in files:
        if( device == f ): 
          return 1
      return 0
    except:
      return 0

  def getCommonStylus( self ):
    try:
      root, dirs, files  = os.walk(settings_path + "/common/stylus").next()

      if( cp.has_option( "haptics device", "stylus" ) ):
        stylus = cp.get( "haptics device", "stylus" ) 
        if( not( self.isCommonStylus( stylus ) ) ):
          files = files + [stylus]

      return files + ["Browse..." ] 
    except:
      return ["Browse..." ] 

  def isCommonViewpoint( self, viewpoint ):
    try:
      root, dirs, files  = os.walk(settings_path + "/common/viewpoint").next()
      for f in files:
        if( viewpoint == f ): 
          return 1
      return 0
    except:
      return 0

  def isDisplayViewpoint( self, display, viewpoint ):
    try:
      root, dirs, files  = os.walk(settings_path + "/display/" + display + "/viewpoint").next()
      for f in files:
        if( viewpoint == f ): 
          return 1
      return 0
    except:
      return 0

  def getCommonViewpoint( self ):
    try:
      root, dirs, files  = os.walk(settings_path + "/common/viewpoint").next()
      return files + ["Browse..." ] 
    except:
      return ["Browse..." ] 

  def getViewpointForDisplay( self, display ):
    try:
      root, dirs, files  = os.walk(settings_path + "/display/" + display + "/viewpoint").next()
      return files + self.browse_viewpoints + self.getCommonViewpoint()
    except:
      return self.browse_viewpoints + self.getCommonViewpoint()

  def getDevicesForDisplay( self, display ):
    try:
      root, dirs, files  = os.walk(settings_path + "/display/" + display + "/device").next()
      return ["None"] + self.getCommonDevices() + files
    except:
      return []

  def onDisplayChoice( self, event ):
    devices = self.getDevicesForDisplay( self.display_choice.GetValue() )
    selected_value = self.device_choice.GetValue()
    self.device_choice.Clear()
    for d in devices:
      self.device_choice.Append( d )
    if( not( self.device_choice.SetStringSelection( selected_value ) ) ):
      self.device_choice.SetSelection( 0 )

    viewpoints = self.getViewpointForDisplay( self.display_choice.GetValue() )
    selected_vp = self.viewpoint_choice.GetValue()
    self.viewpoint_choice.Clear()

    for vp in viewpoints:
      self.viewpoint_choice.Append( vp )
    if( not( self.viewpoint_choice.SetStringSelection( selected_vp ) ) ):
      self.viewpoint_choice.SetSelection( 0 )

  def onStylusChoice( self, event ):
    if( self.stylus_choice.GetValue() == "Browse..." ):
      dlg = wx.FileDialog(
        self, message="Choose a file", defaultDir=os.getcwd(), 
        defaultFile="", wildcard="", style=wx.OPEN | wx.MULTIPLE | wx.CHANGE_DIR
        )
      if dlg.ShowModal() == wx.ID_OK:
        path = dlg.GetPath()
        n = self.stylus_choice.GetCount()
        self.stylus_choice.Insert( path, n-1 )
        self.stylus_choice.SetSelection( n - 1 )
      else:
        self.stylus_choice.SetSelection( 0 )

  def onViewpointChoice( self, event ):
    if( self.viewpoint_choice.GetValue() == "Browse..." ):
      dlg = wx.FileDialog(
        self, message="Choose a file", defaultDir=os.getcwd(), 
        defaultFile="", wildcard="", style=wx.OPEN | wx.MULTIPLE | wx.CHANGE_DIR
        )
      if dlg.ShowModal() == wx.ID_OK:
        path = dlg.GetPath()
        n = self.viewpoint_choice.GetCount()
        self.viewpoint_choice.Insert( path, n-1 )
        self.viewpoint_choice.SetSelection( n - 1 )
        self.browse_viewpoints.append( path )
      else:
        self.viewpoint_choice.SetSelection( 0 )

  def saveCurrentSettings( self ):
##     key = _winreg.OpenKey( _winreg.HKEY_CURRENT_USER,
##                            "Environment",
##                            0,
##                            _winreg.KEY_SET_VALUE )
##     if( self.fullscreen_cb.GetValue() ):
##       _winreg.SetValueEx( key, "H3D_FULLSCREEN", "", _winreg.REG_SZ, "TRUE" )
##     else:
##       _winreg.SetValueEx( key, "H3D_FULLSCREEN", "", _winreg.REG_SZ, "FALSE" )

##     if( self.mirrored_cb.GetValue() ):
##       _winreg.SetValueEx( key, "H3D_MIRRORED", "", _winreg.REG_SZ, "TRUE" )
##     else:
##       _winreg.SetValueEx( key, "H3D_MIRRORED", "", _winreg.REG_SZ, "FALSE" )

##     mode = stereo_mode_strings[ self.stereo_choice.GetSelection() ]

##     _winreg.SetValueEx( key, "H3D_RENDERMODE", "", _winreg.REG_SZ, mode )

##     _winreg.FlushKey( key )
##     _winreg.CloseKey( key )
##     print "send"
## #   win32gui.SendMessage(win32con.HWND_BROADCAST, win32con.WM_SETTINGCHANGE, 0, 'Environment')
##     print "done"
    if( not( cp.has_section( "display" ) ) ):
      cp.add_section( "display" )

    if( not( cp.has_section( "haptics device" ) ) ):
      cp.add_section( "haptics device" )

    if( not( cp.has_section( "graphical" ) ) ):
      cp.add_section( "graphical" )

    cp.set( "graphical", "mirrored", str( self.mirrored_cb.GetValue() ) )
    cp.set( "graphical", "fullscreen", str( self.fullscreen_cb.GetValue() ) )
    cp.set( "graphical", "rendermode", stereo_mode_strings[ self.stereo_choice.GetSelection() ] )
    cp.set ("graphical", "viewpoint", self.viewpoint_choice.GetValue() )

    cp.set( "haptics device", "device", self.device_choice.GetValue() )
    cp.set( "haptics device", "stylus", self.stylus_choice.GetValue() )

    cp.set( "display", "type", self.display_choice.GetValue() )
    
    global h3dload_ini_path
    if not os.access(h3dload_ini_path, os.W_OK):
      if sys.platform == "win32":
        HOMEDRIVE = os.getenv("HOMEDRIVE")
        HOMEPATH = os.getenv( "HOMEPATH" )
        h3dload_ini_path = "%s%s\\h3dload.ini"%(HOMEDRIVE, HOMEPATH)
      else:
        HOME = os.getenv( "HOME" )
        h3dload_ini_path = "%s/.h3dload.ini"%HOME

    f = open( h3dload_ini_path, "w" )
    cp.write( f )
    f.close()

    f = open( settings_path + "/current/deviceinfo.x3d", "w" )
    display = self.display_choice.GetValue()
    device = self.device_choice.GetValue()
    if( device != "None" ):
      if( self.isCommonDevice( device ) ):
        url = "../common/device/" + device
      else:
        url = "../display/" + display +  "/device/" + device
      f.write( "<Inline DEF=\"DEFAULT_DEVICEINFO\" url=\"" + url + "\" />" )
    else:
      f.write("<Group />" )
    f.close()

    f = open( settings_path + "/current/stylus.x3d", "w" )
    stylus = self.stylus_choice.GetValue()
    if( self.isCommonStylus(  stylus ) ):
      url = "../common/stylus/" + stylus
    else:
      url = stylus
    f.write( "<Inline DEF=\"DEFAULT_STYLUS\" url=\"" + url + "\" />" )
    f.close()

    f = open( settings_path + "/current/viewpoint.x3d", "w" )
    vp = self.viewpoint_choice.GetValue()
    if( self.isCommonViewpoint( vp ) ):
      url = "../common/viewpoint/" + vp
    elif( self.isDisplayViewpoint( self.display_choice.GetValue(), vp ) ):
      url = "../display/" + display + "/viewpoint/" + vp
    else:
      url = stylus
    f.write( "<Inline DEF=\"DEFAULT_VIEWPOINT\" url=\"" + url + "\" />" )
    f.close()
    

  def onApply( self, event ):
    self.saveCurrentSettings()

  def getAvailableDisplays( self ):
    try:
      root, dirs, files  = os.walk(settings_path + "/display").next()
      return dirs
    except:
      return []

  def getDefaultDisplay( self ):
    if( cp.has_option( "display", "type" ) ):
      return cp.get( "display", "type" )
    else:
      return "None"

  def getDefaultDevice( self ):
    if( cp.has_option( "haptics device", "device" ) ):
      return cp.get( "haptics device", "device" )
    else:
      return "None"

  def getDefaultMirrored( self ):
    if( cp.has_option( "graphical", "mirrored" ) ):
      return cp.getboolean( "graphical", "mirrored" )
    else:
      return 0

  def getDefaultFullscreen( self ):
    if( cp.has_option( "graphical", "fullscreen" ) ):
      return cp.getboolean( "graphical", "fullscreen" )
    else:
      return 0

  def getDefaultStylus( self ):
    if( cp.has_option( "haptics device", "stylus" ) ):
      return cp.get( "haptics device", "stylus" ) 
    else:
      return "H3D Default.x3d"

  def getDefaultViewpoint( self ):
    if( cp.has_option( "graphical", "viewpoint" ) ):
      return cp.get( "graphical", "viewpoint" ) 
    else:
      return "X3D Default.x3d"

  def getDefaultStereoMode( self ):
    if( cp.has_option( "graphical", "rendermode" ) ):
      mode = cp.get( "graphical", "rendermode" )
      i = 0
      for s in stereo_mode_strings:
        if( mode == s ):
          return stereo_mode_box_options[ i ]
        i = i + 1
    else:
      return "MONO"

    

  def __init__(
    self, parent, ID, title, size=wx.DefaultSize, pos=wx.DefaultPosition, 
    style=wx.DEFAULT_DIALOG_STYLE
    ):

    # Instead of calling wx.Dialog.__init__ we precreate the dialog
    # so we can set an extra style that must be set before
    # creation, and then we create the GUI dialog using the Create
    # method.
    pre = wx.PreDialog()
    pre.SetExtraStyle(wx.DIALOG_EX_CONTEXTHELP)
    pre.Create(parent, ID, title, pos, size, style)
    
    # This next step is the most important, it turns this Python
    # object into the real wrapper of the dialog (instead of pre)
    # as far as the wxPython extension is concerned.
    self.PostCreate(pre)
    
    # Now continue with the normal construction of the dialog
    # contents
    sizer = wx.BoxSizer(wx.VERTICAL)
    
    # Display type choice
    b = wx.StaticBox( self, -1, "Immersive display settings" )
    box = wx.StaticBoxSizer(b, wx.HORIZONTAL)
    label = wx.StaticText(self, -1, "Display type: ")
    label.SetHelpText("The type of immersive display you are using")
    box.Add(label, 0, wx.ALIGN_CENTRE|wx.ALL, 5)
    temp_selection_string = self.getDefaultDisplay()
    self.display_choice = wx.ComboBox(self, -1, temp_selection_string, (90, 50), (180,-1),
                                      self.getAvailableDisplays(), wx.CB_READONLY )
    self.display_choice.SetHelpText("Here's some help text for field #1")
    self.display_choice.SetStringSelection( temp_selection_string )
    box.Add(self.display_choice, 1, wx.ALIGN_CENTRE|wx.ALL, 5)
    
    self.Bind(wx.EVT_COMBOBOX, self.onDisplayChoice, self.display_choice )
    
    sizer.Add(box, 0, wx.GROW|wx.ALIGN_CENTER_VERTICAL|wx.ALL, 5)
    
    # Haptics device choice
    b = wx.StaticBox( self, -1, "Haptics device settings" )
    static_box = wx.StaticBoxSizer(b, wx.VERTICAL)
    box = wx.BoxSizer(wx.HORIZONTAL)
    label = wx.StaticText(self, -1, "Haptics device:")
    label.SetHelpText("The haptics devices used in the display")
    box.Add(label, 0, wx.ALIGN_CENTRE|wx.ALL, 5)
    
    temp_selection_string = self.getDefaultDevice()
    self.device_choice = wx.ComboBox(self, -1, temp_selection_string, (90, 50), (180,-1), self.getDevicesForDisplay( self.display_choice.GetValue() ) , wx.CB_READONLY )
    self.device_choice.SetStringSelection( temp_selection_string )
    
    self.device_choice.SetHelpText("Here's some help text for field #1")
    box.Add(self.device_choice, 1, wx.ALIGN_CENTRE|wx.ALL, 5)
    
    stylus_box = wx.BoxSizer(wx.HORIZONTAL)
    label = wx.StaticText(self, -1, "Default stylus: ")
    label.SetHelpText("blah")
    stylus_box.Add( label, 0, wx.ALIGN_CENTRE|wx.ALL, 5)
    temp_selection_string = self.getDefaultStylus()
    self.stylus_choice = wx.ComboBox(self, -1, temp_selection_string, 
                                     (90, 50), (180,-1), 
                                     self.getCommonStylus(), 
                                     wx.CB_READONLY )
    self.stylus_choice.SetStringSelection( temp_selection_string )
    self.Bind(wx.EVT_COMBOBOX, self.onStylusChoice, self.stylus_choice )
    self.stylus_choice.SetHelpText("Here's some help text for field #1")
    
    stylus_box.Add( self.stylus_choice, 1, wx.ALIGN_CENTRE|wx.ALL, 5)
    
    static_box.Add( box, 1, wx.ALIGN_LEFT|wx.ALL, 5)
    static_box.Add( stylus_box, 1, wx.ALIGN_LEFT|wx.ALL, 5)
    sizer.Add(static_box, 0, wx.GROW|wx.ALIGN_CENTER_VERTICAL|wx.ALL, 5)
    
    # Render settings 
    b = wx.StaticBox( self, -1, "Graphical render settings" )
    static_box = wx.StaticBoxSizer(b, wx.VERTICAL)
    box = wx.BoxSizer( wx.HORIZONTAL )
    label = wx.StaticText(self, -1, "Stereo mode:")
    label.SetHelpText("The mode in which to render the scene graphically")
    box.Add(label, 0, wx.ALIGN_CENTRE|wx.ALL, 5)
    
    temp_selection_string = self.getDefaultStereoMode()
    self.stereo_choice = wx.ComboBox(self, -1, temp_selection_string,
                                     (90, 50), (130,-1), 
                                     stereo_mode_box_options, wx.CB_READONLY )
    self.stereo_choice.SetHelpText("Here's some help text for field #1")
    box.Add(self.stereo_choice, 1, wx.ALIGN_CENTRE|wx.ALL, 5)
    self.stereo_choice.SetStringSelection(temp_selection_string)
    
    check_boxes = wx.BoxSizer(wx.VERTICAL)
    self.mirrored_cb = wx.CheckBox( self, -1, "Mirrored in Y" )
    self.mirrored_cb.SetValue( self.getDefaultMirrored() )
    self.fullscreen_cb = wx.CheckBox( self, -1, "Fullscreen" )
    self.fullscreen_cb.SetValue( self.getDefaultFullscreen() )
    check_boxes.Add(self.mirrored_cb, 1, wx.ALIGN_LEFT|wx.ALL, 5)
    check_boxes.Add(self.fullscreen_cb, 1, wx.ALIGN_LEFT|wx.ALL, 5)
    box.Add(check_boxes, 1, wx.ALIGN_CENTRE|wx.ALL, 5)
    static_box.Add( box, 1, wx.ALIGN_CENTRE|wx.ALL, 5)
    
    viewpoint_box = wx.BoxSizer(wx.HORIZONTAL)
    label = wx.StaticText(self, -1, "Default Viewpoint: ")
    label.SetHelpText("blah")
    viewpoint_box.Add( label, 0, wx.ALIGN_CENTRE|wx.ALL, 5)

    self.browse_viewpoints = []

    if( cp.has_option( "graphical", "viewpoint" ) ):
      vp = cp.get( "graphical", "viewpoint" ) 
      if( not( self.isCommonViewpoint( vp ) ) and 
          not( self.isDisplayViewpoint( self.display_choice.GetValue(), vp ) ) ):
        self.browse_viewpoints.append( vp )
    temp_selection_string = self.getDefaultViewpoint()
    self.viewpoint_choice = wx.ComboBox(self, -1, temp_selection_string,
                                        (90, 50), (120,-1), 
                                        self.getViewpointForDisplay( self.display_choice.GetValue() ), 
                                        wx.CB_READONLY )
    self.viewpoint_choice.SetStringSelection(temp_selection_string)
    self.Bind(wx.EVT_COMBOBOX, self.onViewpointChoice, self.viewpoint_choice )
    self.viewpoint_choice.SetHelpText("Here's some help text for field #1")
    
    viewpoint_box.Add( self.viewpoint_choice, 1, wx.ALIGN_CENTRE|wx.ALL, 5)
    static_box.Add( viewpoint_box, 1, wx.ALIGN_LEFT|wx.ALL, 5)
    sizer.Add(static_box, 0, wx.GROW|wx.ALIGN_CENTER_VERTICAL|wx.ALL, 5)
    
    
    #        line = wx.StaticLine(self, -1, size=(20,-1), style=wx.LI_HORIZONTAL)
    #        sizer.Add(line, 0, wx.GROW|wx.ALIGN_CENTER_VERTICAL|wx.RIGHT|wx.TOP, 5)
    
    btnsizer = wx.StdDialogButtonSizer()
    
    if wx.Platform != "__WXMSW__":
      btn = wx.ContextHelpButton(self)
      btnsizer.AddButton(btn)
      
    btn = wx.Button(self, wx.ID_OK)
    btn.SetHelpText("The OK button completes the dialog")
    btn.SetDefault()
    btnsizer.AddButton(btn)
    
    btn = wx.Button(self, wx.ID_CANCEL)
    btn.SetHelpText("The Cancel button cnacels the dialog. (Cool, huh?)")
    btnsizer.AddButton(btn)

    btn = wx.Button(self, wx.ID_APPLY )
    btn.SetHelpText("The Cancel button cnacels the dialog. (Cool, huh?)")
    self.Bind(wx.EVT_BUTTON, self.onApply, btn)
    btnsizer.AddButton(btn)

    btnsizer.Realize()
    
    sizer.Add(btnsizer, 0, wx.ALIGN_RIGHT|wx.ALL, 5)
    
    self.SetSizer(sizer)
    sizer.Fit(self)

#----------------------------------------------------------------------


class MainApp(wx.App):
  def __init__( self, arg ):
    wx.App.__init__( self, arg )

  def OnInit(self):
    msg = "        Please log in to the database:         "
    dlg = TestDialog( None, -1, "H3DLoad settings" )
    dlg.CenterOnScreen()
      
    # this does not return until the dialog is closed.
    val = dlg.ShowModal()
      
    if val == wx.ID_OK:
      dlg.saveCurrentSettings()
#    else:
#      print "Cancel"

    dlg.Destroy()

    return True

#----------------------------------------------------------------------
      
if __name__ == '__main__':
  app = MainApp(0)
  app.MainLoop()



