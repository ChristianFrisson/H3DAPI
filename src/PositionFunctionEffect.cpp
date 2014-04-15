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
/// \file PositionFunctionEffect.cpp
/// \brief cpp file for PositionFunctionEffect
///
//
//////////////////////////////////////////////////////////////////////////////
#include <H3D/PositionFunctionEffect.h> 
#include <HAPI/HapticPositionFunctionEffect.h>
#include <H3D/H3DHapticsDevice.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase PositionFunctionEffect::database( 
                                       "PositionFunctionEffect", 
                                       &(newInstance<PositionFunctionEffect>),
                                       typeid( PositionFunctionEffect ),
                                       &H3DForceEffect::database );

namespace PositionFunctionEffectInternals {
  FIELDDB_ELEMENT( PositionFunctionEffect, xFunction, INPUT_OUTPUT );
  FIELDDB_ELEMENT( PositionFunctionEffect, yFunction, INPUT_OUTPUT );
  FIELDDB_ELEMENT( PositionFunctionEffect, zFunction, INPUT_OUTPUT );
  FIELDDB_ELEMENT_EX( PositionFunctionEffect, deviceIndex, INPUT_OUTPUT,
                      deviceIndices );
}

/// Constructor
PositionFunctionEffect::PositionFunctionEffect(
                          Inst< SFFunctionNode > _xFunction,
                          Inst< SFFunctionNode > _yFunction,
                          Inst< SFFunctionNode > _zFunction,
                          Inst< MFInt32        > _deviceIndex,
                          Inst< SFNode         > _metadata ) :
  H3DForceEffect( _metadata, _deviceIndex ),
  xFunction( _xFunction ),
  yFunction( _yFunction ),
  zFunction( _zFunction ) {
  
  type_name = "PositionFunctionEffect";

  database.initFields( this );
}

void PositionFunctionEffect::traverseSG( TraverseInfo &ti ) {

  H3DFunctionNode * x_function = xFunction->getValue();
  H3DFunctionNode * y_function = yFunction->getValue();
  H3DFunctionNode * z_function = zFunction->getValue();
  
  if( x_function && x_function->nrInputValues() == 3 &&
      y_function && y_function->nrInputValues() == 3 &&
      z_function && z_function->nrInputValues() == 3 ) {
    
    if( deviceIndex->empty() ) {
      // Render the force on all devices.
      for( int i = 0; i < (H3DInt32)ti.getHapticsDevices().size(); ++i ) {
        if( ti.hapticsEnabled( i ) ) {
          HAPI::HAPIFunctionObject * x_hapi_func =
            x_function->getAsHAPIFunctionObject();
          HAPI::HAPIFunctionObject * y_hapi_func =
            y_function->getAsHAPIFunctionObject();
          HAPI::HAPIFunctionObject * z_hapi_func =
            z_function->getAsHAPIFunctionObject();
          if( x_hapi_func && y_hapi_func && z_hapi_func ) {
            ti.addForceEffect( i,
              new HAPI::HapticPositionFunctionEffect( x_hapi_func,
                                                      y_hapi_func,
                                                      z_hapi_func ) );
          }
        }
      }
    } else {
      // Only render the force on the devices specified by the indices
      // in the deviceIndex field.
      const vector< H3DInt32 > &device_index = deviceIndex->getValue();
      for( unsigned int i = 0; i < device_index.size(); ++i ) {
        if( device_index[i] >= 0 && ti.hapticsEnabled( device_index[i] ) ) {
          HAPI::HAPIFunctionObject * x_hapi_func =
            x_function->getAsHAPIFunctionObject();
          HAPI::HAPIFunctionObject * y_hapi_func =
            y_function->getAsHAPIFunctionObject();
          HAPI::HAPIFunctionObject * z_hapi_func =
            z_function->getAsHAPIFunctionObject();
          if( x_hapi_func && y_hapi_func && z_hapi_func ) {
            ti.addForceEffect( device_index[i],
              new HAPI::HapticPositionFunctionEffect( x_hapi_func,
                                                      y_hapi_func,
                                                      z_hapi_func ) );
          }
        }
      }
    }
  } else {
    Console(3) << "Warning: One or more of the H3DFunctionNodes in node "
               << getName() << " is either missing or has the wrong number "
               << "of input values to work with PositionFunctionEffect nodes."
               << "The number of input values to the functions should be 3."
               << endl;
  }
}
