//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004-2014, SenseGraphics AB
//
//    This file is part of MedX3D.
//
//    MedX3D is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    MedX3D is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with MedX3D; if not, write to the Free Software
//    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//    A commercial license is also available. Please contact us at 
//    www.sensegraphics.com for more information.
//
//
/// \file DicomImageLoader.cpp
/// \brief CPP file for DicomImageLoader, a image loader using the DicomImage
/// library to read images.
///
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/DicomImageLoader.h>
#include <H3DUtil/DicomImage.h>

#ifdef HAVE_DCMTK
#include <dcmtk/dcmdata/dcmetinf.h>
#include <dcmtk/dcmdata/dcdatset.h>
#include <dcmtk/dcmdata/dcdeftag.h>
#include <dcmtk/ofstd/ofconsol.h>

#ifndef WIN32 
#include <dirent.h>
#endif

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase DicomImageLoader::database( "DicomImageLoader", 
                                           &(newInstance<DicomImageLoader>), 
                                           typeid( DicomImageLoader ) );

namespace DicomImageLoaderInternals {
  FIELDDB_ELEMENT( DicomImageLoader, loadSingleFile, INITIALIZE_ONLY );
}

H3DImageLoaderNode::FileReaderRegistration 
DicomImageLoader::reader_registration(
                            "DicomImageLoader",
                            &(newImageLoaderNode< DicomImageLoader >),
                            &DicomImageLoader::supportsFileType 
                            );


DicomImageLoader::DicomImageLoader():
  loadSingleFile( new SFBool ) {

  type_name = "DicomImageLoader";
  database.initFields( this );
  
  loadSingleFile->setValue( false );
}

bool DicomImageLoader::supportsFileType( const string &url ) {
  // set the dcmtk console to write to the H3D console
  ofConsole.setCerr( &H3DUtil::Console );
  ofConsole.setCout( &H3DUtil::Console );

  // temporarily shut down console to avoid warning messages from
  // dcmtk while checking if supported.
  H3DUtil::Console.disable();

  DcmFileFormat fileformat;
  OFCondition status = fileformat.loadFile( url.c_str(), EXS_Unknown, 
                                            EGL_noChange, 0 );

  // restore console output level.
  H3DUtil::Console.enable();
  
  return status.good();
}

H3D::Image *DicomImageLoader::loadImage( const string &url ) {
  return H3DUtil::loadDicomFile( url, loadSingleFile->getValue() );
}

#endif //HAVE_DCMTK
