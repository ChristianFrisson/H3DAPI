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
/// \file H3DDisplayListObject.cpp
/// \brief CPP file for H3DDisplayListObject, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3DDisplayListObject.h>
#include <Node.h>
#include <Scene.h>
#include <GlobalSettings.h>
#include <GraphicsCachingOptions.h>
#include <X3DGeometryNode.h>

using namespace H3D;

auto_ptr< Field > H3DDisplayListObject::DisplayList::break_list_field( new Field );

void H3DDisplayListObject::DisplayList::rebuildAllDisplayLists() {
  break_list_field->touch();
}

/// Constructor
H3DDisplayListObject::DisplayList::DisplayList():
  display_list( 0 ),
  cache_mode( OPTIONS ),
  have_valid_display_list( false ),
  isActive( new IsActive ){

  delay_cache_counter = cachingDelay();
  isActive->setValue( true );
  Scene::time->routeNoEvent( isActive );

  break_list_field->route( this );
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
  if( delay_cache_counter == 0 ) {
    bool have_all_needed_display_lists = childrenCachesReady( cache_broken );
  
    // create the new display list if the displayLists we are dependent 
    if( have_all_needed_display_lists ) {
      //display_list = glGenLists( 1 ); 
      glNewList( display_list, GL_COMPILE_AND_EXECUTE );
      GLuint err = glGetError();
      if( err != GL_NO_ERROR ) {
        Console(4) << "QpenGL error in glNewList() Error: \"" << gluErrorString( err ) 
                   << "\" when rendering " << getFullName() << endl;
        return false;
      }
      owner->render();
      glEndList();
      err = glGetError();
      if( err != GL_NO_ERROR ) {
        Console(4) << "QpenGL error in glEndList() Error: \"" << gluErrorString( err ) 
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
  Field::propagateEvent( e );
  have_valid_display_list = false;
  delay_cache_counter = cachingDelay();
  event_fields.insert( e.ptr );
}

void H3DDisplayListObject::DisplayList::callList( bool build_list ) {
  isActive->callListCalled();

  GLuint err = glGetError();
  if( err != GL_NO_ERROR ) {
    Console(4) << "OpenGL error before H3DDisplayListObject::DisplayList::callList() Error: \"" << gluErrorString( err ) 
               << "\" when rendering parent of " << getFullName() << endl;
  }

  bool using_caching = usingCaching(); 

  if( using_caching && build_list ) { 
    if( !display_list ) {
      display_list = glGenLists( 1 );
    }

    if( event.ptr ) {
      // will update the display list if events have occured
      upToDate();
      return;
    } else {
      // if we do not have any event pending and still have no valid
      // display list, try to build a new one.
      if( !haveValidDisplayList() ) {
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
     	  delay_cache_counter--;
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
       i != routes_in.end(); i++ ) {
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
          if( !dl->childrenCachesReady( consider_active_field ) ) {
            have_all_needed_display_lists = false;
            break;
          }
        } else {
          // if the cache was just broken we cannot use the isActive field 
          // any longer since a DisplayList can be activated when 
          // rebuilding cache.
          if( !consider_active_field ) {
            if( !dl->haveValidDisplayList() ) {
              have_all_needed_display_lists = false;
              break;
            }
          } else {
            if( dl->isActive->getValue() && 
                !dl->haveValidDisplayList() ) {
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
             n != mfnode->end(); n++ ) {
          H3DDisplayListObject *dlo = 
            dynamic_cast< H3DDisplayListObject * >( *n );
          if( dlo ) {
            if( !dlo->displayList->usingCaching() ) { 
              if( !dlo->displayList->childrenCachesReady( consider_active_field ) ) {
                have_all_needed_display_lists = false;
                break;
              }
            } else {
              // if the cache was just broken we cannot use the isActive field 
              // any longer since a DisplayList can be activated when 
              // rebuilding cache.
              if( consider_active_field ) {
                if( !dlo->displayList->haveValidDisplayList() ) {
                  have_all_needed_display_lists = false;
                  break;
                }
              } else {
                if( dlo->displayList->isActive->getValue() && 
                    !dlo->displayList->haveValidDisplayList() ) {
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


bool H3DDisplayListObject::DisplayList::usingCaching() {
  if( cache_mode == ON ) return true;
  if( cache_mode == OFF ) return false;

  GraphicsCachingOptions *options = NULL;
  X3DGeometryNode *geom = dynamic_cast< X3DGeometryNode * >( getOwner() );
  if( geom ) {
    geom->getOptionNode( options );
  }
  if( !options ) {
    GlobalSettings *default_settings = GlobalSettings::getActive();
    if( default_settings ) {
      default_settings->getOptionNode( options );
    }
  }

  if( options ) {
    if( options->useCaching->getValue() ) {
      if( options->cacheOnlyGeometries->getValue() ) {
        return geom != NULL;
      } else {
        return true;
      } 
    } else {
      return false;
    }
  } else
    return true;
}


void H3DDisplayListObject::DisplayList::breakCache() {
  have_valid_display_list = false;
  delay_cache_counter = cachingDelay();
  startEvent();
}

unsigned int H3DDisplayListObject::DisplayList::cachingDelay() {
  GraphicsCachingOptions *options = NULL;
  X3DGeometryNode *geom = dynamic_cast< X3DGeometryNode * >( getOwner() );
  if( geom ) {
    geom->getOptionNode( options );
  }
  if( !options ) {
    GlobalSettings *default_settings = GlobalSettings::getActive();
    if( default_settings ) {
      default_settings->getOptionNode( options );
    }
  }
  
  if( options ) {
    return options->cachingDelay->getValue();
  }
  
  return 3;
}
