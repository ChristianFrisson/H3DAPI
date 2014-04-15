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
/// \file OpenHapticsSurface.h
/// \brief Header file for OpenHapticsSurface
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __OPENHAPTICSSURFACE_H__
#define __OPENHAPTICSSURFACE_H__

#include <H3D/MagneticSurface.h>
#include <H3D/SFBool.h>

#ifdef HAVE_OPENHAPTICS

namespace H3D {

  /// \ingroup H3DNodes
  /// \class OpenHapticsSurface
  /// \brief Surface type for setting OpenHaptics surface parameters directly.
  /// Only works when using the OpenHapticsRenderer.
  class H3DAPI_API OpenHapticsSurface: public MagneticSurface {
  public:

    /// Specialized field which sets the magnetic variable in
    /// HAPI::OpenHapticsRenderer::OpenHapticsSurface when the magnetic
    /// field of OpenHapticsSurface is changed.
    ///
    /// routes_in[0] is the magnetic field
    class H3DAPI_API UpdateMagnetic:
      public AutoUpdate< OnValueChangeSField< SFBool > > {
    protected:
      virtual void onValueChange( const bool &b );
    };

    /// Constructor.
    OpenHapticsSurface( Inst< UpdateStiffness > _stiffness = 0,
                        Inst< UpdateDamping > _damping   = 0,
                        Inst< UpdateStaticFriction > _staticFriction  = 0,
                        Inst< UpdateDynamicFriction > _dynamicFriction = 0,
                        Inst< UpdateMagnetic  > _magnetic  = 0,
                        Inst< UpdateSnapDistance > _snapDistance = 0 );
  
    void initialize();

    /// Specifies if the surface should be magnetic or not.
    ///
    /// <b>Access type: </b> inputOutput \n
    /// <b>Default value: </b> false \n
    auto_ptr< UpdateMagnetic > magnetic;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
#endif
