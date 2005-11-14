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
/// \file HLShape.h
/// \brief Header file for HLShape.h
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __HLSHAPE_H__
#define __HLSHAPE_H__

#include "HLObject.h"
#include <map>
#include <map>
#include <vector>

namespace H3D {

  class HLHapticsDevice;
  class X3DGeometryNode;
  /// The base class for all haptic shapes that uses HLAPI for haptic
  /// rendering.  It provides a unique HLAPI shape identifier to be
  /// used in calls to hlBeginShape.
  ///
  class H3DAPI_API HLShape: public HLObject {
  protected:
    typedef std::map< std::pair< X3DGeometryNode *, HLHapticsDevice * >,
                      unsigned int > ShapeIdMap;

    /// We maintain a database of how many times the same Node has 
    /// been used to create a HLShape. Since Nodes can be shared in
    /// several places in the scene graph it can generate several 
    /// HLShapes. HLAPI requires each of these to have a unique id.
    /// Hence we must keep track of how many times a Node that generates
    /// a HLShape appears in the scene graph. The map is from pointer to
    /// nr of times the pointer has appeared since the last call to 
    /// resetShapeIdDB(). 
    static ShapeIdMap shape_id_db ;

#ifdef HAVE_OPENHAPTICS
    typedef std::multimap< HLuint, HLShape * > HLShapeMap; 
    /// A static map from shape id to HLShape *. 
    static HLShapeMap hl_shape_map;

    /// The shape ids that are used by this HLShape.
    std::vector< HLuint > shape_ids;
#endif
  public:
    /// This function performs all the HLAPI calls that are needed to render
    /// the surface. 
    virtual void hlRender( HLHapticsDevice *hd ) = 0;
#ifdef HAVE_OPENHAPTICS
    /// Destructor. 
    virtual ~HLShape();
    
    /// Get the HLAPI shape id that is used to render this shape.
    HLuint getShapeId( HLHapticsDevice *hd );

    /// Given an shape id return the HLShape with that id.
    inline static HLShape *getHLShape( HLuint id ) {
      HLShapeMap::iterator i = hl_shape_map.find( id );
      if( i == hl_shape_map.end() ) return NULL;
      else return (*i).second;
    }
#endif
    /// Reset the shape id database, removing all previous info
    /// on how many times a Node has appeared.
    inline static void resetShapeIdDB() {
      shape_id_db.clear();
    }

    /// Given a pointer to a Node return an index indicating how many
    /// times this node has been used to generate a HLShape. Can
    /// be given as argument to X3DGeometry::getShapeId() in order to
    /// get a unique shape id regardless of how many times the node appears
    /// in the scene graph.
    inline static unsigned int getFreeShapeIdIndex( X3DGeometryNode *geom,
                                                    HLHapticsDevice *hd ) {
      ShapeIdMap::iterator i = shape_id_db.find( std::make_pair( geom, hd ) );
      if( i != shape_id_db.end() ) {
        (*i).second++;
        return (*i).second;
      } else {
        shape_id_db.insert( i, std::make_pair( std::make_pair( geom, 
                                                               hd ), 0 ) );
        return 0;
      }
    }
  };
}

#endif
