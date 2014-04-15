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
/// \file Text.h
/// \brief Header file for Text, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __TEXT_H__
#define __TEXT_H__

#include <H3D/X3DGeometryNode.h>
#include <H3D/X3DFontStyleNode.h>
#include <H3D/MFFloat.h>
#include <H3D/SFFloat.h>
#include <H3D/MFString.h>
#include <H3D/MFVec2f.h>
#include <H3D/SFVec2f.h>
#include <H3D/SFVec3f.h>
#include <H3D/FontStyle.h>
#include <H3D/PeriodicUpdate.h>

namespace H3D {

  /// \ingroup X3DNodes
  /// \class Text
  /// \brief The Text node specifies a two-sided, flat text string object
  /// positioned in the Z=0 plane of the local coordinate system based on
  /// values defined in the fontStyle field (see FontStyle). 
  ///
  /// Text nodes may contain multiple text strings specified using the
  /// UTF-8 encoding. The text strings are stored in the order in which the
  /// text mode characters are to be produced as defined by the parameters
  /// in the FontStyle node.
  ///
  /// The text strings are contained in the string field. The fontStyle
  /// field contains one FontStyle node that specifies the font size, font
  /// family and style, direction of the text strings, and any specific
  /// language rendering techniques used for the text. 
  ///
  /// The maxExtent field limits and compresses all of the text strings if
  /// the length of the maximum string is longer than the maximum extent, as
  /// measured in the local coordinate system. If the text string with the
  /// maximum length is shorter than the maxExtent, then there is no
  /// compressing. The maximum extent is measured horizontally for
  /// horizontal text (FontStyle node: horizontal=TRUE) and vertically for
  /// vertical text (FontStyle node: horizontal=FALSE). The maxExtent field
  /// shall be greater than or equal to zero.
  ///
  /// The length field contains an MFFloat value that specifies the length
  /// of each text string in the local coordinate system. If the string is
  /// too short, it is stretched (either by scaling the text or by adding
  /// space between the characters). If the string is too long, it is
  /// compressed (either by scaling the text or by subtracting space between
  /// the characters). If a length value is missing (for example, if there
  /// are four strings but only three length values), the missing values are
  /// considered to be 0. The length field shall be greater than or equal to
  /// zero. 
  ///
  /// Specifying a value of 0 for both the maxExtent and length fields
  /// indicates that the string may be any length. 
  /// 
  /// The solid field determines whether one or both sides of each polygon
  /// shall be displayed. If solid is FALSE, each polygon is visible
  /// regardless of the viewing direction and if it is TRUE back face culling
  /// is performed to only show the front face of the polygons.
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/Text.x3d">Text.x3d</a>
  ///     ( <a href="examples/Text.x3d.html">Source</a> )
  ///   - <a href="../../../H3DAPI/examples/All/Text1.x3d">Text1.x3d</a>
  ///     ( <a href="examples/Text1.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile Text.dot
  class H3DAPI_API Text : public X3DGeometryNode {
  protected:
    typedef TypedSFNode< X3DFontStyleNode > SFFontStyleNode;

    /// The SFBound field is specialized to update itself from the
    /// values in the fields of the Text node.
    /// 
    /// - routes_in[0] is the fontStyle field.
    /// - routes_in[1] is the length field.
    /// - routes_in[2] is the maxExtent field.
    /// - routes_in[3] is the string field.
    class H3DAPI_API SFBound: 
      public TypedField< X3DGeometryNode::SFBound,
      Types< SFFontStyleNode, MFFloat, SFFloat, MFString > >{
  protected:
      /// Updates to a BoxBound containing the text.
      virtual void update();
    }; 

    /// We extend the DisplayList to build the fonts of the X3DFontStyleNode
    /// in the fontStyle field of the Text node this DisplayList resides
    /// in. This is needed since the X3DFontStyleNode will build display
    /// lists of all the characters that will be used when rendering
    /// the Text node. So in order to prevent display list error for 
    /// building new display lists while already building a display list
    /// we do this before the Text node display list is built.
    class H3DAPI_API DisplayList : public X3DGeometryNode::DisplayList {
      /// Build the fonts of the fontStyle node before calling the
      /// display lists.
      virtual void callList( bool build_list );
    };

    /// Internal field class to update values of origin, textBounds, 
    /// lineBounds. Only outputs values if default length and maxExtent
    /// are used. Changes on string and fontStyle changes.
    class H3DAPI_API OutputUpdater : 
      public PeriodicUpdate< TypedField< Field, 
                             Types<SFFontStyleNode, MFString> > > {
    protected:
      /// Updates values of origin, lineBounds and textBounds
      virtual void update();
    };

  public:
    
    /// Constructor.
    Text( Inst< SFNode         > _metadata    = 0,
          Inst< SFBound         > _bound       = 0,
          Inst< DisplayList     > _displayList = 0,
          Inst< SFFontStyleNode > _fontStyle   = 0,
          Inst< MFFloat         > _length      = 0,
          Inst< SFFloat         > _maxExtent   = 0,
          Inst< MFString        > _string      = 0,
          Inst< MFVec2f         > _lineBounds  = 0,
          Inst< SFVec3f         > _origin      = 0,
          Inst< SFVec2f         > _textBounds  = 0, 
          Inst< SFBool          > _solid       = 0,
          Inst< OutputUpdater   > _outputUpdater = 0 );

    /// Render the text with OpenGL.
    virtual void render();

    // Traverse the scenegraph. See X3DGeometryNode::traverseSG
    // for more info.
    virtual void traverseSG( TraverseInfo &ti ); 

    /// The number of triangles renderered in this geometry, we don't know
    /// so return -1
    virtual int nrTriangles() {
      return -1;
    }

    /// Detect intersection between a line segment and the texts bounding box.
    /// \param from The start of the line segment.
    /// \param to The end of the line segment.
    /// \param result Contains info about the closest intersection for every
    /// object that intersects the line.
    /// \returns true if intersected, false otherwise.
    virtual bool lineIntersect( 
      const Vec3f &from,
      const Vec3f &to,    
      LineIntersectResult &result );

    /// The style the text should be rendered with. See X3DFontStyleNode.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// 
    /// \dotfile Text_fontStyle.dot
    auto_ptr< SFFontStyleNode   >  fontStyle;
    
    /// The length field contains an MFFloat value that specifies the length
    /// of each text string in the local coordinate system. If the string is
    /// too short, it is stretched (either by scaling the text or by adding
    /// space between the characters). If the string is too long, it is
    /// compressed (either by scaling the text or by subtracting space between
    /// the characters). If a length value is missing (for example, if there
    /// are four strings but only three length values), the missing values are
    /// considered to be 0. The length field shall be greater than or equal to
    /// zero. 
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> [] \n
    /// <b>Valid range:</b> >=0
    /// 
    /// \dotfile Text_length.dot
    auto_ptr< MFFloat  >  length;

    /// The maxExtent field limits and compresses all of the text strings if
    /// the length of the maximum string is longer than the maximum extent, as
    /// measured in the local coordinate system. If the text string with the
    /// maximum length is shorter than the maxExtent, then there is no
    /// compressing. The maximum extent is measured horizontally for
    /// horizontal text (FontStyle node: horizontal=TRUE) and vertically for
    /// vertical text (FontStyle node: horizontal=FALSE). The maxExtent field
    /// shall be greater than or equal to zero.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 0 \n
    /// <b>Valid range:</b> >=0
    /// 
    /// \dotfile Text_maxExtent.dot
    auto_ptr< SFFloat  >  maxExtent;

    /// The strings to render. Each string will be on its own line.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// 
    /// \dotfile Text_string.dot
    auto_ptr< MFString >  stringF;

    /// A set of 2D vectors where each vector contains the size of the 2D 
    /// bounding box for each line of rendered text in local text x and y 
    /// units.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// 
    /// \dotfile Text_lineBounds.dot
    auto_ptr< MFVec2f > lineBounds;

    /// 3D position that specifies the origin of the text local coordinate 
    /// system in units of the coordinate system in which the Text node is
    /// embedded. The value of the origin field represents the upper left 
    /// corner of the textBounds.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// 
    /// \dotfile Text_origin.dot
    auto_ptr< SFVec3f > origin;

    /// The dimensions of Text node’s 2D bounding box.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// 
    /// \dotfile Text_textBounds.dot
    auto_ptr< SFVec2f > textBounds;

    /// The solid field determines whether one or both sides of each polygon
    /// shall be displayed. If solid is FALSE, each polygon is visible
    /// regardless of the viewing direction and if it is TRUE back face culling
    /// is performed to only show the front face of the polygons.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> FALSE \n
    /// 
    /// \dotfile Text_solid.dot
    auto_ptr< SFBool   >  solid;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;

  protected:
    /// Internal field to update output fields origin, lineBounds, textBounds.
    auto_ptr< OutputUpdater > outputUpdater;

    /// Calculates and updates lineBounds field value
    void updateLineBounds();

    /// The font style to use when no style is specified.
    static AutoRef< FontStyle > default_font_style;

    /// Scales the scene so that the text given would be contained within
    /// the value of the maxExtent field. The length field is inspected as
    /// well as the dimensions of the text to determine what the scale should
    /// be.
    virtual void scaleToMaxExtent( const vector< string >&text, 
                                   X3DFontStyleNode *font );
    
    /// Justify the text in the minor alignment by translating it in the 
    /// way described in the FontStyle node.
    virtual void justifyMinor( const vector< string > &text,
                               X3DFontStyleNode *font );

    /// Move to the position of the next line. What the next line is
    /// depends on the values in the FontStyle node, i.e. if the text
    /// is horizontal, if it is top to bottom or the other way around, 
    /// spacing etc.
    virtual void moveToNewLine( const string &text, X3DFontStyleNode *font );

    /// Render a line of text in the style of the X3DFontStyleNode given.
    /// The OpenGL transform matrices will not be changed by the call.
    virtual void renderTextLine( const string& text,
                                 X3DFontStyleNode *font );
    
    /// Justify the line of text in the major alignment by translating
    /// it in the way described in the FontStyle node.
    virtual void justifyLine( const string& text,
                              X3DFontStyleNode *font );
  };
}

#endif
