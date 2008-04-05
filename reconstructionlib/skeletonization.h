/****************************************************************************/
/*                                                                          */
/* File:      skeletonization.h                                             */
/*                                                                          */
/* Purpose:   class to handle primitives for skeletonization                */
/*                                                                          */
/* Author:    Philip J. Broser                                              */
/*            Max-Planck-Institut fuer medizinische Forschung               */
/*            Jahnstrasse 29                                                */
/*    	     D-69120 Heidelberg                                            */
/*                                                                          */
/*                                                                          */
/*            Interdisziplinaeres Zentrum fuer Wissenschaftliches Rechnen   */
/*            Universitaet Heidelberg                                       */
/*            Im Neuenheimer Feld 368                                       */
/*            D-69120 Heidelberg                                            */
/*                                                                          */
/* EMail: philip.broser@iwr.uni-heidelberg.de                               */
/*                                                                          */
/* History:   29.11.2002                                                    */
/*                                                                          */
/* Remarks:                                                                 */
/*                                                                          */
/****************************************************************************/
#ifndef SKELETONIZATION_H
#define SKELETONIZATION_H

#include <stdio.h>
#include "../datalib/vector.h"
#include "../datalib/volume.h"
#include "../datalib/datacube.h"

#include "compartment.h"
#include "primitive.h"

#include <iostream.h>
#include <list.h>
#include <string>
#include "../errorcode.h"



class
Skeletonization
{
public:

Skeletonization();
Skeletonization(int kindofneigbourhood);
~Skeletonization();

int writeOnom2k(char *tempalteName);

int IsCompartmentDeleteable(Compartment *comp);
int IsCompartmentDeleteable26(Compartment *comp);
int IsPrimitivetDeleteable(Primitive *prim);
int CompartmentIsDeleteableBy26(Compartment *comp); // returns which template causes deletion
Primitive* CompartmentToPrimitive(Compartment *comp);
Primitive* CompartmentToPrimitive26(Compartment *comp);


private:
list<Primitive*> PrimitveTemplates;


};


#endif

