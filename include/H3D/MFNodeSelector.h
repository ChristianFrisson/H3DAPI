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
/// \file MFNodeSelector.h
/// \brief Base class for all scripting node interfaces
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __MFNODESELECTOR_H__
#define __MFNODESELECTOR_H__

#include <H3D/X3DChildNode.h>
#include <H3D/MFNode.h>
#include <H3D/SFNode.h>
#include <H3D/SFBool.h>
#include <H3D/SFInt32.h>

namespace H3D {

  /// \ingroup X3DNodes
  /// \brief MFNodeSelector node is used to split MFNode into certain SFNodes
  /// yet.
  class MFNodeSelector : public X3DChildNode {
  public:
    class UpdateSelection : public Field  
    {
    public:
      virtual void update();
    protected:
    private:
    };
    
    // Constructor
    MFNodeSelector( Inst< SFNode   > _metadata = 0,
            Inst< MFNode  > _mfnode = 0,
            Inst< SFNode  > _sfnode = 0,
            Inst< SFInt32 > _index = 0);
    
 
    virtual void traverseSG( TraverseInfo &ti );

                      
    /// The source MFNode to be used as source of the selection
    auto_ptr<MFNode> mfnode;

    /// The result SFNode selected from the index position of MFNode
    auto_ptr<SFNode> sfnode;

    auto_ptr<SFInt32> index;

    /// The X3DNodeDatabase for this node.
    static H3DNodeDatabase database;

  protected:
    auto_ptr<UpdateSelection> updateSelection;
  };
}

#endif
