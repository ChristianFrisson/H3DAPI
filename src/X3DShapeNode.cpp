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
/// \file X3DShapeNode.cpp
/// \brief CPP file for X3DShapeNode, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/X3DShapeNode.h>
#include <H3D/X3DTextureNode.h>
#include <H3D/ShadowCaster.h>
#include <H3D/ShadowTransform.h>
#include <H3D/GlobalSettings.h>

using namespace H3D;

H3DNodeDatabase X3DShapeNode::database( 
        "X3DShapeNode", 
        NULL,
        typeid( X3DShapeNode ),
        &X3DChildNode::database 
        );

bool X3DShapeNode::disable_lighting_if_no_app = true;

namespace X3DShapeNodeInternals {
  FIELDDB_ELEMENT( X3DShapeNode, appearance, INPUT_OUTPUT );
  FIELDDB_ELEMENT( X3DShapeNode, geometry, INPUT_OUTPUT );
  FIELDDB_ELEMENT( X3DShapeNode, hapticGeometry, INPUT_OUTPUT );
  FIELDDB_ELEMENT( X3DShapeNode, shadowVolume, INPUT_OUTPUT );
  FIELDDB_ELEMENT( X3DShapeNode, bboxCenter, INITIALIZE_ONLY );
  FIELDDB_ELEMENT( X3DShapeNode, bboxSize, INITIALIZE_ONLY );
}

X3DShapeNode::GeometryRenderMode X3DShapeNode::geometry_render_mode = X3DShapeNode::ALL;

X3DShapeNode::X3DShapeNode( 
                           Inst< SFAppearanceNode > _appearance,
                           Inst< SFGeometryNode   > _geometry,
                           Inst< SFHapticGeometry > _hapticGeometry,
                           Inst< SFNode           > _metadata,
                           Inst< SFBound          > _bound,
                           Inst< SFVec3f          > _bboxCenter,
                           Inst< SFVec3f          > _bboxSize,
                           Inst< DisplayList      > _displayList ) :
  X3DChildNode( _metadata ),
  X3DBoundedObject( _bound, _bboxCenter, _bboxSize ),
  H3DDisplayListObject( _displayList ),
  appearance( _appearance ),
  geometry  ( _geometry   ),
  hapticGeometry( _hapticGeometry ),
  shadowVolume( new SFShadowObjectNode ),
  use_geometry_bound( false ),
  prev_travinfoadr( NULL ),
  traverse_multipass_transparency( false ),
  last_global_settings( NULL ) {

  geometry->owner = this;

  type_name = "X3DShapeNode";
  database.initFields( this );

  displayList->setOwner( this );
  bound->setOwner( this );
  
  appearance->route( displayList );
  geometry->route( displayList );
}


void X3DShapeNode::render() {
  X3DChildNode::render();
  X3DAppearanceNode *a = appearance->getValue();
  X3DGeometryNode *g = geometry->getValue();
  
  // appearance render
  if ( a ) {
    glPushAttrib( a->getAffectedGLAttribs() );
    a->preRender();
    a->displayList->callList();
  } else {
    if( X3DShapeNode::disable_lighting_if_no_app ) {
      glPushAttrib( GL_LIGHTING_BIT );
      // setting emissive material to white in order for point
      // and light geometries to use white as color when no Material 
      // node.
      GLfloat material[] = { 1, 1, 1, 1 };
      glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION, material );
      glDisable( GL_LIGHTING );
    }
  }

  // force render mode to value determined in GlobalSettings
  // if such a value is specified.
  GlobalSettings *settings = GlobalSettings::getActive();
  if( settings ) {
    const string &render_mode = settings->renderMode->getValue();

    if( render_mode != "DEFAULT" ) {
      glPushAttrib( GL_POLYGON_BIT );

      if( render_mode == "SOLID" ) {
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
      } else if( render_mode == "WIREFRAME" ) {
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
      } else if( render_mode == "POINTS" ) {
        glPolygonMode( GL_FRONT_AND_BACK, GL_POINT );
      }
    }
  }

  // Geometry render
  if ( g ) {
    if( geometry_render_mode == ALL ) {
      g->displayList->callList();
    } else if( geometry_render_mode == SOLID ) {
      // only render non-transparent objects
      if( !a || !a->isTransparent() || !a->usingMultiPassTransparency() ) {
        g->displayList->callList();
      }
    }
    else if( a && a->isTransparent() ) {
      if( geometry_render_mode == TRANSPARENT_ONLY ) {
        g->displayList->callList();
      } else if( geometry_render_mode == TRANSPARENT_FRONT ) {
        GLenum previous_cull_face = g->getCullFace();
        bool previous_culling = g->usingCulling();
        if( !previous_culling || previous_cull_face != GL_FRONT ) { 
          g->setCullFace( GL_BACK );
          g->useCulling( true );
          g->displayList->callList();
          g->setCullFace( previous_cull_face );
          g->useCulling( previous_culling );
        }
      } else if( geometry_render_mode == TRANSPARENT_BACK ) {
        GLenum previous_cull_face = g->getCullFace();
        bool previous_culling = g->usingCulling();
        if( !previous_culling || previous_cull_face != GL_BACK ) { 
          g->setCullFace( GL_FRONT );
          g->useCulling( true );
          g->displayList->callList();
          g->setCullFace( previous_cull_face );
          g->useCulling( previous_culling );
        }
      } 
    }
  }

  // restore polygon bit
  if( settings ) {
    const string &render_mode = settings->renderMode->getValue();
    if( render_mode != "DEFAULT" ) {
      glPopAttrib();
    }
  }

  if( a ) {
    a->postRender();
    glPopAttrib();
  } else if(  X3DShapeNode::disable_lighting_if_no_app  ) {
    glPopAttrib();
  }
};

void X3DShapeNode::traverseSG( TraverseInfo &ti ) {
  if( prev_travinfoadr != &ti ) {
    prev_travinfoadr = &ti;
    traverse_multipass_transparency = false;
  }

  // set up route from the renderMode field in current GlobalSettings
  /// node since a change in the value should break the cache and 
  /// generate a new display list.
  GlobalSettings *settings = GlobalSettings::getActive();
  if( settings != last_global_settings.get() ) {
    if( last_global_settings.get() ) {
      static_cast< GlobalSettings * >(last_global_settings.get())->renderMode->unroute( displayList );
    } 
    
    if( settings ) {
      settings->renderMode->route( displayList );
    }
    last_global_settings.reset( settings );
  }

  X3DAppearanceNode *a = appearance->getValue();
  X3DGeometryNode *g = geometry->getValue();
  Node *hg = hapticGeometry->getValue();;

  if( ti.graphicsEnabled() && g && a && a->hasGeometryShadow() ) {
    // add shadow volume
    ShadowCaster *shadow_caster = NULL;
    if( !ti.getUserData( "ShadowCaster",  (void **)&shadow_caster) ) {
      
      H3DShadowObjectNode *shadow_object = shadowVolume->getValue();
      if( !shadow_object ) shadow_object = g->getShadowObject();
      ShadowTransform *t = new ShadowTransform;
      MatrixTransform *mt = new MatrixTransform;
      t->transform->setValue( mt );
      t->shadowVolume->push_back(shadow_object);
      mt->matrix->setValue( ti.getAccForwardMatrix() );
      shadow_caster->object->push_back( t ); 
    } 
  }

  if ( a ) {
    a->traverseSG( ti );
    traverse_multipass_transparency = traverse_multipass_transparency ||
        ( ti.graphicsEnabled() && a->isTransparent() &&
          a->usingMultiPassTransparency() );
    if( traverse_multipass_transparency ) {
      ti.setMultiPassTransparency( true );
      displayList->breakCache();
    }
  }

  bool * shader_requires_tangents = NULL;
  bool requires_tangents = false;

  if( !ti.getUserData( "shaderRequiresTangents", (void **)&shader_requires_tangents) ) {
    requires_tangents  = *shader_requires_tangents;
  }

  if( hg ) {
    // temporarily disable all haptics for the normal geometry
    // and only allow it to be enabled for the haptic geometry. 
    vector<bool> prev_enabled = ti.getHapticsEnabled(); 
    ti.disableHaptics();
    if ( g ) g->traverseSG( ti );
    ti.setHapticsEnabled( prev_enabled );

    // we temporarily disable tangents so they are not calculated 
    // unnecessarily by haptic geometry (when using OpenGL to find
    // haptic triangles).
    if( shader_requires_tangents )
      *shader_requires_tangents = false;
    hg->traverseSG( ti );
    if( shader_requires_tangents )
      *shader_requires_tangents = requires_tangents;
  } else {
    if ( g ) g->traverseSG( ti );
  }
  // the surface should only be available to the geometry of the shape node
  // so we remove it when the geometry has been rendered.
  ti.setCurrentSurface( NULL );

  // reset shaderRequiresTangents to false if available(set in
  // PhongShader).
  if( shader_requires_tangents ) {
    *shader_requires_tangents = false;
  }

  // Reset shaderRequiresPatches to false if available
  // Could be set when traversing the shader if a tessellation shader
  // is present
  bool* shader_requires_patches= NULL;
  if( ti.getUserData( "shaderRequiresPatches", (void **)&shader_requires_patches)==0 ) {
    *shader_requires_patches= false;
  }
}

void X3DShapeNode::DisplayList::callList( bool build_list ) {
  if( X3DShapeNode::geometry_render_mode != ALL ) {
    breakCache();
  }
  BugWorkaroundDisplayList::callList( build_list );
}

bool X3DShapeNode::lineIntersect(
                           const Vec3f &from,
                           const Vec3f &to,
                           LineIntersectResult &result ) {
  X3DGeometryNode *tmp_geom = geometry->getValue();
  if( tmp_geom )
    return tmp_geom->lineIntersect( from, to, result );
  else
    return false;
}

void X3DShapeNode::closestPoint( const Vec3f &p,
                                 NodeIntersectResult &result ) {
  X3DGeometryNode *tmp_geom = geometry->getValue();
  if( tmp_geom )
    tmp_geom->closestPoint( p, result );
}

bool X3DShapeNode::movingSphereIntersect( H3DFloat radius,
                                          const Vec3f &from, 
                                          const Vec3f &to,
                                          NodeIntersectResult &result ) {
  X3DGeometryNode *tmp_geom = geometry->getValue();
  if( tmp_geom )
    return tmp_geom->movingSphereIntersect( radius, from, to,
                                            result );
  else
    return false;
}

bool X3DShapeNode::BugWorkaroundDisplayList::haveValidDisplayList() {
  return have_valid_display_list && X3DShapeNode::geometry_render_mode == X3DShapeNode::ALL;
}
