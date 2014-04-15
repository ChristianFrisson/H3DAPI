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
/// \file X3DTouchSensorNode.h
/// \brief Header file for X3DTouchSensorNode, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __X3DTOUCHSENSORNODE_H__
#define __X3DTOUCHSENSORNODE_H__

#include <H3D/X3DPointingDeviceSensorNode.h>
#include <H3D/SFTime.h>

namespace H3D {

  /// \ingroup AbstractNodes
  /// \class X3DTouchSensorNode
  /// \brief This abstract node type is the base type for all 
  /// touch-style pointing device sensors.
  ///
  /// \par Internal routes:
  /// \dotfile X3DTouchSensorNode.dot
  
  class H3DAPI_API X3DTouchSensorNode : 
    public X3DPointingDeviceSensorNode {
  public:

    /// The SetTouchTime class is specialize field to generate a touchTime
    /// event.
    ///
    /// - routes_in[0] is the isActive field
    class H3DAPI_API SetTouchTime: 
      public AutoUpdate< TypedField < SFBool, SFBool > > {
    public:
      virtual void setValue( const bool &b, int id = 0 ) {
        SFBool::setValue( b, id );
      }
    protected:
      virtual void update() {
        SFBool::update();
        bool isActive = static_cast< SFBool * >( routes_in[0] )->getValue();
        X3DTouchSensorNode *ts = 
            static_cast< X3DTouchSensorNode * >( getOwner() );
        if( !isActive && ts->isOver->getValue() ) {
          ts->touchTime->setValue( event.time_stamp, ts->id );
        }
      }
    };
#ifdef __BORLANDC__
    friend class SetTouchTime;
#endif
    
    /// Constructor.
    X3DTouchSensorNode( Inst< SFString > _description = 0,
                        Inst< SFBool >  _enabled  = 0,
                        Inst< SFNode >  _metadata = 0,
                        Inst< SFBool >  _isActive = 0,
                        Inst< SFBool > _isOver = 0,
                        Inst< SFTime > _touchTime = 0 );

    // Fields
    /// \brief The field touchTime is generated when all three of the following
    /// conditions are true:
    /// - The pointing device was pointing towards the geometry when it was
    /// initially activated (isActive is TRUE).
    /// - The pointing device is currently pointing towards the geometry
    /// (isOver is TRUE). 
    /// - The pointing device is deactivated (isActive FALSE event is also
    /// generated).
    /// 
    /// <b>Access type:</b> outputOnly \n
    /// 
    /// \dotfile X3DTouchSensorNode_touchTime.dot 
    auto_ptr< SFTime > touchTime;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;

  protected:
    // used to keep track on when the touching of the geometry started.
    H3DTime start_time;
    // instance of specialized field.
    auto_ptr< SetTouchTime > setTouchTime;
  };
}

#endif
