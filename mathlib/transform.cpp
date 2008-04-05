#include "transform.h"
static int
EmptyCallBackFunction(int percentage)
{
    return 0;
}

Transform::
Transform(DataCube *dCube, int (*callBack) (int percentage)) : stat(dCube)
{
    callBackFunction=callBack;
  dataCube=dCube;
  
}

Transform::
Transform(DataCube *dCube) : stat(dCube)
{
    callBackFunction=EmptyCallBackFunction;
    dataCube=dCube;

}

Transform::
~Transform()
{
}

ostream &operator<<(ostream &ostr,Transform &Trans)
{
  ostr << "Transform" << endl;
    return ostr;  
}  

int 
Transform::
ScaleData(double factor)
{
  for (int i=0;i< dataCube->NrOfElements();i++)
    (*dataCube)(i)=(*dataCube)(i)*factor;
  return 0;
}

int
Transform::
AutoScaleData()
{
    double min=stat.minValue();
    addScalar(-1*min);
    double max=stat.maxValue();
    if (max>0.0) ScaleData(1/max);
    return 0;
}




int 
Transform::
Potentiate(double power)
{
  for (int i=0;i< dataCube->NrOfElements();i++)
    { 
      double tmp;
      tmp = (*dataCube)(i);
      if ( tmp < 0 ) 
	{
	  tmp = 0.0;
	  cout << "negative value in DataCube!" << endl;
	}  
      (*dataCube)(i)=pow(tmp,power);
    }
  return 0;
}

int 
Transform::
ThresholdDataLow(double lowerLimit, double lowValue)
{
  for (int i=0;i< dataCube->NrOfElements();i++)
    if ((*dataCube)(i)<lowerLimit) (*dataCube)(i)=lowValue;
  return 0;
}

int
Transform::
ThresholdDataHigh(double higherLimit, double highValue)
{
  for (int i=0;i< dataCube->NrOfElements();i++)
    if ((*dataCube)(i)>higherLimit) (*dataCube)(i)=highValue;
  return 0;
};


int  
Transform::
addScalar(int z,double sum)
{
  if (dataCube->GetDim()==2) return WRONGDIMENSION;
  for (int x=0;x< dataCube->GetSize()[1];x++)
    for (int y=0;y< dataCube->GetSize()[2];y++)
      (*dataCube)(x,y,z)+=sum;
  return 0;
}

int
Transform::
addScalar(double sum)
{
    for (int i=0;i< dataCube->NrOfElements();i++)
        (*dataCube)(i)=(*dataCube)(i)+sum;

    return 0;
}

int  
Transform::
eqalMeanValueRespZ()
{
  if (dataCube->GetDim()==2) return WRONGDIMENSION;
  
  double mean=stat.meanValue();

  for (int z=0;z <dataCube->GetSize()[3];z++)
    {
      double meanDiff=mean-stat.meanValue(z);
      addScalar(z,meanDiff);
    }
  return 0;
}

int  
Transform::
eqalMaxValueRespZ()
{
  if (dataCube->GetDim()==2) return WRONGDIMENSION;
 
  double max=stat.maxValue();

  for (int z=0;z <dataCube->GetSize()[3];z++)
    {
      double maxDiff=max-stat.maxValue(z);
      addScalar(z,maxDiff);
    }
  return 0;
}


int  
Transform::
eqalMinValueRespZ()
{
  if (dataCube->GetDim()==2) return WRONGDIMENSION;
  
  double min=stat.maxValue();

  for (int z=0;z <dataCube->GetSize()[3];z++)
    {
      double minDiff=min-stat.minValue(z);
      addScalar(z,minDiff);
    }
  return 0;
}


#define TOP     1
#define MIDDLE  0
#define BOTTOM -1
#define FRONT  1
#define BACK  -1
#define RIGHT  1
#define LEFT  -1 

double
Transform::
GetNeigbourOfVoxelNr(Neigbour n,Vector<int> vox)
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


        default: return 0.0;
    }


    return 0.0;
}


/*
double
Transform::
CheckVoxel(int x, int y, int z)
{
    double ownvalue=(*dataCube)(x,y,z);
    if (ownvalue<absolutelowthreshold) return 0;
    if (ownvalue>absolutehighthreshold) return 1;
    
    Vector<int> voxel(3);
    voxel[1]=x;
    voxel[2]=y;
    voxel[3]=z;
    double epsilon=0.05;
    int structcounter=0;
    int backgroundcounter=0;
 
    for (Neigbour n=TopFront;n<END;n=(Neigbour)(n+1) )
        if (n!=THIS)
        {
            double value=GetNeigbourOfVoxelNr(n,voxel);
            if ((ownvalue-epsilon)<value)
            {
                structcounter++;
            }
            else
            {
                backgroundcounter++;
            };
        };
    if (structcounter>1)
       if (structcounter<20)
           if (backgroundcounter>5)
                return 0.5;
    return 0;

            
}
*/


double
Transform::
CheckVoxel(int x, int y, int z)
{
    double thisvalue=(*dataCube)(x,y,z);
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
            double nvalue=GetNeigbourOfVoxelNr(n,voxel);
            if (nvalue>(mean+epsilon))
            {
                structcounter++;
            }
	}
   
	if (thisvalue>(mean+epsilon) && structcounter>1) return 0.65;
	return 0;
	               
}





int
Transform::
StructuralThresholding(double abslow, double abshigh, double eps,DataCube **target)
{
    absolutelowthreshold=abslow;
    absolutehighthreshold=abshigh;
    epsilon=eps; //epsilon=0.014;
    (*target)=new DataCube(*dataCube);
    int boudary=3;
    for (int x=boudary;x< dataCube->GetSize()[1]-boudary;x++)
        for (int y=boudary;y< dataCube->GetSize()[2]-boudary;y++)
             for (int z=boudary;z <dataCube->GetSize()[3]-boudary;z++)
                {
                 (**target)(x,y,z)=CheckVoxel(x,y,z);
                 }

    return 0;
}

int
Transform::
StructuralThresholding(double abslow, double abshigh, double eps,DataCube *target)
{
    absolutelowthreshold=abslow;
    absolutehighthreshold=abshigh;
    epsilon=eps; //epsilon=0.014;
    int boudary=3;
    target->SetAll(0.0);    
     for (int z=boudary;z <dataCube->GetSize()[3]-boudary;z++)
     {
         callBackFunction((100*z)/(dataCube->GetSize()[3]-boudary));
         for (int y=boudary;y< dataCube->GetSize()[2]-boudary;y++)
            for (int x=boudary;x< dataCube->GetSize()[1]-boudary;x++)
            {
                (*target)(x,y,z)=CheckVoxel(x,y,z);
            };
     };
    return 0;
}


int
Transform::
StructuralThresholding2D(double abslow, double abshigh, double eps,DataCube *target)
{
    absolutelowthreshold=abslow;
    absolutehighthreshold=abshigh;
    epsilon=eps; //epsilon=0.014;
    target->SetAll(0.0);
    int rangeX, rangeY, rangeZ;
    rangeX=dataCube->GetSize()[1];
    rangeY=dataCube->GetSize()[2];
    rangeZ=dataCube->GetSize()[3];
    
    for (int z=0;z <rangeZ;z++)
    {
	
	for (int y=0;y< rangeY;y++)
	{
	    //float progress=(((float) y/dataCube->GetSize()[2])+z)/dataCube->GetSize()[3]*100.0;
	    float progress=(((float) y/rangeY))*100.0;
	    callBackFunction((int) progress);
	    for (int x=0;x< rangeX;x++)
            {
                (*target)(x,y,z)=CheckVoxel(x,y,z);
            };
	};
    };
    return 0;
}

int
Transform::
GlobalThresholding(double Threshold, DataCube *target)
{
   for (int z=dataCube->GetStart()[3];z <dataCube->GetSize()[3];z++)
       for (int y=dataCube->GetStart()[2];y< dataCube->GetSize()[2];y++)
           for (int x=dataCube->GetStart()[1];x< dataCube->GetSize()[1];x++)
            {
               (*target)(x,y,z)=0.0;
               if ((*dataCube)(x,y,z)>Threshold)
                   (*target)(x,y,z)=1.0;
            };

    return 0;
}



int
Transform::
deleteCone(Vector<double> &smallP,double radiusSmall,Vector<double>bigP,double radiusBig)
{
    Vector<double> r(3);
    Vector<double> q(3);
    Vector<double> p(3);
    r=bigP-smallP;
    r.Multiply(1/Norm(r));
    q=smallP;
    double smax=bigP*r-q*r;
    double smin=0;
    double qr=q*r;
    double s,d;

    for(int z=0;z<(dataCube->GetSize())[3];z++)
        for(int x=0;x<(dataCube->GetSize())[1];x++)
            for(int y=0;y<(dataCube->GetSize())[2];y++)
            {
                p[1]=x;
                p[2]=y;
                p[3]=z;
                s=p*r-qr;
                if (s>smax | s<smin ) continue;
                d=Norm(p-(q+s*r));
                if (d< (radiusSmall-(s/smax)*(radiusSmall-radiusBig))) (*dataCube)(x,y,z)=0.0;

            }


                return 0;
}






int
Transform::
deleteBoudary(int boundarysize)
{
    for (int z=dataCube->GetStart()[3];z <dataCube->GetSize()[3];z++)
        for (int y=dataCube->GetStart()[2];y< dataCube->GetSize()[2];y++)
            for (int x=dataCube->GetStart()[1];x< dataCube->GetSize()[1];x++)
            {
                if (z-dataCube->GetStart()[3]<boundarysize) (*dataCube)(x,y,z)=0.0;
                if (dataCube->GetSize()[3]-z<boundarysize) (*dataCube)(x,y,z)=0.0;

                if (y-dataCube->GetStart()[2]<boundarysize) (*dataCube)(x,y,z)=0.0;
                if (dataCube->GetSize()[2]-y<boundarysize) (*dataCube)(x,y,z)=0.0;

                if (x-dataCube->GetStart()[1]<(boundarysize+3)) (*dataCube)(x,y,z)=0.0;
                if (dataCube->GetSize()[1]-x<boundarysize) (*dataCube)(x,y,z)=0.0;


            };
    

    return 0;
};


int 
Transform::
maxIntensityProjection(int direction, int nrOfSlices, DataCube *target)
{
    NeuraDataType max, present;
    
    
    
    if (direction==Z_DIR)
    {
    for (int z=dataCube->GetStart()[3];z <dataCube->GetSize()[3];z++)
    {
	callBackFunction((100*z)/(dataCube->GetSize()[3]));
	for (int y=dataCube->GetStart()[2];y< dataCube->GetSize()[2];y++)
	    for (int x=dataCube->GetStart()[1];x< dataCube->GetSize()[1];x++)
            {
		max=dataCube->GetData(x,y,z);
		for(int i=-nrOfSlices;i<nrOfSlices;i++)
		{
		    present=dataCube->GetData(x,y,z+i);
		    if(max<present) max=present;
		};
		target->SetData(x,y,z,max);
	    };
    };
    };
    if (direction==Y_DIR)
    {
	for (int z=dataCube->GetStart()[3];z <dataCube->GetSize()[3];z++)
	{
	    callBackFunction((100*z)/(dataCube->GetSize()[3]));
	    for (int y=dataCube->GetStart()[2];y< dataCube->GetSize()[2];y++)
		for (int x=dataCube->GetStart()[1];x< dataCube->GetSize()[1];x++)
		{
		    max=dataCube->GetData(x,y,z);
		    for(int i=-nrOfSlices;i<nrOfSlices;i++)
		    {
			present=dataCube->GetData(x,y+i,z);
			if(max<present) max=present;
		    };
		    target->SetData(x,y,z,max);
		};
	};
	
    };
    
    if (direction==X_DIR)
    {
	for (int z=dataCube->GetStart()[3];z <dataCube->GetSize()[3];z++)
	{
	    callBackFunction((100*z)/(dataCube->GetSize()[3]));
	    for (int y=dataCube->GetStart()[2];y< dataCube->GetSize()[2];y++)
		for (int x=dataCube->GetStart()[1];x< dataCube->GetSize()[1];x++)
		{
		    max=dataCube->GetData(x,y,z);
		    for(int i=-nrOfSlices;i<nrOfSlices;i++)
		    {
			present=dataCube->GetData(x+i,y,z);
			if(max<present) max=present;
		    };
		    target->SetData(x,y,z,max);
		};
	};
	
    };
    
    
    
    
    
    
 return 0;
};



