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
/// \file SAIFunctions.cpp
/// \brief Source file for common SAI interface classes and functions
/// for use by script engine implementations such as SpiderMonkeySAI.
///
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/SAIFunctions.h>

//#include <H3D/Console.h>
#include <H3D/Scene.h>
#include <H3D/X3DGroupingNode.h>
#include <H3D/ResourceResolver.h>

using namespace H3D;
using namespace SAI;

list< ExecutionContext * > ExecutionContext::instances;

Browser::Browser( Scene *s ) :
  name( "Unnamed H3D browser" ),
  version( "Unknown" ),
  SAI_scene( new SAIScene ),
  scene( s ) {
}

/// The getCurrentSpeed service returns the navigation speed 
/// of the current world. The current speed is the average 
/// navigation speed for the currently bound NavigationInfo 
/// node of the active layer in metres per second in the coordinate
/// system of the currently bound viewpoint.
H3DFloat Browser::getCurrentSpeed() { 
  // TODO
  return 0; 
}
    
/// The getCurrentFrameRate service returns the current frame 
/// display rate of the browser. If this is not supported, the
/// value returned is zero.
H3DFloat Browser::getCurrentFrameRate() { 
  // TODO
  return 0; 
}

const vector< ProfileInfo > &Browser::getSupportedProfiles() {
  // TODO
  throw SAIError( SAIError::SAI_NOT_SUPPORTED );
}

const ProfileInfo &Browser::getProfile( const string &profile ) {
  // TODO
  throw SAIError( SAIError::SAI_NOT_SUPPORTED );
}

const vector< ComponentInfo > &Browser::getSupportedComponents() {
  // TODO
  throw SAIError( SAIError::SAI_NOT_SUPPORTED );
}

const ComponentInfo &Browser::getComponent( const string &component,
                                            H3DUInt32 level ) {
  // TODO
  throw SAIError( SAIError::SAI_NOT_SUPPORTED );
}

ExecutionContext *Browser::getExecutionContext() {
  // TODO

  if( Scene::scenes.size() > 0 ) {
    Node *n = (*Scene::scenes.begin())->sceneRoot->getValue();
    X3DGroupingNode *root = 
      dynamic_cast< X3DGroupingNode * >((*Scene::scenes.begin())->sceneRoot->getValue());
    if( root ) {
      ExecutionContext *ec = new ExecutionContext;
      ec->root_node.reset( root );
      return ec;
    }
  } 

  return new ExecutionContext;

}

SAIScene *Browser::createScene( ProfileInfo *pi,
                             const vector< ComponentInfo > &cis ) {
  // TODO
  throw SAIError( SAIError::SAI_NOT_SUPPORTED );
  return NULL;
}

void Browser::replaceWorld( SAIScene *new_scene ) {
  // TODO: check spec. this is a simple implementation. 
  // need to do shutdown and initialize events
  SAI_scene.reset( new_scene );
  if( new_scene ) {
    scene->sceneRoot->setValue( SAI_scene->root_node );
  } else {
    scene->sceneRoot->setValue( NULL );
  }
}

SAIScene *Browser::importDocument( DOMNode *node ) {
  // TODO
  throw SAIError( SAIError::SAI_NOT_SUPPORTED );
  return NULL;
}


void Browser::loadURL( const string &url,
                       const vector< string > &property_list  ) {
  // TODO
  throw SAIError( SAIError::SAI_NOT_SUPPORTED );
}


void Browser::setDescription( const string &description ) {
  // TODO
  throw SAIError( SAIError::SAI_NOT_SUPPORTED );
}

SAIScene *Browser::createX3DFromString( const string &s ) {
  SAIScene *scene = new SAIScene;
  try {
    scene->root_node.reset( X3D::createX3DFromString( s, 
                                                      &scene->named_nodes, 
                                                      &scene->exported_nodes, 
                                                      &scene->protos ) );
    return scene;
  } catch( const Exception::H3DException &e ) {
    delete scene;
    throw SAIError( SAIError::SAI_INVALID_X3D, e.message );
  }
  return NULL;
}

SAIScene *Browser::createX3DFromStream( const istream &s ) {
  // TODO
  throw SAIError( SAIError::SAI_NOT_SUPPORTED );
  return NULL;
}

SAIScene *Browser::createX3DFromURL( MFString *urls ) {
  SAIScene *scene = new SAIScene;
  
  for( MFString::const_iterator i = urls->begin();
       i != urls->end(); i++ ) {
    try {
      scene->root_node.reset( X3D::createX3DFromURL( *i, 
                                                     &scene->named_nodes, 
                                                     &scene->exported_nodes, 
                                                     &scene->protos ) );
      
      return scene;
    } catch( Exception::H3DException &e ) {
      // If there was an exception while reading a file, we check if the
      // file exists. If it does not exist we continue by trying the next file
      // in the MFString. If it did exist, there is something wrong in the 
      // file itself so we throw an exception reflecting this.
      bool is_tmp_file = false;
      string resolved_url = ResourceResolver::resolveURLAsFile( *i, 
                                                                &is_tmp_file );
      if( is_tmp_file ) 
        ResourceResolver::releaseTmpFileName( resolved_url );
      
      if( resolved_url != "" ) {
        delete scene;
        throw SAIError( SAIError::SAI_INVALID_URL, e.message );
      }
    }
  }

  delete scene;
  throw SAIError( SAIError::SAI_URL_UNAVAILABLE );
}

// TODO
    //void updateControl( 
    //    registerBrowserInterest


const vector< string > &Browser::getRenderingProperties() {
  throw SAIError( SAIError::SAI_NOT_SUPPORTED );
  // TODO

}

const vector< string > &Browser::getBrowserProperties() {
  // TODO
  throw SAIError( SAIError::SAI_NOT_SUPPORTED );
}

void Browser::nextViewpoint( H3DInt32 layer  ) {
  // TODO
  throw SAIError( SAIError::SAI_NOT_SUPPORTED );
}
 
void Browser::previousViewpoint( H3DInt32 layer  ) {
  // TODO
  throw SAIError( SAIError::SAI_NOT_SUPPORTED );
}

void Browser::firstViewpoint( H3DInt32 layer  ) {
  // TODO
  throw SAIError( SAIError::SAI_NOT_SUPPORTED );
}

void Browser::lastViewpoint( H3DInt32 layer ) {
  // TODO
  throw SAIError( SAIError::SAI_NOT_SUPPORTED );
}

void Browser::print( const string &s ) {
  Console( 4 ) << s;
  Console.flush();
}

void Browser::dispose() {
  // TODO
  throw SAIError( SAIError::SAI_NOT_SUPPORTED );
}

    /// TODO?:
    /// The setBrowserOption service allows setting options defined in 9.2.5 
    /// Browser options in ISO/IEC 19775-1. The name field shall be one of 
    /// the defined names in Table 9.2 in ISO/IEC 19775-1. This service shall
    /// return an SAIBoolean value indicating whether the change request 
    /// was successful. A browser is not required to support dynamic changes
    /// to any options. If a browser option is not supported, a value 
    /// of FALSE shall be returned.
    /// bool setBrowserOption( const string &


Node *ExecutionContext::createNode( const string &node_name ){
  // TODO: check profile and components
  // throw exception of not found/
  return H3DNodeDatabase::createNode( node_name );
}

MFNode *ExecutionContext::getRootNodes(){
  return root_node->children.get();
}
