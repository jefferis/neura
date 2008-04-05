/*testprogram for reconstruction */
#include <fstream.h>
#include <stdio.h>
#include <time.h>

#include "../datalib/vector.h"
#include "../datalib/volume.h"
#include "../mathlib/statistic.h"
#include "../mathlib/transform.h"
#include "../mathlib/moments.h" 


#include "../errorcode.h"

#include "../datalib/datacube.h"


#include "../reconstructionlib/multicompartmentlist.h"

#include "command_line_parser.h"

using namespace std;
using namespace pramodx;


int callBack(int percentage)
{
  cout << percentage << "% done" << endl;
  return 0;
}


int
main(int argc, char **argv)
{
  DataCube cube(3);
  
  command_line_parser parser(argc, argv);
    if (argc<3) 
    {
      cout << "Usage: anisofilter [Options] inputfilename.tiff  outputfilename.tiff ,  use -h for more help" << endl;
      return 0;
    };
  
  for (int i=0;i<parser.get_n_options();i++)
    {
      option o=parser.get_option(i);
      if (strcmp(o.get_option(),"h")==0 || parser.get_n_filenames()==0)
	{
	 
	  cout << "Usage: anisofilter [Options] inputfilename.tiff  outputfilename.tiff " << endl; 
	  cout << "             Options(optional)" << endl;
	  cout << "-------------------------------------------" << endl;
     	
	  cout << "-------------------------------------------" << endl;
	  cout << "Programmed by Philip J. Broser and Roland Schulte." << endl;
	  cout << "Report bugs to <philip.broser@mpimf-heidelberg.mpg.de>." << endl;
	  return 0;
	}
	
	//Parser command go in here
   };  


   
  char inputfilename[1024];
  sprintf(inputfilename,parser.get_filename(0));
  cout << "Input File: " << inputfilename << endl;
  char outputfilename[1024];
  sprintf(outputfilename,parser.get_filename(1));
  cout << "Out put file:" << outputfilename << endl;
  
 int result=cube.ReadFromTIFF(inputfilename);
 if (result!=0 && result!=NO_SPACEINGS_TAGS)
 {
 	cout << "Error loading file " << inputfilename << endl;
 	return 1;
 }
  	MultiCompartmentList *multi = new MultiCompartmentList(&cube, callBack);
    FilterOptions *filterOptions = new FilterOptions;
    /* To be done! 
    filterOptions->tau=timeStepSize;
    filterOptions->time_steps=nrOfTimeSteps;
    filterOptions->integration_size_x=scanningRange;
    filterOptions->integration_size_y=scanningRange;
    filterOptions->integration_size_z=scanningRange;
    */
    
    
    //Create hierachy
    int i=0, maxsize=200;
    for (i=0;(i<10 && !(maxsize<100) );i++)
	maxsize=multi->createNextLevelSmart();
	
	
   multi->filter(*filterOptions);
    
    
    cout << "Finsed Filtering, write output file" << endl << flush; 
  if (cube.WriteToTIFF(outputfilename))
  {
  	cout << "Error writing file" << outputfilename << endl;
  	return 1;
  };
  
  
  
  return 0;
}


