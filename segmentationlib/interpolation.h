/****************************************************************************/
/*                                                                          */
/* File:      interpolation.h 	                                            */
/*                                                                          */
/* Purpose:   class definition for interpolation data in datacubes          */
/*                                                                          */
/* Author:    Christine Vossen                                              */
/*            Interdisziplinaeres Zentrum fuer Wissenschaftliches Rechnen   */
/*            Universitaet Heidelberg                                       */
/*            Im Neuenheimer Feld 368                                       */
/*            D-69120 Heidelberg                                            */
/*                                                                          */
/* EMail:     christine.vossen@gmx.de                                       */
/*                                                                          */
/* History:   30.04.2003                                                    */
/*                                                                          */
/* Remarks:                                                                 */
/*                                                                          */
/****************************************************************************/

// The class interpolation expects a DataCube which is filled with 0 and with the calculated values
// in the middle of the overlapping smallCubes with the size smallSize.
// The interpolation is linear and can be used with the function interpolateDataCube()  




#ifndef INTERPOLATION_H
#define INTERPOLATION_H


#include <stdio.h>

#include "../datalib/vector.h"
#include "../datalib/volume.h"
#include "../errorcode.h"
#include "../datalib/datacube.h"



class interpolation {

 private:
  DataCube* tDC;
  Vector<int> sizeSC;
 
  double phi (int j, int i, int h, int k, Vector<int> size);
  

 public:
   interpolation(Vector<int> smallSize, DataCube *cube); 
   ~interpolation();   
   int interpolateDataCube();
 
};



#endif
