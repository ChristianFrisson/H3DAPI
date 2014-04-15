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
/// \file FunctionPlotTexture.h
/// \brief Header file for FunctionPlotTexture
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __FUNCTIONPLOTTEXTURE_H__
#define __FUNCTIONPLOTTEXTURE_H__

#include <H3D/X3DTexture2DNode.h>
#include <H3D/SFVec2f.h>
#include <H3D/H3DFunctionNode.h>

namespace H3D {
  /// \ingroup H3DNodes 
  /// \class FunctionPlotTexture
  /// \brief Texture showing a plot of a function.
  /// 
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/FunctionPlotTexture.x3d">FunctionPlotTexture.x3d</a>
  ///     ( <a href="examples/FunctionPlotTexture.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile FunctionPlotTexture.dot
  class H3DAPI_API FunctionPlotTexture : 
    public X3DTexture2DNode {
  public:

    typedef H3D::DependentSFNode< 
      FieldRef< H3DFunctionNode, 
                H3D::Field,
                &H3DFunctionNode::functionChanged > >
   SFFunctionNode;

    /// SFImage is overridden to update the image from the field values
    ///
    /// - routes_in[0] is the function field.
    /// - routes_in[1] is the backgroundColor field.
    /// - routes_in[2] is the plotColor field.
    /// - routes_in[3] is the xRange field.
    /// - routes_in[4] is the yRange field.
    /// - routes_in[5] is the xResolution field.
    /// - routes_in[6] is the yResolution field.
    ///
    class H3DAPI_API SFImage: public TypedField< X3DTexture2DNode::SFImage,
                                                 Types< SFFunctionNode,
                                                        SFColorRGBA,
                                                        SFColorRGBA,
                                                        SFVec2f,
                                                        SFVec2f,
                                                        SFInt32,
                                                        SFInt32 > > {
    protected:
      virtual void update();
      
      inline void setPixel( unsigned int x, unsigned int y, const RGBA &color ) {
        unsigned char p[] = { (unsigned char) ( color.r * 255 ),
                              (unsigned char) ( color.g * 255 ),
                              (unsigned char) ( color.b * 255 ),
                              (unsigned char) ( color.a * 255 ) };
        value->setElement( p, x, y );
      }
    };
      
    /// Constructor.
    FunctionPlotTexture( Inst< DisplayList > _displayList = 0,
                            Inst< SFNode   >  _metadata        = 0,
                            Inst< SFBool   >  _repeatS         = 0,
                            Inst< SFBool   >  _repeatT         = 0,
                            Inst< SFBool   >  _scaleToP2       = 0,
                            Inst< SFImage  > _image            = 0,
                            Inst< SFTextureProperties > _textureProperties = 0,
                            Inst< SFFunctionNode > _function   = 0,
                            Inst< SFColorRGBA > _backgroundColor = 0,
                            Inst< SFColorRGBA > _plotColor     = 0,
                            Inst< SFVec2f     > _xRange        = 0,
                            Inst< SFVec2f     > _yRange        = 0,
                            Inst< SFInt32     > _xResolution   = 0,
                            Inst< SFInt32     > _yResolution   = 0 );

    /// The function to plot.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// 
    /// \dotfile FunctionPlotTexture_function.dot
    auto_ptr< SFFunctionNode > function;

    /// The background color of the plot.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b>RGBA( 1, 1, 1, 1 )
    /// 
    /// \dotfile FunctionPlotTexture_backgroundColor.dot
    auto_ptr< SFColorRGBA > backgroundColor;

    /// The color of the function plot.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b>RGBA( 0, 0, 0, 1 )
    /// 
    /// \dotfile FunctionPlotTexture_plotColor.dot
    auto_ptr< SFColorRGBA > plotColor;

    /// The range in x to show.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b>Vec2f( 0, 1 )
    /// 
    /// \dotfile FunctionPlotTexture_xRange.dot
    auto_ptr< SFVec2f     > xRange;

    /// The range in y to show.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b>Vec2f( 0, 1 )
    /// 
    /// \dotfile FunctionPlotTexture_yRange.dot
    auto_ptr< SFVec2f     > yRange;

    /// The width in pixels of the generated texture.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b>128
    /// 
    /// \dotfile FunctionPlotTexture_xResolution.dot
    auto_ptr< SFInt32     > xResolution;

    /// The width in pixels of the generated texture.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b>128
    /// 
    /// \dotfile FunctionPlotTexture_yResolution.dot
    auto_ptr< SFInt32     > yResolution;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
