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
/// \file Instantiate.h
/// \brief Contains the Inst struct.
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __INSTANTIATE_H__
#define __INSTANTIATE_H__

namespace H3D {
  // Inst guarantees an instance of the class T if one
  // isn't already given to the constructor. Used to
  // ensure that Fields are initialised in the right
  // constructor in the Nodes.
  template< class T >
  struct Inst {
    Inst( T *_ptr ) : ptr( _ptr ? _ptr : new T )  {}
    operator T*() { return ptr; }
    template< class S >
    operator Inst< S >() const throw() {
      return Inst< S >( ptr );
    }
  private:
    T *ptr;
  };
}

#endif
