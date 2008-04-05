#include "axonprocessingcore.h"
using namespace std;



int process(char *inputFileName, char *outputFileName)
{
 	//Load Binary Data
 	
 	ucharDataType segData;
 	Image* img=readBinaryDataFromImageFile(inputFileName, segData);
 	      
    //Morphological Close
    ucharDataType closedData;
    CreateBinaryData(&closedData,segData.sizeX, segData.sizeY);    
    cout << "Perform: Morphological Close" << endl;
    binaryClose(segData, closedData);
    cout << "done. Close" << endl;
    
    //Skeletonization
    ucharDataType skeletonData;
    CreateBinaryData(&skeletonData,segData.sizeX, segData.sizeY);
    cout << "Perform: Skeletonization" << endl;
    executeThining(closedData, skeletonData);
    cout << "done. Skeletonization" << endl;
    
    //Put Data into Image
    cout << "Copy Final Data into green channel of image" << endl;
    binaryDataToImageData(skeletonData, img);
    cout << "done. Copying" << endl;
    
    writeBinaryDataToImageFile(outputFileName, skeletonData, img);   
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
   
    if (argc<4) 
    {
     if (argc==3)
      {
      cout << "Debug Mode" << endl;
      return process(argv[1]  , argv[2]);
      };
     cout << "Missing File Name Usage: programname inputdirectory outputdirectory -tilenr:nr" << endl;
     return 1;
    };  
    
    
	//Parse Options
	command_line_parser parser(argc, argv);
	for (int i=0;i<parser.get_n_options();i++)    {        option o=parser.get_option(i);        if (strcmp(o.get_option(),"tilenr")==0)        {
        	int help;            if (sscanf (o.get_value(),"%d",&(help))==EOF) 
            	{cout << "Error reading tile nr" << endl;return 1;};            tileNr=help;        };
      	
       };
	
	if (tileNr==-1)
	{
	cout << "No tile nr specified bailing out!" << endl;
	return 1;
	}
	
    
    

    sprintf(inputfile,"%s/Tile%06d.bmp",parser.get_filename(0),tileNr);
    sprintf(outputfile,"%s/Tile%06d.bmp",parser.get_filename(1),tileNr);
    

    cout << "*********** segtoskel *****************" << endl;
    cout << "Version:" << versionString << endl;
    cout << "Programmed by P. Broser, philip.broser@mpimf-heidelberg.mpg.de" << endl;
    cout << "Perform with" << endl;
    cout << "Input File:" << inputfile << endl;
    cout << "Output File:"<< outputfile << endl;
    process(inputfile  , outputfile);
    cout << "********************************************" << endl;
    
    return 0;
    
};


