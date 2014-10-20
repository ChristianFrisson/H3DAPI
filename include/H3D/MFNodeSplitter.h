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
/// \file MFNodeSplitter.h
/// \brief Base class for all scripting node interfaces
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __MFNODESPLITTER_H__
#define __MFNODESPLITTER_H__

#include <H3D/X3DChildNode.h>
#include <H3D/H3DDynamicFieldsObject.h>
#include <H3D/MFNode.h>
#include <H3D/SFBool.h>
#include <H3D/MFInt32.h>


namespace H3D {

  /// \ingroup X3DNodes
  /// \brief MFNodeSplitter node is used to split MFNode into certain SFNodes
  /// yet.
  /// 
  /// This is normally used to make the routing from MFNode to SFNode easier.
  /// 
  /// Indexes is a initialized only filed which takes multiple integers and be used
  /// to decide which indexes of the MFNode will be extracted. If the indexes specified
  /// value is bigger or equal than the size of MFNode, the no actual content will be
  /// extracted and output.
  /// If specified indexes is less than the size of MFNode , then the corresponding node
  /// will be taken and be set into a SFNode with the name "sfnode_"+index.  
  ///<b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/MFNodeSplitter.x3d">MFNodeSplitter.x3d</a>
  ///     ( <a href="examples/MFNodeSplitter.x3d.html">Source</a> )
  /// \par Internal routes:
  /// \dotfile MFNodeSplitter.dot
  class MFNodeSplitter : public X3DChildNode, public H3DDynamicFieldsObject {
  public:
    class UpdateSelection : public Field  
    {
    public:
      virtual void update();
    protected:
    private:
    };
    
    // Constructor
    MFNodeSplitter( Inst< SFNode   > _metadata = 0,
            Inst< MFNode  > _mfnode = 0,
            Inst< MFInt32 > _indexes = 0);
    
    virtual void initialize();
    virtual void traverseSG( TraverseInfo &ti );

                      
    /// The source MFNode to be used as source of the selection
    auto_ptr<MFNode> mfnode;

    /// The indexes that the node select from the source MFNode
    auto_ptr<MFInt32> indexes;

    /// The X3DNodeDatabase for this node.
    static H3DNodeDatabase database;

  protected:
    auto_ptr<UpdateSelection> updateSelection;
  };
}

#endif
