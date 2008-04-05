#include "nervecell.h"


//Structure
int Structure::IDCounter=0;

Structure::
Structure()
{
    IDCounter++;
    id=IDCounter;
    kind=0;
    proximal=NULL;
    distal=NULL;
    comp=NULL;
    base=0; // usually Structure is compartemt based
    sprintf(name,"noName");
}


Structure::
~Structure()
{
}


int 
Structure::
SetName(char *lName)
{
    strcpy(name,lName);
    return 0;
}


int
Structure::
SetCompartment(Compartment *lcomp)
{
    comp=lcomp;
    return 0;
}


int
Structure::
SetProximalLink(Link *link)
{
    proximal=link;
    return 0;
}


int
Structure::
SetDistalLink(Link *link)
{
    distal=link;
    return 0;
}

ostream &operator<<(ostream &ostr, const Structure &structure)
{
    Structure *proximalStructure;
  //  Structure *distalStructure;
    Compartment *proxComp;
    Compartment *distComp;
    list<Compartment*>::const_iterator pos;
    list<Compartment*>::const_reverse_iterator rpos;
    list<double>::const_reverse_iterator radiuspos;

    Compartment *comp=structure.comp;
    int nrOfSeg=1;
    if (structure.kind!=CELLBODY)
    {
        if (proximalStructure==NULL) {ostr << "Error no CELLBODY but proximal=0" << endl; return ostr;};
        proximalStructure=(structure.proximal)->GetProximalStructure();
        proxComp=proximalStructure->GetComp();

        for (pos=structure.geometricComps.begin();pos!=structure.geometricComps.end();++pos)
            nrOfSeg++;
    }

    if (structure.kind==DENDRITE)
    {
        //distalStructure=(structure.distal)->GetDistalStructure();
        distComp=(structure.distal)->GetComp();
        comp=distComp; /// this has to be replaced by a set of comps for geometric reconsturion!
    }

    double radius; //Radius of structure element
    double scalex=(comp->GetSpaceing())[1]/1.0;
    double scaley=(comp->GetSpaceing())[2]/1.0;
    double scalez=(comp->GetSpaceing())[3]/1.0;
    Vector<int> start(3);
    Vector<int> size(3);
    Vector<double> pt3d(3);
    double threshold=0.05;



    switch (structure.kind)
    {
        case CELLBODY:
            start=comp->GetStart();
            size=comp->GetSize();
            pt3d[1]=(start[1]+0.5*size[1])*scalex;
            pt3d[2]=(start[2]+0.5*size[2])*scaley;
            pt3d[3]=(start[3]+0.0*size[3])*scalez;
	    radius=comp->CalculateRadius(threshold, 1);

            ostr << "{create Cellbody}\n{access Cellbody}\n{nseg=2}\n{pt3dclear()}\n";
            ostr << "{pt3dadd(" << pt3d[1] << ", " << pt3d[2] << ", " << pt3d[3] << ", " <<radius<<")}\n";
	    
	    //to do!!!!!!
            pt3d[1]=(start[1]+0.5*size[1])*scalex;
            pt3d[2]=(start[2]+0.5*size[2])*scaley;
            pt3d[3]=(start[3]+0.5*size[3])*scalez;
	    radius=comp->CalculateRadius(threshold, 1);

            ostr << "{pt3dadd(" << pt3d[1] << ", " << pt3d[2] << ", " << pt3d[3] << ", " <<radius<< ")}\n"; // this has to be changed

            break;

        case BUTTON:
            ostr << "{create Dendrite" << structure.id << "}\n";
            if (proximalStructure->GetKind()==CELLBODY)
            {
                //ostr << "{Cellbody(1) connect " << "Dendrite" << structure.id << "(0), 1}\n\n";
                ostr << "{connect  Dendrite" << structure.id << "(0), Cellbody(1)}\n\n";
            }
                else
                {
                    //ostr << "{Dendrite" << proximalStructure->GetId() << "(1) connect Dendrite" << structure.id<< "(0), 0}\n";
                    ostr << "{connect Dendrite" << structure.id<< "(0), Dendrite" << proximalStructure->GetId() << "(1)}\n";
                };
            ostr << "{access Dendrite" << structure.id << "}\n" << "{nseg=" << nrOfSeg << "}\n" << "{pt3dclear()}\n";

            //cout << "Size of Geometric Comps is:" << structure.geometricComps.size() << endl;



            radiuspos=structure.radii.rbegin();
	    radiuspos++;                                //comp is the first radius in the list beginning from the back
	   
	    
            for (rpos=structure.geometricComps.rbegin();rpos!=structure.geometricComps.rend();rpos++)
            {

	      start=(*rpos)->start;
	      //start=*pos->GetStart();
	      size=(*rpos)->size;                                    //GetSize();
	      pt3d[1]=(start[1]+0.5*size[1])*scalex;
	      pt3d[2]=(start[2]+0.5*size[2])*scaley;
	      pt3d[3]=(start[3]+0.5*size[3])*scalez;

	      radius=*radiuspos;
	      radiuspos++;

	      ostr << "{pt3dadd(" << pt3d[1] << ", " << pt3d[2] << ", " << pt3d[3] << ", " <<  radius <<")}\n";
            };
            
	    // comp as the most proximal point 
	    start=comp->GetStart();
            size=comp->GetSize();
            pt3d[1]=(start[1]+0.5*size[1])*scalex;
            pt3d[2]=(start[2]+0.5*size[2])*scaley;
            pt3d[3]=(start[3]+0.5*size[3])*scalez;
	    radiuspos=structure.radii.rbegin();
	    radius=*radiuspos;
	    
	    ostr << "{pt3dadd(" << pt3d[1] << ", " << pt3d[2] << ", " << pt3d[3] << ", " <<  radius << ")}\n";
	    ostr << "\n";
	    
            break;


        case DENDRITE:
            ostr << "{create Dendrite" << structure.id << "}\n";
            if (proximalStructure->GetKind()==CELLBODY)
            {
                //ostr << "{Cellbody(1) connect " << "Dendrite" << structure.id << "(0), 1}\n";
                ostr << "{connect Dendrite" << structure.id << "(0), Cellbody(1)}\n";


            }
                else
                {
                    //ostr << "{Dendrite" << proximalStructure->GetId() << "(1) connect Dendrite" << structure.id<< "(0), 1}\n";
                    ostr << "{connect Dendrite"<< structure.id << "(0), Dendrite" << proximalStructure->GetId() << "(1)}\n";
                };
            ostr << "{access Dendrite" << structure.id << "}\n" << "{nseg="<< nrOfSeg << "}\n"<< "{pt3dclear()}\n";


            radiuspos=structure.radii.rbegin();
	    radiuspos++;
	   
            for (rpos=structure.geometricComps.rbegin();rpos!=structure.geometricComps.rend();rpos++)
            {

	      start=(*rpos)->start;                             //GetStart()
	      size=(*rpos)->size;                               //GetSize();

	      pt3d[1]=(start[1]+0.5*size[1])*scalex;
	      pt3d[2]=(start[2]+0.5*size[2])*scaley;
	      pt3d[3]=(start[3]+0.5*size[3])*scalez;
	      radius=(*radiuspos);
	      radiuspos++;
	      ostr << "{pt3dadd(" << pt3d[1] << ", " << pt3d[2] << ", " << pt3d[3] << ", " <<  radius << ")}\n";
            };
         
	    
	    start=comp->GetStart();
            size=comp->GetSize();
            pt3d[1]=(start[1]+0.5*size[1])*scalex;
            pt3d[2]=(start[2]+0.5*size[2])*scaley;
            pt3d[3]=(start[3]+0.5*size[3])*scalez;
	    radiuspos=structure.radii.rbegin();
            radius=(*radiuspos);
	  
            ostr << "{pt3dadd(" << pt3d[1] << ", " << pt3d[2] << ", " << pt3d[3] << ", " <<  radius <<")}\n";

            ostr << "\n";
            break;

    };

    //if (structure.kind!=BUTTON)
    //	ostr << *(structure.distal);

    if (structure.distal!=NULL) ostr << *(structure.distal);


    return ostr;
};


/*!
 * This function draws a ball with radius size around the position pos into the targetDC.
 */
int
drawBall(DataCube *targetDC,Vector<double> pos,double size)
{
  Vector<int> startDC(3);
  Vector<int> sizeDC(3);
  startDC=targetDC->GetStart();
  sizeDC=targetDC->GetSize();

  for(int dx=-((int)size+1);dx<=(int)size+1;dx++)
    for(int dy=-((int)size+1);dy<=(int)size+1;dy++)
      for(int dz=-((int)size+1);dz<=(int)size+1;dz++)
	if((targetDC->DCNorm(dx,dy,dz)*targetDC->DCNorm(dx,dy,dz))<=size*size)
	  if((int)pos[1]+dx>=startDC[1] && (int)pos[1]+dx<=startDC[1]+sizeDC[1] &&
	     (int)pos[2]+dx>=startDC[2] && (int)pos[1]+dx<=startDC[2]+sizeDC[2] &&
	     (int)pos[3]+dx>=startDC[3] && (int)pos[3]+dx<=startDC[3]+sizeDC[3])
	    targetDC->SetData((int)pos[1]+dx,(int)pos[2]+dy,(int)pos[3]+dz, 1.0);
  
  return 0;
}


/*!
 * Plots structure to datacube.
 */
int
writeToDataCube(DataCube *targetDC, const Structure &structure)
{
Structure *proximalStructure;
//Structure *distalStructure;
Compartment *proxComp;
Compartment *distComp;
list<Compartment*>::const_iterator pos;
list<Compartment*>::const_reverse_iterator rpos;
list<double>::const_reverse_iterator radiuspos;
double scalex=1.0;
double scaley=1.0;
double scalez=1.0;
Compartment *comp=structure.comp;
int nrOfSeg=1;
if (structure.kind!=CELLBODY)
{
    if (proximalStructure==NULL) {cout << "Error no CELLBODY but proximal=0" << endl; return 1;};
    proximalStructure=(structure.proximal)->GetProximalStructure();
    proxComp=proximalStructure->GetComp();

    for (pos=structure.geometricComps.begin();pos!=structure.geometricComps.end();++pos)
        nrOfSeg++;
}

if (structure.kind==DENDRITE)
{
    //distalStructure=(structure.distal)->GetDistalStructure();
    distComp=(structure.distal)->GetComp();
    comp=distComp; /// this has to be replaced by a set of comps for geometric reconsturion!
}

double radius; //Radius of structure element
Vector<int> start(3);
Vector<int> size(3);
Vector<double> pt3d(3);
double threshold=0.05;



switch (structure.kind)
{
    case CELLBODY:
        start=comp->GetStart();
        size=comp->GetSize();
        pt3d[1]=(start[1]+0.5*size[1])*scalex;
        pt3d[2]=(start[2]+0.5*size[2])*scaley;
        pt3d[3]=(start[3]+0.0*size[3])*scalez;
	radius=comp->CalculateRadius(threshold, 1);
        drawBall(targetDC,pt3d, radius);
        
        pt3d[1]=(start[1]+0.5*size[1])*scalex;
        pt3d[2]=(start[2]+0.5*size[2])*scaley;
        pt3d[3]=(start[3]+0.5*size[3])*scalez;
	radius=comp->CalculateRadius(threshold, 1);
        drawBall(targetDC,pt3d, radius);

        break;

    case BUTTON:
       
        radiuspos=structure.radii.rbegin();
	radiuspos++;
		
	
        
	for (rpos=structure.geometricComps.rbegin();rpos!=structure.geometricComps.rend();rpos++)
        {
	  start=(*rpos)->start;
	  size=(*rpos)->size;
	  pt3d[1]=(start[1]+0.5*size[1])*scalex;
	  pt3d[2]=(start[2]+0.5*size[2])*scaley;
	  pt3d[3]=(start[3]+0.5*size[3])*scalez;
	  
	  radius=*radiuspos;
	  radiuspos++;
	  drawBall(targetDC,pt3d, radius);
        };

	start=comp->GetStart();
	size=comp->GetSize();
	pt3d[1]=(start[1]+0.5*size[1])*scalex;
	pt3d[2]=(start[2]+0.5*size[2])*scaley;
	pt3d[3]=(start[3]+0.5*size[3])*scalez;
	radiuspos=structure.radii.rbegin();
	radius=*radiuspos;
	drawBall(targetDC,pt3d, radius);
	
	

        break;


    case DENDRITE:
        radiuspos=structure.radii.rbegin();
	radiuspos++;

        for (rpos=structure.geometricComps.rbegin();rpos!=structure.geometricComps.rend();rpos++)
        {
	  start=(*rpos)->start;                           //GetStart();
	  size=(*rpos)->size;                             //GetSize();

	  pt3d[1]=(start[1]+0.5*size[1])*scalex;
	  pt3d[2]=(start[2]+0.5*size[2])*scaley;
	  pt3d[3]=(start[3]+0.5*size[3])*scalez;
	  radius=(*radiuspos);
	  radiuspos++;
	  drawBall(targetDC,pt3d, radius);
        };
      
	start=comp->GetStart();
        size=comp->GetSize();
        pt3d[1]=(start[1]+0.5*size[1])*scalex;
        pt3d[2]=(start[2]+0.5*size[2])*scaley;
        pt3d[3]=(start[3]+0.5*size[3])*scalez;
	radiuspos=structure.radii.rbegin();
        radius=(*radiuspos);
	drawBall(targetDC,pt3d, radius);


        break;
};


if (structure.distal!=NULL) writeToDataCube(targetDC,*(structure.distal));


return 0;
}

 
int 
createDataForDX(strstream &radiusData, strstream &positionData, strstream &connectionData, int &nrofposition, int &nroflastlink, const Structure &structure )
{
    Structure *proximalStructure;
    //Structure *distalStructure;
    Compartment *proxComp;
    Compartment *distComp;
    list<Compartment*>::const_iterator pos;
    list<Compartment*>::const_reverse_iterator rpos;
    list<double>::const_reverse_iterator radiuspos;
    
    Compartment *comp=structure.comp;
    double scalex=(comp->GetDataCube())->GetSpaceing()[1];
    double scaley=(comp->GetDataCube())->GetSpaceing()[2];
    double scalez=(comp->GetDataCube())->GetSpaceing()[3];
    
    
    int nrOfSeg=1;
    if (structure.kind!=CELLBODY)
    {
	if (proximalStructure==NULL) {cout << "Error no CELLBODY but proximal=0" << endl; return 1;};
	proximalStructure=(structure.proximal)->GetProximalStructure();
	proxComp=proximalStructure->GetComp();
	
	for (pos=structure.geometricComps.begin();pos!=structure.geometricComps.end();++pos)
	    nrOfSeg++;
    }
    
    if (structure.kind==DENDRITE)
    {
	//distalStructure=(structure.distal)->GetDistalStructure();
	distComp=(structure.distal)->GetComp();
	comp=distComp; /// this has to be replaced by a set of comps for geometric reconsturion!
    }
    
    double radius; //Radius of structure element
    Vector<int> start(3);
    Vector<int> size(3);
    Vector<double> pt3d(3);
    double threshold=0.05;
    int lastconnection;
    
    
    switch (structure.kind)
    {
	case CELLBODY:
	    start=comp->GetStart();
	    size=comp->GetSize();
	    pt3d[1]=(start[1]+0.5*size[1])*scalex;
	    pt3d[2]=(start[2]+0.5*size[2])*scaley;
	    pt3d[3]=(start[3]+0.0*size[3])*scalez;
	    radius=comp->CalculateRadius(threshold, 1);
	    
	    //write data for dx presentation
	    nrofposition++;
	    radiusData << radius << endl; 
	    positionData << pt3d[1] << " " << pt3d[2] << " " << pt3d[3] << endl;
	    
	    pt3d[1]=(start[1]+0.5*size[1])*scalex;
	    pt3d[2]=(start[2]+0.5*size[2])*scaley;
	    pt3d[3]=(start[3]+0.5*size[3])*scalez;
	    radius=comp->CalculateRadius(threshold, 1);
	   
	    //write data for dx presentation
	    nrofposition++;
	    radiusData << radius << endl; 
	    positionData << pt3d[1] << " " << pt3d[2] << " " << pt3d[3] << endl;
	    connectionData << (nrofposition-1) << " " << nrofposition << endl; 
		
		
	    
	    break;
	    
	case BUTTON:
	    
	    radiuspos=structure.radii.rbegin();
	    radiuspos++;
	    
	    
	    lastconnection=nroflastlink;
	    
	    for (rpos=structure.geometricComps.rbegin();rpos!=structure.geometricComps.rend();rpos++)
	    {
		start=(*rpos)->start;
		size=(*rpos)->size;
		pt3d[1]=(start[1]+0.5*size[1])*scalex;
		pt3d[2]=(start[2]+0.5*size[2])*scaley;
		pt3d[3]=(start[3]+0.5*size[3])*scalez;
		
		radius=*radiuspos;
		radiuspos++;
		//write data for dx presentation
		nrofposition++;
		radiusData << radius << endl; 
		positionData << pt3d[1] << " " << pt3d[2] << " " << pt3d[3] << endl;
		connectionData << lastconnection << " " << nrofposition << endl;
		lastconnection=nrofposition;
		//Question: does this start with first or last connection?
		
		
	    };
	    
	    start=comp->GetStart();
	    size=comp->GetSize();
	    pt3d[1]=(start[1]+0.5*size[1])*scalex;
	    pt3d[2]=(start[2]+0.5*size[2])*scaley;
	    pt3d[3]=(start[3]+0.5*size[3])*scalez;
	    radiuspos=structure.radii.rbegin();
	    radius=*radiuspos;
	    //write data for dx presentation
	    nrofposition++;
	    radiusData << radius << endl; 
	    positionData << pt3d[1] << " " << pt3d[2] << " " << pt3d[3] << endl;
	    connectionData << lastconnection << " " << nrofposition << endl;
	    lastconnection=nrofposition;
	    //Question: does this start with first or last connection?
	    
	    	    
	    
	    
	    break;
	    
	    
	case DENDRITE:
	    radiuspos=structure.radii.rbegin();
	    radiuspos++;
	    lastconnection=nroflastlink;
	    
	    for (rpos=structure.geometricComps.rbegin();rpos!=structure.geometricComps.rend();rpos++)
	    {
		start=(*rpos)->start;                           //GetStart();
		size=(*rpos)->size;                             //GetSize();
		
		pt3d[1]=(start[1]+0.5*size[1])*scalex;
		pt3d[2]=(start[2]+0.5*size[2])*scaley;
		pt3d[3]=(start[3]+0.5*size[3])*scalez;
		radius=(*radiuspos);
		radiuspos++;
		//write data for dx presentation
		nrofposition++;
		radiusData << radius << endl; 
		positionData << pt3d[1] << " " << pt3d[2] << " " << pt3d[3] << endl;
		connectionData << lastconnection << " " << nrofposition << endl;
		lastconnection=nrofposition;
		//Question: does this start with first or last connection?
	    };
	    
	    start=comp->GetStart();
	    size=comp->GetSize();
	    pt3d[1]=(start[1]+0.5*size[1])*scalex;
	    pt3d[2]=(start[2]+0.5*size[2])*scaley;
	    pt3d[3]=(start[3]+0.5*size[3])*scalez;
	    radiuspos=structure.radii.rbegin();
	    radius=(*radiuspos);
	    //write data for dx presentation
	    nrofposition++;
	    radiusData << radius << endl; 
	    positionData << pt3d[1] << " " << pt3d[2] << " " << pt3d[3] << endl;
	    connectionData << lastconnection << " " << nrofposition << endl;
	    lastconnection=nrofposition;
	    //Question: does this start with first or last connection?
	    //drawBall(targetDC,pt3d, radius);<- erstetzen hier daten fŸr dx
	    
	    
	    break;
    };
    
    
    if (structure.distal!=NULL) createDataForDX(radiusData, positionData, connectionData, nrofposition, *(structure.distal));

    return 0;
};






int
Structure::
measureRadii(double threshold)
{
  int choice; //Fit in of a ball starting with a small one (1)
              //Fit in of a ball starting with a samll one (2)
  
  choice=1;

  list<Compartment*>::iterator pos;
  if (kind==DENDRITE)
    comp=(distal)->GetComp();
  cout << "Measure radii" << endl;
  
  pos=geometricComps.begin();
 
  while (pos!=geometricComps.end())
    {
      radii.push_back((*pos)->CalculateRadius(threshold, choice));
      pos++;
    }

  
  radii.push_back(comp->CalculateRadius(threshold, choice));

  if (distal!=NULL)  return distal->measureRadii(threshold);
  return 0;
  
}


/*! This functions comapres the name of the structure with the supposed Name returns 1 if the names are euqal
*/
int 
Structure::
IsThisMyName(char *supposedName) 
{
    if (strcmp(name,supposedName)==0) return 1;
    return 0;
}


int 
Structure::
addPoint3D(Vector<double> &pt3d)
{
    //Start from proximal vectors continue to distal
    pt3dPoints.push_back(pt3d);
    
    return 0;
}



//Cellbody
Cellbody::
Cellbody() : Structure()
{
    kind=CELLBODY;
}

//Dendrite
Dendrite::
Dendrite()
{
    kind=DENDRITE;
}


//Button
Button::
Button()
{
    kind=BUTTON;
}


//Link
int Link::IDCounter=0;

Link::
Link()
{
    IDCounter++;
    id=IDCounter;
    proximal=NULL;
    sprintf(name,"noName");
};

int
Link::
SetProximalStructure(Structure *prox)
{
    proximal=prox;
    return 0;

}

int
Link::
AddDistalStructure(Structure *dist)
{
    distal.push_back(dist);
    return 0;
}



ostream &operator<<(ostream &ostr, const Link &link)
{
    //ostr << "[ Link:" << link.id;
    //ostr << " Conected to:";
    list<Structure*>::const_iterator  pos;
    for(pos=link.distal.begin();pos!=link.distal.end();++pos)
        ostr << **pos;
    //ostr << "]";


    return ostr;
};


int
writeToDataCube(DataCube *targetDC, const Link &link)
{
   
    list<Structure*>::const_iterator  pos;
    for(pos=link.distal.begin();pos!=link.distal.end();++pos)
        if (writeToDataCube(targetDC, **pos)) return 1;

    return 0;
};

int createDataForDX(strstream &radiusData, strstream &positionData, strstream &connectionData, int &nrofposition, const Link &link )
{
    int nroflastlink=nrofposition;
    list<Structure*>::const_iterator  pos;
    for(pos=link.distal.begin();pos!=link.distal.end();++pos)
        if (createDataForDX(radiusData, positionData, connectionData, nrofposition, nroflastlink, **pos)) return 1;
    
    
    return 0;
}




int
Link::
measureRadii(double threshold)
{

    list<Structure*>::iterator  pos;
    for(pos=distal.begin();pos!=distal.end();++pos)
        if ((*pos)->measureRadii(threshold)) return 1;

    return 0;

}


int 
Link::
IsThisMyName(char *supposedName) // returns 1 if its the name of the link
{
    if (strcmp(name,supposedName)==0) return 1;
    return 0;

}




int 
Link::
SetName(char *lName)
{
    strcpy(name,lName);
    return 0;
}








