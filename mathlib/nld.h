/****************************************************************************/
/*                                                                           */
/* File:      nld.h                                                         */
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
/* Remarks:                                                                 */
/*                                                                          */
/****************************************************************************/

#include <iostream>
#include <time.h>
#include "../datalib/datacube.h"
#include "../datalib/array.h"
#include "multivector.h"
#include "../datalib/sparsematrix.h"
#include "filteroptions.h"
#include "fv_3d27.h"
#include "multigrid.h"
#include "../errorcode.h"

#ifndef NLDHEADER
#define NLDHEADER


class NLD
{

 public:
  
  NLD(DataCube* datacube);
  ~NLD();

  void SetFilterOptions(FilterOptions fo);
  
  void SetTimeStep(NeuraDataType t);
  void SetNumberOfTimeSteps(int n);
  void SetPrecision(NeuraDataType eps);
  void SetMultigridLevels(int l);

  void SetLambda(NeuraDataType l);
  void SetAniCoefficients(NeuraDataType ani1, NeuraDataType ani2, NeuraDataType ani3);
  void SetFixedCoefficients(BOOL flag);
  void SetDependenceType(DependenceType type);
  void SetIntegrationSize(int s);
  void SetIntegrationSizeX(int s);
  void SetIntegrationSizeY(int s);
  void SetIntegrationSizeZ(int s);
  void SetGeometryType(GeometryType type);
  void SetIpFlag(int flag);

  int execute();
  
 private:
  
  DataCube* dc; 

  NeuraDataType tau;     // time step
  int time_steps; // number of time steps
  NeuraDataType epsilon; // precision of solution
  int levels;     // number of multigrid levels

  NeuraDataType lambda;                         // parameter of nonlinearity
  NeuraDataType anicoeff1,anicoeff2,anicoeff3;  // anisotropy coefficients
  BOOL fixed_coeffs ; // flag deciding whether coefficients of anisotropy are fixed or variable
  DependenceType dependence_type; // chooses variability of coefficients of anisotropy
  int integration_size_x;                 // integration size for moment calculation
 int integration_size_y;
 int integration_size_z;              
  GeometryType gt;                      // type of integration domain
  int ip_flag;                          // kind of integration points that are used

  double alpha; // ? needed for multigrid, but value does not matter

  int pow_of_two(int size);
  
  
};

#endif 

