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
/// \file NodeRoutesToDotFile.cpp
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

string getFieldName( Field *f, string node_name ) {
  string field_name = f->getName();
  if( node_name == "UniversalJoint" && field_name == "stopBounce1" )
    return "stop1Bounce";
  return field_name;
}

void writeFieldDef( ostream &os, Field *f, string nodename = "", bool filled = false ) {
  string field_name = getFieldName( f, nodename );
  os << "\"" << nodename << field_name << "\" [URL=\"\\ref " 
     << nodename << field_name << "\"";
  if( filled )
    os << ", style=filled, fillcolor=steelblue1";
  os <<"  ]" << endl; 
}

void writeRoute( ostream &os, Field *src, Field *dest, string srcnodename, string destnodename ) {
   os << "\"" << srcnodename << src->getName() << "\"" << " -> " 
      << "\"" << destnodename << dest->getName() << "\"" << endl;
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
      "_" + getFieldName( f, node_name ) + ".dot";

    string field_own_name = "";

    Node * temp_owner = f->getOwner();
    if( temp_owner && temp_owner->getTypeName() != node_name ) {
      field_own_name = temp_owner->getTypeName() + "_";
      if( temp_owner->getName().find( "Unnamed " ) == string::npos ) {
        field_own_name = temp_owner->getName() + "_";
      }
      out_file_field = out_dir + "\\" + node_name + "_" + 
         field_own_name + getFieldName( f, field_own_name ) + ".dot";
    }

    bool set_up_routes = false;

    for( Field::FieldSet::const_iterator out = routes_out.begin();
         out != routes_out.end(); out++ ) {
      if( visited.find( *out ) == visited.end() &&
          to_visit.find( *out ) == to_visit.end() ) {
        to_visit.insert( *out );
      }
    }

    const Field::FieldVector &routes_in = f->getRoutesIn();
    for( Field::FieldVector::const_iterator in = routes_in.begin();
         in != routes_in.end(); in++ ) {
      if( visited.find( *in ) == visited.end() &&
          to_visit.find( *in ) == to_visit.end()) {
        to_visit.insert( *in );
      }
      
    }

    //if( routes_out.empty() && routes_in.empty() ) {
      temp_owner = f->getOwner();
      if( temp_owner && temp_owner == n ) {
        set_up_routes = true;
      }
    //}

    if( set_up_routes ) {
      ofstream os_field( out_file_field.c_str() );
      os_field << "digraph g { \n"
               << "graph [ rankdir = \"LR\" ]; \n"
               << "node [ fontsize = \"10\" \n"
               << "       shape = \"ellipse\" \n"
               << "]; \n";

      writeFieldDef( os, f, field_own_name );
      writeFieldDef( os_field, f, field_own_name, true );

      for( Field::FieldSet::const_iterator out = routes_out.begin();
           out != routes_out.end(); out++ ) {
      
        string dest_node_name = "";
        temp_owner = (*out)->getOwner();
        if( temp_owner && temp_owner->getTypeName() != node_name ) {
          dest_node_name = temp_owner->getTypeName() + "_";
          if( temp_owner->getName().find( "Unnamed " ) == string::npos ) {
            dest_node_name = temp_owner->getName() + "_";
          }
        }

        writeFieldDef( os_field, *out, dest_node_name );
        writeRoute( os_field, f, *out, field_own_name, dest_node_name );
        if( visited.find( *out ) == visited.end() || (temp_owner && temp_owner != n ) ) {
          writeRoute( os, f, *out, field_own_name, dest_node_name );
        }
      }
    
      for( Field::FieldVector::const_iterator in = routes_in.begin();
           in != routes_in.end(); in++ ) {
  
        string src_node_name = "";
        temp_owner = (*in)->getOwner();
        if( temp_owner && temp_owner->getTypeName() != node_name ) {
          src_node_name = temp_owner->getTypeName() + "_";
          if( temp_owner->getName().find( "Unnamed " ) == string::npos ) {
            src_node_name = temp_owner->getName() + "_" ;
          }
        }

        writeFieldDef( os_field, *in, src_node_name );
        writeRoute( os_field,*in, f, src_node_name, field_own_name );
        if( visited.find( *in ) == visited.end() || (temp_owner && temp_owner != n ) ) {
          writeRoute( os,*in, f, src_node_name, field_own_name );
        }
      }
    os_field << "}" << endl;  
    os_field.close();
    }
    to_visit.erase( f );
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
#include <H3D/H3DGeneratedShaderNode.h>

#ifdef HAVE_UI
#include <H3D/UI/Frame.h>
#include <H3D/UI/H3DLabeledWidget.h>
#include <H3D/UI/H3DButtonNode.h>
#endif

#ifdef HAVE_H3DPhysics
#include <H3D/H3DPhysics/X3DNBodyCollisionSpaceNode.h>
#include <H3D/H3DPhysics/SliderJoint.h>
#include <H3D/H3DPhysics/H3DAttachmentNode.h>
#include <H3D/H3DPhysics/H3DBodyConstraintNode.h>
#include <H3D/H3DPhysics/H3DBodyInteractorNode.h>
#include <H3D/H3DPhysics/H3DBodyModifierNode.h>
#include <H3D/H3DPhysics/H3DDeformationStrategyNode.h>
#include <H3D/H3DPhysics/H3DDeviceSoftBodyModifierNode.h>
#include <H3D/H3DPhysics/H3DGeometryMapping.h>
#include <H3D/H3DPhysics/H3DJointNode.h>
#include <H3D/H3DPhysics/H3DPhysicsDampingNode.h>
#include <H3D/H3DPhysics/H3DPhysicsElasticityNode.h>
#include <H3D/H3DPhysics/H3DPhysicsFrictionNode.h>
#include <H3D/H3DPhysics/H3DPhysicsMassNode.h>
#include <H3D/H3DPhysics/H3DPhysicsMaterialNode.h>
#include <H3D/H3DPhysics/H3DPhysicsMaterialPropertyNode.h>
#include <H3D/H3DPhysics/H3DPhysicsPoissonRatioNode.h>
#include <H3D/H3DPhysics/H3DPhysicsStiffnessNode.h>
#include <H3D/H3DPhysics/H3DSoftBodyAttachment.h>
#include <H3D/H3DPhysics/H3DSoftBodyLoader.h>
#include <H3D/H3DPhysics/H3DSoftBodyNode.h>
#include <H3D/H3DPhysics/H3DSoftBodyOutputNode.h>
#include <H3D/H3DPhysics/H3DSolverNode.h>
#include <H3D/H3DPhysics/H3DVertexBodyConstraintNode.h>
#include <H3D/H3DPhysics/X3DNBodyCollidableNode.h>
#endif

inline void resetSceneTimeField() {
  Scene::time.reset( new SFTime( TimeStamp() ) );
  Scene::time->setName( "Scene::time" );
}


int main(int argc, char* argv[]) {

  if( argc < 2 ){
    cerr << "Usage: " << argv[0] << " <Output dir>" << endl;
    return 1;
  }
#ifdef HAVE_UI
  {
    // To make sure that UI is linked in if used.
    auto_ptr<Frame> kurt( new Frame );
  }
#endif

#ifdef HAVE_H3DPhysics
  {
    // To make sure that UI is linked in if used.
    auto_ptr<SliderJoint> kurt( new SliderJoint );
  }
#endif

  vector< string > extra_libraries;
#ifdef _DEBUG${NodeRoutesToDotFile_EXTRA_DEBUG_LIBRARIES_CODE}
#else if NDEBUG${NodeRoutesToDotFile_EXTRA_RELEASE_LIBRARIES_CODE}
#endif

  for( unsigned int i = 0; i < extra_libraries.size(); i++ ) {
    auto_ptr< ImportLibrary > il( new ImportLibrary );
    il->url->push_back( extra_libraries[i] );
    il->ref();
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
  writeNode( out_dir, new H3DGeneratedShaderNode );
  resetSceneTimeField();
#ifdef HAVE_UI
  writeNode( out_dir, new H3DWidgetNode );
  resetSceneTimeField();
  writeNode( out_dir, new H3DLabeledWidget );
  resetSceneTimeField();
  writeNode( out_dir, new H3DButtonNode );
  resetSceneTimeField();
#endif
#ifdef HAVE_H3DPhysics
  writeNode( out_dir, new X3DNBodyCollidableNode );
  resetSceneTimeField();
  writeNode( out_dir, new X3DNBodyCollisionSpaceNode );
  resetSceneTimeField();
  writeNode( out_dir, new H3DAttachmentNode );
  resetSceneTimeField();
  writeNode( out_dir, new H3DBodyConstraintNode );
  resetSceneTimeField();
  writeNode( out_dir, new H3DBodyInteractorNode );
  resetSceneTimeField();
  writeNode( out_dir, new H3DBodyModifierNode );
  resetSceneTimeField();
  writeNode( out_dir, new H3DBodyNode );
  resetSceneTimeField();
  writeNode( out_dir, new H3DDeformationStrategyNode );
  resetSceneTimeField();
  writeNode( out_dir, new H3DDeviceSoftBodyModifierNode );
  resetSceneTimeField();
  writeNode( out_dir, new H3DGeometryMapping );
  resetSceneTimeField();
  writeNode( out_dir, new H3DJointNode );
  resetSceneTimeField();
  writeNode( out_dir, new H3DPhysicsDampingNode );
  resetSceneTimeField();
  writeNode( out_dir, new H3DPhysicsElasticityNode );
  resetSceneTimeField();
  writeNode( out_dir, new H3DPhysicsFrictionNode );
  resetSceneTimeField();
  writeNode( out_dir, new H3DPhysicsMassNode );
  resetSceneTimeField();
  writeNode( out_dir, new H3DPhysicsMaterialNode );
  resetSceneTimeField();
  writeNode( out_dir, new H3DPhysicsMaterialPropertyNode );
  resetSceneTimeField();
  writeNode( out_dir, new H3DPhysicsPoissonRatioNode );
  resetSceneTimeField();
  writeNode( out_dir, new H3DPhysicsStiffnessNode );
  resetSceneTimeField();
  writeNode( out_dir, new H3DSoftBodyAttachment );
  resetSceneTimeField();
  writeNode( out_dir, new H3DSoftBodyLoader );
  resetSceneTimeField();
  writeNode( out_dir, new H3DSoftBodyNode );
  resetSceneTimeField();
  writeNode( out_dir, new H3DSoftBodyOutputNode );
  resetSceneTimeField();
  writeNode( out_dir, new H3DSolverNode );
  resetSceneTimeField();
  writeNode( out_dir, new H3DVertexBodyConstraintNode );
  resetSceneTimeField();
  writeNode( out_dir, new X3DNBodyCollidableNode );
  resetSceneTimeField();
#endif

  try {
    for( H3DNodeDatabase::NodeDatabaseConstIterator i = 
           H3DNodeDatabase::begin();
         i != H3DNodeDatabase::end();
         i++ ) {
      string node_name = (*i).second->getName();
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

