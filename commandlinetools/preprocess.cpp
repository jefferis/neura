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
  preProcessOptions.skip=NO;
  
  command_line_parser parser(argc, argv);
  loadingOptions.filetype=NIH;
  if (argc<2) 
    {
      cout << "Usage: preprocess[Options] filename.cfd  ,  use -h for more help" << endl;
      return 0;
    };
  int counter=0;
  for (int i=0;i<parser.get_n_options();i++)
    {
      option o=parser.get_option(i);
      if (strcmp(o.get_option(),"h")==0 || parser.get_n_filenames()==0)
	{
	  cout << "Usage: preprocess [Options] filename.nih " << endl; 
	  cout << "Options(optional)" << endl;
	  cout << "-------" << endl;
	  cout << "-psx:value  Small point of pipet x" << endl;
	  cout << "-psy:value  Small point of pipet y" << endl;
	  cout << "-psz:value  Small point of pipet z" << endl;
	  cout << "-rs:value   Small radius" << endl;
          cout << "-pbx:value  Big point of pipet x" << endl;
          cout << "-pby:value  Big point of pipet y" << endl;
          cout << "-pbz:value  Big point of pipet z" << endl;
          cout << "-rb:value   Big radius" << endl << endl;
          cout << "-del:value  Delete boundary voxel, value is boundary spread" << endl;
          
          
          cout << endl;
	  cout << "Output" << endl;
	  cout << "The output will be written to the files:" << endl;
	  cout << "preprocessresult.nih  : Saved DataCube for later processing"<< endl;
	  cout << "datatoshow.dat    : Data as unsigned char x,y,z "<< endl;
	  cout << "datatoshow.txt    : Information about size and spaceing"<< endl;
	  cout << "For debugging purpose you will also find a file" << endl;
	  cout << "loadedcube.dat    : native data see above" << endl;
	  return 0;
	}
      
      
      if (strcmp(o.get_option(),"psx")==0)
	{
          preProcessOptions.erasePipet=YES;
          int help;
	  if (sscanf (o.get_value(),"%i",&help)==EOF) continue;
           preProcessOptions.pointSmall[1]=help;
          cout << "pbx:" << preProcessOptions.pointSmall[1] << endl;
          counter++;
	};
      if (strcmp(o.get_option(),"psy")==0)
      {
          preProcessOptions.erasePipet=YES;
          int help;
          if (sscanf (o.get_value(),"%i",&help)==EOF) continue;
           preProcessOptions.pointSmall[2]=help;
          cout << "pby:" << preProcessOptions.pointSmall[2] << endl;
           counter++;
      };
      if (strcmp(o.get_option(),"psz")==0)
      {
          preProcessOptions.erasePipet=YES;
          int help;
          if (sscanf (o.get_value(),"%i",&help)==EOF) continue;
           preProcessOptions.pointSmall[3]=help;
          cout << "pbz:" << preProcessOptions.pointSmall[3] << endl;
           counter++;
      };

      if (strcmp(o.get_option(),"pbx")==0)
      {
          preProcessOptions.erasePipet=YES;
          int help;
          if (sscanf (o.get_value(),"%i",&help)==EOF) continue;
           preProcessOptions.pointBig[1]=help;
          cout << "pbx:" << preProcessOptions.pointSmall[1] << endl;
           counter++;
      };

      if (strcmp(o.get_option(),"pby")==0)
      {
          preProcessOptions.erasePipet=YES;
          int help;
          if (sscanf (o.get_value(),"%i",&help)==EOF) continue;
           preProcessOptions.pointBig[2]=help;
          cout << "pby:" << preProcessOptions.pointSmall[2] << endl;
           counter++;
      };

      if (strcmp(o.get_option(),"pbz")==0)
      {
          preProcessOptions.erasePipet=YES;
          int help;
          if (sscanf (o.get_value(),"%i",&help)==EOF) continue;
           preProcessOptions.pointBig[3]=help;
          cout << "pbz:" << preProcessOptions.pointSmall[3] << endl;
           counter++;
      };
      
      
      if (strcmp(o.get_option(),"rs")==0)
	{
          double help;
	  if (sscanf (o.get_value(),"%f",&help)==EOF) continue;
          preProcessOptions.radiusSmall=help;
	  cout << "Radius small:" << preProcessOptions.radiusSmall << endl;
           counter++;
	};
      
      if (strcmp(o.get_option(),"rb")==0)
      {
          double help;
          if (sscanf (o.get_value(),"%f",&help)==EOF) continue;
          preProcessOptions.radiusBig=help;
          cout << "Radius big:" << preProcessOptions.radiusBig << endl;
           counter++;
      };


      if (strcmp(o.get_option(),"del")==0)
      {
          int help;
          if (sscanf (o.get_value(),"%i",&help)==EOF) help=4;
          preProcessOptions.deleteBoundary=YES;
          preProcessOptions.boundarysize=help;
          cout << "Delete bounday with boundary size:" << preProcessOptions.boundarysize << endl;
      };

      if (strcmp(o.get_option(),"del")==0)
      {
          int help;
          if (sscanf (o.get_value(),"%i",&help)==EOF) help=4;
          preProcessOptions.deleteBoundary=YES;
          preProcessOptions.boundarysize=help;
          cout << "Delete bounday with boundary size:" << preProcessOptions.boundarysize << endl;
      };



    };


  if (preProcessOptions.erasePipet==YES && counter<8)
  {
      cout << "Missing data to delete pipet, use -h for more help" << endl;
      return 1;

  }
  sprintf(loadingOptions.FileName,parser.get_filename(0));
  printf("Process with file:");
  printf(loadingOptions.FileName);
  printf("\n");

  
  reconstruction.SetLoadingOptions(loadingOptions);
  reconstruction.SetPreProcessOptions(preProcessOptions);
  

  if (reconstruction.Load()){cout << "Error: Load"<< endl;return 1;};
  result=reconstruction.GetCube(ORIGINCUBE);
  result->writeRAW("loadedcube.dat","loadedcube.txt");
  cout << (*result) << endl;
  cout << "Preprocess" << endl;
  if (reconstruction.PreProcess()) {cout << "Error: Preprocess Data"<< endl;return 1;};
  cout << "Save data" << endl;
  cout << "Save cube as NIH" << endl;
  if (reconstruction.SaveCube(PREPROCESSED,"preprocessresult.nih",NIH)) {cout << "Error: Saving" << endl;return 1;};
  result=reconstruction.GetCube(PREPROCESSED);
  cout << "Save cube as raw" << endl;
  result->writeRAW("preprocessdatatoshow.dat","preprocessdatatoshow.txt");

  return 0;
}


