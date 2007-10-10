//////////////////////////////////////////////////////////////////////////////
//    H3D API.   Copyright 2004, Danel Evestedt, Mark Dixon
//    All Rights Reserved
//
/// \file PrototypeVector.h  Used by VRMLParser and X3D to implement
/// a PROTO declaration.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __PROTOTYPEVECTOR_H__
#define __PROTOTYPEVECTOR_H__
#include <H3D/ProtoDeclaration.h>

using namespace std;

namespace H3D {
  namespace X3D {

    class PrototypeVector: public AutoPtrVector< ProtoDeclaration > {
    public:
      PrototypeVector(): first_proto( NULL ) {}
      ProtoDeclaration *getProtoDeclaration( const string & name ) {
        for( PrototypeVector::iterator i = begin(); i != end(); i++ ) {
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
    
