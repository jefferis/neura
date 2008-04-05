/****************************************************************************/
/*                                                                          */
/* File:      testdatacubes.h 	                                            */
/*                                                                          */
/* Purpose:   sample cubes for testing new software components              */
/*                                                                          */
/* Author:    Philip J. Broser                                              */
/*            Max-Planck-Institut fuer medizinische Forschung               */
/*            Jahnstrasse 29                                                */
/*	      D-69120 Heidelberg                                            */
/*                                                                          */
/*                                                                          */
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


#ifndef TESTCUBES_H
#define TESTCUBES_H
#include <stdio.h>

#include "volume.h"
#include "vector.h"
#include "datacube.h"
#include "../errorcode.h"


#define SIMPLEY2D  1
#define SIMPLEY2DBREIT  2
#define SIMPLEY3D  1
#define TUBEY3DX    2
#define TUBEY3DY    3
#define TUBEY3DZ    4
#define TUBEY3DXSMALL    5
#define TUBEY3DYSMALL    6
#define TUBEY3DZSMALL   7
#define SPHERE3D 8
#define SPHERE3DHOLES 9
#define PLANE3D 10
#define PLANE3DHOLES 11
#define Y3D_3PIXEL_GABS 12
#define TUBE_LOKAL_VALUE 13
#define TUBEY3DXANDNOTCONNECTED 14
#define TUBEY3DX_VARIABLE 15
#define TUBEYBIG 16

#define DIRX 0
#define DIRY 1
#define DIRZ 2


class TestCube2D : public DataCube
{
 public: 
  TestCube2D(int Lnr);
  ~TestCube2D();
  friend ostream &operator<<(ostream &ostr, const TestCube2D &TC);
  void showCube();
 private:
  void SimpleY2D();
  void SimpleY2Dbreit();
  int StandardSize();
  int nr;
};
 

class TestCube3D : public DataCube
{
 public: 
  TestCube3D(int Lnr);
  ~TestCube3D();
  int Y3d_3pixel_gabs();
  friend ostream &operator<<(ostream &ostr, const TestCube3D &TC);
 private:
  NeuraDataType &permutate(int p1,int p2, int p3, int dir);
  void SimpleY3D();
  void TubeY3D(int dir,int wide);
  void TubeY3D_var(int dir,int wide);
  int Sphere3D(int wide);
  int Sphere3Dholes(int wide);
  int plane3D();
  int plane3Dholes();
  int TubeLocalValue();
  int TubeY3DNotConnected();
  int TubeYBig();
  int StandardSize();
  int nr;
};


#endif













