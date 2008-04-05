/****************************************************************************/
/*                                                                          */
/* File:      transform.h 	                                            */
/*                                                                          */
/* Purpose:   class for simple datatransformation                           */
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
/* History:   28.10.2002                                                    */
/*                                                                          */
/* Remarks:                                                                 */
/*                                                                          */
/****************************************************************************/


#ifndef TRANSFORM_H
#define TRANSFORM_H
#include <stdio.h>
#include <math.h>

#include "../datalib/volume.h"
#include "../datalib/vector.h"
#include "statistic.h"
#include "../datalib/datacube.h"
#include "../errorcode.h"

enum Neigbour {
    //top Front/Back Right/Left
    TopFront,
    TopBack,
    TopFrontRight,
    TopRight,
    TopLeft,
    TopFrontLeft,
    TopBackLeft,
    TopBackRight,
    Top,
    //samelevel
    THIS,
    Right,
    FrontRight,
    Back,
    Left,
    FrontLeft,
    BackLeft,
    BackRight,
    Front,
    //bottom
    BottomRight,
    BottomFrontRight,
    BottomBack,
    BottomLeft,
    BottomFrontLeft,
    BottomBackLeft,
    BottomBackRight,
    BottomFront,
    Bottom,
    END
};




class Transform
{
public:
    Transform(DataCube *dCube);
    Transform(DataCube *dCube, int (*callBack) (int percentage));
    ~Transform();

    //Statistical correction
    int  eqalMaxValueRespZ();
    int  eqalMinValueRespZ();
    int  eqalMeanValueRespZ();
    int  ThresholdDataLow(double lowerLimit, double lowValue);
    int  ThresholdDataHigh(double higherLimit, double highValue);
    int  ScaleData(double factor);
    int  AutoScaleData();

    //Simple linear algebra opperation
    int  Potentiate(double power);
    int  addScalar(int z,double sum);
    int  addScalar(double sum);


    //segmentation
    int StructuralThresholding(double abslow, double abshigh, double eps,DataCube **target);
    int StructuralThresholding(double abslow, double abshigh, double eps,DataCube *target);
    int StructuralThresholding2D(double abslow, double abshigh, double eps,DataCube *target);

    int GlobalThresholding(double Threshold, DataCube *target);

    //geometrical manipulation 
    int deleteCone(Vector<double> &smallP,double r,Vector<double>bigP,double R);
    int deleteBoudary(int boundarysize);

    //derivated cubes
    int maxIntensityProjection(int direction, int nrOfSlices, DataCube *target);
    
    friend ostream &operator<<(ostream &ostr,Transform &Trans);

private:
    int (*callBackFunction) (int percentage);
    
    double GetNeigbourOfVoxelNr(Neigbour n,Vector<int> vox);
    double CheckVoxel(int x, int y, int z);

    double absolutelowthreshold, absolutehighthreshold, epsilon ;
    DataCube *dataCube;
    Statistic stat;

};

 
#endif

