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
    inline H3DMultiPassRenderObject() {
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

    /// Run renderPreViewpoint on all instances of H3DMultiPassRenderObject.
    static void renderPreViewpointAll( X3DChildNode *n,
                                       X3DViewpointNode *vp ) { 
      for( list< H3DMultiPassRenderObject * >::iterator i = instances.begin();
           i != instances.end(); i++ ) {
        (*i)->renderPreViewpoint( n, vp );
      }
    }

    /// Run renderPostViewpoint on all instances of H3DMultiPassRenderObject.
    static void renderPostViewpointAll( X3DChildNode *n,
                                        X3DViewpointNode *vp ) { 
      for( list< H3DMultiPassRenderObject * >::iterator i = instances.begin();
           i != instances.end(); i++ ) {
        (*i)->renderPostViewpoint( n, vp );
      }
    }

    /// Run renderPostScene on all instances of H3DMultiPassRenderObject.
    static void renderPostSceneAll( X3DChildNode *n,
                                    X3DViewpointNode *vp ) { 
      for( list< H3DMultiPassRenderObject * >::iterator i = instances.begin();
           i != instances.end(); i++ ) {
        (*i)->renderPostScene( n, vp );
      }
    }

  protected:
    // All instances of H3DMultiPassRenderObject that has been created.
    static std::list< H3DMultiPassRenderObject * > instances;
  };
}

#endif
