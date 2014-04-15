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
/// \file FillProperties.h
/// \brief Header file for FillProperties, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __FILLPROPERTIES_H__
#define __FILLPROPERTIES_H__

#include <H3D/X3DAppearanceChildNode.h>
#include <H3D/SFColor.h>
#include <H3D/SFInt32.h>

namespace H3D {

  /// \ingroup X3DNodes
  /// \class FillProperties
  /// \brief The FillProperties node specifies additional properties to be applied
  /// to all polygonal areas on top of whatever appearance is specified by
  /// the other fields of the respective Appearance node. 
  ///
  /// \anchor FillProperties_details
  /// The filled field specifies if the geometry should be filled or not. If not
  /// filled it will be rendered as wireframe.
  ///
  /// Hatches are applied on top of the already rendered appearance of
  /// the node. Hatches are not affected by lighting.
  ///
  /// The hatchStyle field selects a hatch pattern as defined in the
  /// International Register of Graphical Items. The hatches
  /// are rendered using the colour specified by the hatchColor field. 
  /// The following hatch styles are available:
  ///
  /// <ol>
  /// <li>horizontal, equally spaced lines
  /// <li>vertical, equally spaced lines
  /// <li>positive slope, equally spaced lines
  /// <li>negative slope, equally spaced lines
  /// <li>horizontal/vertical crosshatch
  /// <li>positive slope/negative slope crosshatch
  /// </ol>
  /// 
  /// The associated geometry shall be filled and/or hatched only when the
  /// respective values of the filled and/or hatched fields have value
  /// TRUE. 
  ///
  /// Changing the hatch style does NOT CHANGE THE HAPTIC RENDERING of lines.
  /// It only changes the visual appearance.
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/FillProperties.x3d">FillProperties.x3d</a>
  ///     ( <a href="examples/FillProperties.x3d.html">Source</a> )
  /// 
  /// \par Internal routes:
  /// \dotfile FillProperties.dot
  class H3DAPI_API FillProperties : public X3DAppearanceChildNode {
  public:
    
    /// Constructor.
    FillProperties( Inst< DisplayList > _displayList = 0,
                    Inst< SFBool      >  _filled     = 0,
                    Inst< SFColor     >  _hatchColor = 0,
                    Inst< SFBool      >  _hatched    = 0,
                    Inst< SFInt32     >  _hatchStyle = 0,
                    Inst< SFNode      >  _metadata   = 0 );


    /// Returns a bitmask of the OpenGL attrib bits that will be affected
    /// by this node. The value can be used in glPushAttrib in order
    /// to save the current state.
    virtual GLbitfield getAffectedGLAttribs() {
      return GL_POLYGON_BIT | GL_LIGHTING_BIT | GL_CURRENT_BIT;
    }

    /// Set up the line properties with OpenGL.
    virtual void render();

    /// Returns the default xml containerField attribute value.
    /// For this node it is "fillProperties".
    ///
    virtual string defaultXMLContainerField() {
      return "fillProperties";
    }
    
    /// The filled field specifies if the geometry should be filled or not. If not
    /// filled it will be rendered as wireframe.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> true \n
    /// 
    /// \dotfile FillProperties_filled.dot 
    auto_ptr< SFBool  >  filled;

    /// The color of the hatches. 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> RGB( 1,1,1 ) \n
    /// 
    /// \dotfile FillProperties_hatchColor.dot 
    auto_ptr< SFColor >  hatchColor;

    /// The hatched field specifies if the geometry should be hatched or not. 
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> true \n
    /// 
    /// \dotfile FillProperties_hatched.dot 
    auto_ptr< SFBool  >  hatched;

    /// The hatchStyle field selects a hatch pattern to be used. For 
    /// more information see the 
    /// \ref FillProperties_details "Detailed description" 
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 1 \n
    /// 
    /// \dotfile FillProperties_hatchStyle.dot 
    auto_ptr< SFInt32 >  hatchStyle;
    
    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
