
#include "statistic.h"

Statistic::
Statistic(DataCube *dCube)
{
  dataCube=dCube;
}

Statistic::
~Statistic()
{
}

ostream &operator<<(ostream &ostr,Statistic &St)
{
  
  ostr << "Statistic" << endl;
  ostr << "MaxValue:" << St.maxValue() << endl;
  ostr << "MinValue:"<< St.minValue()<< endl;
  ostr << "MeanValue:" << St.meanValue() << endl;
  ostr << "StandardDeviationd:" << St.standardDeviation() << endl;
  return (ostr);  
}  



double 
Statistic::
meanValue(Vector<int> &voxel, Vector<int> &size)
{
double mean=0;
int counter=0;
for (int z=voxel[3]-size[3];z<=voxel[3]+size[3];z++)
	for (int y=voxel[2]-size[2];y<=voxel[2]+size[2];y++)
		for (int x=voxel[1]-size[1];x<=voxel[1]+size[1];x++)
		{
		 mean=mean+dataCube->GetData(x,y,z);
		 counter++;
		}
  mean=(double)  mean/counter;
  return mean;
}








double 
Statistic::
meanValue()
{
  double mean=0;
  for (int i=0;i< dataCube->NrOfElements();i++)
    mean=mean+(*dataCube)(i);
  
  mean=(double)  mean/dataCube->NrOfElements();
  return mean;
}

double 
Statistic::
maxValue()
{
  double max=(*dataCube)(0);
  for (int i=0;i< dataCube->NrOfElements();i++)
    if (max<(*dataCube)(i)) max=(*dataCube)(i);
  
  return max;
}


double 
Statistic::
minValue()
{
  double min=(*dataCube)(0);
  for (int i=0;i< dataCube->NrOfElements();i++)
    if (min>(*dataCube)(i)) min=(*dataCube)(i);
  
  return min;
}


double 
Statistic::
standardDeviation()
{
  double mean=meanValue();
  double stdDev=0.0;

  for (int i=0;i< dataCube->NrOfElements();i++)
    stdDev=stdDev+((*dataCube)(i)-mean)*((*dataCube)(i)-mean);
  stdDev=(double)  stdDev/dataCube->NrOfElements();
  return sqrt(stdDev);
  
}



double 
Statistic::
meanValue(int z)
{
  if (dataCube->GetDim()==2) return meanValue();

  double mean=0;
  for (int x=0;x< dataCube->GetSize()[1];x++)
    for (int y=0;y< dataCube->GetSize()[2];y++)
      mean=mean+(*dataCube)(x,y,z);
  
  mean=(double)  mean/(dataCube->GetSize()[1]*dataCube->GetSize()[2]);
  return mean;
}

double 
Statistic::
maxValue(int z)
{

 if (dataCube->GetDim()==2) return maxValue();

  double max=(*dataCube)(0);
  for (int x=0;x< dataCube->GetSize()[1];x++)
    for (int y=0;y< dataCube->GetSize()[2];y++)
      if (max<(*dataCube)(x,y,z)) max=(*dataCube)(x,y,z);
  
  return max;
}


double 
Statistic::
minValue(int z)
{
  if (dataCube->GetDim()==2) return minValue();

  double min=(*dataCube)(0);
  for (int x=0;x< dataCube->GetSize()[1];x++)
    for (int y=0;y< dataCube->GetSize()[2];y++)
      if (min>(*dataCube)(x,y,z)) min=(*dataCube)(x,y,z);
  
  return min;
}


double 
Statistic::
standardDeviation(int z)
{

  if (dataCube->GetDim()==2) return standardDeviation();

  double mean=meanValue(z);
  double stdDev=0.0;

  for (int x=0;x< dataCube->GetSize()[1];x++)
    for (int y=0;y< dataCube->GetSize()[2];y++)
      stdDev=stdDev+((*dataCube)(x,y,z)-mean)*((*dataCube)(x,y,z)-mean);
  stdDev=stdDev/(dataCube->GetSize()[1]*dataCube->GetSize()[2]);
  return sqrt(stdDev);
  
}
