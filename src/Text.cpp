//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004, SenseGraphics AB
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
/// \file Text.cpp
/// \brief CPP file for Text, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include "Text.h"
#include "FontStyle.h"

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase Text::database( 
                               "Text", 
                               &(newInstance<Text>), 
                               typeid( Text ),
                               &X3DGeometryNode::database );

namespace TextInternals {
  FIELDDB_ELEMENT( Text, fontStyle, INPUT_OUTPUT );
  FIELDDB_ELEMENT( Text, length, INPUT_OUTPUT );
  FIELDDB_ELEMENT( Text, maxExtent, INPUT_OUTPUT );
  FieldDBInsert string( INPUT_OUTPUT( &Text::database, "string", &Text::stringF ) );
  FIELDDB_ELEMENT( Text, solid, INPUT_OUTPUT );
}

Text::Text( Inst< SFNode          > _metadata,
            Inst< SFBound         > _bound,
            Inst< DisplayList     > _displayList,
            Inst< SFFontStyleNode > _fontStyle,
            Inst< MFFloat         > _length,
            Inst< SFFloat         > _maxExtent,
            Inst< MFString        > _string,
            Inst< SFBool          > _solid ) :
  X3DGeometryNode( _metadata, _bound, _displayList ),
  fontStyle( _fontStyle ),
  length   ( _length    ),
  maxExtent( _maxExtent ),
  stringF  ( _string   ),
  solid    ( _solid     ) {

  type_name = "Text";
  
  database.initFields( this );
  
  maxExtent->setValue( 0.0 );
  solid->setValue( false );
  
  fontStyle->route( displayList );
  length->route( displayList );
  maxExtent->route( displayList );
  stringF->route( displayList );
  solid->route( displayList );

  fontStyle->route( bound );
  length->route( bound );
  maxExtent->route( bound );
  stringF->route( bound );

}

void Text::justifyMinor( const vector< string > &lines,
                         X3DFontStyleNode *font ) {
  if( lines.size() == 0 ) return;
  
  X3DFontStyleNode::Alignment alignment = font->getAlignment();
  bool left_to_right = font->isLeftToRight();
  bool top_to_bottom = font->isTopToBottom();
  X3DFontStyleNode::Justification minor = font->getMinorJustification();

  // dimension of the first line of text
  Vec3f line1_size = font->stringDimensions( lines[0], alignment );
  // dimension of all lines combined  
  Vec3f total_size = font->stringDimensions( lines, alignment );

  // horizontal text
  if( alignment == X3DFontStyleNode::HORIZONTAL ) {
    if( top_to_bottom ) {
      if( minor == X3DFontStyleNode::BEGIN ) {
        glTranslatef( 0, -line1_size.y, 0 );
      } else if( minor == X3DFontStyleNode::MIDDLE ) {
        glTranslatef( 0, 
                      total_size.y/2 - line1_size.y - font->descender(), 
                      0 );
      } else if( minor == X3DFontStyleNode::END ) {
        glTranslatef( 0, total_size.y - line1_size.y , 0 );
      }
    } else {
      if( minor == X3DFontStyleNode::MIDDLE ) {
        glTranslatef( 0, -total_size.y/2, 0 );
      } else if( minor == X3DFontStyleNode::END ) {
        glTranslatef( 0, -total_size.y, 0 );
      }
    }
    // vertical text
  } else if( alignment == X3DFontStyleNode::VERTICAL ) {
    if( left_to_right ) {
      if( minor == X3DFontStyleNode::MIDDLE ) {
        glTranslatef( -total_size.x/2, 0, 0 );
      } else if( minor == X3DFontStyleNode::END ) {
        glTranslatef( -total_size.x, 0, 0 );
      }
    } else {
      glTranslatef( -line1_size.x, 0, 0 );
      if( minor == X3DFontStyleNode::MIDDLE ) {
        glTranslatef( total_size.x/2, 0, 0 );
      } else if( minor == X3DFontStyleNode::END ) {
        glTranslatef( total_size.x , 0, 0 );
      }
    }
  }
}

void Text::justifyLine( const string& text,
                        X3DFontStyleNode *font ) {
  X3DFontStyleNode::Justification major = font->getMajorJustification();
  X3DFontStyleNode::Alignment alignment = font->getAlignment();
  bool left_to_right = font->isLeftToRight();
  bool top_to_bottom = font->isTopToBottom();
  if( text != "" ) {
    // horizontal text
    if( alignment == X3DFontStyleNode::HORIZONTAL ) {
      H3DFloat size = font->charDimensions( text[0] ).x;
      if( major == X3DFontStyleNode::MIDDLE ) {
        Vec3f dim = font->stringDimensions( text, alignment );
        if( left_to_right ) 
          glTranslatef( -dim.x/2, 0, 0 );
        else 
          glTranslatef( dim.x/2 - size, 0, 0 );
      } else if( major == X3DFontStyleNode::END ) {
        Vec3f dim = font->stringDimensions( text, alignment );
        if( left_to_right) glTranslatef( -dim.x, 0, 0 );
        else  glTranslatef( dim.x - size, 0, 0 );
      } else if( major == X3DFontStyleNode::BEGIN ||
                 major == X3DFontStyleNode::FIRST )
        if( !left_to_right ) glTranslatef( -size, 0, 0 );
      
      // vertical text
    } else if( alignment == X3DFontStyleNode::VERTICAL ) {
      H3DFloat size = font->charDimensions( text[0] ).y;
      if( major == X3DFontStyleNode::MIDDLE ) {
        Vec3f dim = font->stringDimensions( text, alignment );
        if( top_to_bottom ) 
          glTranslatef( 0, dim.y/2 - size - font->descender(), 0 );
        else 
          glTranslatef( 0, -dim.y/2 - font->descender(), 0 );
      } else if( major == X3DFontStyleNode::END ) {
        Vec3f dim = font->stringDimensions( text, alignment );
        if( !top_to_bottom ) glTranslatef( 0, -dim.y, 0 );
        else glTranslatef( 0, dim.y-size, 0 );
      } else if( major == X3DFontStyleNode::BEGIN ||
                 major == X3DFontStyleNode::FIRST )
        if( top_to_bottom ) glTranslatef( 0, -size, 0 );
    }
  }  
}

void Text::moveToNewLine( const string &text, X3DFontStyleNode *font ) {
  X3DFontStyleNode::Alignment alignment = font->getAlignment();
  bool left_to_right = font->isLeftToRight();
  bool top_to_bottom = font->isTopToBottom();
  H3DFloat spacing = font->getSpacing();
  Vec3f dim = font->stringDimensions( text, alignment );
  if( alignment == X3DFontStyleNode::HORIZONTAL ) {
    if( top_to_bottom )
      glTranslatef( 0, -dim.y * spacing, 0 );
    else 
      glTranslatef( 0, dim.y * spacing, 0 );
  } else if( alignment == X3DFontStyleNode::VERTICAL ) {
    if( left_to_right )
      glTranslatef( dim.x * spacing, 0, 0 );
    else 
      glTranslatef( -dim.x * spacing, 0, 0 );
  }
}

void Text::renderTextLine( const string& text,
                           X3DFontStyleNode *font ) {
  glPushMatrix();
  X3DFontStyleNode::Alignment alignment = font->getAlignment();
  bool left_to_right = font->isLeftToRight();
  bool top_to_bottom = font->isTopToBottom();
  
  switch( alignment ) {
  case X3DFontStyleNode::HORIZONTAL: {
    if( left_to_right ) {
      for( unsigned int i = 0; i < text.length(); i++ ) {
        font->renderChar( text[i] );
        H3DFloat size = font->charDimensions( text[i] ).x;
        glTranslatef( size, 0, 0 );
      }
    } else {
      for( unsigned int i = 0; i < text.length(); i++ ) {
        if( i != 0 ) {
          H3DFloat size = font->charDimensions( text[i] ).x;
          glTranslatef( -size, 0, 0 );
        }
        font->renderChar( text[i] );
      }
    }
    break;
  }
  case X3DFontStyleNode::VERTICAL: {
    if( top_to_bottom ) {
      for( unsigned int i = 0; i < text.length(); i++ ) {
        font->renderChar( text[i] );
        H3DFloat size = font->charDimensions( text[i] ).y;
        glTranslatef( 0, -size, 0 );
      }
    } else {
      for( unsigned int i = 0; i < text.length(); i++ ) {
        if( i != 0 ) {
          H3DFloat size = font->charDimensions( text[i] ).y;
          glTranslatef( 0, size, 0 );
        }
        font->renderChar( text[i] );
      }
    }
    break;
  }
  }
  glPopMatrix();
}

void Text::scaleToMaxExtent( const vector< string >& text,
                             X3DFontStyleNode *font ) {
  X3DFontStyleNode::Alignment alignment = font->getAlignment();
  H3DFloat max_extent = maxExtent->getValue();
  const vector< H3DFloat > &line_length = length->getValue();

  if( max_extent > 0 ) {
    if( line_length.size() == 0 ) {
      // no values specified in length field so we just make sure
      // that the total dimension of the string values are within
      // the max extent.
      Vec3f total_size = font->stringDimensions( text, alignment );
      if( alignment == X3DFontStyleNode::HORIZONTAL ) {
        if( total_size.x > max_extent )
          glScalef( max_extent / total_size.x, 1, 1 ); 

      } else if( alignment == X3DFontStyleNode::VERTICAL ) {
          if( total_size.y > max_extent )
            glScalef( 1, max_extent / total_size.y, 1 ); 
      }        
    } else {
      // we have length values so we have to inspect those to know the
      // length of a string value.
      H3DFloat max_length = 0;
      vector< H3DFloat >::const_iterator l = line_length.begin();
      vector< string >::const_iterator s = text.begin();
      
      // find the maximum value in the length field and skip as many 
      // strings.
      for( ; 
           l != line_length.end() && s != text.end();
           l++, s++ ) {
        if( max_length < *l ) max_length = *l;
      }
      
      // check the remaining strings to see if any of them is longer
      // than the maximum length field and scale in order to fit within
      // max extent value.
      if( alignment == X3DFontStyleNode::HORIZONTAL ) {
        while( s != text.end() ) {
          Vec3f dims = font->stringDimensions( *s, alignment );
          if( max_length < dims.x ) max_length = dims.x;
          s++;
        }
        if( max_length > max_extent )
          glScalef( max_extent / max_length, 1, 1 ); 
        
      } else if( alignment == X3DFontStyleNode::VERTICAL ) {
        while( s != text.end() ) {
          Vec3f dims = font->stringDimensions( *s, alignment );
          if( max_length < dims.y ) max_length = dims.y;
          s++;
        }
        if( max_length > max_extent )
          glScalef( 1, max_extent / max_length, 1 ); 
      }
    }
  }
} 

void Text::render() {
  X3DFontStyleNode *font = 
    static_cast< X3DFontStyleNode * >( fontStyle->getValue() );
 
  if( font ) {
    // we will make changes to the transformation matrices so we save
    // the current matrices.
    glPushMatrix();

    font->buildFonts();

    // get the field values
    X3DFontStyleNode::Alignment alignment = font->getAlignment();
    //X3DFontStyleNode::Justification major = font->getMajorJustification();
    const vector< string >& text = stringF->getValue(); 
    //bool top_to_bottom = font->isTopToBottom();
    //bool left_to_right = font->isLeftToRight();
    //H3DFloat max_extent = maxExtent->getValue();
    const vector< H3DFloat > &line_length = length->getValue();

    // scale to make the text fit within max extent value.
    scaleToMaxExtent( text, font );

    // justify the text in the minor alignment.
    justifyMinor( text, font );

    vector< H3DFloat >::const_iterator l = line_length.begin();
    for( vector< string >::const_iterator line = text.begin();
         line != text.end();
         line++ ) {
      
      // translate to the next line. What the next line is depends on the 
      // values in the font node.
      if ( !( alignment == X3DFontStyleNode::VERTICAL && font->isLeftToRight() ) )
        if( line != text.begin() )
          moveToNewLine( *line, font );
      
      glPushMatrix();   
      
      // if we have values in the length field we scale in order to have
      // the text line be the same size as the length.
      if( l != line_length.end() ) {
        if( alignment == X3DFontStyleNode::HORIZONTAL ) 
          glScalef( (*l)/ font->stringDimensions( *line, alignment ).x, 
                    1, 1 );
        else 
          glScalef( 1,
                    (*l)/ font->stringDimensions( *line, alignment ).y, 
                    1 );
        l++;
      }

      // justify the line in the major alignment depending on the justify
      // values in font.
      justifyLine( *line, font ); 
      // render the line of text in the style of the font.
      renderTextLine( *line, font );

      glPopMatrix();

      // translate to the next line. What the next line is depends on the 
      // values in the font node.
      if( alignment == X3DFontStyleNode::VERTICAL && font->isLeftToRight() ) 
        moveToNewLine( *line, font );
      
    }
    glPopMatrix();
  }
}
  
void Text::DisplayList::callList( bool build_list ) {
  Text *text_node = static_cast< Text * >( getOwner() );
  X3DFontStyleNode *font = 
    static_cast< X3DFontStyleNode * >(text_node->fontStyle->getValue());
  if( font && !font->fontsBuilt() ) {
    font->buildFonts();
  }
  X3DGeometryNode::DisplayList::callList( build_list );
}

#ifdef USE_HAPTICS
void Text::traverseSG( TraverseInfo &ti ) {
  // use backface culling if solid is true
  if( solid->getValue() ) useBackFaceCulling( true );
  else useBackFaceCulling( false );

  if( ti.hapticsEnabled() && ti.getCurrentSurface() ) {
#ifdef HAVE_OPENHAPTICS
    ti.addHapticShapeToAll( getOpenGLHapticShape( 
                                                 ti.getCurrentSurface(),
                                                 ti.getAccForwardMatrix() ) );
#endif
  }
}
#endif


void Text::SFBound::update() {
  X3DFontStyleNode *font_style = 
    static_cast< SFFontStyleNode * >( routes_in[0] )->getValue(); 
  const vector<H3DFloat> &length = 
    static_cast< MFFloat * >( routes_in[1] )->getValue();
  H3DFloat max_extent = static_cast< SFFloat * >( routes_in[2] )->getValue();
  const vector< string > &text = static_cast< MFString * >( routes_in[3] )->getValue();

  if( !font_style )
    value = new EmptyBound;
  else {
    font_style->buildFonts();
    X3DFontStyleNode::Alignment alignment = font_style->getAlignment();
    X3DFontStyleNode::Justification minor = font_style->getMinorJustification();
    X3DFontStyleNode::Justification major = font_style->getMajorJustification();
    
    Vec3f box_center( 0, 0, 0 );
    Vec3f text_dims = font_style->stringDimensions( text,
                                                    alignment,
                                                    length );
    if( alignment == X3DFontStyleNode::HORIZONTAL ) {
      if( max_extent > 0 && text_dims.x > max_extent ) text_dims.x = max_extent;
      if( major == X3DFontStyleNode::BEGIN || 
          major == X3DFontStyleNode::FIRST ) 
        box_center.x = text_dims.x / 2;
      else if( major == X3DFontStyleNode::END )
        box_center.x = -text_dims.x / 2;
      if( minor == X3DFontStyleNode::BEGIN ) 
        box_center.y = -text_dims.y / 2;
      else if( minor == X3DFontStyleNode::END )
        box_center.y = text_dims.y / 2;
      else if( minor == X3DFontStyleNode::MIDDLE )
        box_center.y = -font_style->descender();
      else if( minor == X3DFontStyleNode::FIRST ) {
        box_center.y = -text_dims.y / 2;
        if( font_style->isTopToBottom() && text.size() > 0 ) { 
          Vec3f line_dims = font_style->stringDimensions( text[0],
                                                          alignment );
          box_center.y += line_dims.y;
        }
      }
    } else {
      if( max_extent > 0 && text_dims.y > max_extent ) text_dims.y = max_extent;
      if( major == X3DFontStyleNode::BEGIN || 
          major == X3DFontStyleNode::FIRST  ) 
        box_center.y = -text_dims.y / 2;
      else if( major == X3DFontStyleNode::END )
        box_center.y = text_dims.y / 2;
      else if( major == X3DFontStyleNode::MIDDLE )
        box_center.y = font_style->descender();
      if( minor == X3DFontStyleNode::BEGIN || 
          minor == X3DFontStyleNode::FIRST ) 
        box_center.x = text_dims.x / 2;
      else if( minor == X3DFontStyleNode::END )
        box_center.x = -text_dims.x / 2;
    }
    
    if( !font_style->isLeftToRight() ) box_center.x = -box_center.x;
    if( !font_style->isTopToBottom() ) box_center.y = -box_center.y;

    box_center.y += font_style->descender();

    BoxBound *bb= new BoxBound;
    bb->size->setValue( text_dims );
    bb->center->setValue( box_center );
    value = bb;
  }
}
