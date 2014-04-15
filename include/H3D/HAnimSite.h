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
/// \file HAnimSite.h
/// \brief Header file for HAnimSite, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __HANIMSITE_H__
#define __HANIMSITE_H__

#include <H3D/Transform.h>
#include <H3D/MFFloat.h>
#include <H3D/MFInt32.h>

namespace H3D {

  /// \ingroup X3DNodes
  /// \class HAnimSite
  /// \brief An HAnimSite node serves three purposes. The first is to define
  /// an "end effecter" location that can be used by an inverse kinematics
  /// system. The second is to define an attachment point for accessories
  /// such as jewelry and clothing. The third is to define a location for
  /// a virtual camera in the reference frame of an HAnimSegment (such as a
  /// view "through the eyes" of the humanoid for use in multi-user worlds).
  ///
  /// \note This Nodes functionality is not fully implemented.
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/BoxMan.x3d">BoxMan.x3d</a>
  ///     ( <a href="examples/BoxMan.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile HAnimSite.dot
  class H3DAPI_API HAnimSite : public Transform {
  public:
    
    // TODO
    typedef MFNode MFDisplacer;

    /// Constructor.
    HAnimSite( Inst< AddChildren >        _addChildren        = 0,
               Inst< RemoveChildren >     _removeChildren     = 0,
               Inst< SFVec3f    >         _center             = 0,
               Inst< MFChild    >         _children           = 0,
               Inst< SFNode     >         _metadata           = 0,
               Inst< SFRotation >         _rotation           = 0,
               Inst< SFVec3f    >         _scale              = 0,
               Inst< SFRotation >         _scaleOrientation   = 0,
               Inst< SFVec3f    >         _translation        = 0,
               Inst< SFBound    >         _bound              = 0,
               Inst< SFVec3f    >         _bboxCenter         = 0,
               Inst< SFVec3f    >         _bboxSize           = 0,
               Inst< SFTransformedBound > _transformedBound   = 0,
               Inst< Matrix     >         _matrix             = 0,
               Inst< SFMatrix4f >         _accumulatedForward = 0,
               Inst< SFMatrix4f >         _accumulatedInverse = 0,
               Inst< SFString    >        _name               = 0
               );

    //virtual void render();
    //virtual void traverseSG( TraverseInfo &ti );

    /// The name field contains a name  that is used for 
    /// identifying the object. 
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> "" \n
    /// 
    /// \dotfile HAnimSite_name.dot 
    auto_ptr< SFString    >  name;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
