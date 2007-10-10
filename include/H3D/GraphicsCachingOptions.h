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
/// \file GraphicsCachingOptions.h
/// \brief Header file for GraphicsCachingOptions.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __GRAPHICSCACHINGOPTIONS_H__
#define __GRAPHICSCACHINGOPTIONS_H__

#include <H3D/H3DOptionNode.h>
#include <H3D/SFBool.h>
#include <H3D/SFInt32.h>
#include <H3D/X3DGeometryNode.h>

namespace H3D {

  /// \ingroup H3DNodes
  /// \class GraphicsCachingOptions
  /// \brief Specifies parameters to use for graphical rendering of nodes.
  class H3DAPI_API GraphicsCachingOptions : public H3DOptionNode {
  public:
    
    /// Constructor.
    GraphicsCachingOptions( Inst< SFNode  > _metadata = 0,
                            Inst< SFBool  > _useCaching = 0,
                            Inst< SFInt32 > _cachingDelay  = 0,
                            Inst< SFBool  > _cacheOnlyGeometryNodes = 0 );
    
    bool cacheNode( Node *n ) {
      if( !useCaching->getValue() ) return false;
      if( !cacheOnlyGeometries->getValue() ) return true;

      // only X3DGeometryNodes are cached
      return dynamic_cast< X3DGeometryNode * >( n ) != NULL;
      
    }

    /// The useCaching field specifies if display lists should be used to cache
    /// OpenGL calls in order to speed up rendering of non-changing parts.
    ///
    /// <b>Default value: </b> true \n
    /// <b>Access type: </b> inputOutput \n
    auto_ptr< SFBool > useCaching;

    /// Specifies the number of frames the fields of a node that affect the 
    /// graphical rendering must be unchanged in order for a display list 
    /// cache to be built. E.g. a value of 3 means that a display list will be 
    /// build after rendering the node successfully 3 frames in a row without
    /// changes to any fields that affect the rendering.
    ///
    /// <b>Default value: </b> 3 \n
    /// <b>Access type: </b> inputOutput \n
    auto_ptr< SFInt32 > cachingDelay;


    /// If the cacheOnlyGeometries field is true only nodes that inherit
    /// from X3DGeometryNode will be cached. 
    ///
    /// <b>Default value: </b> false \n
    /// <b>Access type: </b> inputOutput \n
    auto_ptr< SFBool > cacheOnlyGeometries;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;

  protected:
    
  };
}

#endif
