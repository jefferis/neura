/*testprogram for reconstruction */
#include <fstream.h>
#include <stdio.h>

#include "../datalib/vector.h"
#include "../datalib/volume.h"
#include "../mathlib/statistic.h"
#include "../mathlib/transform.h"



#include "../errorcode.h"

#include "../datalib/datacube.h"
#include "../datalib/cfddatacube.h"

#include "../datalib/testcubes.h"

#include "../reconstructionlib/primitive.h"
#include "../reconstructionlib/skeletonization.h"

#include "../reconstructionlib/compartmentlist.h"
#include "../reconstructionlib/multicompartmentlist.h"

#include "../mathlib/nld.h"  
 


DataCube* CreateCube(char *name, int test, Vector<int> &body)
{

    DataCube* dC;
    TestCube3D *testCube;
    testCube=new  TestCube3D(TUBEY3DX);
    cout << "Now create cube."<< endl;

    CfdDataCube *cfdTestCube;
    cfdTestCube= new CfdDataCube();
    if (test==1)
    {
        dC=(DataCube*) testCube;
        body[1]=64;
        body[2]=40;
        body[3]=55;//25
    }
    else
    {
        if (cfdTestCube->LoadCfdData(name,0,0,2)) return NULL;
        Vector<double> smallP(3);
        Vector<double> bigP(3);

        smallP[1]=133;
        smallP[2]=127;
        smallP[3]=47;
        double smallr=7;
        bigP[1]=206;
        bigP[2]=250;
        bigP[3]=88;
        double bigr=13;
        cout << "Extract cone:" << endl;
        cout << "Small:" << smallr << endl << smallP;
        cout << "Big:" << bigr << endl << bigP;
       
        //cfdTestCube->deleteCone(smallP,smallr,bigP,bigr);
        
        dC= (DataCube*) cfdTestCube;
        Statistic stat(dC);
        Transform trans(dC);
        trans.ThresholdDataLow(10, 0);
        trans.ThresholdDataHigh(60, 100);
        trans.ScaleData(1/100.0);
        cout << ".done" << endl;
        Moments momentsDetectBody(dC);
        Vector<double> focalPoint(3);
        momentsDetectBody.focal_point3d(focalPoint, *((Volume*) (dC)));
        cout << "Cellbody detection:" << endl <<focalPoint;
        body[1]=125;
        body[2]=125;
        body[3]=30;
        
        
        
	   body[1]=128;
  		body[2]=112;
		body[3]=38; 
    }

    cout << " done." << endl;

    return dC;
}





int Reconstruct(DataCube *dC,Vector<int> body, int maxlevel)
{

    MultiCompartmentList testMulti(dC);
    char buffer[1024];
    int i=0;
    for (i=0;i<maxlevel;i++)
    {
        cout << "Crate Next Level" << endl;
        testMulti.createNextLevel();// this function also: EvaluateAndSetNeigboursForList();
            testMulti.ReduceFinestList(0.4);
            if (i==2)
            {
                dC->quiteSimpleOutput("tmp/beforeFilter.dat","infobefore.txt");
                testMulti.filter();
                dC->quiteSimpleOutput("tmp/afterFilter.dat","infoafter.txt");
            }
            cout << "Create Output" << endl;
            sprintf(buffer,"tmp/DXtestfileLevel%d.dx",i);
            testMulti.writeDX(0,buffer);

    }

    cout << "Post Process" << endl;
    testMulti.SetBody(body);
    /*evaluate neigbour hood*/
    testMulti.PostProcess();
    

    cout << "Marching Front" << endl;
    testMulti.MarchingFront();

    cout << "Delete non connected!" << endl;
    /*They are comps deleted wich should not be deleted due to topological reasons!*/
    testMulti.DeleteNonConnectedFromFinestList();
    

    //testMulti.SearchLokalMaximaOfDistance();
    testMulti.SearchLokalMaximaOfDistance();
    
    testMulti.writeDX(0,"tmp/beforeSkeletonisation.dx");

    cout << "Skeletonization" << endl;
    testMulti.ExtractSkeleton();

    cout << "Detect crosssings and write neuron data set." << endl;
    testMulti.SetNeigbour27();
    testMulti.WalkFromButtonsToBody();

    cout << "Create Output" << endl;
    testMulti.writeDX(0,"tmp/afterSkeletonisation.dx");

    //testMulti.writeTestOutPut();
    cout << "Finished Segmentation and Reconstruction!" << endl;

	//Write Data to Stream!
//This has to be changed!!!
cout << "Write Data to file tmp/neurondata.hoc." << endl;
ofstream neuronData("tmp/neurondata.hoc");
neuronData <<  *(testMulti.GetCellBody());


    return 0;
}


     
     
    
int main(int argc, char **argv) 
{
  char buffer[20];
  char *temp;
  temp="1028S045.cfd";
  if (argc>1) {sprintf(buffer,argv[1]);}
  	else {sprintf(buffer,temp);};
  cout << "Now process with file:" << buffer << endl;
 
  ofstream logfile("tmp/logfile.txt");
  DataCube *dC;
  Vector<int> body(3);
  int  maxlevel;
  
  dC=CreateCube(buffer,1,body);
  
  cout << body;
  if (dC==NULL) {cerr<<"Error during CreateCube" << endl;return 1;};
  
  
  dC->quiteSimpleOutput("tmp/beforeFilter.dat","tmp/infobefore.txt");
  
   
  if (Reconstruct(dC,body,6) ){cerr<<"Error during reconstruction"<< endl;return 1;};
  
  return 0;
} 
  

 




