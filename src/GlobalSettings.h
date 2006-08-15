//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004, SenseGraphics AB
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
/// \file GlobalSettings.h
/// \brief Header file for GlobalSettings
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __GLOBALSETTINGS_H__
#define __GLOBALSETTINGS_H__

#include "X3DBindableNode.h"
#include "SFString.h"
#include "SFBool.h"
#include "SFFloat.h"
#include "H3DOptionNode.h"
#include "MFNode.h"

namespace H3D {
  /// \ingroup H3DNodes
  /// \class GlobalSettings
  /// GlobalSettings is a X3DBindableNode that specifies default settings
  /// for use in H3D API, such as caching options, haptic rendering options etc
  class H3DAPI_API GlobalSettings : public X3DBindableNode {
  public:

    typedef TypedMFNode< H3DOptionNode > MFOptionNode;

    /// Construtor.
    GlobalSettings( Inst< SFSetBind >  _set_bind        = 0,
                    Inst< SFNode    >  _metadata        = 0,
                    Inst< SFTime    >  _bindTime        = 0,
                    Inst< SFBool    >  _isBound         = 0,
                    Inst< MFOptionNode >  _options         = 0 );
    
    /// Destructor.
    ~GlobalSettings() {
      removeFromStack();
    }

    /// Convenience function to get the top of the DeviceInfo stack.
    static inline GlobalSettings *getActive() {
      return static_cast< GlobalSettings * >
        ( X3DBindableNode::getActive( "GlobalSettings" ) );
    }

    /// Get the first option node of the type of the pointer given as argument.
    /// The option argument will contain the node afterwards, or NULL if no
    /// option of that type exists.
    template< class OptionNodeType >
    void getOptionNode( OptionNodeType * &option ) {
       for( MFOptionNode::const_iterator i = options->begin();
           i != options->end(); i++ ) {
        OptionNodeType *options = dynamic_cast< OptionNodeType * >( *i );
        if( options ) {
          option = options;
          return;
        }
      }
      option = NULL;
    }

    /// Contains the H3DOptionNode to be use as default settings. 
    /// If several nodes of the same type exist the first one will be used.
    ///
    /// <b>Access type: </b> inputOutput \n
    auto_ptr< MFOptionNode > options;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
