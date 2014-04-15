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
/// \file NurbsCurve.cpp
/// \brief CPP file for NurbsCurve, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/NurbsCurve.h>
#include <H3D/Coordinate.h>


using namespace H3D;

H3DNodeDatabase NurbsCurve::database( 
                                     "NurbsCurve", 
                                     &(newInstance<NurbsCurve>),
                                     typeid( NurbsCurve ),
                                     &X3DParametricGeometryNode::database );

namespace NurbsCurveInternals {
  FIELDDB_ELEMENT( NurbsCurve, tessellation, INPUT_OUTPUT );
  FIELDDB_ELEMENT( NurbsCurve, weight, INPUT_OUTPUT );
  FIELDDB_ELEMENT( NurbsCurve, knot, INPUT_OUTPUT );
  FIELDDB_ELEMENT( NurbsCurve, order, INPUT_OUTPUT );
  FIELDDB_ELEMENT( NurbsCurve, closed, INPUT_OUTPUT );
  FIELDDB_ELEMENT( NurbsCurve, controlPoint, INPUT_OUTPUT );
}

NurbsCurve::NurbsCurve(  Inst< SFNode >  _metadata,
                       Inst< SFBound           > _bound,
                       Inst< DisplayList       > _displayList,
                       Inst< MFBool            > _isTouched,
                       Inst< MFVec3f           > _force,
                       Inst< MFVec3f           > _contactPoint,
                       Inst< MFVec3f           > _contactNormal,
                       Inst< SFCoordinateNode  > _controlPoint,
                       Inst< SFInt32           > _tessellation,
                       Inst< MFDouble          > _weight,
                       Inst< MFDouble          > _knot,
                       Inst< SFInt32           > _order,
                       Inst< SFBool             > _closed):
X3DParametricGeometryNode( _metadata, _bound, _displayList,
                          _isTouched,_force, _contactPoint, _contactNormal),
                          tessellation( _tessellation ),
                          weight( _weight ),
                          knot( _knot ),
                          order( _order ),
                          closed( _closed ),
                          controlPoint (_controlPoint ),
                          theNurb( NULL ),
                          printWarning( new Field ) {

  type_name = "NurbsCurve";
  database.initFields( this );
  displayList->setOwner( this );

  tessellation->setValue( 0 );
  order->setValue( 3 );
  closed->setValue( false );
  controlPoint->route(bound);

  tessellation->route ( displayList );
  weight->route ( displayList );
  knot->route ( displayList );
  order->route ( displayList );
  closed->route ( displayList );
  controlPoint->route ( displayList );

  tessellation->route( printWarning );
  weight->route( printWarning );
  knot->route( printWarning );
  order->route( printWarning );
  closed->route( printWarning );
  controlPoint->route( printWarning );
}


void NurbsCurve::SFBound::update() {
  X3DCoordinateNode *coord_node =  (static_cast< TypedSFNode
    < X3DCoordinateNode> * >( routes_in[0] )->getValue() );

  Coordinate *c = 
    dynamic_cast< Coordinate * >(coord_node);

  BoxBound *bound = new BoxBound();
  if ( c ){
    bound->fitAroundPoints( c->point->begin(), c->point->end() );
    value = bound;
  }
}

void NurbsCurve::render( ) {
  bool valid_order = true;
  bool valid_weight = true;
  bool valid_controlPoints = true;

  X3DCoordinateNode *coord_node = controlPoint->getValue();

  H3DInt32 local_order = order->getValue();
  H3DInt32 no_of_control_points =coord_node->nrAvailableCoords();
  H3DInt32 no_of_knots = (no_of_control_points + local_order-1);

  H3DInt32 no_of_weights = (H3DInt32)weight->getValue().size();
  GLfloat *weight_float = new GLfloat [no_of_control_points];

  // the knots values are doubles from the X3D-specification, but the
  // gluNurbsCurve can only take a float pointer to the knots values.
  // Therefore, a float pointer is created and then a new array of the
  // knots values converted to floats.
  const vector< H3DDouble > &knots_double = knot->getValue();
  GLfloat *knots_float = new GLfloat [no_of_knots];

  // check the knot-vectors. If they are not according to standard
  // default uniform knot vectors will be generated.
  bool generate_uniform = true;
  vector< H3DFloat > knots;
  if(knots_double.size() == (unsigned int) no_of_knots){
    for ( int i = 0; i<no_of_knots; ++i){
      knots_float[i]= (GLfloat)knots_double[i];
    }
    generate_uniform = false;
    H3DInt32 consecutiveKnots = 0;
    for( int i = 0; i < no_of_knots; ++i ) {
      knots.push_back(knots_float[i]);
      if( i > 0 ) {
        if( knots[i] == knots[ i - 1 ] ){
          ++consecutiveKnots;
          if( consecutiveKnots > (local_order - 1) ){
            if (!generate_uniform) generate_uniform = true;
            if( !printWarning->isUpToDate() ) {
              Console(3) << "Warning: There are more consecutive knots than"
                << " the order -1 in " << getTypeName() << " node( " << getName()
                << "). Default knots are calculated." << endl;
                printWarning->upToDate();
            }
          }
          if( knots[ i - 1 ] > knots[i] ){
            if (!generate_uniform) generate_uniform = true;
            if( !printWarning->isUpToDate() ) {
              Console(3) << "Warning: Change the order of the values of knots so "
                << "they are non-decreasing in " << getTypeName() << " node( "
                << getName() << ") or default knot values are used." << endl;
              printWarning->upToDate();
            }
          }
        }
        else{
          consecutiveKnots = 0;
        }
      }
    }
  }
  else if(generate_uniform || (knots_double.size() != (unsigned int)no_of_knots )){
    for( int i = 0; i < no_of_knots; ++i ){
      knots_float[i] = (GLfloat)( (H3DDouble)i / ( no_of_knots - 1 ) );
    }
  }

  // If the number of weight value is not identical to the number of
  // ControlPoints all weight values shall be ignored and a value of 
  // 1.0 shall be used.
  if(no_of_weights == no_of_control_points){
    const vector< H3DDouble > &w = weight->getValue();
    if(!valid_weight) valid_weight = true;
    for (unsigned int j = 0; j<w.size(); ++j){
      weight_float[j]= (GLfloat)w[j];
    }
  } else {
    if( !printWarning->isUpToDate() ) {
      Console(3) << "Warning: The number of weight values is not the same as "
        << "the number of control points. Default weight 1.0 is assumed." << endl;
      printWarning->upToDate();
    }
    if( valid_weight ) valid_weight = false;
  }


  // Order has to be 2 or greater to define the nurbsCurve
  if( local_order < 2 ) {
    if( !printWarning->isUpToDate() ) {
      Console(3) << "Warning: order is less than 2 in " << getTypeName()
        << " node( "  << getName() 
        << "). Node will not be rendered. " << endl;
      printWarning->upToDate();
    }
    if (valid_order) valid_order=false;
    return;
  }

  // the number of controlPoints should be equal to 
  // or greater than the order
  if( no_of_control_points < local_order )  {
    if( !printWarning->isUpToDate() ) {
      Console(3) << "Warning: The size of controlPoint does not match "
        << "order in " << getTypeName() << " node( "
        << getName() << "). Node will not be rendered. " << endl;
      printWarning->upToDate();
    }
    if (valid_controlPoints) valid_controlPoints = false;
    return;
  }

  // create a new NurbsRendererer
  if( !theNurb ){
    theNurb = gluNewNurbsRenderer();
  }

  // setting the gluNurbsProperty after the value of tessellation   
  H3DInt32 tess_val = tessellation->getValue();
  GLfloat tess_step;

  if(tess_val>0){
    tess_step= (GLfloat)(tess_val+1);
    gluNurbsProperty(theNurb, GLU_U_STEP , tess_step);
  }
  else if(tess_val<0){
    tess_step = (GLfloat)(((-tess_val)*no_of_control_points)+1);
    gluNurbsProperty(theNurb, GLU_U_STEP , tess_step);

  }
  else if(tess_val==0){
    tess_step = (GLfloat)((2*no_of_control_points)+1);
    gluNurbsProperty(theNurb, GLU_U_STEP , tess_step);
  }

  // gluNurbsProperty definition
  //void gluNurbsProperty( GLUnurbsObj *nurbsObj, 
  // GLenum property, 
  // GLfloat value );

  gluNurbsProperty(theNurb, GLU_SAMPLING_METHOD, GLU_DOMAIN_DISTANCE);

  if(valid_controlPoints && valid_order){
    if(coord_node){
      GLfloat *control_point_f = new GLfloat [no_of_control_points*4];
      for ( int i = 0; i<no_of_control_points; ++i){
        Vec3f p = coord_node->getCoord(i);
        control_point_f[i*4]= p.x;
        control_point_f[i*4+1]= p.y;
        control_point_f[i*4+2]= p.z;
        if (valid_weight){
          control_point_f[i*4+3]= weight_float[i];
        }
        else{
          control_point_f[i*4+3]= 1;}
      }
      gluBeginCurve(theNurb);

      gluNurbsCurve(theNurb, 
        no_of_knots, 
        knots_float,
        4,
        control_point_f,
        local_order,
        GL_MAP1_VERTEX_4
        );

      gluEndCurve( theNurb);
      delete [] control_point_f;
    }
  }
  delete [] weight_float;
  delete [] knots_float;
}

