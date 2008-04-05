/****************************************************************************/
/*                                                                          */
/* File:      nld_test.cpp                                                  */
/*                                                                          */
/* Purpose:   routines for test nld.h                                       */
/*                                                                          */
/* Author:    Roland Schulte                                                */
/*            Interdisziplinaeres Zentrum fuer Wissenschaftliches Rechnen   */
/*            Universitaet Heidelberg                                       */
/*            Im Neuenheimer Feld 368                                       */
/*            D-69120 Heidelberg                                            */
/*                                                                          */
/* EMail:     roland.schulte@iwr.uni-heidelberg.de                          */
/*                                                                          */
/* History:   20.01.03 begin                                                */
/*                                                                          */
/* Remarks:                                                                 */
/*                                                                          */
/****************************************************************************/

#include <iostream>
#include <stdlib.h>
#include "../datalib/datacube.h"
#include "../mathlib/nld.h"
#include "../errorcode.h"


int main(int argc, int **argv)
{
  int code;

  //char filename[60];
  //sprintf(filename,"multivector.txt");
  // redict output to specified file
  //std::ofstream file (filename);
  //std::cout.rdbuf (file.rdbuf());
   
  DataCube dc(3); 

  Vector<int> size(3); 
#define SIZE 128;
  size[1] = SIZE; // size of datacube in x-direction
  size[2] = SIZE; // size of datacube in y-direction
  size[3] = SIZE; // size of datacube in z-direction

  Vector<double> spaceing(3);
  spaceing[1]=1.0;
  spaceing[2]=1.0;
  spaceing[3]=1.0;

  dc.SetCubeSizeAndSpaceing(size,spaceing); 
  dc.MallocCube();

 // define the datacube values
  int i,j,k; 
  for ( k = 0; k < dc.GetSize()[3]; k++ ){
    for ( j = 0; j < dc.GetSize()[2]; j++ ){
      for ( i = 0; i < dc.GetSize()[1]; i++ ){
	if ( 
	    ( k == 65 ) 
	    )
	  dc(i,j,k) = (rand()%256); 
	else dc(i,j,k) = 0.0;
      }
    }
  }

 //  // Y
//   // define the datacube values
//   int i,j,k; 
//   for ( k = 0; k < dc.GetSize()[3]; k++ ){
//     for ( j = 0; j < dc.GetSize()[2]; j++ ){
//       for ( i = 0; i < dc.GetSize()[1]; i++ ){
// 	if ( 
// 	    ( k == 16 ) &&
// 	    (
// 	     ( ( i == 16 ) && ( ( j < 8 ) || ( ( j > 9 ) && ( j < 15 ) ) ) ) 
// 	     || ( ( i == j ) && ( ( ( i > 17 ) && ( i < 23 ) ) || ( i > 25 ) ) ) 
// 	     || ( ( i == 32-j ) && ( ( i < 8 ) || ( ( i > 9 ) && ( i < 15 ) ) ) )
// 	     )
// 	    )
// 	  dc(i,j,k) = 1.0;
// 	else dc(i,j,k) = 0.0;
// 	  }
//     }
//   }
 

 //  // print initial value
//   cout << "initial value: " << endl;
//   for ( int k = 0; k < size[3]; k++ ){
//     //k = 4;
//     for ( int j = 0; j < size[2]; j++ ){
//       for ( int i = 0; i < size[1]; i++ ){
// 	cout << dc(i,j,k) << " ";
//       }
//       cout << endl;
//     }
//     cout << endl;
//   }
//   cout << endl;


 cout << "Initialize filter" << endl;

  NLD nld(&dc);
 
  nld.SetTimeStep(2.0);
  nld.SetNumberOfTimeSteps(2);
  nld.SetPrecision(0.000001);
  nld.SetMultigridLevels(1);
  nld.SetLambda(0.1);
  nld.SetFixedCoefficients(YES);
  nld.SetDependenceType(PERONA_MALIK);
  nld.SetAniCoefficients(1.0,0.0,0.0);
  nld.SetIntegrationSize(5);
  nld.SetIpFlag(IP_USUAL);
  nld.SetGeometryType(BALL);
 
  cout << "Output before filter" << endl;
  dc.writeRAW("test2d.dat","test2d.info");
  
  cout << "Start filter" << endl;
  
  code = nld.execute();
  if ( code ) { cout << errorCode(code); exit(1); }
  
  cout << "Image filtered" << endl;
  
  cout << "Output after filter" << endl;
  dc.writeRAW("test2dout_ball.dat","test2dout_ball.info");


 //  cout << "solution calculated:" << endl;
//   cout << endl;

//   double sum = 0;  

//   for ( int k = 0; k < size[3]; k++ ){
//     //k = 4;
//     for ( int j = 0; j < size[2]; j++ ){
//       for ( int i = 0; i < size[1]; i++ ){
// 	cout << dc(i,j,k) << " ";
// 	sum += dc(i,j,k);
//       }
//       cout << endl;
//     }
//     cout << endl;
//   }
//   cout << endl;
  
//   cout << "global sum:" << sum << endl; 
 
 
}
  
