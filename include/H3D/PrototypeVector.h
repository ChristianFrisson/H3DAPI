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
/// \file PrototypeVector.h 
/// \brief Used by VRMLParser and X3D to implement a PROTO declaration.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __PROTOTYPEVECTOR_H__
#define __PROTOTYPEVECTOR_H__
#include <H3D/ProtoDeclaration.h>

using namespace std;

namespace H3D {
  namespace X3D {

    class PrototypeVector: public AutoRefVector< ProtoDeclaration > {
    public:
      PrototypeVector(): first_proto( NULL ) {}

      ProtoDeclaration *getProtoDeclaration( const string & name ) {
        for( PrototypeVector::const_iterator i = begin(); i != end(); ++i ) {
          if( (*i)->getName() == name )
            return *i;
        }
        return NULL;
      }
      inline ProtoDeclaration *getFirstProtoDeclaration() {
        return first_proto;
      }
      
      inline void setFirstProtoDeclaration( ProtoDeclaration *pd ) {
        first_proto = pd;
      }
    protected:
      ProtoDeclaration *first_proto;
    };
  }
};

#endif
    
