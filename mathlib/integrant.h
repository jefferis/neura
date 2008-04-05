/****************************************************************************/
/*                                                                          */
/* File:      integrant.h                                                   */
/*                                                                          */
/* Purpose:   routines to calculate integrants                              */
/*                                                                          */
/* Author:    Roland Schulte                                                */
/*            Interdisziplinaeres Zentrum fuer Wissenschaftliches Rechnen   */
/*            Universitaet Heidelberg                                       */
/*            Im Neuenheimer Feld 368                                       */
/*            D-69120 Heidelberg                                            */
/*                                                                          */
/* EMail:     roland.schulte@iwr.uni-heidelberg.de                          */
/*                                                                          */
/* History:   07.11.02 begin                                                */
/*                                                                          */
/* Remarks:                                                                 */
/*                                                                          */
/****************************************************************************/
#ifndef INTEGRANT
#define INTEGRANT
#include "../datalib/array.h"
#include "../datalib/vector.h"
#include "../datalib/volume.h"
#include "../datalib/datacube.h"


class Integrant
{

 public:

    virtual NeuraDataType g( NeuraDataType pixel, const Vector<int> &coord) {return 0;}; 

  virtual void prepare(){};

 private:

};

#endif
