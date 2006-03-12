
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
/// \file Bound.h
/// \brief Header file for Bound and some of its subclasses.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __H3DBOUND_H__
#define __H3DBOUND_H__

#include "H3DApi.h"
#include "FieldTemplates.h"
#include "RefCountedClass.h"
#include "RefCountSField.h"
#include "Exception.h"
#include "SFVec3f.h"
#include "SFMatrix4f.h"

namespace H3D {
  /// The Bound class is the abstract base class for all classes specifying
  /// bounding objects.
  class H3DAPI_API Bound: public RefCountedClass {
  public:
    /// Determines if a given point is inside the bound or not.
    virtual bool isInside( const Vec3f &p ) = 0;

    /// Checks a line segment for intersection with the bound. If line
    /// intersects, true is returned and the to parameter is set to the 
    /// intersection point. If no intersection, false is returned.
    virtual bool lineSegmentIntersect( const Vec3f& from,
                                       Vec3f &to ) = 0;

    /// Returns a Bound that is the union between all the bounds specified
    /// by the iterators. Iterator is a iterator to a Bound.
    /// 
    template< class Iterator >
    static inline Bound *boundUnion( Iterator begin, Iterator end );

    /// Returns a Bound that is the union between the Bound objects 
    /// in the SFBound fields specified by the iterators.
    /// 
    template< class Iterator >
    static inline Bound *SFBoundUnion( Iterator begin, Iterator end );

  };

  /// An InfiniteBound is a Bound that encompasses everything. So every point
  /// is inside the bound.
  ///
  class H3DAPI_API InfiniteBound: public Bound {
  public:
    /// Determines if a given point is inside the bound or not.
    virtual bool isInside( const Vec3f &p ){ 
      return true;
    }

    /// Returns true always. The 'to' param is set to 'from'. 
    virtual bool lineSegmentIntersect( const Vec3f& from,
                                       Vec3f &to ) {
      to = from;
      return true;
    }
  };  

  /// An EmptyBound is a Bound that encompasses nothing. So every point
  /// is outside the bound.
  ///
  class H3DAPI_API EmptyBound: public Bound {
  public:
    /// Determines if a given point is inside the bound or not.
    virtual bool isInside( const Vec3f &p ){ 
      return false;
    }

    /// Returns false always. 
    virtual bool lineSegmentIntersect( const Vec3f& from,
                                       Vec3f &to ) {
      return false;
    }
  };  

  
  
  /// The BoxBound is a Bound class that specifies the bound using an
  /// axis-aligned bounding box. The center field is the center point
  /// of the bounding box and the size field is the size of the bounding
  /// box.
  ///
  class H3DAPI_API BoxBound: public Bound {
  public:
    
    /// Constructor.
    BoxBound( Inst< SFVec3f > _center = 0,
              Inst< SFVec3f > _size   = 0 ):
      center( _center ),
      size( _size ) {
      // set default values
      center->setValue( Vec3f( 0, 0, 0 ) );
      size->setValue( Vec3f( 0, 0, 0 ) );
    }

    /// This function sets the BoxBound to encompass all the points
    /// between the begin and end iterators. The iterator must have a 
    /// value_type of Vec3f
    template< class InputIterator >
    void fitAroundPoints( InputIterator begin,
                          InputIterator end ) {
      if( begin == end ) {
        center->setValue( Vec3f( 0, 0, 0 ) );
        size->setValue( Vec3f( 0, 0, 0 ) );
        return;
      }
      InputIterator i = begin;
      Vec3f min = *i;
      Vec3f max = *i;
      i++;
      for( ; i != end; ++i ) {
        if( (*i).x < min.x ) min.x = (*i).x;
        if( (*i).y < min.y ) min.y = (*i).y;
        if( (*i).z < min.z ) min.z = (*i).z;
        if( (*i).x > max.x ) max.x = (*i).x;
        if( (*i).y > max.y ) max.y = (*i).y;
        if( (*i).z > max.z ) max.z = (*i).z;
      }
      Vec3f s = max - min;
      center->setValue( min + s / 2.0 );
      size->setValue( s );
    }

    template< class InputIterator >
    void fitAround2DPoints( InputIterator begin,
                            InputIterator end ) {
      if( begin == end ) {
        center->setValue( Vec3f( 0, 0, 0 ) );
        size->setValue( Vec3f( 0, 0, 0 ) );
        return;
      }
      InputIterator i = begin;
      Vec2f min = *i;
      Vec2f max = *i;
      i++;
      for( ; i != end; ++i ) {
        if( (*i).x < min.x ) min.x = (*i).x;
        if( (*i).y < min.y ) min.y = (*i).y;
        if( (*i).x > max.x ) max.x = (*i).x;
        if( (*i).y > max.y ) max.y = (*i).y;
      }
      Vec2f s = max - min;
      Vec2f c = min + s / 2.0;
      center->setValue( Vec3f( c.x, c.y, 0.f ) );
      size->setValue( Vec3f( s.x, s.y, 0.f ) );
    }

    /// Determines if a given point is inside the bound or not.
    virtual bool isInside( const Vec3f &p ){ 
      Vec3f half_size = size->getValue() / 2.0;
      const Vec3f c = center->getValue();
      return ( p.x <= ( c.x + half_size.x ) &&
               p.x >= ( c.x - half_size.x ) &&
               p.y <= ( c.y + half_size.y ) &&
               p.y >= ( c.y - half_size.y ) &&
               p.z <= ( c.z + half_size.z ) &&
               p.z >= ( c.z - half_size.z ) );
    }

    /// Not implemented.
    virtual bool lineSegmentIntersect( const Vec3f& from,
                                       Vec3f &to ) {
      throw Exception::H3DAPIException("BoxBound::lineSegmentIntersect not implemented" );     
    }
    /// The center point of the bounding box.
    auto_ptr< SFVec3f > center;
    /// The size of the bounding box.
    auto_ptr< SFVec3f > size;
  };  

  /// A TransformedBoxBound is a wrapper around another BoxBound
  /// and its values are the values of the BoxBound transformed by
  /// a transformation matrix. 
  /// 
  class TransformedBoxBound: public BoxBound {
  public:
    /// The BoxBound object is reference counted by the field.
    typedef RefCountSField< BoxBound > SFBoxBound;
    
    /// The Center field updates itself from the boxBound and matrix fields.
    ///
    /// routes_in[0] is the boxBound field of the TransformedBoxBound.
    /// routes_in[1] is the matrix field of the TransformedBoxBound.
    ///
    class Center: public TypedField< SFVec3f,
                  SFBoxBound,
                  SFMatrix4f > {
      virtual void update() {
        BoxBound *bb = 
          static_cast< SFBoxBound * >( routes_in[0] )->getValue();
        const Matrix4f &matrix = 
          static_cast< SFMatrix4f * >( routes_in[1] )->getValue();
        value = matrix * bb->center->getValue();
      }
    };

    
    /// The Size field updates itself from the boxBound and matrix fields.
    ///
    /// routes_in[0] is the boxBound field of the TransformedBoxBound.
    /// routes_in[1] is the matrix field of the TransformedBoxBound.
    ///
    class Size: public TypedField< SFVec3f,
                SFBoxBound,
                SFMatrix4f > {
      virtual void update() {
        BoxBound *bb = 
          static_cast< SFBoxBound * >( routes_in[0] )->getValue();
        const Matrix4f &matrix = 
          static_cast< SFMatrix4f * >( routes_in[1] )->getValue();
  
        const Vec3f &bb_center       = bb->center->getValue();
        const Vec3f &half_bb_size    = bb->size->getValue()/2.0;
  
        Vec3f trf_bb = bb_center + half_bb_size;
        Vec3f llc_bb = bb_center - half_bb_size;

        // transform each corder and fit a new BoxBound around them.
        vector<Vec3f> corners;
        // +++
        Vec3f point = trf_bb;
        corners.push_back( matrix * point );
        // ++-
        point.z = llc_bb.z;
        corners.push_back( matrix * point );
        // +--
        point.y = llc_bb.y;
        corners.push_back( matrix * point );
        // +-+
        point.z = trf_bb.z;
        corners.push_back( matrix * point );
        // --+
        point.x = llc_bb.x;
        corners.push_back( matrix * point );
        // -++
        point.y = trf_bb.y;
        corners.push_back( matrix * point );
        // -+-
        point.z = llc_bb.z;
        corners.push_back( matrix * point );
        // ---
        point.y = llc_bb.y;
        corners.push_back( matrix * point );
  
        vector< Vec3f >::iterator i = corners.begin();
        Vec3f min = *i;
        Vec3f max = *i;
        i++;
        for( ; i != corners.end(); ++i ) {
          if( (*i).x < min.x ) min.x = (*i).x;
          if( (*i).y < min.y ) min.y = (*i).y;
          if( (*i).z < min.z ) min.z = (*i).z;
          if( (*i).x > max.x ) max.x = (*i).x;
          if( (*i).y > max.y ) max.y = (*i).y;
          if( (*i).z > max.z ) max.z = (*i).z;
        }
        value = max - min;
      }
    };
    
    /// Constructor.
    TransformedBoxBound( 
                        Inst< Center > _center = 0,
                        Inst< Size > _size   = 0,
                        Inst< SFMatrix4f >  _matrix = 0,
                        Inst< SFBoxBound >  _boxBound = 0 ) :
      BoxBound( _center, _size ),
      matrix( _matrix ),
      boxBound( _boxBound ) {
      
      boxBound->route( center );
      matrix->route( center );
      
      boxBound->route( size );
      matrix->route( size );
    }
    /// The matrix to transform with.
    auto_ptr< SFMatrix4f >  matrix;
    /// The BoxBound which values are to be transformed. 
    auto_ptr< SFBoxBound >  boxBound;
  };


  /// \internal
  /// Namespace for some help functions for Bound. 
  namespace BoundInternal { 
    template< class Iterator >
    inline Bound *SFBoundGetBound( const Iterator &i ) {
      return static_cast< RefCountSField< Bound > * >(*i)->getValue();
    }
    
    template< class Iterator >
    inline Bound *boundGetBound( const Iterator &i ) {
      return *i;
    }
    
    /// Returns a Bound that is the union between all the bounds
    /// 
    template< class Iterator >
    inline Bound *boundUnionBase( Iterator begin, 
                                  Iterator end,
                                  Bound *(*getBound) ( const Iterator & ) ) {
      BoxBound *box_bound = NULL;
      for( Iterator i = begin; i != end; i++ ) {
        Bound *b = getBound( i );
        if( b ) {
          BoxBound *bb = dynamic_cast< BoxBound * >( b );
          
          if( bb ) {
            if( !box_bound ) {
              box_bound = new BoxBound;
              box_bound->center->setValue( bb->center->getValue() );
              box_bound->size->setValue( bb->size->getValue() );
            } else {
              const Vec3f &union_center    = box_bound->center->getValue(); 
              const Vec3f &half_union_size = box_bound->size->getValue()/2.0; 
              const Vec3f &bb_center       = bb->center->getValue();
              const Vec3f &half_bb_size    = bb->size->getValue()/2.0;
              
              Vec3f trf_union = union_center + half_union_size;
              Vec3f llc_union = union_center - half_union_size;
              Vec3f trf_bb = bb_center + half_bb_size;
              Vec3f llc_bb = bb_center - half_bb_size;
              
              Vec3f trf_new = 
                Vec3f( trf_union.x > trf_bb.x ? trf_union.x: trf_bb.x,
                       trf_union.y > trf_bb.y ? trf_union.y: trf_bb.y,
                       trf_union.z > trf_bb.z ? trf_union.z: trf_bb.z );
              
              Vec3f llc_new = 
                Vec3f( llc_union.x < llc_bb.x ? llc_union.x: llc_bb.x,
                       llc_union.y < llc_bb.y ? llc_union.y: llc_bb.y,
                       llc_union.z < llc_bb.z ? llc_union.z: llc_bb.z );
              Vec3f new_size = trf_new - llc_new;
              Vec3f new_center = llc_new + new_size / 2.0;
              box_bound->center->setValue( new_center );
              box_bound->size->setValue( new_size );
            }
          } else {
            InfiniteBound *ib = dynamic_cast< InfiniteBound * >( b );
            if( ib ) {
              if( box_bound ) delete box_bound;
              return new InfiniteBound;
            } else {
              EmptyBound *eb = dynamic_cast< EmptyBound * >( b );
              if( eb ) {
                // do nothing
              } else {
                stringstream s;
                s << "Unsupported Bound type " 
                  << typeid( *(b) ).name();
                throw Exception::H3DAPIException( s.str(), H3D_FULL_LOCATION );
              }
            }
          }
        }
      }
      if( box_bound )
        return box_bound;
      else 
        // There were no Bounds specified by the iterators so return an 
        // EmptyBound.
        return new EmptyBound;
    }
  }

  template< class Iterator >
  inline Bound *Bound::boundUnion( Iterator begin, 
                                   Iterator end ) {
    using namespace BoundInternal;
    return boundUnionBase( begin, end, boundGetBound< Iterator > );
  }
  
  template< class Iterator >
  inline Bound *Bound::SFBoundUnion( Iterator begin, 
                                     Iterator end ) {
    using namespace BoundInternal;
    return boundUnionBase( begin, end, SFBoundGetBound< Iterator > );
  }
}

#endif
