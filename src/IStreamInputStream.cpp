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
/// \file IStreamInputStream.cpp
/// \brief CPP file for the IStreamInputStream class.
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/IStreamInputStream.h>

#ifdef HAVE_XERCES

using namespace H3D;

  
#if(XERCES_VERSION_MAJOR > 2 )  

XMLFilePos X3D::IStreamInputStream::curPos() const {
  return total_count;
}
      
XMLSize_t X3D::IStreamInputStream::readBytes( 
                                          XMLByte* const to_fill,
                                          const XMLSize_t max_to_read ) {
  is.read( (char *) to_fill, max_to_read );
  return (unsigned int) is.gcount();
}
#else
unsigned int X3D::IStreamInputStream::curPos() const {
  return total_count;
}
      
unsigned int X3D::IStreamInputStream::readBytes( 
                                          XMLByte* const to_fill,
                                          const unsigned int max_to_read ) {
  is.read( (char *) to_fill, max_to_read );
  return (unsigned int) is.gcount();
}
#endif

#endif //HAVE_XERCES
