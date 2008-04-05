/****************************************************************************/
/*                                                                          */
/* File:      primitive.h 	                                                 */
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
#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include <stdio.h>
#include <string.h>
#include "../datalib/vector.h"
#include "../datalib/volume.h"
#include "../datalib/datacube.h"

#include <iostream.h>
#include <list.h>
#include <string>
#include "../errorcode.h"

#define XAxis 1
#define YAxis 2
#define ZAxis 3

#define YZPlane 1
#define XZPlane 2
#define YXPlane 3




#define FOREGROUND 1
#define BACKGROUND 0
#define DONTCARE   2

#define XLeft   0
#define XMiddle 1
#define XRight  2

#define YBack   0
#define YMiddle 1
#define YFront  2

#define ZBottom  0
#define ZMiddle  1
#define ZTop     2

#define MAXNROFPRIM6  13
#define MAXNROFPRIM26 22

class Primitive
{
public:
    Primitive(int nr);
    Primitive(int nr, int kindofneigbourhood); //6, 18, 26
    Primitive();
    Primitive(unsigned char d[3][3][3]);

    int Rotate(short axis, short angle);
    int Mirror(short plane);
    int writeOnom2K(char* Name);

    int GetDataField(unsigned char d[3][3][3]);
    int GetDerivationNr() {return derivatedFrom;}
    friend int operator==(const Primitive &P1,const Primitive &P2);
	 friend int TheyAreTheSame(const Primitive &P1,const Primitive &P2);
private:
             void Templates6(int nr);
             void Templates26(int nr);         
    unsigned char data[3][3][3];
    short derivatedFrom;
    //order in datafield is x y z

};


#endif














