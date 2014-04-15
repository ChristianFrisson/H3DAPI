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
/// \file FontStyle.h
/// \brief Header file for FontStyle, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __FONTSTYLE_H__
#define __FONTSTYLE_H__

#include <H3D/X3DFontStyleNode.h>
#include <H3D/SFString.h>
#include <H3D/MFString.h>
#include <H3D/SFBool.h>
#include <H3D/SFFloat.h>

// forward declaration
class FTFont;

namespace H3D {

  /// \ingroup X3DNodes
  /// \class FontStyle
  /// \brief The FontStyle node defines the size, family, and style used
  /// for Text nodes, as well as the direction of the text strings and 
  /// any language-specific rendering techniques used for non-English text.
  /// See Text for a description of the Text node.
  ///
  /// The size field specifies the nominal height, in the local coordinate
  /// system of the Text node, of glyphs rendered and determines the spacing
  /// of adjacent lines of text. Values of the size field shall be greater
  /// than zero. 
  ///
  /// The spacing field determines the line spacing between adjacent lines
  /// of text. The distance between the baseline of each line of text is
  /// (spacing × size) in the appropriate direction (depending on other
  /// fields described below). 
  ///
  /// \anchor FontStyle_family
  /// The family field contains a case-sensitive MFString value that 
  /// specifies a sequence of font family names in preference order. 
  /// The MFString value is searched for the first font family name matching 
  /// a supported font family. If none of the string values matches a 
  /// supported font family, the default font family "SERIF" shall be used. 
  /// "SERIF" (the default) for a serif font such as Times Roman; 
  /// "SANS" for a sans-serif font such as Helvetica; and "TYPEWRITER" for 
  /// a fixed-pitch font such as Courier. An empty family value is identical
  /// to ["SERIF"]. Any font family may be specified as shown in the 
  /// following example of the specification of a font family:
  /// \code
  /// ["Lucida Sans Typewriter", "Lucida Sans", "Helvetica", "SANS"]
  /// \endcode
  /// In this example, the browser would first look for the font family
  /// "Lucida Sans Typewriter" on the system on which the browser is
  /// operating. If that is not available, the browser looks for 
  /// "Lucida Sans". If that is not available, the browser looks for 
  /// "Helvetica". If that is not available, the browser looks for any 
  /// sans-serif font. If there are not sans-serif fonts installed, the
  /// browser will use any serif font (the default). It is the responsibility
  /// of the author that a suitable list of font families be specified so
  /// that the desired appearance is achieved in most operating environments.
  /// However, the author should always be willing to accept that the 
  /// requested font families may not be available resulting in the use
  /// of a browser-selected "SERIF" font being used.
  ///
  /// The style field specifies a case-sensitive SFString value that may be 
  /// "PLAIN" (the default) for default plain type; "BOLD" for boldface type; 
  /// "ITALIC" for italic type; or "BOLDITALIC" for bold and italic type. An
  /// empty style value ("") is identical to "PLAIN". 
  ///
  /// \anchor FontStyle_direction
  /// The horizontal, leftToRight, and topToBottom fields indicate the 
  /// direction of the text. The horizontal field indicates whether the
  /// text advances horizontally in its major direction (horizontal = TRUE, 
  /// the default) or vertically in its major direction (horizontal = FALSE).
  /// The leftToRight and topToBottom fields indicate direction of text 
  /// advance in the major (characters within a single string) and minor
  /// (successive strings) axes of layout. Which field is used for the major
  /// direction and which is used for the minor direction is determined by
  /// the horizontal field. Note that the direction specification overrides
  /// any modes inherent in a particular language.
  ///
  /// For horizontal text (horizontal = TRUE), characters on each line of text
  /// advance in the positive X direction if leftToRight is TRUE or in the
  /// negative X direction if leftToRight is FALSE. Characters are advanced 
  /// according to their natural advance width. Each line of characters is 
  /// advanced in the negative Y direction if topToBottom is TRUE or in the
  /// positive Y direction if topToBottom is FALSE. Lines are advanced by the
  /// amount of size × spacing.
  ///
  /// For vertical text (horizontal = FALSE), characters on each line of text
  /// advance in the negative Y direction if topToBottom is TRUE or in the 
  /// positive Y direction if topToBottom is FALSE. Characters are advanced 
  /// according to their natural advance height. Each line of characters is 
  /// advanced in the positive X direction if leftToRight is TRUE or in the
  /// negative X direction if leftToRight is FALSE. Lines are advanced by 
  /// the amount of size × spacing.
  /// 
  /// \anchor FontStyle_justify
  /// The justify field determines alignment of the above text layout 
  /// relative to the origin of the object coordinate system. The justify 
  /// field is an MFString which can contain 2 values. The first value
  /// specifies alignment along the major axis and the second value specifies
  /// alignment along the minor axis, as determined by the horizontal field. 
  /// An empty justify value ("") is equivalent to the default value. If the
  /// second string, minor alignment, is not specified, minor alignment 
  /// defaults to the value "FIRST". Thus, justify values of "", "BEGIN", and
  /// ["BEGIN" "FIRST"] are equivalent.
  /// 
  /// The major alignment is along the X-axis when horizontal is TRUE and 
  /// along the Y-axis when horizontal is FALSE. The minor alignment is along
  /// the Y-axis when horizontal is TRUE and along the X-axis when horizontal
  /// is FALSE. The possible values for each enumerant of the justify field
  /// are "FIRST", "BEGIN", "MIDDLE", and "END". For major alignment, each 
  /// line of text is positioned individually according to the major alignment
  /// enumerant. For minor alignment, the block of text representing all lines
  /// together is positioned according to the minor alignment enumerant. The 
  /// default minor alignment is "FIRST". This is a special case of minor
  /// alignment when horizontal is TRUE. Text starts at the baseline at the
  /// Y-axis. In all other cases, "FIRST" is identical to "BEGIN". 
  ///
  /// The language field specifies the context of the language for the text
  /// string in the form of a language and a country in which that language 
  /// is used. This field is not used by H3DApi.
  /// 
  /// The renderType field is H3D API specific and specifies which way
  /// the font will be rendered. Possible values are "TEXTURE", "POLYGON"
  /// and "OUTLINE". "TEXTURE" will use texture maps to render the font,
  /// "POLYGON" will create a polygon model for each character and render
  /// it and "OUTLINE" will render the outlines for each character.
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/FontStyle.x3d">FontStyle.x3d</a>
  ///     ( <a href="examples/FontStyle.x3d.html">Source</a> )
  class H3DAPI_API FontStyle : public X3DFontStyleNode {
  public:
    /// Thrown when the string in the style field is an invalid value.
    H3D_VALUE_EXCEPTION( string, InvalidFontStyleStyle );
    /// Thrown when the strings in the justify field are invalid values.
    H3D_VALUE_EXCEPTION( string, InvalidFontStyleJustify );
    /// Thrown when the strings in the renderType field are invalid values.
    H3D_VALUE_EXCEPTION( string, InvalidFontStyleRenderType );

    /// Constructor.
    FontStyle( Inst< SFNode   > _metadata    = 0,
               Inst< MFString > _family      = 0,
               Inst< SFBool   > _horizontal  = 0,
               Inst< MFString > _justify     = 0,
               Inst< SFString > _language    = 0,
               Inst< SFBool   > _leftToRight = 0,
               Inst< SFFloat  > _size        = 0,
               Inst< SFFloat  > _spacing     = 0,
               Inst< SFString > _style       = 0,
               Inst< SFBool   > _topToBottom = 0,
               Inst< SFString > _renderType  = 0 );
#if defined( HAVE_FREETYPE ) && defined( HAVE_FTGL )
    /// Build Character instances for all characters that will be allowed
    /// for use by this font node. For Windows wglUseFontOutlines will 
    /// be used to generate polygon versions of the characters of the font.
    virtual void buildFonts();

    /// Returns true if the text should be rendered from top to bottom,
    /// false otherwise.
    virtual bool isTopToBottom() {
      return topToBottom->getValue();
    }

    /// Returns true if the text should be rendered from left to right,
    /// false otherwise.
    virtual bool isLeftToRight() {
      return leftToRight->getValue();
    }

    /// Returns the spacing between lines of text. See FontStyle for more
    /// details.
    virtual H3DFloat getSpacing() { 
      return spacing->getValue();
    }

    /// Gets the alignment of the text.
    virtual Alignment getAlignment() {
      if( horizontal->getValue() )
        return HORIZONTAL;
      else
        return VERTICAL;
    }
    
    /// Render the character.
    virtual void renderChar( unsigned char c );

    /// Render the given string with this font from left to right horizontally.
    virtual void renderHorizontalLine( const string &c );

    /// Returns how many metres in the the positive y-direction from the
    /// origin the characters of this font use. 
    virtual H3DFloat ascender();

    /// Returns how many metres in the the negative y-direction from the
    /// origin the characters of this font use. 
    virtual H3DFloat descender();

    /// Get the bounding box dimensions of the 3d-representation of
    /// the given character.
    virtual Vec3f charDimensions( unsigned char c );

    /// Gets the justification of the text in the major alignment direction.
    virtual Justification getMajorJustification();

    /// Gets the justification of the text in the minor alignment direction.
    virtual Justification getMinorJustification();

#endif
    /// The family field contains a case-sensitive MFString value that 
    /// specifies a sequence of font family names in preference order. 
    /// See the 
    /// \ref FontStyle_family "detailed desription"
    /// for details.
    /// 
    /// <b>Access type:</b> initializeOnly \n
    /// <b>Default value:</b> ["SERIF"] \n
    auto_ptr< MFString >  family;

    /// The horizontal field indicates whether the
    /// text advances horizontally in its major direction (horizontal = TRUE, 
    /// the default) or vertically in its major direction 
    /// (horizontal = FALSE).
    /// 
    /// <b>Access type:</b> initializeOnly \n
    /// <b>Default value:</b> TRUE \n
    auto_ptr< SFBool   >  horizontal;

    /// The justify field determines alignment of the above text layout 
    /// relative to the origin of the object coordinate system. See the 
    /// \ref FontStyle_justify "detailed desription"
    /// for details.
    /// 
    /// <b>Access type:</b> initializeOnly \n
    /// <b>Default value:</b> ["BEGIN"] \n
    /// <b>Value range:</b> "BEGIN", "FIRST", "MIDDLE" or "END" \n
    auto_ptr< MFString >  justify;

    /// The language field specifies the context of the language for the text
    /// string in the form of a language and a country in which that language 
    /// is used. This field is not used by H3DApi.
    /// 
    /// <b>Access type:</b> initializeOnly \n
    auto_ptr< SFString >  language;

    /// The leftToRight field indicate direction of text 
    /// advance in the major (characters within a single string) or minor
    /// (successive strings) axes of layout depending on the value of the
    /// horizontal field. See 
    /// \ref FontStyle_direction "detailed description" 
    /// for details.
    /// 
    /// <b>Access type:</b> initializeOnly \n
    /// <b>Default value:</b> TRUE \n
    auto_ptr< SFBool   >  leftToRight;

    /// The size field specifies the nominal height, in the local coordinate
    /// system of the Text node, of glyphs rendered and determines the spacing
    /// of adjacent lines of text. 
    /// 
    /// <b>Access type:</b> initializeOnly \n
    /// <b>Default value:</b> 1 \n
    /// <b>Value range:</b> >0 \n
    auto_ptr< SFFloat  >  size;

    /// The spacing field determines the line spacing between adjacent lines
    /// of text. The distance between the baseline of each line of text is
    /// (spacing × size) in the appropriate direction.
    /// 
    /// <b>Access type:</b> initializeOnly \n
    /// <b>Default value:</b> 1 \n
    /// <b>Value range:</b> >0 \n
    auto_ptr< SFFloat  >  spacing;

    /// The style field specifies a case-sensitive SFString value that may be 
    /// "PLAIN" (the default) for default plain type; 
    /// "BOLD" for boldface type; 
    /// "ITALIC" for italic type; or "BOLDITALIC" for bold and italic type. An
    /// empty style value ("") is identical to "PLAIN".
    /// 
    /// <b>Access type:</b> initializeOnly \n
    /// <b>Default value:</b> "PLAIN" \n
    /// <b>Value range:</b> "PLAIN", "BOLD", "ITALIC" or "BOLDITALIC" \n
    auto_ptr< SFString >  style;

    /// The topToBottom field indicate direction of text 
    /// advance in the major (characters within a single string) or minor
    /// (successive strings) axes of layout depending on the value of
    /// the horizontal field. See 
    /// \ref FontStyle_direction "detailed description" 
    /// for details.
    /// 
    /// <b>Access type:</b> initializeOnly \n
    /// <b>Default value:</b> TRUE \n
    auto_ptr< SFBool >  topToBottom;

    /// The renderType field is H3D API specific and specifies which way
    /// the font will be rendered. Possible values are "TEXTURE", "POLYGON"
    /// and "OUTLINE". "TEXTURE" will use texture maps to render the font,
    /// "POLYGON" will create a polygon model for each character and render
    /// it and "OUTLINE" will render the outlines for each character.
    /// <b>Access type:</b> initializeOnly \n
    /// <b>Default value:</b> "TEXTURE" \n
    /// <b>Value range:</b> "TEXTURE", "POLYGON" or "OUTLINE" \n
    auto_ptr< SFString > renderType;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  protected:
#if defined( HAVE_FREETYPE ) && defined( HAVE_FTGL )
    /// FTGL container class for all glyphs in the current font, used to
    /// measure bounding  boxes, render, advance, etc.
    FTFont *font;
#endif
  };
}

#endif
