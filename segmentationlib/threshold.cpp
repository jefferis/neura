#include "threshold.h"
static int
EmptyCallBackFunction(int percentage)
{
    return 0;
}


threshold::
threshold(DataCube *sourceCube,int (*callBack) (int percentage)): sizeSC(3), size(3), start(3)
{

  callBackFunction=callBack;
  sizeSC[1]=6;
  sizeSC[2]=6;
  sizeSC[3]=6;

  k=100;
  MinThreshold = 0.1;
  MaxThreshold = 0.8;
  minBetween2= 1.0;
  
  sourceDC = sourceCube;
  size=sourceDC->GetSize();
  start=sourceDC->GetStart();
  
  
  targetOtsuCube=new DataCube(*sourceDC);
}




//default constructor
threshold::threshold(DataCube *sourceCube): sizeSC(3), size(3), start(3)
{
  callBackFunction=EmptyCallBackFunction;
  sizeSC[1]=6;
  sizeSC[2]=6;
  sizeSC[3]=6;

  k=100;
  MinThreshold = 0.1;
  MaxThreshold = 0.8;
  minBetween2= 1.0;
  
  sourceDC = sourceCube;
  size=sourceDC->GetSize();
  start=sourceDC->GetStart();
  
  
  targetOtsuCube=new DataCube(*sourceDC);
}

//constructor
threshold::threshold(Vector<int> sizeSmallCube, DataCube *cube, int scalar, double absMinTh, double absMaxTh, double minimalBetween2): sizeSC(3), size(3), start(3)
{
  callBackFunction=EmptyCallBackFunction;
  

  sizeSC=sizeSmallCube;
  
  k=scalar;
  MinThreshold = absMinTh;
  MaxThreshold = absMaxTh;
  minBetween2= minimalBetween2;
  
  sourceDC = cube;
  size=sourceDC->GetSize();
  start=sourceDC->GetStart();
  
  
  targetOtsuCube=new DataCube(*sourceDC);
}







//destructor
  threshold::~threshold()
{
  

}

DataCube* threshold::getOtsuCube()
 {
   return targetOtsuCube;
 }




//Calculation: mean of the field below ( m[0]) the threshold T and abow ( m[1] )
int threshold::calculateMean(double *mean, int *nFeld, double *pFeld, int T)
{
  int n=0;
  
  mean[0]=0.0;
  mean[1]=0.0;
    
  for (int i=0; i<T; i++)
    {
      mean[0]=mean[0]+i*nFeld[i];
      n=n+nFeld[i];
    }
  if (n!=0)
  mean[0]=mean[0]/(double)n;
   
  n=0;
  for (int i=T; i<=k; i++)
    {
      mean[1]=mean[1]+i*nFeld[i];
      n=n+nFeld[i];
    }
  if (n!=0)
  mean[1]=mean[1]/(double)n;
  
  return 0;
}



//Calculation of local fields: nField: number of pixel belonging to one colour
//                             pField: probability distribution of one colour 
int threshold::calculateFields(int *nFeld, double *pFeld, Vector<int> middle)
{
  int x,y,z,a;
  int dim=0;
  Vector<int> start(3);
  
  for (int i=1; i<=3; i++)
    start[i]=middle[i]-sizeSC[i]/2;
  
  for (int i=0; i<=k; i++)
    {
      pFeld[i]=0.0;
      nFeld[i]=0;
    }
  
   for (x=start[1];x<start[1]+sizeSC[1];x++)
    {
      for (y=start[2];y<start[2]+sizeSC[2];y++)
	{
	  for (z=start[3];z<start[3]+sizeSC[3];z++) 
	    {
	      dim=dim+1;
	      a=(int)((double)((*sourceDC)(x,y,z))*k);
	      nFeld[a]++;
	    }
	}
    }
  

 
  for (int i=0; i<=k; i++)
    {
      if (dim==0)
	pFeld[i]=0;
      else
      pFeld[i]=(double)nFeld[i]/(double)dim;
    }

  return 0;
}


//Calculation of the sum of probabilities below n[0] and abow n[1] the threshold T
int threshold::calculateN(double *pFeld, double *n,int T)
{
  n[0]=0.0;
  n[1]=0.0;
  
  for (int i=0; i<T; i++)
    {
      n[0]=n[0]+pFeld[i];
    }

  for (int i=T; i<=k; i++)
    {
      n[1]=n[1]+pFeld[i];
    }
  return 0;
}


//calculation of the between-class which has to be maximized for the threshold
int threshold::calculateBetween(double *n, double *mean, int T, double &between2)
{
  between2=n[0]*n[1]*(mean[0]-mean[1])*(mean[0]-mean[1]);
  return 0;
}


//old -running- version (slow)
int threshold::oldVersion(int *nFeld,double *pFeld, int &T)
{
  double between2, Between=0.0;
  double n[2], mean[2];
  T=0;
  
  
  for (int t=0; t<=k; t++)
      {
	calculateN(pFeld, n,t);
	calculateMean(mean, nFeld,pFeld, t);
	calculateBetween(n,mean,t,between2);
	if (between2>Between)
	  {
	    Between = between2;
	    T=t;
	  };
      }
  
  if (Between <  minBetween2)
    {
      //cout<<"Hallo";
      if (mean[0]<MaxThreshold)
      	T=k;
      else T=0;
    }
  

  return 0;
}


// optimated Version: Otsu Method
int threshold::optVersion(int *nFeld,double *pFeld, int &T)
{
  double between2, Between=0.0;
  double n[2],nHelp[2],mean[2],meanHelp[2];
  double eps=10E-12;  

  calculateMean(mean, nFeld,pFeld,0);
  calculateN(pFeld, n,0);
  calculateBetween(n,mean,0,between2);
  Between=between2;
  T=0;
  //int h=0;  
  
  
  for (int t=1;t<=k; t++)
    {
      nHelp[0]=n[0]+pFeld[t-1];
      nHelp[1]=n[1]-pFeld[t-1];
           
       if (nHelp[0]>eps && nHelp[1]>eps)
	 {
	  meanHelp[0]=(mean[0]*n[0]+pFeld[t-1]*(t-1))/nHelp[0];
	  meanHelp[1]=(mean[1]*n[1]-pFeld[t-1]*(t-1))/nHelp[1];

	  mean[0]=meanHelp[0];
 	  mean[1]=meanHelp[1];
	
	  n[0]=nHelp[0];
	  n[1]=nHelp[1];
	 }
       
else
  {
    n[0]=nHelp[0];
    n[1]=nHelp[1];

    calculateMean(mean, nFeld,pFeld,t);
  }
      

      calculateBetween(n,mean,t,between2);
      if (between2>Between)
	{
	  Between = between2;
	  T=t;
	};
    }
    

    if (Between <  minBetween2)
    {
      //cout<<"Hallo";
      if (mean[0]<k*MaxThreshold)
      	T=(int)((double)k*MinThreshold);
      else T=(int) ((double)k*MaxThreshold);
    }
    
  
  return 0;
}



// calculation of the threshold of small cubes - respects marginal problems 
int threshold::calculateLokalThreshold(Vector<int> middleSC, int &T)
{
  double *pFeld;
  int *nFeld;
  Vector<int> helpSC(3);
 
  pFeld=new double[k+1];
  nFeld=new int[k+1];

  for (int i=1; i<=3; i++)
    helpSC[i]=middleSC[i];

  if (middleSC[1]>=(start[1]+size[1]-sizeSC[1]/2))
    helpSC[1]=start[1]+size[1]-sizeSC[1]/2-1;
  if (middleSC[2]>=(start[2]+size[2]-sizeSC[2]/2))
    helpSC[2]=start[2]+size[2]-sizeSC[1]/2-1; 
  if (middleSC[3]>=(start[3]+size[3]-sizeSC[3]/2))
    helpSC[3]=start[3]+size[3]-sizeSC[1]/2-1;
  if (middleSC[1]<(start[1]+sizeSC[1]/2))
   helpSC[1]=start[1]+sizeSC[1]/2;
  if (middleSC[2]<(start[2]+sizeSC[2]/2))
    helpSC[2]=start[2]+sizeSC[2]/2;
  if (middleSC[3]<(start[3]+sizeSC[3]/2))
    helpSC[3]=start[3]+sizeSC[3]/2;
  
  calculateFields(nFeld, pFeld,helpSC);
 // int TProbe;
  optVersion(nFeld, pFeld, T);
  
  delete [] pFeld;
  delete [] nFeld;
  
  return 0;
}


// Design of the gatter
int 
threshold::
calcualteLokalOtsuThreshold()
{
cout <<"calculation of Otsuthreshold" << endl;
  Vector<int> middleSmallCube(3);
  int T;
  int counter=0;
  
  for (int x=start[1]-sizeSC[1]/2;x<start[1]+size[1]-sizeSC[1]/2;x=x+sizeSC[1]/2)
    {
      callBackFunction(2*x/sourceDC->GetSize()[1]);
      for (int y=start[2]-sizeSC[2]/2;y<start[2]+size[2]-sizeSC[2]/2;y=y+sizeSC[2]/2)
	{
	  for (int z=start[3]-sizeSC[3]/2;z<start[3]+size[3]-sizeSC[3]/2;z=z+sizeSC[3]/2) 
	    {
	     
	      middleSmallCube[1]=x+sizeSC[1]/2;
	      middleSmallCube[2]=y+sizeSC[2]/2;
	      middleSmallCube[3]=z+sizeSC[3]/2;
	     
	      counter++;
	      
	      
	      calculateLokalThreshold(middleSmallCube, T);
	       if ((double)T/(double)k<MinThreshold)
		{
		  (*targetOtsuCube)(middleSmallCube[1], middleSmallCube[2], middleSmallCube[3])=MinThreshold;
		  continue;
		}
	      
	      if ((double)T/(double)k>MaxThreshold)
		{
		  (*targetOtsuCube)(middleSmallCube[1], middleSmallCube[2], middleSmallCube[3])=0.0;
		  continue;
		}

   	      (*targetOtsuCube)(middleSmallCube[1], middleSmallCube[2], middleSmallCube[3])=(double)T/(double)k;
	    }
	}
    }
  
  interpolation inst(sizeSC, targetOtsuCube);
  inst.interpolateDataCube();
  
  return 0;
}

