#include "reconstructionoptions.h"

Options::
Options()
{
    skip=NO;
}

Options::
~Options()
{

}

LoadingOptions::
LoadingOptions() 
{
	filetype=TEST;
	//FileName="";
	
	MultipleFiles=NO;
	StartNr=0;
	EndNr=0;
	channel=1;
	
	extractVolume=NO;
        
   sprintf(HelpMsg, "Press the button Load to open a data stack.\nPossible Fileformats: .cfd .nih .xml");
}

LoadingOptions::
LoadingOptions(LoadingOptions& lo)
{
    skip=lo.skip;
    filetype=lo.filetype;
    sprintf(FileName,lo.FileName);

    MultipleFiles=lo.MultipleFiles;
    StartNr=lo.StartNr;
    EndNr=lo.EndNr;
    channel=lo.channel;

    extractVolume=lo.extractVolume;
    cout << "Called Coppy Constructor" << endl;
    

}


int
LoadingOptions::
operator=(LoadingOptions& lo)
{
    skip=lo.skip;
    filetype=lo.filetype;
    sprintf(FileName,lo.FileName);

    MultipleFiles=lo.MultipleFiles;
    StartNr=lo.StartNr;
    EndNr=lo.EndNr;
    channel=lo.channel;

    extractVolume=lo.extractVolume;
    return 0;

}







TransformOptions::
TransformOptions()
{
    skip=YES;
    ThresholdLow=0.0;
    ThresholdHigh=255.0;
    
    sprintf(HelpMsg, "Hint: for most two photon images a upper threshold of 100 is usefull.");
}

int
TransformOptions::
operator=(TransformOptions& lo)
{
    skip=lo.skip;
    sprintf(lo.HelpMsg,HelpMsg);
    ThresholdLow=lo.ThresholdLow;
    ThresholdHigh=lo.ThresholdHigh;
    return 0;
}



RecFilterOptions::
RecFilterOptions()
{
    skip=YES;
    filterLevel=-1; // =Automaticdetection
    
    sprintf(HelpMsg,"In most cases just press the \"Filer\" button to proceed(the filter needs 1h of time!).\nTo make the filter stonger use more time steps, \nTo detect smaller structures reduce integration size.");
}


int
RecFilterOptions::
operator=(RecFilterOptions &lo)
{
    skip=lo.skip;
    filterLevel=lo.filterLevel;
    
    tau=lo.tau;       
    time_steps=lo.time_steps;   
    epsilon=lo.epsilon;   
    levels=lo.levels;       
   
    fixed_coeffs=lo.fixed_coeffs;
    anicoeff1=lo.anicoeff1;
    anicoeff2=lo.anicoeff2;
    anicoeff3=lo.anicoeff3;
    dependence_type=lo.dependence_type;
    
    integration_size_x=lo.integration_size_x;
    integration_size_y=lo.integration_size_y;
    integration_size_z=lo.integration_size_z;
    gt=lo.gt;
    
    lambda=lo.lambda;
    //kind_of_moments=lo.kind_of_moments;
    //integration_size=lo.integration_size;
    ip_flag=lo.ip_flag;
    //small_boundary_pixels=lo.small_boundary_pixels;

    return 0;
}


SegmentationOptions::
SegmentationOptions()
{
    skip=YES;
    segmentationType=LocalMeanValueAndNeigbours;
    epsilon=0.01;
    absoluteHighThreshold=0.3;
    absoluteLowThreshold=0.02;
    
    sprintf(HelpMsg, "Use \"Local meanvalue and neigbours\" as primary segmentation algorithm \nand \"Global threshsholdig\" as second.");

    
}

int 
SegmentationOptions::
operator=(SegmentationOptions &lo)
{
    skip=lo.skip;
    segmentationType=lo.segmentationType;
    epsilon=lo.epsilon;
    absoluteHighThreshold=lo.absoluteHighThreshold;
    absoluteLowThreshold=lo.absoluteLowThreshold;
    return 0;

}




PreProcessOptions::
PreProcessOptions() : pointSmall(3), pointBig(3), Point11(3), Point12(3),Point21(3),Point22(3)
{
    skip=YES;
    deleteBoundary=YES;
    erasePipet=NO;

    boundarysize=4;
    
    radiusSmall=0.0;
    radiusBig=0.0;
    
    sprintf(HelpMsg, "Set four marks to erase the pipet.\n(to set a mark press the left mouse button and the shift key)");	
}  

int
PreProcessOptions::
operator=(PreProcessOptions& lo)
{
    skip=lo.skip;
    deleteBoundary=lo.deleteBoundary;
    erasePipet=lo.erasePipet;
    pointSmall=lo.pointSmall;
    pointBig=lo.pointBig;
    radiusSmall=lo.radiusSmall;
    radiusBig=lo.radiusBig;
    boundarysize=lo.boundarysize;
    Point11=lo.Point11;
	 Point12=lo.Point12 ;
	 Point21=lo.Point21;
	 Point22=lo.Point22;
    return 0;
    
}



PostProcessOptions::
PostProcessOptions()
{

    sprintf(HelpMsg, "Postprocess help message");

}

TreeGenerationOptions::
TreeGenerationOptions() : body(3)
{
    writeDX=YES;
    createReconstr=YES; //Set to NO till cellbody is set!
    body[1]=64;//Body of TestCube TUBEY3D
    body[2]=40;
    body[3]=55;
    sprintf(dxFileName,"DXTestOutput");
    sprintf(hocFileName,"TestHocfile.hoc");
    writeHocFile=YES;
	
	minLengthOfDendriteInqm=5;
	minDistanceFromBodyCompartment=30.0;

   //not used anymore
	 maxlevel=5; //Automatic detection!
    thresholdlevel=1;
    threshold=0.05;
    
    sprintf(HelpMsg, "Set a mark at the cellbody by pressing the left mouse button and the shift key.\n To remove the mark press the mouse button and the Control key.");


}

int
TreeGenerationOptions::
operator=(TreeGenerationOptions &lo)
{
    skip=lo.skip;
    writeDX=lo.writeDX;
    createReconstr=lo.createReconstr;
    body=lo.body;
    sprintf(dxFileName,lo.dxFileName);
    sprintf(hocFileName,lo.hocFileName);
    writeHocFile=lo.writeHocFile;
    minLengthOfDendriteInqm=lo.minLengthOfDendriteInqm;
    minDistanceFromBodyCompartment=lo.minDistanceFromBodyCompartment;
        
    maxlevel=lo.maxlevel;
    thresholdlevel=lo.thresholdlevel;
    threshold=lo.threshold;




    return 0;
}



    





