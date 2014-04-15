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
/// \file LineProperties.h
/// \brief Header file for LineProperties, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __LINEPROPERTIES_H__
#define __LINEPROPERTIES_H__

#include <H3D/X3DAppearanceChildNode.h>
#include <H3D/SFInt32.h>
#include <H3D/SFFloat.h>

namespace H3D {

  /// \ingroup X3DNodes
  /// \class LineProperties
  /// \brief The LineProperties node specifies additional properties to be
  /// applied to all line geometry. 
  ///
  /// \anchor LineProperties_details
  /// The linetype and linewidth shall only be applied when the applied
  /// field has value TRUE. When the value of the applied field is FALSE, a
  /// solid line of nominal width shall be produced. The colour of the line
  /// is specified by the associated Material node.
  ///
  /// The linetype field selects a line pattern as defined in the
  /// International Register of Graphical Items. The following line patterns 
  /// are available:
  ///
  /// <ol>
  /// <li>Solid 
  /// <li>Dashed 
  /// <li>Dotted 
  /// <li>Dashed-dotted 
  /// <li>Dash-dot-dot 
  /// </ol>
  ///
  /// The linewidthScaleFactor is a multiplicative value that scales the
  /// nominal linewidth by the linewidth scale factor. This resulting value
  /// shall then be mapped to the nearest available line width. A value less
  /// than or equal to zero refers to the minimum available line width. 
  ///
  /// Changing the line types does NOT CHANGE THE HAPTIC RENDERING of lines.
  /// It only changes the visual appearance.
  ///
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/LineProperties.x3d">LineProperties.x3d</a>
  ///     ( <a href="examples/LineProperties.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile LineProperties.dot
  class H3DAPI_API LineProperties : public X3DAppearanceChildNode {
  public:

    /// Constructor.
    LineProperties( Inst< DisplayList > _displayList          = 0,
                    Inst< SFNode      > _metadata             = 0,
                    Inst< SFBool      > _applied              = 0,
                    Inst< SFInt32     > _linetype             = 0,
                    Inst< SFFloat     > _linewidthScaleFactor = 0 );

    /// Returns a bitmask of the OpenGL attrib bits that will be affected
    /// by this node. The value can be used in glPushAttrib in order
    /// to save the current state.
    virtual GLbitfield getAffectedGLAttribs() {
      return GL_LINE_BIT;
    }

    /// Set up the line properties with OpenGL.
    virtual void render();
    
    /// Returns the default xml containerField attribute value.
    /// For this node it is "lineProperties".
    ///
    virtual string defaultXMLContainerField() {
      return "lineProperties";
    }
    
    /// Determines if the linetype and linewidthScaleFactor should be applied 
    /// or not.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> true \n
    /// 
    /// \dotfile LineProperties_applied.dot 
    
    auto_ptr< SFBool  >  applied;

    /// The linetype field selects a line pattern to be used for lines. For 
    /// more information see the 
    /// \ref LineProperties_details "Detailed description" 
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 1 \n
    /// 
    /// \dotfile LineProperties_linetype.dot 
    auto_ptr< SFInt32 >  linetype;

    /// The linewidthScaleFactor is a multiplicative value that scales the
    /// nominal linewidth by the linewidth scale factor. This resulting value
    /// shall then be mapped to the nearest available line width. A value less
    /// than or equal to zero refers to the minimum available line width.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 0 \n
    /// 
    /// \dotfile LineProperties_linewidthScaleFactor.dot 
    auto_ptr< SFFloat >  linewidthScaleFactor;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
