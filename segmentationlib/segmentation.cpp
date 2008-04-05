#include "segmentation.h"
static int
EmptyCallBackFunction(int percentage)
{
    return 0;
}


//default constructor
Segmentation::
Segmentation(DataCube *sourceCube, DataCube *targetCube)
  : threshold(sourceCube) , stat(sourceCube)
{
  callBackFunction=EmptyCallBackFunction;
  segDC = targetCube;

}

Segmentation::
Segmentation(DataCube *sourceCube, DataCube *targetCube, int (*callBack) (int percentage))
  : threshold(sourceCube, callBack), stat(sourceCube)
{
  callBackFunction=callBack;
  segDC = targetCube;
}





//constructor
Segmentation::
Segmentation(Vector<int> sizeSmallCube, DataCube *sourceCube, int scalar, DataCube *segmenDC, double absMinTh, double absMaxTh, double minimalBetween2)
  : threshold(sizeSmallCube, sourceCube, scalar,absMinTh, absMaxTh, minimalBetween2), stat(sourceCube)
{
  callBackFunction=EmptyCallBackFunction;
  segDC = segmenDC;

}



//destructor
Segmentation::
~Segmentation()
{
}


/*
 *Segmentation of the source Cube whether with the maximal and minimal absolute threshold or with help of the OtsuCube tDC 
 */
int
Segmentation::
SegmentateByOtsuMethod()
{


  calcualteLokalOtsuThreshold();
  for (int x=start[1];x<start[1]+size[1];x++)
    {
      callBackFunction((int)((double)100.0*x/sourceDC->GetSize()[1]));
      for (int y=start[2];y<start[2]+size[2];y++)
	{
	  for (int z=start[3];z<start[3]+size[3];z++) 
	    {
	      if (((*sourceDC)(x,y,z))<(*targetOtsuCube)(x,y,z))
		((*segDC)(x,y,z))=0.0;
	      
	      if (((*sourceDC)(x,y,z))>=(*targetOtsuCube)(x,y,z))
		((*segDC)(x,y,z))=1.0;
	    }
	}
    }
  
  return 0;  
}



/*!
 *Segmentation of the source Cube whether with the maximal and minimal absolute threshold or with help of the OtsuCube tDC 
 */
int  
Segmentation::
SegmentateByOtsuDevidingThroughTheThreshold()
{
  calcualteLokalOtsuThreshold();
  for (int x=start[1];x<start[1]+size[1];x++)
    {
      callBackFunction((int)((double)100.0*x/sourceDC->GetSize()[1]));
      for (int y=start[2];y<start[2]+size[2];y++)
	{
	  for (int z=start[3];z<start[3]+size[3];z++) 
	    {
	      
	      if (((*targetOtsuCube)(x,y,z))==0)
		{
		  ((*segDC)(x,y,z))=0.0;
		}
	      else
		
		((*segDC)(x,y,z))=(*sourceDC)(x,y,z)/(*targetOtsuCube)(x,y,z);
	    }
	}
    }
  
  return 0;  
}

/*!
 * Segmentation by structural thresholding
 */



enum Neigbour {
    //top Front/Back Right/Left
    TopFront,
    TopBack,
    TopFrontRight,
    TopRight,
    TopLeft,
    TopFrontLeft,
    TopBackLeft,
    TopBackRight,
    Top,
    //samelevel
    THIS,
    Right,
    FrontRight,
    Back,
    Left,
    FrontLeft,
    BackLeft,
    BackRight,
    Front,
    //bottom
    BottomRight,
    BottomFrontRight,
    BottomBack,
    BottomLeft,
    BottomFrontLeft,
    BottomBackLeft,
    BottomBackRight,
    BottomFront,
    Bottom,
    END
};


#define TOP     1
#define MIDDLE  0
#define BOTTOM -1
#define FRONT  1
#define BACK  -1
#define RIGHT  1
#define LEFT  -1 

double
GetNeigbourOfVoxelNr(Neigbour n,Vector<int> vox, DataCube *dataCube)
{
    switch (n)
    {

        case   TopFront		: return dataCube->GetData(vox[1]+MIDDLE,vox[2]+FRONT,vox[3]+TOP);break;
        case   TopBack		: return dataCube->GetData(vox[1]+MIDDLE,vox[2]+BACK,vox[3]+TOP);break;
        case   TopFrontRight	: return dataCube->GetData(vox[1]+RIGHT,vox[2]+FRONT,vox[3]+TOP);break;
        case   TopRight		: return dataCube->GetData(vox[1]+RIGHT,vox[2]+MIDDLE,vox[3]+TOP);break;
        case   TopLeft		: return dataCube->GetData(vox[1]+LEFT,vox[2]+MIDDLE,vox[3]+TOP);break;
        case   TopFrontLeft	: return dataCube->GetData(vox[1]+LEFT,vox[2]+FRONT,vox[3]+TOP);break;
        case   TopBackLeft	: return dataCube->GetData(vox[1]+LEFT,vox[2]+BACK,vox[3]+TOP);break;
        case   TopBackRight	: return dataCube->GetData(vox[1]+RIGHT,vox[2]+BACK,vox[3]+TOP);break;
        case   Top		: return dataCube->GetData(vox[1]+MIDDLE,vox[2]+MIDDLE,vox[3]+TOP);break;
            //samelevel
        case   THIS		: return dataCube->GetData(vox[1]+MIDDLE,vox[2]+MIDDLE,vox[3]+MIDDLE);break;
        case   Right		: return dataCube->GetData(vox[1]+RIGHT,vox[2]+MIDDLE,vox[3]+MIDDLE);break;
        case   FrontRight	: return dataCube->GetData(vox[1]+RIGHT,vox[2]+FRONT,vox[3]+MIDDLE);break;
        case   Back		: return dataCube->GetData(vox[1]+MIDDLE,vox[2]+BACK,vox[3]+MIDDLE);break;
        case   Left		: return dataCube->GetData(vox[1]+LEFT,vox[2]+MIDDLE,vox[3]+MIDDLE);break;
        case   FrontLeft	: return dataCube->GetData(vox[1]+LEFT,vox[2]+FRONT,vox[3]+MIDDLE);break;
        case   BackLeft		: return dataCube->GetData(vox[1]+LEFT,vox[2]+BACK,vox[3]+MIDDLE);break;
        case   BackRight	: return dataCube->GetData(vox[1]+RIGHT,vox[2]+BACK,vox[3]+MIDDLE);break;
        case   Front		: return dataCube->GetData(vox[1]+MIDDLE,vox[2]+FRONT,vox[3]+MIDDLE);break;
            //bottom
        case    BottomRight	: return dataCube->GetData(vox[1]+RIGHT,vox[2]+MIDDLE,vox[3]+ BOTTOM);break;
        case    BottomFrontRight: return dataCube->GetData(vox[1]+RIGHT,vox[2]+FRONT,vox[3]+ BOTTOM);break;
        case    BottomBack	: return dataCube->GetData(vox[1]+MIDDLE,vox[2]+BACK,vox[3]+ BOTTOM);break;
        case    BottomLeft	: return dataCube->GetData(vox[1]+LEFT,vox[2]+MIDDLE,vox[3]+ BOTTOM);break;
        case    BottomFrontLeft	: return dataCube->GetData(vox[1]+LEFT,vox[2]+FRONT,vox[3]+ BOTTOM);break;
        case    BottomBackLeft	: return dataCube->GetData(vox[1]+LEFT,vox[2]+BACK,vox[3]+ BOTTOM);break;
        case    BottomBackRight	: return dataCube->GetData(vox[1]+RIGHT,vox[2]+BACK,vox[3]+ BOTTOM);break;
        case    BottomFront	: return dataCube->GetData(vox[1]+MIDDLE,vox[2]+FRONT,vox[3]+ BOTTOM);break;
        case    Bottom		: return dataCube->GetData(vox[1]+MIDDLE,vox[2]+MIDDLE,vox[3]+ BOTTOM);break;

        default: return -1.0;
    }


    return -1.0;
}





double
Segmentation::
CheckVoxel(int x, int y, int z)
{
    double thisvalue=(*sourceDC)(x,y,z);
    if (thisvalue<absolutelowthreshold) return 0;
    if (thisvalue>absolutehighthreshold) return 1;
    
    Vector<int> voxel(3), meansize(3);
    voxel[1]=x;
    voxel[2]=y;
    voxel[3]=z;
    meansize[1]=2;
    meansize[2]=2; 
    meansize[3]=0;//2 test!!!
    double mean=stat.meanValue(voxel,meansize);
    int structcounter=0;
   
    for (Neigbour n=TopFront;n<END;n=(Neigbour)(n+1) )
        if (n!=THIS)
        {
            double nvalue=GetNeigbourOfVoxelNr(n,voxel, sourceDC);
            if (nvalue>(mean+epsilon))
            {
                structcounter++;
            }
	}
   
    if (thisvalue>(mean+epsilon) && structcounter>1) return 0.65;
    return 0;
    
}


int 
Segmentation::
SegmentateByStructuralThresholding(double abslow, double abshigh, double eps)
{
  absolutelowthreshold=abslow;
  absolutehighthreshold=abshigh;
  epsilon=eps; //epsilon=0.014;
  int boudary=3;
  segDC->SetAll(0.0);    
  for (int z=boudary;z <sourceDC->GetSize()[3]-boudary;z++)
    {
      
      callBackFunction((100*z)/(sourceDC->GetSize()[3]-boudary));
      for (int y=boudary;y< sourceDC->GetSize()[2]-boudary;y++)
	for (int x=boudary;x< sourceDC->GetSize()[1]-boudary;x++)
	  {
	    (*segDC)(x,y,z)=CheckVoxel(x,y,z);
	  };
    };
  return 0;
}


/*!
 * Segmentation by global thresholding
 */
int 
Segmentation::
SegmentateByGlobalThresholding(double Threshold)
{
  for (int z=sourceDC->GetStart()[3];z<sourceDC->GetSize()[3];z++)
    {
      callBackFunction((int)((double)100.0*z/sourceDC->GetSize()[3]));
      for (int y=sourceDC->GetStart()[2];y<sourceDC->GetSize()[2];y++)
	for (int x=sourceDC->GetStart()[1];x< sourceDC->GetSize()[1];x++)
	  {
	    (*segDC)(x,y,z)=0.0;
       
	    if ((*sourceDC)(x,y,z)>Threshold)
	    (*segDC)(x,y,z)=1.0;
	  };
    }
  return 0;
}

/*!
 * Segmentation by region growing
 */

int 
Segmentation::
SegmentateByRegionGrowing(RegionGrowingTypes vMode, int StopTime, int sPx, int sPy, int sPz)
{ 
 if (vMode==GrayvalueDivOtsuvalue)
  {// Calculate  Otsu Cube
   calcualteLokalOtsuThreshold();
   for (int x=start[1];x<start[1]+size[1];x++)
     {
       callBackFunction((int)((double)100.0*x/sourceDC->GetSize()[1]));
       for (int y=start[2];y<start[2]+size[2];y++)
	 {
	   for (int z=start[3];z<start[3]+size[3];z++) 
	     {
	       
	       if (((*targetOtsuCube)(x,y,z))==0)
		 {
		   ((*targetOtsuCube)(x,y,z))=0.0;
		 }
	       else
		 
		 ((*targetOtsuCube)(x,y,z))=(*sourceDC)(x,y,z)/(*targetOtsuCube)(x,y,z);
	     }
	 }
     }
   targetOtsuCube->writeRAW("../examples/tmp/afterSegmentationGV.dat","../examples/tmp/info.txt");
   RegionGrowing rg(targetOtsuCube, segDC, vMode, StopTime);
   rg.SetStartPoint(sPx,sPy,sPz);
   cout <<"before regiongrowing" << endl;
   rg.calculateRegionGrowing();
   cout <<"after regiongrowing" << endl;
   return 0;
  }
 
 if (vMode == Grayvalue)
 {
   RegionGrowing rg(sourceDC, segDC, vMode, StopTime);
   rg.SetStartPoint(sPx,sPy,sPz);
   rg.calculateRegionGrowing();
  return 0;
 }
    return 0;
}
