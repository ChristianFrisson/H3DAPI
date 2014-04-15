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
/// \file X3DColorNode.cpp
/// \brief CPP file for X3DColorNode, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/X3DColorNode.h>
#ifdef MACOSX
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

using namespace H3D;

X3DColorNode::X3DColorNode( 
                           Inst< SFNode>  _metadata ) :
  X3DGeometricPropertyNode( _metadata ),
  GLVertexAttributeObject(VERTEXATTRIBUTE::COLOR){
  type_name = "X3DColorNode";
}

void X3DColorNode::preRender() {
  glEnable( GL_COLOR_MATERIAL );
}

void X3DColorNode::postRender() {
  glDisable( GL_COLOR_MATERIAL );
}

