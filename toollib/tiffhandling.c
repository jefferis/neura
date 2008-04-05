
#include "tiffhandling.h"
#include <stdlib.h>


int readTiffFile(char *fileName, densityDataType *densityData)
{
  TIFF *image;
  uint32 width, height, *raster;
  tsize_t stripSize;
  unsigned long imagesize, c, d, e;

  // Open the TIFF image
  if((image = TIFFOpen(fileName, "r")) == NULL){
    fprintf(stderr, "Could not open incoming image\n");
    return 1;
  }

  // Find the width and height of the image
  TIFFGetField(image, TIFFTAG_IMAGEWIDTH, &width);
  TIFFGetField(image, TIFFTAG_IMAGELENGTH, &height);
  imagesize = height * width + 1;
  
  if((raster = (uint32 *) malloc(sizeof(uint32) * imagesize)) == NULL){
    fprintf(stderr, "Could not allocate enough memory\n");
    exit(42); 
  }

  // Read the image into the memory buffer
  if(TIFFReadRGBAStrip(image, 0, raster) == 0){
    fprintf(stderr, "Could not read image\n");
    exit (1);
    return 1;
  }
 
	//Copy Data
	(*densityData).sizeX=width;
	(*densityData).sizeY=height;
	(*densityData).data=(float*) malloc( sizeof(float) * imagesize);
printf("Read Tiff Data\n");
  // Here I fix the reversal of the image (vertically) and show you how to get the color values from each pixel
  d = 0;
  for(e = height - 1; e != -1; e--){
    for(c = 0; c < width; c++){
      // Red = TIFFGetR(raster[e * width + c]);
      ACCESSDATAATPOINT((*densityData), c, e) =  TIFFGetG(raster[e * width + c]);
      if (TIFFGetG(raster[e * width + c]) > 1) printf("h");
      //printf("r:%d g:%d b:%b",TIFFGetR(raster[e * width + c]),TIFFGetG(raster[e * width + c]),TIFFGetB(raster[e * width + c]));
      
      
      // Blue = TIFFGetB(raster[e * width + c]);
    }
  }
	printf("h");
  TIFFClose(image);
  return 0;
}

int writeTiffFile(char *fileName, densityDataType densityData)
{
  TIFF *output;
  uint32 width, height;
  unsigned char *raster;

  // Open the output image
  if((output = TIFFOpen(fileName, "w")) == NULL){
    fprintf(stderr, "Could not open outgoing image\n");
    return 1;
  }

  // We need to know the width and the height before we can malloc
  int x,y;
  width = densityData.sizeX;
  height= densityData.sizeY;
  float max=densityData.data[0];
  float min=densityData.data[1];
  for(y=0;y<height;y=y+1)
  	for (x=0;x<width;x=x+1)
	{
		float presentData=ACCESSDATAATPOINT(densityData, x, y);
		if (presentData>max) max=presentData;
		if (presentData<min) min=presentData;
	}
	if (max-min==0) {max=1.0;min=0.0;};
	float scale=255.0/(max-min);

  if((raster = (unsigned char *) malloc(sizeof(char) * width * height * 3)) == NULL){
    fprintf(stderr, "Could not allocate enough memory\n");
    return 1;
  }

  // Magical stuff for creating the image
  //Simple Test
  
  
  for(y=0;y<height;y=y+1)
  for (x=0;x<width;x=x+1)
  {
 	float help=(ACCESSDATAATPOINT(densityData, x, y)-min)*scale;
  	raster[3*(y*width+x)+0]=0; //red 
	  	raster[3*(y*width+x)+1]=(unsigned char) help; //green
	  	 if (raster[3*(y*width+x)+1] > 100) printf("f");
  	raster[3*(y*width+x)+2]=0; //blue
   }
  
  
  // ...

  // Write the tiff tags to the file
  TIFFSetField(output, TIFFTAG_IMAGEWIDTH, width);
  TIFFSetField(output, TIFFTAG_IMAGELENGTH, height);
 // TIFFSetField(output, TIFFTAG_COMPRESSION, COMPRESSION_DEFLATE);
  TIFFSetField(output, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
  TIFFSetField(output, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
  TIFFSetField(output, TIFFTAG_BITSPERSAMPLE, 8);
  TIFFSetField(output, TIFFTAG_SAMPLESPERPIXEL, 3);

  // Actually write the image
  if(TIFFWriteEncodedStrip(output, 0, raster, width * height * 3) == 0){
    fprintf(stderr, "Could not write image\n");
    exit(42);
  }

  TIFFClose(output);
  return 0;
}





