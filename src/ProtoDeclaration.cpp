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
/// \file ProtoDeclaration.cpp
/// \brief cpp file for ProtoDeclaration.
///
//
//////////////////////////////////////////////////////////////////////////////


#include "ProtoDeclaration.h"
#include "X3D.h"
#include "X3DTypeFunctions.h"
#include "X3DFieldConversion.h"
#include "X3DSAX2Handlers.h"
#include "IStreamInputSource.h"

using namespace H3D;

X3DPrototypeInstance *ProtoDeclaration::newProtoInstance() { 
  try {
    PrototypeInstance *proto = new PrototypeInstance( NULL );

    for( list< FieldDeclaration >::iterator i = field_declarations.begin();
         i != field_declarations.end(); i++ ) {
      Field *f = X3DTypes::newFieldInstance( (*i).type );
      if( f ) {
        f->setOwner( proto );
        f->setName( (*i).name );
        if( (*i).value != "" ) {
          X3DTypes::X3DType type = f->getX3DType();
          if( type == X3DTypes::SFNODE || type == X3DTypes::MFNODE ) {
            try {
              if( MFNode *mfnode = dynamic_cast< MFNode * >( f ) ) {
                mfnode->push_back( X3D::createX3DNodeFromString( (*i).value ).get() );
              } else if( SFNode *sfnode = dynamic_cast< SFNode * >( f ) ) {
                sfnode->setValue( X3D::createX3DNodeFromString( (*i).value ) );
              }
            } catch( const Exception::H3DException &e ) {
              cerr << "Could not create default value for " << f->getFullName() << endl;
              cerr << e << endl;
              return NULL;
            }
          } else {
            ParsableField *pfield = 
              dynamic_cast< ParsableField * >( f );
            if( !pfield ) {
              cerr << "Cannot parse value field for\"" << f->getFullName() 
                   << "\". Field type must be a subclass of ParsableField "
                   << "in order to be parsable. " << endl;
            }
            if( (*i).access_type == Field::INITIALIZE_ONLY ||
                (*i).access_type == Field::INPUT_OUTPUT ) {
              try {
                pfield->setValueFromString( (*i).value ); 
              }
              catch( const X3D::Convert::X3DFieldConversionError &e ) {
                cerr << "Could not convert \"" 
                     << ( (*i).value.size() < 100 ? (*i).value: "value" )
                     << "\" to " << e.value << " for field \"" 
                     << f->getFullName() << "\"." << endl;
              }
              catch( const X3D::Convert::UnimplementedConversionType &e ) {
                cerr << "Field conversion error when converting value for field \"" 
                     << f->getFullName() << "\". Conversion for " << e.value 
                     << " not implemented" << endl;
              }
            } else {
              cerr << "Warning: 'value' attribute ignored. Only used if "
                   << "accesstype is initializeOnly or inputOutput " 
                   << endl;
            }
          } 
        }
        proto->addField( (*i).name, (*i).access_type, f );

      } else {
        cerr << "Warning: Invalid field type in \"field\" element. " 
             << endl;
      }
    }

    if( body == "" ) return NULL;

    auto_ptr< SAX2XMLReader > parser( X3D::getNewXMLParser() );
    X3D::X3DSAX2Handlers handler;
    handler.proto_instance = proto;
    stringstream s;
    s << body << ends;
    parser->setContentHandler(&handler);
    parser->setErrorHandler(&handler); 
    parser->parse( X3D::IStreamInputSource( s, (const XMLCh*)L"<string input>" ) );
    AutoRef< Node > n = handler.getResultingNode();
    proto->setPrototypedNode( n.get() );
    return proto;
  } catch( const Exception::H3DException &e ) {
    cerr << "Could not create X3DPrototypeInstance of " << name << endl;
    cerr << e << endl;
    return NULL;
  }
}
