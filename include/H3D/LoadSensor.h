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
/// \file LoadSensor.h
/// \brief Header file for LoadSensor, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __LOADSENSOR_H__
#define __LOADSENSOR_H__

#include <H3D/X3DNetworkSensorNode.h>
#include <H3D/X3DUrlObject.h>
#include <H3D/MFNode.h>
#include <H3D/SFTime.h>
#include <H3D/SFFloat.h>

namespace H3D {

  /// \ingroup X3DNodes
  /// \class LoadSensor
  ///
  /// \brief The LoadSensor monitors the progress and success of downloading
  /// URL elements over a network. Only nodes that contain a valid URL field
  /// (i.e., descendants of X3DUrlObject), may be specified in the watchList
  /// field. Multiple nodes may be watched with a single LoadSensor.
  ///
  /// The timeOut field specifies the maximum time for which the LoadSensor
  /// will monitor loading, starting from when the sensor becomes active.
  /// A value of 0 for the timeOut field indicates an indefinite time out
  /// period; i.e., the LoadSensor will wait until loading has completed either
  /// with success or failure.
  ///
  /// The watchList field contains one or more URL objects to monitor. Only
  /// nodes that contain a valid URL field (i.e., descendants of X3DUrlObject),
  /// may be specified as elements of watchList. If multiple values are
  /// specified for this field, output events are generated only when all of
  /// the children have loaded or at least one has failed. If individual load
  /// status information is desired for different nodes, multiple LoadSensor
  /// nodes may be used, each with a single watchList element.
  ///
  /// The isActive field generates events when loading of the LoadSensor's
  /// watchList elements begins and ends. An isActive TRUE event is generated
  /// when the first element begins loading. An isActive FALSE event is
  /// generated when loading has completed, either with a successful load of
  /// all objects or a failed load of one of the objects, or when the timeout
  /// period is reached as specified in the timeout field.
  ///
  /// The isLoaded field generates events when loading of the LoadSensor's
  /// watchList has completed. An isLoaded TRUE event is generated when all of
  /// the elements have been loaded. An isLoaded FALSE event is generated when
  /// one or more of the elements has failed to load, or when the timeout
  /// period is reached as specified in the timeout field. If all elements in
  /// the watchlist are already loaded by the time the LoadSensor is processed,
  /// the LoadSensor shall generate an isLoaded event with value TRUE and a
  /// progress event with value 1 at the next event cascade.
  ///
  /// The loadTime event is generated when loading of the LoadSensor's
  /// watchList has successfully completed. If loading fails or the timeout
  /// period is reached, a loadTime event is not generated.
  ///
  /// The progress field generates events as loading progresses. The value of
  /// progress is a floating-point number between 0 and 1 inclusive. A value of
  /// 1 indicates complete loading of all watchList elements. The exact meaning
  /// of all other values (i.e., whether these indicate a percentage of total
  /// bytes, a percentage of total number of files, or some other measurement)
  /// and the frequency with which progress events are generated are
  /// browser-dependent. Regardless, the browser shall in all cases guarantee
  /// that a progress value of 1 is generated upon successful load of all URL
  /// objects.
  ///
  /// The following example defines a LoadSensor that monitors the progress of
  /// loading two different ImageTexture nodes:
  ///
  /// Shape {
  ///   appearance Appearance {
  ///     material Material {
  ///       texture DEF TEX1 ImageTexture { url "Amypic.png" }
  ///     }
  ///   }
  ///   geometry Sphere {}
  /// }
  /// Shape {
  ///   appearance Appearance {
  ///     material Material {
  ///       texture DEF TEX2 ImageTexture { url "Bmypic.png" }
  ///     }
  ///   }
  ///   geometry Sphere {}
  /// }
  /// DEF LS LoadSensor {
  ///   watchList [ USE TEX1, USE TEX2 ]
  /// }
  /// ROUTE LS.loadTime TO MYSCRIPT.loadTime
  ///
  /// <ul>
  ///  <li>Success of all children:
  ///   <ul>
  ///     <li>isLoaded = true</li>
  ///     <li>loadTime = now</li>
  ///     <li>progress = 1</li>
  ///     <li>isActive = false</li>
  ///   </ul></li>
  ///  <li>Timeout of any children, failure of any children, or no network present:
  ///   <ul>
  ///     <li>isLoaded = false</li>
  ///     <li>isActive = false</li>
  ///   </ul>
  ///  </li>
  /// </ul>
  ///
  /// For watchList elements that allow dynamic reloading of their contents,
  /// any reload of that element (EXAMPLE  changing the url field of an
  /// ImageTexture or setting the load field of an Inline), resets the
  /// LoadSensor so that it monitors those elements based on the new values and
  /// resets its timeout period if one was specified.
  ///
  /// For streamed media types, the first frame of data available means
  /// successful load of the URL object (i.e., the browser can render one frame
  /// of a movie or start playing an audio file).
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/LoadSensor.x3d">LoadSensor.x3d</a>
  ///     ( <a href="examples/LoadSensor.x3d.html">Source</a> )
  class H3DAPI_API LoadSensor : public X3DNetworkSensorNode {
  public:
    
    typedef TypedMFNodeObject< X3DUrlObject > MFUrlObject;  

    /// Constructor.
    LoadSensor( Inst< SFBool > _enabled  = 0,
                Inst< SFNode > _metadata = 0,
                Inst< SFBool > _isActive = 0,
                Inst< SFTime > _timeOut  = 0,
                Inst< MFUrlObject > _watchList = 0,
                Inst< SFBool > isLoaded = 0,
                Inst< SFTime > loadTime = 0,
                Inst< SFFloat > progress = 0 );

    virtual void traverseSG( TraverseInfo &ti );

    /// The watchList field contains one or more URL objects to monitor. Only
    /// nodes that contain a valid URL field (i.e., descendants of
    /// X3DUrlObject), may be specified as elements of watchList. If multiple
    /// values are specified for this field, output events are generated only
    /// when all of the children have loaded or at least one has failed.
    ///
    /// <b>Access type:</b> inputOutput
    /// 
    /// \dotfile LoadSensor_watchList.dot
    auto_ptr< MFUrlObject > watchList;

    /// The timeOut field specifies the maximum time for which the LoadSensor
    /// will monitor loading, starting from when the sensor becomes active.
    /// A value of 0 for the timeOut field indicates an indefinite time out
    /// period; i.e., the LoadSensor will wait until loading has completed
    /// either with success or failure.
    ///
    /// <b>Access type:</b> inputOutput
    /// <b>Default value:</b> 0 \n
    /// 
    /// \dotfile LoadSensor_timeOut.dot
    auto_ptr< SFTime  > timeOut;

    /// The isLoaded field generates events when loading of the LoadSensor's
    /// watchList has completed. An isLoaded TRUE event is generated when all
    /// of the elements have been loaded. An isLoaded FALSE event is generated
    /// when one or more of the elements has failed to load, or when the
    /// timeout period is reached as specified in the timeout field. If all
    /// elements in the watchlist are already loaded by the time the LoadSensor
    /// is processed, the LoadSensor shall generate an isLoaded event with
    /// value TRUE and a progress event with value 1 at the next event cascade.
    ///
    /// <b>Access type:</b> outputOnly
    /// 
    /// \dotfile LoadSensor_isLoaded.dot
    auto_ptr< SFBool  > isLoaded;

    /// The loadTime event is generated when loading of the LoadSensor's
    /// watchList has successfully completed. If loading fails or the timeout
    /// period is reached, a loadTime event is not generated.
    ///
    /// <b>Access type:</b> outputOnly
    /// 
    /// \dotfile LoadSensor_loadTime.dot
    auto_ptr< SFTime  > loadTime;

    /// The progress field generates events as loading progresses. The value of
    /// progress is a floating-point number between 0 and 1 inclusive. A value
    /// of 1 indicates complete loading of all watchList elements.
    ///
    /// <b>Access type:</b> outputOnly
    /// 
    /// \dotfile LoadSensor_progress.dot
    auto_ptr< SFFloat > progress;

    static H3DNodeDatabase database;
  protected:
    bool loading_last_loop;
    bool finished_loading;
    TimeStamp activation_time;

  };
}

#endif
