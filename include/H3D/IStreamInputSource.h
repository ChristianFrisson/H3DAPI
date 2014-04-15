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
/// \file IStreamInputSource.h
/// \brief Header file for the IStreamInputSource class.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __ISTREAMINPUTSOURCE_H__
#define __ISTREAMINPUTSOURCE_H__

#include <H3D/H3DApi.h>

#ifdef HAVE_XERCES

#include <xercesc/sax/InputSource.hpp>
#include <iostream>

using namespace std;
XERCES_CPP_NAMESPACE_USE

namespace H3D {
  namespace X3D {
    
    ///
    /// This class is a derivative of the standard Xerxes C InputSource class. 
    /// It provides the parser access to data via a general istream. 
    /// 
    class H3DAPI_API IStreamInputSource : public InputSource {
    public :
      /// Constructor.
      /// \param _is the istream to use as input to the parser.
      /// \param manager Pointer to the memory manager to be used to 
      /// allocate objects.
      IStreamInputSource( istream &_is,
                          MemoryManager* const manager = 
                          XMLPlatformUtils::fgMemoryManager ) : 
        InputSource( manager ),
        is( _is ) {}
      
      /// Constructor.
      /// \param _is the istream to use as input to the parser.
      /// \param system_id The system identifier(URI).
      /// \param manager Pointer to the memory manager to be used to 
      /// allocate objects.
      IStreamInputSource( istream &_is,
                          const XMLCh *const system_id,
                          MemoryManager* const manager = 
                          XMLPlatformUtils::fgMemoryManager ):
        InputSource( system_id, manager ),
        is( _is ) {}
      
      ///
      /// This method will return a binary input stream derivative that will
      /// parse from the standard input of the local host.
      ///
      /// \return A dynamically allocated binary input stream derivative that
      ///         can parse from the standardinput.
      ///
      BinInputStream* makeStream() const;

    protected:
      istream &is;
    public:
      /// unimplemented copy constructors
      IStreamInputSource( const IStreamInputSource & );
      /// unimplemented assignment operator
      IStreamInputSource& operator=(const IStreamInputSource&);
    };
  };
}

#endif
#endif
