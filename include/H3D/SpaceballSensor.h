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
/// \file SpaceballSensor.h
/// \brief Header file for SpaceballSensor
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __SPACEBALLSENSOR_H__
#define __SPACEBALLSENSOR_H__


#include <H3D/X3DSensorNode.h>
#include <list>

#include <H3D/SFVec3f.h>
#include <H3D/MFBool.h>



namespace H3D {

  /// \ingroup Nodes
  /// \class SpaceballSensor
  /// \brief A sensor node for reading GLUT Spaceball values. 
  ///
  /// 
  ///
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/SpaceballSensor.x3d">SpaceballSensor.x3d</a>
  ///     ( <a href="examples/SpaceballSensor.x3d.html">Source</a> )
  class H3DAPI_API SpaceballSensor : public X3DSensorNode {
  public:

    /// Constructor.
    SpaceballSensor( Inst< SFBool  > _enabled   = 0,
                     Inst< SFNode  > _metadata  = 0,
                     Inst< SFBool  > _isActive  = 0,
                     Inst< SFVec3f > _motion    = 0,
                     Inst< SFVec3f > _rotation  = 0,
                     Inst< MFBool  > _buttons   = 0  );

    /// Destructor.
    virtual ~SpaceballSensor();

    void addGLUTMotionAction( int x, int y, int z );

    void addGLUTRotateAction( int x, int y, int z );

    void addGLUTButtonAction( int button, int state );


    /// glut callback function. Calls addGLUT*Action() on all
    /// SpaceballSensor instances.
    static void glutMotionCallback( int x, int y, int z );
    
    /// glut callback function. Calls addGLUT*Action() on all
    /// SpaceballSensor instances.
    static void glutRotateCallback( int x, int y, int z );
    
    /// glut callback function. Calls addGLUT*Action() on all
    /// SpaceballSensor instances.
    static void glutButtonCallback( int button, int state );
    
    /// The translation component of the Spaceball
    /// 
    /// <b>Access type: </b> outputOnly
    auto_ptr< SFVec3f>  motion;

    /// The rotation component of the Spaceball.
    /// 
    /// <b>Access type: </b> outputOnly
    auto_ptr< SFVec3f>   rotation;

    /// The status of the middle button. TRUE indicates that it is pressed.
    /// 
    /// <b>Access type: </b> outputOnly
    auto_ptr< MFBool >   buttons;


    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;

  private:
    static list< SpaceballSensor * > instances;  
  };
}

#endif
