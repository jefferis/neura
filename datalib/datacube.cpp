#include "datacube.h"
#include "DataCubeXMLReader.h"

#include "../nraConfig.h"
#include "../nraAssert.h"

#include "nraDataCubeReader.h"
#include "nraDataCubeTIFFReader.h"

DataCube::
~DataCube()
{
  if (allocated )
     {
       delete [] data;
     }
}


DataCube::
DataCube()     
  :Volume(0)
{
  dim=0;
  flags=FLAG_INITIALIZED;
  data = 0L;
}; 


DataCube::
DataCube(const int dimL)  
  :Volume(dimL)
{
  dim=dimL;
  flags=FLAG_INITIALIZED;
  data = 0L;
}; 

/*!
 * creates a datacube with all properties of 
 * specified datacube, but WITHOUT the data of it
 */
DataCube::
DataCube( DataCube &dc )
{
  dim = dc.GetDim();
  size = dc.GetSize();
  d = dc.GetSpaceing();
  start = dc.GetStart();
  gt = dc.gt;//GeometyType see volume.h
  flags = dc.flags;

  MallocCube();
  
}


void 
DataCube::
freeMemory()
{
  allocated=0;
  delete [] data;
  data = 0L;
}


int
DataCube::
NrOfElements()
{
  if (dim==2) return size[1]*size[2];
  if (dim==3) return size[1]*size[2]*size[3];
  return -1;
}

void 
DataCube::
SetDataPointer(NeuraDataType *dP) 
{
  data=dP;
};

NeuraDataType*
DataCube::
GetDataPointer()
{
  return data; 
}

int
DataCube::
MallocCube()
{
 int fieldSize=1;
 int i;

 for (i=1;i<=dim;i++) fieldSize=fieldSize*size[i];
 data=new(NeuraDataType[fieldSize]);
  if (data==NULL) 
    {
      cout << "No Memory left! Quit!" << endl;
      return CANTMALLOC;
    }
  for (i=0;i<fieldSize;i++) data[i]=0.0;

  flags=flags |  FLAG_MEMORYALLOCATED;

  allocated = 1;
  return OK;
}

#define NOTSLEEPING 1
#define ALREADYSLEEPING 2

int 
DataCube::
sleep()
{
    if (sleeping==false) return ALREADYSLEEPING;
    int nr=NrOfElements();
    sleepingData= new (NeuraCompressionType[nr]);
    NeuraDataType fac=255.0/maxValue();
    
    for (int i=0;i<nr;i++)
    {
	//Geschwindigkeits vergleich mit pointer schieben!!
	sleepingData[i]=(NeuraCompressionType) ( (NeuraDataType) data[i]*fac) ;
    }
    sleeping=true;
    delete [] data;
    return 0;
}


int 
DataCube::
wakeup()
{
    if (sleeping==false) return NOTSLEEPING;
    int nr=NrOfElements();
    data= new (NeuraDataType[nr]);
    
    for (int i=0;i<nr;i++)
    {
	//Geschwindigkeits vergleich mit pointer schieben!!
	data[i]= (NeuraDataType) sleepingData[i]/255.0;
    }
    sleeping=false;
    delete [] sleepingData;
    return 0;
}




/*! 
 * sets all entrys to specified value 
 */
void
DataCube::
SetAll(NeuraDataType value)
{
    int i;
    int nr=NrOfElements();
    for ( i = 0; i < nr ; i++)
        data[i]=value;
        //(*this)(i) = value;
}


/*!
 * Reads datacube from a XML file.
 */

int 
DataCube::
ReadFromXML(char* strFileName)
{
  /*
    // There is a existing datacube in memory -> delete.
    if (allocated)
        freeMemory();
    CDataCubeHeader Header;
    int iRes = ReadXMLDataCube( strFileName, Header, data );
    if (iRes != 0) return iRes; // something went wrong. 
    dim = Header.GetNumOfDimensions();
    size.Reset(dim);
    d.Reset(dim);
    for( int i = 0; i < dim; i++)
    {
        size.SetElement(i+1, Header.Dimension(i).Size);  // i+1 is needed because Vector(array.h) index is 1 based.
        d.SetElement(i+1, Header.Dimension(i).GridSpaceing);
    }
    allocated = 1;
  */
    return OK;
}


/*!
 * Writes datacube to a XML file.
 */
int 
DataCube::
WriteToXML(char* strFileName)
{
  /*
    int iRes = OK;
    if(allocated)
    {
        CDataCubeHeader Header;
        char* chLabel = new char[2];
        chLabel[0] = 32;
        chLabel[1] = 0;
        Header.SetNumOfDimensions(dim);
        for( int i = 0; i < dim; i++)
        {
            Header.Dimension(i).Label = chLabel;
            Header.Dimension(i).Size = size[i+1];  // i+1 is needed because Vector(array.h) index is 1 based.
            Header.Dimension(i).GridSpaceing = d[i+1];
        }
        iRes = WriteXMLDataCube( strFileName, Header , data );
        delete [] chLabel;
    }
  
    return iRes;
  */
}


/*!
 * Reads datacube from a Tiff file.
 */
int 
DataCube::
ReadFromTIFF(char* strFileName)
{   
	    
	cout << "Start to read tiff" << endl << flush;
    nraDataCubeTIFFReader<NeuraDataType> Reader;
    try
    { 
    	cout << "Read Tiff File:" <<  strFileName << endl;
		int iRes = Reader.ReadCube(strFileName);    
		cout << "iRes" << iRes << endl<< flush; 
		if(iRes == NO_SPACEINGS_TAGS || iRes == OK)
		{
			// GJ: moved these Resets BEFORE copying Reader.Dimension into size, d
			// The reset function is not guaranteed to leave the original elements 
			// intact
			dim = 3;
			size.Reset(dim);
			d.Reset(dim);
			for(int i = 0; i < dim; i++)
			{   
	    		size.SetElement(i+1, Reader.Dimension(i).Size);   // i+1 is needed because Vector(array.h) index is 1 based.
	    		d.SetElement(i+1, Reader.Dimension(i).GridSpaceing);
			}
	    	if(allocated)
   		 	{
   		 	cout << "free" << endl<< flush;
				freeMemory();
				cout << "done" << endl<< flush; 
    		}
    		
			data = Reader.GetData();
			allocated = 1;  
			return iRes;
			//return NO_SPACEINGS_TAGS;
		}
		else if(iRes == UNABLE_TO_OPEN_TIFF)
		{
			cout << "Unable to open TIFF - file!" << endl;
			return CANTREADTIFF;
		}
		else if(iRes == TIFFSTACK_INCONSISTENT)
		{
			cout << "Slices int TIFF - stack are of different sizes!" << endl;
			return CANTREADTIFF;
		}
		else if(iRes == TIFFTYPE_NOT_SUPPORTED)
		{
			cout << "TIFF - type is not supported!" << endl;
			return CANTREADTIFF;			
		}
		else
		{
			cout << "An unknown error occured while reading a TIFF - file!" << endl;
			return CANTREADTIFF;
		}
	
    }
    catch(nraAssertion Assertion)
    {
		cout << "Error while reading from TIFF in [DataCube::ReadFromTIFF]!" << endl;
		cout << Assertion << endl;
		return CANTREADTIFF; 
    }
}

/*!
 * Writes datacube to a Tiff file.
 */
int 
DataCube::
WriteToTIFF(char* strFileName)
{
	cout << "Write Tiff File:" << strFileName << endl;
	
    nraDataCubeTIFFReader<NeuraDataType> Reader;
     
    if(allocated)
    {
		try
		{
		    for( int i = 0; i < dim; i++)
		    {
				Reader.Dimension(i).Label = "";
				Reader.Dimension(i).Size = size[i+1];  // i+1 is needed because Vector(array.h) index is 1 based.
				Reader.Dimension(i).GridSpaceing = d[i+1];
		    }
		    Reader.SetData(data);
		    
		    cout << "Write Cube" << endl << flush;
		   	if(Reader.WriteCube(strFileName) == UNABLE_TO_OPEN_TIFF)
		   	{
		    	cout << "Unable to open TIFF - file!" << endl << flush;
		    	return CANTWRITETIFF;
		    }
		    cout << "Finish Write Cube" << endl << flush;
		    return OK;
		}
		catch(nraAssertion Assertion)
		{
		    cout << "Error while writing to TIFF in [DataCube::WriteToTIFF]!" << endl;
		    cout << Assertion <<  endl;
		    return CANTWRITETIFF;
		}
    }
    
    return CANTWRITETIFF;

}

/*!
 * Writes data from source Cube to this cube,pos says where in this cube the data is set
 * start and size says which volume will be extracted
 */
int 
DataCube::
CopyDataCubeIntoDataCube(DataCube *source,Vector<int> start, Vector<int> size, Vector<int> pos)
{
for(int x=0;x<size[1];x++)	
	for(int y=0;y<size[2];y++)
   	for(int z=0;z<size[3];z++)	
			SetData(x+pos[1],y+pos[2],z+pos[3],source->GetData(x+start[1],y+start[2],z+start[3]));
			    
return 0;
}	


/*!
 * copies specified MultiVector into this DataCube
 * dimension and sizes have to accord
 */
int 
DataCube::
CopyFromMultiVector(MultiVector<NeuraDataType> &mv)
{
   for ( int i = 0; i < NrOfElements(); i++ )
    {
      (*this)(i) = mv[i+1]; 
    } 
  return OK;

}
/*!
* copies this DataCube into a MultiVector
 */
int
DataCube::
CopyIntoMultiVector(MultiVector<NeuraDataType> &mv) 
{
    mv.Reset( (*this).GetSize() );
    
    for ( int i = 0; i < NrOfElements(); i++ )
    {
	mv.Set(i+1,(*this)(i)); 
    } 
    return OK;
}

 
NeuraDataType
&DataCube::
GetData(int k)
{
#ifdef DEBUG 
  if ( dim  == 2 )
    {
      if ( ( k < 0 ) || ( k >= size[1]*size[2] ) )
	{
	  cout << "Acess out of range" << endl;
	  exit(1);
	} 
    }
  if ( dim  == 3 )
    {
      if ( ( k < 0 ) || ( k >= size[1]*size[2]*size[3] ) )
	{
	  cout << "Acess out of range" << endl;
	  exit(1);
	} 
    }
#endif
  return data[k];
}

NeuraDataType
&DataCube::
operator ()(int k)
{
#ifdef DEBUG 
  if ( dim == 2 )
    {
      if ( ( k < 0 ) || ( k >= size[1]*size[2] ) )
	{
	  cout << "Acess out of range" << endl;
	  exit(1);
	} 
    }
  
  if ( dim == 3 )
    {
      if ( ( k < 0 ) || ( k >= size[1]*size[2]*size[3] ) )
	{
	  cout << "Acess out of range" << endl;
	  exit(1);
	} 
    }
#endif
  return data[k];
}


NeuraDataType
&DataCube::
operator ()(int x, int y)
{
#ifdef DEBUG
  if (dim>2) 
    {
      cout << "Error try 2Dimension acess but 3D Data!" << endl;
      exit(1);
    }

  if ((x>=size[1])|| (y>=size[2]))
    {
      cout << "Out of range!" << endl;
      exit(1);
    }

#endif
  return data[x+size[1]*y];
}


NeuraDataType
&DataCube::
operator ()(int x, int y, int z)
{
#ifdef DEBUG
  if (dim<3) 
    {
      cout << "Error try 3Dimension acess but 2D Data!" << endl;
      exit(1); 
    }
  if ((x>=size[1])|| (y>=size[2]) || (z>=size[3]) )
    {
      cout << "Out of range!" << endl;
      cout << "Try to acess: (" << x<< ","<< y <<  ","<<z << ")"<<endl;
      exit(1);
    }
  
#endif

 return data[x+size[1]*y+size[1]*size[2]*z];
}


/*!
 * returns the datacube value at specified relative grid position
 * if it exists, else returns zero
 * needed for restiction by gridtransfer
 */
NeuraDataType
DataCube::
GetData(int x, int y, int z)
{
#ifdef DEBUG
  if (dim<3) 
    {
      cout << "Error try 3Dimension acess but 2D Data!" << endl;
      exit(1); 
    }
#endif
  if ( ( x >= size[1] ) || ( y >= size[2] ) || ( z >= size[3]) ||
       ( x < 0 ) || ( y < 0 ) || ( z < 0 ) )
    {
      return 0.0;
    }
  
  return data[x+size[1]*y+size[1]*size[2]*z];
}


/*!
 * sets the datacube value at specified relative grid position
 * if it exists, else do nothing
 * needed for create some testcubes
 */
void
DataCube::
SetData(int x, int y, int z, NeuraDataType val)
{
#ifdef DEBUG
  if (dim<3) 
    {
      cout << "Error try 3Dimension acess but 2D Data!" << endl;
      exit(1); 
    }
#endif
  if ( ( x >= size[1] ) || ( y >= size[2] ) || ( z >= size[3]) ||
       ( x < 0 ) || ( y < 0 ) || ( z < 0 ) )
    {
      ;
    }
  
  else data[x+size[1]*y+size[1]*size[2]*z] = val;
}


/*!
* sets the datacube value at specified relative grid position
 * if it exists, else do nothing
 * needed for create some testcubes
 */
void
DataCube::
SetData(int i, NeuraDataType val)
{

    if ( ( i >= size[1]*size[2]*size[3]) || ( i < 0 ) )
    {
	;
    }
    
    else data[i] = val;
}





NeuraDataType 
&DataCube::
operator[](Vector<int> X)
{
#ifdef DEBUG
  if (X.GetLength()!=dim) 
    {
      cout << "Error wrong acess vector!" << endl;
      exit(1); 
    }
  if ((X[1]>=size[1])|| (X[2]>=size[2]) || (X[3]>=size[3]) )
    {
      cout << "Out of range!" << endl;
      exit(1);
    }
  
#endif
//  if (dim==2) return data[X[1]+size[1]*X[2]]; no 2d Datacube anymore!
//  if (dim==3) better for speed!
      return data[X[1]+size[1]*X[2]+size[1]*size[2]*X[3]];
}


/*
 *The dataCube norm is diffrent to the euklidean norm due to the different spaecing!
 *
 */

double
DataCube::
DCNorm(double x, double y, double z)
{
    return sqrt( (x*d[1])* (x*d[1]) +(y*d[2])* (y*d[2])+ (z*d[3])* (z*d[3]) );
}






/*!
 * subtracts b from this elementwise
 * caution! this is changed by that
 */
int 
DataCube::
operator-(DataCube& b)
{
  int n = NrOfElements();
  if ( n != b.NrOfElements() )
    {
      cout << "DataCube:operator-: DataCube must have the same number of Elements!" << endl;
      exit(1);
    }
  for ( int i = 0; i < n; i++ )
    {
      (*this)(i) -= b(i);
    }
  return OK;
} 

/*!
 * Sets all elements of this datacube to that of specified datacube
 */
int
DataCube::
operator=(DataCube& dc)
{
  int n = NrOfElements();
  if ( n != dc.NrOfElements() )
    {
      cout << "DataCube:operator=: DataCube must have the same number of Elements!" << endl;
      exit(1);
    }
  /*  
  for ( int i = 0; i < n; i++ )
    {
      (*this)(i) = dc(i);
    }
    */
    	
    NeuraDataType *source=dc.GetDataPointer();
    NeuraDataType *target=data;
    //This should be faster
    memcpy(target,source,n*sizeof(NeuraDataType));	
    
  return OK;
}
 

ostream &operator<<(ostream &ostr, const DataCube &DC)
{
  ostr << "Datacube" << endl;
  ostr << "Dimension:" << DC.dim << endl;
  ostr << "Size:" << endl << DC.size << endl;
  ostr << "d:" << endl << DC.d << endl;
  ostr << "Flags:" << DC.flags << endl;

  return (ostr);  
}  

int 
DataCube::
CalcualteMinAndMax(NeuraDataType &min,NeuraDataType &max)
{   
    int maxNr=this->NrOfElements();
    max=(*this)(0);
    min=(*this)(0);
    for (int i=0;i<maxNr;i++)
    {
	if (max<(*this)(i)) {max=(*this)(i);continue;};
	if (min>(*this)(i)) {min=(*this)(i);continue;};
    }
    return max;
}


NeuraDataType 
DataCube::
maxValue()
{
  double max=(*this)(0);
  for (int i=0;i< this->NrOfElements();i++)
    if (max<(*this)(i)) max=(*this)(i);
  
  return max;
}


NeuraDataType
DataCube::
minValue()
{
  double min=(*this)(0);
  for (int i=0;i< this->NrOfElements();i++)
    if (min>(*this)(i)) min=(*this)(i);
  
  return min;
}


int 
DataCube::
writeSlicePPM (char *filename,int sliceNr, int direction)
{
  if (dim==2) return writePPM2D(filename);
  // int num;
  FILE *fptr;
  unsigned char outData;
  double currentData;

  double min = minValue();
  double max = maxValue();
  
  if (filename == NULL)
    fptr = stdout;
  else
    fptr = fopen(filename, "w");

  /* header for PPM file */
  fprintf(fptr, "P6\n");
  
  if (direction==Z_DIR)
    {
      fprintf(fptr, "%d %d\n", size[1], size[2]);
      fprintf(fptr, "255\n");
      /* writing out actual image bytes */
      for (int y = 0; y < size[2];  y++) {
	for (int x = 0; x < size[1]; x++) {
	  currentData = (*this)(x,y,sliceNr);
	  outData = (unsigned char) ( 255.0 * ( currentData - min )/( max - min ) );
	  fwrite(&outData, 1, sizeof(unsigned char), fptr);
	  fwrite(&outData, 1, sizeof(unsigned char), fptr);
	  fwrite(&outData, 1, sizeof(unsigned char), fptr);
	}
      }
    }
  
  if (direction==Y_DIR)
    {
      fprintf(fptr, "%d %d\n", size[1], size[3]);
      fprintf(fptr, "255\n");
      /* writing out actual image bytes */
      for (int z = 0; z < size[3];  z++) {
	for (int x = 0; x < size[1]; x++) {
	  currentData = (*this)(x,sliceNr,z);
	  outData = (unsigned char) (255.0 * ( currentData - min )/( max - min ) );
	  fwrite(&outData, 1, sizeof(unsigned char), fptr);
	  fwrite(&outData, 1, sizeof(unsigned char), fptr);
	  fwrite(&outData, 1, sizeof(unsigned char), fptr);
	}
      }
    }

 if (direction==X_DIR)
    {
      fprintf(fptr, "%d %d\n", size[2], size[3]);
      fprintf(fptr, "255\n");
      /* writing out actual image bytes */
      for (int y = 0; y < size[2];  y++) {
	for (int z = 0; z < size[3]; z++) {
	  currentData = (*this)(sliceNr,y,z);
	  outData = (unsigned char) (255.0 * ( currentData - min )/( max - min ) );
	  fwrite(&outData, 1, sizeof(unsigned char), fptr);
	  fwrite(&outData, 1, sizeof(unsigned char), fptr);
	  fwrite(&outData, 1, sizeof(unsigned char), fptr);
	}
      }
    }

 return 0;
};

int 
DataCube::
writeSlicesPPM (char *filename,int direction)
{
char  buffer[FILENAMELENGTH];

if (direction==X_DIR)
    {
   for (int x=0;x<size[1];x++)
      {
      sprintf(buffer,filename,x);   
      writeSlicePPM (buffer,x, X_DIR);
      }
   }


if (direction==Y_DIR)
    {
   for (int y=0;y<size[2];y++)
      {
      sprintf(buffer,filename,y);   
      writeSlicePPM (buffer,y, Y_DIR);
      }
   }


if (direction==Z_DIR)
    {
   for (int z=0;z<size[3];z++)
      {
      sprintf(buffer,filename,z);   
      writeSlicePPM (buffer,z,Z_DIR);
      }
   }
return 0;
}




int
DataCube::
writeSlicePPM (char *filename,int sliceNrStart,int sliceNrEnde, int direction)
{
 char  buffer[FILENAMELENGTH];

if (direction==X_DIR)
   {
    if (sliceNrStart<0 ||sliceNrEnde >=size[1])
	{
	  cerr<<"datacube:: writeSlicePPM, out of range"<<endl;
	  return 1;
	};  
 
    for (int x=sliceNrStart;x<sliceNrEnde;x++)
      {
      sprintf(buffer,filename,x);   
      writeSlicePPM (buffer,x, X_DIR);
      }
   }


if (direction==Y_DIR)
   {
       if (sliceNrStart<0 ||sliceNrEnde >=size[2])
	{
	  cerr<<"datacube:: writeSlicePPM, out of range"<<endl;
	  return 1;
	};  

      for (int y=sliceNrStart;y<sliceNrEnde;y++)
      {
      sprintf(buffer,filename,y);   
      writeSlicePPM (buffer,y, Y_DIR);
      }
   }


if (direction==Z_DIR)
   {
      if (sliceNrStart<0 ||sliceNrEnde >=size[3])
	{
	  cerr<<"datacube:: writeSlicePPM, out of range"<<endl;
	  return 1;
	};  
      for (int z=sliceNrStart;z<sliceNrEnde;z++)
      {
      sprintf(buffer,filename,z);   
      writeSlicePPM (buffer,z,Z_DIR);
      }
   }
return 0; 
}


int 
DataCube::
writePPM2D (char *filename) {
  //int num;
  FILE *fptr;
  unsigned char outData;
  
  if (filename == NULL)
    fptr = stdout;
  else
    fptr = fopen(filename, "w");

  /* header for PPM file */
  fprintf(fptr, "P6\n");
  fprintf(fptr, "%d %d\n", size[1], size[2]);
  fprintf(fptr, "255\n");

  /* writing out actual image bytes */
  for (int y = 0; y < size[2];  y++) {
    for (int x = 0; x < size[1]; x++) {
      outData = (unsigned char) (255.0 *(*this)(x,y));
      fwrite(&outData, 1, sizeof(unsigned char), fptr);
      fwrite(&outData, 1, sizeof(unsigned char), fptr);
      fwrite(&outData, 1, sizeof(unsigned char), fptr);
    }
  }
  return 0;
}
  
/*!
 * prints this datacube and all its values
 */
void
DataCube::
print()
{
  int i;
  //cout << (*this);
  for ( i = 0; i < NrOfElements(); i++ )
    {
      cout << (*this)(i) << " ";
    }
  cout << endl;
}



int 
DataCube::
writeRAW(char *name,char *oldInfo)
{
char info[150];
sprintf(info,"%s.info.txt",name);
double max=maxValue();
if (max<0.0001) max=1.0;
double scalefactor=255.0/max;

int x,y,z;
FILE *datafile;
datafile=fopen(name,"wb");
if (datafile==NULL)
	return CANTWRITERAW;

ofstream infoFile(info);
double help;
unsigned char convert;
for(z=0;z<(GetSize())[3];z++)
    for(y=(GetSize())[2]-1;y>=0;y--)
        for(x=0;x<(GetSize())[1];x++)	
        {
            help=((double)(*this)(x,y,z)*scalefactor);
            if (help>255) help=255;
            if (help<0) help=0;
            convert= (unsigned char) help;
            fwrite(&convert,sizeof(unsigned char)*1,1,datafile);
        };

fclose(datafile);
infoFile << "Start:" << (GetStart()) << endl;
infoFile << "Size:" << (GetSize()) << endl;
infoFile << "Spaceing:" << (GetSpaceing()) << endl;
return 0;
}



//Caution! to use this function, the datacube must allready be initalized and the memory has to be allocated!
int
DataCube::
readRAW(char *name)
{
    int x,y,z;
    FILE *datafile;
    datafile=fopen(name,"r");
    if (datafile==NULL)
            return CANTREADRAW;
    
    double help;
    unsigned char convert;
    for(z=0;z<(GetSize())[3];z++)
        for(y=(GetSize())[2]-1;y>=0;y--)
            for(x=0;x<(GetSize())[1];x++)
            {
                if (fread(&convert,sizeof(char)*1,1,datafile)!=1) return 1;
                help=convert;
                if (help>255) help=255;
                if (help<0) help=0;
                //help=(255.0-help)/255.0;
                help=help/255.0;
                (*this)(x,y,z)=help;
                //if (x<20) cout << help << endl; 
               
                
            };

    fclose(datafile);
    return 0;
}





int
DataCube::
writeRAWZXSwitched(char *name,char *info)
{
    int x,y,z;
    double max=maxValue();
    if (max=0.000001) max=1;
    double scalefactor=255.0/max;

    FILE *datafile;
    datafile=fopen(name,"wb");
    if (datafile==NULL)
		return CANTWRITERAW;
    ofstream infoFile(info);

    unsigned char convert;
    for(x=0;x<(GetSize())[1];x++)
        for(y=0;y<(GetSize())[2];y++)
            for(z=0;z<(GetSize())[3];z++)
            {

                convert= (unsigned char)((double)(*this)(x,y,z)*scalefactor);
                fwrite(&convert,sizeof(unsigned char)*1,1,datafile);
            };

    fclose(datafile);
    infoFile << "Size:" << (GetSize()) << endl;
    infoFile << "Spaceing:" << (GetSpaceing()) << endl;
    infoFile << "z,x switched so data follows like this z,y,x" << endl;
    return 0;
}





const char FileTemplate[100] = "file=%s\n";
const char GridTemplate[100] = "grid = %d  x %d  x %d\n";
const char StructureTemplate[250] = "format = msb ieee \ninterleaving = record \nmajority = row \nfield = field0 \nstructure = scalar \ntype = unsigned byte\ndependency = positions\n";
const char PositionsTemplate[200] = "positions = regular, regular, regular, 0, %f , 0, %f, 0, %f \n \n end\n";


int 
DataCube::
writeDataForDX(char *name)
{
    char buffer[500];
    sprintf(buffer,"%s.info.txt",name);
    writeRAWZXSwitched(name,buffer);
    sprintf(buffer,"%s.general",name);
    ofstream generalFile(buffer);
    sprintf(buffer,FileTemplate,name);
    generalFile<<buffer;
    sprintf(buffer,GridTemplate,(GetSize())[1],(GetSize())[2],(GetSize())[3]);
    generalFile<<buffer;
    generalFile<<StructureTemplate;
    sprintf(buffer,PositionsTemplate,(GetSpaceing())[1],(GetSpaceing())[2],(GetSpaceing())[3]);
    generalFile<<buffer;
    return 0;
}

int 
DataCube::
writeToVrend(char* name)
{
    double max=maxValue();
    if (max<0.0001) max=1.0;
    double scalefactor=255.0/max;

    int x,y,z;
    FILE *datafile;
    datafile=fopen(name,"wb");
    if (datafile==NULL)
        return CANTWRITERAW;
    //write Header
    //Size of File
    unsigned char out[4];
    for(int i=1;i<=3;i++)
    {
        
        out[0]=(unsigned char) ((int)  size[i] & 255);
        out[1]=(unsigned char) ((int) (size[i] & 768)/256);
        out[2]=0;
        out[3]=0;
        if (fwrite(out,1,4,datafile)!=4) return CANTWRITERAW;
    }

    //Spceing
    for(int i=1;i<=3;i++)
    {

        out[0]=(unsigned char) ((double) d[i] * 10);
        //if (i<3) {out[0]=1;} else {out[0]=2;};
        out[1]=0;
        out[2]=0;
        out[3]=0;
        if (fwrite(out,1,4,datafile)!=4) return CANTWRITERAW;
    }
    


    //write Data
    double help;
    unsigned char convert;
    for(z=0;z<(GetSize())[3];z++)
        for(y=(GetSize())[2]-1;y>=0;y--)
            for(x=0;x<(GetSize())[1];x++)
            {
                help=((double)(*this)(x,y,z)*scalefactor);
                if (help>255) help=255;
                if (help<0.0) help=0.0;
                convert= (unsigned char) help;
                fwrite(&convert,sizeof(unsigned char)*1,1,datafile);
            };

    fclose(datafile);
    
    return 0;
}




#define NIHMAGICNUMBERSGI   1001
#define NIHMAGICNUMBERLINUX 1002

int
DataCube::
ReadFromNIH(char* name)
{
   
    if (allocated)
        freeMemory();
    cout << "Read NIH file" << endl<< flush;
    FILE *datafile;
    datafile=fopen(name,"rb");
    if (datafile==NULL)
    {
        cout << "Cant open file" << endl<< flush;
        return CANTREADNIHFILE;
    }
    int magicNumber;
    if (fread(&magicNumber,sizeof(int),1,datafile)!=1)
    {
        cout << "wrong magic number" << endl<< flush;
        return NONIHFILE;
    };
 
   
#ifdef sgi
    if (magicNumber!=NIHMAGICNUMBERSGI) cout << "Warning, this is not a sgi nih file" << endl;
#endif
    
#ifdef linux
    if (magicNumber!=NIHMAGICNUMBERLINUX) cout << "Warning, this is not a linux nih file" << endl;
#endif
    



    for (int i=1;i<=3;i++)
    {
        int readData;
        if (fread(&readData,sizeof(int),1,datafile)!=1)
        {
            cout << "Failed to read start" << endl;
            return NONIHFILE;
        };
        start[i]=readData;
    };
    cout << "Start:" << start << endl << flush;
  
    for (int i=1;i<=3;i++)
    {
        int readData;
        if (fread(&readData,sizeof(int),1,datafile)!=1)
        {
            cout << "Failed to read size" << endl;
            return NONIHFILE;
        };
        size[i]=readData;
        if (readData<0 || readData>10000)
        {
            cout << "Failed to read size, size too big" << endl;
            return NONIHFILE;
        };
    };
    cout << "Size:" << size << endl << flush;

    //
    for (int i=1;i<=3;i++)
    {
        double readData;
        if (fread(&readData,sizeof(double),1,datafile)!=1)
        {
            cout << "Failed to read spaceing" << endl;
            return NONIHFILE;
        };
        
        d[i]=readData;
        if (readData<0) {
            cout << "Failed to read spaceing" << endl;
            return NONIHFILE;
        };
    };
    if (MallocCube()) return 1;
    for (int i=0;i<NrOfElements();i++)
    {
	double help;
	if ( (int)fread(&help,sizeof(double),1,datafile)!=1)
	{
	    cout << "DataMissing" << endl;
	    return DATAMISSING;
	};
	(*this)(i)=(NeuraDataType) help;
    }
    fclose(datafile);
        
    return 0;
};



int
DataCube::
WriteToNIH(char* name)
{
   // int x,y,z;
    FILE *datafile;
    datafile=fopen(name,"wb");
    if (datafile==NULL)
		return CANTWRITERAW;
    int magicNumber=NIHMAGICNUMBERLINUX;
#ifdef sgi
    magicNumber=NIHMAGICNUMBERSGI;
#endif
#ifdef linux
    magicNumber=NIHMAGICNUMBERLINUX;
#endif
    fwrite(&(magicNumber),sizeof(int),1,datafile);
    for (int i=1;i<=3;i++) fwrite( &(start[i])  ,sizeof(int)*1,1,datafile);
    for (int i=1;i<=3;i++) fwrite( &(size[i])  ,sizeof(int)*1,1,datafile);
    for (int i=1;i<=3;i++) fwrite( &(d[i])  ,sizeof(double)*1,1,datafile); 
    for (int i=0;i<NrOfElements();i++)
    {
	double help=(NeuraDataType) (*this)(i);
	fwrite(&help,sizeof(double),1,datafile);
    }
    
    fclose(datafile);
    return 0;
}
;



ostream &operator<<(ostream &ostr,DataCube &dC)
{
  ostr << "DataCube Volume:" << ( (Volume)dC) ;
  ostr << "Flags:" << dC.flags << endl; 
  ostr << "Allocated:" << dC.allocated << endl; 
  return ostr;

}





