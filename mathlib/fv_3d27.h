/****************************************************************************/
/*                                                                          */
/* File:      fv_3d27.h                                                     */
/*                                                                          */
/* Purpose:   routines to discretisize nonlinear anisotropic diffusion      */
/*            by finite volume method in 3d using a 27-point stencil        */
/*                                                                          */
/* Author:    Roland Schulte                                                */
/*            Interdisziplinaeres Zentrum fuer Wissenschaftliches Rechnen   */
/*            Universitaet Heidelberg                                       */
/*            Im Neuenheimer Feld 368                                       */
/*            D-69120 Heidelberg                                            */
/*                                                                          */
/* EMail:     roland.schulte@iwr.uni-heidelberg.de                          */
/*                                                                          */
/* History:   12.12.02 begin                                                */
/*                                                                          */
/* Remarks:   enumeration of elements and points in datacube begins with 0, */
/*            enumeration of corresponding rows in the matrix with 1        */ 
/*                                                                          */
/****************************************************************************/
#ifndef FV_3D27
#define FV_3D27

#include <math.h>
#include <time.h>
#include "../datalib/array.h"
#include "multivector.h"
#include "../datalib/datacube.h"
#include "../datalib/sparsematrix.h"
#include "moments.h"
#include "filteroptions.h"

#define PI 3.14159265

// ------------------------philips nan----------------------------------
#ifdef sgi
#define isnan(a) (!(a*0==0))
#endif
// ------------------------philips nan----------------------------------


class FV_3d27
{
  
 public:
  
  FV_3d27(SparseMatrix<NeuraDataType> *matrix, int ip_flag);
  ~FV_3d27();

  void SetLambda(NeuraDataType l);

  void SetAniCoefficients(NeuraDataType ani1, NeuraDataType ani2, NeuraDataType ani3);

  void SetFixedCoefficients(BOOL flag);

  void SetDependenceType(DependenceType type);

  void SetIntegrationSizeX(int s);

  void SetIntegrationSizeY(int s);
  
  void SetIntegrationSizeZ(int s);
 
  void SetGeometryType(GeometryType type);
  
  int discretize(DataCube &dc);

  int SetDirichlet(NeuraDataType val, MultiVector<NeuraDataType>&);

  int SetTimeDependent(NeuraDataType tau);

 private:
  
  SparseMatrix<NeuraDataType> *a; // pointer to matrix

  NeuraDataType lambda; // parameter of nonlinearity
  int integration_size_x; // integration size for moment calculation
  int integration_size_y; // integration size for moment calculation
  int integration_size_z; // integration size for moment calculation
  GeometryType gt; // Type of integration domain

  int en[8]; // global numbers of elements nodes
    
  NeuraDataType ip[8][2][3]; // integration points 
  NeuraDataType nip[8][2][3]; // normals in ips
  int intact[8][2];// interacting point to ip

  NeuraDataType anicoeff1,anicoeff2,anicoeff3; // anisotropy coefficients
  BOOL fixed_coeffs ; // flag deciding whether coefficients of anisotropy are fixed or variable
  DependenceType dependence_type; // chooses variability of coefficients of anisotropy

  NeuraDataType (*ani1)(NeuraDataType);
  NeuraDataType (*ani2)(NeuraDataType);
  NeuraDataType (*ani3)(NeuraDataType);

  KindOfMoments kind_of_moments; 

  inline NeuraDataType g_pm(NeuraDataType s);
  inline NeuraDataType g_w(NeuraDataType s);
  inline NeuraDataType g_bs(NeuraDataType s);

  void fill_integration_points(int ip_flag); 
  inline void elements_nodes(int e, int n, int m, int en[8]);
  inline NeuraDataType phi(int i, NeuraDataType xi, NeuraDataType eta, NeuraDataType theta );
  inline NeuraDataType grad_phi_x(int j, NeuraDataType xi, NeuraDataType eta, NeuraDataType theta );
  inline NeuraDataType grad_phi_y(int j, NeuraDataType xi, NeuraDataType eta, NeuraDataType theta );
  inline NeuraDataType grad_phi_z(int j, NeuraDataType xi, NeuraDataType eta, NeuraDataType theta );


};

#endif
