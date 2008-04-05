#ifndef TIFFIO
#define TIFFIO

#include <stdio.h>
#include <tiffio.h>
#include "datatypes.h"

extern int readTiffFile(char *fileName, densityDataType *densityData);
extern int writeTiffFile(char *fileName, densityDataType densityData);



#endif

