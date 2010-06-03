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
	FilterOptions *filterOptions = new FilterOptions;

  command_line_parser parser(argc, argv);
  
	Vector<double> spacings(3);
	bool comandlineSpacings = false;
	
  for (int i=0;i<parser.get_n_options();i++)
  {
    option o=parser.get_option(i);
    if (strcmp(o.get_option(),"h")==0 || parser.get_n_filenames()==0) {

			cout << "Usage: anisofilter [Options] inputfilename.tiff  outputfilename.tiff " << endl; 
			// cout << "             Options(optional)" << endl;
			cout << "-------------------------------------------" << endl;
			
			cout << "Options (default values):" << endl;
			cout << "-scanrange:2"<<endl<<"-tau:4.0"<<endl<<"-ipflag:0"<<endl;
			cout << "-nsteps:4"<<endl<<"-anicoeff{1,3}:1 1 0"<<endl<<"-lambda: 0.1"<< endl;
  	
			cout << "-------------------------------------------" << endl;
			cout << "Programmed by Philip J. Broser and Roland Schulte." << endl;
			cout << "Report bugs to <philip.broser@mpimf-heidelberg.mpg.de>." << endl;
			return 0;
		}		

		if (strcmp(o.get_option(),"scanrange")==0)
		{
			int help;
			if (sscanf (o.get_value(),"%d",&(help))==EOF) continue;
			filterOptions->integration_size_x=help;
			filterOptions->integration_size_y=help;
			filterOptions->integration_size_z=help;
			cout << "scanrange=" << filterOptions->integration_size_x << endl;
		};
		
		if (strcmp(o.get_option(),"tau")==0)
		{
			float help;
			if (sscanf (o.get_value(),"%f",&(help))==EOF) continue;
			filterOptions->tau=help;
			cout << "tau=" << filterOptions->tau << endl;
		};

		if (strcmp(o.get_option(),"ipflag")==0)
		{
			int help;
			if (sscanf (o.get_value(),"%d",&(help))==EOF) continue;
			filterOptions->ip_flag=help;
			cout << "ipflag=" << filterOptions->ip_flag << endl;
		};
	
		if (strcmp(o.get_option(),"nsteps")==0)
		{
	    int help;
	    if (sscanf (o.get_value(),"%d",&(help))==EOF) continue;
	    filterOptions->time_steps=help;
	    cout << "nsteps=" << filterOptions->time_steps << endl;
		};

		if (strcmp(o.get_option(),"anicoeff1")==0)
	  {
	    float coeff;
	    if (sscanf (o.get_value(),"%f",&(coeff))==EOF) continue;
	    filterOptions->anicoeff1=coeff;
	    cout << "anicoeff1=" << filterOptions->anicoeff1 << endl;
	  };

		if (strcmp(o.get_option(),"anicoeff2")==0)
	  {
		  float coeff;
		  if (sscanf (o.get_value(),"%f",&(coeff))==EOF) continue;
		  filterOptions->anicoeff2=coeff;
		  cout << "anicoeff2=" << filterOptions->anicoeff2 << endl;
	  };

		if (strcmp(o.get_option(),"anicoeff3")==0)
	  {
		  float coeff;
		  if (sscanf (o.get_value(),"%f",&(coeff))==EOF) continue;
		  filterOptions->anicoeff3=coeff;
		  cout << "anicoeff3=" << filterOptions->anicoeff3 << endl;
	  };

		if (strcmp(o.get_option(),"lambda")==0)
		{
	    float coeff;
	    if (sscanf (o.get_value(),"%f",&(coeff))==EOF) continue;
	    filterOptions->lambda=coeff;
	    cout << "lambda=" << filterOptions->lambda << endl;
		};
		if (strcmp(o.get_option(),"dx")==0)
		{
			float help;
			if (sscanf (o.get_value(),"%f",&(help))==EOF) continue;
			spacings[1]=help;
			cout << "dx=" << spacings[1] << endl;
			comandlineSpacings=true;
		};
		
		// Handle spacings
		if (strcmp(o.get_option(),"dy")==0)
		{
			float help;
			if (sscanf (o.get_value(),"%f",&(help))==EOF) continue;
			spacings[2]=help;
			cout << "dy=" << spacings[2] << endl;
		};
		
		if (strcmp(o.get_option(),"dz")==0)
		{
			float help;
			if (sscanf (o.get_value(),"%f",&(help))==EOF) continue;
			spacings[3]=help;
			cout << "dz=" << spacings[3] << endl;
		};
		
	};
  
	if (argc<3) 
  {
    cout << "Usage: anisofilter [Options] inputfilename.tiff  outputfilename.tiff ,  use -h for more help" << endl;
    return 0;
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
	// If we were given spacings on the command line use them 
	if(comandlineSpacings)	cube.SetSpaceing(spacings);
	MultiCompartmentList *multi = new MultiCompartmentList(&cube, callBack);
    /* To be done! 
    filterOptions->tau=timeStepSize;
    filterOptions->time_steps=nrOfTimeSteps;
    filterOptions->integration_size_x=scanningRange;
    filterOptions->integration_size_y=scanningRange;
    filterOptions->integration_size_z=scanningRange;
    */
//    filterOptions->tau=4.0;
//    filterOptions->time_steps=2;
//    filterOptions->integration_size_x=30;
//    filterOptions->integration_size_y=30;
//    filterOptions->integration_size_z=30;
//    filterOptions->dependence_type=BLACK_SAPIRO;
    
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


