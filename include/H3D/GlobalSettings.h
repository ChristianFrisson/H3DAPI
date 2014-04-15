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
/// \file GlobalSettings.h
/// \brief Header file for GlobalSettings
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __GLOBALSETTINGS_H__
#define __GLOBALSETTINGS_H__

#include <H3D/X3DBindableNode.h>
#include <H3D/SFString.h>
#include <H3D/SFBool.h>
#include <H3D/SFFloat.h>
#include <H3D/H3DOptionNode.h>
#include <H3D/GraphicsOptions.h>
#include <H3D/MFNode.h>
#include <H3D/PeriodicUpdate.h>

namespace H3D {
  /// \ingroup H3DNodes
  /// \class GlobalSettings
  /// GlobalSettings is a X3DBindableNode that specifies default settings
  /// for use in H3D API, such as caching options, haptic rendering options etc
  ///
  /// According to the X3D specification the ROUTE statement in an X3D
  /// file is not supposed to send an event. H3D has always done this 
  /// by default up until 2.2 though. The x3dROUTESendsEvent lets you control if an event
  /// should be sent or not. 
  ///
  /// The loadTexturesInThread controls if the downloading/reading of texture image data from file 
  /// should be done in a separate thread or in the main thread. The 
  /// result for doing it in separate thread is that the main program 
  /// can start up faster and the textures will be applied later when they
  /// are loaded versus having to wait for all textures to be read before
  /// program starts.
  ///
  /// Controls the render mode of all geometries in the scene. The possible 
  /// values are:
  /// <table>
  /// <tr><td>"DEFAULT"</td><td>Render as X3D scene nodes say.</td></tr> 
  /// <tr><td>"FILLED"</td><td>Draw filled polygons always.</td></tr> 
  /// <tr><td>"WIREFRAME"</td><td>Draw wireframe polygons always.</td></tr> 
  /// <tr><td>"POINTS"</td><td>Draw points at vertices for polygons always.</td></tr> 
  /// </table>
  /// 
  /// For a way to set fill mode per object see the X3D node FillProperties.
  ///
  /// <b>Access type: </b> inputOutput \n
  /// <b>Default value: </b> "DEFAULT"
  ///
  /// The options field contains H3DOptionNode instances for various options.
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/GlobalSettings.x3d">GlobalSettings.x3d</a>
  ///     ( <a href="examples/GlobalSettings.x3d.html">Source</a> )
  //class GraphicsOptions;
  class H3DAPI_API GlobalSettings : public X3DBindableNode {
  public:
    typedef PeriodicUpdate< Field > UpdateOptions;
    class H3DAPI_API  MFOptionNode : public TypedMFNode< H3DOptionNode >
    {
    protected:
      virtual void onAdd( Node *n );
      virtual void onRemove( Node *n );
    };
    
    //typedef TypedMFNode< H3DOptionNode > MFOptionNode;

    /// Construtor.
    GlobalSettings( Inst< SFSetBind    > _set_bind             = 0,
                    Inst< SFNode       > _metadata             = 0,
                    Inst< SFTime       > _bindTime             = 0,
                    Inst< SFBool       > _isBound              = 0,
                    Inst< MFOptionNode > _options           = 0,
                    Inst< SFBool       > _x3dROUTESendsEvent   = 0,
                    Inst< SFBool       > _loadTexturesInThread = 0,
                    Inst< SFString     > _renderMode           = 0,
                    Inst< SFBool       > _multiThreadedPython  = 0);
    
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
           i != options->end(); ++i ) {
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

    /// According to the X3D specification the ROUTE statement in an X3D
    /// file is not supposed to send an event. H3D has always done this 
    /// by default up until 2.2 though. This field lets you control if an event
    /// should be sent or not. The default value is the value of 
    /// GlobalSettings::default_x3d_route_sends_event.
    ///
    /// <b>Access type: </b> inputOutput \n
    /// <b>Default value: </b> GlobalSettings::default_x3d_route_sends_event 
    /// (in H3DLoad and H3DViewer this is false) \n
    auto_ptr< SFBool > x3dROUTESendsEvent;

    /// Controls if the downloading/reading of texture image data from file 
    /// should be done in a separate thread or in the main thread. The 
    /// result for doing it in separate thread is that the main program 
    /// can start up faster and the textures will be applied later when they
    /// are loaded versus having to wait for all textures to be read before
    /// program starts.
    ///
    /// <b>Access type: </b> inputOutput \n
    /// <b>Default value: </b> X3DTextureNode::load_images_in_separate_thread 
    /// (in H3DLoad and H3DViewer this is true) \n
    auto_ptr< SFBool > loadTexturesInThread;

    /// Controls the render mode of all geometries in the scene. The possible 
    /// values are:
    /// <table>
    /// <tr><td>"DEFAULT"</td><td>Render as X3D scene nodes say.</td></tr> 
    /// <tr><td>"FILLED"</td><td>Draw filled polygons always.</td></tr> 
    /// <tr><td>"WIREFRAME"</td><td>Draw wireframe polygons always.</td></tr> 
    /// <tr><td>"POINTS"</td><td>Draw points at vertices for polygons always.</td></tr> 
    /// </table>
    /// 
    /// For a way to set fill mode per object see the X3D node FillProperties.
    ///
    /// <b>Access type: </b> inputOutput \n
    /// <b>Default value: </b> "DEFAULT"
    auto_ptr< SFString > renderMode;

    /// If true then the embedded Python interpreter is initialized to support
    /// multi-threaded use of the Python C API by calling PyEval_InitThreads().
    ///
    /// This is not required to use threads created in a Python script, but is
    /// required if threads other than the main thread, not created by Python,
    /// will call the Python C API.
    ///
    /// There is a small overhead if enabled, and this option should be disabled
    /// for most applications.
    ///
    /// This field has no effect if modified after a PythonScript node instance
    /// has been created.
    ///
    /// <b>Access type: </b> inputOutput \n
    /// <b>Default value: </b> false
    auto_ptr< SFBool > multiThreadedPython;
    
    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;

    /// The default value for the x3dROUTESendsEvent field.
    static bool default_x3d_route_sends_event;

    /// check whether option nodes has updated since last scene graph loop
    bool optionNodesUpdated(){ return !updateOptions->isUpToDate(); }

  protected:
    
    auto_ptr< UpdateOptions > updateOptions;
    
  };
}

#endif
