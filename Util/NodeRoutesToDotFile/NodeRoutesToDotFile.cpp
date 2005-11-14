
#include "H3DNodeDatabase.h"
#include "Scene.h"
#include "Exception.h"
#include "Node.h"
#include <set>
#include <queue>
#include <fstream>

using namespace std;
using namespace H3D;

void writeFieldDef( ostream &os, Field *f, bool filled = false ) {
  os << "\"" << f->getName() << "\" [URL=\"\\ref " 
     << f->getName() << "\"";
  if( filled )
    os << ", style=filled, fillcolor=steelblue1";
  os <<"  ]" << endl; 
}

void writeRoute( ostream &os, Field *src, Field *dest ) {
   os << "\"" << src->getName() << "\"" << " -> " 
      << "\"" << dest->getName() << "\"" << endl;
}

void writeNode( string out_dir, Node *n ) {
  if( !n ) {
 
    return;
  }
  set< Field * > visited;
  set< Field * > to_visit;
    
  H3DNodeDatabase *db = H3DNodeDatabase::lookupTypeId( typeid(*n) );  
    
  string node_name = n->getTypeName();    
    
  for( H3DNodeDatabase::FieldDBConstIterator i = db->fieldDBBegin();
       i != db->fieldDBEnd(); i++ ) {
    to_visit.insert( n->getField( (*i).c_str() ) );
  }
  

  
  string out_file = out_dir + "\\" + node_name + ".dot";
  
  ofstream os( out_file.c_str() );
  os << "digraph g { \n"
     << "graph [ rankdir = \"LR\" ]; \n"
     << "node [ fontsize = \"10\" \n"
     << "       shape = \"ellipse\" \n"
     << "]; \n";
  
  while( !to_visit.empty() ) {
    Field *f = *(to_visit.begin());
    visited.insert( f );
	if( f == Scene::eventSink ) {
	  writeFieldDef( os, f );
	  to_visit.erase( f );
	  continue;
	}

    const Field::FieldSet &routes_out = f->getRoutesOut();
    
    string out_file_field = 
      out_dir + "\\" + node_name +
      "_" + f->getName() + ".dot";
    
    ofstream os_field( out_file_field.c_str() );
    os_field << "digraph g { \n"
             << "graph [ rankdir = \"LR\" ]; \n"
             << "node [ fontsize = \"10\" \n"
             << "       shape = \"ellipse\" \n"
             << "]; \n";
    
    writeFieldDef( os, f );
    writeFieldDef( os_field, f, true );
    
    for( Field::FieldSet::const_iterator out = routes_out.begin();
         out != routes_out.end(); out++ ) {
      
      writeFieldDef( os_field, *out );
      writeRoute( os_field, f, *out );
      writeRoute( os, f, *out );
      
      if( visited.find( *out ) == visited.end() &&
          to_visit.find( *out ) == to_visit.end() ) {
        to_visit.insert( *out );
      }
    }
    const Field::FieldVector &routes_in = f->getRoutesIn();
    for( Field::FieldVector::const_iterator in = routes_in.begin();
         in != routes_in.end(); in++ ) {
      writeFieldDef( os_field, *in );
      writeRoute( os_field,*in, f );
      if( visited.find( *in ) == visited.end() &&
          to_visit.find( *in ) == to_visit.end()) {
        to_visit.insert( *in );
      }
    }
    to_visit.erase( f );
    os_field << "}" << endl;  
    os_field.close();
  }
  os << "}" << endl;
  os.close();
}

#include "X3DComposedGeometryNode.h"
#include "X3DTexture2DNode.h"
#include "X3DTextureTransform2DNode.h"
#include "X3DShapeNode.h"
#include "X3DBackgroundNode.h"
#include "H3DHapticsDevice.h"
#include "X3DTexture3DNode.h"


int main(int argc, char* argv[]) {

  if (argc < 2){
    cerr << "Usage: " << argv[0] << " <Output dir>" << endl;
    return 1;
  }

  string out_dir = argv[1];

  writeNode( out_dir, new X3DComposedGeometryNode );
  writeNode( out_dir, new H3DHapticsDevice );

  writeNode( out_dir, new X3DTexture2DNode );
  writeNode( out_dir, new X3DTexture3DNode );
  writeNode( out_dir, new X3DTextureTransform2DNode );
  writeNode( out_dir, new X3DShapeNode );
  writeNode( out_dir, new X3DBackgroundNode );

  try {
    for( H3DNodeDatabase::NodeDatabaseConstIterator i = 
           H3DNodeDatabase::begin();
         i != H3DNodeDatabase::end();
         i++ ) {
      string node_name = (*i).first;
      Node *n = H3DNodeDatabase::createNode( node_name.c_str()  );
	  if( !n ) {
		  cerr << node_name << ": No such node exists in the node database" << endl;
	  } else {
      writeNode( out_dir, n );
	  }
    }
  cerr << "DONE!" << endl;
  }
  catch (const Exception::H3DException &e) {
      cerr << e << endl;
    }
  }

