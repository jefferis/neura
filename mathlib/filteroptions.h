/****************************************************************************/
/*                                                                          */
/* File:      filteroptions.h                                               */
/*                                                                          */
/* Purpose:   datastructure for filter options                              */
/*                                                                          */
/* Author:    Roland Schulte                                                */
/*            Interdisziplinaeres Zentrum fuer Wissenschaftliches Rechnen   */
/*            Universitaet Heidelberg                                       */
/*            Im Neuenheimer Feld 368                                       */
/*            D-69120 Heidelberg                                            */
/*                                                                          */
/* EMail:     roland.schulte@iwr.uni-heidelberg.de                          */
/*                                                                          */
/* History:   24.03.03 begin                                                */
/*                                                                          */
/* Remarks:                                                                 */
/*                                                                          */
/****************************************************************************/

#ifndef FILTEROPTIONSHEADER
#define FILTEROPTIONSHEADER

#include "../datalib/datacube.h"

enum BOOL{NO,YES};

enum DependenceType{PERONA_MALIK, WEIKERT, BLACK_SAPIRO};

enum KindOfMoments{ELEMENTWISE,NODEWISE};

#define IP_USUAL 0
#define IP_BND 1

class FilterOptions
{
 public:

  FilterOptions();

  // solver options
  NeuraDataType tau;       // time step size
  int time_steps;   // number of time steps
  NeuraDataType epsilon;   // precision of solution
  int levels;       // number of multigrid levels
  
  // discretation options
  BOOL fixed_coeffs; // flag deciding wheter coefficients of anisotropy
                    // are fixed or variable
  NeuraDataType anicoeff1, anicoeff2, anicoeff3;   // coefficients of anisotropy
  DependenceType dependence_type;           // choose of variability of the 
                                            // coefficients of anisotropy
  NeuraDataType lambda;                            // parameter for DependenceType function
  GeometryType gt;                          // kind of integration domain
  int integration_size_x;                     // integration size for moment calculation
  int integration_size_y;
  int integration_size_z;

  int ip_flag;                              // place of integration points that are used
  
 

};

#endif
