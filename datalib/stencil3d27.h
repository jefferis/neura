/****************************************************************************/
/*                                                                          */
/* File:      stencil3d27.h                                                 */
/*                                                                          */
/* Purpose:   datastructures for 3d 27point-stencil                         */
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
/* Remarks:                                                                 */
/*                                                                          */
/****************************************************************************/
#ifndef STENCIL3D27
#define STENCIL3D27

#include <stream.h>
#include <math.h>

#include "stencil.h"
#include "../errorcode.h"

class Stencil3d27 : public Stencil
{

 public:

  Stencil3d27();
  ~Stencil3d27();

  int Length();
  int SetElem(int i, double val);
  int AddToElem(int i, double val);
  int SetElem(int x, int y, int z, double val);
  int AddToElem(int x, int y, int z, double val);
  double GetElem(int i);
  void multiply( double factor );
  void clear();
  void print();
  double &operator()(int i);

  friend ostream &operator<<(ostream &ostr, Stencil &ST);

  // position of stencils midpoint within elem[] 
  static int DIAG; 

  // number of elements 
  static int ELEMS;

 private:
 
  double elem[27];

  // [

  // u[x-1,y-1,z-1] u[x,y-1,z-1] u[x+1,y-1,z-1]  
  // u[x-1,y,z-1]   u[x,y,z-1]   u[x+1,y,z-1]
  // u[x-1,y+1,z-1] u[x,y+1,z-1] u[x+1,y+1,z-1]
  
  // u[x-1,y-1,z]   u[x,y-1,z]   u[x+1,y-1,z]
  // u[x-1,y,z]     u[x,y,z]     u[x+1,y,z]
  // u[x-1,y+1,z]   u[x,y+1,z]   u[x+1,y+1,z]
  
  // u[x-1,y-1,z+1] u[x,y-1,z+1] u[x+1,y-1,z+1]
  // u[x-1,y,z+1]   u[x,y,z+1]   u[x+1,y,z+1]
  // u[x-1,y+1,z+1] u[x,y+1,z+1] u[x+1,y+1,z+1]
  
  // ]

};

#endif
