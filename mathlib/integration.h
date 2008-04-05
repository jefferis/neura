/****************************************************************************/
/*                                                                          */
/* File:      integration.h                                                 */
/*                                                                          */
/* Purpose:   routines for integration                                      */
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
#ifndef INTEGRATION_H
#define INTEGRATION_H

#include "../datalib/vector.h"
#include "../datalib/datacube.h"
#include "integrant.h"
#include "../errorcode.h"
#include <math.h>

class Integration
{

public:

  Integration();
  Integration( DataCube *datacube, Integrant *integrant_class);
  ~Integration(); 
  int execute( NeuraDataType &res, Volume &vol); 
  int execute2d( NeuraDataType &res, Volume &vol);
  int execute3d( NeuraDataType &res, Volume &vol );

  void SetIntegrant( Integrant *integrant_class );
  void SetDatacube( DataCube *datacube);
  void SetBallItDepht(int depht);
  void SetBallRefinement(int refinement);

  int GetBallRadX();
  int GetBallRadY();
  int GetBallRadZ();
  int GetBallRadSet();

  DataCube BallWeights; // for to store the integration weights for each pixel 
  int computeBallWeights2d(Volume &vol);
  int computeBallWeights3d(Volume &vol);

private:

  Integrant *inte; // pointer on integrant class
  DataCube *dc; // pointer on the DataCube

  //inline int within_dc(int x, int y, int n, int m );
  //inline int within_dc(int x, int y, int z, int n, int m, int k);


  // all following is needed for to calculate and store 
  // the weights needed for integration on ball

  int BallRad_x; // radius of integration ball to which weights are stored
  int BallRad_y;
  int BallRad_z;
  int BallRadSet; // flag whether integration radius is set 

  int BallItDepht;  // depht of iteration for calculation of ball volume
  int BallRefinement; // refinement in each direction in one iteration step 
                     // for calculation of ball volume    


  inline int point_in_ball(NeuraDataType mxp, NeuraDataType myp, 
		    NeuraDataType radx, NeuraDataType rady, 
		    NeuraDataType x, NeuraDataType y);

  inline int point_in_ball(NeuraDataType mxp, NeuraDataType myp, NeuraDataType mzp, 
		    NeuraDataType radx, NeuraDataType rady, NeuraDataType radz, 
		    NeuraDataType x, NeuraDataType y, NeuraDataType z);

  NeuraDataType calculate_locale_volume(NeuraDataType mpx, NeuraDataType mpy, 
				 NeuraDataType radx, NeuraDataType rady,
				 NeuraDataType volume, 
				 NeuraDataType bxl, NeuraDataType bxu, 
				 NeuraDataType byl, NeuraDataType byu, 
				 int itdepht, int refinement );
  
  NeuraDataType calculate_locale_volume(NeuraDataType mpx, NeuraDataType mpy, NeuraDataType mpz, 
				 NeuraDataType radx, NeuraDataType rady, NeuraDataType radz,
				 NeuraDataType volume, 
				 NeuraDataType bxl, NeuraDataType bxu, 
				 NeuraDataType byl, NeuraDataType byu, 
				 NeuraDataType bzl, NeuraDataType bzu,
				 int itdepht, int refinement );

 

};

#endif
