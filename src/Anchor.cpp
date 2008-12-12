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
/// \file Anchor.cpp
/// \brief CPP file for Anchor, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/Anchor.h>
#include <H3D/TouchSensor.h>
#include <H3D/X3D.h>
#include <H3D/NavigationInfo.h>
#include <H3D/Scene.h>
#include <H3D/H3DNavigation.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase Anchor::database( "Anchor", 
                                 &(newInstance<Anchor>), 
                                 typeid( Anchor ),
                                 &X3DGroupingNode::database );

namespace AnchorInternals {
  FIELDDB_ELEMENT( Anchor, description, INPUT_OUTPUT );
  FIELDDB_ELEMENT( Anchor, parameter, INPUT_OUTPUT );
  FIELDDB_ELEMENT( Anchor, url, INPUT_OUTPUT );
}

static int temp_internname = 0;
Anchor::Anchor( 
             Inst< AddChildren    > _addChildren,
             Inst< RemoveChildren > _removeChildren,
             Inst< MFChild        > _children,
             Inst< SFString       > _description,
             Inst< SFNode         > _metadata,
             Inst< SFBound        > _bound,
             Inst< MFString       > _parameter,
             Inst< MFString       > _url,
             Inst< SFVec3f        > _bboxCenter,
             Inst< SFVec3f        > _bboxSize ) :
  X3DGroupingNode( _addChildren, _removeChildren, _children,
                   _metadata, _bound, _bboxCenter, _bboxSize ),
  description( _description ),
  parameter( _parameter ),
  url( _url ),
  on_click( new GeometrySelected() ),
  intern_pdsn( new TouchSensor() ) {
  
  type_name = "Anchor";
  database.initFields( this );

  on_click->setOwner( this );
  intern_pdsn->isActive->routeNoEvent( on_click );
  stringstream stm;
  stm << temp_internname;
  intern_pdsn->setName( stm.str() );
  temp_internname++;
  pt_dev_sensors.push_back( intern_pdsn.get() );
  pt_dev_sens_index[ intern_pdsn.get() ] = -1;
}

Anchor::~Anchor() {
}


void Anchor::GeometrySelected::update() {
  AutoUpdate< SFBool >::update();
  bool isActive = static_cast< SFBool * >( event.ptr )->getValue();
  if( isActive ) {
    Anchor * anchor = static_cast< Anchor * >( getOwner() );
    const vector< string > &url = anchor->url->getValue();
    if( url.empty() ) {
    }
    else {
      bool no_valid_things = true;

      for( vector<string>::const_iterator i = url.begin(); i != url.end(); i++ ) {
        X3D::DEFNodes node_names;
        string base_url;
        string vp_name = "";
        string::size_type pos = (*i).find_last_of( "#" );
        if( pos == string::npos ) {
          base_url = *i;
        } else {
          base_url = (*i).substr( 0, pos );
        }

        bool file_exist = true;
        AutoRef< Node > new_world;
        try {
          new_world.reset(
            X3D::createX3DFromURL( base_url, &node_names, NULL, NULL ) );
        } catch( const Exception::H3DException & ) {
          file_exist = false;
        }
        if( pos != string::npos )
          vp_name = (*i).substr( pos + 1, (*i).size() - pos );

        if( file_exist ) {
          X3DViewpointNode *temp_vp = 0;
          if( vp_name != "" ) {
            // if there are more than one viewpoint with vp_name results are
            // undefined.
            Node * vp_node = node_names.getNode( vp_name );
            if( vp_node ) {
              temp_vp = static_cast< X3DViewpointNode * >(vp_node);
            }
          }
          
          Anchor::replaceScene( new_world, temp_vp, anchor );
          
          no_valid_things = false;
          break;
        }
        else if( vp_name != "" ) {
          const X3DViewpointNode::ViewpointList &vp_list =
            X3DViewpointNode::getAllViewpoints();
          bool found = false;
          for( X3DViewpointNode::ViewpointList::const_iterator j = vp_list.begin();
               j != vp_list.end() ; j++ ) {
            if( vp_name == (*j)->getName() ) {
              NavigationInfo::force_jump = true;
              (*j)->set_bind->setValue( true );
              found = true;
              break;
            }
          }
          if( found ) {
            no_valid_things = false;
          }
        }
      }
      if( no_valid_things )
        Console(3) << "Warning: The url field in Anchor node "
                   << anchor->getName() << " is incorrect. "
                   << "The anchor will not be activated." << endl;
    }
  }
}

void Anchor::replaceScene( AutoRef< Node > new_world,
                           const X3DViewpointNode *new_vp,
                           const Anchor *the_anchor) {
  for( set< Scene * >::iterator i = Scene::scenes.begin();
       i != Scene::scenes.end();
       i++ ) {
    if( (*i)->isActive() ) {
      X3DGroupingNode * scene_root = dynamic_cast< X3DGroupingNode * >
        ( (*i)->sceneRoot->getValue() );
      if( scene_root && isAnchorInScene( scene_root, the_anchor ) ) {
        scene_root->children->clear();
        (*i)->sceneRoot->setValue( new_world.get() );
        if( new_vp ) {
          new_vp->set_bind->setValue( true );
        }
        else {
          const X3DViewpointNode::ViewpointList vp_list
            = X3DViewpointNode::getAllViewpoints();
          if( !vp_list.empty() ) {
            vp_list.front()->set_bind->setValue( true );
          }
        }
        H3DNavigation::enableDevice( H3DNavigation::MOUSE );
        break;
      }
    }
  }
}

bool Anchor::isAnchorInScene( const X3DGroupingNode *group_node,
                              const Anchor *anchor_to_find ) {
  NodeVector c = group_node->children->getValue();
  for( unsigned int i = 0; i < c.size(); i++ ) {
    if( c[i] == anchor_to_find )
      return true;
  }

  for( unsigned int i = 0; i < c.size(); i++ ) {
    X3DGroupingNode *temp_group = dynamic_cast< X3DGroupingNode * >( c[i] );
    if( temp_group )
      if( isAnchorInScene( temp_group, anchor_to_find ) )
        return true;
  }
  return false;
}

