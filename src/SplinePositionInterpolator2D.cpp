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
/// \file SplinePositionInterpolator2D.cpp
/// \brief CPP file for SplinePositionInterpolator2D, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/SplinePositionInterpolator2D.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase SplinePositionInterpolator2D::database( 
  "SplinePositionInterpolator2D", 
  &(newInstance<SplinePositionInterpolator2D>),
  typeid( SplinePositionInterpolator2D ),
  &X3DInterpolatorNode::database
  );

namespace SplinePositionInterpolator2DInternals {
  FIELDDB_ELEMENT( SplinePositionInterpolator2D, keyValue, INPUT_OUTPUT );
  FIELDDB_ELEMENT( SplinePositionInterpolator2D, value_changed, OUTPUT_ONLY );
  FIELDDB_ELEMENT( SplinePositionInterpolator2D, keyVelocity, INPUT_OUTPUT );
  FIELDDB_ELEMENT( SplinePositionInterpolator2D, closed, INPUT_OUTPUT );
  FIELDDB_ELEMENT( SplinePositionInterpolator2D, normalizeVelocity, INPUT_OUTPUT );

}


SplinePositionInterpolator2D::SplinePositionInterpolator2D( 
  Inst< SFNode  >  _metadata,
  Inst< SFFloat >  _set_fraction,
  Inst< MFFloat >  _key,
  Inst< MFVec2f >  _keyValue,
  Inst< MFVec2f >  _keyVelocity,
  Inst< SFValue >  _value_changed,
  Inst< SFBool  >  _closed,
  Inst< SFBool  >  _normalizeVelocity) :
X3DInterpolatorNode( _metadata, _set_fraction, _key ),
keyValue     ( _keyValue      ),
keyVelocity( _keyVelocity), 
closed( _closed), 
normalizeVelocity( _normalizeVelocity),
value_changed( _value_changed ) { 

  type_name = "SplinePositionInterpolator2D";
  database.initFields( this );

  set_fraction->route( value_changed, id );
  key->route( value_changed, id );
  keyValue->route( value_changed, id );  
  keyVelocity->route( value_changed, id );
}

void SplinePositionInterpolator2D::SFValue::update() {
  SplinePositionInterpolator2D *interpolator = 
    static_cast<SplinePositionInterpolator2D*>( getOwner() );

  // Routing
  // Fraction
  H3DFloat fraction = 
    static_cast<SFFloat*>(routes_in[0])->getValue( interpolator->id );
  //keySize
  int key_size = static_cast<MFFloat*>(routes_in[1])->size();
  //keyValues (vector)
  const vector< Vec2f > &key_value = 
    static_cast<MFVec2f*>(routes_in[2])->getValue();
  //keyVelocity  
  const vector< Vec2f > &key_velocity = 
    static_cast<MFVec2f*>(routes_in[3])->getValue();
  H3DFloat weight;

  // Return the lowest of the index i the value is between plus the weight
  int key_index = interpolator->lookupKey( fraction, 
    weight );


  bool closed=interpolator->closed->getValue();
  bool normalizeVelocity=interpolator->normalizeVelocity->getValue();
  vector< H3DFloat > key=interpolator->key->getValue();
  
  vector< Vec2f >  T;
  Vec2f T0;
  Vec2f T1;
  vector< H3DFloat > F1;
  vector< H3DFloat > F2;


  if( key_index >= 0 && (key_index + 1) < (int)key_value.size() ) {

    int tMinus1 = (key_size-2);
    int tN = 1;
    H3DFloat D =0.0;
    bool  specified = false;
    bool  notSpecified = false;
    bool  ignoreClosed = false;

    //if the first and last value in keyValue aren't the same, ignore closed
    if(( (key_value[0]- key_value[(key_size-1)]).lengthSqr() ) > Constants::f_epsilon){
      ignoreClosed = true;
    }
    //Calculate  D = distance from keyValue[0]->keyValue[N-1]
    for(int i=0; i<key_size-1; ++i){ //n=keys.size()
      D = D + ((key_value[i]-key_value[i+1]).length());
    }

    // Calculate T for the different conditions
    if(key_velocity.size() == key_value.size())
    {
      if(!normalizeVelocity){
        T=key_velocity;
      }
      else if(normalizeVelocity){  
        Vec2f a;
        for(int i=0; i<key_size; ++i){
          a = key_velocity[i];
          T.push_back(key_velocity[i]*(D/a.length()) );
        }
      }
      specified = true;
    }


    else if(closed && !ignoreClosed){
      for(int i=0; i<key_size; ++i){
        if(i==0){
          T.push_back((key_value[i+1]-key_value[tMinus1])/2);
        }
        else if(i==(key_size-1)){
          T.push_back((key_value[tN]-key_value[i-1])/2);
        }
        else {
          T.push_back((key_value[i+1]-key_value[i-1])/2);
        }
      }
    }

    else if(!closed || ignoreClosed){
      T.push_back(Vec2f(0,0)); 
      for(int i=1; i<key_size-1; ++i){
        T.push_back((key_value[i+1]-key_value[i-1])/2);
      }
      notSpecified=true;
    }

    if(key_velocity.size() ==2){
      if(!normalizeVelocity){
        T[0] =key_velocity[0];
        if(notSpecified){
          T.push_back(key_velocity[1]); 
        }
        else{
          T[key_size-1] = key_velocity[1]; 
        }
      }
      else if(normalizeVelocity){
        Vec2f a= key_velocity[0];
        T[0]=(key_velocity[0]*(D/(a.length() )));
        if(notSpecified){
          Vec2f b = key_velocity[1];
          T.push_back(key_velocity[1]*(D/b.length() ));
        }
        else{
          Vec2f b=key_velocity[1];
          T[(key_size -1)] = (key_velocity[1]*(D/b.length() ));
        }
      } 
      specified = true;
    } // Calculations for T are ready

    //Calculate F1, F2, T0, T1
    if (!closed || specified || ignoreClosed){
      for(int i=1; i<key_size-1; ++i){
        F1.push_back(2*((key[i] - key[i-1])/(key[i+1] - key[i-1])));
        F2.push_back(2*((key[i+1] - key[i])/(key[i+1] - key[i-1])));
      }
      if((key_index>0) && (key_index <key_size-2)){
        T0 = (T[key_index]*F1[key_index-1]); 
        T1 = (F2[key_index]*T[key_index+1]);
      }
      else if(specified && (key_index ==0)){
        T0=T[0];
        T1 =(F2[key_index]*T[key_index+1]);       
      }
      else if(notSpecified && (key_index ==0)){
        T0=Vec2f(0,0);
        T1 = (F2[key_index]*T[key_index+1]);
      }
      else if(specified && (key_index ==(key_size-2))){
        T0 = key_value[key_size-1];
        T1 = T[key_size-1];
      }
      else if(notSpecified &&(key_index ==(key_size-1))){
        T0=(F1[key_index-1]*T[key_index]);
        T1=Vec2f(0,0);
      }
    }

    else if(closed && !ignoreClosed){
      for(int i=0; i<key_size; ++i){
        if(i==0){
          F1.push_back(2*((key[i] - key[tMinus1])/(key[i+1] - key[tMinus1])));
          F2.push_back(2*((key[i+1] - key[i])/(key[i+1] - key[tMinus1])));
        }
        else if(i==(key_size-1)){
          F1.push_back((key[i] - key[i-1])/(key[tN] - key[i-1]));
          F2.push_back(2*((key[tN] - key[i])/(key[tN] - key[i-1])));
        }
        else{
          F1.push_back(2*((key[i] - key[i-1])/(key[i+1] - key[i-1])));
          F2.push_back(2*((key[i+1] - key[i])/(key[i+1] - key[i-1])));
        }
      }
      T0 = (F1[key_index]*T[key_index]);
      T1 = (F2[key_index+1]*T[key_index +1]); 
    }

   vector < Vec2f > C;
   C.push_back(key_value[key_index]);
   C.push_back(key_value[key_index+1]);
   C.push_back(T0);
   C.push_back(T1);
    Vec4f S = Vec4f(H3DPow(weight,3), H3DPow(weight,2), weight, 1);
                  
    Matrix4f H = Matrix4f(2,-2,1,1, -3,3,-2,-1, 0,0,1,0, 1,0,0,0);
             
    Vec2f HC0 = Vec2f((H[0][0]*C[0][0] + H[0][1]*C[1][0] + H[0][2]*C[2][0] + H[0][3]*C[3][0]), (H[0][0]*C[0][1] + H[0][1]*C[1][1] + H[0][2]*C[2][1] + H[0][3]*C[3][1]));
    Vec2f HC1 = Vec2f((H[1][0]*C[0][0] + H[1][1]*C[1][0] + H[1][2]*C[2][0] + H[1][3]*C[3][0]), (H[1][0]*C[0][1] + H[1][1]*C[1][1] + H[1][2]*C[2][1] + H[1][3]*C[3][1]));
    Vec2f HC2 = Vec2f((H[2][0]*C[0][0] + H[2][1]*C[1][0] + H[2][2]*C[2][0] + H[2][3]*C[3][0]), (H[2][0]*C[0][1] + H[2][1]*C[1][1] + H[2][2]*C[2][1] + H[2][3]*C[3][1]));
    Vec2f HC3 = Vec2f((H[3][0]*C[0][0] + H[3][1]*C[1][0] + H[3][2]*C[2][0] + H[3][3]*C[3][0]), (H[3][0]*C[0][1] + H[3][1]*C[1][1] + H[3][2]*C[2][1] + H[3][3]*C[3][1]));
    
    vector < Vec2f > HC;
    HC.push_back(HC0);
    HC.push_back(HC1);
    HC.push_back(HC2);
    HC.push_back(HC3);

    H3DFloat HCS0 = (HC[0][0]*S[0]+ HC[1][0]*S[1] + HC[2][0]*S[2] + HC[3][0]*S[3]);
    H3DFloat HCS1 = (HC[0][1]*S[0]+ HC[1][1]*S[1] + HC[2][1]*S[2] + HC[3][1]*S[3]);

    value = Vec2f(HCS0,HCS1);
  
  }
}

