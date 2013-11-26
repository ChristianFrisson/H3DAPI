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
/// \file CheckH3DAgainstX3DSpec.cpp
/// \brief CPP file that takes components html from X3D as input and checks
/// what H3D is missing. It uses C++11 features.
///
//
//
//////////////////////////////////////////////////////////////////////////////
#include <H3D/H3DNodeDatabase.h>
#include <regex>
#include <H3D/Scene.h>
#include <H3DUtil/Exception.h>
#include <H3D/Node.h>
#include <H3D/ImportLibrary.h>
#include <H3D/X3DTypeFunctions.h>
#include <set>
#include <queue>
#include <fstream>

using namespace std;
using namespace H3D;

// The nodes and field names in this list are ignored when comparing against X3D spec.
vector< pair< string, string > > deprecated_names_list;

// The nodes and field names in this list are for those where the name of the field is not the name of the variable containing the field.
// for example Text::stringF which contains the field string.
vector< pair< string, pair< string, string > > > field_name_not_variable_name;

void checkNode( Node *n ) {
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
  }
  delete n;
}

inline void resetSceneTimeField() {
  Scene::time.reset( new SFTime( TimeStamp() ) );
  Scene::time->setName( "Scene::time" );
}


string vc_post_fix = "_vc10";
vector< string > extra_output;
vector< string > base_path;

struct FieldDefinition  {
	FieldDefinition( Field * _field = NULL ):field(_field), have_default_value( false ) {}
	~FieldDefinition() {
		delete field;
		field = NULL;
	}
	Field * field;
	vector< string > extra_info;
  bool have_default_value;
};

struct NodeDefinition {
  NodeDefinition( string _name = "" ) : name(_name) {}
  ~NodeDefinition() {
    for( unsigned int i = 0; i < fields.size(); ++i )
      delete fields[i];
  }
  string name;
	vector< string > parent_names;
  vector< FieldDefinition * > fields;
};

void splitString( string str, vector< string > &split_result, string split_chars = ", " ) {
	string one_string = "";
	bool add_to_string = false;
	for( unsigned int i = 0; i < str.size(); ++i ) {
		const char &current_char = str[i];
		bool is_split_char = false;
		for( unsigned int j = 0; j < split_chars.size(); ++j )
			if( split_chars[j] == current_char ) {
				is_split_char = true;
				break;
			}
				
		if( !is_split_char ) {
			if( !add_to_string ) {
				add_to_string = true;
				one_string = "";
			}
		} else if( add_to_string ) {
			add_to_string = false;
			split_result.push_back( one_string );
		}
		if( add_to_string ) {
			one_string += current_char;
		}
	}
}

void checkForNoSpace( string the_string, Field *new_field, string error_message = "new version of default value for " ) {
	smatch match_object;
	if( regex_search( the_string, match_object, regex("[^[:space:]]") ) ) {
		cerr << error_message << new_field->getTypeName() << "." << endl;
		system( "PAUSE" );
	}
}

string inf_as_unicode, minus_as_unicode, pi_as_unicode;
void handleParsableField( vector< string >possible_values, string range, NodeDefinition &node_definition ) {
  FieldDefinition * field_def = node_definition.fields.back();
	for( unsigned int i = 0; i < possible_values.size(); ++i ) {
		if( possible_values[i] != "" ) {
			string value_string = possible_values[i];
			if( value_string == "&pi;/4" || value_string == pi_as_unicode + "/4" || value_string == "&#960;/4" ) {
				stringstream ss;
				ss << Constants::pi / 4;
				value_string = ss.str();
			} else if( value_string == "&#960;/2" || value_string == pi_as_unicode + "/2" ) {
				stringstream ss;
				ss << Constants::pi / 2;
				value_string = ss.str();
			} else if( value_string == "&#960;/12" ) {
				stringstream ss;
				ss << Constants::pi / 12;
				value_string = ss.str();
			} else if( value_string == "&pi;" ) {
				stringstream ss;
				ss << Constants::pi;
				value_string = ss.str();
			} else if( value_string == "-&pi;" ) {
				stringstream ss;
				ss << -Constants::pi;
				value_string = ss.str();
			} else if( value_string == "NULL" ) {
				value_string = "";
			}
      if( value_string != "" ) {
        static_cast< ParsableField * >(field_def->field)->setValueFromString( value_string );
        field_def->have_default_value = true;
        break;
      }
		}
	}
	// "(-&#8734;,&#8734;)" is infinite range.
	vector< regex > value_range_regex;
	value_range_regex.push_back( regex( "\\[[\\-.0-9]+(?:,|\\.\\.)[\\-.0-9]+\\]" ) );
	value_range_regex.push_back( regex( "\\[[\\-.0-9]+,[\\-.0-9]+\\]" ) );
	bool regex_match_range = false;
	for( unsigned int i = 0; i < value_range_regex.size(); ++i ) {
		smatch range_match_object;
		if( regex_search( range, range_match_object, value_range_regex[i] ) ) {
			field_def->extra_info.push_back( range_match_object.str(0) );
			regex_match_range = true;
			break;
		}
	}
	
	if( !regex_match_range ) {
		const char *s = range.c_str();
		range = ( X3D::Convert::skipWhitespaces( s ) );
		size_t last_not_whitespace_pos = range.find_last_not_of( "\n\t " );
		if( last_not_whitespace_pos != string::npos )
			range = range.substr( 0, last_not_whitespace_pos + 1 );
		if( range == "(0,&infin;)" ||
				range == "(0,&#8734;)" ||
				range == ( "(0," + inf_as_unicode + ")" ) ) {
			field_def->extra_info.push_back( "(0,inf)" );
		} else if( range == "[0,&infin;)" ||
							 range == "[0,&#8734;)" ||
							 range == ( "[0," + inf_as_unicode + ")" ) ||
							 range == "[0, &infin;)" ) {
			field_def->extra_info.push_back( "[0,inf)" );
		} else if( range == "[-1,&#8734;)" ) {
			field_def->extra_info.push_back( "[-1,inf)" );
		} else if( range == "[2,&#8734;)" ) {
			field_def->extra_info.push_back( "[2,inf)" );
		} else if( range == "[3,&#8734;)" ) {
			field_def->extra_info.push_back( "[3,inf)" );
		} else if( range == "(2,&#8734;)" ) {
			field_def->extra_info.push_back( "(2,inf)" );
		} else if( range == "[0,&#8734;) or -1" ||
							 range == "[0,&#8734;) or -1 " ||
							 range == "[0,&infin;) or -1" ||
							 range == ( "[0,&#8734;) or " + minus_as_unicode + "1" ) ) {
			field_def->extra_info.push_back( "[0,inf)" );
			field_def->extra_info.push_back( "-1" );
		} else if( range == "[0,&pi;/2]" || range == "[0,&#960;/2]" ) {
			stringstream range_ss;
			range_ss << "[0," << Constants::pi / 2 << "]";
			field_def->extra_info.push_back( range_ss.str() );
		} else if( range == "[0,&pi;]" ) {
			stringstream range_ss;
			range_ss << "[0," << Constants::pi << "]";
			field_def->extra_info.push_back( range_ss.str() );
		} else if( range == "[see <a href=\"#Specifyingaspatialreference\">25.2.3</a>]  " || range == "[see <a href=\"#Specifyingaspatialreference\">25.2.3</a>]" ) {
			// Some really special case... don't know how to handle it yet.
			field_def->extra_info.push_back( "see section 25.2.3" );
		} else if( range == "[\"LEFT\"|\"CENTER\"|\"RIGHT\"&amp;\n                                                      \"BOTTOM\"|\"CENTER\"|\"TOP\"]" ) {
			field_def->extra_info.push_back( "LEFT|CENTER|RIGHT&BOTTOM|CENTER|TOP" );
		} else if( range == "[\"WORLD\",\"FRACTION\",\"PIXEL\"]" ) {
			field_def->extra_info.push_back( "WORLD,FRACTION,PIXEL" );
		} else if( range == "[\"NONE\",\"FRACTION\",\"STRETCH\",\"PIXEL\"]" ) {
			field_def->extra_info.push_back( "NONE,FRACTION,STRETCH,PIXEL" ); 
		} else if( range == "[\"TELEPORT\",\"LINEAR\",\n                                                       \"ANIMATE\",...]" ) {
			field_def->extra_info.push_back( "TELEPORT,LINEAR,ANIMATE" ); 
		} else if( range == "[\"<code>ANY</code>\",\"<code>WALK</code>\",\"<code>EXAMINE</code>\",\"<code>FLY</code>\",\n                                                       \"<code>LOOKAT</code>\",\"<code>NONE</code>\",\"EXPLORE\",...]" ) {
			field_def->extra_info.push_back( "ANY,WALK,EXAMINE,FLY,LOOKAT,NONE,EXPLORE" ); 
		} else if( range == "[\"LINEAR\"|\"EXPONENTIAL\"]" || range == "[\"LINEAR\"|\"EXPONENTIAL\"]" ) {
			field_def->extra_info.push_back( "LINEAR|EXPONENTIAL" ); 
		} else if( range == "[\"NONE\"|\"NEXT_FRAME_ONLY\"|\"ALWAYS\"]" ) {
			field_def->extra_info.push_back( "NONE|NEXT_FRAME_ONLY|ALWAYS" ); 
		} else if( range == "[\"LINE\"|\"POINT\"|\"QUAD\"|\"SPRITE\"|\"TRIANGLE\"|\"GEOMETRY\"|...]" ) {
			field_def->extra_info.push_back( "LINE|POINT|QUAD|SPRITE|TRIANGLE|GEOMETRY" ); 
		} else if( range == "[\"PIE\"|\"CHORD\"]" ) {
			field_def->extra_info.push_back( "PIE|CHORD" ); 
		} else if( range == "[\"PLAIN\"|\"BOLD\"|\"ITALIC\"|\"BOLDITALIC\"|\"\"]" ) {
			field_def->extra_info.push_back( "PLAIN|BOLD|ITALIC|BOLDITALIC|\"\"" ); 
		} else if( range == "[\"MATCH_ANY\"|\"MATCH_EVERY\"|\n                                                  \"MATCH_ONLY_ONE\"]" ) {
			field_def->extra_info.push_back( "MATCH_ANY|MATCH_EVERY|MATCH_ONLY_ONE" ); 
		} else if( range == "[\"GEOMETRY\"|\"BOUNDS\"|...]" ) {
			field_def->extra_info.push_back( "GEOMETRY|BOUNDS" ); 
		} else if( range == "[\"ANY\"|\"CLOSEST\"|\"ALL\"|\"ALL_SORTED\"]" ) {
			field_def->extra_info.push_back( "ANY|CLOSEST|ALL|ALL_SORTED" ); 
		} else if( range == "[\"CLOSEST\"|\"ALL\"|\"ALL_SORTED\"]" ) {
			field_def->extra_info.push_back( "CLOSEST|ALL|ALL_SORTED" ); 
		} else if( range == "[0,&#8734;) or &minus;1 &minus;1 &minus;1" ||
							 range == "[0,&#8734;) or &minus;1 &minus;1 &minus;1 " ||
							 range == "[0, &infin;) or -1 -1 -1" ||
							 range == "[0,&#8734;) or -1 -1 -1" ||
							 range == ( "[0," + inf_as_unicode + ") or " + minus_as_unicode + "1 " + minus_as_unicode + "1 " + minus_as_unicode + "1" ) ||
							 range == "[0,&infin;) or -1 -1 -1" ) {
			field_def->extra_info.push_back( "-1 -1 -1" );
			field_def->extra_info.push_back( "[0,inf)" );
		} else if( range == "(0,&#8734;) or -1 -1 -1" || range == "(0,&infin;) or -1 -1 -1" ) {
			field_def->extra_info.push_back( "-1 -1 -1" );
			field_def->extra_info.push_back( "(0,inf)" );
		} else if( range == "[0,-&#8734;)" || range == "[0,-&infin;)" ) {
			field_def->extra_info.push_back( "[0,-inf)" );
		} else if( range == "[1,&#8734;)" ) {
			field_def->extra_info.push_back( "[1, inf)" );
		} else if( range == "-1 or [0,&#8734;)" ||
							 range == "-1 or [0..&#8734;]" ||
							 range == ("-1 or [0," + inf_as_unicode + ")" ) ) {
			field_def->extra_info.push_back( "-1" );
			field_def->extra_info.push_back( "[0,inf)" );
		} else if( range == "(-&#8734;,&#8734;) (deprecated as of vs. 3.3)" ) {
			field_def->extra_info.push_back( "deprecated" );
		} else if( range == "(0,&pi;)" || range == "(0,&#960;)" ) {
			stringstream range_ss;
			range_ss << "(0," << Constants::pi << ")";
			field_def->extra_info.push_back( range_ss.str() );
		} else if( range == "[-2&#960;,2&#960;]" ) {
			H3DDouble twopi = 2*Constants::pi;
			stringstream range_ss;
			range_ss << "[" << -twopi << "," << twopi << "]";
			field_def->extra_info.push_back( range_ss.str() );
		} else if( range == "(0," + pi_as_unicode + "/2]" ) {
			stringstream range_ss;
			range_ss << "(0," << Constants::pi / 2 << ")";
			field_def->extra_info.push_back( range_ss.str() );
		} else if( range == "[-&pi;,&pi;]" ) {
			stringstream range_ss;
			range_ss << "[" << -Constants::pi << "," << Constants::pi << "]";
			field_def->extra_info.push_back( range_ss.str() );
		} else if( range == "[\"Cg\"|\"GLSL\"|\"HLSL\"|...]" ||
							 range == "[\"Cg\"|\"GLSL\"|\"HLSL\"|...]\n\n  # And any number of:\n  fieldType []       fieldName\n  fieldType [in]     fieldName\n  fieldType [out]    fieldName\n  fieldType [in,out] fieldName" ||
							 range == "[\"Cg\"|\"GLSL\"|\"HLSL\"|...]\n\n  # And any number of:\n  fieldType [in]     fieldName\n  fieldType [in,out] fieldName initialValue\n  fieldType [out]    fieldName\n  fieldType []       fieldName initialValue" ) {
			field_def->extra_info.push_back( "Cg|GLSL|HLSL" );
		} else if( range == "[\"VERTEX\"|\"FRAGMENT\"]" ||
							 range == "[\"VERTEX\"|\"FRAGMENT\"]\n\n  # And any number of:\n  fieldType [in]     fieldName\n  fieldType [in,out] fieldName initialValue\n  fieldType [out]    fieldName\n  fieldType []       fieldName initialValue" ) {
			field_def->extra_info.push_back( "VERTEX|FRAGMENT" );
		} else if( range == "[see Table 18.6]" ) {
			field_def->extra_info.push_back( "Table 18.6" );
		} else if( range == "[\"CONSTANT\", \"ALPHA0\", \"ALPHA1\", \"TABLE\",\n                                                        \"ONE_MINUS_ALPHA0\", \"ONE_MINUS_ALPHA1\" ]" ) {
			field_def->extra_info.push_back( "CONSTANT,ALPHA0,ALPHA1,TABLE,ONE_MINUS_ALPHA0,ONE_MINUS_ALPHA1" );
		} else if( range == "[\"MAX\", \"MIN\", \"AVERAGE\"]" ) {
			field_def->extra_info.push_back( "MAX,MIN,AVERAGE" );
		} else if( range == "[\"Henyey-Greenstein\",\"NONE\",...]" ) {
			field_def->extra_info.push_back( "Henyey-Greenstein,NONE" );
		} else if( range == "(Required)" ) {
			field_def->extra_info.push_back( "(Required, can't be empty after X3D reading.)" );
		} else if( range == "[\"BEGIN\",\"END\",\"FIRST\",\"MIDDLE\",\"\"]" ) {
			field_def->extra_info.push_back( "BEGIN,END,FIRST,MIDDLE" );
		} else if( range == "[\"ALL\",\"NONE\",\"TERRAIN\",...]" ) {
			field_def->extra_info.push_back( "ALL,NONE,TERRAIN" );
		} else if( range == "[\"ALL\",\"NONE\",...]" ) {
			field_def->extra_info.push_back( "ALL,NONE" );
		} else if( range != "" &&
							 range != "[URI]" && // URI is url field
							 range != "(-&#8734;,&#8734;)" &&
							 range != "(-&#8734;,&#8734;) " &&
							 range != "(-&#8734;,&#8734;)  " &&
							 range != "(-&#8734;,&#8734;) or -1 1" &&
							 range != "(-&infin;, &infin;)" &&
							 range != "(-&#8734;,&#8734;) (starting with vs. 3.3)" &&
							 range != ( "(-" + inf_as_unicode + "," + inf_as_unicode + ")" ) &&
							 range != "(-&#8734;, &#8734;)" &&
							 range != "(&infin;,&infin;)" &&
							 range != "[-&#8734;,&#8734;)" &&
							 range != "(-&infin;,&infin;)" ) {
			checkForNoSpace( range, field_def->field, "not handling default range :" + range + ": yet for " );
		}
	}
}

string createFieldDefinitionString( const string &field_definition_string, int type = X3DTypes::SFFLOAT ) {
	auto_ptr< Field > field( X3DTypes::newFieldInstance( (X3DTypes::X3DType )type ) );
	if( type >= X3DTypes::MFMATRIX4D )
		return field->getTypeName();
	return "(?:" + field->getTypeName() + "|" + createFieldDefinitionString( field_definition_string, type + 1 ) + ")";
}

bool checkSeveralRegEx( string &to_check, smatch &match_object, vector< regex >regexs ) {
	for( unsigned int i = 0; i < regexs.size(); ++i ) {
		if( regex_search( to_check, match_object, regexs[i] ) )
			return true;
	}
	return false;
}

void replaceInString( string &string_to_modify, string to_replace, string replace_with ) {
	size_t found_pos = string_to_modify.find( to_replace );
	while( found_pos != string::npos ) {
		string_to_modify = string_to_modify.substr( 0, found_pos ) + replace_with + string_to_modify.substr( found_pos + to_replace.size(), string_to_modify.length() );
		found_pos = string_to_modify.find( to_replace, found_pos + 1 );
	}
}

string accessTypeAsString( Field::AccessType access_type ) {
  switch( access_type ) {
    case Field::INITIALIZE_ONLY: return "INITIALIZE_ONLY";
    case Field::OUTPUT_ONLY: return "OUTPUT_ONLY";
    case Field::INPUT_ONLY: return "INPUT_ONLY";
    case Field::  INPUT_OUTPUT: return "INPUT_OUTPUT";
    default: return "NOT DEFINED";
  }
}

string getFieldDefaultValueAsString( Field * f) {
	if( ParsableField * parse_field = dynamic_cast< ParsableField * >(f) ) {
		return parse_field->getValueAsString();
	} else if( SFNode * sf_field = dynamic_cast< SFNode * >(f) ) {
		Node * tmp_n = sf_field->getValue();
		if( tmp_n )
			return tmp_n->getTypeName();
		else
			return "NULL";
	} else if( MFNode * mf_field = dynamic_cast< MFNode * >(f) ) {
		const NodeVector &node_vector = mf_field->getValue();
		string return_string = "";
		for( unsigned int k = 0; k < node_vector.size(); k++ ) {
			Node * tmp_n = node_vector[k];
			if( tmp_n )
				return_string += tmp_n->getTypeName();
			else
				return_string += "NULL";
		}
		return return_string;
	}
}

string fieldDeclaredInParent( NodeDefinition * node_def, string field_name, vector< NodeDefinition * > &node_defs, string *parent_field_info = NULL ) {
	for( unsigned int i = 0; i < node_def->parent_names.size(); ++i ) {
		for( unsigned int j = 0; j < node_defs.size(); ++j ) {
			NodeDefinition *parent_node_def = node_defs[j];
			if( parent_node_def->name == node_def->parent_names[i] ) {
				for( unsigned int k = 0; k < parent_node_def->fields.size(); ++k ) {
					if( parent_node_def->fields[k]->field->getName() == field_name ) {
						string return_value = fieldDeclaredInParent( parent_node_def , field_name, node_defs );
						if( return_value == "" ) {
							return_value = parent_node_def->name;
							if( parent_field_info != NULL ) {
								Field * parent_node_field = parent_node_def->fields[k]->field;
								*parent_field_info = "Type; access type; default value; range: " + parent_node_field->getTypeName() + "; " + accessTypeAsString( parent_node_field->getAccessType() ) + "; " + getFieldDefaultValueAsString( parent_node_field ) + "; ";
								for( unsigned int l = 0; l < parent_node_def->fields[k]->extra_info.size(); ++l )
									(*parent_field_info) += parent_node_def->fields[k]->extra_info[l];
							}
						}
						return return_value;
					}
				}
			}
		}
	}
	return "";
}

int main(int argc, char* argv[]) {
#ifdef HAVE_Geospatial
  extra_output.push_back( "Geospatial" );
  base_path.push_back( "E:/Markus/H3DAll/bin/" );
#endif
#ifdef HAVE_H3DPhysics
  extra_output.push_back( "H3DPhysics" );
  base_path.push_back( "E:/Markus/H3DWithToolkitsWin/bin32/" );
#endif
#ifdef HAVE_MedX3D
  extra_output.push_back( "MedX3D" );
  base_path.push_back( "E:/Markus/H3DWithToolkitsWin/bin32/" );
#endif

  if (argc < 3){
    cerr << "Usage: " << argv[0] << " <components html input directory> <output file>";
    cerr << endl;
    return 1;
  }
  
  for( unsigned int i = 0; i < extra_output.size(); ++i ) {
    ImportLibrary * imp_lib = new ImportLibrary();
    string url = base_path[i] + extra_output[i];
		if( extra_output[i] != "Geospatial" )
			url += vc_post_fix;
    imp_lib->url->push_back( url );
    imp_lib->initialize();
    delete imp_lib;
  }

	deprecated_names_list.push_back( pair< string, string >( "ImportLibrary", "library" ) );

	field_name_not_variable_name.push_back( pair< string, pair< string, string > >( "Text", pair< string, string >( "string", "stringF" ) ) );
  field_name_not_variable_name.push_back( pair< string, pair< string, string > >( "MetadataDouble", pair< string, string >( "name", "nameF" ) ) );
  field_name_not_variable_name.push_back( pair< string, pair< string, string > >( "MetadataFloat", pair< string, string >( "name", "nameF" ) ) );
  field_name_not_variable_name.push_back( pair< string, pair< string, string > >( "MetadataInteger", pair< string, string >( "name", "nameF" ) ) );
  field_name_not_variable_name.push_back( pair< string, pair< string, string > >( "MetadataSet", pair< string, string >( "name", "nameF" ) ) );
  field_name_not_variable_name.push_back( pair< string, pair< string, string > >( "MetadataString", pair< string, string >( "name", "nameF" ) ) );

  string input_dir = string( argv[1] );
	string first_file = input_dir + "\\*";
	if (first_file.size() > (MAX_PATH - 3)) {
		cerr << "Path to long, have to switch to UNICODE version" << argv[1] << endl;
		return 1;
	}
  WIN32_FIND_DATA find_data;
  HANDLE hFind = FindFirstFile( first_file.c_str(), &find_data );
  if (hFind == INVALID_HANDLE_VALUE) {
    cerr << "Can not find any files in input directory " << argv[1] << " for some reason." << endl;
    return 1;
  }
  
	// Used to find the name and its parent names.
	regex node_name_regex( ">[[:space:]]*([[:w:]]*)[[:space:]]*(?:[[:space:]]*:[[:space:]]*([^{]*))*[{]" );
	// Used to find field definition.
	vector< regex > field_definition_regex;
	field_definition_regex.push_back( regex( createFieldDefinitionString("") ) );
	field_definition_regex.push_back( regex( "[}]" ) );

	// Used to find field type, access type, name, default value and node type (if applicable)
	regex field_parts_regex( "[\\t ]*([/[:w:]]+)[\\t ]+\\[([,[:w:]]*)\\][ \\t]+([[:w:]]+)((?:.|\\s)*)" );

	// Using this for comparing of some ranges, should probably use wstring (or some unicode string) instead.
	inf_as_unicode.push_back( char(-30) );
	inf_as_unicode.push_back( char(-120) );
	inf_as_unicode.push_back( char(-98) );
	minus_as_unicode.push_back( char(-30) );
	minus_as_unicode.push_back( char(-120) );
	minus_as_unicode.push_back( char(-110) );
	pi_as_unicode.push_back( char(-49) );
	pi_as_unicode.push_back( char(-128) );

	vector< NodeDefinition * > node_definitions;
  do {
#ifndef UNICODE  
  string file_name = input_dir + "\\" + string( find_data.cFileName );
#else
  string file_name( wstring( find_data.cFileName ).c_str() );
#endif
    ifstream input_stream( file_name.c_str() );
		if( !input_stream.is_open() ) {
			cerr << "Could not open file " << file_name << " for some reason." << endl;
			continue;
		}
    vector< string > pre_strings;
    string tmp_string = "";
    bool storing = false;
    while( !input_stream.eof() && input_stream.peek() != EOF ) {
      if( !storing && input_stream.peek() == '<' ) {
        storing = true;
        tmp_string = "";
      }
      
			char c = input_stream.get();
      if( storing ) {
        tmp_string += c;
        if( tmp_string.size() == 4 && tmp_string != "<pre" )
          storing = false;
        else if( tmp_string.size() > 10 && tmp_string.substr( tmp_string.size()-7, 6 ) == "</pre>" ) {
          storing = false;
          pre_strings.push_back( tmp_string );
        }
      }
    }
    
		for( unsigned int i = 0; i < pre_strings.size(); ++i ) {
			smatch node_name_match;
			regex_search( pre_strings[i], node_name_match, node_name_regex );
			if( !node_name_match.empty()) {
				node_definitions.push_back( new NodeDefinition( node_name_match.str(1) ) );
				cerr << pre_strings[i] << endl;
				cerr << "matches " << node_name_match.size() << endl;
				for( unsigned int j = 0; j < node_name_match.size(); ++j )
					cerr << j << ":" << node_name_match.str(j) << ":" << endl;
				cerr << endl;
				string full_node_match = node_name_match.str();
				if( full_node_match == ">CADPart {" ||
						full_node_match == ">   DISEntityTypeMapping {" ||
						full_node_match == ">Transform {" ||
						full_node_match == ">  Transform {" ||
						full_node_match == ">Anchor {" ||
						full_node_match == ">Shape {" ||
						full_node_match == ">Script {" )
					continue;
				
				splitString( node_name_match.str(2), node_definitions.back()->parent_names );

				smatch field_name_match;
				string to_check = pre_strings[i];
				checkSeveralRegEx( to_check, field_name_match, field_definition_regex );
				/*cerr << "field matches" << field_name_match.size() << endl;
				for( unsigned int j = 0; j < field_name_match.size(); ++j )
					cerr << j << ":" << field_name_match.str(j) << ":" << endl;
				cerr << endl;*/
				string one_field_def = field_name_match.str(0);
				to_check = field_name_match.suffix().str();
				while( checkSeveralRegEx( to_check, field_name_match, field_definition_regex ) ) {
					/*cerr << "field matches" << field_name_match.size() << endl;
					for( unsigned int j = 0; j < field_name_match.size(); ++j )
						cerr << j << ":" << field_name_match.str(j) << ":" << endl;
					cerr << endl;*/
					smatch field_parts_match;
					one_field_def = one_field_def + field_name_match.prefix().str();
          replaceInString( one_field_def, "&quot;", "\"" );
					replaceInString( one_field_def, "<span class=\"times\">", "" );
					replaceInString( one_field_def, "</span>", "" );
					if( regex_search( one_field_def, field_parts_match, field_parts_regex ) ) {
						/*cerr << "field part" << field_parts_match.size() << endl;
						for( unsigned int j = 0; j < field_parts_match.size(); ++j )
							cerr << j << ":" << field_parts_match.str(j) << ":" << endl;
						cerr << endl;*/
						Field * new_field = NULL;
						string field_type_string = field_parts_match.str(1);
						// Handle /d typo? or if it is a typo?
						if( field_type_string.at( field_type_string.size()-2 ) == '/' )
							new_field = X3DTypes::newFieldInstance( field_type_string.substr( 0, field_type_string.size()-2 ).c_str() );
						else
							new_field = X3DTypes::newFieldInstance( field_type_string.c_str() );
						if( new_field ) {
							new_field->setName( field_parts_match.str(3) );
							node_definitions.back()->fields.push_back( new FieldDefinition( new_field ) );
							string in_out = field_parts_match.str(2);
							if( in_out.empty() )
								new_field->setAccessType( Field::INITIALIZE_ONLY );
							else if( in_out == "in" )
								new_field->setAccessType( Field::INPUT_ONLY );
							else if( in_out == "out" )
								new_field->setAccessType( Field::OUTPUT_ONLY );
							string default_value_string = field_parts_match.str(4);
							if( default_value_string != "" ) {
								switch( new_field->getX3DType() ) {
									case X3DTypes::MFFLOAT:
									case X3DTypes::MFDOUBLE:
									case X3DTypes::MFINT32:
									case X3DTypes::MFVEC3F:
									case X3DTypes::MFVEC3D:
									case X3DTypes::MFCOLOR: {
										regex value_regex( "[\\t ]*(?:([\\-.0-9]+[\\t ]+[\\-.0-9]+[\\t ]+[\\-.0-9])+|(?:\\[|\")([^\\]]*)(?:\\]|\")[\\t ]*)(.*)" );
										smatch match_object;
										if( regex_search( default_value_string, match_object, value_regex ) ) {
											vector< string > possible_values;
											possible_values.push_back( match_object.str(1) );
											possible_values.push_back( match_object.str(2) );
											handleParsableField( possible_values, match_object.str(3), *(node_definitions.back()) );
										} else {
											value_regex = regex( "[\\t ]*([\\-.0-9]+[\\t ,]+[\\-.0-9]+[\\t ,]+[\\-.0-9][\\t, ]+[\\-.0-9]+)[\\t ]*(.*)" );
											if( regex_search( default_value_string, match_object, value_regex ) ) {
												vector< string > possible_values;
												possible_values.push_back( match_object.str(1) );
												handleParsableField( possible_values, match_object.str(2), *(node_definitions.back()) );
											} else {
												value_regex = regex( "[\\t ]*([\\-.0-9]+)[\\t ]*(.*)" );
												if( regex_search( default_value_string, match_object, value_regex ) ) {
													vector< string > possible_values;
													possible_values.push_back( match_object.str(1) );
													handleParsableField( possible_values, match_object.str(2), *(node_definitions.back()) );
												} else {
													checkForNoSpace( default_value_string, new_field );
												}
											}
										}
										break;
									}
									case X3DTypes::MFSTRING: {
										regex value_regex = regex( "[\\t ]*(?:\\[([^\\]]*)\\]|\"([^\"]*)\")[\\t ]*((?:.|[\\t\\n ])*)" );
										smatch match_object;
										if( regex_search( default_value_string, match_object, value_regex ) ) {
											vector< string > possible_values;
											possible_values.push_back( match_object.str(1) );
											possible_values.push_back( match_object.str(2) );
											handleParsableField( possible_values, match_object.str(3), *(node_definitions.back()) );
										} else {
											checkForNoSpace( default_value_string, new_field );
										}
										break;
									}
									case X3DTypes::SFVEC2F: {
										regex value_regex( "[\\t ]*([\\-.0-9]+[\\t ]+[\\-.0-9]+)[\\t ]*(.*)" );
										smatch match_object;
										if( regex_search( default_value_string, match_object, value_regex ) ) {
											vector< string > possible_values;
											possible_values.push_back( match_object.str(1) );
											handleParsableField( possible_values, match_object.str(2), *(node_definitions.back()) );
										} else {
											value_regex = regex( "[\\t ]*\\([^)]*\\)$" );
											if( regex_search( default_value_string, match_object, value_regex ) ) {
												vector< string > possible_values;
												handleParsableField( possible_values, match_object.str(1), *(node_definitions.back()) );
											} else {
												checkForNoSpace( default_value_string, new_field );
											}
										}
										break;
									}
									case X3DTypes::SFCOLOR:
									case X3DTypes::SFVEC3F:
									case X3DTypes::SFVEC3D: {
										regex value_regex( "[\\t ]*([\\-.0-9]+[\\t ]+[\\-.0-9]+[\\t ]+[\\-.0-9]+)[\\t ]*(.*)" );
										smatch match_object;
										if( regex_search( default_value_string, match_object, value_regex ) ) {
											vector< string > possible_values;
											possible_values.push_back( match_object.str(1) );
											handleParsableField( possible_values, match_object.str(2), *(node_definitions.back()) );
										} else {
											checkForNoSpace( default_value_string, new_field );
										}
										break;
									}
									case X3DTypes::SFTIME:
									case X3DTypes::SFFLOAT:
									case X3DTypes::SFDOUBLE:
									case X3DTypes::SFINT32: {
										regex value_regex( "[\\t ]*([\\-.0-9&pi;/#" + pi_as_unicode + "]*)[\\t ]*(.*)" );
										smatch match_object;
										if( regex_search( default_value_string, match_object, value_regex ) ) {
											vector< string > possible_values;
											possible_values.push_back( match_object.str(1) );
											handleParsableField( possible_values, match_object.str(2), *(node_definitions.back()) );
										} else {
											cerr << "new version of default value for " << new_field->getTypeName() << "." << endl;
											system( "PAUSE" );
										}
										break;
									}
									case X3DTypes::SFBOOL: {
										regex value_regex( "[\\t ]*(TRUE|FALSE)?" );
										smatch match_object;
										if( regex_search( default_value_string, match_object, value_regex ) ) {
											if( match_object.str(1) != "" ) {
												static_cast< ParsableField * >(new_field)->setValueFromString( match_object.str(1) );
                        node_definitions.back()->fields.back()->have_default_value = true;
                      }
										} else {
											cerr << "new version of default value for " << new_field->getTypeName() << "." << endl;
											system( "PAUSE" );
										}
										break;
									}
									case X3DTypes::MFVEC2F:
									case X3DTypes::MFVEC2D:
									case X3DTypes::MFBOOL: {
										regex value_regex( "[\\t ]*(?:\\[(.*)\\]|([\\-.0-9]+[\\t ]+[\\-.0-9]+))[\\t ]*(.*)" );
										smatch match_object;
										if( regex_search( default_value_string, match_object, value_regex ) ) {
											vector< string > possible_values;
											possible_values.push_back( match_object.str(1) );
											possible_values.push_back( match_object.str(2) );
											handleParsableField( possible_values, match_object.str(3), *(node_definitions.back()) );
										} else {
											checkForNoSpace( default_value_string, new_field );
										}
										break;
									}
									case X3DTypes::SFSTRING: {
										regex value_regex( "[\\t ]*\"([^\"]*)\"[\\t ]*((?:.|[\\t\\n ])*)" );
										smatch match_object;
										if( regex_search( default_value_string, match_object, value_regex ) ) {
											vector< string > possible_values;
											possible_values.push_back( match_object.str(1) );
											handleParsableField( possible_values, match_object.str(2), *(node_definitions.back()) );
										} else {
											checkForNoSpace( default_value_string, new_field );
										}
										break;
									}
									case X3DTypes::SFNODE: {
										regex value_regex( "[\\t ]*([[:w:]]+)[\\t ]+\\[([,| [:w:]]*)\\]" );
										smatch match_object;
										if( regex_search( default_value_string, match_object, value_regex ) ) {
											if( match_object.str(1) != "NULL" ) {
												cerr << "don't handle non-NULL objects yet for" << new_field->getTypeName() << "." << endl;
                        node_definitions.back()->fields.back()->have_default_value = true;
												system( "PAUSE" );
											}
											if( match_object.str(2) != "" ) {
												splitString( match_object.str(2), node_definitions.back()->fields.back()->extra_info );
											}
										} else {
											cerr << "new version of default value for " << new_field->getTypeName() << "." << endl;
											system( "PAUSE" );
										}
										break;
									}
									case X3DTypes::MFNODE: {
										regex value_regex( "[\\t ]*\\[([[:w:]]*)\\][\\t ]+\\[([,[:w:]]*)\\]" );
										smatch match_object;
										if( regex_search( default_value_string, match_object, value_regex ) ) {
											if( match_object.str(1) != "" ) {
												cerr << "don't handle non-empty objects yet for " << new_field->getTypeName() << "." << endl;
                        node_definitions.back()->fields.back()->have_default_value = true;
												system( "PAUSE" );
											}
											if( match_object.str(2) != "" ) {
												splitString( match_object.str(2), node_definitions.back()->fields.back()->extra_info );
											}
										} else {
											value_regex = regex( "[\\t ]*\\[([[:w:],]*)\\][\\t ]*$" );
											if( regex_search( default_value_string, match_object, value_regex ) ) {
												if( match_object.str(1) != "" ) {
													splitString( match_object.str(1), node_definitions.back()->fields.back()->extra_info );
												}
											} else {
												value_regex = regex( "[\\t ]*(NULL|[ ])[\\t ]+\\[((?:.|[\\t\\n ])*)\\]" );
												if( regex_search( default_value_string, match_object, value_regex ) ) {
													if( match_object.str(2) != "" ) {
														splitString( match_object.str(2), node_definitions.back()->fields.back()->extra_info );
													}
												} else {
													checkForNoSpace( default_value_string, new_field );
												}
											}
										}
										break;
									}
									case X3DTypes::SFVEC4F:
									case X3DTypes::SFCOLORRGBA:
									case X3DTypes::MFCOLORRGBA:
									case X3DTypes::MFROTATION:
									case X3DTypes::SFROTATION: {
										regex value_regex( "[\\t ]*(?:([\\-.0-9]+[\\t ]+[\\-.0-9]+[\\t ]+[\\-.0-9]+[\\t ]+[\\-.0-9]+)|\\[([^\\]]*)\\])[\\t ]*(.*)" );
										smatch match_object;
										if( regex_search( default_value_string, match_object, value_regex ) ) {
											vector< string > possible_values;
											possible_values.push_back( match_object.str(1) );
											possible_values.push_back( match_object.str(2) );
											handleParsableField( possible_values, match_object.str(3), *(node_definitions.back()) );
										} else {
											checkForNoSpace( default_value_string, new_field );
										}
										break;
									}
									case X3DTypes::SFMATRIX3F: {
										regex value_regex( "[\\t ]*([\\-.0-9]+[\\t ]+[\\-.0-9]+[\\t ]+[\\-.0-9]+[\\t \\n]+[\\-.0-9]+[\\t ]+[\\-.0-9]+[\\t ]+[\\-.0-9]+[\\t \\n]+[\\-.0-9]+[\\t ]+[\\-.0-9]+[\\t ]+[\\-.0-9]+)[\\t ]*((?:.|[\\t\\n ])*)" );
										smatch match_object;
										if( regex_search( default_value_string, match_object, value_regex ) ) {
											vector< string > possible_values;
											possible_values.push_back( match_object.str(1) );
											handleParsableField( possible_values, match_object.str(2), *(node_definitions.back()) );
										} else {
											checkForNoSpace( default_value_string, new_field );
										}
										break;
									}
									case X3DTypes::SFMATRIX4F: {
										regex value_regex( "[\\t ]*([\\-.0-9]+[\\t ]+[\\-.0-9]+[\\t ]+[\\-.0-9]+[\\t ]+[\\-.0-9]+[\\t ]+[\\-.0-9]+[\\t ]+[\\-.0-9]+[\\t ]+[\\-.0-9]+[\\t ]+[\\-.0-9]+[\\t ]+[\\-.0-9]+[\\t ]+[\\-.0-9]+[\\t ]+[\\-.0-9]+[\\t ]+[\\-.0-9]+[\\t ]+[\\-.0-9]+[\\t ]+[\\-.0-9]+[\\t ]+[\\-.0-9]+[\\t ]+[\\-.0-9]+)[\\t ]*((?:.|[\\t\\n ])*)" );
										smatch match_object;
										if( regex_search( default_value_string, match_object, value_regex ) ) {
											vector< string > possible_values;
											possible_values.push_back( match_object.str(1) );
											handleParsableField( possible_values, match_object.str(2), *(node_definitions.back()) );
										} else {
											checkForNoSpace( default_value_string, new_field );
										}
										break;
									}

									case X3DTypes::MFVEC4F:
									case X3DTypes::MFMATRIX3F:
									case X3DTypes::MFMATRIX4F: {
										regex value_regex( "[\\t ]*\\[([[:w:]]*)\\][\\t ]*((?:.|[\\t\\n ])*)" );
										smatch match_object;
										if( regex_search( default_value_string, match_object, value_regex ) ) {
											vector< string > possible_values;
											possible_values.push_back( match_object.str(1) );
											handleParsableField( possible_values, match_object.str(2), *(node_definitions.back()) );
										} else {
											checkForNoSpace( default_value_string, new_field );
										}
										break;
									}
									default: {
										cerr << "did not implement handling of default value for " << new_field->getTypeName() << "." << endl;
										cerr << "Should not get here." << endl;
										system( "PAUSE" );
									}
								}
							}
						} else {
							cerr << "could not create field of type " << field_parts_match.str(1).c_str() << endl;
							system( "PAUSE" );
						}
					} else {
						cerr << "Unhandled field definition" << one_field_def << endl;
						system( "PAUSE" );
					}
					one_field_def = field_name_match.str(0);
					to_check = field_name_match.suffix().str();
				}
			}
		}
  } while( FindNextFile( hFind, &find_data ) != 0 );
   
  FindClose(hFind);
  
  string out_file = argv[2];
  ofstream os( out_file.c_str() );
  if( !os.is_open() )
    return 1;

	try {
    typedef pair< Node *, NodeDefinition * > NodePair;
    vector< NodePair > nodes_to_check;
		int total_non_abstract_nodes = 0;
		os << "-----------Node in X3D not in H3D------------" << endl;
		for( unsigned int i = 0; i < node_definitions.size(); ++i ) {
			if( node_definitions[i]->name.substr( 0, 3 ) != "X3D" ) {
				total_non_abstract_nodes++;
        Node *n = H3DNodeDatabase::createNode( node_definitions[i]->name );
        if( n ) {
          nodes_to_check.push_back( NodePair( n, node_definitions[i] ) );
        } else if( !n ) {
          os << node_definitions[i]->name << endl;
        }
      }
    }
		os << "In total we have implemented " << 100 * float( nodes_to_check.size() ) / total_non_abstract_nodes << " % of the X3D nodes that does not start with X3D." << endl;
		os << "----------------------------------------------" << endl << endl << endl;
    
    
    os << "-----------Differences between X3D and H3D------------" << endl;
    for( unsigned int i = 0; i < nodes_to_check.size(); ++i ) {
      Node * n = nodes_to_check[i].first;
      NodeDefinition * n_def = nodes_to_check[i].second;
      bool listed = false;
      for( vector< FieldDefinition * >::iterator j = n_def->fields.begin(); j != n_def->fields.end(); ++j ) {
        Field * x3d_field = (*j)->field;
        Field * h3d_field = n->getField( x3d_field->getName() );
        if( h3d_field ) {
					string parent_field_info = "";
					string parent_prefix = fieldDeclaredInParent( n_def, x3d_field->getName(), node_definitions, &parent_field_info );
					if( parent_prefix != "" )
						parent_prefix += "::";
          bool access_type = x3d_field->getAccessType() != h3d_field->getAccessType();
          bool x3d_type = x3d_field->getTypeName() != h3d_field->getTypeName();
          bool default_value = false;
          if( (*j)->have_default_value ) {
            if( ParsableField * x3d_parse_field = dynamic_cast< ParsableField * >(x3d_field) ) {
              if( ParsableField * h3d_parse_field = dynamic_cast< ParsableField * >(h3d_field) )
                default_value = x3d_parse_field->getValueAsString() != h3d_parse_field->getValueAsString();
            } else if( SFNode * x3d_sf_field = dynamic_cast< SFNode * >(x3d_field) ) {
              if( SFNode * h3d_sf_field = dynamic_cast< SFNode * >(h3d_field) ) {
                default_value = x3d_sf_field->getValue() != h3d_sf_field->getValue();
              }
            } else if( MFNode * x3d_mf_field = dynamic_cast< MFNode * >(x3d_field) ) {
              if( MFNode * h3d_mf_field = dynamic_cast< MFNode * >(h3d_field) ) {
                const NodeVector x3d_node_vector = x3d_mf_field->getValue();
                const NodeVector h3d_node_vector = h3d_mf_field->getValue();
                if( x3d_node_vector.empty() && h3d_node_vector.empty() ) {
                  default_value = true;
                } else if( x3d_node_vector.size() == h3d_node_vector.size() ) {
                  bool all_values_equal = true;
                  for( unsigned int k = 0; k < x3d_node_vector.size(); k++ ) {
                    if( x3d_node_vector[k] != h3d_node_vector[k] ) {
                      all_values_equal = false;
                    }
                  }
                  default_value = all_values_equal;
                }
              }
            }
          }
#ifdef DONT_LIST_INITIALIZE_TO_INPUT_OUTPUT
          if( access_type && x3d_field->getAccessType() == Field::INITIALIZE_ONLY && h3d_field->getAccessType() == Field::INPUT_OUTPUT )
            access_type = false;
#endif
          if( access_type || x3d_type || default_value ) {
            if( !listed ) {
              os << "***********************" << endl
                 << n_def->name << endl;
              listed = true;
            }
            os << "Field: " << parent_prefix << x3d_field->getName() << endl;
            if( access_type )
              os << "  Access type X3D vs H3D: " << accessTypeAsString( x3d_field->getAccessType() ) << " ; " << accessTypeAsString( h3d_field->getAccessType() ) << endl;
            if( x3d_type )
              os << "  Type X3D vs H3D: " << x3d_field->getTypeName() << " ; " << h3d_field->getTypeName() << endl;
            if( default_value ) {
              os << "  Default value X3D vs H3D: " << getFieldDefaultValueAsString( x3d_field ) << " ; " << getFieldDefaultValueAsString( h3d_field ) << endl;
            }
						if( parent_field_info != "" )
							os << "  Parent field info: " << parent_field_info << endl;
          }
        } else {
          if( !listed ) {
            os << "***********************" << endl
               << n_def->name << endl;
            listed = true;
          }
					string parent_prefix = fieldDeclaredInParent( n_def, x3d_field->getName(), node_definitions );
					if( parent_prefix != "" )
						parent_prefix += "::";
          os << "Missing Field: " << parent_prefix << x3d_field->getName() << endl;
        }
      }
      if( listed )
        os << "***********************" << endl;
      delete n;
    }
    os << "----------------------------------------------" << endl << endl << endl;

    
    vector< string > ordered_node_names;
		for( H3DNodeDatabase::NodeDatabaseConstIterator i = 
								H3DNodeDatabase::begin();
				i != H3DNodeDatabase::end();
				i++ ) {
			ordered_node_names.push_back( (*i).second->getName() );
		}

		sort( ordered_node_names.begin(), ordered_node_names.end() );
		os << "-----------Node in H3D not in X3D------------" << endl;
		for( unsigned int i = 0; i < ordered_node_names.size(); ++i ) {
			if( ordered_node_names[i].substr( 0, 3 ) != "X3D" && ordered_node_names[i].substr( 0, 3 ) != "H3D" && ordered_node_names[i] != "Image3DTexture" && ordered_node_names[i] != "Pixel3DTexture" ) {
				bool found = false;
				for( unsigned int j = 0; j < node_definitions.size(); ++j ) {
					if( ordered_node_names[i] == node_definitions[j]->name ) {
						found = true;
						break;
					}
				}
				if( !found )
					os << ordered_node_names[i] << endl;
			}
		}
		os << "----------------------------------------------" << endl << endl << endl;

		cerr << "DONE!" << endl;
		os.close();
	} catch (const Exception::H3DException &e) {
		os.close();
		cerr << e << endl;
	}
  for( unsigned int i = 0; i < node_definitions.size(); ++i )
		delete node_definitions[i];

  system( "PAUSE" );
  return 0;
}

 
