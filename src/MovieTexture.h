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
/// \file MovieTexture.h
/// \brief Header file for MovieTexture.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __MOVIETEXTURE_H__
#define __MOVIETEXTURE_H__

#include <H3DVideoTextureNode.h>

namespace H3D {
  /// \ingroup Nodes 
  /// \class MovieTexture
  class H3DAPI_API MovieTexture : public H3DVideoTextureNode {
  public:
    
    /// Constructor.
    MovieTexture( 
                 Inst< DisplayList > _displayList = 0,
                 Inst< SFNode      > _metadata    = 0,
                 Inst< SFBool      > _repeatS     = 0,
                 Inst< SFBool      > _repeatT     = 0,
                 Inst< SFBool      > _scaleToP2   = 0,
                 Inst< SFImage     > _image       = 0,
                 Inst< SFBool      > _interpolate = 0 );

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
