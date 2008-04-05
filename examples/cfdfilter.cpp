/*testprogram for filtering cfd data */
#include <fstream.h>
#include <stdio.h>

#include "../datalib/vector.h"
#include "../datalib/volume.h"
#include "../mathlib/statistic.h"
#include "../mathlib/transform.h"
#include "../errorcode.h"

#include "../datalib/datacube.h"
#include "../datalib/cfddatacube.h"
#include "../mathlib/nld.h"  


int main(int argc, char **argv)
{

  int code; // errorcode
  char *name="../../CFDData/S041B/S041B080.cfd";
  int start[3];
  int size[3];
  start[0]=30;
  start[1]=10;
  start[2]=0;
  size [0]=128;
  size [1]=128;
  size [2]=128;
  CfdDataCube *cfddc;
  cfddc = new CfdDataCube();
  
  if (cfddc->LoadCfdData(name,0,0, start, size)) return 1;
  Transform trans((DataCube*) cfddc);
  trans.ThresholdDataLow(10, 0);
  trans.ThresholdDataHigh(40, 100);
  trans.ScaleData(1/100.0);
  
  NLD nld((DataCube*) cfddc);
  nld.SetIpFlag(IP_USUAL);
  nld.SetTimeStep(4.0);
  nld.SetNumberOfTimeSteps(1);
  nld.SetPrecision(1e-02);
  nld.SetMultigridLevels(1);
  nld.SetLambda(0.1);
  nld.SetFixedCoefficients(YES);
  nld.SetDependenceType(WEIKERT);
  nld.SetAniCoefficients(1.0,0.0,0.0);
  nld.SetIntegrationSizeX(10);
  nld.SetIntegrationSizeY(10);
  nld.SetIntegrationSizeZ(10);
  nld.SetGeometryType(CUBE);
  
  cout << "Output before filter" << endl;
  cfddc->writeRAW("testbefore.dat","testbefore.info");
  cout << "Start to filter..." ;
  
  code = nld.execute();
  if ( code ) { cout << errorCode(code); exit(1); }
  cout << "done." << endl;
  cout << "Output after filter" << endl;
  cfddc->writeRAW("testafter.dat","testafter.info");


 return 0;
	
}

