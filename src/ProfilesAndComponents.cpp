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
/// \file ProfilesAndComponents.cpp
/// \brief CPP file for ProfilesAndComponents, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/ProfilesAndComponents.h>

#ifdef HAVE_XERCES

#include <H3DUtil/Console.h>

#include <algorithm>
#include <iostream>
#include <sstream>
#include <fstream>
#include <memory>

using namespace H3D;
using namespace std;

ProfilesAndComponents * ProfilesAndComponents::instance = 0;
bool ProfilesAndComponents::check_profiles_components = false;

namespace ProfilesAndComponentsInternal {
  SAX2XMLReader* getNewXMLParser() {
    SAX2XMLReader::ValSchemes    valScheme    = SAX2XMLReader::Val_Never;
    bool                         doNamespaces = true;
    bool                         doSchema = false;
    bool                         schemaFullChecking = false;
    bool                         namespacePrefixes = false;
    char                         localeStr[64];
    memset(localeStr, 0, sizeof localeStr);
    SAX2XMLReader* parser = XMLReaderFactory::createXMLReader();
    parser->setFeature(XMLUni::fgSAX2CoreNameSpaces, doNamespaces);
    parser->setFeature(XMLUni::fgXercesSchema, doSchema);
    parser->setFeature(XMLUni::fgXercesSchemaFullChecking, 
      schemaFullChecking);
    parser->setFeature(XMLUni::fgSAX2CoreNameSpacePrefixes, 
      namespacePrefixes);
    if (valScheme == SAX2XMLReader::Val_Auto) {
      parser->setFeature(XMLUni::fgSAX2CoreValidation, true);
      parser->setFeature(XMLUni::fgXercesDynamic, true);
    }
    if (valScheme == SAX2XMLReader::Val_Never) {
      parser->setFeature(XMLUni::fgSAX2CoreValidation, false);
    }
    if (valScheme == SAX2XMLReader::Val_Always) {
      parser->setFeature(XMLUni::fgSAX2CoreValidation, true);
      parser->setFeature(XMLUni::fgXercesDynamic, false);
    }
    return parser;
  }
}

ProfilesAndComponents::ProfilesAndComponents(): main_profile_set( false ) {
  // Hard coded which profiles and components that are supported
  // by H3DAPI.
  profiles_supported["3.0"].push_back("Core");
  profiles_supported["3.0"].push_back("Interchange");
  // TODO: verify that immersive is supported since it is not yet
  // supported when coding this.
  profiles_supported["3.0"].push_back("Immersive");
  profiles_supported["3.0"].push_back("Interactive");

  profiles_supported["3.1"].push_back("Core");
  profiles_supported["3.1"].push_back("Interchange");
  // TODO: verify that immersive is supported since it is not yet
  // supported when coding this.
  profiles_supported["3.1"].push_back("Immersive");
  profiles_supported["3.1"].push_back("Interactive");

  profiles_supported["3.2"].push_back("Core");
  profiles_supported["3.2"].push_back("Interchange");
  // TODO: verify that immersive is supported since it is not yet
  // supported when coding this.
  profiles_supported["3.2"].push_back("Immersive");
  profiles_supported["3.2"].push_back("Interactive");
  // TODO: verify that Medicalinterchange is supported.
  profiles_supported["3.2"].push_back("MedicalInterchange");

  components_supported["3.0"]["Core"] = 2;
  components_supported["3.0"]["DIS"] = 0;
  components_supported["3.0"]["EnvironmentalEffects"] = 3;
  components_supported["3.0"]["EnvironmentalSensor"] = 2;
  components_supported["3.0"]["EventUtilities"] = 1;
  components_supported["3.0"]["Geometry2D"] = 2;
  components_supported["3.0"]["Geometry3D"] = 4;
  components_supported["3.0"]["Geospatial"] = 0;
  components_supported["3.0"]["Grouping"] = 2;
  components_supported["3.0"]["H3DAPI"] = 1;
  components_supported["3.0"]["H-Anim"] = 0;
  components_supported["3.0"]["Interpolation"] = 3;
  components_supported["3.0"]["KeyDeviceSensor"] = 2;
  components_supported["3.0"]["Lighting"] = 3;
  components_supported["3.0"]["Navigation"] = 2;
  components_supported["3.0"]["Networking"] = 1;
  components_supported["3.0"]["NURBS"] = 0;
  components_supported["3.0"]["PointingDeviceSensor"] = 1;
  components_supported["3.0"]["Rendering"] = 4;
  components_supported["3.0"]["Scripting"] = 1;
  components_supported["3.0"]["Shape"] = 3;
  components_supported["3.0"]["Sound"] = 1;
  components_supported["3.0"]["Text"] = 1;
  components_supported["3.0"]["Texturing"] = 3;
  components_supported["3.0"]["Time"] = 2;

  components_supported["3.1"]["CADGeometry"] = 0;
  components_supported["3.1"]["Core"] = 2;
  components_supported["3.1"]["CubeMapTexturing"] = 1;
  components_supported["3.1"]["DIS"] = 0;
  components_supported["3.1"]["EnvironmentalEffects"] = 3;
  components_supported["3.1"]["EnvironmentalSensor"] = 2;
  components_supported["3.1"]["EventUtilities"] = 1;
  components_supported["3.1"]["Geometry2D"] = 2;
  components_supported["3.1"]["Geometry3D"] = 4;
  components_supported["3.1"]["Geospatial"] = 0;
  components_supported["3.1"]["Grouping"] = 2;
  components_supported["3.1"]["H3DAPI"] = 1;
  components_supported["3.1"]["H-Anim"] = 0;
  components_supported["3.1"]["Interpolation"] = 3;
  components_supported["3.1"]["KeyDeviceSensor"] = 2;
  components_supported["3.1"]["Lighting"] = 3;
  components_supported["3.1"]["Navigation"] = 2;
  components_supported["3.1"]["Networking"] = 1;
  components_supported["3.1"]["NURBS"] = 0;
  components_supported["3.1"]["PointingDeviceSensor"] = 1;
  components_supported["3.1"]["Rendering"] = 4;
  components_supported["3.1"]["Scripting"] = 1;
  components_supported["3.1"]["Shaders"] = 1;
  components_supported["3.1"]["Shape"] = 3;
  components_supported["3.1"]["Sound"] = 1;
  components_supported["3.1"]["Text"] = 1;
  components_supported["3.1"]["Texturing"] = 3;
  components_supported["3.1"]["Texturing3D"] = 2;
  components_supported["3.1"]["Time"] = 2;

  components_supported["3.2"]["CADGeometry"] = 0;
  components_supported["3.2"]["Core"] = 2;
  components_supported["3.2"]["CubeMapTexturing"] = 1;
  components_supported["3.2"]["DIS"] = 0;
  components_supported["3.2"]["EnvironmentalEffects"] = 3;
  components_supported["3.2"]["EnvironmentalSensor"] = 2;
  components_supported["3.2"]["EventUtilities"] = 1;
  components_supported["3.2"]["Followers"] = 0;
  components_supported["3.2"]["Geometry2D"] = 2;
  components_supported["3.2"]["Geometry3D"] = 4;
  components_supported["3.2"]["Geospatial"] = 0;
  components_supported["3.2"]["Grouping"] = 2;
  components_supported["3.2"]["H-Anim"] = 0;
  components_supported["3.2"]["H3DAPI"] = 1;
  components_supported["3.2"]["Interpolation"] = 3;
  components_supported["3.2"]["KeyDeviceSensor"] = 2;
  components_supported["3.2"]["Layering"] = 0;
  components_supported["3.2"]["Layout"] = 0;
  components_supported["3.2"]["Lighting"] = 3;
  components_supported["3.2"]["Navigation"] = 3;
  components_supported["3.2"]["Networking"] = 1;
  components_supported["3.2"]["NURBS"] = 0;
  components_supported["3.2"]["ParticleSystems"] = 0;
  components_supported["3.2"]["PickingSensor"] = 0;
  components_supported["3.2"]["PointingDeviceSensor"] = 1;
  components_supported["3.2"]["Rendering"] = 5;
  components_supported["3.2"]["RigidBodyPhysics"] = 0;
  components_supported["3.2"]["Scripting"] = 1;
  components_supported["3.2"]["Shaders"] = 1;
  components_supported["3.2"]["Shape"] = 4;
  components_supported["3.2"]["Sound"] = 1;
  components_supported["3.2"]["Text"] = 1;
  components_supported["3.2"]["Texturing"] = 3;
  components_supported["3.2"]["Texturing3D"] = 2;
  components_supported["3.2"]["Time"] = 2;

  //TODO: verify support for VolumeRendering
  components_supported["3.2"]["VolumeRendering"] = 3;
}

void ProfilesAndComponents::destroy() {
  if( instance )
  {
    delete instance;
    instance = 0;
  }
}

string ProfilesAndComponents::getProfile() {
  if( instance )
    return instance->used_profile.name;
  else
    return "";
}

bool ProfilesAndComponents::setProfile( string profile, string &err_msg,
                                        string _version ) {
  if( check_profiles_components ) {
    if( !instance ) {
      instance = new ProfilesAndComponents();
    }

    return instance->setProfileInternal( profile, err_msg, _version );
  }
  return true;
}

bool ProfilesAndComponents::setProfileInternal( string profile,
                                                string &err_msg,
                                                string _version) {
  if( _version != "3.0" &&
      _version != "3.1" &&
      _version != "3.2" ) {
    err_msg = "The version number " + _version + " does not exist, " +
              "the profile file will not be found for profile" + profile +"\n";
    return false;
  }
  auto_ptr< SAX2XMLReader >
    parser( ProfilesAndComponentsInternal::getNewXMLParser() );

  // Try to find the profile file definition in list of 
  // supported profiles.
  if( find( profiles_supported[_version].begin(),
    profiles_supported[_version].end(),
    profile ) != profiles_supported[_version].end() ) {
      ProfileSAX2Handlers handler;
      parser->setContentHandler(&handler);
      parser->setErrorHandler(&handler);
      char *r = getenv( "H3D_ROOT" );
      string h3d_root = r ? r : "";
      string profile_path = h3d_root + "/Conformance/" + 
        _version +"/Profiles/" + profile + ".xml";
      ifstream profile_file( profile_path.c_str() );
      profile_file.close();
      if( !profile_file.fail() ) {
        parser->parse( profile_path.c_str() );
        ProfileSAX2Handlers::myX3DProfileVector parsed_profile =
          handler.getProfileVector();
        ProfileSAX2Handlers::myX3DProfile temp_profile
          = parsed_profile.front();

        // Treating a found profile in two ways. If there is no
        // profile and component definition set for the scene
        // set this one if it is a good one. Otherwise compare to
        // the one already set.
        if( !main_profile_set ) {
          used_profile = temp_profile;
          for( map< string, int >::iterator i =
            used_profile.component_names.begin();
            i != used_profile.component_names.end();
            ++i ) {
              string component_path = h3d_root + "/Conformance/" +
                _version +"/Components/" + (*i).first + ".xml";
              ifstream component_file( component_path.c_str() );
              component_file.close();
              if( !component_file.fail() ) {
                parser->parse( component_path.c_str() );
              }
              else {
                err_msg =
                  "File with component definition could not be found. " +
                  string( "Path used is:\n" ) + component_path + "\n";
                return false;
              }
          }
          components_used = handler.getComponentVector();
          for( int i = 0; i < (int)components_used.size(); ++i ) {
            components_used[i].conformance_level =
              components_supported[_version][ components_used[i].name ];
            components_used[i].used_level =
              used_profile.component_names[ components_used[i].name ];
          }
          version = _version;
          return true;
        }
        else {
          for( map< string, int >::iterator i =
               temp_profile.component_names.begin();
               i != temp_profile.component_names.end();
               ++i ) {
            int place;
            if( findComponent( components_used, (*i).first, place ) ) {
              /*stringstream stm;
              stm << (*i).second;
              if( components_used[ place ].used_level < (*i).second ) {
                err_msg = "Profile " + profile + " for version " + _version +
                  " contains a component named "
                  + (*i).first + " with level " + stm.str() +
             " which is higher than in the the original profile definition.\n";
              return false;
              } */             
            }
            else {
              err_msg = "Profile " + profile + 
                        " contains a component which is not in the original" +
                        string(" profile definition.\n");
              return false;
            }
          }
          return true;
        }
      }
      else {
        err_msg = "File with profile definition could not be found. " +
          string( "Path used is:\n" ) + profile_path + "\n";
        return false;
      }
  }
  err_msg = "Profile " + profile + " for version " + _version +
    " is currently not supported by H3DAPI.\n";
  return false;
}

bool ProfilesAndComponents::addComponent( std::string component, int level,
                                          string &err_msg ) {
  if( check_profiles_components ) {
    if( !instance ) {
      instance = new ProfilesAndComponents();
    }

    return instance->addComponentInternal( component, level, err_msg );
  }
  return true;
}

bool ProfilesAndComponents::addComponentInternal( std::string component,
                                                  int level,
                                                  string &err_msg ) {
  // If a profile definition is already set, compare components to those
  // already defined. If no profile definition is set, add this component
  // to used component list
  if( !main_profile_set ) {
    if( components_supported[version].find( component ) !=
        components_supported[version].end() ) {
      if( level <= components_supported[version][ component ] ) {
        int place = -1;
        if( findComponent( components_used, component, place ) ) {
          if( components_used[ place ].used_level < level )
            components_used[ place ].used_level = level;
        }
        else {
          auto_ptr< SAX2XMLReader >
            parser( ProfilesAndComponentsInternal::getNewXMLParser() );
          ProfileSAX2Handlers handler;
          parser->setContentHandler(&handler);
          parser->setErrorHandler(&handler);
          char *r = getenv( "H3D_ROOT" );
          string h3d_root = r ? r : "";
          string component_path = h3d_root + "/Conformance/" +
            version +"/Components/" + component + ".xml";
          ifstream component_file( component_path.c_str() );
          component_file.close();
          if( !component_file.fail() ) {
            parser->parse( component_path.c_str() );
          }
          else {
            err_msg = "File with component definition could not be found. " +
              string( "Path used is:\n" ) + component_path + "\n";
            return false;
          }

          ProfileSAX2Handlers::myX3DComponent temp_comp =
            handler.getComponentVector().front();
          temp_comp.conformance_level =
            components_supported[version][component];
          temp_comp.used_level = level;
          components_used.push_back( temp_comp );
        }
        return true;
      }
      else {
        stringstream stm, stm2;
        stm << level;
        stm2 << components_supported[version][ component ];
        err_msg = "Component " + component + " at level " + stm.str() +
          " is not supported by H3DAPI. " +
          "The maximum level for this component supported by H3DAPI is "
          + stm2.str() + ".\n";
        return false;
      }
    }
    err_msg = "Component " + component + " is not defined for version "
      + version + " of the X3D specification.\n";
    return false;
  } else {
    int place = -1;
    if( !findComponent( components_used, component, place ) ) {
      stringstream stm;
      stm << level;
      err_msg = "Component " + component + " at level "
        + stm.str()
        + " was not defined in the first profile definition \n";
      return false;
    }
    else if( components_used[ place ].used_level < level ) {
      stringstream stm, stm2;
      stm << level;
      stm2 << components_used[ place ].used_level;
      err_msg = "The level of component " + component +
        " is higher than when the component was first defined. The level is "
        + stm.str() + " but should not be higher than " +
        stm2.str() + "  \n";
      return false;
    }
    return true;
  }
}

void ProfilesAndComponents::setMainProfileDone( bool _main_profile_set ) {
  if( check_profiles_components ) {
    if( !instance->main_profile_set ) {
      instance->main_profile_set = _main_profile_set;
    }
  }
}

bool ProfilesAndComponents::findComponent(
    ProfileSAX2Handlers::myX3DComponentVector &component_vector,
    string name,
    int &place ) {
  for( int i = 0; i < (int)component_vector.size(); ++i ) {
    if( component_vector[i].name == name ) {
      place = i;
      return true;
    }
  }
  return false;
}

#endif
