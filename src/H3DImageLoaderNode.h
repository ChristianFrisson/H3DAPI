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
/// \file H3DImageLoaderNode.h
/// \brief Header file for H3DImageLoaderNode, the abstract base
/// class for all image loaders for different file formats.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __H3DIMAGELOADERNODE_H__
#define __H3DIMAGELOADERNODE_H__

#include "Image.h"
#include "Node.h"
#include <list>

namespace H3D {

  /// \ingroup AbstractNodes
  /// \class H3DImageLoaderNode
  /// \brief H3DImageLoaderNode is a virtual base class for classes that read
  /// an image file format such as PNG or JPEG and construct an Image class
  /// with the data from the file.
  ///
  /// Each subclass must define the loadImage () function. 
  /// It is used by e.g. the ImageTexture node.
  ///
  class H3DAPI_API H3DImageLoaderNode : public Node {
  public:
    typedef H3DImageLoaderNode*( *CreateNodeFunc)(); 

    /// Function ptr type for  
    typedef bool ( *SupportsFileFunc)( const string &url ); 
    
    template< class N >
    static H3DImageLoaderNode *newImageLoaderNode() { return new N; };

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
		  
        if( !H3DImageLoaderNode::initialized ) {
          H3DImageLoaderNode::registered_file_readers = 
            new list< FileReaderRegistration >;
          initialized = true;
        }
        H3DImageLoaderNode::registerFileReader( *this );
      }

      string name;
      CreateNodeFunc create_func;
      SupportsFileFunc supports_func;
    };
#ifdef __BORLANDC__
    friend struct FileReaderRegistration;
#endif

    /// Constructor.
    H3DImageLoaderNode() {
      type_name = "H3DImageLoaderNode";
    }

    /// Pure virtual function to load an Image from a url.
    /// \returns An Image * with the image data loaded from the
    /// url.
    ///
    virtual Image *loadImage( const string &url ) = 0;
    /// TODO: Implement 
    ////virtual Image *loadImage( const istream &is ) = 0;
    
    /// Returns the default xml containerField attribute value.
    /// For this node it is "imageLoader".
    ///
    virtual string defaultXMLContainerField() {
      return "imageLoader";
    }

        /// Given an url to a file, it returns an instance of a H3DSoundFileNode
    /// class that can handle that file type. If no such class is registered
    /// NULL is returns.
    static H3DImageLoaderNode *getSupportedFileReader( const string &url );

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
