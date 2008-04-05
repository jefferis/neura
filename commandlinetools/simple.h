#ifndef SIMPLE_H
#define SIMPLE_H


#include <stdio.h>

#include <fstream.h>
#include <stdio.h>
#include <time.h>

#include "../datalib/vector.h"
#include "../mathlib/statistic.h"
#include "../mathlib/transform.h"


#include "../errorcode.h"

#include "../datalib/datacube.h"
#include "../datalib/cfddatacube.h"
#include "../datalib/testcubes.h"
#include "../segmentationlib/segmentation.h"


#include "command_line_parser.h"


class simple : public Segmentation, public DataCube
{

  public:
  //DataCube convert;
  //Constructor
  simple::simple(DataCube *sourceCube, DataCube *targetCube) : Segmentation(sourceCube, targetCube)
    {}
 
//Segmentation
int first();



};


int simple::first()
    {
      //possible kinds of segmentation

      //SegmentateByOtsuDevidingThroughTheThreshold();
 
      //SegmentateByOtsuMethod();

      //SegmentateByOtsuDevidingThroughTheThreshold();

      //SegmentateByStructuralThresholding(double abslow, double abshigh, double eps);

      SegmentateByGlobalThresholding(0.0);

      //SegmentateByRegionGrowing(RegionGrowingTypes vMode, int Stoptime,int x, int y, int z );

    }


#endif
