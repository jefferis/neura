#include "cfddatacube.h"


CfdDataCube::
~CfdDataCube()
{

}

CfdDataCube::
CfdDataCube() 
  :DataCube(3)
{
}; 



int
CfdDataCube::
LoadCfdData(char templateName[FILENAMELENGTH], int startNr, int endNr)
//useage template="SA0064%d.cfd" 1,3 = "SA00641.cfd" "SA00642.cfd" "SA00643.cfd"
{
  
  if (startNr>endNr) return 1;
  int flags=IGNOREFIRSTTHREE_X;
  int Nr=endNr-startNr;
  CFDFile *Reader;
  char dummy[FILENAMELENGTH];
  Reader=new CFDFile[Nr+1];
  for (int i=startNr;i<=endNr;i++)
    {
      sprintf(dummy,templateName,i);
      cout << "Read File:" << dummy << endl;
      int error=Reader[i-startNr].readCFDFile(dummy,flags);
      if (error) return error;
      cout << " done." << endl;
    }

  int z=0;
  for (int i=0;i<=Nr;i++)
    {
      if (Reader[0].NUMBER_OF_PIXELS_X()!=Reader[i].NUMBER_OF_PIXELS_X()) return CFDFILESDONTFIT;
      if (Reader[0].NUMBER_OF_PIXELS_Y()!=Reader[i].NUMBER_OF_PIXELS_Y()) return CFDFILESDONTFIT;
      z+=Reader[i].NUMBER_OF_PIXELS_Z();
     cout << "File Nr. " << i+1 << " has :" << Reader[i].NUMBER_OF_PIXELS_Z() << " Pixels in z direction." << endl;

    }

  start[1]=0;
  start[2]=0;
  start[3]=0;
  size[1]=Reader[0].NUMBER_OF_PIXELS_X();
  size[2]=Reader[0].NUMBER_OF_PIXELS_Y();
  size[3]=z;
  d[1]= (double) 125.0/Reader[0].NUMBER_OF_PIXELS_X()/Reader[0].ZOOM();
  d[2]= (double) 125.0/Reader[0].NUMBER_OF_PIXELS_Y()/Reader[0].ZOOM();
  d[3]=-(double) Reader[0].STEPSIZE()/1000.0;
  if (MallocCube()) return CANTMALLOC;
  
  unsigned char *tempData;
  int outerZ=0;
  for (int i=0;i<=Nr;i++)
    {
      tempData= Reader[i].DATA_POINTER();
      
  for(z=0; z<Reader[i].NUMBER_OF_PIXELS_Z(); z++) 
	for(int y=0; y<Reader[i].NUMBER_OF_PIXELS_Y(); y++) 
	  for(int x=0; x<Reader[i].NUMBER_OF_PIXELS_X(); x++) 
	    {
	      data[x+size[1]*y+size[1]*size[2]*(z+outerZ)]=tempData[x+size[1]*y+size[1]*size[2]*z];
	    }
      outerZ+=Reader[i].NUMBER_OF_PIXELS_Z();
      
    }

  for (int i=0;i<=Nr;i++)
    {
      Reader[i].closeCFDFile();  
    
    }

  if(flags & IGNOREFIRSTTHREE_X)
   for(int x=0;x<3;x++)
    for(int y=0;y<size[2];y++)
     for(int z=0;z<size[3];z++)
      (*this)(x,y,z)=0.0;
   
   delete [] Reader;
   
  return OK;
}

int
CfdDataCube::
LoadCfdData(char Name[FILENAMELENGTH])
{
  return LoadCfdData(Name, 0,0);
};

int 
CfdDataCube::
LoadCfdData(char templateName[FILENAMELENGTH], int startNr, int endNr, int *StartVector, int *SizeVector)
{
  int error,x,y,z;
  NeuraDataType *newData;
  error=LoadCfdData(templateName, startNr, endNr);
  if (error) return error;
  if (SizeVector[0]<=0 || SizeVector[0]<=0 || SizeVector[0]<=0) return WRONGPARAMETERVALUE;
  cout << "Malloc" << SizeVector[0]*SizeVector[1]*SizeVector[2] << "bytes\n";
  newData=new(NeuraDataType[SizeVector[0]*SizeVector[1]*SizeVector[2]]);
  if (newData==NULL) return NOMEMORY;

  for (x=0;x<SizeVector[0];x++)
    for (y=0;y<SizeVector[1];y++)
      for (z=0;z<SizeVector[2];z++)
	{
	  if ((x+StartVector[0]>=size[1]) || (y+StartVector[1]>=size[2]) || (z+StartVector[2]>=size[3]))
	    {
	      newData[x+y*SizeVector[0]+z*SizeVector[0]*SizeVector[1]]=0.0; 
	    }
	  else 
	    newData[x+y*SizeVector[0]+z*SizeVector[0]*SizeVector[1]]=(*this)(x+StartVector[0],y+StartVector[1],z+StartVector[2]);
	}

  size[1]=SizeVector[0];
  size[2]=SizeVector[1];
  size[3]=SizeVector[2];
  
  delete [] data;
  data=newData;

  return 0;
}

int
CfdDataCube::
LoadCfdData(char templateName[FILENAMELENGTH], int startNr, int endNr, int *StartVector, int *SizeVector,int channel)
{
    int error,x,y,z;
    NeuraDataType *newData;
    error=LoadCfdData(templateName, startNr, endNr, channel);
    if (error) return error;
    if (SizeVector[0]<=0 || SizeVector[0]<=0 || SizeVector[0]<=0) return WRONGPARAMETERVALUE;
    cout << "Malloc" << SizeVector[0]*SizeVector[1]*SizeVector[2] << "bytes\n";
    newData=new(NeuraDataType[SizeVector[0]*SizeVector[1]*SizeVector[2]]);
    if (newData==NULL) return NOMEMORY;

    for (x=0;x<SizeVector[0];x++)
        for (y=0;y<SizeVector[1];y++)
            for (z=0;z<SizeVector[2];z++)
            {
                if ((x+StartVector[0]>=size[1]) || (y+StartVector[1]>=size[2]) || (z+StartVector[2]>=size[3]))
                {
                    newData[x+y*SizeVector[0]+z*SizeVector[0]*SizeVector[1]]=0.0;
                }
                else
                    newData[x+y*SizeVector[0]+z*SizeVector[0]*SizeVector[1]]=(*this)(x+StartVector[0],y+StartVector[1],z+StartVector[2]);
            }

                size[1]=SizeVector[0];
    size[2]=SizeVector[1];
    size[3]=SizeVector[2];

    delete [] data;
    data=newData;
	 allocated=1;
	
    return 0;

}








int
CfdDataCube::
LoadCfdData(char templateName[FILENAMELENGTH], int startNr, int endNr,int channel)
{
if (startNr>endNr) return 1;
int flags=IGNOREFIRSTTHREE_X;
int Nr=endNr-startNr;
CFDFile *Reader;
char dummy[FILENAMELENGTH];
Reader=new CFDFile[Nr+1];
for (int i=startNr;i<=endNr;i++)
{
    sprintf(dummy,templateName,i);
    cout << "Read File:" << dummy << endl;
    int error=Reader[i-startNr].readCFDFile(dummy,flags,channel);
    if (error) return error;
    cout << " done." << endl;
}

int z=0;
for (int i=0;i<=Nr;i++)
{
    if (Reader[0].NUMBER_OF_PIXELS_X()!=Reader[i].NUMBER_OF_PIXELS_X()) return CFDFILESDONTFIT;
    if (Reader[0].NUMBER_OF_PIXELS_Y()!=Reader[i].NUMBER_OF_PIXELS_Y()) return CFDFILESDONTFIT;
    z+=Reader[i].NUMBER_OF_PIXELS_Z();
    cout << "File Nr. " << i+1 << " has :" << Reader[i].NUMBER_OF_PIXELS_Z() << " Pixels in z direction." << endl;

}

start[1]=0;
start[2]=0;
start[3]=0;
size[1]=Reader[0].NUMBER_OF_PIXELS_X();
size[2]=Reader[0].NUMBER_OF_PIXELS_Y();
size[3]=z;
d[1]= (double) 125.0/Reader[0].NUMBER_OF_PIXELS_X()/Reader[0].ZOOM();
d[2]= (double) 125.0/Reader[0].NUMBER_OF_PIXELS_Y()/Reader[0].ZOOM();
d[3]=-(double) Reader[0].STEPSIZE()/1000.0;
if (MallocCube()) return CANTMALLOC;

unsigned char *tempData;
int outerZ=0;
for (int i=0;i<=Nr;i++)
{
    tempData= Reader[i].DATA_POINTER();

    for(z=0; z<Reader[i].NUMBER_OF_PIXELS_Z(); z++)
        for(int y=0; y<Reader[i].NUMBER_OF_PIXELS_Y(); y++)
            for(int x=0; x<Reader[i].NUMBER_OF_PIXELS_X(); x++)
            {
                data[x+size[1]*y+size[1]*size[2]*(z+outerZ)]=tempData[x+size[1]*y+size[1]*size[2]*z];
            }
                outerZ+=Reader[i].NUMBER_OF_PIXELS_Z();

}

for (int i=0;i<=Nr;i++)
{
    Reader[i].closeCFDFile();
}

if(flags & IGNOREFIRSTTHREE_X)
for(int x=0;x<3;x++)
for(int y=0;y<size[2];y++)
for(int z=0;z<size[3];z++)
(*this)(x,y,z)=0.0;

return OK;
}



int
CfdDataCube::
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

    for(int z=0;z<(GetSize())[3];z++)	
        for(int x=0;x<(GetSize())[1];x++)	
            for(int y=0;y<(GetSize())[2];y++)
            {
                p[1]=x;
                p[2]=y;
                p[3]=z;
                s=p*r-qr;
                if (s>smax | s<smin ) continue; 
                d=Norm(p-(q+s*r));
                if (d< (radiusSmall-(s/smax)*(radiusSmall-radiusBig))) (*this)(x,y,z)=0.0;

            }
    
   
    return 0;
}






ostream &operator<<(ostream &ostr, CfdDataCube& DC)
{
  DataCube *dataCube;
  dataCube= (DataCube*) &DC;
  ostr <<   *dataCube;
  return ostr;
}
  
