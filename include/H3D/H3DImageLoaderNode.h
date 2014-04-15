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
/// \file H3DImageLoaderNode.h
/// \brief Header file for H3DImageLoaderNode, the abstract base
/// class for all image loaders nodes for different file formats.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __H3DIMAGELOADERNODE_H__
#define __H3DIMAGELOADERNODE_H__

#include <H3DUtil/Image.h>
#include <H3D/Node.h>
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

    /// Function ptr type for callback to ask if loader supports a url
    typedef bool ( *SupportsFileFunc)( const string &url );

    /// Function ptr type for callback to ask if loader supports a stream
    typedef bool ( *SupportsStreamFunc)( istream &is );
    
    template< class N >
    static H3DImageLoaderNode *newImageLoaderNode() { return new N; };

    /// Class used to register a class to the registered file readers.
    struct H3DAPI_API FileReaderRegistration{
    public:
      /// Constructor.
      FileReaderRegistration( const string &_name,
                              CreateNodeFunc _create, 
                              SupportsFileFunc _supports,
                              SupportsStreamFunc _supports_stream= NULL ):
      name( _name ),
      create_func( _create ),
      supports_func( _supports ),
      supports_stream_func ( _supports_stream ) {

        if( !H3DImageLoaderNode::initialized ) {
          H3DImageLoaderNode::registered_file_readers.reset(
            new list< FileReaderRegistration > );
          initialized = true;
        }
        H3DImageLoaderNode::registerFileReader( *this );
      }

      string name;
      CreateNodeFunc create_func;
      SupportsFileFunc supports_func;
      SupportsStreamFunc supports_stream_func;
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

    /// Load an image from an istream
    ///
    /// Does not need to be implemented for all image loaders
    ///
    /// \returns An Image * with the image data loaded from the
    /// url. Returns NULL if not implemented for image loader.
    ///
    virtual Image *loadImage( istream &is ) { return NULL; }
    
    /// Returns the default xml containerField attribute value.
    /// For this node it is "imageLoader".
    ///
    virtual string defaultXMLContainerField() {
      return "imageLoader";
    }

    /// Given an url to a file, it returns an instance of a H3DImageLoaderNode
    /// class that can handle that file type. If no such class is registered
    /// NULL is returned.
    static H3DImageLoaderNode *getSupportedFileReader( const string &url );

    /// Given an istream, it returns an instance of a H3DImageLoaderNode
    /// class that can handle that image type. If no such class is registered
    /// NULL is returned.
    static H3DImageLoaderNode *getSupportedFileReader( istream &is );

    /// Register a file reader that can then be returned by 
    /// getSupportedFileReader().
    /// \param name The name of the class
    /// \param create A function for creating an instance of that class.
    /// \param supports A function to determine if the class supports a
    /// given file type.
    /// \param supports_stream A function to determine if the class supports loading
    /// from a file stream.
    static void registerFileReader( const string &name,
                                    CreateNodeFunc create, 
                                    SupportsFileFunc supports,
                                    SupportsStreamFunc supports_stream= NULL ) {
      registerFileReader( FileReaderRegistration( name, create, supports, supports_stream ) );
    }

    /// Register a file reader that can then be returned by 
    /// getSupportedFileReader().
    static void registerFileReader( const FileReaderRegistration &fr ) {
      registered_file_readers->push_back( fr );
    }

    // Creating a new auto_ptr local for this node, because 
    // registrated_file_reader caused a memory leak and because
    // of the order of setting the static variables the auto_ptr's
    // constructor resets the auto_ptr to 0 eventhough the 
    // registered_file_readers has been initilazed, and therefore
    // cause an error making it impossible to use the standard auto_ptr.
    template<class T>
    class local_auto_ptr{
    private:
      T* ap;    // refers to the actual owned object (if any)
    public:
      typedef T element_type;

      // constructor
      explicit local_auto_ptr (T* ptr = 0) {
        if(!initialized){
          ap=ptr;
        }
      }
      
      // copy constructors (with implicit conversion)
      // - note: nonconstant parameter
      local_auto_ptr (local_auto_ptr& rhs) throw() : ap(rhs.release()) { }

      template<class Y>
      local_auto_ptr (local_auto_ptr<Y>& rhs) throw() : ap(rhs.release()) { }
      
      // assignments (with implicit conversion)
      // - note: nonconstant parameter
      local_auto_ptr& operator= (local_auto_ptr& rhs) throw(){
        if(!initialized){  
          reset(rhs.release());
          return *this;
        }
      }
      template<class Y>
      local_auto_ptr& operator= (local_auto_ptr<Y>& rhs) throw(){
        if(!initialized){
          reset(rhs.release());
          return *this;
        }
      }

      // destructor
      ~local_auto_ptr() throw(){
        delete ap;
      }

      // value access
      T* get() const throw(){
        return ap;
      }
      T& operator*() const throw(){
        return *ap;
      }
      T* operator->() const throw(){
        return ap;
      }

      // release ownership
      T* release() throw(){
        if(!initialized){
          T* tmp(ap);
          ap = 0;
          return tmp;
        }
      }

      // reset value
      void reset (T* ptr=0) throw(){
        if(!initialized){
          if (ap != ptr){
            delete ap;
            ap = ptr;
          }
        }
      }
    };

  protected:
    static local_auto_ptr< list< FileReaderRegistration > > registered_file_readers;
    //static list< FileReaderRegistration > *registered_file_readers;
    static bool initialized;
  };
}

#endif
