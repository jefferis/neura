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
#include "../datalib/cfddatacube.h"
#include "../datalib/testcubes.h"

#include "../reconstructionlib/reconstruction.h"
#include "../mathlib/nld.h"  

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
  DataCube *result;
  Reconstruction reconstruction(callBack);
  LoadingOptions loadingOptions;
  PreProcessOptions preProcessOptions;
  TransformOptions transformOptions;
  RecFilterOptions recFilterOptions;
  preProcessOptions.skip=NO;
  transformOptions.skip=NO;
  recFilterOptions.skip=NO;
  command_line_parser parser(argc, argv);
  loadingOptions.filetype=CFD;
  if (argc<2) 
    {
      cout << "Usage: cfdfilter[Options] filename.cfd  ,  use -h for more help" << endl;
      return 0;
    };
  
  for (int i=0;i<parser.get_n_options();i++)
    {
      option o=parser.get_option(i);
      if (strcmp(o.get_option(),"h")==0 || parser.get_n_filenames()==0)
	{
	 
	  cout << "Usage: cfdfilter [Options] filename.cfd " << endl; 
	  cout << "             Options(optional)" << endl;
	  cout << "-------------------------------------------" << endl;
          cout << "-lnih      load nih file insted of cfd" << endl;
	  cout << "-ch:nr     Read Channel nr" << endl;
	  cout << "-is:value  Set Integrationsize to value(int), must be odd" << endl;
	  cout << "-ts:nr     Number of time steps" << endl;
	  cout << "-fl:level  Filter on level" << endl;
	  cout << "-tl:value  Set low threshold to value(0..255)" << endl;
	  cout << "-th:value  Set high threshold to value(0..255)" << endl;
	  cout << endl;
	  cout << "Output" << endl;
	  cout << "The output will be written to the files:" << endl;
	  cout << "filterresult.nih  : Saved DataCube for later processing"<< endl;
	  cout << "datatoshow.dat    : Data as unsigned char x,y,z "<< endl;
	  cout << "datatoshow.txt    : Information about size and spaceing"<< endl;
	  cout << "For debugging purpose you will also find a file" << endl;
	  cout << "loadedcube.dat    : native data see above" << endl;

	  cout << "-------------------------------------------" << endl;
	  cout << "Programmed by Philip J. Broser and Roland Schulte." << endl;
	  cout << "Report bugs to <philip.broser@mpimf-heidelberg.mpg.de>." << endl;
	  return 0;
	}
      if (strcmp(o.get_option(),"lnih")==0)
      {
          loadingOptions.filetype=NIH;
          cout << "Read nih file" << endl;
      };
      
      if (strcmp(o.get_option(),"ch")==0)
	{
	  if (sscanf (o.get_value(),"%i",&(loadingOptions.channel))==EOF) continue;
	  cout << "Reading Channel:" << loadingOptions.channel << endl;
	};
      if (strcmp(o.get_option(),"is")==0)
	{
	  if (sscanf (o.get_value(),"%i",&(recFilterOptions.integration_size_x))==EOF) continue;
	  cout << "Integartion size:" << recFilterOptions.integration_size_x << endl;
	};

      if (strcmp(o.get_option(),"ts")==0)
	{
	  if (sscanf (o.get_value(),"%i",&(recFilterOptions.time_steps))==EOF) continue;
	  cout << "Time steps:" << recFilterOptions.time_steps << endl;
	};
      if (strcmp(o.get_option(),"fl")==0)
	{
	  if (sscanf (o.get_value(),"%i",&(recFilterOptions.filterLevel))==EOF) continue;
	  cout << "Filter level:" << recFilterOptions.filterLevel << endl;
	};



      
      if (strcmp(o.get_option(),"tl")==0)
	{
	  int help=0;
	  if (sscanf (o.get_value(),"%i",&help)==EOF) continue;
	  transformOptions.ThresholdLow=help;
	  cout << "Threshold low:" << transformOptions.ThresholdLow << endl;
	};
      if (strcmp(o.get_option(),"th")==0)
	{
	  int help=255;
	  if (sscanf (o.get_value(),"%i",&help)==EOF) continue;
	  transformOptions.ThresholdHigh=help;
	  cout << "Threshold High:" << transformOptions.ThresholdHigh << endl;
	};

        
    };
  
  sprintf(loadingOptions.FileName,parser.get_filename(0));
  printf("Process with file:");
  printf(loadingOptions.FileName);
  printf("\n");

  
  reconstruction.SetLoadingOptions(loadingOptions);
  reconstruction.SetPreProcessOptions(preProcessOptions);
  reconstruction.SetTransformOptions(transformOptions);
  reconstruction.SetFirstFilterOptions(recFilterOptions);

  if (reconstruction.Load()){cout << "Error: Load"<< endl;return 1;};
  if (reconstruction.SaveCube(ORIGINCUBE,"loadedcube.nih",NIH)){cout << "ERROR, while writing loadedcube.nih";return 1;}
  result=reconstruction.GetCube(ORIGINCUBE);
  result->writeRAW("loadedcube.dat","loadedcube.txt");
  if (reconstruction.PreProcess()) {cout << "Error: TransformData"<< endl;return 1;};
  if (reconstruction.TransformData()) {cout << "Error: TransformData"<< endl;return 1;};
  if (reconstruction.FirstFilter()) {cout << "Error: FirstFilter"<< endl;return 1;};
  if (reconstruction.SaveCube(FILTERED,"filterresult.nih",NIH)) {cout << "Error: Saving" << endl;return 1;};
  result=reconstruction.GetCube(FILTERED);
  result->writeRAW("datatoshow.dat","datatoshow.txt");

  return 0;
}


