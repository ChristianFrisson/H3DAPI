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

#include <H3D/H3DSingleTextureNode.h>

namespace H3D {

  /// \ingroup AbstractNodes
  /// \class ShaderImageNode
  /// \brief This is the base node type for the child nodes of the
  ///  ComposedShader node which is image node type, it will a texture which
  ///  will be used as texture image for opengl.
  ///
  /// 
  class H3DAPI_API ShaderImageNode : 
    public H3DSingleTextureNode {
  public:
  
    /// Constructor.
    ShaderImageNode( Inst< DisplayList > _displayList = 0,
                            Inst< SFNode>  _metadata = 0 );

    /// override preRender function to do nothing as it is not needed
    /// for shader image node
    virtual void preRender( ){};

    /// override postRender function to do nothing as it is not needed for 
    /// shader image node
    virtual void postRender(){};

    /// enableTexturing is not needed for shader image node
    virtual void enableTexturing(){};
    /// disableTexturing is not needed for shader image node
    virtual void disableTexturing(){};

    virtual unsigned int getImageUnit ( ){
      return image_unit;
    }
    virtual void setImageUnit ( unsigned int _image_unit ){
      image_unit = _image_unit;
    }

    virtual ~ShaderImageNode ( );
  protected:

    /// image unit used, will be used to bind to shader program
    /// the value of image_unit will be maintained in this node.
    /// Note: this is different from texture image unit which is used by
    /// texture object to be accessed in shader
    unsigned int image_unit;

    
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
