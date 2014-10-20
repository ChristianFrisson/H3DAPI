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
/// \file MFNodeSplitter.cpp
/// \brief cpp file for MFNodeSplitter
///
//
//////////////////////////////////////////////////////////////////////////////
#include <H3D/MFNodeSplitter.h>


using namespace H3D;

H3DNodeDatabase MFNodeSplitter::database( 
        "MFNodeSplitter", 
        &(newInstance< MFNodeSplitter >),
        typeid( MFNodeSplitter ),
        &X3DChildNode::database 
        );

namespace ScriptInternals {
  FIELDDB_ELEMENT( MFNodeSplitter, mfnode, INPUT_OUTPUT );
  FIELDDB_ELEMENT( MFNodeSplitter, indexes, INITIALIZE_ONLY );
  
}

MFNodeSplitter::MFNodeSplitter( Inst< SFNode   > _metadata,
                Inst< MFNode > _mfnode,
                Inst< MFInt32 > _indexes) : 
X3DChildNode( _metadata ),
H3DDynamicFieldsObject( &database ),
mfnode(_mfnode),
indexes(_indexes),
updateSelection(new UpdateSelection){
  type_name = "MFNodeSplitter";
  database.initFields( this );
  updateSelection->setName("updateSelection");
  updateSelection->setOwner(this);
  mfnode->route(updateSelection);
}

void MFNodeSplitter::traverseSG( TraverseInfo &ti ) {
  updateSelection->upToDate();
  X3DChildNode::traverseSG(ti);
}

void MFNodeSplitter::initialize(){
  X3DChildNode::initialize();
  for( int i = 0 ; i<indexes->size();++i ) {
    SFNode* sf = new SFNode;
    stringstream ss;
    ss<<"sfnode_"<<i;
    addField(ss.str(), Field::INPUT_OUTPUT, sf);
    ss.str("");
    ss.clear();
  }
  
}

void MFNodeSplitter::UpdateSelection::update(){
  MFNodeSplitter* mfs = static_cast<MFNodeSplitter*>(this->getOwner());
  vector<int> indexes = mfs->indexes->getValue();
  // update the sfnode value
  for( int i = 0; i<mfs->indexes->size(); ++i ) {
    stringstream ss;
    ss<<"sfnode_"<<i;
    SFNode* sf = static_cast<SFNode*>( mfs->getField(ss.str()) );
    ss.str("");
    ss.clear();
    if( mfs->mfnode->getValue().size()>indexes[i] ) {
      sf->setValue(mfs->mfnode->getValueByIndex(indexes[i]));
    }else{
      sf->setValue(NULL);
    }
  }
}

