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

int process(char *inputFileName, char *outputFileName,int globalMinThreshold, int globalMaxThreshold, int gridSize)
{
    cout << "Read file.." ;
    Image *img = NULL;
    //FILE *file=fopen("Tile000295.bmp","rb");
    FILE *file=fopen(inputFileName,"rb");
    if (file==NULL) cout << "Error reading image!" << endl;
    img = BMPReadImage(file);
    if (img==NULL) cout << "Error reading image!" << endl;
    cout << ".done." << endl;
    
    densityDataType imageDensityData;
    
    cout << "Create density Data" << endl;
    imageDataToDensityData(img, &imageDensityData);

    //cout << "Invert Density Data" << endl;
    //invertDensityData(imageDensityData);
    

//Binary Segmented Data
    ucharDataType segData;
    CreateBinaryData(&segData,imageDensityData.sizeX, imageDensityData.sizeY); 
     
    //cout << "Otsu Thresholding" << endl;
    //OtsuThresholdingWithParameter(imageDensityData, segData,  globalMinThreshold, globalMaxThreshold, gridSize);
	
	
	float SglobalMinThreshold=0.01; 
	float SglobalMaxThreshold=0.7; 
	float Sepsilon=0.02;//0.03
	int SscanningRange=10;
	structuralThresholdingWithParameter(imageDensityData, segData,  SglobalMinThreshold, SglobalMaxThreshold, Sepsilon, SscanningRange);
	
    binaryDataToImageData(segData, img);
   
    FILE *fileOut=fopen(outputFileName,"wb");
    BMPWriteImage(img, fileOut);
    fclose(fileOut);
    
    delete img;


     return 0;
}



int main(int argc, char **argv)  
{
    if (argc<3) 
    {
	cout << "Missing File Name Usage: segmentation inputfilename outputfilename abslowthreshold absmaxthreshold gridsize" << endl << "only bmp File Types!" << endl;
	return 0;
    };  
    
    int gridSize=20; //must be divideable by 2
    int globalMaxThreshold=255;
    int globalMinThreshold=50;  //30;  
    cout << "Input File:" << argv[1] << endl;
    cout << "Output File:" << argv[2] << endl;
    if (argc>3)
    {
	sscanf(argv[3],"%d",&globalMinThreshold);
	cout << "Set globat min threshold to:" << globalMinThreshold << endl;
    }
    if (argc>4)
    {
	sscanf(argv[4],"%d",&globalMaxThreshold);
	cout << "Set globat min globalMaxThreshold to:" << globalMaxThreshold << endl;

    
    }
    if (argc>5)
    {
	sscanf(argv[5],"%d",&gridSize);
	cout << "Set gridSize to:" << gridSize << endl;

    
    }
    
    process(argv[1]  , argv[2], globalMinThreshold, globalMaxThreshold,  gridSize);
    
    return 0;
    
};


