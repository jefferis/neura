/****************************************************************************/
/*                                                                          */
/* File:      moments.h                                                     */
/*                                                                          */
/* Purpose:   routines to calculate moments                                 */
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
#ifndef MOMENTS
#define MOMENTS

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include "../datalib/vector.h"
#include "../datalib/volume.h"
#include "../datalib/datacube.h"
#include "../datalib/momentcube.h"
#include "integration.h"
#include "integrant.h"
#include "../errorcode.h"

class Moments
{

 public:

  Moments(DataCube *dc);

  ~Moments();

  int focal_point(Vector<NeuraDataType> &fp, Volume &vol, NeuraDataType &mass_res );
 
  int focal_point2d(Vector<NeuraDataType> &fp, Volume &vol, NeuraDataType &mass_res );

  int focal_point3d(Vector<NeuraDataType> &fp, Volume &vol, NeuraDataType &mass_res );

  int inertian_tensor(Vector<NeuraDataType> &it, Volume &vol);

  int inertian_tensor2d(Vector<NeuraDataType> &it, Volume &vol);

  int inertian_tensor3d(Vector<NeuraDataType> &it, Volume &vol);

  int inertian_values(Vector<NeuraDataType> &vals, Vector<NeuraDataType> &vects, Volume &vol );

  int all_inertian_values(Vector<NeuraDataType> *vals, Vector<NeuraDataType> *vects, Volume &vol);

  int fast_all_inertian_values(Vector<NeuraDataType> *vals, Vector<NeuraDataType> *vects, Volume &vol);
 
 int elemwise_fast_all_inertian_values(Vector<NeuraDataType> *vals, Vector<NeuraDataType> *vects, Volume &vol);

 int elemwise_fast2_all_inertian_values(Vector<NeuraDataType> *vals, Vector<NeuraDataType> *vects, Volume &vol);

 int fourier_all_inertian_values(Vector<NeuraDataType> *vals, Vector<NeuraDataType> *vects, Volume &vol, int N, int M, int L);

 
 private:

  DataCube *dc; // pointer to datacube
  Integration i_t; // integration object

  Vector<NeuraDataType> f_p; // focal point
 
  int all_cube_inertian_values( Vector<NeuraDataType> *vals, Vector<NeuraDataType> *vects, Volume &vol);

 int pow_of_two(int size);
 

  // foollowing functions are copied from numerical recipes
  void nrerror(char error_text[]);
  float **matrix(long nrl, long nrh, long ncl, long nch);
  double **dmatrix(long nrl, long nrh, long ncl, long nch);
  float ***f3tensor(long nrl, long nrh, long ncl, long nch, long ndl, long ndh);
  void free_matrix(float **m, long nrl, long nrh, long ncl, long nch);
  void free_dmatrix(double **m, long nrl, long nrh, long ncl, long nch);
  void free_f3tensor(float ***t, long nrl, long nrh, long ncl, long nch,
		     long ndl, long ndh);
  void tred2(double **a, int n, double d[], double e[]);
  float pythag(float a, float b);
  int tqli(double d[], double e[], int n, double **z);
  void fourn(float data[], unsigned long nn[], int ndim, int isign);
  void rlft3(float ***data, float **speq, unsigned long nn1, unsigned long nn2,
	     unsigned long nn3, int isign);
};

#endif
