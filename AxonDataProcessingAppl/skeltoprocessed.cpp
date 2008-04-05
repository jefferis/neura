#include "axonprocessingcore.h"
using namespace std;


int secureGetData(ucharDataType &data, int x, int y)
{
	if (x<0 || y<0 || x>=data.sizeX || y>=data.sizeY) return 0;
	return GetBinaryDataAtPoint(data,x,y);
}

void secureSetData(ucharDataType &data, int x, int y, unsigned char d)
{
	if (x<0 || y<0 || x>=data.sizeX || y>=data.sizeY) return;
	SetBinaryDataAtPoint(data,x,y,d);
}

short howManyNeigbours(ucharDataType &data, int x, int y)
{
int dx, dy;
short counter=0;
for (dy=-1;dy<=1;dy++)
	for (dx=-1;dx<=1;dx++)
		if (secureGetData(data, (x+dx), (y+dy)))
			counter++;	

	return counter;
}



float nextStep(ucharDataType &skelData, int x, int y,int & dx, int & dy)
{
short counter=0;
for (dy=-1;dy<=1;dy++)
	for (dx=-1;dx<=1;dx++)
	{
	    //cout << "dx=" << dx <<  " dy=" << dy << " Secure Get Data=" <<  secureGetData(skelData, x+dx, y+dy) << endl;
		if (secureGetData(skelData, x+dx, y+dy)) 
		{
		//cout << "Here is something !" << endl;
		return sqrt((float) dx*dx+dy*dy);
		};
		};
return 0.0;	
};


#include <list>

class Compartment
{
public:
	int x,y;
	float length;
};

#define maxRecursiceDepth 20
#define maxIterationNr 200


float trackAxon(int x, int y, ucharDataType &skelData, list<Compartment> &myAxonCompList, float minBranchLength, int recursciveDepth)
{
	secureSetData(skelData, x, y, 0);
	float length=0;
	if (recursciveDepth>maxRecursiceDepth) return 0.0;
	recursciveDepth++;
	int iterationNr=0;    
	    
	//Walk along the axon
	bool reachedTermination=false;
	while (!reachedTermination && iterationNr<maxIterationNr)
	{
	//cout << "*" ;// at x:" << x << " and y:" << y << " "; 
	int dx, dy;
	iterationNr++;
	if (howManyNeigbours(skelData, x, y)>1)//This is a bifurcation have to walk throuh all paths
		{
		//cout << "Reached bifurcation" << endl;
		for (dy=-1;dy<=1;dy++)
			for (dx=-1;dx<=1;dx++)
				if (secureGetData(skelData, x+dx, y+dy)) 
				{
					list<Compartment> branchAxonCompList;
					float branchLength=trackAxon(x+dx, y+dy, skelData, branchAxonCompList, minBranchLength, recursciveDepth);
					if (branchLength>=minBranchLength)
						{
						length=length+branchLength;
						myAxonCompList.splice(myAxonCompList.end(), branchAxonCompList);
						}
				}
		}
		
		
	float deltaLength=nextStep(skelData, x, y, dx, dy);
	//cout << "Delta Length:" << deltaLength << endl; 
	if (deltaLength>0.0)
	{
		x=x+dx;
		y=y+dy;
		length=length+deltaLength;
		Compartment comp;
		comp.x=x;
		comp.y=y;
		myAxonCompList.push_back(comp);
		secureSetData(skelData, x, y, 0);
	}
	else
	{
		reachedTermination=true;
		//cout << "Reached termination Length of this axon:" << length << endl;
	}
	}
	return length;

}





int plotAxonToDataSet(list<Compartment> &axonCompList,ucharDataType processedData)
{
    list<Compartment>::iterator pos;
    for (pos=axonCompList.begin();pos!=axonCompList.end();++pos)
    {
	int x=(*pos).x;
	int y=(*pos).y;
	secureSetData(processedData, x, y, 1);
    };
    return 0;
}



int process(char *inputFileName, char *outputFileName, int minimalbranchlengthpixel, int translateX, int translateY)
{
	float minLength=minimalbranchlengthpixel;
    cout << "Remove all branches shorter then:" << minLength << " pixel" << endl; 
 	//Load Binary Data
 	ucharDataType skelData;
 	Image* img=readBinaryDataFromImageFile(inputFileName, skelData);
 	
 	//TargetData
 	ucharDataType processedData;
 	CreateBinaryData(&processedData,skelData.sizeX, skelData.sizeY);
 	
 	//Search for Axon Start Points:
 	for (int y=0;y<skelData.sizeY;y++)
 	for (int x=0;x<skelData.sizeX;x++)
 	{
 		if (!secureGetData(skelData, x, y)) continue; //No Axon here nothing to do
 		if (howManyNeigbours(skelData, x, y)<4) // is it the beginning of the axon?
 			{
		    //cout << "Start to track axon at position " << x << " " << y << endl;
			list<Compartment> axonCompList;
 			float axonLength=trackAxon(x,y, skelData,axonCompList, minLength, 0);
 			if (axonLength>=minLength) 
			{
			    //cout << "this axon is long enough" << endl;		
			    plotAxonToDataSet(axonCompList, processedData);
			};
		};
 	
 	}
 	      
 	      
    //Put Data into Image
    cout << "Copy Final Data into green channel of image" << endl;
    binaryDataToImageData(processedData, img);
    cout << "done. Copying" << endl;  
    Image *img2=createTranslatedImage(img, translateX, translateY);

	cout << "write Image:" <<  outputFileName << endl;   
	FILE *fileOut=fopen(outputFileName,"wb");
    BMPWriteImage(img2, fileOut);
    fclose(fileOut);
    if (fclose(fileOut)) return 1;
       
    delete img;
	delete img2;
    return 0;
}

#include "command_line_parser.h"
using namespace pramodx;


int main(int argc, char **argv)  
{
  char inputfile[1024];
  char outputfile[1024];
  int tileNr=-1;
  int translateX=0;
  int translateY=0;
  
  //DefaultParameter
  int minimalbranchlengthpixel=18;//Syncroscan: 4qm/0.23 OlympusSlide: 4qm/0.32 /13
   
    if (argc<4) 
    {
     if (argc==3)
      {
      cout << "Debug Mode" << endl;
      cout << "In file:" << argv[1] << endl;
       cout << "Out file:" << argv[2] << endl;
      
      return process(argv[1]  , argv[2], minimalbranchlengthpixel, translateX, translateY);
      };
     cout << "Missing File Name Usage: programname inputdirectory outputdirectory -tilenr:nr -minimalbranchlengthpixel:nr -translatex:dist -translatey:dist" << endl;
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
     if (strcmp(o.get_option(),"translatex")==0)
        {
        	int help;
            if (sscanf (o.get_value(),"%d",&(help))==EOF) 
            	{cout << "Error reading translatex" << endl;return 1;};
            translateX=help;
            cout << "Set translateX=" << translateX << endl;
        };
     
     if (strcmp(o.get_option(),"translatey")==0)
        {
        	int help;
            if (sscanf (o.get_value(),"%d",&(help))==EOF) 
            	{cout << "Error reading translatex" << endl;return 1;};
            translateY=help;
            cout << "Set translateY=" << translateY << endl;
        };
        
     if (strcmp(o.get_option(),"minimalbranchlengthpixel")==0)
        {
        	int help;
            if (sscanf (o.get_value(),"%d",&(help))==EOF) 
            	{cout << "Error reading translatex" << endl;return 1;};
            minimalbranchlengthpixel=help;
            cout << "Set minimalbranchlengthpixel=" << minimalbranchlengthpixel << endl;
        };

     
     };
	
	if (tileNr==-1)
	{
	cout << "No tile nr specified bailing out!" << endl;
	return 1;
	}
	  

    sprintf(inputfile,"%s/Tile%06d.bmp",parser.get_filename(0),tileNr);
    sprintf(outputfile,"%s/Tile%06d.bmp",parser.get_filename(1),tileNr);
    
    
    cout << "*********** skeltoprocessed *****************" << endl;
    cout << "Version:" << versionString << endl;
    cout << "Programmed by P. Broser, philip.broser@mpimf-heidelberg.mpg.de" << endl;
    cout << "Perform with" << endl;
    cout << "Input File:" << inputfile << endl;
    cout << "Output File:"<< outputfile << endl;
    process(inputfile  , outputfile, minimalbranchlengthpixel, translateX, translateY);
    cout << "********************************************" << endl;
    
    return 0;
    
};


