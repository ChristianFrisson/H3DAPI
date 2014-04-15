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
/// \file FitToBoxTransform.h
/// \brief Header file for FitToBoxTransform
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __FITTOBOXTRANSFORM_H__
#define __FITTOBOXTRANSFORM_H__

#include <H3D/MatrixTransform.h>

namespace H3D {

  /// \ingroup H3DNodes
  /// \class FitToBoxTransform
  /// The FitToBoxTransform node is a MatrixTransform that is used when 
  /// one wants to transform a part of a scene into a specific region
  /// in the coordinate space. The region to transform into is defined
  /// by a box using the boxCenter and boxSize fields. The bounding box
  /// of the FitToBoxTransform node is then used to determine the scale and
  /// translation needed to fit that bounding box into the box defined.
  /// The uniformScalingOnly determines if the scaling should be uniform
  /// or if nonuniform scaling can be used. Nonuniform scaling will fit the
  /// children more accurately into the box, while uniform scaling will
  /// preserve the ratio of the dimentions of the objects.
  /// The active field can be used to control if the transformation
  /// matrix should be updated or not. If active is true the matrix will
  /// be updated.
  ///
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/FitToBoxTransform.x3d">FitToBoxTransform.x3d</a>
  ///     ( <a href="examples/FitToBoxTransform.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile FitToBoxTransform.dot

  class H3DAPI_API FitToBoxTransform : public MatrixTransform {
  public:
    /// We specialize the SFMatrix4f field to that update 
    /// itself from a SFBound field and two SFVec3f fields. If the 
    /// SFBound contains a BoxBound the result will
    /// be a transform so that the bounding box will fit into the box
    /// defined by the SFVec3f fields. Otherwise no transformations are
    /// done.
    ///
    /// routes_in[0] is the boxSize field of the FitToBoxTransform containing
    /// the field.
    /// routes_in[1] is the BoxCenter field of the FitToBoxTransform.
    /// routes_in[2] is the bound field of the FitToBoxTransform.
    /// routes_in[3] is the uniformScalingOnly field of the FitToBoxTransform.
    /// routes_in[4] is the active field of the FitToBoxTransform.
    ///
    class SFMatrix4f: public TypedField< MatrixTransform::SFMatrix4f,
    Types< SFVec3f, SFVec3f,
    SFBound, SFBool, SFBool > > {
      /// Update from the size and bound fields.
      virtual void update();
    };


    /// Constructor.
    FitToBoxTransform( 
                      Inst< AddChildren        > _addChildren        = 0,
                      Inst< RemoveChildren     > _removeChildren     = 0,
                      Inst< MFChild            > _children           = 0,
                      Inst< SFNode             > _metadata           = 0,
                      Inst< SFBound            > _bound              = 0,
                      Inst< SFVec3f            > _bboxCenter         = 0,
                      Inst< SFVec3f            > _bboxSize           = 0,
                      Inst< SFTransformedBound > _transformedBound   = 0,
                      Inst< SFMatrix4f         > _matrix             = 0,
                      Inst< SFVec3f            > _boxSize            = 0,
                      Inst< SFVec3f            > _boxCenter          = 0,
                      Inst< SFBool             > _uniformScalingOnly = 0,
                      Inst< SFBool             > _active             = 0 );

    /// The size of the box we want to transform into.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> Vec3f( 0, 0, 0 ) \n
    /// <b>Valid range:</b> >0
    /// 
    /// \dotfile FitToBoxTransform_boxSize.dot
    auto_ptr< SFVec3f >  boxSize;
    
    /// The center of the box we want to transform into.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> Vec3f( 0.1, 0.1, 0.1 ) \n
    /// 
    /// \dotfile FitToBoxTransform_boxCenter.dot
    auto_ptr< SFVec3f >  boxCenter;

    /// Determines if uniform scaling only should be used,
    /// or if nonuniform scaling is allowed.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> FALSE \n
    /// 
    /// \dotfile FitToBoxTransform_uniformScalingOnly.dot
    auto_ptr< SFBool  >  uniformScalingOnly;
    
    /// Determines if the transformation matrix should be updated 
    /// or not.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> TRUE \n
    /// 
    /// \dotfile FitToBoxTransform_active.dot
    auto_ptr< SFBool  >  active;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
