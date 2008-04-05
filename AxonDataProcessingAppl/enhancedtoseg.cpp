#include "axonprocessingcore.h"
using namespace std;

int process(char *inputFileName, char *outputFileName, float SglobalMinThreshold, float SglobalMaxThreshold, float Sepsilon, int SscanningRange)
{
   densityDataType imageDensityData;
   Image* img=readDensityDataFromImageFile(inputFileName, imageDensityData);
   ucharDataType segData;
   CreateBinaryData(&segData,imageDensityData.sizeX, imageDensityData.sizeY); //DebugLinker
    
    //Otsu Thresholding
    /*
    cout << "Perform: Otsu Thresholding" << endl;
    int gridSize=20; //must be divideable by 2
    int globalMaxThreshold=255;
    int globalMinThreshold=50;  //30;
    cout << "With Parameter: gridSize=" << gridSize << " globalMaxThreshold=" << globalMaxThreshold << " globalMinThreshold=" << globalMinThreshold << endl;
    OtsuThresholdingWithParameter(imageDensityData, segData, globalMinThreshold, globalMaxThreshold, gridSize);
    cout << "done. Otsu Thresholding" << endl;
    */
    
    //Structural Thresholding
    /*
    float SglobalMinThreshold=0.01; 
	float SglobalMaxThreshold=0.7; 
	float Sepsilon=0.03;
	int SscanningRange=10;
	*/
	optimizeDataDynamic(imageDensityData);
	structuralThresholdingWithParameter(imageDensityData, segData,  SglobalMinThreshold, SglobalMaxThreshold, Sepsilon, SscanningRange);

	writeBinaryDataToImageFile(outputFileName, segData, img);    
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
  float globalMinThreshold=0.01; 
  float globalMaxThreshold=0.7; 
  float epsilon=0.03;
  int scanningRange=10;
  
   if (argc<4) 
    {
     if (argc==3)
      {
      cout << "Debug Mode" << endl;
      return process(argv[1]  , argv[2], globalMinThreshold, globalMaxThreshold, epsilon, scanningRange);
      };
     cout << "Missing File Name Usage: programname inputdirectory outputdirectory  -tilenr:nr -globalmin:float -globalmax:float -epsilon:float -scanningrange:nr" << endl;
     return 1;
    }; 
    
     
    //Parse Options
	command_line_parser parser(argc, argv);
	for (int i=0;i<parser.get_n_options();i++)    {        option o=parser.get_option(i);        if (strcmp(o.get_option(),"tilenr")==0)        {
        	int help;            if (sscanf (o.get_value(),"%d",&(help))==EOF) 
            	{cout << "Error reading tile nr" << endl;return 1;};            tileNr=help;        };
      	
      	     
     	if (strcmp(o.get_option(),"scanningrange")==0)        {
        	int help;            if (sscanf (o.get_value(),"%d",&(help))==EOF) 
            	{cout << "Error reading time steps nr" << endl;return 1;};
            scanningRange=help;
            cout << "Set scanningrange to:" << scanningRange << endl;                    };
	
		if (strcmp(o.get_option(),"globalmin")==0)        {
        	float help;            if (sscanf (o.get_value(),"%f",&(help))==EOF) 
            	{cout << "Error reading parameter" << endl;return 1;};
            cout << "Set globalMinThreshold to:" << help << endl;
            globalMinThreshold=help;
        };
     if (strcmp(o.get_option(),"globalmax")==0)        {
        	float help;            if (sscanf (o.get_value(),"%f",&(help))==EOF) 
            	{cout << "Error reading parameter" << endl;return 1;};
            cout << "Set globalMaxThreshold to:" << help << endl;
            globalMaxThreshold=help;
        };
        
         if (strcmp(o.get_option(),"epsilon")==0)        {
        	float help;            if (sscanf (o.get_value(),"%f",&(help))==EOF) 
            	{cout << "Error reading parameter" << endl;return 1;};
            cout << "Set epsilon to:" << help << endl;
            epsilon=help;
        };

        
        
     
     };
	
	if (tileNr==-1)
	{
	cout << "No tile nr specified bailing out!" << endl;
	return 1;
	}
	
    
    

    sprintf(inputfile,"%s/Tile%06d.bmp",parser.get_filename(0),tileNr);
    sprintf(outputfile,"%s/Tile%06d.bmp",parser.get_filename(1),tileNr);
    
    
    cout << "*********** enhancedtoseg *****************" << endl;
    cout << "Version:" << versionString << endl;
    cout << "Programmed by P. Broser, philip.broser@mpimf-heidelberg.mpg.de" << endl;
    cout << "Perform with" << endl;
    cout << "Input File:" << inputfile << endl;
    cout << "Output File:"<< outputfile << endl;
    process(inputfile  , outputfile, globalMinThreshold, globalMaxThreshold, epsilon, scanningRange);
    cout << "********************************************" << endl;
    
    return 0;
    
};


