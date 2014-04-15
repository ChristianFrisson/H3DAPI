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
/// \file Script.cpp
/// \brief cpp file for Script
///
//
//////////////////////////////////////////////////////////////////////////////
#include <H3D/Script.h>
#include <H3D/ResourceResolver.h>
#include <H3D/SpiderMonkeySAI.h>
#include <fstream>

using namespace H3D;

H3DNodeDatabase Script::database( 
        "Script", 
        &(newInstance< Script >),
        typeid( Script ),
        &X3DScriptNode::database 
        );

namespace ScriptInternals {
  FIELDDB_ELEMENT( Script, directOutput, INITIALIZE_ONLY );
  FIELDDB_ELEMENT( Script, mustEvaluate, INITIALIZE_ONLY );
}

Script::Script( Inst< SFNode   > _metadata,
                Inst< MFString > _url,
                Inst< SFBool   > _directOutput,
                Inst< SFBool   > _mustEvaluate ) : 
X3DScriptNode( _metadata, _url ),
directOutput( _directOutput ),
mustEvaluate( _mustEvaluate ) {
  type_name = "Script";
  database.initFields( this );
  directOutput->setValue( false );
  mustEvaluate->setValue( false );
}

Script::~Script() {
#ifdef HAVE_SPIDERMONKEY
  if( sai.isInitialized() ) {
    // When engine is uninitialized a contained SFNode field will
    // call ref and then unref twice on this Script node instance.
    // In order to not get memory leaks or other problems we simply call
    // ref here twice which means that delete is not called again on
    // this script node. see node in SpiderMonkeySAI::initializeScriptEngine
    manual_initialize = true;
    ref();
    ref();
    sai.uninitializeScriptEngine( );
  }
#endif
 // cerr << "Delete" << endl;
}


Scene::CallbackCode Script::initEngineCallback( void *data ) {
  Script *script = static_cast< Script * >( data );
#ifdef HAVE_SPIDERMONKEY
  string s = script->scriptString->getValue();
  script->sai.initializeScriptEngine( script );
  script->sai.loadScript( s, script->getURLUsed() );
#endif
  return Scene::CALLBACK_DONE;
}

void Script::initialize() {
  X3DScriptNode::initialize();
  
}

void Script::traverseSG( TraverseInfo &ti ) {
#ifdef HAVE_SPIDERMONKEY
  if( !sai.isInitialized() ) {
    Scene::addCallback( initEngineCallback, this );
    //cerr << "init" << endl;
  }
#endif
}




/// Override the addField method from H3DDynamicFieldsObject
/// to add the field to the script engine.
bool Script::addField( const string &name,
                       const Field::AccessType &access,
                       Field *field ) {
  bool b = H3DDynamicFieldsObject::addField( name, access, field );

  /// TODO: do proper access check in SpiderMonkeyTypes.cpp. For now
  /// disable so that. outputOnly fields would not work otherwise
  /// They should be settable from within the script but not from 
  /// outside.
  if( b ) {
    field->setAccessCheck( false );
  }

#ifdef HAVE_SPIDERMONKEY
  if( sai.isInitialized() ) {
    sai.addField( field );
  }
#endif
  return b;
}
