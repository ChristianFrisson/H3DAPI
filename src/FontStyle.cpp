//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004-2007, SenseGraphics AB
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
/// \file FontStyle.cpp
/// \brief CPP file for FontStyle, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/FontStyle.h>
#if defined(__APPLE__) && defined(__MACH__)
#include <AGL/agl.h>
#endif

#if defined( HAVE_FREETYPE ) && defined( HAVE_FTGL )
//#include <FTGL/FTGL.h>
#include <FTGL/ftgl.h>
#include <FTGL/FTGLTextureFont.h>
#include <FTGL/FTGLPolygonFont.h>
#include <FTGL/FTGLOutlineFont.h>
#include <FTGL/FTGLExtrdFont.h>
#if defined(__APPLE__) && defined(__MACH__)
#include <freetype/ftmac.h>
#else
#include <freetype/freetype.h>
#endif
#ifdef LINUX
#include <fontconfig/fontconfig.h>
#include <string.h>
#endif
#endif

using namespace H3D;

#if defined( HAVE_FREETYPE ) && defined( HAVE_FTGL )
namespace FontStyleInternals {
	static map< string, FTFont* > font_db;

#if defined(__APPLE__) && defined(__MACH__)
FT_Error  xFT_GetFile_From_Mac_Name( const char* fontName,
                                     FSSpec*     pathSpec,
                                     FT_Long*    face_index )
{
    OptionBits            options = kFMUseGlobalScopeOption;
    
    FMFontFamilyIterator  famIter;
    OSStatus              status = FMCreateFontFamilyIterator( NULL, NULL,
                                                               options,
                                                               &famIter );
    FMFont                the_font = NULL;
    FMFontFamily          family   = NULL;
    
    
    *face_index = 0;
    while ( status == 0 && !the_font )
    {
      status = FMGetNextFontFamily( &famIter, &family );
      if ( status == 0 )
      {
        int                           stat2;
        FMFontFamilyInstanceIterator  instIter;
        Str255                        famNameStr;
        char                          famName[256];
        
        
        /* get the family name */
        FMGetFontFamilyName( family, famNameStr );
        CopyPascalStringToC( famNameStr, famName );
        
        /* iterate through the styles */
        FMCreateFontFamilyInstanceIterator( family, &instIter );
        
        *face_index = 0;
        stat2 = 0;
        while ( stat2 == 0 && !the_font )
        {
          FMFontStyle  style;
          FMFontSize   size;
          FMFont       font;
          
          
          stat2 = FMGetNextFontFamilyInstance( &instIter, &font,
                                               &style, &size );
          if ( stat2 == 0 && size == 0 )
          {
            char  fullName[256];
            
            
            /* build up a complete face name */
            ft_strcpy( fullName, famName );
            if ( style & bold )
              strcat( fullName, " Bold" );
            if ( style & italic )
              strcat( fullName, " Italic" );
            
            /* compare with the name we are looking for */
            if ( ft_strcmp( fullName, fontName ) == 0 )
            {
              /* found it! */
              the_font = font;
            }
            else
              ++(*face_index);
          }
        }
        
        FMDisposeFontFamilyInstanceIterator( &instIter );
      }
    }
    
    FMDisposeFontFamilyIterator( &famIter );
    
    if ( the_font )
    {
      FMGetFontContainer( the_font, pathSpec );
      return FT_Err_Ok;
    }
    else
      return FT_Err_Unknown_File_Format;
}

#endif

#ifdef WIN32

LONG GetNameValue(HKEY key, LPCTSTR subkey, LPCTSTR valuename, LPTSTR retdata)
{
	HKEY hkey;
	LONG retval = RegOpenKeyEx(key, subkey, 0, KEY_QUERY_VALUE, &hkey);
	
	*retdata = 0;

	if (retval == ERROR_SUCCESS) 
	{
		DWORD datasize = MAX_PATH;
		BYTE data[MAX_PATH];
		DWORD type = 0;

		retval = RegQueryValueEx(hkey, valuename, NULL, &type, data, &datasize);
		if (retval == ERROR_SUCCESS) 
		{
			lstrcpy(retdata, (const char *)data);
		}

		RegCloseKey(hkey);
	}


	return retval;
}

///////////////////////////////////////////////////////////////////////////////
// GetNextNameValue
LONG GetNextNameValue(HKEY key, LPCTSTR subkey, LPTSTR szName, LPTSTR szData)
{
	static HKEY hkey = NULL;
	static DWORD dwIndex = 0;
	LONG retval;

	if (subkey == NULL && szName == NULL && szData == NULL)	{
		if (hkey)
			RegCloseKey(hkey);
		hkey = NULL;
		return ERROR_SUCCESS;
	}

	if (subkey && subkey[0] != 0)	{
		retval = RegOpenKeyEx(key, subkey, 0, KEY_READ, &hkey);
		if (retval != ERROR_SUCCESS){
			return retval;
		}
		dwIndex = 0;
	}	else	{
		dwIndex++;
	}
	
	*szName = 0;
	*szData = 0;

	char szValueName[MAX_PATH];
	DWORD dwValueNameSize = sizeof(szValueName)-1;
	BYTE szValueData[MAX_PATH];
	DWORD dwValueDataSize = sizeof(szValueData)-1;
	DWORD dwType = 0;

	retval = RegEnumValue(hkey, dwIndex, szValueName, &dwValueNameSize, NULL, 
		&dwType, szValueData, &dwValueDataSize);
	if (retval == ERROR_SUCCESS) 	{
		lstrcpy(szName, (char *)szValueName);
		lstrcpy(szData, (char *)szValueData);
	}

	return retval;
}

BOOL GetFontFile(LPCTSTR lpszFontName, 
                 string& strDisplayName, 
                 string& strFontFile)
{

	TCHAR szName[2 * MAX_PATH];
	TCHAR szData[2 * MAX_PATH];

	LPCTSTR strFont = "Software\\Microsoft\\Windows NT\\CurrentVersion\\Fonts";

	BOOL bResult = FALSE;

	while (GetNextNameValue(HKEY_LOCAL_MACHINE, strFont, szName, szData) == 
         ERROR_SUCCESS)
	{
		if (_strnicmp(lpszFontName, szName, strlen(lpszFontName)) == 0)
		{
			strDisplayName = szName;
			strFontFile = szData;
			bResult = TRUE;
			break;
		}
    strFont = "";


	}
  // close the registry key
	GetNextNameValue(HKEY_LOCAL_MACHINE, NULL, NULL, NULL);	

	return bResult;
}


#endif

#ifdef LINUX
string FC_GetFontByName( const char *font_name ) {
  string fn="";
  FcObjectSet *os = 0;
  FcFontSet	*fs;
  FcPattern   *pat;

  pat = FcPatternCreate ();
  os = FcObjectSetBuild (FC_FAMILY, FC_FILE, NULL);
  fs = FcFontList (0, pat, os);
  if (pat)
    FcPatternDestroy (pat);
  
  if (fs) {
    int	j;
    
    for (j = 0; j < fs->nfont; j++) {
      FcChar8 *font;
      FcChar8 *file;
      
      font = FcNameUnparse(fs->fonts[j]);
      if ( FcPatternGetString(fs->fonts[j], FC_FILE, 0, &file) 
	   == FcResultMatch) {
	if (strncasecmp(font_name, (char*)font, strlen(font_name)) == 0)
	  fn=string((char*)file);
	//printf ("%s\n ", file);
	//printf ("%s: ", font);

      }
      free (font);
    }
    FcFontSetDestroy (fs);
  }
  return fn;
}



#endif


  FTFont* getFontByName( const string &font_name,
                         bool bold, 
                         bool italic,
                         const string &render_type ) {

	string full_font_path;
#if defined(__APPLE__) && defined(__MACH__)
    FSSpec ps;
    FT_Long face_index;
    FT_Error r = xFT_GetFile_From_Mac_Name( font_name.c_str(), &ps, &face_index );
    if ( r != FT_Err_Ok ) {
      return NULL;
    }
    
    FSRef  ref;
    char  path[256];
    OSErr  err;
    
      
    err = FSpMakeFSRef(&ps, &ref);
    if ( !err ) {
      err = FSRefMakePath( &ref, (UInt8*)path, sizeof ( path ) );
      if ( !err ) {
        full_font_path = path;
      } else {
        return NULL;
      }
    } else {
      return NULL;
    }
#endif
#ifdef WIN32
    string font_display_name, font_file_name;
    string full_font_name = font_name;
    if( bold ) {
      full_font_name = full_font_name + " Bold";
    }
    
    if( italic ) {
      full_font_name = full_font_name + " Italic";
    }
    if( !GetFontFile( full_font_name.c_str(), font_display_name, font_file_name ) ) {
      return NULL;
    } 
    
    string path = string( getenv( "windir" ) ) + "\\fonts\\";
    full_font_path = path + font_file_name;
#endif
#ifdef LINUX
    full_font_path = FC_GetFontByName( font_name.c_str() );
#endif
    FTFont *font = NULL;
	// search font cache first:
    string font_to_search = render_type + full_font_path;
	map<string,FTFont*>::iterator f = font_db.find( font_to_search );
	if ( f != font_db.end() )
	  return (*f).second;
  if( render_type == "POLYGON" ) {
    font = new FTGLPolygonFont( full_font_path.c_str() ); 
  } else if( render_type == "TEXTURE" ) {
    font = new FTGLTextureFont( full_font_path.c_str() ); 
  } else if( render_type == "OUTLINE" ) {
    font = new FTGLOutlineFont( full_font_path.c_str() ); 
  } else if( render_type == "EXTRUDED" ) {
    font = new FTGLExtrdFont( full_font_path.c_str() ); 
  }
  if( !font || font->Error() ) return NULL;
	else {
	  font->FaceSize( 72 );
	  font->CharMap(ft_encoding_unicode);
	  font->UseDisplayList( true );
	  font_db[font_to_search]=font;
	  return font;
	}
  }

}

#endif

#ifndef LINUX
#define DEFAULT_SERIF_FONT "Times New Roman"
#else
#define DEFAULT_SERIF_FONT "FreeSerif"
#endif
#define DEFAULT_SANS_FONT "Helvetica"
#define DEFAULT_TYPEWRITER_FONT "Courier New"

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase FontStyle::database( "FontStyle", 
                                     &(newInstance<FontStyle>), 
                                     typeid( FontStyle ),
                                     &X3DFontStyleNode::database );

namespace FontStyleInternals {
  FIELDDB_ELEMENT( FontStyle, family, INITIALIZE_ONLY );
  FIELDDB_ELEMENT( FontStyle, horizontal, INITIALIZE_ONLY );
  FIELDDB_ELEMENT( FontStyle, justify, INITIALIZE_ONLY );
  FIELDDB_ELEMENT( FontStyle, language, INITIALIZE_ONLY );
  FIELDDB_ELEMENT( FontStyle, leftToRight, INITIALIZE_ONLY );
  FIELDDB_ELEMENT( FontStyle, size, INITIALIZE_ONLY );
  FIELDDB_ELEMENT( FontStyle, spacing, INITIALIZE_ONLY );
  FIELDDB_ELEMENT( FontStyle, style, INITIALIZE_ONLY );
  FIELDDB_ELEMENT( FontStyle, topToBottom, INITIALIZE_ONLY );
  FIELDDB_ELEMENT( FontStyle, renderType, INITIALIZE_ONLY );
}

FontStyle::FontStyle( 
                     Inst< SFNode   >  _metadata,
                     Inst< MFString >  _family,
                     Inst< SFBool   >  _horizontal,
                     Inst< MFString >  _justify,
                     Inst< SFString >  _language,
                     Inst< SFBool   >  _leftToRight,
                     Inst< SFFloat  >  _size,
                     Inst< SFFloat  >  _spacing,
                     Inst< SFString >  _style,
                     Inst< SFBool   >  _topToBottom,
                     Inst< SFString > _renderType ) :
  X3DFontStyleNode( _metadata ),
  family     ( _family      ),
  horizontal ( _horizontal  ),
  justify    ( _justify     ),
  language   ( _language    ),
  leftToRight( _leftToRight ),
  size       ( _size        ),
  spacing    ( _spacing     ),
  style      ( _style       ),
  topToBottom( _topToBottom ),
  renderType ( _renderType  )
#if defined( HAVE_FREETYPE ) && defined( HAVE_FTGL )
  ,font( NULL )
#endif
 {

  type_name = "FontStyle";
  
  database.initFields( this );

  // set default values
  family->push_back( "SERIF" );
  horizontal->setValue( true );
  justify->push_back( "BEGIN" );
  leftToRight->setValue( true );
  size->setValue( 1.0 );
  spacing->setValue( 1.0 );

  style->addValidValue("PLAIN");
  style->addValidValue( "BOLD" );
  style->addValidValue("ITALIC");
  style->addValidValue( "BOLDITALIC" );
  style->addValidValue("");
  style->setValue( "PLAIN" );
  topToBottom->setValue( true );
  renderType->addValidValue( "POLYGON" );
  renderType->addValidValue( "TEXTURE" );
  renderType->addValidValue( "OUTLINE" );
  renderType->addValidValue( "EXTRUDED" );
  renderType->setValue( "TEXTURE" );

#if !( defined( HAVE_FREETYPE ) && defined( HAVE_FTGL ) )
  Console(4) << "Warning: H3D API compiled withour FTGL or FreeType. FontStyle"
             << " nodes will be unusable." << endl;
#endif
}

#if defined( HAVE_FREETYPE ) && defined( HAVE_FTGL )
void FontStyle::buildFonts() {

  if( fonts_built )
    return;
  
  const string &render_type = renderType->getValue();
  if( render_type != "POLYGON" && render_type != "TEXTURE" &&
      render_type != "OUTLINE" && render_type != "EXTRUDED" ) {
    stringstream s;
    s << " in " << renderType->getFullName() 
      << ". Valid values are \"POLYGON\", \"TEXTURE\", \"EXTRUDED\" and \"OUTLINE\"";
    throw InvalidFontStyleRenderType( render_type, 
                                      s.str(), 
                                      H3D_FULL_LOCATION ); 
  }

  const string &font_style = style->getValue();
  bool italic = false;
  bool bold = false;
  if( font_style == "PLAIN" || font_style == "" ) {
  } else if( font_style == "ITALIC" ) {
    italic = true;
  } else if( font_style == "BOLD" ) {
    bold = true;
  } else if( font_style == "BOLDITALIC" ) {
    italic = true;
    bold = true;
  } else {
    stringstream s;
    s << " in " << style->getFullName() 
      << ". Valid values are \"PLAIN\", \"BOLD\", \"ITALIC\" and \"BOLDITALIC\"";
    throw InvalidFontStyleStyle( font_style, s.str(), H3D_FULL_LOCATION );  
  }

  // try all the font family names specified to find a font.
  for( MFString::const_iterator n = family->begin();
       n != family->end();
       n++ ) {
    const char *font_name = (*n).c_str();
    if( strcmp( font_name, "SANS" ) == 0 ) {
      font_name = DEFAULT_SANS_FONT;
    } else if( strcmp( font_name, "SERIF" ) == 0 ) {
      font_name = DEFAULT_SERIF_FONT;
    } else if( strcmp( font_name, "TYPEWRITER" ) == 0 ) {
      font_name = DEFAULT_TYPEWRITER_FONT;
    } 
    
	font =FontStyleInternals::getFontByName( font_name, bold, italic, render_type );
    // if bold or italic font was not found try to get the plain font
    // instead.
    if( !font && ( bold||italic) ) 
      font = FontStyleInternals::getFontByName( font_name, false, false, render_type );

    // the font was found, so use that font
    if( font )
      break;
  }
  
  // no font was found use default font and print a warning message.
  if( !font ) {
    if( family->size() != 0 ) {
      Console(4) << "Warning: None of the fonts [";
      for( MFString::const_iterator n = family->begin();
           n != family->end();
           n++ ) {
        Console(4) << " \"" << *n << "\""; 
      }

      Console(4) << " ] could be found. Using \"" << DEFAULT_SERIF_FONT 
           << "\" instead. " << endl;
    }

    font = FontStyleInternals::getFontByName( DEFAULT_SERIF_FONT, 
                                              bold, 
                                              italic,
											  render_type);
    if( !font && ( bold||italic) )
      font = FontStyleInternals::getFontByName( DEFAULT_SERIF_FONT, 
                                         false, 
                                         false,
										 render_type);


    if( !font ) {
      throw Exception::H3DAPIException( "Unable to load any font", 
                                        H3D_FULL_LOCATION );
    }
  }

  fonts_built = true;
}

X3DFontStyleNode::Justification FontStyle::getMajorJustification() {
  if( justify->size() == 0 ) 
    return BEGIN;
  else {
    const string &s = justify->getValueByIndex(0);
    if( s == "" ) return BEGIN;
    if( s == "FIRST" ) return FIRST;
    if( s == "BEGIN" ) return BEGIN;
    if( s == "MIDDLE" ) return MIDDLE;
    if( s == "END" ) return END;

    stringstream serr;
    serr << " in " << justify->getFullName() 
         << ". Valid values are \"BEGIN\", \"FIRST\", \"MIDDLE\" and \"END\"";
    throw InvalidFontStyleJustify( s, serr.str(), H3D_FULL_LOCATION ); 
  }
}

X3DFontStyleNode::Justification FontStyle::getMinorJustification() {
  if( justify->size() < 2 ) 
    return FIRST;
  else {
    const string &s = justify->getValueByIndex(1);
    if( s == "FIRST" ) return FIRST;
    if( s == "BEGIN" ) return BEGIN;
    if( s == "MIDDLE" ) return MIDDLE;
    if( s == "END" ) return END;

    stringstream serr;
    serr << " in " << justify->getFullName() 
         << ". Valid values are \"BEGIN\", \"FIRST\", \"MIDDLE\" and \"END\"";
    throw InvalidFontStyleJustify( s, serr.str(), H3D_FULL_LOCATION ); 
  }
}

void FontStyle::renderChar( unsigned char c ) {
  char t[2];  t[0]=c; t[1]='\0';
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  H3DFloat s = size->getValue();
  H3DFloat default_size = font->Ascender() - font->Descender();
  H3DFloat scale_factor = s / default_size;
  glScalef( scale_factor, scale_factor, scale_factor );
  
  if( renderType->getValue() == "TEXTURE" ) {
    glEnable( GL_TEXTURE_2D);
    glEnable( GL_ALPHA_TEST );
    glAlphaFunc (GL_GREATER, 0);
  }
  
  glNormal3f( 0, 0, 1 );
  font->Render( t );
  
  if( renderType->getValue() == "TEXTURE" ) {
    glDisable( GL_ALPHA_TEST );
    glDisable( GL_TEXTURE_2D);
  }
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
}

H3DFloat FontStyle::ascender() {
  H3DFloat default_size = font->Ascender() - font->Descender();
  H3DFloat scale_factor = size->getValue() / default_size;
  return font->Ascender() * scale_factor;
}

H3DFloat FontStyle::descender() {
  H3DFloat default_size = font->Ascender() - font->Descender();
  H3DFloat scale_factor = size->getValue() / default_size;
  return font->Descender() * scale_factor;
}

Vec3f FontStyle::charDimensions( unsigned char c ) {
      
  H3DFloat default_size = font->Ascender() - font->Descender();
  H3DFloat scale_factor = size->getValue() / default_size;
  char t[2];  t[0]=c; t[1]='\0';
  float llx, lly, llz, urx, ury, urz;
  font->BBox( t, llx, lly, llz, urx, ury, urz );
  
  return Vec3f(font->Advance(t),
	       font->Ascender()-font->Descender(),
	       llz-urz) * scale_factor;

}

#endif
