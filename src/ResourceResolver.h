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
/// \file ResourceResolver.h
/// \brief Header file for ResourceResolver.
///
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __RESOURCERESOLVER_H__
#define __RESOURCERESOLVER_H__

#include "H3DApi.h"
#include <string>
using namespace std;
#include "URNResolver.h"
#include "AutoPtrVector.h"


namespace H3D {

  /// The ResourceResolver class is both a base class for all classes
  /// that resolves resource names and contains static functions for
  /// resolving them.
  class H3DAPI_API ResourceResolver {
  public:

    /// Destructor.
    virtual ~ResourceResolver() {}

    /// This function must be implemeted by all subclasses to 
    /// ResourceResolver. It returns a local filename that contains
    /// the resource specified by url.
    virtual string resolveURLAsTmpFile( const string &url ) = 0;

    /// Set the URNResolver to use when resolving resource.
    static void setURNResolver( URNResolver *resolver ) {
      urn_resolver.reset( resolver );
    } 

    /// Get the current URNResolver.
    static URNResolver* getURNResolver() {
      return urn_resolver.get();
    } 

    /// Add a ResourceResolver that can be used when resolving resources. 
    static void addResolver( ResourceResolver *resolver ) {
      resolvers.push_back( resolver );
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
    /// by urn.
    static string resolveURLAsFile( const string &urn );

  protected:
    static auto_ptr< URNResolver > urn_resolver;
    static AutoPtrVector< ResourceResolver > resolvers;
    static string baseURL;
  };
}

#endif
