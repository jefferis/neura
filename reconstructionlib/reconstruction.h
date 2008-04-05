/****************************************************************************/
/*                                                                          */
/* File:      reconstruction.h                                              */
/*                                                                          */
/* Purpose:   control for filtering, segmentation and graph extraction      */
/*                                                                          */
/* Author:    Philip J. Broser                                              */
/*            Max-Planck-Institut fuer medizinische Forschung               */
/*            Jahnstrasse 29                                                */
/*	      D-69120 Heidelberg	                                    */
/*                                                                          */
/*                                                                          */
/*            Interdisziplinaeres Zentrum fuer Wissenschaftliches Rechnen   */
/*            Universitaet Heidelberg                                       */
/*            Im Neuenheimer Feld 368                                       */
/*            D-69120 Heidelberg                                            */
/*                                                                          */
/* EMail: philip.broser@iwr.uni-heidelberg.de                               */
/*                                                                          */
/* History:   18.03.2003                                                    */
/*                                                                          */
/* Remarks:                                                                 */
/*                                                                          */
/****************************************************************************/
#ifndef RECONSTRUCTION
#define RECONSTRUCTION

//System Includes
#include <iostream>
#include <sstream>
#include <math.h>

//Datastructure
#include "../datalib/cfddatacube.h"
#include "../datalib/testcubes.h"

//UserInterface Communication
#include "reconstructionoptions.h"
#include "viewcube.h"

//Mathlib
#include "../mathlib/transform.h"
#include "../mathlib/nld.h"

//Segmentationlib
#include "../segmentationlib/segmentation.h"


//Treegeneration Includes
#include "primitive.h"
#include "skeletonization.h"
#include "compartmentlist.h"
#include "multicompartmentlist.h"
#include "nervecell.h"




enum KindOfCubes{ORIGINCUBE, TRANSFORMED, FILTERED, SEGMENTED, FILTEREDSEG, FINAL, PREPROCESSED, COMPPLOT, RECPLOT, //base cubes
		 PREPROCMAXINT5, PREPROCMAXINT10, ORIGINMAXINT5, ORIGINMAXINT10, COMPMAXINT5,  COMPMAXINT10	    //further cubes which are created from the base cubes
};


class Reconstruction
{
    
public:
    Reconstruction();
    Reconstruction(int (*callBack) (int percentage));
    ~Reconstruction();
    
    //Execute Modules
    //Filter and Segmentation Modules
    int Load(); //success-> state=1; returns errocode for no spaceings when tiff reader finds no tiff tags NO_SPACEINGS_TAGS
    int TransformData();
    int FirstFilter();
    int FirstSegmentation();
    int SecondFilter();
    int SecondSegmentation();
    
    //Reconstruction Modules
    int PreProcess();
    int TreeGenerationDetectTerminations();
    
    //dendrite Termination Marks
    list<Mark*>* GetMarkList();
    int AddMark(Mark &newMark);
    int DeleteMark(int idOfTermination);
    
    int TreeGenerationCreateNervecell();
    
    // Marks for geometric Orientation
    // Barrell, Pia or addtional
    // Gui model layer stays owner of the marks, but is not allowed to remove a mark before calling RemoveOrientationMark()!!!
    // but the gui ist allowed to change everything of the mark, exept the id!
    int AddOrientationMark(Mark &newMark); //
    int RemoveOrientationMark(int idOfMark);
    
    
    
    
    //SetOptions
    int SetLoadingOptions(LoadingOptions &lo) {loadingOptions=lo;cout << "SetLoadingOptions" << endl << "filetype:" << loadingOptions.filetype << endl; return 0;}; 
    int SetTransformOptions(TransformOptions &trans) {transformOptions=trans;return 0;}
    int SetFirstFilterOptions(RecFilterOptions &fo) {filterOptions1=fo;return 0;};
    int SetFirstSegmentationOptions(SegmentationOptions &se) {segmentationOptions1=se;return 0;};
    int SetSecondFilterOptions(RecFilterOptions &fo) {filterOptions2=fo;return 0;};
    int SetPreProcessOptions(PreProcessOptions &pre) {preProcessOptions=pre;return 0;};
    int SetSpaceings(Vector<double>& spaceings);
    
    int SetSecondSegmentationOptions(SegmentationOptions &se) {segmentationOptions2=se;return 0;};
    int SetTreeGenerationOptions(TreeGenerationOptions &tree) {treeGenerationOptions=tree;return 0;};
    
    //Data accsessing
    DataCube* GetCube(KindOfCubes kind);  
    Cellbody* AccsessCellBody();
    
    //Gui Viewer 
    ViewCube* CreateViewCube(KindOfCubes kind);
    int SetContrast(int value); //0..100
    int SetBrightness(int value);//0..100
    
    //DataIOModules
    int SaveCube(KindOfCubes kind,char *name,FILETYPE type);
    int WriteHocFile(char *filename);
    int WriteNeuronAsDXFile(char *filename);
    
	//Messageing
    friend ostream &operator<<(ostream &ostr, const Reconstruction &rec);
    
    //<ALEX>
    inline int GetState() const {return state;};
    //</ALEX>
    
    
private:
	//GUI window
    ViewCube *presentView;
    bool presentViewAllocated;
    int contrast, brightness; //0..100
	
    int CleanUp(int toState); //frees memory
    int state;
    //will bee shown at tap  
    DataCube *origin; //Loading
    bool originAllocated;
	
	list<Vector<double> > *cellbodyList;
    
    DataCube *transformed;
    bool transfomedAllocated;
    
    DataCube *filtered;
    bool filteredAllocated;
    
    DataCube *segmented;
    bool segmentedAllocated;
    
    DataCube *filteredsegmented; // second filter
    bool filteredsegmentedAllocated;
    
    DataCube *postprocess; //not implemeted yet!
    bool postprocessAllocated;
    
    DataCube *final; //mark boundary
    bool finalAllocated;
    
    DataCube *preprocessed;
    bool preprocessedAllocated;	
    
    //Ploted reconstruction
    DataCube *compartmentsPlot;
    bool compartmentsPlotAllocated;
    
    DataCube *reconstructionPlot;
    bool reconstructionPlotAllocated;
    
    DataCube *nervecellPlot;
    bool nervecellPlotAllocated;
    
    MultiCompartmentList *multiCompartmentList;
    Cellbody *cellBody;
    
    
    
    //Options
    LoadingOptions loadingOptions;
    PreProcessOptions	preProcessOptions;
    TransformOptions transformOptions;
    RecFilterOptions filterOptions1;
    SegmentationOptions segmentationOptions1;
    RecFilterOptions filterOptions2;
    SegmentationOptions segmentationOptions2;
    PostProcessOptions postProcessOptions;
    
    TreeGenerationOptions treeGenerationOptions;
    
    //Modifie Reconstruction
    //list<Mark*> markList;
    
    //Mend the data, add orientation marks
    list<Mark*> orientationMarkList;
    
    //Messageing
    int lastErrorCode;
    ostringstream message;
    int (*callBackFunction) (int percentage);
    
    
};


#endif











