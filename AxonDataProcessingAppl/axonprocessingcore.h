
#ifndef AXONPROCESSINGCORE
#define AXONPROCESSINGCORE
extern char *versionString;


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

float maxOfData(densityDataType data);
int scaleData(densityDataType data, float factor);
int optimizeDataDynamic(densityDataType data);
int invertDensityData(densityDataType data);
int imageDataToDensityData(Image *img, densityDataType *imageDensityData);
int densityDataToImageData(densityDataType imageDensityData, Image *img);
int binaryDataToImageData(ucharDataType imageBinaryData, Image *img);

Image* createTranslatedImage(Image *img,int  translateX,int  translateY);
    

int writeDensityDataToImageFile(char *fileName, densityDataType data, Image *img);
Image* readDensityDataFromImageFile(char *fileName, densityDataType &data);
int writeBinaryDataToImageFile(char *fileName, ucharDataType data, Image *img);
Image* readBinaryDataFromImageFile(char *fileName, ucharDataType &data);

#endif

