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

using namespace H3D;

#if defined( HAVE_FREETYPE ) && defined( HAVE_FTGL )

#ifdef FTGL_INCLUDE_IS_UPPER
#include <FTGL/FTGL.h>
#else
#include <FTGL/ftgl.h>
#endif

#include <FTGL/FTGLTextureFont.h>
#include <FTGL/FTGLPolygonFont.h>
#include <FTGL/FTGLOutlineFont.h>
#include <FTGL/FTGLExtrdFont.h>


#ifdef HAVE_FONTCONFIG
  //#include <freetype/freetype.h>
  // from 2.5 to 2.5.1 for freetype libarary, the freetype.h path is 
  // changed, it is the official way to handle it as below
  #include <ft2build.h>
  #include FT_FREETYPE_H
  #include FT_TRUETYPE_TABLES_H
  #include <fontconfig/fontconfig.h>
  #include <string.h>
#else //HAVE_FONTCONFIG
  #if defined(__APPLE__) && defined(__MACH__)
    #include <freetype/ftmac.h>
  #else
    //#include <freetype/freetype.h>
    // from 2.5 to 2.5.1 for freetype library, the freetype.h path is 
    // changed, it is the official way to handle it as below
    #include <ft2build.h>
    #include FT_FREETYPE_H
    #include FT_TRUETYPE_TABLES_H
  #endif
#endif //else HAVE_FONTCONFIG

namespace FontStyleInternals {
  // Internal map where the destructor deletes allocated
  // FTFonts. Note, this code will not work if FTGL is delayloaded
  // in Windows because of different initialization order.
  class FontStyleMap : public map< string, FTFont* > {
  public:
    // This will not work if FTGL is delayloaded.
    ~FontStyleMap() {
// The reason for only cleaning up in Windows is that cleaning up on non-windows might
// cause segmentation fault. I have not fully understood why but since this is
// only a memory leak on exit issue we will disable it on non-windows for now.
// The segmentation fault occurrs in freetype function FT_Done_Face. Perhaps it
// has to do with shared library initialization/deinitialization order and static
// variables.
#ifdef WIN32
      while( !empty() ) {
        iterator i = begin();
        delete (*i).second;
        erase( i );
      }
#endif
    }
  };
  static FontStyleMap font_db;

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
// NOTE: Never call this function unless subkey, szName, szData are all NULL or all not NULL.
LONG GetNextNameValue(HKEY key, LPCTSTR subkey, LPTSTR szName, LPTSTR szData)
{
  static HKEY hkey = NULL;
  static DWORD dwIndex = 0;
  LONG retval;

  if (subkey == NULL && szName == NULL && szData == NULL)  {
    if (hkey)
      RegCloseKey(hkey);
    hkey = NULL;
    return ERROR_SUCCESS;
  }

  if (subkey && subkey[0] != 0)  {
    retval = RegOpenKeyEx(key, subkey, 0, KEY_READ, &hkey);
    if (retval != ERROR_SUCCESS){
      return retval;
    }
    dwIndex = 0;
  }  else  {
    ++dwIndex;
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
  if (retval == ERROR_SUCCESS)   {
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

#ifdef HAVE_FONTCONFIG
string FC_GetFontByName( const char *font_name, bool bold, bool italic ) {
  // the fn_canditate variable will be set if we have found a font
  // of the given family but with an unrecognised style. It will be
  // used of no appropriate style is found for the family.
  string fn_candidate = "";
  FcObjectSet *os = 0;
  FcFontSet  *fs;
  FcPattern   *pat;

  vector< string > styles;

  if( bold && italic ) {
    styles.push_back( "BoldItalic" );
    styles.push_back( "Bold Italic" );
    styles.push_back( "Bold Oblique" );
    styles.push_back( "BoldOblique" );
  } else if( bold ) {
    styles.push_back( "Bold" );
  }  else if( italic ) {
    styles.push_back( "Italic" );
    styles.push_back( "Oblique" );
  } else {
    styles.push_back( "Regular" );
    styles.push_back( "Normal" );
    styles.push_back( "Medium" );
  }

  pat = FcPatternCreate ();
  os = FcObjectSetBuild (FC_FAMILY, FC_FILE, FC_STYLE, NULL);
  fs = FcFontList (NULL, pat, os);
  if (pat)
    FcPatternDestroy (pat);
  
  if (fs) {
    int  j;
    
    for (j = 0; j < fs->nfont; ++j) {
      FcChar8 *file;
      FcChar8 *style;
      FcChar8 *family;

      FcPatternGetString(fs->fonts[j], FC_FILE, 0, &file); 
      FcPatternGetString(fs->fonts[j], FC_STYLE, 0, &style );
      FcPatternGetString(fs->fonts[j], FC_FAMILY, 0, &family );
   
      if (strncasecmp(font_name, (char*)family, strlen(font_name)) == 0) {
        // check if the font is of the correct type.
        for( size_t s = 0; s < styles.size(); ++s ) {
          if (strcasecmp( styles[s].c_str(), 
              (char*)style ) == 0) {
            string match( (char *)file );
            FcFontSetDestroy (fs);
            return( match );
        }

        // set value to use if no more correct font of same family is found.
        fn_candidate = string( (char *)file );
      }

      }
    }
    FcFontSetDestroy (fs);
  }
  return fn_candidate;
}



#endif


  FTFont* getFontByName( const string &font_name,
                         bool bold, 
                         bool italic,
                         const string &render_type ) {

  string full_font_path;

  string full_font_name = font_name;
  if( bold ) {
    full_font_name = full_font_name + " Bold";
  }

  if( italic ) {
    full_font_name = full_font_name + " Italic";
  }
 
#ifdef WIN32
    string font_display_name, font_file_name;
    if( !GetFontFile( full_font_name.c_str(), font_display_name, font_file_name ) ) {
      return NULL;
    } 
    
    string path = string( getenv( "windir" ) ) + "\\fonts\\";
    full_font_path = path + font_file_name;
#else
  #ifdef HAVE_FONTCONFIG
    full_font_path = FC_GetFontByName( font_name.c_str(), bold, italic );
    if( full_font_path == "" ) return NULL;
  #else
 
    #if defined(__APPLE__) && defined(__MACH__)
      // the following code only works if freetype has been compiled with the --with-old-mac-fonts defined in configure.
      char path[1024];
      FT_Long face_index;
      FT_Error r = FT_GetFilePath_From_Mac_ATS_Name( full_font_name.c_str(),
                                                     (UInt8 * )path,
                                                     1024,
                                                     &face_index );
      if ( r != FT_Err_Ok ) {
        return NULL;
      }
    
      full_font_path = path;
    #else
      // no implementation to lookup fonts based on names was found
      return NULL;
    #endif
   #endif // else HAVE_FONTCONFIG
#endif //
    FTFont *font = NULL;
  // search font cache first:
    string font_to_search = render_type + full_font_path;
    FontStyleInternals::FontStyleMap::iterator f =
      font_db.find( font_to_search );
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
    s << "Invalid renderType: \"" << render_type
      << "\" in " << renderType->getFullName()
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
    Console(4) << "Invalid FontStyle style: \"" << font_style 
                     << "\" in " << style->getFullName() 
                     << ". Valid values are \"PLAIN\", \"BOLD\", \"ITALIC\" and \"BOLDITALIC\"";
  }

  // try all the font family names specified to find a font.
  for( MFString::const_iterator n = family->begin();
       n != family->end();
       ++n ) {
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
           ++n ) {
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

 void FontStyle::renderHorizontalLine( const string &text ) {
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
#ifdef H3D_WINDOWS
   wchar_t * wtext = new wchar_t[text.size()];
   const char *src = text.c_str();
   size_t ret = mbsrtowcs( wtext, &src, size_t( text.size() ), NULL );
   if( errno == EILSEQ ) {
    // Could not convert try to use original text string.
    font->Render( text.c_str() );
   } else {
    font->Render( wtext, ret );
   }
   delete [] wtext;
#else
   font->Render( text.c_str() );
#endif

   if( renderType->getValue() == "TEXTURE" ) {
     glDisable( GL_ALPHA_TEST );
     glDisable( GL_TEXTURE_2D);
   }
   glPopMatrix();
 }

void FontStyle::renderChar( unsigned char c ) {
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

#ifndef FTGL_INCLUDE_IS_UPPER
  if( c < 128 ) {
    // Ascii c is below 127 which means that we should not
    // touch the value.
#endif
    char t[2];  t[0]=c; t[1]='\0';
    font->Render( t );
#ifndef FTGL_INCLUDE_IS_UPPER
  } else {
    // The math to explain this conversion can be found here
    // http://everything2.com/title/Converting+ASCII+to+UTF-8
    // It is needed because FTGL added unicode support and H3DAPI does
    // not support unicode correctly yet. This avoids a crash.
    char t[3];
    t[0]=(c >> 6) | 0xC0;
    t[1]=(c & 0x3F) | 0x80;
    t[2]='\0';
    font->Render( t );
  }
#endif
  
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
#ifndef FTGL_INCLUDE_IS_UPPER
  if( c < 128 ) {
    // Ascii c is below 127 which means that we should not
    // touch the value.
#endif
    char t[2];
    t[0]=c; t[1]='\0';
    float llx, lly, llz, urx, ury, urz;
    font->BBox( t, llx, lly, llz, urx, ury, urz );
    return Vec3f(font->Advance(t),
         font->Ascender()-font->Descender(),
         llz-urz) * scale_factor;
#ifndef FTGL_INCLUDE_IS_UPPER
  } else {
    // The math to explain this conversion can be found here
    // http://everything2.com/title/Converting+ASCII+to+UTF-8
    // It is needed because FTGL added unicode support and H3DAPI does
    // not support unicode correctly yet. This avoids a crash.
    char t[3];
    t[0]=(c >> 6) | 0xC0;
    t[1]=(c & 0x3F) | 0x80;
    t[2]='\0';
    float llx, lly, llz, urx, ury, urz;
    font->BBox( t, llx, lly, llz, urx, ury, urz );
    return Vec3f(font->Advance(t),
         font->Ascender()-font->Descender(),
         llz-urz) * scale_factor;
  }
#endif

}

#endif
