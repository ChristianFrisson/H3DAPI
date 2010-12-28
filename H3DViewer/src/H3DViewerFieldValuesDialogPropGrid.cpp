#include "H3DViewerFieldValuesDialogPropGrid.h"

#ifdef HAVE_WXPROPGRID

#include <H3D/Group.h>
#include <H3D/SFString.h>
#include <H3D/SFInt32.h>
#include <H3D/SFColor.h>
#include <H3D/PythonScript.h>

using namespace H3D;

H3DViewerFieldValuesDialogPropGrid::H3DViewerFieldValuesDialogPropGrid( 
             wxWindow* parent, 
	     wxWindowID id, 
	     const wxString& title, 
	     const wxPoint& pos, 
	     const wxSize& size, 
	     long style ) : 
  wxFrame( parent, id, title, pos, size, style ) {

  this->SetSizeHints( wxDefaultSize, wxDefaultSize );
  
  wxBoxSizer* bSizer2;
  bSizer2 = new wxBoxSizer( wxVERTICAL );
  
  prop_grid_panel = new H3DViewerFieldValuesPanelPropGrid( this, wxID_ANY, wxDefaultPosition, wxDefaultSize,  wxPG_BOLD_MODIFIED );
	
  bSizer2->Add( prop_grid_panel, 1, wxALL|wxEXPAND, 5 );
  this->SetSizer( bSizer2 );
  this->Layout();
	
  // Connect Events
  this->Connect( wxEVT_IDLE, wxIdleEventHandler( H3DViewerFieldValuesPanelPropGrid::OnIdle ), NULL, prop_grid_panel );
}

H3DViewerFieldValuesDialogPropGrid::~H3DViewerFieldValuesDialogPropGrid()
{
  // Disconnect Events
  this->Disconnect( wxEVT_IDLE, wxIdleEventHandler( H3DViewerFieldValuesPanelPropGrid::OnIdle ), NULL, prop_grid_panel );
}

void H3DViewerFieldValuesDialogPropGrid::displayFieldsFromNode( Node *n ) {
  if( !n ) {
    SetTitle(wxT(""));
  } else {
    SetTitle( wxString(n->getTypeName().c_str(),wxConvUTF8) );
  }
  prop_grid_panel->displayFieldsFromNode( n );
}

H3DViewerFieldValuesPanelPropGrid::H3DViewerFieldValuesPanelPropGrid( wxWindow* parent, 
                                      wxWindowID id, 
                                      const wxPoint& pos, 
                                      const wxSize& size, 
                                      long style ) : 
  wxPanel( parent, id, pos, size, style ),
  displayed_node( NULL ) {

  this->SetSizeHints( wxDefaultSize, wxDefaultSize );
  
  wxBoxSizer* bSizer2;
  bSizer2 = new wxBoxSizer( wxVERTICAL );
  
  FieldValuesGrid = new wxPropertyGrid( this, wxID_ANY, wxDefaultPosition, wxDefaultSize,  wxPG_SPLITTER_AUTO_CENTER | wxPG_BOLD_MODIFIED );
  
  bSizer2->Add( FieldValuesGrid, 1, wxALL|wxEXPAND, 5 );
  this->SetSizer( bSizer2 );
  this->Layout();
  
  // Connect Events
  FieldValuesGrid->Connect( wxEVT_PG_CHANGED, wxPropertyGridEventHandler( H3DViewerFieldValuesPanelPropGrid::OnPropertyChanged ), NULL, this );
}

H3DViewerFieldValuesPanelPropGrid::~H3DViewerFieldValuesPanelPropGrid()
{
  // Disconnect Events
  FieldValuesGrid->Disconnect( wxEVT_PG_CHANGED, wxPropertyGridEventHandler( H3DViewerFieldValuesPanelPropGrid::OnPropertyChanged ), NULL, this );
}

void H3DViewerFieldValuesPanelPropGrid::displayFieldsFromNode( Node *n ) {
  bool new_node = n != displayed_node.get();
  displayed_node.reset( n );
  if( !n ) {
#ifdef DEFAULT_VALUES
    default_values_node.reset( NULL );
#endif
    property_update_fields.clear();
    FieldValuesGrid->Clear();
    return;
  }

  H3DNodeDatabase *db = H3DNodeDatabase::lookupTypeId( typeid( *n ) );

#ifdef DEFAULT_VALUES
  if( new_node ) {
    default_values_node.reset( db->createNode() );
  }
#endif

  if( new_node ) {
    property_update_fields.clear();
    populateGridFromNode( FieldValuesGrid, n, property_update_fields );
  }
}


void H3DViewerFieldValuesPanelPropGrid::OnIdle( wxIdleEvent& event ) {
  TimeStamp now;
  if( now - last_fields_update > 0.1 ) {
    displayFieldsFromNode( displayed_node.get() );
    last_fields_update = now;
    for( unsigned int i = 0; i < property_update_fields.size(); i++ ) {
      property_update_fields[i]->upToDate();
    }
  }
  TimeStamp after;
  //Console(4) << after - now << endl;
}

void H3DViewerFieldValuesPanelPropGrid::OnPropertyChanged( wxPropertyGridEvent& event ) {
  if( displayed_node.get() ) {
    wxString name = event.GetPropertyName();
    wxPGProperty *property = event.GetProperty();
    
    Field *f = displayed_node->getField( string(name.mb_str()) );

    wxVariant value = property->GetValue();

    X3DTypes::X3DType x3d_type = f->getX3DType();
    
    if( x3d_type == X3DTypes::SFBOOL ) {    
      bool b = value.GetBool();
      static_cast< SFBool * >( f )->setValue( b );
    } else {
      if( ParsableField *pf = dynamic_cast< ParsableField * >( f ) ) {
        string s( value.GetString().mb_str() );
        try {
          pf->setValueFromString( s );
        } catch(...) {
          
        }
      }
    }
  }
}
  

void H3DViewerFieldValuesPanelPropGrid::populateGridFromNode( wxPropertyGrid *FieldValuesGrid,
                                                               Node *n,
                                                               AutoPtrVector< Field > &property_update_fields ) {

  FieldValuesGrid->Clear();

  H3DNodeDatabase *db = H3DNodeDatabase::lookupTypeId( typeid( *n ) );

  list< FieldDBElement * > init_only_fields;
  list< FieldDBElement * > input_only_fields;
  list< FieldDBElement * > output_only_fields;
  list< FieldDBElement * > input_output_fields;

  for( H3DNodeDatabase::FieldDBConstIterator i = db->fieldDBBegin();
       db->fieldDBEnd() != i; i++ ) {
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
       i != init_only_fields.end(); i++ ) {
    Field *f = (*i)->getField( n ); 
    wxPGProperty *p = getPropertyFromField( f );
    PropertyUpdater *updater = new PropertyUpdater(p);
    f->route( updater );
    property_update_fields.push_back( updater );
    if(p) FieldValuesGrid->Append( p );
  }

  // input output fields
  if( input_output_fields.size() > 0 ) {
    FieldValuesGrid->Append( new wxPropertyCategory(wxT("Input/output"),wxPG_LABEL) );
  }

  for( list< FieldDBElement * >::iterator i = input_output_fields.begin();
       i != input_output_fields.end(); i++ ) {
    Field *f = (*i)->getField( n ); 
    wxPGProperty *p = getPropertyFromField( f );
    PropertyUpdater *updater = new PropertyUpdater( p );
    f->route( updater );
    property_update_fields.push_back( updater );
    if( p ) FieldValuesGrid->Append( p );
  }

  
  // output only fields.
  if( output_only_fields.size() > 0 ) {
    FieldValuesGrid->Append( new wxPropertyCategory(wxT("Output only"),wxPG_LABEL) );
  }

  for( list< FieldDBElement * >::iterator i = output_only_fields.begin();
       i != output_only_fields.end(); i++ ) {
    Field *f = (*i)->getField( n ); 
    wxPGProperty *p = getPropertyFromField( f );
    PropertyUpdater *updater = new PropertyUpdater(p);
    f->route( updater );
    property_update_fields.push_back( updater );
    if( p ) FieldValuesGrid->Append( p );
  }
  
  // input only fields.
  if( input_only_fields.size() > 0 ) {
    FieldValuesGrid->Append( new wxPropertyCategory(wxT("Input only"),wxPG_LABEL) );
  }

  for( list< FieldDBElement * >::iterator i = input_only_fields.begin();
       i != input_only_fields.end(); i++ ) {
    Field *f = (*i)->getField( n ); 
    wxPGProperty *p = getPropertyFromField( f );
    PropertyUpdater *updater = new PropertyUpdater(p);
    f->route( updater );
    property_update_fields.push_back( updater );
    if( p ) FieldValuesGrid->Append( p );
  }
  
  // if PythonScript node add top-level fields.
  PythonScript *ps = dynamic_cast< PythonScript * >( n );
  if( ps ) {
    vector< pair< string, Field *> > python_fields;
    ps->getTopLevelFields( python_fields );
  
    FieldValuesGrid->Append( new wxPropertyCategory(wxT("Python fields"),wxPG_LABEL) );

    for( vector< pair< string, Field *> >::iterator i = python_fields.begin();
         i != python_fields.end(); i++ ) {
      Field *f = (*i).second;
      const string &name = (*i).first;
      if( dynamic_cast< SFNode * >( f ) || 
          dynamic_cast< MFNode * >( f ) ) continue;
      wxPGProperty *p = getPropertyFromField( f, name );
      PropertyUpdater *updater = new PropertyUpdater( p );
      f->route( updater );
      property_update_fields.push_back( updater );
      if( p ) FieldValuesGrid->Append( p );
    }
  }
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
  } else if ( x3d_type == X3DTypes::SFTIME ) {
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
      property->SetValueFromString( wxString( s.c_str(), wxConvUTF8 ) );
    }
  }
}

wxPGProperty *H3DViewerFieldValuesPanelPropGrid::getPropertyFromField( Field *f,
                                                                        const string &custom_field_name ) {
  
  
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
             i != valid_values.end(); i++ ) {
          choices.Add( wxString( (*i).c_str(), wxConvUTF8 ), index++ );
        }
        
        wxEnumProperty *p = new wxEnumProperty( field_name.c_str(), wxPG_LABEL, choices );
        return p;
      } else {
        return new wxLongStringProperty( field_name.c_str(), wxPG_LABEL );
      }
    } else if( x3d_type == X3DTypes::MFSTRING ) {
      return new wxArrayStringProperty(field_name.c_str(), wxPG_LABEL);
    } else if( x3d_type == X3DTypes::SFBOOL ) {
      return new wxBoolProperty(field_name.c_str(), wxPG_LABEL);
    } else if( x3d_type == X3DTypes::SFINT32  ) {
      return new wxIntProperty(field_name.c_str(), wxPG_LABEL);
    } else if( x3d_type == X3DTypes::SFFLOAT || x3d_type == X3DTypes::SFDOUBLE || x3d_type == X3DTypes::SFTIME  ) {
      //wxPG_FLOAT_PRECISION
      return new wxFloatProperty(field_name.c_str(), wxPG_LABEL);
    } else if( x3d_type == X3DTypes::SFCOLOR ) {
      return new wxColourProperty(field_name.c_str(), wxPG_LABEL);
    } else if( ParsableField *pfield = dynamic_cast< ParsableField * >( f ) ) {
      return new wxLongStringProperty( field_name.c_str(), wxPG_LABEL );
    }
  }

  return NULL;
}

#endif // HAVE_WXPROPGRID
