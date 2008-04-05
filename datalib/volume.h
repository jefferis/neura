/****************************************************************************/
/*                                                                          */
/* File:      volume.h   	                                            */
/*                                                                          */
/* Purpose:   superclass definition for data handling                       */
/*                                                                          */
/* Author:    Philip J. Broser                                              */
/*            Max-Planck-Institut fuer medizinische Forschung               */
/*            Jahnstrasse 29                                                */
/*	      D-69120 Heidelberg                                            */
/*                                                                          */
/*            Roland Schulte                                                */
/*            Interdisziplinaeres Zentrum fuer Wissenschaftliches Rechnen   */
/*            Universitaet Heidelberg                                       */
/*            Im Neuenheimer Feld 368                                       */
/*            D-69120 Heidelberg                                            */
/*                                                                          */
/* EMail: philip.broser@iwr.uni-heidelberg.de                               */
/*                                                                          */
/* History:   27.09.2002                                                    */
/*                                                                          */
/* Remarks:                                                                 */
/*                                                                          */
/****************************************************************************/
#ifndef VOLUME_H
#define VOLUME_H

#include "vector.h"
#include "../errorcode.h"
#include <math.h>
#include <complex.h>

enum GeometryType{CUBE,BALL};

class Volume  	
{
 public:
  Volume();
  Volume(Volume &vol);
  Volume(const Volume& vol);
  Volume(int dimension);
  Volume(GeometryType t, int dim, int six, int siy);
  Volume(GeometryType t, int dim, int six, int siy, int siz);
  Volume(int dim,const Vector<int> &st,const Vector<int> &si,Vector<double> &dl); 
  Volume(GeometryType t, int dimension);
  Volume(int stx,int sty, int six, int siy, double dx, double dy);
  Volume(GeometryType t,int stx,int sty, int six, int siy, double dx, double dy);
  Volume(int stx,int sty, int stz,int six, int siy, int siz, double dx, double dy, double dz);
  Volume(GeometryType t,int stx,int sty, int stz,int six, int siy, int siz, double dx, double dy, double dz);

  ~Volume();

  Vector<double> discreteToCont(Vector<int> posDiscrete);
  int VolumeCopy(Volume &vol);
  int TransformToCube(Volume &vol);

  void SetCubeSizeAndSpaceing(const Vector<int> &sl,Vector<double> &dl);
  void SetStart(int x, int y);
  void SetStart(int x, int y, int z);
  void SetStart(Vector<int>& s);

  void SetSize(Vector<int> &s);
  void SetSpaceing(Vector<double> &dl);
  
  int GetGeometryType();
  int GetDim();
  Vector<int>& GetStart() {return start;};
  Vector<int>& GetSize();
  Vector<double>& GetSpaceing();

  int is_in( int x, int y, int z);
  int calc_node( Vector<int>& pos);
  int calc_pos(int node, Vector<int>& pos);

  int dim;
  GeometryType gt; // case 0: cube , case 1: ball
  Vector<int> start;
  Vector<int> size;
  Vector<double> d;  
  
  friend ostream &operator<<(ostream &ostr, const Volume &v);
};


#endif
