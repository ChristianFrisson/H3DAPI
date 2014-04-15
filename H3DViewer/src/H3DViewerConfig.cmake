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
/// \file H3DViewerConfig.h
/// \brief Header file configuration of H3DViewer compilation.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __H3DVIEWERCONFIG_H__
#define __H3DVIEWERCONFIG_H__


/// If defined all plugins in the plugins directory for H3DViewer is 
/// loaded on startup. The plugins directory depends on the system.
/// E.g. for Windows it is path_to_executable/../plugins and on OSX 
/// it is the Contents/Plugins directory of the H3DViewer.app.
#define INCLUDE_PLUGINS_DIR_PLUGINS

/// If defined buttons will be added to the plugins dialog allowing
/// a user to select any file at all and load it as a plugin.
#define ALLOW_ADDING_PLUGINS

/// This will be the H3DViewer application name and is therefore the registry
/// key for where recent history and settings will be stored. The application
/// name depends on whether H3DViewer is built for a stand alone release or as
/// a part of H3DAPI release.
#define H3DVIEWER_APP_NAME wxT("${H3DVIEWER_APP_NAME}")

/// This will be defined if wxWidgets propgrid library is available. propgrid
/// is part of wxWidgets 2.9 and will give nicer tree view and program
/// settings option
#cmakedefine HAVE_WXPROPGRID

/// This will be defined if DirectX 9 SDK is installed. Enables the NVidia 
/// 3DVision stereo mode.
#cmakedefine HAVE_DX9

#endif
