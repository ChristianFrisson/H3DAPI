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
/// \file NoiseTexture3D.h
/// \brief Header file for NoiseTexture3D.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __NOISETEXTURE3D_H__
#define __NOISETEXTURE3D_H__

#include <H3D/X3DTexture3DNode.h>
#include <H3D/SFInt32.h>

namespace H3D {
  /// \ingroup H3DNodes 
  /// \brief The NoiseTexture3D node creates a texture with Perlin noise.
  /// It can be used to filter the values of the style in order to only use
  /// values in the range specified by the fields windowCenter and windowWidth.
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/NoiseTexture3D.x3d">NoiseTexture3D.x3d</a>
  ///     ( <a href="examples/NoiseTexture3D.x3d.html">Source</a> )
  class H3DAPI_API NoiseTexture3D : public X3DTexture3DNode {
  public:
    
    /// SFImage is overridden to update the value from the url 
    /// and imageLoader fields of the ImageTexture. Each url is tried
    /// with all ImageLoader and the first one that is successful is 
    /// the one that is used.
    /// routes_in[0] is the frequency field
    /// routes_in[1] is the lacunarity field
    /// routes_in[2] is the octaveCount field
    /// routes_in[3] is the persistence field
    /// routes_in[4] is the seed field
    /// routes_in[5] is the width (x-dimension) field
    /// routes_in[6] is the height (y-dimension) field.
    /// routes_in[7] is the depth (z-dimension) field.
    /// routes_in[8] is the type field.
    /// routes_in[9] is the tileable field.
    class H3DAPI_API SFImage: public TypedField< X3DTexture3DNode::SFImage,
      Types< SFFloat, SFFloat, SFInt32, SFFloat, SFInt32, SFInt32, SFInt32, SFInt32, SFString, SFBool > > {
      /// Creates an image to be used as color table.
      virtual void update();
    };

    /// Constructor.
    NoiseTexture3D(Inst< DisplayList > _displayList  = 0,
                   Inst< SFNode      > _metadata     = 0,
                   Inst< SFImage     > _image        = 0,
                   Inst< SFTextureProperties > _textureProperties = 0,
                   Inst< SFInt32     > _width = 0,
                   Inst< SFInt32     > _height = 0,
                   Inst< SFInt32     > _depth = 0,
                   Inst< SFFloat     > _frequency = 0,
                   Inst< SFFloat     > _lacunarity  = 0,
                   Inst< SFInt32     > _octaveCount = 0,
                   Inst< SFFloat     > _persistence = 0,
                   Inst< SFInt32     > _seed = 0,
                   Inst< SFString    > _type = 0,
                   Inst< SFBool      > _tileable = 0);

    static H3DFloat simplexNoise( H3DFloat x, H3DFloat y, H3DFloat z, 
                                  H3DFloat frequency, 
                                  H3DFloat lacunarity, 
                                  H3DFloat octaves, 
                                  H3DFloat persistence );
    static H3DFloat simplexNoiseTileable( H3DFloat x, H3DFloat y, H3DFloat z,
                                          H3DFloat frequency, 
                                          H3DFloat lacunarity, 
                                          H3DFloat octaves, 
                                          H3DFloat persistence );

    /// Width (x-dimension) of texture.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b> Default value:</b> 512 \n
    auto_ptr< SFInt32 > width;

    /// Height (y-dimension) of texture.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b> Default value:</b> 512 \n
    auto_ptr< SFInt32 > height;
    
    /// Depth (z-dimension) of texture.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b> Default value:</b> 512 \n
    auto_ptr< SFInt32 > depth;
    
    /// Frequency of the first octave.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b> Default value:</b> 1.0 \n
    auto_ptr< SFFloat > frequency;

    /// Frequency multiplier between successive octaves.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b> Default value:</b> 2.0 \n
    auto_ptr< SFFloat > lacunarity;

    /// Total number of octaves that generate the Perlin noise, higher value increases computation time, maximum value is 30.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b> Default value:</b> 6 \n
    auto_ptr< SFInt32 > octaveCount;

    /// Sets the persistence value of the Perlin noise.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b> Default value:</b> 0.5 \n
    auto_ptr< SFFloat > persistence;

    /// Seed value used by the Perlin-noise function.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b> Default value:</b> 0 \n
    auto_ptr< SFInt32 > seed;

    /// Noise type. 
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b> Default value:</b> "PERLIN" \n
    /// <b>Valid values:</b> "PERLIN", "SIMPLEX"
    auto_ptr< SFString > type;

    /// If set to TRUE the produced texture will be tileable seamlessly. This takes more computation time
    /// and produce some artifacts though.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b> Default value:</b> false \n
    auto_ptr< SFBool > tileable;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;

  };
}

#endif
