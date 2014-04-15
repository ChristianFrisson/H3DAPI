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
/// \file H3DDisplayListObject.cpp
/// \brief CPP file for H3DDisplayListObject, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/H3DDisplayListObject.h>
#include <H3D/Node.h>
#include <H3D/Scene.h>
#include <H3D/GlobalSettings.h>
#include <H3D/GraphicsOptions.h>
#include <H3D/X3DGeometryNode.h>
#include <H3D/MatrixTransform.h>
#include <H3D/H3DWindowNode.h>

using namespace H3D;

auto_ptr< Field >
  H3DDisplayListObject::DisplayList::break_list_field( new Field );

void H3DDisplayListObject::DisplayList::rebuildAllDisplayLists() {
  break_list_field->touch();
}

/// Constructor
H3DDisplayListObject::DisplayList::DisplayList():
  display_list( 0 ),
  cache_mode( OPTIONS ),
  frustum_culling_mode( OPTIONS ),
  have_valid_display_list( false ),
  isActive( new IsActive ){
  initGraphicOption();
  reset_delay_cache_counter = true;
  delay_cache_counter = cachingDelay();
  isActive->setValue( true );
  isActive->setName( "H3DDisplayListObject::isActive" );
  Scene::time->routeNoEvent( isActive );

#ifndef H3D_GENERATE_DOTROUTE_FILES
  break_list_field->route( this );
#endif
}

H3DDisplayListObject::H3DDisplayListObject( 
                                       Inst< DisplayList > _displayList ) :
  displayList( _displayList ) {
    displayList->setName( "displayList" );
  }


void H3DDisplayListObject::DisplayList::update() {
  have_valid_display_list = tryBuildDisplayList( true );
  if( !have_valid_display_list ) owner->render();
  event_fields.clear();
}

bool H3DDisplayListObject::DisplayList::tryBuildDisplayList( bool cache_broken ) {
  // do not build a display list if we are not using caching.
  if( !usingCaching() ) return false;

  if( delay_cache_counter == 0 ) {
    bool have_all_needed_display_lists = childrenCachesReady( cache_broken );
  
    // create the new display list if the displayLists we are dependent 
    if( have_all_needed_display_lists ) {
      //display_list = glGenLists( 1 ); 
      glNewList( display_list, GL_COMPILE_AND_EXECUTE );
      GLuint err = glGetError();
      if( err != GL_NO_ERROR ) {
        Console(4) << "OpenGL error in glNewList() Error: \"" << gluErrorString( err ) 
                   << "\" when rendering " << getFullName() << endl;
        return false;
      }
      owner->render();
      glEndList();
      err = glGetError();
      if( err != GL_NO_ERROR ) {
        Console(4) << "OpenGL error in glEndList() Error: \"" << gluErrorString( err ) 
                   << "\" when rendering " << getFullName() << endl;
        return false;
      }
      return true;
    } else {
      return false;
    }
  } else {
    return false;
  }
}

void H3DDisplayListObject::DisplayList::propagateEvent( Event e ) {
  // this function could be called a lot of times in one frame, depending on
  // how many events finally route to this display list object. Normally, there will
  // be quite a lot, so add as less logic as possible here.
  Field::propagateEvent( e );
  have_valid_display_list = false;
  reset_delay_cache_counter = true;
  event_fields.insert( e.ptr );
  /*Console(4)<<"display list object have a event from :"<<e.ptr->getFullName()<<endl;*/
}

void H3DDisplayListObject::DisplayList::callList( bool build_list ) {
  bool was_active = isActive->getValue();
  isActive->callListCalled();

  GLuint err = glGetError();
  if( err != GL_NO_ERROR ) {
    Console(4) << "OpenGL error before H3DDisplayListObject::DisplayList::callList() Error: \"" << gluErrorString( err ) 
               << "\" when rendering parent of " << getFullName() << endl;
  }

  // if we are using frustum culling and bounding box is outside frustum
  // return.
  if( usingFrustumCulling() && isOutsideViewFrustum() ) 
    return;
  bool using_caching = usingCaching(); 

  if( using_caching && build_list ) { 
    if( !display_list ) {
      display_list = glGenLists( 1 );
    }
    // when trying to build display list, first check if need to reset
    // delay_cache_counter, no matter it is caused by break cache or propagateEvent
    if( reset_delay_cache_counter ) {
      delay_cache_counter = cachingDelay();
      reset_delay_cache_counter = false;
    }
    if( event.ptr ) {
      // will update the display list if events have occurred
      upToDate();
      return;
    } else {
      // if we do not have any event pending and still have no valid
      // display list, try to build a new one.
      if( !haveValidDisplayList() && was_active ) {
        have_valid_display_list = tryBuildDisplayList( false );
        if( have_valid_display_list ) return;
      }
    }
  } 
  
  if ( using_caching && haveValidDisplayList() ) {
    glCallList( display_list );
    GLuint err = glGetError();
    if( err != GL_NO_ERROR ) {
      Console(4) << "OpenGL error in glCallList() Error: \"" << gluErrorString( err ) 
                 << "\" when rendering " << getFullName() << endl;
      have_valid_display_list = false;
    }
  } else {
    owner->render();
    event_fields.clear();
    GLuint err = glGetError();
    if( err != GL_NO_ERROR ) {
      Console(4) << "OpenGL error in render() Error: \"" << gluErrorString( err ) 
                 << "\" when rendering " << getFullName() << endl;
    } else {
      if( delay_cache_counter > 0 ) 
        --delay_cache_counter;
    }
  }  
}

bool H3DDisplayListObject::DisplayList::childrenCachesReady( bool consider_active_field ) {
  bool have_all_needed_display_lists = true;
  // Check all the fields routed to us. If field contains a 
  // H3DDisplayListObject then we check if it is possible to build
  // a display list for this field based on the status of the 
  // DisplayList field in the H3DDisplayListObjects.
  for( FieldVector::iterator i = routes_in.begin();
       i != routes_in.end(); ++i ) {
    SFNode *sfnode = 
      dynamic_cast< SFNode* >( *i );
    DisplayList *dl = dynamic_cast< DisplayList * >( *i );
    if( sfnode || dl ) {
      if( !dl ) {
        H3DDisplayListObject *dlo = 
          dynamic_cast< H3DDisplayListObject * >( sfnode->getValue() );
        if( dlo ) {
          dl = dlo->displayList.get();
        }
      }
      if( dl ) {
        if( !dl->usingCaching() ) { 
          // child does not use caching, so it is not ready
          return false;
        } else {
          // if the cache was just broken we cannot use the isActive field 
          // any longer since a DisplayList can be activated when 
          // rebuilding cache.
          if( consider_active_field ) {
            if( !dl->haveValidDisplayList() ||
                 dl->usingFrustumCulling()) {
              have_all_needed_display_lists = false;
              break;
            }
          } else {
            if( dl->isActive->getValue() && 
                ( !dl->haveValidDisplayList() ||
                   dl->usingFrustumCulling() ) ) {
              have_all_needed_display_lists = false;
              break;
            } 
          }
        }
      }
    } else {
      MFNode *mfnode = 
        dynamic_cast< MFNode* >( *i );
      if( mfnode ) {
        for( MFNode::const_iterator n = mfnode->begin();
             n != mfnode->end(); ++n ) {
          H3DDisplayListObject *dlo = 
            dynamic_cast< H3DDisplayListObject * >( *n );
          if( dlo ) {
            if( !dlo->displayList->usingCaching() ) { 
              // child does not use caching, so it is not ready
              return false;
            } else {
              // if the cache was just broken we cannot use the isActive field 
              // any longer since a DisplayList can be activated when 
              // rebuilding cache.
              if( consider_active_field ) {
                if( !dlo->displayList->haveValidDisplayList() || 
                    dlo->displayList->usingFrustumCulling() ) {
                  have_all_needed_display_lists = false;
                  break;
                }
              } else {
                if( dlo->displayList->isActive->getValue() && 
                    (!dlo->displayList->haveValidDisplayList() ||
                      dlo->displayList->usingFrustumCulling() ) ) {
                  have_all_needed_display_lists = false;
                  break;
                } 
              }
            }
          }
        }
      }
    }
  }
  return have_all_needed_display_lists;
}

namespace H3DDisplayListObjectInternal {
  struct Plane {
    /// Returns true if the point is behind the plane.
    bool isBehind( const Vec3f &p ) {
      return a*p.x + b*p.y + c*p.z + d < 0;
    }
    
    H3DFloat a,b,c,d;
  };
}

bool H3DDisplayListObject::DisplayList::usingFrustumCulling() {
  
  if( frustum_culling_mode == ON ) return true;
  if( frustum_culling_mode == OFF ) return false;

  GraphicsOptions *options = NULL;
  H3DBoundedObject *bound_object = 
    dynamic_cast< H3DBoundedObject * >( getOwner() );

  // if not a bounded object, frustum culling cannot be used.
  if( !bound_object ) return false;

  X3DGeometryNode *geom = dynamic_cast< X3DGeometryNode * >( getOwner() );
  if( geom ) {
    geom->getOptionNode( options );
  }
  if( !options ) {
    GlobalSettings *default_settings = GlobalSettings::getActive();
    if( default_settings&&default_settings->optionNodesUpdated() ) {
      default_settings->getOptionNode( options );
      if( options ) {
        graphic_options_previous = options;
        const string &mode = options->frustumCullingMode->getValue();
        if( mode == "NO_CULLING" ) {
          return false;
        } else if( mode == "GEOMETRY" ) {
          return geom != NULL;
        } else if( mode == "ALL" ) {
          return true;
        }
      }else{
        // update global settings do not have option, return false by default
        graphic_options_previous = NULL;
        return false;
      }
    }else if( default_settings ) {// global setting unchanged
      if( graphic_options_previous!=NULL ) {
        const string &mode = graphic_options_previous->frustumCullingMode->getValue();
        if( mode == "NO_CULLING" ) {
          return false;
        } else if( mode == "GEOMETRY" ) {
          return geom != NULL;
        } else if( mode == "ALL" ) {
          return true;
        }
      }else{ // return false by default, as previous graphic setting is null
        return false;
      }
    }else{
      // no global setting, return false by default.
      graphic_options_previous = NULL;
      return false;
    }
  }
  if( options ){ 
    //geometry node has local graphic setting, use local
    const string &mode = options->frustumCullingMode->getValue();
    if( mode == "NO_CULLING" ) {
      return false;
    } else if( mode == "GEOMETRY" ) {
      return geom != NULL;
    } else if( mode == "ALL" ) {
      return true;
    }
  }
  return false;
}

bool H3DDisplayListObject::DisplayList::isOutsideViewFrustum() {
  H3DBoundedObject *bound_object = 
    dynamic_cast< H3DBoundedObject * >( getOwner() );
  if( !bound_object ) return false;

  BoxBound *box_bound = NULL;

  MatrixTransform *t = dynamic_cast< MatrixTransform *>( getOwner() );
  if( t ) {
    box_bound = 
      dynamic_cast< BoxBound * >( t->transformedBound->getValue() ); 
  } else {
    box_bound = 
      dynamic_cast< BoxBound * >( bound_object->bound->getValue() );
  }

  if( !box_bound ) return false;

  const Vec3f &half_size = box_bound->size->getValue()/2;
  const Vec3f &center = box_bound->center->getValue();

  // the eight corner points of the bounding box.
  Vec3f box_points[8];
  
  box_points[0] = center + half_size; 
  box_points[1] = center + Vec3f( -half_size.x, half_size.y, half_size.z );
  box_points[2] = center + Vec3f( half_size.x, -half_size.y, half_size.z );
  box_points[3] = center + Vec3f( -half_size.x, -half_size.y, half_size.z );
  box_points[4] = center + Vec3f( half_size.x, half_size.y, -half_size.z );
  box_points[5] = center + Vec3f( -half_size.x, half_size.y, -half_size.z );
  box_points[6] = center + Vec3f( half_size.x, -half_size.y, -half_size.z );
  box_points[7] = center - half_size;
  

  // get the current matrices
  GLfloat pm[16], mv[16];
  glGetFloatv( GL_PROJECTION_MATRIX, pm );
  glGetFloatv( GL_MODELVIEW_MATRIX, mv );
 
  // find the position of the near plane.
  Matrix4f pm_matrix( pm[0], pm[4], pm[8],  pm[12],
                      pm[1], pm[5], pm[9],  pm[13],
                      pm[2], pm[6], pm[10], pm[14],
                      pm[3], pm[7], pm[11], pm[15] );
  
  Matrix4f mv_matrix( mv[0], mv[4], mv[8],  mv[12],
                      mv[1], mv[5], mv[9],  mv[13],
                      mv[2], mv[6], mv[10], mv[14],
                      mv[3], mv[7], mv[11], mv[15] );
  Matrix4f pm_mv_matrix = pm_matrix * mv_matrix;

  // find the view frustum planes.
  H3DDisplayListObjectInternal::Plane planes[6];
  // Left clipping plane
  planes[0].a = pm_mv_matrix[3][0] + pm_mv_matrix[0][0];
  planes[0].b = pm_mv_matrix[3][1] + pm_mv_matrix[0][1];
  planes[0].c = pm_mv_matrix[3][2] + pm_mv_matrix[0][2];
  planes[0].d = pm_mv_matrix[3][3] + pm_mv_matrix[0][3];
  // Right clipping plane
  planes[1].a = pm_mv_matrix[3][0] - pm_mv_matrix[0][0];
  planes[1].b = pm_mv_matrix[3][1] - pm_mv_matrix[0][1];
  planes[1].c = pm_mv_matrix[3][2] - pm_mv_matrix[0][2];
  planes[1].d = pm_mv_matrix[3][3] - pm_mv_matrix[0][3];
  // Top clipping plane
  planes[2].a = pm_mv_matrix[3][0] - pm_mv_matrix[1][0];
  planes[2].b = pm_mv_matrix[3][1] - pm_mv_matrix[1][1];
  planes[2].c = pm_mv_matrix[3][2] - pm_mv_matrix[1][2];
  planes[2].d = pm_mv_matrix[3][3] - pm_mv_matrix[1][3];
  // Bottom clipping plane
  planes[3].a = pm_mv_matrix[3][0] + pm_mv_matrix[1][0];
  planes[3].b = pm_mv_matrix[3][1] + pm_mv_matrix[1][1];
  planes[3].c = pm_mv_matrix[3][2] + pm_mv_matrix[1][2];
  planes[3].d = pm_mv_matrix[3][3] + pm_mv_matrix[1][3];
  // Near clipping plane
  planes[4].a = pm_mv_matrix[3][0] + pm_mv_matrix[2][0];
  planes[4].b = pm_mv_matrix[3][1] + pm_mv_matrix[2][1];
  planes[4].c = pm_mv_matrix[3][2] + pm_mv_matrix[2][2];
  planes[4].d = pm_mv_matrix[3][3] + pm_mv_matrix[2][3];
  // Far clipping plane
  planes[5].a = pm_mv_matrix[3][0] - pm_mv_matrix[2][0];
  planes[5].b = pm_mv_matrix[3][1] - pm_mv_matrix[2][1];
  planes[5].c = pm_mv_matrix[3][2] - pm_mv_matrix[2][2];
  planes[5].d = pm_mv_matrix[3][3] - pm_mv_matrix[2][3];

  // for each plane check if all bounding box points are behind it.
  // if so, we can cull it.
  for( int plane = 0; plane < 6; ++plane ) {
    bool all_behind = true;
    for( int i = 0; i < 8; ++i ) {
      if( !planes[plane].isBehind( box_points[i] ) ) {
        all_behind = false;
      }
    }
    if( all_behind ) {
      //Console(4) << "Culled " << plane << endl;
      return true;
    }
  }
  return false;
}

bool H3DDisplayListObject::DisplayList::usingCaching() {
  // geometry node will be have specific usingCaching(), no geometry node condition 
  // will be considered here
  
  // if multi_pass_transparency is in use caching cannot be used
  // for anything else than geometries since the scene will be 
  // rendered thrice with different properties set and if e.g. a solid
  // object is cached it might me rendered for each of these passes
  // when it should only be rendered for one.
  if( H3DWindowNode::getMultiPassTransparency() ) {
    return false;
  }
  if( cache_mode == ON ) return true;
  if( cache_mode == OFF ) return false;

  GraphicsOptions *options = NULL;
  // as no geometry node condition will be considered, only check global option
  // setting
  GlobalSettings *default_settings = GlobalSettings::getActive();
  if( default_settings&&default_settings->optionNodesUpdated() ) {
    default_settings->getOptionNode( options );
    if( options ) {
      //Console(4)<<"global setting update, update graphic option"<<endl;
      // options exist in global setting and gs is updated
      // need to update graphic option
      graphic_options_previous = options;
      if( options->useCaching->getValue() ) {
        // for non-geometry object, caching is determined by cacheOnlyGeometry also
        return !options->cacheOnlyGeometries->getValue();
      }else{
        return false;
      }
    }else{// global setting update, but no option in global setting, return true by default
      graphic_options_previous = NULL;
      return true;
    }
  }else if( default_settings ) {// global setting exist, but not update since last frame
    if( graphic_options_previous!=NULL ) {
      return graphic_options_previous->useCaching->getValue();
    }else{ // no previous graphic options exist, return true by default.
      return true;
    }
  }else{// no default settings at all, return true by default, set previous to null
    graphic_options_previous = NULL;
    return true;
  }
}


void H3DDisplayListObject::DisplayList::breakCache() {
  have_valid_display_list = false;
  reset_delay_cache_counter = true;
  startEvent();
}

void H3DDisplayListObject::DisplayList::initGraphicOption() {
  GraphicsOptions *options = NULL;
  // if global setting exist and it contains graphic option, assign it
  // to graphic_option_previous
  GlobalSettings *default_settings = GlobalSettings::getActive();
  if( default_settings ) {
    default_settings->getOptionNode( options );
    if( options ) {
      graphic_options_previous = options;
      return;
    }
  }
  // else set previous graphic option to NULL.
  graphic_options_previous = NULL;
}

unsigned int H3DDisplayListObject::DisplayList::cachingDelay() {
  GraphicsOptions *options = NULL;

  GlobalSettings *default_settings = GlobalSettings::getActive();
  if( default_settings&&default_settings->optionNodesUpdated() ) {
    default_settings->getOptionNode( options );
    if( options ) {// update graphic options
      graphic_options_previous = options;
      //cache_delay_previous = options->cachingDelay->getValue();
      return options->cachingDelay->getValue();
    }else{
      // global setting change in last frame, but no graphic in it now
      graphic_options_previous = NULL;
      return cache_delay_default;
    }
  }
  else if( default_settings ) { 
    // global setting option node exist but not updated
    if( graphic_options_previous!=NULL ) {
      return graphic_options_previous->cachingDelay->getValue();
    }else{
      return cache_delay_default;
    }
  }else{
    // no global settings at all now
    graphic_options_previous = NULL;
    return cache_delay_default;
  }
}
