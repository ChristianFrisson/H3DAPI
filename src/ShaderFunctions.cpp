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
/// \file ShaderFunctions.cpp
/// \brief CPP file for functions used to connect shaders to X3D/H3D.
///
//
//
//////////////////////////////////////////////////////////////////////////////
#include <H3D/ShaderFunctions.h>
#include <H3D/FieldTemplates.h>
#include <H3D/SFNode.h>
#include <H3D/MFNode.h>
#include <H3D/X3DTexture2DNode.h>
#include <H3D/X3DTexture3DNode.h>
#include <H3D/ShaderImageNode.h>
#include <H3D/ShaderStorageBuffer.h>
#include <H3D/ShaderAtomicCounter.h>
#include <H3D/X3DEnvironmentTextureNode.h>
#include <H3D/SFFloat.h>
#include <H3D/MFFloat.h>
#include <H3D/SFDouble.h>
#include <H3D/MFDouble.h>
#include <H3D/SFTime.h>
#include <H3D/MFTime.h>
#include <H3D/SFInt32.h>
#include <H3D/MFInt32.h>
#include <H3D/SFVec2f.h>
#include <H3D/MFVec2f.h>
#include <H3D/SFVec3f.h>
#include <H3D/MFVec3f.h>
#include <H3D/SFVec4f.h>
#include <H3D/MFVec4f.h>
#include <H3D/SFVec2d.h>
#include <H3D/MFVec2d.h>
#include <H3D/SFVec3d.h>
#include <H3D/MFVec3d.h>
#include <H3D/SFVec4d.h>
#include <H3D/MFVec4d.h>
#include <H3D/SFBool.h>
#include <H3D/MFBool.h>
#include <H3D/SFString.h>
#include <H3D/MFString.h>
#include <H3D/SFColor.h>
#include <H3D/MFColor.h>
#include <H3D/SFColorRGBA.h>
#include <H3D/MFColorRGBA.h>
#include <H3D/SFRotation.h>
#include <H3D/MFRotation.h>
#include <H3D/SFQuaternion.h>
#include <H3D/MFQuaternion.h>
#include <H3D/SFMatrix3f.h>
#include <H3D/MFMatrix3f.h>
#include <H3D/SFMatrix3d.h>
#include <H3D/MFMatrix3d.h>
#include <H3D/SFMatrix4f.h>
#include <H3D/MFMatrix4f.h>
#include <H3D/SFMatrix4d.h>
#include <H3D/MFMatrix4d.h>
#include <H3D/X3DShaderNode.h>

using namespace H3D;

namespace H3D {
  namespace Shaders {
    template< class Type >
    GLint *toIntArray( const vector< Type > &values ) {
      unsigned int size = (unsigned int) values.size();
      GLint *v = new GLint[ size ];
      for( unsigned int i = 0; i < size; ++i ) 
        v[i] = (GLint) values[i];
      return v;
    }

    template< class Type >
    float *toFloatArray( const vector< Type > &values ) {
      unsigned int size = (unsigned int) values.size();
      float *v = new float[ size ];
      for( unsigned int i = 0; i < size; ++i ) 
        v[i] = (float) values[i];
      return v;
    }

    float *toFloatArray( const vector< Vec2f > &values ) {
      unsigned int size = (unsigned int) values.size();
      float *v = new float[ size * 2 ];
      for( unsigned int i = 0; i < size; ++i ) {
        const Vec2f &a = values[i];
        v[i*2] = a.x;
        v[i*2+1] = a.y;
      }
      return v;
    }

    
    float *toFloatArray( const vector< Vec3f > &values ) {
      unsigned int size = (unsigned int) values.size();
      float *v = new float[ size * 3 ];
      for( unsigned int i = 0; i < size; ++i ) {
        const Vec3f &a = values[i];
        v[i*3] = a.x;
        v[i*3+1] = a.y;
        v[i*3+2] = a.z;
      }
      return v;
    }

    
    float *toFloatArray( const vector< Vec4f > &values ) {
      unsigned int size = (unsigned int) values.size();
      float *v = new float[ size * 4 ];
      for( unsigned int i = 0; i < size; ++i ) {
        const Vec4f &a = values[i];
        v[i*4] = a.x;
        v[i*4+1] = a.y;
        v[i*4+2] = a.z;
        v[i*4+3] = a.w;
      }
      return v;
    }

    
    float *toFloatArray( const vector< Vec2d > &values ) {
      unsigned int size = (unsigned int) values.size();
      float *v = new float[ size * 2 ];
      for( unsigned int i = 0; i < size; ++i ) {
        const Vec2d &a = values[i];
        v[i*2]   = (H3DFloat) a.x;
        v[i*2+1] = (H3DFloat) a.y;
      }
      return v;
    }

    
    float *toFloatArray( const vector< Vec3d > &values ) {
      unsigned int size = (unsigned int) values.size();
      float *v = new float[ size * 3 ];
      for( unsigned int i = 0; i < size; ++i ) {
        const Vec3d &a = values[i];
        v[i*3] =   (H3DFloat)a.x;
        v[i*3+1] = (H3DFloat)a.y;
        v[i*3+2] = (H3DFloat)a.z;
      }
      return v;
    }

    
    float *toFloatArray( const vector< Vec4d > &values ) {
      unsigned int size = (unsigned int) values.size();
      float *v = new float[ size * 4 ];
      for( unsigned int i = 0; i < size; ++i ) {
        const Vec4d &a = values[i];
        v[i*4]   = (H3DFloat) a.x;
        v[i*4+1] = (H3DFloat) a.y;
        v[i*4+2] = (H3DFloat) a.z;
        v[i*4+3] = (H3DFloat) a.w;
      }
      return v;
    }

    
    float *toFloatArray( const vector< Rotation > &values ) {
      unsigned int size = (unsigned int) values.size();
      float *v = new float[ size * 4 ];
      for( unsigned int i = 0; i < size; ++i ) {
        const Rotation &r = values[i];
        v[i*4]   = (H3DFloat) r.axis.x;
        v[i*4+1] = (H3DFloat) r.axis.y;
        v[i*4+2] = (H3DFloat) r.axis.z;
        v[i*4+3] = (H3DFloat) r.angle;
      }
      return v;
    }

    
    float *toFloatArray( const vector< RGB > &values ) {
      unsigned int size = (unsigned int) values.size();
      float *v = new float[ size * 3 ];
      for( unsigned int i = 0; i < size; ++i ) {
        const RGB &r = values[i];
        v[i*3]   = (H3DFloat) r.r;
        v[i*3+1] = (H3DFloat) r.g;
        v[i*3+2] = (H3DFloat) r.b;
      }
      return v;
    }

    
    float *toFloatArray( const vector< RGBA > &values ) {
      unsigned int size = (unsigned int) values.size();
      float *v = new float[ size * 4 ];
      for( unsigned int i = 0; i < size; ++i ) {
        const RGBA &r = values[i];
        v[i*4]   = (H3DFloat) r.r;
        v[i*4+1] = (H3DFloat) r.g;
        v[i*4+2] = (H3DFloat) r.b;
        v[i*4+3] = (H3DFloat) r.a;
      }
      return v;
    }

    
    float *toFloatArray( const vector< Matrix3f > &values ) {
      unsigned int size = (unsigned int) values.size();
      float *v = new float[ size * 9 ];
      for( unsigned int i = 0; i < size; ++i ) {
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
      unsigned int size = (unsigned int) values.size();
      float *v = new float[ size * 16 ];
      for( unsigned int i = 0; i < size; ++i ) {
        const Matrix4f &m = values[i];
        v[i*16]   = (H3DFloat) m[0][0];
        v[i*16+1] = (H3DFloat) m[1][0];
        v[i*16+2] = (H3DFloat) m[2][0];
        v[i*16+3] = (H3DFloat) m[3][0];
        v[i*16+4] = (H3DFloat) m[0][1];
        v[i*16+5] = (H3DFloat) m[1][1];
        v[i*16+6] = (H3DFloat) m[2][1];
        v[i*16+7] = (H3DFloat) m[3][1];
        v[i*16+8] = (H3DFloat) m[0][2];
        v[i*16+9] = (H3DFloat) m[1][2];
        v[i*16+10] = (H3DFloat) m[2][2];
        v[i*16+11] = (H3DFloat) m[3][2];
        v[i*16+12] = (H3DFloat) m[0][3];        
        v[i*16+13] = (H3DFloat) m[1][3];
        v[i*16+14] = (H3DFloat) m[2][3];
        v[i*16+15] = (H3DFloat) m[3][3];
      }
      return v;
    }



    template< class Type >
    double *toDoubleArray( const vector< Type > &values ) {
      unsigned int size = (unsigned int)values.size();
      double *v = new double[ size ];
      for( unsigned int i = 0; i < size; ++i ) 
        v[i] = (double) values[i];
      return v;
    }

    double *toDoubleArray( const vector< Vec2f > &values ) {
      unsigned int size = (unsigned int) values.size();
      double *v = new double[ size * 2 ];
      for( unsigned int i = 0; i < size; ++i ) {
        const Vec2f &a = values[i];
        v[i*2] = a.x;
        v[i*2+1] = a.y;
      }
      return v;
    }

    
    double *toDoubleArray( const vector< Vec3f > &values ) {
      unsigned int size = (unsigned int) values.size();
      double *v = new double[ size * 3 ];
      for( unsigned int i = 0; i < size; ++i ) {
        const Vec3f &a = values[i];
        v[i*3] = a.x;
        v[i*3+1] = a.y;
        v[i*3+2] = a.z;
      }
      return v;
    }

    
    double *toDoubleArray( const vector< Vec4f > &values ) {
      unsigned int size = (unsigned int) values.size();
      double *v = new double[ size * 4 ];
      for( unsigned int i = 0; i < size; ++i ) {
        const Vec4f &a = values[i];
        v[i*4] = a.x;
        v[i*4+1] = a.y;
        v[i*4+2] = a.z;
        v[i*4+3] = a.w;
      }
      return v;
    }

    
    double *toDoubleArray( const vector< Vec2d > &values ) {
      unsigned int size = (unsigned int) values.size();
      double *v = new double[ size * 2 ];
      for( unsigned int i = 0; i < size; ++i ) {
        const Vec2d &a = values[i];
        v[i*2]   = (H3DDouble) a.x;
        v[i*2+1] = (H3DDouble) a.y;
      }
      return v;
    }

    
    double *toDoubleArray( const vector< Vec3d > &values ) {
      unsigned int size = (unsigned int) values.size();
      double *v = new double[ size * 3 ];
      for( unsigned int i = 0; i < size; ++i ) {
        const Vec3d &a = values[i];
        v[i*3] =   (H3DDouble)a.x;
        v[i*3+1] = (H3DDouble)a.y;
        v[i*3+2] = (H3DDouble)a.z;
      }
      return v;
    }

    
    double *toDoubleArray( const vector< Vec4d > &values ) {
      unsigned int size = (unsigned int) values.size();
      double *v = new double[ size * 4 ];
      for( unsigned int i = 0; i < size; ++i ) {
        const Vec4d &a = values[i];
        v[i*4]   = (H3DDouble) a.x;
        v[i*4+1] = (H3DDouble) a.y;
        v[i*4+2] = (H3DDouble) a.z;
        v[i*4+3] = (H3DDouble) a.w;
      }
      return v;
    }

    
    double *toDoubleArray( const vector< Rotation > &values ) {
      unsigned int size = (unsigned int) values.size();
      double *v = new double[ size * 4 ];
      for( unsigned int i = 0; i < size; ++i ) {
        const Rotation &r = values[i];
        v[i*4]   = (H3DDouble) r.axis.x;
        v[i*4+1] = (H3DDouble) r.axis.y;
        v[i*4+2] = (H3DDouble) r.axis.z;
        v[i*4+3] = (H3DDouble) r.angle;
      }
      return v;
    }

    
    double *toDoubleArray( const vector< RGB > &values ) {
      unsigned int size =(unsigned int)  values.size();
      double *v = new double[ size * 3 ];
      for( unsigned int i = 0; i < size; ++i ) {
        const RGB &r = values[i];
        v[i*3]   = (H3DDouble) r.r;
        v[i*3+1] = (H3DDouble) r.g;
        v[i*3+2] = (H3DDouble) r.b;
      }
      return v;
    }

    
    double *toDoubleArray( const vector< RGBA > &values ) {
      unsigned int size = (unsigned int) values.size();
      double *v = new double[ size * 4 ];
      for( unsigned int i = 0; i < size; ++i ) {
        const RGBA &r = values[i];
        v[i*4]   = (H3DDouble) r.r;
        v[i*4+1] = (H3DDouble) r.g;
        v[i*4+2] = (H3DDouble) r.b;
        v[i*4+3] = (H3DDouble) r.a;
      }
      return v;
    }

    
    double *toDoubleArray( const vector< Matrix3f > &values ) {
      unsigned int size = (unsigned int) values.size();
      double *v = new double[ size * 9 ];
      for( unsigned int i = 0; i < size; ++i ) {
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
      unsigned int size = (unsigned int) values.size();
      double *v = new double[ size * 16 ];
      for( unsigned int i = 0; i < size; ++i ) {
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

    double *toDoubleArray( const vector< Matrix3d > &values ){
      unsigned int size = ( unsigned int ) values.size();
      double *v = new double[ size * 9 ];
      for( unsigned int i = 0; i < size; ++i ) {
        const Matrix3d &m = values[i];
        v[i*9]   = m[0][0];
        v[i*9+1] = m[1][0];
        v[i*9+2] = m[2][0];
        v[i*9+3] = m[0][1];
        v[i*9+4] = m[1][1];
        v[i*9+5] = m[2][1];
        v[i*9+6] = m[0][2];
        v[i*9+7] = m[1][2];
        v[i*9+8] = m[2][2];
      }
      return v;
    }

    double *toDoubleArray( const vector< Matrix4d > &values ){
      unsigned int size = (unsigned int) values.size();
      double *v = new double[ size * 16 ];
      for( unsigned int i = 0; i < size; ++i ) {
        const Matrix4d &m = values[i];
        v[i*9]   = m[0][0];
        v[i*9+1] = m[1][0];
        v[i*9+2] = m[2][0];
        v[i*9+3] = m[3][0];
        v[i*9+4] = m[0][1];
        v[i*9+5] = m[1][1];
        v[i*9+6] = m[2][1];
        v[i*9+7] = m[3][1];
        v[i*9+8] = m[0][2];
        v[i*9+9] = m[1][2];
        v[i*9+10] = m[2][2];
        v[i*9+11] = m[3][2];
        v[i*9+12] = m[0][3];        
        v[i*9+13] = m[1][3];
        v[i*9+14] = m[2][3];
        v[i*9+15] = m[3][3];
      }
      return v;
    }
  }
}


/// Set the value of a uniform variable in the current GLSL shader.
/// The name of the uniform variable is the same as the name of the field. 
bool H3D::Shaders::setGLSLUniformVariableValue( GLhandleARB program_handle,
                                                Field *field, UniformInfo* ui, bool force ) {
  GLint location = -1;
  if( !ui ) {// no extra uniform info was set, need to extract location based on name
    const string &name = field->getName();
    location = glGetUniformLocationARB( program_handle,
      name.c_str() );
  } else
  {
    location = ui->location;
  }
  GLenum glerr1 = glGetError(); // clear the OpenGL error status
  if( location == -1 ) 
  {
    // check if the field is actually not uniform value
    // TODO: logically ShaderAtomicCounter and ShaderStorageBuffer field should never reach here
    // need to to handled earlier
    if ( field->getX3DType()==X3DTypes::SFNODE )
    {
      SFNode *f = static_cast< SFNode * >(field);
      Node *n = f->getValue ( );
      if ( n && (dynamic_cast<ShaderAtomicCounter*>(n)||dynamic_cast<ShaderStorageBuffer*>(n) ) ){
        return true;
      }
    }
    return false;
  }
  X3DTypes::X3DType x3d_type = field->getX3DType();
  switch(x3d_type)
  {
  case X3DTypes::SFFLOAT:
    {
      // implicit SFUniform field, check actualChanged
      if( !force && field->getTypeName()=="SFUniform" ) {
        SFUniform< SFFloat > * f = static_cast< SFUniform< SFFloat >* >( field );
        if( f->actualChanged ) {
          glUniform1fARB( location, f->getValue() );
        }
        break;
      }
      // normal X3D field, always set uniform value
      SFFloat* f = static_cast<SFFloat*>(field);
      glUniform1fARB( location, f->getValue() );
      break;
    }
  case X3DTypes::MFFLOAT:
    {
      MFFloat *f = static_cast< MFFloat * >( field );
      GLfloat *v = toFloatArray( f->getValue() );
      glUniform1fvARB( location, f->size(), v );
      delete[] v;
      break;
    }
  case X3DTypes::SFDOUBLE:
    {
      if( !force && field->getTypeName()=="SFUniform" ) {
        SFUniform< SFDouble > * f = static_cast< SFUniform< SFDouble >* >( field );
        if( f->actualChanged ) {
          glUniform1fARB( location, (GLfloat)f->getValue() );
        }
        break;
      }
      SFDouble* f = static_cast<SFDouble*>(field);
      glUniform1fARB( location, (GLfloat)f->getValue() );
      break;
    }
  case X3DTypes::MFDOUBLE:
    {
      MFDouble *f = static_cast< MFDouble * >( field );
      GLfloat *v = toFloatArray( f->getValue() );
      glUniform1fvARB( location, f->size(), v );
      delete[] v;
      break;
    }
  case X3DTypes::SFTIME:
    { 
      if( !force && field->getTypeName()=="SFUniform" ) {
        SFUniform< SFTime > * f = static_cast< SFUniform< SFTime >* >( field );
        if( f->actualChanged ) {
          glUniform1fARB( location, (GLfloat)f->getValue() );
        }
        break;
      }
      glUniform1fARB( location, (GLfloat)static_cast<SFTime*>(field)->getValue() );
      break;
    }
  case X3DTypes::MFTIME:
    {
      MFTime *f = static_cast< MFTime * >( field );
      GLfloat *v = toFloatArray( f->getValue() );
      glUniform1fvARB( location, f->size(), v );
      delete[] v;
      break;
    }
  case X3DTypes::SFINT32:
    {
      if( !force && field->getTypeName()=="SFUniform" ) {
        SFUniform< SFInt32 > * f = static_cast< SFUniform< SFInt32 >* >( field );
        if( f->actualChanged ) {
          glUniform1iARB( location, f->getValue() );
        }
        break;
      }
      glUniform1iARB( location, static_cast<SFInt32*>(field)->getValue() );
      break;
    }
  case X3DTypes::MFINT32:
    {
      MFInt32 *f = static_cast< MFInt32 * >( field );
      GLint *v = toIntArray( f->getValue() );
      glUniform1ivARB( location, f->size(), v );
      delete[] v;
      break;
    }
  case X3DTypes::SFVEC2F:
    {
      if( !force && field->getTypeName()=="SFUniform" ) {
        SFUniform< SFVec2f > * f = static_cast< SFUniform< SFVec2f >* >( field );
        if( f->actualChanged ) {
          const Vec2f &v = f->getValue(); 
          glUniform2fARB( location, 
            (GLfloat)v.x,
            (GLfloat)v.y );
        }
        break;
      }
      SFVec2f *f = static_cast< SFVec2f * >( field );
      const Vec2f &v = f->getValue(); 
      glUniform2fARB( location, 
                    (GLfloat)v.x,
                    (GLfloat)v.y );
      break;
    }
  case X3DTypes::MFVEC2F:
    {
      MFVec2f *f = static_cast< MFVec2f * >( field );
      GLfloat *v = toFloatArray( f->getValue() );
      glUniform2fvARB( location, f->size(), v );
      delete[] v;
      break;
    }
  case X3DTypes::SFVEC2D:
    {
      if( !force && field->getTypeName()=="SFUniform" ) {
        SFUniform< SFVec2d > * f = static_cast< SFUniform< SFVec2d >* >( field );
        if( f->actualChanged ) {
          const Vec2d &v = f->getValue(); 
          glUniform2fARB( location, 
            (GLfloat)v.x,
            (GLfloat)v.y );
        }
        break;
      }
      SFVec2d *f = static_cast< SFVec2d * >( field );
      const Vec2d &v = f->getValue(); 
      glUniform2fARB( location, 
                    (GLfloat)v.x,
                    (GLfloat)v.y );
      break;
    }
  case X3DTypes::MFVEC2D:
    {
      MFVec2d *f = static_cast< MFVec2d * >( field );
      GLfloat *v = toFloatArray( f->getValue() );
      glUniform2fvARB( location, f->size(), v );
      delete[] v;
      break;
    }
  case X3DTypes::SFVEC3F:
    {
      if( !force && field->getTypeName()=="SFUniform" ) {
        SFUniform< SFVec3f > * f = static_cast< SFUniform< SFVec3f >* >( field );
        if( f->actualChanged ) {
          const Vec3f &v = f->getValue(); 
          glUniform3fARB( location, 
            (GLfloat)v.x,
            (GLfloat)v.y,
            (GLfloat)v.z );
        }
        break;
      }
      SFVec3f *f = static_cast< SFVec3f * >( field );
      const Vec3f &v = f->getValue(); 
      glUniform3fARB( location, 
                    (GLfloat)v.x,
                    (GLfloat)v.y,
                    (GLfloat)v.z );
      break;
    }
  case X3DTypes::MFVEC3F:
    {
      MFVec3f *f = static_cast< MFVec3f * >( field );
      GLfloat *v = toFloatArray( f->getValue() );
      glUniform3fvARB( location, f->size(), v );
      delete[] v;
      break;
    }
  case X3DTypes::SFVEC3D:
    {
      if( !force && field->getTypeName()=="SFUniform" ) {
        SFUniform< SFVec3d > * f = static_cast< SFUniform< SFVec3d >* >( field );
        if( f->actualChanged ) {
          const Vec3d &v = f->getValue(); 
          glUniform3fARB( location, 
            (GLfloat)v.x,
            (GLfloat)v.y,
            (GLfloat)v.z );
        }
        break;
      }
      SFVec3d *f = static_cast< SFVec3d * >( field );
      const Vec3d &v = f->getValue(); 
      glUniform3fARB( location, 
                    (GLfloat)v.x,
                    (GLfloat)v.y,
                    (GLfloat)v.z );
      break;
    }
  case X3DTypes::MFVEC3D:
    {
      MFVec3d *f = static_cast< MFVec3d * >( field );
      GLfloat *v = toFloatArray( f->getValue() );
      glUniform3fvARB( location, f->size(), v );
      delete[] v;
      break;
    }
  case X3DTypes::SFVEC4F:
    {
      if( !force && field->getTypeName()=="SFUniform" ) {
        SFUniform< SFVec4f > * f = static_cast< SFUniform< SFVec4f >* >( field );
        if( f->actualChanged ) {
          const Vec4f &v = f->getValue(); 
          glUniform4fARB( location, 
            (GLfloat)v.x,
            (GLfloat)v.y,
            (GLfloat)v.z,
            (GLfloat)v.w  );
        }
        break;
      }
      SFVec4f *f = static_cast< SFVec4f * >( field );
      const Vec4f &v = f->getValue(); 
      glUniform4fARB( location, 
                    (GLfloat)v.x,
                    (GLfloat)v.y,
                    (GLfloat)v.z,
                    (GLfloat)v.w  );
      break;
    }
  case X3DTypes::MFVEC4F:
    {
      MFVec4f *f = static_cast< MFVec4f * >( field );
      GLfloat *v = toFloatArray( f->getValue() );
      glUniform4fvARB( location, f->size(), v );
      delete[] v;
      break;
    }
  case X3DTypes::SFVEC4D:
    {
      if( !force && field->getTypeName()=="SFUniform" ) {
        SFUniform< SFVec4d > * f = static_cast< SFUniform< SFVec4d >* >( field );
        if( f->actualChanged ) {
          const Vec4d &v = f->getValue(); 
          glUniform4fARB( location, 
            (GLfloat)v.x,
            (GLfloat)v.y,
            (GLfloat)v.z,
            (GLfloat)v.w  );
        }
        break;
      }
      SFVec4d *f = static_cast< SFVec4d * >( field );
      const Vec4d &v = f->getValue(); 
      glUniform4fARB( location, 
                    (GLfloat)v.x,
                    (GLfloat)v.y,
                    (GLfloat)v.z,
                    (GLfloat)v.w  );
      break;
    }
  case X3DTypes::MFVEC4D:
    {
      MFVec4d *f = static_cast< MFVec4d * >( field );
      GLfloat *v = toFloatArray( f->getValue() );
      glUniform4fvARB( location, f->size(), v );
      delete[] v;
      break;
    }
  case X3DTypes::SFBOOL:
    {
      if( !force && field->getTypeName()=="SFUniform" ) {
        SFUniform< SFBool > * f = static_cast< SFUniform< SFBool >* >( field );
        if( f->actualChanged ) {
          glUniform1iARB( location, f->getValue() );
        }
        break;
      }
      glUniform1iARB( location, static_cast<SFBool*>(field)->getValue() );
      break;
    }

  case X3DTypes::MFBOOL:
    {
      MFBool *f = static_cast<MFBool*>(field);
      GLint *v = toIntArray( f->getValue() );
      glUniform1ivARB( location, f->size(), v );
      delete[] v;
      break;
    }
  case X3DTypes::SFSTRING: return false;
  case X3DTypes::MFSTRING: return false;
  case X3DTypes::SFNODE:
    {
      SFNode *f = static_cast< SFNode * >( field );
      Node *n = f->getValue(); 
      if( n == NULL ) return true;
      if( H3DSingleTextureNode *t = dynamic_cast< H3DSingleTextureNode *>( n ) ) 
      {
#ifdef GL_ARB_bindless_texture
        if ( X3DProgrammableShaderObject::use_bindless_textures ) {
          GLuint64 h= t->getTextureHandle();
          if ( h != 0 ) {
            glUniformHandleui64ARB ( location, h );
          }
        } else {
#endif
          glUniform1iARB( location, t->getTextureUnit() - GL_TEXTURE0_ARB );
#ifdef GL_ARB_bindless_texture
        }
#endif
        break;
      }
      else if ( ShaderImageNode* si = dynamic_cast< ShaderImageNode* >( n ) )
      {
        glUniform1iARB ( location, si->getImageUnit ( ) );
        break;
      }
      //else if ( ShaderImageNode *i = dynamic_cast< ShaderImageNode* >( n ) )
      //{
      //  /*glUniform1i ( location, t->getImageUnit ( ) );*/
      //}
      else 
      {
        return false;
      }
    }
  case X3DTypes::MFNODE:
    {
      MFNode *f = static_cast< MFNode * >( field );
      unsigned int size = f->size();
      GLint *v = NULL;
      GLuint64 *b = NULL;
      for( unsigned int i = 0; i < size; ++i ) 
      {
        Node *n = f->getValueByIndex( i ); 
        if( n == NULL ) continue;
        if( H3DSingleTextureNode *t = dynamic_cast< H3DSingleTextureNode *>( n ) ) 
        {
          if ( X3DProgrammableShaderObject::use_bindless_textures ) {
            GLuint64 h= t->getTextureHandle();
            if ( h != 0 ) {
              if ( !b ) b= new GLuint64 [ size ];
              b[i] = h;
            } else {
              delete[] b;
              b= NULL;
              break; // come back when you have a handle
            }
          } else {
            if ( !v ) v= new GLint [ size ];
            v[i] = t->getTextureUnit() - GL_TEXTURE0_ARB;
          }

          
        } 
        else if ( ShaderImageNode* si = dynamic_cast< ShaderImageNode* >( n ) )
        {
          if ( !v ) v= new GLint [ size ];
          v[i] = si->getImageUnit ( );
        }
        else 
        {
          delete [] v;
          delete [] b;
          return false;
        }
      }

      if ( v && b ) {
        Console(4) << "ERROR: You cannot mix H3DSingleTextureNode and ShaderImageNode in the "
          "same MFNode shader field when using bindless textures!" << endl;
        return false;
      } else if ( v ) {
        glUniform1ivARB( location, size, v );
        delete[] v;
      } else if ( b ) {
#ifdef GL_ARB_bindless_texture
        glUniformHandleui64vARB ( location, size, b );
#endif
        delete[] b;
      }
      
      break;
    }
  case X3DTypes::SFCOLOR:
    {
      if( !force && field->getTypeName()=="SFUniform" ) {
        SFUniform< SFColor > * f = static_cast< SFUniform< SFColor >* >( field );
        if( f->actualChanged ) {
          const RGB &r = f->getValue(); 
          glUniform3fARB( location, 
            (GLfloat)r.r,
            (GLfloat)r.g,
            (GLfloat)r.b );
        }
        break;
      }
      SFColor *f = static_cast< SFColor * >( field );
      const RGB &r = f->getValue(); 
      glUniform3fARB( location, 
                    (GLfloat)r.r,
                    (GLfloat)r.g,
                    (GLfloat)r.b );
      break;
    }
  case X3DTypes::MFCOLOR:
    {
      MFColor *f = static_cast< MFColor * >( field );
      GLfloat *v = toFloatArray( f->getValue() );
      glUniform4fvARB( location, f->size(), v );
      delete[] v;
      break;
    }
  case X3DTypes::SFCOLORRGBA:
    {
      if( !force && field->getTypeName()=="SFUniform" ) {
        SFUniform< SFColorRGBA > * f = static_cast< SFUniform< SFColorRGBA >* >( field );
        if( f->actualChanged ) {
          const RGBA &r = f->getValue(); 
          glUniform4fARB( location, 
            (GLfloat)r.r,
            (GLfloat)r.g,
            (GLfloat)r.b,
            (GLfloat)r.a );
        }
        break;
      }
      SFColorRGBA *f = static_cast< SFColorRGBA * >( field );
      const RGBA &r = f->getValue(); 
      glUniform4fARB( location, 
                    (GLfloat)r.r,
                    (GLfloat)r.g,
                    (GLfloat)r.b,
                    (GLfloat)r.a );
      break;
    }
  case X3DTypes::MFCOLORRGBA:
    {
      MFColorRGBA *f = static_cast< MFColorRGBA * >( field );
      GLfloat *v = toFloatArray( f->getValue() );
      glUniform4fvARB( location, f->size(), v );
      delete[] v;
      break;
    }
  case X3DTypes::SFROTATION:
    {
      if( !force && field->getTypeName()=="SFUniform" ) {
        SFUniform< SFRotation > * f = static_cast< SFUniform< SFRotation >* >( field );
        if( f->actualChanged ) {
          const Rotation &r = f->getValue(); 
          glUniform4fARB( location, 
            (GLfloat)r.axis.x,
            (GLfloat)r.axis.y,
            (GLfloat)r.axis.z,
            (GLfloat)r.angle  );
        }
        break;
      }
      SFRotation *f = static_cast< SFRotation * >( field );
      const Rotation &r = f->getValue(); 
      glUniform4fARB( location, 
                    (GLfloat)r.axis.x,
                    (GLfloat)r.axis.y,
                    (GLfloat)r.axis.z,
                    (GLfloat)r.angle  );
      break;
    }
  case X3DTypes::MFROTATION:
    {
      MFRotation *f = static_cast< MFRotation * >( field );
      GLfloat *v = toFloatArray( f->getValue() );
      glUniform4fvARB( location, f->size(), v );
      delete[] v;
      break;
    }
  case X3DTypes::SFQUATERNION: return false;
  case X3DTypes::MFQUATERNION: return false;
  case X3DTypes::SFMATRIX3F:
    {
      if( !force && field->getTypeName()=="SFUniform" ) {
        SFUniform< SFMatrix3f > * f = static_cast< SFUniform< SFMatrix3f >* >( field );
        if( f->actualChanged ) {
          const Matrix3f &m = f->getValue(); 
          glUniformMatrix3fvARB( location, 
            1,
            true,
            m[0] );
        }
        break;
      }
      SFMatrix3f *f = static_cast< SFMatrix3f * >( field );
      const Matrix3f &m = f->getValue(); 
      glUniformMatrix3fvARB( location, 
                           1,
                           true,
                           m[0] );
      break;
    }
  case X3DTypes::MFMATRIX3F:
    {
      MFMatrix3f *f = static_cast< MFMatrix3f * >( field );
      GLfloat *v = toFloatArray( f->getValue() );
      glUniformMatrix3fvARB( location, f->size(), false, v );
      delete[] v;
      break;
    }
  case X3DTypes::SFMATRIX3D:
    {
      if( !force && field->getTypeName()=="SFUniform" ) {
        SFUniform< SFMatrix3d > * f = static_cast< SFUniform< SFMatrix3d >* >( field );
        if( f->actualChanged ) {
          const Matrix3d &m = f->getValue(); 
          glUniformMatrix3fvARB( location, 
            1,
            true,
            (GLfloat*)m[0] );
        }
        break;
      }
      SFMatrix3d *f = static_cast< SFMatrix3d* >( field );
      const Matrix3d &m = f->getValue();
      glUniformMatrix3fvARB( location, 
                            1,
                            true,
                            (GLfloat*)m[0]);
      break;
    }
  case X3DTypes::MFMATRIX3D:
    {
      MFMatrix3d *f = static_cast< MFMatrix3d* >( field );
      GLdouble* v = toDoubleArray( f->getValue() );
      glUniformMatrix3fvARB( location, f->size(), false, (GLfloat*)v );
      delete[] v;
      break;
    }
  case X3DTypes::SFMATRIX4F:
    {
      if( !force && field->getTypeName()=="SFUniform" ) {
        SFUniform< SFMatrix4f > * f = static_cast< SFUniform< SFMatrix4f >* >( field );
        if( f->actualChanged ) {
          const Matrix4f &m = f->getValue(); 
          glUniformMatrix4fvARB( location, 
            1,
            true,
            m[0] );
        }
        break;
      }
      SFMatrix4f *f = static_cast< SFMatrix4f * >( field );
      const Matrix4f &m = f->getValue(); 
      glUniformMatrix4fvARB( location, 
                           1,
                           true,
                           m[0] );
      break;
    }
  case X3DTypes::MFMATRIX4F:
    {
      MFMatrix4f *f = static_cast< MFMatrix4f * >( field );
      GLfloat *v = toFloatArray( f->getValue() );
      glUniformMatrix4fvARB( location, f->size(), false, v );
      delete[] v;
      break;
    }
  case  X3DTypes::SFMATRIX4D:
    {
      if( !force && field->getTypeName()=="SFUniform" ) {
        SFUniform< SFMatrix4d > * f = static_cast< SFUniform< SFMatrix4d >* >( field );
        if( f->actualChanged ) {
          const Matrix4d &m = f->getValue(); 
          glUniformMatrix4fvARB( location, 
            1,
            true,
            (GLfloat*)m[0] );
        }
        break;
      }
      SFMatrix4d * f = static_cast< SFMatrix4d* >( field );
      const Matrix4d &m = f->getValue();
      glUniformMatrix4fvARB( location,
                            1,
                            true,
                            (GLfloat*)m[0]);
      break;
    }
  case X3DTypes::MFMATRIX4D:
    {
      MFMatrix4d *f = static_cast< MFMatrix4d* >( field );
      GLdouble *v = toDoubleArray( f->getValue() );
      glUniformMatrix4fvARB( location, f->size(), false, (GLfloat*)v );
      delete[] v;
      break;
    }
  default:
    return false;                                                                   
  } 
  GLenum glerr = glGetError();
  
  // ignore any errors that occurs when setting uniform variables.
  return glerr == GL_NO_ERROR;
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
      Console(3) << "Warning: Invalid profile name" << endl;
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
    delete[] v;
  } else if( SFInt32 *f = dynamic_cast< SFInt32 * >( field ) ) {
    cgGLSetParameter1f( param, (GLfloat)f->getValue() );
  } else if( MFInt32 *f = dynamic_cast< MFInt32 * >( field ) ) {
    GLfloat *v = toFloatArray( f->getValue() );
    cgGLSetParameterArray1f( param, 0, f->size(), v );
    delete[] v;
  } else if( SFFloat *f = dynamic_cast< SFFloat * >( field ) ) {
    cgGLSetParameter1f( param, f->getValue() );
  } else if( MFFloat *f = dynamic_cast< MFFloat * >( field ) ) {
    GLfloat *v = toFloatArray( f->getValue() );
    cgGLSetParameterArray1f( param, 0, f->size(), v );
    delete[] v;
  } else if( SFDouble *f = dynamic_cast< SFDouble * >( field ) ) {
    cgGLSetParameter1f( param, (GLfloat)f->getValue() );
  } else if( MFDouble *f = dynamic_cast< MFDouble * >( field ) ) {
    GLfloat *v = toFloatArray( f->getValue() );
    cgGLSetParameterArray1f( param, 0, f->size(), v );
    delete[] v;
  } else if( SFTime *f = dynamic_cast< SFTime * >( field ) ) {
    cgGLSetParameter1d( param, f->getValue() );
  } else if( MFTime *f = dynamic_cast< MFTime * >( field ) ) {
    GLdouble *v = toDoubleArray( f->getValue() );
    cgGLSetParameterArray1d( param, 0, f->size(), v );
    delete[] v;
  } else if( SFVec2f *f = dynamic_cast< SFVec2f * >( field ) ) {
    const Vec2f &v = f->getValue(); 
    cgGLSetParameter2f( param, (GLfloat)v.x, (GLfloat)v.y );
  } else if( MFVec2f *f = dynamic_cast< MFVec2f * >( field ) ) {
    GLfloat *v = toFloatArray( f->getValue() );
    cgGLSetParameterArray2f( param, 0, f->size(), v );
    delete[] v;
  } else if( SFVec3f *f = dynamic_cast< SFVec3f * >( field ) ) {
    const Vec3f &v = f->getValue(); 
    cgGLSetParameter3f( param, 
                    (GLfloat)v.x,
                    (GLfloat)v.y,
                    (GLfloat)v.z );
  } else if( MFVec3f *f = dynamic_cast< MFVec3f * >( field ) ) {
    GLfloat *v = toFloatArray( f->getValue() );
    cgGLSetParameterArray3f( param, 0, f->size(), v );
    delete[] v;
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
    delete[] v;
  } else if( SFVec2d *f = dynamic_cast< SFVec2d * >( field ) ) {
    const Vec2d &v = f->getValue(); 
    cgGLSetParameter2d( param, v.x, v.y );
  } else if( MFVec2d *f = dynamic_cast< MFVec2d * >( field ) ) {
    GLdouble *v = toDoubleArray( f->getValue() );
    cgGLSetParameterArray2d( param, 0, f->size(), v );
    delete[] v;
  } else if( SFVec3d *f = dynamic_cast< SFVec3d * >( field ) ) {
    const Vec3d &v = f->getValue(); 
    cgGLSetParameter3d( param, v.x, v.y, v.z );
  } else if( MFVec3d *f = dynamic_cast< MFVec3d * >( field ) ) {
    GLdouble *v = toDoubleArray( f->getValue() );
    cgGLSetParameterArray3d( param, 0, f->size(), v );
    delete[] v;
  } else if( SFVec4d *f = dynamic_cast< SFVec4d * >( field ) ) {
    const Vec4d &v = f->getValue(); 
    cgGLSetParameter4d( param, v.x, v.y, v.z, v.w  );
  } else if( MFVec4d *f = dynamic_cast< MFVec4d * >( field ) ) {
    GLdouble *v = toDoubleArray( f->getValue() );
    cgGLSetParameterArray4d( param, 0, f->size(), v );
    delete[] v;
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
    delete[] v;
  } else if( SFColor *f = dynamic_cast< SFColor * >( field ) ) {
    const RGB &r = f->getValue(); 
    cgGLSetParameter3f( param, 
                    (GLfloat)r.r,
                    (GLfloat)r.g,
                    (GLfloat)r.b );
  } else if( MFColor *f = dynamic_cast< MFColor * >( field ) ) {
    GLfloat *v = toFloatArray( f->getValue() );
    cgGLSetParameterArray4f( param, 0, f->size(), v );
    delete[] v;
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
    delete[] v;
  } else if( SFMatrix3f *f = dynamic_cast< SFMatrix3f * >( field ) ) {
    const Matrix3f &m = f->getValue(); 
    cgGLSetMatrixParameterfr( param, m[0] );
  } else if( MFMatrix3f *f = dynamic_cast< MFMatrix3f * >( field ) ) {
    GLfloat *v = toFloatArray( f->getValue() );
    cgGLSetMatrixParameterArrayfr( param, 0, f->size(), v );
    delete[] v;
  } else if( SFMatrix4f *f = dynamic_cast< SFMatrix4f * >( field ) ) {
    const Matrix4f &m = f->getValue(); 
    cgGLSetMatrixParameterfr( param, m[0] );
  } else if( MFMatrix4f *f = dynamic_cast< MFMatrix4f * >( field ) ) {
    GLfloat *v = toFloatArray( f->getValue() );
    cgGLSetMatrixParameterArrayfr( param, 0, f->size(), v );
    delete[] v;
  } else if( SFNode *f = dynamic_cast< SFNode * >( field ) ) {
    Node *n = f->getValue(); 
    if( H3DSingleTextureNode *t = dynamic_cast< H3DSingleTextureNode *>( n ) ) {
      cgGLSetTextureParameter( param, t->getTextureId());
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
    Console(3) << cgGetErrorString( err ) << endl;
  return err == GL_NO_ERROR;
 }

#endif // HAVE_CG

GLbitfield H3D::Shaders::getAffectedGLAttribs( H3DDynamicFieldsObject *dfo ) {
  GLbitfield res = 0;
  Node *n;
  MFNode *mfnode;
  for( H3DDynamicFieldsObject::field_iterator f = dfo->firstField(); f != dfo->endField(); ++f ) 
  {
    X3DTypes::X3DType x3d_type = (*f)->getX3DType();
    if(x3d_type==X3DTypes::SFNODE)
    {
      n = static_cast<SFNode*>(*f)->getValue(); 
      if( H3DSingleTextureNode *t = 
        dynamic_cast< H3DSingleTextureNode *>( n ) ) 
      {
          res = res | t->getAffectedGLAttribs();
      } 
    }
    else if(x3d_type==X3DTypes::MFNODE)
    {
      mfnode = static_cast< MFNode * >( *f );
      for( unsigned int i = 0; i < mfnode->size(); ++i ) {
        n = mfnode->getValueByIndex( i ); 
        if( H3DSingleTextureNode *t = 
          dynamic_cast< H3DSingleTextureNode *>( n ) ) {
            res = res | t->getAffectedGLAttribs();
        } 
      }
    }
  }
  return res;
}

void H3D::Shaders::preRenderTextures( H3DDynamicFieldsObject *dfo ) {

  if ( X3DProgrammableShaderObject::use_bindless_textures ) {
    // Bindless textures

    Node* n;
    MFNode *mfnode;
    for( H3DDynamicFieldsObject::field_iterator f = dfo->firstField();f != dfo->endField(); ++f ) 
    {
      // only SFNODE and MFNODE type of in the dynamic filed can be texture object
      if((*f)->getX3DType()==X3DTypes::SFNODE)
      {
        n = static_cast<SFNode*>(*f)->getValue(); 
        if( H3DSingleTextureNode *t = 
          dynamic_cast< H3DSingleTextureNode *>( n ) ) {
            GLuint64 h= t->getTextureHandle();
          if ( h == 0 ) {
            t->displayList->callList();

            if ( t->getTextureId() != 0 ) {

              if ( !t->makeResident () ) {
                Console(4) << "ERROR: Cannot make texture resident " << (*f)->getFullName () << ": " << t->getName() << endl;
              }

            }
          }
        } 
      }
      else if((*f)->getX3DType()==X3DTypes::MFNODE)
      {
        mfnode = static_cast< MFNode * >( *f );
        for( unsigned int i = 0; i < mfnode->size(); ++i ) {
          Node *n = mfnode->getValueByIndex( i ); 
          if( H3DSingleTextureNode *t = 
            dynamic_cast< H3DSingleTextureNode *>( n ) ) {
            GLuint64 h= t->getTextureHandle();
            if ( h == 0 ) {
              t->displayList->callList();

              if ( t->getTextureId() != 0 ) {

                if ( !t->makeResident () ) {
                  Console(4) << "ERROR: Cannot make texture resident " << (*f)->getFullName () << ": " << t->getName() << endl;
                }

              }
            }
          }
        }
      }
    }
  } else {
    // Using texture binding
    GLint nr_textures_supported;
    glGetIntegerv( GL_MAX_TEXTURE_IMAGE_UNITS_ARB, &nr_textures_supported );
    X3DTextureNode *active_texture = X3DTextureNode::getActiveTexture();
    unsigned int nr_textures = 0; 
    Node* n;
    MFNode *mfnode;
    GLenum current_texture_unit = GL_TEXTURE0_ARB;
    for( H3DDynamicFieldsObject::field_iterator f = dfo->firstField();f != dfo->endField(); ++f ) 
    {
      // only SFNODE and MFNODE type of in the dynamic filed can be texture object
      if((*f)->getX3DType()==X3DTypes::SFNODE)
      {
        n = static_cast<SFNode*>(*f)->getValue(); 
        if( H3DSingleTextureNode *t = 
          dynamic_cast< H3DSingleTextureNode *>( n ) ) {
            glActiveTextureARB(GL_TEXTURE0_ARB + nr_textures );
            t->preRender();
            ++nr_textures;
        } 
        if ( ShaderImageNode *si = dynamic_cast<ShaderImageNode*>(n) ){
          current_texture_unit = GL_TEXTURE0_ARB + nr_textures;
          glActiveTextureARB ( current_texture_unit );
          si->preRender ( current_texture_unit );
          ++nr_textures;
        }
      }
      else if((*f)->getX3DType()==X3DTypes::MFNODE)
      {
        mfnode = static_cast< MFNode * >( *f );
        for( unsigned int i = 0; i < mfnode->size(); ++i ) {
          Node *n = mfnode->getValueByIndex( i ); 
          if( H3DSingleTextureNode *t = 
            dynamic_cast< H3DSingleTextureNode *>( n ) ) {
              glActiveTextureARB(GL_TEXTURE0_ARB + nr_textures );
              t->preRender();
              ++nr_textures;
          }
          else if ( ShaderImageNode* si = dynamic_cast<ShaderImageNode*>(n) ){
            current_texture_unit = GL_TEXTURE0_ARB + nr_textures;
            glActiveTextureARB ( current_texture_unit );
            si->preRender ( current_texture_unit );
          }
        }
      }
      if( nr_textures > (unsigned int)nr_textures_supported ) {
        Console(4) << "Warning: Nr of textures provided to shader is larger than the maximum number supported(" << nr_textures_supported << ") " << endl;
        break;
      }
    }

    glActiveTextureARB(GL_TEXTURE0_ARB );
    X3DTextureNode::setActiveTexture( active_texture );
  }
}

// preRender some resources to assign necessary context value of the shader
// to specific shader resources. For example, preRender shaderImage
// shaderStorageBuffer, shaderAtomicCounter
void H3D::Shaders::preRenderShaderResources( H3DDynamicFieldsObject * dfo, GLhandleARB program){

  Node* n;
  MFNode *mfnode;
  for( H3DDynamicFieldsObject::field_iterator f = dfo->firstField();f != dfo->endField(); ++f ) {
    if((*f)->getX3DType()==X3DTypes::SFNODE){
      n = static_cast<SFNode*>(*f)->getValue(); 
      if( ShaderStorageBuffer* ssbo = dynamic_cast<ShaderStorageBuffer*>(n) ) {
        ssbo->preRender( program );
      }else if( ShaderAtomicCounter* sac = dynamic_cast<ShaderAtomicCounter*>(n) ) {
        sac->preRender( program );
      }
    }else if( (*f)->getX3DType()==X3DTypes::MFNODE ) {
      mfnode = static_cast< MFNode * >( *f );
      for( unsigned int i = 0; i < mfnode->size(); ++i ) {
        n = mfnode->getValueByIndex( i ); 
        if( ShaderStorageBuffer* ssbo = dynamic_cast<ShaderStorageBuffer*>(n) ) {
          ssbo->preRender( program );
        }else if( ShaderAtomicCounter* sac = dynamic_cast<ShaderAtomicCounter*>(n) ) {
          sac->preRender( program );
        }
      }
    }
  }
}

void H3D::Shaders::postRenderShaderResources( H3DDynamicFieldsObject* dfo, GLhandleARB program){
  Node* n;
  MFNode *mfnode;
  for( H3DDynamicFieldsObject::field_iterator f = dfo->firstField();f != dfo->endField(); ++f ) {
    if((*f)->getX3DType()==X3DTypes::SFNODE){
      n = static_cast<SFNode*>(*f)->getValue(); 
      if( ShaderStorageBuffer* ssbo = dynamic_cast<ShaderStorageBuffer*>(n) ) {
        ssbo->preRender( program );
      }else if( ShaderAtomicCounter* sac = dynamic_cast<ShaderAtomicCounter*>(n) ) {
        sac->preRender( program );
      }
    }else if( (*f)->getX3DType()==X3DTypes::MFNODE ) {
      mfnode = static_cast< MFNode * >( *f );
      for( unsigned int i = 0; i < mfnode->size(); ++i ) {
        n = mfnode->getValueByIndex( i ); 
        if( ShaderStorageBuffer* ssbo = dynamic_cast<ShaderStorageBuffer*>(n) ) {
          ssbo->preRender( program );
        }else if( ShaderAtomicCounter* sac = dynamic_cast<ShaderAtomicCounter*>(n) ) {
          sac->preRender( program );
        }
      }
    }
  }
}

void H3D::Shaders::postRenderTextures( H3DDynamicFieldsObject *dfo ) {
  if ( X3DProgrammableShaderObject::use_bindless_textures ) return;

  GLint nr_textures_supported;
  glGetIntegerv( GL_MAX_TEXTURE_IMAGE_UNITS_ARB, &nr_textures_supported );

  unsigned int nr_textures = 0; 
  Node *n;
  MFNode *mfnode;
  GLenum current_texture_unit = GL_TEXTURE0_ARB;
  for( H3DDynamicFieldsObject::field_iterator f = dfo->firstField();f != dfo->endField(); ++f ) 
  {
    // only SFNODE and MFNODE type can be texture object.
    if((*f)->getX3DType()==X3DTypes::SFNODE)
    {
      n = static_cast<SFNode*>(*f)->getValue();
      if( H3DSingleTextureNode *t = dynamic_cast< H3DSingleTextureNode *>( n ) ) 
      {
        glActiveTextureARB(GL_TEXTURE0_ARB + nr_textures );
        t->postRender();
        ++nr_textures;
      } else if ( ShaderImageNode *si = dynamic_cast<ShaderImageNode*>(n) ){
        current_texture_unit = GL_TEXTURE0_ARB + nr_textures;
        glActiveTextureARB ( current_texture_unit );
        si->preRender ( current_texture_unit );
        ++nr_textures;
      }
    }
    else if((*f)->getX3DType()==X3DTypes::MFNODE) 
    {
      mfnode = static_cast< MFNode * >( *f );
      for( unsigned int i = 0; i < mfnode->size(); ++i ) {
        n = mfnode->getValueByIndex( i ); 

        if( H3DSingleTextureNode *t = 
          dynamic_cast< H3DSingleTextureNode *>( n ) ) {
            glActiveTextureARB(GL_TEXTURE0_ARB + nr_textures );
            t->postRender();
            ++nr_textures;
        } else if ( ShaderImageNode* si = dynamic_cast<ShaderImageNode*>(n) ){
          current_texture_unit = GL_TEXTURE0_ARB + nr_textures;
          glActiveTextureARB ( current_texture_unit );
          si->preRender ( current_texture_unit );
        }
      }
    }
    if( nr_textures > (unsigned int)nr_textures_supported ) {
      Console(4) << "Warning: Nr of textures provided to shader is larger than the maximum number supported(" << nr_textures_supported << ") " << endl;
      break;
    }
  }
  glActiveTextureARB(GL_TEXTURE0_ARB );
}


void H3D::Shaders::renderTextures( H3DDynamicFieldsObject *dfo ) {
  if ( X3DProgrammableShaderObject::use_bindless_textures ) return;

  GLint nr_textures_supported;
  glGetIntegerv( GL_MAX_TEXTURE_IMAGE_UNITS_ARB, &nr_textures_supported );
  unsigned int nr_textures = 0; 
  Node* n;
  MFNode* mfnode;
  for( H3DDynamicFieldsObject::field_iterator f = dfo->firstField();f != dfo->endField(); ++f ) 
  {
    X3DTypes::X3DType x3d_type = (*f)->getX3DType();
    if(x3d_type==X3DTypes::SFNODE)
    {
      n = static_cast<SFNode*>(*f)->getValue(); 
      if( H3DSingleTextureNode *t = 
        dynamic_cast< H3DSingleTextureNode *>( n ) ) {
          glActiveTextureARB(GL_TEXTURE0_ARB + nr_textures );
          t->displayList->callList();
          ++nr_textures;
      }
      else if ( ShaderImageNode *si = dynamic_cast<ShaderImageNode*>( n ) ){
        glActiveTextureARB (GL_TEXTURE0_ARB + nr_textures ); // this is not needed, in theory
        si->displayList->callList ( );
        ++nr_textures;
      }
    }
    else if(x3d_type==X3DTypes::MFNODE)
    {
      mfnode = static_cast< MFNode * >( *f );
      for( unsigned int i = 0; i < mfnode->size(); ++i ) {
          Node *n = mfnode->getValueByIndex( i ); 
          if( H3DSingleTextureNode *t = 
            dynamic_cast< H3DSingleTextureNode *>( n ) ) {
              glActiveTextureARB(GL_TEXTURE0_ARB + nr_textures );
              t->displayList->callList();
              ++nr_textures;
          }
          else if ( ShaderImageNode *si = dynamic_cast<ShaderImageNode*>( n ) ){
            glActiveTextureARB ( GL_TEXTURE0_ARB + nr_textures );
            si->displayList->callList ( );
            ++nr_textures;
          }
        }
    }
    if( nr_textures > (unsigned int)nr_textures_supported ) {
      Console(4) << "Warning: Nr of textures provided to shader is larger than the maximum number supported(" << nr_textures_supported << ") " << endl;
      break;
    }
  }
  glActiveTextureARB(GL_TEXTURE0_ARB );
}

void H3D::Shaders::renderShaderResources( H3DDynamicFieldsObject * dfo){
  Node* n;
  MFNode* mfnode;
  for( H3DDynamicFieldsObject::field_iterator f = dfo->firstField();f != dfo->endField(); ++f ) 
  {
    X3DTypes::X3DType x3d_type = (*f)->getX3DType();
    if(x3d_type==X3DTypes::SFNODE){
      n = static_cast<SFNode*>(*f)->getValue();
      if( ShaderStorageBuffer* ssbo = dynamic_cast<ShaderStorageBuffer*>(n) ) {
        ssbo->displayList->callList();
      }else if( ShaderAtomicCounter* sac = dynamic_cast<ShaderAtomicCounter*>(n) ) {
        sac->displayList->callList();
      }
    }else if( x3d_type==X3DTypes::MFNODE ) {
      mfnode = static_cast< MFNode* >(*f);
      for( unsigned int i = 0; i < mfnode->size(); ++i ) {
        Node *n = mfnode->getValueByIndex( i ); 
        if( ShaderStorageBuffer* ssbo = dynamic_cast<ShaderStorageBuffer*>(n) ) {
          ssbo->displayList->callList();
        }else if( ShaderAtomicCounter* sac = dynamic_cast<ShaderAtomicCounter*>(n) ) {
          sac->displayList->callList();
        }
      }
      
    }
    
  }
}
