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
/// \file HumanHand.cpp
/// \brief CPP file for HumanHand.
///
//
//
//////////////////////////////////////////////////////////////////////////////

// H3D includes
#include <H3D/HumanHand.h>
#include <H3D/Scene.h>

#ifdef HAVE_VIRTUAL_HAND_SDK
// Virtual Hand SDK includes
#include <vhandtk/vhtIOConn.h>
#include <vhandtk/vhtBaseException.h>
#include <vhandtk/vhtTrackerEmulator.h>
#include <vhandtk/vhtTracker.h>
#include <vhandtk/vhtHandMaster.h>
#endif

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase HumanHand::database( "HumanHand", 
                                     &(newInstance<HumanHand>),
                                     typeid( HumanHand ),
                                     &X3DGeometryNode::database );

namespace HumanHandInternal {
  FIELDDB_ELEMENT( HumanHand, cyberGlove, INITIALIZE_ONLY );
  FIELDDB_ELEMENT( HumanHand, gloveHandedness, INITIALIZE_ONLY );
  FIELDDB_ELEMENT( HumanHand, scale, INITIALIZE_ONLY );
}


HumanHand::HumanHand( Inst< SFNode             > _metadata,
                      Inst< SFBound            > _bound,
                      Inst< DisplayList        > _displayList,
                      Inst< SFCyberGloveSensor > _cyberGlove,
                      Inst< SFString           > _gloveHandedness,
                      Inst< SFFloat            > _scale ) :
  X3DGeometryNode( _metadata, _bound, _displayList ),
  cyberGlove( _cyberGlove ),
  gloveHandedness( _gloveHandedness ),
  scale( _scale ),
#ifdef HAVE_VIRTUAL_HAND_SDK
  hand( 0 ),
  drawer( 0 ),
  cameraXForm(0),
#endif
  sceneGraphScaling( new SFVec3f ) {

  type_name = "HumanHand";
  database.initFields( this );

  gloveHandedness->addValidValue( "right" );
  gloveHandedness->addValidValue( "left" );
  cyberGlove->routeNoEvent( bound, id );
  scale->setValue( 0.01f );
  scale->routeNoEvent( bound, id );
  sceneGraphScaling->setName( "sceneGraphScaling" );
  sceneGraphScaling->setOwner( this );
  sceneGraphScaling->setValue( Vec3f( 1, 1, 1 ) );
  sceneGraphScaling->routeNoEvent( bound, id );
  cyberGlove->route( displayList, id );
  
}

void HumanHand::initialize() {
  X3DGeometryNode::initialize();
#ifdef HAVE_VIRTUAL_HAND_SDK
  CyberGloveSensor * cyber_glove = cyberGlove->getValue();
  if( cyber_glove && cyber_glove->getGlove() ) {
    // Try to connect to the tracker, if no tracker is found use a tracker
    // emulator.
    vhtTracker * tracker = 0;
    vhtIOConn * trackerIOConn = vhtIOConn::getDefault( vhtIOConn::tracker );
    if( trackerIOConn ) {
      try {
        tracker = new vhtTracker( trackerIOConn );
      } catch( vhtBaseException *e ) {
        tracker = 0;
        Console(3) << "Warning, could not initialize real tracker in node "
                   << getName()
                   << "Error message from Virtual Hand SDK is: "
                   << e->getMessage() << endl;
      }
    }

    if( !tracker ) {
      Console(3) << "Warning: No real tracker found, in node "
                 << getName() << " a tracker will be emulated." << endl;
      tracker = new vhtTrackerEmulator();
    }

    // Create a vhtHandMaster which contains information about the CyberGlove
    // connection and the tracker.
    vhtHandMaster * master = 0;
    if( tracker ) {
      try {
        vht6DofDevice *rcvr1 = tracker->getLogicalDevice(0);
        if( rcvr1 )
          master = new vhtHandMaster( cyber_glove->getGlove(),
                                      rcvr1 );
      } catch( vhtBaseException *e ) {
        master = 0;
        Console(3) << e->getMessage() << endl;
      }
    }

    // Create a vhtHumanHand, this will decide how the hand will look
    // when rendered.
    if( master ) {
      try {
        GHM::Handedness glove_handedness= GHM::rightHand;
        if( gloveHandedness->getValue() == "left" )
          glove_handedness= GHM::leftHand;
        hand.reset( new vhtHumanHand( master,
                                      (double)( scale->getValue() ),
                                      glove_handedness ) );
      } catch( vhtBaseException *e ) {
        hand.reset(0);
        Console(3) << e->getMessage() << endl;
      }
    }

    // Create a vhtOglDrawer which will be used to render the hand.
    // The vhtTransform3D is used to control where the hand is drawn.
    try {
      drawer.reset( new vhtOglDrawer() );
      cameraXForm = new vhtTransform3D();
    } catch( vhtBaseException *e ) {
      drawer.reset(0);
      Console(3)
                 << e->getMessage() << endl;
    }

  } else {
    Console(3) << "Warning: No CyberGlove specified for node: "
               << getName() << ". Node will not be used." << endl;
  }
#endif
}

void HumanHand::render() {
#ifdef HAVE_VIRTUAL_HAND_SDK
  if( hand.get() && drawer.get() ) {
    // Get the modelview matrix and calculate the scale values use to
    // calculate the bounding box.
    GLdouble modelview[16];
    glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
    Vec3d first_column = Vec3f( (H3DFloat)modelview[0],
                                (H3DFloat)modelview[1],
                                (H3DFloat)modelview[2] );
    Vec3d second_column = Vec3f( (H3DFloat)modelview[4], 
                                 (H3DFloat)modelview[5],
                                 (H3DFloat)modelview[6] );
    Vec3d third_column = Vec3f( (H3DFloat)modelview[8],
                                (H3DFloat)modelview[9],
                                (H3DFloat)modelview[10] );
    Vec3f scene_graph_scaling_old = sceneGraphScaling->getValue();
    Vec3f scene_graph_scaling_new( (H3DFloat)first_column.length(),
                                   (H3DFloat)second_column.length(),
                                   (H3DFloat)third_column.length() );
    if( H3DUtil::H3DAbs( scene_graph_scaling_new.x -
          scene_graph_scaling_old.x ) > Constants::f_epsilon ||
        H3DUtil::H3DAbs( scene_graph_scaling_new.y -
          scene_graph_scaling_old.y ) > Constants::f_epsilon ||
        H3DUtil::H3DAbs( scene_graph_scaling_new.z -
        scene_graph_scaling_old.z ) > Constants::f_epsilon ) {
      sceneGraphScaling->setValue( scene_graph_scaling_new );
    }
    // Remove scaling before creating matrix which will be sent to
    // Virtual Hand SDK because otherwise the transform will invert itself when
    // rendering. I have no idea why.
    first_column.normalizeSafe();
    second_column.normalizeSafe();
    third_column.normalizeSafe();
    // Create a matrix in Virtual Hand SDK form. This will be used to transform
    // the hand.
    double xform[4][4];
    xform[0][0] = first_column[0];
    xform[0][1] = second_column[0];
    xform[0][2] = third_column[0];
    xform[0][3] = modelview[12];
    xform[1][0] = first_column[1];
    xform[1][1] = second_column[1];
    xform[1][2] = third_column[1];
    xform[1][3] = modelview[13];
    xform[2][0] = first_column[2];
    xform[2][1] = second_column[2];
    xform[2][2] = third_column[2];
    xform[2][3] = modelview[14];
    xform[3][0] = modelview[3];
    xform[3][1] = modelview[7];
    xform[3][2] = modelview[11];
    xform[3][3] = modelview[15];
    // Update the visual geometry of the vhtHumanHand. That is, the internal
    // graphical representation of the hand.
    hand->updateVisualGeometry();
    // Update the transform.
    cameraXForm->setTransform( xform );
    // Draw the hand.
    drawer->renderHand( cameraXForm, hand.get() );
  }
#endif
}

void HumanHand::traverseSG( TraverseInfo &ti) {
  X3DGeometryNode::traverseSG( ti );
#ifdef HAVE_VIRTUAL_HAND_SDK
  // Update tracker and cyberGlove.
  CyberGloveSensor * cgs = cyberGlove->getValue();
  if( hand.get() && cgs && cgs->isActive->getValue() )
    hand->update(false);
#endif
}

