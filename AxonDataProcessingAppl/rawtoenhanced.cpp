
#include "axonprocessingcore.h"
using namespace std;

int process(char *inputFileName, char *outputFileName, int scanningRange, int nrOfTimeSteps)
{
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
    
    
    /*float value, structure, alpha;*/ 
    Tophat(&imageDensityData, 9, &tophatData1);	
    optimizeDataDynamic(tophatData1);
    free(imageDensityData.data);
    
   
       
    //We test how the algorithm works without all this filtering and only use top hat onece
    /*
    densityDataType filteredData;
    filteredData.sizeX=imageDensityData.sizeX;
    filteredData.sizeY=imageDensityData.sizeY;
    filteredData.data=(float*) malloc(sizeof(float)*imageDensityData.sizeX*imageDensityData.sizeY);
    
    //int scanningRange=10;//7; 
    float deltaT=0.25; // max 1/4
    int maxTimeStep=4; 
    filterDensityData(tophatData1, filteredData, scanningRange, deltaT, nrOfTimeSteps);
    free(tophatData1.data);
    
    optimizeDataDynamic(filteredData);
        
    densityDataType tophatData2;
    tophatData2.sizeX=imageDensityData.sizeX;
    tophatData2.sizeY=imageDensityData.sizeY;
    tophatData2.data=(float*) malloc(sizeof(float)*imageDensityData.sizeX*imageDensityData.sizeY);
    
    Tophat(&filteredData, 9, &tophatData2);	
    optimizeDataDynamic(tophatData2);
    free(filteredData.data);
    
    
    //Put Data into Image
    writeDensityDataToImageFile(outputFileName, tophatData2, img);
    */
    cout << "Write out put file" << endl;
    writeDensityDataToImageFile(outputFileName, tophatData1, img);

     
    delete img;
    return 0;
}

  
#include "command_line_parser.h"
using namespace pramodx;



int main(int argc, char **argv)  
{
  char inputfile[1024];
  char outputfile[1024];
  int tileNr=-1;
  
  //Default Parameter
  int nrOfTimeSteps=1;
  int scanningRange=10; 
 
  
    if (argc<4) 
    {
     if (argc==3)
      {
      cout << "Debug Mode" << endl;
      return process(argv[1]  , argv[2], scanningRange, nrOfTimeSteps);
      };
     cout << "Missing File Name Usage: programname inputdirectory outputdirectory  -tilenr:nr -timesteps:nr -scanningrange:nr" << endl;
     return 1;
    };  
    //Parse Options
	command_line_parser parser(argc, argv);
	for (int i=0;i<parser.get_n_options();i++)
    {
        option o=parser.get_option(i);
        if (strcmp(o.get_option(),"tilenr")==0)
        {
        	int help;
            if (sscanf (o.get_value(),"%d",&(help))==EOF) 
            	{cout << "Error reading tile nr" << endl;return 1;};
            tileNr=help;
        };
      	
      	if (strcmp(o.get_option(),"timesteps")==0)
        {
        	int help;
            if (sscanf (o.get_value(),"%d",&(help))==EOF) 
            	{cout << "Error reading time steps nr" << endl;return 1;};
            nrOfTimeSteps=help;
            cout << "Set nr of time steps to:" << nrOfTimeSteps << endl;
            
        };
     
     	if (strcmp(o.get_option(),"scanningrange")==0)
        {
        	int help;
            if (sscanf (o.get_value(),"%d",&(help))==EOF) 
            	{cout << "Error reading time steps nr" << endl;return 1;};
            scanningRange=help;
            cout << "Set scanningrange to:" << scanningRange << endl;
            
        };

     
     
     };
	
	if (tileNr==-1)
	{
	cout << "No tile nr specified bailing out!" << endl;
	return 1;
	}
	
    
    

    sprintf(inputfile,"%s/Tile%06d.bmp",parser.get_filename(0),tileNr);
    sprintf(outputfile,"%s/Tile%06d.bmp",parser.get_filename(1),tileNr);
    
    cout << "*********** rawtoenhanced *****************" << endl;
    cout << "Version:" << versionString << endl;
    cout << "Programmed by P. Broser, philip.broser@mpimf-heidelberg.mpg.de" << endl;
    cout << "Perform with" << endl;
    cout << "Input File:" << inputfile << endl;
    cout << "Output File:"<< outputfile << endl;
   	process(inputfile  , outputfile,  scanningRange, nrOfTimeSteps);
    cout << "********************************************" << endl;
    
    return 0;
    
};


