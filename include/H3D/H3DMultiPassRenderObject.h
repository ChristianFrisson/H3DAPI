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
/// \file H3DMultiPassRenderObject.h
/// \brief Header file for H3DMultiPassRenderObject.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __H3DMULTIPASSRENDEROBJECT_H__
#define __H3DMULTIPASSRENDEROBJECT_H__

#include <H3D/X3DChildNode.h>
#include <H3D/X3DViewpointNode.h>

namespace H3D {
  /// \ingroup AbstractInterface
  /// \class H3DMultiPassRenderObject
  /// \brief This abstract interface is inherited by all nodes that 
  /// needs to render the scene an extra time(or more) to perform its task, 
  /// e.g. mirrors and similar effects.
  class H3DAPI_API H3DMultiPassRenderObject {
  public:

    /// Constructor.
    inline H3DMultiPassRenderObject():
      nr_times_in_scene( 0 ) {
      instances.push_back( this );
    }
    
    /// Destructor.
    virtual ~H3DMultiPassRenderObject() {
      instances.remove( this );
    }

    /// This function will be called before any viewpoint calls has been
    /// made when rendering the scene and before the scene is rendered. 
    /// Observe that the function is just called once even when stereo 
    /// is enabled. It is not called once per eye. Use renderPostViewpoint()
    /// for this.
    virtual void renderPreViewpoint( X3DChildNode *scene_root,
                                     X3DViewpointNode *vp ) {}

    /// This function will be called after the viewpoint has been
    /// set up for the scene to be rendered and before the scene is rendered.
    /// In stereo mode it will be called once for each eye.
    virtual void renderPostViewpoint( X3DChildNode *scene_root,
                                      X3DViewpointNode *vp ) {}

    /// This function will be called after the scene has been rendered. The
    /// In stereo mode it will be called once for each eye.
    virtual void renderPostScene( X3DChildNode *scene_root,
                                  X3DViewpointNode *vp) {}

    /// Traverse the scenegraph. 
    virtual void traverseSG( TraverseInfo &ti ) {
      ++nr_times_in_scene;
    }

    /// Reset the counter.
    inline void resetCounter() {
      nr_times_in_scene = 0;
    }


    /// Run renderPreViewpoint on all instances of H3DMultiPassRenderObject.
    static void renderPreViewpointAll( X3DChildNode *n,
                                       X3DViewpointNode *vp ) { 
      for( list< H3DMultiPassRenderObject * >::iterator i = instances.begin();
           i != instances.end(); ++i ) {
        if( (*i)->nr_times_in_scene > 0 )
          (*i)->renderPreViewpoint( n, vp );
      }
    }

    /// Run renderPostViewpoint on all instances of H3DMultiPassRenderObject.
    static void renderPostViewpointAll( X3DChildNode *n,
                                        X3DViewpointNode *vp ) { 
      for( list< H3DMultiPassRenderObject * >::iterator i = instances.begin();
           i != instances.end(); ++i ) {
        if( (*i)->nr_times_in_scene > 0 )
          (*i)->renderPostViewpoint( n, vp );
      }
    }

    /// Run renderPostScene on all instances of H3DMultiPassRenderObject.
    static void renderPostSceneAll( X3DChildNode *n,
                                    X3DViewpointNode *vp ) { 
      for( list< H3DMultiPassRenderObject * >::iterator i = instances.begin();
           i != instances.end(); ++i ) {
        if( (*i)->nr_times_in_scene > 0 )
          (*i)->renderPostScene( n, vp );
      }
    }

    /// Run resetCounter on all instances of H3DMultiPassRenderObject.
    static void resetCounters() { 
      for( list< H3DMultiPassRenderObject * >::iterator i = instances.begin();
           i != instances.end(); ++i ) {
        (*i)->resetCounter();
      }
    }

    /// If multi pass transparency is set to true the scene should be rendered
    /// three times graphically, once for all solid objects, once for the back
    /// side of transparent objects and once for the front face of 
    /// transparent objects. This is in order to avoid artifacts for 
    /// transparent objects when rendered in the wrong order. 
    static void setMultiPassTransparencyAll( bool b ) {
      multi_pass_transparency = b;
    }

  protected:
    // All instances of H3DMultiPassRenderObject that has been created.
    static std::list< H3DMultiPassRenderObject * > instances;

    // The number of times the node appears in the current rendered scene-graph.
    unsigned int nr_times_in_scene;

    // Flag used to know if scene should be rendered three times. See comment
    // for setMultiPassTransparency function.
    // Static since it is set just before calling renderPreViewpoint
    // and it is assumed that H3DWindowNodes are rendered sequentially (and not
    // from different threads).
    static bool multi_pass_transparency;
  };
}

#endif
