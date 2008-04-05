/*commanlinetool for segmentation */
/* ./reconstructor -bx:128 -by:112 -bz:38 readyforreconstruction.macosx.nih */

#include <fstream.h>
#include <stdio.h>
#include <time.h>

#include "../datalib/vector.h"
#include "../datalib/volume.h"
#include "../mathlib/statistic.h"
#include "../mathlib/transform.h"
#include "../mathlib/moments.h"


#include "../errorcode.h"

#include "../datalib/datacube.h"
#include "../datalib/cfddatacube.h"
#include "../datalib/testcubes.h"

#include "../reconstructionlib/reconstruction.h"
#include "../mathlib/nld.h"

#include "command_line_parser.h"

using namespace std;
using namespace pramodx;


int callBack(int percentage)
{
    cout << "\r " << percentage << " \% done" << flush;
    return 0;
}


int
main(int argc, char **argv)
{
#ifdef DEBUG
    cout << "DEBUG Version of reconstructor" << endl;
#endif
    int errorcode;
    DataCube *result;
    Reconstruction reconstruction(callBack);
    LoadingOptions loadingOptions;
    PreProcessOptions preProcessOptions;
    TransformOptions transformOptions;
    RecFilterOptions recFilterOptions;
    SegmentationOptions segmentationOptions;
    TreeGenerationOptions treeGenerationOptions;
    loadingOptions.filetype=NIH;
    preProcessOptions.skip=YES;
    transformOptions.skip=YES;
    recFilterOptions.skip=YES;
    segmentationOptions.skip=YES;
    treeGenerationOptions.skip=NO;
    command_line_parser parser(argc, argv);
    if (argc<2)
    {
        cout << "Usage: reconstructor -bx:value -by:value -bz:value [Options] filename.nih  ,  use -h for more help" << endl;
        return 0;
    };
    int cordcounter=0;

    for (int i=0;i<(int) parser.get_n_options();i++)
    {
        option o=parser.get_option(i);
        if (strcmp(o.get_option(),"h")==0 || parser.get_n_filenames()==0)
        {
            cout << "Usage: reconstructor -bx:value -by:value -bz:value [Options] filename.nih " << endl;
            cout << "-bx:value , Position of cellbody x" << endl;
            cout << "-by:value , Position of cellbody y" << endl;
            cout << "-bz:value , Position of cellbody z" << endl;
            cout << "Options(optional)" << endl;
            cout << "-------" << endl;
            cout << "-lo:CFD    , load cfd in stead of nih" << endl;
            cout << "-thr:value , strucure detection threshold" << endl;
            cout << "-lev:value , final reconstruction level, -1 automatic detection(default)" << endl;
            cout << endl;
            cout << "Output" << endl;
            cout << "The output will be written to the files:" << endl;
            cout << "finaltree.nih : Saved final Plot into"<< endl;
            cout << "DXTestOutputNR.dx : Files readable with opendx to controll reconstrucion"<< endl;
            cout << "dxdatatoshow.dat    : Data with z x Coordinates switched to show in opendx"<< endl;
            cout << "dxdatatoshow.txt    : Information about size and spaceing"<< endl;
            cout << "TestHocfile.hoc   : Hoc file of reconstruction" << endl;
            return 0;
        };
        
        if (strcmp(o.get_option(),"bx")==0)
        {
            int help;
            if (sscanf (o.get_value(),"%d",&(help))==EOF) continue;
            treeGenerationOptions.body[1]=help;
            cout << "bx=" << treeGenerationOptions.body[1] << endl;
            cordcounter++;
        };
        if (strcmp(o.get_option(),"by")==0)
        {
            int help;
            if (sscanf (o.get_value(),"%d",&(help))==EOF) continue;
            treeGenerationOptions.body[2]=help;
            cout << "by=" << treeGenerationOptions.body[2] << endl;
            cordcounter++;
        };

        if (strcmp(o.get_option(),"bz")==0)
        {
            int help;
            if (sscanf (o.get_value(),"%d",&(help))==EOF) continue;
            treeGenerationOptions.body[3]=help;
            cout << "bz=" << treeGenerationOptions.body[3] << endl;
            cordcounter++;
        };
        if (strcmp(o.get_option(),"lev")==0)
        {
            int help;
            if (sscanf (o.get_value(),"%d",&(help))==EOF) continue;
            treeGenerationOptions.maxlevel=help;
            cout << "level=" << treeGenerationOptions.maxlevel << endl;
            cordcounter++;
        };

        
        if (strcmp(o.get_option(),"lo")==0)
        {
            if(strcmp(o.get_value(),"CFD")==0)
            {
                loadingOptions.filetype=CFD;
                cout << "FileType=CFD" << endl;
            }
        }
        if (strcmp(o.get_option(),"thr")==0)
        {
            float help;
            if (sscanf (o.get_value(),"%f",&(help))==EOF) continue;
            treeGenerationOptions.threshold=help;
            cout << "threshold=" << treeGenerationOptions.threshold << endl;
        };


        


};
if (cordcounter<3)
{
    cout << "Missinig coordinate for body, quit use -h for help" << endl;
    return 1;

}    
sprintf(loadingOptions.FileName,parser.get_filename(0));
printf("Process with file:");
printf(loadingOptions.FileName);
printf("\n");


reconstruction.SetLoadingOptions(loadingOptions);
reconstruction.SetPreProcessOptions(preProcessOptions);
reconstruction.SetTransformOptions(transformOptions);
reconstruction.SetFirstFilterOptions(recFilterOptions);
reconstruction.SetFirstSegmentationOptions(segmentationOptions);
reconstruction.SetSecondFilterOptions(recFilterOptions);
reconstruction.SetSecondSegmentationOptions(segmentationOptions);
reconstruction.SetTreeGenerationOptions(treeGenerationOptions);

if (reconstruction.Load()){cout << "Error: Load"<< endl;return 1;};
if (reconstruction.TransformData()) {cout << "Error: TransformData"<< endl;return 1;};
if (reconstruction.FirstFilter()) {cout << "Error: FirstFilter"<< endl;return 1;};
if (reconstruction.FirstSegmentation()) {cout << "Error: FirstSegmentaion"<< endl;return 1;};
if (reconstruction.SecondFilter()) {cout << "Error: SecondFilter"<< endl;return 1;};
if (reconstruction.SecondSegmentation()) {cout << "Error: SecondSegmentaion"<< endl;return 1;};
if (reconstruction.PreProcess()) {cout << "Error: PreprocessData"<< endl;return 1;};
cout << "Start reconstruction: " << endl << flush;
errorcode=reconstruction.TreeGenerationDetectTerminations();
if (errorcode)
{ 
    cout << reconstruction;
    cout << "Error durring reconstruction, error message:" << endl;
    cout <<  errorCode(errorcode) << endl;
    cout << "Quit" << endl;
    return 1;
}
errorcode=reconstruction.TreeGenerationCreateNervecell();
if (errorcode)
{
    
    cout << "Error durring reconstruction" << endl;
    cout << "Quit" << endl;
    return 1;
}
if (reconstruction.WriteNeuronAsDXFile("neuronasdx.dx"))
{
	cout << "could not write neuron as dx" << endl;
};


return 0;
}


