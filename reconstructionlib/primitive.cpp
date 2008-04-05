#include "primitive.h"




//constructor
Primitive::
Primitive(int nr)
{
    derivatedFrom=nr;
for (short x=0;x<3;x++)
	for(short y=0;y<3;y++)
		for(short z=0;z<3;z++)
			data[x][y][z]=BACKGROUND;

Templates6(nr);
}


void
Primitive::
Templates6(int nr)
{
switch (nr) 
	{
	
	case 1:{
	data[ZBottom][YBack]  [XLeft]  = DONTCARE;// BACKGROUND;
	data[ZBottom][YBack]  [XMiddle]= DONTCARE;//BACKGROUND;
	data[ZBottom][YBack]  [XRight] = DONTCARE;// BACKGROUND;
	data[ZBottom][YMiddle][XLeft]  = FOREGROUND;
	data[ZBottom][YMiddle][XMiddle]= FOREGROUND;
	data[ZBottom][YMiddle][XRight] = DONTCARE;// BACKGROUND;
	data[ZBottom][YFront] [XLeft]  = DONTCARE;// BACKGROUND;
	data[ZBottom][YFront] [XMiddle]= FOREGROUND;
	data[ZBottom][YFront] [XRight] = DONTCARE;// BACKGROUND;
	
	data[ZMiddle][YBack]  [XLeft]  = FOREGROUND;
	data[ZMiddle][YBack]  [XMiddle]= FOREGROUND;
	data[ZMiddle][YBack]  [XRight] = DONTCARE;// BACKGROUND;
	data[ZMiddle][YMiddle][XLeft]  = FOREGROUND;
	data[ZMiddle][YMiddle][XMiddle]= FOREGROUND;
	data[ZMiddle][YMiddle][XRight] = FOREGROUND;
	data[ZMiddle][YFront] [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZMiddle][YFront] [XMiddle]= BACKGROUND;
	data[ZMiddle][YFront] [XRight] = FOREGROUND;
	
	
	data[ZTop]   [YBack]  [XLeft]  = DONTCARE;// BACKGROUND;
	data[ZTop]   [YBack]  [XMiddle]= FOREGROUND;
	data[ZTop]   [YBack]  [XRight] = DONTCARE;// BACKGROUND;
	data[ZTop]   [YMiddle][XLeft]  = DONTCARE;// BACKGROUND;
	data[ZTop]   [YMiddle][XMiddle]= FOREGROUND;
	data[ZTop]   [YMiddle][XRight] = FOREGROUND;
	data[ZTop]   [YFront] [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZTop]   [YFront] [XMiddle]= DONTCARE;//BACKGROUND;
	data[ZTop]   [YFront] [XRight] = DONTCARE;//BACKGROUND;
	break;};
// FOREGROUND;//
// BACKGROUND;//
// DONTCARE;//   
	
	
	case 2:{
	data[ZBottom][YBack]  [XLeft]  = DONTCARE;// BACKGROUND;
	data[ZBottom][YBack]  [XMiddle]= DONTCARE;// BACKGROUND;
	data[ZBottom][YBack]  [XRight] = DONTCARE;//BACKGROUND;
	data[ZBottom][YMiddle][XLeft]  = FOREGROUND;
	data[ZBottom][YMiddle][XMiddle]= FOREGROUND;
	data[ZBottom][YMiddle][XRight] = DONTCARE;//BACKGROUND;
	data[ZBottom][YFront] [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZBottom][YFront] [XMiddle]= FOREGROUND;
	data[ZBottom][YFront] [XRight] = DONTCARE;//BACKGROUND;
	
	data[ZMiddle][YBack]  [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZMiddle][YBack]  [XMiddle]= FOREGROUND;
	data[ZMiddle][YBack]  [XRight] = FOREGROUND;
	data[ZMiddle][YMiddle][XLeft]  = FOREGROUND;
	data[ZMiddle][YMiddle][XMiddle]= FOREGROUND;
	data[ZMiddle][YMiddle][XRight] = FOREGROUND;
	data[ZMiddle][YFront] [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZMiddle][YFront] [XMiddle]= BACKGROUND;//onlyone that stays background
	data[ZMiddle][YFront] [XRight] = FOREGROUND;
	
	
	data[ZTop]   [YBack]  [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZTop]   [YBack]  [XMiddle]= FOREGROUND;
	data[ZTop]   [YBack]  [XRight] = DONTCARE;//BACKGROUND;
	data[ZTop]   [YMiddle][XLeft]  = FOREGROUND;
	data[ZTop]   [YMiddle][XMiddle]= FOREGROUND;
	data[ZTop]   [YMiddle][XRight] = DONTCARE;//BACKGROUND;
	data[ZTop]   [YFront] [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZTop]   [YFront] [XMiddle]= DONTCARE;//BACKGROUND;
	data[ZTop]   [YFront] [XRight] = DONTCARE;//BACKGROUND;
	break;};
	
	case 3:{
	data[ZBottom][YBack]  [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZBottom][YBack]  [XMiddle]= DONTCARE;//BACKGROUND;
	data[ZBottom][YBack]  [XRight] = DONTCARE;//BACKGROUND;
	data[ZBottom][YMiddle][XLeft]  = FOREGROUND;
	data[ZBottom][YMiddle][XMiddle]= FOREGROUND;
	data[ZBottom][YMiddle][XRight] = DONTCARE;//BACKGROUND;
	data[ZBottom][YFront] [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZBottom][YFront] [XMiddle]= FOREGROUND;
	data[ZBottom][YFront] [XRight] = DONTCARE;//BACKGROUND;
	
	data[ZMiddle][YBack]  [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZMiddle][YBack]  [XMiddle]= BACKGROUND;//stays background!!!
	data[ZMiddle][YBack]  [XRight] = DONTCARE;//BACKGROUND;
	data[ZMiddle][YMiddle][XLeft]  = FOREGROUND;
	data[ZMiddle][YMiddle][XMiddle]= FOREGROUND;
	data[ZMiddle][YMiddle][XRight] = FOREGROUND;
	data[ZMiddle][YFront] [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZMiddle][YFront] [XMiddle]= DONTCARE;//BACKGROUND;
	data[ZMiddle][YFront] [XRight] = FOREGROUND;
	
	
	data[ZTop]   [YBack]  [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZTop]   [YBack]  [XMiddle]= DONTCARE;//BACKGROUND;
	data[ZTop]   [YBack]  [XRight] = DONTCARE;//BACKGROUND;
	data[ZTop]   [YMiddle][XLeft]  = FOREGROUND;
	data[ZTop]   [YMiddle][XMiddle]= FOREGROUND;
	data[ZTop]   [YMiddle][XRight] = FOREGROUND;
	data[ZTop]   [YFront] [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZTop]   [YFront] [XMiddle]= DONTCARE;//BACKGROUND;
	data[ZTop]   [YFront] [XRight] = DONTCARE;//BACKGROUND;
	break;};
	

	case 4:{
	data[ZBottom][YBack]  [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZBottom][YBack]  [XMiddle]= DONTCARE;//FOREGROUND;
	data[ZBottom][YBack]  [XRight] = DONTCARE;//BACKGROUND;
	data[ZBottom][YMiddle][XLeft]  = DONTCARE;//BACKGROUND;
	data[ZBottom][YMiddle][XMiddle]= FOREGROUND;
	data[ZBottom][YMiddle][XRight] = DONTCARE;//BACKGROUND;
	data[ZBottom][YFront] [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZBottom][YFront] [XMiddle]= FOREGROUND;
	data[ZBottom][YFront] [XRight] = DONTCARE;//BACKGROUND;
	
	data[ZMiddle][YBack]  [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZMiddle][YBack]  [XMiddle]= FOREGROUND;
	data[ZMiddle][YBack]  [XRight] = FOREGROUND;
	data[ZMiddle][YMiddle][XLeft]  = BACKGROUND;//stays back!
	data[ZMiddle][YMiddle][XMiddle]= FOREGROUND;
	data[ZMiddle][YMiddle][XRight] = FOREGROUND;
	data[ZMiddle][YFront] [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZMiddle][YFront] [XMiddle]= FOREGROUND;
	data[ZMiddle][YFront] [XRight] = FOREGROUND;
	
	
	data[ZTop]   [YBack]  [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZTop]   [YBack]  [XMiddle]= DONTCARE;//BACKGROUND;
	data[ZTop]   [YBack]  [XRight] = DONTCARE;//BACKGROUND;
	data[ZTop]   [YMiddle][XLeft]  = DONTCARE;//BACKGROUND;
	data[ZTop]   [YMiddle][XMiddle]= BACKGROUND;//stays back
	data[ZTop]   [YMiddle][XRight] = DONTCARE;//BACKGROUND;
	data[ZTop]   [YFront] [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZTop]   [YFront] [XMiddle]= DONTCARE;//BACKGROUND;
	data[ZTop]   [YFront] [XRight] = DONTCARE;//BACKGROUND;
	break;};
	
	case 5:{
	data[ZBottom][YBack]  [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZBottom][YBack]  [XMiddle]= DONTCARE;//FOREGROUND;
	data[ZBottom][YBack]  [XRight] = DONTCARE;//BACKGROUND;
	data[ZBottom][YMiddle][XLeft]  = DONTCARE;//BACKGROUND;
	data[ZBottom][YMiddle][XMiddle]= FOREGROUND;
	data[ZBottom][YMiddle][XRight] = DONTCARE;//BACKGROUND;
	data[ZBottom][YFront] [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZBottom][YFront] [XMiddle]= FOREGROUND;
	data[ZBottom][YFront] [XRight] = DONTCARE;//BACKGROUND;
	
	data[ZMiddle][YBack]  [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZMiddle][YBack]  [XMiddle]= FOREGROUND;
	data[ZMiddle][YBack]  [XRight] = DONTCARE;//BACKGROUND;
	data[ZMiddle][YMiddle][XLeft]  = BACKGROUND;//stayBackground
	data[ZMiddle][YMiddle][XMiddle]= FOREGROUND;
	data[ZMiddle][YMiddle][XRight] = BACKGROUND;//staysback
	data[ZMiddle][YFront] [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZMiddle][YFront] [XMiddle]= FOREGROUND;
	data[ZMiddle][YFront] [XRight] = DONTCARE;//BACKGROUND;
	
	
	data[ZTop]   [YBack]  [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZTop]   [YBack]  [XMiddle]= FOREGROUND;
	data[ZTop]   [YBack]  [XRight] = DONTCARE;//BACKGROUND;
	data[ZTop]   [YMiddle][XLeft]  = DONTCARE;//BACKGROUND;
	data[ZTop]   [YMiddle][XMiddle]= FOREGROUND;
	data[ZTop]   [YMiddle][XRight] = DONTCARE;//BACKGROUND;
	data[ZTop]   [YFront] [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZTop]   [YFront] [XMiddle]= FOREGROUND;
	data[ZTop]   [YFront] [XRight] = DONTCARE;//BACKGROUND;
	break;};
	
	case 6:{
	data[ZBottom][YBack]  [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZBottom][YBack]  [XMiddle]= DONTCARE;//BACKGROUND;
	data[ZBottom][YBack]  [XRight] = DONTCARE;//BACKGROUND;
	data[ZBottom][YMiddle][XLeft]  = DONTCARE;//BACKGROUND;
	data[ZBottom][YMiddle][XMiddle]= DONTCARE;//FOREGROUND;test
	data[ZBottom][YMiddle][XRight] = FOREGROUND;
	data[ZBottom][YFront] [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZBottom][YFront] [XMiddle]= FOREGROUND;
	data[ZBottom][YFront] [XRight] = DONTCARE;//BACKGROUND;
	
	data[ZMiddle][YBack]  [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZMiddle][YBack]  [XMiddle]= BACKGROUND;//staysbackground
	data[ZMiddle][YBack]  [XRight] = DONTCARE;//BACKGROUND;
	data[ZMiddle][YMiddle][XLeft]  = BACKGROUND;//staybackground
	data[ZMiddle][YMiddle][XMiddle]= FOREGROUND;
	data[ZMiddle][YMiddle][XRight] = FOREGROUND;
	data[ZMiddle][YFront] [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZMiddle][YFront] [XMiddle]= FOREGROUND;
	data[ZMiddle][YFront] [XRight] = FOREGROUND;
		
	data[ZTop]   [YBack]  [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZTop]   [YBack]  [XMiddle]= DONTCARE;//BACKGROUND;
	data[ZTop]   [YBack]  [XRight] = DONTCARE;//BACKGROUND;
	data[ZTop]   [YMiddle][XLeft]  = DONTCARE;//BACKGROUND;
	data[ZTop]   [YMiddle][XMiddle]= BACKGROUND;//BACKGROUND;
	data[ZTop]   [YMiddle][XRight] = DONTCARE;//BACKGROUND;
	data[ZTop]   [YFront] [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZTop]   [YFront] [XMiddle]= DONTCARE;//BACKGROUND;
	data[ZTop]   [YFront] [XRight] = DONTCARE;//BACKGROUND;
	break;};


case 7:{
	data[ZBottom][YBack]  [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZBottom][YBack]  [XMiddle]= DONTCARE;//BACKGROUND;
	data[ZBottom][YBack]  [XRight] = DONTCARE;//BACKGROUND;
	data[ZBottom][YMiddle][XLeft]  = DONTCARE;//BACKGROUND;
	data[ZBottom][YMiddle][XMiddle]= BACKGROUND;//botton
	data[ZBottom][YMiddle][XRight] = DONTCARE;//BACKGROUND;
	data[ZBottom][YFront] [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZBottom][YFront] [XMiddle]= DONTCARE;//BACKGROUND;
	data[ZBottom][YFront] [XRight] = DONTCARE;//BACKGROUND;
	
	data[ZMiddle][YBack]  [XLeft]  = FOREGROUND;
	data[ZMiddle][YBack]  [XMiddle]= FOREGROUND;//back
	data[ZMiddle][YBack]  [XRight] = DONTCARE;//BACKGROUND;
	data[ZMiddle][YMiddle][XLeft]  = FOREGROUND;//left
	data[ZMiddle][YMiddle][XMiddle]= FOREGROUND;
	data[ZMiddle][YMiddle][XRight] = BACKGROUND;//right
	data[ZMiddle][YFront] [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZMiddle][YFront] [XMiddle]= BACKGROUND;//front
	data[ZMiddle][YFront] [XRight] = DONTCARE;//BACKGROUND;
		
	data[ZTop]   [YBack]  [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZTop]   [YBack]  [XMiddle]= DONTCARE;//BACKGROUND;
	data[ZTop]   [YBack]  [XRight] = DONTCARE;//BACKGROUND;
	data[ZTop]   [YMiddle][XLeft]  = DONTCARE;//BACKGROUND;
	data[ZTop]   [YMiddle][XMiddle]= BACKGROUND;//top
	data[ZTop]   [YMiddle][XRight] = DONTCARE;//BACKGROUND;
	data[ZTop]   [YFront] [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZTop]   [YFront] [XMiddle]= DONTCARE;//BACKGROUND;
	data[ZTop]   [YFront] [XRight] = DONTCARE;//BACKGROUND;
	break;};

case 8:{
	data[ZBottom][YBack]  [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZBottom][YBack]  [XMiddle]= DONTCARE;//BACKGROUND;
	data[ZBottom][YBack]  [XRight] = DONTCARE;//BACKGROUND;
	data[ZBottom][YMiddle][XLeft]  = DONTCARE;//BACKGROUND;
	data[ZBottom][YMiddle][XMiddle]= BACKGROUND;//botton
	data[ZBottom][YMiddle][XRight] = DONTCARE;//BACKGROUND;
	data[ZBottom][YFront] [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZBottom][YFront] [XMiddle]= DONTCARE;//BACKGROUND;
	data[ZBottom][YFront] [XRight] = DONTCARE;//BACKGROUND;
	
	data[ZMiddle][YBack]  [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZMiddle][YBack]  [XMiddle]= FOREGROUND;//back
	data[ZMiddle][YBack]  [XRight] = FOREGROUND;
	data[ZMiddle][YMiddle][XLeft]  = BACKGROUND;//left
	data[ZMiddle][YMiddle][XMiddle]= FOREGROUND;
	data[ZMiddle][YMiddle][XRight] = FOREGROUND;//right
	data[ZMiddle][YFront] [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZMiddle][YFront] [XMiddle]= FOREGROUND;//front
	data[ZMiddle][YFront] [XRight] = FOREGROUND;
		
	data[ZTop]   [YBack]  [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZTop]   [YBack]  [XMiddle]= DONTCARE;//BACKGROUND;
	data[ZTop]   [YBack]  [XRight] = DONTCARE;//BACKGROUND;
	data[ZTop]   [YMiddle][XLeft]  = DONTCARE;//BACKGROUND;
	data[ZTop]   [YMiddle][XMiddle]= BACKGROUND;//top
	data[ZTop]   [YMiddle][XRight] = DONTCARE;//BACKGROUND;
	data[ZTop]   [YFront] [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZTop]   [YFront] [XMiddle]= DONTCARE;//BACKGROUND;
	data[ZTop]   [YFront] [XRight] = DONTCARE;//BACKGROUND;
	break;};

case 9:{
	data[ZBottom][YBack]  [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZBottom][YBack]  [XMiddle]= DONTCARE;//BACKGROUND;
	data[ZBottom][YBack]  [XRight] = DONTCARE;//BACKGROUND;
	data[ZBottom][YMiddle][XLeft]  = DONTCARE;//BACKGROUND;
	data[ZBottom][YMiddle][XMiddle]= BACKGROUND;//botton
	data[ZBottom][YMiddle][XRight] = DONTCARE;//BACKGROUND;
	data[ZBottom][YFront] [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZBottom][YFront] [XMiddle]= DONTCARE;//BACKGROUND;
	data[ZBottom][YFront] [XRight] = DONTCARE;//BACKGROUND;
	
	data[ZMiddle][YBack]  [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZMiddle][YBack]  [XMiddle]= FOREGROUND;//back
	data[ZMiddle][YBack]  [XRight] = FOREGROUND;
	data[ZMiddle][YMiddle][XLeft]  = BACKGROUND;//left
	data[ZMiddle][YMiddle][XMiddle]= FOREGROUND;
	data[ZMiddle][YMiddle][XRight] = FOREGROUND;//right
	data[ZMiddle][YFront] [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZMiddle][YFront] [XMiddle]= BACKGROUND;//front
	data[ZMiddle][YFront] [XRight] = DONTCARE;
		
	data[ZTop]   [YBack]  [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZTop]   [YBack]  [XMiddle]= DONTCARE;//BACKGROUND;
	data[ZTop]   [YBack]  [XRight] = DONTCARE;//BACKGROUND;
	data[ZTop]   [YMiddle][XLeft]  = DONTCARE;//BACKGROUND;
	data[ZTop]   [YMiddle][XMiddle]= FOREGROUND;//top
	data[ZTop]   [YMiddle][XRight] = FOREGROUND;
	data[ZTop]   [YFront] [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZTop]   [YFront] [XMiddle]= DONTCARE;//BACKGROUND;
	data[ZTop]   [YFront] [XRight] = DONTCARE;//BACKGROUND;
	break;};

case 10:{
	data[ZBottom][YBack]  [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZBottom][YBack]  [XMiddle]= DONTCARE;//BACKGROUND;
	data[ZBottom][YBack]  [XRight] = DONTCARE;//BACKGROUND;  
	data[ZBottom][YMiddle][XLeft]  = DONTCARE;//BACKGROUND;
	data[ZBottom][YMiddle][XMiddle]= BACKGROUND;//botton
	data[ZBottom][YMiddle][XRight] = DONTCARE;//BACKGROUND;
	data[ZBottom][YFront] [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZBottom][YFront] [XMiddle]= DONTCARE;//BACKGROUND;
	data[ZBottom][YFront] [XRight] = DONTCARE;//BACKGROUND;
	
	data[ZMiddle][YBack]  [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZMiddle][YBack]  [XMiddle]= BACKGROUND;//back
	data[ZMiddle][YBack]  [XRight] = DONTCARE;
	data[ZMiddle][YMiddle][XLeft]  = BACKGROUND;//left
	data[ZMiddle][YMiddle][XMiddle]= FOREGROUND;
	data[ZMiddle][YMiddle][XRight] = FOREGROUND;//right
	data[ZMiddle][YFront] [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZMiddle][YFront] [XMiddle]= BACKGROUND;//front
	data[ZMiddle][YFront] [XRight] = DONTCARE;
		
	data[ZTop]   [YBack]  [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZTop]   [YBack]  [XMiddle]= DONTCARE;//BACKGROUND;
	data[ZTop]   [YBack]  [XRight] = DONTCARE;//BACKGROUND;
	data[ZTop]   [YMiddle][XLeft]  = DONTCARE;//BACKGROUND;
	data[ZTop]   [YMiddle][XMiddle]= BACKGROUND;//top
	data[ZTop]   [YMiddle][XRight] = DONTCARE;
	data[ZTop]   [YFront] [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZTop]   [YFront] [XMiddle]= DONTCARE;//BACKGROUND;
	data[ZTop]   [YFront] [XRight] = DONTCARE;//BACKGROUND;
	break;};

case 11:{
	data[ZBottom][YBack]  [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZBottom][YBack]  [XMiddle]= FOREGROUND;//BACKGROUND;
	data[ZBottom][YBack]  [XRight] = DONTCARE;//BACKGROUND;  
	data[ZBottom][YMiddle][XLeft]  = DONTCARE;//BACKGROUND;
	data[ZBottom][YMiddle][XMiddle]= FOREGROUND;//botton
	data[ZBottom][YMiddle][XRight] = DONTCARE;//BACKGROUND;
	data[ZBottom][YFront] [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZBottom][YFront] [XMiddle]= FOREGROUND;//BACKGROUND;
	data[ZBottom][YFront] [XRight] = DONTCARE;//BACKGROUND;
	
	data[ZMiddle][YBack]  [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZMiddle][YBack]  [XMiddle]= DONTCARE;//BACKGROUND;//back
	data[ZMiddle][YBack]  [XRight] = DONTCARE;
	data[ZMiddle][YMiddle][XLeft]  = BACKGROUND;//left
	data[ZMiddle][YMiddle][XMiddle]= FOREGROUND;
	data[ZMiddle][YMiddle][XRight] = BACKGROUND;//right
	data[ZMiddle][YFront] [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZMiddle][YFront] [XMiddle]= FOREGROUND;//front
	data[ZMiddle][YFront] [XRight] = DONTCARE;
		
	data[ZTop]   [YBack]  [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZTop]   [YBack]  [XMiddle]= DONTCARE;//BACKGROUND;
	data[ZTop]   [YBack]  [XRight] = DONTCARE;//BACKGROUND;
	data[ZTop]   [YMiddle][XLeft]  = DONTCARE;//BACKGROUND;
	data[ZTop]   [YMiddle][XMiddle]= DONTCARE;//BACKGROUND;//top
	data[ZTop]   [YMiddle][XRight] = DONTCARE;
	data[ZTop]   [YFront] [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZTop]   [YFront] [XMiddle]= FOREGROUND;//BACKGROUND;
	data[ZTop]   [YFront] [XRight] = DONTCARE;//BACKGROUND;
	break;};

case 12:{
	data[ZBottom][YBack]  [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZBottom][YBack]  [XMiddle]= DONTCARE;//BACKGROUND;
	data[ZBottom][YBack]  [XRight] = DONTCARE;//BACKGROUND;
	data[ZBottom][YMiddle][XLeft]  = DONTCARE;//BACKGROUND;
	data[ZBottom][YMiddle][XMiddle]= FOREGROUND;//botton
	data[ZBottom][YMiddle][XRight] = FOREGROUND;//BACKGROUND;
	data[ZBottom][YFront] [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZBottom][YFront] [XMiddle]= DONTCARE;//BACKGROUND;
	data[ZBottom][YFront] [XRight] = DONTCARE;//BACKGROUND;
	
	data[ZMiddle][YBack]  [XLeft]  = DONTCARE;
	data[ZMiddle][YBack]  [XMiddle]= BACKGROUND;//back
	data[ZMiddle][YBack]  [XRight] = DONTCARE;//BACKGROUND;
	data[ZMiddle][YMiddle][XLeft]  = BACKGROUND;//left
	data[ZMiddle][YMiddle][XMiddle]= FOREGROUND;
	data[ZMiddle][YMiddle][XRight] = FOREGROUND;//right
	data[ZMiddle][YFront] [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZMiddle][YFront] [XMiddle]= BACKGROUND;//front
	data[ZMiddle][YFront] [XRight] = DONTCARE;//BACKGROUND;
		
	data[ZTop]   [YBack]  [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZTop]   [YBack]  [XMiddle]= DONTCARE;//BACKGROUND;
	data[ZTop]   [YBack]  [XRight] = DONTCARE;//BACKGROUND;
	data[ZTop]   [YMiddle][XLeft]  = DONTCARE;//BACKGROUND;
	data[ZTop]   [YMiddle][XMiddle]= FOREGROUND;//top
	data[ZTop]   [YMiddle][XRight] = FOREGROUND;//BACKGROUND;
	data[ZTop]   [YFront] [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZTop]   [YFront] [XMiddle]= DONTCARE;//BACKGROUND;
	data[ZTop]   [YFront] [XRight] = DONTCARE;//BACKGROUND;
	break;};

case 13:{
	data[ZBottom][YBack]  [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZBottom][YBack]  [XMiddle]= DONTCARE;//BACKGROUND;
	data[ZBottom][YBack]  [XRight] = DONTCARE;//BACKGROUND;  
	data[ZBottom][YMiddle][XLeft]  = DONTCARE;//BACKGROUND;
	data[ZBottom][YMiddle][XMiddle]= BACKGROUND;//botton
	data[ZBottom][YMiddle][XRight] = DONTCARE;//BACKGROUND;
	data[ZBottom][YFront] [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZBottom][YFront] [XMiddle]= DONTCARE;//BACKGROUND;
	data[ZBottom][YFront] [XRight] = DONTCARE;//BACKGROUND;
	
	data[ZMiddle][YBack]  [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZMiddle][YBack]  [XMiddle]= BACKGROUND;//back
	data[ZMiddle][YBack]  [XRight] = DONTCARE;
	data[ZMiddle][YMiddle][XLeft]  = BACKGROUND;//left
	data[ZMiddle][YMiddle][XMiddle]= FOREGROUND;
	data[ZMiddle][YMiddle][XRight] = BACKGROUND;//right
	data[ZMiddle][YFront] [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZMiddle][YFront] [XMiddle]= BACKGROUND;//front
	data[ZMiddle][YFront] [XRight] = DONTCARE;
		
	data[ZTop]   [YBack]  [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZTop]   [YBack]  [XMiddle]= DONTCARE;//BACKGROUND;
	data[ZTop]   [YBack]  [XRight] = DONTCARE;//BACKGROUND;
	data[ZTop]   [YMiddle][XLeft]  = DONTCARE;//BACKGROUND;
	data[ZTop]   [YMiddle][XMiddle]= FOREGROUND;//top
	data[ZTop]   [YMiddle][XRight] = DONTCARE;
	data[ZTop]   [YFront] [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZTop]   [YFront] [XMiddle]= DONTCARE;//BACKGROUND;
	data[ZTop]   [YFront] [XRight] = DONTCARE;//BACKGROUND;
	break;};

}
}


 /*!
 *Primitives for 26 neigbourhood!
 */
Primitive::
Primitive(int nr,int kindofneigbourhood)
{
derivatedFrom=nr;
for (short x=0;x<3;x++)
	for(short y=0;y<3;y++)
		for(short z=0;z<3;z++)
			data[x][y][z]=BACKGROUND;

if (kindofneigbourhood==6) Templates6(nr);
Templates26(nr);
}


void
Primitive::
Templates26(int nr)
{
switch (nr) 
	{
	
	case 1:{
	data[ZBottom][YBack]  [XLeft]  = DONTCARE;// BACKGROUND;
	data[ZBottom][YBack]  [XMiddle]= DONTCARE;//BACKGROUND;
	data[ZBottom][YBack]  [XRight] = DONTCARE;// BACKGROUND;
	data[ZBottom][YMiddle][XLeft]  = FOREGROUND;
	data[ZBottom][YMiddle][XMiddle]= FOREGROUND;
	data[ZBottom][YMiddle][XRight] = DONTCARE;// BACKGROUND;
	data[ZBottom][YFront] [XLeft]  = DONTCARE;// BACKGROUND;
	data[ZBottom][YFront] [XMiddle]= FOREGROUND;
	data[ZBottom][YFront] [XRight] = DONTCARE;// BACKGROUND;
	
	data[ZMiddle][YBack]  [XLeft]  = FOREGROUND;
	data[ZMiddle][YBack]  [XMiddle]= FOREGROUND;
	data[ZMiddle][YBack]  [XRight] = DONTCARE;// BACKGROUND;
	data[ZMiddle][YMiddle][XLeft]  = FOREGROUND;
	data[ZMiddle][YMiddle][XMiddle]= FOREGROUND;
	data[ZMiddle][YMiddle][XRight] = FOREGROUND;
	data[ZMiddle][YFront] [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZMiddle][YFront] [XMiddle]= BACKGROUND;
	data[ZMiddle][YFront] [XRight] = FOREGROUND;
	
	
	data[ZTop]   [YBack]  [XLeft]  = DONTCARE;// BACKGROUND;
	data[ZTop]   [YBack]  [XMiddle]= FOREGROUND;
	data[ZTop]   [YBack]  [XRight] = DONTCARE;// BACKGROUND;
	data[ZTop]   [YMiddle][XLeft]  = DONTCARE;// BACKGROUND;
	data[ZTop]   [YMiddle][XMiddle]= FOREGROUND;
	data[ZTop]   [YMiddle][XRight] = FOREGROUND;
	data[ZTop]   [YFront] [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZTop]   [YFront] [XMiddle]= DONTCARE;//BACKGROUND;
	data[ZTop]   [YFront] [XRight] = DONTCARE;//BACKGROUND;
	break;};
// FOREGROUND;//
// BACKGROUND;//
// DONTCARE;//   
	
	
	case 2:{
	data[ZBottom][YBack]  [XLeft]  = DONTCARE;// BACKGROUND;
	data[ZBottom][YBack]  [XMiddle]= DONTCARE;// BACKGROUND;
	data[ZBottom][YBack]  [XRight] = DONTCARE;//BACKGROUND;
	data[ZBottom][YMiddle][XLeft]  = FOREGROUND;
	data[ZBottom][YMiddle][XMiddle]= FOREGROUND;
	data[ZBottom][YMiddle][XRight] = DONTCARE;//BACKGROUND;
	data[ZBottom][YFront] [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZBottom][YFront] [XMiddle]= FOREGROUND;
	data[ZBottom][YFront] [XRight] = DONTCARE;//BACKGROUND;
	
	data[ZMiddle][YBack]  [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZMiddle][YBack]  [XMiddle]= FOREGROUND;
	data[ZMiddle][YBack]  [XRight] = FOREGROUND;
	data[ZMiddle][YMiddle][XLeft]  = FOREGROUND;
	data[ZMiddle][YMiddle][XMiddle]= FOREGROUND;
	data[ZMiddle][YMiddle][XRight] = FOREGROUND;
	data[ZMiddle][YFront] [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZMiddle][YFront] [XMiddle]= BACKGROUND;//onlyone that stays background
	data[ZMiddle][YFront] [XRight] = FOREGROUND;
	
	
	data[ZTop]   [YBack]  [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZTop]   [YBack]  [XMiddle]= FOREGROUND;
	data[ZTop]   [YBack]  [XRight] = DONTCARE;//BACKGROUND;
	data[ZTop]   [YMiddle][XLeft]  = FOREGROUND;
	data[ZTop]   [YMiddle][XMiddle]= FOREGROUND;
	data[ZTop]   [YMiddle][XRight] = DONTCARE;//BACKGROUND;
	data[ZTop]   [YFront] [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZTop]   [YFront] [XMiddle]= DONTCARE;//BACKGROUND;
	data[ZTop]   [YFront] [XRight] = DONTCARE;//BACKGROUND;
	break;};
	
	case 3:{
	data[ZBottom][YBack]  [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZBottom][YBack]  [XMiddle]= DONTCARE;//BACKGROUND;
	data[ZBottom][YBack]  [XRight] = DONTCARE;//BACKGROUND;
	data[ZBottom][YMiddle][XLeft]  = FOREGROUND;
	data[ZBottom][YMiddle][XMiddle]= FOREGROUND;
	data[ZBottom][YMiddle][XRight] = DONTCARE;//BACKGROUND;
	data[ZBottom][YFront] [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZBottom][YFront] [XMiddle]= FOREGROUND;
	data[ZBottom][YFront] [XRight] = DONTCARE;//BACKGROUND;
	
	data[ZMiddle][YBack]  [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZMiddle][YBack]  [XMiddle]= BACKGROUND;//stays background!!!
	data[ZMiddle][YBack]  [XRight] = DONTCARE;//BACKGROUND;
	data[ZMiddle][YMiddle][XLeft]  = FOREGROUND;
	data[ZMiddle][YMiddle][XMiddle]= FOREGROUND;
	data[ZMiddle][YMiddle][XRight] = FOREGROUND;
	data[ZMiddle][YFront] [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZMiddle][YFront] [XMiddle]= DONTCARE;//BACKGROUND;
	data[ZMiddle][YFront] [XRight] = FOREGROUND;
	
	
	data[ZTop]   [YBack]  [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZTop]   [YBack]  [XMiddle]= DONTCARE;//BACKGROUND;
	data[ZTop]   [YBack]  [XRight] = DONTCARE;//BACKGROUND;
	data[ZTop]   [YMiddle][XLeft]  = FOREGROUND;
	data[ZTop]   [YMiddle][XMiddle]= FOREGROUND;
	data[ZTop]   [YMiddle][XRight] = FOREGROUND;
	data[ZTop]   [YFront] [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZTop]   [YFront] [XMiddle]= DONTCARE;//BACKGROUND;
	data[ZTop]   [YFront] [XRight] = DONTCARE;//BACKGROUND;
	break;};
	

	case 4:{
	data[ZBottom][YBack]  [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZBottom][YBack]  [XMiddle]= DONTCARE;//FOREGROUND;
	data[ZBottom][YBack]  [XRight] = DONTCARE;//BACKGROUND;
	data[ZBottom][YMiddle][XLeft]  = DONTCARE;//BACKGROUND;
	data[ZBottom][YMiddle][XMiddle]= FOREGROUND;
	data[ZBottom][YMiddle][XRight] = DONTCARE;//BACKGROUND;
	data[ZBottom][YFront] [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZBottom][YFront] [XMiddle]= FOREGROUND;
	data[ZBottom][YFront] [XRight] = DONTCARE;//BACKGROUND;
	
	data[ZMiddle][YBack]  [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZMiddle][YBack]  [XMiddle]= FOREGROUND;
	data[ZMiddle][YBack]  [XRight] = FOREGROUND;
	data[ZMiddle][YMiddle][XLeft]  = BACKGROUND;//stays back!
	data[ZMiddle][YMiddle][XMiddle]= FOREGROUND;
	data[ZMiddle][YMiddle][XRight] = FOREGROUND;
	data[ZMiddle][YFront] [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZMiddle][YFront] [XMiddle]= FOREGROUND;
	data[ZMiddle][YFront] [XRight] = FOREGROUND;
	
	
	data[ZTop]   [YBack]  [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZTop]   [YBack]  [XMiddle]= DONTCARE;//BACKGROUND;
	data[ZTop]   [YBack]  [XRight] = DONTCARE;//BACKGROUND;
	data[ZTop]   [YMiddle][XLeft]  = BACKGROUND;
	data[ZTop]   [YMiddle][XMiddle]= BACKGROUND;//stays back
	data[ZTop]   [YMiddle][XRight] = DONTCARE;//BACKGROUND;
	data[ZTop]   [YFront] [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZTop]   [YFront] [XMiddle]= DONTCARE;//BACKGROUND;
	data[ZTop]   [YFront] [XRight] = DONTCARE;//BACKGROUND;
	break;};
	
	case 5:{
	data[ZBottom][YBack]  [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZBottom][YBack]  [XMiddle]= DONTCARE;//FOREGROUND;
	data[ZBottom][YBack]  [XRight] = DONTCARE;//BACKGROUND;
	data[ZBottom][YMiddle][XLeft]  = DONTCARE;//BACKGROUND;
	data[ZBottom][YMiddle][XMiddle]= FOREGROUND;
	data[ZBottom][YMiddle][XRight] = DONTCARE;//BACKGROUND;
	data[ZBottom][YFront] [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZBottom][YFront] [XMiddle]= FOREGROUND;
	data[ZBottom][YFront] [XRight] = DONTCARE;//BACKGROUND;
	
	data[ZMiddle][YBack]  [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZMiddle][YBack]  [XMiddle]= FOREGROUND;
	data[ZMiddle][YBack]  [XRight] = DONTCARE;//BACKGROUND;
	data[ZMiddle][YMiddle][XLeft]  = BACKGROUND;//stayBackground
	data[ZMiddle][YMiddle][XMiddle]= FOREGROUND;
	data[ZMiddle][YMiddle][XRight] = BACKGROUND;//staysback
	data[ZMiddle][YFront] [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZMiddle][YFront] [XMiddle]= FOREGROUND;
	data[ZMiddle][YFront] [XRight] = DONTCARE;//BACKGROUND;
	
	
	data[ZTop]   [YBack]  [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZTop]   [YBack]  [XMiddle]= FOREGROUND;
	data[ZTop]   [YBack]  [XRight] = DONTCARE;//BACKGROUND;
	data[ZTop]   [YMiddle][XLeft]  = DONTCARE;//BACKGROUND;
	data[ZTop]   [YMiddle][XMiddle]= FOREGROUND;
	data[ZTop]   [YMiddle][XRight] = DONTCARE;//BACKGROUND;
	data[ZTop]   [YFront] [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZTop]   [YFront] [XMiddle]= FOREGROUND;
	data[ZTop]   [YFront] [XRight] = DONTCARE;//BACKGROUND;
	break;};
	
	case 6:{
	data[ZBottom][YBack]  [XLeft]  = BACKGROUND;//BACKGROUND;
	data[ZBottom][YBack]  [XMiddle]= DONTCARE;//BACKGROUND;
	data[ZBottom][YBack]  [XRight] = DONTCARE;//BACKGROUND;
	data[ZBottom][YMiddle][XLeft]  = DONTCARE;//BACKGROUND;
	data[ZBottom][YMiddle][XMiddle]= DONTCARE;//FOREGROUND;test
	data[ZBottom][YMiddle][XRight] = FOREGROUND;
	data[ZBottom][YFront] [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZBottom][YFront] [XMiddle]= FOREGROUND;
	data[ZBottom][YFront] [XRight] = DONTCARE;//BACKGROUND;
	
	data[ZMiddle][YBack]  [XLeft]  = BACKGROUND;//BACKGROUND;
	data[ZMiddle][YBack]  [XMiddle]= DONTCARE;//staysbackground
	data[ZMiddle][YBack]  [XRight] = DONTCARE;//BACKGROUND;
	data[ZMiddle][YMiddle][XLeft]  = BACKGROUND;//staybackground
	data[ZMiddle][YMiddle][XMiddle]= FOREGROUND;
	data[ZMiddle][YMiddle][XRight] = FOREGROUND;
	data[ZMiddle][YFront] [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZMiddle][YFront] [XMiddle]= FOREGROUND;
	data[ZMiddle][YFront] [XRight] = FOREGROUND;
		
	data[ZTop]   [YBack]  [XLeft]  = BACKGROUND;//BACKGROUND;
	data[ZTop]   [YBack]  [XMiddle]= DONTCARE;//BACKGROUND;
	data[ZTop]   [YBack]  [XRight] = DONTCARE;//BACKGROUND;
	data[ZTop]   [YMiddle][XLeft]  = BACKGROUND;
	data[ZTop]   [YMiddle][XMiddle]= DONTCARE;//BACKGROUND;
	data[ZTop]   [YMiddle][XRight] = DONTCARE;//BACKGROUND;
	data[ZTop]   [YFront] [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZTop]   [YFront] [XMiddle]= DONTCARE;//BACKGROUND;
	data[ZTop]   [YFront] [XRight] = DONTCARE;//BACKGROUND;
	break;};


case 7:{
	data[ZBottom][YBack]  [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZBottom][YBack]  [XMiddle]= DONTCARE;//BACKGROUND;
	data[ZBottom][YBack]  [XRight] = DONTCARE;//BACKGROUND;
	data[ZBottom][YMiddle][XLeft]  = DONTCARE;//BACKGROUND;
	data[ZBottom][YMiddle][XMiddle]= BACKGROUND;//botton
	data[ZBottom][YMiddle][XRight] = DONTCARE;//BACKGROUND;
	data[ZBottom][YFront] [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZBottom][YFront] [XMiddle]= BACKGROUND;
	data[ZBottom][YFront] [XRight] = BACKGROUND;
	
	data[ZMiddle][YBack]  [XLeft]  = FOREGROUND;
	data[ZMiddle][YBack]  [XMiddle]= FOREGROUND;//back
	data[ZMiddle][YBack]  [XRight] = DONTCARE;//BACKGROUND;
	data[ZMiddle][YMiddle][XLeft]  = FOREGROUND;//left
	data[ZMiddle][YMiddle][XMiddle]= FOREGROUND;
	data[ZMiddle][YMiddle][XRight] = BACKGROUND;//right
	data[ZMiddle][YFront] [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZMiddle][YFront] [XMiddle]= BACKGROUND;//front
	data[ZMiddle][YFront] [XRight] = BACKGROUND;
		
	data[ZTop]   [YBack]  [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZTop]   [YBack]  [XMiddle]= DONTCARE;//BACKGROUND;
	data[ZTop]   [YBack]  [XRight] = DONTCARE;//BACKGROUND;
	data[ZTop]   [YMiddle][XLeft]  = DONTCARE;//BACKGROUND;
	data[ZTop]   [YMiddle][XMiddle]= BACKGROUND;//top
	data[ZTop]   [YMiddle][XRight] = BACKGROUND;
	data[ZTop]   [YFront] [XLeft]  = BACKGROUND;
	data[ZTop]   [YFront] [XMiddle]= BACKGROUND;
	data[ZTop]   [YFront] [XRight] = BACKGROUND;
	break;};

case 8:{
	data[ZBottom][YBack]  [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZBottom][YBack]  [XMiddle]= DONTCARE;//BACKGROUND;
	data[ZBottom][YBack]  [XRight] = DONTCARE;//BACKGROUND;
	data[ZBottom][YMiddle][XLeft]  = DONTCARE;//BACKGROUND;
	data[ZBottom][YMiddle][XMiddle]= DONTCARE;//BACKGROUND;//botton
	data[ZBottom][YMiddle][XRight] = DONTCARE;//BACKGROUND;
	data[ZBottom][YFront] [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZBottom][YFront] [XMiddle]= DONTCARE;//BACKGROUND;
	data[ZBottom][YFront] [XRight] = DONTCARE;//BACKGROUND;
	
	data[ZMiddle][YBack]  [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZMiddle][YBack]  [XMiddle]= FOREGROUND;//back
	data[ZMiddle][YBack]  [XRight] = FOREGROUND;
	data[ZMiddle][YMiddle][XLeft]  = BACKGROUND;//left
	data[ZMiddle][YMiddle][XMiddle]= FOREGROUND;
	data[ZMiddle][YMiddle][XRight] = FOREGROUND;//right
	data[ZMiddle][YFront] [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZMiddle][YFront] [XMiddle]= FOREGROUND;//front
	data[ZMiddle][YFront] [XRight] = FOREGROUND;
		
	data[ZTop]   [YBack]  [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZTop]   [YBack]  [XMiddle]= DONTCARE;//BACKGROUND;
	data[ZTop]   [YBack]  [XRight] = DONTCARE;//BACKGROUND;
	data[ZTop]   [YMiddle][XLeft]  = DONTCARE;//BACKGROUND;
	data[ZTop]   [YMiddle][XMiddle]= DONTCARE; //BACKGROUND;//top
	data[ZTop]   [YMiddle][XRight] = DONTCARE;//BACKGROUND;
	data[ZTop]   [YFront] [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZTop]   [YFront] [XMiddle]= DONTCARE;//BACKGROUND;
	data[ZTop]   [YFront] [XRight] = DONTCARE;//BACKGROUND;
	break;};

case 9:{
	data[ZBottom][YBack]  [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZBottom][YBack]  [XMiddle]= DONTCARE;//BACKGROUND;
	data[ZBottom][YBack]  [XRight] = DONTCARE;//BACKGROUND;
	data[ZBottom][YMiddle][XLeft]  = DONTCARE;//BACKGROUND;
	data[ZBottom][YMiddle][XMiddle]= BACKGROUND;//botton
	data[ZBottom][YMiddle][XRight] = DONTCARE;//BACKGROUND;
	data[ZBottom][YFront] [XLeft]  = BACKGROUND;
	data[ZBottom][YFront] [XMiddle]= BACKGROUND; //DONTCARE;//BACKGROUND;
	data[ZBottom][YFront] [XRight] = DONTCARE;//BACKGROUND;
	
	data[ZMiddle][YBack]  [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZMiddle][YBack]  [XMiddle]= FOREGROUND;//back
	data[ZMiddle][YBack]  [XRight] = FOREGROUND;
	data[ZMiddle][YMiddle][XLeft]  = BACKGROUND;//left
	data[ZMiddle][YMiddle][XMiddle]= FOREGROUND;
	data[ZMiddle][YMiddle][XRight] = FOREGROUND;//right
	data[ZMiddle][YFront] [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZMiddle][YFront] [XMiddle]= BACKGROUND;//front
	data[ZMiddle][YFront] [XRight] = DONTCARE;
		
	data[ZTop]   [YBack]  [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZTop]   [YBack]  [XMiddle]= DONTCARE;//BACKGROUND;
	data[ZTop]   [YBack]  [XRight] = DONTCARE;//BACKGROUND;
	data[ZTop]   [YMiddle][XLeft]  = DONTCARE;//BACKGROUND;
	data[ZTop]   [YMiddle][XMiddle]= FOREGROUND;//top
	data[ZTop]   [YMiddle][XRight] = FOREGROUND;
	data[ZTop]   [YFront] [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZTop]   [YFront] [XMiddle]= DONTCARE;//BACKGROUND;
	data[ZTop]   [YFront] [XRight] = DONTCARE;//BACKGROUND;
	break;};

case 10:{
	data[ZBottom][YBack]  [XLeft]  = BACKGROUND;
	data[ZBottom][YBack]  [XMiddle]= DONTCARE;//BACKGROUND;
	data[ZBottom][YBack]  [XRight] = DONTCARE;//BACKGROUND;  
	data[ZBottom][YMiddle][XLeft]  = BACKGROUND;
	data[ZBottom][YMiddle][XMiddle]= BACKGROUND;//botton
	data[ZBottom][YMiddle][XRight] = DONTCARE;//BACKGROUND;
	data[ZBottom][YFront] [XLeft]  = BACKGROUND;
	data[ZBottom][YFront] [XMiddle]= DONTCARE;//BACKGROUND;
	data[ZBottom][YFront] [XRight] = DONTCARE;//BACKGROUND;
	
	data[ZMiddle][YBack]  [XLeft]  = BACKGROUND;
	data[ZMiddle][YBack]  [XMiddle]= BACKGROUND;//back
	data[ZMiddle][YBack]  [XRight] = DONTCARE;
	data[ZMiddle][YMiddle][XLeft]  = BACKGROUND;//left
	data[ZMiddle][YMiddle][XMiddle]= FOREGROUND;
	data[ZMiddle][YMiddle][XRight] = FOREGROUND;//right
	data[ZMiddle][YFront] [XLeft]  = BACKGROUND;
	data[ZMiddle][YFront] [XMiddle]= BACKGROUND;//front
	data[ZMiddle][YFront] [XRight] = DONTCARE;
		
	data[ZTop]   [YBack]  [XLeft]  = BACKGROUND;
	data[ZTop]   [YBack]  [XMiddle]= DONTCARE;//BACKGROUND;
	data[ZTop]   [YBack]  [XRight] = DONTCARE;//BACKGROUND;
	data[ZTop]   [YMiddle][XLeft]  = BACKGROUND;
	data[ZTop]   [YMiddle][XMiddle]= BACKGROUND;//top
	data[ZTop]   [YMiddle][XRight] = DONTCARE;
	data[ZTop]   [YFront] [XLeft]  = BACKGROUND;
	data[ZTop]   [YFront] [XMiddle]= DONTCARE;//BACKGROUND;
	data[ZTop]   [YFront] [XRight] = DONTCARE;//BACKGROUND;
	break;};

case 11:{
	data[ZBottom][YBack]  [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZBottom][YBack]  [XMiddle]= FOREGROUND;//BACKGROUND;
	data[ZBottom][YBack]  [XRight] = DONTCARE;//BACKGROUND;  
	data[ZBottom][YMiddle][XLeft]  = DONTCARE;//BACKGROUND;
	data[ZBottom][YMiddle][XMiddle]= FOREGROUND;//botton
	data[ZBottom][YMiddle][XRight] = DONTCARE;//BACKGROUND;
	data[ZBottom][YFront] [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZBottom][YFront] [XMiddle]= FOREGROUND;//BACKGROUND;
	data[ZBottom][YFront] [XRight] = DONTCARE;//BACKGROUND;
	
	data[ZMiddle][YBack]  [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZMiddle][YBack]  [XMiddle]= DONTCARE;//BACKGROUND;//back
	data[ZMiddle][YBack]  [XRight] = DONTCARE;
	data[ZMiddle][YMiddle][XLeft]  = DONTCARE;//BACKGROUND;//left
	data[ZMiddle][YMiddle][XMiddle]= FOREGROUND;
	data[ZMiddle][YMiddle][XRight] = DONTCARE;//BACKGROUND;//right
	data[ZMiddle][YFront] [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZMiddle][YFront] [XMiddle]= FOREGROUND;//front
	data[ZMiddle][YFront] [XRight] = DONTCARE;
		
	data[ZTop]   [YBack]  [XLeft]  = BACKGROUND;
	data[ZTop]   [YBack]  [XMiddle]= BACKGROUND;
	data[ZTop]   [YBack]  [XRight] = BACKGROUND;
	data[ZTop]   [YMiddle][XLeft]  = DONTCARE;//BACKGROUND;
	data[ZTop]   [YMiddle][XMiddle]= DONTCARE;//BACKGROUND;//top
	data[ZTop]   [YMiddle][XRight] = DONTCARE;
	data[ZTop]   [YFront] [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZTop]   [YFront] [XMiddle]= FOREGROUND;//BACKGROUND;
	data[ZTop]   [YFront] [XRight] = DONTCARE;//BACKGROUND;
	break;};

case 12:{
	data[ZBottom][YBack]  [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZBottom][YBack]  [XMiddle]= DONTCARE;//BACKGROUND;
	data[ZBottom][YBack]  [XRight] = DONTCARE;//BACKGROUND;
	data[ZBottom][YMiddle][XLeft]  = DONTCARE;//BACKGROUND;
	data[ZBottom][YMiddle][XMiddle]= FOREGROUND;//botton
	data[ZBottom][YMiddle][XRight] = FOREGROUND;//BACKGROUND;
	data[ZBottom][YFront] [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZBottom][YFront] [XMiddle]= DONTCARE;//BACKGROUND;
	data[ZBottom][YFront] [XRight] = DONTCARE;//BACKGROUND;
	
	data[ZMiddle][YBack]  [XLeft]  = DONTCARE;
	data[ZMiddle][YBack]  [XMiddle]= BACKGROUND;//back
	data[ZMiddle][YBack]  [XRight] = DONTCARE;//BACKGROUND;
	data[ZMiddle][YMiddle][XLeft]  = BACKGROUND;//left
	data[ZMiddle][YMiddle][XMiddle]= FOREGROUND;
	data[ZMiddle][YMiddle][XRight] = FOREGROUND;//right
	data[ZMiddle][YFront] [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZMiddle][YFront] [XMiddle]= BACKGROUND;//front
	data[ZMiddle][YFront] [XRight] = DONTCARE;//BACKGROUND;
		
	data[ZTop]   [YBack]  [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZTop]   [YBack]  [XMiddle]= DONTCARE;//BACKGROUND;
	data[ZTop]   [YBack]  [XRight] = DONTCARE;//BACKGROUND;
	data[ZTop]   [YMiddle][XLeft]  = DONTCARE;//BACKGROUND;
	data[ZTop]   [YMiddle][XMiddle]= FOREGROUND;//top
	data[ZTop]   [YMiddle][XRight] = FOREGROUND;//BACKGROUND;
	data[ZTop]   [YFront] [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZTop]   [YFront] [XMiddle]= DONTCARE;//BACKGROUND;
	data[ZTop]   [YFront] [XRight] = DONTCARE;//BACKGROUND;
	break;};

case 13:{
	data[ZBottom][YBack]  [XLeft]  = BACKGROUND;
	data[ZBottom][YBack]  [XMiddle]= BACKGROUND;
	data[ZBottom][YBack]  [XRight] = BACKGROUND;  
	data[ZBottom][YMiddle][XLeft]  = BACKGROUND;
	data[ZBottom][YMiddle][XMiddle]= BACKGROUND;//botton
	data[ZBottom][YMiddle][XRight] = BACKGROUND;
	data[ZBottom][YFront] [XLeft]  = BACKGROUND;
	data[ZBottom][YFront] [XMiddle]= BACKGROUND;
	data[ZBottom][YFront] [XRight] = BACKGROUND;
	
	data[ZMiddle][YBack]  [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZMiddle][YBack]  [XMiddle]= DONTCARE;//back
	data[ZMiddle][YBack]  [XRight] = DONTCARE;
	data[ZMiddle][YMiddle][XLeft]  = DONTCARE;//left
	data[ZMiddle][YMiddle][XMiddle]= FOREGROUND;
	data[ZMiddle][YMiddle][XRight] = DONTCARE;//right
	data[ZMiddle][YFront] [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZMiddle][YFront] [XMiddle]= DONTCARE;//front
	data[ZMiddle][YFront] [XRight] = DONTCARE;
		
	data[ZTop]   [YBack]  [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZTop]   [YBack]  [XMiddle]= DONTCARE;//BACKGROUND;
	data[ZTop]   [YBack]  [XRight] = DONTCARE;//BACKGROUND;
	data[ZTop]   [YMiddle][XLeft]  = DONTCARE;//BACKGROUND;
	data[ZTop]   [YMiddle][XMiddle]= FOREGROUND;//top
	data[ZTop]   [YMiddle][XRight] = DONTCARE;
	data[ZTop]   [YFront] [XLeft]  = DONTCARE;//BACKGROUND;
	data[ZTop]   [YFront] [XMiddle]= DONTCARE;//BACKGROUND;
	data[ZTop]   [YFront] [XRight] = DONTCARE;//BACKGROUND;
	break;};

case 14:{
    data[ZBottom][YBack]  [XLeft]  = DONTCARE;
    data[ZBottom][YBack]  [XMiddle]= DONTCARE;
    data[ZBottom][YBack]  [XRight] = FOREGROUND;
    data[ZBottom][YMiddle][XLeft]  = DONTCARE;
    data[ZBottom][YMiddle][XMiddle]= FOREGROUND;
    data[ZBottom][YMiddle][XRight] = FOREGROUND;
    data[ZBottom][YFront] [XLeft]  = DONTCARE;
    data[ZBottom][YFront] [XMiddle]= FOREGROUND;
    data[ZBottom][YFront] [XRight] = DONTCARE;

    data[ZMiddle][YBack]  [XLeft]  = DONTCARE;;
    data[ZMiddle][YBack]  [XMiddle]= FOREGROUND;
    data[ZMiddle][YBack]  [XRight] = FOREGROUND;
    data[ZMiddle][YMiddle][XLeft]  = DONTCARE;;
    data[ZMiddle][YMiddle][XMiddle]= FOREGROUND;
    data[ZMiddle][YMiddle][XRight] = DONTCARE;;
    data[ZMiddle][YFront] [XLeft]  = DONTCARE;
    data[ZMiddle][YFront] [XMiddle]= DONTCARE;;
    data[ZMiddle][YFront] [XRight] = DONTCARE;;

    data[ZTop]   [YBack]  [XLeft]  = DONTCARE;;
    data[ZTop]   [YBack]  [XMiddle]= FOREGROUND;
    data[ZTop]   [YBack]  [XRight] = DONTCARE;;
    data[ZTop]   [YMiddle][XLeft]  = DONTCARE;;
    data[ZTop]   [YMiddle][XMiddle]= DONTCARE;;
    data[ZTop]   [YMiddle][XRight] = DONTCARE;;
    data[ZTop]   [YFront] [XLeft]  = BACKGROUND;
    data[ZTop]   [YFront] [XMiddle]= BACKGROUND;
    data[ZTop]   [YFront] [XRight] = BACKGROUND;
    break;};



case 15:{
    data[ZBottom][YBack]  [XLeft]  = DONTCARE;
    data[ZBottom][YBack]  [XMiddle]= DONTCARE;
    data[ZBottom][YBack]  [XRight] = FOREGROUND;
    data[ZBottom][YMiddle][XLeft]  = DONTCARE;
    data[ZBottom][YMiddle][XMiddle]= DONTCARE;
    data[ZBottom][YMiddle][XRight] = FOREGROUND;
    data[ZBottom][YFront] [XLeft]  = DONTCARE;
    data[ZBottom][YFront] [XMiddle]= DONTCARE;
    data[ZBottom][YFront] [XRight] = FOREGROUND;

    data[ZMiddle][YBack]  [XLeft]  = DONTCARE;
    data[ZMiddle][YBack]  [XMiddle]= FOREGROUND;
    data[ZMiddle][YBack]  [XRight] = DONTCARE;
    data[ZMiddle][YMiddle][XLeft]  = DONTCARE;
    data[ZMiddle][YMiddle][XMiddle]= FOREGROUND;
    data[ZMiddle][YMiddle][XRight] = DONTCARE;
    data[ZMiddle][YFront] [XLeft]  = DONTCARE;
    data[ZMiddle][YFront] [XMiddle]= FOREGROUND;
    data[ZMiddle][YFront] [XRight] = DONTCARE;

    data[ZTop]   [YBack]  [XLeft]  = DONTCARE;
    data[ZTop]   [YBack]  [XMiddle]= DONTCARE;
    data[ZTop]   [YBack]  [XRight] = DONTCARE;
    data[ZTop]   [YMiddle][XLeft]  = DONTCARE;
    data[ZTop]   [YMiddle][XMiddle]= DONTCARE;
    data[ZTop]   [YMiddle][XRight] = DONTCARE;
    data[ZTop]   [YFront] [XLeft]  = DONTCARE;
    data[ZTop]   [YFront] [XMiddle]= DONTCARE;
    data[ZTop]   [YFront] [XRight] = DONTCARE;
    break;};
    
    

    case 16:{
        data[ZBottom][YBack]  [XLeft]  = BACKGROUND;
        data[ZBottom][YBack]  [XMiddle]= BACKGROUND;
        data[ZBottom][YBack]  [XRight] = BACKGROUND;
        data[ZBottom][YMiddle][XLeft]  = BACKGROUND;
        data[ZBottom][YMiddle][XMiddle]= BACKGROUND;
        data[ZBottom][YMiddle][XRight] = BACKGROUND;
        data[ZBottom][YFront] [XLeft]  = BACKGROUND;
        data[ZBottom][YFront] [XMiddle]= BACKGROUND;
        data[ZBottom][YFront] [XRight] = BACKGROUND;

        data[ZMiddle][YBack]  [XLeft]  = BACKGROUND;
        data[ZMiddle][YBack]  [XMiddle]= BACKGROUND;
        data[ZMiddle][YBack]  [XRight] = BACKGROUND;
        data[ZMiddle][YMiddle][XLeft]  = BACKGROUND;
        data[ZMiddle][YMiddle][XMiddle]= FOREGROUND;
        data[ZMiddle][YMiddle][XRight] = BACKGROUND;
        data[ZMiddle][YFront] [XLeft]  = BACKGROUND;
        data[ZMiddle][YFront] [XMiddle]= BACKGROUND;
        data[ZMiddle][YFront] [XRight] = BACKGROUND;

        data[ZTop]   [YBack]  [XLeft]  = BACKGROUND;
        data[ZTop]   [YBack]  [XMiddle]= BACKGROUND;
        data[ZTop]   [YBack]  [XRight] = BACKGROUND;
        data[ZTop]   [YMiddle][XLeft]  = DONTCARE;
        data[ZTop]   [YMiddle][XMiddle]= DONTCARE;
        data[ZTop]   [YMiddle][XRight] = DONTCARE;
        data[ZTop]   [YFront] [XLeft]  = DONTCARE;
        data[ZTop]   [YFront] [XMiddle]= FOREGROUND;
        data[ZTop]   [YFront] [XRight] = DONTCARE;
        break;};


    case 17:{
        data[ZBottom][YBack]  [XLeft]  = BACKGROUND;
        data[ZBottom][YBack]  [XMiddle]= BACKGROUND;
        data[ZBottom][YBack]  [XRight] = BACKGROUND;
        data[ZBottom][YMiddle][XLeft]  = BACKGROUND;
        data[ZBottom][YMiddle][XMiddle]= BACKGROUND;
        data[ZBottom][YMiddle][XRight] = BACKGROUND;
        data[ZBottom][YFront] [XLeft]  = BACKGROUND;
        data[ZBottom][YFront] [XMiddle]= BACKGROUND;
        data[ZBottom][YFront] [XRight] = BACKGROUND;

        data[ZMiddle][YBack]  [XLeft]  = BACKGROUND;
        data[ZMiddle][YBack]  [XMiddle]= BACKGROUND;
        data[ZMiddle][YBack]  [XRight] = BACKGROUND;
        data[ZMiddle][YMiddle][XLeft]  = BACKGROUND;
        data[ZMiddle][YMiddle][XMiddle]= FOREGROUND;
        data[ZMiddle][YMiddle][XRight] = BACKGROUND;
        data[ZMiddle][YFront] [XLeft]  = BACKGROUND;
        data[ZMiddle][YFront] [XMiddle]= BACKGROUND;
        data[ZMiddle][YFront] [XRight] = BACKGROUND;

        data[ZTop]   [YBack]  [XLeft]  = BACKGROUND;
        data[ZTop]   [YBack]  [XMiddle]= BACKGROUND;
        data[ZTop]   [YBack]  [XRight] = BACKGROUND;
        data[ZTop]   [YMiddle][XLeft]  = BACKGROUND;
        data[ZTop]   [YMiddle][XMiddle]= DONTCARE;
        data[ZTop]   [YMiddle][XRight] = DONTCARE;
        data[ZTop]   [YFront] [XLeft]  = BACKGROUND;
        data[ZTop]   [YFront] [XMiddle]= DONTCARE;
        data[ZTop]   [YFront] [XRight] = FOREGROUND;
        break;};
        

    case 18:{
        data[ZBottom][YBack]  [XLeft]  = DONTCARE;
        data[ZBottom][YBack]  [XMiddle]= DONTCARE;
        data[ZBottom][YBack]  [XRight] = FOREGROUND;
        data[ZBottom][YMiddle][XLeft]  = DONTCARE;
        data[ZBottom][YMiddle][XMiddle]= DONTCARE;
        data[ZBottom][YMiddle][XRight] = DONTCARE;
        data[ZBottom][YFront] [XLeft]  = FOREGROUND;
        data[ZBottom][YFront] [XMiddle]= FOREGROUND;
        data[ZBottom][YFront] [XRight] = DONTCARE;

        data[ZMiddle][YBack]  [XLeft]  = DONTCARE;
        data[ZMiddle][YBack]  [XMiddle]= DONTCARE;
        data[ZMiddle][YBack]  [XRight] = FOREGROUND;
        data[ZMiddle][YMiddle][XLeft]  = FOREGROUND;
        data[ZMiddle][YMiddle][XMiddle]= FOREGROUND;
        data[ZMiddle][YMiddle][XRight] = DONTCARE;
        data[ZMiddle][YFront] [XLeft]  = FOREGROUND;
        data[ZMiddle][YFront] [XMiddle]= DONTCARE;
        data[ZMiddle][YFront] [XRight] = DONTCARE;

        data[ZTop]   [YBack]  [XLeft]  = FOREGROUND;
        data[ZTop]   [YBack]  [XMiddle]= FOREGROUND;
        data[ZTop]   [YBack]  [XRight] = DONTCARE;
        data[ZTop]   [YMiddle][XLeft]  = DONTCARE;
        data[ZTop]   [YMiddle][XMiddle]= DONTCARE;
        data[ZTop]   [YMiddle][XRight] = DONTCARE;
        data[ZTop]   [YFront] [XLeft]  = DONTCARE;
        data[ZTop]   [YFront] [XMiddle]= DONTCARE;
        data[ZTop]   [YFront] [XRight] = BACKGROUND;
        break;};


    case 19:{
        data[ZBottom][YBack]  [XLeft]  = DONTCARE;
        data[ZBottom][YBack]  [XMiddle]= FOREGROUND;
        data[ZBottom][YBack]  [XRight] = FOREGROUND;
        data[ZBottom][YMiddle][XLeft]  = DONTCARE;
        data[ZBottom][YMiddle][XMiddle]= FOREGROUND;
        data[ZBottom][YMiddle][XRight] = FOREGROUND;
        data[ZBottom][YFront] [XLeft]  = DONTCARE;
        data[ZBottom][YFront] [XMiddle]= DONTCARE;
        data[ZBottom][YFront] [XRight] = DONTCARE;

        data[ZMiddle][YBack]  [XLeft]  = DONTCARE;
        data[ZMiddle][YBack]  [XMiddle]= FOREGROUND;
        data[ZMiddle][YBack]  [XRight] = FOREGROUND;
        data[ZMiddle][YMiddle][XLeft]  = DONTCARE;
        data[ZMiddle][YMiddle][XMiddle]= FOREGROUND;
        data[ZMiddle][YMiddle][XRight] = DONTCARE;
        data[ZMiddle][YFront] [XLeft]  = DONTCARE;
        data[ZMiddle][YFront] [XMiddle]= DONTCARE;
        data[ZMiddle][YFront] [XRight] = DONTCARE;

        data[ZTop]   [YBack]  [XLeft]  = DONTCARE;
        data[ZTop]   [YBack]  [XMiddle]= FOREGROUND;
        data[ZTop]   [YBack]  [XRight] = FOREGROUND;
        data[ZTop]   [YMiddle][XLeft]  = DONTCARE;
        data[ZTop]   [YMiddle][XMiddle]= DONTCARE;
        data[ZTop]   [YMiddle][XRight] = DONTCARE;
        data[ZTop]   [YFront] [XLeft]  = BACKGROUND;
        data[ZTop]   [YFront] [XMiddle]= BACKGROUND;
        data[ZTop]   [YFront] [XRight] = BACKGROUND;
        break;};

    case 20:{
        data[ZBottom][YBack]  [XLeft]  = DONTCARE;
        data[ZBottom][YBack]  [XMiddle]= FOREGROUND;
        data[ZBottom][YBack]  [XRight] = DONTCARE;
        data[ZBottom][YMiddle][XLeft]  = DONTCARE;
        data[ZBottom][YMiddle][XMiddle]= FOREGROUND;
        data[ZBottom][YMiddle][XRight] = DONTCARE;
        data[ZBottom][YFront] [XLeft]  = DONTCARE;
        data[ZBottom][YFront] [XMiddle]= DONTCARE;
        data[ZBottom][YFront] [XRight] = DONTCARE;

        data[ZMiddle][YBack]  [XLeft]  = DONTCARE;
        data[ZMiddle][YBack]  [XMiddle]= FOREGROUND;
        data[ZMiddle][YBack]  [XRight] = DONTCARE;
        data[ZMiddle][YMiddle][XLeft]  = DONTCARE;
        data[ZMiddle][YMiddle][XMiddle]= FOREGROUND;
        data[ZMiddle][YMiddle][XRight] = DONTCARE;
        data[ZMiddle][YFront] [XLeft]  = BACKGROUND;
        data[ZMiddle][YFront] [XMiddle]= BACKGROUND;
        data[ZMiddle][YFront] [XRight] = BACKGROUND;

        data[ZTop]   [YBack]  [XLeft]  = DONTCARE;
        data[ZTop]   [YBack]  [XMiddle]= FOREGROUND;
        data[ZTop]   [YBack]  [XRight] = DONTCARE;
        data[ZTop]   [YMiddle][XLeft]  = DONTCARE;
        data[ZTop]   [YMiddle][XMiddle]= DONTCARE;
        data[ZTop]   [YMiddle][XRight] = DONTCARE;
        data[ZTop]   [YFront] [XLeft]  = BACKGROUND;
        data[ZTop]   [YFront] [XMiddle]= BACKGROUND;
        data[ZTop]   [YFront] [XRight] = BACKGROUND;
        break;};

    case 21:{
        data[ZBottom][YBack]  [XLeft]  = DONTCARE;
        data[ZBottom][YBack]  [XMiddle]= FOREGROUND;
        data[ZBottom][YBack]  [XRight] = FOREGROUND;
        data[ZBottom][YMiddle][XLeft]  = DONTCARE;
        data[ZBottom][YMiddle][XMiddle]= DONTCARE;
        data[ZBottom][YMiddle][XRight] = FOREGROUND;
        data[ZBottom][YFront] [XLeft]  = BACKGROUND;
        data[ZBottom][YFront] [XMiddle]= DONTCARE;
        data[ZBottom][YFront] [XRight] = DONTCARE;

        data[ZMiddle][YBack]  [XLeft]  = FOREGROUND;
        data[ZMiddle][YBack]  [XMiddle]= DONTCARE;
        data[ZMiddle][YBack]  [XRight] = DONTCARE;
        data[ZMiddle][YMiddle][XLeft]  = DONTCARE;
        data[ZMiddle][YMiddle][XMiddle]= FOREGROUND;
        data[ZMiddle][YMiddle][XRight] = DONTCARE;
        data[ZMiddle][YFront] [XLeft]  = BACKGROUND;
        data[ZMiddle][YFront] [XMiddle]= DONTCARE;
        data[ZMiddle][YFront] [XRight] = FOREGROUND;

        data[ZTop]   [YBack]  [XLeft]  = BACKGROUND;
        data[ZTop]   [YBack]  [XMiddle]= BACKGROUND;
        data[ZTop]   [YBack]  [XRight] = BACKGROUND;
        data[ZTop]   [YMiddle][XLeft]  = BACKGROUND;
        data[ZTop]   [YMiddle][XMiddle]= BACKGROUND;
        data[ZTop]   [YMiddle][XRight] = BACKGROUND;
        data[ZTop]   [YFront] [XLeft]  = BACKGROUND;
        data[ZTop]   [YFront] [XMiddle]= BACKGROUND;
        data[ZTop]   [YFront] [XRight] = BACKGROUND;
        break;};

    case 22:{
        data[ZBottom][YBack]  [XLeft]  = DONTCARE;
        data[ZBottom][YBack]  [XMiddle]= DONTCARE;
        data[ZBottom][YBack]  [XRight] = DONTCARE;
        data[ZBottom][YMiddle][XLeft]  = DONTCARE;
        data[ZBottom][YMiddle][XMiddle]= FOREGROUND;
        data[ZBottom][YMiddle][XRight] = DONTCARE;
        data[ZBottom][YFront] [XLeft]  = DONTCARE;
        data[ZBottom][YFront] [XMiddle]= DONTCARE;
        data[ZBottom][YFront] [XRight] = DONTCARE;

        data[ZMiddle][YBack]  [XLeft]  = DONTCARE;
        data[ZMiddle][YBack]  [XMiddle]= DONTCARE;
        data[ZMiddle][YBack]  [XRight] = DONTCARE;
        data[ZMiddle][YMiddle][XLeft]  = DONTCARE;
        data[ZMiddle][YMiddle][XMiddle]= FOREGROUND;
        data[ZMiddle][YMiddle][XRight] = DONTCARE;
        data[ZMiddle][YFront] [XLeft]  = FOREGROUND;
        data[ZMiddle][YFront] [XMiddle]= DONTCARE;
        data[ZMiddle][YFront] [XRight] = DONTCARE;

        data[ZTop]   [YBack]  [XLeft]  = BACKGROUND;
        data[ZTop]   [YBack]  [XMiddle]= BACKGROUND;
        data[ZTop]   [YBack]  [XRight] = BACKGROUND;
        data[ZTop]   [YMiddle][XLeft]  = DONTCARE;
        data[ZTop]   [YMiddle][XMiddle]= DONTCARE;
        data[ZTop]   [YMiddle][XRight] = BACKGROUND;
        data[ZTop]   [YFront] [XLeft]  = FOREGROUND;
        data[ZTop]   [YFront] [XMiddle]= DONTCARE;
        data[ZTop]   [YFront] [XRight] = BACKGROUND;
        break;};
        

        
}


    
}





Primitive::
Primitive(unsigned char d[3][3][3])
{
    derivatedFrom=-1;
/*
for (short x=0;x<3;x++)
	for(short y=0;y<3;y++)
		for(short z=0;z<3;z++)
			data[x][y][z]=d[x][y][z];
		
*/
//This should be faster
memcpy(data,d,3*3*3*sizeof(unsigned char));



}

Primitive::
Primitive()
{
    derivatedFrom=-1;
    for (short x=0;x<3;x++)
        for(short y=0;y<3;y++)
            for(short z=0;z<3;z++)
                data[x][y][z]=BACKGROUND;
};







int operator==(const Primitive &P1,const Primitive &P2)
{

for (short x=0;x<3;x++)
	for(short y=0;y<3;y++)
		for(short z=0;z<3;z++)
			{if ( (P1.data[x][y][z]!=P2.data[x][y][z]) && (P1.data[x][y][z]!=DONTCARE) && (P2.data[x][y][z]!=DONTCARE ) ) return 0;};
		
return 1;
}


int TheyAreTheSame(const Primitive &P1,const Primitive &P2)
{
for (short x=0;x<3;x++)
	for(short y=0;y<3;y++)
		for(short z=0;z<3;z++)
			{if ( (P1.data[x][y][z]!=P2.data[x][y][z])) return 0;};
		
return 1;
}




int
Primitive::
Rotate(short axis,short angle)
{
if (angle==0) return 0;
unsigned char help[3][3][3];
for (short x=0;x<3;x++)
	for(short y=0;y<3;y++)
		for(short z=0;z<3;z++)
			help[x][y][z]=data[x][y][z];

for (short x=0;x<3;x++)
	for(short y=0;y<3;y++)
		for(short z=0;z<3;z++)
			switch (axis)
				{
				case XAxis: switch (angle)
									{
									case  90:help[x][y][z]=data[x][z][y];break;
									case 180:help[x][y][z]=data[x][y][2-z];break;
									case 270:help[x][y][z]=data[x][2-y][2-z];break;
									};
								 break;
				
				case YAxis: switch (angle)
									{
									case  90:help[x][y][z]=data[z][y][x];break;
									case 180:help[x][y][z]=data[x][y][2-z];break;
									case 270:help[x][y][z]=data[2-x][y][2-z];break;
									};
								 break;
				
				case ZAxis: switch (angle)
									{
									case  90:help[x][y][z]=data[y][x][z];break;
									case 180:help[x][y][z]=data[2-x][y][z];break;
									case 270:help[x][y][z]=data[2-x][2-y][z];break;
									};
								 break;
				
				default:
				 {cout << "Error!!!" << endl;exit(1);}
				
				}

for (short x=0;x<3;x++)
	for(short y=0;y<3;y++)
		for(short z=0;z<3;z++)
			data[x][y][z]=help[x][y][z];
			
return 0;
}


int
Primitive::
Mirror(short plane)
{
if (plane==0) return 0;
unsigned char help[3][3][3];

for (short x=0;x<3;x++)
	for(short y=0;y<3;y++)
		for(short z=0;z<3;z++)
			help[x][y][z]=data[x][y][z];
			


for (short x=0;x<3;x++)
	for(short y=0;y<3;y++)
		for(short z=0;z<3;z++)
			switch (plane)
				{
				case YZPlane: help[x][y][z]=data[2-x][y][z]; break;
				
				case XZPlane: help[x][y][z]=data[x][2-y][z]; break;
				
				case YXPlane: help[x][y][z]=data[x][y][2-z]; break;
				
				}

for (short x=0;x<3;x++)
	for(short y=0;y<3;y++)
		for(short z=0;z<3;z++)
			data[x][y][z]=help[x][y][z];
			
return 0;
}





const char headerTemplate[500] = 
"ONOM2KDATA\nGRIDTYPE UNSTRUCTURED_MIX\nSYMMETRY NONE\nSIZE %d %d\nDATANUMBER 4\nZ\nNAME location\nLABEL x\nMIN %d.0\nMAX %d.0\nZ\nNAME location\nLABEL y\nMIN %d.0\nMAX %d.0\nZ\nNAME location\nLABEL z\nMIN %d.0\nMAX %d.0\nZ\nNAME location\nLABEL c\nMIN 0.0\nMAX %d.0\nTEXT1 Compartmentlist\nTEXT2 \n";



int 
Primitive::
writeOnom2K(char* Name)
{
 int nr=0;
 
 for (int x=0;x<3;x++)
	for(int y=0;y<3;y++)
		for(int z=0;z<3;z++)
			if (data[x][y][z]!=BACKGROUND) nr++;
	
  Vector<int> start(3);
  Vector<int> size(3);
  size[1]=1;
  size[2]=1;
  size[3]=1;
  
  FILE *fptr;
  char header[1000];
  char buffer[300];
  int maxcolor=4;
  if (Name == NULL)
    fptr = stdout;
  else
    fptr = fopen(Name, "w");
  if (fptr==NULL) return 1;
  sprintf(header,headerTemplate, nr*8, nr*6,0,3,0,3,0,3,maxcolor);
  fprintf(fptr, header);
  
  fprintf(fptr,"DATA\n");
  fprintf(fptr,"x\ty\tz\tc\n");
  int i=0;
  
  for (int x=0;x<3;x++)
	for(int y=0;y<3;y++)
		for(int z=0;z<3;z++)
	 if (data[z][y][x]!=BACKGROUND)
    {
      i++;
      int color;
      if (data[z][y][x]==FOREGROUND) color=1;
      if (data[z][y][x]==DONTCARE) color=2;

      
      
      start[1]=x;
      start[2]=y;
      start[3]=z;
      //--size[1];
      //--size[2]; 
      //--size[3];
      sprintf(buffer,"%d.0\t%d.0\t%d.0\t%d.0\n",start[1],start[2],start[3],color); //0
      fprintf(fptr,buffer);
      sprintf(buffer,"%d.0\t%d.0\t%d.0\t%d.0\n",start[1]+size[1],start[2],start[3],color); //1
      fprintf(fptr,buffer);
      sprintf(buffer,"%d.0\t%d.0\t%d.0\t%d.0\n",start[1]+size[1],start[2]+size[2],start[3],color); //2
      fprintf(fptr,buffer);
      sprintf(buffer,"%d.0\t%d.0\t%d.0\t%d.0\n",start[1],start[2]+size[2],start[3],color);//3
      fprintf(fptr,buffer);
      
      sprintf(buffer,"%d.0\t%d.0\t%d.0\t%d.0\n",start[1],start[2],start[3]+size[3],color);//4
      fprintf(fptr,buffer);
      sprintf(buffer,"%d.0\t%d.0\t%d.0\t%d.0\n",start[1]+size[1],start[2],start[3]+size[3],color);//5
      fprintf(fptr,buffer);
      sprintf(buffer,"%d.0\t%d.0\t%d.0\t%d.0\n",start[1]+size[1],start[2]+size[2],start[3]+size[3],color);//6
      fprintf(fptr,buffer);
      sprintf(buffer,"%d.0\t%d.0\t%d.0\t%d.0\n",start[1],start[2]+size[2],start[3]+size[3],color);//7
      fprintf(fptr,buffer);
    };
  fprintf(fptr,"ELEMENTS\n");
  int j;
  for (int i=0;i<nr;i++)
    {
      j=i*8;
      sprintf(buffer,"%d\t%d\t%d\t%d\n",j+0,j+1,j+2,j+3); //deckel
      fprintf(fptr,buffer);
      sprintf(buffer,"%d\t%d\t%d\t%d\n",j+0,j+4,j+5,j+1); 
      fprintf(fptr,buffer);
      sprintf(buffer,"%d\t%d\t%d\t%d\n",j+5,j+6,j+2,j+1); 
      fprintf(fptr,buffer);
      sprintf(buffer,"%d\t%d\t%d\t%d\n",j+7,j+6,j+2,j+3); 
      fprintf(fptr,buffer);
      sprintf(buffer,"%d\t%d\t%d\t%d\n",j+4,j+5,j+6,j+7); 
      fprintf(fptr,buffer);
      sprintf(buffer,"%d\t%d\t%d\t%d\n",j+0,j+4,j+7,j+3); 
      fprintf(fptr,buffer);
    }
 
  fprintf(fptr,"END\n");
  fclose(fptr);
  return 0;
}


int
Primitive::
GetDataField(unsigned char d[3][3][3])
{
    d=data;
    return 0;
}






