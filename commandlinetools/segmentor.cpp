/*commanlinetool for segmentation */
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
    DataCube *result;
    Reconstruction reconstruction(callBack);
    LoadingOptions loadingOptions;
    PreProcessOptions preProcessOptions;
    TransformOptions transformOptions;
    RecFilterOptions recFilterOptions;
    SegmentationOptions segmentationOptions;
    loadingOptions.filetype=NIH;
    preProcessOptions.skip=YES;
    transformOptions.skip=YES;
    recFilterOptions.skip=YES;
    segmentationOptions.skip=NO;
    command_line_parser parser(argc, argv);
    if (argc<2)
    {
        cout << "Usage: segmentor[Options] filename.nih  ,  use -h for more help" << endl;
        return 0;
    };

    for (int i=0;i<parser.get_n_options();i++)
    {
        option o=parser.get_option(i);
        if (strcmp(o.get_option(),"h")==0 || parser.get_n_filenames()==0)
        {
            cout << "Usage: segmentor [Options] filename.nih " << endl;
            cout << "Options(optional)" << endl;
            cout << "-------" << endl;
            cout << "-lo:CFD , load cfd in stead of nih" << endl;
            cout << "-st:(kind) , kind= GT(GlobalThresholding) , LMV (LocalMeanValueAndNeigbours), FM (FastMarching), OTSU (Otsu Method)" << endl;
            cout << "-eps:value   eps higher than neigbours" << endl;
            cout << "-absl:value  Set absolute low threshold to value(0..1)" << endl;
            cout << "-absh:value  Set absolute high threshold to value(0..1)" << endl;
            cout << endl;
            cout << "Output" << endl;
            cout << "The output will be written to the files:" << endl;
            cout << "segmentationresult.nih : Saved DataCube for later processing"<< endl;
            cout << "datatoshow.dat    : Data as unsigned char x,y,z "<< endl;
            cout << "datatoshow.txt    : Information about size and spaceing"<< endl;
            cout << "For debugging purpose you will also find a file" << endl;
            cout << "loadedcube.dat    : native data see above" << endl;
            return 0;
        };
        if (strcmp(o.get_option(),"lo")==0)
        {
            if(strcmp(o.get_value(),"CFD")==0)
            {
                loadingOptions.filetype=CFD;
                cout << "FileType=CFD" << endl;
            }
        }
        if (strcmp(o.get_option(),"st")==0)
        {
            if (strcmp(o.get_value(),"GT")==0)
            {
                segmentationOptions.segmentationType=GlobalThresholding;
                cout << "Segmentation Type= GlobalThresholding";
            };
            if (strcmp(o.get_value(),"LMV")==0)
            {
                segmentationOptions.segmentationType=LocalMeanValueAndNeigbours;
                cout << "Segmentation Type= LocalMeanValueAndNeigbours";
            };
            if (strcmp(o.get_value(),"FM")==0)
            {
                segmentationOptions.segmentationType=FastMarching;
                cout << "Segmentation Type=FastMarching";
            };
	    if (strcmp(o.get_value(),"OTSU")==0)
            {
                segmentationOptions.segmentationType=Otsu;
                cout << "Segmentation Type=Otsu";
            };


            cout << endl;
        };
        if (strcmp(o.get_option(),"eps")==0)
        {
            float help;
            if (sscanf (o.get_value(),"%f",&(help))==EOF) continue;
            segmentationOptions.epsilon=help;
            cout << "eps=" << segmentationOptions.epsilon << endl;
        };
        if (strcmp(o.get_option(),"absl")==0)
        {
            float help;
            if (sscanf (o.get_value(),"%f",&(help))==EOF) continue;
            segmentationOptions.absoluteLowThreshold=help;
            cout << "absoluteLowThreshold=" << segmentationOptions.absoluteLowThreshold << endl;
        };
        if (strcmp(o.get_option(),"absh")==0)
        {
            float help;
            if (sscanf (o.get_value(),"%f",&(help))==EOF) continue;
            segmentationOptions.absoluteHighThreshold=help;
            cout << "absoluteLowHighhold=" << segmentationOptions.absoluteHighThreshold << endl;
        };

    };
    
sprintf(loadingOptions.FileName,parser.get_filename(0));
printf("Process with file:");
printf(loadingOptions.FileName);
printf("\n");


reconstruction.SetLoadingOptions(loadingOptions);
reconstruction.SetPreProcessOptions(preProcessOptions);
reconstruction.SetTransformOptions(transformOptions);
reconstruction.SetFirstFilterOptions(recFilterOptions);
reconstruction.SetFirstSegmentationOptions(segmentationOptions);
if (reconstruction.Load()){cout << "Error: Load"<< endl;return 1;};
result=reconstruction.GetCube(ORIGINCUBE);
result->writeRAW("loadedcube.dat","loadedcube.txt");
if (reconstruction.PreProcess()) {cout << "Error: TransformData"<< endl;return 1;};
if (reconstruction.TransformData()) {cout << "Error: TransformData"<< endl;return 1;};
if (reconstruction.FirstFilter()) {cout << "Error: FirstFilter"<< endl;return 1;};
cout << "Start Segmentation" << endl;
if (reconstruction.FirstSegmentation()) {cout << "Error: FirstSegmentaion"<< endl;return 1;};
cout << endl << "Finished" << endl;
if (reconstruction.SaveCube(SEGMENTED,"segmentationresult.nih",NIH)) {cout << "Error: Saving" << endl;return 1;};
result=reconstruction.GetCube(SEGMENTED);
result->writeRAW("datatoshow.dat","datatoshow.txt");

return 0;
}


