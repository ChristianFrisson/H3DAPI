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
/// \file CreateH3DNodesFieldsList.cpp
/// \brief CPP file that goes through the node database and generates
/// dot files that can be used with doxygen documentation.
///
//
//
//////////////////////////////////////////////////////////////////////////////
#include <H3D/H3DNodeDatabase.h>
#include <H3D/Scene.h>
#include <H3DUtil/Exception.h>
#include <H3D/Node.h>
#include <H3DUtil/DynamicLibrary.h>
#include <H3D/ResourceResolver.h>
#include <H3D/URNResolver.h>
#include <H3D/X3DUrlObject.h>
#include <set>
#include <queue>
#include <fstream>

#cmakedefine OUTPUT_ABREVIATION_OR_NAME_CLASH 

using namespace std;
using namespace H3D;

// The nodes and field names in this list are ignored when generating the list of nodes and fields.
vector< pair< string, string > > deprecated_names_list;

// The nodes and field names in this list are for those where the name of the field is not the name of the variable containing the field.
// for example Text::stringF which contains the field string.
vector< pair< string, pair< string, string > > > field_name_not_variable_name;


struct less_than_field_name {
  bool operator()(const pair< string, string > &s1, const pair< string, string > &s2 ) const {
        return s1.first < s2.first;
      }
};

string getFieldName( Field *f, string node_name ) {
  string field_name = f->getName();
  if( node_name == "UniversalJoint" && field_name == "stopBounce1" )
    return "stop1Bounce";
  return field_name;
}

void writeNode( ostream &os, Node *n ) {
  if( !n ) {
    return;
  }

  H3DNodeDatabase *db = H3DNodeDatabase::lookupTypeId( typeid(*n) );

  string node_name = n->getTypeName();
  // In MedX3D ISOSurfaceVolumeData is the name of the class but IsoSurfaceVolumeData is the name of the type.
  // Doxygen need the class name to link properly.
  string class_name = node_name;
  if( node_name == "IsoSurfaceVolumeData" )
    class_name = "ISOSurfaceVolumeData";
  os << "/// <ul>" << endl
     << "///   <li>\\link H3D::" << class_name << " "
     << node_name << " \\endlink" << endl;

  if( db->fieldDBSize() > 0 ) {
    vector< pair< string, string > > ordered_field_names;
    for( H3DNodeDatabase::FieldDBConstIterator i = db->fieldDBBegin();
         i != db->fieldDBEnd(); i++ ) {
      string tmp_field_name = i.getFieldDBElement()->getName();
      bool valid_name = true;
      for( unsigned int j = 0; j < deprecated_names_list.size(); j++ )
        if( deprecated_names_list[j].first == node_name &&
            deprecated_names_list[j].second == tmp_field_name )
            valid_name = false;
      if( valid_name ) {
        bool field_name_is_variable_name = true;
        for( unsigned int j = 0; j < field_name_not_variable_name.size(); j++ )
          if( field_name_not_variable_name[j].first == node_name &&
              field_name_not_variable_name[j].second.first == tmp_field_name ) {
            ordered_field_names.push_back( pair< string, string >( tmp_field_name, field_name_not_variable_name[j].second.second ) );
            field_name_is_variable_name = false;
            break;
          }
        if( field_name_is_variable_name )
          ordered_field_names.push_back( pair< string, string >( tmp_field_name, getFieldName( i.getFieldDBElement()->getField(n), node_name ) ) );
      }
    }
    sort( ordered_field_names.begin(), ordered_field_names.end() );
    os << "///     <ul>" << endl;

    for( vector< pair< string, string > >::iterator i = ordered_field_names.begin();
         i != ordered_field_names.end(); i++ ) {
      os << "///       <li>\\link H3D::" << class_name << "::"
         << (*i).second << " " << (*i).first << " \\endlink" << "</li>" << endl;
    }
    os << "///     </ul>" << endl;
  }
  os << "///   </li>" << endl
     << "/// </ul>" << endl;
  delete n;
}

inline void resetSceneTimeField() {
  Scene::time.reset( new SFTime( TimeStamp() ) );
  Scene::time->setName( "Scene::time" );
}

DynamicLibrary::LIBHANDLE loadLibrary( string url) {
  string urn_name = url;
  URNResolver *urn_resolver = ResourceResolver::getURNResolver();
  if( urn_resolver ) {
    urn_name = urn_resolver->resolveURN( url );
  }
  DynamicLibrary::LIBHANDLE handle = NULL;
#ifdef WIN32
  bool ends_in_dll = urn_name.find( ".dll" ) != string::npos;

#ifdef _DEBUG

  // try absolute path.
  handle = DynamicLibrary::load( urn_name + "_d" );
  if( handle ) return handle;

#endif // _DEBUG

#endif // WIN32

  auto_ptr< X3DUrlObject > url_object( new X3DUrlObject );
  // test the given name directly.
  string filename = url_object->resolveURLAsFile( urn_name );
  if( filename == "" ) filename = urn_name;
  handle =  DynamicLibrary::load( filename );

  return handle;
}

string findOutputName( const string &library_path ) {
  string output_name = library_path;
  size_t last_slash = output_name.find_last_of( "/\\" );
  if( last_slash )
    output_name = output_name.substr( last_slash + 1, output_name.size() );
  size_t last_dot = output_name.find_last_of( "." );
  if( last_dot )
    output_name = output_name.substr( 0, last_dot );
  return output_name;
}

#ifdef OUTPUT_ABREVIATION_OR_NAME_CLASH
vector< string > abreviateHelp( const unsigned int &j, const vector< int > &capital_list, const int &max_depth, const string &node_name ) {
  vector< string > sub_abreviations;
  if( j+1 < capital_list.size() )
    sub_abreviations = abreviateHelp( j+1, capital_list, max_depth, node_name );
  vector< string > abreviations;
  for( int i = 1; i <= max_depth; ++i ) {
    int string_size = 0;
    if( j + 1 < capital_list.size() ) {
      string_size = H3DMin( i, capital_list[j+1]-capital_list[j] );
    } else if( capital_list[j] + i < node_name.size() )
      string_size = H3DMin( i, (int)node_name.size()-capital_list[j] );
    if( string_size > 0 ) {
      string tmp_abreviation = node_name.substr( capital_list[j], string_size );
      if( sub_abreviations.empty() ) {
        if( j != 0 || tmp_abreviation.size() > 1 )
          abreviations.push_back( tmp_abreviation );
      } else {
        for( unsigned int k = 0; k < sub_abreviations.size(); k++ ) {
          string new_abreviation = tmp_abreviation + sub_abreviations[k];
          if( j != 0 || new_abreviation.size() > 1 )
            abreviations.push_back( new_abreviation );
        }
      }
    }
  }
  for( vector<string>::iterator i = abreviations.begin(); i != abreviations.end(); ++i )
    transform((*i).begin(), (*i).end(), (*i).begin(), ::toupper);
  return abreviations;
}

bool my_sort_helper(string i,string j) {
  if( i.size() == j.size() )
    return (i<j);
  return i.size() < j.size();
}

vector< string > abreviateNodeName( const string &node_name, int max_depth = 3, int max_nr_of_abreviations = 9 ) {
  // Find capital letters.
  vector< int > capital_list;
  for( unsigned int i = 0; i < node_name.size(); ++i )
    if( node_name[i] != tolower( node_name[i] ) || isdigit(node_name[i]) )
      capital_list.push_back( i );
  if( capital_list.size() < 2 )
    max_depth = max_depth * 2;
  vector<string> abreviations = abreviateHelp( 0, capital_list, max_depth, node_name ) ;
  sort( abreviations.begin(), abreviations.end(), my_sort_helper );
  if( abreviations.size() > max_nr_of_abreviations )
    abreviations.erase( abreviations.begin() + max_nr_of_abreviations, abreviations.end() );
  return abreviations;
}
#endif

int main(int argc, char* argv[]) {
  string vc_post_fix = "_vc${H3D_MSVC_VERSION}";
  vector< string > extra_libraries;
  vector< string > extra_library_names;
#ifdef HAVE_UI
  extra_libraries.push_back( "${UI_SEARCH_PATH}/UI" + vc_post_fix );
  extra_library_names.push_back( findOutputName( extra_libraries.back() ) );
#endif
#ifdef HAVE_H3DPhysics
  extra_libraries.push_back( "${H3DPhysics_SEARCH_PATH}/H3DPhysics" + vc_post_fix );
  extra_library_names.push_back( findOutputName( extra_libraries.back() ) );
#endif
#ifdef HAVE_MedX3D
  extra_libraries.push_back( "${MedX3D_SEARCH_PATH}/MedX3D" + vc_post_fix );
  extra_library_names.push_back( findOutputName( extra_libraries.back() ) );
#endif
  int before_extra_libraries = extra_libraries.size();
${CreateH3DNodesFieldsList_EXTRA_BINARIES_CODE}

  vector< string > nodes_fields_output_file;
  ${CreateH3DNodesFieldsList_OUTPUT_FILE_CODE}
  for( unsigned int i = before_extra_libraries; i < extra_libraries.size(); ++i ) {
    extra_library_names.push_back( findOutputName( extra_libraries[i] ) );
  }
  string extra_libraries_output_file_default( "${CreateH3DNodesFieldsList_EXTRA_BINARIES_nodesFieldsList_OUTPUT_DIR}" );
  if( !extra_libraries_output_file_default.empty() ) {
    for( unsigned int i = before_extra_libraries; i < extra_libraries.size(); ++i ) {
      string file_name = extra_library_names[i];
      if( file_name != "" ) {
        file_name = extra_libraries_output_file_default + "/" + file_name + "_nodesFieldsList";
      }
      nodes_fields_output_file.push_back( file_name );
    }
  }

  if( argc > 1 || nodes_fields_output_file.size() != extra_libraries.size() + 1 ) {
    if (argc < 2){
      cerr << "Usage: " << argv[0] << " <Output file 1>";
      for( unsigned int i = 0; i < extra_libraries.size(); i++ ) {
        stringstream s;
        s << (i+1);
        cerr << " (Output file " << s.str() << ")";
      }
      cerr << endl;
      if( extra_libraries.size() > 0 ) {
        cerr << "The optional output files indicate that nodesfields lists for ";
        for( unsigned int i = 0; i < extra_libraries.size(); i++ ) {
          cerr << extra_libraries[i];
          if( i+2 == extra_libraries.size() )
            cerr << " and ";
          else if( i+1 < extra_libraries.size() )
            cerr << ", ";
        }
        cerr << " should be generated." << endl;
      }
      return 1;
    }
  }

  deprecated_names_list.push_back( pair< string, string >( "ImportLibrary", "library" ) );
  deprecated_names_list.push_back( pair< string, string >( "Contact", "minBounceSpeed" ) );
  deprecated_names_list.push_back( pair< string, string >( "UniversalJoint", "stopBounce1" ) );

  field_name_not_variable_name.push_back( pair< string, pair< string, string > >( "Text", pair< string, string >( "string", "stringF" ) ) );
  field_name_not_variable_name.push_back( pair< string, pair< string, string > >( "MetadataDouble", pair< string, string >( "name", "nameF" ) ) );
  field_name_not_variable_name.push_back( pair< string, pair< string, string > >( "MetadataFloat", pair< string, string >( "name", "nameF" ) ) );
  field_name_not_variable_name.push_back( pair< string, pair< string, string > >( "MetadataInteger", pair< string, string >( "name", "nameF" ) ) );
  field_name_not_variable_name.push_back( pair< string, pair< string, string > >( "MetadataSet", pair< string, string >( "name", "nameF" ) ) );
  field_name_not_variable_name.push_back( pair< string, pair< string, string > >( "MetadataString", pair< string, string >( "name", "nameF" ) ) );
  field_name_not_variable_name.push_back( pair< string, pair< string, string > >( "Contact", pair< string, string >( "minbounceSpeed", "minBounceSpeed" ) ) );
  field_name_not_variable_name.push_back( pair< string, pair< string, string > >( "SizeJustifiedText", pair< string, string >( "string", "stringF" ) ) );

  vector< string > h3d_node_names;
#ifdef OUTPUT_ABREVIATION_OR_NAME_CLASH
  map< string, vector< string > > nodenames_per_library;
  vector< string > unique_node_names;
  map< string, vector< string > > name_clashes;
  vector< string > library_endings_to_test;
#ifdef WIN32
  library_endings_to_test.push_back( ".dll" );
#ifdef _DEBUG
  library_endings_to_test.push_back( "_d.dll" );
#endif
#endif
#endif
  for( unsigned int i = 0; i <= extra_libraries.size(); i++ ) {
    if( i + 1 >= argc ) {
      if( i >= nodes_fields_output_file.size() )
        return 0;
    } else {
      if( i >= nodes_fields_output_file.size() ) {
        nodes_fields_output_file.resize( i+1, "" );
      }
      nodes_fields_output_file[i] = argv[i + 1];
    }

    string page_name = "";
    DynamicLibrary::LIBHANDLE library_handle = NULL;
    vector< string > dependencies;
    if( i > 0 ) {
      library_handle = loadLibrary( extra_libraries[i-1] );
      page_name = extra_library_names[i-1];
#ifdef WIN32
      // If already loaded just return the handle to the loaded library.
      for( unsigned int j = 0; j < extra_library_names.size(); ++j ) {
        if( page_name != extra_library_names[j] ) {
          DynamicLibrary::LIBHANDLE handle = GetModuleHandle( (extra_library_names[j] + "_d.dll").c_str() );
          if( handle )
            dependencies.push_back( extra_library_names[j] );
        }
      }
#endif
    }

    string out_file = nodes_fields_output_file[i];

    ofstream os( out_file.c_str() );
    if( !os.is_open() )
      return 1;
#ifdef OUTPUT_ABREVIATION_OR_NAME_CLASH
    vector< string > nodes_in_this_library;
#endif

    os << "/// \\file " << out_file << endl
       << "/// \\brief Extra page listing all nodes and fields." << endl
       << "/// \\page NodeFieldList" << page_name << " List of nodes and their fields." << endl;

    try {
      vector< string > ordered_node_names;
      for( H3DNodeDatabase::NodeDatabaseConstIterator j = 
             H3DNodeDatabase::begin();
           j != H3DNodeDatabase::end();
           j++ ) {
        ordered_node_names.push_back( (*j).second->getName() );
      }

      sort( ordered_node_names.begin(), ordered_node_names.end() );

      for( vector< string >::iterator j = ordered_node_names.begin();
           j != ordered_node_names.end(); j++ ) {
        if( find( h3d_node_names.begin(), h3d_node_names.end(), (*j) ) == h3d_node_names.end() ) {
          if( i == 0 )
            h3d_node_names.push_back( (*j) );
          Node *n = H3DNodeDatabase::createNode( (*j) );
          if( !n ) {
            cerr << (*j) << ": No such node exists in the node database"
                 << endl;
          } else {
            writeNode( os, n );
            resetSceneTimeField();
#ifdef OUTPUT_ABREVIATION_OR_NAME_CLASH
            if( find( unique_node_names.begin(), unique_node_names.end(), *j ) == unique_node_names.end() ) {
              nodes_in_this_library.push_back( *j );
              unique_node_names.push_back( *j );
            } else {
              bool clash_was_in_dependent_library = false;
              for( vector< string >::iterator k = dependencies.begin(); k != dependencies.end(); ++k ) {
                if( nodenames_per_library.find(*k) != nodenames_per_library.end() ) {
                  if( find( nodenames_per_library[*k].begin(), nodenames_per_library[*k].end(), *j ) != nodenames_per_library[*k].end() ) {
                    clash_was_in_dependent_library = true;
                    break;
                  }
                }
              }

              if( !clash_was_in_dependent_library ) {
                map< string, vector< string > >::iterator node_in_name_clashes = name_clashes.find( *j );
                if( node_in_name_clashes == name_clashes.end() ) {
                  // First time this node name clashes with other libraries. Simply find in which previous library
                  // the node name exists.
                  for( map< string, vector< string > >::iterator k = nodenames_per_library.begin(); k != nodenames_per_library.end(); ++k ) {
                    vector< string >::iterator found_node_name = find( (*k).second.begin(), (*k).second.end(), *j );
                    if( found_node_name != (*k).second.end() ) {
                      string name_to_add = (*k).first;
                      if( name_to_add == "" )
                        name_to_add = "H3DAPI";
                      name_clashes[*j].push_back( (*k).first ); // Found first instance of this node name that caused the clash.
                      break;
                    }
                  }
                }
                string name_to_add = page_name;
                if( name_to_add == "" )
                  name_to_add = "H3DAPI";
                name_clashes[*j].push_back( page_name );
              }
            }
#endif
          }
        }
      }
      cerr << "DONE!" << endl;
      os.close();
    } catch (const Exception::H3DException &e) {
        os.close();
        cerr << e << endl;
    }
    nodenames_per_library[page_name].swap( nodes_in_this_library );
    if( library_handle && DynamicLibrary::close( library_handle ) != 0 ) {
      cerr << "Failed to close handle for library " << extra_libraries[i] << endl;
    }
  }
  
#ifdef OUTPUT_ABREVIATION_OR_NAME_CLASH
  string abreviation_file_name( "${CreateH3DNodesFieldsList_OUTPUT_ABREVIATION_FILE}" );
  string name_clash_file_name( "${CreateH3DNodesFieldsList_OUTPUT_NAME_CLASH_FILE}" );
  sort( unique_node_names.begin(), unique_node_names.end() );
  ofstream os( abreviation_file_name.c_str() );
  if( os.is_open() ) {
    vector< string > abreviations;
    vector< vector< string > > abr_per_node;
    for( unsigned int i = 0; i < unique_node_names.size(); ++i ){
      vector< string > new_abreviations = abreviateNodeName( unique_node_names[i] );
      for( vector< string >::iterator j = new_abreviations.begin(); j != new_abreviations.end(); ++j ) {
        vector< string >::iterator abrv_exists = find( abreviations.begin(), abreviations.end(), *j );
        if( abrv_exists == abreviations.end() ) {
          abreviations.push_back( *j );
          abr_per_node.push_back( vector<string>() );
          abr_per_node[i].push_back( *j );
          new_abreviations.erase( j );
          break;
        }
      }
      if( i >= abr_per_node.size() ) {
        abr_per_node.push_back( vector<string>() );
        abr_per_node[i].push_back( "-" ); // Could not find an abreviation.
      }
      abr_per_node[i].insert( abr_per_node[i].end(), new_abreviations.begin(), new_abreviations.end() );
    }
    os << "Nr of unique node names are:," << unique_node_names.size() << endl;
    os << "Node type,Abreviation,Alternative suggestion" << endl;
    for( unsigned int i = 0; i < unique_node_names.size(); ++i ){
      os << unique_node_names[i];
      for( unsigned int j = 0; j < abr_per_node[i].size(); ++j ) {
        if( j < 2 )
          os << ",";
        else
          os << " ";
        os << abr_per_node[i][j];
      }
      os << endl;
    }
    os.close();
  }
  ofstream os2( name_clash_file_name.c_str() );
  if( os2.is_open() ) {
    os2 << "There are this many node names that exist in several libraries:," << name_clashes.size() << endl;
    os2 << "Node name, Library name 0, Library name 1, Library name 2, well you get the picture I got lazy" << endl;
    for( map< string, vector< string > >::iterator i = name_clashes.begin(); i != name_clashes.end(); ++i ){
      os2 << (*i).first;
      for( vector< string >::iterator j = (*i).second.begin(); j != (*i).second.end(); ++j )
        os2 << "," << *j;
      os2 << endl;
    }
    os2.close();
  }
#endif
  return 0;
}

