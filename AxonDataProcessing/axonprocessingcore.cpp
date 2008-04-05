
#include "axonprocessingcore.h"

int errno2;
char *versionString="createskeleton v 0.2";
   
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

float maxOfData(densityDataType data)
{
	float max=0.0;
	for (int y=0;y<data.sizeY;y++)
	for (int x=0;x<data.sizeX;x++)
	{
    	float d=ACCESSDATAATPOINT(data,x,y);
    	if (d>max) max=d;
    };
    return max;
};

int scaleData(densityDataType data, float factor)
{
	float max=maxOfData(data);
	if (max<0.0) max=1.0;
	for (int y=0;y<data.sizeY;y++)
	for (int x=0;x<data.sizeX;x++)
	{
    	float d=ACCESSDATAATPOINT(data,x,y);
    	ACCESSDATAATPOINT(data,x,y)=factor/max*d;
    };
    return 0;
};



int optimizeDataDynamic(densityDataType data)
{
	float max=maxOfData(data);
	if (max<=0.0) max=1.0;
	for (int y=0;y<data.sizeY;y++)
	for (int x=0;x<data.sizeX;x++)
	{
    float d=ACCESSDATAATPOINT(data,x,y);
    ACCESSDATAATPOINT(data,x,y)=d/max;
	};

	    
    return 0;
}


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

int imageDataToBinaryData(Image *img, ucharDataType *data)
{
//Euklidean map data
CreateBinaryData(data,img->width, img->height); 
for(int y=0;y<img->Height();y++)
for (int x=0;x<img->Width();x++)
{
    Pixel myPix=img->GetPixel(x,y);
    if (myPix.g)
    {
    SetBinaryDataAtPoint((*data),x,y,1);
    continue;
    }
    SetBinaryDataAtPoint((*data),x,y,0);
    }
return 0;
};
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
 

int writeDensityDataToImageFile(char *fileName, densityDataType data, Image *img)
{
  
    cout << "Copy Final Data into green channel of image" << endl;
    densityDataToImageData(data, img);
    cout << "done. Copying" << endl;
    
    //Write Image
    cout << "Write File:" << fileName << endl;
    FILE *fileOut=fopen(fileName,"wb");
    BMPWriteImage(img, fileOut);
    if (fclose(fileOut)) return 1;
    cout << "done." << endl;
	return 0;
}

Image* readDensityDataFromImageFile(char *fileName, densityDataType &data)
{
 	//Load Image Data
    cout << "Read Image file:" << fileName << endl << flush;
    Image *img = NULL;
    FILE *file=fopen(fileName,"rb");
    if (file==NULL) 
      {
      cout << "Error reading image!" << endl;
      exit(1);
      };
    img = BMPReadImage(file);
    if (img==NULL) 
    {
    cout << "Error reading image!" << endl;
    exit(1);
    };
    cout << ".done." << endl;
    
    //Convert Data to Axon density data
    cout << "Create density Data from green Channel of Image " << endl;
    imageDataToDensityData(img, &data);
    cout << "done. Create" << endl;
    if (fclose(file)) return NULL;
    return img;
};

int writeBinaryDataToImageFile(char *fileName, ucharDataType data, Image *img)
{
  
	binaryDataToImageData(data, img);
	FILE *fileOut=fopen(fileName,"wb");
    BMPWriteImage(img, fileOut);
    fclose(fileOut);
    if (fclose(fileOut)) return 1;
    cout << "done." << endl;
	return 0;
}
 
Image* readBinaryDataFromImageFile(char *fileName, ucharDataType &data)
{
 	//Load Image Data
    cout << "Read Image file:" << fileName << endl << flush;
    Image *img = NULL;
    FILE *file=fopen(fileName,"rb");
    if (file==NULL) 
      {
      cout << "Error reading image!" << endl;
      exit(1);
      };
    img = BMPReadImage(file);
    if (img==NULL) 
    {
    cout << "Error reading image!" << endl;
    exit(1);
    };
    cout << ".done." << endl;
    
    //Convert Data to Axon density data
    cout << "Create density Data from green Channel of Image " << endl;
    imageDataToBinaryData(img, &data);
    cout << "done. Create" << endl;
    if (fclose(file)) return NULL;
    return img;
    
};

 Image* createTranslatedImage(Image *img,int  translateX,int  translateY)
 {	 
 	int width=img->Width();
 	int height=img->Height();
 
 	Image *image2=new Image(width, height);
 	int y, x, nx, ny;
 	//cout << "Old Image width:" <<  img->Width() << " height:" << img->Height() << endl;
 	//cout << "New Image width:" <<  image2->Width() << " height:" << image2->Height() << endl;
 	
 	//cout << "Set image zero" << endl;
 	for (y=0;y<height;y++)
 	for (x=0;x<width;x++)
 		(image2->GetPixel (x, y)).Set(0,0,0,0); //set to zero
 	//cout << "done." << endl;
 	
 	for (y=0;y<height;y++)
 	for (x=0;x<width;x++)
 	{
 	nx=x+translateX;
 	ny=y+translateY;
 	if (nx>=0 && nx<width && ny>=0 && ny<height)
 		{
 		Component r=(img->GetPixel (x, y)).r;
 		Component g=(img->GetPixel (x, y)).g;
 		Component b=(img->GetPixel (x, y)).b;
 		Component a=(img->GetPixel (x, y)).a;
 		(image2->GetPixel (nx, ny)).Set(r,g,b,a);
 		};
 };
 	return image2;
 }
    

