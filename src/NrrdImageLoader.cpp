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
/// \file NrrdImageLoader.cpp
/// \brief CPP file for NrrdImageLoader, a image loader using the NrrdImage
/// library to read images.
///
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/NrrdImageLoader.h>

#include <algorithm>

#ifdef HAVE_TEEM
#include <H3DUtil/PixelImage.h>

#define TEEM_STATIC
#include <teem/nrrd.h>
#include <teem/air.h>
using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase NrrdImageLoader::database( "NrrdImageLoader", 
                                           &(newInstance<NrrdImageLoader>), 
                                           typeid( NrrdImageLoader ) );

H3DImageLoaderNode::FileReaderRegistration 
NrrdImageLoader::reader_registration(
                            "NrrdImageLoader",
                            &(newImageLoaderNode< NrrdImageLoader >),
                            &NrrdImageLoader::supportsFileType 
                            );

bool NrrdImageLoader::supportsFileType( const string &url ) {
#if TEEM_VERSION < 11100
  // only allow it to read files with the extension .nrrd since
  // otherwise nrrd can crash or hand when it gets a .gif or .jpg 
  // file in the code below (at least on Linux).
  if( url.size() < 5 ) return false;
  string end_of_url =  url.substr( url.size() - 5, url.size() );
  std::transform(end_of_url.begin(), end_of_url.end(),
     end_of_url.begin(), ::tolower);
  string vtk_end = url.substr( url.size() - 4, end_of_url.size() ); 
  if( end_of_url != ".nrrd" && vtk_end != ".vtk" ) return false;
#endif

  NrrdIoState *nio;
  Nrrd *nin;
  
  /* create a new nrrd */
  nin = nrrdNew();
  
  /* tell nrrdLoad to only read the header, not the data */
  nio = nrrdIoStateNew();
  nrrdIoStateSet(nio, nrrdIoStateSkipData, AIR_TRUE);
  
  /* read in the nrrd header from file */
  if (nrrdLoad(nin, url.c_str(), nio)) {
    nio = nrrdIoStateNix(nio);
    nrrdNuke( nin );
    return false;
  }
  
  /* we're done with the nrrdIoState, this sets it to NULL */
  nio = nrrdIoStateNix(nio);
  nrrdNuke(nin);
  return true;
}

Image *NrrdImageLoader::loadImage( const string &url ) {
  return H3DUtil::loadNrrdFile( url ); 
}


#endif // HAVE_NRRDIMAGE
