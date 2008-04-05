
#include "testcubes.h"

TestCube2D::
TestCube2D(int Lnr) :  DataCube(2)
{ 
  nr=Lnr;
   switch (nr) 
    {
    case SIMPLEY2D: 
      SimpleY2D();
      break;
    case SIMPLEY2DBREIT: 
      SimpleY2Dbreit();
      break;
    }

}


TestCube2D::
~TestCube2D()
{
}

int 
TestCube2D::
StandardSize()
{
  start[1]=0;
  start[2]=0;
  size[1]=125;
  size[2]=125;
  d[1]=1.0;
  d[2]=1.0;
  return MallocCube();


}

void
TestCube2D::
SimpleY2D()
{
  int y; 
  if (StandardSize()) exit(1);
  for (y=0;y<65;y++)
    (*this)(64,y)=1.0;
  for (y=65;y<125;y++)
    {
      (*this)(129-y,y)=1.0;
      (*this)(y,y)=1.0;
    }
}

void
TestCube2D::
SimpleY2Dbreit()
{
  int y;
  if (StandardSize()) exit(1);
  for (y=0;y<124;y++)
   (*this)(64,y)=1.0;
  
  for (y=65;y<125;y++)
    {
      (*this) (y,0)=1.0;
      (*this)(129-y,y)=1.0;
      (*this)(130-y,y)=1.0;
      (*this)(y,y)=1.0;
      (*this)(y-1,y)=1.0;
    }
  
}
void 
TestCube2D::  
showCube()
{
  int x,y;
  cout << "Show TestCube 2D" << endl;
  cout << "Size:" << size[1] << "*" << size[2] << endl;
  for (x=0;x<size[1];x++)
    {
    for(y=0;y<size[2];y++)
      {
	if ((*this)(x,y)==1.0) 
	  {
	    cout<<"*";
	    continue;
	  }
	cout << ".";
      }
    cout << endl;
    }
  cout << endl;

}




ostream &operator<<(ostream &ostr, const TestCube2D &TC)
{
  ostr << "TestDatacube 2D" << endl;
  switch (TC.nr) 
    {
      case 1: ostr<< "1:Simple Y 2D 125*125" << endl;break;
    }
  ostr << "Dimension:" << TC.dim << endl;
  ostr << "Size:" << endl << TC.size << endl;
  ostr << "d:" << endl << TC.d << endl;
  ostr << "Flags:" << TC.flags << endl;

  return (ostr);  
}  



/* from here 3D Testcubes */

TestCube3D::
TestCube3D(int Lnr) :    DataCube(3)
{
  nr=Lnr;
  switch (nr) 
    {
    case SIMPLEY3D: SimpleY3D();break;
    case TUBEY3DX:            TubeY3D(DIRX,1);break;   
    case TUBEY3DY:            TubeY3D(DIRY,3);break;   
    case TUBEY3DZ:            TubeY3D(DIRZ,3);break; 
    case TUBEY3DX_VARIABLE:   TubeY3D_var(DIRX,4); break;
    case TUBEY3DXSMALL:   TubeY3D(DIRX,0);break;   
    case TUBEY3DYSMALL:   TubeY3D(DIRY,0);break;   
    case TUBEY3DZSMALL:   TubeY3D(DIRZ,0);break;   

    case SPHERE3D: Sphere3D(3);break;

    case SPHERE3DHOLES: Sphere3Dholes(1);break;

    case PLANE3D: plane3D(); break;

    case PLANE3DHOLES: plane3Dholes();break;
 
    case Y3D_3PIXEL_GABS: Y3d_3pixel_gabs();break;

    case TUBE_LOKAL_VALUE: TubeLocalValue() ;break;

    case TUBEY3DXANDNOTCONNECTED: TubeY3DNotConnected();break;
 
	 case TUBEYBIG: TubeYBig();break; 
    }

}


TestCube3D::
~TestCube3D()
{
}


int 
TestCube3D::
StandardSize()
{
  start[1]=0;
  start[2]=0;
  start[3]=0;
  size[1]=125;
  size[2]=125;
  size[3]=125;
  d[1]=1.0;
  d[2]=1.0;
  d[3]=1.0;
  return MallocCube();
}

void
TestCube3D::
SimpleY3D()
{
  int y,z;
  if (StandardSize()) exit(1);

  for (z=0;z<125;z++)
    {
      for (y=0;y<65;y++)
	(*this)(64,y,z)=1.0;
      for (y=65;y<125;y++)
	{
	  (*this)(129-y,y,z)=1.0;
	  (*this)(y,y,z)=1.0;
	}
    }
}


NeuraDataType 
&TestCube3D::
permutate(int p1,int p2, int p3, int dir)
{
 if (dir==DIRX) return (*this)(p1,p2,p3);
 if (dir==DIRY) return (*this)(p2,p1,p3);
 if (dir==DIRZ) return (*this)(p3,p2,p1);
 return (*this)(p1,p2,p3);
}  


void
TestCube3D::
TubeY3D(int dir,int wide)
{
  int param1,param2,param3;
  if (StandardSize()) exit(1);

  for (param3=50;param3<53;param3++)
    {
    
    for (param2=0;param2<65;param2++)
	    for (param1=-wide;param1<=wide;param1++) permutate(64+param1,param2,param3,dir)=1.0;
       
        
    
    for (param2=65;param2<125;param2++)
	  {
	   for (param1=-wide;param1<=wide;param1++) 
	      {
	         if (129-param2+param1>0 && 129-param2+param1<125)permutate(129-param2+param1,param2,param3,dir)=1.0;
	         if (param2+param1>0 && param2+param1<125) permutate(param2+param1,param2,param3,dir)=1.0;
	       }
	   }
    }
}



void
TestCube3D::
TubeY3D_var(int dir,int wide)
{
  int param1,param2,param3;
  if (StandardSize()) exit(1);

  for (param3=53;param3<57;param3++)
    {
    
    for (param2=0;param2<65;param2++)
	    for (param1=-(int)((double)param2/65.0*(double)wide);param1<=(int)((double)param2/65.0*(double)wide);param1++) permutate(64+param1,param2,param3,dir)=1.0;
       
        
    
    for (param2=65;param2<125;param2++)
	  {
	   for (param1=-(int)((double)(125.0-param2)/60.0*(double)wide);param1<=(int)((double)(125.0-param2)/60.0*(double)wide);param1++) 
	      {
	         if (129-param2+param1>0 && 129-param2+param1<125)permutate(129-param2+param1,param2,param3,dir)=1.0;
	         if (param2+param1>0 && param2+param1<125) permutate(param2+param1,param2,param3,dir)=1.0;
	       }
	   }
    }
}







int
TestCube3D::
Sphere3D(int wide)
{
  int param1,param2,param3;
  int MPX=60; 
  int MPY=60;
  int MPZ=60;
  int radius=55;
  if (StandardSize()) exit(1);
  
  for(param1=0;param1<125;param1++)
   for(param2=0;param2<125;param2++)
      for(param3=0;param3<125;param3++)
	 if ( (sqrt((double) (param1-MPX)*(param1-MPX)+(param2-MPY)*(param2-MPY)+(param3-MPZ)*(param3-MPZ))>(double) (radius-wide)  && (sqrt)((double) (param1-MPX)*(param1-MPX)+(param2-MPY)*(param2-MPY)+(param3-MPZ)*(param3-MPZ))<(double) (radius+wide))   )
	   (*this).SetData(param1,param2,param3,1.0); 

return 1; 
} 




int
TestCube3D::
Sphere3Dholes(int wide)
{
  double param1,param2,param3;
  int MPX=60; 
  int MPY=60;
  int MPZ=60;
  int radius=55;
  if (StandardSize()) exit(1);
  
  for(param1=0;param1<125;param1++)
   for(param2=0;param2<125;param2++)
      for(param3=0;param3<125;param3++)
	 if ( (sqrt((double) (param1-MPX)*(param1-MPX)+(param2-MPY)*(param2-MPY)+(param3-MPZ)*(param3-MPZ))>= (double) (radius-wide)  && (sqrt)((double) (param1-MPX)*(param1-MPX)+(param2-MPY)*(param2-MPY)+(param3-MPZ)*(param3-MPZ))<= (double) (radius+wide))   )
	   (*this).SetData(param1,param2,param3,1.0); 


  /*  for(param1=85;param1<125;param1++)
   for(param2=85;param2<125;param2++)
      for(param3=85;param3<125;param3++)
         if ( (sqrt((double) (param1-MPX)*(param1-MPX)+(param2-MPY)*(param2-MPY)+(param3-MPZ)*(param3-MPZ))>= (double) (radius-wide)  && (sqrt)((double) (param1-MPX)*(param1-MPX)+(param2-MPY)*(param2-MPY)+(param3-MPZ)*(param3-MPZ))<= (double) (radius+wide))   )
	 (*this).SetData(param1,param2,param3,0.0); */



  radius=25;
  

for(param1=0;param1<125;param1++)
   for(param2=0;param2<125;param2++)
      for(param3=0;param3<125;param3++)
	 if ( (sqrt((double) (param1-MPX)*(param1-MPX)+(param2-MPY)*(param2-MPY) <= (double) (radius-wide)  ))   )
	   (*this).SetData(param1,param2,param3,0.0); 

 radius=10;
 MPX=30;

for(param1=0;param1<125;param1++)
   for(param2=0;param2<125;param2++)
      for(param3=0;param3<125;param3++)
	 if ( (sqrt((double) (param1-MPX)*(param1-MPX)+(param2-MPY)*(param2-MPY) <= (double) (radius-wide)  ))   )
	   (*this).SetData(param1,param2,param3,0.0); 

 radius=15;
 MPY=30;
for(param1=0;param1<125;param1++)
   for(param2=0;param2<125;param2++)
      for(param3=0;param3<125;param3++)
	 if ( (sqrt((double) (param1-MPX)*(param1-MPX)+(param2-MPY)*(param2-MPY) <= (double) (radius-wide)  ))   )
	   (*this).SetData(param1,param2,param3,0.0); 




return 1; 
} 


int TestCube3D::
plane3D()
{

  int x, y, z;

  if(StandardSize()) exit(1);

  for(x=0; x<=125; x++)
    for (y=0; y<=125; y++)
      for(z=60;z<=60; z++)
	(*this).SetData(x, y, z, 1.0);
  return 1;
}


int TestCube3D::
plane3Dholes()
{

  int x, y, z;
  int mpx, mpy, mpz;
  int rad1 = 3;
  int rad2 = 4;
  int rad3 = 5;
  int rad4 = 6;
  int rad5 = 7;

  if(StandardSize()) exit(1);

  for(x=0; x<=125; x++)
    for (y=0; y<=125; y++)
      for(z=60;z<=60; z++)
	(*this).SetData(x, y, z, 1.0);

  mpx = 20;
  mpy = 20;
  mpz = 60;
  z = 60;

  for(x=0; x<=125; x++)
    for(y=0; y<=125; y++)
      if( (sqrt((double) (x-mpx)*(x-mpx)+(y-mpy)*(y-mpy)) <= rad1))
	  (*this).SetData(x,y,z,0.0);

  mpx = 80;
  mpy = 50;
  mpz = 60;
  z = 60;

  for(x=0; x<=125; x++)
    for(y=0; y<=125; y++)
      if( (sqrt((double) (x-mpx)*(x-mpx)+(y-mpy)*(y-mpy)) <= rad2))
	  (*this).SetData(x,y,z,0.0);


  mpx = 50;
  mpy = 40;
  mpz = 60;
  z = 60;

  for(x=0; x<=125; x++)
    for(y=0; y<=125; y++)
      if( (sqrt((double) (x-mpx)*(x-mpx)+(y-mpy)*(y-mpy)) <= rad3))
	  (*this).SetData(x,y,z,0.0);


  mpx = 30;
  mpy = 80;
  mpz = 60;
  z = 60;

  for(x=0; x<=125; x++)
    for(y=0; y<=125; y++)
      if( (sqrt((double) (x-mpx)*(x-mpx)+(y-mpy)*(y-mpy)) <= rad4))
	  (*this).SetData(x,y,z,0.0);


  mpx = 90;
  mpy = 90;
  mpz = 60;
  z = 60;

  for(x=0; x<=125; x++)
    for(y=0; y<=125; y++)
      if( (sqrt((double) (x-mpx)*(x-mpx)+(y-mpy)*(y-mpy)) <= rad5))
	  (*this).SetData(x,y,z,0.0);



  return 1;
}



int
TestCube3D::
Y3d_3pixel_gabs()
{
int i,j,k;  
  int SIZE = 65;
  Vector<int> size(3); 
  size[1] = SIZE; // size of datacube in x-direction
  size[2] = SIZE; // size of datacube in y-direction
  size[3] = SIZE; // size of datacube in z-direction
  
  Vector<double> spaceing(3);
  spaceing[1]=1.0;
  spaceing[2]=1.0;
  spaceing[3]=1.0;

  (*this).SetCubeSizeAndSpaceing(size,spaceing); 
  int code = (*this).MallocCube();
  if ( code ) return code;

  for ( k = 0; k < SIZE; k++ ){
    for ( j = 0; j < SIZE; j++ ){
      for ( i = 0; i < SIZE; i++ ){

	if ( 
	    ( ( k >= 31 ) && ( k <= 33 ) )
	    &&
	     ( ( ( i >= 31 ) && ( i <= 33 ) ) && ( ( j < 15 ) || ( ( j > 17 ) && ( j < 31 ) ) ) ) 
	    )
	  {
	    (*this).SetData(i,j,k,1.0);
	  }
	if (  
	    ( ( k >= 31 ) && ( k <= 33 ) )
	    &&
	    ( ( i == j ) && ( ( ( i > 33 ) && ( i < 47 ) ) || ( i > 49 ) ) ) 
	    )
	  {
	    (*this).SetData(i,j+2,k,1.0);
	    (*this).SetData(i-1,j+1,k,1.0); (*this).SetData(i,j+1,k,1.0); (*this).SetData(i+1,j+1,k,1.0);
	    (*this).SetData(i,j,k,1.0); (*this).SetData(i+1,j,k,1.0); (*this).SetData(i+2,j,k,1.0);
	    (*this).SetData(i+1,j-1,k,1.0);	    
	  }	
	if ( 
	    ( ( k >= 31 ) && ( k <= 33 ) )
	    &&
	    ( ( i == 64-j ) && ( ( i <= 30 ) || ( ( i >= 28 ) && ( i < 31 ) ) ) )
	    )
	  {
	    (*this).SetData(i,j+2,k,1.0);
	    (*this).SetData(i-1,j+1,k,1.0); (*this).SetData(i,j+1,k,1.0); (*this).SetData(i+1,j+1,k,1.0);
	    (*this).SetData(i-2,j,k,1.0); (*this).SetData(i-1,j,k,1.0); (*this).SetData(i,j,k,1.0);
	    (*this).SetData(i-1,j-1,k,1.0);
	  }
	
      }
    }
  }

  return OK;
}

int 
TestCube3D::
TubeLocalValue()
{
  int wide=5; 
  int dir=DIRX;
  int param1,param2,param3;
  int MPX=60; 
  int MPY=60;
  int MPZ=60;
  int length=20;
  int dentritewide=1;
	
  if (StandardSize()) exit(1);
 
  //noisy background
  /*for(param1=0;param1<125;param1++)
    {
      for(param2=0;param2<125;param2++)
	{
	  for(param3=0;param3<125;param3++)
	    {
	      if (1.0-1.0/(0.01*param2+1.0)<0.4)
		(*this)(param1,param2,param3)=1.0-1.0/(0.01*param2+1.0);
	      else(*this)(param1,param2,param3)=1.0/(0.01*param2+1.0)-0.4;
	    }  
	}
    }
  */  
  

 //circle in 1.0
 for(param1=0;param1<125;param1++)
   for(param2=0;param2<125;param2++)
     for(param3=0;param3<125;param3++)
       if (sqrt((double) (param1-MPX)*(param1-MPX)+(param2-MPY)*(param2-MPY)+(param3-MPZ)*(param3-MPZ))<=(double) wide)   
	  	 (*this)(param1,param2,param3)=1.0;


for(param1=MPX-length;param1<MPX+length;param1++)
	for(int i=-dentritewide; i<=dentritewide; i++)
		for(int j=-dentritewide; j<=dentritewide; j++)
			(*this)(param1,MPY+i,MPZ+j)=1.0;

for(param2=MPY-length;param2<MPY+length;param2++)
	for(int i=-dentritewide; i<=dentritewide; i++)
		for(int j=-dentritewide; j<=dentritewide; j++)
			(*this)(MPX+i, param2,MPZ+j)=1.0;

for(param3=MPZ-length;param3<MPZ+length;param3++)
	for(int k=-dentritewide; k<=dentritewide; k++)
		for(int j=-dentritewide; j<=dentritewide; j++)
			(*this)(MPX+k, MPY+j, param3)=1.0;

  for (int i=-20; i<=20;i++)
	for(int k=-dentritewide; k<=dentritewide; k++)
		for(int j=-dentritewide; j<=dentritewide; j++)
			for(int h=-dentritewide; h<=dentritewide; h++)
				(*this)(MPX+i+k, MPY+i+j, MPZ+i+h)=1.0;	 
  
for (int i=-20; i<=20;i++)
for(int k=-dentritewide; k<=dentritewide; k++)
		for(int j=-dentritewide; j<=dentritewide; j++)
			for(int h=-dentritewide; h<=dentritewide; h++)
				(*this)(MPX-i+k, MPY+i+j, MPZ+i+h)=1.0;

for (int i=-20; i<=20;i++)
  for(int k=-dentritewide; k<=dentritewide; k++)
		for(int j=-dentritewide; j<=dentritewide; j++)
			for(int h=-dentritewide; h<=dentritewide; h++)
				(*this)(MPX+i+j, MPY-i+k, MPZ+i+h)=1.0;	
	
for (int i=-20; i<=20;i++)
for(int k=-dentritewide; k<=dentritewide; k++)
		for(int j=-dentritewide; j<=dentritewide; j++)
			for(int h=-dentritewide; h<=dentritewide; h++)
				(*this)(MPX+i+j, MPY+i+k, MPZ-i+h)=1.0;		

 // Y in 1.0
/*for (param3=50;param3<60;param3++)
    {
 
     for (param2=0;param2<65;param2++)
        for (param1=-wide;param1<=wide;param1++) permutate(64+param1,param2,param3,dir)=1.0;
     
     for (param2=65;param2<125;param2++)
      {
	  for (param1=-wide;param1<=wide;param1++) 
		{
		if (129-param2+param1>0 && 129-param2+param1<125)permutate(129-param2+param1,param2,param3,dir)=1.0;
		if (param2+param1>0 && param2+param1<125) permutate(param2+param1,param2,param3,dir)=1.0;
	     }
	}
   }
*/
 return 0;
}

int
TestCube3D::
TubeY3DNotConnected()
{
    TubeY3D(DIRX,3);
    int param1;
    for(param1=0;param1<125;param1++)
      {
        (*this)(param1,30,30)=1.0;

      };

    return 0;
}



int
TestCube3D::
TubeYBig()
{
	int dir=DIRX;
	int wide=4;
  int offset=90;
  start[1]=0;
  start[2]=0;
  start[3]=0;
  size[1]=256;
  size[2]=256;
  size[3]=100;
  d[1]=1.0;
  d[2]=1.0;
  d[3]=1.0;
  MallocCube();
 	int param1,param2,param3;
  
  param3=0;
  /*
  for (param2=0;param2<256;param2++)
  for (param1=0;param1<256;param1++)
 		permutate(param1,param2,param3,dir)=1.0;
  */

  for (param3=50;param3<60;param3++)
    {
    
    for (param2=offset;param2<offset+65;param2++)
	    for (param1=-wide;param1<=wide;param1++) permutate((256/2-2*wide)+param1,param2,param3,dir)=1.0;
       
        
    
    for (param2=offset+65;param2<offset+125;param2++)
	  {
	   for (param1=-wide;param1<=wide;param1++) 
	      {
	         permutate(18+256-param2+param1,param2,param3,dir)=1.0;
	         permutate(+10+param2+param1-(offset/2),param2,param3,dir)=1.0;
	       }
	   }
    }
	



	return 0;
}

ostream &operator<<(ostream &ostr, const TestCube3D &TC)
{
  ostr << "TestDatacube" << endl;
  switch (TC.nr) 
    {
      case 1: ostr<< "1:Simple Y 3D 125*125*125" << endl;break;
    }
  ostr << "Dimension:" << TC.dim << endl;
  ostr << "Size:" << endl << TC.size << endl;
  ostr << "d:" << endl << TC.d << endl;
  ostr << "Flags:" << TC.flags << endl;

  return (ostr);  
}  
