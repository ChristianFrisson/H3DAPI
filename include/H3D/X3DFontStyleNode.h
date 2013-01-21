//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004-2013, SenseGraphics AB
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
/// \file X3DFontStyleNode.h
/// \brief Header file for X3DFontStyleNode, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __X3DFONTSTYLENODE_H__
#define __X3DFONTSTYLENODE_H__

#include <H3D/X3DNode.h>
#include "GL/glew.h"

namespace H3D {

  /// \ingroup AbstractNodes
  /// \class X3DFontStyleNode
  /// \brief This abstract node type is the base node type for all FontStyle
  ///  nodes. 
  class H3DAPI_API X3DFontStyleNode : public X3DNode {
  public:

    /// Enumeration of the alignments allowed for text.
    typedef enum {
      HORIZONTAL,
      VERTICAL
    } Alignment;

    /// Enumeration for the different justification values allowed.
    typedef enum {
      FIRST, 
      BEGIN,
      MIDDLE,
      END 
    } Justification;
    
    /// Constructor.
    X3DFontStyleNode( Inst< SFNode>  _metadata = 0 );

    /// Build Character instances for all characters that will be allowed
    /// for use by this font node. Characters are to be put into the 
    /// characters vector using its own value as an index. Also buildFonts
    /// has to be set to true when defining this function.
    virtual void buildFonts() {}

    /// Returns true if buildFonts has been called, false otherwise.
    inline bool fontsBuilt() {
      return fonts_built;
    }

    /// Returns true if the text should be rendered from top to bottom,
    /// false otherwise.
    virtual bool isTopToBottom() {
      return true;
    }

    /// Returns true if the text should be rendered from left to right,
    /// false otherwise.
    virtual bool isLeftToRight() {
      return true;
    }

    /// Returns the spacing between lines of text. See FontStyle for more
    /// details.
    virtual H3DFloat getSpacing() { 
      return 1;
    }

    /// Gets the alignment of the text.
    virtual Alignment getAlignment() {
      return HORIZONTAL;
    }
    
    /// Gets the justification of the text in the major alignment direction.
    virtual Justification getMajorJustification() {
      return BEGIN;
    }
    
    /// Gets the justification of the text in the minor alignment direction.
    virtual Justification getMinorJustification() {
      return FIRST;
    }

    /// Render the given character with this font.
    virtual void renderChar( unsigned char c ) {}

    /// Get the dimensions in metres of the character given when 
    /// rendered with this font.
    virtual Vec3f charDimensions( unsigned char c ) { return Vec3f(); }

    /// Returns how many metres in the the positive y-direction from the
    /// origin the characters of this font use. 
    virtual H3DFloat ascender() { return 0; }

    /// Returns how many metres in the the negative y-direction from the
    /// origin the characters of this font use. 
    virtual H3DFloat descender() { return 0; }

    /// Given a text string and an alignment of the text the function
    /// returns the dimensions of the bounding box of the 3d representation
    /// of the text using this font.
    Vec3f stringDimensions( const string &text,
                            Alignment alignment );

    /// Given a vector of text string and an alignment of the text the 
    /// function returns the dimensions of the bounding box of the 3d 
    /// representation of the text using this font. A vector specifying
    /// the length of the text fields as in the Text node. If this is 
    /// specified this length will be used as length for as many strings
    /// in text as there are values in length.
    Vec3f stringDimensions( const vector< string > &text,
                            Alignment alignment,
                            const vector< H3DFloat > &length = 
                            vector< H3DFloat >() );

    /// Returns the default xml containerField attribute value.
    /// For this node it is "fontStyle".
    ///
    virtual string defaultXMLContainerField() {
      return "fontStyle";
    }

protected:
    /// True if buildFonts has been called.
    bool fonts_built;
  };
}

#endif
