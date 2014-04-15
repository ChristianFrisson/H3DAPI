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
/// \file ProtoDeclaration.cpp
/// \brief cpp file for ProtoDeclaration.
///
//
//////////////////////////////////////////////////////////////////////////////


#include <H3D/ProtoDeclaration.h>
#include <H3D/PrototypeVector.h>
#include <H3D/X3D.h>
#include <H3D/X3DTypeFunctions.h>
#include <H3D/X3DFieldConversion.h>
#ifdef HAVE_XERCES
#include <H3D/X3DSAX2Handlers.h>
#endif
#include <H3D/IStreamInputSource.h>
#include <H3D/VrmlDriver.h>
#include <H3D/VrmlParser.h>


using namespace H3D;

ProtoDeclaration::ProtoDeclaration( const string &_name,
                                    const string &_body,
                                    const vector<string > &_body_extra,
                                    PrototypeVector *_existing_protos ) :
      name( _name ),
      existing_protos( NULL ),
      body( _body ),
      body_extra( _body_extra ) {
  if( _existing_protos ) {
    existing_protos = new PrototypeVector;
    for( PrototypeVector::const_iterator i = _existing_protos->begin();
         i != _existing_protos->end(); ++i ) {
      if( (*i)->name != name )
        existing_protos->push_back( *i );
    }
    if( _existing_protos->getFirstProtoDeclaration() &&
        _existing_protos->getFirstProtoDeclaration()->name != name ) {
      existing_protos->setFirstProtoDeclaration( _existing_protos->getFirstProtoDeclaration() );
    }
  }
}

ProtoDeclaration::~ProtoDeclaration() {
  if( existing_protos ) {
    delete existing_protos;
    existing_protos = NULL;
  }
}

X3DPrototypeInstance *ProtoDeclaration::newProtoInstance() { 
  PrototypeInstance *proto = new PrototypeInstance( NULL );
  try {

    for( list< FieldDeclaration >::iterator i = field_declarations.begin();
         i != field_declarations.end(); ++i ) {
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
              Console(3) << "Could not create default value for " << f->getFullName() << endl;
              Console(3) << e << endl;
            }
          } else {
            ParsableField *pfield = 
              dynamic_cast< ParsableField * >( f );
            if( !pfield ) {
              Console(3) << "Cannot parse value field for\"" 
                         << f->getFullName() 
                         << "\". Field type must be a subclass of ParsableField "
                         << "in order to be parsable. " << endl;
            }
            if( (*i).access_type == Field::INITIALIZE_ONLY ||
                (*i).access_type == Field::INPUT_OUTPUT ||
                (*i).access_type == Field::INPUT_ONLY ) {
              try {
                pfield->setValueFromString( (*i).value ); 
              }
              catch( const X3D::Convert::X3DFieldConversionError &e ) {
                Console(3) << "Could not convert \"" 
                     << ( (*i).value.size() < 100 ? (*i).value: (string)"value" )
                     << "\" to " << e.value << " for field \"" 
                     << f->getFullName() << "\"." << endl;
              }
              catch( const X3D::Convert::UnimplementedConversionType &e ) {
                Console(3) << "Field conversion error when converting value for field \"" 
                     << f->getFullName() << "\". Conversion for " << e.value 
                     << " not implemented" << endl;
              }
            } else {
              Console(3) << "Warning: 'value' attribute ignored. Only used if "
                   << "accesstype is initializeOnly, inputOnly or inputOutput " 
                   << endl;
            }
          } 
        }
        proto->addField( (*i).name, (*i).access_type, f );

      } else {
        Console(3) << "Warning: Invalid field type in \"field\" element. " 
                   << endl;
      }
    }

    if( body == "" ) return NULL;
    X3D::DEFNodes dn;

    if ( X3D::isVRML( body ) ) {
      // parse and set the main prodo body node. 
      AutoRef<Node> n = createProtoInstanceNodeVRML( proto, &dn, body );
      proto->setPrototypedNode( n.get() );
    } else {
      // parse and set the main prodo body node.
      AutoRef<Node> n = createProtoInstanceNodeX3D( proto, &dn, body );
      string proto_declare_tag = "<ProtoDeclare";
      bool set_body_instead = !n.get() && body.substr( 0, proto_declare_tag.size() ) == proto_declare_tag;
      if( !set_body_instead ) {
        proto->setPrototypedNode( n.get() );
      }
      
      // parse and set any extra nodes from the proto body
      for( unsigned int i = 0; i < body_extra.size(); ++i ) {
        if( !body_extra[i].empty() ) {
          AutoRef<Node> n = createProtoInstanceNodeX3D( proto, &dn, body_extra[i] );
          if( n.get() ) {
            if( set_body_instead )
              proto->setPrototypedNode( n.get() );
            else
              proto->addPrototypedNodeExtra( n.get() );
          }
        }
      }
    }

    return proto;
    
  } catch( const Exception::H3DException &e ) {
    delete proto;
    Console(3) << "Could not create X3DPrototypeInstance of " << name << endl;
    Console(3) << e << endl;
    return NULL;
  }
}

AutoRef< Node > ProtoDeclaration::createProtoInstanceNodeVRML( PrototypeInstance *proto,
                                                               X3D::DEFNodes *dn,
                                                               const string &body ) {
  AutoRef< Node > n;
  stringstream s;
  s << body;
  VrmlDriver driver;
  driver.proto_instance = proto;
  if (driver.parse( &s, "<proto>", dn, NULL, NULL )) {
    Group *c = driver.getRoot();
    if ( c && !c->children->empty() ) {
      n.reset( c->children->front() );
      for( unsigned int i = 1; i < c->children->size(); ++i ) {
        if( c->children->getValueByIndex( i ) )
          proto->addPrototypedNodeExtra( c->children->getValueByIndex(i) );
      }
    }
  } else {
    Console(3) << "Warning: Could not parse VRML from string" << endl;
  }
  return n;
}

AutoRef< Node > ProtoDeclaration::createProtoInstanceNodeX3D( PrototypeInstance *proto,
                                                              X3D::DEFNodes *dn,
                                                              const string &body ) {
#ifdef HAVE_XERCES
  auto_ptr< SAX2XMLReader > parser( X3D::getNewXMLParser() );
  X3D::X3DSAX2Handlers handler(dn, NULL, existing_protos, true );
  handler.proto_instance = proto;
  stringstream s;
  s << body;
  parser->setContentHandler(&handler);
  parser->setErrorHandler(&handler); 
  parser->setLexicalHandler( &handler );
  parser->parse( X3D::IStreamInputSource( s, (const XMLCh*)L"<string input>" ) );
  return handler.getResultingNode();
#else
  return AutoRef< Node >();
#endif
}
