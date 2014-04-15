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
/// \file GeometryGroup.cpp
/// \brief CPP file for GeometryGroup, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/GeometryGroup.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase GeometryGroup::database( 
                                 "GeometryGroup", 
                                 &(newInstance<GeometryGroup>), 
                                 typeid( GeometryGroup ),
                                 &X3DGeometryNode::database );

namespace GeometryGroupInternals {
  FIELDDB_ELEMENT( GeometryGroup, geometry, INPUT_OUTPUT );
}

GeometryGroup::GeometryGroup( Inst< SFNode > _metadata,
                              Inst< SFBound > _bound,
                              Inst< DisplayList > _displayList,
                              Inst< IsTouched   > _isTouched,
                              Inst< MFVec3fPerIndex > _force,
                              Inst< MFVec3fPerIndex > _contactPoint,
                              Inst< MFVec3fPerIndex > _contactNormal,
                              Inst< MFVec3fPerIndex > _contactTexCoord,
                              Inst< SFBoundTree > _boundTree,
                              Inst< MFGeometry > _geometry ) :
  X3DGeometryNode( _metadata, _bound, _displayList, _isTouched,
                   _force, _contactPoint, _contactNormal,
                   _contactTexCoord, _boundTree ),
  geometry  ( _geometry ) {

  type_name = "GeometryGroup";
  database.initFields( this );

  geometry->route( displayList );
}

int GeometryGroup::nrTriangles() {
  int nr_triangles = 0;
  const NodeVector &geometries = geometry->getValue();
  for( unsigned int i = 0; i < geometries.size(); ++i ) {
    if( geometries[i] ) {
      nr_triangles += static_cast< X3DGeometryNode * >(geometries[i])->nrTriangles();
    }
  }
  return nr_triangles;
}

void GeometryGroup::render() {
  const NodeVector &geometries = geometry->getValue();
  for( unsigned int i = 0; i < geometries.size(); ++i ) {
    if( geometries[i] ) {
      static_cast< X3DGeometryNode * >(geometries[i])->displayList->callList();
    }
  }
}

void GeometryGroup::traverseSG( TraverseInfo &ti ) {
  const NodeVector &geometries = geometry->getValue();
  for( unsigned int i = 0; i < geometries.size(); ++i ) {
    if( geometries[i] ) {
      geometries[i]->traverseSG(ti);
    }
  }
}

bool GeometryGroup::lineIntersect(
                  const Vec3f &from, 
                  const Vec3f &to,    
                  LineIntersectResult &result ) {
  bool intersect = false;
  Bound * the_bound = bound->getValue();
  if( !the_bound ||
      the_bound->lineSegmentIntersect( from, to ) ) {
    const NodeVector &geometries = geometry->getValue();
    for( unsigned int i = 0; i < geometries.size(); ++i ) {
      if( geometries[i] &&
          geometries[i]->lineIntersect( from,
                                        to,
                                        result ) ) {
          intersect = true;
      }
    }
  }
  return intersect;
}

void GeometryGroup::closestPoint( const Vec3f &p,
                           NodeIntersectResult &result ) {
  const NodeVector &geometries = geometry->getValue();
  for( unsigned int i = 0; i < geometries.size(); ++i ) {
    if( geometries[i] )
      geometries[i]->closestPoint( p, result );
  }
}

bool GeometryGroup::movingSphereIntersect( H3DFloat radius,
                                    const Vec3f &from, 
                                    const Vec3f &to,
                                    NodeIntersectResult &result ) {
  Bound * the_bound = bound->getValue();
  if( !the_bound || the_bound->movingSphereIntersect( from, to, radius ) ) {
    const NodeVector &geometries = geometry->getValue();
    bool hit = false;
    for( unsigned int i = 0; i < geometries.size(); ++i ) {
      if( geometries[i] &&
          geometries[i]->movingSphereIntersect( radius, from, to, result ))
        hit = true;
    }
    return hit;
  }
  return false;
}

bool GeometryGroup::supportsTangentAttributes() {
  const NodeVector &geometries = geometry->getValue();
  for( unsigned int i = 0; i < geometries.size(); ++i )
    if( geometries[i] && static_cast< X3DGeometryNode * >(geometries[i])->
                          supportsTangentAttributes())
        return true;
  return false;
}

void GeometryGroup::glRender() {
  const NodeVector &geometries = geometry->getValue();
  for( unsigned int i = 0; i < geometries.size(); ++i )
    if( geometries[i] )
      static_cast< X3DGeometryNode * >(geometries[i])->glRender();
}

void GeometryGroup::MFGeometry::onAdd( Node *n ) {
  MFGeometryBase::onAdd( n );
  GeometryGroup *o = static_cast< GeometryGroup* >( owner );
  if ( n ) {
    X3DGeometryNode *geom = static_cast< X3DGeometryNode * >(n);
    geom->bound->route( o->bound );
    geom->isTouched->route( o->isTouched, o->id );
  }
}

void GeometryGroup::MFGeometry::onRemove( Node *n ) {
  GeometryGroup *o = static_cast< GeometryGroup* >( owner );
  if ( n ) {
    X3DGeometryNode *geom = static_cast< X3DGeometryNode * >(n);
    geom->bound->unroute( o->bound );
    geom->isTouched->unroute( o->isTouched );
    // Removing these routes in case they exist.
    geom->force->unroute( o->force );
    geom->contactPoint->unroute( o->contactPoint );
    geom->contactNormal->unroute( o->contactNormal );
    geom->contactTexCoord->unroute( o->contactTexCoord );
  }
  MFGeometryBase::onRemove( n );
}

void GeometryGroup::SFBound::update() {
  value = Bound::SFBoundUnion( routes_in.begin(),
                               routes_in.end() );
}

void GeometryGroup::IsTouched::update() {
  vector< int > route_index;
  value.clear();
  GeometryGroup *o = static_cast< GeometryGroup* >( owner );
  FieldVector force_routes_in = o->force->getRoutesIn();
  const NodeVector &geometries = o->geometry->getValue();
  bool update_field_routes = false;
  for( unsigned int i = 0; i < routes_in.size(); ++i ) {
    const vector< bool > &v = static_cast< MFBool *>(routes_in[i])->getValue();
    if( value.size() < v.size() ) {
      value.resize( v.size(), false );
      route_index.resize( v.size(), i );
    }
    for( unsigned int j = 0; j < v.size(); j++ ) {
      if( !value[j] && v[j] ) {
        value[j] = true;
        route_index[j] = i;
        if( i >= force_routes_in.size() ||
            static_cast< X3DGeometryNode * >(geometries[i])->force.get() !=
            force_routes_in[i] )
          update_field_routes = true;
      }
    }
    // Even if every value in the vector is true we can't exit the loop early
    // in case any of the routes that is later in the list have a isTouched field
    // which is bigger than the current size.
  }

  if( update_field_routes ) {
    static_cast< MFVec3fPerIndex *>(o->force.get())->unrouteAllFrom();
    static_cast< MFVec3fPerIndex *>(o->contactPoint.get())->unrouteAllFrom();
    static_cast< MFVec3fPerIndex *>(o->contactNormal.get())->unrouteAllFrom();
    static_cast< MFVec3fPerIndex *>(o->contactTexCoord.get())->unrouteAllFrom();
    for( unsigned int i = 0; i < route_index.size(); ++i ) {
      X3DGeometryNode *geom =
       static_cast< X3DGeometryNode * >(geometries[route_index[i]]);
      if( i == route_index.size() - 1 ) {
        geom->force->route( o->force, o->id );
        geom->contactPoint->route( o->contactPoint, o->id );
        geom->contactNormal->route( o->contactNormal, o->id );
        geom->contactTexCoord->route( o->contactTexCoord, o->id );
      } else {
        geom->force->routeNoEvent( o->force, o->id );
        geom->contactPoint->routeNoEvent( o->contactPoint, o->id );
        geom->contactNormal->routeNoEvent( o->contactNormal, o->id );
        geom->contactTexCoord->routeNoEvent( o->contactTexCoord, o->id );
      }
      Console(3) << "index i: " << route_index[i] << endl;
    }
  }
}

void GeometryGroup::MFVec3fPerIndex::unrouteAllFrom() {
  if( !routes_in.empty() ) {
    int prev_routes_in_size;
    do {
      prev_routes_in_size = routes_in.size();
      routes_in.front()->unroute( this );
    } while( !routes_in.empty() || prev_routes_in_size == routes_in.size() );
  }
}

void GeometryGroup::MFVec3fPerIndex::update() {
  value.resize( routes_in.size() );
  for( unsigned int i = 0; i < routes_in.size(); ++i )
    value[i] = static_cast< MFVec3f * >(routes_in[i])->getValueByIndex(i);
}
