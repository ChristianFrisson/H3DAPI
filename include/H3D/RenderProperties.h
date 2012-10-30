//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004-2007, SenseGraphics AB
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
/// \file RenderProperties.h
/// \brief Header file for RenderProperties, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __RENDERPROPERTIES_H__
#define __RENDERPROPERTIES_H__

#include <H3D/X3DAppearanceChildNode.h>
#include <H3D/SFColor.h>
#include <H3D/SFInt32.h>
#include <H3D/SFColorRGBA.h>
#include <H3D/SFString.h>
#include <H3D/SFFloat.h>

namespace H3D {

  /// \ingroup H3DNodes
  /// \class RenderProperties
  /// \brief The RenderProperties node specifies additional rendering options
  /// that cannot be specified with the X3D Appearance node. 
  ///
  /// The depthTestEnabled field specifies if depth test should be enabled
  /// or not. If depth test is disabled geometries will be drawn over the 
  /// current values in the frame buffer regardless of if it is in front of 
  /// or behind the previous objects drawn.
  ///
  /// The smoothShading field specifies if smooth shading should be used.
  /// If false, flat shading is used.
  ///
  /// The depthBufferWriteEnabled field specifies if depth values should be written
  /// to the depth buffer or not during rendering.
  /// 
  /// The colorBufferRedWriteEnabled field specifies if red color values should be written
  /// to the color buffer or not during rendering.
  ///
  /// The colorBufferGreenWriteEnabled field specifies if green color values should be written
  /// to the color buffer or not during rendering.
  ///
  /// The colorBufferBlueWriteEnabled field specifies if blue color values should be written
  /// to the color buffer or not during rendering.
  /// 
  /// The colorBufferAlphaWriteEnabled field specifies if alpha values should be written
  /// to the color buffer or not during rendering.
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/RenderProperties.x3d">RenderProperties.x3d</a>
  ///     ( <a href="examples/RenderProperties.x3d.html">Source</a> )
  ///   - <a href="../../../H3DAPI/examples/All/RenderProperties1.x3d">RenderProperties1.x3d</a>
  ///     ( <a href="examples/RenderProperties1.x3d.html">Source</a> )
  ///   - <a href="../../../H3DAPI/examples/All/RenderProperties2.x3d">RenderProperties2.x3d</a>
  ///     ( <a href="examples/RenderProperties2.x3d.html">Source</a> )
  /// 
  /// \par Internal routes:
  /// \dotfile RenderProperties.dot
  class H3DAPI_API RenderProperties : public X3DAppearanceChildNode {
  public:
    
    /// Constructor.
    RenderProperties( Inst< SFNode      >  _metadata   = 0 ,
                      Inst< DisplayList > _displayList      = 0,
                      Inst< SFBool      > _depthTestEnabled = 0,
                      Inst< SFBool      > _smoothShading    = 0,
                      Inst< SFBool      > _multiPassTransparency = 0,
                      Inst< SFBool      > _depthBufferWriteEnabled = 0,
                      Inst< SFBool      > _colorBufferRedWriteEnabled = 0,
                      Inst< SFBool      > _colorBufferGreenWriteEnabled = 0,
                      Inst< SFBool      > _colorBufferBlueWriteEnabled = 0,
                      Inst< SFBool      > _colorBufferAlphaWriteEnabled = 0,
                      Inst< SFString    > _alphaFunc = 0,
                      Inst< SFFloat     > _alphaFuncValue = 0,
                      Inst< SFString    > _blendFuncSrcFactorRGB = 0,
                      Inst< SFString    > _blendFuncSrcFactorAlpha = 0,
                      Inst< SFString    > _blendFuncDstFactorRGB = 0,
                      Inst< SFString    > _blendFuncDstFactorAlpha = 0,
                      Inst< SFString    > _blendEquationRGB = 0,
                      Inst< SFString    > _blendEquationAlpha = 0,
                      Inst< SFColorRGBA > _blendColor = 0
                      );


    /// Returns a bitmask of the OpenGL attrib bits that will be affected
    /// by this node. The value can be used in glPushAttrib in order
    /// to save the current state.
    virtual GLbitfield getAffectedGLAttribs() {
      return( GL_LIGHTING_BIT | GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    }

    /// Set up the line properties with OpenGL.
    virtual void render();

    /// Returns the default xml containerField attribute value.
    /// For this node it is "fillProperties".
    ///
    virtual string defaultXMLContainerField() {
      return "renderProperties";
    }

    /// The depthTestEnabled field specifies if depth test should be enabled
    /// or not. If depth test is disabled geometries will be drawn over the 
    /// current values in the frame buffer regardless of if they are in front of 
    /// or behind the previous objects drawn.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> true \n
    /// 
    /// \dotfile RenderProperties_depthTestEnabled.dot 
    auto_ptr< SFBool >  depthTestEnabled;

    /// The smoothShading field specifies if smooth shading of colors
		/// should be used. If true then
		/// colors are interpolated between vertices of a face. If false then
		/// each face has a uniform color chosen from one of its vertices.
		/// For more information see OpenGL reference for the
		/// command glShadeModel.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> true \n
    /// 
    /// \dotfile RenderProperties_smoothShading.dot 
    auto_ptr< SFBool > smoothShading;

    /// If the multiPassTransparency field is true, and we have a transparent
    /// shape, the entire scene will be rendered three times, once with all
    /// non-transparant objects drawn, once with all transparant objects drawn
    /// with front face culled and once with all transparent objects with back
    /// face culled. This will avoid many artifact of transparency caused
    /// by rendering objects in the wrong order, however it requires three
    /// traversals of the scenegraph. If set to false, transparent objects
    /// will just be drawn in the order they appear in the scene graph.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> true \n
    /// 
    /// \dotfile RenderProperties_multiPassTransparency.dot 
    auto_ptr< SFBool > multiPassTransparency;

    /// The depthBufferWriteEnabled field specifies if depth values should be written
    /// to the depth buffer of not during rendering.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> true \n
    /// 
    /// \dotfile RenderProperties_depthBufferWriteEnabled.dot 
    auto_ptr< SFBool >  depthBufferWriteEnabled;

    /// The colorBufferRedWriteEnabled field specifies if red color values should be written
    /// to the color buffer of not during rendering.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> true \n
    /// 
    /// \dotfile RenderProperties_colorBufferRedWriteEnabled.dot 
    auto_ptr< SFBool >  colorBufferRedWriteEnabled;

    /// The colorBufferGreenWriteEnabled field specifies if green color values should be written
    /// to the color buffer of not during rendering.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> true \n
    /// 
    /// \dotfile RenderProperties_colorBufferGreenWriteEnabled.dot 
    auto_ptr< SFBool >  colorBufferGreenWriteEnabled;

    /// The colorBufferBlueWriteEnabled field specifies if blue color values should be written
    /// to the color buffer of not during rendering.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> true \n
    /// 
    /// \dotfile RenderProperties_colorBufferBlueWriteEnabled.dot 
    auto_ptr< SFBool >  colorBufferBlueWriteEnabled;

    /// The colorBufferAlphaWriteEnabled field specifies if alpha values should be written
    /// to the color buffer of not during rendering.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> true \n
    /// 
    /// \dotfile RenderProperties_colorBufferAlphaWriteEnabled.dot 
    auto_ptr< SFBool >  colorBufferAlphaWriteEnabled;

    auto_ptr< SFString > alphaFunc;
    auto_ptr< SFFloat  > alphaFuncValue;
    auto_ptr< SFString > blendFuncSrcFactorRGB;
    auto_ptr< SFString > blendFuncDstFactorRGB;
    auto_ptr< SFString > blendFuncSrcFactorAlpha;
    auto_ptr< SFString > blendFuncDstFactorAlpha;
    auto_ptr< SFString > blendEquationRGB;
    auto_ptr< SFString > blendEquationAlpha;
    auto_ptr< SFColorRGBA > blendColor;
    
    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  protected:
    int getSFactor( const string &sfactor_string, GLenum &sfactor );
    int getDFactor( const string &dfactor_string, GLenum &dfactor );
    int getBlendEquation( const string &equation_string, GLenum &equation );
    int getAlphaFunc( const string &alpha_func_string, GLenum &alpha_func );
  };
}

#endif
