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
/// \file MultiTextureCoordinate.h
/// \brief Header file for MultiTextureCoordinate, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __MULTITEXTURECOORDINATE_H__
#define __MULTITEXTURECOORDINATE_H__

#include "X3DTextureCoordinateNode.h"
#include "MFNode.h"

namespace H3D {

  /// \class MultiTextureCoordinate
  /// \brief MultiTextureCoordinate supplies multiple texture coordinates
  /// per vertex.
  /// This node can be used to set the texture coordinates for the different
  /// texture channels.
  ///
  /// By default, if using MultiTexture with an IndexedFaceSet without a 
  /// MultiTextureCoordinate texCoord node, texture coordinates for channel
  /// 0 are replicated along the other channels. Likewise, if there are too
  /// few entries in the texCoord field, the last entry is replicated. 
  ///
  /// Example:
  /// \code
  /// Shape { 
  ///   appearance Appearance { 
  ///     texture MultiTexture {  
  ///       mode [ "MODULATE" "MODULATE" ] 
  ///       texture [ 
  ///         ImageTexture { url "brick.jpg")  
  ///         ImageTexture { repeatS FALSE repeatT FALSE url "light_gray.png"} 
  ///       ]
  ///     }
  ///   }
  ///   geometry IndexedFaceSet {  
  ///     ... 
  ///     texCoord MultiTextureCoord {
  ///       texCoord [ 
  ///         TextureCoordinate { ... } 
  ///         TextureCoordinate { ... } 
  ///       ] 
  ///     }
  ///   }
  /// }
  /// \endcode
  class H3DAPI_API MultiTextureCoordinate : public X3DTextureCoordinateNode {
  public:
    
    typedef TypedMFNode< X3DTextureCoordinateNode > MFTextureCoordinateNode;    

    /// Constructor.
    MultiTextureCoordinate( Inst< SFNode                  > _metadata = 0,
                            Inst< MFTextureCoordinateNode > _texCoord = 0 );

    /// This function all is not allowed. Use renderForTextureUnit() or 
    /// renderForTextureUnits() instead. Call will raise an exception.
    virtual void render( int index );

    /// Render the texture coordinate for the currently active texture unit.
    virtual void renderForTextureUnit( int index,
                                       unsigned int texture_unit );
    
    /// Render the texture transform for the texture units between and
    /// inluding start_unit and end_unit.
    virtual void renderForTextureUnits( int index,
                                        unsigned int start_unit,
                                        unsigned int end_unit );

    /// Returns the number of texture coordinates this node can render.
    virtual unsigned int nrAvailableTexCoords();

    /// This function all is not allowed. Use renderForTextureUnit() or 
    /// renderForTextureUnits() instead. Call will raise an exception.
    virtual void renderArray();

    /// Perform the OpenGL commands to render all texture coordinates as 
    /// an array for the given texture unit.
    virtual void renderArrayForTextureUnit( unsigned int texture_unit );
    
    /// Perform the OpenGL commands to render all texture coordinates as 
    /// an array for the texture units between and
    /// inluding start_unit and end_unit.
    virtual void renderArrayForTextureUnits( unsigned int start_unit,
                                             unsigned int end_unit );

    /// Disable the array state enabled in renderArrayForTextureUnit().
    virtual void disableArrayForTextureUnit( unsigned int texture_unit );

    /// Disable the array state enabled in renderArrayForTextureUnits().
    virtual void disableArrayForTextureUnits( unsigned int start_unit,
                                              unsigned int end_unit );

    /// The X3DTextureCoordinate nodes to use for each texture unit.
    /// 
    /// <b>Access type:</b> inputOutput \n
    auto_ptr< MFTextureCoordinateNode >  texCoord;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
    
  };
}

#endif
