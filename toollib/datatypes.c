/*
 *  datatypes.c
 *  DistanceMap
 *
 *  Created by Philip J. Broser on Wed Jul 07 2004.
 *  Copyright (c) 2004 __MyCompanyName__. All rights reserved.
 *
 */

#include "datatypes.h"
#include <stdio.h> 
#include <stdlib.h>

void CreateBinaryData(ucharDataType *data,int sizeX, int sizeY)
{
    int i;
    int sizeInByte=(sizeX*sizeY)/8+2;
    unsigned char *temp = (unsigned char *) malloc (sizeInByte);
    for(i=0;i<sizeInByte;i=i+1) temp[i]=0;
    (*data).sizeX=sizeX;
    (*data).sizeY=sizeY;
    (*data).data=temp;
    return; 
};
 
void DestroyBinaryData(ucharDataType *data)
{
    free((*data).data);
    
}



unsigned char GetBinaryDataAtPoint(ucharDataType data,int x, int y)
{
    int bytePosition=(x+data.sizeX*y)/8;
    int bitPosition=(x+data.sizeX*y)-8*bytePosition;
    unsigned char help=1;
    help= help << bitPosition;
    return data.data[bytePosition]&help;
};

void SetBinaryDataAtPoint(ucharDataType data,int x, int y, unsigned char d)
{
    int bytePosition=(x+data.sizeX*y)/8;
    unsigned char bitPosition=(x+data.sizeX*y)-8*bytePosition;
    unsigned char help=1;
    help= help << bitPosition;
    if (d) 
    {
	data.data[bytePosition]=data.data[bytePosition]|help;return;
    }
    else
    {
	data.data[bytePosition]=data.data[bytePosition]&(255-help);return;
    };
};

int writeBinaryDataToPPMFile(char *filename, ucharDataType data)
{
  /*
    FILE *fptr;
    unsigned char outData;
    int sizeX=data.sizeX;
    int sizeY=data.sizeY;
    
    
    if (filename == NULL)
	fptr = stdout;
    else
	fptr = fopen(filename, "w");
    if (fptr==NULL) return 1;
    
    // header for PPM file 
   // fprintf(fptr, "P6\n");
   // fprintf(fptr, "%d %d\n", sizeX, sizeY);
   // fprintf(fptr, "255\n");
//	writing out actual image bytes 
    int x,y;
    for (y = 0; y < sizeY;  y++) {
	for (x = 0; x < sizeY; x++) 
	{
	    if (GetBinaryDataAtPoint(data, x, y)) 
	    {outData = 255;}
	    else
	    {outData = 0;};
	    fwrite(&outData, 1, sizeof(unsigned char), fptr);
	    //fwrite(&outData, 1, sizeof(unsigned char), fptr);
	    //fwrite(&outData, 1, sizeof(unsigned char), fptr);
	}
    }
*/    
return 0;
 }













