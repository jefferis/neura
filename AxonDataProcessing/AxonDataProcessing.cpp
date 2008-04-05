
#include "axonprocessingcore.h"
#include "AxonPathTracking.h"
using namespace std;

int process(char *inputFileName, char *outputFileName)
{
    //Parameter
    //TopHat
    int ScanningRangeTopHat=9;
    //Segmentation
    float SglobalMinThreshold=0.05; 
    float SglobalMaxThreshold=0.7; 
    float Sepsilon=0.05;
    int SscanningRange=10;
    
    //Process Skeleton
    int minimalbranchlengthpixel=18; //Olympus: 0.35qm/pixel Syncroscan: 0.23qm/pixel
    
    densityDataType imageDensityData;
    Image* img=readDensityDataFromImageFile(inputFileName, imageDensityData);
    /*Invert Data*/
    cout << "Invert Density Data" << endl;
    invertDensityData(imageDensityData);
    cout << "done. Invert" << endl;
    /*free density data*/
    
    /*Create Data Storage*/
    densityDataType tophatData1;
    tophatData1.sizeX=imageDensityData.sizeX;
    tophatData1.sizeY=imageDensityData.sizeY;
    tophatData1.data=(float*) malloc(sizeof(float)*imageDensityData.sizeX*imageDensityData.sizeY);
    
    //TopHat
    cout << "Perform: Top Hat" << endl;
    Tophat(&imageDensityData, ScanningRangeTopHat, &tophatData1);	
    optimizeDataDynamic(tophatData1);
    free(imageDensityData.data);
    
    //Segmentation
    ucharDataType segData;
    CreateBinaryData(&segData,tophatData1.sizeX, tophatData1.sizeY); 
    cout << "Perform: Segmentation" << endl;
    structuralThresholdingWithParameter(tophatData1, segData,  SglobalMinThreshold, SglobalMaxThreshold, Sepsilon, SscanningRange);
    free(tophatData1.data);

    
    //Morphological Close
    ucharDataType closedData;
    CreateBinaryData(&closedData,segData.sizeX, segData.sizeY);    
    cout << "Perform: Morphological Close" << endl;
    binaryClose(segData, closedData);
    free(segData.data);
    
    //Skeletonization
    ucharDataType skeletonData;
    CreateBinaryData(&skeletonData,closedData.sizeX, closedData.sizeY);
    cout << "Perform: Skeletonization" << endl;
    executeThining(closedData, skeletonData);
    free(closedData.data);
    
   
    //Processing 
    ucharDataType processedData;
    CreateBinaryData(&processedData,skeletonData.sizeX, skeletonData.sizeY);
    senseSkeleton(skeletonData, processedData,  minimalbranchlengthpixel);
    free(skeletonData.data);
  
   
    //Put Data into Image
    cout << "Copy Final Data into green channel of image" << endl;
    binaryDataToImageData(processedData, img);
    cout << "write Image:" <<  outputFileName << endl;   
    FILE *fileOut=fopen(outputFileName,"wb");
    BMPWriteImage(img, fileOut);
    fclose(fileOut);
    if (fclose(fileOut)) return 1;
    
    delete img;
    return 0;
}

  
#include "command_line_parser.h"
using namespace pramodx;



int main(int argc, char **argv)  
{
  char inputfile[1024];
  char outputfile[1024];
  if (argc<3)
  {
      cout << "Missing File Name Usage: programm inputfilename outputfilename" << endl;
  }
 
  cout << "InputFileName:" << argv[1] << endl;
  cout << "OutputFileName:" << argv[2] << endl;
  return process(argv[1],argv[2]);

    
};


