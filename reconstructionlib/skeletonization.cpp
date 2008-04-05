#include "skeletonization.h"


Skeletonization::
Skeletonization()
{
    //create primitve list; these primitive are necessary to keep topologie
    Primitive *prim;
    for (int i=1;i<=MAXNROFPRIM6;i++)
    {
        //if (i==13) continue; //-3,-4,-5,-6,-2,-1,0,-7,-9,-10
        //if (i==10) continue;
        for (short axis1=1;axis1<=3;axis1++)
            for (short angle1=0;angle1<=270;angle1=angle1+90)
                for (short axis2=1;axis2<=3;axis2++)
                {
                    if (axis2==axis1) continue;
                    for (short angle2=0;angle2<=270;angle2=angle2+90)
                        for (short axis3=1;axis3<=3;axis3++)
                        {
                            if (axis3==axis2) continue;
                            for (short angle3=0;angle3<=270;angle3=angle3+90)


                                for (short plane=0;plane<=3;plane++)
                                {
                                    prim=new Primitive(i);
                                    prim->Mirror(plane);
                                    prim->Rotate(axis1,angle1);
                                    prim->Rotate(axis2,angle2);
                                    prim->Rotate(axis3,angle3);
                                    PrimitveTemplates.push_front(prim);
                                }
                        }
                }

    }
//delete redundant templates

list<Primitive*>::iterator it1, it2;
int counter=0;
for (it1=PrimitveTemplates.begin();it1!=PrimitveTemplates.end();++it1)
    for (it2=PrimitveTemplates.begin();it2!=PrimitveTemplates.end();++it2)  
    	if ( it1!=it2 ) 
    	  		if (TheyAreTheSame(**it1,**it2))
    	  		{
    	  		counter++;
    	  		delete *it2;
    	  		it2=PrimitveTemplates.erase(it2);
    	  		    	  		
    	  		}		

	
};

Skeletonization::
Skeletonization(int kindofneigbourhood)
{
    //create primitve list; these primitive are necessary to keep topologie
    Primitive *prim;
    int MAXNROFPRIM;
    if (kindofneigbourhood==6) MAXNROFPRIM=MAXNROFPRIM6;
    if (kindofneigbourhood==26) MAXNROFPRIM=MAXNROFPRIM26;
    
    for (int i=1;i<=MAXNROFPRIM;i++)
    {
        //if (i==13) continue; //-3,-4,-5,-6,-2,-1,0,-7,-9,-10
        //if (i==10) continue;
        for (short axis1=1;axis1<=3;axis1++)
            for (short angle1=0;angle1<=270;angle1=angle1+90)
                for (short axis2=1;axis2<=3;axis2++)
                {
                   if (axis2==axis1) continue;
                    for (short angle2=0;angle2<=270;angle2=angle2+90)
                        for (short axis3=1;axis3<=3;axis3++)
                        {
                            if (axis3==axis2) continue;
                            for (short angle3=0;angle3<=270;angle3=angle3+90)


                                for (short plane=0;plane<=3;plane++)
                                {
                                    prim=new Primitive(i,kindofneigbourhood);
                                    prim->Mirror(plane);
                                    prim->Rotate(axis1,angle1);
                                    prim->Rotate(axis2,angle2);
                                    prim->Rotate(axis3,angle3);
                                    PrimitveTemplates.push_front(prim);
                                }
                        }
                }

    }
//delete redundant templates
list<Primitive*>::iterator it1, it2;
int counter=0;
for (it1=PrimitveTemplates.begin();it1!=PrimitveTemplates.end();++it1)
    for (it2=PrimitveTemplates.begin();it2!=PrimitveTemplates.end();++it2)  
    	if ( it1!=it2 ) 
    	  		if (TheyAreTheSame(**it1,**it2))
    	  		{
    	  		counter++;
    	  		delete *it2;
    	  		it2=PrimitveTemplates.erase(it2);
    	  		    	  		
    	  		}		

                    
};
Skeletonization::
~Skeletonization()
{
    list<Primitive*>::iterator pos;
    for (pos=PrimitveTemplates.begin();pos!=PrimitveTemplates.end();++pos)
        delete (*pos);
    
};

int 
Skeletonization::
writeOnom2k(char *templateName)
{
char buffer[90];
Primitive *prim;
for (int i=1;i<=MAXNROFPRIM26;i++)
{
	sprintf(buffer,templateName,i);
	prim=new Primitive(i,26);
	prim->writeOnom2K(buffer);
	delete prim;
}

return 0;
};


Primitive*
Skeletonization::
CompartmentToPrimitive26(Compartment *comp)
{
    unsigned char d[3][3][3]= {
    {{BACKGROUND,BACKGROUND,BACKGROUND},{BACKGROUND,BACKGROUND,BACKGROUND},{BACKGROUND,BACKGROUND,BACKGROUND}},
    {{BACKGROUND,BACKGROUND,BACKGROUND},{BACKGROUND,BACKGROUND,BACKGROUND},{BACKGROUND,BACKGROUND,BACKGROUND}},
    {{BACKGROUND,BACKGROUND,BACKGROUND},{BACKGROUND,BACKGROUND,BACKGROUND},{BACKGROUND,BACKGROUND,BACKGROUND}}
    };
    /*
     for (int x=0;x<3;x++)
     for(int y=0;y<3;y++)
     for(int z=0;z<3;z++)
     d[x][y][z]=BACKGROUND;

     */


    //botton
    if (comp->Neigbour(N_BottomBackLeft)!=NULL) d[XLeft][YBack][ZBottom]= FOREGROUND;
    if (comp->Neigbour(N_BottomBack)!=NULL) d[XMiddle][YBack][ZBottom]= FOREGROUND;
    if (comp->Neigbour(N_BottomBackRight)!=NULL) d[XRight][YBack][ZBottom]= FOREGROUND;

    if (comp->Neigbour(N_BottomLeft)!=NULL) d[XLeft][YMiddle][ZBottom]= FOREGROUND;
    if (comp->Neigbour(N_Bottom)!=NULL)            d[XMiddle][YMiddle][ZBottom]= FOREGROUND;
    if (comp->Neigbour(N_BottomRight)!=NULL) d[XRight][YMiddle][ZBottom]= FOREGROUND;

    if (comp->Neigbour(N_BottomFrontLeft)!=NULL) d[XLeft][YFront][ZBottom]= FOREGROUND;
    if (comp->Neigbour(N_BottomFront)!=NULL) d[XMiddle][YFront][ZBottom]= FOREGROUND;
    if (comp->Neigbour(N_BottomFrontRight)!=NULL) d[XRight][YFront][ZBottom]= FOREGROUND;


    //middle
    if (comp->Neigbour(N_BackLeft)!=NULL) d[XLeft][YBack][ZMiddle]= FOREGROUND;
    if (comp->Neigbour(N_Back)!=NULL) d[XMiddle][YBack][ZMiddle]= FOREGROUND;
    if (comp->Neigbour(N_BackRight)!=NULL) d[XRight][YBack][ZMiddle]= FOREGROUND;

    if (comp->Neigbour(N_Left)!=NULL) d[XLeft][YMiddle][ZMiddle]= FOREGROUND;
    d[XMiddle][YMiddle][ZMiddle]= FOREGROUND;
    if (comp->Neigbour(N_Right)!=NULL) d[XRight][YMiddle][ZMiddle]= FOREGROUND;

    if (comp->Neigbour(N_FrontLeft)!=NULL) d[XLeft][YFront][ZMiddle]= FOREGROUND;
    if (comp->Neigbour(N_Front)!=NULL) d[XMiddle][YFront][ZMiddle]= FOREGROUND;
    if (comp->Neigbour(N_FrontRight)!=NULL) d[XRight][YFront][ZMiddle]= FOREGROUND;


    //top
    if (comp->Neigbour(N_TopBackLeft)!=NULL) d[XLeft][YBack][ZTop]= FOREGROUND;
    if (comp->Neigbour(N_TopBack)!=NULL) d[XMiddle][YBack][ZTop]= FOREGROUND;
    if (comp->Neigbour(N_TopBackRight)!=NULL) d[XRight][YBack][ZTop]= FOREGROUND;

    if (comp->Neigbour(N_TopLeft)!=NULL) d[XLeft][YMiddle][ZTop]= FOREGROUND;
    if (comp->Neigbour(N_Top)!=NULL)            d[XMiddle][YMiddle][ZTop]= FOREGROUND;
    if (comp->Neigbour(N_TopRight)!=NULL) d[XRight][YMiddle][ZTop]= FOREGROUND;

    if (comp->Neigbour(N_TopFrontLeft)!=NULL) d[XLeft][YFront][ZTop]= FOREGROUND;
    if (comp->Neigbour(N_TopFront)!=NULL) d[XMiddle][YFront][ZTop]= FOREGROUND;
    if (comp->Neigbour(N_TopFrontRight)!=NULL) d[XRight][YFront][ZTop]= FOREGROUND;

    return new Primitive(d);

}



Primitive* 
Skeletonization::
CompartmentToPrimitive(Compartment *comp)
{
    unsigned char d[3][3][3]= {
    {{BACKGROUND,BACKGROUND,BACKGROUND},{BACKGROUND,BACKGROUND,BACKGROUND},{BACKGROUND,BACKGROUND,BACKGROUND}},
    {{BACKGROUND,BACKGROUND,BACKGROUND},{BACKGROUND,BACKGROUND,BACKGROUND},{BACKGROUND,BACKGROUND,BACKGROUND}},
    {{BACKGROUND,BACKGROUND,BACKGROUND},{BACKGROUND,BACKGROUND,BACKGROUND},{BACKGROUND,BACKGROUND,BACKGROUND}}
    };
/*
    for (int x=0;x<3;x++)
	for(int y=0;y<3;y++)
		for(int z=0;z<3;z++)
			d[x][y][z]=BACKGROUND;
			
*/
    

//botton			
if (comp->NeigbourOfNeigbourOfNeigbour(N_Bottom,N_Back,N_Left)!=NULL) d[XLeft][YBack][ZBottom]= FOREGROUND;
if (comp->NeigbourOfNeigbour(N_Bottom,N_Back)!=NULL) d[XMiddle][YBack][ZBottom]= FOREGROUND;
if (comp->NeigbourOfNeigbourOfNeigbour(N_Bottom,N_Back,N_Right)!=NULL) d[XRight][YBack][ZBottom]= FOREGROUND;

if (comp->NeigbourOfNeigbour(N_Bottom,N_Left)!=NULL) d[XLeft][YMiddle][ZBottom]= FOREGROUND;
if (comp->Neigbour(N_Bottom)!=NULL)            d[XMiddle][YMiddle][ZBottom]= FOREGROUND;
if (comp->NeigbourOfNeigbour(N_Bottom,N_Right)!=NULL) d[XRight][YMiddle][ZBottom]= FOREGROUND;

if (comp->NeigbourOfNeigbourOfNeigbour(N_Bottom,N_Front,N_Left)!=NULL) d[XLeft][YFront][ZBottom]= FOREGROUND;
if (comp->NeigbourOfNeigbour(N_Bottom,N_Front)!=NULL) d[XMiddle][YFront][ZBottom]= FOREGROUND;
if (comp->NeigbourOfNeigbourOfNeigbour(N_Bottom,N_Front,N_Right)!=NULL) d[XRight][YFront][ZBottom]= FOREGROUND;


//middle
if (comp->NeigbourOfNeigbour(N_Left,N_Back)!=NULL) d[XLeft][YBack][ZMiddle]= FOREGROUND;
if (comp->Neigbour(N_Back)!=NULL) d[XMiddle][YBack][ZMiddle]= FOREGROUND;
if (comp->NeigbourOfNeigbour(N_Right,N_Back)!=NULL) d[XRight][YBack][ZMiddle]= FOREGROUND;

if (comp->Neigbour(N_Left)!=NULL) d[XLeft][YMiddle][ZMiddle]= FOREGROUND;
d[XMiddle][YMiddle][ZMiddle]= FOREGROUND;
if (comp->Neigbour(N_Right)!=NULL) d[XRight][YMiddle][ZMiddle]= FOREGROUND;

if (comp->NeigbourOfNeigbour(N_Left,N_Front)!=NULL) d[XLeft][YFront][ZMiddle]= FOREGROUND;
if (comp->Neigbour(N_Front)!=NULL) d[XMiddle][YFront][ZMiddle]= FOREGROUND;
if (comp->NeigbourOfNeigbour(N_Right,N_Front)!=NULL) d[XRight][YFront][ZMiddle]= FOREGROUND;


//top
if (comp->NeigbourOfNeigbourOfNeigbour(N_Left,N_Back,N_Top)!=NULL) d[XLeft][YBack][ZTop]= FOREGROUND;
if (comp->NeigbourOfNeigbour(N_Back,N_Top)!=NULL) d[XMiddle][YBack][ZTop]= FOREGROUND;
if (comp->NeigbourOfNeigbourOfNeigbour(N_Right,N_Back,N_Top)!=NULL) d[XRight][YBack][ZTop]= FOREGROUND;

if (comp->NeigbourOfNeigbour(N_Left,N_Top)!=NULL) d[XLeft][YMiddle][ZTop]= FOREGROUND;
if (comp->Neigbour(N_Top)!=NULL)            d[XMiddle][YMiddle][ZTop]= FOREGROUND;
if (comp->NeigbourOfNeigbour(N_Right,N_Top)!=NULL) d[XRight][YMiddle][ZTop]= FOREGROUND;

if (comp->NeigbourOfNeigbourOfNeigbour(N_Left,N_Front,N_Top)!=NULL) d[XLeft][YFront][ZTop]= FOREGROUND;
if (comp->NeigbourOfNeigbour(N_Front,N_Top)!=NULL) d[XMiddle][YFront][ZTop]= FOREGROUND;
if (comp->NeigbourOfNeigbourOfNeigbour(N_Right,N_Front,N_Top)!=NULL) d[XRight][YFront][ZTop]= FOREGROUND;

return new Primitive(d);

}

int 
Skeletonization::
IsPrimitivetDeleteable(Primitive *prim)
{
list<Primitive*>::iterator pos;

for (pos=PrimitveTemplates.begin();pos!=PrimitveTemplates.end();++pos)
    if ( (**pos)==(*prim) ) break;

//if the Primitive does not belong to the primitive template list, topologi will not be changed if deleted
 if (pos!=PrimitveTemplates.end()) return 1;
  
 return 0;
}


int 
Skeletonization::
IsCompartmentDeleteable(Compartment *comp)
{
Primitive *prim;
prim=CompartmentToPrimitive(comp);

list<Primitive*>::iterator pos;

for (pos=PrimitveTemplates.begin();pos!=PrimitveTemplates.end();++pos)
    if ( (**pos)==(*prim) ) break;
//if the Primitive does not belong to the primitive template list, topologi will not be changed if deleted   
    
 if (pos!=PrimitveTemplates.end())
 {
     delete prim;
     return 1;
 };

 delete prim;	
 return 0;
};



int
Skeletonization::
IsCompartmentDeleteable26(Compartment *comp)
{
    Primitive *prim;
    prim=CompartmentToPrimitive26(comp);

    list<Primitive*>::iterator pos;

    for (pos=PrimitveTemplates.begin();pos!=PrimitveTemplates.end();++pos)
        if ( (**pos)==(*prim) ) break;
    //if the Primitive does not belong to the primitive template list, topologi will not be changed if deleted

    if (pos!=PrimitveTemplates.end())
    {
        delete prim;
        return 1;
    };

    delete prim;
    return 0;
}


int
Skeletonization::
CompartmentIsDeleteableBy26(Compartment *comp)
{
    Primitive *prim;
    prim=CompartmentToPrimitive26(comp);

    list<Primitive*>::iterator pos;

    for (pos=PrimitveTemplates.begin();pos!=PrimitveTemplates.end();++pos)
        if ( (**pos)==(*prim) ) break;
    //if the Primitive does not belong to the primitive template list, topologi will not be changed if deleted

    if (pos!=PrimitveTemplates.end())
    {
        short nr=(*pos)->GetDerivationNr() ;
        delete prim;
        return nr;
    };

    delete prim;
    return 0;

};




    

