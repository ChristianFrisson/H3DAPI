
#include "ShaderFunctions.h"
#include "FieldTemplates.h"
#include "SFNode.h"
#include "MFNode.h"
#include "X3DTexture2DNode.h"
#include "X3DTexture3DNode.h"
#include "X3DEnvironmentTextureNode.h"

using namespace H3D;

namespace H3D {
  namespace Shaders {
    template< class Type >
    GLint *toIntArray( const vector< Type > &values ) {
      unsigned int size = values.size();
      GLint *v = new GLint[ size ];
      for( unsigned int i = 0; i < size; i++ ) 
        v[i] = (GLint) values[i];
      return v;
    }

    template< class Type >
    float *toFloatArray( const vector< Type > &values ) {
      unsigned int size = values.size();
      float *v = new float[ size ];
      for( unsigned int i = 0; i < size; i++ ) 
        v[i] = (float) values[i];
      return v;
    }

    float *toFloatArray( const vector< Vec2f > &values ) {
      unsigned int size = values.size();
      float *v = new float[ size * 2 ];
      for( unsigned int i = 0; i < size; i++ ) {
        const Vec2f &a = values[i];
        v[i*2] = a.x;
        v[i*2+1] = a.y;
      }
      return v;
    }

    
    float *toFloatArray( const vector< Vec3f > &values ) {
      unsigned int size = values.size();
      float *v = new float[ size * 3 ];
      for( unsigned int i = 0; i < size; i++ ) {
        const Vec3f &a = values[i];
        v[i*3] = a.x;
        v[i*3+1] = a.y;
        v[i*3+2] = a.z;
      }
      return v;
    }

    
    float *toFloatArray( const vector< Vec4f > &values ) {
      unsigned int size = values.size();
      float *v = new float[ size * 4 ];
      for( unsigned int i = 0; i < size; i++ ) {
        const Vec4f &a = values[i];
        v[i*4] = a.x;
        v[i*4+1] = a.y;
        v[i*4+2] = a.z;
        v[i*4+3] = a.w;
      }
      return v;
    }

    
    float *toFloatArray( const vector< Vec2d > &values ) {
      unsigned int size = values.size();
      float *v = new float[ size * 2 ];
      for( unsigned int i = 0; i < size; i++ ) {
        const Vec2d &a = values[i];
        v[i*2]   = (H3DFloat) a.x;
        v[i*2+1] = (H3DFloat) a.y;
      }
      return v;
    }

    
    float *toFloatArray( const vector< Vec3d > &values ) {
      unsigned int size = values.size();
      float *v = new float[ size * 3 ];
      for( unsigned int i = 0; i < size; i++ ) {
        const Vec3d &a = values[i];
        v[i*3] =   (H3DFloat)a.x;
        v[i*3+1] = (H3DFloat)a.y;
        v[i*3+2] = (H3DFloat)a.z;
      }
      return v;
    }

    
    float *toFloatArray( const vector< Vec4d > &values ) {
      unsigned int size = values.size();
      float *v = new float[ size * 4 ];
      for( unsigned int i = 0; i < size; i++ ) {
        const Vec4d &a = values[i];
        v[i*4]   = (H3DFloat) a.x;
        v[i*4+1] = (H3DFloat) a.y;
        v[i*4+2] = (H3DFloat) a.z;
        v[i*4+3] = (H3DFloat) a.w;
      }
      return v;
    }

    
    float *toFloatArray( const vector< Rotation > &values ) {
      unsigned int size = values.size();
      float *v = new float[ size * 4 ];
      for( unsigned int i = 0; i < size; i++ ) {
        const Rotation &r = values[i];
        v[i*4]   = (H3DFloat) r.axis.x;
        v[i*4+1] = (H3DFloat) r.axis.y;
        v[i*4+2] = (H3DFloat) r.axis.z;
        v[i*4+3] = (H3DFloat) r.angle;
      }
      return v;
    }

    
    float *toFloatArray( const vector< RGB > &values ) {
      unsigned int size = values.size();
      float *v = new float[ size * 3 ];
      for( unsigned int i = 0; i < size; i++ ) {
        const RGB &r = values[i];
        v[i*3]   = (H3DFloat) r.r;
        v[i*3+1] = (H3DFloat) r.g;
        v[i*3+2] = (H3DFloat) r.b;
      }
      return v;
    }

    
    float *toFloatArray( const vector< RGBA > &values ) {
      unsigned int size = values.size();
      float *v = new float[ size * 4 ];
      for( unsigned int i = 0; i < size; i++ ) {
        const RGBA &r = values[i];
        v[i*4]   = (H3DFloat) r.r;
        v[i*4+1] = (H3DFloat) r.g;
        v[i*4+2] = (H3DFloat) r.b;
        v[i*4+3] = (H3DFloat) r.a;
      }
      return v;
    }

    
    float *toFloatArray( const vector< Matrix3f > &values ) {
      unsigned int size = values.size();
      float *v = new float[ size * 9 ];
      for( unsigned int i = 0; i < size; i++ ) {
        const Matrix3f &m = values[i];
        v[i*9]   = (H3DFloat) m[0][0];
        v[i*9+1] = (H3DFloat) m[1][0];
        v[i*9+2] = (H3DFloat) m[2][0];
        v[i*9+3] = (H3DFloat) m[0][1];
        v[i*9+4] = (H3DFloat) m[1][1];
        v[i*9+5] = (H3DFloat) m[2][1];
        v[i*9+6] = (H3DFloat) m[0][2];
        v[i*9+7] = (H3DFloat) m[1][2];
        v[i*9+8] = (H3DFloat) m[2][2];
      }
      return v;
    }

    
    float *toFloatArray( const vector< Matrix4f > &values ) {
      unsigned int size = values.size();
      float *v = new float[ size * 9 ];
      for( unsigned int i = 0; i < size; i++ ) {
        const Matrix4f &m = values[i];
        v[i*9]   = (H3DFloat) m[0][0];
        v[i*9+1] = (H3DFloat) m[1][0];
        v[i*9+2] = (H3DFloat) m[2][0];
        v[i*9+3] = (H3DFloat) m[3][0];
        v[i*9+4] = (H3DFloat) m[0][1];
        v[i*9+5] = (H3DFloat) m[1][1];
        v[i*9+6] = (H3DFloat) m[2][1];
        v[i*9+7] = (H3DFloat) m[3][1];
        v[i*9+8] = (H3DFloat) m[0][2];
        v[i*9+9] = (H3DFloat) m[1][2];
        v[i*9+10] = (H3DFloat) m[2][2];
        v[i*9+11] = (H3DFloat) m[3][2];
        v[i*9+12] = (H3DFloat) m[0][3];        
        v[i*9+13] = (H3DFloat) m[1][3];
        v[i*9+14] = (H3DFloat) m[2][3];
        v[i*9+15] = (H3DFloat) m[3][3];
      }
      return v;
    }



    template< class Type >
    double *toDoubleArray( const vector< Type > &values ) {
      unsigned int size = values.size();
      double *v = new double[ size ];
      for( unsigned int i = 0; i < size; i++ ) 
        v[i] = (double) values[i];
      return v;
    }

    double *toDoubleArray( const vector< Vec2f > &values ) {
      unsigned int size = values.size();
      double *v = new double[ size * 2 ];
      for( unsigned int i = 0; i < size; i++ ) {
        const Vec2f &a = values[i];
        v[i*2] = a.x;
        v[i*2+1] = a.y;
      }
      return v;
    }

    
    double *toDoubleArray( const vector< Vec3f > &values ) {
      unsigned int size = values.size();
      double *v = new double[ size * 3 ];
      for( unsigned int i = 0; i < size; i++ ) {
        const Vec3f &a = values[i];
        v[i*3] = a.x;
        v[i*3+1] = a.y;
        v[i*3+2] = a.z;
      }
      return v;
    }

    
    double *toDoubleArray( const vector< Vec4f > &values ) {
      unsigned int size = values.size();
      double *v = new double[ size * 4 ];
      for( unsigned int i = 0; i < size; i++ ) {
        const Vec4f &a = values[i];
        v[i*4] = a.x;
        v[i*4+1] = a.y;
        v[i*4+2] = a.z;
        v[i*4+3] = a.w;
      }
      return v;
    }

    
    double *toDoubleArray( const vector< Vec2d > &values ) {
      unsigned int size = values.size();
      double *v = new double[ size * 2 ];
      for( unsigned int i = 0; i < size; i++ ) {
        const Vec2d &a = values[i];
        v[i*2]   = (H3DDouble) a.x;
        v[i*2+1] = (H3DDouble) a.y;
      }
      return v;
    }

    
    double *toDoubleArray( const vector< Vec3d > &values ) {
      unsigned int size = values.size();
      double *v = new double[ size * 3 ];
      for( unsigned int i = 0; i < size; i++ ) {
        const Vec3d &a = values[i];
        v[i*3] =   (H3DDouble)a.x;
        v[i*3+1] = (H3DDouble)a.y;
        v[i*3+2] = (H3DDouble)a.z;
      }
      return v;
    }

    
    double *toDoubleArray( const vector< Vec4d > &values ) {
      unsigned int size = values.size();
      double *v = new double[ size * 4 ];
      for( unsigned int i = 0; i < size; i++ ) {
        const Vec4d &a = values[i];
        v[i*4]   = (H3DDouble) a.x;
        v[i*4+1] = (H3DDouble) a.y;
        v[i*4+2] = (H3DDouble) a.z;
        v[i*4+3] = (H3DDouble) a.w;
      }
      return v;
    }

    
    double *toDoubleArray( const vector< Rotation > &values ) {
      unsigned int size = values.size();
      double *v = new double[ size * 4 ];
      for( unsigned int i = 0; i < size; i++ ) {
        const Rotation &r = values[i];
        v[i*4]   = (H3DDouble) r.axis.x;
        v[i*4+1] = (H3DDouble) r.axis.y;
        v[i*4+2] = (H3DDouble) r.axis.z;
        v[i*4+3] = (H3DDouble) r.angle;
      }
      return v;
    }

    
    double *toDoubleArray( const vector< RGB > &values ) {
      unsigned int size = values.size();
      double *v = new double[ size * 3 ];
      for( unsigned int i = 0; i < size; i++ ) {
        const RGB &r = values[i];
        v[i*3]   = (H3DDouble) r.r;
        v[i*3+1] = (H3DDouble) r.g;
        v[i*3+2] = (H3DDouble) r.b;
      }
      return v;
    }

    
    double *toDoubleArray( const vector< RGBA > &values ) {
      unsigned int size = values.size();
      double *v = new double[ size * 4 ];
      for( unsigned int i = 0; i < size; i++ ) {
        const RGBA &r = values[i];
        v[i*4]   = (H3DDouble) r.r;
        v[i*4+1] = (H3DDouble) r.g;
        v[i*4+2] = (H3DDouble) r.b;
        v[i*4+3] = (H3DDouble) r.a;
      }
      return v;
    }

    
    double *toDoubleArray( const vector< Matrix3f > &values ) {
      unsigned int size = values.size();
      double *v = new double[ size * 9 ];
      for( unsigned int i = 0; i < size; i++ ) {
        const Matrix3f &m = values[i];
        v[i*9]   = (H3DDouble) m[0][0];
        v[i*9+1] = (H3DDouble) m[1][0];
        v[i*9+2] = (H3DDouble) m[2][0];
        v[i*9+3] = (H3DDouble) m[0][1];
        v[i*9+4] = (H3DDouble) m[1][1];
        v[i*9+5] = (H3DDouble) m[2][1];
        v[i*9+6] = (H3DDouble) m[0][2];
        v[i*9+7] = (H3DDouble) m[1][2];
        v[i*9+8] = (H3DDouble) m[2][2];
      }
      return v;
    }

    
    double *toDoubleArray( const vector< Matrix4f > &values ) {
      unsigned int size = values.size();
      double *v = new double[ size * 9 ];
      for( unsigned int i = 0; i < size; i++ ) {
        const Matrix4f &m = values[i];
        v[i*9]   = (H3DDouble) m[0][0];
        v[i*9+1] = (H3DDouble) m[1][0];
        v[i*9+2] = (H3DDouble) m[2][0];
        v[i*9+3] = (H3DDouble) m[3][0];
        v[i*9+4] = (H3DDouble) m[0][1];
        v[i*9+5] = (H3DDouble) m[1][1];
        v[i*9+6] = (H3DDouble) m[2][1];
        v[i*9+7] = (H3DDouble) m[3][1];
        v[i*9+8] = (H3DDouble) m[0][2];
        v[i*9+9] = (H3DDouble) m[1][2];
        v[i*9+10] = (H3DDouble) m[2][2];
        v[i*9+11] = (H3DDouble) m[3][2];
        v[i*9+12] = (H3DDouble) m[0][3];        
        v[i*9+13] = (H3DDouble) m[1][3];
        v[i*9+14] = (H3DDouble) m[2][3];
        v[i*9+15] = (H3DDouble) m[3][3];
      }
      return v;
    }

  }
}

/// Set the value of a uniform variable in the current GLSL shader.
/// The name of the uniform variable is the same as the name of the field. 
bool H3D::Shaders::setGLSLUniformVariableValue( GLhandleARB program_handle,
                                                  Field *field ) {
  const string &name = field->getName();
  GLint location = glGetUniformLocationARB( program_handle,
                                            name.c_str() );
  if( location == -1 ) return false;
      
  // clear the error flag
  glGetError();
  if( SFBool *f = dynamic_cast< SFBool * >( field ) ) {
    glUniform1iARB( location, f->getValue() );
  } else if( MFBool *f = dynamic_cast< MFBool * >( field ) ) {
    GLint *v = toIntArray( f->getValue() );
    glUniform1ivARB( location, f->size(), v );
    delete v;
  } else if( SFInt32 *f = dynamic_cast< SFInt32 * >( field ) ) {
    glUniform1iARB( location, f->getValue() );
  } else if( MFInt32 *f = dynamic_cast< MFInt32 * >( field ) ) {
    glUniform1ivARB( location, f->size(), &(f->getValue()[0]) );
  } else if( SFFloat *f = dynamic_cast< SFFloat * >( field ) ) {
    glUniform1fARB( location, f->getValue() );
  } else if( MFFloat *f = dynamic_cast< MFFloat * >( field ) ) {
    GLfloat *v = toFloatArray( f->getValue() );
    glUniform1fvARB( location, f->size(), v );
    delete v;
  } else if( SFDouble *f = dynamic_cast< SFDouble * >( field ) ) {
    glUniform1fARB( location, (GLfloat)f->getValue() );
  } else if( MFDouble *f = dynamic_cast< MFDouble * >( field ) ) {
    GLfloat *v = toFloatArray( f->getValue() );
    glUniform1fvARB( location, f->size(), v );
    delete v;
  } else if( SFTime *f = dynamic_cast< SFTime * >( field ) ) {
    glUniform1fARB( location, (GLfloat)f->getValue() );
  } else if( MFTime *f = dynamic_cast< MFTime * >( field ) ) {
    GLfloat *v = toFloatArray( f->getValue() );
    glUniform1fvARB( location, f->size(), v );
    delete v;
  } else if( SFVec2f *f = dynamic_cast< SFVec2f * >( field ) ) {
    const Vec2f &v = f->getValue(); 
    glUniform2fARB( location, 
                    (GLfloat)v.x,
                    (GLfloat)v.y );
  } else if( MFVec2f *f = dynamic_cast< MFVec2f * >( field ) ) {
    GLfloat *v = toFloatArray( f->getValue() );
    glUniform2fvARB( location, f->size(), v );
    delete v;
  } else if( SFVec3f *f = dynamic_cast< SFVec3f * >( field ) ) {
    const Vec3f &v = f->getValue(); 
    glUniform3fARB( location, 
                    (GLfloat)v.x,
                    (GLfloat)v.y,
                    (GLfloat)v.z );
  } else if( MFVec3f *f = dynamic_cast< MFVec3f * >( field ) ) {
    GLfloat *v = toFloatArray( f->getValue() );
    glUniform3fvARB( location, f->size(), v );
    delete v;
  } else if( SFVec4f *f = dynamic_cast< SFVec4f * >( field ) ) {
    const Vec4f &v = f->getValue(); 
    glUniform4fARB( location, 
                    (GLfloat)v.x,
                    (GLfloat)v.y,
                    (GLfloat)v.z,
                    (GLfloat)v.w  );
  } else if( MFVec4f *f = dynamic_cast< MFVec4f * >( field ) ) {
    GLfloat *v = toFloatArray( f->getValue() );
    glUniform4fvARB( location, f->size(), v );
    delete v;
  } else if( SFVec2d *f = dynamic_cast< SFVec2d * >( field ) ) {
    const Vec2d &v = f->getValue(); 
    glUniform2fARB( location, 
                    (GLfloat)v.x,
                    (GLfloat)v.y );
  } else if( MFVec2d *f = dynamic_cast< MFVec2d * >( field ) ) {
    GLfloat *v = toFloatArray( f->getValue() );
    glUniform2fvARB( location, f->size(), v );
    delete v;
  } else if( SFVec3d *f = dynamic_cast< SFVec3d * >( field ) ) {
    const Vec3d &v = f->getValue(); 
    glUniform3fARB( location, 
                    (GLfloat)v.x,
                    (GLfloat)v.y,
                    (GLfloat)v.z );
  } else if( MFVec3d *f = dynamic_cast< MFVec3d * >( field ) ) {
    GLfloat *v = toFloatArray( f->getValue() );
    glUniform3fvARB( location, f->size(), v );
    delete v;
  } else if( SFVec4d *f = dynamic_cast< SFVec4d * >( field ) ) {
    const Vec4d &v = f->getValue(); 
    glUniform4fARB( location, 
                    (GLfloat)v.x,
                    (GLfloat)v.y,
                    (GLfloat)v.z,
                    (GLfloat)v.w  );
  } else if( MFVec4d *f = dynamic_cast< MFVec4d * >( field ) ) {
    GLfloat *v = toFloatArray( f->getValue() );
    glUniform4fvARB( location, f->size(), v );
    delete v;
  } else if( SFRotation *f = dynamic_cast< SFRotation * >( field ) ) {
    const Rotation &r = f->getValue(); 
    glUniform4fARB( location, 
                    (GLfloat)r.axis.x,
                    (GLfloat)r.axis.y,
                    (GLfloat)r.axis.z,
                    (GLfloat)r.angle  );
  } else if( MFRotation *f = dynamic_cast< MFRotation * >( field ) ) {
    GLfloat *v = toFloatArray( f->getValue() );
    glUniform4fvARB( location, f->size(), v );
    delete v;
  } else if( SFColor *f = dynamic_cast< SFColor * >( field ) ) {
    const RGB &r = f->getValue(); 
    glUniform3fARB( location, 
                    (GLfloat)r.r,
                    (GLfloat)r.g,
                    (GLfloat)r.b );
  } else if( MFColor *f = dynamic_cast< MFColor * >( field ) ) {
    GLfloat *v = toFloatArray( f->getValue() );
    glUniform4fvARB( location, f->size(), v );
    delete v;
  } else if( SFColorRGBA *f = dynamic_cast< SFColorRGBA * >( field ) ) {
    const RGBA &r = f->getValue(); 
    glUniform4fARB( location, 
                    (GLfloat)r.r,
                    (GLfloat)r.g,
                    (GLfloat)r.b,
                    (GLfloat)r.a );
  } else if( MFColorRGBA *f = dynamic_cast< MFColorRGBA * >( field ) ) {
    GLfloat *v = toFloatArray( f->getValue() );
    glUniform4fvARB( location, f->size(), v );
    delete v;
  } else if( SFMatrix3f *f = dynamic_cast< SFMatrix3f * >( field ) ) {
    const Matrix3f &m = f->getValue(); 
    glUniformMatrix3fvARB( location, 
                           1,
                           true,
                           m[0] );
  } else if( MFMatrix3f *f = dynamic_cast< MFMatrix3f * >( field ) ) {
    GLfloat *v = toFloatArray( f->getValue() );
    glUniformMatrix3fvARB( location, f->size(), false, v );
    delete v;
  } else if( SFMatrix4f *f = dynamic_cast< SFMatrix4f * >( field ) ) {
    const Matrix4f &m = f->getValue(); 
    glUniformMatrix3fvARB( location, 
                           1,
                           true,
                           m[0] );
  } else if( MFMatrix4f *f = dynamic_cast< MFMatrix4f * >( field ) ) {
    GLfloat *v = toFloatArray( f->getValue() );
    glUniformMatrix4fvARB( location, f->size(), false, v );
    delete v;
  } else if( SFNode *f = dynamic_cast< SFNode * >( field ) ) {
    Node *n = f->getValue(); 
    if( X3DTexture2DNode *t = dynamic_cast< X3DTexture2DNode *>( n ) ) {
      glUniform1iARB( location, t->getTextureUnit() - GL_TEXTURE0_ARB );
    } else if( X3DTexture3DNode *t = 
               dynamic_cast< X3DTexture3DNode *>( n ) ) {
      glUniform1iARB( location, t->getTextureUnit() - GL_TEXTURE0_ARB );
    } else if( X3DEnvironmentTextureNode *t = 
                dynamic_cast< X3DEnvironmentTextureNode *>( n ) ) {
      glUniform1iARB( location, t->getTextureUnit() - GL_TEXTURE0_ARB );
    } else {
      return false;
    }
                                                                           
  } else if( MFNode *f = dynamic_cast< MFNode * >( field ) ) {
    unsigned int size = f->size();
    GLint *v = new GLint[ size ];
    for( unsigned int i = 0; i < size; i++ ) {
      Node *n = f->getValueByIndex( i ); 
      if( X3DTexture2DNode *t = dynamic_cast< X3DTexture2DNode *>( n ) ) {
        v[i] = t->getTextureUnit() - GL_TEXTURE0_ARB;
      } else if( X3DTexture3DNode *t = 
                 dynamic_cast< X3DTexture3DNode *>( n ) ) {
        v[i] = t->getTextureUnit() - GL_TEXTURE0_ARB;
      } else if( X3DEnvironmentTextureNode *t = 
                dynamic_cast< X3DEnvironmentTextureNode *>( n ) ) {
        v[i] = t->getTextureUnit() - GL_TEXTURE0_ARB;
      } else {
        delete v;
        return false;
      }
    }
    glUniform1ivARB( location, size, v );
  } else {
    return false;
  }
  
  // ignore any errors that occurs when setting uniform variables.
  return glGetError() == GL_NO_ERROR;
}

#ifdef HAVE_CG
CGprofile H3D::Shaders::cgProfileFromString( const string &profile, 
                                             const string &type ) {
  CGbool profile_supported = false;
  CGprofile cg_profile;
  if( profile == "CG_OPENGL_ARB" ) {
    if( type == "VERTEX" ) {
      profile_supported = cgGLIsProfileSupported(CG_PROFILE_ARBVP1); 
      cg_profile = CG_PROFILE_ARBVP1;
    }
    else if( type == "FRAGMENT" ) {
      profile_supported = cgGLIsProfileSupported(CG_PROFILE_ARBFP1);
      cg_profile = CG_PROFILE_ARBFP1;
    } 
  } else if( profile == "CG_OPENGL_NV40" ) {
    if( type == "VERTEX" ) {
      profile_supported = cgGLIsProfileSupported(CG_PROFILE_VP40);
      cg_profile = CG_PROFILE_VP40;
    }
    else if( type == "FRAGMENT" ) {
      profile_supported = cgGLIsProfileSupported(CG_PROFILE_FP40);
      cg_profile = CG_PROFILE_FP40;
    }
  } else if( profile == "CG_OPENGL_NV30" ) {
    if( type == "VERTEX" ) {
      profile_supported = cgGLIsProfileSupported(CG_PROFILE_VP30);
      cg_profile = CG_PROFILE_VP30;
    }
    else if( type == "FRAGMENT" ) {
      profile_supported = cgGLIsProfileSupported(CG_PROFILE_FP30);
      cg_profile = CG_PROFILE_FP30;
    }
  } else if( profile == "CG_OPENGL_NV20" ) {
    if( type == "VERTEX" ) {
      profile_supported = cgGLIsProfileSupported(CG_PROFILE_VP20);
      cg_profile = CG_PROFILE_VP20;
    }
    else if( type == "FRAGMENT" ) {
      profile_supported = cgGLIsProfileSupported(CG_PROFILE_FP20);
      cg_profile = CG_PROFILE_FP20;
    } 
  } else {
    if( profile != "CG" ) {
      cerr << "Warning: Invalid profile name" << endl;
    }
    if( type == "VERTEX" )
      cg_profile = cgGLGetLatestProfile( CG_GL_VERTEX );
    if( type == "FRAGMENT" )
      cg_profile = cgGLGetLatestProfile( CG_GL_FRAGMENT );
    profile_supported = cgGLIsProfileSupported( cg_profile );
  }
  return cg_profile;
}

bool H3D::Shaders::setCGUniformVariableValue( CGprogram program_handle,
                                              Field *field ) {
  const string &name = field->getName();
  CGparameter param = cgGetNamedProgramParameter( program_handle,
                                                  CG_PROGRAM,
                                                  name.c_str() );
  // local variable not found in shader, try to find global
  if( !param ) 
    param = cgGetNamedProgramParameter( program_handle,
                                        CG_GLOBAL,
                                        name.c_str() );
  if( param == NULL ) return false;
      
  // clear the error flag
  cgGetError();

  if( SFBool *f = dynamic_cast< SFBool * >( field ) ) {
    cgGLSetParameter1f( param, f->getValue() );
  } else if( MFBool *f = dynamic_cast< MFBool * >( field ) ) {
    GLfloat *v = toFloatArray( f->getValue() );
    cgGLSetParameterArray1f( param, 0, f->size(), v );
    delete v;
  } else if( SFInt32 *f = dynamic_cast< SFInt32 * >( field ) ) {
    cgGLSetParameter1f( param, (GLfloat)f->getValue() );
  } else if( MFInt32 *f = dynamic_cast< MFInt32 * >( field ) ) {
    GLfloat *v = toFloatArray( f->getValue() );
    cgGLSetParameterArray1f( param, 0, f->size(), v );
    delete v;
  } else if( SFFloat *f = dynamic_cast< SFFloat * >( field ) ) {
    cgGLSetParameter1f( param, f->getValue() );
  } else if( MFFloat *f = dynamic_cast< MFFloat * >( field ) ) {
    GLfloat *v = toFloatArray( f->getValue() );
    cgGLSetParameterArray1f( param, 0, f->size(), v );
    delete v;
  } else if( SFDouble *f = dynamic_cast< SFDouble * >( field ) ) {
    cgGLSetParameter1f( param, (GLfloat)f->getValue() );
  } else if( MFDouble *f = dynamic_cast< MFDouble * >( field ) ) {
    GLfloat *v = toFloatArray( f->getValue() );
    cgGLSetParameterArray1f( param, 0, f->size(), v );
    delete v;
  } else if( SFTime *f = dynamic_cast< SFTime * >( field ) ) {
    cgGLSetParameter1d( param, f->getValue() );
  } else if( MFTime *f = dynamic_cast< MFTime * >( field ) ) {
    GLdouble *v = toDoubleArray( f->getValue() );
    cgGLSetParameterArray1d( param, 0, f->size(), v );
    delete v;
  } else if( SFVec2f *f = dynamic_cast< SFVec2f * >( field ) ) {
    const Vec2f &v = f->getValue(); 
    cgGLSetParameter2f( param, (GLfloat)v.x, (GLfloat)v.y );
  } else if( MFVec2f *f = dynamic_cast< MFVec2f * >( field ) ) {
    GLfloat *v = toFloatArray( f->getValue() );
    cgGLSetParameterArray2f( param, 0, f->size(), v );
    delete v;
  } else if( SFVec3f *f = dynamic_cast< SFVec3f * >( field ) ) {
    const Vec3f &v = f->getValue(); 
    cgGLSetParameter3f( param, 
                    (GLfloat)v.x,
                    (GLfloat)v.y,
                    (GLfloat)v.z );
  } else if( MFVec3f *f = dynamic_cast< MFVec3f * >( field ) ) {
    GLfloat *v = toFloatArray( f->getValue() );
    cgGLSetParameterArray3f( param, 0, f->size(), v );
    delete v;
  } else if( SFVec4f *f = dynamic_cast< SFVec4f * >( field ) ) {
    const Vec4f &v = f->getValue(); 
    cgGLSetParameter4f( param, 
                    (GLfloat)v.x,
                    (GLfloat)v.y,
                    (GLfloat)v.z,
                    (GLfloat)v.w  );
  } else if( MFVec4f *f = dynamic_cast< MFVec4f * >( field ) ) {
    GLfloat *v = toFloatArray( f->getValue() );
    cgGLSetParameterArray4f( param, 0, f->size(), v );
    delete v;
  } else if( SFVec2d *f = dynamic_cast< SFVec2d * >( field ) ) {
    const Vec2d &v = f->getValue(); 
    cgGLSetParameter2d( param, v.x, v.y );
  } else if( MFVec2d *f = dynamic_cast< MFVec2d * >( field ) ) {
    GLdouble *v = toDoubleArray( f->getValue() );
    cgGLSetParameterArray2d( param, 0, f->size(), v );
    delete v;
  } else if( SFVec3d *f = dynamic_cast< SFVec3d * >( field ) ) {
    const Vec3d &v = f->getValue(); 
    cgGLSetParameter3d( param, v.x, v.y, v.z );
  } else if( MFVec3d *f = dynamic_cast< MFVec3d * >( field ) ) {
    GLdouble *v = toDoubleArray( f->getValue() );
    cgGLSetParameterArray3d( param, 0, f->size(), v );
    delete v;
  } else if( SFVec4d *f = dynamic_cast< SFVec4d * >( field ) ) {
    const Vec4d &v = f->getValue(); 
    cgGLSetParameter4d( param, v.x, v.y, v.z, v.w  );
  } else if( MFVec4d *f = dynamic_cast< MFVec4d * >( field ) ) {
    GLdouble *v = toDoubleArray( f->getValue() );
    cgGLSetParameterArray4d( param, 0, f->size(), v );
    delete v;
  } else if( SFRotation *f = dynamic_cast< SFRotation * >( field ) ) {
    const Rotation &r = f->getValue(); 
    cgGLSetParameter4f( param, 
                    (GLfloat)r.axis.x,
                    (GLfloat)r.axis.y,
                    (GLfloat)r.axis.z,
                    (GLfloat)r.angle  );
  } else if( MFRotation *f = dynamic_cast< MFRotation * >( field ) ) {
    GLfloat *v = toFloatArray( f->getValue() );
    cgGLSetParameterArray4f( param, 0, f->size(), v );
    delete v;
  } else if( SFColor *f = dynamic_cast< SFColor * >( field ) ) {
    const RGB &r = f->getValue(); 
    cgGLSetParameter3f( param, 
                    (GLfloat)r.r,
                    (GLfloat)r.g,
                    (GLfloat)r.b );
  } else if( MFColor *f = dynamic_cast< MFColor * >( field ) ) {
    GLfloat *v = toFloatArray( f->getValue() );
    cgGLSetParameterArray4f( param, 0, f->size(), v );
    delete v;
  } else if( SFColorRGBA *f = dynamic_cast< SFColorRGBA * >( field ) ) {
    const RGBA &r = f->getValue(); 
    cgGLSetParameter4f( param, 
                    (GLfloat)r.r,
                    (GLfloat)r.g,
                    (GLfloat)r.b,
                    (GLfloat)r.a );
  } else if( MFColorRGBA *f = dynamic_cast< MFColorRGBA * >( field ) ) {
    GLfloat *v = toFloatArray( f->getValue() );
    cgGLSetParameterArray4f( param, 0, f->size(), v );
    delete v;
  } else if( SFMatrix3f *f = dynamic_cast< SFMatrix3f * >( field ) ) {
    const Matrix3f &m = f->getValue(); 
    cgGLSetMatrixParameterfr( param, m[0] );
  } else if( MFMatrix3f *f = dynamic_cast< MFMatrix3f * >( field ) ) {
    GLfloat *v = toFloatArray( f->getValue() );
    cgGLSetMatrixParameterArrayfr( param, 0, f->size(), v );
    delete v;
  } else if( SFMatrix4f *f = dynamic_cast< SFMatrix4f * >( field ) ) {
    const Matrix4f &m = f->getValue(); 
    cgGLSetMatrixParameterfr( param, m[0] );
  } else if( MFMatrix4f *f = dynamic_cast< MFMatrix4f * >( field ) ) {
    GLfloat *v = toFloatArray( f->getValue() );
    cgGLSetMatrixParameterArrayfr( param, 0, f->size(), v );
    delete v;
  } else if( SFNode *f = dynamic_cast< SFNode * >( field ) ) {
    Node *n = f->getValue(); 
    if( X3DTexture2DNode *t = dynamic_cast< X3DTexture2DNode *>( n ) ) {
      cgGLSetTextureParameter( param, t->getTextureId());
    } else if( X3DTexture3DNode *t = 
               dynamic_cast< X3DTexture3DNode *>( n ) ) {
      cgGLSetTextureParameter( param, t->getTextureId() );
    } else if( X3DEnvironmentTextureNode *t = 
                dynamic_cast< X3DEnvironmentTextureNode *>( n ) ) {
      cgGLSetTextureParameter( param, t->getTextureId() );
    } else {
      return false;
    }
                                                                           
  } else if( MFNode *f = dynamic_cast< MFNode * >( field ) ) {
    return false;
  } else {
    return false;
  }
  
  // ignore any errors that occurs when setting uniform variables.
  CGerror err = cgGetError();
  if( err != CG_NO_ERROR )
    cerr << cgGetErrorString( err ) << endl;
  return err == GL_NO_ERROR;
 }

#endif // HAVE_CG

void H3D::Shaders::preRenderTextures( H3DDynamicFieldsObject *dfo ) {
  unsigned int nr_textures = 0; 
  for( H3DDynamicFieldsObject::field_iterator f = dfo->firstField();
       f != dfo->endField(); f++ ) {
    if( SFNode *sfnode = dynamic_cast< SFNode * >( *f ) ) {
      Node *n = sfnode->getValue(); 
      if( X3DTexture2DNode *t = dynamic_cast< X3DTexture2DNode *>( n ) ) {
        glActiveTextureARB(GL_TEXTURE0_ARB + nr_textures );
        t->preRender();
        nr_textures++;
      } else if( X3DTexture3DNode *t = 
                 dynamic_cast< X3DTexture3DNode *>( n ) ) {
        glActiveTextureARB(GL_TEXTURE0_ARB + nr_textures );
        t->preRender();
        nr_textures++;
      } else if( X3DEnvironmentTextureNode *t = 
                 dynamic_cast< X3DEnvironmentTextureNode *>( n ) ) {
        glActiveTextureARB(GL_TEXTURE0_ARB + nr_textures );
        t->preRender();
        nr_textures++;
      } 

    } else if( MFNode *mfnode = dynamic_cast< MFNode * >( *f ) ) {
      for( unsigned int i = 0; i < mfnode->size(); i++ ) {
        Node *n = mfnode->getValueByIndex( i ); 
        if( X3DTexture2DNode *t = 
            dynamic_cast< X3DTexture2DNode *>( n ) ) {
          glActiveTextureARB(GL_TEXTURE0_ARB + nr_textures );
          t->preRender();
          nr_textures++;
        } else if( X3DTexture3DNode *t = 
                   dynamic_cast< X3DTexture3DNode *>( n ) ) {
          glActiveTextureARB(GL_TEXTURE0_ARB + nr_textures );
          t->preRender();
          nr_textures++;
        } else if( X3DEnvironmentTextureNode *t = 
                   dynamic_cast< X3DEnvironmentTextureNode *>( n ) ) {
          glActiveTextureARB(GL_TEXTURE0_ARB + nr_textures );
          t->preRender();
          nr_textures++;
        } 
      }
    }
  }
}

void H3D::Shaders::postRenderTextures( H3DDynamicFieldsObject *dfo ) {
  unsigned int nr_textures = 0; 
  for( H3DDynamicFieldsObject::field_iterator f = dfo->firstField();
       f != dfo->endField(); f++ ) {
    if( SFNode *sfnode = dynamic_cast< SFNode * >( *f ) ) {
      Node *n = sfnode->getValue(); 
      if( X3DTexture2DNode *t = dynamic_cast< X3DTexture2DNode *>( n ) ) {
        glActiveTextureARB(GL_TEXTURE0_ARB + nr_textures );
        t->postRender();
        nr_textures++;
      } else if( X3DTexture3DNode *t = 
                 dynamic_cast< X3DTexture3DNode *>( n ) ) {
        glActiveTextureARB(GL_TEXTURE0_ARB + nr_textures );
        t->postRender();
        nr_textures++;
      } else if( X3DEnvironmentTextureNode *t = 
                 dynamic_cast< X3DEnvironmentTextureNode *>( n ) ) {
        glActiveTextureARB(GL_TEXTURE0_ARB + nr_textures );
        t->postRender();
        nr_textures++;
      } 
    } else if( MFNode *mfnode = dynamic_cast< MFNode * >( *f ) ) {
      for( unsigned int i = 0; i < mfnode->size(); i++ ) {
        Node *n = mfnode->getValueByIndex( i ); 
        if( X3DTexture2DNode *t = 
            dynamic_cast< X3DTexture2DNode *>( n ) ) {
          glActiveTextureARB(GL_TEXTURE0_ARB + nr_textures );
          t->postRender();
          nr_textures++;
        } else if( X3DTexture3DNode *t = 
                   dynamic_cast< X3DTexture3DNode *>( n ) ) {
          glActiveTextureARB(GL_TEXTURE0_ARB + nr_textures );
          t->postRender();
          nr_textures++;
        } else if( X3DEnvironmentTextureNode *t = 
                 dynamic_cast< X3DEnvironmentTextureNode *>( n ) ) {
          glActiveTextureARB(GL_TEXTURE0_ARB + nr_textures );
          t->postRender();
          nr_textures++;
      } 
      }
    }
  }
}

void H3D::Shaders::renderTextures( H3DDynamicFieldsObject *dfo ) {
  unsigned int nr_textures = 0; 
  for( H3DDynamicFieldsObject::field_iterator f = dfo->firstField();
       f != dfo->endField(); f++ ) {
    if( SFNode *sfnode = dynamic_cast< SFNode * >( *f ) ) {
      Node *n = sfnode->getValue(); 
      if( X3DTexture2DNode *t = dynamic_cast< X3DTexture2DNode *>( n ) ) {
        glActiveTextureARB(GL_TEXTURE0_ARB + nr_textures );
        t->displayList->callList();
        nr_textures++;
      } else if( X3DTexture3DNode *t = 
                 dynamic_cast< X3DTexture3DNode *>( n ) ) {
        glActiveTextureARB(GL_TEXTURE0_ARB + nr_textures );
        t->displayList->callList();
        nr_textures++;
      } else if( X3DEnvironmentTextureNode *t = 
                 dynamic_cast< X3DEnvironmentTextureNode *>( n ) ) {
        glActiveTextureARB(GL_TEXTURE0_ARB + nr_textures );
        t->displayList->callList();
        nr_textures++;
      } 
    } else if( MFNode *mfnode = dynamic_cast< MFNode * >( *f ) ) {
      for( unsigned int i = 0; i < mfnode->size(); i++ ) {
        Node *n = mfnode->getValueByIndex( i ); 
        if( X3DTexture2DNode *t = 
            dynamic_cast< X3DTexture2DNode *>( n ) ) {
          glActiveTextureARB(GL_TEXTURE0_ARB + nr_textures );
          t->displayList->callList();
          nr_textures++;
        } else if( X3DTexture3DNode *t = 
                   dynamic_cast< X3DTexture3DNode *>( n ) ) {
          glActiveTextureARB(GL_TEXTURE0_ARB + nr_textures );
          t->displayList->callList();
          nr_textures++;
        } else if( X3DEnvironmentTextureNode *t = 
                   dynamic_cast< X3DEnvironmentTextureNode *>( n ) ) {
          glActiveTextureARB(GL_TEXTURE0_ARB + nr_textures );
          t->displayList->callList();
          nr_textures++;
        } 
      }
    }
  }
}
