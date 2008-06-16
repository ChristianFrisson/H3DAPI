#include <H3D/H3DNodeDatabase.h>
#include <H3D/Scene.h>
#include <H3DUtil/Exception.h>
#include <H3D/Node.h>
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
	if( f == Scene::eventSink.get() ) {
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
  delete n;
}

#include <H3D/X3DComposedGeometryNode.h>
#include <H3D/X3DTexture2DNode.h>
#include <H3D/X3DTextureTransform2DNode.h>
#include <H3D/X3DShapeNode.h>
#include <H3D/X3DBackgroundNode.h>
#include <H3D/H3DHapticsDevice.h>
#include <H3D/X3DTexture3DNode.h>
#include <H3D/X3DSoundSourceNode.h>
#include <H3D/X3DViewpointNode.h>
#include <H3D/X3DPointingDeviceSensorNode.h>
#include <H3D/X3DTouchSensorNode.h>
#include <H3D/X3DNurbsSurfaceGeometryNode.h>
#include <H3D/X3DLightNode.h>
#include <H3D/X3DGroupingNode.h>
#include <H3D/X3DGeometryNode.h>
#include <H3D/Scene.h>

inline void resetSceneTimeField() {
  Scene::time.reset( new SFTime( TimeStamp() ) );
  Scene::time->setName( "Scene::time" );
}


int main(int argc, char* argv[]) {

  if (argc < 2){
    cerr << "Usage: " << argv[0] << " <Output dir>" << endl;
    return 1;
  }

  string out_dir = argv[1];

  writeNode( out_dir, new Scene );
  resetSceneTimeField();
  writeNode( out_dir, new X3DComposedGeometryNode );
  resetSceneTimeField();
  writeNode( out_dir, new H3DHapticsDevice );
  resetSceneTimeField();
  writeNode( out_dir, new X3DTexture2DNode );
  resetSceneTimeField();
  writeNode( out_dir, new X3DTexture3DNode );
  resetSceneTimeField();
  writeNode( out_dir, new X3DTextureTransform2DNode );
  resetSceneTimeField();
  writeNode( out_dir, new X3DShapeNode );
  resetSceneTimeField();
  writeNode( out_dir, new X3DBackgroundNode );
  resetSceneTimeField();
  writeNode( out_dir, new X3DSoundSourceNode );
  resetSceneTimeField();
  writeNode( out_dir, new X3DViewpointNode );
  resetSceneTimeField();
  writeNode( out_dir, new X3DPointingDeviceSensorNode );
  resetSceneTimeField();
  writeNode( out_dir, new X3DTouchSensorNode );
  resetSceneTimeField();
  writeNode( out_dir, new X3DNurbsSurfaceGeometryNode );
  resetSceneTimeField();
  writeNode( out_dir, new X3DLightNode );
  resetSceneTimeField();
  writeNode( out_dir, new X3DGroupingNode );
  resetSceneTimeField();

  try {
    for( H3DNodeDatabase::NodeDatabaseConstIterator i = 
           H3DNodeDatabase::begin();
         i != H3DNodeDatabase::end();
         i++ ) {
      string node_name = (*i).first;
      Node *n = H3DNodeDatabase::createNode( node_name.c_str()  );
      // The dot hierarchy gets completely screwed up without this since
      // routes and fields are left over from previous instance of Scene::time.
      if( !n ) {
        cerr << node_name << ": No such node exists in the node database" <<
          endl;
      } else {
        writeNode( out_dir, n );
        resetSceneTimeField();
      }
    }
    cerr << "DONE!" << endl;
  }
  catch (const Exception::H3DException &e) {
      cerr << e << endl;
    }
  }

