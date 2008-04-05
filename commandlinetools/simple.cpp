#include <fstream.h>
#include <stdio.h>
#include <time.h>

#include "../datalib/vector.h"
#include "../datalib/volume.h"
#include "../mathlib/statistic.h"
#include "../mathlib/transform.h"


#include "../errorcode.h"

#include "../datalib/datacube.h"
#include "../datalib/cfddatacube.h"
#include "../datalib/testcubes.h" 
#include "../segmentationlib/segmentation.h"
#include "../commandlinetools/simple.h"

#include "command_line_parser.h"


 
using namespace std;
using namespace pramodx;
 




             

int
main(int argc, char **argv)
{

  DataCube convert(3);
  Vector<double> d(3);
  d[1]=1.0;d[2]=1.0;d[3]= 1.0;


  int scalar, minimalBetween2;
  double absMinTh, absMaxTh;
  Vector<int> sizeSmallCube(3);

DataCube *sourceCube;

    int res=convert.ReadFromTIFF("../../Desktop/zelle1_deconv_filt.tiff"); 
  //int res=convert.ReadFromTIFF("filteredtest.tiff");
  // int res=convert.ReadFromTIFF("testsimplefilt.tiff");
  if (res!=0 && res!=NO_SPACEINGS_TAGS) 
   {
	cout << "Error during read!" << endl;
	return 1;
    };
  convert.SetSpaceing(d); 
    

  DataCube *segmenDC=new DataCube(convert);
    
  Segmentation segmen(&convert, segmenDC);

  double Threshold = 0.35;   
  cout << "Threshold: " << Threshold << endl;
  //segmen.SegmentateByGlobalThresholding(Threshold);    
  segmen.SegmentateByOtsuMethod();  
  //segmen.SegmentateByOtsuDevidingThroughTheThreshold(); 
  double abslow = 0.5;
  double abshigh = 1.0;
  double eps = 0.014; 
  //segmen.SegmentateByStructuralThresholding(abslow, abshigh, eps); 
  
    
          
  (*segmenDC).WriteToTIFF("../../Desktop/zelle1_deconv_seg_filt.tiff"); 
  //(*segmenDC).writeDataForDX("./numerische_optimierung/sphere3D_holes/sphere3D_holes_filt1_seg03.dx");   
  
   
  

 


  
  
  
  //int floor;
  //int y, z;

  //floor = 35;
  //for(y=0; y <=512; y++)
  //for(z=0; z <=512; z++)
  //convert.SetData(y, z,floor, 1.0);


  //convert.WriteToTIFF("./280605/c1/stack128_test0_seg_filt2_big.tiff");         
  //convert.writeDataForDX("./280605/c1/stack_128_test0_seg_filt.dx");

  
  //zum Teilen des Cubes
  
  //Schnitt 1.1:
  

  Vector<int> size(3);
  size=convert.GetSize();
  

  cout <<"Groesse: " << size << endl;

  convert.ReadFromTIFF("stack1_seg_filt_test_floor.tiff");
  int paramx1, paramx2, paramx3;
  int paramy1, paramy2, paramy3;
  int paramz1, paramz2, paramz3;
  
  for(paramx1=0; paramx1<=255; paramx1++)
  for(paramx2=0; paramx2<=512; paramx2++)
  for(paramx3=0; paramx3<=36; paramx3++)
  {
  convert.SetData(paramx1, paramx2, paramx3, 0.0);
  }
   
  //convert.WriteToTIFF("stack1_cutx1.tiff"); 
  convert.writeDataForDX("stack_128_test0_seg_filt.dx");   
  
  
  
  
  
  //Schnitt 1.2:
  
  convert.ReadFromTIFF("stack1_seg_filt_test_floor.tiff");
  
  for(paramx1=256; paramx1<=512; paramx1++)
  for(paramx2=0; paramx2<=512; paramx2++)
  for(paramx3=0; paramx3<=36; paramx3++)
  {
  convert.SetData(paramx1, paramx2, paramx3, 0.0);
  }
  
  convert.WriteToTIFF("stack1_cutx2.tiff"); 
  convert.writeDataForDX("stack1_cutx2.dx"); 
  
  
  //Schnitt 2.1:
  
  convert.ReadFromTIFF("stack1_seg_filt_test_floor.tiff");
  for(paramy1=0; paramy1<=512; paramy1++)
  for(paramy2=0; paramy2<=255; paramy2++)
  for(paramy3=0; paramy3<=36; paramy3++)
  {
  convert.SetData(paramy1, paramy2, paramy3, 0.0);
  }
  
  convert.WriteToTIFF("stack1_cuty1.tiff"); 
  convert.writeDataForDX("stack1_cuty1.dx"); 
  
  
  
  
  
  
  //Schnitt 2.2:
  
  convert.ReadFromTIFF("stack1_seg_filt_test_floor.tiff");
  
  for(paramy1=0; paramy1<=512; paramy1++)
  for(paramy2=256; paramy2<=512; paramy2++)
  for(paramy3=0; paramy3<=36; paramy3++)
  {
  convert.SetData(paramy1, paramy2, paramy3, 0.0);
  }
  
  convert.WriteToTIFF("stack1_cuty2.tiff"); 
  convert.writeDataForDX("stack1_cuty2.dx"); 
  
  
  
  
  

  //Schnitt 3.1:
  
  convert.ReadFromTIFF("stack1_seg_filt_test_floor.tiff");
    
  
  for(paramz1=0; paramz1<=512; paramz1++)
  for(paramz2=0; paramz2<=512; paramz2++)
  for(paramz3=0; paramz3<=18; paramz3++)
  {
  convert.SetData(paramz1, paramz2, paramz3, 0.0);
  }
  
  convert.WriteToTIFF("stack1_cutz1.tiff"); 
  convert.writeDataForDX("stack1_cutz1.dx"); 
  
  
  
  
  
  //Schnitt 3.2:
  
  convert.ReadFromTIFF("stack1_seg_filt_test_floor.tiff");
    
  
  for(paramz1=0; paramz1<=512; paramz1++)
  for(paramz2=0; paramz2<=512; paramz2++)
  for(paramz3=19; paramz3<=36; paramz3++)
  {
  convert.SetData(paramz1, paramz2, paramz3, 0.0);
  }
  
  convert.WriteToTIFF("stack1_cutz2.tiff"); 
  convert.writeDataForDX("stack1_cutz2.dx");
  

  //(*segmenDC).WriteToTIFF("stack1_seg_cut.tiff");
  //(*segmenDC).writeDataForDX("stack1_seg_cut.dx");
  
	  
  //TestCube3D sphere(SPHERE3DHOLES);     
        
 
   
  //sphere.WriteToTIFF("./numerische_optimierung/sphere3D/sphere3D_holes.tiff");           
  //plane.writeDataForDX("plane3D_holes_raw.dx");       
 
return 0; 
}
 
