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
/// \file MFNodeAutoRefVector.h
/// \brief Contains the MFNodeAutoRefVector class.
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __MFNODEAUTOREFVECTOR_H__
#define __MFNODEAUTOREFVECTOR_H__

#include <H3DUtil/AutoRefVector.h>

namespace H3D {

  // forward declaration
  template< class RefClass >
  class RefCountMField;
  
  namespace FieldInternals {

#if _MSC_VER >= 1500
#pragma warning( disable : 4275 )
#endif
    /// \internal
    /// The MFNodeAutoRefVector is a class used in MFNode in order to 
    /// call the onAdd and onRemove functions when
    /// the values of the vector are changed.
    ///
    template< class RefClass >
    class H3DAPI_API MFNodeAutoRefVector: public AutoRefVector< RefClass > {
    public :
#if _MSC_VER >= 1500
#pragma warning( default : 4275 )
#endif

      /// Creates an empty vector.
      inline MFNodeAutoRefVector() {}
      
      /// Creates a vector with n elements.
      inline MFNodeAutoRefVector( typename AutoRefVector< RefClass >::size_type n ):
        AutoRefVector< RefClass >( n ) {}
      
      /// Destructor.
      inline virtual ~MFNodeAutoRefVector() {
        this->clear();
      }
      
      /// Assignment operator.
      inline MFNodeAutoRefVector &operator=( 
                                            const MFNodeAutoRefVector &v ) {
        AutoRefVector< RefClass >::operator=( v );
        return *this;
      }
      
      /// Assignment operator.
      inline MFNodeAutoRefVector &operator=( 
                                            const AutoRefVector< RefClass > &v ) {
        AutoRefVector< RefClass >::operator=( v );
        return *this;
      }
      
      /// Assignement operator.
      inline MFNodeAutoRefVector &operator=( 
                                            const vector< RefClass *> &v ) {
        AutoRefVector< RefClass >::operator=( v );
        return *this;
      }
      
      /// This function is called when a Node * added to the AutoRefVector.
      /// It increments the reference counter of the Node by calling the 
      /// ref() function. Subclasses can override this function in order
      /// to get specialized behaviour.
      /// \param n The node that is added to the the AutoRefVector
      ///
      virtual void ref( RefClass *n ) const {
        if( n ) {
          AutoRefVector< RefClass >::ref( n );
          owner->onAdd( owner->preOnAdd( n ) );
        }
      }
      
      /// This function is called when a Node * is removed from the 
      /// AutoRefVector. 
      /// It decrements the reference counter of the Node by calling the 
      /// unref() function. Subclasses can override this function in 
      /// order to get specialized behaviour.
      /// \param n The node being removed from the AutoRefVector.
      /// 
      virtual void unref( RefClass *n ) const {
        if( n ) {
          owner->onRemove( owner->preOnRemove( n ) );
          AutoRefVector< RefClass >::unref( n );
        }
      }
      
      /// The field the vector resides in.
      RefCountMField< RefClass > *owner;
    };
  }
}



#endif
