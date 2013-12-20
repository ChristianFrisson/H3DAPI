//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004-2013, SenseGraphics AB
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
/// \file ShaderImageNode.h
/// \brief Header file for ShaderImageNode, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __SHADERIMAGENode_H__
#define __SHADERIMAGENode_H__

#include <H3D/ShaderChildNode.h>

namespace H3D {

  /// \ingroup AbstractNodes
  /// \class ShaderImageNode
  /// \brief This is the base node type for the child nodes of the
  ///  ComposedShader node.
  ///
  /// 
  class H3DAPI_API ShaderImageNode : 
    public ShaderChildNode {
  public:
  
    /// Constructor.
    ShaderImageNode( Inst< DisplayList > _displayList = 0,
                            Inst< SFNode>  _metadata = 0 );

    /// specify the texture_unit will be used for rendering this shader image
    virtual void preRender( GLenum texture_unit_id );


    virtual void postRender();

    /// update texture and image if necessary, then bind it to shader program
    ///virtual void render ( );

    //virtual void renderImage( unsigned int texture_unit_id );
    /// install texture and image for current node to specified texture unit
    virtual void prepareShaderImage ( unsigned int texture_unit_id );

    virtual unsigned int getImageUnit ( ){
      return image_unit;
    }

    ~ShaderImageNode ( );
  protected:
    /// texutre unit used to provide the texture layer for the image unit
    /// the value will comes from outside of this node when actually use it
    /// from a shader program
    unsigned int texture_unit;

    /// image unit used, will be used to bind to shader program
    /// the value of image_unit will be maintained in this node
    unsigned int image_unit;

    /// texture object id which is used to bind to the texture unit
    /// this value will be maintained in this node
    unsigned int texture_id;

    
    
    /// all the current image units already used, it will be used to help
    /// generate idential image unit id
    static set<unsigned int> global_image_units;
    static H3DUtil::MutexLock global_image_units_lock;
    static unsigned int max_image_unit;
    static unsigned generateImage ( );
    static void deleteImage ( unsigned int id );
  };
}

#endif
