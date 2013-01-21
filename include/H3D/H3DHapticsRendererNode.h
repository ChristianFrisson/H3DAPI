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
/// \file H3DHapticsRendererNode.h
/// \brief Header file for H3DHapticsRendererNode
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __H3DHAPTICSRENDERERNODE_H__
#define __H3DHAPTICSRENDERERNODE_H__

// H3DApi includes
#include <H3D/H3DApi.h>
#include <H3D/Node.h>

// HAPI includes
#include <HAPI/HAPIHapticsRenderer.h>

namespace H3D {

  /// \ingroup AbstractNodes
  /// \brief Base class for all haptic renderers, i.e. algorithms for 
  /// generating forces based on geometries. 
  class H3DAPI_API H3DHapticsRendererNode: public Node {
  public:

    /// Constructor.
    H3DHapticsRendererNode() {
    }
    
    /// Get the haptics renderer to use for a certain layer.
    virtual HAPI::HAPIHapticsRenderer *
    getHapticsRenderer( unsigned int layer ) {
      if( renderers.size() < layer + 1 ) {
        renderers.resize( layer + 1, NULL );
      }
      if( !renderers[layer] )
        renderers[layer] = getNewHapticsRenderer();
      
      return renderers[layer];
    }

    /// This function should only be called if the HAPI::HAPIHapticsRenderer
    /// obtained for the given layer is removed from the HAPIDevice ( and
    /// therefore destroyed ).
    /// The reason for having this function is to not accidently leave
    /// a pointer to the destroyed instance in this node in case the node
    /// will be used again or elsewhere.
    virtual void hapticsRendererRemoved( unsigned int layer ) {
      if( layer < renderers.size() ) {
        renderers[layer] = NULL;
      }
    }


    /// Returns the default containerField value for this node. In this
    /// case "hapticsRenderer"
    virtual string defaultXMLContainerField() {
      return "hapticsRenderer";
    }
    
  protected:
    /// Create a new HAPIHapticsRenderer to use.
    virtual HAPI::HAPIHapticsRenderer *getNewHapticsRenderer() = 0;
    vector< HAPI::HAPIHapticsRenderer * > renderers;
  };
}

#endif
