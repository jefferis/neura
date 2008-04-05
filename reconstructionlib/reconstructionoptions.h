/****************************************************************************/
/*                                                                          */
/* File:      reconstructionoptions.h                                       */
/*                                                                          */
/* Purpose:   datastructure for reconstruction                              */
/*                                                                          */
/* Author:    Philip Broser                                                 */
/*            Interdisziplinaeres Zentrum fuer Wissenschaftliches Rechnen   */
/*            Universitaet Heidelberg                                       */
/*            Im Neuenheimer Feld 368                                       */
/*            D-69120 Heidelberg                                            */
/*                                                                          */
/* EMail:     philip.broser@iwr.uni-heidelberg.de                           */
/*                                                                          */
/* History:   29.04.03 begin                                                */
/*                                                                          */
/* Remarks:                                                                 */
/*                                                                          */
/****************************************************************************/
#ifndef OPTIONS
#define OPTIONS
#include "../datalib/vector.h"
#include "../mathlib/filteroptions.h"


#define MAXFILENAMELENGTH 100


enum FILETYPE{CFD, XML, NIH, RAW, VREND, TEST, TIFF};

class Options
{
public:
    Options();
    virtual ~Options();
    BOOL skip;
    char HelpMsg[1024];

/*
    virtual function to make the class polymorphic.
    This will result in the possibility to transport
    a derived class in a 'Option' pointer and then
    perform a dynamic_cast on it.
*/

protected:
    virtual void Dummy(){};

};

class LoadingOptions : public Options
{
	public:
	LoadingOptions();
	LoadingOptions(LoadingOptions& lo);
	FILETYPE filetype;
	char FileName[MAXFILENAMELENGTH];
	
	BOOL MultipleFiles;
	int StartNr;
	int EndNr;
	int channel;
	
	BOOL extractVolume;
	int start[3];
	int size[3];
        int operator=(LoadingOptions& lo);
	

};


/*! 
*	Vector PointAB points from set marks
*/
class PreProcessOptions : public Options
{
	public:
	PreProcessOptions();
    
	BOOL deleteBoundary;
	BOOL erasePipet;
	
	double radiusSmall;
	double radiusBig;
	Vector<double> pointSmall;
	Vector<double> pointBig;

	Vector<int> Point11;
	Vector<int> Point12;
	
	Vector<int> Point21;
	Vector<int> Point22;
	
	
   int boundarysize;
        
        int operator=(PreProcessOptions& lo);
	
};


class TransformOptions : public Options
{
	public:
	TransformOptions();
	
	double ThresholdLow;
	double ThresholdHigh;
        int operator=(TransformOptions& lo);
	
};


//FilterOptions are in File filteroptions.h

class RecFilterOptions : public Options, public FilterOptions
{
public:
    RecFilterOptions();
    int filterLevel; // -1 for Automatic detection
    int operator=(RecFilterOptions &lo);

};









enum SegmentationType{GlobalThresholding, LocalMeanValueAndNeigbours, Otsu, FastMarching};
class SegmentationOptions : public Options
{
	public:
	SegmentationOptions();
	
	SegmentationType	segmentationType;
	
	double epsilon;
        double absoluteHighThreshold;
        double absoluteLowThreshold;
        int operator=(SegmentationOptions &lo);
        
};

class PostProcessOptions : public Options
{
public:
    PostProcessOptions();

  
    //int operator=(PostProcessOptions &lo);

};






//FilterOPtions are in File filteroptions.h

class TreeGenerationOptions : public Options
{
public:
    TreeGenerationOptions();

    BOOL writeDX;
    char dxFileName[MAXFILENAMELENGTH];

    BOOL createReconstr;

    BOOL writeHocFile;
    char hocFileName[MAXFILENAMELENGTH];

    
    Vector<int> body;
	 double minLengthOfDendriteInqm;
	 double minDistanceFromBodyCompartment;

	//to be deleted
	 int maxlevel;
    int thresholdlevel;
    double threshold;
//


    int operator=(TreeGenerationOptions &lo);
};





#endif
