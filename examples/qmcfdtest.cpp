
/****************************************************************************/
/*                                                                          */
/* File:      program to test qmcfddatacube                                 */
/*                                                                          */
/* Purpose:   structure that can handle  dattransfer between                */
/*            datacube and quocmesh                                         */
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
/* History:   04.11.02                                                      */
/*                                                                          */
/* Remarks:                                                                 */
/*                                                                          */
/****************************************************************************/
  
#include "../quocmesh/include/qcTravOp.h"
#include <qcScalarArray3d.h>
#include <qcVector.h>
#include <stdio.h>
#include "../mathlib/volume.h"
#include "../datalib/vector.h"
#include "../datalib/qmcfddatacube.h"
#include "../errorcode.h"

int main(int argc, char **argv)
{  
  int error=0;
  QmCfdDataCube testCube;
  qcScalarArray3d<double>  *A;
  
  error=testCube.LoadCfdData("../samplePictures/S071A10%d.CFD",0,2);
  if (error) {cout << errorCode(error) <<  "->Exit!" << endl;exit(1);};
  cout << "TestCube" << endl << testCube;
  Volume volume(0,0,0,256,256,256,1.0,1.0,1.0);
  testCube.extractArray(volume,&A,0);
  A->saveSlices("tmp/img-%03d.pgm", QC_Z);

  return OK;
}
   
