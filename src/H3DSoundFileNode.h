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
/// \file H3DSoundFileNode.h
/// \brief Header file for H3DSoundFileNode, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __H3DSOUNDFILENODE_H__
#define __H3DSOUNDFILENODE_H__

#include "H3DSoundStreamNode.h"
#include <list>

namespace H3D {

  /// \ingroup AbstractNodes
  /// \class H3DSoundFileNode
  /// \brief This abstract node type is used to derive node types that
  /// can stream PCM sound data from a file.
  ///
  /// It also upholds a database over all subclasses that registers
  /// itself. The getSupportedFileReader function can then be used to
  /// get an instance of a H3DSoundFileNode that supports a certain
  /// file type.
  class H3DAPI_API H3DSoundFileNode : public H3DSoundStreamNode {
  public:
    /// 
    typedef H3DSoundFileNode*( *CreateNodeFunc)(); 

    /// Function ptr type for  
    typedef bool ( *SupportsFileFunc)( const string &url ); 
    
    template< class N >
    static H3DSoundFileNode *newSoundFileNode() { return new N; };

    /// Class used to register a class to the registered file readers.
    struct H3DAPI_API FileReaderRegistration{
    public:
      /// Constructor.
      FileReaderRegistration( const string &_name,
                              CreateNodeFunc _create, 
                              SupportsFileFunc _supports ):
      name( _name ),
      create_func( _create ),
      supports_func( _supports ) {
        if( !H3DSoundFileNode::initialized ) {
          H3DSoundFileNode::registered_file_readers = 
            new list< FileReaderRegistration >;
          initialized = true;
        }
        H3DSoundFileNode::registerFileReader( *this );
      }

      string name;
      CreateNodeFunc create_func;
      SupportsFileFunc supports_func;
    };

    /// Load a sound file from the given url that will be used to
    /// generate PCM data.
    virtual unsigned int load( const string &_url ) = 0;
    
    /// Given an url to a file, it returns an instance of a H3DSoundFileNode
    /// class that can handle that file type. If no such class is registered
    /// NULL is returns.
    static H3DSoundFileNode *getSupportedFileReader( const string &url );

    /// Register a file reader that can then be returned by 
    /// getSupportedFileReader().
    /// \param name The name of the class
    /// \param create A function for creating an instance of that class.
    /// \param supports A function to determine if the class supports a
    /// given file type.
    static void registerFileReader( const string &name,
                                    CreateNodeFunc create, 
                                    SupportsFileFunc supports ) {
      registerFileReader( FileReaderRegistration( name, create, supports ) );
    }

    /// Register a file reader that can then be returned by 
    /// getSupportedFileReader().
    static void registerFileReader( const FileReaderRegistration &fr ) {
      registered_file_readers->push_back( fr );
    }

  protected:
    static list< FileReaderRegistration > *registered_file_readers;
    static bool initialized;
  };
}

#endif
