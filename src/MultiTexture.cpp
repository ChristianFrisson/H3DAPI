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
/// \file MultiTexture.cpp
/// \brief CPP file for MultiTexture, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/MultiTexture.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase MultiTexture::database( 
                                       "MultiTexture", 
                                       &(newInstance<MultiTexture>), 
                                       typeid( MultiTexture ),
                                       &X3DTextureNode::database );

namespace MultiTextureInternals {
  FIELDDB_ELEMENT( MultiTexture, alpha, INPUT_OUTPUT );
  FIELDDB_ELEMENT( MultiTexture, color, INPUT_OUTPUT );
  FIELDDB_ELEMENT( MultiTexture, function, INPUT_OUTPUT );
  FIELDDB_ELEMENT( MultiTexture, mode, INPUT_OUTPUT );
  FIELDDB_ELEMENT( MultiTexture, source, INPUT_OUTPUT );
  FIELDDB_ELEMENT( MultiTexture, texture, INPUT_OUTPUT );
}

MultiTexture::MultiTexture( 
                           Inst< DisplayList > _displayList,
                           Inst< SFFloat  >  _alpha,
                           Inst< SFColor  >  _color,
                           Inst< MFString >  _function,
                           Inst< SFNode   >  _metadata,
                           Inst< MFString >  _mode,
                           Inst< MFString >  _source,
                           Inst< MFTexture >  _texture ):
  X3DTextureNode( _displayList, _metadata ),
  alpha( _alpha ),
  color( _color ),
  function( _function ),
  mode( _mode ),
  source( _source ),
  texture( _texture ) {

  alpha->setValue( 1 );
  color->setValue( RGB( 1, 1, 1 ) );
  type_name = "MultiTexture";

  database.initFields( this );

  alpha->route( displayList );
  color->route( displayList );
  function->route( displayList );
  mode->route( displayList );
  source->route( displayList );
  texture->route( displayList );
}

void MultiTexture::render() {
  if( !GLEW_ARB_multitexture ) {
    Console(4) << "Warning: Your graphic card driver does not support "
               << "the ARB_multitexture extension so you cannot use the MultiTexture "
               << "node. " << endl;
    return;
  }
  if( !GLEW_ARB_texture_env_combine ) {
    Console(4) << "Warning: Your graphic card driver does not support "
               << "the ARB_texture_env_combine extension so you cannot use the "
               << "MultiTexture node. " << endl;
    return;
  }

  GLint saved_texture;
  glGetIntegerv( GL_ACTIVE_TEXTURE_ARB, &saved_texture );
    
  GLint nr_textures_supported;
  glGetIntegerv( GL_MAX_TEXTURE_UNITS_ARB, &nr_textures_supported );

  for( unsigned int i = 0; i < texture->size(); ++i ) {
    if( i >= (unsigned int)nr_textures_supported ) {
      Console(4) << "Warning! MultiTexture: Unable to display all"
                 << "textures. Your device only has support for " 
                 << nr_textures_supported << " texture units.\n";
      break;
    }
    glActiveTexture( GL_TEXTURE0_ARB + i );
    string rgb_blend_mode = "MODULATE";
    string alpha_blend_mode = "MODULATE";
    string arg2 = "";
    string previous_func = "";
    if( i < mode->size() ) {
      const string &blend_mode = mode->getValueByIndex( i );
      string::size_type pos = blend_mode.find( ',' );
      if( pos == string::npos ) {
        rgb_blend_mode = blend_mode;
        alpha_blend_mode = blend_mode;
      } else {
        rgb_blend_mode = blend_mode.substr( 0, pos );
        alpha_blend_mode = blend_mode.substr( pos + 1, 
                                              blend_mode.size() - pos -1 );
      }
    }
    if( i < source->size() ) arg2 = source->getValueByIndex( i ); 
    if( i > 0 && i < function->size() + 1) 
      previous_func =  function->getValueByIndex( i - 1 ); 

    texture->getValueByIndex( i )->displayList->callList();
    glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_ARB );
  
    const RGB &rgb = color->getValue();
    GLfloat constant_color[] = { rgb.r, rgb.g, rgb.b, alpha->getValue() };
    glTexEnvfv( GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, constant_color );
      
    // set up source 0 to be the texture.
    glTexEnvi( GL_TEXTURE_ENV, GL_SOURCE0_RGB_ARB, GL_TEXTURE );
    glTexEnvi( GL_TEXTURE_ENV, GL_OPERAND0_RGB_ARB, GL_SRC_COLOR );
    glTexEnvi( GL_TEXTURE_ENV, GL_SOURCE0_ALPHA_ARB, GL_TEXTURE ); 
    glTexEnvi( GL_TEXTURE_ENV, GL_OPERAND0_ALPHA_ARB, GL_SRC_ALPHA );


    // set up source 1 depending on the fields.
    GLenum arg2_rgb_source = GL_PREVIOUS_ARB;
    GLenum arg2_rgb_operand = GL_SRC_COLOR;
    GLenum arg2_alpha_source = GL_PREVIOUS_ARB;
    GLenum arg2_alpha_operand = GL_SRC_ALPHA;

    if( arg2 == "DIFFUSE" ) {
      arg2_rgb_source = arg2_alpha_source = GL_PRIMARY_COLOR_ARB;
    } else if( arg2 == "FACTOR" ) {
      arg2_rgb_source = arg2_alpha_source = GL_CONSTANT_ARB;
    } else {
      if( arg2 == "" ) {
        if( previous_func == "COMPLEMENT" ) {
          arg2_rgb_operand = GL_ONE_MINUS_SRC_COLOR;
          arg2_alpha_operand = GL_ONE_MINUS_SRC_ALPHA;
        } else if( previous_func == "ALPHAREPLICATE" ) {
          arg2_rgb_operand = GL_SRC_ALPHA;
        } else {
          if( previous_func != "" ) {
            Console(3) << "Warning: Invalid function \"" << previous_func 
                       << "\" in MultiTexture "
                       << " node (" << getName() << "). " << endl; 
          }
        }
      } else {
        if( arg2 == "SPECULAR" ) {
          Console(3) << "Warning: Unsupported source \"" << arg2 
                     << "\" in MultiTexture "
                     << " node (" << getName() << ")." << endl; 
        } else {
          Console(3) << "Warning: Invalid source \"" << arg2 << "\" in MultiTexture "
                     << " node (" << getName() << "). " << endl; 
        }
      }
    }

    glTexEnvi( GL_TEXTURE_ENV, GL_SOURCE1_RGB_ARB, arg2_rgb_source );
    glTexEnvi( GL_TEXTURE_ENV, GL_OPERAND1_RGB_ARB, arg2_rgb_operand );
    glTexEnvi( GL_TEXTURE_ENV, GL_SOURCE1_ALPHA_ARB, arg2_alpha_source ); 
    glTexEnvi( GL_TEXTURE_ENV, GL_OPERAND1_ALPHA_ARB, arg2_alpha_operand );
      
    // set up the combine mode for the rgb color.
    if( rgb_blend_mode == "MODULATE" ) {
      glTexEnvi( GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, GL_MODULATE );
    } else if( rgb_blend_mode == "REPLACE" ) {
      glTexEnvi( GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, GL_REPLACE );
    } else if( rgb_blend_mode == "MODULATE2X" ) {
      glTexEnvi( GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, GL_MODULATE );
      glTexEnvi( GL_TEXTURE_ENV, GL_RGB_SCALE_ARB, 2 );
    } else if( rgb_blend_mode == "MODULATE4X" ) {
      glTexEnvi( GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, GL_MODULATE );
      glTexEnvi( GL_TEXTURE_ENV, GL_RGB_SCALE_ARB, 4 );
    } else if( rgb_blend_mode == "ADD" ) {
      glTexEnvi( GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, GL_ADD );
    } else if( rgb_blend_mode == "ADDSIGNED" ) {
      glTexEnvi( GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, GL_ADD_SIGNED_ARB );
    } else if( rgb_blend_mode == "ADDSIGNED2X" ) {
      glTexEnvi( GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, GL_ADD_SIGNED_ARB );
      glTexEnvi( GL_TEXTURE_ENV, GL_RGB_SCALE_ARB, 2 );
    } else if( rgb_blend_mode == "SUBTRACT" ) {
      glTexEnvi( GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, GL_SUBTRACT_ARB );
    } else if( rgb_blend_mode == "BLENDDIFFUSEALPHA" ) {
      glTexEnvi( GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, GL_INTERPOLATE_ARB );
      glTexEnvi( GL_TEXTURE_ENV, GL_SOURCE2_RGB_ARB, GL_PRIMARY_COLOR_ARB );
      glTexEnvi( GL_TEXTURE_ENV, GL_OPERAND2_RGB_ARB, GL_SRC_ALPHA );
    } else if( rgb_blend_mode == "BLENDTEXTUREALPHA" ) {
      glTexEnvi( GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, GL_INTERPOLATE_ARB );
      glTexEnvi( GL_TEXTURE_ENV, GL_SOURCE2_RGB_ARB, GL_TEXTURE );
      glTexEnvi( GL_TEXTURE_ENV, GL_OPERAND2_RGB_ARB, GL_SRC_ALPHA );
    } else if( rgb_blend_mode == "BLENDFACTORALPHA" ) {
      glTexEnvi( GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, GL_INTERPOLATE_ARB );
      glTexEnvi( GL_TEXTURE_ENV, GL_SOURCE2_RGB_ARB, GL_CONSTANT_ARB );
      glTexEnvi( GL_TEXTURE_ENV, GL_OPERAND2_RGB_ARB, GL_SRC_ALPHA );
    } else if( rgb_blend_mode == "OFF" ) {
      glTexEnvi( GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, GL_REPLACE );
      glTexEnvi( GL_TEXTURE_ENV, GL_SOURCE0_RGB_ARB, GL_PREVIOUS_ARB );
      glTexEnvi( GL_TEXTURE_ENV, GL_OPERAND0_RGB_ARB, arg2_rgb_operand );
    } else if( rgb_blend_mode == "SELECTARG1" ) {
      glTexEnvi( GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, GL_REPLACE);
    } else if( rgb_blend_mode == "SELECTARG2" ) {
      glTexEnvi( GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, GL_REPLACE );
      glTexEnvi( GL_TEXTURE_ENV, GL_SOURCE0_RGB_ARB, arg2_rgb_source );
      glTexEnvi( GL_TEXTURE_ENV, GL_OPERAND0_RGB_ARB, arg2_rgb_operand );
    } else if( rgb_blend_mode == "DOTPRODUCT3" ) {
      if( GLEW_ARB_texture_env_dot3 ) {
        if( alpha_blend_mode == "DOTPRODUCT3" )
          glTexEnvi( GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, GL_DOT3_RGBA_ARB );
        else 
          glTexEnvi( GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, GL_DOT3_RGB_ARB );
      } else {
        glTexEnvi( GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, GL_MODULATE );
        Console(4) << "Warning: Your graphics card does not support the "
             << "ARB_texture_env_dot3 extension. \"DOTPRODUCT3\" mode cannot "
             << "be used in MultiTexture node( "
             << getName() << "). Using MODULATE instead. " << endl; 
      }
    } else {
      glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
      if( rgb_blend_mode == "ADDSMOOTH" ||
          rgb_blend_mode == "MODULATEALPHA_ADDCOLOR" ||
          rgb_blend_mode == "MODULATEINVALPHA_ADDCOLOR" ||
          rgb_blend_mode == "MODULATEINVCOLOR_ADDALPHA" ) {
        Console(3) << "Warning: Unsupported mode \"" << rgb_blend_mode 
                   << "\" in MultiTexture "
                   << " node (" << getName() 
                   << "). Using MODULATE instead. " << endl; 
      } else {
        Console(3) << "Warning: Invalid mode \"" << rgb_blend_mode 
                   << "\" in MultiTexture "
                   << " node (" << getName() 
                   << "). Using MODULATE instead. " << endl; 
      }
    }

    // set up the combine mode for the alpha channel.
    if( alpha_blend_mode == "MODULATE" ) {
      glTexEnvi( GL_TEXTURE_ENV, GL_COMBINE_ALPHA_ARB, GL_MODULATE );
    } else if( alpha_blend_mode == "REPLACE" ) {
      glTexEnvi( GL_TEXTURE_ENV, GL_COMBINE_ALPHA_ARB, GL_REPLACE );
    } else if( alpha_blend_mode == "MODULATE2X" ) {
      glTexEnvi( GL_TEXTURE_ENV, GL_COMBINE_ALPHA_ARB, GL_MODULATE );
      glTexEnvi( GL_TEXTURE_ENV, GL_ALPHA_SCALE, 2 );
    } else if( alpha_blend_mode == "MODULATE4X" ) {
      glTexEnvi( GL_TEXTURE_ENV, GL_COMBINE_ALPHA_ARB, GL_MODULATE );
      glTexEnvi( GL_TEXTURE_ENV, GL_ALPHA_SCALE, 4 );
    } else if( alpha_blend_mode == "ADD" ) {
      glTexEnvi( GL_TEXTURE_ENV, GL_COMBINE_ALPHA_ARB, GL_ADD );
    } else if( alpha_blend_mode == "ADDSIGNED" ) {
      glTexEnvi( GL_TEXTURE_ENV, GL_COMBINE_ALPHA_ARB, GL_ADD_SIGNED_ARB );
    } else if( alpha_blend_mode == "ADDSIGNED2X" ) {
      glTexEnvi( GL_TEXTURE_ENV, GL_COMBINE_ALPHA_ARB, GL_ADD_SIGNED_ARB );
      glTexEnvi( GL_TEXTURE_ENV, GL_ALPHA_SCALE, 2 );
    } else if( alpha_blend_mode == "SUBTRACT" ) {
      glTexEnvi( GL_TEXTURE_ENV, GL_COMBINE_ALPHA_ARB, GL_SUBTRACT_ARB );
    } else if( alpha_blend_mode == "BLENDDIFFUSEALPHA" ) {
      glTexEnvi( GL_TEXTURE_ENV, GL_COMBINE_ALPHA_ARB, GL_INTERPOLATE_ARB );
      glTexEnvi( GL_TEXTURE_ENV, GL_SOURCE2_ALPHA_ARB, GL_PRIMARY_COLOR_ARB );
      glTexEnvi( GL_TEXTURE_ENV, GL_OPERAND2_ALPHA_ARB, GL_SRC_ALPHA );
    } else if( alpha_blend_mode == "BLENDTEXTUREALPHA" ) {
      glTexEnvi( GL_TEXTURE_ENV, GL_COMBINE_ALPHA_ARB, GL_INTERPOLATE_ARB );
      glTexEnvi( GL_TEXTURE_ENV, GL_SOURCE2_ALPHA_ARB, GL_TEXTURE );
      glTexEnvi( GL_TEXTURE_ENV, GL_OPERAND2_ALPHA_ARB, GL_SRC_ALPHA );
    } else if( alpha_blend_mode == "BLENDFACTORALPHA" ) {
      glTexEnvi( GL_TEXTURE_ENV, GL_COMBINE_ALPHA_ARB, GL_INTERPOLATE_ARB );
      glTexEnvi( GL_TEXTURE_ENV, GL_SOURCE2_ALPHA_ARB, GL_CONSTANT_ARB );
      glTexEnvi( GL_TEXTURE_ENV, GL_OPERAND2_ALPHA_ARB, GL_SRC_ALPHA );
    } else if( alpha_blend_mode == "OFF" ) {
      glTexEnvi( GL_TEXTURE_ENV, GL_COMBINE_ALPHA_ARB, GL_REPLACE );
      glTexEnvi( GL_TEXTURE_ENV, GL_SOURCE0_ALPHA_ARB, GL_PREVIOUS_ARB );
      glTexEnvi( GL_TEXTURE_ENV, GL_OPERAND0_ALPHA_ARB, arg2_alpha_operand );
    } else if( alpha_blend_mode == "SELECTARG1" ) {
      glTexEnvi( GL_TEXTURE_ENV, GL_COMBINE_ALPHA_ARB, GL_REPLACE);
    } else if( alpha_blend_mode == "SELECTARG2" ) {
      glTexEnvi( GL_TEXTURE_ENV, GL_COMBINE_ALPHA_ARB, GL_REPLACE );
      glTexEnvi( GL_TEXTURE_ENV, GL_SOURCE0_ALPHA_ARB, arg2_alpha_source );
      glTexEnvi( GL_TEXTURE_ENV, GL_OPERAND0_ALPHA_ARB, arg2_alpha_operand );
    } else if( alpha_blend_mode == "DOTPRODUCT3" ) {
      // skip since it is handled in src_blend_mode DOTPRODUCT3
    } else {
      glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
      if( alpha_blend_mode == "ADDSMOOTH" ||
          alpha_blend_mode == "MODULATEALPHA_ADDCOLOR" ||
          alpha_blend_mode == "MODULATEINVALPHA_ADDCOLOR" ||
          alpha_blend_mode == "MODULATEINVCOLOR_ADDALPHA" ) {
        Console(3) << "Warning: Unsupported mode \"" << alpha_blend_mode
                   << "\" in MultiTexture "
                   << " node (" << getName() 
                   << "). Using MODULATE instead. " << endl; 
      } else {
        Console(3) << "Warning: Invalid mode \"" << alpha_blend_mode
                   << "\" in MultiTexture "
                   << " node (" << getName() 
                   << "). Using MODULATE instead. " << endl; 
      }
    }
  }

  // If the last texture has a function specified we have to render one extra 
  // texture unit in order to perform that function.
  if( function->size() >= texture->size() && texture->size() > 0 ) {
    const string &func = function->getValueByIndex( texture->size() - 1 );
    glActiveTexture( GL_TEXTURE0_ARB + texture->size() );
    glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_ARB );
    if( func == "COMPLEMENT" ) {
      // render any texture, will not be used but is required.
      texture->getValueByIndex( 0 )->displayList->callList();
      glTexEnvi( GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, GL_REPLACE );
      glTexEnvi( GL_TEXTURE_ENV, GL_SOURCE0_RGB_ARB, GL_PREVIOUS_ARB );
      glTexEnvi( GL_TEXTURE_ENV, GL_OPERAND0_RGB_ARB, GL_ONE_MINUS_SRC_COLOR );
      glTexEnvi( GL_TEXTURE_ENV, GL_SOURCE0_ALPHA_ARB, GL_PREVIOUS_ARB ); 
      glTexEnvi( GL_TEXTURE_ENV, GL_OPERAND0_ALPHA_ARB, GL_ONE_MINUS_SRC_ALPHA );
    } else if( func == "ALPHAREPLICATE" ) {
      // render any texture, will not be used but is required.
      texture->getValueByIndex( 0 )->displayList->callList();
      glTexEnvi( GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, GL_REPLACE );
      glTexEnvi( GL_TEXTURE_ENV, GL_SOURCE0_RGB_ARB, GL_PREVIOUS_ARB );
      glTexEnvi( GL_TEXTURE_ENV, GL_OPERAND0_RGB_ARB, GL_SRC_ALPHA );
      glTexEnvi( GL_TEXTURE_ENV, GL_SOURCE0_ALPHA_ARB, GL_PREVIOUS_ARB ); 
      glTexEnvi( GL_TEXTURE_ENV, GL_OPERAND0_ALPHA_ARB, GL_SRC_ALPHA );
    } else {
      if( func != "" ) {
        Console(3) << "Warning: Invalid function \"" << func 
                   << "\" in MultiTexture "
                   << " node (" << getName() << "). " << endl; 
      }
    }
  }

  glActiveTexture( saved_texture );
  setActiveTexture( this );
}

void MultiTexture::enableTexturing() {
  GLint saved_texture;
  glGetIntegerv( GL_ACTIVE_TEXTURE_ARB, &saved_texture );
  int used_texture_units = 0;
  for( MFTexture::const_iterator i = texture->begin();
       i != texture->end();
       ++i ) {
    if( dynamic_cast< MultiTexture * >( *i ) ){
      //TODO::::
    } else {
      glActiveTexture( GL_TEXTURE0_ARB + used_texture_units );
      ++used_texture_units;
      static_cast< X3DTextureNode * >(*i)->enableTexturing();
    }
  }
  glActiveTexture( saved_texture );
}


void MultiTexture::disableTexturing() {
  GLint saved_texture;
  glGetIntegerv( GL_ACTIVE_TEXTURE_ARB, &saved_texture );
  int used_texture_units = 0;
  for( MFTexture::const_iterator i = texture->begin();
       i != texture->end();
       ++i ) {
    if( dynamic_cast< MultiTexture * >( *i ) ) {
      //TODO:::
    } else {
      glActiveTexture( GL_TEXTURE0_ARB + used_texture_units );
      ++used_texture_units;
      static_cast< X3DTextureNode * >(*i)->disableTexturing();
    }
  }
  glActiveTexture( saved_texture );
}
