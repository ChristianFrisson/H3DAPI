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
/// \file H3DVideoClipDecoderNode.h
/// \brief Header file for H3DVideoClipDecoderNode, the abstract base
/// class for all nodes the decodes video clips.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __H3DVIDEOCLIPDECODERNODE_H__
#define __H3DVIDEOCLIPDECODERNODE_H__

#include "Node.h"
#include "Image.h"
#include <list>

namespace H3D {

  /// \ingroup AbstractNodes
  /// \class H3DVideoClipDecoderNode
  /// \brief H3DVideoClipDecoderNode is a virtual base class for classes
  /// decoding video clips making new frames available.
  ///
  /// Each subclass must define the loadImage () function. 
  /// It is used by e.g. the MovieTexture node.
  ///
  class H3DVideoClipDecoderNode : public Node {
  public:
    typedef H3DVideoClipDecoderNode*( *CreateNodeFunc)(); 

    /// Function ptr type for  
    typedef bool ( *SupportsFileFunc)( const string &url ); 
    
    template< class N >
    static H3DVideoClipDecoderNode *newImageLoaderNode() { return new N; };

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
		  
        if( !H3DVideoClipDecoderNode::initialized ) {
          H3DVideoClipDecoderNode::registered_file_readers = 
            new list< FileReaderRegistration >;
          initialized = true;
        }
        H3DVideoClipDecoderNode::registerFileReader( *this );
      }

      string name;
      CreateNodeFunc create_func;
      SupportsFileFunc supports_func;
    };

    /// Constructor.
    H3DVideoClipDecoderNode() {
      type_name = "H3DVideoClipDecoderNode";
    }

    /// Pure virtual function to load an Image from a url.
    /// \returns An Image * with the image data loaded from the
    /// url.
    ///
    virtual bool loadClip( const string &url ) = 0;

    virtual bool haveNewFrame() = 0;
    virtual void getNewFrame( unsigned char *buffer ) = 0;
    virtual H3DInt32 getFrameWidth() = 0;
    virtual H3DInt32 getFrameHeight() = 0;
    virtual Image::PixelType getFramePixelType() = 0;
    virtual Image::PixelComponentType getFramePixelComponentType() = 0;
    virtual void startPlaying() = 0;
    virtual void stopPlaying() = 0;
    
    /// Returns the default xml containerField attribute value.
    /// For this node it is "imageLoader".
    ///
    virtual string defaultXMLContainerField() {
      return "decoder";
    }

    /// Given an url to a file, it returns an instance of a 
    /// H3DVideoClipDecoderNode
    /// class that can handle that file type. If no such class is registered
    /// NULL is returns.
    static H3DVideoClipDecoderNode *getSupportedFileReader( const string &url );

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
