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
/// \file LOD.h
/// \brief Header file for LOD, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __LOD_H__
#define __LOD_H__

#include <H3D/MatrixTransform.h>
#include <H3D/SFInt32.h>
#include <H3D/MFFloat.h>


namespace H3D {

  /// \ingroup X3DNodes
  /// \class LOD
  /// \brief The LOD node specifies various levels of detail or 
  ///  complexity for a given object, and provides hints allowing 
  ///  browsers to automatically choose the appropriate version of 
  ///  the object based on the distance from the user. The children 
  ///  field contains a list of nodes that represent the same object
  ///  or objects at varying levels of detail, ordered from highest 
  ///  level of detail to the lowest level of detail. 
  ///
  ///  The range field specifies the ideal distances at which to 
  ///  switch between the levels. The forceTransitions field specifies 
  ///  whether browsers are allowed to disregard level distances in 
  ///  order to provide better performance. A forceTransitions value of
  ///  TRUE specifies that every transition should be performed regardless 
  ///  of any internal optimizations that might be available. 
  ///  A forceTransitions value of FALSE specifies that browsers are 
  ///  allowed to disregard level distances in order to provide better
  ///  performance. 
  ///
  ///  The center field is a translation offset in the local coordinate 
  ///  system that specifies the centre of the LOD node for distance 
  ///  calculations.
  ///
  ///  The number of nodes in the children field shall exceed the number
  ///  of values in the range field by one (i.e., N+1 children nodes for 
  ///  N range values). The range field contains monotonic increasing 
  ///  values that shall be greater than zero. In order to calculate 
  ///  which level to display, first the distance is calculated from the 
  ///  viewer's location, transformed into the local coordinate system of 
  ///  the LOD node (including any scaling transformations), to the center
  ///  point of the LOD node. Then, the LOD node evaluates the step function 
  ///  L(d) to choose a level for a given value of d (where d is the distance
  ///  from the viewer position to the centre of the LOD node).
  ///  



  class H3DAPI_API LOD : public MatrixTransform {
  public:
    
    /// Constructor.
    LOD(       Inst< MFChild             > _addChildren      = 0,
               Inst< MFChild             > _removeChildren   = 0,
               Inst< MFChild             > _children         = 0,
               Inst< SFNode              > _metadata         = 0,
               Inst< SFBound             > _bound            = 0,
               Inst< SFInt32             > _level_changed    = 0,
               Inst< SFVec3f             > _bboxCenter       = 0,
               Inst< SFVec3f             > _bboxSize         = 0,
               Inst< SFVec3f             > _center           = 0,    
               Inst< SFBool              > _forceTransitions = 0, 
               Inst< MFFloat             > _range            = 0 );
    
    virtual void render();

    /// Traverse the scenegraph. Saves the accumulated inverse
    /// matrix for later use when transforming the X3DViewpointNode in
    /// GLWindow.
     virtual void traverseSG( TraverseInfo &ti );


    /// The LOD node generates a level_changed 
    /// event with value i where the value of i
    /// identifies which level was activated 
    /// for display
    /// 
    /// <b>Access type:</b> outputOnly \n
    /// <b>Default value:</b> SFInt32 = 0 \n
    /// 
    /// \dotfile LOD_level_changed.dot
    auto_ptr< SFInt32 > level_changed;

    auto_ptr <SFInt32> display_index; 

    ///  The center field is a translation
    ///  offset in the local coordinate system 
    ///  that specifies the centre of the LOD
    ///  node for distance calculations.
    ///
    /// <b>Default value:</b> SFVec3f = 0 0 0 \n
    /// 
    /// \dotfile LOD_center.dot
    auto_ptr< SFVec3f > center;

    /// The forceTransitions field specifies
    /// whether browsers are allowed to disregard 
    /// level distances in order to provide better 
    /// performance.
    ///
    /// <b>Default value:</b> SFBool = FALSE \n
    /// 
    /// \dotfile LOD_forceTransitions.dot
    auto_ptr< SFBool > forceTransitions;

    /// The range field specifies the ideal 
    /// distances at which to switch between 
    /// the levels. 
    ///
    /// Each value in the range field shall 
    /// be greater than the previous value.
    /// 
    /// \dotfile LOD_range.dot
    auto_ptr< MFFloat > range;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
