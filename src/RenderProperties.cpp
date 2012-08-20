//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004-2007, SenseGraphics AB
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
/// \file RenderProperties.cpp
/// \brief CPP file for RenderProperties
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/RenderProperties.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase RenderProperties::database( 
                                         "RenderProperties", 
                                         &(newInstance<RenderProperties>), 
                                         typeid( RenderProperties ),
                                         &X3DAppearanceChildNode::database );

namespace RenderPropertiesInternals {
  FIELDDB_ELEMENT( RenderProperties, depthTestEnabled, INPUT_OUTPUT );
  FIELDDB_ELEMENT( RenderProperties, depthBufferWriteEnabled, INPUT_OUTPUT );
  FIELDDB_ELEMENT( RenderProperties, colorBufferRedWriteEnabled, INPUT_OUTPUT );
  FIELDDB_ELEMENT( RenderProperties, colorBufferGreenWriteEnabled, INPUT_OUTPUT );
  FIELDDB_ELEMENT( RenderProperties, colorBufferBlueWriteEnabled, INPUT_OUTPUT );
  FIELDDB_ELEMENT( RenderProperties, colorBufferAlphaWriteEnabled, INPUT_OUTPUT );
  FIELDDB_ELEMENT( RenderProperties, smoothShading, INPUT_OUTPUT );
  FIELDDB_ELEMENT( RenderProperties, multiPassTransparency, INPUT_OUTPUT );
  FIELDDB_ELEMENT( RenderProperties, alphaFunc, INPUT_OUTPUT );
  FIELDDB_ELEMENT( RenderProperties, alphaFuncValue, INPUT_OUTPUT );
  FIELDDB_ELEMENT( RenderProperties, blendFuncSrcFactorRGB, INPUT_OUTPUT );
  FIELDDB_ELEMENT( RenderProperties, blendFuncSrcFactorAlpha, INPUT_OUTPUT );
  FIELDDB_ELEMENT( RenderProperties, blendFuncDstFactorRGB, INPUT_OUTPUT );
  FIELDDB_ELEMENT( RenderProperties, blendFuncDstFactorAlpha, INPUT_OUTPUT );
  FIELDDB_ELEMENT( RenderProperties, blendEquationRGB, INPUT_OUTPUT );
  FIELDDB_ELEMENT( RenderProperties, blendEquationAlpha, INPUT_OUTPUT );
  FIELDDB_ELEMENT( RenderProperties, blendColor, INPUT_OUTPUT );
  
}


RenderProperties::RenderProperties( Inst< SFNode      >  _metadata,
                                    Inst< DisplayList > _displayList,
                                    Inst< SFBool      > _depthTestEnabled,
                                    Inst< SFBool      > _smoothShading,
                                    Inst< SFBool      > _multiPassTransparency,
                                    Inst< SFBool      > _depthBufferWriteEnabled,
                                    Inst< SFBool      > _colorBufferRedWriteEnabled,
                                    Inst< SFBool      > _colorBufferGreenWriteEnabled,
                                    Inst< SFBool      > _colorBufferBlueWriteEnabled,
                                    Inst< SFBool      > _colorBufferAlphaWriteEnabled,
                                    Inst< SFString    > _alphaFunc,
                                    Inst< SFFloat    > _alphaFuncValue,
                                    Inst< SFString    > _blendFuncSrcFactorRGB,
                                    Inst< SFString    > _blendFuncSrcFactorAlpha,
                                    Inst< SFString    > _blendFuncDstFactorRGB,
                                    Inst< SFString    > _blendFuncDstFactorAlpha,
                                    Inst< SFString    > _blendEquationRGB,
                                    Inst< SFString    > _blendEquationAlpha,
                                    Inst< SFColorRGBA      > _blendColor

 ) :
  X3DAppearanceChildNode( _displayList, _metadata ),
  depthTestEnabled      ( _depthTestEnabled ),
  smoothShading         ( _smoothShading ),
  multiPassTransparency( _multiPassTransparency ),
  depthBufferWriteEnabled( _depthBufferWriteEnabled ),
  colorBufferRedWriteEnabled( _colorBufferRedWriteEnabled ),
  colorBufferGreenWriteEnabled( _colorBufferGreenWriteEnabled ),
  colorBufferBlueWriteEnabled( _colorBufferBlueWriteEnabled ),
  colorBufferAlphaWriteEnabled( _colorBufferAlphaWriteEnabled ),
  alphaFunc( _alphaFunc ),
  alphaFuncValue( _alphaFuncValue ),
  blendFuncSrcFactorRGB( _blendFuncSrcFactorRGB ),
  blendFuncSrcFactorAlpha( _blendFuncSrcFactorAlpha ),
  blendFuncDstFactorRGB( _blendFuncDstFactorRGB ),
  blendFuncDstFactorAlpha( _blendFuncDstFactorAlpha ),
  blendEquationRGB ( _blendEquationRGB ),
  blendEquationAlpha ( _blendEquationAlpha ),
  blendColor( _blendColor ) {

  type_name = "RenderProperties";
  database.initFields( this );
  
  depthTestEnabled->setValue( true );
  smoothShading->setValue( true );
  multiPassTransparency->setValue( true );
  depthBufferWriteEnabled->setValue( true );
  colorBufferRedWriteEnabled->setValue( true );
  colorBufferGreenWriteEnabled->setValue( true );
  colorBufferBlueWriteEnabled->setValue( true );
  colorBufferAlphaWriteEnabled->setValue( true );

  alphaFunc->addValidValue ( "NEVER" );
  alphaFunc->addValidValue ( "LESS" );
  alphaFunc->addValidValue ( "EQUAL" );
  alphaFunc->addValidValue ( "LEQUAL" );
  alphaFunc->addValidValue ( "GREATER" );
  alphaFunc->addValidValue ( "NOTEQAUL" );
  alphaFunc->addValidValue ( "GEQAUL" );
  alphaFunc->addValidValue ( "ALWAYS" );

  alphaFunc->setValue( "ALWAYS" );

  alphaFuncValue->setValue( 0 );

  blendFuncSrcFactorRGB->addValidValue( "ZERO" );
  blendFuncSrcFactorRGB->addValidValue( "ONE" );
  blendFuncSrcFactorRGB->addValidValue( "SRC_COLOR" );
  blendFuncSrcFactorRGB->addValidValue( "ONE_MINUS_SRC_COLOR" );
  blendFuncSrcFactorRGB->addValidValue( "DST_COLOR" );
  blendFuncSrcFactorRGB->addValidValue( "ONE_MINUS_DST_COLOR" );
  blendFuncSrcFactorRGB->addValidValue( "SRC_ALPHA" );
  blendFuncSrcFactorRGB->addValidValue( "ONE_MINUS_SRC_ALPHA" );
  blendFuncSrcFactorRGB->addValidValue( "DST_ALPHA" );
  blendFuncSrcFactorRGB->addValidValue( "ONE_MINUS_DST_ALPHA" );
  blendFuncSrcFactorRGB->addValidValue( "CONSTANT_COLOR" );
  blendFuncSrcFactorRGB->addValidValue( "ONE_MINUS_CONSTANT_COLOR" );
  blendFuncSrcFactorRGB->addValidValue( "CONSTANT_ALPHA" );
  blendFuncSrcFactorRGB->addValidValue( "ONE_MINUS_CONSTANT_ALPHA" );
  blendFuncSrcFactorRGB->addValidValue( "SRC_ALPHA_SATURATE" );

  blendFuncSrcFactorRGB->setValue( "ONE" );

  blendFuncSrcFactorAlpha->addValidValue( "ZERO" );
  blendFuncSrcFactorAlpha->addValidValue( "ONE" );
  blendFuncSrcFactorAlpha->addValidValue( "SRC_COLOR" );
  blendFuncSrcFactorAlpha->addValidValue( "ONE_MINUS_SRC_COLOR" );
  blendFuncSrcFactorAlpha->addValidValue( "DST_COLOR" );
  blendFuncSrcFactorAlpha->addValidValue( "ONE_MINUS_DST_COLOR" );
  blendFuncSrcFactorAlpha->addValidValue( "SRC_ALPHA" );
  blendFuncSrcFactorAlpha->addValidValue( "ONE_MINUS_SRC_ALPHA" );
  blendFuncSrcFactorAlpha->addValidValue( "DST_ALPHA" );
  blendFuncSrcFactorAlpha->addValidValue( "ONE_MINUS_DST_ALPHA" );
  blendFuncSrcFactorAlpha->addValidValue( "CONSTANT_COLOR" );
  blendFuncSrcFactorAlpha->addValidValue( "ONE_MINUS_CONSTANT_COLOR" );
  blendFuncSrcFactorAlpha->addValidValue( "CONSTANT_ALPHA" );
  blendFuncSrcFactorAlpha->addValidValue( "ONE_MINUS_CONSTANT_ALPHA" );
  blendFuncSrcFactorAlpha->addValidValue( "SRC_ALPHA_SATURATE" );

  blendFuncSrcFactorAlpha->setValue( "ONE" );

  blendFuncDstFactorRGB->addValidValue( "ZERO" );
  blendFuncDstFactorRGB->addValidValue( "ONE" );
  blendFuncDstFactorRGB->addValidValue( "SRC_COLOR" );
  blendFuncDstFactorRGB->addValidValue( "ONE_MINUS_SRC_COLOR" );
  blendFuncDstFactorRGB->addValidValue( "DST_COLOR" );
  blendFuncDstFactorRGB->addValidValue( "ONE_MINUS_DST_COLOR" );
  blendFuncDstFactorRGB->addValidValue( "SRC_ALPHA" );
  blendFuncDstFactorRGB->addValidValue( "ONE_MINUS_SRC_ALPHA" );
  blendFuncDstFactorRGB->addValidValue( "DST_ALPHA" );
  blendFuncDstFactorRGB->addValidValue( "ONE_MINUS_DST_ALPHA" );
  blendFuncDstFactorRGB->addValidValue( "CONSTANT_COLOR" );
  blendFuncDstFactorRGB->addValidValue( "ONE_MINUS_CONSTANT_COLOR" );
  blendFuncDstFactorRGB->addValidValue( "CONSTANT_ALPHA" );
  blendFuncDstFactorRGB->addValidValue( "ONE_MINUS_CONSTANT_ALPHA" );

  blendFuncDstFactorRGB->setValue( "ZERO" );

  blendFuncDstFactorAlpha->addValidValue( "ZERO" );
  blendFuncDstFactorAlpha->addValidValue( "ONE" );
  blendFuncDstFactorAlpha->addValidValue( "SRC_COLOR" );
  blendFuncDstFactorAlpha->addValidValue( "ONE_MINUS_SRC_COLOR" );
  blendFuncDstFactorAlpha->addValidValue( "DST_COLOR" );
  blendFuncDstFactorAlpha->addValidValue( "ONE_MINUS_DST_COLOR" );
  blendFuncDstFactorAlpha->addValidValue( "SRC_ALPHA" );
  blendFuncDstFactorAlpha->addValidValue( "ONE_MINUS_SRC_ALPHA" );
  blendFuncDstFactorAlpha->addValidValue( "DST_ALPHA" );
  blendFuncDstFactorAlpha->addValidValue( "ONE_MINUS_DST_ALPHA" );
  blendFuncDstFactorAlpha->addValidValue( "CONSTANT_COLOR" );
  blendFuncDstFactorAlpha->addValidValue( "ONE_MINUS_CONSTANT_COLOR" );
  blendFuncDstFactorAlpha->addValidValue( "CONSTANT_ALPHA" );
  blendFuncDstFactorAlpha->addValidValue( "ONE_MINUS_CONSTANT_ALPHA" );

  blendFuncDstFactorAlpha->setValue( "ZERO" );

  blendEquationRGB->addValidValue( "ADD" );
  blendEquationRGB->addValidValue( "SUBTRACT" );
  blendEquationRGB->addValidValue( "REVERSE_SUBTRACT" );
  blendEquationRGB->addValidValue( "MIN" );
  blendEquationRGB->addValidValue( "MAX" );

  blendEquationRGB->setValue( "ADD" );

  blendEquationAlpha->addValidValue( "ADD" );
  blendEquationAlpha->addValidValue( "SUBTRACT" );
  blendEquationAlpha->addValidValue( "REVERSE_SUBTRACT" );
  blendEquationAlpha->addValidValue( "MIN" );
  blendEquationAlpha->addValidValue( "MAX" );

  blendEquationAlpha->setValue( "ADD" );

  blendColor->setValue( RGBA( 0, 0, 0, 1 ) );

  depthTestEnabled->route( displayList );
  smoothShading->route( displayList );
  multiPassTransparency->route( displayList );
  depthBufferWriteEnabled->route( displayList );
  colorBufferRedWriteEnabled->route( displayList );
  colorBufferGreenWriteEnabled->route( displayList );
  colorBufferBlueWriteEnabled->route( displayList );
  colorBufferAlphaWriteEnabled->route( displayList );
  alphaFunc->route( displayList );
  alphaFuncValue->route( displayList );
  blendFuncSrcFactorRGB->route( displayList );
  blendFuncSrcFactorAlpha->route( displayList );
  blendFuncDstFactorRGB->route( displayList );
  blendFuncDstFactorAlpha->route( displayList );
  blendEquationRGB->route( displayList );
  blendEquationAlpha->route( displayList );
  blendColor->route( displayList );
}


int RenderProperties::getSFactor( const string &sfactor_string, 
                                  GLenum &s_factor ) {
  int res = getDFactor( sfactor_string, s_factor );
  
  if( res == 0 ) return 0;
  else if( sfactor_string == "SRC_ALPHA_SATURATE" ) {
    s_factor = GL_SRC_ALPHA_SATURATE;
  } else {
    return 1;
  }
  return 0;
}

int RenderProperties::getDFactor( const string &dfactor_string, 
                                  GLenum &d_factor ) {

  if( dfactor_string == "ZERO" ) {
    d_factor = GL_ZERO;
  } else if( dfactor_string == "ONE" ) {
    d_factor = GL_ONE;
  } else if( dfactor_string == "SRC_COLOR" ) {
    d_factor = GL_SRC_COLOR;
  } else if( dfactor_string == "ONE_MINUS_SRC_COLOR" ) {
    d_factor = GL_ONE_MINUS_SRC_COLOR;
  } else if( dfactor_string == "DST_COLOR" ) {
    d_factor = GL_DST_COLOR;
  } else if( dfactor_string == "ONE_MINUS_DST_COLOR" ) {
    d_factor = GL_ONE_MINUS_DST_COLOR;
  } else if( dfactor_string == "SRC_ALPHA" ) {
    d_factor = GL_SRC_ALPHA;
  } else if( dfactor_string == "ONE_MINUS_SRC_ALPHA" ) {
    d_factor = GL_ONE_MINUS_SRC_ALPHA;
  } else if( dfactor_string == "DST_ALPHA" ) {
    d_factor = GL_DST_ALPHA;
  } else if( dfactor_string == "ONE_MINUS_DST_ALPHA" ) {
    d_factor = GL_ONE_MINUS_DST_ALPHA;
  } else if( dfactor_string == "CONSTANT_COLOR" ) {
    d_factor = GL_CONSTANT_COLOR;
  } else if( dfactor_string == "ONE_MINUS_CONSTANT_COLOR" ) {
    d_factor = GL_ONE_MINUS_CONSTANT_COLOR;
  } else if( dfactor_string == "CONSTANT_ALPHA" ) {
    d_factor = GL_CONSTANT_ALPHA;
  } else if( dfactor_string == "ONE_MINUS_CONSTANT_ALPHA" ) {
    d_factor = GL_ONE_MINUS_CONSTANT_ALPHA;
  } else {
    return 1;
  }
   
  return 0;
}



int RenderProperties::getAlphaFunc( const string &alpha_func_string, 
                                    GLenum &alpha_func ) {

  if( alpha_func_string == "NEVER" ) {
    alpha_func = GL_NEVER;
  } else if( alpha_func_string == "LESS" ) {
    alpha_func = GL_LESS;
  } else if( alpha_func_string == "EQUAL" ) {
    alpha_func = GL_EQUAL;
  } else if( alpha_func_string == "LEQUAL" ) {
    alpha_func = GL_LEQUAL;
  } else if( alpha_func_string == "GREATER" ) {
    alpha_func = GL_GREATER;
  } else if( alpha_func_string == "NOTEQUAL" ) {
    alpha_func = GL_NOTEQUAL;
  } else if( alpha_func_string == "GEQUAL" ) {
    alpha_func = GL_GEQUAL;
  } else if( alpha_func_string == "ALWAYS" ) {
    alpha_func = GL_ALWAYS;
  } else {
    return 1;
  }
   
  return 0;
} 

int RenderProperties::getBlendEquation( const string &equation_string, GLenum &equation ) {
  if( equation_string == "ADD" ) equation = GL_FUNC_ADD;
  else if( equation_string == "SUBTRACT" ) equation = GL_FUNC_SUBTRACT;
  else if( equation_string == "REVERSE_SUBTRACT" ) equation = GL_FUNC_REVERSE_SUBTRACT;
  else if( equation_string == "MIN" ) equation = GL_MIN;
  else if( equation_string == "MAX" ) equation = GL_MAX;
  else return 1;

  return 0;
}

void RenderProperties::render() {

  if( depthTestEnabled->getValue() ) {
    glEnable( GL_DEPTH_TEST );
  } else {
    glDisable( GL_DEPTH_TEST );
  }

  if( smoothShading->getValue() ) {
    glShadeModel( GL_SMOOTH );
  } else {
    glShadeModel( GL_FLAT );
  }

  if( depthBufferWriteEnabled->getValue() ) {
    glDepthMask( GL_TRUE );
  } else {
    glDepthMask( GL_FALSE );
  }

  glColorMask( colorBufferRedWriteEnabled->getValue(),
               colorBufferGreenWriteEnabled->getValue(),
               colorBufferBlueWriteEnabled->getValue(),
               colorBufferAlphaWriteEnabled->getValue() );


  //  const string &alpha_test = alpha_test->getValue();
  //  if( alphaTest
  glEnable( GL_BLEND );


  // src factors
  const string &blend_func_rgb_src = blendFuncSrcFactorRGB->getValue();
  GLenum s_factor;
  if( getSFactor( blend_func_rgb_src, s_factor ) != 0 ) {
    Console(4) << "Invalid value \"" << blend_func_rgb_src << "\" for blendFuncSrcFactorRGB field in RenderProperties node. Using \"ONE\" instead." << endl;
    s_factor = GL_ONE;
  }

  const string &blend_func_alpha_src = blendFuncSrcFactorAlpha->getValue();
  GLenum s_factor_alpha;
  if( getSFactor( blend_func_alpha_src, s_factor_alpha ) != 0 ) {
    Console(4) << "Invalid value \"" << blend_func_alpha_src << "\" for blendFuncSrcFactorAlpha field in RenderProperties node. Using \"ONE\" instead." << endl;
    s_factor_alpha = GL_ONE;
  }


  // dst factors
  const string &blend_func_rgb_dst = blendFuncDstFactorRGB->getValue();
  GLenum d_factor;
  if( getDFactor( blend_func_rgb_dst, d_factor ) != 0 ) {
    Console(4) << "Invalid value \"" << blend_func_rgb_dst << "\" for blendFuncDstFactorRGB field in RenderProperties node. Using \"ZERO\" instead." << endl;
    d_factor = GL_ZERO;
  }

  const string &blend_func_alpha_dst = blendFuncDstFactorAlpha->getValue();
  GLenum d_factor_alpha;
  if( getDFactor( blend_func_alpha_dst, d_factor_alpha ) != 0 ) {
    Console(4) << "Invalid value \"" << blend_func_alpha_dst << "\" for blendFuncDstFactorAlpha field in RenderProperties node. Using \"ZERO\" instead." << endl;
    d_factor_alpha = GL_ZERO;
  }

  glBlendFuncSeparate( s_factor, d_factor, s_factor_alpha, d_factor_alpha );


  // blendEquation
  const string &blend_equation_rgb = blendEquationRGB->getValue();
  GLenum equation_rgb = GL_FUNC_ADD;
  if( getBlendEquation( blend_equation_rgb, equation_rgb ) != 0 ) {
    Console(4) << "Invalid value \"" << blend_equation_rgb << "\" for blendEquationRGB field in RenderProperties node. Using \"ADD\" instead." << endl;
    equation_rgb = GL_FUNC_ADD;
  }

  const string &blend_equation_alpha = blendEquationAlpha->getValue();
  GLenum equation_alpha = GL_FUNC_ADD;
  if( getBlendEquation( blend_equation_alpha, equation_alpha ) != 0 ) {
    Console(4) << "Invalid value \"" << blend_equation_alpha << "\" for blendEquationAlpha field in RenderProperties node. Using \"ADD\" instead." << endl;
    equation_alpha = GL_FUNC_ADD;
  }

  
  glBlendEquationSeparate( equation_rgb,
                           equation_alpha );

  const RGBA &blend_color = blendColor->getValue();

  // blendColor
  glBlendColor( blend_color.r, blend_color.g, blend_color.b, blend_color.a );

  // alpha test func


  const string &alpha_func = alphaFunc->getValue();
  GLenum gl_alpha_func = GL_ALWAYS;
  if( getAlphaFunc( alpha_func, gl_alpha_func ) != 0 ) {
    Console(4) << "Invalid value \"" << alpha_func << "\" for alphaFunc field in RenderProperties node. Using \"ALWAYS\" instead." << endl;
    gl_alpha_func = GL_ALWAYS;
  }

  if( gl_alpha_func != GL_ALWAYS ) {
    glEnable( GL_ALPHA_TEST );
    glAlphaFunc( gl_alpha_func, alphaFuncValue->getValue() );
  }


}
