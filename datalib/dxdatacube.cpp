#include "dxdatacube.h"

DXDataCube::
~DXDataCube()
{
  
}

DXDataCube::
DXDataCube() : CfdDataCube()  
  //  :QmCfdDataCube()
{
  
}; 

int  
DXDataCube::
SetCubeToDX(int LposInKnt,int LpDim,int *LpCounts,float *Lorigin,float *Ldeltas,  int LinputCount,double *LinputData,int LoutputCount,double *LoutputData,int flags)
  //this member functions sets cube constanz according to DX Parameter
{
  int i;
  posInKnt=LposInKnt;     
  pDim=LpDim;         
  pCounts=LpCounts;     
  origin=Lorigin;    
  deltas=Ldeltas;    
  
  inputCount=LinputCount;   
  inputData=LinputData; 
  
  outputCount=LoutputCount;  
  outputData=LoutputData;
  
  for (i=0;i<pDim;i++)
    {
      start[i+1]= (int) origin[i];
      size[i+1]=LpCounts[i];
      d[i+1]=deltas[i+i*LpDim];
    }

  return MallocCube();
}

void
DXDataCube::
transferDXtoCube()
{
  if (pDim==2)
    {
      for(int i=0;i<inputCount;i++)
	data[i]=(double) inputData[i];
    }
  if (pDim==3)
    {
      cout << "Writing 3D Data from DX to Cube" << endl;
      cout << "Size: " << pCounts[0] << "x" << pCounts[1] << "x" << pCounts[2] << endl;
      for (int x=0;x<pCounts[0];x++)
	for (int y=0;y<pCounts[1];y++)
	   for (int z=0;z<pCounts[2];z++)
	     (*this)(x,y,z)= inputData[z+y*pCounts[2]+x*pCounts[2]*pCounts[1]];
    }
       
}


void
DXDataCube::
transferCubetoDX()
{

 if (pDim==2)
    {
      for(int i=0;i<inputCount;i++)
	inputData[i]=(double)data[i];
    }
  if (pDim==3)
    {cout << "Writing 3D Data from Cube to DX" << endl;
      cout << "Size: " << pCounts[0] << "x" << pCounts[1] << "x" << pCounts[2] << endl;
      for (int x=0;x<pCounts[0];x++)
	for (int y=0;y<pCounts[1];y++)
	   for (int z=0;z<pCounts[2];z++)
	     {
	       outputData[z+y*pCounts[2]+x*pCounts[2]*pCounts[1]]=(*this)(x,y,z);
	       
	     }
    }
      

}



ostream &operator<<(ostream &ostr, const DXDataCube &DC)
{
  DataCube *dataCube;
  dataCube= (DataCube*) &DC;
  ostr << "DXDatacube" << endl;
  ostr <<   *dataCube << endl; 
  ostr << "Dimension:" << DC.pDim << "  ";
  ostr << "#Positions:" <<  DC.posInKnt<< "  " << endl;
  ostr <<  "i   " <<  "origin  " << " length  "<< "delta  " << endl;
  for (int i=0;i< DC.pDim;i++)
    ostr << i << " " <<  DC.origin[i] << " " <<  DC.pCounts[i] <<  DC.deltas[i] << endl;
  ostr << "Total Nr. of input:" <<  DC.inputCount << " " << "TotalNr of outpu:" <<  DC.outputCount<< endl;
  return (ostr);  
}  
