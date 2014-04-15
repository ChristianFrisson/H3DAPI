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
/// \file H3DExports.h
/// \brief header file for getH3DExportNode function. Used to be able to access
/// HDEV, HDEV0, HDEV1 and HDEV2 from X3D.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __H3DEXPORTS_H__
#define __H3DEXPORTS_H__

#include <H3D/Node.h>

namespace H3D {
  namespace H3DExports {
    Node *getH3DExportNode( const string &name );
  }
}

#endif
