
#include "reconstruction.h"

static int
EmptyCallBackFunction(int percentage)
{
    return 0;
}


Reconstruction::
Reconstruction()
{
    contrast=50;
    brightness=50;
    presentViewAllocated=true;
    presentView= new ViewCube(256,256,10);
    callBackFunction=EmptyCallBackFunction;
    state=0;
};


Reconstruction::
Reconstruction(int (*callBack) (int percentage))
{
    contrast=0;
    brightness=0;
    presentViewAllocated=true;
    presentView= new ViewCube(256,256,1);
    callBackFunction=callBack;
    state=0;
};





Reconstruction::
~Reconstruction()
{
    cout << "deleting reconstruction, Free all data" << endl;
    CleanUp(0);
    cout << "done." << endl;
};



int
Reconstruction::
Load()
{
    
    CleanUp(0);
    //   int i;
    cout << "This is neura! A tool for automatic reconstruction of neuronal morphologies. Neura is developed by the University of Heidelberg (Ifi) and" << endl;
    cout << "The Max Planck Institut for medical reserach. For more information contact: philip.broser@mpimf-heidelberg.mpg.de" << endl;
    
    cout << "-----------------" << endl;
    cout << "Called Loadmodule" << endl;
    cout << "FileName:" << loadingOptions.FileName << endl;
    cout << "Filetype: " << loadingOptions.filetype << endl;
    
    if (loadingOptions.filetype==CFD)
    {
        cout << "Load CFD Data" << endl;
        CfdDataCube *loadingCube=new CfdDataCube();
	
        if (loadingOptions.extractVolume==NO)
        {
            lastErrorCode=loadingCube-> LoadCfdData(loadingOptions.FileName,loadingOptions.StartNr,loadingOptions.EndNr,loadingOptions.channel);
            if (lastErrorCode!=0)
            {
                message << "Reconstruction::Load:Error, while loading CFD" << endl;
                cout << "-----------------" << endl;
                return 1;
            };
	    
        }
        else
        {
            lastErrorCode=loadingCube->LoadCfdData(loadingOptions.FileName,loadingOptions.StartNr,loadingOptions.EndNr,loadingOptions.start,loadingOptions.size,loadingOptions.channel);
            if (lastErrorCode!=0)
            {
                message << "Reconstruction::Load:Error, while loading CFD" << endl;
                cout << "-----------------" << endl;
                return 1;
            };
	    
        }
        origin=loadingCube;
        originAllocated=true;
        state=1;
	Transform trans(origin);
	trans.AutoScaleData();
        return 0;
    }
    
    if (loadingOptions.filetype==XML)
    {
        cout << "Load XML Data" << endl;
        origin=new DataCube(3);
        lastErrorCode=origin->ReadFromXML(loadingOptions.FileName);
        if (lastErrorCode!=0)
        {
	    
            message << "Reconstruction::Load:Error, while loading XML" << endl;
            cout << "-----------------" << endl;
            return 1;
        };
	
	Transform trans(origin);
	trans.AutoScaleData();
        originAllocated=true;
        state=1;
        return 0;
    }
    
    if (loadingOptions.filetype==NIH)
    {
        cout << "Load NIH Data" << endl;
        origin=new DataCube(3);
        lastErrorCode=origin->ReadFromNIH(loadingOptions.FileName);
        if (lastErrorCode!=0)
        {
            message << "Reconstruction::Load:Error, while loading NIH" << endl;
            cout << "-----------------" << endl;
            return 1;
        };
	Transform trans(origin);
	trans.AutoScaleData();
        originAllocated=true;
        state=1;
        return 0;
    }
    
    if (loadingOptions.filetype==TIFF)
    {
        cout << "Load TIFF Data" << endl;
        origin=new DataCube(3);
        lastErrorCode=origin->ReadFromTIFF(loadingOptions.FileName);
        if (lastErrorCode!=0 && lastErrorCode!=NO_SPACEINGS_TAGS)
        {
            message << "Reconstruction::Load:Error, while loading TIFF" << endl;
            cout << "-----------------" << endl;
            return 1;
        };
	cout << "AutoScale data" << endl;
	Transform trans(origin);
	trans.AutoScaleData();
        originAllocated=true;
        state=1;
	cout << "Ready For reconstruction" << endl;
        return lastErrorCode; //either 0 or NO_SPACEINGS_TAGS
    }
    
    
    
    
    if (loadingOptions.filetype==TEST)
    {
        cout << "Created Testcube" << endl;
        message << "Created TestCube" << endl;
        //TestCube3D *testCube=new TestCube3D(TUBEY3DXANDNOTCONNECTED);
        //TestCube3D *testCube=new TestCube3D(TUBEY3DX);
        TestCube3D *testCube=new TestCube3D(TUBEYBIG); 
        //TestCube3D *testCube=new TestCube3D(TUBEY3DXSMALL);
        origin=testCube;
        originAllocated=true;
        state=1;
        cout << "-----------------" << endl;
        return 0;
    }
    
    cout << "Reconstruction::Load:ERROR, FileType not set" << endl;
    cout << "-----------------" << endl;
    state=0;
    return 1;
};


/*! if there had been no spaceing tags set them
*/
int 
Reconstruction::
SetSpaceings(Vector<double> &spaceings)
{
    if (originAllocated)
    {
	origin->SetSpaceing(spaceings);
	return 0;
    };
	
    return 1;
}

int
Reconstruction::
TransformData()
{

    if (state<1) return OPPERATION_NOT_PERMITTED;
    CleanUp(1);
    if (transformOptions.skip!=YES)
    {
        transformed= new DataCube(*origin);
        (*transformed)=(*origin);
        transfomedAllocated=true;
        Transform trans(transformed);
        trans.ThresholdDataLow(transformOptions.ThresholdLow/255.0, 0.0);
        trans.ThresholdDataHigh(transformOptions.ThresholdHigh/255.0, 1.0);
        trans.AutoScaleData();
    }
    else
    {
        transformed= origin;
        transfomedAllocated=false;
	
    };
    state=2;
    return 0;
};




int
Reconstruction::
FirstFilter()
{
    if (state<2) return OPPERATION_NOT_PERMITTED;
    CleanUp(2);
    
    
    if (filterOptions1.skip!=YES)
    {
      cout << "Start to filter" << endl << flush;
	filtered= new DataCube(*transformed);
        (*filtered)=(*transformed);
        filteredAllocated=true;
	

	cout << "1" << endl << flush;
        //Filter
        MultiCompartmentList *multiCompList= new MultiCompartmentList(filtered,callBackFunction);
	

	cout << "2" << endl << flush;
	//Create hierachy
	int i=0, maxsize=200;
	for (i=0;(i<10 && !(maxsize<100) );i++)
	    maxsize=multiCompList->createNextLevelSmart();
    
	//Run filter
	cout << "3" << endl << flush;
        lastErrorCode=multiCompList->filter(filterOptions1);
        if (lastErrorCode) return lastErrorCode;
	
	cout << "3" << endl << flush;
        delete multiCompList;
        //finish
    }
    else
    {
        filtered=transformed;
        filteredAllocated=false;
    }
    state=3;
    return 0;
};



int
Reconstruction::
FirstSegmentation()
{
    if (state<3) return OPPERATION_NOT_PERMITTED;
    CleanUp(3);
    
    if (segmentationOptions1.skip!=YES)
    {
        segmented= new DataCube(*filtered);
        (*segmented)=(*filtered);
        segmentedAllocated=true;
	
	
        //Execute Segmentation
        Segmentation segmentation(filtered,segmented,callBackFunction);
        switch (segmentationOptions1.segmentationType)
        {
            case GlobalThresholding:
                cout << "Start Global Thresholding" << endl;
                segmentation. SegmentateByGlobalThresholding(segmentationOptions1. absoluteHighThreshold);
                cout << "done." << endl;
                break;
            case LocalMeanValueAndNeigbours:
                cout << "Start structural thresholding" << endl;
                cout << "AbsLow=" << segmentationOptions1.absoluteLowThreshold << " absHigh=" << segmentationOptions1.absoluteHighThreshold << " eps=" << segmentationOptions1.epsilon << endl;
                segmentation.SegmentateByStructuralThresholding(segmentationOptions1.absoluteLowThreshold, segmentationOptions1.absoluteHighThreshold, segmentationOptions1.epsilon);
                cout << "done." << endl;
                break;
            case Otsu:
                cout << "Start segmentation by Otsumethod" << endl;
                segmentation.SegmentateByOtsuMethod();
                cout << "done." << endl;
                break;
            case FastMarching:
                cout << "Fast Marching not implemented yet" << endl;
                return 1;
                break;
		
		
        }
    }
    else
    {
	
        segmented=filtered;
        segmentedAllocated=false;
	
	
    }
    //finish.
    
    
    state=4;
    return 0;
};


int
Reconstruction::
SecondFilter()
{
    
    if (state<4) return OPPERATION_NOT_PERMITTED;
    CleanUp(4);
    
    if (filterOptions2.skip!=YES)
    {
        filteredsegmented = new DataCube(*segmented);
        (*filteredsegmented) = (*segmented);
        filteredsegmentedAllocated=true;
	
	//Filter
        MultiCompartmentList *multiCompList= new MultiCompartmentList(filteredsegmented,callBackFunction);
	
	//create hierachy
	int i=0, maxsize=200;
	for (i=0;(i<10 && !(maxsize<100) );i++)
		maxsize=multiCompList->createNextLevelSmart();
	
	//Run filter
	lastErrorCode=multiCompList->filter(filterOptions2);
	if (lastErrorCode) return lastErrorCode;
	
	delete multiCompList;
	//finish
    }
    else
    {
        filteredsegmented = segmented;
        filteredsegmentedAllocated=false;
	
	
    }
    state=5;
    return 0;
    
    
};


int
Reconstruction::
SecondSegmentation()
{
    if (state<5) return OPPERATION_NOT_PERMITTED;
    CleanUp(5);
    
    if (segmentationOptions2.skip!=YES)
    {
	
        final = new DataCube(*filteredsegmented);
        (*final)=(*filteredsegmented);
        finalAllocated=true;
	
        //Execute Segmentation
        Transform segmentation(filteredsegmented,callBackFunction);
        switch (segmentationOptions2.segmentationType)
        {
            case GlobalThresholding:
                segmentation.GlobalThresholding(segmentationOptions2. absoluteHighThreshold, final);
                break;
            case LocalMeanValueAndNeigbours:
                segmentation.StructuralThresholding(segmentationOptions2.absoluteLowThreshold, segmentationOptions1.absoluteHighThreshold, segmentationOptions1.epsilon,final);
                break;
		
            case Otsu :
            case FastMarching :
                cout << "Not implemented yet" << endl;
                return 1;
                break;
        }
        //finish.
    }
    else
    {
        final=filteredsegmented;
        finalAllocated=false;
	
	
	
    }
    state=6;
    return 0;
};

//Start of the reconstruction algorithm




/*! In this routine the data can manualy be modified to 
*
*/
int
Reconstruction::
PreProcess()
{
    if (state<6)
    {
        cout << "Reconstruction::PreProcess:Error" << endl;
        return OPPERATION_NOT_PERMITTED;
    };
    CleanUp(6);
    cout << "Start Preprocess" << endl;
    
    if (preProcessOptions.skip!=YES)
    {
        preprocessed=new DataCube(*final);
        (*preprocessed)=(*final);
        preprocessedAllocated=true;
	
        Transform trans(preprocessed);
        if (preProcessOptions.erasePipet==YES)
        {
            cout << "delete Pipett" << endl << flush;
            trans.deleteCone(preProcessOptions.pointSmall,preProcessOptions.radiusSmall,preProcessOptions.pointBig,preProcessOptions.radiusBig);
            cout << "finished" << endl;
        };
	
        if (preProcessOptions.deleteBoundary==YES)
        {
            cout << "delete boundary" << endl << flush;
            trans.deleteBoudary(preProcessOptions.boundarysize);
            cout << "done" << endl;
        };
	
    }
    else
    {
        preprocessed=final;
        preprocessedAllocated=false;
	
    };
    
    
    state=7;
    return 0;
    
};

int
Reconstruction::
TreeGenerationDetectTerminations()
{
    int errorcode=0;
    if (state<7) return OPPERATION_NOT_PERMITTED;
    CleanUp(7);
    compartmentsPlot=new DataCube (*preprocessed);
    compartmentsPlotAllocated=true;
    
    if (treeGenerationOptions.skip!=YES)
    {
	
        /* Reconstruction first Part */
        multiCompartmentList=new MultiCompartmentList(preprocessed,callBackFunction);
        int maxlevel;
        
        cout << "Reconstruction::TreeGenerationDetectTerminations::One level approach, one voxel = one compartment" << endl;
        multiCompartmentList->createFinestCompartmentListFromScratch();
        cout << (* multiCompartmentList) << endl;
        cout << "Set Cellbody" << endl;
        errorcode=multiCompartmentList->SetBodyDirectFromFinestList(treeGenerationOptions.body);
        if (errorcode) return errorcode;
	
	
        //*evaluate neigbour hood
        cout << "econstruction::TreeGenerationDetectTerminations::Mark boundary compartments" << endl << flush;
        multiCompartmentList->MarkBoundaryCompartments();
	
	cout << "reconstruction::TreeGenerationDetectTerminations::SetNeigbours27()" << endl;
        multiCompartmentList->SetNeigbour27(); 
	
        cout << "reconstruction::TreeGenerationDetectTerminations::Calculate and set distances from cellbody using champfer function" << endl << flush;
        multiCompartmentList->CalcualteAndSetDistancesFromBodyCompartmentUsingChamferFunction(); 
	
		////////////////////// Shell Model ////////////////////////////
		cout << "reconstruction::TreeGenerationDetectTerminations::Calculate and set distances from cellbody and indice list with the shell model" << endl << flush;
        multiCompartmentList->CalcualteAndSetDistancesFromBodyCompartment_ShellModel();  

		// detects cellbody  
		cout<< "reconstruction::Detection of the cellbody"<<endl; 
		cellbodyList= new list<Vector<double> >;
		multiCompartmentList->DetectCellbody(cellbodyList, 50.); 
		list<Vector<double> >::iterator pos;
		cout<<"the following compartments were identified as part of the cellbody: "<<endl;
		if((*cellbodyList).empty())
		{
		cout<<"CellbodyListe leer!"<<endl;
		}
		else
		{
			for(pos=(*cellbodyList).begin(); pos!=(*cellbodyList).end(); pos++)
				{
					for(int i=1; i<=4; i++)
					 	cout<<i<<":  "<<(*pos)[i]<<"  ";
					cout<<endl;
				}
		}
   		///////////////////////////////////////////////////////////////
	
        
        cout << "reconstruction::TreeGenerationDetectTerminations::Search for dendrite terminations" << endl << flush;
        multiCompartmentList->SearchLokalMaximaOfDistance27AndCreateButtonCompList();
        
	
        cout << "reconstruction::TreeGenerationDetectTerminations::Check if dendrite terminations are meaningfull" << endl << flush;
		multiCompartmentList->CheckIfButtonsInButtonCompListAreMeaningfullAndDeleteOther(treeGenerationOptions.minDistanceFromBodyCompartment,treeGenerationOptions.minLengthOfDendriteInqm);
        //usually dendrite terminations are 10qm from soma
        //usually they are longer than 3qm
	
	
	
        if (treeGenerationOptions.writeDX==YES)
        {
	    char info[250];
	    sprintf(info,"%s.beforeSkeletonisation.dx",treeGenerationOptions.dxFileName);
            cout << "write DX before skeletonization" << endl << flush;
            multiCompartmentList->writeDX(0,info);
        }
        
        multiCompartmentList->writeToDataCube(compartmentsPlot);
		
    };
    
    
    state=8;
    return 0;
};


/*!
* These function are used to set and delete dendrite terminations
*
*/
list<Mark*>*
Reconstruction::
GetMarkList()
{
    if (state<8)
    {
        lastErrorCode=OPPERATION_NOT_PERMITTED;
        return NULL;
    }
    list<Mark*> *markList= new list<Mark*>;
    multiCompartmentList->createMarkList(*markList);
    return markList;
}

int
Reconstruction::
AddMark(Mark &newMark)
{
    return multiCompartmentList->AddMark(newMark);
    
}

int
Reconstruction::
DeleteMark(int id)
{
    return multiCompartmentList->DeleteMark(id);
}




/*!
 * Uses detected terminations to extarct the skeleton of the neuron 
 * then creates a tree representation with cell, dendrite and termination elements
 *
 */
int
Reconstruction::
TreeGenerationCreateNervecell()
{
    cout << "Start to create nervecell" << endl;
    if (state<8) return OPPERATION_NOT_PERMITTED;
    CleanUp(8);
    reconstructionPlot= new DataCube(*compartmentsPlot);
    reconstructionPlotAllocated=true;
    nervecellPlot= new DataCube(*compartmentsPlot);
    nervecellPlotAllocated=true;
    
    if (treeGenerationOptions.skip!=YES)
    {
        cout << "Reconstruction::TreeGenerationCreateNervecell::Skeletonization" <<  endl << flush;
	multiCompartmentList->ExtractSkeleton2(); //test!!!
	
	
        cout << "Reconstruction::TreeGenerationCreateNervecell::Detect crosssings and write neuron data set." << endl;
        if (multiCompartmentList->WalkFromButtonsToBodyAndCreateNeuronStructure())
        {
            cout << "Error durring reconstruction" << endl;
            return 1;
        }
        state=9;
	
	cout << "Reconstruction::TreeGenerationCreateNervecell::MeasureRadii" << endl;
        (AccsessCellBody())->measureRadii(treeGenerationOptions.threshold); 
	
	
	cout << "Reconstruction::TreeGenerationCreateNervecell::Finished Segmentation and Reconstruction!" << endl;
	
	
	
	if (treeGenerationOptions.writeHocFile==YES) 
	{
	    cout << "Reconstruction::TreeGenerationCreateNervecell::write hoc file" << endl;
	    WriteHocFile(treeGenerationOptions.hocFileName);
	};
        
        
        
	
	if (treeGenerationOptions.writeDX==YES)
        {
	    cout << "write dx output" << endl;
	    char buffer2[250];
	    sprintf(buffer2,"%s.afterSkeletonisation.dx",treeGenerationOptions.dxFileName);
            cout << "write DX after skeletonization" << endl << flush;
            multiCompartmentList->writeDX(0,buffer2);
	    
	    sprintf(buffer2,"%s.datafordx.raw",treeGenerationOptions.dxFileName);
	    preprocessed->writeDataForDX(buffer2);
	    
	    sprintf(buffer2,"%s.nervecell.tube.dx",treeGenerationOptions.dxFileName);
	    WriteNeuronAsDXFile(buffer2);
	    
        }
	
	
	
	
        cout << "Plot nervecell to datacube" << endl;
        multiCompartmentList->writeToDataCube(reconstructionPlot);
        writeToDataCube( nervecellPlot , *(AccsessCellBody()) );
	
    };
    
    return 0;
};




/*!
 *These function are used to identify special positions in the data set to mend the data
 *for instance to mark the pia or the barrel direction and orientation
 */
int 
Reconstruction::
AddOrientationMark(Mark &newMark)
{   

    // Possible marktypes 0: Position, 1: Direction, 2: Axon
    // FIXME if (newMark.GetType()!=OrientationType) return 1; 
    newMark.SetIdOfTermination(newMark.GetId());

    //add Mark to own List
    orientationMarkList.push_back(&newMark);
    return 0;
};


int 
Reconstruction::
RemoveOrientationMark(int idOfMark)
{
    //Remove mark from Own List!!!
    list<Mark*>::iterator pos;
    for (pos=orientationMarkList.begin();pos!=orientationMarkList.end();++pos)
        if ((**pos).GetId()==idOfMark)
	{
            orientationMarkList.erase(pos);
            return 0;
	};  
    
    return 1;
};



/*!
 *This function cretates an ouput file which is readable by neuron, see www.neuron.yale.edu
*/
int
Reconstruction::
WriteHocFile(char *filename)
{
    if (state<9) return OPPERATION_NOT_PERMITTED;
    ofstream neuronData(filename);
    if (!neuronData) return CANTWRITEHOCFILE;
    neuronData << "/* This hoc file was created using neura */" << endl;
    neuronData << "/* For information about Neura see http://sun0.mpimf-heidelberg.mpg.de/~pbroser/  .*/" << endl;
    neuronData << "/* This file is based on data file:" <<  loadingOptions.FileName << " */" << endl;
    neuronData << "/* Cube:*/" << endl;
    neuronData << "/* "<< endl << (Volume) *origin << "*/" << endl;
    neuronData << "/* Marks in Data Set: */" << endl;
    neuronData << "/* no Marks set */" << endl;
    neuronData << "/* Reconstruction Options: */" << endl;
    neuronData << "minLengthOfDendriteInqm = " << treeGenerationOptions.minLengthOfDendriteInqm << endl;
    neuronData << "minDistanceFromBodyCompartmen = " << treeGenerationOptions.minDistanceFromBodyCompartment << endl;
    neuronData << "/* Morphology */" << endl;
    neuronData <<  *(AccsessCellBody());
	
	neuronData << "/* Cellbody Geometry */"<< endl;
	list<Vector<double> >::iterator pos;
	if((*cellbodyList).empty())
		{
		cout<<"CellbodyListe leer!"<<endl;
		}
		else
		{
			for(pos=(*cellbodyList).begin(); pos!=(*cellbodyList).end(); pos++)
				neuronData << "/* [x]  "<<(*pos)[1]<<"  [y]  "<<(*pos)[2]<<"  [z]  "<<(*pos)[3]<<"  distance to cellbody-boundary  "<<(*pos)[4]<<"*/"<<endl;
		}
	return 0;
};



int
Reconstruction::
WriteNeuronAsDXFile(char *filename)
{
    cout << "Request to write nervecell to dx file" << endl;
    if (state<9) return OPPERATION_NOT_PERMITTED;
    ofstream neuronDXData(filename);
    if (!neuronDXData) return CANTWRITEHOCFILE;
    cout << "write nervecell to dx file" << endl;
    //read data from  structure
    strstream radiusData;
    strstream positionData; 
    strstream connectionData;
    strstream header;
    int nrofposition=-1;
    int nroflastlink=0;
    createDataForDX(radiusData, positionData, connectionData, nrofposition, nroflastlink, *(AccsessCellBody()) );
    //write to file
    neuronDXData << "object 1 class array type float rank 0 items "<< (nrofposition+1) << " data follows" << endl;
    neuronDXData << radiusData.str() << flush;
    neuronDXData << "attribute \"dep\" string \"positions\" " << endl;
    neuronDXData << "object 2 class array type float rank 1 shape 3 items " << (nrofposition+1) << " data follows" << endl;
    neuronDXData << positionData.str() << flush;
    neuronDXData << "attribute \"dep\" string \"positions\" " << endl;
    neuronDXData << "object 3 class array type int rank 1 shape 2 items " << (nrofposition) << " data follows" << endl; //first compartment has only one connection
    neuronDXData << connectionData.str() << flush;
    neuronDXData << "attribute \"element type\" string \"lines\" " << endl;
    neuronDXData << "attribute \"ref\" string \"positions\" " << endl;
    neuronDXData << "object \"molecule\" class field" << endl;
    neuronDXData << "component \"data\" value 1" << endl;
    neuronDXData << "component \"positions\" value 2" << endl;
    neuronDXData << "component \"connections\" value 3" << endl;
    neuronDXData << "attribute \"name\" string \"molecule\" " << endl; 
    neuronDXData << "end" << endl;
    return 0;
}



/*! Function to create data for gui
*/  
ViewCube* 
Reconstruction::
CreateViewCube(KindOfCubes kind)
{
    cout << "reconstruction::asked to create new view cube:" << endl << flush;
    DataCube *source;
    switch (kind)
    {
	case PREPROCMAXINT5:
	    source=GetCube(PREPROCESSED);
	    if (source==NULL) return presentView;
		if (presentViewAllocated) delete presentView;
		    presentViewAllocated=false;
	    presentView= new ViewCube(source,maxintensity5,contrast,brightness);
	    presentViewAllocated=true;
	    cout << "reconstrcution::created view cube: maxint5, preprocessed" << endl << flush;
	    return presentView;
	    break;    
	    
	case PREPROCMAXINT10:
	    source=GetCube(PREPROCESSED);
	    if (source==NULL) return presentView;
		if (presentViewAllocated) delete presentView;
		    presentViewAllocated=false;
	    presentView= new ViewCube(source,maxintensity10,contrast,brightness);
	    presentViewAllocated=true;
	    cout << "reconstrcution::created view cube: maxint5, preprocessed" << endl << flush;
	    return presentView;
	    break;
	case ORIGINMAXINT5:
	    source=GetCube(ORIGINCUBE);
	    if (source==NULL) return presentView;
		if (presentViewAllocated) delete presentView;
		    presentViewAllocated=false;
	    presentView= new ViewCube(source,maxintensity5,contrast,brightness);
	    presentViewAllocated=true;
	    cout << "reconstrcution::created view cube: maxint5, Origin" << endl << flush;
	    return presentView;
	    break;
	case ORIGINMAXINT10:
	    source=GetCube(ORIGINCUBE);
	    if (source==NULL) return presentView;
		if (presentViewAllocated) delete presentView;
		    presentViewAllocated=false;
	    presentView= new ViewCube(source,maxintensity10,contrast,brightness);
	    presentViewAllocated=true;
	    cout << "reconstrcution::created view cube: maxint5, Origin" << endl << flush;
	    return presentView;
	    
	    break;
	case COMPMAXINT5:
	    source=GetCube(COMPPLOT);
	    if (source==NULL) return presentView;
		if (presentViewAllocated) delete presentView;
		    presentViewAllocated=false;
	    presentView= new ViewCube(source,maxintensity5,contrast,brightness);
	    presentViewAllocated=true;
	    cout << "reconstrcution::created view cube: maxint5, Origin" << endl << flush;
	    return presentView;
	    break;
	case COMPMAXINT10:
	    source=GetCube(COMPPLOT);
	    if (source==NULL) return presentView;
		if (presentViewAllocated) delete presentView;
		    presentViewAllocated=false;
	    presentView= new ViewCube(source,maxintensity10,contrast,brightness);
	    presentViewAllocated=true;
	    cout << "reconstrcution::created view cube: maxint5, Origin" << endl << flush;
	    return presentView;
	    break;
	default:
	// This is for the base cubes ORIGINCUBE, TRANSFORMED, FILTERED, SEGMENTED, FILTEREDSEG, FINAL, PREPROCESSED, COMPPLOT, RECPLOT: 
	    source=GetCube(kind);
	    if (source==NULL) return presentView;
		if (presentViewAllocated) delete presentView;
		    presentViewAllocated=false;
	    presentView= new ViewCube(source,linear,contrast,brightness);
	    presentViewAllocated=true;
	    cout << "reconstrcution::created view cube: linear, base" << endl << flush;
	    return presentView;
	    break;
	    
    };
    
    return presentView;
};

int 
Reconstruction::
SetContrast(int value) //0..100
{
    contrast=value;
    cout << "Set contrast to:" << value << endl;
    return 0;
};


int 
Reconstruction::
SetBrightness(int value) //0..100
{
    brightness=value; 
    cout << "Set brightness to:" << value << endl;
    return 0;
};



/*!
 * Allows acces to the data cube according to kind, this function will soon be private
 *
 */

DataCube*
Reconstruction::
GetCube(KindOfCubes kind)
{
    switch (kind)
    {
        case ORIGINCUBE:
            if (state<1)
            {
                lastErrorCode=OPPERATION_NOT_PERMITTED;
                cerr << "Acess of ORIGINCUBE not permited! State=" << state << endl;
                return NULL;
            };
            return (DataCube*) origin;
            break;
	    
        case TRANSFORMED:
            if (state<2)
            {
                lastErrorCode=OPPERATION_NOT_PERMITTED;
                cerr << "Acess of TRANSFORMED not permited! State=" << state << endl;
                return NULL;
            };
            return transformed;
            break;
        case FILTERED:
            if (state<3)
            {
                lastErrorCode=OPPERATION_NOT_PERMITTED;
                cerr << "Acess of FILTERED not permited! State=" << state << endl;
                return NULL;
            };
            return filtered;
            break;
        case SEGMENTED:
            if (state<4)
            {
                lastErrorCode=OPPERATION_NOT_PERMITTED;
                cerr << "Acess of SEGMENTED not permited! State=" << state << endl;
                return NULL;
            };
            return segmented;
            break;
        case FILTEREDSEG:
            if (state<5)
            {
                lastErrorCode=OPPERATION_NOT_PERMITTED;
                cerr << "Acess of FILTERDSEGMENTED not permited! State=" << state << endl;
                return NULL;
            };
            return filteredsegmented;
            break;
        case FINAL:
            if (state<6)
            {
                lastErrorCode=OPPERATION_NOT_PERMITTED;
                cerr << "Acess of FINAL not permited! State=" << state << endl;
                return NULL;
            };
            return final;
            break;
        case PREPROCESSED:
            if (state<7)
            {
                lastErrorCode=OPPERATION_NOT_PERMITTED;
                cerr << "Acess of PREPROCESSED not permited! State=" << state << endl;
                return NULL;
            };
            return preprocessed;
            break;
        case COMPPLOT:
            if (state<8)
            {
                lastErrorCode=OPPERATION_NOT_PERMITTED;
                cerr << "Acess COMPPLOT not permited! State=" << state << endl;
                return NULL;
            };
            return compartmentsPlot;
            break;
        case RECPLOT:
            if (state<9)
            {
                lastErrorCode=OPPERATION_NOT_PERMITTED;
                cerr << "Acess RECPLOT not permited! State=" << state << endl;
                return NULL;
            };
            //return reconstructionPlot;
            return nervecellPlot;
            break;
    };
    
    return NULL;
};


Cellbody*
Reconstruction::
AccsessCellBody()
{
    if (state<9)
    {
        lastErrorCode=OPPERATION_NOT_PERMITTED;
        return NULL;
    }
    
    return multiCompartmentList->GetCellBody();
};


//Save Data
int
Reconstruction::
SaveCube(KindOfCubes kind,char *name,FILETYPE filetype)
{
    DataCube *CubeToSave=GetCube(kind);
    if (CubeToSave==NULL) {cout << "SAVECUBE : ERROR, ACESS NOT POSSIBLE" << endl;return lastErrorCode;};
    if (filetype==XML) return CubeToSave->WriteToXML(name);
    if (filetype==NIH) {cout << "Save NIH File:" << name << endl;return CubeToSave->WriteToNIH(name);}
    if (filetype==RAW) return CubeToSave->writeRAW(name,"rawinfo.txt");
    if (filetype==VREND) return CubeToSave->writeToVrend(name);
    if (filetype==TIFF) return CubeToSave->WriteToTIFF(name);
    
    return 1;
};



/*!
* Function to clean up memory usage
*/
int
Reconstruction::
CleanUp(int toState)
{
    if (state>=1 && toState<1 && originAllocated) {delete origin;cout << "delete origin" << endl;};
    if (state>=2 && toState<2 && transfomedAllocated) {delete transformed;cout << "delete transformed" << endl;};
    if (state>=3 && toState<3 && filteredAllocated) {delete filtered;cout << "delete filtered" << endl;};
    if (state>=4 && toState<4 && segmentedAllocated) {delete segmented;cout << "delete segmented" << endl;};
    if (state>=5 && toState<5 && filteredsegmentedAllocated) {delete filteredsegmented;cout << "delete filteredsegmented" << endl;};
    if (state>=6 && toState<6 && finalAllocated) {delete final;cout << "delete final" << endl;};
    if (state>=7 && toState<7 && preprocessedAllocated) {delete preprocessed;cout << "delete preprocessed" << endl;};
    
    //Ploted reconstruction
    if (state>=8 && toState<8) {delete multiCompartmentList;cout << "delete multiCompartmentList" << endl;};
    if (state>=8 && toState<8 && compartmentsPlotAllocated) {delete compartmentsPlot;cout << "delete compartmentsPlot" << endl;};
    if (state>=9 && toState<9 && reconstructionPlotAllocated) {delete reconstructionPlot;cout << "delete reconstructionPlot" << endl;};
    if (state>=9 && toState<9 && nervecellPlotAllocated) {delete nervecellPlot;cout << "delete nervecellPlot" << endl;};
    //if (state>=9 && toState<9) delete cellBody; somewere here is a bug!
    
    return 0;
}



ostream &operator<<(ostream &ostr, const Reconstruction &rec)
{
    ostr <<  rec.message;
    return ostr;
    
}










