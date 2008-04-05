/****************************************************************************/
/*                                                                          */
/* File:      inttest3d.cpp                                                 */
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
/* History:   05.11.02 begin                                                */
/*                                                                          */
/* Remarks:                                                                 */
/*                                                                          */
/****************************************************************************/
#include <iostream>
#include "../mathlib/vector.h"
#include "../datalib/datacube.h"
#include "../mathlib/integration.h"
#include "../mathlib/integrant.h"
#include "../mathlib/momentcalc.h"
#include "../errorcode.h"


int main(int argc, char **argv)
{
 
  DataCube dc(3); // datacube of dimension 3

  Vector<int> size(3); 
  size[1] = 7; // range of datacube in x-direction
  size[2] = 7; // range of datacube in y-direction
  size[3] = 7; // range of datacube in z-direction

  Vector<double> spaceing(3);
  spaceing[1]=1.0;
  spaceing[2]=1.0;
  spaceing[3]=1.0;

  dc.SetCubeSizeAndSpaceing(size,spaceing); 
  dc.MallocCube();

  // define the dataarray
  for ( int i = 0; i < size[1]; i++ ){
    for ( int j = 0; j < size[2]; j++ ){
      for ( int l = 0; l < size[3]; l++ ){

	if ( ( i == 2 ) && ( l == 2 ) )
	dc(i,j,l) = 1.0;

      }
    } 
  }

  // test the integration

  //define volume
  Volume vol_cube(0, 0, 0, 0, 7, 7, 7, spaceing[1], spaceing[2], spaceing[3]);
  Volume vol_sphere(1, 3, 3, 3, 3, 3, 3, spaceing[1], spaceing[2], spaceing[3]);

  // calulate mass
  MassIntegrant mass;
  Integration m(&dc,&mass);

  double mass_res = 0.0;
  int code = m.execute(mass_res, vol_sphere );
  if ( code ) cout << errorCode(code);

  cout << "mass: " << mass_res << endl;

 
}




