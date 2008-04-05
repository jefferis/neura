
#ifndef PARAMETERSET_HPP
#define PARAMETERSET_HPP

#include <string.h>
#include <fstream.h>

#include "ParserUtilities.h"

class ParameterSet
{

private:
  ParameterTree<int> integerParameters;
  ParameterTree<double> doubleParameters;
  ParameterTree<String> stringParameters;

public:
  ParameterSet();
  ParameterSet(char* filename);
  ~ParameterSet();
  
  int CreateDoubleParameter(char* name);
  int CreateIntegerParameter(char* name);
  int CreateStringParameter(char* name);

  int Read(char* filename);
  int GetDouble(char* name, double&);
  int GetInteger(char* name, int&);
  int GetString(char* name, String&);
};


#endif // PARAMETERSET_HPP
