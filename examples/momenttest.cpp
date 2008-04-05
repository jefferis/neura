/****************************************************************************/
/*                                                                          */
/* File:      momenttest.cpp                                                */
/*                                                                          */
/* Purpose:   routines to test moments.h                                    */
/*                                                                          */
/* Author:    Roland Schulte                                                */
/*            Interdisziplinaeres Zentrum fuer Wissenschaftliches Rechnen   */
/*            Universitaet Heidelberg                                       */
/*            Im Neuenheimer Feld 368                                       */
/*            D-69120 Heidelberg                                            */
/*                                                                          */
/* EMail:     roland.schulte@iwr.uni-heidelberg.de                          */
/*                                                                          */
/* History:   04.11.02 begin                                                */
/*                                                                          */
/* Remarks:                                                                 */
/*                                                                          */
/****************************************************************************/
#include <iostream>
#include "../datalib/vector.h"
#include "../datalib/datacube.h"
#include "../mathlib/integration.h"
#include "../mathlib/moments.h"
#include "../errorcode.h"

int main(int argc, char **argv)
{
  
  DataCube dc(2); // datacube of dimension 2

  Vector<int> size(2); 
  size[1] = 5; // range of datacube in x-direction
  size[2] = 5; // range of datacube in y-direction

  Vector<double> spaceing(2);
  spaceing[1]=1.0;
  spaceing[2]=1.0;

  dc.SetCubeSizeAndSpaceing(size,spaceing); 
  dc.MallocCube();

  // define the dataarray
  for ( int i = 0; i < size[1]; i++ ){
    for ( int j = 0; j < size[2]; j++ ){

      if (  ( i == 2 ) || ( ( i > 1 ) && ( j == 2 ) ) )
	dc(i,j) = 1.0;

    } 
  }

  // test the calculation of moments

  //define volume
  Volume vol(0, 0, 0, 5, 5, spaceing[1], spaceing[2]);

  Moments mom(&dc);

  Vector<double> fp(2);
  int code = mom.focal_point( fp, vol );
  if ( code ) cout << errorCode(code);

  cout << "focal point: ( " << fp[1] << " " << fp[2] << " )" << endl;

  // caluclulate inertian tensor
  Vector<double> i_t(4);
  code = mom.inertian_tensor(i_t, vol );
  if ( code ) cout << errorCode(code);

  cout << "inertian tensor: ( " << i_t[1] << " " << i_t[2] << " )" << endl;
  cout << "                 ( " << i_t[3] << " " << i_t[4] << " )" << endl;

   // calculate inertian values

  Vector<double> ew(2);
  Vector<double> ev(4);
  code = mom.inertian_values(ew,ev,vol);
  if ( code ) cout << errorCode(code);

  cout << "eigenvektor: ( " << ev[1] << " " << ev[2] << " )" << endl;
  cout << "dazu eigenwert: " << ew[1] << endl;
  cout << "eigenvektor: ( " << ev[3] << " " << ev[4] << " )" << endl;
  cout << "dazu eigenwert: " << ew[2] << endl;
 
}




