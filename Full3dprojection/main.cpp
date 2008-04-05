#include <iostream>
using namespace std;

#include "../mathlib/transform.h"
#include "../reconstructionlib/nervecell.h"
#include "cellloader.h"


int CubeAndCube(DataCube *target, DataCube *source1, double threshold1, DataCube *source2, double threshold2)
{
    int nrOfElements=target->NrOfElements();
    
    for (int i=0;i<nrOfElements;i++)
    {
	if (source1->GetData(i)<threshold1) {continue;};
	if (source2->GetData(i)<threshold2) {continue;};
	
	target->SetData(i,1.0);
    }
    
    
    return 0;
}


int detectSynapses(DataCube *source1, double threshold1, DataCube *source2, double threshold2, list< Vector<double> > &synapses)
{
    Vector<double> spaceing(3);
    spaceing=source1->GetSpaceing();

    for (int x=0;x< source1->GetSize()[1];x++)
        for (int y=0;y< source1->GetSize()[2];y++)
	    for (int z=0;z <source1->GetSize()[3];z++)
	    {
		if (source1->GetData(x,y,z)<threshold1) {continue;};
		if (source2->GetData(x,y,z)<threshold2) {continue;};
		Vector<double> synapseP(4);
		synapseP[1]=x*spaceing[1];
		synapseP[2]=x*spaceing[2];
		synapseP[3]=x*spaceing[3];
		synapseP[4]=1.0;;
		synapses.push_back(synapseP);
	    }

    return 0;
}



int MaximumOfCubes(DataCube *target, DataCube *source1, DataCube *source2)
{
    int nrOfElements=target->NrOfElements();
    
    for (int i=0;i<nrOfElements;i++)
    {
	if (source1->GetData(i)>source2->GetData(i)) target->SetData(i,source1->GetData(i));
	target->SetData(i,source2->GetData(i));
    }
    



    return 0;
}



int main2 (int argc, const char * argv[]) 
{
    CellLoader loader[10];
    
    
    
    //loader1.LoadHocFile("/Volumes/Data/cfd/Full3dprojection/example.hoc");
    //loader[1].LoadHocFile("/Volumes/Data/hocToDX/jack.hoc");
    char *base="/Users/philip/NeuraData/automatischeRekonstruktionen/%s";
    char buffer[120];
   
    sprintf(buffer,base,"cubeS001P.HOC");
   loader[1].LoadHocFile(buffer);
   
   sprintf(buffer,base,"cubeS015P.HOC");
   loader[2].LoadHocFile(buffer);
   
   sprintf(buffer,base,"cubeS040P.HOC");
   loader[3].LoadHocFile(buffer);
   
   sprintf(buffer,base,"cubeS045P.HOC");
   loader[4].LoadHocFile(buffer);
   
   sprintf(buffer,base,"cubeS050P.HOC");
   loader[5].LoadHocFile(buffer);
   int nrofcubes=5;
    
   // cout << "Maximal distance from cellbody:" << loader2.MaximumDistanceFromCellbody() << endl;
    
    DataCube cube(3);
    Vector<int> size(3);
    Vector<double> spaceing(3);
    size[1]=256;
    size[2]=256;
    size[3]=200;
    spaceing[1]=0.98;
    spaceing[2]=0.98;
    spaceing[3]=2.0;
    cube.SetCubeSizeAndSpaceing(size,spaceing);
    cube.MallocCube();
    
    DataCube helpCube(cube);
    
    Vector<double> alignment(3);
    alignment[1]=125.0;
    alignment[2]=125.0;
    alignment[3]=100.0;
    
    
    for (int i=1;i<=nrofcubes; i++) 
    {
	
	cout << "Plot Loader " << i << " to cube." << endl;
	loader[i].PlotIntoDataCube(&helpCube,alignment);
	MaximumOfCubes(&cube, &cube, &helpCube);
	
    }
    Transform trans(&cube);
    trans.ScaleData((double)1.0/nrofcubes);
    
    
    cout << "write to tiff ";
    cube.WriteToTIFF("/Users/philip/NeuraData/cfd/Full3dprojection/cube.tiff");
    cout << "done" << endl;
    cout << "write for data explorer";
    cube.writeDataForDX("/Users/philip/NeuraData/cfd/Full3dprojection/cube.dx");
    cout << " done." << endl;
    
    return 0;
}




int main (int argc, const char * argv[]) 
{
    CellLoader loader[10];
    
    
    
    
    char *base="/Users/philip/NeuraData/cfd/Full3dprojection/Alexa/%s";
    char buffer[120];
    int res;
    
    sprintf(buffer,base,"cell1.hoc");
    res=loader[1].LoadHocFile(buffer);
    if (res) {cout << "error durring read hoc"<< endl;return 1;}
    sprintf(buffer,base,"cell2.hoc");
    res=loader[2].LoadHocFile(buffer);
    if (res) {cout << "error durring read hoc"<< endl;return 1;}
       
    DataCube axonCube(3);
    sprintf(buffer,base,"Alexa6_20_8Ch00.tif");
    cout << "Read File:" << buffer << endl;
    res=axonCube.ReadFromTIFF(buffer);
    if (res!=0 && res!=NO_SPACEINGS_TAGS) 
    {
	cout << "Error durring read of tiff file!" << endl;
	return 1;
    };
    cout << "done." << endl;
    
    Vector<double> d(3);
    d[1]=0.725;
    d[2]=0.725;
    d[3]=0.957;
    
    axonCube.SetSpaceing(d);
    
    //DataCube helpCube(axonCube);
    //DataCube cube(axonCube);
    /*
    for (int i=1;i<=2; i++) 
    {
	
	cout << "Plot Loader " << i << " to cube." << endl;
	loader[i].PlotIntoDataCube(&helpCube);
	cout << "done with loader" << i << endl;
	
    }
    
    cout << "Detect Synapses using cubes" << endl;
    CubeAndCube(&cube, &axonCube, 0.2, &helpCube, 0.2);
    */
    cout << "Detect synapses using nervecell" << endl;
    list< Vector<double> > synapses;
    
   // loader[1].detectSynapses(&axonCube, 0.2, synapses);
   loader[2].detectSynapses(&axonCube, 0.2, synapses);

    cout << "finished synapse detection." << endl;
    
    
    // Write Synapse File
    int nrOfData=synapses.size();
    ofstream Synapse("/Users/philip/NeuraData/cfd/Full3dprojection/Synapses2.dx");
    if (!Synapse) 
    { 
	cout << "Can not write dx file!" << endl;
	return CANTWRITEHOCFILE;
    }
    Synapse << "object 1 class array type float rank 1 shape 3 items " << nrOfData << " data follows" << endl;
    
    list< Vector<double> >::iterator pos;
    for (pos=synapses.begin();pos!=synapses.end();++pos)
    {
	Synapse << (*pos)[1] << " " << (*pos)[2] << " " << (*pos)[3] << endl;
    }
    
    Synapse << "attribute \"dep\" string \"positions\" " << endl;
    Synapse << "object 2 class array type float rank 0 items "<< nrOfData << " data follows" << endl;
    for (pos=synapses.begin();pos!=synapses.end();++pos)
    {
	Synapse << (*pos)[4] << endl;
    }
    Synapse << "attribute \"dep\" string \"positions\" " << endl;
    Synapse << "object \"molecule\" class field " << endl;
    Synapse << "component \"positions\" value 1 " << endl;
    Synapse << "component \"data\" value 2 " << endl;
    Synapse << "attribute \"name\" string \"molecule\" " << endl;
    Synapse << "end" << endl;
    
	
    
    
    
    

    
    
    
    
   
	
	
    
    //
    cout << "done."<< endl;
    
    cout << "write to tiff ";
    //cube.WriteToTIFF("/Users/philip/NeuraData/cfd/Full3dprojection/AxonAndDendrite.tiff");
    cout << "done" << endl;
    cout << "write for data explorer";
    //cube.writeDataForDX("/Users/philip/NeuraData/cfd/Full3dprojection/AxonAndDendrite.dx");
    cout << " done." << endl;
    
    return 0;
}





