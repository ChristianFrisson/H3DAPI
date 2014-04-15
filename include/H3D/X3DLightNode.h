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
/// \file X3DLightNode.h
/// \brief Header file for X3DLightNode, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __X3DLIGHTNODE_H__
#define __X3DLIGHTNODE_H__

#include <H3D/X3DChildNode.h>
#include <H3D/H3DDisplayListObject.h>
#include <H3D/H3DRenderStateObject.h>
#include <H3D/SFFloat.h>
#include <H3D/SFColor.h>

namespace H3D {

  /// \ingroup AbstractNodes
  /// \class X3DLightNode
  /// \brief The X3DLightNode abstract node type is the base type from which
  /// all node types that serve as light sources are derived. 
  ///
  /// Any node used as a source of illumination is derived from X3DLightNode.
  /// All light sources contain an intensity, a color, and an ambientIntensity
  /// field. The intensity field specifies the brightness of the direct
  /// emission from the light, and the ambientIntensity specifies the 
  /// intensity of the ambient emission from the light. Light intensity may
  /// range from 0.0 (no light emission) to 1.0 (full intensity). The color
  /// field specifies the spectral colour properties of both the direct and
  /// ambient light emission as an RGB value. The on field specifies whether
  /// the light is enabled or disabled. If the value is FALSE, then the light
  /// is disabled and will not affect any nodes in the scene. If the value
  /// is TRUE, then the light will affect other nodes.
  ///
  /// Each light type defines a global field that determines whether the light
  /// is global or scoped. Global lights illuminate all objects that fall
  /// within their volume of lighting influence. Scoped lights only illuminate
  /// objects that are in the same transformation hierarchy as the light; i.e.,
  /// only the children and descendants of its enclosing parent group are
  /// illuminated. This allows the creation of realistic effects such as lights
  /// that illuminate a single room.
  ///
  /// \par Internal routes:
  /// \dotfile X3DLightNode.dot
  class H3DAPI_API X3DLightNode : 
    public X3DChildNode,
    public H3DDisplayListObject,
    public H3DRenderStateObject {
  public:

    /// A structure representing a the OpenGL representation of the 
    /// specified light source.
    struct GLLightInfo {
      GLLightInfo (
                   RGBA _ambient    = RGBA(0.0f,0.0f,0.0f, 1.0f), 
                   RGBA _diffuse    = RGBA(0.0f,0.0f,0.0f,1.0f), 
                   RGBA _specular   = RGBA(0.0f,0.0f,0.0f,1.0f), 
        Vec4f _position = Vec4f(0.0f,0.0f,0.0f,0.0f),
        Vec3f _spotDirection= Vec3f(),
        H3DFloat _spotExponent= 0,
        H3DFloat _spotCutoff= 180.0f,
        H3DFloat _constantAttenuation= 0.0f,
        H3DFloat _linearAttenuation= 0.0f, 
        H3DFloat _quadraticAttenuation= 0.0f ) : 
          ambient ( _ambient ), 
          diffuse ( _diffuse ), 
          specular ( _specular ),
          position ( _position ),
          spotDirection ( _spotDirection ),
            spotExponent( _spotExponent ),
          spotCutoff ( _spotCutoff ),
          constantAttenuation ( _constantAttenuation ),
          linearAttenuation ( _linearAttenuation ), 
          quadraticAttenuation ( _quadraticAttenuation ) {}

      RGBA ambient;   
      RGBA diffuse;   
      RGBA specular;  
      Vec4f position;
      Vec3f spotDirection;
      H3DFloat spotExponent;
      H3DFloat spotCutoff;          // (range: [0.0,90.0], 180.0) 
      H3DFloat linearAttenuation; 
      H3DFloat constantAttenuation; 
      H3DFloat quadraticAttenuation;    
      
      bool isDirectionalLight() {
        return position.w == 0;
      }

      bool isPointLight() {
        return !isDirectionalLight() && spotCutoff == 180;
      }

      bool isSpotLight() {
        return !isDirectionalLight() && !isPointLight();
      }

    };

    /// Constructor.
    X3DLightNode( Inst< SFNode >  _metadata         = 0,
                  Inst< SFFloat>  _ambientIntensity = 0,
                  Inst< SFColor>  _color            = 0,
                  Inst< SFBool >  _global           = 0,
                  Inst< SFFloat>  _intensity        = 0,
                  Inst< SFBool >  _on               = 0,
                  Inst< SFBool >  _shadows          = 0 );

    /// Turn the light on.
    virtual void enableGraphicsState();

    /// Turn the light off.
    virtual void disableGraphicsState();

    virtual GLLightInfo getGLLightInfo() 
#ifndef H3D_GENERATE_DOTROUTE_FILES
                                        = 0;
#else
    { return GLLightInfo (RGBA(0.0f,0.0f,0.0f, 1.0f), RGBA(0.0f,0.0f,0.0f,1.0f), 
                   RGBA(0.0f,0.0f,0.0f,1.0f), Vec4f(0.0f,0.0f,0.0f,0.0f), Vec3f(), 0,
           180.0f, 0.0f, 0.0f, 0.0f ); }
#endif

    /// Add light to TraverseInfo.
    virtual void enableHapticsState( TraverseInfo &ti );

    /// Remove light from TraverseInfo
    virtual void disableHapticsState( TraverseInfo &ti );

    /// Traverse the scenegraph. Stores pointer to X3DLightNode and
    /// transformation matrices for those that are global.
    /// \param ti The TraverseInfo object containing information about the
    /// traversal.
    virtual void traverseSG( TraverseInfo &ti );

    /// returns an index that can be used to enable light.
    /// \param name_for_error contains the name to print in case
    /// the light index should be ignored.
    static GLuint getLightIndex( string name_for_error );

    /// must be called if getLightIndex have been called outside
    /// X3DLightNode.
    static void decreaseLightIndex() { --global_light_index; }

    /// Specifies the intensity of the ambient emission from the light.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 0 \n
    /// <b>Valid range:</b> [0-1]
    /// 
    /// \dotfile X3DLightNode_ambientIntensity.dot
    auto_ptr< SFFloat >  ambientIntensity;

    /// The color field specifies the spectral colour properties of both 
    /// the direct and ambient light emission as an RGB value.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> RGB( 1, 1, 1 ) \n
    /// <b>Valid range:</b> [0-1]
    /// 
    /// \dotfile X3DLightNode_color.dot
    auto_ptr< SFColor >  color;

    /// The global field specifies whether the X3DLightNode affects all
    /// geometries in the scene or only those geometries that are in the
    /// same parent group or any descendant to the parent group.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> FALSE \n
    /// 
    /// \dotfile X3DLightNode_global.dot
    auto_ptr< SFBool  >  global;

    /// The intensity field specifies the brightness of the direct emission
    /// from the light.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 1 \n
    /// <b>Valid range:</b> [0-1]
    /// 
    /// \dotfile X3DLightNode_intensity.dot
    auto_ptr< SFFloat >  intensity;

    /// The on field specifies whether the light is enabled or disabled. If 
    /// the value is FALSE, then the light is disabled and will not affect
    /// any nodes in the scene.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> TRUE \n
    /// 
    /// \dotfile X3DLightNode_on.dot
    auto_ptr< SFBool  >  on;

    /// The shadows field will determine if this light will be part of the
    /// shadow calculations of the scene. 
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> TRUE \n
    /// 
    /// \dotfile X3DLightNode_shadows.dot
    auto_ptr< SFBool  >  shadows;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;

  protected:
    /// Light index for this node, used to enable light.
    GLuint light_index;
    static GLint max_lights;
    static GLint global_light_index;

    /// only interested in adress, what it points to will be invalid
    TraverseInfo * last_ti_ptr;

    /// count how many times this X3DLightNodes traverseSG function
    /// is called.
    H3DInt32 traverse_sg_counter;
    /// count nr of times enableGraphicsState is called.
    H3DInt32 graphics_state_counter;

    vector< Matrix4f > global_light_transforms;

    /// used if the light should act as a global light.
    bool act_global;

    /// true if a light index was created for each call to
    /// enableGraphicsState for this X3DLightNode
    list< bool > had_light_index;
  };
}

#endif
