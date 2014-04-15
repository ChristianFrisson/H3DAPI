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
/// \file TextureProperties.cpp
/// \brief CPP file for TextureProperties, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/TextureProperties.h>

using namespace H3D;

H3DNodeDatabase TextureProperties::database( 
        "TextureProperties", 
        &newInstance< TextureProperties>,
        typeid( TextureProperties ),
        &X3DNode::database 
        );

namespace TexturePropertiesInternals {
  FIELDDB_ELEMENT( TextureProperties, anisotropicDegree, INPUT_OUTPUT );
  FIELDDB_ELEMENT( TextureProperties, borderColor, INPUT_OUTPUT );
  FIELDDB_ELEMENT( TextureProperties, borderWidth, INPUT_OUTPUT );
  FIELDDB_ELEMENT( TextureProperties, boundaryModeS, INPUT_OUTPUT );
  FIELDDB_ELEMENT( TextureProperties, boundaryModeT, INPUT_OUTPUT );
  FIELDDB_ELEMENT( TextureProperties, boundaryModeR, INPUT_OUTPUT );
  FIELDDB_ELEMENT( TextureProperties, magnificationFilter, INPUT_OUTPUT );
  FIELDDB_ELEMENT( TextureProperties, minificationFilter, INPUT_OUTPUT );
  FIELDDB_ELEMENT( TextureProperties, textureCompression, INPUT_OUTPUT );
  FIELDDB_ELEMENT( TextureProperties, texturePriority, INPUT_OUTPUT );
  FIELDDB_ELEMENT( TextureProperties, generateMipMaps, INPUT_OUTPUT );
  FIELDDB_ELEMENT( TextureProperties, textureTransferScale, INPUT_OUTPUT );
  FIELDDB_ELEMENT( TextureProperties, textureTransferBias, INPUT_OUTPUT );
  FIELDDB_ELEMENT( TextureProperties, textureCompareMode, INPUT_OUTPUT );
  FIELDDB_ELEMENT( TextureProperties, textureCompareFailValue, INPUT_OUTPUT );
  FIELDDB_ELEMENT( TextureProperties, textureType, INPUT_OUTPUT );
}

TextureProperties::TextureProperties( 
                       Inst< SFNode  > _metadata             ,
                       Inst< SFFloat  > _anisotropicDegree   ,
                       Inst< SFColorRGBA > _borderColor      ,
                       Inst< SFInt32  > _borderWidth         ,
                       Inst< SFString > _boundaryModeS       ,
                       Inst< SFString > _boundaryModeT       ,
                       Inst< SFString > _boundaryModeR       ,
                       Inst< SFString  > _magnificationFilter,
                       Inst< SFString  > _minificationFilter ,
                       Inst< SFString > _textureCompression  ,
                       Inst< SFFloat > _texturePriority       ,
                       Inst< SFBool  > _generateMipMaps,
                       Inst< SFVec4f > _textureTransferScale,
                       Inst< SFVec4f > _textureTransferBias,
                       Inst< SFString > _textureCompareMode,
                       Inst< SFFloat  > _textureCompareFailValue,
                       Inst< SFString > _textureType ):
  X3DNode( _metadata ),
  anisotropicDegree ( _anisotropicDegree  ),
  borderColor ( _borderColor  ),
  borderWidth( _borderWidth ),
  boundaryModeS( _boundaryModeS ),
  boundaryModeT( _boundaryModeT ),
  boundaryModeR( _boundaryModeR ),
  magnificationFilter( _magnificationFilter ),
  minificationFilter( _minificationFilter ),
  textureCompression( _textureCompression ),
  texturePriority( _texturePriority ),
  generateMipMaps( _generateMipMaps ),
  textureTransferScale( _textureTransferScale ),
  textureTransferBias( _textureTransferBias ),
  textureCompareMode( _textureCompareMode ),
  textureCompareFailValue( _textureCompareFailValue ),
  textureType( _textureType ),
  propertyChanged( new Field ) {

  type_name = "TextureProperties";

  database.initFields( this );

  anisotropicDegree->setValue( 1.0f );
  borderColor->setValue( RGBA( 0, 0, 0, 0 ) );
  borderWidth->setValue( 0 );
  boundaryModeS->addValidValue( "REPEAT" );
  boundaryModeS->addValidValue( "CLAMP" );
  boundaryModeS->addValidValue( "CLAMP_TO_EDGE" );
  boundaryModeS->addValidValue( "CLAMP_TO_BOUNDARY" );
  boundaryModeS->addValidValue( "MIRRORED_REPEAT" );
  boundaryModeS->setValue( "REPEAT" );
  boundaryModeT->addValidValue( "REPEAT" );
  boundaryModeT->addValidValue( "CLAMP" );
  boundaryModeT->addValidValue( "CLAMP_TO_EDGE" );
  boundaryModeT->addValidValue( "CLAMP_TO_BOUNDARY" );
  boundaryModeT->addValidValue( "MIRRORED_REPEAT" );
  boundaryModeT->setValue( "REPEAT" );
  boundaryModeR->addValidValue( "REPEAT" );
  boundaryModeR->addValidValue( "CLAMP" );
  boundaryModeR->addValidValue( "CLAMP_TO_EDGE" );
  boundaryModeR->addValidValue( "CLAMP_TO_BOUNDARY" );
  boundaryModeR->addValidValue( "MIRRORED_REPEAT" );
  boundaryModeR->setValue( "REPEAT" );
  magnificationFilter->addValidValue( "FASTEST" );
  magnificationFilter->addValidValue( "NEAREST_PIXEL" );
  magnificationFilter->addValidValue( "AVG_PIXEL" );
  magnificationFilter->addValidValue( "DEFAULT" );
  magnificationFilter->addValidValue( "NICEST" );
  magnificationFilter->setValue( "FASTEST" );
  minificationFilter->addValidValue( "FASTEST" );
  minificationFilter->addValidValue( "NEAREST_PIXEL" );
  minificationFilter->addValidValue( "AVG_PIXEL" );
  minificationFilter->addValidValue( "DEFAULT" );
  minificationFilter->addValidValue( "NICEST" );
  minificationFilter->setValue( "FASTEST" );
  textureCompression->addValidValue( "DEFAULT" );
  textureCompression->addValidValue( "FASTEST" );
  textureCompression->addValidValue( "HIGH" );
  textureCompression->addValidValue( "MEDIUM" );
  textureCompression->addValidValue( "NICEST" );
  textureCompression->addValidValue( "LOW" );
  textureCompression->setValue( "FASTEST" );
  texturePriority->setValue( 1.0f );
  generateMipMaps->setValue( false );
  textureTransferScale->setValue( Vec4f( 1, 1, 1, 1 ) );
  textureTransferBias->setValue( Vec4f( 0, 0, 0, 0 ) );
  textureCompareMode->addValidValue( "NONE" );
  textureCompareMode->addValidValue( "GEQUAL" );
  textureCompareMode->addValidValue( "LEQUAL" );
  textureCompareMode->setValue( "NONE" );
  textureCompareFailValue->setValue( 0 );
  textureType->addValidValue( "NORMAL" );
  textureType->addValidValue( "2D_RECTANGLE" );
  textureType->addValidValue( "2D_ARRAY" );
  textureType->setValue( "NORMAL" );

  propertyChanged->setName( "propertyChanged" );
  anisotropicDegree->route( propertyChanged );
  borderColor->route( propertyChanged );
  borderWidth->route( propertyChanged );
  boundaryModeS->route( propertyChanged );
  boundaryModeT->route( propertyChanged );
  boundaryModeR->route( propertyChanged );
  magnificationFilter->route( propertyChanged );
  minificationFilter->route( propertyChanged );
  textureCompression->route( propertyChanged );
  texturePriority->route( propertyChanged );
  generateMipMaps->route( propertyChanged );
  textureTransferScale->route( propertyChanged );
  textureTransferBias->route( propertyChanged );
  textureCompareMode->route( propertyChanged );
  textureCompareFailValue->route( propertyChanged );
  textureType->route( propertyChanged );
}

void TextureProperties::renderTextureProperties( GLenum texture_target ) {
  // anisotropicDegree
  H3DFloat anisotropic = anisotropicDegree->getValue();
  if( anisotropic < 1 ) {
    Console(3) << "Warning: Invalid anisotropicDegree \"" << anisotropic 
               << "\". Must be greater that 1.0 (in " << getName()
               << ")" << endl;
    
  } else {
    if( GLEW_EXT_texture_filter_anisotropic ) {
      glTexParameterf(texture_target, 
                      GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropic );
    }
  }
  // border color
  const RGBA &border_color = borderColor->getValue();
  GLfloat c[4];
  c[0] = border_color.r;
  c[1] = border_color.g;
  c[2] = border_color.b;
  c[3] = border_color.a;
  glTexParameterfv( texture_target, GL_TEXTURE_BORDER_COLOR, c );

  // boundary modes

  // S
  const string &s_mode = boundaryModeS->getValue();
  if( s_mode == "CLAMP" ) {
    glTexParameteri( texture_target, GL_TEXTURE_WRAP_S, GL_CLAMP );
  } else if( s_mode == "CLAMP_TO_EDGE" ) {
    glTexParameteri( texture_target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
  } else if( s_mode == "CLAMP_TO_BOUNDARY" ) {
    if( GLEW_ARB_texture_border_clamp ) {
      glTexParameteri( texture_target, GL_TEXTURE_WRAP_S, 
                       GL_CLAMP_TO_BORDER );
    } else {
      Console(3) << "Warning: MIRRORED_REPEAT boundary mode not "
                 << "supported by your graphics card (in " << getName()
                 << ")" << endl;
    }
  } else if( s_mode == "MIRRORED_REPEAT" ) {
    if( GLEW_ARB_texture_mirrored_repeat ) {
      glTexParameteri( texture_target, GL_TEXTURE_WRAP_S, 
                       GL_MIRRORED_REPEAT_ARB );
    } else {
      Console(3) << "Warning: MIRRORED_REPEAT boundary mode not "
                 << "supported by your graphics card (in " << getName()
                 << ")" << endl;
    }
  } else if( s_mode == "REPEAT" ) {
    glTexParameteri( texture_target, GL_TEXTURE_WRAP_S, GL_REPEAT );
  } else {
    Console(3) << "Warning: Invalid boundary mode \"" << s_mode 
               << "\" in TextureProperties "
               << " node for texture node(" << getName() << ")." << endl; 
  }

  // T
  const string &t_mode = boundaryModeT->getValue();
  if( t_mode == "CLAMP" ) {
    glTexParameteri( texture_target, GL_TEXTURE_WRAP_T, GL_CLAMP );
  } else if( t_mode == "CLAMP_TO_EDGE" ) {
    glTexParameteri( texture_target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
  } else if( t_mode == "CLAMP_TO_BOUNDARY" ) {
    if( GLEW_ARB_texture_border_clamp ) {
      glTexParameteri( texture_target, GL_TEXTURE_WRAP_T, 
                       GL_CLAMP_TO_BORDER );
    } else {
      Console(3) << "Warning: MIRRORED_REPEAT boundary mode not "
                 << "supported by your graphics card (in " << getName()
                 << ")" << endl;
    }
  } else if( t_mode == "MIRRORED_REPEAT" ) {
    if( GLEW_ARB_texture_mirrored_repeat ) {
      glTexParameteri( texture_target, GL_TEXTURE_WRAP_T, 
                       GL_MIRRORED_REPEAT_ARB );
    } else {
      Console(3) << "Warning: MIRRORED_REPEAT boundary mode not "
                 << "supported by your graphics card (in " << getName()
                 << ")" << endl;
    }
  } else if( t_mode == "REPEAT" ) {
    glTexParameteri( texture_target, GL_TEXTURE_WRAP_T, GL_REPEAT );
  } else {
    Console(3) << "Warning: Invalid boundary mode \"" << t_mode 
               << "\" in TextureProperties "
               << " node for texture node(" << getName() << ")." << endl; 
  }


  // R
  const string &r_mode = boundaryModeR->getValue();
  if( r_mode == "CLAMP" ) {
    glTexParameteri( texture_target, GL_TEXTURE_WRAP_R, GL_CLAMP );
  } else if( r_mode == "CLAMP_TO_EDGE" ) {
    glTexParameteri( texture_target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE );
  } else if( r_mode == "CLAMP_TO_BOUNDARY" ) {
    if( GLEW_ARB_texture_border_clamp ) {
      glTexParameteri( texture_target, GL_TEXTURE_WRAP_R, 
                       GL_CLAMP_TO_BORDER );
    } else {
      Console(3) << "Warning: MIRRORED_REPEAT boundary mode not "
                 << "supported by your graphics card (in " << getName()
                 << ")" << endl;
    }
  } else if( r_mode == "MIRRORED_REPEAT" ) {
    if( GLEW_ARB_texture_mirrored_repeat ) {
      glTexParameteri( texture_target, GL_TEXTURE_WRAP_R, 
                       GL_MIRRORED_REPEAT_ARB );
    } else {
      Console(3) << "Warning: MIRRORED_REPEAT boundary mode not "
                 << "supported by your graphics card (in " << getName()
                 << ")" << endl;
    }
  } else if( r_mode == "REPEAT" ) {
    glTexParameteri( texture_target, GL_TEXTURE_WRAP_R, GL_REPEAT );
  } else {
    Console(3) << "Warning: Invalid boundary mode \"" << r_mode 
               << "\" in TextureProperties "
               << " node for texture node(" << getName() << ")." << endl; 
  }

  // magnification filter
  const string &mag_filter = 
    magnificationFilter->getValue();

  if( mag_filter == "NEAREST_PIXEL" ||
      mag_filter == "FASTEST" )
    glTexParameteri( texture_target, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
  else if( mag_filter == "AVG_PIXEL" ||
           mag_filter == "DEFAULT" ||
           mag_filter == "NICEST" ) {
    glTexParameteri( texture_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  } else  {
    Console(3) << "Warning: Invalid magnification filter \"" << mag_filter 
               << "\" in TextureProperties "
               << " node for texture node(" << getName() << ")." << endl; 
  }

  // minification filter
  const string &min_filter = 
    minificationFilter->getValue();

  if( min_filter == "NEAREST_PIXEL" ||
      min_filter == "FASTEST" )
    glTexParameteri( texture_target, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
  else if( min_filter == "AVG_PIXEL" ||
           min_filter == "DEFAULT" ||
           min_filter == "NICEST" ) {
    glTexParameteri( texture_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
  } else if( min_filter == "AVG_PIXEL_AVG_MIPMAP") {
    glTexParameteri( texture_target, GL_TEXTURE_MIN_FILTER, 
                     GL_LINEAR_MIPMAP_LINEAR );
  } else if( min_filter == "AVG_PIXEL_NEAREST_MIPMAP") {
    glTexParameteri( texture_target, GL_TEXTURE_MIN_FILTER, 
                     GL_LINEAR_MIPMAP_NEAREST );
  } else if( min_filter == "NEAREST_PIXEL_AVG_MIPMAP") {
    glTexParameteri( texture_target, GL_TEXTURE_MIN_FILTER, 
                     GL_NEAREST_MIPMAP_LINEAR );
  } else if( min_filter == "NEAREST_PIXEL_NEAREST_MIPMAP") {
    glTexParameteri( texture_target, GL_TEXTURE_MIN_FILTER, 
                     GL_NEAREST_MIPMAP_NEAREST );
  } else {
    Console(3) << "Warning: Invalid minification filter \"" << min_filter 
               << "\" in TextureProperties "
               << " node for texture node(" << getName() << ")." << endl; 
  }

  // priority
  H3DFloat priority = texturePriority->getValue();
  if( priority < 0 || priority > 1 ) {
    Console(3) << "Warning: Invalid texturePriority \"" << priority
               << "\". Must be in range [0, 1] (in " << getName()
               << ")" << endl;
        
  } else {
    glTexParameterf(texture_target, 
                    GL_TEXTURE_PRIORITY, priority );
  }

  // compression
  if( GLEW_ARB_texture_compression ) {
    // compression
    const string &compression = 
      textureCompression->getValue();

    if( compression == "DEFAULT" ) {
      glHint( GL_TEXTURE_COMPRESSION_HINT_ARB, GL_DONT_CARE );
    } else if( compression == "FASTEST" ||
               compression == "HIGH" ||
               compression == "MEDIUM" ) {
      glHint( GL_TEXTURE_COMPRESSION_HINT_ARB, GL_FASTEST );
    } else if( compression == "NICEST" ||
               compression == "LOW"  ) {
      glHint( GL_TEXTURE_COMPRESSION_HINT_ARB, GL_NICEST );
    } else {
      Console(3) << "Warning: Invalid textureCompression mode \"" 
                 << compression  << "\" in TextureProperties "
                 << " node for texture node(" << getName() << ")." << endl; 
    }
  }

  // textureCompareMode
  if( GLEW_ARB_shadow ) {
    const string &compare_mode = textureCompareMode->getValue();
    if( compare_mode == "LEQUAL" ) {
      glTexParameteri( texture_target, GL_TEXTURE_COMPARE_MODE_ARB, GL_COMPARE_R_TO_TEXTURE_ARB );
      glTexParameteri( texture_target, GL_TEXTURE_COMPARE_FUNC_ARB, GL_LEQUAL );
    } else if( compare_mode == "GEQUAL" ) {
      glTexParameteri( texture_target, GL_TEXTURE_COMPARE_MODE_ARB, GL_COMPARE_R_TO_TEXTURE_ARB );
      glTexParameteri( texture_target, GL_TEXTURE_COMPARE_FUNC_ARB, GL_GEQUAL );
    } else {
      if( compare_mode != "NONE" ) {
        Console(3) << "Warning: Invalid textureCompareMode: " << compare_mode 
                   << " in TextureProperties node for texture node(" << getName() << ")." << endl; 
      }
      glTexParameteri( texture_target, GL_TEXTURE_COMPARE_MODE_ARB, GL_NONE );
    }
  } else {
    Console(3) << "Warning: textureCompareMode is not supported by your graphics card. " 
               << "Requires the ARB_shadow extension (in TextureProperties node). " << endl;
  }

  GLfloat fail_value = textureCompareFailValue->getValue();
  // textureCompareFailValue
  if( GLEW_ARB_shadow_ambient ) {
    glTexParameterf( texture_target, GL_TEXTURE_COMPARE_FAIL_VALUE_ARB, fail_value );
  } else {
    if( fail_value != 0 ) {
      Console(3) << "Warning: textureCompareFailValue is not supported by your graphics card. " 
                 << "Requires the ARB_shadow_ambient extension (in TextureProperties node). " << endl;
    }
  }

}

