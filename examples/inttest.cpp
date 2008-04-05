/****************************************************************************/
/*                                                                          */
/* File:      inttest.cpp                                                   */
/*                                                                          */
/* Purpose:   routines to test integration.h                                */
/*                                                                          */
/* Author:    Roland Schulte                                                */
/*            Interdisziplinaeres Zentrum fuer Wissenschaftliches Rechnen   */
/*            Universitaet Heidelberg                                       */
/*            Im Neuenheimer Feld 368                                       */
/*            D-69120 Heidelberg                                            */
/*                                                                          */
/* EMail:     roland.schulte@iwr.uni-heidelberg.de                          */
/*                                                                          */
/* History:   29.10.02 begin                                                */
/*                                                                          */
/* Remarks:                                                                 */
/*                                                                          */
/****************************************************************************/
#include <iostream>
#include "../datalib/vector.h"
#include "../datalib/datacube.h"
#include "../mathlib/integration.h"
#include "../mathlib/integrant.h"
#include "../mathlib/momentcalc.h"
#include "../errorcode.h"

int main(int argc, char **argv)
{
  DataCube dc(2); // datacube of dimension 2

  Vector<int> size(2); 
  size[1] = 7; // size of datacube in x-direction
  size[2] = 7; // size of datacube in y-direction

  Vector<double> spaceing(2);
  spaceing[1]=1.0;
  spaceing[2]=1.0;

  dc.SetCubeSizeAndSpaceing(size,spaceing); 
  dc.MallocCube();

  // define the dataarray
  for ( int i = 0; i < size[1]; i++ ){
    for ( int j = 0; j < size[2]; j++ ){

      if ( ( i == 2 ) && ( j > 1 ) )
	dc(i,j) = 1.0;

    } 
  }

  // test the integration

  //define volume
  Volume vol_circle(1, 3, 3, 3, 3, spaceing[1], spaceing[2]);
  Volume vol_cube(0, 0, 0, 7, 7, spaceing[1], spaceing[2]);

  // calulate mass
  MassIntegrant mass;
  Integration i_t(&dc,&mass);

  double mass_res = 0.0; 
  int code = i_t.execute(mass_res, vol_circle );
  if ( code ) cout << errorCode(code);

  cout << "mass: " << mass_res << endl;

  // calculate focal point

  Focal_point_x focal_point_x;
  i_t.SetIntegrant(&focal_point_x);

  double fp_x = 0.0;
  code = i_t.execute(fp_x, vol_circle );
  if ( code ) cout << errorCode(code);

  fp_x /= mass_res;

  Focal_point_y focal_point_y; 
  i_t.SetIntegrant(&focal_point_y);

  double fp_y = 0.0;
  code = i_t.execute(fp_y, vol_circle );
  if ( code ) cout << errorCode(code);


  fp_y /= mass_res;

  cout << "focal point: ( " << fp_x << ", " << fp_y << " )" << endl;

}




