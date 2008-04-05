/****************************************************************************/
/*                                                                          */
/* File:      datacube.h 	                                            */
/*                                                                          */
/* Purpose:   class extension for cfd data handling                         */
/*                                                                          */
/* Author:    Philip J. Broser                                              */
/*            Max-Planck-Institut fuer medizinische Forschung               */
/*            Jahnstrasse 29                                                */
/*	      D-69120 Heidelberg                                            */
/*                                                                          */
/*                                                                          */
/*            Interdisziplinaeres Zentrum fuer Wissenschaftliches Rechnen   */
/*            Universitaet Heidelberg                                       */
/*            Im Neuenheimer Feld 368                                       */
/*            D-69120 Heidelberg                                            */
/*                                                                          */
/* EMail: philip.broser@iwr.uni-heidelberg.de                               */
/*                                                                          */
/* History:   29.10.2002                                                    */
/*                                                                          */
/* Remarks:                                                                 */
/*                                                                          */
/****************************************************************************/


#ifndef CFDDATACUBE_H
#define CFDDATACUBE_H
#include <stdio.h>

#include "volume.h"
#include "vector.h"
#include "datacube.h"

#include "../cfdlib/cfdReader.h"

#include "../errorcode.h"


class CfdDataCube : public DataCube
{
 public:
  CfdDataCube();
  ~CfdDataCube();
  
  int LoadCfdData(char Name[FILENAMELENGTH]);
  int LoadCfdData(char templateName[FILENAMELENGTH], int startNr, int endNr);
  int LoadCfdData(char templateName[FILENAMELENGTH], int startNr, int endNr, int *StartVector, int *SizeVector);

  int LoadCfdData(char templateName[FILENAMELENGTH], int startNr, int endNr, int *StartVector, int *SizeVector,int channel);
  int LoadCfdData(char templateName[FILENAMELENGTH], int startNr, int endNr,int channel);

  int deleteCone(Vector<double> &smallP,double r,Vector<double>bigP,double R);
  
  friend ostream &operator<<(ostream &ostr, CfdDataCube &DC);  
  
 private:
 


};
 
#endif













