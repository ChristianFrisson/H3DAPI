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
/// \file TransferFunctionTexture.h
/// \brief Header file for TransferFunctionTexture.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __TRANSFERFUNCTIONTEXTURE_H__
#define __TRANSFERFUNCTIONTEXTURE_H__

#include <H3D/X3DTexture2DNode.h>
#include <H3D/H3DFunctionNode.h>

namespace H3D {
  /// \ingroup H3DNodes 
  /// \class TransferFunctionTexture
  /// \brief Texture for defining a transfer function from a single
  /// value to RGBA values using H3DFunctionNode instances.
  ///
  /// A texture of size 256x1 pixels will be generated where each pixel
  /// value is determined by the functions provided for each component.
  ///
  /// The functions provided should both take input and give output in the
  /// range [0,1]. If they do not the result is undefined.
  /// 
  /// If a function is not specified for a component the function will be
  /// F(x) = x for that component.
  ///
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/TransferFunctionTexture.x3d">TransferFunctionTexture.x3d</a>
  ///     ( <a href="examples/TransferFunctionTexture.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile TransferFunctionTexture.dot
  class H3DAPI_API TransferFunctionTexture : public X3DTexture2DNode {
  public:
    
   typedef H3D::DependentSFNode< 
                         FieldRef< H3DFunctionNode, 
                                   H3D::Field,
                                   &H3DFunctionNode::functionChanged > >
   SFFunctionNode;

    /// SFImage is overridden to update the value from the functions.
    class H3DAPI_API SFImage: 
      public H3D::TypedField< H3D::X3DTexture2DNode::SFImage,
                   H3D::Types< SFFunctionNode, SFFunctionNode, 
                   SFFunctionNode, SFFunctionNode > > {
      /// Creates an image to be used as color table.
      virtual void update();
    };

    /// Constructor.
    TransferFunctionTexture( 
                 Inst< DisplayList    > _displayList   = 0,
                 Inst< SFNode         > _metadata      = 0,
                 Inst< SFBool         >  _repeatS      = 0,
                 Inst< SFBool         >  _repeatT      = 0,
                 Inst< SFBool         >  _scaleToP2    = 0,
                 Inst< SFImage        > _image         = 0,
                 Inst< SFTextureProperties > _textureProperties = 0,
                 Inst< SFFunctionNode > _redFunction   = 0,
                 Inst< SFFunctionNode > _greenFunction = 0,
                 Inst< SFFunctionNode > _blueFunction  = 0,
                 Inst< SFFunctionNode > _alphaFunction = 0 );

    /// The function for the red component. 
    ///
    /// <b>Access type:</b> inputOutput
    /// 
    /// \dotfile TransferFunctionTexture_redFunction.dot
    auto_ptr< SFFunctionNode > redFunction; 

    /// The function for the green component. 
    ///
    /// <b>Access type:</b> inputOutput
    /// 
    /// \dotfile TransferFunctionTexture_greenFunction.dot
    auto_ptr< SFFunctionNode > greenFunction;

    /// The function for the blue component. 
    ///
    /// <b>Access type:</b> inputOutput
    /// 
    /// \dotfile TransferFunctionTexture_blueFunction.dot
    auto_ptr< SFFunctionNode > blueFunction; 

    /// The function for the alpha component. 
    ///
    /// <b>Access type:</b> inputOutput
    /// 
    /// \dotfile TransferFunctionTexture_alphaFunction.dot
    auto_ptr< SFFunctionNode > alphaFunction;

    /// The H3DNodeDatabase for this node.
  static H3DNodeDatabase database;

  };
}

#endif
