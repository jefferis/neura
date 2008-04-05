#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <math.h>

//BMP Reader
#include <image.h>
#include <bmp.h>


extern "C" 
{
#include "moments.h"
#include "diffusion.h"
#include "segmentation.h"
#include "distancemap.h"
#include "morphology.h"
#include "thining.h"
#include "tophat.h"
#include "datatypes.h"
}

char *versionString="createskeleton v 0.1";

/**
* OS specific
 **/
#define OS_UNIX 1
#define USE_UNIX

#ifdef USE_UNIX
#   define OS OS_UNIX
#else
#   define OS OS_WINNT
#endif

int invertDensityData(densityDataType data)
{

for (int y=0;y<data.sizeY;y++)
for (int x=0;x<data.sizeX;x++)
{
    float d=ACCESSDATAATPOINT(data,x,y);
    ACCESSDATAATPOINT(data,x,y)=1.0-d;
};

return 0;
};

int imageDataToDensityData(Image *img, densityDataType *imageDensityData)
{
//Euklidean map data
(*imageDensityData).sizeX=img->width;
(*imageDensityData).sizeY=img->height;
(*imageDensityData).data=(float*) malloc(sizeof(float)*(*imageDensityData).sizeX*(*imageDensityData).sizeY);

for(int y=0;y<img->Height();y++)
for (int x=0;x<img->Width();x++)
{
    Pixel myPix=img->GetPixel(x,y);
    float help=myPix.g/255.0;
    if (help>1.0) help=1.0;
    if (help<0.0) help=0.0;
    ACCESSDATAATPOINT((*imageDensityData),x,y)=help;
}

return 0;
}

int densityDataToImageData(densityDataType imageDensityData, Image *img)
{
//Euklidean map data
    
    for(int y=0;y<img->height;y++)
	for (int x=0;x<img->width;x++)
	{
	    Pixel myPix=img->GetPixel(x,y);
	    float help=ACCESSDATAATPOINT(imageDensityData,x,y)*255.0;
	    if (help>255.0) help=255.0;
	    if (help<0.0) help=0.0;
	    (img->GetPixel(x,y)).Set (myPix.r, (unsigned char) help , myPix.b);
	}
	    
	    return 0;
}

int binaryDataToImageData(ucharDataType imageBinaryData, Image *img)
{
//Euklidean map data
    for(int y=0;y<img->height;y++)
	for (int x=0;x<img->width;x++)
	{
	    Pixel myPix=img->GetPixel(x,y);
	    if (GetBinaryDataAtPoint(imageBinaryData,x,y))
	    {
		(img->GetPixel(x,y)).Set (myPix.r, 255 , myPix.b);
	    }
	    else 
		(img->GetPixel(x,y)).Set (myPix.r, 0 , myPix.b);
	}
	    
    return 0;
}


using namespace std;

int process(char *inputFileName, char *outputFileName)
{
    //Load Image Data
    cout << "Read Image file:" << inputFileName << endl << flush;
    Image *img = NULL;
    FILE *file=fopen(inputFileName,"rb");
    if (file==NULL) 
      {
      cout << "Error reading image!" << endl;
      exit(1);
      };
      img = BMPReadImage(file);
    if (img==NULL) cout << "Error reading image!" << endl;
    cout << ".done." << endl;
    
    //Convert Data to Axon density data
    densityDataType imageDensityData;
    cout << "Create density Data from green Channel of Image " << endl;
    imageDataToDensityData(img, &imageDensityData);
    cout << "done. Create" << endl;
    
    //Invert Data
    cout << "Invert Density Data" << endl;
    invertDensityData(imageDensityData);
    cout << "done. Invert" << endl;
    
    //Otsu Thresholding
    ucharDataType segData;
    CreateBinaryData(&segData,imageDensityData.sizeX, imageDensityData.sizeY); //DebugLinker
    cout << "Perform: Otsu Thresholding" << endl;
    int gridSize=20; //must be divideable by 2
    int globalMaxThreshold=255;
    int globalMinThreshold=50;  //30;
    cout << "With Parameter: gridSize=" << gridSize << " globalMaxThreshold=" << globalMaxThreshold << " globalMinThreshold=" << globalMinThreshold << endl;
    OtsuThresholdingWithParameter(imageDensityData, segData, globalMinThreshold, globalMaxThreshold, gridSize);
    cout << "done. Otsu Thresholding" << endl;
    
    
    //Morphological Close
    ucharDataType closedData;
    CreateBinaryData(&closedData,imageDensityData.sizeX, imageDensityData.sizeY);    
    cout << "Perform: Morphological Close" << endl;
    binaryClose(segData, closedData);
    cout << "done. Close" << endl;
    
    //Skeletonization
    ucharDataType skeletonData;
    CreateBinaryData(&skeletonData,imageDensityData.sizeX, imageDensityData.sizeY);
    cout << "Perform: Skeletonization" << endl;
    executeThining(closedData, skeletonData);
    cout << "done. Skeletonization" << endl;
    
    //Put Data into Image
    cout << "Copy Final Data into green channel of image" << endl;
    binaryDataToImageData(skeletonData, img);
    cout << "done. Copying" << endl;
    
    //Write Image
    cout << "Write File:" << outputFileName << endl;
    FILE *fileOut=fopen(outputFileName,"wb");
    BMPWriteImage(img, fileOut);
    fclose(fileOut);
    cout << "done." << endl;
    
    delete img;

    return 0;
}



int main(int argc, char **argv)  
{
  char inputfile[1024];
  char outputfile[1024];
 
    if (argc<4) 
    {
      cout << "Missing File Name Usage: createskeleton inputdirectory outputdirectory tilenr" << endl;
      return 0;
    };  

    int tileNr;
    sscanf(argv[3],"%d",&tileNr);

    sprintf(inputfile,"%s/Tile%06d.bmp",argv[1],tileNr);
    sprintf(outputfile,"%s/Tile%06d.bmp",argv[2],tileNr);
    
    cout << "*********** createskeleton *****************" << endl;
    cout << "Version:" << versionString << endl;
    cout << "Programmed by P. Broser, philip.broser@mpimf-heidelberg.mpg.de" << endl;
    cout << "Perform with" << endl;
    cout << "Input File:" << inputfile << endl;
    cout << "Output File:"<< outputfile << endl;
    process(inputfile  , outputfile);
    cout << "********************************************" << endl;
    
    return 0;
    
};


