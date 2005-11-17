//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004, SenseGraphics AB
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
/// \file GLWindow.h
/// \brief Header file for GLWindow, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __GLWINDOW_H__
#define __GLWINDOW_H__

#include "Viewpoint.h"
#include "MFNode.h"

namespace H3D {

  /// \ingroup Nodes
  /// \class GLWindow
  /// \brief Standard OpenGL based window node.
  /// 
  class H3DAPI_API GLWindow : public Node {
  public:
    
    typedef TypedSFNode< Viewpoint > SFViewpoint;

    /// The mode for rendering specified as a string.
    class RenderMode: public SFString {
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
        /// Stereo mode where every second vertical line is rendered for the 
        /// left eye and every other line for the right eye, but with the green 
        /// pixel shifted one pixel to the right. Used with for example the 
        /// Sharp auto-stereoscopic displays.
        VERTICAL_INTERLACED_GREEN_SHIFT,  
        /// Stereo mode where the left eye is rendered in red and the right 
        /// eye in blue. 
        RED_BLUE_STEREO,
        /// Stereo mode where the left eye is rendered in red and the right 
        /// eye in cyan. 
        RED_CYAN_STEREO
      } Mode;

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
    GLWindow( Inst< SFInt32     > _width      = 0,
              Inst< SFInt32     > _height     = 0,
              Inst< SFBool      > _fullscreen = 0,
              Inst< SFBool      > _mirrored   = 0,
              Inst< RenderMode  > _renderMode = 0, 
              Inst< SFViewpoint > _viewpoint  = 0,
              Inst< SFTime      > _time       = 0 );

    /// Destructor.
    ~GLWindow();

    /// Creates and Initializes the GLUT window.
    virtual void initialize();

    /// Given the identifier of a GLUT window the GLWindow instance
    /// that created that window is returned.
    static GLWindow * getGLWindow( int glut_id ); 

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
    bool calculateFarAndNearPlane( H3DFloat &far,
                                   H3DFloat &near,
                                   X3DChildNode *child,
                                   Viewpoint *vp,
                                   bool include_stylus );
                                   
    
    /// This function renders the X3DChildNode given into the 
    /// window of the GLWindow.
    virtual void render( X3DChildNode *child_to_render );

    /// Returns the GLUT window id for this window.
    int getGLUTWindowId() {
      return window_id;
    }

    /// This function will be called when the GLUT notices that
    /// the window size has changed.
    virtual void reshape( int w, int h );

    /// This function is called when the window has to redraw itself.
    virtual void display();

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
    /// by the rendering in GLWindow instead of the stack top of 
    /// the Viewpoint bindable stack. This is so that multiple windows
    /// with different viewpoints can be specified.
    ///  
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> NULL \n
    auto_ptr< SFViewpoint > viewpoint;
    
    /// Initialize GLUT. 
    static void initGLUT();
    
    auto_ptr< SFTime > time;
    static set< GLWindow* > windows;
    
    /// This function shares the rendering context between this GLWindow
    /// and the one given as an argument. This means that the two windows
    /// after the call can share display lists and textures. When several
    /// GLWindow instances are created this function will always be called
    /// to share the rendering context between them, i.e. all GLWindow
    /// instances share rendering context by default.
    void shareRenderingContext( GLWindow *w );

    /// Returns true if the initialize() function has been called.
    bool isInitialized() {
      return initialized;
    }

#if WIN32
    HGLRC getRenderingContext() {
      return rendering_context;
    }
#endif
    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  private:
#if WIN32
    HGLRC rendering_context;
#endif
    X3DChildNode *last_render_child;
    bool initialized;
    static bool GLUT_init;
    static bool GLEW_init;
    int window_id;
    bool rebuild_stencil_mask;
  };
}

#endif
