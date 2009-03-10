#include "compartment.h"
/*! A Compartment represents a fixed entity of the data set. Usually that means one compartemnt represents one voxel of 
*   the neuron structure. The compartments form a graph from which finnaly the tree of the neuron is extracted.
*   The special use of the compartemt class is the ability to add and delete elements and to navigate through the graph.
*   The compartment class is connected to the neuron class (see neuron.h). 
*/

// Basis methods of the class
//friend functions of Connection class
int operator==(const Connection &A, const Connection &B)
{
    return ( (A.kind==B.kind) && (A.from==B.from) && (A.to==B.to) );
};

int isKindEqual(const Connection &A,const short kind)
{
    return (A.kind==kind);
};

ostream &operator<<(ostream &ostr, const Connection &CON)
{
    ostr << "----Connection----"<<endl;
    ostr << "Kind:" << CON.kind << endl;
    ostr << "From:" << CON.from << endl;
    ostr << "To:"   << CON.to   << endl;
    ostr << "-------------------"<<endl;
    return (ostr);
}


//Static Variable to Identifie Compartments Unique
int Compartment::IDCounter=0;


//Constructor set inital values and father
Compartment::
Compartment(DataCube *dC,int llevel,const Vector<int> &lstart,const Vector<int> &lsize,Compartment *lfather, int lpositionInFatherComp)
:Volume(dC->GetDim(),lstart,lsize,dC->GetSpaceing())
{
    dataCube=dC;
    level=llevel;

    id=IDCounter;
    IDCounter=IDCounter+1;

    distanceFromBody=MAXARRIVALTIME;
	shellModelDistanceFromBody=MAXARRIVALTIME; 

    flags= 0;
    positionInFatherComp=lpositionInFatherComp;
    if (lfather!=NULL) addToConnectionList(FATHER, lfather);

    nrPasingThrouh=0;
    nrOfLinkConnections=0;

    link=NULL;
    structure=NULL;

    localThreshold=1.0;
};


/*!
 * The destructor deltes this Compartment from every Connection List of other compatments. This is
 * necessary to keep all connection up to date.
 */
Compartment::
~Compartment()
{
    //delete Refernces
    list<Connection>::iterator pos;
    for (pos=reverseConnectionList.begin();pos!=reverseConnectionList.end();++pos)
    {
        if (pos==reverseConnectionList.end()) return;
				// GJ: you are not apparently allowed a null
        // if (pos==NULL)
        // {
        //     cerr <<" Error pos Points EMPTY!!!" << endl;
        //     exit(1);};

        if ((*pos).to!=this)
        {
            cerr << "Error, wrong connection! .to" << endl;
            exit(1);};

        if ((*pos).from==NULL )
        {
            cerr <<" Error Referrence Points EMPTY!!!" << endl;
            exit(1);}

        if (((*pos).from)->deleteFromConnectionList(*pos))
        {
            cerr << "I was looking for:" << endl << (*pos) << endl;
            cerr << "Could not find entry, quit!!!"<< endl;
            exit(1);
        };
    }

    for (pos=ConnectionList.begin();pos!=ConnectionList.end();++pos)
    {
        if (pos==ConnectionList.end()) return;
        if (pos==NULL)
        {
            cerr <<" Error pos Points EMPTY!!!" << endl;
            exit(1);};

        if ((*pos).from!=this)
        {
            cerr << "Error, wrong connection! .from" << endl;
            exit(1);};

        if ((*pos).from==NULL )
        {
            cerr <<" Error Referrence Points EMPTY!!!" << endl;
            exit(1);}


        if (((*pos).to)->deleteFromReverseConnectionList(*pos))
        {
            cerr << "Try to delete from ReverseConnection List!"<<endl;
            cerr << "I was looking:" << endl << (*pos) << endl;
            cerr << "Could not find entry, quit!!!"<< endl;
            exit(1);
        };
    }


}


//Access to underlying datacube data

/////////////////////////   ShellModel    //////////////////////
//Initializes index list  
int
Compartment::
SetShellIndex(list<int> index)
{
    list<int>::iterator pos;
    
    while(!shellIndex.empty())
    {
	pos=shellIndex.begin();
	shellIndex.erase(pos);
    }
    
    for (pos=index.begin(); pos!=index.end(); ++pos)
	shellIndex.push_back(*pos);
    return 0;
}
////////////////////////////////////////////////////////////

//old 2D Acess
NeuraDataType
&Compartment::
operator ()(int x, int y)
{
    return (*dataCube)(start[1]+x,start[2]+y);
}

NeuraDataType
&Compartment::
operator ()(int x, int y, int z)
{

    if (!((x>=0 && x<size[1]) && (y>=0 && y<size[2]) && (z>=0 && z<size[3])))
    {
        cerr << "Access without a valid range!" << endl;
        cerr << "x,y,z=" << x << "," << y << "," << z << endl;
        cerr << "Size" << endl << size << endl;



    }
    return (*dataCube)(start[1]+x,start[2]+y,start[3]+z);
}


NeuraDataType
&Compartment::
operator[](Vector<int> X)
{ return (*dataCube)[X+start];
}


//IO Functions
int
Compartment::
writePPM2D (char *filename) {
    //int num;
    FILE *fptr;
    unsigned char outData;

    if (filename == NULL)
        fptr = stdout;
    else
        fptr = fopen(filename, "w");

    /* header for PPM file */
    fprintf(fptr, "P6\n");
    fprintf(fptr, "%d %d\n", size[1], size[2]);
    fprintf(fptr, "255\n");

    /* writing out actual image bytes */
    for (int y = 0; y < size[2];  y++) {
        for (int x = 0; x < size[1]; x++) {
            outData = (unsigned char) (255.0 *(*this)(x,y));
            fwrite(&outData, 1, sizeof(unsigned char), fptr);
            fwrite(&outData, 1, sizeof(unsigned char), fptr);
            fwrite(&outData, 1, sizeof(unsigned char), fptr);
        }
    }
    return 0;
}

int
Compartment::
writeSlicePPM (char *filename,int sliceNr, int direction)
{
    if (dim==2) return writePPM2D(filename);
   // int num;
    FILE *fptr;
    unsigned char outData;

    if (filename == NULL)
        fptr = stdout;
    else
        fptr = fopen(filename, "w");

    /* header for PPM file */
    fprintf(fptr, "P6\n");

    if (direction==Z_DIR)
    {
        fprintf(fptr, "%d %d\n", size[1], size[2]);
        fprintf(fptr, "255\n");
        /* writing out actual image bytes */
        for (int y = 0; y < size[2];  y++) {
            for (int x = 0; x < size[1]; x++) {
                outData = (unsigned char) (255.0 *(*this)(x,y,sliceNr));
                fwrite(&outData, 1, sizeof(unsigned char), fptr);
                fwrite(&outData, 1, sizeof(unsigned char), fptr);
                fwrite(&outData, 1, sizeof(unsigned char), fptr);
            }
        }
    }

    if (direction==Y_DIR)
    {
        fprintf(fptr, "%d %d\n", size[1], size[3]);
        fprintf(fptr, "255\n");
        /* writing out actual image bytes */
        for (int z = 0; z < size[3];  z++) {
            for (int x = 0; x < size[1]; x++) {
                outData = (unsigned char) (255.0 *(*this)(x,sliceNr,z));
                fwrite(&outData, 1, sizeof(unsigned char), fptr);
                fwrite(&outData, 1, sizeof(unsigned char), fptr);
                fwrite(&outData, 1, sizeof(unsigned char), fptr);
            }
        }
    }

    if (direction==X_DIR)
    {
        fprintf(fptr, "%d %d\n", size[2], size[3]);
        fprintf(fptr, "255\n");
        /* writing out actual image bytes */
        for (int y = 0; y < size[2];  y++) {
            for (int z = 0; z < size[3]; z++) {
                outData = (unsigned char) (255.0 *(*this)(sliceNr,y,z));
                fwrite(&outData, 1, sizeof(unsigned char), fptr);
                fwrite(&outData, 1, sizeof(unsigned char), fptr);
                fwrite(&outData, 1, sizeof(unsigned char), fptr);
            }
        }
    }

    return 0;
}




int
Compartment::
writeToDataCube(DataCube *target)
{
    int sz=start[3];
    int sy=start[2];
    int sx=start[1];
    int tz=size[3]+start[3];
    int ty=size[2]+start[2];
    int tx=size[1]+start[1];
	
	 NeuraDataType value=0.9;
	 if (FLAG(NOTCONNECTEDTOBODY)) value=0.5;
	 if (FLAG(BODY)) value=1.0;
	 if (FLAG(ENDPOINT)) value=1.0;
	 if (FLAG(BIFURCATION)) value=1.0;

    for (int z=sz;z<tz;z++)
        for (int y=sy;y<ty;y++)
            for (int x=sx;x<tx;x++)
                (*target)(x,y,z)=value;

    return 0;
}


//translates meaning of flag
int cleartextFlag(int flag, std::string &buffer)
{
    char help[20];
    sprintf(help,"Flags:");
    buffer=help;
    if (flag &  STRUCTUREONMYSURFACE) buffer+= "STRUCTUREONMYSURFACE ";
    if (flag &  SINGLE) buffer+= "SINGLE ";
    if (flag &  LOCKED) buffer+= "LOCKED ";
    if (flag &  BOUNDARYCOMP) buffer+= "BOUNDARYCOMP ";
    if (flag &  BODY) buffer+= "BODY ";
    if (flag &  ENDPOINT) buffer+= "ENDPOINT ";
    if (flag &  SKELETONCOMP) buffer+= "SKELETONCOMP ";

    return 0;

}

//friend function for information output

//translates kind of neigbour to position
int cleartext(int kind, std::string &buffer)
{
    char help[20];
    sprintf(help,"%d=",kind);
    buffer=help;

    switch (kind)
    {
        case N_TopFront: buffer+="NTopFront";break;
        case N_TopBack : buffer+= "NTopBack";break;
        case N_TopFrontRight: buffer+= "NTopFrontRight";break;
        case N_TopRight : buffer+="NTopRight";break;
        case N_TopLeft: buffer+= "NTopLeft";break;
        case N_TopFrontLeft: buffer+= "NTopFrontLeft";break;
        case N_TopBackLeft: buffer+= "NTopBackLeft";break;
        case N_Top: buffer+= "NTop";break;
        case N_Right: buffer+= "N Right";break;
        case N_FrontRight: buffer+= "NFrontRight";break;
        case N_Back : buffer+= "NBack";break;
        case N_Left : buffer+= "NLeft";break;
        case N_FrontLeft: buffer+= "NFrontLeft";break;
        case N_BackLeft  : buffer+= "NBackLeft";break;
        case N_BackRight: buffer+= "NBackRight";break;
        case N_Front: buffer+= "NFront";break;
        case N_Bottom: buffer+= "NBottom";break;
        case N_BottomRight: buffer+= "N_BottomRight";break;
        case  N_BottomFrontRight: buffer+= " N_BottomFrontRight";break;
        case N_BottomBack: buffer+= "N_BottomBack";break;
        case  N_BottomLeft : buffer+= "N_BottomLeft";break;
        case  N_BottomFrontLeft: buffer+= "N_BottomFrontLeft";break;
        case  N_BottomBackLeft: buffer+= "N_BottomBackLef";break;
        case  N_BottomBackRight: buffer+= " N_BottomBackRight";break;
        case  N_BottomFront: buffer+= "N_BottomFront";break;
        case S_TopLeftFront    :  buffer+= "STopLeftFront ";break;
        case  S_TopRightFront  : buffer+= "STopRightFront";break;
        case S_TopLeftRear  : buffer+= "STopLeftRear";break;
        case   S_TopRightRear : buffer+= "STopRightRear";break;
        case  S_BottomLeftFront  : buffer+= "SBottomLeftFront ";break;
        case  S_BottomRightFront  : buffer+= "SBottomRightFront";break;
        case  S_BottomLeftRear  : buffer+= "SBottomLeftRear ";break;
        case  S_BottomRightRear : buffer+= "SBottomRightRear ";break;
        case  S_SingleChild : buffer+= "SSingleChild";break;
        case  FATHER: buffer+= "Father";break;
        default: {buffer+="Unknown!:";};
    };

    return 0;

}



ostream &operator<<(ostream &ostr, const Compartment &CC)
{

    ostr << "----Compartment----"<<endl;
    ostr << "id:" << CC.id << endl;
    ostr << "Dimension:" << CC.dim << endl;
    ostr << "Start:" << endl << CC.start;
    ostr << "Size:" << endl << CC.size;
    ostr << "d:" << endl << CC.d;
    ostr << "Flags:" << CC.flags << endl;
std::string help;
    cleartextFlag(CC.flags,help);
    ostr << help << endl;
    ostr << "d:" << CC.distanceFromBody << endl;
    ostr << "Connections:" ;
    list<Connection>::const_iterator pos;
    int counter=0;
    for (pos=CC.ConnectionList.begin();pos!=CC.ConnectionList.end();++pos)
    {
std::string s;
        counter++;
        cleartext((*pos).kind,s);
        ostr << "(k:" ;
        ostr <<  s ;
        ostr << " id:" << (*pos).to->id << ")";
    }
    ostr << "There are" << counter << "Elements in the connection list" << endl;
    ostr << endl << "Reverseconnections:" ;
    counter=0;
    for (pos=CC.reverseConnectionList.begin();pos!=CC.reverseConnectionList.end();++pos)
    {
std::string s;
        counter++;
        cleartext((*pos).kind,s);
        ostr << "(k:" ;
        ostr <<  s ;
        ostr << " id:" << (*pos).from->id << ")";
    }

 ostr << "There are" << counter << "Elements in the reverse connection list" << endl;
    ostr << endl;
    ostr << "-------------------"<<endl;


    return (ostr);

}

// Management of Graph
//handles list of neigbours, sons and father
int
Compartment::
addToConnectionList(Connection &con)
{
    ConnectionList.push_front(con);
    (con.to)->addToReverseConnectionList(con);
    return 0;
}


int
Compartment::
addToConnectionList(short kind, Compartment *to)
{
    if (to==NULL) return 1;
    Connection con(kind,this,to);
    addToConnectionList(con);
    return 0;
}


int
Compartment::
addToReverseConnectionList(Connection &con)
{
    reverseConnectionList.push_front(con);
    return 0;
}


int
Compartment::
SetNeigbour(Compartment *neigbour, int position)
{
    if (position<N_TopFront || position > N_Bottom || neigbour==NULL) return 1;
    if (neigbour==NULL) return 1;
    addToConnectionList(position,neigbour);
    return 0;
}


int
Compartment::
SetSon(Compartment *son, int nr)
{
    if (nr<S_TopLeftFront || nr >  S_SingleChild) return 1;
    if (son==NULL) return 1;
    addToConnectionList(nr,son);
    return 0;
}


int
Compartment::
deleteFromReverseConnectionList(Connection &con)
{
    list<Connection>::iterator pos;
    for (pos=reverseConnectionList.begin();pos!=reverseConnectionList.end();++pos)
	if ((*pos)==con)
	{
	    reverseConnectionList.erase(pos);
	    return 0;
	};
    
    return 1; //not found!!!
}



int
Compartment::
deleteFromConnectionList(Connection &con)
{
    list<Connection>::iterator pos;
    for (pos=ConnectionList.begin();pos!=ConnectionList.end();++pos)
	if ((*pos)==con)
	{
	    ConnectionList.erase(pos);
	    return 0;
	};
    
    return 1; //not found!!!
}


//Data access
Compartment*
Compartment::
GetConnection(short kind)
{
    list<Connection>::iterator pos;
    //pos=find_if(ConnectionList.begin(),ConnectionList.end(),bind2nd(isKindEqual(),kind));
    for (pos=ConnectionList.begin();pos!=ConnectionList.end();++pos)
    {if (isKindEqual(*pos,kind)) break;};


    if (pos!=ConnectionList.end()) return (*pos).to;
    return NULL;
}

Compartment*
Compartment::
GetNeigbourThrouhPositionIdentifier(int neigbourOfFather, int compInNeigbour)
{
    Compartment* father;
    father=GetConnection(FATHER);
    if (father==NULL)
    {
        cerr << "Cant find father, quit!!!" << endl;
        exit(1);
    }

    if (neigbourOfFather==ORIGIN)
    {
        if ( (father->GetConnection(compInNeigbour))==NULL) return NULL;
        return father->GetConnection(compInNeigbour);
    };

    if (father->GetConnection(neigbourOfFather)==NULL) return NULL;

    if ( (father->GetConnection(neigbourOfFather))->GetConnection(compInNeigbour)==NULL) return father->GetConnection(neigbourOfFather);


    return (father->GetConnection(neigbourOfFather))->GetConnection(compInNeigbour);

}

Compartment*
Compartment::
NeigbourOfNeigbour(short Neigbour1,short Neigbour2)
{
    if (GetConnection(Neigbour1)!=NULL)
        return (GetConnection(Neigbour1))->GetConnection(Neigbour2);
    // return NULL;//debug!!!

    if (GetConnection(Neigbour2)!=NULL)
        return (GetConnection(Neigbour2))->GetConnection(Neigbour1);

    return NULL;

}

Compartment*
Compartment::
NeigbourOfNeigbourOfNeigbour(short Neigbour1,short Neigbour2,short Neigbour3)
{
    if (GetConnection(Neigbour1)!=NULL)
        if ( (GetConnection(Neigbour1))->NeigbourOfNeigbour(Neigbour2,Neigbour3) !=NULL)
            return (GetConnection(Neigbour1))->NeigbourOfNeigbour(Neigbour2,Neigbour3);

    //return NULL; //debug!!!

    if (GetConnection(Neigbour2)!=NULL)
        if ( (GetConnection(Neigbour2))->NeigbourOfNeigbour(Neigbour1,Neigbour3) !=NULL)
            return (GetConnection(Neigbour2))->NeigbourOfNeigbour(Neigbour1,Neigbour3);

    if (GetConnection(Neigbour3)!=NULL)
        if ( (GetConnection(Neigbour3))->NeigbourOfNeigbour(Neigbour1,Neigbour2) !=NULL)
            return (GetConnection(Neigbour3))->NeigbourOfNeigbour(Neigbour1,Neigbour2);

    return NULL;

}

//neigbourhood
/*!
* This method searches for the the neigbours of the compartent
 */
int
Compartment::
FindAndSetMyNeigbours6()
{
    
    switch (positionInFatherComp)
    {
	case S_TopLeftFront    :
	{
	    SetNeigbour(GetNeigbourThrouhPositionIdentifier(N_Front,S_TopLeftRear),N_Front);
	    SetNeigbour(GetNeigbourThrouhPositionIdentifier(N_Left,S_TopRightFront),N_Left);
	    SetNeigbour(GetNeigbourThrouhPositionIdentifier(ORIGIN,S_TopLeftRear),N_Back);
	    SetNeigbour(GetNeigbourThrouhPositionIdentifier(ORIGIN,S_TopRightFront),N_Right);
	    SetNeigbour(GetNeigbourThrouhPositionIdentifier(N_Top,S_BottomLeftFront),N_Top);
	    SetNeigbour(GetNeigbourThrouhPositionIdentifier(ORIGIN,S_BottomLeftFront),N_Bottom);
	    
	};break;
	    
	case S_TopRightFront   :
	{
	    SetNeigbour(GetNeigbourThrouhPositionIdentifier(N_Front,S_TopRightRear),N_Front);
	    SetNeigbour(GetNeigbourThrouhPositionIdentifier(ORIGIN,S_TopLeftFront),N_Left);
	    SetNeigbour(GetNeigbourThrouhPositionIdentifier(ORIGIN,S_TopRightRear),N_Back);
	    SetNeigbour(GetNeigbourThrouhPositionIdentifier(N_Right,S_TopLeftFront),N_Right);
	    SetNeigbour(GetNeigbourThrouhPositionIdentifier(N_Top,S_BottomRightFront),N_Top);
	    SetNeigbour(GetNeigbourThrouhPositionIdentifier(ORIGIN,S_BottomRightFront),N_Bottom);
	};break;
	    
	case S_TopLeftRear     :
	{
	    SetNeigbour(GetNeigbourThrouhPositionIdentifier(ORIGIN,S_TopLeftFront),N_Front);
	    SetNeigbour(GetNeigbourThrouhPositionIdentifier(N_Left,S_TopRightRear),N_Left);
	    SetNeigbour(GetNeigbourThrouhPositionIdentifier(N_Back,S_TopLeftFront),N_Back);
	    SetNeigbour(GetNeigbourThrouhPositionIdentifier(ORIGIN,S_TopRightRear),N_Right);
	    SetNeigbour(GetNeigbourThrouhPositionIdentifier(N_Top,S_BottomLeftRear),N_Top);
	    SetNeigbour(GetNeigbourThrouhPositionIdentifier(ORIGIN,S_BottomLeftRear),N_Bottom);
	};break;
	    
	case S_TopRightRear    :
	{
	    SetNeigbour(GetNeigbourThrouhPositionIdentifier(ORIGIN,S_TopRightFront),N_Front);
	    SetNeigbour(GetNeigbourThrouhPositionIdentifier(ORIGIN,S_TopLeftRear),N_Left);
	    SetNeigbour(GetNeigbourThrouhPositionIdentifier(N_Back,S_TopRightFront),N_Back);
	    SetNeigbour(GetNeigbourThrouhPositionIdentifier(N_Right,S_TopLeftRear),N_Right);
	    SetNeigbour(GetNeigbourThrouhPositionIdentifier(N_Top,S_BottomRightRear),N_Top);
	    SetNeigbour(GetNeigbourThrouhPositionIdentifier(ORIGIN,S_BottomRightRear),N_Bottom);
	};break;
	case S_BottomLeftFront :
	{
	    SetNeigbour(GetNeigbourThrouhPositionIdentifier(N_Front,S_BottomLeftRear),N_Front);
	    SetNeigbour(GetNeigbourThrouhPositionIdentifier(N_Left,S_BottomRightFront),N_Left);
	    SetNeigbour(GetNeigbourThrouhPositionIdentifier(ORIGIN,S_BottomLeftRear),N_Back);
	    SetNeigbour(GetNeigbourThrouhPositionIdentifier(ORIGIN,S_BottomRightFront),N_Right);
	    SetNeigbour(GetNeigbourThrouhPositionIdentifier(ORIGIN,S_TopLeftFront),N_Top);
	    SetNeigbour(GetNeigbourThrouhPositionIdentifier(N_Bottom,S_TopLeftFront),N_Bottom);
	};break;
	    
	case S_BottomRightFront:
	{
	    SetNeigbour(GetNeigbourThrouhPositionIdentifier(N_Front,S_BottomRightRear),N_Front);
	    SetNeigbour(GetNeigbourThrouhPositionIdentifier(ORIGIN,S_BottomLeftFront),N_Left);
	    SetNeigbour(GetNeigbourThrouhPositionIdentifier(ORIGIN,S_BottomRightRear),N_Back);
	    SetNeigbour(GetNeigbourThrouhPositionIdentifier(N_Right,S_BottomLeftFront),N_Right);
	    SetNeigbour(GetNeigbourThrouhPositionIdentifier(ORIGIN,S_TopRightFront),N_Top);
	    SetNeigbour(GetNeigbourThrouhPositionIdentifier(N_Bottom,S_TopRightFront),N_Bottom);
	};break;
	    
	case S_BottomLeftRear  :
	{
	    SetNeigbour(GetNeigbourThrouhPositionIdentifier(ORIGIN,S_BottomLeftFront),N_Front);
	    SetNeigbour(GetNeigbourThrouhPositionIdentifier(N_Left,S_BottomRightRear),N_Left);
	    SetNeigbour(GetNeigbourThrouhPositionIdentifier(N_Back,S_BottomLeftFront),N_Back);
	    SetNeigbour(GetNeigbourThrouhPositionIdentifier(ORIGIN,S_BottomRightRear),N_Right);
	    SetNeigbour(GetNeigbourThrouhPositionIdentifier(ORIGIN,S_TopLeftRear),N_Top);
	    SetNeigbour(GetNeigbourThrouhPositionIdentifier(N_Bottom,S_TopLeftRear),N_Bottom);
	};break;
	case S_BottomRightRear :
	{
	    SetNeigbour(GetNeigbourThrouhPositionIdentifier(ORIGIN,S_BottomRightFront),N_Front);
	    SetNeigbour(GetNeigbourThrouhPositionIdentifier(ORIGIN,S_BottomLeftRear),N_Left);
	    SetNeigbour(GetNeigbourThrouhPositionIdentifier(N_Back,S_BottomRightFront),N_Back);
	    SetNeigbour(GetNeigbourThrouhPositionIdentifier(N_Right,S_BottomLeftRear),N_Right);
	    SetNeigbour(GetNeigbourThrouhPositionIdentifier(ORIGIN,S_TopRightRear),N_Top);
	    SetNeigbour(GetNeigbourThrouhPositionIdentifier(N_Bottom,S_TopRightRear),N_Bottom);
	};break;
	    
	case S_SingleChild :
	{
	    //to do!!!
	    SetNeigbour(GetNeigbourThrouhPositionIdentifier(ORIGIN,S_BottomRightFront),N_Front);
	    SetNeigbour(GetNeigbourThrouhPositionIdentifier(ORIGIN,S_BottomLeftRear),N_Left);
	    SetNeigbour(GetNeigbourThrouhPositionIdentifier(N_Back,S_BottomRightFront),N_Back);
	    SetNeigbour(GetNeigbourThrouhPositionIdentifier(N_Right,S_BottomLeftRear),N_Right);
	    SetNeigbour(GetNeigbourThrouhPositionIdentifier(ORIGIN,S_TopRightRear),N_Top);
	    SetNeigbour(GetNeigbourThrouhPositionIdentifier(N_Bottom,S_TopRightRear),N_Bottom);
	    
	    
	};break;
	    
	    
	    
	case ORIGIN: ;break;
    }
    
    
    
    return 0;
}



int
Compartment::
FindAndSetMyNeigbours27()
{
    
    //top
    SetNeigbour(NeigbourOfNeigbour(N_Top,N_Front),N_TopFront);
    SetNeigbour(NeigbourOfNeigbour(N_Top,N_Back),N_TopBack);
    SetNeigbour(NeigbourOfNeigbourOfNeigbour(N_Top,N_Front,N_Right),N_TopFrontRight);
    SetNeigbour(NeigbourOfNeigbour(N_Top,N_Right),N_TopRight);
    SetNeigbour(NeigbourOfNeigbour(N_Top,N_Left),N_TopLeft);
    SetNeigbour(NeigbourOfNeigbourOfNeigbour(N_Top,N_Front,N_Left),N_TopFrontLeft);
    SetNeigbour(NeigbourOfNeigbourOfNeigbour(N_Top,N_Back,N_Left),N_TopBackLeft);
    SetNeigbour(NeigbourOfNeigbourOfNeigbour(N_Top,N_Back,N_Right),N_TopBackRight);
    
    
    //samelevel
    SetNeigbour(NeigbourOfNeigbour(N_Front,N_Right),N_FrontRight);
    SetNeigbour(NeigbourOfNeigbour(N_Front,N_Left),N_FrontLeft);
    SetNeigbour(NeigbourOfNeigbour(N_Back,N_Left),N_BackLeft);
    SetNeigbour(NeigbourOfNeigbour(N_Back,N_Right),N_BackRight);
    
    //bottom
    SetNeigbour(NeigbourOfNeigbour(N_Bottom,N_Right),N_BottomRight);
    SetNeigbour(NeigbourOfNeigbourOfNeigbour(N_Bottom,N_Front,N_Right),N_BottomFrontRight);
    SetNeigbour(NeigbourOfNeigbour(N_Bottom,N_Back),N_BottomBack);
    SetNeigbour(NeigbourOfNeigbour(N_Bottom,N_Left),N_BottomLeft);
    SetNeigbour(NeigbourOfNeigbourOfNeigbour(N_Bottom,N_Front,N_Left),N_BottomFrontLeft);
    SetNeigbour(NeigbourOfNeigbourOfNeigbour(N_Bottom,N_Back,N_Left),N_BottomBackLeft);
    SetNeigbour(NeigbourOfNeigbourOfNeigbour(N_Bottom,N_Back,N_Right),N_BottomBackRight);
    SetNeigbour(NeigbourOfNeigbour(N_Bottom,N_Front),N_BottomFront);
    
    return 0;
}



//evaluate underlying data
int
Compartment::
IsThereStructureOnMySurface(double epsilon)
{


    CLEARFLAG(STRUCTUREONMYSURFACE);
    SETFLAG(LOCKED);
    int x,y,z;

    for (x=0;x<size[1];x++)
        for (y=0;y<size[2];y++)
        {
            if ((*this)(x,y,size[3]-1)>epsilon)
            {
                SETFLAG(STRUCTUREONMYSURFACE);
            }
            else
            {
                CLEARFLAG(LOCKED);
            };

            if ((*this)(x,y,0)>epsilon)
            {
                SETFLAG(STRUCTUREONMYSURFACE);
            }
            else
            {
                CLEARFLAG(LOCKED);
            };

        };

    for (z=0;z<size[3];z++)
        for (y=0;y<size[2];y++)
        {
            if ((*this)(size[1]-1,y,z)>epsilon)
            {
                SETFLAG(STRUCTUREONMYSURFACE);
            }
            else
            {
                CLEARFLAG(LOCKED);
            };
            if ((*this)(0,y,z)>epsilon)
            {
                SETFLAG(STRUCTUREONMYSURFACE);
            }
            else
            {
                CLEARFLAG(LOCKED);
            };
        };


    for (x=0;x<size[1];x++)
        for (z=0;z<size[3];z++)
        {
            if ((*this)(x,size[2]-1,z)>epsilon)
            {
                SETFLAG(STRUCTUREONMYSURFACE);
            }
            else
            {
                CLEARFLAG(LOCKED);
            };
            if ((*this)(x,0,z)>epsilon)
            {
                SETFLAG(STRUCTUREONMYSURFACE);
            }
            else
            {
                CLEARFLAG(LOCKED);
            };
        };



    return 0;
};





// evaluate graph structure

int
Compartment::
AmIASingleOrDoIHaveNeigbours()
{
  SETFLAG(SINGLE);
  list<Connection>::iterator pos;
  
  for (pos=ConnectionList.begin();pos!=ConnectionList.end();++pos)
    {
      if ( ((*pos).kind>=N_TopFront) && ((*pos).kind<=N_Bottom))
        {
	  if ( (*pos).to->FLAG(STRUCTUREONMYSURFACE))
            {
	      CLEARFLAG(SINGLE);
	      return 0;
            }
        }
      

    }
  return 0;
}



/*!
* Checks if Compartment is a boundary compartment 6neigbourhood (27 would detect to much boundarie)!
*/
int
Compartment::
AmIABoundaryComp()
{
  //cout << " Lets see if it is a boundary node!" << endl;
  CLEARFLAG(BOUNDARYCOMP);
  nrOfNeigbours=6;
  if (GetConnection(N_Top)==NULL) {SETFLAG(BOUNDARYCOMP);nrOfNeigbours--;};
  if (GetConnection(N_Right)==NULL) {SETFLAG(BOUNDARYCOMP);nrOfNeigbours--;};
  if (GetConnection(N_Back)==NULL) {SETFLAG(BOUNDARYCOMP);nrOfNeigbours--;};
  if (GetConnection(N_Left)==NULL) {SETFLAG(BOUNDARYCOMP);nrOfNeigbours--;};
  if (GetConnection(N_Front)==NULL) {SETFLAG(BOUNDARYCOMP);nrOfNeigbours--;};
  if (GetConnection(N_Bottom)==NULL) {SETFLAG(BOUNDARYCOMP);nrOfNeigbours--;};
  return 0;
}

int
Compartment::
AmIABoundaryComp(short neigbourhoodType)
{
    if (neigbourhoodType==6)
    {
    CLEARFLAG(BOUNDARYCOMP);
    nrOfNeigbours=6;
    if (GetConnection(N_Top)==NULL) {SETFLAG(BOUNDARYCOMP);nrOfNeigbours--;};
    if (GetConnection(N_Right)==NULL) {SETFLAG(BOUNDARYCOMP);nrOfNeigbours--;};
    if (GetConnection(N_Back)==NULL) {SETFLAG(BOUNDARYCOMP);nrOfNeigbours--;};
    if (GetConnection(N_Left)==NULL) {SETFLAG(BOUNDARYCOMP);nrOfNeigbours--;};
    if (GetConnection(N_Front)==NULL) {SETFLAG(BOUNDARYCOMP);nrOfNeigbours--;};
    if (GetConnection(N_Bottom)==NULL) {SETFLAG(BOUNDARYCOMP);nrOfNeigbours--;};
    return 0;
    };

    if (neigbourhoodType==18)
    {
        CLEARFLAG(BOUNDARYCOMP);
        nrOfNeigbours=18;
        //faces
        if (GetConnection(N_Top)==NULL) {SETFLAG(BOUNDARYCOMP);nrOfNeigbours--;};
        if (GetConnection(N_Right)==NULL) {SETFLAG(BOUNDARYCOMP);nrOfNeigbours--;};
        if (GetConnection(N_Back)==NULL) {SETFLAG(BOUNDARYCOMP);nrOfNeigbours--;};
        if (GetConnection(N_Left)==NULL) {SETFLAG(BOUNDARYCOMP);nrOfNeigbours--;};
        if (GetConnection(N_Front)==NULL) {SETFLAG(BOUNDARYCOMP);nrOfNeigbours--;};
        if (GetConnection(N_Bottom)==NULL) {SETFLAG(BOUNDARYCOMP);nrOfNeigbours--;};
        //sides
        //top
        if (GetConnection(N_TopFront)==NULL) {SETFLAG(BOUNDARYCOMP);nrOfNeigbours--;};
        if (GetConnection(N_TopBack)==NULL) {SETFLAG(BOUNDARYCOMP);nrOfNeigbours--;};
        if (GetConnection( N_TopRight)==NULL) {SETFLAG(BOUNDARYCOMP);nrOfNeigbours--;};
        if (GetConnection( N_TopLeft)==NULL) {SETFLAG(BOUNDARYCOMP);nrOfNeigbours--;};

        //same
        if (GetConnection(N_FrontRight)==NULL) {SETFLAG(BOUNDARYCOMP);nrOfNeigbours--;};
        if (GetConnection(N_FrontLeft)==NULL) {SETFLAG(BOUNDARYCOMP);nrOfNeigbours--;};
        if (GetConnection(N_BackLeft)==NULL) {SETFLAG(BOUNDARYCOMP);nrOfNeigbours--;};
        if (GetConnection(N_BackRight)==NULL) {SETFLAG(BOUNDARYCOMP);nrOfNeigbours--;};

        //bottom
        if (GetConnection(N_BottomRight)==NULL) {SETFLAG(BOUNDARYCOMP);nrOfNeigbours--;};
        if (GetConnection(N_BottomBack)==NULL) {SETFLAG(BOUNDARYCOMP);nrOfNeigbours--;};
        if (GetConnection(N_BottomLeft)==NULL) {SETFLAG(BOUNDARYCOMP);nrOfNeigbours--;};
        if (GetConnection(N_BottomFront)==NULL) {SETFLAG(BOUNDARYCOMP);nrOfNeigbours--;};	

        
        return 0;
    }
    
    
    if (neigbourhoodType==26)
    {
        CLEARFLAG(BOUNDARYCOMP);
        nrOfNeigbours=6;
        //faces
        if (GetConnection(N_Top)==NULL) {SETFLAG(BOUNDARYCOMP);nrOfNeigbours--;};
        if (GetConnection(N_Right)==NULL) {SETFLAG(BOUNDARYCOMP);nrOfNeigbours--;};
        if (GetConnection(N_Back)==NULL) {SETFLAG(BOUNDARYCOMP);nrOfNeigbours--;};
        if (GetConnection(N_Left)==NULL) {SETFLAG(BOUNDARYCOMP);nrOfNeigbours--;};
        if (GetConnection(N_Front)==NULL) {SETFLAG(BOUNDARYCOMP);nrOfNeigbours--;};
        if (GetConnection(N_Bottom)==NULL) {SETFLAG(BOUNDARYCOMP);nrOfNeigbours--;};
        //sides
        //top
        if (GetConnection(N_TopFront)==NULL) {SETFLAG(BOUNDARYCOMP);nrOfNeigbours--;};
        if (GetConnection(N_TopBack)==NULL) {SETFLAG(BOUNDARYCOMP);nrOfNeigbours--;};
        if (GetConnection( N_TopRight)==NULL) {SETFLAG(BOUNDARYCOMP);nrOfNeigbours--;};
        if (GetConnection( N_TopLeft)==NULL) {SETFLAG(BOUNDARYCOMP);nrOfNeigbours--;};

        //same
        if (GetConnection(N_FrontRight)==NULL) {SETFLAG(BOUNDARYCOMP);nrOfNeigbours--;};
        if (GetConnection(N_FrontLeft)==NULL) {SETFLAG(BOUNDARYCOMP);nrOfNeigbours--;};
        if (GetConnection(N_BackLeft)==NULL) {SETFLAG(BOUNDARYCOMP);nrOfNeigbours--;};
        if (GetConnection(N_BackRight)==NULL) {SETFLAG(BOUNDARYCOMP);nrOfNeigbours--;};

        //bottom
        if (GetConnection(N_BottomRight)==NULL) {SETFLAG(BOUNDARYCOMP);nrOfNeigbours--;};
        if (GetConnection(N_BottomBack)==NULL) {SETFLAG(BOUNDARYCOMP);nrOfNeigbours--;};
        if (GetConnection(N_BottomLeft)==NULL) {SETFLAG(BOUNDARYCOMP);nrOfNeigbours--;};
        if (GetConnection(N_BottomFront)==NULL) {SETFLAG(BOUNDARYCOMP);nrOfNeigbours--;};

        //points
        //top
        if (GetConnection(N_TopFrontRight)==NULL) {SETFLAG(BOUNDARYCOMP);nrOfNeigbours--;};
        if (GetConnection(N_TopFrontLeft)==NULL) {SETFLAG(BOUNDARYCOMP);nrOfNeigbours--;};
        if (GetConnection(N_TopBackLeft)==NULL) {SETFLAG(BOUNDARYCOMP);nrOfNeigbours--;};
        if (GetConnection(N_TopBackRight)==NULL) {SETFLAG(BOUNDARYCOMP);nrOfNeigbours--;};

        //bottom
        if (GetConnection(N_BottomFrontRight)==NULL) {SETFLAG(BOUNDARYCOMP);nrOfNeigbours--;};
        if (GetConnection(N_BottomFrontLeft)==NULL) {SETFLAG(BOUNDARYCOMP);nrOfNeigbours--;};
        if (GetConnection(N_BottomBackLeft)==NULL) {SETFLAG(BOUNDARYCOMP);nrOfNeigbours--;};
        if (GetConnection(N_BottomBackRight)==NULL) {SETFLAG(BOUNDARYCOMP);nrOfNeigbours--;};        
        return 0;
    }

    return 1;
}






int
Compartment::
isTheDistanceFromASecondOrderNeigbourBigger()
{
    short kind1, kind2;
    Compartment *secondOrderNeigbour;
    for (kind1=N_TopFront;kind1<=N_Bottom;kind1++)
        for (kind2=N_TopFront;kind2<=N_Bottom;kind2++)
            secondOrderNeigbour=NeigbourOfNeigbour(kind1,kind2);
            if (secondOrderNeigbour!=NULL)
            {
                int distSecondOrder;
                distSecondOrder=secondOrderNeigbour->GetDistanceFromBody();
                if (distSecondOrder>distanceFromBody) return 1;
            };

    
    return 0;
}


int
Compartment::
isTheDistanceFromAThirdNeigbourBigger()
{
    short kind1, kind2, kind3;
    Compartment *thirdOrderNeigbour;
    for (kind1=N_TopFront;kind1<=N_Bottom;kind1++)
        for (kind2=N_TopFront;kind2<=N_Bottom;kind2++)
        	for (kind3=N_TopFront;kind3<=N_Bottom;kind3++)
            thirdOrderNeigbour=NeigbourOfNeigbourOfNeigbour(kind1,kind2,kind3);
            if (thirdOrderNeigbour!=NULL)
            {
                int distThirdOrder;
                distThirdOrder=thirdOrderNeigbour->GetDistanceFromBody();
                if (distThirdOrder>distanceFromBody) return 1;
            };

    
    return 0;
}




//Navigate through graph strucure

/*!
* Moves throug the compartment list, accoring to the distances from cellbody, watch for Macro
*/
#define CHECKANSSETIF(NN) if (present->GetConnection(NN)!=NULL) if ((present->GetConnection(NN))->GetDistanceFromBody()<dist) {dist=(present->GetConnection(NN))->GetDistanceFromBody();next=(present->GetConnection(NN));};



int
Compartment::
MarchToCalcNrOfPasingThrouhAndDetectBifurations(Compartment *bodyComp)
{
    int steps;
    Compartment *present, *next;
    int dist=MAXARRIVALTIME;
    steps=0;
    for(present=this;(present!=bodyComp & steps<MAXARRIVALTIME);present=next)
    {
        next=NULL;
        steps++;
        dist=present->GetDistanceFromBody();
        int kind;
        for (kind=N_TopFront;kind<=N_Bottom;kind++)
            CHECKANSSETIF(kind);
        if (next==NULL) return 1;
        if ((next->GetNrOfPassingThrough())>=(present->GetNrOfPassingThrough()))
        {
            next->SetNrOfLinkConnections(next->GetNrOfLinkConnections()+1); 
            next->SETFLAG(BIFURCATION);
        };
    }


    if (steps<MAXARRIVALTIME)
    {
        bodyComp->SetNrOfLinkConnections(bodyComp->GetNrOfLinkConnections()+1); 
        bodyComp->SETFLAG(BIFURCATION);
        return 0;
    }
    return 1;
}





int
Compartment::
MarchToAndAddPassingThroughs(Compartment *bodyComp)
{
  int steps;
  Compartment *present, *next;
  int dist=MAXARRIVALTIME;
  steps=0;
  for(present=this;(present!=bodyComp & steps<MAXARRIVALTIME);present=next)
    {
      next=NULL;
      steps++;
      dist=present->GetDistanceFromBody();
      int kind;
      for (kind=N_TopFront;kind<=N_Bottom;kind++)
	CHECKANSSETIF(kind);
      if (next==NULL) return 1;
      if ((next->GetNrOfPassingThrough())>=(present->GetNrOfPassingThrough())) next->SETFLAG(BIFURCATION);
      next->addPassingThrough(this);
    }
  
  
  if (steps<MAXARRIVALTIME)
    {
      bodyComp->addPassingThrough(this);
      bodyComp->SETFLAG(BIFURCATION);
      return 0;
    }
  return 1;
}


Compartment*
Compartment::
MarchtToNextAndMeasureDistance(int searchFlag, int &distance)
{
    distance=0;
    int steps;
    Compartment *present, *next;
    int dist=MAXARRIVALTIME;
    steps=0;
    for(present=this;( ( (steps==0) | !(present->FLAG(searchFlag))) & steps<MAXARRIVALTIME);present=next)
    {
        next=NULL;
        steps++;
        dist=present->GetDistanceFromBody();
        int kind;
        for (kind=N_TopFront;kind<=N_Bottom;kind++)
            CHECKANSSETIF(kind);
        if (next==NULL) return NULL;
        distance+=(present->GetDistanceFromBody())-(next->GetDistanceFromBody());
    };


    if (steps<MAXARRIVALTIME)
    {
        return present;
    }

    return NULL;
}

Compartment*
Compartment::
MarchToNext(int searchFlag,list<Compartment*> &geometricComps)
{
  int steps;
  Compartment *present, *next;
  int dist=MAXARRIVALTIME;
  steps=0;
  for(present=this;( ( (steps==0) | !(present->FLAG(searchFlag))) & steps<MAXARRIVALTIME);present=next)
    {
      next=NULL;
      steps++;
      dist=present->GetDistanceFromBody();
      int kind;
      for (kind=N_TopFront;kind<=N_Bottom;kind++)
	CHECKANSSETIF(kind);
      if (next==NULL) return NULL;
      geometricComps.push_back(next);
    }
  
  
  if (steps<MAXARRIVALTIME)
    {
      return present;
    }
  
  return NULL;
}


int
Compartment::
MarchTo6(Compartment *bodyComp)
{
  int steps;
  Compartment *present, *next;
  int dist=MAXARRIVALTIME;
  steps=0;
  for(present=this;(present!=bodyComp & steps<MAXARRIVALTIME);present=next)
    {
      next=NULL;
      steps++;
      dist=present->GetDistanceFromBody();
      CHECKANSSETIF(N_Top);
      CHECKANSSETIF(N_Right);
      CHECKANSSETIF(N_Back);
      CHECKANSSETIF(N_Left);
      CHECKANSSETIF(N_Front);
      CHECKANSSETIF(N_Bottom);
      if (next==NULL) return 1;
      if ((next->GetNrOfPassingThrough())>=(present->GetNrOfPassingThrough())) next->SETFLAG(BIFURCATION);
      next->addPassingThrough(this);
    }
  

  if (steps<MAXARRIVALTIME)
    {
      bodyComp->addPassingThrough(this);
      bodyComp->SETFLAG(BIFURCATION);
      return 0;
    }
  return 1;
}


Compartment*
Compartment::
MarchToNext6(int searchFlag,list<Compartment*> &geometricComps)
{
  int steps;
  Compartment *present, *next;
  int dist=MAXARRIVALTIME;
  steps=0;
  for(present=this;( ( (steps==0) | !(present->FLAG(searchFlag))) & steps<MAXARRIVALTIME);present=next)
    {
      next=NULL;
      steps++;
      dist=present->GetDistanceFromBody();
      CHECKANSSETIF(N_Top);
      CHECKANSSETIF(N_Right);
      CHECKANSSETIF(N_Back);
      CHECKANSSETIF(N_Left);
      CHECKANSSETIF(N_Front);
      CHECKANSSETIF(N_Bottom);
      if (next==NULL) return NULL;
      geometricComps.push_back(next);
    }
  
  
  if (steps<MAXARRIVALTIME)
    {
      return present;
    }
  
  return NULL;
}

int
Compartment::
addPassingThrough(Compartment *comp)
{
  pasingThrouh.push_back(comp);
  nrPasingThrouh++;
  return 0;
}





//Auxillary functions
int
Compartment::
IsPointInCompartment(int gx,int gy,int gz)
{
  gx-=start[1];
  gy-=start[2];
  gz-=start[3];
  return ((gx>0 & gx<=size[1]) & (gy>0 & gy<=size[2]) &(gz>0 & gz<=size[3]));
  
}

int
Compartment::
IsPointInCompartment(Vector<int> position)
{
  Vector<int> help=start;
  help.Multiply(-1);
  Vector<int> pos=position+help;
  return ((pos[1]>=0 && pos[1]<size[1]) && (pos[2]>=0 && pos[2]<size[2]) && (pos[3]>=0 && pos[3]<size[3]));
}




/*!
* measures the radius of the structure
 * method: 1) a ball is blown up until it hits the frontier of the structure
 *         2) starting from a big radius a ball is decreased until it lays completly in the structure  
 */

double
Compartment::
CalculateRadius(double threshold, int choice)
{
    switch(choice)
    {
	case 1: return BigestBallAroundBeginningWithSmallBall(threshold);
	case 2: return BigestBallAroundBeginningWithHugeBall(threshold);
	default: {
	    cerr<<"Wrong parameter, calculate radius with method (1)";
	    return BigestBallAroundBeginningWithSmallBall(threshold);
	}
    }
    
}

double
Compartment::
BigestBallAroundBeginningWithSmallBall(double threshold)
{
    double maxSize=Norm(dataCube->GetSize());
    
    
    int midX=start[1]+size[1]/2;
    int midY=start[2]+size[2]/2;
    int midZ=start[3]+size[3]/2;
    int dx,dy,dz;
    double dist=maxSize;
    double radiushelp=0.0, disthelp=0.0;
    
    // calculation of the minRadius - needed if the dentrite consists only of one pixel
    Vector<double> spacing(3);
    spacing = dataCube->GetSpaceing();
    double minRadius;
    if (spacing[1]<=spacing[2])
	minRadius=spacing[1]/2.0;
    else minRadius=spacing[2]/2.0;
    if (spacing[3]/2.0<minRadius)
	minRadius=spacing[3]/2.0;
    
    
    //cout<<dataCube->GetData(midX,midY,midZ)<<"    ";
    //cout<<"Threshold    "<<threshold;
    for(int testSize=0;testSize<maxSize;testSize++)
    {
        for(dx=-testSize;dx<=testSize;dx++)
	    for(dy=-testSize;dy<=testSize;dy++)
		for(dz=-testSize;dz<=testSize;dz++)
		    if ((dataCube->DCNorm(dx,dy,dz)*dataCube->DCNorm(dx,dy,dz))<=testSize*testSize)
		    {
			if (dataCube->GetData(midX+dx,midY+dy,midZ+dz)>=threshold)
			{
			    if(dataCube->DCNorm(dx,dy,dz)>=disthelp)
				if(dataCube->DCNorm(dx,dy,dz)<0.0000000001)
				    disthelp=minRadius;
				else 
				    disthelp=dataCube->DCNorm(dx,dy,dz);
			}
			else
			{
			    return radiushelp;
			}
		    }
		radiushelp=disthelp;
    }
}




double
Compartment::
BigestBallAroundBeginningWithHugeBall(double threshold)
{
    double maxSize=Norm(dataCube->GetSize());
    
    int midX=start[1]+size[1]/2;
    int midY=start[2]+size[2]/2;
    int midZ=start[3]+size[3]/2;
    int dx,dy,dz;
    int tooBig;
    double dist=maxSize;
    double disthelp=0.0;
    int testSize, startSize;
    
    for(int testSize=0;testSize<maxSize;testSize++)
    {
	if (dataCube->GetData(midX+testSize,midY,midZ)<threshold)
	{
	    startSize=(int)dataCube->DCNorm(testSize,0,0);
	    break;
	}
    }
    
    for(int testSize=0;testSize<maxSize;testSize++)
    {
	if (dataCube->GetData(midX,midY+testSize,midZ)<threshold)
	{
	    if(startSize>dataCube->DCNorm(0,testSize,0))
		startSize=(int)dataCube->DCNorm(0,testSize,0);
	    break;
	}
    }
    
    for(int testSize=0;testSize<maxSize;testSize++)
    {
	if (dataCube->GetData(midX,midY,midZ+testSize)<threshold)
	{
	    if(startSize>dataCube->DCNorm(0,0,testSize))
		startSize=(int)dataCube->DCNorm(0,0,testSize);
	    break;
	}
    }
    startSize++;
    
    
    for(testSize=startSize;testSize>=0;testSize--)
    {
	disthelp=0;
	tooBig=0;
	for(dx=-testSize;dx<=testSize;dx++)
	{
	    for(dy=-testSize;dy<=testSize;dy++)
	    {
		for(dz=-testSize;dz<=testSize;dz++)
		{
		    if ((dataCube->DCNorm(dx,dy,dz)*dataCube->DCNorm(dx,dy,dz))<=testSize*testSize)
		    {
			if (dataCube->GetData(midX+dx,midY+dy,midZ+dz)>=threshold)
			    if(dataCube->DCNorm(dx,dy,dz)>disthelp)
				disthelp=dataCube->DCNorm(dx,dy,dz);
			
			if (dataCube->GetData(midX+dx,midY+dy,midZ+dz)<threshold)
			{
			    tooBig=1;
			    break;
			}
		    }
		}
		if(tooBig==1)
		    break;
	    }
	    if(tooBig==1)
		break;
	}
	if(tooBig!=1)
	    return (disthelp);
	
	
    }
}






















