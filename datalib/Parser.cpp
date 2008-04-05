#include "Parser.h"

ParameterSet::ParameterSet()
{
    CreateStringParameter("FILENAME");
    CreateIntegerParameter("XMLCFD");
    CreateIntegerParameter("CHANNEL");
    CreateIntegerParameter("FLAGS");

    CreateIntegerParameter("EVOLUME");
    CreateIntegerParameter("STARTX");
    CreateIntegerParameter("STARTY");
    CreateIntegerParameter("STARTZ");

    CreateIntegerParameter("SIZEX");
    CreateIntegerParameter("SIZEY");
    CreateIntegerParameter("SIZEZ");

    CreateIntegerParameter("ERASEPIPET");

    CreateDoubleParameter("BIGX");
    CreateDoubleParameter("BIGY");
    CreateDoubleParameter("BIGZ");
    CreateDoubleParameter("RADIUSB");

    CreateDoubleParameter("SMALLX");
    CreateDoubleParameter("SMALLY");
    CreateDoubleParameter("SMALLZ");
    CreateDoubleParameter("RADIUSS");

    CreateDoubleParameter("THRESHOLDDATAHIGH");
    CreateDoubleParameter("THRESHOLDDATALOW");


    CreateIntegerParameter("CELLBODYX");
    CreateIntegerParameter("CELLBODYY");
    CreateIntegerParameter("CELLBODYZ");

    CreateIntegerParameter("MAXLEVEL");
    CreateIntegerParameter("FILTERLEVEL");
    CreateDoubleParameter("THRESHOLD");
    CreateIntegerParameter("THRESHOLDLEVEL");
    CreateIntegerParameter("REDUCTIONLEVEL");

    CreateDoubleParameter("TIMESTEPSIZE");
    CreateIntegerParameter("TIMESTEPS");
    CreateIntegerParameter("INTEGRATIONSIZE");
    CreateStringParameter("KINDOFFILTER");
    CreateDoubleParameter("LAMBDA");
    CreateStringParameter("RECONSTRUCT");

    CreateStringParameter("OUTPUTHOC");

    CreateStringParameter("NIHOUTFILENAME");
    CreateStringParameter("NIHOUTINFOFILENAME");
    CreateStringParameter("XMLOUTFILENAME");

    CreateStringParameter("NIHOUTFILENAMEBEFORE");
    CreateStringParameter("NIHOUTINFOFILENAMEBEFORE");
}


  
 



ParameterSet::~ParameterSet()
{
}



int ParameterSet::CreateDoubleParameter(char* name)
{
  doubleParameters.CreateSorted(name);
    return 0;
}



int ParameterSet::CreateIntegerParameter(char* name)
{
  integerParameters.CreateSorted(name);
    return 0;
}



int ParameterSet::CreateStringParameter(char* name)
{
  stringParameters.CreateSorted(name);
    return 0;
}





int ParameterSet::Read(char* filename)
{
  const int maxLength = 100;

  char string[maxLength];

  double doubleValue;
  int integerValue;
  char stringValue[maxLength];

  ifstream i(filename);

  while(!i.eof())
    {
      i >> string;

      if(strcmp(string,"*")==0)
	{
	  do
	    {
	      i >> string;
	    }
	  while(strcmp(string,"*")!=0);

	  i >> string;
	}

      if(!doubleParameters.GoTo(string))
	{
	  //	  cout << string << endl << endl;
	  i >> doubleValue;
	  doubleParameters.Insert(doubleValue);
	}
      else if(!integerParameters.GoTo(string))
	{
	  i >> integerValue;
	  integerParameters.Insert(integerValue);
	}
      else if(!stringParameters.GoTo(string))
	{
	  i >> stringValue;
	  stringParameters.Insert(stringValue);
	}
      else
	{
	  cout << "Warning: Unknown parameter " << string << "!" << endl;
	}
    }

  i.close();

  return(0);
}

  
  
int ParameterSet::GetDouble(char* name, double& value)
{
  if(!doubleParameters.GoTo(name))
    {
      doubleParameters.Get(value);
      //      cout << value << endl;
    }
  else
    {
      cout << "Error: Unknown double parameter!" << endl;
      exit(1);
    }

  return(0);
}



int ParameterSet::GetInteger(char* name, int& value)
{
  if(!integerParameters.GoTo(name))
    {
      integerParameters.Get(value);
    }
  else
    {
      cout << "Error: Unknown integer parameter!" << endl;
      exit(1);
    }

  return(0);
}



int ParameterSet::GetString(char* name, String& value)
{
  if(!stringParameters.GoTo(name))
    {
      stringParameters.Get(value);
      //      cout << "GetString: " << value << endl;
    }
  else
    {
      cout << "Error: Unknown string parameter!" << endl;
      exit(1);
    }

  return(0);
}

