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
/// \file PythonTypes.h
/// \brief Python C Types that wrap the X3D C++ type classes
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __PYTHONTYPES_H__
#define __PYTHONTYPES_H__

// If the following comment is read through the generated documentation then
// ignore this comment section.
// The following is doxygen comment in order to generate a page explaining the
// connection between python and H3DAPI. The % sign is a doxygen feature and
// can be ignored when reading the documentation directly in this header file.
/// \page PythonSyntax How to use H3DAPI from python.
/// \section TheBasics The Basics
/// The PythonScript node is a binding to the Python scripting language, which
/// can be used to e.g. modify scenes and handle events. In order to use Python
/// a PythonScript node have to be added to the scenegraph. The PythonScript
/// node specifies an url to where the actual Python code is located. So if
/// there is a simle Python file named Hello.py which contains:
/// <pre>print "Hello"</pre>
/// You can get this code to execute by adding a PythonScript node to your
/// scenegraph. This can be done by specifying it directly in an x3d file, e.g.
/// <pre>\<Scene\><br>&nbsp;&nbsp;\<PythonScript url="Hello.py" /\><br>\</Scene\></pre>
/// If you use H3DLoad on the x3d file the Python code should execute and 
/// "Hello" should be printed to the command prompt.
///
/// %H3D API also adds its own types and field types and classes to be used from
/// python. All these are defined in the H3DInterface module. In order to get
/// access to these types and classes you have to import the Python module
/// named H3DInterface. This contains all %H3D specific Python bindings. It can
/// be imported by writing 
/// <pre>from H3DInterface import *</pre>
/// in your Python file.
///
/// \section H3DInterfaceModule Content of H3DInterface module
/// Doxygen documentation for python bindings can be found
/// <a href="../../../H3DAPI/lib/doc/html/index.html">here</a>.


#include <H3DUtil/Exception.h>
#include <H3D/X3DTypes.h>
#include <H3D/X3DFieldConversion.h>
#include <H3D/Node.h>
#include <H3D/H3DApi.h>
#include <H3D/PyTypeWrapper.h>

#ifdef HAVE_PYTHON

#if defined(_MSC_VER)
// undefine _DEBUG since we want to always link to the release version of
// python and pyconfig.h automatically links debug version if _DEBUG is
// defined.
#ifdef _DEBUG
#define _DEBUG_UNDEFED
#undef _DEBUG
#endif
#endif
#if defined(__APPLE__) && defined(__MACH__)
#include <Python/Python.h>
#else
#include <Python.h>
#endif
#if defined(_MSC_VER)
// redefine _DEBUG if it was undefed
#ifdef _DEBUG_UNDEFED
#define _DEBUG
#endif
#endif

namespace H3D {

  class PyNodePtr {
  public:
    PyNodePtr() : refCountNode ( true ) {}

    /// If true, then the PyNode should keep a reference count
    /// for the Node ptr. Usually this is the desired behaviour, except
    /// for a PythonScript node, which would never be deleted if this were
    /// the case.
    inline void setRefCountNode ( bool _refCountNode ) {
      refCountNode= _refCountNode;
    }

  protected:
    bool refCountNode;
    Node *ptr;
  };

  //////////////////////////////////////////////////////
  // Python C declarations for PyNode type
  //////////////////////////////////////////////////////

  extern H3DAPI_API PyTypeObject PyNode_Type;
  /// Returns which is the name the PyNode is to be installed
  /// as in Python.
  inline string PyNode_Name() { return "Node"; }   

  /// Creates a new PyNode object based on the value of v.
  ///
  /// \param v The Node object to create the PyNode object from
  /// \param _refCountNode If true, then the PyNode should keep a reference count
  ///                      for the Node ptr. Usually this is the desired behaviour, except
  ///                      for a PythonScript node, which would never be deleted if this were
  ///                      the case.
  PyObject H3DAPI_API *PyNode_FromNode( Node *v, bool _refCountNode= true );

  /// Returns an Node * representation of the contents of o.
  Node H3DAPI_API *PyNode_AsNode( PyObject *o );

  /// Returns true if its argument is a PyNode.
  inline bool PyNode_Check( PyObject *o) {
    return PyObject_TypeCheck(o,&PyNode_Type ) || o == Py_None;
  }

  /// \class PyNode
  /// \brief Python C Type wrapper around Node*
  struct PyNode : public PyType, PyNodePtr {

    /// Set the value of the encapsulated Node *.
    inline void setNodePtr( Node *_ptr ) {
      if( refCountNode && ptr ) ptr->unref();
      ptr = _ptr;
      if( refCountNode && ptr ) ptr->ref();
    }

    /// Get the value of the encapsulated Node *.
    inline Node *nodePtr() {
      return ptr;
    }
    
    static void installType( PyObject* H3D_module );
    
    /// create() a new instance of PyNode using Python to allocate
    /// the memory and initialise the PyObject headers - to be called
    /// by PythonScript's internals.
    static PyObject* create(); 
    
        /// Python type deallocation
    static void dealloc( PyNode *self );

    /// Python type initialisation: will set the Node value from a
    /// python string argument list
    static int init(PyNode *self, PyObject *args, PyObject *kwds);

    /// Converts a PyNode to a char* string
    static PyObject* repr( PyNode *myself, PyObject *args);

    /// Test if two PyNode are equal or not. They are equal if they
    /// contain the same node pointer.
    static int compare( PyObject *veca, PyObject *vecb );
    
    /// Get the field of a node. arg is the name of the field. 
    static PyObject* getField( PyObject *myself, char* arg );

    /// If myself encapsulates a node that is not a PythonScript
    /// node it works just as getField. If a PythonScript node
    /// it will first check the name against the global attributes
    /// of the script/module in the PythonScript node and return
    /// that if it exists. If it does not it will also work as getField.
    static PyObject* getAttr( PyObject *myself, char* arg );

    /// Get the list of fields available in this Node.
    static PyObject* getFieldList( PyObject *myself );

    /// Get the name of this node.
    static PyObject* getName( PyObject *myself );

    /// Get the type name of this node, i.e. "Group"
    static PyObject* getTypeName( PyObject *myself );

    /// Set the name of this node.
    static PyObject* setName( PyObject *self, PyObject *args );

    /// Get a field from the node database.
    static PyObject* getSingleField( PyObject *self, PyObject *args );

    /// If this Node is a DynamicFieldsObject a new field is added.
    static PyObject* addField( PyObject *self, PyObject *args, 
                               PyObject *field_type, PyObject *access_type );

    /// Create a clone the node
    static PyObject* clone ( PyObject* self, PyObject *args );
  };

  //////////////////////////////////////////////////////
  // Python C declarations for PyVec2f type
  //////////////////////////////////////////////////////

  extern H3DAPI_API PyTypeObject PyVec2f_Type;
  /// Returns which is the name the PyVec2f is to be installed
  /// as in Python.
  inline string PyVec2f_Name() { return "Vec2f"; }   

  /// Creates a new PyVec2f object based on the value of v.
  PyObject *PyVec2f_FromVec2f( const Vec2f &v);

  /// Returns an Vec2f representation of the contents of o.
  Vec2f H3DAPI_API PyVec2f_AsVec2f( PyObject *o );

  /// Returns true if its argument is a PyVec2f.
  inline bool PyVec2f_Check( PyObject *o) {
    return PyObject_TypeCheck(o,&PyVec2f_Type);
  }

  /// \class PyVec2f
  /// \brief Python C Type wrapper around Vec2f
  struct PyVec2f : 
        public PyVecTypeWrapper< Vec2f, 
    &PyVec2f_Type,
    PyVec2f_Name,
    PyVec2f_Check,
    PyVec2f_AsVec2f, 
    PyVec2f_FromVec2f >{
    
    /// Python type initialisation: will set the Vec2f value from a
    /// python string argument list
    static int init(PyVec2f *self, PyObject *args, PyObject *kwds);

    /// Get the length of the PyVec2f. args is ignored.
    static PyObject* length( PyObject *self, PyObject *args );
    
    /// Get the length squared of the PyVec2f. args is ignored.
    static PyObject* lengthSqr( PyObject *self, PyObject *args );

    /// Normalizes the PyVec2f to be of length 1. args is ignored.
    static PyObject* normalize( PyObject *self, PyObject *args );
    
    /// Normalize the vector to be of length 1. If the vector is of 
    /// zero length nothing will be done. args is ignored.
    static PyObject* normalizeSafe( PyObject *self, PyObject *args );

    /// Returns the dot product between two vectors.
    static PyObject* dotProduct( PyObject *self, PyObject *args );

  };
  
  //////////////////////////////////////////////////////
  // Python C declarations for PyVec2d type
  //////////////////////////////////////////////////////

  extern H3DAPI_API PyTypeObject PyVec2d_Type;
  /// Returns which is the name the PyVec2d is to be installed
  /// as in Python.
  inline string PyVec2d_Name() { return "Vec2d"; }   

  /// Creates a new PyVec2d object based on the value of v.
  PyObject *PyVec2d_FromVec2d( const Vec2d &v);

  /// Returns an Vec2d representation of the contents of o.
  Vec2d H3DAPI_API PyVec2d_AsVec2d( PyObject *o );

  /// Returns true if its argument is a PyVec2d.
  inline bool PyVec2d_Check( PyObject *o) {
    return PyObject_TypeCheck(o,&PyVec2d_Type);
  }

  namespace PythonInternals {
    /// Returns true if its argument is a PyVec2d or PyVec2f.
    inline bool PyVec2d2f_Check( PyObject *o) {
      return PyObject_TypeCheck(o,&PyVec2d_Type) ||
        PyObject_TypeCheck( o, &PyVec2f_Type );
    }
    
    /// Returns an Vec2d representation of the contents of o.
    Vec2d PyVec2d2f_AsVec2d( PyObject *o );
  }
  
  /// \class PyVec2d
  /// \brief Python C Type wrapper around Vec2d

  // In the PyVec2d operators a PyVec2f can be used instead of a PyVec2d so we 
  // used the specially defined PyVec2d2f operators for this type.
  struct PyVec2d : 
        public PyVecTypeWrapper< Vec2d, 
    &PyVec2d_Type,
    PyVec2d_Name,
    PythonInternals::PyVec2d2f_Check,
    PythonInternals::PyVec2d2f_AsVec2d, 
    PyVec2d_FromVec2d >{
    
    /// Python type initialisation: will set the Vec2d value from a
    /// python string argument list
    static int init(PyVec2d *self, PyObject *args, PyObject *kwds);

    /// Get the length of the PyVec2d. args is ignored.
    static PyObject* length( PyObject *self, PyObject *args );
    
    /// Get the length squared of the PyVec2d. args is ignored.
    static PyObject* lengthSqr( PyObject *self, PyObject *args );

    /// Normalizes the PyVec2d to be of length 1. args is ignored.
    static PyObject* normalize( PyObject *self, PyObject *args );
    
    /// Normalize the vector to be of length 1. If the vector is of 
    /// zero length nothing will be done. args is ignored.
    static PyObject* normalizeSafe( PyObject *self, PyObject *args );

    /// Returns the dot product between two vectors.
    static PyObject* dotProduct( PyObject *self, PyObject *args );
  };
  
  //////////////////////////////////////////////////////
  // Python C declarations for PyVec3f type
  //////////////////////////////////////////////////////

  extern H3DAPI_API PyTypeObject PyVec3f_Type;

  /// Returns which is the name the PyVec3f is to be installed
  /// as in Python.
  inline string PyVec3f_Name() { return "Vec3f"; }   

  /// Creates a new PyVec3f object based on the value of v.
  PyObject *PyVec3f_FromVec3f( const Vec3f &v);

  /// Returns an Vec3f representation of the contents of o.
  Vec3f H3DAPI_API PyVec3f_AsVec3f( PyObject *o );

  /// Returns true if its argument is a PyVec3f.
  inline bool PyVec3f_Check( PyObject *o) {
    return PyObject_TypeCheck(o,&PyVec3f_Type);
  }

  /// \class PyVec3f
  /// \brief Python C Type wrapper around Vec3f.
  struct PyVec3f : 
    public PyVecTypeWrapper< Vec3f, 
    &PyVec3f_Type,
    PyVec3f_Name,
    PyVec3f_Check,
    PyVec3f_AsVec3f, 
    PyVec3f_FromVec3f > {

    /// Python type initialisation: will set the Vec3f value from a
    /// python string argument list
    static int init(PyVec3f *self, PyObject *args, PyObject *kwds);
    
    /// Performs Vec3f cross product of two PyVec3f instances
    static PyObject* mod( PyObject *veca, PyObject *vecb );
    
    /// Get the length of the PyVec3f. args is ignored.
    static PyObject* length( PyObject *self, PyObject *args );
    
    /// Get the length squared of the PyVec3f. args is ignored.
    static PyObject* lengthSqr( PyObject *self, PyObject *args );

    /// Normalizes the PyVec3f to be of length 1. args is ignored.
    static PyObject* normalize( PyObject *self, PyObject *args );
    
    /// Normalize the vector to be of length 1. If the vector is of 
    /// zero length nothing will be done. args is ignored.
    static PyObject* normalizeSafe( PyObject *self, PyObject *args );

    /// Returns the dot product between two vectors.
    static PyObject* dotProduct( PyObject *self, PyObject *args );

    /// Returns the dot product between two vectors.
    static PyObject* crossProduct( PyObject *self, PyObject *args );

  };
  
  //////////////////////////////////////////////////////
  // Python C declarations for PyVec3d type
  //////////////////////////////////////////////////////

  extern H3DAPI_API PyTypeObject PyVec3d_Type;

  /// Returns which is the name the PyVec3f is to be installed
  /// as in Python.
  inline string PyVec3d_Name() { return "Vec3d"; }   

  /// Creates a new PyVec3d object based on the value of v.
  PyObject *PyVec3d_FromVec3d( const Vec3d &v);

  /// Returns an Vec3d representation of the contents of o.
  Vec3d H3DAPI_API PyVec3d_AsVec3d( PyObject *o );

  /// Returns true if its argument is a PyVec3f.
  inline bool PyVec3d_Check( PyObject *o) {
    return PyObject_TypeCheck(o,&PyVec3d_Type);
  }

  namespace PythonInternals {
    /// Returns true if its argument is a PyVec3d or PyVec3f.
    inline bool PyVec3d3f_Check( PyObject *o) {
      return PyObject_TypeCheck(o,&PyVec3d_Type) ||
        PyObject_TypeCheck( o, &PyVec3f_Type );
    }
    
    /// Returns an Vec3d representation of the contents of o.
    Vec3d PyVec3d3f_AsVec3d( PyObject *o );
  }

  /// \class PyVec3d
  /// \brief Python C Type wrapper around Vec3d. 

  // In the PyVec3d operators a PyVec3f can be used instead of a PyVec3d so we 
  // used the specially defined PyVec3d3f operators for this type.
  struct PyVec3d : public PyVecTypeWrapper< Vec3d, 
                   &PyVec3d_Type,
                   PyVec3d_Name,
                   PythonInternals::PyVec3d3f_Check,
                   PythonInternals::PyVec3d3f_AsVec3d, 
                   PyVec3d_FromVec3d > {
    /// Python type initialisation: will set the Vec3d value from a
    /// python string argument list
    static int init(PyVec3d *self, PyObject *args, PyObject *kwds);
    static PyObject* mod( PyObject *veca, PyObject *vecb );
    /// Get the length of the PyVec3d.
    static PyObject* length( PyObject *self, PyObject *args );
    /// Get the length squared of the PyVec3d.
    static PyObject* lengthSqr( PyObject *self, PyObject *args );
    /// Normalizes the PyVec3d to be of length 1.
    static PyObject* normalize( PyObject *self, PyObject *args );
    /// Normalize the vector to be of length 1. If the vector is of 
    /// zero length nothing will be done.
    static PyObject* normalizeSafe( PyObject *self, PyObject *args );
    /// Returns the dot product between two vectors.
    static PyObject* dotProduct( PyObject *self, PyObject *args );

    /// Returns the dot product between two vectors.
    static PyObject* crossProduct( PyObject *self, PyObject *args );
  };
  
  //////////////////////////////////////////////////////
  // Python C declarations for PyVec4f type
  //////////////////////////////////////////////////////

  extern H3DAPI_API PyTypeObject PyVec4f_Type;

  /// Returns which is the name the PyVec3f is to be installed
  /// as in Python.
  inline string PyVec4f_Name() { return "Vec4f"; }   

  /// Creates a new PyVec4f object based on the value of v.
  PyObject *PyVec4f_FromVec4f( const Vec4f &v);

  /// Returns an Vec4f representation of the contents of o.
  Vec4f H3DAPI_API PyVec4f_AsVec4f( PyObject *o );

  /// Returns true if its argument is a PyVec3f.
  inline bool PyVec4f_Check( PyObject *o) {
    return PyObject_TypeCheck(o,&PyVec4f_Type);
  }

  /// \class PyVec4f
  /// \brief Python C Type wrapper around Vec4f
  struct PyVec4f : public PyVecTypeWrapper< Vec4f, 
                   &PyVec4f_Type,
                   PyVec4f_Name,
                   PyVec4f_Check,
                   PyVec4f_AsVec4f, 
                   PyVec4f_FromVec4f > {
    
    /// Python type initialisation: will set the Vec4f value from a
    /// python string argument list
    static int init(PyVec4f *self, PyObject *args, PyObject *kwds);
  };
  
  
  //////////////////////////////////////////////////////
  // Python C declarations for PyVec4d type
  //////////////////////////////////////////////////////

  extern H3DAPI_API PyTypeObject PyVec4d_Type;

  /// Returns which is the name the PyVec3f is to be installed
  /// as in Python.
  inline string PyVec4d_Name() { return "Vec4d"; }   

  /// Creates a new PyVec4d object based on the value of v.
  PyObject *PyVec4d_FromVec4d( const Vec4d &v);

  /// Returns an Vec4d representation of the contents of o.
  Vec4d H3DAPI_API PyVec4d_AsVec4d( PyObject *o );

  /// Returns true if its argument is a PyVec3f.
  inline bool PyVec4d_Check( PyObject *o) {
    return PyObject_TypeCheck(o,&PyVec4d_Type);
  }

  namespace PythonInternals {
    /// Returns true if its argument is a PyVec3d or PyVec3f.
    inline bool PyVec4d4f_Check( PyObject *o) {
      return PyObject_TypeCheck(o,&PyVec4d_Type) ||
        PyObject_TypeCheck( o, &PyVec4f_Type );
    }
    
    /// Returns an Vec4d representation of the contents of o.
    Vec4d PyVec4d4f_AsVec4d( PyObject *o );
  }

  /// \class PyVec4d
  /// \brief Python C Type wrapper around Vec4d
  struct PyVec4d : public PyVecTypeWrapper< Vec4d, 
                   &PyVec4d_Type,
                   PyVec4d_Name,
                   PythonInternals::PyVec4d4f_Check,
                   PythonInternals::PyVec4d4f_AsVec4d, 
                   PyVec4d_FromVec4d > {
    
    /// Python type initialisation: will set the Vec4d value from a
    /// python string argument list
    static int init(PyVec4d *self, PyObject *args, PyObject *kwds);
  };

  //////////////////////////////////////////////////////
  // Python C declarations for PyRotation type
  //////////////////////////////////////////////////////

  extern H3DAPI_API PyTypeObject PyRotation_Type;

  /// Returns which is the name the PyVec3f is to be installed
  /// as in Python.
  inline string PyRotation_Name() { return "Rotation"; }   

  /// Creates a new PyRotation object based on the value of v.
  PyObject *PyRotation_FromRotation( const Rotation &v);

  /// Returns an Rotation representation of the contents of o.
  Rotation H3DAPI_API PyRotation_AsRotation( PyObject *o );

  /// Returns true if its argument is a PyRotation.
  inline bool PyRotation_Check( PyObject *o) {
    return PyObject_TypeCheck(o,&PyRotation_Type);
  }

  /// \class PyRotation
  /// \brief Python C Type wrapper around Rotation
  struct PyRotation : public PyNumberTypeWrapper< Rotation, 
                   &PyRotation_Type,
                   PyRotation_Name,
                   PyRotation_Check,
                   PyRotation_AsRotation, 
                   PyRotation_FromRotation > {
  
    /// Python type initialisation: will set the Rotation value from a
    /// python string argument list
    static int init(PyRotation *self, PyObject *args, PyObject *kwds);

    /// Get the euler angles( yaw, pitch, roll ) representation of 
    /// the Rotation. 
    static PyObject *toEulerAngles( PyObject *self, PyObject *args );

    /// Spherical linear interpolation between two Rotations.
    static PyObject *slerp( PyObject *self, PyObject *args );

    /// Multiplies two Matrix3f objects
    static PyObject* mul( PyObject *rota, PyObject *rotb );
  };
  
  //////////////////////////////////////////////////////
  // Python C declarations for PyQuaternion type
  //////////////////////////////////////////////////////
  
  extern H3DAPI_API PyTypeObject PyQuaternion_Type;

  /// Returns which is the name the PyQuaternion is to be installed
  /// as in Python.
  inline string PyQuaternion_Name() { return "Quaternion"; }   

  /// Creates a new PyQuaternion object based on the value of v.
  PyObject *PyQuaternion_FromQuaternion( const Quaternion &v);

  /// Returns an Quaternion representation of the contents of o.
  Quaternion H3DAPI_API PyQuaternion_AsQuaternion( PyObject *o );

  /// Returns true if its argument is a PyVec3f.
  inline bool PyQuaternion_Check( PyObject *o) {
    return PyObject_TypeCheck(o,&PyQuaternion_Type);
  }

  /// \class PyQuaternion
  /// \brief Python C Type wrapper around Quaternion
  struct PyQuaternion : public PyNumberTypeWrapper< Quaternion, 
                   &PyQuaternion_Type,
                   PyQuaternion_Name,
                   PyQuaternion_Check,
                   PyQuaternion_AsQuaternion, 
                   PyQuaternion_FromQuaternion > {

    /// Python type initialisation: will set the Quaternion value from
    /// a python string argument list
    static int init(PyQuaternion *self, PyObject *args, PyObject *kwds);
    
    /// Multiplies two PyQuaternion objects
    static PyObject* mul( PyObject *rota, PyObject *rotb );

    /// Get the euler angles( yaw, pitch, roll ) representation of 
    /// the Rotation. 
    static PyObject *toEulerAngles( PyObject *self, PyObject *args );

    /// Returns the Quaternion norm.
    static PyObject *norm( PyObject *self, PyObject *args );

    /// Normalize the Quaternion, i.e. scale it so that the magnitude
    /// is 1.
    static PyObject* normalize( PyObject *self, PyObject *args );

    /// Returns the conjugate of the Quaternion.
    static PyObject* conjugate( PyObject *self, PyObject *args );

    /// Returns the inverse of the Quaternion.
    static PyObject* inverse( PyObject *self, PyObject *args ); 

    /// Spherical linear interpolation between two Quaternions.
    static PyObject* slerp( PyObject *self, PyObject *args ); 

    /// dotProduct() returns the cos(angle) between two quaternions
    static PyObject* dotProduct( PyObject *self, PyObject *args ); 
  };

  //////////////////////////////////////////////////////
  // Python C declarations for PyMatrix3f type
  //////////////////////////////////////////////////////

  extern H3DAPI_API PyTypeObject PyMatrix3f_Type;

  /// Returns which is the name the PyMatrix3f is to be installed
  /// as in Python.
  inline string PyMatrix3f_Name() { return "Matrix3f"; }   

  /// Creates a new PyMatrix3f object based on the value of v.
  PyObject *PyMatrix3f_FromMatrix3f( const Matrix3f &v);

  /// Returns an Matrix3f representation of the contents of o.
  Matrix3f H3DAPI_API PyMatrix3f_AsMatrix3f( PyObject *o );

  /// Returns true if its argument is a PyMatrix3f.
  inline bool PyMatrix3f_Check( PyObject *o) {
    return PyObject_TypeCheck(o,&PyMatrix3f_Type);
  }
  /// \class PyMatrix3f
  /// \brief Python C Type wrapper around Matrix3f
  struct PyMatrix3f : public PyNumberTypeWrapper< Matrix3f, 
                      &PyMatrix3f_Type,
                      PyMatrix3f_Name,
                      PyMatrix3f_Check,
                      PyMatrix3f_AsMatrix3f, 
                      PyMatrix3f_FromMatrix3f > {
    
    /// Python type initialisation: will set the Matrix3f value from a
    /// python string argument list
    static int init(PyMatrix3f *self, PyObject *args, PyObject *kwds);
    
    /// Multiplies two Matrix3f objects
    static PyObject* mul( PyObject *rota, PyObject *rotb );
    
    /// Set to the identity matrix.
    static PyObject* setToIdentity( PyObject *self, PyObject *args );

    /// Returns the inverse of the matrix.
    static PyObject* inverse( PyObject *self, PyObject *args );

    /// Returns the transpose of the matrix.
    static PyObject* transpose( PyObject *self, PyObject *args );

    /// Get the euler angles( yaw, pitch, roll ) representation of 
    /// the rotation matrix. The Matrix3f must be a rotation matrix.
    static PyObject* toEulerAngles( PyObject *self, PyObject *args );

      /// Get the scaling part of the matrix for each axis.
    static PyObject* getScalePart( PyObject *self, PyObject *args );

    /// Get a row of the matrix.
    static PyObject* getRow( PyObject *self, PyObject *args );

    /// Get a column of the matrix.
    static PyObject* getColumn( PyObject *self, PyObject *args );

    /// Get an element in the matrix.
    static PyObject* getElement( PyObject *self, PyObject *args );

    /// Set an element in the matrix.
    static PyObject* setElement( PyObject *self, PyObject *args );
  };
    
  //////////////////////////////////////////////////////
  // Python C declarations for PyMatrix4f type
  //////////////////////////////////////////////////////

  extern H3DAPI_API PyTypeObject PyMatrix4f_Type;

  /// Returns which is the name the PyMatrix4f is to be installed
  /// as in Python.
  inline string PyMatrix4f_Name() { return "Matrix4f"; }   

  /// Creates a new PyMatrix4f object based on the value of v.
  PyObject *PyMatrix4f_FromMatrix4f( const Matrix4f &v);

  /// Returns an Matrix4f representation of the contents of o.
  Matrix4f H3DAPI_API PyMatrix4f_AsMatrix4f( PyObject *o );

  /// Returns true if its argument is a PyMatrix4f.
  inline bool PyMatrix4f_Check( PyObject *o) {
    return PyObject_TypeCheck(o,&PyMatrix4f_Type);
  }
  /// \class PyMatrix4f
  /// \brief Python C Type wrapper around Matrix4f
  struct PyMatrix4f : public PyNumberTypeWrapper< Matrix4f, 
                      &PyMatrix4f_Type,
                      PyMatrix4f_Name,
                      PyMatrix4f_Check,
                      PyMatrix4f_AsMatrix4f, 
                      PyMatrix4f_FromMatrix4f > {
    
    
    /// Python type initialisation: will set the Matrix4f value from a
    /// python string argument list
    static int init(PyMatrix4f *self, PyObject *args, PyObject *kwds);
    
    /// Multiplies two Matrix4f objects
    static PyObject* mul( PyObject *rota, PyObject *rotb );
    
    /// Set to the identity matrix.
    static PyObject* setToIdentity( PyObject *self, PyObject *args );

    /// Returns the inverse of the matrix assuming that it is on 
    /// the form
    ///
    ///  [ m00 m01 m02 m03
    ///    m10 m11 m12 m13
    ///    m20 m21 m22 m23
    ///    0   0   0   1   ]
    ///
    static PyObject* transformInverse( PyObject *self, PyObject *args );

    /// Returns the inverse of the matrix.
    static PyObject* inverse( PyObject *self, PyObject *args );

   /// Returns the inverse of the matrix.
    static PyObject* transpose( PyObject *self, PyObject *args );

    /// Get a row of the matrix.
    static PyObject* getRow( PyObject *self, PyObject *args );

    /// Get a column of the matrix.
    static PyObject* getColumn( PyObject *self, PyObject *args );

    /// Returns the scale and rotation part of the Matrix4f.
    static PyObject* getScaleRotationPart( PyObject *self, PyObject *args );

    /// Returns the rotation part of the Matrix4f.
    static PyObject* getRotationPart( PyObject *self, PyObject *args );

    /// Returns the scaling part of the Matrix4f.
    static PyObject* getScalePart( PyObject *self, PyObject *args );
    
    /// Returns the translation part of the Matrix4f.
    static PyObject* getTranslationPart( PyObject *self, PyObject *args );

    /// Get an element in the matrix.
    static PyObject* getElement( PyObject *self, PyObject *args );

    /// Set an element in the matrix.
    static PyObject* setElement( PyObject *self, PyObject *args );
  };


  //////////////////////////////////////////////////////
  // Python C declarations for PyMatrix3d type
  //////////////////////////////////////////////////////

  extern H3DAPI_API PyTypeObject PyMatrix3d_Type;

  /// Returns which is the name the PyMatrix3d is to be installed
  /// as in Python.
  inline string PyMatrix3d_Name() { return "Matrix3d"; }   

  /// Creates a new PyMatrix3d object based on the value of v.
  PyObject *PyMatrix3d_FromMatrix3d( const Matrix3d &v);

  /// Returns an Matrix3d representation of the contents of o.
  Matrix3d H3DAPI_API PyMatrix3d_AsMatrix3d( PyObject *o );

  /// Returns true if its argument is a PyMatrix3d.
  inline bool PyMatrix3d_Check( PyObject *o) {
    return PyObject_TypeCheck(o,&PyMatrix3d_Type);
  }

  namespace PythonInternals {
    /// Returns true if its argument is a PyVec3d or PyVec3f.
    inline bool PyMatrix3d3f_Check( PyObject *o) {
      return PyObject_TypeCheck(o,&PyMatrix3d_Type) ||
        PyObject_TypeCheck( o, &PyMatrix3f_Type );
    }
    
    /// Returns an Vec3d representation of the contents of o.
    Matrix3d PyMatrix3d3f_AsMatrix3d( PyObject *o );
  }

  /// \class PyMatrix3d
  /// \brief Python C Type wrapper around Matrix3d
  struct PyMatrix3d : public PyNumberTypeWrapper< Matrix3d, 
                      &PyMatrix3d_Type,
                      PyMatrix3d_Name,
                      PythonInternals::PyMatrix3d3f_Check,
                      PythonInternals::PyMatrix3d3f_AsMatrix3d, 
                      PyMatrix3d_FromMatrix3d > {
    
    /// Python type initialisation: will set the Matrix3d value from a
    /// python string argument list
    static int init(PyMatrix3d *self, PyObject *args, PyObject *kwds);
    
    /// Multiplies two Matrix3d objects
    static PyObject* mul( PyObject *rota, PyObject *rotb );
    
    /// Set to the identity matrix.
    static PyObject* setToIdentity( PyObject *self, PyObject *args );

    /// Returns the inverse of the matrix.
    static PyObject* inverse( PyObject *self, PyObject *args );

    /// Returns the transpose of the matrix.
    static PyObject* transpose( PyObject *self, PyObject *args );

    /// Get the euler angles( yaw, pitch, roll ) representation of 
    /// the rotation matrix. The Matrix3f must be a rotation matrix.
    static PyObject* toEulerAngles( PyObject *self, PyObject *args );

      /// Get the scaling part of the matrix for each axis.
    static PyObject* getScalePart( PyObject *self, PyObject *args );

    /// Get a row of the matrix.
    static PyObject* getRow( PyObject *self, PyObject *args );

    /// Get a column of the matrix.
    static PyObject* getColumn( PyObject *self, PyObject *args );

    /// Get an element in the matrix.
    static PyObject* getElement( PyObject *self, PyObject *args );

    /// Set an element in the matrix.
    static PyObject* setElement( PyObject *self, PyObject *args );
  };
    
  //////////////////////////////////////////////////////
  // Python C declarations for PyMatrix4d type
  //////////////////////////////////////////////////////

  extern H3DAPI_API PyTypeObject PyMatrix4d_Type;

  /// Returns which is the name the PyMatrix4d is to be installed
  /// as in Python.
  inline string PyMatrix4d_Name() { return "Matrix4d"; }   

  /// Creates a new PyMatrix4d object based on the value of v.
  PyObject *PyMatrix4d_FromMatrix4d( const Matrix4d &v);

  /// Returns an Matrix4d representation of the contents of o.
  Matrix4d H3DAPI_API PyMatrix4d_AsMatrix4d( PyObject *o );

  /// Returns true if its argument is a PyMatrix4d.
  inline bool PyMatrix4d_Check( PyObject *o) {
    return PyObject_TypeCheck(o,&PyMatrix4d_Type);
  }

  namespace PythonInternals {
    /// Returns true if its argument is a PyVec3d or PyVec3f.
    inline bool PyMatrix4d4f_Check( PyObject *o) {
      return PyObject_TypeCheck(o,&PyMatrix4d_Type) ||
        PyObject_TypeCheck( o, &PyMatrix4f_Type );
    }
    
    /// Returns an Vec4d representation of the contents of o.
    Matrix4d PyMatrix4d4f_AsMatrix4d( PyObject *o );
  }

  /// \class PyMatrix4d
  /// \brief Python C Type wrapper around Matrix4d
  struct PyMatrix4d : public PyNumberTypeWrapper< Matrix4d, 
                      &PyMatrix4d_Type,
                      PyMatrix4d_Name,
                      PythonInternals::PyMatrix4d4f_Check,
                      PythonInternals::PyMatrix4d4f_AsMatrix4d, 
                      PyMatrix4d_FromMatrix4d > {
    
    
    /// Python type initialisation: will set the Matrix4d value from a
    /// python string argument list
    static int init(PyMatrix4d *self, PyObject *args, PyObject *kwds);
    
    /// Multiplies two Matrix4d objects
    static PyObject* mul( PyObject *rota, PyObject *rotb );
    
    /// Set to the identity matrix.
    static PyObject* setToIdentity( PyObject *self, PyObject *args );

    /// Returns the inverse of the matrix assuming that it is on 
    /// the form
    ///
    ///  [ m00 m01 m02 m03
    ///    m10 m11 m12 m13
    ///    m20 m21 m22 m23
    ///    0   0   0   1   ]
    ///
    static PyObject* transformInverse( PyObject *self, PyObject *args );

    /// Returns the inverse of the matrix.
    static PyObject* inverse( PyObject *self, PyObject *args );

    /// Returns the inverse of the matrix.
    static PyObject* transpose( PyObject *self, PyObject *args );

    /// Get a row of the matrix.
    static PyObject* getRow( PyObject *self, PyObject *args );

    /// Get a column of the matrix.
    static PyObject* getColumn( PyObject *self, PyObject *args );

    /// Returns the scale and rotation part of the Matrix4d.
    static PyObject* getScaleRotationPart( PyObject *self, PyObject *args );

    /// Returns the rotation part of the Matrix4d.
    static PyObject* getRotationPart( PyObject *self, PyObject *args );

    /// Returns the scaling part of the Matrix4d.
    static PyObject* getScalePart( PyObject *self, PyObject *args );

    /// Returns the translation part of the Matrix4d.
    static PyObject* getTranslationPart( PyObject *self, PyObject *args );

    /// Get an element in the matrix.
    static PyObject* getElement( PyObject *self, PyObject *args );

    /// Set an element in the matrix.
    static PyObject* setElement( PyObject *self, PyObject *args );
  };
    
  //////////////////////////////////////////////////////
  // Python C declarations for PyRGB type
  //////////////////////////////////////////////////////
  
  extern H3DAPI_API PyTypeObject PyRGB_Type;

  /// Returns which is the name the PyVec3f is to be installed
  /// as in Python.
  inline string PyRGB_Name() { return "RGB"; }   

  /// Creates a new PyRGB object based on the value of v.
  PyObject *PyRGB_FromRGB( const RGB &v);

  /// Returns an RGB representation of the contents of o.
  RGB H3DAPI_API PyRGB_AsRGB( PyObject *o );

  /// Returns true if its argument is a PyVec3f.
  inline bool PyRGB_Check( PyObject *o) {
    return PyObject_TypeCheck(o,&PyRGB_Type);
  }

  /// \class PyRGB
  /// \brief Python C Type wrapper around RGB
  struct PyRGB : public PyTypeWrapper< RGB, 
                 &PyRGB_Type,
                 PyRGB_Name,
                 PyRGB_Check,
                 PyRGB_AsRGB, 
                 PyRGB_FromRGB > {
    
    /// Python type initialisation: will set the RGB value from a
    /// python string argument list
    static int init(PyRGB *self, PyObject *args, PyObject *kwds);
  };

  //////////////////////////////////////////////////////
  // Python C declarations for PyRGBA type
  //////////////////////////////////////////////////////
  
  extern H3DAPI_API PyTypeObject PyRGBA_Type;

  /// Returns which is the name the PyRGBA is to be installed
  /// as in Python.
  inline string PyRGBA_Name() { return "RGBA"; }   

  /// Creates a new PyRGBA object based on the value of v.
  PyObject *PyRGBA_FromRGBA( const RGBA &v);

  /// Returns an RGBA representation of the contents of o.
  RGBA H3DAPI_API PyRGBA_AsRGBA( PyObject *o );

  /// Returns true if its argument is a PyRGBA.
  inline bool PyRGBA_Check( PyObject *o) {
    return PyObject_TypeCheck(o,&PyRGBA_Type);
  }

  /// \class PyRGBA
  /// \brief Python C Type wrapper around RGBA
  struct PyRGBA : public PyTypeWrapper< RGBA, 
                 &PyRGBA_Type,
                 PyRGBA_Name,
                 PyRGBA_Check,
                 PyRGBA_AsRGBA, 
                 PyRGBA_FromRGBA > {
    
    /// Python type initialisation: will set the RGBA value from a
    /// python string argument list
    static int init(PyRGBA *self, PyObject *args, PyObject *kwds);
  };

  //////////////////////////////////////////////////////
  // Python C declarations for PyConsole type(interface to Console)
  //////////////////////////////////////////////////////
  extern H3DAPI_API PyTypeObject PyConsole_Type;

  struct PyConsole: public PyType {
    /// Install type in the given python module. Must be called if the
    /// type is to be used in a module.
    static void installType( PyObject* H3D_module ) {
      if (PyType_Ready( &PyConsole_Type ) < 0 )
        return; // THROW ERROR!?
      
      Py_INCREF( &PyConsole_Type );
      PyModule_AddObject( H3D_module, 
                          "H3DConsole", 
                          (PyObject *)&PyConsole_Type );
    }
  
    /// create() a new instance of Type using Python to allocate
    /// the memory and initialise the PyObject headers - to be called
    /// by PythonScript's internals.
    static PyObject* create() {
      return PyType_GenericAlloc( &PyConsole_Type, 1 ); 
    } 
    
    /// Python type deallocation
    static void dealloc( PyObject *self ) {
      self->ob_type->tp_free( self );
    }

    static PyObject* write( PyObject *self, PyObject *to_write );
  };

  /// Returns which is the name the PyVec3f is to be installed
  /// as in Python.
  inline string PyConsole_Name() { return "Console"; }   

  /// Returns true if its argument is a PyVec3f.
  inline bool PyConsole_Check( PyObject *o) {
    return PyObject_TypeCheck(o,&PyConsole_Type);
  }
  
    
}
  
#endif
#endif
