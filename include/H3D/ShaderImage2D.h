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
/// \file ShaderImage2D.h
/// \brief Header file for ShaderImage2D, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __SHADERIMAGE2D_H__
#define __SHADERIMAGE2D_H__

#include <H3D/ShaderImageNode.h>
#include <H3D/SFString.h>
#include <H3D/SFInt32.h>
#include <H3D/SFFloat.h>

namespace H3D {

  /// \ingroup AbstractNodes
  /// \class ShaderImage2D
  /// \brief This is the base node type for the child nodes of the
  ///  ComposedShader node.
  ///
  /// 
  class H3DAPI_API ShaderImage2D :
    public ShaderImageNode {
  public:

    /// Constructor.
    ShaderImage2D ( Inst< DisplayList  > _displayList = 0,
                    Inst< SFNode       > _metadata = 0,
                    Inst< SFInt32      > _imageWidth = 0,
                    Inst< SFInt32      > _imageHeight = 0,
                    Inst< SFString     > _imageFormat = 0
  );

    //virtual void preRender();


    //virtual void postRender();


    virtual void render ( );

    ///virtual void renderImage( unsigned int texture_unit_id );
    /// set up texture and image and configure them
    virtual void prepareShaderImage ( );

    auto_ptr<SFInt32> imageWidth;

    auto_ptr<SFInt32> imageHeight;

    /// image internal format to describe how many component are there for the image
    /// and for each component, how many bits are there.
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> GL_RGBA16F \n
    auto_ptr<SFString> imageFormat;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;

  private:

    static std::map<string, GLenum> stringImageFormat_map;
    static std::map<string, GLenum> initStringImageFormat_map ( );
    static const std::vector<string> image_formats;
    static vector<string> initImage_formats ( );
    

  };
}

#endif
