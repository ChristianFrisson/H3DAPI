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
/// \brief Header file for ShaderImageNode.
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
  ///  ComposedShader node which is image node type.
  ///
  /// 
  class H3DAPI_API ShaderImageNode : 
    public ShaderChildNode {
  public:
  
    /// Constructor.
    ShaderImageNode( Inst< DisplayList > _displayList = 0,
                            Inst< SFNode>  _metadata = 0 );

    /// specify the texture_unit will be used for rendering this shader image
    /// or more specifically, it is the texture image unit where this image
    /// unit is located. 
    virtual void preRender( GLenum texture_unit_id );


    virtual void postRender();

    virtual int getImageUnit ( ){
      return image_unit;
    }

    ~ShaderImageNode ( );
  protected:
    /// The texture image unit for the texture where the image located
    unsigned int texture_unit;

    /// image unit used, will be used to bind to shader program
    /// the value of image_unit will be maintained in this node.
    /// Note: this is different from texture image unit which is used by
    /// texture object to be accessed in shader
    int image_unit;

    /// texture object id which is used to bind to the texture unit
    /// this value will be maintained in this node
    unsigned int texture_id;

    
    
    // collection of all the current image units already used, 
    //it is be used to help generate non-identical image unit id
    static set<unsigned int> global_image_units;
    // lock to ensure the access to global_image_units is always safe
    static H3DUtil::MutexLock global_image_units_lock;
    static unsigned int max_image_unit;
    // create a image unit id, it will be ensured that any shader image
    // used have different image unit it
    static int generateImage ( );
    // remove the image unit id
    static void deleteImage ( int id );
  };
}

#endif
