//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004-2007, SenseGraphics AB
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
/// \file SAIFunctions.cpp
/// \brief Source file for common SAI interface classes and functions
/// for use by script engine implementations such as SpiderMonkeySAI.
///
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/SAIFunctions.h>

//#include <H3D/Console.h>
#include <H3D/Scene.h>

using namespace H3D;
using namespace SAI;

string Browser::getName() {
  // TODO
  return "H3DViewer";
}

string Browser::getVersion() {
  // TODO
  return "2.1";
}

/// The getCurrentSpeed service returns the navigation speed 
/// of the current world. The current speed is the average 
/// navigation speed for the currently bound NavigationInfo 
/// node of the active layer in metres per second in the coordinate
/// system of the currently bound viewpoint.
H3DFloat Browser::getCurrentSpeed() { 
  // TODO
  return 0; 
}
    
/// The getCurrentFrameRate service returns the current frame 
/// display rate of the browser. If this is not supported, the
/// value returned is zero.
H3DFloat Browser::getCurrentFrameRate() { 
  // TODO
  return 0; 
}

const vector< ProfileInfo > &Browser::getSupportedProfiles() {
  // TODO
  return vector< ProfileInfo >();
}

const ProfileInfo &Browser::getProfile( const string &profile ) {
  // TODO
  return ProfileInfo();
}

const vector< ComponentInfo > &Browser::getSupportedComponents() {
  // TODO
  return vector< ComponentInfo >();
}

const ComponentInfo &Browser::getComponent( const string &component,
                                            H3DUInt32 level ) {
  // TODO
  return ComponentInfo();
}

ExecutionContext *Browser::getExecutionContext() {
  // TODO
  return NULL;
}

SAIScene *Browser::createScene( ProfileInfo *pi,
                             const vector< ComponentInfo > &cis ) {
  // TODO
  return NULL;
}

void Browser::replaceWorld( SAIScene *scene ) {
  // TODO
}

SAIScene *Browser::importDocument( DOMNode *node ) {
  // TODO
  return NULL;
}


void Browser::loadURL( const string &url,
                       const vector< string > &property_list  ) {
  // TODO
}


void Browser::setDescription( const string &description ) {
  // TODO
}

SAIScene *Browser::createX3DFromString( const string &s ) {
  // TODO
  return NULL;
}

SAIScene *Browser::createX3DFromStream( const istream &s ) {
  // TODO
  return NULL;
}

SAIScene *Browser::createX3DFromURL( const string &url ) {
  // TODO
  return NULL;
}

// TODO
    //void updateControl( 
    //    registerBrowserInterest


const vector< string > &Browser::getRenderingProperties() {
  // TODO
  return vector< string >();
}

const vector< string > &Browser::getBrowserProperties() {
  // TODO
  return vector< string > ();
}

void Browser::nextViewpoint( H3DInt32 layer  ) {
  // TODO

}
 
void Browser::previousViewpoint( H3DInt32 layer  ) {
  // TODO

}

void Browser::firstViewpoint( H3DInt32 layer  ) {
  // TODO

}

void Browser::lastViewpoint( H3DInt32 layer ) {
  // TODO

}

void Browser::print( const string &s ) {
  Console( 4 ) << s << endl;
}

void Browser::dispose() {
  // TODO
}

    /// TODO?:
    /// The setBrowserOption service allows setting options defined in 9.2.5 
    /// Browser options in ISO/IEC 19775-1. The name field shall be one of 
    /// the defined names in Table 9.2 in ISO/IEC 19775-1. This service shall
    /// return an SAIBoolean value indicating whether the change request 
    /// was successful. A browser is not required to support dynamic changes
    /// to any options. If a browser option is not supported, a value 
    /// of FALSE shall be returned.
    /// bool setBrowserOption( const string &


