//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004-2013, SenseGraphics AB
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
#include <H3D/ImportLibrary.h>
#include <set>
#include <queue>
#include <fstream>

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
          ordered_field_names.push_back( pair< string, string >( tmp_field_name, i.getFieldDBElement()->getField(n)->getName() ) );
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


string vc_post_fix = "_vc${H3D_MSVC_VERSION}";
vector< string > extra_output;
vector< string > base_path;

int main(int argc, char* argv[]) {
#ifdef HAVE_UI
  extra_output.push_back( "UI" );
  base_path.push_back( "${UI_SEARCH_PATH}/" );
#endif
#ifdef HAVE_H3DPhysics
  extra_output.push_back( "H3DPhysics" );
  base_path.push_back( "${H3DPhysics_SEARCH_PATH}/" );
#endif
#ifdef HAVE_MedX3D
  extra_output.push_back( "MedX3D" );
  base_path.push_back( "${MedX3D_SEARCH_PATH}/" );
#endif

  if (argc < 2){
    cerr << "Usage: " << argv[0] << " <Output file 1>";
    for( unsigned int i = 0; i < extra_output.size(); i++ ) {
      stringstream s;
      s << (i+1);
      cerr << " (Output file " << s.str() << ")";
    }
    cerr << endl;
    if( extra_output.size() > 0 ) {
      cerr << "The optional output files indicate that nodesfields lists for ";
      for( unsigned int i = 0; i < extra_output.size(); i++ ) {
        cerr << extra_output[i];
        if( i+2 == extra_output.size() )
          cerr << " and ";
        else if( i+1 < extra_output.size() )
          cerr << ", ";
      }
      cerr << " should be generated." << endl;
    }
    return 1;
  }

	deprecated_names_list.push_back( pair< string, string >( "ImportLibrary", "library" ) );

	field_name_not_variable_name.push_back( pair< string, pair< string, string > >( "Text", pair< string, string >( "string", "stringF" ) ) );
  field_name_not_variable_name.push_back( pair< string, pair< string, string > >( "MetadataDouble", pair< string, string >( "name", "nameF" ) ) );
  field_name_not_variable_name.push_back( pair< string, pair< string, string > >( "MetadataFloat", pair< string, string >( "name", "nameF" ) ) );
  field_name_not_variable_name.push_back( pair< string, pair< string, string > >( "MetadataInteger", pair< string, string >( "name", "nameF" ) ) );
  field_name_not_variable_name.push_back( pair< string, pair< string, string > >( "MetadataSet", pair< string, string >( "name", "nameF" ) ) );
  field_name_not_variable_name.push_back( pair< string, pair< string, string > >( "MetadataString", pair< string, string >( "name", "nameF" ) ) );

  vector< string > all_node_names;
  for( unsigned int i = 0; i <= extra_output.size(); i++ ) {
    if( i + 1 >= argc )
		  return 0;
    
    string page_name = "";
    if( i > 0 ) {
      ImportLibrary * imp_lib = new ImportLibrary();
      vector< string > url;
      url.push_back( base_path[i-1] + extra_output[i-1] + vc_post_fix
/*#ifdef _DEBUG
                     + "_d"
#endif
#ifdef H3D_WINDOWS
                     + ".dll"
#endif*/
			               );
      imp_lib->url->setValue( url );
      imp_lib->initialize();
      delete imp_lib;
      page_name = extra_output[i-1];
    }

    string out_file = argv[i + 1];

    ofstream os( out_file.c_str() );
    if( !os.is_open() )
      return 1;

    os << "/// \\file " << out_file << endl
       << "/// \\brief Extra page listing all nodes and fields." << endl
       << "/// \\page NodeFieldList" << page_name << " List of nodes and their fields." << endl;

    try {
      vector< string > ordered_node_names;
      for( H3DNodeDatabase::NodeDatabaseConstIterator i = 
             H3DNodeDatabase::begin();
           i != H3DNodeDatabase::end();
           i++ ) {
        ordered_node_names.push_back( (*i).second->getName() );
      }

      sort( ordered_node_names.begin(), ordered_node_names.end() );

      for( vector< string >::iterator i = ordered_node_names.begin();
           i != ordered_node_names.end(); i++ ) {
        if( find( all_node_names.begin(), all_node_names.end(), (*i) ) == all_node_names.end() ) {
          all_node_names.push_back( (*i) );
          Node *n = H3DNodeDatabase::createNode( (*i) );
          if( !n ) {
            cerr << (*i) << ": No such node exists in the node database"
                 << endl;
          } else {
            writeNode( os, n );
            resetSceneTimeField();
          }
        }
      }
      cerr << "DONE!" << endl;
      os.close();
    } catch (const Exception::H3DException &e) {
        os.close();
        cerr << e << endl;
    }
  }
  return 0;
}

