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
/// \file ProfilesAndComponents.h
/// \brief Header file for ProfilesAndComponents, classes for navigation
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __PROFILESANDCOMPONENTS_H__
#define __PROFILESANDCOMPONENTS_H__

#include <H3D/H3DApi.h>
#include <H3D/ProfileSAX2Handlers.h>
#include <string>
#include <vector>
#include <map>

#ifdef HAVE_XERCES

namespace H3D {
  
  /// ProfilesAndComponents is a Singleton class used for taking care of
  /// profile and component conformance in H3DAPI
  /// At the moment no checking is done when creating nodes that this node
  /// exists in the given profile. This file currently only checks whether some
  /// definitions for profile, version and components is correct.
  /// Used by x3d-parser (X3DSAX2Handlers.cpp). Vrml is not yet supported.
  class H3DAPI_API ProfilesAndComponents {
  public:

    /// destroy the instance when not needed anymore
    static void destroy();

    /// Called from the (x3d-)parser when a profile name is found.
    /// Create instance of itself if not already created then
    /// call setProfileInternal.
    /// \param profile Name of profile.
    /// \param err_msg String containing error message if function fails.
    /// \param _version String with version number.
    /// \returns True if profile is set, false otherwise.
    static bool setProfile( std::string profile, std::string &err_msg,
                            std::string _version );

    /// Called from the (x3d-)parser when a component name is found.
    /// Create instance of itself if not already created then
    /// call addComponentInternal.
    /// \param component Name of component.
    /// \param level Support level of component.
    /// \param err_msg String containing error message if function fails.
    /// \returns True if profile is set, false otherwise.
    static bool addComponent( std::string component,
                              int level, std::string &err_msg );

    /// returns a string containing the currently selected profile.
    /// used for debug purposes.
    static std::string getProfile();

    /// Called by (x3d-)parser when the first file with a profile
    /// and component definition is set.
    /// \param _main_profile_set Value used when setting flag.
    static void setMainProfileDone( bool _main_profile_set );

    /// Set to false before starting an application ( or loading a file )
    /// to not do any profile and component checking.
    static bool check_profiles_components;

  private:
    /// If the profile name and version is valid
    /// store it as the profile to use. If main_profile_set is true
    /// the given profile string and version is compared
    /// to already set profile and component and checks if something is not
    /// correct. Store error messages in err_msg if something goes wrong.
    bool setProfileInternal( std::string profile, std::string &err_msg,
                             std::string _version );

    /// If the component exists add it to components_used vector unless
    /// it violates some rules from the x3d-spec. Store error messages in
    /// err_msg if something goes wrong.
    bool addComponentInternal( std::string component,
                               int level, std::string &err_msg );

    /// Find a component named name in the given component_vector.
    bool findComponent( ProfileSAX2Handlers::myX3DComponentVector &component_vector,
                        std::string name, int &place );

    // Temporary solution to know which profiles H3DAPI supports.
    std::map< std::string, std::vector< std::string > > profiles_supported;
    ProfileSAX2Handlers::myX3DProfile used_profile;
    std::string version;
    bool main_profile_set;
    std::map< std::string, std::map< std::string, int > > components_supported;
    ProfileSAX2Handlers::myX3DComponentVector components_used;

    static ProfilesAndComponents * instance;

    // Private declarations of constructor for singleton class.
    ProfilesAndComponents();
    ProfilesAndComponents(const ProfilesAndComponents&) {}
    ProfilesAndComponents& operator =
      (const ProfilesAndComponents&) { return *this; }
    ~ProfilesAndComponents() {}
  };

}

#endif
#endif
