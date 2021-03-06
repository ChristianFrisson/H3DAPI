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
/// \file H3DVideoClipDecoderNode.cpp
/// \brief CPP file for H3DVideoClipDecoderNode
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/H3DVideoClipDecoderNode.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DVideoClipDecoderNode::local_auto_ptr<list< H3DVideoClipDecoderNode::
DecoderRegistration> >H3DVideoClipDecoderNode::registered_decoders; 

//list< H3DVideoClipDecoderNode::DecoderRegistration > 
//*H3DVideoClipDecoderNode::registered_decoders; 

bool H3DVideoClipDecoderNode::initialized = false; 

H3DVideoClipDecoderNode *H3DVideoClipDecoderNode::getSupportedDecoder( 
   const string &url ) {
  if( initialized ) {
    for( list< DecoderRegistration >::iterator i = 
           registered_decoders->begin();
         i != registered_decoders->end(); ++i ) {
      if( (*i).supports_func( url ) )
        return (*(*i).create_func)();
    }
  }
  return NULL;
}
