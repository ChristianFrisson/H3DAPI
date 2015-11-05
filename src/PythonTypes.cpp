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
/// \file PythonScript.cpp
/// \brief cpp file for PythonScript
///
//
//////////////////////////////////////////////////////////////////////////////
#include <H3D/PythonTypes.h>
#include <H3D/PythonMethods.h>
#include <H3D/PythonScript.h>
#include <H3D/X3DTypes.h>
#include <H3D/X3DFieldConversion.h>
#include <H3D/H3DDynamicFieldsObject.h>

#ifdef HAVE_PYTHON
// DEV WARNING, never use PyBool_Check without also using PyInt_Check, the reason
// is that pythonfieldGetValue returns an integer for the SFBool type. Until that is
// changed this PyInt_Check also have to be used.

#if defined(_MSC_VER)
// undefine _DEBUG since we want to always link to the release version of
// python and pyconfig.h automatically links debug version if _DEBUG is
// defined.
#ifdef _DEBUG
#define _DEBUG_UNDEFED
#undef _DEBUG
#endif
#endif
#define DONT_HAVE_SYS_STAT_H
#undef HAVE_STAT_H
#if defined(__APPLE__) && defined(__MACH__)
#include <Python/Python.h>
#include <Python/pythonrun.h>
#include <Python/ceval.h>
#include <Python/moduleobject.h>
#include <Python/structmember.h>
#else
#include <Python.h>
#include <pythonrun.h>
#include <ceval.h>
#include <moduleobject.h>
#include <structmember.h>
#endif

#if defined(_MSC_VER)
// redefine _DEBUG if it was undefed
#ifdef _DEBUG_UNDEFED
#define _DEBUG
#endif
#endif


using namespace H3D;
using namespace X3D;


namespace H3D {
  namespace PythonInternals {
    // Returns an Vec2d representation of the contents of o.
    Vec2d PyVec2d2f_AsVec2d( PyObject *o ) {
      if( PyVec2d_Check( o ) ) {
        return PyVec2d_AsVec2d( o );
      } else if( PyVec2f_Check( o ) ) {
        return PyVec2f_AsVec2f( o );
      } else {
        throw Exception::H3DAPIException( "PyObject * is not a PyVec2d *", 
                                          H3D_FULL_LOCATION );
      }
    }  

    // Returns an Vec3d representation of the contents of o.
    Vec3d PyVec3d3f_AsVec3d( PyObject *o ) {
      if( PyVec3d_Check( o ) ) {
        return PyVec3d_AsVec3d( o );
      } else if( PyVec3f_Check( o ) ) {
        return PyVec3f_AsVec3f( o );
      } else {
        throw Exception::H3DAPIException( "PyObject * is not a PyVec3d *", 
                                          H3D_FULL_LOCATION );
      }
    }  

    // Returns an Vec4d representation of the contents of o.
    Vec4d PyVec4d4f_AsVec4d( PyObject *o ) {
      if( PyVec4d_Check( o ) ) {
        return PyVec4d_AsVec4d( o );
      } else if( PyVec4f_Check( o ) ) {
        return PyVec4f_AsVec4f( o );
      } else {
        throw Exception::H3DAPIException( "PyObject * is not a PyVec4d *", 
                                          H3D_FULL_LOCATION );
      }
    }  

    // Returns an Matrix3d representation of the contents of o.
    Matrix3d PyMatrix3d3f_AsMatrix3d( PyObject *o ) {
      if( PyMatrix3d_Check( o ) ) {
        return PyMatrix3d_AsMatrix3d( o );
      } else if( PyMatrix3f_Check( o ) ) {
        return Matrix3d( PyMatrix3f_AsMatrix3f( o ) );
      } else {
        throw Exception::H3DAPIException( "PyObject * is not a PyMatrix3d *", 
                                          H3D_FULL_LOCATION );
      }
    }

    // Returns an Matrix4d representation of the contents of o.
    Matrix4d PyMatrix4d4f_AsMatrix4d( PyObject *o ) {
      if( PyMatrix4d_Check( o ) ) {
        return PyMatrix4d_AsMatrix4d( o );
      } else if( PyMatrix4f_Check( o ) ) {
        return Matrix4d( PyMatrix4f_AsMatrix4f( o ) );
      } else {
        throw Exception::H3DAPIException( "PyObject * is not a PyMatrix4d *", 
                                          H3D_FULL_LOCATION );
      }
    }
    
  }

  // New datatypes to reflect base H3D API data types:

  ///////////////////////////////////////////////////////////////////
  /// NODE
  /// 
  PyMethodDef PyNode_methods[] = {
    { "__repr__", (PyCFunction) PyNode::repr, 0 },
    { "__str__", (PyCFunction) PyNode::repr, 0 },
    { "getFieldList", (PyCFunction) PyNode::getFieldList, 0 },
    { "addField", (PyCFunction) PyNode::addField, 0 },
    { "getField", (PyCFunction) PyNode::getSingleField, 0 },
    { "getName", (PyCFunction) PyNode::getName, 0 },
    { "setName", (PyCFunction) PyNode::setName, 0 },
    { "getTypeName", (PyCFunction) PyNode::getTypeName, 0 },
    { "clone", (PyCFunction) PyNode::clone, 0 },
    {NULL, NULL}
  };
  
  PyMemberDef PyNode_members[] = {
  //  {"ptr", T_FLOAT, offsetof(PyNode, ptr), 0,"Node*"},
    {NULL}  /* Sentinel */
  };
  

  PyTypeObject PyNode_Type = {
    PyObject_HEAD_INIT(NULL)
    0,
    "H3D.Node",
    sizeof(PyNode),
    0,                         /*tp_itemsize*/
    (destructor)PyNode::dealloc, /*tp_dealloc*/
    0,                         /*tp_print*/
    (getattrfunc)PyNode::getAttr,                         /*tp_getattr*/
    0,                         /*tp_setattr*/
    (cmpfunc)PyNode::compare,  /*tp_compare*/
    (reprfunc) PyNode::repr,                         /*tp_repr*/
    0,                         /*tp_as_number*/
    0,                         /*tp_as_sequence*/
    0,                         /*tp_as_mapping*/
    0,                         /*tp_hash */
    0,                         /*tp_call*/
    0,                         /*tp_str*/
    0,                         /*tp_getattro*/
    0,                         /*tp_setattro*/
    0,                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
    "Node* Object",            /* tp_doc */
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    PyNode_methods,           /* tp_methods */
    PyNode_members,           /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)PyNode::init,   /* tp_init */
    PyType_GenericAlloc,       /* tp_alloc */
    (newfunc) PyType_GenericAlloc,            /* tp_new */
  };
  
  // Returns an Node * representation of the contents of o.
  Node * PyNode_AsNode( PyObject *o ) {
    if( o == Py_None ) {
      return NULL;
    } else if( PyNode_Check( o ) ) {
      PyNode *p = (PyNode *)(o);
      return p->nodePtr();
    } else {
      throw Exception::H3DAPIException( "PyObject * is not a PyNode *", 
                                        H3D_FULL_LOCATION );
    }
  }  

  // Creates a new PyNode object based on the value of v.
  PyObject *PyNode_FromNode( Node *n, bool _refCountNode ) {
    if( n ) {
      PyObject *o = PyType_GenericAlloc( &PyNode_Type, 1 );
      PyNode *p = (PyNode *)( o );
      // Should the PyNode hold a reference count for the Node?
      p->setRefCountNode ( _refCountNode );
      p->setNodePtr( n );
      return o;
    } else {
      Py_INCREF( Py_None );
      return Py_None;
    }
  }

  PyObject* PyNode::addField( PyObject *self, 
                              PyObject *args, 
                              PyObject *ffield_type, 
                              PyObject *fpy_access_type ) {
    Node *n = PyNode_AsNode( self );
    
    if( !PyTuple_Check( args ) || PyTuple_Size( args ) != 3 ) {
      PyErr_SetString( PyExc_ValueError, 
                       "Invalid argument(s) to function PyNode.addField( \
self, name, field_type, access_type )" );
      return NULL;
    } 
    
    PyObject *name = PyTuple_GetItem( args, 0 );
    PyObject *field_type = PyTuple_GetItem( args, 1 );
    PyObject *py_access_type = PyTuple_GetItem( args, 2 );

    if(! PyString_Check( name ) || ! PyInt_Check( field_type ) || 
       ! PyString_Check( py_access_type ) ) {
      PyErr_SetString( PyExc_ValueError, 
                       "Invalid argument(s) to function PyNode.addField( \
self, name, field_type, access_type )" );
      return NULL;
    }

    H3DDynamicFieldsObject *dfo =
      dynamic_cast< H3DDynamicFieldsObject * >( n );
    if( !dfo ) {
      PyErr_SetString( PyExc_ValueError, 
        "Trying to add field to Node that is not a H3DDynamicFieldsObject." );
      return NULL;
    }
    Field *f = X3DTypes::newFieldInstance(
      (X3DTypes::X3DType)PyInt_AsLong( field_type ) );

    string access_type_string = PyString_AsString( py_access_type );
    Field::AccessType access_type;

    if( access_type_string == "initializeOnly" ) 
      access_type= Field::INITIALIZE_ONLY;
    else if( access_type_string == "outputOnly" )
      access_type = Field::OUTPUT_ONLY;
    else if( access_type_string == "inputOnly" )
      access_type = Field::INPUT_ONLY;
    else if( access_type_string == "inputOutput" )
      access_type = Field::INPUT_OUTPUT;
    else {
       PyErr_SetString( PyExc_ValueError, 
         "Invalid access type" );
       return NULL;
    }
    
    dfo->addField( PyString_AsString( name ),
                   access_type,
                   f );
    Py_INCREF( Py_None );
    return Py_None;
  }
  
  PyObject* PyNode::clone ( PyObject *self, PyObject *arg ) {
    Node *n = PyNode_AsNode( self );

    bool deepCopy= true;
    if( arg && !( PyBool_Check( arg ) || PyInt_Check( arg ) ) ) {
      PyErr_SetString( PyExc_ValueError, 
                       "Invalid argument(s) to function PyNode.clone( \
self, deepCopy )" );
      return NULL;
    }
    if ( arg ) {
      deepCopy= PyObject_IsTrue ( arg ) == 1;
    }

    return PyNode_FromNode ( n->clone ( deepCopy ) );
  }

  void PyNode::installType( PyObject* H3D_module ) {
    // Install Node
    if (PyType_Ready(&PyNode_Type) < 0 )
      return; // THROW ERROR!?
  
    Py_INCREF( &PyNode_Type );
    PyModule_AddObject( H3D_module, "Node", (PyObject *)&PyNode_Type );
  }
  
  PyObject* PyNode::create() {
    return PyType_GenericAlloc( &PyNode_Type, 1 ); 
  }
  
  int PyNode::compare( PyObject *node_a, PyObject *node_b ) {
    if( PyNode_Check( node_a ) && PyNode_Check( node_b ) ){
      Node * a = PyNode_AsNode( node_a );
      Node * b = PyNode_AsNode( node_b );
      return a != b;
    } else {
      ostringstream s;
      s << "PyObject * is not a PyNode *"; 
      throw Exception::H3DAPIException( s.str(),
                                        H3D_FULL_LOCATION );
    }
  }

  void PyNode::dealloc( PyNode *self ) {
    if( self->refCountNode && self->ptr ) self->ptr->unref();
    self->ob_type->tp_free( (PyObject*)self );
  };
  
  int PyNode::init(PyNode *self, PyObject *args, PyObject *kwds)  {
    static char *kwlist[] = {"ptr", NULL};
    if (! PyArg_ParseTupleAndKeywords(args, kwds, "|i", kwlist, 
                                      &(self->ptr) ))
      return -1; 
    return 0;
  }
  
  PyObject* PyNode::repr( PyNode *myself, PyObject *args) {
    ostringstream s;
    if ( myself->ptr )
      s << myself->ptr->getName() << "( " << myself->ptr << ") ";
    else
      s << "<invalid Node>";
    return PyString_FromString( s.str().c_str() );
  }

  PyObject* PyNode::getName( PyObject *myself ) {
    PyNode *n = (PyNode*)myself;
    if( n->ptr)
      return PyString_FromString( n->ptr->getName().c_str() );
    else {
      Py_INCREF( Py_None );
      return Py_None;
    }
  }

  PyObject* PyNode::getTypeName( PyObject *myself ) {
    PyNode *n = (PyNode*)myself;
    if( n->ptr)
      return PyString_FromString( n->ptr->getTypeName().c_str() );
    else {
      Py_INCREF( Py_None );
      return Py_None;
    }
  }

  PyObject* PyNode::setName( PyObject *self, PyObject *args ) {
    if(!args || ! PyString_Check( args ) ) {
      PyErr_SetString( PyExc_ValueError, 
           "Invalid argument(s) to function H3D.Node.setName( self, name )" );
      return 0;
    }
    PyNode *n = (PyNode*)self;
    if (n->ptr) {
      n->ptr->setName( PyString_AsString( args ) );
    }
    Py_INCREF( Py_None );
    return Py_None;
  }

  PyObject* PyNode::getAttr( PyObject *myself, char* arg ) {
    PyNode *n = (PyNode*)myself;
    if (n->ptr) {

      // if PythonScript node, try to get the name as a 
      PythonScript *ps = dynamic_cast< PythonScript * >( n->ptr );
      if( ps ) {
        PyObject *attr = ps->getPythonAttribute( arg );
        if( attr ) {
          Py_INCREF( attr );
          return attr;
        }
      } 
    }

    return PyNode::getField( myself, arg );
  }

  PyObject* PyNode::getField( PyObject *myself, char* arg ) {
    PyNode *n = (PyNode*)myself;
    if (n->ptr) {
      Field *f = n->ptr->getField( arg );
      if ( f ) 
        return ( PyObject * ) PythonInternals::fieldAsPythonObject( f, false );
      
    }
    // if arg was not a field, then default to Py_FindMethod:
    return Py_FindMethod(PyNode_methods, (PyObject *)myself, arg);
  }

  PyObject* PyNode::getSingleField( PyObject *self, PyObject *args ) {
    if(!args || ! PyString_Check( args ) ) {
      PyErr_SetString( PyExc_ValueError, 
            "Invalid argument(s) to function H3D.Node.getField( self, f )" );
        return 0;
    }
    char *field_name = PyString_AsString( args );
    PyNode *n = (PyNode*)self;
    if (n->ptr) {
      Field *f = n->ptr->getField( field_name );
      if ( f ) 
        return ( PyObject * ) PythonInternals::fieldAsPythonObject( f, false );
      else {
        Py_INCREF( Py_None );
        return Py_None;
      }
    }
    Py_INCREF( Py_None );
    return Py_None;
  }
  
  PyObject* PyNode::getFieldList( PyObject *myself ) {
    PyNode *n = (PyNode*)myself;
    H3DNodeDatabase *db = H3DNodeDatabase::lookupNodeInstance( n->ptr );
    // Can't set the size of the list beforehand since dynamic nodes are not
    // handled correctly then (they would get too many fields).
    PyObject *list = PyList_New( 0 );
    for( H3DNodeDatabase::FieldDBConstIterator i = db->fieldDBBegin();
         db->fieldDBEnd() != i; ++i ) {
        if( n->ptr->getField( (*i) ) ) {
            PyObject *v = PyString_FromString( (*i).c_str() );
            PyList_Append( list, v );
        }
    }
    return list;
  }
  
  
  
  
  ///////////////////////////////////////////////////////////////////
  /// VEC2F
  /// 
  static PyMethodDef PyVec2f_methods[] = {
    { "__repr__", (PyCFunction) PyVec2f::repr, 0 },
    { "__str__", (PyCFunction) PyVec2f::repr, 0 },
    { "length", PyVec2f::length, 0 },
    { "lengthSqr", PyVec2f::lengthSqr, 0 },
    { "normalize", PyVec2f::normalize, 0 },
    { "normalizeSafe", PyVec2f::normalizeSafe, 0 },
    { "dotProduct", PyVec2f::dotProduct, 0 },
    {NULL, NULL}
  };
  
  static PyMemberDef PyVec2f_members[] = {
    {"x", T_FLOAT, offsetof(PyVec2f, x), 0,"x"},
    {"y", T_FLOAT, offsetof(PyVec2f, y), 0,"y"},
    {NULL}  /* Sentinel */
  };
  
  
  static PyNumberMethods PyVec2f_as_number = {
    (binaryfunc)  PyVec2f::add,   /* nb_add */
    (binaryfunc)  PyVec2f::sub,   /* nb_subtract */
    (binaryfunc)  PyVec2f::mul,       /* nb_multiply */
    (binaryfunc)    PyVec2f::div,   /* nb_divide */
    (binaryfunc)    0,          /* nb_remainder */
    (binaryfunc)    0,          /* nb_divmod */
    (ternaryfunc)   0,          /* nb_power */
    (unaryfunc)   PyVec2f::neg, /* nb_negative */
    (unaryfunc)     0,          /* tp_positive */
    (unaryfunc)     0,          /* tp_absolute */
    (inquiry)       0,          /* tp_nonzero */
    (unaryfunc)     0,          /* nb_invert */
    (binaryfunc)    0,          /* nb_lshift */
    (binaryfunc)    0,          /* nb_rshift */
    (binaryfunc)    0,          /* nb_and */
    (binaryfunc)    0,          /* nb_xor */
    (binaryfunc)    0,          /* nb_or */
    (coercion)      0,          /* nb_coerce */
    (unaryfunc)     0,          /* nb_int */
    (unaryfunc)     0,          /* nb_long */
    (unaryfunc)     0,          /* nb_float */
    (unaryfunc)     0,          /* nb_oct */
    (unaryfunc)   0,                  /* nb_hex */
      
    /* Added in release 2.0 */
    /* These require the Py_TPFLAGS_HAVE_INPLACEOPS flag */
    0,                      /* nb_inplace_add */
    0,                      /* nb_inplace_subtract */
    0,                      /* nb_inplace_multiply */
    0,                      /* nb_inplace_divide */
    0,                      /* nb_inplace_remainder */
    (ternaryfunc)0,         /* nb_inplace_power */
    0,                      /* nb_inplace_lshift */
    0,                      /* nb_inplace_rshift */
    0,                      /* nb_inplace_and */
    0,                      /* nb_inplace_xor */
    0,                      /* nb_inplace_or */
      
    /* Added in release 2.2 */
    /* These require the Py_TPFLAGS_HAVE_CLASS flag */
    (binaryfunc)    PyVec2f::div,  /* nb_floor_divide */
    (binaryfunc)    PyVec2f::div,  /* nb_true_divide */
    0,                      /* nb_inplace_floor_divide */
    0,                      /* nb_inplace_true_divide */
  };    
  
  PyTypeObject PyVec2f_Type = {
    PyObject_HEAD_INIT(NULL)
    0,
    "H3D.Vec2f",
    sizeof(PyVec2f),
    0,                         /*tp_itemsize*/
    (destructor)PyVec2f::dealloc, /*tp_dealloc*/
    0,                         /*tp_print*/
    0,                         /*tp_getattr*/
    0,                         /*tp_setattr*/
    (cmpfunc) PyVec2f::compare,                         /*tp_compare*/
    (reprfunc) PyVec2f::repr,                         /*tp_repr*/
    &PyVec2f_as_number,                         /*tp_as_number*/
    0,                         /*tp_as_sequence*/
    0,                         /*tp_as_mapping*/
    0,                         /*tp_hash */
    0,                         /*tp_call*/
    0,                         /*tp_str*/
    0,                         /*tp_getattro*/
    0,                         /*tp_setattro*/
    0,                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | 
    Py_TPFLAGS_BASETYPE |
    Py_TPFLAGS_CHECKTYPES,     /*tp_flags*/
    "Vec2f Object",            /* tp_doc */
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    PyVec2f_methods,           /* tp_methods */
    PyVec2f_members,           /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)PyVec2f::init,   /* tp_init */
    PyType_GenericAlloc,       /* tp_alloc */
    (newfunc) PyType_GenericAlloc,            /* tp_new */
  };

  PyObject* PyVec2f::dotProduct( PyObject *myself, PyObject *args ) {
    if( !args || !PyVec2f_Check( args ) ) {
      PyErr_SetString(PyExc_TypeError, 
              "invalid type given as argument to PyVec2f::dotProduct( v )." );
      return 0;
    }

    Vec2f v = PyVec2f_AsVec2f( myself );
    Vec2f arg = PyVec2f_AsVec2f( args );
    return PyFloat_FromDouble( v.dotProduct( arg ) );
  }

  // Returns an Vec2f representation of the contents of o.
  Vec2f PyVec2f_AsVec2f( PyObject *o ) {
    if( PyVec2f_Check( o ) ) {
      return *(Vec2f *)(PyVec2f *)(o);
    } else {
      throw Exception::H3DAPIException( "PyObject * is not a PyVec2f *", 
                                        H3D_FULL_LOCATION );
    }
  }  

  // Creates a new PyVec2f object based on the value of v.
  PyObject *PyVec2f_FromVec2f( const Vec2f &v) {
    PyObject *o = PyType_GenericAlloc( &PyVec2f_Type, 1 );
    Vec2f *part = (Vec2f *)(PyVec2f *)( o );
    *part = v;
    return o;
  }
  
  int PyVec2f::init(PyVec2f *self, PyObject *args, PyObject *kwds)  {
    if( PyTuple_Size( args ) == 0 ) {
      Vec2f *self_v = (Vec2f *)self;
      *self_v = Vec2f();
    } else if( PyTuple_Size( args ) == 1 ) {
      PyObject *o = PyTuple_GetItem( args, 0 );
      if( PyVec2d_Check( o ) ) {
        Vec2d vd = PyVec2d_AsVec2d( o );
        self->x = (H3DFloat)vd.x;
        self->y = (H3DFloat)vd.y;
      } else if( PyVec2f_Check( o ) ) {
        Vec2f vd = PyVec2f_AsVec2f( o );
        self->x = vd.x;
        self->y = vd.y;
      } else {
        PyErr_SetString(PyExc_TypeError, 
                        "invalid type given to Vec2f constructor." );
        return -1;
      }
    } else {
      static char *kwlist[] = {"x", "y", NULL};
      if (! PyArg_ParseTupleAndKeywords(args, kwds, "ff", kwlist, 
                                        &(self->x), &(self->y) ))
        return -1; 
    }
    return 0;
  }

  PyObject* PyVec2f::length( PyObject *myself, PyObject *args ) {
    Vec2f vec = PyVec2f_AsVec2f( myself );
    return PyFloat_FromDouble( vec.length() );
  }
  
  PyObject* PyVec2f::lengthSqr( PyObject *myself, PyObject *args ) {
    Vec2f vec = PyVec2f_AsVec2f( myself );
    return PyFloat_FromDouble( vec.lengthSqr() );
  }
  
  PyObject* PyVec2f::normalize( PyObject *myself, PyObject *args ) {
    PyVec2f *v = (PyVec2f*)myself;
    Vec2f *vec = (Vec2f*)v;
    try {
      vec->normalize();
    } catch ( Exception::H3DException &e ) {
      PyErr_SetString( PyExc_ValueError, e.message.c_str() );
      return 0;
    }
    Py_INCREF( myself );
    return myself;
  }
  
   PyObject* PyVec2f::normalizeSafe( PyObject *myself, PyObject *args ) {
    PyVec2f *v = (PyVec2f*)myself;
    Vec2f *vec = (Vec2f*)v;
    vec->normalizeSafe();
    Py_INCREF( myself );
    return myself;
  }  

  ///////////////////////////////////////////////////////////////////
  /// VEC2D
  /// 
  static PyMethodDef PyVec2d_methods[] = {
    { "__repr__", (PyCFunction) PyVec2d::repr, 0 },
    { "__str__", (PyCFunction) PyVec2d::repr, 0 },
    { "length", PyVec2d::length, 0 },
    { "lengthSqr", PyVec2d::lengthSqr, 0 },
    { "normalize", PyVec2d::normalize, 0 },
    { "normalizeSafe", PyVec2d::normalizeSafe, 0 },
    { "dotProduct", PyVec2d::dotProduct, 0 },
    {NULL, NULL}
  };
  
  static PyMemberDef PyVec2d_members[] = {
    {"x", T_DOUBLE, offsetof(PyVec2d, x), 0,"x"},
    {"y", T_DOUBLE, offsetof(PyVec2d, y), 0,"y"},
    {NULL}  /* Sentinel */
  };
  
  
  static PyNumberMethods PyVec2d_as_number = {
    (binaryfunc)  PyVec2d::add,   /* nb_add */
    (binaryfunc)  PyVec2d::sub,   /* nb_subtract */
    (binaryfunc)  PyVec2d::mul,   /* nb_multiply */
    (binaryfunc)    PyVec2d::div, /* nb_divide */
    (binaryfunc)    0,            /* nb_remainder */
    (binaryfunc)    0,            /* nb_divmod */
    (ternaryfunc)   0,            /* nb_power */
    (unaryfunc)     PyVec2d::neg, /* nb_negative */
    (unaryfunc)     0,            /* tp_positive */
    (unaryfunc)     0,            /* tp_absolute */
    (inquiry)       0,            /* tp_nonzero */
    (unaryfunc)     0,            /* nb_invert */
    (binaryfunc)    0,            /* nb_lshift */
    (binaryfunc)    0,            /* nb_rshift */
    (binaryfunc)    0,            /* nb_and */
    (binaryfunc)    0,            /* nb_xor */
    (binaryfunc)    0,            /* nb_or */
    (coercion)      0,            /* nb_coerce */
    (unaryfunc)     0,            /* nb_int */
    (unaryfunc)     0,            /* nb_long */
    (unaryfunc)     0,            /* nb_float */
    (unaryfunc)     0,            /* nb_oct */
    (unaryfunc)   0,              /* nb_hex */
      
    /* Added in release 2.0 */
    /* These require the Py_TPFLAGS_HAVE_INPLACEOPS flag */
    0,                      /* nb_inplace_add */
    0,                      /* nb_inplace_subtract */
    0,                      /* nb_inplace_multiply */
    0,                      /* nb_inplace_divide */
    0,                      /* nb_inplace_remainder */
    (ternaryfunc)0,         /* nb_inplace_power */
    0,                      /* nb_inplace_lshift */
    0,                      /* nb_inplace_rshift */
    0,                      /* nb_inplace_and */
    0,                      /* nb_inplace_xor */
    0,                      /* nb_inplace_or */
      
    /* Added in release 2.2 */
    /* These require the Py_TPFLAGS_HAVE_CLASS flag */
    (binaryfunc)    PyVec2d::div,  /* nb_floor_divide */
    (binaryfunc)    PyVec2d::div,  /* nb_true_divide */
    0,                      /* nb_inplace_floor_divide */
    0,                      /* nb_inplace_true_divide */
  };    
  
  PyTypeObject PyVec2d_Type = {
    PyObject_HEAD_INIT(NULL)
    0,
    "H3D.Vec2d",
    sizeof(PyVec2d),
    0,                         /*tp_itemsize*/
    (destructor)PyVec2d::dealloc, /*tp_dealloc*/
    0,                         /*tp_print*/
    0,                         /*tp_getattr*/
    0,                         /*tp_setattr*/
    (cmpfunc) PyVec2d::compare,                         /*tp_compare*/
    (reprfunc) PyVec2d::repr,                         /*tp_repr*/
    &PyVec2d_as_number,                         /*tp_as_number*/
    0,                         /*tp_as_sequence*/
    0,                         /*tp_as_mapping*/
    0,                         /*tp_hash */
    0,                         /*tp_call*/
    0,                         /*tp_str*/
    0,                         /*tp_getattro*/
    0,                         /*tp_setattro*/
    0,                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | 
    Py_TPFLAGS_BASETYPE |
    Py_TPFLAGS_CHECKTYPES,     /*tp_flags*/
    "Vec2d Object",            /* tp_doc */
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    PyVec2d_methods,           /* tp_methods */
    PyVec2d_members,           /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)PyVec2d::init,   /* tp_init */
    PyType_GenericAlloc,       /* tp_alloc */
    (newfunc) PyType_GenericAlloc,            /* tp_new */
  };
  
  PyObject* PyVec2d::dotProduct( PyObject *myself, PyObject *args ) {
    if( !args || !PyVec2d_Check( args ) ) {
      PyErr_SetString(PyExc_TypeError, 
         "invalid type given as argument to PyVec2d::dotProduct( v )." );
      return 0;
    }

    Vec2d v = PyVec2d_AsVec2d( myself );
    Vec2d arg = PyVec2d_AsVec2d( args );
    return PyFloat_FromDouble( v.dotProduct( arg ) );
  }

  // Returns an Vec2d representation of the contents of o.
  Vec2d PyVec2d_AsVec2d( PyObject *o ) {
    if( PyVec2d_Check( o ) ) {
      return *(Vec2d *)(PyVec2d *)(o);
    } else {
      throw Exception::H3DAPIException( "PyObject * is not a PyVec2d *", 
                                        H3D_FULL_LOCATION );
    }
  }  

  // Creates a new PyVec2d object based on the value of v.
  PyObject *PyVec2d_FromVec2d( const Vec2d &v) {
    PyObject *o = PyType_GenericAlloc( &PyVec2d_Type, 1 );
    Vec2d *part = (Vec2d *)(PyVec2d *)( o );
    *part = v;
    return o;
  }
  int PyVec2d::init(PyVec2d *self, PyObject *args, PyObject *kwds)  {
    if( PyTuple_Size( args ) == 0 ) {
      Vec2d *self_v = (Vec2d *)self;
      *self_v = Vec2d();
    } else if( PyTuple_Size( args ) == 1 ) {
      PyObject *o = PyTuple_GetItem( args, 0 );
      if( PyVec2d_Check( o ) ) {
        Vec2d vd = PyVec2d_AsVec2d( o );
        self->x = vd.x;
        self->y = vd.y;
      } else if( PyVec2f_Check( o ) ) {
        Vec2f vd = PyVec2f_AsVec2f( o );
        self->x = vd.x;
        self->y = vd.y;
      } else {
        PyErr_SetString(PyExc_TypeError, 
                        "invalid type given to Vec2d constructor." );
        return -1;
      }
    } else {
      static char *kwlist[] = {"x", "y", NULL};
      if (! PyArg_ParseTupleAndKeywords(args, kwds, "dd", kwlist, 
                                        &(self->x), &(self->y)))
        return -1; 
    }
    return 0;
  }

  
  PyObject* PyVec2d::length( PyObject *myself, PyObject *args ) {
    Vec2d vec = PyVec2d_AsVec2d( myself );
    return PyFloat_FromDouble( vec.length() );
  }
  
  PyObject* PyVec2d::lengthSqr( PyObject *myself, PyObject *args ) {
    Vec2d vec = PyVec2d_AsVec2d( myself );
    return PyFloat_FromDouble( vec.lengthSqr() );
  }
  
  PyObject* PyVec2d::normalize( PyObject *myself, PyObject *args ) {
    PyVec2d *v = (PyVec2d*)myself;
    Vec2d *vec = (Vec2d*)v;
    try {
      vec->normalize();
    } catch ( Exception::H3DException &e ) {
      PyErr_SetString( PyExc_ValueError, e.message.c_str() );
      return 0;
    }
    Py_INCREF( myself );
    return myself;
  }
  
   PyObject* PyVec2d::normalizeSafe( PyObject *myself, PyObject *args ) {
    PyVec2d *v = (PyVec2d*)myself;
    Vec2d *vec = (Vec2d*)v;
    vec->normalizeSafe();
    Py_INCREF( myself );
    return myself;
  }  
  
  
  
  ///////////////////////////////////////////////////////////////////
  /// VEC3F
  /// 
  static PyMethodDef PyVec3f_methods[] = {
    { "__repr__", (PyCFunction) PyVec3f::repr, 0 },
    { "__str__", (PyCFunction) PyVec3f::repr, 0 },
    { "length", PyVec3f::length, 0 },
    { "lengthSqr", PyVec3f::lengthSqr, 0 },
    { "normalize", PyVec3f::normalize, 0 },
    { "normalizeSafe", PyVec3f::normalizeSafe, 0 },
    { "dotProduct", PyVec3f::dotProduct, 0 },
    { "crossProduct", PyVec3f::crossProduct, 0 },
    {NULL, NULL}
  };
  
  static PyMemberDef PyVec3f_members[] = {
    {"x", T_FLOAT, offsetof(PyVec3f, x), 0,"x"},
    {"y", T_FLOAT, offsetof(PyVec3f, y), 0,"y"},
    {"z", T_FLOAT, offsetof(PyVec3f, z), 0,"z"},
    {NULL}  /* Sentinel */
  };
  
  
  static PyNumberMethods PyVec3f_as_number = {
    (binaryfunc)  PyVec3f::add, /* nb_add */
    (binaryfunc)  PyVec3f::sub, /* nb_subtract */
    (binaryfunc)  PyVec3f::mul, /* nb_multiply */
    (binaryfunc)  PyVec3f::div, /* nb_divide */
    (binaryfunc)  PyVec3f::mod, /* nb_remainder */
    (binaryfunc)    0,          /* nb_divmod */
    (ternaryfunc)   0,          /* nb_power */
    (unaryfunc)   PyVec3f::neg, /* nb_negative */
    (unaryfunc)     0,          /* tp_positive */
    (unaryfunc)     0,          /* tp_absolute */
    (inquiry)       0,          /* tp_nonzero */
    (unaryfunc)     0,          /* nb_invert */
    (binaryfunc)    0,          /* nb_lshift */
    (binaryfunc)    0,          /* nb_rshift */
    (binaryfunc)    0,          /* nb_and */
    (binaryfunc)    0,          /* nb_xor */
    (binaryfunc)    0,          /* nb_or */
    (coercion)      0,          /* nb_coerce */
    (unaryfunc)     0,          /* nb_int */
    (unaryfunc)     0,          /* nb_long */
    (unaryfunc)     0,          /* nb_float */
    (unaryfunc)     0,          /* nb_oct */
    (unaryfunc)     0,          /* nb_hex */
      
    /* Added in release 2.0 */
    /* These require the Py_TPFLAGS_HAVE_INPLACEOPS flag */
    0,                      /* nb_inplace_add */
    0,                      /* nb_inplace_subtract */
    0,                      /* nb_inplace_multiply */
    0,                      /* nb_inplace_divide */
    0,                      /* nb_inplace_remainder */
    (ternaryfunc)0,         /* nb_inplace_power */
    0,                      /* nb_inplace_lshift */
    0,                      /* nb_inplace_rshift */
    0,                      /* nb_inplace_and */
    0,                      /* nb_inplace_xor */
    0,                      /* nb_inplace_or */
      
    /* Added in release 2.2 */
    /* These require the Py_TPFLAGS_HAVE_CLASS flag */
    (binaryfunc)    PyVec3f::div,  /* nb_floor_divide */
    (binaryfunc)    PyVec3f::div,  /* nb_true_divide */
    0,                      /* nb_inplace_floor_divide */
    0,                      /* nb_inplace_true_divide */
  };    
  PyTypeObject PyVec3f_Type = {
    PyObject_HEAD_INIT(NULL)
    0,
    "H3D.Vec3f",
    sizeof(PyVec3f),
    0,                         /*tp_itemsize*/
    (destructor)PyVec3f::dealloc, /*tp_dealloc*/
    0,                         /*tp_print*/
    0,                         /*tp_getattr*/
    0,                         /*tp_setattr*/
    (cmpfunc) PyVec3f::compare,                         /*tp_compare*/
    (reprfunc) PyVec3f::repr,                         /*tp_repr*/
    &PyVec3f_as_number,                         /*tp_as_number*/
    0,                         /*tp_as_sequence*/
    0,                         /*tp_as_mapping*/
    0,                         /*tp_hash */
    0,                         /*tp_call*/
    0,                         /*tp_str*/
    0,                         /*tp_getattro*/
    0,                         /*tp_setattro*/
    0,                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | 
    Py_TPFLAGS_BASETYPE | 
    Py_TPFLAGS_CHECKTYPES,     /*tp_flags*/
    "Vec3f Object",            /* tp_doc */
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    PyVec3f_methods,           /* tp_methods */
    PyVec3f_members,           /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)PyVec3f::init,   /* tp_init */
    PyType_GenericAlloc,       /* tp_alloc */
    (newfunc) PyType_GenericAlloc,            /* tp_new */
  };

  // Returns an Vec3f representation of the contents of o.
  Vec3f PyVec3f_AsVec3f( PyObject *o ) {
    if( PyVec3f_Check( o ) ) {
      return *(Vec3f *)(PyVec3f *)(o);
    } else {
      throw Exception::H3DAPIException( "PyObject * is not a PyVec3f *", 
                                        H3D_FULL_LOCATION );
    }
  }  

  // Creates a new PyVec3f object based on the value of v.
  PyObject *PyVec3f_FromVec3f( const Vec3f &v) {
    PyObject *o = PyType_GenericAlloc( &PyVec3f_Type, 1 );
    Vec3f *part = (Vec3f *)(PyVec3f *)( o );
    *part = v;
    return o;
  }
  
  int PyVec3f::init(PyVec3f *self, PyObject *args, PyObject *kwds)  {
    if( PyTuple_Size( args ) == 0 ) {
      Vec3f *self_v = (Vec3f *)self;
      *self_v = Vec3f();
    } else if( PyTuple_Size( args ) == 1 ) {
      PyObject *o = PyTuple_GetItem( args, 0 );
      if( PyVec3d_Check( o ) ) {
        Vec3d vd = PyVec3d_AsVec3d( o );
        self->x = (H3DFloat)vd.x;
        self->y = (H3DFloat)vd.y;
        self->z = (H3DFloat)vd.z;
      } else if( PyVec3f_Check( o ) ) {
        Vec3f vd = PyVec3f_AsVec3f( o );
        self->x = vd.x;
        self->y = vd.y;
        self->z = vd.z;
      } else {
        PyErr_SetString(PyExc_TypeError, 
                        "invalid type given to Vec3f constructor." );
        return -1;
      }
    } else {
      static char *kwlist[] = {"x", "y", "z", NULL};
      if (! PyArg_ParseTupleAndKeywords(args, kwds, "fff", kwlist, 
                                        &(self->x), &(self->y), &(self->z)))
        return -1; 
    }
    return 0;
  }
  
  PyObject* PyVec3f::dotProduct( PyObject *myself, PyObject *args ) {
    if( !args || !PyVec3f_Check( args ) ) {
      PyErr_SetString(PyExc_TypeError, 
         "invalid type given as argument to PyVec3f::dotProduct( v )." );
      return 0;
    }

    Vec3f v = PyVec3f_AsVec3f( myself );
    Vec3f arg = PyVec3f_AsVec3f( args );
    return PyFloat_FromDouble( v.dotProduct( arg ) );
  }

  PyObject* PyVec3f::crossProduct( PyObject *myself, PyObject *args ) {
    if( !args || !PyVec3f_Check( args ) ) {
      PyErr_SetString(PyExc_TypeError, 
         "invalid type given as argument to PyVec3f::crossProduct( v )." );
      return 0;
    }

    Vec3f v = PyVec3f_AsVec3f( myself );
    Vec3f arg = PyVec3f_AsVec3f( args );
    return PyVec3f_FromVec3f( v.crossProduct( arg ) );
  }

  PyObject* PyVec3f::mod( PyObject *veca, PyObject *vecb ) {
    if( PyVec3f_Check( veca ) && PyVec3f_Check( vecb ) ){
      Vec3f c = PyVec3f_AsVec3f( veca ) % PyVec3f_AsVec3f( vecb );
      return PyVec3f_FromVec3f( c );
    }
    Py_INCREF( Py_NotImplemented );
    return Py_NotImplemented;
  }

    
  PyObject* PyVec3f::length( PyObject *myself, PyObject *args ) {
    Vec3f vec = PyVec3f_AsVec3f( myself );
    return PyFloat_FromDouble( vec.length() );
  }
  
  PyObject* PyVec3f::lengthSqr( PyObject *myself, PyObject *args ) {
    Vec3f vec = PyVec3f_AsVec3f( myself );
    return PyFloat_FromDouble( vec.lengthSqr() );
  }
  
  PyObject* PyVec3f::normalize( PyObject *myself, PyObject *args ) {
    PyVec3f *v = (PyVec3f*)myself;
    Vec3f *vec = (Vec3f*)v;
    try {
      vec->normalize();
    } catch ( Exception::H3DException &e ) {
      PyErr_SetString( PyExc_ValueError, e.message.c_str() );
      return 0;
    }
    Py_INCREF( myself );
    return myself;
  }
  
   PyObject* PyVec3f::normalizeSafe( PyObject *myself, PyObject *args ) {
    PyVec3f *v = (PyVec3f*)myself;
    Vec3f *vec = (Vec3f*)v;
    vec->normalizeSafe();
    Py_INCREF( myself );
    return myself;
  }
  
 
  ///////////////////////////////////////////////////////////////////
  /// VEC3D
  /// 
  static PyMethodDef PyVec3d_methods[] = {
    { "__repr__", (PyCFunction) PyVec3d::repr, 0 },
    { "__str__", (PyCFunction) PyVec3d::repr, 0 },
    { "length", PyVec3d::length, 0 },
    { "lengthSqr", PyVec3d::lengthSqr, 0 },
    { "normalize", PyVec3d::normalize, 0 },
    { "normalizeSafe", PyVec3d::normalizeSafe, 0 },
    { "dotProduct", PyVec3d::dotProduct, 0 },
    { "crossProduct", PyVec3d::crossProduct, 0 },
    {NULL, NULL}
  };
  
  static PyMemberDef PyVec3d_members[] = {
    {"x", T_DOUBLE, offsetof(PyVec3d, x), 0,"x"},
    {"y", T_DOUBLE, offsetof(PyVec3d, y), 0,"y"},
    {"z", T_DOUBLE, offsetof(PyVec3d, z), 0,"z"},
    {NULL}  /* Sentinel */
  };
  
  
  static PyNumberMethods PyVec3d_as_number = {
    (binaryfunc)  PyVec3d::add, /* nb_add */
    (binaryfunc)  PyVec3d::sub, /* nb_subtract */
    (binaryfunc)  PyVec3d::mul, /* nb_multiply */
    (binaryfunc)  PyVec3d::div, /* nb_divide */
    (binaryfunc)  PyVec3d::mod, /* nb_remainder */
    (binaryfunc)    0,          /* nb_divmod */
    (ternaryfunc)   0,          /* nb_power */
    (unaryfunc)   PyVec3d::neg, /* nb_negative */
    (unaryfunc)     0,          /* tp_positive */
    (unaryfunc)     0,          /* tp_absolute */
    (inquiry)       0,          /* tp_nonzero */
    (unaryfunc)     0,          /* nb_invert */
    (binaryfunc)    0,          /* nb_lshift */
    (binaryfunc)    0,          /* nb_rshift */
    (binaryfunc)    0,          /* nb_and */
    (binaryfunc)    0,          /* nb_xor */
    (binaryfunc)    0,          /* nb_or */
    (coercion)      0,          /* nb_coerce */
    (unaryfunc)     0,          /* nb_int */
    (unaryfunc)     0,          /* nb_long */
    (unaryfunc)     0,          /* nb_float */
    (unaryfunc)     0,          /* nb_oct */
    (unaryfunc)     0,          /* nb_hex */
      
    /* Added in release 2.0 */
    /* These require the Py_TPFLAGS_HAVE_INPLACEOPS flag */
    0,                      /* nb_inplace_add */
    0,                      /* nb_inplace_subtract */
    0,                      /* nb_inplace_multiply */
    0,                      /* nb_inplace_divide */
    0,                      /* nb_inplace_remainder */
    (ternaryfunc)0,         /* nb_inplace_power */
    0,                      /* nb_inplace_lshift */
    0,                      /* nb_inplace_rshift */
    0,                      /* nb_inplace_and */
    0,                      /* nb_inplace_xor */
    0,                      /* nb_inplace_or */
      
    /* Added in release 2.2 */
    /* These require the Py_TPFLAGS_HAVE_CLASS flag */
    (binaryfunc)    PyVec3d::div,  /* nb_floor_divide */
    (binaryfunc)    PyVec3d::div,  /* nb_true_divide */
    0,                      /* nb_inplace_floor_divide */
    0,                      /* nb_inplace_true_divide */
  };    
  PyTypeObject PyVec3d_Type = {
    PyObject_HEAD_INIT(NULL)
    0,
    "H3D.Vec3d",
    sizeof(PyVec3d),
    0,                         /*tp_itemsize*/
    (destructor)PyVec3d::dealloc, /*tp_dealloc*/
    0,                         /*tp_print*/
    0,                         /*tp_getattr*/
    0,                         /*tp_setattr*/
    (cmpfunc) PyVec3d::compare,                         /*tp_compare*/
    (reprfunc) PyVec3d::repr,                         /*tp_repr*/
    &PyVec3d_as_number,                         /*tp_as_number*/
    0,                         /*tp_as_sequence*/
    0,                         /*tp_as_mapping*/
    0,                         /*tp_hash */
    0,                         /*tp_call*/
    0,                         /*tp_str*/
    0,                         /*tp_getattro*/
    0,                         /*tp_setattro*/
    0,                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | 
    Py_TPFLAGS_BASETYPE | 
    Py_TPFLAGS_CHECKTYPES,     /*tp_flags*/
    "Vec3d Object",            /* tp_doc */
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    PyVec3d_methods,           /* tp_methods */
    PyVec3d_members,           /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)PyVec3d::init,   /* tp_init */
    PyType_GenericAlloc,       /* tp_alloc */
    (newfunc) PyType_GenericAlloc,            /* tp_new */
  };
  
  // Returns an Vec3d representation of the contents of o.
  Vec3d PyVec3d_AsVec3d( PyObject *o ) {
    if( PyVec3d_Check( o ) ) {
      return *(Vec3d *)(PyVec3d *)(o);
    } else {
      throw Exception::H3DAPIException( "PyObject * is not a PyVec3d *", 
                                        H3D_FULL_LOCATION );
    }
  }  

  // Creates a new PyVec3d object based on the value of v.
  PyObject *PyVec3d_FromVec3d( const Vec3d &v) {
    PyObject *o = PyType_GenericAlloc( &PyVec3d_Type, 1 );
    Vec3d *part = (Vec3d *)(PyVec3d *)( o );
    *part = v;
    return o;
  }

  PyObject* PyVec3d::dotProduct( PyObject *myself, PyObject *args ) {
    if( !args || !PyVec3d_Check( args ) ) {
      PyErr_SetString(PyExc_TypeError, 
         "invalid type given as argument to PyVec3d::dotProduct( v )." );
      return 0;
    }

    Vec3d v = PyVec3d_AsVec3d( myself );
    Vec3d arg = PyVec3d_AsVec3d( args );
    return PyFloat_FromDouble( v.dotProduct( arg ) );
  }

  PyObject* PyVec3d::crossProduct( PyObject *myself, PyObject *args ) {
    if( !args || !PyVec3d_Check( args ) ) {
      PyErr_SetString(PyExc_TypeError, 
         "invalid type given as argument to PyVec3d::crossProduct( v )." );
      return 0;
    }

    Vec3d v = PyVec3d_AsVec3d( myself );
    Vec3d arg = PyVec3d_AsVec3d( args );
    return PyVec3d_FromVec3d( v.crossProduct( arg ) );
  }

  int PyVec3d::init(PyVec3d *self, PyObject *args, PyObject *kwds)  {
    if( PyTuple_Size( args ) == 0 ) {
      Vec3d *self_v = (Vec3d *)self;
      *self_v = Vec3d();
    } else if( PyTuple_Size( args ) == 1 ) {
      PyObject *o = PyTuple_GetItem( args, 0 );
      if( PyVec3d_Check( o ) ) {
        Vec3d vd = PyVec3d_AsVec3d( o );
        self->x = vd.x;
        self->y = vd.y;
        self->z = vd.z;
      } else if( PyVec3f_Check( o ) ) {
        Vec3f vd = PyVec3f_AsVec3f( o );
        self->x = vd.x;
        self->y = vd.y;
        self->z = vd.z;
      } else {
        PyErr_SetString(PyExc_TypeError, 
                        "invalid type given to Vec3d constructor." );
        return -1;
      }
    } else {
      static char *kwlist[] = {"x", "y", "z", NULL};
      if (! PyArg_ParseTupleAndKeywords(args, kwds, "ddd", kwlist, 
                                        &(self->x), &(self->y), &(self->z)))
        return -1; 
    }
    return 0;
  }
  
  PyObject* PyVec3d::mod( PyObject *veca, PyObject *vecb ) {
    if( PyVec3d_Check( veca ) && PyVec3d_Check( vecb ) ){
      Vec3d c = PyVec3d_AsVec3d( veca ) % PyVec3d_AsVec3d( vecb );
      return PyVec3d_FromVec3d( c );
    }
    Py_INCREF( Py_NotImplemented );
    return Py_NotImplemented;
  }

  PyObject* PyVec3d::length( PyObject *myself, PyObject *args ) {
    Vec3d vec = PyVec3d_AsVec3d( myself );
    return PyFloat_FromDouble( vec.length() );
  }
  
  PyObject* PyVec3d::lengthSqr( PyObject *myself, PyObject *args ) {
    Vec3d vec = PyVec3d_AsVec3d( myself );
    return PyFloat_FromDouble( vec.lengthSqr() );
  }
  
  PyObject* PyVec3d::normalize( PyObject *myself, PyObject *args ) {
    PyVec3d *v = (PyVec3d*)myself;
    Vec3d *vec = (Vec3d*)v;
    try {
      vec->normalize();
    } catch ( Exception::H3DException &e ) {
      PyErr_SetString( PyExc_ValueError, e.message.c_str() );
      return 0;
    }
    Py_INCREF( myself );
    return myself;
  }
  
   PyObject* PyVec3d::normalizeSafe( PyObject *myself, PyObject *args ) {
    PyVec3d *v = (PyVec3d*)myself;
    Vec3d *vec = (Vec3d*)v;
    vec->normalizeSafe();
    Py_INCREF( myself );
    return myself;
  }
  
  ///////////////////////////////////////////////////////////////////
  /// VEC4F
  /// 
  static PyMethodDef PyVec4f_methods[] = {
    { "__repr__", (PyCFunction) PyVec4f::repr, 0 },
    { "__str__", (PyCFunction) PyVec4f::repr, 0 },
    {NULL, NULL}
  };
  
  static PyMemberDef PyVec4f_members[] = {
    {"x", T_FLOAT, offsetof(PyVec4f, x), 0,"x"},
    {"y", T_FLOAT, offsetof(PyVec4f, y), 0,"y"},
    {"z", T_FLOAT, offsetof(PyVec4f, z), 0,"z"},
    {"w", T_FLOAT, offsetof(PyVec4f, w), 0,"w"},
    {NULL}  /* Sentinel */
  };
  
  
  static PyNumberMethods PyVec4f_as_number = {
    (binaryfunc)  PyVec4f::add,   /* nb_add */
    (binaryfunc)  PyVec4f::sub,   /* nb_subtract */
    (binaryfunc)  PyVec4f::mul,   /* nb_multiply */
    (binaryfunc)    PyVec4f::div, /* nb_divide */
    (binaryfunc)    0,          /* nb_remainder */
    (binaryfunc)    0,          /* nb_divmod */
    (ternaryfunc)   0,          /* nb_power */
    (unaryfunc)  PyVec4f::neg,  /* nb_negative */
    (unaryfunc)     0,          /* tp_positive */
    (unaryfunc)     0,          /* tp_absolute */
    (inquiry)       0,          /* tp_nonzero */
    (unaryfunc)     0,          /* nb_invert */
    (binaryfunc)    0,          /* nb_lshift */
    (binaryfunc)    0,          /* nb_rshift */
    (binaryfunc)    0,          /* nb_and */
    (binaryfunc)    0,          /* nb_xor */
    (binaryfunc)    0,          /* nb_or */
    (coercion)      0,          /* nb_coerce */
    (unaryfunc)     0,          /* nb_int */
    (unaryfunc)     0,          /* nb_long */
    (unaryfunc)     0,          /* nb_float */
    (unaryfunc)     0,          /* nb_oct */
    (unaryfunc)   0,                  /* nb_hex */
      
    /* Added in release 2.0 */
    /* These require the Py_TPFLAGS_HAVE_INPLACEOPS flag */
    0,                      /* nb_inplace_add */
    0,                      /* nb_inplace_subtract */
    0,                      /* nb_inplace_multiply */
    0,                      /* nb_inplace_divide */
    0,                      /* nb_inplace_remainder */
    (ternaryfunc)0,         /* nb_inplace_power */
    0,                      /* nb_inplace_lshift */
    0,                      /* nb_inplace_rshift */
    0,                      /* nb_inplace_and */
    0,                      /* nb_inplace_xor */
    0,                      /* nb_inplace_or */
      
    /* Added in release 2.2 */
    /* These require the Py_TPFLAGS_HAVE_CLASS flag */
    (binaryfunc)    PyVec4f::div,  /* nb_floor_divide */
    (binaryfunc)    PyVec4f::div,  /* nb_true_divide */
    0,                      /* nb_inplace_floor_divide */
    0,                      /* nb_inplace_true_divide */
  };    
  PyTypeObject PyVec4f_Type = {
    PyObject_HEAD_INIT(NULL)
    0,
    "H3D.Vec4f",
    sizeof(PyVec4f),
    0,                         /*tp_itemsize*/
    (destructor)PyVec4f::dealloc, /*tp_dealloc*/
    0,                         /*tp_print*/
    0,                         /*tp_getattr*/
    0,                         /*tp_setattr*/
    (cmpfunc) PyVec4f::compare,                         /*tp_compare*/
    (reprfunc) PyVec4f::repr,                         /*tp_repr*/
    &PyVec4f_as_number,                         /*tp_as_number*/
    0,                         /*tp_as_sequence*/
    0,                         /*tp_as_mapping*/
    0,                         /*tp_hash */
    0,                         /*tp_call*/
    0,                         /*tp_str*/
    0,                         /*tp_getattro*/
    0,                         /*tp_setattro*/
    0,                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | 
    Py_TPFLAGS_BASETYPE |
    Py_TPFLAGS_CHECKTYPES,     /*tp_flags*/
    "Vec4f Object",            /* tp_doc */
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    PyVec4f_methods,           /* tp_methods */
    PyVec4f_members,           /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)PyVec4f::init,   /* tp_init */
    PyType_GenericAlloc,       /* tp_alloc */
    (newfunc) PyType_GenericAlloc,            /* tp_new */
  };

  // Returns an Vec4f representation of the contents of o.
  Vec4f PyVec4f_AsVec4f( PyObject *o ) {
    if( PyVec4f_Check( o ) ) {
      return *(Vec4f *)(PyVec4f *)(o);
    } else {
      throw Exception::H3DAPIException( "PyObject * is not a PyVec4f *", 
                                        H3D_FULL_LOCATION );
    }
  }  

  // Creates a new PyVec4f object based on the value of v.
  PyObject *PyVec4f_FromVec4f( const Vec4f &v) {
    PyObject *o = PyType_GenericAlloc( &PyVec4f_Type, 1 );
    Vec4f *part = (Vec4f *)(PyVec4f *)( o );
    *part = v;
    return o;
  }
  
  int PyVec4f::init(PyVec4f *self, PyObject *args, PyObject *kwds)  {
    if( PyTuple_Size( args ) == 0 ) {
      Vec4f *self_v = (Vec4f *)self;
      *self_v = Vec4f();
    } else if( PyTuple_Size( args ) == 1 ) {
      PyObject *o = PyTuple_GetItem( args, 0 );
      if( PyVec4d_Check( o ) ) {
        Vec4d vd = PyVec4d_AsVec4d( o );
        self->x = (H3DFloat)vd.x;
        self->y = (H3DFloat)vd.y;
        self->z = (H3DFloat)vd.z;
        self->w = (H3DFloat)vd.w;
      } else if( PyVec4f_Check( o ) ) {
        Vec4f vd = PyVec4f_AsVec4f( o );
        self->x = vd.x;
        self->y = vd.y;
        self->z = vd.z;
        self->w = vd.w;
      } else {
        PyErr_SetString(PyExc_TypeError, 
                        "invalid type given to Vec4f constructor." );
        return -1;
      }
    } else {
      static char *kwlist[] = {"x", "y", "z", "w", NULL};
      if (! PyArg_ParseTupleAndKeywords(args, kwds, "ffff", kwlist, 
                                        &(self->x), &(self->y), 
                                        &(self->z), &(self->w)))
        return -1; 
    }
    return 0;
  }
  
  ///////////////////////////////////////////////////////////////////
  /// VEC4D
  /// 
  static PyMethodDef PyVec4d_methods[] = {
    { "__repr__", (PyCFunction) PyVec4d::repr, 0 },
    { "__str__", (PyCFunction) PyVec4d::repr, 0 },
    {NULL, NULL}
  };
  
  static PyMemberDef PyVec4d_members[] = {
    {"x", T_DOUBLE, offsetof(PyVec4d, x), 0,"x"},
    {"y", T_DOUBLE, offsetof(PyVec4d, y), 0,"y"},
    {"z", T_DOUBLE, offsetof(PyVec4d, z), 0,"z"},
    {"w", T_DOUBLE, offsetof(PyVec4d, w), 0,"w"},
    {NULL}  /* Sentinel */
  };
  
  static PyNumberMethods PyVec4d_as_number = {
    (binaryfunc)  PyVec4d::add,   /* nb_add */
    (binaryfunc)  PyVec4d::sub,   /* nb_subtract */
    (binaryfunc)  PyVec4d::mul,       /* nb_multiply */
    (binaryfunc)    PyVec4d::div,   /* nb_divide */
    (binaryfunc)    0,          /* nb_remainder */
    (binaryfunc)    0,          /* nb_divmod */
    (ternaryfunc)   0,          /* nb_power */
    (unaryfunc)   PyVec4d::neg, /* nb_negative */
    (unaryfunc)     0,          /* tp_positive */
    (unaryfunc)     0,          /* tp_absolute */
    (inquiry)       0,          /* tp_nonzero */
    (unaryfunc)     0,          /* nb_invert */
    (binaryfunc)    0,          /* nb_lshift */
    (binaryfunc)    0,          /* nb_rshift */
    (binaryfunc)    0,          /* nb_and */
    (binaryfunc)    0,          /* nb_xor */
    (binaryfunc)    0,          /* nb_or */
    (coercion)      0,          /* nb_coerce */
    (unaryfunc)     0,          /* nb_int */
    (unaryfunc)     0,          /* nb_long */
    (unaryfunc)     0,          /* nb_float */
    (unaryfunc)     0,          /* nb_oct */
    (unaryfunc)   0,                  /* nb_hex */
      
    /* Added in release 2.0 */
    /* These require the Py_TPFLAGS_HAVE_INPLACEOPS flag */
    0,                      /* nb_inplace_add */
    0,                      /* nb_inplace_subtract */
    0,                      /* nb_inplace_multiply */
    0,                      /* nb_inplace_divide */
    0,                      /* nb_inplace_remainder */
    (ternaryfunc)0,         /* nb_inplace_power */
    0,                      /* nb_inplace_lshift */
    0,                      /* nb_inplace_rshift */
    0,                      /* nb_inplace_and */
    0,                      /* nb_inplace_xor */
    0,                      /* nb_inplace_or */
      
    /* Added in release 2.2 */
    /* These require the Py_TPFLAGS_HAVE_CLASS flag */
    (binaryfunc)    PyVec4d::div,  /* nb_floor_divide */
    (binaryfunc)    PyVec4d::div,  /* nb_true_divide */
    0,                      /* nb_inplace_floor_divide */
    0,                      /* nb_inplace_true_divide */
  };    
  PyTypeObject PyVec4d_Type = {
    PyObject_HEAD_INIT(NULL)
    0,
    "H3D.Vec4d",
    sizeof(PyVec4d),
    0,                         /*tp_itemsize*/
    (destructor)PyVec4d::dealloc, /*tp_dealloc*/
    0,                         /*tp_print*/
    0,                         /*tp_getattr*/
    0,                         /*tp_setattr*/
    (cmpfunc) PyVec4d::compare,                         /*tp_compare*/
    (reprfunc) PyVec4d::repr,                         /*tp_repr*/
    &PyVec4d_as_number,                         /*tp_as_number*/
    0,                         /*tp_as_sequence*/
    0,                         /*tp_as_mapping*/
    0,                         /*tp_hash */
    0,                         /*tp_call*/
    0,                         /*tp_str*/
    0,                         /*tp_getattro*/
    0,                         /*tp_setattro*/
    0,                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | 
    Py_TPFLAGS_BASETYPE |
    Py_TPFLAGS_CHECKTYPES,     /*tp_flags*/
    "Vec4d Object",            /* tp_doc */
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    PyVec4d_methods,           /* tp_methods */
    PyVec4d_members,           /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)PyVec4d::init,   /* tp_init */
    PyType_GenericAlloc,       /* tp_alloc */
    (newfunc) PyType_GenericAlloc,            /* tp_new */
  };
  
  // Returns an Vec4d representation of the contents of o.
  Vec4d PyVec4d_AsVec4d( PyObject *o ) {
    if( PyVec4d_Check( o ) ) {
      return *(Vec4d *)(PyVec4d *)(o);
    } else {
      throw Exception::H3DAPIException( "PyObject * is not a PyVec4d *", 
                                        H3D_FULL_LOCATION );
    }
  }  

  // Creates a new PyVec4d object based on the value of v.
  PyObject *PyVec4d_FromVec4d( const Vec4d &v) {
    PyObject *o = PyType_GenericAlloc( &PyVec4d_Type, 1 );
    Vec4d *part = (Vec4d *)(PyVec4d *)( o );
    *part = v;
    return o;
  }

  int PyVec4d::init(PyVec4d *self, PyObject *args, PyObject *kwds)  {
    if( PyTuple_Size( args ) == 0 ) {
      Vec4d *self_v = (Vec4d *)self;
      *self_v = Vec4d();
    } else if( PyTuple_Size( args ) == 1 ) {
      PyObject *o = PyTuple_GetItem( args, 0 );
      if( PyVec4d_Check( o ) ) {
        Vec4d vd = PyVec4d_AsVec4d( o );
        self->x = vd.x;
        self->y = vd.y;
        self->z = vd.z;
        self->w = vd.w;
      } else if( PyVec4f_Check( o ) ) {
        Vec4f vd = PyVec4f_AsVec4f( o );
        self->x = vd.x;
        self->y = vd.y;
        self->z = vd.z;
        self->w = vd.w;
      } else {
        PyErr_SetString(PyExc_TypeError, 
                        "invalid type given to Vec4f constructor." );
        return -1;
      }
    } else {
      static char *kwlist[] = {"x", "y", "z", "w", NULL};
      if (! PyArg_ParseTupleAndKeywords(args, kwds, "dddd", kwlist, 
                                        &(self->x), &(self->y), 
                                        &(self->z), &(self->w)))
        return -1; 
    }
    return 0;
  }
  

  ///////////////////////////////////////////////////////////////////
  /// MATRIX3F
  /// 
  static PyMethodDef PyMatrix3f_methods[] = {
    { "__repr__", (PyCFunction) PyMatrix3f::repr, 0 },
    { "__str__", (PyCFunction) PyMatrix3f::repr, 0 },
    { "setToIdentity", (PyCFunction)PyMatrix3f::setToIdentity, 0 },
    { "inverse", (PyCFunction)PyMatrix3f::inverse, 0 },
    { "getRow", (PyCFunction)PyMatrix3f::getRow, 0 },
    { "getColumn", (PyCFunction)PyMatrix3f::getColumn, 0 },
    { "getElement", (PyCFunction)PyMatrix3f::getElement, 0 },    
    { "setElement", (PyCFunction)PyMatrix3f::setElement, 0 },
    { "getScalePart", (PyCFunction)PyMatrix3f::getScalePart, 0 },
    { "transpose", (PyCFunction)PyMatrix3f::transpose, 0 },
    { "toEulerAngles", (PyCFunction)PyMatrix3f::toEulerAngles, 0 },
    {NULL, NULL}
  };
  
  static PyMemberDef PyMatrix3f_members[] = {
    {NULL}  /* Sentinel */
  };
  
  
  static PyNumberMethods PyMatrix3f_as_number = {
    (binaryfunc)  PyMatrix3f::add,   /* nb_add */
    (binaryfunc)  PyMatrix3f::sub,   /* nb_subtract */
    (binaryfunc)  PyMatrix3f::mul,       /* nb_multiply */
    (binaryfunc)    PyMatrix3f::div,   /* nb_divide */
    (binaryfunc)    0,   /* nb_remainder */
    (binaryfunc)    0,          /* nb_divmod */
    (ternaryfunc)   0,          /* nb_power */
    (unaryfunc)     0,          /* nb_negative */
    (unaryfunc)     0,          /* tp_positive */
    (unaryfunc)     0,          /* tp_absolute */
    (inquiry)       0,          /* tp_nonzero */
    (unaryfunc)     0,          /* nb_invert */
    (binaryfunc)    0,          /* nb_lshift */
    (binaryfunc)    0,          /* nb_rshift */
    (binaryfunc)    0,          /* nb_and */
    (binaryfunc)    0,          /* nb_xor */
    (binaryfunc)    0,          /* nb_or */
    (coercion)      0,          /* nb_coerce */
    (unaryfunc)     0,          /* nb_int */
    (unaryfunc)     0,          /* nb_long */
    (unaryfunc)     0,          /* nb_float */
    (unaryfunc)     0,          /* nb_oct */
    (unaryfunc)   0,                  /* nb_hex */
      
    /* Added in release 2.0 */
    /* These require the Py_TPFLAGS_HAVE_INPLACEOPS flag */
    0,                      /* nb_inplace_add */
    0,                      /* nb_inplace_subtract */
    0,                      /* nb_inplace_multiply */
    0,                      /* nb_inplace_divide */
    0,                      /* nb_inplace_remainder */
    (ternaryfunc)0,         /* nb_inplace_power */
    0,                      /* nb_inplace_lshift */
    0,                      /* nb_inplace_rshift */
    0,                      /* nb_inplace_and */
    0,                      /* nb_inplace_xor */
    0,                      /* nb_inplace_or */
      
    /* Added in release 2.2 */
    /* These require the Py_TPFLAGS_HAVE_CLASS flag */
    (binaryfunc)    PyMatrix3f::div,  /* nb_floor_divide */
    (binaryfunc)    PyMatrix3f::div,  /* nb_true_divide */
    0,                      /* nb_inplace_floor_divide */
    0,                      /* nb_inplace_true_divide */
  };    
  PyTypeObject PyMatrix3f_Type = {
    PyObject_HEAD_INIT(NULL)
    0,
    "H3D.Matrix3f",
    sizeof(PyMatrix3f),
    0,                         /*tp_itemsize*/
    (destructor)PyMatrix3f::dealloc, /*tp_dealloc*/
    0,                         /*tp_print*/
    0,                         /*tp_getattr*/
    0,                         /*tp_setattr*/
    (cmpfunc) PyMatrix3f::compare,                         /*tp_compare*/
    (reprfunc) PyMatrix3f::repr,                         /*tp_repr*/
    &PyMatrix3f_as_number,                         /*tp_as_number*/
    0,                         /*tp_as_sequence*/
    0,                         /*tp_as_mapping*/
    0,                         /*tp_hash */
    0,                         /*tp_call*/
    0,                         /*tp_str*/
    0,                         /*tp_getattro*/
    0,                         /*tp_setattro*/
    0,                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | 
    Py_TPFLAGS_BASETYPE |
    Py_TPFLAGS_CHECKTYPES,     /*tp_flags*/
    "Matrix3f Object",         /* tp_doc */
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    PyMatrix3f_methods,        /* tp_methods */
    PyMatrix3f_members,        /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)PyMatrix3f::init,   /* tp_init */
    PyType_GenericAlloc,       /* tp_alloc */
    (newfunc) PyType_GenericAlloc,            /* tp_new */
  };
  
  // Returns an Matrix3f representation of the contents of o.
  Matrix3f PyMatrix3f_AsMatrix3f( PyObject *o ) {
    if( PyMatrix3f_Check( o ) ) {
      return *(Matrix3f *)(PyMatrix3f *)(o);
    } else {
      throw Exception::H3DAPIException( "PyObject * is not a PyMatrix3f *", 
                                        H3D_FULL_LOCATION );
    }
  }  

  PyObject* PyMatrix3f::getElement( PyObject *myself, PyObject *args ) {
    if( !args || !PyTuple_Check( args ) || PyTuple_Size( args ) != 2  ) {
        ostringstream err;
        err << "Invalid number of arguments  PyMatrix.getElement( i, j )";
        PyErr_SetString( PyExc_ValueError, err.str().c_str() );
        return 0;
      }
    Matrix3f m = PyMatrix3f_AsMatrix3f( myself );
    PyObject *i = PyTuple_GetItem( args, 0 );
    PyObject *j = PyTuple_GetItem( args, 1 );

    if( !PyInt_Check( i ) || !PyInt_Check( j ) ) {
      PyErr_SetString(PyExc_TypeError, 
          "invalid type given as argument to PyMatrix::getElement( i, j )." );
      return 0;
    }

    return PyFloat_FromDouble( m.getElement( PyInt_AsLong( i ), 
                                             PyInt_AsLong( j ) ) );
  }

  PyObject* PyMatrix3f::setElement( PyObject *myself, PyObject *args ) {
    if( !args || !PyTuple_Check( args ) || PyTuple_Size( args ) != 3  ) {
        ostringstream err;
        err << "Invalid number of arguments  PyMatrix.setElement( i, j, v )";
        PyErr_SetString( PyExc_ValueError, err.str().c_str() );
        return 0;
      }
    Matrix3f *m = (Matrix3f *)(PyMatrix3f *)(myself);
    PyObject *i = PyTuple_GetItem( args, 0 );
    PyObject *j = PyTuple_GetItem( args, 1 );
    PyObject *v = PyTuple_GetItem( args, 2 );

    if( !PyInt_Check( i ) || !PyInt_Check( j ) || 
        !(PyFloat_Check( v ) || PyInt_Check( v ) || PyLong_Check( v ) ) ) {
      PyErr_SetString(PyExc_TypeError, 
        "invalid type given as argument to PyMatrix::setElement( i, j, v )." );
      return 0;
    }

    long r = PyInt_AsLong( i );
    long c = PyInt_AsLong( j );
        
    if( PyFloat_Check( v ) ) {
      m->setElement( r, c, (H3DFloat)PyFloat_AsDouble( v ) );
      // int type
    } else if( PyInt_Check( v ) ) {
      m->setElement( r, c, (H3DFloat)PyInt_AsLong( v ) );
      // long type
    } else if( PyLong_Check( v ) ) {
      m->setElement( r, c, (H3DFloat) PyLong_AsLong( v ) );
    } 
    Py_INCREF( Py_None );
    return Py_None;
  }

  // Creates a new PyMatrix3f object based on the value of v.
  PyObject *PyMatrix3f_FromMatrix3f( const Matrix3f &v) {
    PyObject *o = PyType_GenericAlloc( &PyMatrix3f_Type, 1 );
    Matrix3f *part = (Matrix3f *)(PyMatrix3f *)( o );
    *part = v;
    return o;
  }
  
  int PyMatrix3f::init(PyMatrix3f *self, PyObject *args, PyObject *kwds)  {
    Py_ssize_t args_size =  PyTuple_Size( args );
    if( args_size == 0 ) {
      // no arguments, identify matrix
      self->setToIdentity( (PyObject *) self, args );
    } else if( args_size == 1 ) {
      // from Quaternion, Rotation and Matrix3f
      PyObject *o = PyTuple_GetItem( args, 0 );
      Matrix3f *self_m = (Matrix3f *)self;
      if( PyRotation_Check( o ) ) {
        Rotation r = PyRotation_AsRotation( o );
        *self_m = Matrix3f(r);
      } else if( PyQuaternion_Check( o ) ) {
        Quaternion q = PyQuaternion_AsQuaternion( o );
        *self_m = Matrix3f(q);
      } else if( PyMatrix3f_Check( o ) ) {
        Matrix3f m = PyMatrix3f_AsMatrix3f( o );
        *self_m = m;
      } else if( PyMatrix3d_Check( o ) ) {
        Matrix3d m = PyMatrix3d_AsMatrix3d( o );
        *self_m = (Matrix3f) m;
      } else {
        PyErr_SetString(PyExc_TypeError, 
                        "invalid type given to Matrix3f constructor." );
        return -1;
      }
    } else {
      // full matrix specification.
      if (! PyArg_ParseTuple(args, 
                             "fffffffff", 
                             &(*self)[0][0],
                             &(*self)[0][1],
                             &(*self)[0][2],
                             &(*self)[1][0],
                             &(*self)[1][1],
                             &(*self)[1][2],
                             &(*self)[2][0],
                             &(*self)[2][1],
                             &(*self)[2][2] ) )
        return -1; 
    }
    // success
    return 0;
  }
  
  PyObject* PyMatrix3f::mul( PyObject *a, PyObject *b ) {
    if( PyMatrix3f_Check( a ) ) {
      Matrix3f ma = PyMatrix3f_AsMatrix3f( a );
      if( PyMatrix3f_Check( b ) ) {
        Matrix3f mb = PyMatrix3f_AsMatrix3f( b );
        return PyMatrix3f_FromMatrix3f( ma * mb );
      } else if( PyMatrix3d_Check( b ) ) {
        Matrix3d mb = PyMatrix3d_AsMatrix3d( b );
        return PyMatrix3d_FromMatrix3d( Matrix3d(ma) * mb );
      } else if( PyVec3f_Check( b ) ) {
        Vec3f vb = PyVec3f_AsVec3f( b );
        return PyVec3f_FromVec3f( ma * vb );
      } else if( PyVec3d_Check( b ) ) {
        Vec3d vb = PyVec3d_AsVec3d( b );
        return PyVec3d_FromVec3d( ma * vb );
      }
    }

    return PyNumberTypeWrapper< Matrix3f, 
      &PyMatrix3f_Type,
      PyMatrix3f_Name,
      PyMatrix3f_Check,
      PyMatrix3f_AsMatrix3f, 
      PyMatrix3f_FromMatrix3f >::mul( a, b );
  }

  PyObject* PyMatrix3f::setToIdentity( PyObject *myself, PyObject *args ) {
    PyMatrix3f *v = (PyMatrix3f*)myself;
    Matrix3f *m = (Matrix3f*)v;
    m->setToIdentity();
    Py_INCREF( myself );
    return myself;
  }
  
  PyObject* PyMatrix3f::inverse( PyObject *myself, PyObject *args ) {
    Matrix3f m = PyMatrix3f_AsMatrix3f( myself );
    return PyMatrix3f_FromMatrix3f( m.inverse() );
  }

  PyObject* PyMatrix3f::transpose( PyObject *myself, PyObject *args ) {
    Matrix3f m = PyMatrix3f_AsMatrix3f( myself );
    return PyMatrix3f_FromMatrix3f( m.transpose() );
  }

  PyObject* PyMatrix3f::toEulerAngles( PyObject *myself, PyObject *args ) {
    Matrix3f m = PyMatrix3f_AsMatrix3f( myself );
    return PyVec3f_FromVec3f( m.toEulerAngles() );
  }

  PyObject* PyMatrix3f::getScalePart( PyObject *myself, PyObject *args ) {
    Matrix3f m = PyMatrix3f_AsMatrix3f( myself );
    return PyVec3f_FromVec3f( m.getScalePart() );
  }

  PyObject* PyMatrix3f::getRow( PyObject *myself, PyObject *args ) {
    if( args && PyInt_Check( args ) ) {
      int row = PyInt_AsLong( args );
      Matrix3f m = PyMatrix3f_AsMatrix3f( myself );
      return PyVec3f_FromVec3f( m.getRow( row ) );
    } 
    PyErr_SetString(PyExc_TypeError, 
                    "int type required as argument to getRow" );
    return NULL;
  }

  PyObject* PyMatrix3f::getColumn( PyObject *myself, PyObject *args ) {
    if( args && PyInt_Check( args ) ) {
      int col = PyInt_AsLong( args );
      Matrix3f m = PyMatrix3f_AsMatrix3f( myself );
      return PyVec3f_FromVec3f( m.getColumn( col ) );
    } 
    PyErr_SetString(PyExc_TypeError, 
                    "int type required as argument to getColumn" );
    return NULL;
  }

  ///////////////////////////////////////////////////////////////////
  /// MATRIX4F
  /// 
  static PyMethodDef PyMatrix4f_methods[] = {
    { "__repr__", (PyCFunction) PyMatrix4f::repr, 0 },
    { "__str__", (PyCFunction) PyMatrix4f::repr, 0 },
    { "setToIdentity", (PyCFunction)PyMatrix4f::setToIdentity, 0 },
    { "transformInverse", (PyCFunction)PyMatrix4f::transformInverse, 0 },
    { "inverse", (PyCFunction)PyMatrix4f::inverse, 0 },
    { "transpose", (PyCFunction)PyMatrix4f::transpose, 0 },
    { "getRow", (PyCFunction)PyMatrix4f::getRow, 0 },
    { "getColumn", (PyCFunction)PyMatrix4f::getColumn, 0 },
    { "getScaleRotationPart", 
     (PyCFunction)PyMatrix4f::getScaleRotationPart, 0 },
    { "getRotationPart", (PyCFunction)PyMatrix4f::getRotationPart, 0 },
    { "getScalePart", (PyCFunction)PyMatrix4f::getScalePart, 0 },
    { "getTranslationPart", (PyCFunction)PyMatrix4f::getTranslationPart, 0 },
    { "getElement", (PyCFunction)PyMatrix4f::getElement, 0 },    
    { "setElement", (PyCFunction)PyMatrix4f::setElement, 0 },
    {NULL, NULL}
  };
  
  static PyMemberDef PyMatrix4f_members[] = {
    {NULL}  /* Sentinel */
  };
  
  
  static PyNumberMethods PyMatrix4f_as_number = {
    (binaryfunc)  PyMatrix4f::add,   /* nb_add */
    (binaryfunc)  PyMatrix4f::sub,   /* nb_subtract */
    (binaryfunc)  PyMatrix4f::mul,       /* nb_multiply */
    (binaryfunc)    PyMatrix4f::div,   /* nb_divide */
    (binaryfunc)    0,   /* nb_remainder */
    (binaryfunc)    0,          /* nb_divmod */
    (ternaryfunc)   0,          /* nb_power */
    (unaryfunc)     0,          /* nb_negative */
    (unaryfunc)     0,          /* tp_positive */
    (unaryfunc)     0,          /* tp_absolute */
    (inquiry)       0,          /* tp_nonzero */
    (unaryfunc)     0,          /* nb_invert */
    (binaryfunc)    0,          /* nb_lshift */
    (binaryfunc)    0,          /* nb_rshift */
    (binaryfunc)    0,          /* nb_and */
    (binaryfunc)    0,          /* nb_xor */
    (binaryfunc)    0,          /* nb_or */
    (coercion)      0,          /* nb_coerce */
    (unaryfunc)     0,          /* nb_int */
    (unaryfunc)     0,          /* nb_long */
    (unaryfunc)     0,          /* nb_float */
    (unaryfunc)     0,          /* nb_oct */
    (unaryfunc)   0,                  /* nb_hex */
      
    /* Added in release 2.0 */
    /* These require the Py_TPFLAGS_HAVE_INPLACEOPS flag */
    0,                      /* nb_inplace_add */
    0,                      /* nb_inplace_subtract */
    0,                      /* nb_inplace_multiply */
    0,                      /* nb_inplace_divide */
    0,                      /* nb_inplace_remainder */
    (ternaryfunc)0,         /* nb_inplace_power */
    0,                      /* nb_inplace_lshift */
    0,                      /* nb_inplace_rshift */
    0,                      /* nb_inplace_and */
    0,                      /* nb_inplace_xor */
    0,                      /* nb_inplace_or */
      
    /* Added in release 2.2 */
    /* These require the Py_TPFLAGS_HAVE_CLASS flag */
    (binaryfunc)    PyMatrix4f::div,  /* nb_floor_divide */
    (binaryfunc)    PyMatrix4f::div,  /* nb_true_divide */
    0,                      /* nb_inplace_floor_divide */
    0,                      /* nb_inplace_true_divide */
  };    
  PyTypeObject PyMatrix4f_Type = {
    PyObject_HEAD_INIT(NULL)
    0,
    "H3D.Matrix4f",
    sizeof(PyMatrix4f),
    0,                         /*tp_itemsize*/
    (destructor)PyMatrix4f::dealloc, /*tp_dealloc*/
    0,                         /*tp_print*/
    0,                         /*tp_getattr*/
    0,                         /*tp_setattr*/
    (cmpfunc) PyMatrix4f::compare,                         /*tp_compare*/
    (reprfunc) PyMatrix4f::repr,                         /*tp_repr*/
    &PyMatrix4f_as_number,                         /*tp_as_number*/
    0,                         /*tp_as_sequence*/
    0,                         /*tp_as_mapping*/
    0,                         /*tp_hash */
    0,                         /*tp_call*/
    0,                         /*tp_str*/
    0,                         /*tp_getattro*/
    0,                         /*tp_setattro*/
    0,                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | 
    Py_TPFLAGS_BASETYPE |
    Py_TPFLAGS_CHECKTYPES,     /*tp_flags*/
    "Matrix4f Object",         /* tp_doc */
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    PyMatrix4f_methods,        /* tp_methods */
    PyMatrix4f_members,        /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)PyMatrix4f::init,   /* tp_init */
    PyType_GenericAlloc,       /* tp_alloc */
    (newfunc) PyType_GenericAlloc,            /* tp_new */
  };
  
  // Returns an Matrix4f representation of the contents of o.
  Matrix4f PyMatrix4f_AsMatrix4f( PyObject *o ) {
    if( PyMatrix4f_Check( o ) ) {
      return *(Matrix4f *)(PyMatrix4f *)(o);
    } else {
      throw Exception::H3DAPIException( "PyObject * is not a PyMatrix4f *", 
                                        H3D_FULL_LOCATION );
    }
  }  

  // Creates a new PyMatrix4f object based on the value of v.
  PyObject *PyMatrix4f_FromMatrix4f( const Matrix4f &v) {
    PyObject *o = PyType_GenericAlloc( &PyMatrix4f_Type, 1 );
    Matrix4f *part = (Matrix4f *)(PyMatrix4f *)( o );
    *part = v;
    return o;
  }
  
  int PyMatrix4f::init(PyMatrix4f *self, PyObject *args, PyObject *kwds)  {
    Matrix4f *self_m = (Matrix4f *)self;
    Py_ssize_t args_size =  PyTuple_Size( args );
    if( args_size == 0 ) {
      // no arguments, identify matrix
      self->setToIdentity( (PyObject *) self, args );
    } else if( args_size == 1 ) {
      // from Quaternion, Rotation and Matrix4f
      PyObject *o = PyTuple_GetItem( args, 0 );
      if( PyRotation_Check( o ) ) {
        Rotation r = PyRotation_AsRotation( o );
        *self_m = Matrix4f(r);
      } else if( PyQuaternion_Check( o ) ) {
        Quaternion q = PyQuaternion_AsQuaternion( o );
        *self_m = Matrix4f(q);
      } else if( PyMatrix4f_Check( o ) ) {
        Matrix4f m = PyMatrix4f_AsMatrix4f( o );
        *self_m = m;
      } else if( PyMatrix4d_Check( o ) ) {
        Matrix4d m = PyMatrix4d_AsMatrix4d( o );
        *self_m = (Matrix4f) m;
        // Create transform matrix from a position
      } else if ( PyVec3f_Check( o ) ) {
        Vec3f v = PyVec3f_AsVec3f ( o );
        *self_m = Matrix4f ( v );
      } else if ( PyVec3d_Check( o ) ) {
        Vec3d v = PyVec3d_AsVec3d ( o );
        *self_m = Matrix4f ( Matrix4d ( v ) );
      } else {
        PyErr_SetString(PyExc_TypeError, 
                        "invalid type given to Matrix4f constructor." );
        return -1;
      }
    } else if ( args_size == 2 ) {
      // Create transform matrix from position and rotation
      PyObject* pos= NULL;
      PyObject* rot= NULL;
      if ( !PyArg_ParseTuple ( args, "OO", &pos, &rot ) ) {
        return -1;
      }
      if ( PyVec3f_Check ( pos ) && PyRotation_Check ( rot ) ) {
        Vec3f v = PyVec3f_AsVec3f ( pos );
        Rotation r = PyRotation_AsRotation ( rot );
        *self_m= Matrix4f ( v, r );
      } else if ( PyVec3d_Check ( pos ) && PyRotation_Check ( rot ) ) {
        Vec3d v = PyVec3d_AsVec3d ( pos );
        Rotationd r = PyRotation_AsRotation ( rot );
        *self_m= Matrix4f ( Matrix4d ( v, r ) );
      } else {
        return -1;
      }
    } else if ( args_size == 3 ) {
      // Create transform matrix from position, rotation and scale
      PyObject* pos= NULL;
      PyObject* rot= NULL;
      PyObject* sca= NULL;
      if ( !PyArg_ParseTuple ( args, "OOO", &pos, &rot, &sca ) ) {
        return -1;
      }
      if ( PyVec3f_Check ( pos ) && PyRotation_Check ( rot ) && PyVec3f_Check ( sca ) ) {
        Vec3f v = PyVec3f_AsVec3f ( pos );
        Rotation r = PyRotation_AsRotation ( rot );
        Vec3f s = PyVec3f_AsVec3f ( sca );
        *self_m= Matrix4f ( v, r, s );
      } else if ( PyVec3d_Check ( pos ) && PyRotation_Check ( rot ) && PyVec3d_Check ( sca ) ) {
        Vec3d v = PyVec3d_AsVec3d ( pos );
        Rotationd r = PyRotation_AsRotation ( rot );
        Vec3d s = PyVec3d_AsVec3d ( sca );
        *self_m= Matrix4f ( Matrix4d ( v, r, s ) );
      } else {
        return -1;
      }
    } else {
      // full matrix specification.
      if (! PyArg_ParseTuple(args, 
                             "ffffffffffffffff", 
                             &(*self)[0][0],
                             &(*self)[0][1],
                             &(*self)[0][2],
                             &(*self)[0][3],
                             &(*self)[1][0],
                             &(*self)[1][1],
                             &(*self)[1][2],
                             &(*self)[1][3],
                             &(*self)[2][0],
                             &(*self)[2][1],
                             &(*self)[2][2],
                             &(*self)[2][3],
                             &(*self)[3][0],
                             &(*self)[3][1],
                             &(*self)[3][2],
                             &(*self)[3][3] ) )
        return -1; 
    }
    // success
    return 0;
  }
  
  
  PyObject* PyMatrix4f::mul( PyObject *a, PyObject *b ) {
    if( PyMatrix4f_Check( a ) ) {
      Matrix4f ma = PyMatrix4f_AsMatrix4f( a );
      if( PyMatrix4f_Check( b ) ) {
        Matrix4f mb = PyMatrix4f_AsMatrix4f( b );
        return PyMatrix4f_FromMatrix4f( ma * mb );
      } else if( PyVec4f_Check( b ) ) {
        Vec4f vb = PyVec4f_AsVec4f( b );
        return PyVec4f_FromVec4f( ma * vb );
      } else if( PyVec3f_Check( b ) ) {
        Vec3f vb = PyVec3f_AsVec3f( b );
        return PyVec3f_FromVec3f( ma * vb );
      } else if( PyMatrix4d_Check( b ) ) {
        Matrix4d mb = PyMatrix4d_AsMatrix4d( b );
        return PyMatrix4d_FromMatrix4d( ma * mb );
      } else if( PyVec4d_Check( b ) ) {
        Vec4d vb = PyVec4d_AsVec4d( b );
        return PyVec4d_FromVec4d( ma * vb );
      } else if( PyVec3d_Check( b ) ) {
        Vec3d vb = PyVec3d_AsVec3d( b );
        return PyVec3d_FromVec3d( ma * vb );
      }
    }

    return PyNumberTypeWrapper< Matrix4f, 
      &PyMatrix4f_Type,
      PyMatrix4f_Name,
      PyMatrix4f_Check,
      PyMatrix4f_AsMatrix4f, 
      PyMatrix4f_FromMatrix4f >::mul( a, b );
  }


  PyObject* PyMatrix4f::getElement( PyObject *myself, PyObject *args ) {
    if( !args || !PyTuple_Check( args ) || PyTuple_Size( args ) != 2  ) {
        ostringstream err;
        err << "Invalid number of arguments  PyMatrix.getElement( i, j )";
        PyErr_SetString( PyExc_ValueError, err.str().c_str() );
        return 0;
      }
    Matrix4f m = PyMatrix4f_AsMatrix4f( myself );
    PyObject *i = PyTuple_GetItem( args, 0 );
    PyObject *j = PyTuple_GetItem( args, 1 );

    if( !PyInt_Check( i ) || !PyInt_Check( j ) ) {
      PyErr_SetString(PyExc_TypeError, 
          "invalid type given as argument to PyMatrix::getElement( i, j )." );
      return 0;
    }

    return PyFloat_FromDouble( m.getElement( PyInt_AsLong( i ), 
                                             PyInt_AsLong( j ) ) );
  }

  PyObject* PyMatrix4f::setElement( PyObject *myself, PyObject *args ) {
    if( !args || !PyTuple_Check( args ) || PyTuple_Size( args ) != 3  ) {
        ostringstream err;
        err << "Invalid number of arguments  PyMatrix.setElement( i, j, v )";
        PyErr_SetString( PyExc_ValueError, err.str().c_str() );
        return 0;
      }
    Matrix4f *m = (Matrix4f *)(PyMatrix4f *)(myself);
    PyObject *i = PyTuple_GetItem( args, 0 );
    PyObject *j = PyTuple_GetItem( args, 1 );
    PyObject *v = PyTuple_GetItem( args, 2 );

    if( !PyInt_Check( i ) || !PyInt_Check( j ) || 
        !(PyFloat_Check( v ) || PyInt_Check( v ) || PyLong_Check( v ) ) ) {
      PyErr_SetString(PyExc_TypeError, 
           "invalid type given as argument to PyMatrix::getElement( i, j )." );
      return 0;
    }

    long r = PyInt_AsLong( i );
    long c = PyInt_AsLong( j );
        
    if( PyFloat_Check( v ) ) {
      m->setElement( r, c, (H3DFloat) PyFloat_AsDouble( v ) );
      // int type
    } else if( PyInt_Check( v ) ) {
      m->setElement( r, c, (H3DFloat) PyInt_AsLong( v ) );
      // long type
    } else if( PyLong_Check( v ) ) {
      m->setElement( r, c, (H3DFloat) PyLong_AsLong( v ) );
    } 
    Py_INCREF( Py_None );
    return Py_None;
  }

  PyObject* PyMatrix4f::setToIdentity( PyObject *myself, PyObject *args ) {
    PyMatrix4f *v = (PyMatrix4f*)myself;
    Matrix4f *m = (Matrix4f*)v;
    m->setToIdentity();
    Py_INCREF( myself );
    return myself;
  }
  
  PyObject* PyMatrix4f::transformInverse( PyObject *myself, PyObject *args ) {
    Matrix4f m = PyMatrix4f_AsMatrix4f( myself );
    return PyMatrix4f_FromMatrix4f( m.transformInverse() );
  }

  PyObject* PyMatrix4f::inverse( PyObject *myself, PyObject *args ) {
    Matrix4f m = PyMatrix4f_AsMatrix4f( myself );
    return PyMatrix4f_FromMatrix4f( m.inverse() );
  }

  PyObject* PyMatrix4f::transpose( PyObject *myself, PyObject *args ) {
    Matrix4f m = PyMatrix4f_AsMatrix4f( myself );
    return PyMatrix4f_FromMatrix4f( m.transpose() );
  }

  PyObject* PyMatrix4f::getScalePart( PyObject *myself, PyObject *args ) {
    Matrix4f m = PyMatrix4f_AsMatrix4f( myself );
    return PyVec3f_FromVec3f( m.getScalePart() );
  }

  PyObject* PyMatrix4f::getTranslationPart( PyObject *myself, PyObject *args ) {
    Matrix4f m = PyMatrix4f_AsMatrix4f( myself );
    return PyVec3f_FromVec3f( m.getTranslationPart() );
  }

  PyObject* PyMatrix4f::getRow( PyObject *myself, PyObject *args ) {
    if( args && PyInt_Check( args ) ) {
      int row = PyInt_AsLong( args );
      Matrix4f m = PyMatrix4f_AsMatrix4f( myself );
      return PyVec4f_FromVec4f( m.getRow( row ) );
    } 
    PyErr_SetString(PyExc_TypeError, 
                    "int type required as argument to getRow" );
    return NULL;
  }

  PyObject* PyMatrix4f::getColumn( PyObject *myself, PyObject *args ) {
    if( args && PyInt_Check( args ) ) {
      int col = PyInt_AsLong( args );
      Matrix4f m = PyMatrix4f_AsMatrix4f( myself );
      return PyVec4f_FromVec4f( m.getColumn( col ) );
    } 
    PyErr_SetString(PyExc_TypeError, 
                    "int type required as argument to getColumn" );
    return NULL;
  }

   PyObject* PyMatrix4f::getScaleRotationPart( PyObject *myself, 
                                               PyObject *args ) {
     Matrix4f m = PyMatrix4f_AsMatrix4f( myself );
     return PyMatrix3f_FromMatrix3f( m.getScaleRotationPart() );
   }

   PyObject* PyMatrix4f::getRotationPart( PyObject *myself, 
                                          PyObject *args ) {
     Matrix4f m = PyMatrix4f_AsMatrix4f( myself );
     return PyMatrix3f_FromMatrix3f( m.getRotationPart() );
   }


  ///////////////////////////////////////////////////////////////////
  /// MATRIX3D
  /// 

  static PyMethodDef PyMatrix3d_methods[] = {
    { "__repr__", (PyCFunction) PyMatrix3d::repr, 0 },
    { "__str__", (PyCFunction) PyMatrix3d::repr, 0 },
    { "setToIdentity", (PyCFunction)PyMatrix3d::setToIdentity, 0 },
    { "inverse", (PyCFunction)PyMatrix3d::inverse, 0 },
    { "getRow", (PyCFunction)PyMatrix3d::getRow, 0 },
    { "getColumn", (PyCFunction)PyMatrix3d::getColumn, 0 },
    { "getScalePart", (PyCFunction)PyMatrix3d::getScalePart, 0 },
    { "transpose", (PyCFunction)PyMatrix3d::transpose, 0 },
    { "toEulerAngles", (PyCFunction)PyMatrix3d::toEulerAngles, 0 },
    { "getElement", (PyCFunction)PyMatrix3d::getElement, 0 },    
    { "setElement", (PyCFunction)PyMatrix3d::setElement, 0 },
    {NULL, NULL}
  };
  
  static PyMemberDef PyMatrix3d_members[] = {
    {NULL}  /* Sentinel */
  };
  
  
  static PyNumberMethods PyMatrix3d_as_number = {
    (binaryfunc)  PyMatrix3d::add,   /* nb_add */
    (binaryfunc)  PyMatrix3d::sub,   /* nb_subtract */
    (binaryfunc)  PyMatrix3d::mul,       /* nb_multiply */
    (binaryfunc)    PyMatrix3d::div,   /* nb_divide */
    (binaryfunc)    0,   /* nb_remainder */
    (binaryfunc)    0,          /* nb_divmod */
    (ternaryfunc)   0,          /* nb_power */
    (unaryfunc)     0,          /* nb_negative */
    (unaryfunc)     0,          /* tp_positive */
    (unaryfunc)     0,          /* tp_absolute */
    (inquiry)       0,          /* tp_nonzero */
    (unaryfunc)     0,          /* nb_invert */
    (binaryfunc)    0,          /* nb_lshift */
    (binaryfunc)    0,          /* nb_rshift */
    (binaryfunc)    0,          /* nb_and */
    (binaryfunc)    0,          /* nb_xor */
    (binaryfunc)    0,          /* nb_or */
    (coercion)      0,          /* nb_coerce */
    (unaryfunc)     0,          /* nb_int */
    (unaryfunc)     0,          /* nb_long */
    (unaryfunc)     0,          /* nb_float */
    (unaryfunc)     0,          /* nb_oct */
    (unaryfunc)   0,                  /* nb_hex */
      
    /* Added in release 2.0 */
    /* These require the Py_TPFLAGS_HAVE_INPLACEOPS flag */
    0,                      /* nb_inplace_add */
    0,                      /* nb_inplace_subtract */
    0,                      /* nb_inplace_multiply */
    0,                      /* nb_inplace_divide */
    0,                      /* nb_inplace_remainder */
    (ternaryfunc)0,         /* nb_inplace_power */
    0,                      /* nb_inplace_lshift */
    0,                      /* nb_inplace_rshift */
    0,                      /* nb_inplace_and */
    0,                      /* nb_inplace_xor */
    0,                      /* nb_inplace_or */
      
    /* Added in release 2.2 */
    /* These require the Py_TPFLAGS_HAVE_CLASS flag */
    (binaryfunc)    PyMatrix3d::div,  /* nb_floor_divide */
    (binaryfunc)    PyMatrix3d::div,  /* nb_true_divide */
    0,                      /* nb_inplace_floor_divide */
    0,                      /* nb_inplace_true_divide */
  };    
  PyTypeObject PyMatrix3d_Type = {
    PyObject_HEAD_INIT(NULL)
    0,
    "H3D.Matrix3d",
    sizeof(PyMatrix3d),
    0,                         /*tp_itemsize*/
    (destructor)PyMatrix3d::dealloc, /*tp_dealloc*/
    0,                         /*tp_print*/
    0,                         /*tp_getattr*/
    0,                         /*tp_setattr*/
    (cmpfunc) PyMatrix3d::compare,                         /*tp_compare*/
    (reprfunc) PyMatrix3d::repr,                         /*tp_repr*/
    &PyMatrix3d_as_number,                         /*tp_as_number*/
    0,                         /*tp_as_sequence*/
    0,                         /*tp_as_mapping*/
    0,                         /*tp_hash */
    0,                         /*tp_call*/
    0,                         /*tp_str*/
    0,                         /*tp_getattro*/
    0,                         /*tp_setattro*/
    0,                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | 
    Py_TPFLAGS_BASETYPE |
    Py_TPFLAGS_CHECKTYPES,     /*tp_flags*/
    "Matrix3d Object",         /* tp_doc */
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    PyMatrix3d_methods,        /* tp_methods */
    PyMatrix3d_members,        /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)PyMatrix3d::init,   /* tp_init */
    PyType_GenericAlloc,       /* tp_alloc */
    (newfunc) PyType_GenericAlloc,            /* tp_new */
  };
  
  // Returns an Matrix3d representation of the contents of o.
  Matrix3d PyMatrix3d_AsMatrix3d( PyObject *o ) {
    if( PyMatrix3d_Check( o ) ) {
      return *(Matrix3d *)(PyMatrix3d *)(o);
    } else {
      throw Exception::H3DAPIException( "PyObject * is not a PyMatrix3d *", 
                                        H3D_FULL_LOCATION );
    }
  }  

  // Creates a new PyMatrix3d object based on the value of v.
  PyObject *PyMatrix3d_FromMatrix3d( const Matrix3d &v) {
    PyObject *o = PyType_GenericAlloc( &PyMatrix3d_Type, 1 );
    Matrix3d *part = (Matrix3d *)(PyMatrix3d *)( o );
    *part = v;
    return o;
  }
  
  int PyMatrix3d::init(PyMatrix3d *self, PyObject *args, PyObject *kwds)  {
    Py_ssize_t args_size =  PyTuple_Size( args );
    if( args_size == 0 ) {
      // no arguments, identify matrix
      self->setToIdentity( (PyObject *) self, args );
    } else if( args_size == 1 ) {
      // from Quaternion, Rotation and Matrix3d
      PyObject *o = PyTuple_GetItem( args, 0 );
      Matrix3d *self_m = (Matrix3d *)self;
      if( PyRotation_Check( o ) ) {
        Rotation r = PyRotation_AsRotation( o );
        *self_m = Matrix3d(r);
      } else if( PyQuaternion_Check( o ) ) {
        Quaternion q = PyQuaternion_AsQuaternion( o );
        *self_m = Matrix3d(q);
      } else if( PyMatrix3d_Check( o ) ) {
        Matrix3d m = PyMatrix3d_AsMatrix3d( o );
        *self_m = m;
      } else if( PyMatrix3f_Check( o ) ) {
        Matrix3f m = PyMatrix3f_AsMatrix3f( o );
        *self_m = Matrix3d(m);
      } else {
        PyErr_SetString(PyExc_TypeError, 
                        "invalid type given to Matrix3d constructor." );
        return -1;
      }
    } else {
      // full matrix specification.
      if (! PyArg_ParseTuple(args, 
                             "ddddddddd", 
                             &(*self)[0][0],
                             &(*self)[0][1],
                             &(*self)[0][2],
                             &(*self)[1][0],
                             &(*self)[1][1],
                             &(*self)[1][2],
                             &(*self)[2][0],
                             &(*self)[2][1],
                             &(*self)[2][2] ) )
        return -1; 
    }
    // success
    return 0;
  }
  
  PyObject* PyMatrix3d::mul( PyObject *a, PyObject *b ) {
    if( PyMatrix3d_Check( a ) ) {
      Matrix3d ma = PyMatrix3d_AsMatrix3d( a );
      if( PyMatrix3d_Check( b ) ) {
        Matrix3d mb = PyMatrix3d_AsMatrix3d( b );
        return PyMatrix3d_FromMatrix3d( ma * mb );
      } else if( PyMatrix3f_Check( b ) ) {
        Matrix3f mb = PyMatrix3f_AsMatrix3f( b );
        return PyMatrix3d_FromMatrix3d( ma * mb );
      } else if( PyVec3f_Check( b ) ) {
        Vec3f vb = PyVec3f_AsVec3f( b );
        return PyVec3d_FromVec3d( ma * vb );
      } else if( PyVec3d_Check( b ) ) {
        Vec3d vb = PyVec3d_AsVec3d( b );
        return PyVec3d_FromVec3d( ma * vb );
      }
    }

    return PyNumberTypeWrapper< Matrix3d, 
      &PyMatrix3d_Type,
      PyMatrix3d_Name,
      PyMatrix3d_Check,
      PyMatrix3d_AsMatrix3d, 
      PyMatrix3d_FromMatrix3d >::mul( a, b );
  }

  PyObject* PyMatrix3d::getElement( PyObject *myself, PyObject *args ) {
    if( !args || !PyTuple_Check( args ) || PyTuple_Size( args ) != 2  ) {
        ostringstream err;
        err << "Invalid number of arguments  PyMatrix.getElement( i, j )";
        PyErr_SetString( PyExc_ValueError, err.str().c_str() );
        return 0;
      }
    Matrix3d m = PyMatrix3d_AsMatrix3d( myself );
    PyObject *i = PyTuple_GetItem( args, 0 );
    PyObject *j = PyTuple_GetItem( args, 1 );

    if( !PyInt_Check( i ) || !PyInt_Check( j ) ) {
      PyErr_SetString(PyExc_TypeError, 
          "invalid type given as argument to PyMatrix::getElement( i, j )." );
      return 0;
    }

    return PyFloat_FromDouble( m.getElement( PyInt_AsLong( i ), 
                                             PyInt_AsLong( j ) ) );
  }

  PyObject* PyMatrix3d::setElement( PyObject *myself, PyObject *args ) {
    if( !args || !PyTuple_Check( args ) || PyTuple_Size( args ) != 3  ) {
        ostringstream err;
        err << "Invalid number of arguments  PyMatrix.setElement( i, j, v )";
        PyErr_SetString( PyExc_ValueError, err.str().c_str() );
        return 0;
      }
    Matrix3d *m = (Matrix3d *)(PyMatrix3d *)(myself);
    PyObject *i = PyTuple_GetItem( args, 0 );
    PyObject *j = PyTuple_GetItem( args, 1 );
    PyObject *v = PyTuple_GetItem( args, 2 );

    if( !PyInt_Check( i ) || !PyInt_Check( j ) || 
        !(PyFloat_Check( v ) || PyInt_Check( v ) || PyLong_Check( v ) ) ) {
      PyErr_SetString(PyExc_TypeError, 
           "invalid type given as argument to PyMatrix::getElement( i, j )." );
      return 0;
    }

    long r = PyInt_AsLong( i );
    long c = PyInt_AsLong( j );
        
    if( PyFloat_Check( v ) ) {
      m->setElement( r, c, PyFloat_AsDouble( v ) );
      // int type
    } else if( PyInt_Check( v ) ) {
      m->setElement( r, c, PyInt_AsLong( v ) );
      // long type
    } else if( PyLong_Check( v ) ) {
      m->setElement( r, c, PyLong_AsLong( v ) );
    } 
    Py_INCREF( Py_None );
    return Py_None;
  }

  PyObject* PyMatrix3d::setToIdentity( PyObject *myself, PyObject *args ) {
    PyMatrix3d *v = (PyMatrix3d*)myself;
    Matrix3d *m = (Matrix3d*)v;
    m->setToIdentity();
    Py_INCREF( myself );
    return myself;
  }
  
  PyObject* PyMatrix3d::inverse( PyObject *myself, PyObject *args ) {
    Matrix3d m = PyMatrix3d_AsMatrix3d( myself );
    return PyMatrix3d_FromMatrix3d( m.inverse() );
  }

  PyObject* PyMatrix3d::transpose( PyObject *myself, PyObject *args ) {
    Matrix3d m = PyMatrix3d_AsMatrix3d( myself );
    return PyMatrix3d_FromMatrix3d( m.transpose() );
  }

  PyObject* PyMatrix3d::toEulerAngles( PyObject *myself, PyObject *args ) {
    Matrix3d m = PyMatrix3d_AsMatrix3d( myself );
    return PyVec3d_FromVec3d( m.toEulerAngles() );
  }

  PyObject* PyMatrix3d::getScalePart( PyObject *myself, PyObject *args ) {
    Matrix3d m = PyMatrix3d_AsMatrix3d( myself );
    return PyVec3d_FromVec3d( m.getScalePart() );
  }

  PyObject* PyMatrix3d::getRow( PyObject *myself, PyObject *args ) {
    if( args && PyInt_Check( args ) ) {
      int row = PyInt_AsLong( args );
      Matrix3d m = PyMatrix3d_AsMatrix3d( myself );
      return PyVec3d_FromVec3d( m.getRow( row ) );
    } 
    PyErr_SetString(PyExc_TypeError, 
                    "int type required as argument to getRow" );
    return NULL;
  }

  PyObject* PyMatrix3d::getColumn( PyObject *myself, PyObject *args ) {
    if( args && PyInt_Check( args ) ) {
      int col = PyInt_AsLong( args );
      Matrix3d m = PyMatrix3d_AsMatrix3d( myself );
      return PyVec3d_FromVec3d( m.getColumn( col ) );
    } 
    PyErr_SetString(PyExc_TypeError, 
                    "int type required as argument to getColumn" );
    return NULL;
  }

  ///////////////////////////////////////////////////////////////////
  /// MATRIX4D
  /// 
  static PyMethodDef PyMatrix4d_methods[] = {
    { "__repr__", (PyCFunction) PyMatrix4d::repr, 0 },
    { "__str__", (PyCFunction) PyMatrix4d::repr, 0 },
    { "setToIdentity", (PyCFunction)PyMatrix4d::setToIdentity, 0 },
    { "transformInverse", (PyCFunction)PyMatrix4d::transformInverse, 0 },
    { "inverse", (PyCFunction)PyMatrix4d::inverse, 0 },
    { "transpose", (PyCFunction)PyMatrix4d::transpose, 0 },
    { "getRow", (PyCFunction)PyMatrix4d::getRow, 0 },
    { "getColumn", (PyCFunction)PyMatrix4d::getColumn, 0 },
    { "getScaleRotationPart",
      (PyCFunction)PyMatrix4d::getScaleRotationPart, 0 },
    { "getRotationPart", (PyCFunction)PyMatrix4d::getRotationPart, 0 },
    { "getScalePart", (PyCFunction)PyMatrix4d::getScalePart, 0 },
    { "getTranslationPart", (PyCFunction)PyMatrix4d::getTranslationPart, 0 },
    { "getElement", (PyCFunction)PyMatrix4d::getElement, 0 },    
    { "setElement", (PyCFunction)PyMatrix4d::setElement, 0 },
    {NULL, NULL}
  };
  
  static PyMemberDef PyMatrix4d_members[] = {
    {NULL}  /* Sentinel */
  };
  
  
  static PyNumberMethods PyMatrix4d_as_number = {
    (binaryfunc)  PyMatrix4d::add,   /* nb_add */
    (binaryfunc)  PyMatrix4d::sub,   /* nb_subtract */
    (binaryfunc)  PyMatrix4d::mul,       /* nb_multiply */
    (binaryfunc)    PyMatrix4d::div,   /* nb_divide */
    (binaryfunc)    0,   /* nb_remainder */
    (binaryfunc)    0,          /* nb_divmod */
    (ternaryfunc)   0,          /* nb_power */
    (unaryfunc)     0,          /* nb_negative */
    (unaryfunc)     0,          /* tp_positive */
    (unaryfunc)     0,          /* tp_absolute */
    (inquiry)       0,          /* tp_nonzero */
    (unaryfunc)     0,          /* nb_invert */
    (binaryfunc)    0,          /* nb_lshift */
    (binaryfunc)    0,          /* nb_rshift */
    (binaryfunc)    0,          /* nb_and */
    (binaryfunc)    0,          /* nb_xor */
    (binaryfunc)    0,          /* nb_or */
    (coercion)      0,          /* nb_coerce */
    (unaryfunc)     0,          /* nb_int */
    (unaryfunc)     0,          /* nb_long */
    (unaryfunc)     0,          /* nb_float */
    (unaryfunc)     0,          /* nb_oct */
    (unaryfunc)   0,                  /* nb_hex */
      
    /* Added in release 2.0 */
    /* These require the Py_TPFLAGS_HAVE_INPLACEOPS flag */
    0,                      /* nb_inplace_add */
    0,                      /* nb_inplace_subtract */
    0,                      /* nb_inplace_multiply */
    0,                      /* nb_inplace_divide */
    0,                      /* nb_inplace_remainder */
    (ternaryfunc)0,         /* nb_inplace_power */
    0,                      /* nb_inplace_lshift */
    0,                      /* nb_inplace_rshift */
    0,                      /* nb_inplace_and */
    0,                      /* nb_inplace_xor */
    0,                      /* nb_inplace_or */
      
    /* Added in release 2.2 */
    /* These require the Py_TPFLAGS_HAVE_CLASS flag */
    (binaryfunc)    PyMatrix4d::div,  /* nb_floor_divide */
    (binaryfunc)    PyMatrix4d::div,  /* nb_true_divide */
    0,                      /* nb_inplace_floor_divide */
    0,                      /* nb_inplace_true_divide */
  };    
  PyTypeObject PyMatrix4d_Type = {
    PyObject_HEAD_INIT(NULL)
    0,
    "H3D.Matrix4d",
    sizeof(PyMatrix4d),
    0,                         /*tp_itemsize*/
    (destructor)PyMatrix4d::dealloc, /*tp_dealloc*/
    0,                         /*tp_print*/
    0,                         /*tp_getattr*/
    0,                         /*tp_setattr*/
    (cmpfunc) PyMatrix4d::compare,                         /*tp_compare*/
    (reprfunc) PyMatrix4d::repr,                         /*tp_repr*/
    &PyMatrix4d_as_number,                         /*tp_as_number*/
    0,                         /*tp_as_sequence*/
    0,                         /*tp_as_mapping*/
    0,                         /*tp_hash */
    0,                         /*tp_call*/
    0,                         /*tp_str*/
    0,                         /*tp_getattro*/
    0,                         /*tp_setattro*/
    0,                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | 
    Py_TPFLAGS_BASETYPE |
    Py_TPFLAGS_CHECKTYPES,     /*tp_flags*/
    "Matrix4d Object",         /* tp_doc */
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    PyMatrix4d_methods,        /* tp_methods */
    PyMatrix4d_members,        /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)PyMatrix4d::init,   /* tp_init */
    PyType_GenericAlloc,       /* tp_alloc */
    (newfunc) PyType_GenericAlloc,            /* tp_new */
  };
  
  // Returns an Matrix4d representation of the contents of o.
  Matrix4d PyMatrix4d_AsMatrix4d( PyObject *o ) {
    if( PyMatrix4d_Check( o ) ) {
      return *(Matrix4d *)(PyMatrix4d *)(o);
    } else {
      throw Exception::H3DAPIException( "PyObject * is not a PyMatrix4d *", 
                                        H3D_FULL_LOCATION );
    }
  }  

  // Creates a new PyMatrix4d object based on the value of v.
  PyObject *PyMatrix4d_FromMatrix4d( const Matrix4d &v) {
    PyObject *o = PyType_GenericAlloc( &PyMatrix4d_Type, 1 );
    Matrix4d *part = (Matrix4d *)(PyMatrix4d *)( o );
    *part = v;
    return o;
  }
  
  int PyMatrix4d::init(PyMatrix4d *self, PyObject *args, PyObject *kwds)  {
    Matrix4d *self_m = (Matrix4d *)self;
    Py_ssize_t args_size =  PyTuple_Size( args );
    if( args_size == 0 ) {
      // no arguments, identify matrix
      self->setToIdentity( (PyObject *) self, args );
    } else if( args_size == 1 ) {
      // from Quaternion, Rotation and Matrix4d
      PyObject *o = PyTuple_GetItem( args, 0 );
      if( PyRotation_Check( o ) ) {
        Rotation r = PyRotation_AsRotation( o );
        *self_m = Matrix4d(r);
      } else if( PyQuaternion_Check( o ) ) {
        Quaternion q = PyQuaternion_AsQuaternion( o );
        *self_m = Matrix4d(q);
      } else if( PyMatrix4d_Check( o ) ) {
        Matrix4d m = PyMatrix4d_AsMatrix4d( o );
        *self_m = m;
      } else if( PyMatrix4f_Check( o ) ) {
        Matrix4f m = PyMatrix4f_AsMatrix4f( o );
        *self_m = Matrix4d(m);
      // Create transform matrix from a position
      } else if ( PyVec3f_Check( o ) ) {
        Vec3f v = PyVec3f_AsVec3f ( o );
        *self_m = Matrix4d ( Matrix4f ( v ) );
      } else if ( PyVec3d_Check( o ) ) {
        Vec3d v = PyVec3d_AsVec3d ( o );
        *self_m = Matrix4d ( v );
      } else {
        PyErr_SetString(PyExc_TypeError, 
                        "invalid type given to Matrix4d constructor." );
        return -1;
      }
    } else if ( args_size == 2 ) {
      // Create transform matrix from position and rotation
      PyObject* pos= NULL;
      PyObject* rot= NULL;
      if ( !PyArg_ParseTuple ( args, "OO", &pos, &rot ) ) {
        return -1;
      }
      if ( PyVec3f_Check ( pos ) && PyRotation_Check ( rot ) ) {
        Vec3f v = PyVec3f_AsVec3f ( pos );
        Rotation r = PyRotation_AsRotation ( rot );
        *self_m= Matrix4f ( v, r );
      } else if ( PyVec3d_Check ( pos ) && PyRotation_Check ( rot ) ) {
        Vec3d v = PyVec3d_AsVec3d ( pos );
        Rotationd r = PyRotation_AsRotation ( rot );
        *self_m= Matrix4f ( Matrix4d ( v, r ) );
      } else {
        return -1;
      }
    } else if ( args_size == 3 ) {
      // Create transform matrix from position, rotation and scale
      PyObject* pos= NULL;
      PyObject* rot= NULL;
      PyObject* sca= NULL;
      if ( !PyArg_ParseTuple ( args, "OOO", &pos, &rot, &sca ) ) {
        return -1;
      }
      if ( PyVec3f_Check ( pos ) && PyRotation_Check ( rot ) && PyVec3f_Check ( sca ) ) {
        Vec3f v = PyVec3f_AsVec3f ( pos );
        Rotation r = PyRotation_AsRotation ( rot );
        Vec3f s = PyVec3f_AsVec3f ( sca );
        *self_m= Matrix4d ( Matrix4f ( v, r, s ) );
      } else if ( PyVec3d_Check ( pos ) && PyRotation_Check ( rot ) && PyVec3d_Check ( sca ) ) {
        Vec3d v = PyVec3d_AsVec3d ( pos );
        Rotationd r = PyRotation_AsRotation ( rot );
        Vec3d s = PyVec3d_AsVec3d ( sca );
        *self_m= Matrix4d ( v, r, s );
      } else {
        return -1;
      }
    } else {
      // full matrix specification.
      if (! PyArg_ParseTuple(args, 
                             "dddddddddddddddd", 
                             &(*self)[0][0],
                             &(*self)[0][1],
                             &(*self)[0][2],
                             &(*self)[0][3],
                             &(*self)[1][0],
                             &(*self)[1][1],
                             &(*self)[1][2],
                             &(*self)[1][3],
                             &(*self)[2][0],
                             &(*self)[2][1],
                             &(*self)[2][2],
                             &(*self)[2][3],
                             &(*self)[3][0],
                             &(*self)[3][1],
                             &(*self)[3][2],
                             &(*self)[3][3] ) )
        return -1; 
    }
    // success
    return 0;
  }
  
  
  PyObject* PyMatrix4d::mul( PyObject *a, PyObject *b ) {
    if( PyMatrix4d_Check( a ) ) {
      Matrix4d ma = PyMatrix4d_AsMatrix4d( a );
      if( PyMatrix4d_Check( b ) ) {
        Matrix4d mb = PyMatrix4d_AsMatrix4d( b );
        return PyMatrix4d_FromMatrix4d( ma * mb );
      } else if( PyVec4d_Check( b ) ) {
        Vec4d vb = PyVec4d_AsVec4d( b );
        return PyVec4d_FromVec4d( ma * vb );
      } else if( PyVec3d_Check( b ) ) {
        Vec3d vb = PyVec3d_AsVec3d( b );
        return PyVec3d_FromVec3d( ma * vb );
      } else if( PyMatrix4f_Check( b ) ) {
        Matrix4f mb = PyMatrix4f_AsMatrix4f( b );
        return PyMatrix4d_FromMatrix4d( ma * mb );
      } else if( PyVec4f_Check( b ) ) {
        Vec4f vb = PyVec4f_AsVec4f( b );
        return PyVec4d_FromVec4d( ma * vb );
      } else if( PyVec3f_Check( b ) ) {
        Vec3f vb = PyVec3f_AsVec3f( b );
        return PyVec3d_FromVec3d( ma * vb );
      }
    }

    return PyNumberTypeWrapper< Matrix4d, 
      &PyMatrix4d_Type,
      PyMatrix4d_Name,
      PyMatrix4d_Check,
      PyMatrix4d_AsMatrix4d, 
      PyMatrix4d_FromMatrix4d >::mul( a, b );
  }

  PyObject* PyMatrix4d::getElement( PyObject *myself, PyObject *args ) {
    if( !args || !PyTuple_Check( args ) || PyTuple_Size( args ) != 2  ) {
        ostringstream err;
        err << "Invalid number of arguments  PyMatrix.getElement( i, j )";
        PyErr_SetString( PyExc_ValueError, err.str().c_str() );
        return 0;
      }
    Matrix4d m = PyMatrix4d_AsMatrix4d( myself );
    PyObject *i = PyTuple_GetItem( args, 0 );
    PyObject *j = PyTuple_GetItem( args, 1 );

    if( !PyInt_Check( i ) || !PyInt_Check( j ) ) {
      PyErr_SetString(PyExc_TypeError, 
          "invalid type given as argument to PyMatrix::getElement( i, j )." );
      return 0;
    }

    return PyFloat_FromDouble( m.getElement( PyInt_AsLong( i ), 
                                             PyInt_AsLong( j ) ) );
  }

  PyObject* PyMatrix4d::setElement( PyObject *myself, PyObject *args ) {
    if( !args || !PyTuple_Check( args ) || PyTuple_Size( args ) != 3  ) {
        ostringstream err;
        err << "Invalid number of arguments  PyMatrix.setElement( i, j, v )";
        PyErr_SetString( PyExc_ValueError, err.str().c_str() );
        return 0;
      }
    Matrix4d *m = (Matrix4d *)(PyMatrix4d *)(myself);
    PyObject *i = PyTuple_GetItem( args, 0 );
    PyObject *j = PyTuple_GetItem( args, 1 );
    PyObject *v = PyTuple_GetItem( args, 2 );

    if( !PyInt_Check( i ) || !PyInt_Check( j ) || 
        !(PyFloat_Check( v ) || PyInt_Check( v ) || PyLong_Check( v ) ) ) {
      PyErr_SetString(PyExc_TypeError, 
           "invalid type given as argument to PyMatrix::getElement( i, j )." );
      return 0;
    }

    long r = PyInt_AsLong( i );
    long c = PyInt_AsLong( j );
        
    if( PyFloat_Check( v ) ) {
      m->setElement( r, c, PyFloat_AsDouble( v ) );
      // int type
    } else if( PyInt_Check( v ) ) {
      m->setElement( r, c, PyInt_AsLong( v ) );
      // long type
    } else if( PyLong_Check( v ) ) {
      m->setElement( r, c, PyLong_AsLong( v ) );
    }  
    Py_INCREF( Py_None );
    return Py_None;
  }

  PyObject* PyMatrix4d::setToIdentity( PyObject *myself, PyObject *args ) {
    PyMatrix4d *v = (PyMatrix4d*)myself;
    Matrix4d *m = (Matrix4d*)v;
    m->setToIdentity();
    Py_INCREF( myself );
    return myself;
  }
  
  PyObject* PyMatrix4d::transformInverse( PyObject *myself, PyObject *args ) {
    Matrix4d m = PyMatrix4d_AsMatrix4d( myself );
    return PyMatrix4d_FromMatrix4d( m.transformInverse() );
  }

  PyObject* PyMatrix4d::inverse( PyObject *myself, PyObject *args ) {
    Matrix4d m = PyMatrix4d_AsMatrix4d( myself );
    return PyMatrix4d_FromMatrix4d( m.inverse() );
  }

  PyObject* PyMatrix4d::transpose( PyObject *myself, PyObject *args ) {
    Matrix4d m = PyMatrix4d_AsMatrix4d( myself );
    return PyMatrix4d_FromMatrix4d( m.transpose() );
  }

  PyObject* PyMatrix4d::getScalePart( PyObject *myself, PyObject *args ) {
    Matrix4d m = PyMatrix4d_AsMatrix4d( myself );
    return PyVec3d_FromVec3d( m.getScalePart() );
  }

  PyObject* PyMatrix4d::getTranslationPart( PyObject *myself, PyObject *args ) {
    Matrix4d m = PyMatrix4d_AsMatrix4d( myself );
    return PyVec3d_FromVec3d( m.getTranslationPart() );
  }

  PyObject* PyMatrix4d::getRow( PyObject *myself, PyObject *args ) {
    if( args && PyInt_Check( args ) ) {
      int row = PyInt_AsLong( args );
      Matrix4d m = PyMatrix4d_AsMatrix4d( myself );
      return PyVec4d_FromVec4d( m.getRow( row ) );
    } 
    PyErr_SetString(PyExc_TypeError, 
                    "int type required as argument to getRow" );
    return NULL;
  }

  PyObject* PyMatrix4d::getColumn( PyObject *myself, PyObject *args ) {
    if( args && PyInt_Check( args ) ) {
      int col = PyInt_AsLong( args );
      Matrix4d m = PyMatrix4d_AsMatrix4d( myself );
      return PyVec4d_FromVec4d( m.getColumn( col ) );
    } 
    PyErr_SetString(PyExc_TypeError, 
                    "int type required as argument to getColumn" );
    return NULL;
  }

   PyObject* PyMatrix4d::getScaleRotationPart( PyObject *myself, 
                                               PyObject *args ) {
     Matrix4d m = PyMatrix4d_AsMatrix4d( myself );
     return PyMatrix3d_FromMatrix3d( m.getScaleRotationPart() );
   }

   PyObject* PyMatrix4d::getRotationPart( PyObject *myself, 
                                          PyObject *args ) {
     Matrix4d m = PyMatrix4d_AsMatrix4d( myself );
     return PyMatrix3d_FromMatrix3d( m.getRotationPart() );
   }

  
  ///////////////////////////////////////////////////////////////////
  /// ROTATION
  /// 
  static PyMethodDef PyRotation_methods[] = {
    { "__repr__", (PyCFunction) PyRotation::repr, 0 },
    { "__str__", (PyCFunction) PyRotation::repr, 0 },
    { "toEulerAngles", (PyCFunction) PyRotation::toEulerAngles, 0 },
    { "slerp", (PyCFunction) PyRotation::slerp, 0 },
    {NULL, NULL}
  };
  
  static PyMemberDef PyRotation_members[] = {
    {"x", T_FLOAT, offsetof(PyRotation, axis.x), 0,"x"},
    {"y", T_FLOAT, offsetof(PyRotation, axis.y), 0,"y"},
    {"z", T_FLOAT, offsetof(PyRotation, axis.z), 0,"z"},
    {"a", T_FLOAT, offsetof(PyRotation, angle), 0,"a"},
    {"angle", T_FLOAT, offsetof(PyRotation, angle), 0,"angle"}, 
    {NULL}  /* Sentinel */
  };
  
  
  static PyNumberMethods PyRotation_as_number = {
    (binaryfunc)  0,          /* nb_add */
    (binaryfunc)  0,          /* nb_subtract */
    (binaryfunc)  PyRotation::mul, /* nb_multiply */
    (binaryfunc)  PyRotation::div,  /* nb_divide */
    (binaryfunc)    0,          /* nb_remainder */
    (binaryfunc)    0,          /* nb_divmod */
    (ternaryfunc)   0,          /* nb_power */
    (unaryfunc)     PyRotation::neg, /* nb_negative */
    (unaryfunc)     0,          /* tp_positive */
    (unaryfunc)     0,          /* tp_absolute */
    (inquiry)       0,          /* tp_nonzero */
    (unaryfunc)     0,          /* nb_invert */
    (binaryfunc)    0,          /* nb_lshift */
    (binaryfunc)    0,          /* nb_rshift */
    (binaryfunc)    0,          /* nb_and */
    (binaryfunc)    0,          /* nb_xor */
    (binaryfunc)    0,          /* nb_or */
    (coercion)      0,          /* nb_coerce */
    (unaryfunc)     0,          /* nb_int */
    (unaryfunc)     0,          /* nb_long */
    (unaryfunc)     0,          /* nb_float */
    (unaryfunc)     0,          /* nb_oct */
    (unaryfunc)   0,                  /* nb_hex */
    
    /* Added in release 2.0 */
    /* These require the Py_TPFLAGS_HAVE_INPLACEOPS flag */
    0,                      /* nb_inplace_add */
    0,                      /* nb_inplace_subtract */
    0,                      /* nb_inplace_multiply */
    0,                      /* nb_inplace_divide */
    0,                      /* nb_inplace_remainder */
    (ternaryfunc)0,         /* nb_inplace_power */
    0,                      /* nb_inplace_lshift */
    0,                      /* nb_inplace_rshift */
    0,                      /* nb_inplace_and */
    0,                      /* nb_inplace_xor */
    0,                      /* nb_inplace_or */
    
    /* Added in release 2.2 */
    /* These require the Py_TPFLAGS_HAVE_CLASS flag */
    (binaryfunc)   0,  /* nb_floor_divide */
    (binaryfunc)   0,  /* nb_true_divide */
    0,                      /* nb_inplace_floor_divide */
    0,                      /* nb_inplace_true_divide */
  };    
  PyTypeObject PyRotation_Type = {
    PyObject_HEAD_INIT(NULL)
    0,
    "H3D.Rotation",
    sizeof(PyRotation),
    0,                         /*tp_itemsize*/
    (destructor)PyRotation::dealloc, /*tp_dealloc*/
    0,                         /*tp_print*/
    0,                         /*tp_getattr*/
    0,                         /*tp_setattr*/
    (cmpfunc) PyRotation::compare,                         /*tp_compare*/
    (reprfunc) PyRotation::repr,                         /*tp_repr*/
    &PyRotation_as_number,                         /*tp_as_number*/
    0,                         /*tp_as_sequence*/
    0,                         /*tp_as_mapping*/
    0,                         /*tp_hash */
    0,                         /*tp_call*/
    0,                         /*tp_str*/
    0,                         /*tp_getattro*/
    0,                         /*tp_setattro*/
    0,                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | 
    Py_TPFLAGS_BASETYPE |
    Py_TPFLAGS_CHECKTYPES,     /*tp_flags*/
    "Rotation Object",         /* tp_doc */
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    PyRotation_methods,           /* tp_methods */
    PyRotation_members,           /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)PyRotation::init,   /* tp_init */
    PyType_GenericAlloc,       /* tp_alloc */
    (newfunc) PyType_GenericAlloc,            /* tp_new */
  };

  // Returns an Rotation representation of the contents of o.
  Rotation PyRotation_AsRotation( PyObject *o ) {
    if( PyRotation_Check( o ) ) {
      return *(Rotation *)(PyRotation *)(o);
    } else {
      throw Exception::H3DAPIException( "PyObject * is not a PyRotation *", 
                                        H3D_FULL_LOCATION );
    }
  }  

  // Creates a new PyRotation object based on the value of v.
  PyObject *PyRotation_FromRotation( const Rotation &v) {
    PyObject *o = PyType_GenericAlloc( &PyRotation_Type, 1 );
    Rotation *part = (Rotation *)(PyRotation *)( o );
    *part = v;
    return o;
  }
   
  PyObject* PyRotation::slerp( PyObject *myself, PyObject *args ) {
    if( !args || !PyTuple_Check( args ) || PyTuple_Size( args ) != 2  ) {
      PyErr_SetString(PyExc_TypeError, 
             "invalid number of arguments given Rotation::slerp( Rot, t )." );
      return 0;
    }

    PyObject *q = PyTuple_GetItem( args, 0 );
    PyObject *t = PyTuple_GetItem( args, 1 );

    if( !PyRotation_Check( q ) || !PyFloat_Check( t ) ) {
      PyErr_SetString(PyExc_TypeError, 
           "invalid type given as arguments to Rotation::slerp( Rot, t )." );
      return 0;
    }

    Rotation rot1 = PyRotation_AsRotation( myself );
    Rotation rot2 = PyRotation_AsRotation( q );
    H3DFloat arg = (H3DFloat) PyFloat_AsDouble( t );
    return PyRotation_FromRotation( rot1.slerp( rot2, arg ) );
  }

  int PyRotation::init(PyRotation *self, PyObject *args, PyObject *kwds)  {
    Py_ssize_t args_size =  PyTuple_Size( args );
    Rotation *self_r = (Rotation *)self;
    if( args_size == 0 ) {
      *self_r = Rotation();
    } else if( args_size == 1 ) {
      // from Quaternion, Rotation, Matrix3d and Matrix3f
      PyObject *o = PyTuple_GetItem( args, 0 );
      if( PyRotation_Check( o ) ) {
        Rotation r = PyRotation_AsRotation( o );
        *self_r = r;
      } else if( PyQuaternion_Check( o ) ) {
        Quaternion q = PyQuaternion_AsQuaternion( o );
        *self_r = Rotation(q);
      } else if( PyMatrix3d_Check( o ) ) {
        Matrix3f m = PyMatrix3f_AsMatrix3f( o );
        *self_r = (Rotation)m;
      } else if( PyMatrix3f_Check( o ) ) {
        Matrix3f m = PyMatrix3f_AsMatrix3f( o );
        *self_r = (Rotation)m;
      } else if( PyVec3f_Check( o ) ) {
        Vec3f v = PyVec3f_AsVec3f( o );
        *self_r = Rotation( v );
      } else if( PyVec3d_Check( o ) ) {
        Vec3d v = PyVec3d_AsVec3d( o );
        *self_r = Rotation( v );
      }else {
        PyErr_SetString(PyExc_TypeError, 
                        "invalid type given to Rotation constructor." );
        return -1;
      }
    } else if( args_size == 2 ) {
      PyObject *o0 = PyTuple_GetItem( args, 0 );
      PyObject *o1 = PyTuple_GetItem( args, 1 );
      if( PyVec3f_Check( o0 ) ) {
        Vec3f v0 = PyVec3f_AsVec3f( o0 );
        if( PyVec3f_Check( o1 ) ) {
          Vec3f v1 = PyVec3f_AsVec3f( o1 );
          *self_r = Rotation( v0, v1 );
        } else if( PyFloat_Check( o1 ) ) {
          *self_r = Rotation( v0, (H3DFloat) PyFloat_AsDouble( o1 ) );
        } else if( PyInt_Check( o1 ) ) {
          *self_r = Rotation( v0, (H3DFloat) PyInt_AsLong( o1 ) );
        } else {
          PyErr_SetString(PyExc_TypeError, 
                          "invalid type given to Rotation constructor." );
          return -1;
        }
      } else {
        PyErr_SetString(PyExc_TypeError, 
                        "invalid type given to Rotation constructor." );
        return -1;
      }
    } else {
      static char *kwlist[] = {"x", "y", "z", "a", NULL};
      if (! PyArg_ParseTupleAndKeywords(args, kwds, "|ffff", kwlist, 
                                        &(self->axis.x), &(self->axis.y), 
                                        &(self->axis.z), &(self->angle)))
        return -1; 
    }
    // success
    return 0;
  }
  
  PyObject* PyRotation::mul( PyObject *a, PyObject *b ) {
    if( PyRotation_Check( a ) ) {
      Rotation ma = PyRotation_AsRotation( a );
      if( PyVec3f_Check( b ) ) {
        Vec3f vb = PyVec3f_AsVec3f( b );
        return PyVec3f_FromVec3f( ma * vb );
      } else if( PyVec3d_Check( b ) ) {
        Vec3d vb = PyVec3d_AsVec3d( b );
        return PyVec3d_FromVec3d( ma * vb );
      }
    }

    return PyNumberTypeWrapper< Rotation, 
      &PyRotation_Type,
      PyRotation_Name,
      PyRotation_Check,
      PyRotation_AsRotation, 
      PyRotation_FromRotation >::mul( a, b );
  }

  PyObject* PyRotation::toEulerAngles( PyObject *myself, PyObject *args ) {
    Rotation r = PyRotation_AsRotation( myself );
    return (PyObject *)PyVec3f_FromVec3f( r.toEulerAngles() );
  }
  
  
  ///////////////////////////////////////////////////////////////////
  /// QUATERNION
  /// 
  static PyMethodDef PyQuaternion_methods[] = {
    { "__repr__", (PyCFunction) PyQuaternion::repr, 0 },
    { "__str__", (PyCFunction) PyQuaternion::repr, 0 },
    { "toEulerAngles", (PyCFunction) PyQuaternion::toEulerAngles, 0 },
    { "norm", (PyCFunction) PyQuaternion::norm, 0 },
    { "normalize", (PyCFunction) PyQuaternion::normalize, 0 },
    { "conjugate", (PyCFunction) PyQuaternion::conjugate, 0 },
    { "inverse", (PyCFunction) PyQuaternion::inverse, 0 },
    { "slerp", (PyCFunction) PyQuaternion::slerp, 0 },
    { "dotProduct", (PyCFunction) PyQuaternion::dotProduct, 0 },
    {NULL, NULL}
  };
  
  static PyMemberDef PyQuaternion_members[] = {
    {"x", T_FLOAT, offsetof(PyQuaternion, v.x), 0,"x"},
    {"y", T_FLOAT, offsetof(PyQuaternion, v.y), 0,"y"},
    {"z", T_FLOAT, offsetof(PyQuaternion, v.z), 0,"z"},
    {"w", T_FLOAT, offsetof(PyQuaternion, w), 0,"w"},
    {NULL}  /* Sentinel */
  };
  
  
  static PyNumberMethods PyQuaternion_as_number = {
    (binaryfunc)  PyQuaternion::add,    /* nb_add */
    (binaryfunc)  PyQuaternion::sub,    /* nb_subtract */
    (binaryfunc)  PyQuaternion::mul,       /* nb_multiply */
    (binaryfunc)  PyQuaternion::div,    /* nb_divide */
    (binaryfunc)    0,          /* nb_remainder */
    (binaryfunc)    0,          /* nb_divmod */
    (ternaryfunc)   0,          /* nb_power */
    (unaryfunc)   PyQuaternion::neg,  /* nb_negative */
    (unaryfunc)     0,          /* tp_positive */
    (unaryfunc)     0,          /* tp_absolute */
    (inquiry)       0,          /* tp_nonzero */
    (unaryfunc)     0,          /* nb_invert */
    (binaryfunc)    0,          /* nb_lshift */
    (binaryfunc)    0,          /* nb_rshift */
    (binaryfunc)    0,          /* nb_and */
    (binaryfunc)    0,          /* nb_xor */
    (binaryfunc)    0,          /* nb_or */
    (coercion)      0,          /* nb_coerce */
    (unaryfunc)     0,          /* nb_int */
    (unaryfunc)     0,          /* nb_long */
    (unaryfunc)     0,          /* nb_float */
    (unaryfunc)     0,          /* nb_oct */
    (unaryfunc)   0,                  /* nb_hex */
    
    /* Added in release 2.0 */
    /* These require the Py_TPFLAGS_HAVE_INPLACEOPS flag */
    0,                      /* nb_inplace_add */
    0,                      /* nb_inplace_subtract */
    0,                      /* nb_inplace_multiply */
    0,                      /* nb_inplace_divide */
    0,                      /* nb_inplace_remainder */
    (ternaryfunc)0,         /* nb_inplace_power */
    0,                      /* nb_inplace_lshift */
    0,                      /* nb_inplace_rshift */
    0,                      /* nb_inplace_and */
    0,                      /* nb_inplace_xor */
    0,                      /* nb_inplace_or */
    
    /* Added in release 2.2 */
    /* These require the Py_TPFLAGS_HAVE_CLASS flag */
    (binaryfunc)    PyQuaternion::div,  /* nb_floor_divide */
    (binaryfunc)    PyQuaternion::div,  /* nb_true_divide */
    0,                      /* nb_inplace_floor_divide */
    0,                      /* nb_inplace_true_divide */
  };    
  PyTypeObject PyQuaternion_Type = {
    PyObject_HEAD_INIT(NULL)
    0,
    "H3D.Quaternion",
    sizeof(PyQuaternion),
    0,                         /*tp_itemsize*/
    (destructor)PyQuaternion::dealloc, /*tp_dealloc*/
    0,                         /*tp_print*/
    0,                         /*tp_getattr*/
    0,                         /*tp_setattr*/
    (cmpfunc) PyQuaternion::compare,                         /*tp_compare*/
    (reprfunc) PyQuaternion::repr,                         /*tp_repr*/
    &PyQuaternion_as_number,                         /*tp_as_number*/
    0,                         /*tp_as_sequence*/
    0,                         /*tp_as_mapping*/
    0,                         /*tp_hash */
    0,                         /*tp_call*/
    0,                         /*tp_str*/
    0,                         /*tp_getattro*/
    0,                         /*tp_setattro*/
    0,                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | 
    Py_TPFLAGS_BASETYPE |
    Py_TPFLAGS_CHECKTYPES, /*tp_flags*/
    "Quaternion Object",            /* tp_doc */
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    PyQuaternion_methods,           /* tp_methods */
    PyQuaternion_members,           /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)PyQuaternion::init,   /* tp_init */
    PyType_GenericAlloc,       /* tp_alloc */
    (newfunc) PyType_GenericAlloc,            /* tp_new */
  };
  
  PyObject* PyQuaternion::mul( PyObject *a, PyObject *b ) {
    if( PyQuaternion_Check( a ) ) {
      Quaternion ma = PyQuaternion_AsQuaternion( a );
      if( PyVec3f_Check( b ) ) {
        Vec3f vb = PyVec3f_AsVec3f( b );
        return PyVec3f_FromVec3f( ma * vb );
      } else if( PyVec3d_Check( b ) ) {
        Vec3d vb = PyVec3d_AsVec3d( b );
        return PyVec3d_FromVec3d( ma * vb );
      }
    }

    return PyNumberTypeWrapper< Quaternion, 
      &PyQuaternion_Type,
      PyQuaternion_Name,
      PyQuaternion_Check,
      PyQuaternion_AsQuaternion, 
      PyQuaternion_FromQuaternion >::mul( a, b );
  }

  // Returns an Quaternion representation of the contents of o.
  Quaternion PyQuaternion_AsQuaternion( PyObject *o ) {
    if( PyQuaternion_Check( o ) ) {
      return *(Quaternion *)(PyQuaternion *)(o);
    } else {
      throw Exception::H3DAPIException( "PyObject * is not a PyQuaternion *", 
                                        H3D_FULL_LOCATION );
    }
  }  

  // Creates a new PyQuaternion object based on the value of v.
  PyObject *PyQuaternion_FromQuaternion( const Quaternion &v) {
    PyObject *o = PyType_GenericAlloc( &PyQuaternion_Type, 1 );
    Quaternion *part = (Quaternion *)(PyQuaternion *)( o );
    *part = v;
    return o;
  }

  int PyQuaternion::init(PyQuaternion *self, PyObject *args, PyObject *kwds)  {
    Py_ssize_t args_size =  PyTuple_Size( args );
    if( args_size == 0 ) {
      Quaternion *self_q = (Quaternion *)self;
      *self_q = Quaternion();
    } else if( args_size == 1 ) {
      // from Quaternion, Rotation and Matrix3f
      PyObject *o = PyTuple_GetItem( args, 0 );
      Quaternion *self_m = (Quaternion *)self;
      if( PyRotation_Check( o ) ) {
        Rotation r = PyRotation_AsRotation( o );
        *self_m = Quaternion(r);
      } else if( PyQuaternion_Check( o ) ) {
        Quaternion q = PyQuaternion_AsQuaternion( o );
        *self_m = q;
      } else if( PyMatrix3f_Check( o ) ) {
        Matrix3f m = PyMatrix3f_AsMatrix3f( o );
        *self_m = (Quaternion)m;
      } else {
        PyErr_SetString(PyExc_TypeError, 
                        "invalid type given to Quaternion constructor." );
        return -1;
      }
    } else if( args_size == 2 ) {
      Quaternion *self_q = (Quaternion *)self;
      PyObject *o0 = PyTuple_GetItem( args, 0 );
      PyObject *o1 = PyTuple_GetItem( args, 1 );
      if( PyVec3f_Check( o0 ) ) {
        Vec3f v0 = PyVec3f_AsVec3f( o0 );
        if( PyFloat_Check( o1 ) ) {
          *self_q = Quaternion( v0, (H3DFloat) PyFloat_AsDouble( o1 ) );
        } else if( PyInt_Check( o1 ) ) {
          *self_q = Quaternion( v0, (H3DFloat) PyInt_AsLong( o1 ) );
        } else {
          PyErr_SetString(PyExc_TypeError, 
                          "invalid type given to Quaternion constructor." );
          return -1;
        }
      } else {
        PyErr_SetString(PyExc_TypeError, 
                        "invalid type given to Quaternion constructor." );
        return -1;
      }
    } else {
      static char *kwlist[] = {"x", "y", "z", "w", NULL};
      if (! PyArg_ParseTupleAndKeywords(args, kwds, "|ffff", kwlist, 
                                        &(self->v.x), &(self->v.y), 
                                        &(self->v.z), &(self->w)))
        return -1; 
    }
    // success
    return 0;
  }
  
  PyObject* PyQuaternion::normalize( PyObject *myself, PyObject *args ) {
    PyQuaternion *q = (PyQuaternion*)myself;
    Quaternion *quat = (Quaternion*)q;
    quat->normalize();
    Py_INCREF( myself );
    return myself;
  }

  PyObject* PyQuaternion::dotProduct( PyObject *myself, PyObject *args ) {
    if( !args || !PyQuaternion_Check( args ) ) {
      PyErr_SetString(PyExc_TypeError, 
                "invalid type given as argument to Quaternion::dotProduct." );
      return 0;
    }
    Quaternion quat = PyQuaternion_AsQuaternion( myself );
    Quaternion arg = PyQuaternion_AsQuaternion( args );
    return PyFloat_FromDouble( quat.dotProduct( arg ) );
  }

  PyObject* PyQuaternion::slerp( PyObject *myself, PyObject *args ) {
    if( !args || !PyTuple_Check( args ) || PyTuple_Size( args ) != 2  ) {
      PyErr_SetString(PyExc_TypeError, 
          "invalid number of arguments given Quaternion::slerp( Quat, t )." );
      return 0;
    }

    PyObject *q = PyTuple_GetItem( args, 0 );
    PyObject *t = PyTuple_GetItem( args, 1 );

    if( !PyQuaternion_Check( q ) || !PyFloat_Check( t ) ) {
      PyErr_SetString(PyExc_TypeError, 
         "invalid type given as arguments to Quaternion::slerp( Quat, t )." );
      return 0;
    }

    Quaternion quat1 = PyQuaternion_AsQuaternion( myself );
    Quaternion quat2 = PyQuaternion_AsQuaternion( q );
    H3DFloat arg = (H3DFloat) PyFloat_AsDouble( t );
    return PyQuaternion_FromQuaternion( quat1.slerp( quat2, arg ) );
  }

  PyObject* PyQuaternion::norm( PyObject *myself, PyObject *args ) {
    Quaternion quat = PyQuaternion_AsQuaternion( myself );
    return PyFloat_FromDouble( quat.norm() );
  }

  PyObject* PyQuaternion::toEulerAngles( PyObject *myself, PyObject *args ) {
    Quaternion quat = PyQuaternion_AsQuaternion( myself );
    return PyVec3f_FromVec3f( quat.toEulerAngles() );
  }

  PyObject* PyQuaternion::conjugate( PyObject *myself, PyObject *args ) {
    Quaternion quat = PyQuaternion_AsQuaternion( myself );
    return PyQuaternion_FromQuaternion( quat.conjugate() );
  }

  PyObject* PyQuaternion::inverse( PyObject *myself, PyObject *args ) {
    Quaternion quat = PyQuaternion_AsQuaternion( myself );
    return PyQuaternion_FromQuaternion( quat.inverse() );
  }

  
  ///////////////////////////////////////////////////////////////////
  /// RGB
  /// 
  static PyMethodDef PyRGB_methods[] = {
  { "__repr__", (PyCFunction) PyRGB::repr, 0 },
  { "__str__", (PyCFunction) PyRGB::repr, 0 },
  {NULL, NULL}
  };
  
  static PyMemberDef PyRGB_members[] = {
  {"r", T_FLOAT, offsetof(PyRGB, r), 0,"r"},
  {"g", T_FLOAT, offsetof(PyRGB, g), 0,"g"},
  {"b", T_FLOAT, offsetof(PyRGB, b), 0,"b"},
  {NULL}  /* Sentinel */
  };
  
  
  static PyNumberMethods PyRGB_as_number = {
    (binaryfunc)    0,   /* nb_add */
    (binaryfunc)    0,   /* nb_subtract */
    (binaryfunc)    0,       /* nb_multiply */
    (binaryfunc)    0,   /* nb_divide */
    (binaryfunc)    0,   /* nb_remainder */
    (binaryfunc)    0,          /* nb_divmod */
    (ternaryfunc)   0,          /* nb_power */
    (unaryfunc)     0,          /* nb_negative */
    (unaryfunc)     0,          /* tp_positive */
    (unaryfunc)     0,          /* tp_absolute */
    (inquiry)       0,          /* tp_nonzero */
    (unaryfunc)     0,          /* nb_invert */
    (binaryfunc)    0,          /* nb_lshift */
    (binaryfunc)    0,          /* nb_rshift */
    (binaryfunc)    0,          /* nb_and */
    (binaryfunc)    0,          /* nb_xor */
    (binaryfunc)    0,          /* nb_or */
    (coercion)      0,          /* nb_coerce */
    (unaryfunc)     0,          /* nb_int */
    (unaryfunc)     0,          /* nb_long */
    (unaryfunc)     0,          /* nb_float */
    (unaryfunc)     0,          /* nb_oct */
    (unaryfunc)   0,                  /* nb_hex */
    
    /* Added in release 2.0 */
    /* These require the Py_TPFLAGS_HAVE_INPLACEOPS flag */
    0,                      /* nb_inplace_add */
    0,                      /* nb_inplace_subtract */
    0,                      /* nb_inplace_multiply */
    0,                      /* nb_inplace_divide */
    0,                      /* nb_inplace_remainder */
    (ternaryfunc)0,         /* nb_inplace_power */
    0,                      /* nb_inplace_lshift */
    0,                      /* nb_inplace_rshift */
    0,                      /* nb_inplace_and */
    0,                      /* nb_inplace_xor */
    0,                      /* nb_inplace_or */
    
    /* Added in release 2.2 */
    /* These require the Py_TPFLAGS_HAVE_CLASS flag */
    (binaryfunc)    0,  /* nb_floor_divide */
    (binaryfunc)    0,  /* nb_true_divide */
    0,                      /* nb_inplace_floor_divide */
    0,                      /* nb_inplace_true_divide */
  };    
  PyTypeObject PyRGB_Type = {
    PyObject_HEAD_INIT(NULL)
    0,
    "H3D.RGB",
    sizeof(PyRGB),
    0,                         /*tp_itemsize*/
    (destructor)PyRGB::dealloc, /*tp_dealloc*/
    0,                         /*tp_print*/
    0,                         /*tp_getattr*/
    0,                         /*tp_setattr*/
    (cmpfunc) PyRGB::compare,                         /*tp_compare*/
    (reprfunc) PyRGB::repr,                         /*tp_repr*/
    &PyRGB_as_number,                         /*tp_as_number*/
    0,                         /*tp_as_sequence*/
    0,                         /*tp_as_mapping*/
    0,                         /*tp_hash */
    0,                         /*tp_call*/
    0,                         /*tp_str*/
    0,                         /*tp_getattro*/
    0,                         /*tp_setattro*/
    0,                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
    "RGB Object",            /* tp_doc */
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    PyRGB_methods,           /* tp_methods */
    PyRGB_members,           /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)PyRGB::init,   /* tp_init */
    PyType_GenericAlloc,       /* tp_alloc */
    (newfunc) PyType_GenericAlloc,            /* tp_new */
  };
  
  // Returns an RGB representation of the contents of o.
  RGB PyRGB_AsRGB( PyObject *o ) {
    if( PyRGB_Check( o ) ) {
      return *(RGB *)(PyRGB *)(o);
    } else {
      throw Exception::H3DAPIException( "PyObject * is not a PyRGB *", 
                                        H3D_FULL_LOCATION );
    }
  }  

  // Creates a new PyRGB object based on the value of v.
  PyObject *PyRGB_FromRGB( const RGB &v) {
    PyObject *o = PyType_GenericAlloc( &PyRGB_Type, 1 );
    RGB *part = (RGB *)(PyRGB *)( o );
    *part = v;
    return o;
  }

  int PyRGB::init(PyRGB *self, PyObject *args, PyObject *kwds)  {
    if( PyTuple_Size( args ) == 0 ) {
      RGB *self_v = (RGB *)self;
      *self_v = RGB();
    } else {
      static char *kwlist[] = {"r", "g", "b", NULL};
      if (! PyArg_ParseTupleAndKeywords(args, kwds, "fff", kwlist, 
                                        &(self->r), &(self->g), &(self->b)))
        return -1; 
    }
    return 0;
  }
    
  ///////////////////////////////////////////////////////////////////
  /// RGBA
  /// 
  static PyMethodDef PyRGBA_methods[] = {
  { "__repr__", (PyCFunction) PyRGBA::repr, 0 },
  { "__str__", (PyCFunction) PyRGBA::repr, 0 },
  {NULL, NULL}
  };
  
  static PyMemberDef PyRGBA_members[] = {
  {"r", T_FLOAT, offsetof(PyRGBA, r), 0,"r"},
  {"g", T_FLOAT, offsetof(PyRGBA, g), 0,"g"},
  {"b", T_FLOAT, offsetof(PyRGBA, b), 0,"b"},
  {"a", T_FLOAT, offsetof(PyRGBA, a), 0,"a"},
  {NULL}  /* Sentinel */
  };
  
  
  static PyNumberMethods PyRGBA_as_number = {
    (binaryfunc)    0,   /* nb_add */
    (binaryfunc)    0,   /* nb_subtract */
    (binaryfunc)    0,       /* nb_multiply */
    (binaryfunc)    0,   /* nb_divide */
    (binaryfunc)    0,   /* nb_remainder */
    (binaryfunc)    0,          /* nb_divmod */
    (ternaryfunc)   0,          /* nb_power */
    (unaryfunc)     0,          /* nb_negative */
    (unaryfunc)     0,          /* tp_positive */
    (unaryfunc)     0,          /* tp_absolute */
    (inquiry)       0,          /* tp_nonzero */
    (unaryfunc)     0,          /* nb_invert */
    (binaryfunc)    0,          /* nb_lshift */
    (binaryfunc)    0,          /* nb_rshift */
    (binaryfunc)    0,          /* nb_and */
    (binaryfunc)    0,          /* nb_xor */
    (binaryfunc)    0,          /* nb_or */
    (coercion)      0,          /* nb_coerce */
    (unaryfunc)     0,          /* nb_int */
    (unaryfunc)     0,          /* nb_long */
    (unaryfunc)     0,          /* nb_float */
    (unaryfunc)     0,          /* nb_oct */
    (unaryfunc)   0,                  /* nb_hex */
    
    /* Added in release 2.0 */
    /* These require the Py_TPFLAGS_HAVE_INPLACEOPS flag */
    0,                      /* nb_inplace_add */
    0,                      /* nb_inplace_subtract */
    0,                      /* nb_inplace_multiply */
    0,                      /* nb_inplace_divide */
    0,                      /* nb_inplace_remainder */
    (ternaryfunc)0,         /* nb_inplace_power */
    0,                      /* nb_inplace_lshift */
    0,                      /* nb_inplace_rshift */
    0,                      /* nb_inplace_and */
    0,                      /* nb_inplace_xor */
    0,                      /* nb_inplace_or */
    
    /* Added in release 2.2 */
    /* These require the Py_TPFLAGS_HAVE_CLASS flag */
    (binaryfunc)    0,  /* nb_floor_divide */
    (binaryfunc)    0,  /* nb_true_divide */
    0,                      /* nb_inplace_floor_divide */
    0,                      /* nb_inplace_true_divide */
  };    
  PyTypeObject PyRGBA_Type = {
    PyObject_HEAD_INIT(NULL)
    0,
    "H3D.RGBA",
    sizeof(PyRGBA),
    0,                         /*tp_itemsize*/
    (destructor)PyRGBA::dealloc, /*tp_dealloc*/
    0,                         /*tp_print*/
    0,                         /*tp_getattr*/
    0,                         /*tp_setattr*/
    (cmpfunc) PyRGBA::compare,                         /*tp_compare*/
    (reprfunc) PyRGBA::repr,                         /*tp_repr*/
    &PyRGBA_as_number,                         /*tp_as_number*/
    0,                         /*tp_as_sequence*/
    0,                         /*tp_as_mapping*/
    0,                         /*tp_hash */
    0,                         /*tp_call*/
    0,                         /*tp_str*/
    0,                         /*tp_getattro*/
    0,                         /*tp_setattro*/
    0,                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
    "RGBA Object",            /* tp_doc */
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    PyRGBA_methods,           /* tp_methods */
    PyRGBA_members,           /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)PyRGBA::init,   /* tp_init */
    PyType_GenericAlloc,       /* tp_alloc */
    (newfunc) PyType_GenericAlloc,            /* tp_new */
  };
  
  // Returns an RGBA representation of the contents of o.
  RGBA PyRGBA_AsRGBA( PyObject *o ) {
    if( PyRGBA_Check( o ) ) {
      return *(RGBA *)(PyRGBA *)(o);
    } else {
      throw Exception::H3DAPIException( "PyObject * is not a PyRGBA *", 
                                        H3D_FULL_LOCATION );
    }
  }  

  // Creates a new PyRGBA object based on the value of v.
  PyObject *PyRGBA_FromRGBA( const RGBA &v) {
    PyObject *o = PyType_GenericAlloc( &PyRGBA_Type, 1 );
    RGBA *part = (RGBA *)(PyRGBA *)( o );
    *part = v;
    return o;
  }


  int PyRGBA::init(PyRGBA *self, PyObject *args, PyObject *kwds)  {
    if( PyTuple_Size( args ) == 0 ) {
      RGBA *self_v = (RGBA *)self;
      *self_v = RGBA();
    } else {
      static char *kwlist[] = {"r", "g", "b", "a", NULL};
      if (! PyArg_ParseTupleAndKeywords(args, kwds, "ffff", kwlist, 
                                        &(self->r), 
                                        &(self->g), 
                                        &(self->b), 
                                        &(self->a)))
        return -1; 
    }
    return 0;
  }

  static PyMethodDef PyConsole_methods[] = {
    { "write", (PyCFunction) PyConsole::write, 0 },
    { "writeAtLevel", (PyCFunction)PyConsole::writeAtLevel, 0 },
    {NULL, NULL}
  };

  PyTypeObject PyConsole_Type = {
    PyObject_HEAD_INIT(NULL)
    0,
    "H3D.Console",
    sizeof(PyConsole),
    0,                         /*tp_itemsize*/
    (destructor)PyConsole::dealloc, /*tp_dealloc*/
    0,                         /*tp_print*/
    0,                         /*tp_getattr*/
    0,                         /*tp_setattr*/
    0,
    0,
    0,
    0,                         /*tp_as_sequence*/
    0,                         /*tp_as_mapping*/
    0,                         /*tp_hash */
    0,                         /*tp_call*/
    0,                         /*tp_str*/
    0,                         /*tp_getattro*/
    0,                         /*tp_setattro*/
    0,                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
    "Console Object",            /* tp_doc */
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    PyConsole_methods,           /* tp_methods */
    0, 
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    0,
    PyType_GenericAlloc,       /* tp_alloc */
    (newfunc) PyType_GenericAlloc,            /* tp_new */
  };

  void writeHelper ( int _level, const std::string& _msg ) {
    Console ( _level ) << _msg;

    // Python print results in a separate call with the newline
    // and we only want to flush once for each print since Console
    // may addprefix time stamp etc on each sync
    if( !_msg.empty() && _msg[_msg.length() - 1] == '\n' ) {
      Console.flush();
    }
  }

  PyObject* PyConsole::write( PyObject *self, PyObject *args ) {
    PyObject *s = PyObject_Repr( args );
    char *msg = PyString_AsString( args );
    if( msg ) {
      writeHelper ( LogLevel::Info, msg );

      Py_DECREF( s );
      Py_INCREF( Py_None );
      return Py_None;
    } else {
      Py_DECREF( s );
      return 0;
    }
  }

  PyObject* PyConsole::writeAtLevel( PyObject *self, PyObject *args ) {

    if( !PyTuple_Check( args ) || PyTuple_Size( args ) != 2 ) {
      PyErr_SetString( PyExc_ValueError, 
                       "Invalid argument(s) to function PyConsole.writeAtLevel( \
self, level, message )" );
      return NULL;
    } 
    
    PyObject *levelObj = PyTuple_GetItem( args, 0 );
    PyObject *msgObj = PyTuple_GetItem( args, 1 );
    
    if( PyInt_Check( levelObj ) && PyString_Check ( msgObj ) ) {
      int level = PyInt_AsLong( levelObj );
      char* msg = PyString_AsString( msgObj );
      
      if ( msg ) {
        writeHelper ( level, msg );
      }

      Py_INCREF( Py_None );
      return Py_None;
    }

    PyErr_SetString( PyExc_ValueError,
      "Invalid argument(s) to function H3D.Console.setLevel( self, level )" );
    return 0;
  }

}

#endif // HAVE_PYTHON
