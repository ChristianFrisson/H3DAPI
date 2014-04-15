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
/// \file TimeFunctionEffect.cpp
/// \brief cpp file for TimeFunctionEffect
///
//
//////////////////////////////////////////////////////////////////////////////
#include <H3D/TimeFunctionEffect.h> 
#include <HAPI/HapticTimeFunctionEffect.h>
#include <H3D/H3DHapticsDevice.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase TimeFunctionEffect::database( 
                                       "TimeFunctionEffect", 
                                       &(newInstance<TimeFunctionEffect>),
                                       typeid( TimeFunctionEffect ),
                                       &H3DForceEffect::database );

namespace TimeFunctionEffectInternals {
  FIELDDB_ELEMENT( TimeFunctionEffect, xFunction, INPUT_OUTPUT );
  FIELDDB_ELEMENT( TimeFunctionEffect, yFunction, INPUT_OUTPUT );
  FIELDDB_ELEMENT( TimeFunctionEffect, zFunction, INPUT_OUTPUT );
  FIELDDB_ELEMENT_EX( TimeFunctionEffect, deviceIndex, INPUT_OUTPUT,
                      deviceIndices );
}

/// Constructor
TimeFunctionEffect::TimeFunctionEffect(
                          Inst< SFFunctionNode > _xFunction,
                          Inst< SFFunctionNode > _yFunction,
                          Inst< SFFunctionNode > _zFunction,
                          Inst< MFInt32        > _deviceIndex,
                          Inst< SFNode         > _metadata ) :
  H3DForceEffect( _metadata, _deviceIndex ),
  xFunction( _xFunction ),
  yFunction( _yFunction ),
  zFunction( _zFunction ) {
  
  type_name = "TimeFunctionEffect";

  database.initFields( this );
}

void TimeFunctionEffect::traverseSG( TraverseInfo &ti ) {

  H3DFunctionNode * x_function = xFunction->getValue();
  H3DFunctionNode * y_function = yFunction->getValue();
  H3DFunctionNode * z_function = zFunction->getValue();
  
  if( x_function && x_function->nrInputValues() == 1 &&
      y_function && y_function->nrInputValues() == 1 &&
      z_function && z_function->nrInputValues() == 1 ) {
    if( deviceIndex->empty() ) {
      // Render the force on all devices.
      for( int i = 0; i < (H3DInt32)ti.getHapticsDevices().size(); ++i ) {
        if( ti.hapticsEnabled(i) ) {
          HAPI::HAPIFunctionObject * x_hapi_func =
            x_function->getAsHAPIFunctionObject();
          HAPI::HAPIFunctionObject * y_hapi_func =
            y_function->getAsHAPIFunctionObject();
          HAPI::HAPIFunctionObject * z_hapi_func =
            z_function->getAsHAPIFunctionObject();
          if( x_hapi_func && y_hapi_func && z_hapi_func ) {
            ti.addForceEffect( i,
              new HAPI::HapticTimeFunctionEffect( x_hapi_func,
                                                  y_hapi_func,
                                                  z_hapi_func,
                                                  0, 0, 0 ) );
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
              new HAPI::HapticTimeFunctionEffect( x_hapi_func,
                                                  y_hapi_func,
                                                  z_hapi_func,
                                                  0, 0, 0 ) );
          }
        }
      }
    }
  } else {
    Console(3) << "Warning: One or more of the H3DFunctionNodes in node "
               << getName() << " is either missing or has the wrong number "
               << "of input values to work with TimeFunctionEffect nodes."
               << "The number of input values to the functions should be 1."
               << endl;
  }
}
