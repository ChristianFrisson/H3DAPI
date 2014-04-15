//////////////////////////////////////////////////////////////////////////////
//    Copyright 2010-2014, SenseGraphics AB
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
/// \file H3DViewerFieldValuesDialogPropGrid.cpp
/// \brief CPP file for H3DViewerFieldValuesDialogPropGrid.
///
//
//
//////////////////////////////////////////////////////////////////////////////
#include "H3DViewerFieldValuesDialogPropGrid.h"

#ifdef HAVE_WXPROPGRID

#include <H3D/Group.h>
#include <H3D/SFString.h>
#include <H3D/SFInt32.h>
#include <H3D/SFVec3d.h>
#include <H3D/SFVec2f.h>
#include <H3D/SFVec2d.h>
#include <H3D/SFVec4f.h>
#include <H3D/SFVec4d.h>
#include <H3D/SFRotation.h>
#include <H3D/SFColor.h>
#include <H3D/PythonScript.h>
#include <H3D/Scene.h>

using namespace H3D;

H3DLongStringProperty::H3DLongStringProperty( bool _read_only,
                                              const wxString& label,
                                              const wxString& name,
                                              const wxString& value ) : 
  wxLongStringProperty( label, name, value ),
  read_only( _read_only ) {
  ChangeFlag( wxPG_PROP_NOEDITOR, read_only );
}

bool H3DLongStringProperty::OnButtonClick( wxPropertyGrid* propGrid, wxString& value ) {
  // the following code is a copy of the wxLongStringProperty::OnButtonClick
  // code with the only difference being that the dialog text is not editable 
  // and there is no cancel button when read_only is true.

  wxPGProperty * prop = this;
  // launch editor dialog
  wxDialog* dlg = new wxDialog(propGrid,-1,prop->GetLabel(),wxDefaultPosition,wxDefaultSize,
                               wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER|wxCLIP_CHILDREN);

  dlg->SetFont(propGrid->GetFont()); // To allow entering chars of the same set as the propGrid
  
  // Multi-line text editor dialog.
#if !wxPG_SMALL_SCREEN
  const int spacing = 8;
#else
  const int spacing = 4;
#endif
  wxBoxSizer* topsizer = new wxBoxSizer( wxVERTICAL );
  wxBoxSizer* rowsizer = new wxBoxSizer( wxHORIZONTAL );
  wxTextCtrl* ed = new wxTextCtrl(dlg,11,value,
                                  wxDefaultPosition,wxDefaultSize,wxTE_MULTILINE|(read_only?wxTE_READONLY:0) );
  
  rowsizer->Add( ed, 1, wxEXPAND|wxALL, spacing );
  topsizer->Add( rowsizer, 1, wxEXPAND, 0 );
  
  wxStdDialogButtonSizer* buttonSizer = new wxStdDialogButtonSizer();
  buttonSizer->AddButton(new wxButton(dlg, wxID_OK));
  if( !read_only )
    buttonSizer->AddButton(new wxButton(dlg, wxID_CANCEL));
  buttonSizer->Realize();
  topsizer->Add( buttonSizer, 0,
                 wxALIGN_RIGHT|wxALIGN_CENTRE_VERTICAL|wxBOTTOM|wxRIGHT,
                 spacing );
  
  dlg->SetSizer( topsizer );
  topsizer->SetSizeHints( dlg );
  
#if !wxPG_SMALL_SCREEN
  dlg->SetSize(400,300);
  
  dlg->Move( propGrid->GetGoodEditorDialogPosition(prop,dlg->GetSize()) );
#endif
  
  int res = dlg->ShowModal();
  
  if ( res == wxID_OK )
    {
      value = ed->GetValue();
      dlg->Destroy();
      return true;
    }
  dlg->Destroy();
  return false;
}

H3DViewerFieldValuesDialogPropGrid::H3DViewerFieldValuesDialogPropGrid( 
             wxWindow* parent, 
             wxWindowID id, 
             const wxString& title, 
             const wxPoint& pos, 
             const wxSize& size, 
             long style,
             bool _destroy_on_close ) : 
  wxFrame( parent, id, title, pos, size, style ),
  destroy_on_close( _destroy_on_close ) {

  this->SetSizeHints( wxDefaultSize, wxDefaultSize );
  
  wxBoxSizer* bSizer2;
  bSizer2 = new wxBoxSizer( wxVERTICAL );
  
  prop_grid_panel = new H3DViewerFieldValuesPanelPropGrid( this, wxID_ANY, wxDefaultPosition, wxDefaultSize );
        
  bSizer2->Add( prop_grid_panel, 1, wxALL|wxEXPAND, 5 );
  this->SetSizer( bSizer2 );
  this->Layout();
        
  // Connect Events
  this->Connect( wxEVT_IDLE, wxIdleEventHandler( H3DViewerFieldValuesPanelPropGrid::OnIdle ), NULL, prop_grid_panel );
  this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( H3DViewerFieldValuesDialogPropGrid::OnClose ) );
}

H3DViewerFieldValuesDialogPropGrid::~H3DViewerFieldValuesDialogPropGrid()
{
  // Disconnect Events
  this->Disconnect( wxEVT_IDLE, wxIdleEventHandler( H3DViewerFieldValuesPanelPropGrid::OnIdle ), NULL, prop_grid_panel );
}

void H3DViewerFieldValuesDialogPropGrid::OnClose( wxCloseEvent& event ) {
  if( destroy_on_close ) {
    Destroy();
  } else {
    Hide();
  }
}

void H3DViewerFieldValuesDialogPropGrid::displayFieldsFromNode( Node *n ) {
  if( !n ) {
    SetTitle(wxT(""));
  } else {
    SetTitle( wxString(n->getTypeName().c_str(),wxConvUTF8) );
  }
  prop_grid_panel->displayFieldsFromNode( n );
}

void H3DViewerFieldValuesDialogPropGrid::displayFieldsFromProgramSettings() {
  SetTitle( wxT( "Program settings" ) );
  prop_grid_panel->displayFieldsFromProgramSettings();
}

H3DViewerFieldValuesPanelPropGrid::H3DViewerFieldValuesPanelPropGrid( wxWindow* parent, 
                                      wxWindowID id, 
                                      const wxPoint& pos, 
                                      const wxSize& size, 
                                      long style ) : 
  wxPanel( parent, id, pos, size, style ),
  displayed_node( NULL ),
  mode( 0 ) {

  this->SetSizeHints( wxDefaultSize, wxDefaultSize );
  
  wxBoxSizer* bSizer2;
  bSizer2 = new wxBoxSizer( wxVERTICAL );
  
  FieldValuesGrid = new wxPropertyGrid( this, wxID_ANY, wxDefaultPosition, wxDefaultSize,  wxPG_SPLITTER_AUTO_CENTER  );
  
  bSizer2->Add( FieldValuesGrid, 1, wxALL|wxEXPAND, 5 );
  this->SetSizer( bSizer2 );
  this->Layout();
  
  // Connect Events
  FieldValuesGrid->Connect( wxEVT_PG_CHANGED, wxPropertyGridEventHandler( H3DViewerFieldValuesPanelPropGrid::OnPropertyChanged ), NULL, this );

  Scene::addProgramSettingsCallback( settingsCallback, this );
}

H3DViewerFieldValuesPanelPropGrid::~H3DViewerFieldValuesPanelPropGrid()
{
  Scene::clearProgramSettingsCallbacks();

  // Disconnect Events
  FieldValuesGrid->Disconnect( wxEVT_PG_CHANGED, wxPropertyGridEventHandler( H3DViewerFieldValuesPanelPropGrid::OnPropertyChanged ), NULL, this );
  
}

void H3DViewerFieldValuesPanelPropGrid::displayFieldsFromProgramSettings() {
  //  if( mode != 1 ) {
    property_update_fields.clear();
    
    populateGridFromProgramSettings( FieldValuesGrid, property_update_fields );
    mode = 1;
    //  }
}

void H3DViewerFieldValuesPanelPropGrid::displayFieldsFromNode( Node *n ) {
  mode = 0;
  bool new_node = n != displayed_node.get();
  displayed_node.reset( n );
  if( !n ) {
    property_update_fields.clear();
    FieldValuesGrid->Clear();
    return;
  }

  if( new_node ) {
    // Using lookupTypeId here instead of lookupNodeInstance because
    // the fields are not interesting here.
    H3DNodeDatabase *db = H3DNodeDatabase::lookupTypeId( typeid( *n ) );
    // cannot use AutoRef because we do not want the node to become initialized.
    Node *default_values_node = NULL;
    if( !n->getProtoInstanceParent() ) {
      default_values_node = db->createNode();
    }
    property_update_fields.clear();
    populateGridFromNode( FieldValuesGrid, n, default_values_node, property_update_fields );
    if( default_values_node ) delete default_values_node;
  }
}


void H3DViewerFieldValuesPanelPropGrid::OnIdle( wxIdleEvent& event ) {
  TimeStamp now;
  if( now - last_fields_update > 0.25 ) {
    if( mode == 0 ) displayFieldsFromNode( displayed_node.get() );
    last_fields_update = now;
    for( unsigned int i = 0; i < property_update_fields.size(); ++i ) {
      property_update_fields[i]->upToDate();
    }
  }
}

void H3DViewerFieldValuesPanelPropGrid::OnPropertyChanged( wxPropertyGridEvent& event ) {
  string name( event.GetPropertyName().mb_str() );
  wxPGProperty *property = event.GetProperty();
  Field *f = (Field *)property->GetClientData();

  if( f ) {

    try {

      wxVariant value = property->GetValue();
      X3DTypes::X3DType x3d_type = f->getX3DType();
  
      if( x3d_type == X3DTypes::SFFLOAT ) {
        double b = value.GetDouble();
        static_cast< SFFloat * >( f )->setValue( (H3DFloat) b );
      } else if ( x3d_type == X3DTypes::SFDOUBLE ) {
        double b = value.GetDouble();
        static_cast< SFDouble * >( f )->setValue( b );
      } else if ( x3d_type == X3DTypes::SFTIME ) {
        double b = value.GetDouble();
        static_cast< SFTime * >( f )->setValue( b );
      } else if ( x3d_type == X3DTypes::SFCOLOR ) {
        wxColour wc = wxAny(value).As<wxColour>();
        static_cast< SFColor * >( f )->setValue( RGB( wc.Red()/255.0, wc.Green()/255.0, wc.Blue()/255.0 ) );
        } 
  #ifdef USE_VECTOR_PROPERTIES
      else if ( x3d_type == X3DTypes::SFVEC2F ) {
        H3DwxVector wc = wxAny(value).As<H3DwxVector>();
        Vec2f v( wc.getValue(0), wc.getValue(1) );
        static_cast< SFVec2f * >( f )->setValue( v );
      } else if ( x3d_type == X3DTypes::SFVEC2D ) {
        H3DwxVector wc = wxAny(value).As<H3DwxVector>();
        Vec2f v( wc.getValue(0), wc.getValue(1) );
        static_cast< SFVec2d * >( f )->setValue( v );
      } else if ( x3d_type == X3DTypes::SFVEC3F ) {
        H3DwxVector wc = wxAny(value).As<H3DwxVector>();
        Vec3f v( wc.getValue(0), wc.getValue(1), wc.getValue(2) );
        static_cast< SFVec3f * >( f )->setValue( v );
      } else if ( x3d_type == X3DTypes::SFVEC3D ) {
        H3DwxVector wc = wxAny(value).As<H3DwxVector>();
        Vec3f v( wc.getValue(0), wc.getValue(1), wc.getValue(2) );
        static_cast< SFVec3d * >( f )->setValue( v );
      } else if ( x3d_type == X3DTypes::SFVEC4F ) {
        H3DwxVector wc = wxAny(value).As<H3DwxVector>();
        Vec4f v( wc.getValue(0), wc.getValue(1), wc.getValue(2), wc.getValue(3) );
        static_cast< SFVec4f * >( f )->setValue( v );
      } else if ( x3d_type == X3DTypes::SFVEC4D ) {
        H3DwxVector wc = wxAny(value).As<H3DwxVector>();
        Vec4f v( wc.getValue(0), wc.getValue(1), wc.getValue(2), wc.getValue(3) );
        static_cast< SFVec4d * >( f )->setValue( v );
      } else if ( x3d_type == X3DTypes::SFROTATION ) {
        H3DwxVector wc = wxAny(value).As<H3DwxVector>();
        Rotation v( Vec3f( wc.getValue(0), wc.getValue(1), wc.getValue(2)), wc.getValue(3) );
        static_cast< SFRotation * >( f )->setValue( v );
      } 
  #endif //USE_VECTOR_PROPERTIES
      else if( x3d_type == X3DTypes::SFBOOL ) {    
        bool b = value.GetBool();
        static_cast< SFBool * >( f )->setValue( b );
      } else if( x3d_type == X3DTypes::SFSTRING && 
                 dynamic_cast< wxEnumProperty * >( property ) ) {    
        string s (property->ValueToString( value ).mb_str() );
        static_cast< SFString * >( f )->setValue( s );
      } else {
        if( ParsableField *pf = dynamic_cast< ParsableField * >( f ) ) {
          string s( property->ValueToString( value ).mb_str() );
          try {
            pf->setValueFromString( s );
          } catch(...) {
            // touch field in order for editor value to be reset
            pf->touch();
          }
        }
      }

    } catch ( const Field::FieldAccessError& e ) {
      stringstream s;
      s << e;
      wxMessageBox( wxString(s.str().c_str(),wxConvLibc),
                    wxT("Error"), wxOK | wxICON_EXCLAMATION);
    }
  }
}

void H3DViewerFieldValuesPanelPropGrid::populateGridFromNode( wxPropertyGrid *FieldValuesGrid,
                                                               Node *n,
                                                               Node *default_values_node,
                                                               AutoPtrVector< Field > &property_update_fields ) {

  FieldValuesGrid->Clear();

  H3DNodeDatabase *db = H3DNodeDatabase::lookupNodeInstance( n );

  list< FieldDBElement * > init_only_fields;
  list< FieldDBElement * > input_only_fields;
  list< FieldDBElement * > output_only_fields;
  list< FieldDBElement * > input_output_fields;

  for( H3DNodeDatabase::FieldDBConstIterator i = db->fieldDBBegin();
       db->fieldDBEnd() != i; ++i ) {
    Field *f = i.getField( n ); 
   
    if( !f ||
        dynamic_cast< SFNode * >( f ) ||
        dynamic_cast< MFNode * >( f ) ) {
      continue;    
    }

    if( f->getAccessType() == Field::INITIALIZE_ONLY ) {
      init_only_fields.push_back( i.getFieldDBElement() );
    } else if( f->getAccessType() == Field::INPUT_ONLY ) {
      input_only_fields.push_back( i.getFieldDBElement() );
    } else if( f->getAccessType() == Field::OUTPUT_ONLY ) {
      output_only_fields.push_back( i.getFieldDBElement() );
    } else if( f->getAccessType() == Field::INPUT_OUTPUT ) {
      input_output_fields.push_back( i.getFieldDBElement() );
    }
  }

  int rows = 0;

  // initialize only fields.
  if( init_only_fields.size() > 0 ) {
    FieldValuesGrid->Append( new wxPropertyCategory(wxT("Initialize only"),wxPG_LABEL) );
  }

  for( list< FieldDBElement * >::iterator i = init_only_fields.begin();
       i != init_only_fields.end(); ++i ) {
    Field *f = (*i)->getField( n ); 
    Field *default_f = (*i)->getField( default_values_node );
    setupProperty( FieldValuesGrid, f, default_f, property_update_fields );
  }

  // input output fields
  if( input_output_fields.size() > 0 ) {
    FieldValuesGrid->Append( new wxPropertyCategory(wxT("Input/output"),wxPG_LABEL) );
  }

  for( list< FieldDBElement * >::iterator i = input_output_fields.begin();
       i != input_output_fields.end(); ++i ) {
    Field *f = (*i)->getField( n ); 
    Field *default_f = (*i)->getField( default_values_node );
    setupProperty( FieldValuesGrid, f, default_f, property_update_fields );
  }

  
  // output only fields.
  if( output_only_fields.size() > 0 ) {
    FieldValuesGrid->Append( new wxPropertyCategory(wxT("Output only"),wxPG_LABEL) );
  }

  for( list< FieldDBElement * >::iterator i = output_only_fields.begin();
       i != output_only_fields.end(); ++i ) {
    Field *f = (*i)->getField( n ); 
    Field *default_f = (*i)->getField( default_values_node );
    setupProperty( FieldValuesGrid, f, default_f, property_update_fields );
  }
  
  // input only fields.
  if( input_only_fields.size() > 0 ) {
    FieldValuesGrid->Append( new wxPropertyCategory(wxT("Input only"),wxPG_LABEL) );
  }

  for( list< FieldDBElement * >::iterator i = input_only_fields.begin();
       i != input_only_fields.end(); ++i ) {
    Field *f = (*i)->getField( n ); 
    // input only fields do not have a default value
    setupProperty( FieldValuesGrid, f, NULL, property_update_fields );
  }

#ifdef HAVE_PYTHON
  // if PythonScript node add top-level fields.
  PythonScript *ps = dynamic_cast< PythonScript * >( n );
  if( ps ) {
    vector< pair< string, Field *> > python_fields;
    ps->getTopLevelFields( python_fields );
  
    FieldValuesGrid->Append( new wxPropertyCategory(wxT("Python fields"),wxPG_LABEL) );

    for( vector< pair< string, Field *> >::iterator i = python_fields.begin();
         i != python_fields.end(); ++i ) {
      Field *f = (*i).second;
      const string &name = (*i).first;
      if( dynamic_cast< SFNode * >( f ) || 
          dynamic_cast< MFNode * >( f ) ) continue;
      wxPGProperty *p = getPropertyFromField( f, name );
      if( p ) {
        PropertyUpdater *updater = new PropertyUpdater( p );
        f->route( updater );
        property_update_fields.push_back( updater );
        FieldValuesGrid->Append( p );
      }
    }
  }
#endif
}

void H3DViewerFieldValuesPanelPropGrid::PropertyUpdater::update() {
  Field *f = event.ptr;

  X3DTypes::X3DType x3d_type = f->getX3DType();
  
  if( x3d_type == X3DTypes::SFFLOAT ) {
    SFFloat *sffloat = static_cast< SFFloat * >( f );
    wxFloatProperty *p = static_cast< wxFloatProperty * >( property );
    p->SetValue( sffloat->getValue() );
  } else if ( x3d_type == X3DTypes::SFDOUBLE ) {
    SFDouble *sfdouble = static_cast< SFDouble * >( f );
    wxFloatProperty *p = static_cast< wxFloatProperty * >( property );
    p->SetValue( sfdouble->getValue() );
  }  
#ifdef USE_VECTOR_PROPERTIES
  else if ( x3d_type == X3DTypes::SFVEC2F ) {
    SFVec2f *sfvec2f = static_cast< SFVec2f * >( f );
    wxVectorProperty *p = static_cast< wxVectorProperty * >( property );
    const Vec2f &v = sfvec2f->getValue();
    H3DwxVector wx_v(2);
    wx_v.setValue(0, v.x); wx_v.setValue(1, v.y); 
    p->SetValue( WXVARIANT(wx_v) );
  } else if ( x3d_type == X3DTypes::SFVEC2D ) {
    SFVec2d *sfvec2d = static_cast< SFVec2d * >( f );
    wxVectorProperty *p = static_cast< wxVectorProperty * >( property );
    const Vec2d &v = sfvec2d->getValue();
    H3DwxVector wx_v(2);
    wx_v.setValue(0, v.x); wx_v.setValue(1, v.y); 
    p->SetValue( WXVARIANT(wx_v) );
  } else if ( x3d_type == X3DTypes::SFVEC3F ) {
    SFVec3f *sfvec3f = static_cast< SFVec3f * >( f );
    wxVectorProperty *p = static_cast< wxVectorProperty * >( property );
    const Vec3f &v = sfvec3f->getValue();
    H3DwxVector wx_v(3);
    wx_v.setValue(0, v.x); wx_v.setValue(1, v.y); wx_v.setValue(2, v.z);
    p->SetValue( WXVARIANT(wx_v) );
  } else if ( x3d_type == X3DTypes::SFVEC3D ) {
    SFVec3d *sfvec3d = static_cast< SFVec3d * >( f );
    wxVectorProperty *p = static_cast< wxVectorProperty * >( property );
    const Vec3d &v = sfvec3d->getValue();
    H3DwxVector wx_v(3);
    wx_v.setValue(0, v.x); wx_v.setValue(1, v.y); wx_v.setValue(2, v.z);
    p->SetValue( WXVARIANT(wx_v) );
  } else if ( x3d_type == X3DTypes::SFVEC4F ) {
    SFVec4f *sfvec4f = static_cast< SFVec4f * >( f );
    wxVectorProperty *p = static_cast< wxVectorProperty * >( property );
    const Vec4f &v = sfvec4f->getValue();
    H3DwxVector wx_v(4);
    wx_v.setValue(0, v.x); wx_v.setValue(1, v.y); wx_v.setValue(2, v.z); wx_v.setValue(3, v.w);
    p->SetValue( WXVARIANT(wx_v) );
  } else if ( x3d_type == X3DTypes::SFVEC4D ) {
    SFVec4d *sfvec4d = static_cast< SFVec4d * >( f );
    wxVectorProperty *p = static_cast< wxVectorProperty * >( property );
    const Vec4d &v = sfvec4d->getValue();
    H3DwxVector wx_v(4);
    wx_v.setValue(0, v.x); wx_v.setValue(1, v.y); wx_v.setValue(2, v.z); wx_v.setValue(3, v.z);
    p->SetValue( WXVARIANT(wx_v) );
  } else if ( x3d_type == X3DTypes::SFROTATION ) {
    SFRotation *sfrotation = static_cast< SFRotation * >( f );
    wxVectorProperty *p = static_cast< wxVectorProperty * >( property );
    const Rotation &v = sfrotation->getValue();
    H3DwxVector wx_v(4);
    wx_v.setValue(0, v.axis.x); wx_v.setValue(1, v.axis.y); wx_v.setValue(2, v.axis.z); wx_v.setValue(3, v.angle );
    p->SetValue( WXVARIANT(wx_v) );
  } 
#endif //USE_VECTOR_PROPERTIES
  else if ( x3d_type == X3DTypes::SFTIME ) {
    SFTime *sftime = static_cast< SFTime * >( f );
    wxFloatProperty *p = static_cast< wxFloatProperty * >( property );
    p->SetValue( sftime->getValue() );
  } else if ( x3d_type == X3DTypes::SFCOLOR ) {
    SFColor *sfcolor = static_cast< SFColor * >( f );
    wxColourProperty *p = static_cast< wxColourProperty * >( property );
    const RGB &c = sfcolor->getValue();
    wxColour wc( c.r * 255, c.g * 255, c.b * 255 );
    wxVariant w;
    w << wc;
    p->SetValue( w );
  } else {
    ParsableField *pfield = dynamic_cast< ParsableField * >( f );
    if( pfield ) {
      string s = pfield->getValueAsString();
      wxString the_string( s.c_str(), wxConvUTF8 );
      if( !s.empty() && the_string.IsEmpty() )
        the_string = wxString( s.c_str(), *wxConvCurrent );
      property->SetValueFromString( the_string );
    }
  }

  // Set all properties that are different from default value to be red.
  if( isDefaultValue( f ) ) {
    property->SetTextColour( *wxBLACK );
  } else {
    property->SetTextColour( *wxRED );
  }

}

bool H3DViewerFieldValuesPanelPropGrid::PropertyUpdater::isDefaultValue( Field *f ) {
  // if non-existing default values we treat the value as a default value.
  if( default_value.get() == NULL || default_value_size == 0 ) {
    return true;
  }

  unsigned char *value = NULL;
  unsigned int value_size = 0;

 if( SFieldClass *sfield = dynamic_cast< SFieldClass * >( f ) ) {
   if( SFString *sfstring = dynamic_cast< SFString * > ( f ) ) {
     // the getValueAsVoidPtr function does not work well with
     // SFString fields so do a special case for that type.
     return sfstring->getValue() == default_value.get();
   } else {
     unsigned int data_size = sfield->valueTypeSize();
     value = new unsigned char[ data_size ];
     value_size = sfield->getValueAsVoidPtr( value, data_size );
   }
 } else if( MFieldClass *mfield = dynamic_cast< MFieldClass * >( f ) ) {
    unsigned int data_size = mfield->valueTypeSize() * mfield->size();
    unsigned int nr_elements;
    value = new unsigned char[ data_size ];
    value_size = mfield->getValueAsVoidPtr( value, nr_elements, data_size );
  } 

  if( value_size != default_value_size ) {
    delete [] value;
    return false;
  }

  for( unsigned int i = 0; i < value_size; ++i ) {
    if( value[i] != default_value.get()[i] ) {
      delete [] value;
      return false;
    }
  }
  
  delete [] value;
  return true;
} 


wxPGProperty *H3DViewerFieldValuesPanelPropGrid::getPropertyFromField( Field *f,
                                                                        const string &custom_field_name ) {
  
  wxPGProperty *property = NULL;

  bool is_readonly_field = 
    f->getAccessType() == Field::INITIALIZE_ONLY ||
    f->getAccessType() == Field::OUTPUT_ONLY;
  
  X3DTypes::X3DType x3d_type = f->getX3DType();
  if( x3d_type == X3DTypes::SFNODE ) {
    return NULL;
  } else if( x3d_type == X3DTypes::MFNODE ) {
    return NULL;
  } else {
    string value;
    string default_value;
    string field_name = custom_field_name;
    if( field_name.empty() ) field_name = f->getName();

    // we need a unique name for each property or Linux and OSX
    // versions of wxWidgets will get into an assert. Using a
    // combination of field name and pointer.
    stringstream wxprop_name_s;
    wxprop_name_s << field_name << f;
    string wxprop_name = wxprop_name_s.str();

    bool allow_cell_update = true;

   

    if( x3d_type == X3DTypes::SFSTRING ) {
      SFString *sfstring = static_cast< SFString * >( f );

      // use a wxEnumProperty if a set of valid values have been
      // specified for the field.
      if( sfstring->hasValidValues() ) {    
        const set<string > &valid_values = sfstring->getValidValues();
        wxPGChoices choices;

        unsigned int index = 0;
        for( set<string>::const_iterator i = valid_values.begin();
             i != valid_values.end(); ++i ) {
          choices.Add( wxString( (*i).c_str(), wxConvUTF8 ), index++ );
        }
        
        property = new wxEnumProperty( field_name.c_str(), wxprop_name.c_str(), choices );
      } else {
        property = new H3DLongStringProperty( false, field_name.c_str(), wxprop_name.c_str() );
      }
    } else if( x3d_type == X3DTypes::MFSTRING ) {
      property = new wxArrayStringProperty(field_name.c_str(), wxprop_name.c_str());
    } else if( x3d_type == X3DTypes::SFBOOL ) {
      property = new wxBoolProperty(field_name.c_str(), wxprop_name.c_str());
    } else if( x3d_type == X3DTypes::SFINT32  ) {
      property = new wxIntProperty(field_name.c_str(), wxprop_name.c_str());
      if( f->getAccessType() == Field::INPUT_OUTPUT ||
          f->getAccessType() == Field::INPUT_ONLY ) {
        property->SetEditor( "SpinCtrl" );
      }
    } else if( x3d_type == X3DTypes::SFFLOAT || x3d_type == X3DTypes::SFDOUBLE || x3d_type == X3DTypes::SFTIME  ) {
      //wxPG_FLOAT_PRECISION
      property = new wxFloatProperty(field_name.c_str(), wxprop_name.c_str());
      if( f->getAccessType() == Field::INPUT_OUTPUT ||
          f->getAccessType() == Field::INPUT_ONLY ) {
        property->SetEditor( "SpinCtrl" );
        property->SetAttribute( wxPG_ATTR_SPINCTRL_STEP, WXVARIANT( 0.05 ) );
      }
    }  
#ifdef USE_VECTOR_PROPERTIES
    else if( x3d_type == X3DTypes::SFVEC2F || x3d_type == X3DTypes::SFVEC2D) {
      wxString labels[] = {wxT("x"), wxT("y")};
      property = new wxVectorProperty(field_name.c_str(), wxprop_name.c_str(), wxArrayString( 2, labels ), H3DwxVector(2), !is_readonly_field );
    } else if( x3d_type == X3DTypes::SFVEC3F || x3d_type == X3DTypes::SFVEC3D) {
      wxString labels[] = {wxT("x"), wxT("y"), wxT( "z" )};
      property = new wxVectorProperty(field_name.c_str(), wxprop_name.c_str(), wxArrayString( 3, labels ), H3DwxVector(3), !is_readonly_field );
    } else if( x3d_type == X3DTypes::SFVEC4F || x3d_type == X3DTypes::SFVEC4D) {
      wxString labels[] = {wxT("x"), wxT("y"), wxT( "z" ), wxT( "w" ) };
      property = new wxVectorProperty(field_name.c_str(), wxprop_name.c_str(), wxArrayString( 4, labels ), H3DwxVector(4), !is_readonly_field  );
    } else if( x3d_type == X3DTypes::SFROTATION /*|| x3d_type == X3DTypes::SFROTATIOND*/ ) {
      wxString labels[] = {wxT("axis.x"), wxT("axis.y"), wxT( "axis.z" ), wxT("angle") };
      property = new wxVectorProperty(field_name.c_str(), wxprop_name.c_str(), wxArrayString( 4, labels ), H3DwxVector(4), !is_readonly_field  );
    } 
#endif //USE_VECTOR_PROPERTIES
    else if( x3d_type == X3DTypes::SFCOLOR ) {
      property = new wxColourProperty(field_name.c_str(), wxprop_name.c_str());
    } else if( ParsableField *pfield = dynamic_cast< ParsableField * >( f ) ) {
      property = new H3DLongStringProperty( false, field_name.c_str(), wxprop_name.c_str() );
    }
  }

  if( property ) {
    property->SetClientData( f );
    if(f->getAccessType() == Field::INITIALIZE_ONLY ||
       f->getAccessType() == Field::OUTPUT_ONLY ) {
      if( H3DLongStringProperty *lp = dynamic_cast< H3DLongStringProperty * >( property ) ) {
        lp->setReadOnlyDialog( true );
      }
      
      // Make sure it is not possible to set read only field values
      property->Enable ( false );
    }
  }

  return property;
}

void H3DViewerFieldValuesPanelPropGrid::setupProperty( wxPropertyGrid *FieldValuesGrid,
                                                       Field *f,
                                                       Field *default_f,
                                                       H3DUtil::AutoPtrVector< H3D::Field > &property_update_fields,
                                                       const string &property_name ) {

  void *default_value = NULL;
  unsigned int default_value_size = 0;
  

  if( MFieldClass *mfield = dynamic_cast< MFieldClass * >( default_f ) ) {
    // TODO: MFString does not work with getValueAsVoidPtr. Do special case or fix getValueAsVoidPtr.
    if( !dynamic_cast< MFString * >( default_f ) ) {
      unsigned int data_size = mfield->valueTypeSize() * mfield->size();
      unsigned int nr_elements;
      default_value = new unsigned char[ data_size ];
      default_value_size = mfield->getValueAsVoidPtr( default_value, nr_elements, data_size );
    }
  } else  if( SFieldClass *sfield = dynamic_cast< SFieldClass * >( default_f ) ) {
    if( SFString *sfstring = dynamic_cast< SFString * > ( f ) ) {
      // the getValueAsVoidPtr function does not work well with
      // SFString fields so do a special case for that type.
      const string &s = sfstring->getValue();
      default_value_size = s.size()+ 1;
      default_value = new char[ default_value_size ];
      strcpy( (char *) default_value, s.c_str() );
    } else {
      unsigned int data_size = sfield->valueTypeSize();
      default_value = new unsigned char[ data_size ];
      default_value_size = sfield->getValueAsVoidPtr( default_value, data_size );
    }
  }
  
  wxPGProperty *p = getPropertyFromField( f, property_name );

  if(p) {
    if( f->getAccessType() != Field::INPUT_ONLY ) {
      PropertyUpdater *updater = new PropertyUpdater(p, default_value, default_value_size );
      f->route( updater );
      
      property_update_fields.push_back( updater );
    }


    FieldValuesGrid->Append( p );
    FieldValuesGrid->GetEditorTextCtrl();
    FieldValuesGrid->GetEditorControl();
  }
}


// -----------------------------------------------------------------------
// wxVectorProperty
// -----------------------------------------------------------------------

// See propgridsample.h for H3DwxVector class

WX_PG_IMPLEMENT_VARIANT_DATA_DUMMY_EQ(H3DwxVector)

WX_PG_IMPLEMENT_PROPERTY_CLASS(wxVectorProperty,wxPGProperty,
                               H3DwxVector,const H3DwxVector&,TextCtrl)


wxVectorProperty::wxVectorProperty( const wxString& label,
                                    const wxString& name,
                                    const wxArrayString &element_labels,
                                    const H3DwxVector& value,
                                    bool use_spin_ctrl)
: wxPGProperty(label,name) {
  SetValue( WXVARIANT(value) );

  assert( value.nrValues() == element_labels.GetCount() );
  
  for( unsigned int i = 0; i < value.nrValues(); ++i ) {
    wxPGProperty *property =  new wxFloatProperty( element_labels.Item( i ) ,wxPG_LABEL );
    if( use_spin_ctrl ) {
      property->SetEditor( "SpinCtrl" );
      property->SetAttribute( wxPG_ATTR_SPINCTRL_STEP, WXVARIANT( 0.01 ) );
    }
    AddPrivateChild( property );
  }
}

wxVectorProperty::~wxVectorProperty() { }

void wxVectorProperty::RefreshChildren()
{
  if ( !GetChildCount() ) return;
  const H3DwxVector& vector = H3DwxVectorRefFromVariant(m_value);
  for( unsigned int i = 0; i < vector.nrValues(); ++i ) {
    Item(i)->SetValue( vector.getValue( i ) );
  }
}

wxVariant wxVectorProperty::ChildChanged( wxVariant& thisValue,
                                          int childIndex,
                                          wxVariant& childValue ) const
{
    H3DwxVector vector;
    vector << thisValue;

    vector.setValue( childIndex, childValue.GetDouble() );

    wxVariant newVariant;
    newVariant << vector;
    return newVariant;
}


void H3DViewerFieldValuesPanelPropGrid::populateGridFromProgramSettings( wxPropertyGrid *FieldValuesGrid,
                                                                         AutoPtrVector< Field > &property_update_fields ) {

  FieldValuesGrid->Clear();

  FieldValuesGrid->Append( new wxPropertyCategory(wxT("Main settings"),wxPG_LABEL) );  

  typedef  map< string, vector< Scene::ProgramSetting > > SectionsMap;
  SectionsMap sections;

  // Add the main settings and collect other settings in sections map.
  for( Scene::SettingsIterator i = Scene::programSettingsBegin();
       i != Scene::programSettingsEnd(); ++i ) {
    const Scene::ProgramSetting &setting = *i;
    if( setting.section == "Main settings" ) {
      setupProperty( FieldValuesGrid, setting.field, 
                     NULL, property_update_fields, setting.name );  
    } else {
      sections[setting.section].push_back( setting );
    }
  }

  // Add all other sections
  for( SectionsMap::iterator i = sections.begin();
       i != sections.end(); ++i ) {
    FieldValuesGrid->Append( new wxPropertyCategory(wxString((*i).first.c_str(),wxConvUTF8 ),wxPG_LABEL) ); 
    for( vector< Scene::ProgramSetting >::iterator j = (*i).second.begin();
         j != (*i).second.end(); ++j ) {

      Scene::ProgramSetting &setting = *j;
      setupProperty( FieldValuesGrid, setting.field, 
                     NULL, property_update_fields, setting.name );
    }
  }
}

void H3DViewerFieldValuesPanelPropGrid::settingsCallback( Scene::ProgramSettingsEvent event,
                                                          const Scene::ProgramSetting &setting,
                                                          void *data ) {
  H3DViewerFieldValuesPanelPropGrid *panel = 
    static_cast< H3DViewerFieldValuesPanelPropGrid * >( data );
  if( panel->mode == 1 && panel->IsShown() )
    panel->displayFieldsFromProgramSettings();
 /* if( event == Scene::ADD_SETTING ) {
    Console(4) << "Added:" << setting.name << endl; 
  } else {
    Console(4) << "Removed: " << setting.name << endl; 
  }*/
}

#endif // HAVE_WXPROPGRID
