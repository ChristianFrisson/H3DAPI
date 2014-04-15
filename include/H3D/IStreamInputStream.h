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
/// \file IStreamInputStream.h
/// \brief Header file for the IStreamInputStream class.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __ISTREAMINPUTSTREAM_H__
#define __ISTREAMINPUTSTREAM_H__

#include <H3D/H3DApi.h>

#ifdef HAVE_XERCES
#include <xercesc/util/BinInputStream.hpp>
#include <iostream>

using namespace std;
XERCES_CPP_NAMESPACE_USE

namespace H3D {
  namespace X3D {
    /// This class implements the BinInputStream interface to read
    /// from a istream
    class IStreamInputStream : public BinInputStream {
    public :
      /// Constructor.
      IStreamInputStream ( istream &_is ) : is( _is ), total_count( 0 ) {}
      
#if(XERCES_VERSION_MAJOR > 2 )  
      virtual XMLFilePos curPos() const;
      
      virtual XMLSize_t readBytes( XMLByte* const toFill,
                                   const XMLSize_t maxToRead );

      virtual const XMLCh* getContentType() const {
        return 0;
      }
#else
      virtual unsigned int curPos() const;
      
      virtual unsigned int readBytes( XMLByte* const toFill,
                                   const unsigned int maxToRead );

#endif
      
    protected :
      //BinFileInputStream(const BinFileInputStream&);
      // BinFileInputStream& operator=(const BinFileInputStream&);   
  
      istream &is;
      unsigned int total_count;
      
    };
  }
}

#endif
#endif
