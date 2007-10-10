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
/// \file H3DImageObject.h
/// \brief Header file for H3DImageObject, X3D abstract texture scene-graph 
/// node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __H3DIMAGEOBJECT_H__
#define __H3DIMAGEOBJECT_H__

#include <H3D/X3DAppearanceChildNode.h>
#include <H3DUtil/Image.h>
#include "GL/glew.h"

namespace H3D {

  /// \ingroup AbstractInterface
  /// \class H3DImageObject
  /// \brief This abstract interface is inherited by all nodes that contains an
  /// SFImage object with an Image containing data. The data can be used
  /// for e.g. texturing or volume haptics rendering. 
  /// 
  class H3DAPI_API H3DImageObject  {
  public:
    /// The SFImage field is a field containing an Image pointer. The 
    /// pointer will be reference counted by the SFImage field.
    class H3DAPI_API SFImage: 
      public RefCountSField< Image, ParsableField > {
      virtual void setValueFromString( const string &s ) {
        setValue( X3D::X3DStringTo3DImage( s ) );
      }
    };
        
    /// Constructor.
    H3DImageObject( Inst< SFImage > _image = 0 ):
      image( _image ) {}

    /// Field containing the Image to use as a texture. 
    ///
    /// \dotfile H3DImageObject_image.dot 
    auto_ptr< SFImage > image;
        
  };
}

#endif
