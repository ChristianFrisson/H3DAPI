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
/// \file ResourceResolver.h
/// \brief Header file for ResourceResolver.
///
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __RESOURCERESOLVER_H__
#define __RESOURCERESOLVER_H__

#include <H3D/H3DApi.h>
#include <string>
#include <list>
using namespace std;
#include <H3D/URNResolver.h>
#include <H3DUtil/AutoPtrVector.h>
#include <memory>



namespace H3D {

  /// The ResourceResolver class is both a base class for all classes
  /// that resolves resource names and contains static functions for
  /// resolving them.
  class H3DAPI_API ResourceResolver {
  public:

    /// List of tmpfile names
    class TmpFileNameList: public list< string > {
    public:
      ~TmpFileNameList() {
        for( list< string >::iterator i = begin();
           i != end(); ++i ) {
          std::remove( (*i).c_str() );
        }
      }
    };

    /// Destructor.
    virtual ~ResourceResolver() {}

    /// This function should be implemented by resource resolvers that
    /// extracts file data. It returns a local filename that contains
    /// the resource specified by url.
    virtual string resolveURLAsTmpFile( const string &url ){ return ""; }

    /// This function should be implemented by resource resolvers that
    /// extracts folder tree data. It returns a local folder that contains
    /// the resource data specified by url.
    virtual string resolveURLAsTmpFolder( const string &url ){ return ""; }

    /// Set the URNResolver to use when resolving resource.
    static void setURNResolver( URNResolver *resolver ) {
      urn_resolver().reset( resolver );
    } 

    /// Get the current URNResolver.
    static URNResolver* getURNResolver() {
      return urn_resolver().get();
    } 

    /// Add a ResourceResolver that can be used when resolving resources. 
    static void addResolver( ResourceResolver *resolver ) {
      resolvers().push_back( resolver );
    }

    /// Set the current base URL. The base URL will be used as the base
    /// when the url to resolve is a relative url.
    static void setBaseURL( const string &base ) {
      baseURL = base;
    }

    /// Get the current base URL,
    static const string & getBaseURL() {
      return baseURL;
    }

    /// Returns a local filename that contains the resource specified
    /// by urn. The boolean pointed to by the is_tmp_file argument 
    /// is set to true if the resolved file is a temporary file.
    static string resolveURLAsFile( const string &urn,
                                    bool *is_tmp_file = NULL ){
      return resolveURLAs(urn,is_tmp_file,false,false);
    }

    /// Returns a local filename that contains the resource specified
    /// by urn. The boolean pointed to by the is_tmp_folder argument 
    /// is set to true if the resolved file is a temporary folder.
    static string resolveURLAsFolder( const string &urn,
                                      bool *is_tmp_folder = NULL ){
      return resolveURLAs(urn,is_tmp_folder,true,false);
    }

    /// Returns a string containing the contents of the url resource
    ///
    /// This function should be implemented by resource resolvers that
    /// extract file data and wish to avoid the use of temporary files
    ///
    /// \param[in]   urn     The URL to resolve
    static string resolveURLAsString ( const string &urn ) {
      return resolveURLAs(urn,NULL,false,true);
    }

    /// Returns a new unique filename that can be used to create a temporary
    /// file. The filename should be released as soon as it is not needed
    /// any more with the releaseTmpFileName function. 
    static string getTmpFileName();

    /// Remove a temporary file. Returns true on success or false if the 
    /// tmpfile does not exist or have not been allocated with the 
    /// getTmpFileName function. 
    static bool releaseTmpFileName( const string &file );

  protected:
    
    /// Returns a string containing the contents of the url resource
    ///
    /// This function should be implemented by resource resolvers that
    /// extract file data and wish to avoid the use of temporary files.
    ///
    /// \param[in]   url     The URL to resolve
    /// \return The contents of the url or an empty string on failure
    ///
    virtual string resolveURLAsStringInternal( const string &url ) { return ""; }

    /// Resolves a URN and returns either a local file name or the file
    /// contents itself.
    ///
    /// \param[in] urn              The URN to resolve.
    /// \param[out] is_tmp_file     Will be set to true if the filename returned is temporary
    ///                             the caller should then remove the file once it is no longer 
    ///                             needed.
    /// \param[in] folder           True if the resource is a folder.
    /// \param[in] return_contents  If true then the file contents is returned, 
    ///                             otherwise the local file name is returned.
    static string resolveURLAs( const string &urn,
                                bool *is_tmp_file,
                                bool folder,
                                bool return_contents= false );
    
    static auto_ptr< URNResolver > & urn_resolver();
    
    static H3DUtil::AutoPtrVector< ResourceResolver > & resolvers() {
      static H3DUtil::AutoPtrVector< ResourceResolver > resolvers;
      return resolvers;
    }
    
    static string baseURL;
    static TmpFileNameList tmp_files;
  };
}

#endif
