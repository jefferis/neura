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
/* History:   06.11.02 begin                                                */
/*                                                                          */
/* Remarks:                                                                 */
/*                                                                          */
/****************************************************************************/
#include <iostream>
#include "../mathlib/vector.h"
#include "../datalib/datacube.h"
#include "../mathlib/integration.h"
#include "../mathlib/moments.h"
#include "../mathlib/momentcalc.h"
#include "../errorcode.h"

int main(int argc, char **argv)
{

  DataCube dc(3); // datacube of dimension 2
  Vector<int> size(3); 
  size[1]= 33; // range of datacube in x-direction
  size[2]= 33; // range of datacube in y-direction
  size[3]= 33; // range of datacube in z-direction
  Vector<double> spaceing(3);
  spaceing[1]=1.0;
  spaceing[2]=1.0;
  spaceing[3]=1.0;

  dc.SetCubeSizeAndSpaceing(size,spaceing); 
  dc.MallocCube();

  // define the dataarray
  for ( int k = 0; k < size[3]; k++ ){
    for ( int j = 0; j < size[2]; j++ ){
      for ( int i = 0; i < size[1]; i++ ){

	 if (  
	     ( j == 16 ) 
	     && 
	        ( k == i )
	     && ( ( i < 16 ) || ( i > 18 ) )
	     
	     )
	dc(i,j,k) = 1.0;
	 else dc(i,j,k) = 0.0;
	
      } 
    } 
  }

 // test the calculation of moments

  Moments mom(&dc);
 
  int intsize = 5;
  //define volume
  Volume vol(1, 3, intsize,intsize,intsize );
  cout << "Integration auf Kugel" << endl;
  cout << "Radius = " << intsize << endl;

  int code;

 //  // calculate mass 

 //  cout << "calculate mass" << endl;

//   MassIntegrant mass;
//   Integration m(&dc,&mass);

//   double mass_res = 0.0;
//   code = m.execute(mass_res, vol );
//   if ( code ) cout << errorCode(code);

//   cout << "mass: " << mass_res << endl;

//   //calculate focal point

//   cout << "calculate focal point" << endl;

//   Vector<double> fp(3);
 
//   code = mom.focal_point(fp, vol );
//   if ( code ) cout << errorCode(code);

//   cout << "focal point: ( " << fp[1] << ", " << fp[2] << ", " << fp[3] <<" )" << endl;


// //   // caluclulate inertian tensor

//   cout << "calculate inertian tensor" << endl;

//   Vector<double> i_t(9);
//   code = mom.inertian_tensor(i_t, vol );
//   if ( code ) cout << errorCode(code);

//   cout << "inertian tensor: ( " << i_t[1] << " " << i_t[2] << " " << i_t[3] << " )" << endl;
//   cout << "                 ( " << i_t[4] << " " << i_t[5] << " " << i_t[6] << " )" << endl;
//   cout << "                 ( " << i_t[7] << " " << i_t[8] << " " << i_t[9] << " )" << endl;


//   // calculate inertian values

//   cout << "calculate inertian values" << endl;

//   Vector<double> ew(3);
//   Vector<double> ev(9);

//   code = mom.inertian_values(ew,ev,vol);
//   if ( code ) cout << errorCode(code);

//   cout << "Traegheitswerte am Punkt (3,3,3): " << endl;

//   cout << "eigenvektor: ( " << ev[1] << " " << ev[2] << " " << ev[3] << " )" << endl;
//   cout << "dazu eigenwert: " << ew[1] << endl;
//   cout << "eigenvektor: ( " << ev[4] << " " << ev[5] << " " << ev[6] << " )" << endl;
//   cout << "dazu eigenwert: " << ew[2] << endl;
//   cout << "eigenvektor: ( " << ev[7] << " " << ev[8] << " " << ev[9] << " )" << endl;
//   cout << "dazu eigenwert: " << ew[3] << endl;


  // calculate inertian values all over the datacube

  cout << "calculate inertian values all over the datacube" << endl;

  int DataSize = dc.GetSize()[1]*dc.GetSize()[2]*dc.GetSize()[3];

  Vector<double> *vals = new Vector<double>[DataSize](3);
 
  Vector<double> *vects = new Vector<double>[DataSize](9);
 
  code = mom.all_inertian_values(vals,vects,vol);
  if ( code ) cout << errorCode(code);
  
  int pos;
  double swap, sumew, cl, cp, ci;
  for ( int k = 0; k < size[3]; k++ ){
    for ( int j = 0; j < size[2]; j++ ){
      for ( int i = 0; i < size[1]; i++ ){
	
	pos = i+j*size[1]+k*size[1]*size[2];
	
	if ( 
	    ( j == 16 ) &&  
	      ( i >= 14 ) && ( i <= 17 ) && ( ( k == i ) || ( k == i+1) )
	    ) 
	     {
	
	  cout << "Traegheitswerte am Punkt ( " << i << ", " << j << ", " << k <<" ): " << endl;
	  
	  cout << "eigenvektor: ( " << vects[pos][1] << " " << vects[pos][2] << " " << vects[pos][3] << " )" << endl;
	  cout << "dazu eigenwert: " << vals[pos][1] << endl;
	  cout << "eigenvektor: ( " << vects[pos][4] << " " << vects[pos][5] << " " << vects[pos][6] << " )" << endl;
	  cout << "dazu eigenwert: " << vals[pos][2] << endl;
	  cout << "eigenvektor: ( " << vects[pos][7] << " " << vects[pos][8] << " " << vects[pos][9] << " )" << endl;
	  cout << "dazu eigenwert: " << vals[pos][3] << endl;

	  // sort eigenvalues 
	  if ( vals[pos][1] < vals[pos][2] ){ 
	    swap = vals[pos][1]; vals[pos][1] = vals[pos][2]; vals[pos][2] = swap;
	  }
	  if ( vals[pos][1] < vals[pos][3] ){ 
	    swap = vals[pos][1]; vals[pos][1] = vals[pos][3]; vals[pos][3] = swap;
	  }
	  if ( vals[pos][2] < vals[pos][3] ){ 
	    swap = vals[pos][2]; vals[pos][2] = vals[pos][3]; vals[pos][3] = swap;  
	  }
	  
	  sumew = vals[pos][1]+vals[pos][2]+vals[pos][3];
	  cl = (vals[pos][1]-vals[pos][2])/sumew;
	  cp = 2*(vals[pos][2]-vals[pos][3])/sumew;
	  ci = 3*vals[pos][3]/sumew;

	  cout << "Anisotropie-Koeffizienten: "<< endl;
	  cout << " cl = " << cl << endl;
	  cout << " cp = " << cp << endl;
	  cout << " ci = " << ci << endl;
	}

      }
    }
  }

}




