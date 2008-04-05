/****************************************************************************/
/*                                                                          */
/* File:      nld_auto.h                                                    */
/*                                                                          */
/* Purpose:   routines for control nonlinear anisotropic diffusion          */
/*                                                                          */
/* Author:    Roland Schulte                                                */
/*            Interdisziplinaeres Zentrum fuer Wissenschaftliches Rechnen   */
/*            Universitaet Heidelberg                                       */
/*            Im Neuenheimer Feld 368                                       */
/*            D-69120 Heidelberg                                            */
/*                                                                          */
/* EMail:     roland.schulte@iwr.uni-heidelberg.de                          */
/*                                                                          */
/* History:   16.01.03 begin                                                */
/*                                                                          */
/* Remarks:   Written to make automatically tests possible.                 */
/*            Method execute() gets a number, for to make storing slices    */
/*	      into different files possible, if execute() is called more    */
/*	      than one time while programm execution. This may be if the    */
/*	      mainfile calls execute() with different filter options        */
/*	      within a loop.                                                */
/*	                                                                    */
/*                                                                          */
/*                                                                          */
/****************************************************************************/

#include <iostream>
#include <time.h>
#include "../datalib/datacube.h"
#include "../datalib/array.h"
#include "multivector.h"
#include "../datalib/sparsematrix.h"
#include "fv_3d27.h"
#include "multigrid.h"
#include "../errorcode.h"

#ifndef NLDAUTOHEADER
#define NLDAUTOHEADER

class NLD_Auto
{

 public:
  
  NLD_Auto(DataCube* datacube);
  ~NLD_Auto();
  
 void SetFilterOptions(FilterOptions fo);

  void SetTimeStep(double t);
  void SetNumberOfTimeSteps(int n);
  void SetPrecision(double eps);
  void SetMultigridLevels(int l);

  void SetLambda(double l);
  void SetAniCoefficients(double ani1, double ani2, double ani3);
  void SetNonlinear(int non);
  /* FIXME!!! Alexander
  void SetNonlinearType(NonlinearType type);
  */
  void SetKindOfMoments(KindOfMoments kind);
  void SetIntegrationSize(int s);
  void SetSmallBoundaryPixels(int flag);
  void SetIpFlag(int flag);

  int execute(int number);
  
 private:
  
  DataCube* dc; 

  double tau;     // time step
  int time_steps; // number of time steps
  double epsilon; // precision of solution
  int levels;     // number of multigrid levels

  double lambda;                         // parameter of nonlinearity
  double anicoeff1,anicoeff2,anicoeff3;  // anisotropy coefficients
  int nonlinear;                         // flag deciding whether diffusion is nonlinear or linear
  /* FIXME!!! Alexander
  NonlinearType nonlinear_type;                   // chooses nonlinear function
  */
  KindOfMoments kind_of_moments;         // kind of calculate moments in integration points
 int small_boundary_pixels; // flag deciding whether amount of 
                             // boundary pixels is 1 or smaller
 int integration_size;                 // integartion size for moment calculation
  int ip_flag;                          // kind of integration points that are used

  double alpha; // ? needed for multigrid, but value does not matter
  
  
};

#endif
