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
/// \file MFNodeSelector.cpp
/// \brief cpp file for MFNodeSelector
///
//
//////////////////////////////////////////////////////////////////////////////
#include <H3D/MFNodeSelector.h>


using namespace H3D;

H3DNodeDatabase MFNodeSelector::database( 
        "MFNodeSelector", 
        &(newInstance< MFNodeSelector >),
        typeid( MFNodeSelector ),
        &X3DChildNode::database 
        );

namespace ScriptInternals {
  FIELDDB_ELEMENT( MFNodeSelector, mfnode, INPUT_OUTPUT );
  FIELDDB_ELEMENT( MFNodeSelector, sfnode, INPUT_OUTPUT );
  FIELDDB_ELEMENT( MFNodeSelector, index, INITIALIZE_ONLY );
  
}

MFNodeSelector::MFNodeSelector( Inst< SFNode   > _metadata,
                Inst< MFNode > _mfnode,
                Inst< SFNode > _sfnode,
                Inst< SFInt32 > _index) : 
X3DChildNode( _metadata ),
mfnode(_mfnode),
sfnode(_sfnode),
index(_index),
updateSelection(new UpdateSelection){
  type_name = "MFNodeSelector";
  database.initFields( this );
  updateSelection->setName("updateSelection");
  updateSelection->setOwner(this);
  mfnode->route(updateSelection);
}

void MFNodeSelector::traverseSG( TraverseInfo &ti ) {
  updateSelection->upToDate();
  X3DChildNode::traverseSG(ti);
}


void MFNodeSelector::UpdateSelection::update(){
  MFNodeSelector* mfs = static_cast<MFNodeSelector*>(this->getOwner());
  unsigned int index = mfs->index->getValue(mfs->id);
  if( mfs->mfnode->getValue().size()>index ) {
    mfs->sfnode->setValue(mfs->mfnode->getValueByIndex(index));
  }else{
    mfs->sfnode->setValue(NULL,mfs->id);
  }
  
 
}

