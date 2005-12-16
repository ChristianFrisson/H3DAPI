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
/// \file Inline.cpp
/// \brief CPP file for Inline, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include "Inline.h"
#include "X3D.h"
#include "X3DSAX2Handlers.h"

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase Inline::database( 
                                 "Inline", 
                                 &(newInstance<Inline>), 
                                 typeid( Inline ),
                                 &X3DChildNode::database );

namespace InlineInternals {
  FIELDDB_ELEMENT( Inline, load, INPUT_OUTPUT );
  FIELDDB_ELEMENT( Inline, url, INPUT_OUTPUT );
  FIELDDB_ELEMENT( Inline, bboxCenter, INPUT_OUTPUT );
  FIELDDB_ELEMENT( Inline, bboxSize, INPUT_OUTPUT );
  FIELDDB_ELEMENT( Inline, loadedScene, OUTPUT_ONLY );
}


Inline::Inline( 
               Inst< SFNode      > _metadata,
               Inst< DisplayList > _displayList,
               Inst< MFString    > _url,
               Inst< SFBound     > _bound,
               Inst< SFVec3f     > _bboxCenter,
               Inst< SFVec3f     > _bboxSize,
               Inst< SFBool      > _load,
               Inst< LoadedScene > _loadedScene ) :
  X3DChildNode( _metadata   ),
  X3DBoundedObject( _bound, _bboxCenter, _bboxSize ),
  X3DUrlObject( _url ),
  H3DDisplayListObject( _displayList ),
  load        ( _load       ),
  loadedScene ( _loadedScene ),
  use_union_bound( false ) {

  type_name = "Inline";
  database.initFields( this );
  displayList->setOwner( this );
  bound->setOwner( this );
  
  load->setValue( true );
  bound->setValue( new EmptyBound );

  load->route( loadedScene, id );
  url->route( loadedScene, id );
  loadedScene->route( displayList );
}

void Inline::render() {
  if( load->getValue() ) {
    for( unsigned int i = 0; i < loadedScene->size(); i++ ) {
      Group *g = loadedScene->getValueByIndex( i );
      if( g ) g->displayList->callList();
    }
  }
}

void Inline::traverseSG( TraverseInfo &ti ) {
  if( load->getValue() ) {
    for( unsigned int i = 0; i < loadedScene->size(); i++ ) {
      Group *g = loadedScene->getValueByIndex( i );
      if( g ) g->traverseSG( ti );
    }
  }
}

void Inline::LoadedScene::update() {
  Inline *inline_node = static_cast< Inline * >( getOwner() );
  value.clear();
  bool load = static_cast< SFBool * >( routes_in[0] )->getValue();
  if( load ) {
    MFString *urls = static_cast< MFString * >( routes_in[1] );
    for( MFString::const_iterator i = urls->begin(); i != urls->end(); ++i ) {
      string url = inline_node->resolveURLAsFile( *i );
      if( url != "" ) {
        try {
          Group *g = X3D::createX3DFromURL( url );
          value.push_back( g );
          inline_node->setURLUsed( *i );
        } catch( const X3D::XMLParseError &e ) {
          cerr << "Warning: Error when parsing \"" << *i << "\" in \"" 
               << getOwner()->getName() << "\" (" << e << ")." << endl;
        } 
        return;
      }
    }

    cerr << "Warning: None of the urls in Inline node with url [";
    for( MFString::const_iterator i = urls->begin(); i != urls->end(); ++i ) {  
      cerr << " \"" << *i << "\"";
    }
    cerr << "] could be loaded. "
         << "(in " << getOwner()->getName() << ")" << endl;
    inline_node->setURLUsed( "" );
  }
}

void Inline::SFBound::update() {
  value = Bound::SFBoundUnion( routes_in.begin(),
                               routes_in.end() );
}

void Inline::LoadedScene::onAdd( Node *n ) {
  LoadedSceneBase::onAdd( n );
  Group *group_node = static_cast< Group* >( n );
  Inline *inline_owner = static_cast< Inline* >( owner );
  if ( n && inline_owner->use_union_bound ) {
    X3DBoundedObject *bo = 
      dynamic_cast< X3DBoundedObject * >( group_node );
    if( bo ) {
      bo->bound->route( inline_owner->bound );
    }
  }
}

void Inline::LoadedScene::onRemove( Node *n ) {
  Group *group_node = static_cast< Group* >( n );
  Inline *inline_owner = static_cast< Inline* >( owner );
  if ( n && inline_owner->use_union_bound ) {
    X3DBoundedObject *bo = 
      dynamic_cast< X3DBoundedObject * >( group_node );
    if( bo ) {
      bo->bound->unroute( inline_owner->bound );
    }
  }
  LoadedSceneBase::onRemove( n );
}


