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
/// \file Script.h
/// \brief Base class for all scripting node interfaces
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __SCRIPT_H__
#define __SCRIPT_H__

#include <H3D/X3DScriptNode.h>
#include <H3D/H3DDynamicFieldsObject.h>
#include <H3D/SFBool.h>
#include <H3D/SpiderMonkeySAI.h>

namespace H3D {

  /// \ingroup X3DNodes
  /// \brief No scripting is supported with the Script node 
  /// yet.
  class Script : public X3DScriptNode,
                             public H3DDynamicFieldsObject {
  public:
    // Constructor
    Script( Inst< SFNode   > _metadata = 0,
            Inst< MFString > _url = 0,
            Inst< SFBool   > _directOutput = 0,
            Inst< SFBool   > _mustEvaluate = 0 );
    
    /// Destructor.
    ~Script();

    virtual void initialize();    
    virtual void traverseSG( TraverseInfo &ti );

    /// Override the addField method from H3DDynamicFieldsObject
    /// to add the field to the script engine.
    virtual bool addField( const string &name,
                           const Field::AccessType &access,
                           Field *field );

    /// Field is not supported by H3DAPI yet.
    auto_ptr< SFBool > directOutput;

    /// Field is not supported by H3DAPI yet.
    auto_ptr< SFBool > mustEvaluate;

    /// The X3DNodeDatabase for this node.
    static H3DNodeDatabase database;

  protected:

    static Scene::CallbackCode initEngineCallback( void *data );
#ifdef HAVE_SPIDERMONKEY
    SpiderMonkeySAI sai;
#endif
  };
}

#endif
