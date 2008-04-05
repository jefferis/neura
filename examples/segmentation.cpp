#include <list>
#include "../datalib/datacube.h"
#include "../datalib/testcubes.h"
#include "../datalib/cfddatacube.h"

#include "../mathlib/statistic.h"
#include "../mathlib/transform.h"
// #include "../mathlib/nld.h"
#include "../errorcode.h"

#include <iostream>
#include <ostream>
#include <algorithm>

#include "../segmentationlib/segmentation.h"


using namespace std;



int realDataSegmentation()
{
  
  char *name="../samplePictures/S041B080.cfd";
  int start[3];
  int size[3];
  start[0]=90;
  start[1]=70;
  start[2]=20;
  size [0]=65;
  size [1]=65;
  size [2]=65;
  CfdDataCube *cfddc;
  DataCube *targetcube;
  DataCube *targetcube2;
  cfddc = new CfdDataCube();
  if (cfddc->LoadCfdData(name,0,0, start, size)) return 1;
  cout << "Now transform";
  Transform trans((DataCube*) cfddc);
  trans.ThresholdDataLow(20, 0);
  trans.ThresholdDataHigh(80, 100);
  trans.ScaleData(1/100.0);
  cout << " done." << endl;
  targetcube=new DataCube(*cfddc);
  targetcube2=new DataCube(*cfddc);
  cfddc->writeRAW("tmp/beforeSegmentation.dat","tmp/info.txt");
  Segmentation FiltSeg1(cfddc, targetcube);
  cout<<"Start der Segm. mit VeloMode Grauwert"<<endl;
  FiltSeg1.SegmentateByRegionGrowing(Grayvalue,200,33,56, 6);
  targetcube->writeRAW("tmp/afterSegmentationGV.dat","tmp/info.txt");
  cout<<"...bendet"<<endl;
  cout<<"Start der Segm. mit VeloMode Grauwert/Otsuwert"<<endl;
  Segmentation FiltSeg2(cfddc, targetcube2);
  FiltSeg2.SegmentateByRegionGrowing(GrayvalueDivOtsuvalue,2,33,56, 6);
  targetcube2->writeRAW("tmp/afterSegmentationGVDivOV.dat","tmp/info.txt");
  cout<<"...bendet"<<endl;
}

int filteredDataSegmentation()
{
  Vector<int> sl(3);
  Vector<double> dl(3);
  sl[1]=256;
  sl[2]=256;
  sl[3]=150;

  dl[1]=0;
  dl[2]=0;
  dl[3]=0;
  CfdDataCube *testcube;
  DataCube *targetcube;
  DataCube *targetcube2;
  testcube= new CfdDataCube();
  testcube->SetCubeSizeAndSpaceing(sl,dl);
  testcube->MallocCube();
  testcube->readRAW("../samplePictures/aniso2.dat");
  targetcube=new DataCube(*testcube);
  targetcube2=new DataCube(*testcube);
  testcube->writeRAW("tmp/beforeSegmentation.dat","tmp/info.txt");
  Segmentation FiltSeg1(testcube, targetcube);
  cout<<"Start der Segm. mit VeloMode Grauwert"<<endl;
  FiltSeg1.SegmentateByRegionGrowing(Grayvalue,4000,93,120, 28);
  targetcube->writeRAW("tmp/afterSegmentationGV.dat","tmp/info.txt");
  cout<<"...bendet"<<endl;
  cout<<"Start der Segm. mit VeloMode Grauwert/Otsuwert"<<endl;
  Segmentation FiltSeg2(testcube, targetcube2);
  FiltSeg2.SegmentateByRegionGrowing(GrayvalueDivOtsuvalue,0.5,93,120, 28);
  targetcube2->writeRAW("tmp/afterSegmentationGVDivOV.dat","tmp/info.txt");
  cout<<"...bendet"<<endl;
  return 0;
	
}

int NihSegmentation()
{
  
  char *name="../samplePictures/readyforsegmentation2.nih";
  DataCube *targetcube;
  DataCube *targetcube2;
  DataCube *sourcecube;
  sourcecube = new DataCube(3);
  if (sourcecube==NULL)
    {
      cout << "Error: Can not malloc" << endl;
      return 1;
    }
  int errorcode=sourcecube->ReadFromNIH(name);
  if (errorcode) 
    {
      cout << "Error during load exit" << endl;
      cout << errorCode(errorcode) << endl;
      exit(1);
    }

  targetcube=new DataCube(*sourcecube);
  targetcube2=new DataCube(*sourcecube);
  sourcecube->writeRAW("tmp/beforeSegmentation.dat","tmp/info.txt");
  Segmentation FiltSeg1(sourcecube, targetcube);
  cout<<"Start der Segm. mit VeloMode Grauwert"<<endl;
  FiltSeg1.SegmentateByRegionGrowing(Grayvalue,7000,123,132, 23); 
  targetcube->writeRAW("tmp/afterSegmentation001.dat","tmp/info.txt");
  cout<<"...bendet"<<endl;
  Segmentation FiltSeg2(sourcecube, targetcube2);
  cout<<"Start der Segm. mit VeloMode Grauwert/Otsuwert"<<endl;
  FiltSeg2.SegmentateByRegionGrowing(GrayvalueDivOtsuvalue,4000,123,132, 23);
  targetcube2->writeRAW("tmp/afterSegmentationGVDivOV.dat","tmp/info.txt");
  cout<<"...bendet"<<endl;
  return 0;
}


int main (int argc, char **argv)
{
  int test=3;
  switch (test)
    {
   
    case 1:
      realDataSegmentation();
      break;
    case 2:
      filteredDataSegmentation();
      break;
    case 3:
      NihSegmentation();
      break;
    }
  return 0;


    };

 
