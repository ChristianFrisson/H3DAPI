//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004-2014, SenseGraphics AB
//
//    This file is part of H3D API.
//
//    H3D API is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    H3D API is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with H3D API; if not, write to the Free Software
//    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//    A commercial license is also available. Please contact us at 
//    www.sensegraphics.com for more information.
//
//
/// \file H3DWindowNode.h
/// \brief Header file for H3DWindowNode.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __H3DWINDOWNODE_H__
#define __H3DWINDOWNODE_H__

#include <H3D/X3DViewpointNode.h>
#include <H3D/MFNode.h>
#include <H3D/SFInt32.h>
#include <H3D/SFVec2f.h>
#include <H3D/DefaultAppearance.h>
#include <H3D/NavigationInfo.h>
#include <H3D/FrameBufferTextureGenerator.h>

namespace H3D {
  class H3DNavigation;

  /// \ingroup X3DNodes
  /// \class H3DWindowNode
  /// \brief The base class for all window nodes.
  /// 
  /// A H3DWindowNode handles creation of windows and window properties for 
  /// looking into a Scene. To implement a new window class the following
  /// functions have to be specified:
  ///   swapBuffers(), initWindow(), initWindowHandler(), setFullscreen( bool fullscreen )
  ///   makeWindowActive(), setCursorType( const std::string & cursor_mode )
  /// and getCursorForMode.
  /// For example implementation see GLUTWindow.

  class H3DAPI_API H3DWindowNode : public Node {
  public:

    typedef TypedSFNode< X3DViewpointNode > SFViewpoint;
    typedef TypedSFNode< NavigationInfo > SFNavigationInfo;

    /// The mode for rendering specified as a string.
    class H3DAPI_API RenderMode: public SFString {
    public:
      /// Thrown when the value of RenderMode is an invalid mode.
      H3D_VALUE_EXCEPTION( string, InvalidRenderMode );
      /// The different render modes supported.
      typedef enum {
        /// Normal rendering.
        MONO,
        /// Quad buffered stereo rendering. Needs a quad buffered
        /// enabled graphics card
        QUAD_BUFFERED_STEREO, 
        /// Stereo mode where the left eye is rendered as the top half 
        /// of the screen and the right eye as the bottom half of the screen.
        HORIZONTAL_SPLIT,
        /// Stereo mode where the left eye is rendered as the left half 
        /// of the screen and the right eye as the right half of the screen.
        VERTICAL_SPLIT,
        /// Stereo mode where every second horizontal line is rendered for the 
        /// left eye and every other line for the right eye.
        HORIZONTAL_INTERLACED,  
        /// Stereo mode where every second vertical line is rendered for the 
        /// left eye and every other line for the right eye.
        VERTICAL_INTERLACED,  
        /// Stereo mode where "white" and "black" in a checker pattern
        /// are rendered for the left and right eye, respectively.
        CHECKER_INTERLACED,
        /// Stereo mode where every second vertical line is rendered for the 
        /// left eye and every other line for the right eye, but with the green 
        /// pixel shifted one pixel to the right. Used with for example the 
        /// Sharp auto-stereoscopic displays.
        VERTICAL_INTERLACED_GREEN_SHIFT,  
        /// Stereo mode where the left eye is rendered in red and the right 
        /// eye in blue. 
        RED_BLUE_STEREO,
        /// Stereo mode where the left eye is rendered in red and the right 
        /// eye in green. 
        RED_GREEN_STEREO,
        /// Stereo mode where the left eye is rendered in red and the right 
        /// eye in cyan. 
        RED_CYAN_STEREO,

        /// Stereo mode where the left eye is rendered as the left half 
        /// of the screen and the right eye as the right half of the screen.
        /// The aspect ratio is preserved in this mode, so that it can be used
        /// when e.g. extending the desktop in order for rendering each
        /// the left eye for one projector and the right for another.
        VERTICAL_SPLIT_KEEP_RATIO,
    
        /// Stereo mode where the left eye is rendered as the top half 
        /// of the screen and the right eye as the bottom half of the screen.
        /// The aspect ratio is preserved in this mode, so that it can be used
        /// when e.g. extending the desktop in order for rendering each
        /// the left eye for one projector and the right for another.  
        HORIZONTAL_SPLIT_KEEP_RATIO,

        /// HDMI 1.4 frame packed format. Basically horizontal split with
        /// 30 lines of black between each image.
        HDMI_FRAME_PACKED_720P,

        /// HDMI 1.4 frame packed format. Basically horizontal split with
        /// 45 lines of black between each image.
        HDMI_FRAME_PACKED_1080P,

        /// NVidia 3DVision format when not using quad-buffered stereo. Will
        /// copy OpenGL buffer and display with DirectX. Executable must be
        /// named to one of the NVidia sanctioned 3D programs in order to work,
        /// e.g. googleearth.exe.
        NVIDIA_3DVISION

      } Mode;

      /// setValue is specialized so that the stencil mask is rebuilt if
      /// the mode choosen is one that requires a stencil mask
      virtual void setValue( const string &v,  int id = 0 ) {
        SFString::setValue( v, id );
        if( value == "VERTICAL_INTERLACED" ||
            value == "HORIZONTAL_INTERLACED" ||
            value == "VERTICAL_INTERLACED_GREEN_SHIFT" ) {
          H3DWindowNode *window = static_cast< H3DWindowNode * >( getOwner() );
          window->rebuild_stencil_mask = true;
        }
      }

      /// update is specialized so that the stecil mask is rebuilt if
      /// the mode choosen is one that requires a stencil mask
      virtual void update() {
        SFString::update();
        if( value == "VERTICAL_INTERLACED" ||
            value == "HORIZONTAL_INTERLACED" ||
            value == "VERTICAL_INTERLACED_GREEN_SHIFT" ) {
          H3DWindowNode *window = static_cast< H3DWindowNode * >( getOwner() );
          window->rebuild_stencil_mask = true;
        }
      }

      /// Returns true if the current render mode is a stereo mode.
      inline bool isStereoMode() { 
        upToDate();
        return value != "MONO";
      }

      /// Get the current RenderMode string as an Mode enum. 
      /// \throws InvalidRenderMode if the string is an invalid RenderMode.
      RenderMode::Mode getRenderMode();
    };

    /// Constructor.
    H3DWindowNode( Inst< SFInt32     > _width      = 0,
                   Inst< SFInt32     > _height     = 0,
                   Inst< SFBool      > _fullscreen = 0,
                   Inst< SFBool      > _mirrored   = 0,
                   Inst< RenderMode  > _renderMode = 0,
                   Inst< SFViewpoint > _viewpoint  = 0,
                   Inst< SFInt32     > _posX       = 0,
                   Inst< SFInt32     > _posY       = 0,
                   Inst< SFBool      > _manualCursorControl = 0,
                   Inst< SFString    > _cursorType = 0,
                   Inst< SFNavigationInfo > _navigationInfo = 0,
                   Inst< SFBool      > _useFullscreenAntiAliasing = 0,
                   Inst< SFVec2f     > _clipDistances = 0 );

    /// Destructor.
    ~H3DWindowNode();

    /// Virtual function to swap buffers.
    virtual void swapBuffers() = 0;

    /// Virtual function that should create a new window and set its properties
    /// depending on the fields. This function should set window_is_made_active
    /// to true if window is made active at initialization.
    virtual void initWindow() = 0;

    /// Virtual function to initialize the window handler if needed.
    /// E.g. glutInit().
    virtual void initWindowHandler() = 0;

    /// Virtual function to set whether the window should be fullscreen or not.
    virtual void setFullscreen( bool fullscreen ) = 0;

    /// Virtual function to make the current window active, i.e. make subsequent
    /// OpenGL calls draw in the context of this window.
    /// This function should set window_is_made_activeto true if window is
    /// made active.
    virtual void makeWindowActive() = 0;

    /// Initialize the window node. 
    virtual void initialize();

    static void fboCallback( FrameBufferTextureGenerator *g, int i, void *args );

    /// This function renders the X3DChildNode given into the 
    /// window of the H3DWindowNode.
    virtual void render( X3DChildNode *child_to_render );

    /// This function will be called when the window size has changed.
    virtual void reshape( int w, int h );

    /// This function is called when the window has to redraw itself.
    virtual void display();

    /// This function should be called by a window when a key is pressed.
    /// When special is true the key is a special key 
    /// enumerated in X3DKeyDeviceSensorNode.h
    /// The window handles the translation from its internal 
    /// behaviour to ASCII and the special numbers.
    /// \param key Specifies the key that is pressed.
    /// \param special True when the key is a special key, and
    /// false when it is not.
    virtual void onKeyDown( int key, bool special );

    /// This function should be called by a window when a key is released.
    /// When special is true the key is a special key 
    /// enumerated in X3DKeyDeviceSensorNode.h
    /// The window handles the translation from its internal 
    /// behaviour to ASCII and the special numbers.
    /// \param key Specifies the key that is released
    /// \param special True when the key is a special key, and
    /// false when it is not.
    virtual void onKeyUp( int key, bool special );

    /// This function should be called by a window when a mouse button
    /// is pressed or released. For values of button and state see
    /// MouseSensor.h
    /// \param button specifies the mouse button pressed or released
    /// \param state shows that the button is pressed or released
    virtual void onMouseButtonAction( int button, int state );

    /// This function should be called by a window when a mouse
    /// sends a motion event. x, y are window relative coordinates
    virtual void onMouseMotionAction( int x, int y );

    /// This function should be called by a window when a mouse
    /// wheel is scrolled either from or towards the user
    /// For values of direction see MouseSensor.h
    virtual void onMouseWheelAction( int direction );

    /// Calculate the far and near clipping planes from the bounding
    /// box of a X3DChildNode. The far and near planes will be calculated
    /// on contain the node but nothing else. Requires the bounding box 
    /// type to be BoxBound.
    /// \param far Return param for the far plane.
    /// \param near Return param for the near plane.
    /// \param child The child node we want to be visible.
    /// \param vp The Viewpoint in the scene. 
    /// \param include_stylus if true the stylus from each HapticsDevice
    /// in DeviceInfo will also be included in the calculations of the
    /// planes. The bounding box we will use is then the union of the 
    /// stylus fields and the child parameter.
    /// \returns True if successfully calculated and returned parameters.
    /// false if the bounding boxes was of another type than BoxBound.
    static bool calculateFarAndNearPlane( H3DFloat &far,
                                   H3DFloat &near,
                                   X3DChildNode *child,
                                   X3DViewpointNode *vp,
                                   bool include_stylus );

    /// The x coordinate in pixels of the window in screen coordinates.
    ///  
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> -1 meaning let the window manager choose\n
    auto_ptr< SFInt32 >  posX;

    /// The y coordinate in pixels of the window in screen coordinates.
    ///  
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> -1  meaning let the window manager choose\n
    auto_ptr< SFInt32 >  posY;

    /// The width in pixels of the window.
    ///  
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 800 \n
    auto_ptr< SFInt32 >  width;

    /// The height in pixels of the window.
    ///  
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 600 \n
    auto_ptr< SFInt32 >  height;

    /// Determines if the window should be in fullscreen or not.
    ///  
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> FALSE \n
    auto_ptr< SFBool >  fullscreen;

    /// Determines if the scene rendered should be mirrored or not.
    ///  
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> FALSE \n
    auto_ptr< SFBool >  mirrored;

    /// The mode to render the scene in the window in.
    /// See RenderMode::Mode for valid values. Values are specified 
    /// as a string with the same name as the enum value of the mode 
    /// you want.
    ///  
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> "MONO" \n
    auto_ptr< RenderMode >  renderMode;

    /// If the viewpoint field is specified, that viewpoint is used 
    /// by the rendering in H3DWindowNode instead of the stack top of 
    /// the Viewpoint bindable stack. This is so that multiple windows
    /// with different viewpoints can be specified.
    ///  
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> NULL \n
    auto_ptr< SFViewpoint > viewpoint;

    /// If set to true changes of cursor is totally up to the user of
    /// the window by changing the cursorType. If false, H3D API will 
    /// control which cursor to show, changing cursor when e.g. a
    /// X3DPointingDeviceSensorNode is being pointed at.
    ///  
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> false \n
    auto_ptr< SFBool > manualCursorControl;

    /// The cursorType field specifies the look of the mouse pointer.
    /// For the user to be in control of what cursorType should be 
    /// shown the manualCursorControl field must be set to true.
    /// The valid values depend on the underlying window system
    /// so query the valid values before setting a cursor type. See
    /// implementations of H3DWindowNode such as GLUTWindow or
    /// wxWindowNode to see that cursors they support.
    ///  
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> "DEFAULT" \n
    /// <b>Valid values:</b> Different depending on implementation.
    auto_ptr< SFString >  cursorType;

    /// If the navigationInfo field is specified, that NavigationInfo is used 
    /// for rendering in H3DWindowNode instead of the stack top of 
    /// the NavigationInfo bindable stack. This is so that multiple windows
    /// with different NavigationInfos can be specified.
    ///  
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> NULL \n
    auto_ptr< SFNavigationInfo > navigationInfo;

    /// The distance (in meters) used for the far and near clip
    /// plane. Far plane is x, near plane is y. A value
    /// of -1 for far plane means that far plane is at infinity. 
    ///
    /// <b>Access type:</b> outputOnly \n
    auto_ptr< SFVec2f > clipDistances;

    /// If set to true fullscreen anti-aliasing is used for this window. 
    /// This will smooth the appearance of objects and reduce jaggedness 
    /// of the rendering.
    ///
    /// <b>Access type:</b> initializeOnly \n
    /// <b>Default value:</b> true \n
    auto_ptr< SFBool > useFullscreenAntiAliasing;

    static set< H3DWindowNode* > windows;

    inline static bool getMultiPassTransparency() {
      return multi_pass_transparency;
    }

    inline X3DViewpointNode::EyeMode getEyeMode() {
      return eye_mode;
    }

    /// This function shares the rendering context between this H3DWindowNode
    /// and the one given as an argument. This means that the two windows
    /// after the call can share display lists and textures. When several
    /// H3DWindowNode instances are created this function will always be called
    /// to share the rendering context between them, i.e. all H3DWindowNode
    /// instances share rendering context by default.
    virtual void shareRenderingContext( H3DWindowNode *w );

#ifdef WIN32
    HGLRC getRenderingContext() {
      return rendering_context;
    }

    /// Class Conversion Operator For Window Handle 
    /// (H3DWindowNode Can Be Used As A Window Handle)
    operator HWND() { return hWnd; }
#endif

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;

    // Variables containing default values for navigation.
    string default_nav;
    vector< H3DFloat > default_avatar;
    H3DFloat default_speed;
    bool default_collision;
    vector< string > default_transition_type;
    H3DTime default_transition_time;

  protected:
    /// internal help function to initialise the window and
    /// check if success. it will call the initWindow function
    /// to initialize the window.
    void initWindowWithContext();
#ifdef WIN32
    HGLRC rendering_context;
    HWND hWnd;
    HINSTANCE windowInstance;

    // WindowProc calls the Window Procedure stored in wpOrigProc
    // Default is DefWindowProc
    WNDPROC wpOrigProc;

    /// Whenever a subclass to H3DWindowNode is created
    /// this callback is a subclass callback. The only function
    /// of this callback is to send the right events to KeySensor.
    /// When the events are processed it calls the Window Procedure
    /// stored in wpOrigProc.
    static LRESULT CALLBACK WindowProc(HWND _hWnd, UINT uMsg, 
                                       WPARAM wParam, LPARAM lParam);
    /// Handles messages. Called by WindowProc.
    LRESULT Message(HWND _hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
#endif

    /// If multi pass transparency is set to true the scene will be rendered
    /// three times graphically, once for all solid objects, once for the back
    /// side of transparent objects and once for the front face of 
    /// transparent objects. This is in order to avoid artifacts for 
    /// transparent objects when rendered in the wrong order. 
    inline static void setMultiPassTransparency( bool b ) {
      multi_pass_transparency = b;
    }

    // Render the child node. Will be called after all viewports etc has
    // been set up in order to render the child and everything else that
    // needs to be rendered.
    void renderChild( X3DChildNode *c );

    /// Help function for renderChild.
    void renderChild( X3DChildNode *child_to_render,
                      bool render_fbo_only );

    /// Set the cursor to the given cursor type. See cursorType field
    /// for valid values. Returns 0 on success. -1 if the cursor_type is 
    /// not supported.
    virtual int setCursorType( const std::string & cursor_type ) { 
      return -1; 
    }

    /// Return the cursor type to use for given modes. This should
    /// be implemented for each subclass to choose appropriate cursors.
    /// The standard modes are:
    /// "DEFAULT" - normal mode
    /// "ON_SENSOR_OVER" - when mouse pointer is over a pointing device
    /// sensor.
    /// "ON_SENSOR_ACTIVE" - when a sensor node is active
    /// "ON_NAV_LOOKAT" - when lookat mode is chosen
    virtual string getCursorForMode( const string &) { return "DEFAULT"; }

    friend class Scene; 

    AutoRef< FrameBufferTextureGenerator > generator;

    static bool multi_pass_transparency;
    X3DChildNode *last_render_child;
    static bool GLEW_init;
    int window_id;
    bool rebuild_stencil_mask;
    unsigned char *stencil_mask;
    unsigned int stencil_mask_height;
    unsigned int stencil_mask_width;
    bool last_loop_mirrored;

    AutoRef< X3DViewpointNode > vp_ref;

    /// The render mode used in the last render loop.
    RenderMode::Mode last_render_mode;

    /// Current eye mode
    X3DViewpointNode::EyeMode eye_mode;

    /// The cursor currently in use.
    string current_cursor;

    // Stores the current mouse_position as it was when updating MouseSensors.
    // Used as input to updateX3DPointingDeviceSensors.
    H3DInt32 mouse_position[2];
    // Stores the previous mouse position.
    H3DInt32 previous_mouse_position[2];
    // Stores the current left mouse button state.
    vector< bool > left_mouse_button;
    // Stores previous left mouse button state.
    bool previous_left_mouse_button;

    // Instance to class handling navigation for this window.
    H3DNavigation * h3d_navigation;

    bool window_is_made_active, check_if_stereo_obtained;
    void checkIfStereoObtained();
  public:
    /// Saves the currently rendered buffer to file. That is, a screenshot
    /// of the openGL window.
    static string takeScreenShot( const string &url );
  };
}

#endif
