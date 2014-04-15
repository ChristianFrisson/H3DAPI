//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004-2014, SenseGraphics AB
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
/// \file H3DBoundedObject.h
/// \brief Header file for H3DBoundedObject.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __H3DBOUNDEDOBJECT_H__
#define __H3DBOUNDEDOBJECT_H__

#include <H3D/H3DApi.h>
#include <H3D/FieldTemplates.h>
#include <H3D/Bound.h>
#include <H3D/RefCountSField.h>
#include <H3D/Instantiate.h>

namespace H3D {
  /// \ingroup AbstractInterface
  /// \brief This abstract interface is the base class  for all node types
  /// that include a bounding box.
  class H3DAPI_API H3DBoundedObject {
  public:
    /// The Bound object is reference counted by the field.
    class H3DAPI_API SFBound:  public RefCountSField< Bound > {
    };

    /// Constructor.
    H3DBoundedObject( Inst< SFBound > _bound = 0 ):
      bound( _bound ){
      bound->setValue( new InfiniteBound() );
      bound->setName( "bound" );
    }
    
    /// The field containing the bound object.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> InfiniteBound \n
    auto_ptr< SFBound > bound;
  };
}

#endif
