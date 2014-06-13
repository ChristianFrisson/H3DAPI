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
/// \file Text.cpp
/// \brief CPP file for Text, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/Text.h>
#include <H3D/FontStyle.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase Text::database( 
                               "Text", 
                               &(newInstance<Text>), 
                               typeid( Text ),
                               &X3DGeometryNode::database );

AutoRef<FontStyle > Text::default_font_style( NULL );

namespace TextInternals {
  FIELDDB_ELEMENT( Text, fontStyle, INPUT_OUTPUT );
  FIELDDB_ELEMENT( Text, length, INPUT_OUTPUT );
  FIELDDB_ELEMENT( Text, maxExtent, INPUT_OUTPUT );
  FieldDBInsert string( INPUT_OUTPUT( &Text::database, "string", &Text::stringF ) );
  FIELDDB_ELEMENT( Text, lineBounds, OUTPUT_ONLY );
  FIELDDB_ELEMENT( Text, origin, OUTPUT_ONLY );
  FIELDDB_ELEMENT( Text, textBounds, OUTPUT_ONLY );
  FIELDDB_ELEMENT( Text, solid, INPUT_OUTPUT );
}

Text::Text( Inst< SFNode          > _metadata,
            Inst< SFBound         > _bound,
            Inst< DisplayList     > _displayList,
            Inst< SFFontStyleNode > _fontStyle,
            Inst< MFFloat         > _length,
            Inst< SFFloat         > _maxExtent,
            Inst< MFString        > _string,
            Inst< MFVec2f         > _lineBounds,
            Inst< SFVec3f         > _origin,
            Inst< SFVec2f         > _textBounds,
            Inst< SFBool          > _solid,
            Inst< OutputUpdater   > _outputUpdater ) :
  X3DGeometryNode( _metadata, _bound, _displayList ),
  fontStyle( _fontStyle ),
  length   ( _length    ),
  maxExtent( _maxExtent ),
  stringF  ( _string   ),
  lineBounds( _lineBounds ),
  origin( _origin ),
  textBounds( _textBounds ),
  solid    ( _solid     ),
  outputUpdater( _outputUpdater ) {

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

  outputUpdater->setName( "outputUpdater" );
  outputUpdater->setOwner( this );
  fontStyle->route( outputUpdater );
  stringF->route( outputUpdater );

  // initialize static default font.
  if( !default_font_style.get() ) {
    default_font_style.reset( new FontStyle );
  }
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
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  X3DFontStyleNode::Alignment alignment = font->getAlignment();
  bool left_to_right = font->isLeftToRight();
  bool top_to_bottom = font->isTopToBottom();
  
  switch( alignment ) {
  case X3DFontStyleNode::HORIZONTAL: {
    if( left_to_right ) {
      // Using special function for normal case of horizontal lines since this
      // can be optimized by FTGL and render much faster than doing it 
      // character by character.
      font->renderHorizontalLine( text );
    } else {
      for( unsigned int i = 0; i < text.length(); ++i ) {
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
      for( unsigned int i = 0; i < text.length(); ++i ) {
        font->renderChar( text[i] );
        H3DFloat size = font->charDimensions( text[i] ).y;
        glTranslatef( 0, -size, 0 );
      }
    } else {
      for( unsigned int i = 0; i < text.length(); ++i ) {
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
  glMatrixMode(GL_MODELVIEW);
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
           ++l, ++s ) {
        if( max_length < *l ) max_length = *l;
      }
      
      // check the remaining strings to see if any of them is longer
      // than the maximum length field and scale in order to fit within
      // max extent value.
      if( alignment == X3DFontStyleNode::HORIZONTAL ) {
        while( s != text.end() ) {
          Vec3f dims = font->stringDimensions( *s, alignment );
          if( max_length < dims.x ) max_length = dims.x;
          ++s;
        }
        if( max_length > max_extent )
          glScalef( max_extent / max_length, 1, 1 ); 
        
      } else if( alignment == X3DFontStyleNode::VERTICAL ) {
        while( s != text.end() ) {
          Vec3f dims = font->stringDimensions( *s, alignment );
          if( max_length < dims.y ) max_length = dims.y;
          ++s;
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
 
  if( !font ) {
    font = default_font_style.get();
  }

  // we will make changes to the transformation matrices so we save
  // the current matrices.
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();

  try {
    font->buildFonts();
  } catch( const Exception::H3DException &e ) {
    Console(3) << "Warning: Text node " << getName() << " will not be rendered. "
               << "FontStyle::buildFonts message is: " << e.message << endl;
  }

  if( font->fontsBuilt() ) {
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
         ++line ) {
      
      // translate to the next line. What the next line is depends on the 
      // values in the font node.
      if ( !( alignment == X3DFontStyleNode::VERTICAL && font->isLeftToRight() ) )
        if( line != text.begin() )
          moveToNewLine( *line, font );
      
      glMatrixMode(GL_MODELVIEW);
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
        ++l;
      }

      // justify the line in the major alignment depending on the justify
      // values in font.
      justifyLine( *line, font ); 
      // render the line of text in the style of the font.
      renderTextLine( *line, font );
    
      glMatrixMode(GL_MODELVIEW);
      glPopMatrix();
      // translate to the next line. What the next line is depends on the 
      // values in the font node.
      if( alignment == X3DFontStyleNode::VERTICAL && font->isLeftToRight() ) 
        moveToNewLine( *line, font );
    }
  }
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
}
  
void Text::DisplayList::callList( bool build_list ) {
  Text *text_node = static_cast< Text * >( getOwner() );
  X3DFontStyleNode *font = 
    static_cast< X3DFontStyleNode * >(text_node->fontStyle->getValue());
  if( font && !font->fontsBuilt() ) {
    try {
      font->buildFonts();
    } catch( const Exception::H3DException &e ) {
    }
  }
  X3DGeometryNode::DisplayList::callList( build_list );
}

void Text::traverseSG( TraverseInfo &ti ) {
  X3DGeometryNode::traverseSG( ti );
  // use backface culling if solid is true
  if( solid->getValue() ) useBackFaceCulling( true );
  else useBackFaceCulling( false );
}

void Text::SFBound::update() {
  X3DFontStyleNode *font_style = 
    static_cast< SFFontStyleNode * >( routes_in[0] )->getValue(); 
  const vector<H3DFloat> &length = 
    static_cast< MFFloat * >( routes_in[1] )->getValue();
  H3DFloat max_extent = static_cast< SFFloat * >( routes_in[2] )->getValue();
  const vector< string > &text = static_cast< MFString * >( routes_in[3] )->getValue();

  if( !font_style ) font_style = default_font_style.get();
  

  try {
    font_style->buildFonts();
  } catch( const Exception::H3DException &e ) {
    // Not doing anything here.
  }

  if( font_style->fontsBuilt() ) {
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
  
    BoxBound *bb= new BoxBound;
    box_center.y += font_style->descender();
    bb->size->setValue( text_dims );
    bb->center->setValue( box_center );
    value = bb;
  } else
    value = NULL;
}

bool Text::lineIntersect(
                  const Vec3f &from, 
                  const Vec3f &to,    
                  LineIntersectResult &result ) {
  if( result.detect_pt_device && !result.hasCurrentPointingDevice() ) {
    // If this function is called because we are detecting pointing device
    // sensors but there is no current one then there is no use in doing
    // line intersect on the actual triangles.
    return false;
  }

  bool returnValue = false;
  Bound * the_bound = bound->getValue();
  if( the_bound ) {
      returnValue = the_bound->lineSegmentIntersect( from, to );
      if( returnValue ) {
        IntersectionInfo temp_result;
        temp_result.point = Vec3f( 0, 0, 0 );
        temp_result.normal = Vec3f( 0, 0, 1 );
        result.addResults( temp_result, this );
        result.addPtDevMap();
      }
  }
  return returnValue;
}

void Text::OutputUpdater::update() {
  Text * text_node = static_cast< Text * >( getOwner() );
  // Do not bother if length and maxExtent are not defaults
  if ( text_node->length->getValue().size() != 0 || 
       text_node->maxExtent->getValue() != 0 ) 
    return;

  BoxBound * bb = dynamic_cast< BoxBound *>( text_node->bound->getValue() );
  if ( bb == NULL ) return;

  X3DFontStyleNode * font_style = 
    static_cast< SFFontStyleNode * >( routes_in[0] )->getValue();
  const vector< string > &text = 
    static_cast< MFString * >( routes_in[1] )->getValue();
  
  const Vec3f &size = bb->size->getValue();
  Vec3f center = bb->center->getValue();

  // update textBounds
  text_node->textBounds->setValue( 
    Vec2f(size.x, size.y), text_node->id );
  // update origin
  text_node->origin->setValue( 
    Vec3f(center.x-(size.x/2), center.y+(size.y/2), 0), text_node->id );
  // update line bounds
  text_node->updateLineBounds();
}

void Text::updateLineBounds() {  
  BoxBound * bb = dynamic_cast< BoxBound * >( bound->getValue() );
  if ( bb == NULL ) return;

  X3DFontStyleNode * font_style 
    = static_cast< X3DFontStyleNode * >( fontStyle->getValue() );
  if( !font_style ) font_style = default_font_style.get();
  X3DFontStyleNode::Alignment alignment 
    = font_style->getAlignment();
  X3DFontStyleNode::Justification justification 
    = font_style->getMajorJustification();

  vector< string > text = stringF->getValue();
  lineBounds->clear( id );

  if ( alignment == X3DFontStyleNode::HORIZONTAL ) {
    // Case of horizontal alignment
    H3DFloat offset = bb->size->getValue().x;
    for ( vector<string>::const_iterator i = text.begin(); 
      i != text.end(); ++i ) {
      const string &line = *i;
      Vec3f dim = font_style->
        stringDimensions( line, X3DFontStyleNode::HORIZONTAL );
      Vec2f line_bound( 0, 0 );

      switch( justification ) {
        case X3DFontStyleNode::BEGIN:
        case X3DFontStyleNode::FIRST:
            line_bound.x = dim.x;
            break;
        case X3DFontStyleNode::END:
            line_bound.x = offset;
            break;
        case X3DFontStyleNode::MIDDLE:
            line_bound.x = dim.x + (offset-dim.x)/2;
            break;
      }
      line_bound.y = dim.y;
      lineBounds->push_back( line_bound, id );
    }
  } else {
    // Case of vertical alignment
    H3DFloat offset = bb->size->getValue().y;
    for ( vector<string>::const_iterator i = text.begin(); 
      i != text.end(); ++i ) {
      const string &line = *i;
      Vec3f dim = font_style->
        stringDimensions( line, X3DFontStyleNode::VERTICAL );
      Vec2f line_bound( 0, 0 );

      switch( justification ) {
        case X3DFontStyleNode::BEGIN:
        case X3DFontStyleNode::FIRST:
            line_bound.y = dim.y;
            break;
        case X3DFontStyleNode::END:
            line_bound.y = offset;
            break;
        case X3DFontStyleNode::MIDDLE:
            line_bound.y = dim.y + (offset-dim.y)/2;
            break;
      }
      line_bound.x = dim.x;
      lineBounds->push_back( line_bound, id );
    }
  }
}
