#include "stencil3d27.h"


Stencil3d27::
 Stencil3d27()
{
  dim = 3;
}


Stencil3d27::
~Stencil3d27()
{

}

// define midpoints position
int
Stencil3d27::
DIAG = 13;


// define number of elements
int 
Stencil3d27::
ELEMS = 27;


/*!
 * get number of elements
 */
int 
Stencil3d27::
Length()
{
  return ELEMS;
}

int 
Stencil3d27::
SetElem(int i, double val){
  
#ifdef DEBUG
  if ( ( i < 0 ) || ( i >= 27 ) )
    {
      cout << "Stencil3d27: Tried to acess to entry number " << i << endl;
      cout << "possible are [ 0 ... 26 ]" << endl;
      exit(1); 
    }
#endif

    elem[i] = val;    
    return 0;
}


int 
Stencil3d27::
AddToElem(int i, double val){
  
#ifdef DEBUG
  if ( ( i < 0 ) || ( i >= 27 ) )
    {
      cout << "Stencil3d27: Tried to acess to entry number " << i << endl;
      cout << "possible are [ 0 ... 26 ]" << endl;
      exit(1); 
    }
#endif

    elem[i] += val;
    return 0;
}


/*!
 * sets element at position (x,y,z) relative to 
 * stencils midpoint (0,0)
 */
int 
Stencil3d27::
SetElem(int x, int y, int z, double val){

#ifdef DEBUG
  if ( 
      ( abs(x) > 1 ) ||
      ( abs(y) > 1 ) ||
      ( abs(z) > 1 )
      ) 
    return OUT_OF_STENCIL_RANGE;
#endif

  elem[13+x+3*y+9*z] = val;

  return OK;

}


/*!
 * adds val to the element at position (x,y,z) 
 * relative to stencils midpoint (0,0)
 */
int 
Stencil3d27::
AddToElem(int x, int y, int z, double val){

  #ifdef DEBUG
  if ( 
      ( abs(x) > 1 ) ||
      ( abs(y) > 1 ) ||
      ( abs(z) > 1 )
      ) 
    return OUT_OF_STENCIL_RANGE;
  #endif
 
  elem[13+x+3*y+9*z] += val;

  return OK;
}



/*!
 * acess to stencil entrys
 */
double 
Stencil3d27::
GetElem(int i)
{
#ifdef DEBUG
  if ( ( i < 0 ) || ( i >= 27 ) )
    {
      cout << "Stencil3d7: Tried to acess to entry number " << i << endl;
      cout << "possible are [ 0 ... 26 ]" << endl;
      exit(1); 
    }
#endif
  return elem[i];
}

void 
Stencil3d27::
multiply( double factor )
{
  for ( int i = 0; i < 27; i++ )
    {
      elem[i] *= factor;
    } 
}


/*!
 * sets all entrys in the stencil to zero
 */
void
Stencil3d27::
clear()
{
  for ( int i = 0; i < 27; i++ ) 
    elem[i] = 0.0;
}


/*!
 * acess to stencil entrys
 */
double 
&Stencil3d27::
operator()(int i)
{
  if ( ( i < 0 ) || ( i >= 27 ) )
    {
      cout << "Stencil3d27: Tried to acess to entry number " << i << endl;
      cout << "possible are [ 0 ... 26 ]" << endl;
      exit(1); 
    }
  return elem[i];
}


/*!
 * print the stencil3d27
 */
void
Stencil3d27::
print()
{
  cout << "Stencil3d27" << endl;

  cout << "x-1,y-1,z-1 : " << GetElem(0) << " x,y-1,z-1 : " << GetElem(1) << " x+1,y-1,z-1 : " << GetElem(2) <<endl;
  cout << "x-1,y,z-1   : " << GetElem(3) << " x,y,z-1   : " << GetElem(4) << " x+1,y,z-1   : " << GetElem(5) <<endl;
  cout << "x-1,y+1,z-1 : " << GetElem(6) << " x,y+1,z-1 : " << GetElem(7) << " x+1,y+1,z-1 : " << GetElem(8) <<endl;
  
  cout << "x-1,y-1,z   : " << GetElem(9) << " x,y-1,z   : " << GetElem(10) << " x+1,y-1,z   : " << GetElem(11) <<endl;
  cout << "x-1,y,z     : " << GetElem(12) << " x,y,z     : " << GetElem(13) << " x+1,y,z     : " << GetElem(14) <<endl;
  cout << "x-1,y+1,z   : " << GetElem(15) << " x,y+1,z   : " << GetElem(16) << " x+1,y+1,z   : " << GetElem(17) <<endl;
  
  cout << "x-1,y-1,z+1 : " << GetElem(18) << " x,y-1,z+1 : " << GetElem(19) << " x+1,y-1,z+1 : " << GetElem(20) <<endl;
  cout << "x-1,y,z+1   : " << GetElem(21) << " x,y,z+1   : " << GetElem(22) << " x+1,y,z+1   : " << GetElem(23) <<endl;
  cout << "x-1,y+1,z+1 : " << GetElem(24) << " x,y+1,z+1 : " << GetElem(25) << " x+1,y+1,z+1 : " << GetElem(26) <<endl;

}

/*
ostream &operator<<(ostream &ostr, Stencil2d5 &ST)
{
 ostr << "Stencil3d27" << endl;

  ostr << "x-1,y-1,z-1 : " << GetElem(0) << " x,y-1,z-1 : " << GetElem(1) << " x+1,y-1,z-1 : " << GetElem(2) <<endl;
  ostr << "x-1,y,z-1   : " << GetElem(3) << " x,y,z-1   : " << GetElem(4) << " x+1,y,z-1   : " << GetElem(5) <<endl;
  ostr << "x-1,y+1,z-1 : " << GetElem(6) << " x,y+1,z-1 : " << GetElem(7) << " x+1,y+1,z-1 : " << GetElem(8) <<endl;
  
  ostr << "x-1,y-1,z   : " << GetElem(9) << " x,y-1,z   : " << GetElem(10) << " x+1,y-1,z   : " << GetElem(11) <<endl;
  ostr << "x-1,y,z     : " << GetElem(12) << " x,y,z     : " << GetElem(13) << " x+1,y,z     : " << GetElem(14) <<endl;
  ostr << "x-1,y+1,z   : " << GetElem(15) << " x,y+1,z   : " << GetElem(16) << " x+1,y+1,z   : " << GetElem(17) <<endl;
  
  ostr << "x-1,y-1,z+1 : " << GetElem(18) << " x,y-1,z+1 : " << GetElem(19) << " x+1,y-1,z+1 : " << GetElem(20) <<endl;
  ostr << "x-1,y,z+1   : " << GetElem(21) << " x,y,z+1   : " << GetElem(22) << " x+1,y,z+1   : " << GetElem(23) <<endl;
  ostr << "x-1,y+1,z+1 : " << GetElem(24) << " x,y+1,z+1 : " << GetElem(25) << " x+1,y+1,z+1 : " << GetElem(26) <<endl;

}
*/
