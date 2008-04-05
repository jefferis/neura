#include "volume.h"

Volume::
Volume(Volume &vol): dim(vol.dim), start(vol.dim), size(vol.dim), d(vol.dim)
{

  dim = vol.dim;
  start = vol.start;
  size = vol.size;
  d = vol.d;
  gt = vol.gt;
 
}

Volume::
Volume(const Volume& vol): dim(vol.dim), start(vol.dim), size(vol.dim), d(vol.dim)
{

  dim = vol.dim;
  start = vol.start;
  size = vol.size;
  d = vol.d;
  gt = vol.gt;
 
}



Volume::
Volume() : dim(0), start(0), size(0), d(0)
{
  start.SetZero();
  size.SetZero();
  d.SetZero();
  gt = CUBE; // default is the cube case
};


Volume::
Volume(int dimension) : dim(dimension), start(dimension), size(dimension), d(dimension)
{
  start.SetZero();
  size.SetZero();
  d.SetZero();
  gt = CUBE; // default is the cube case
};


Volume::
Volume(GeometryType t,int dim, int six, int siy) 
  : dim(2), start(2), size(2), d(2)
{
  start[1] = 0;
  start[2] = 0;
  size[1] = six;
  size[2] = siy;
  d[1] = 1.0;
  d[2] = 1.0;
  gt = t;
}


Volume::
Volume(GeometryType t,int dim, int six, int siy, int siz) 
  : dim(3), start(3), size(3), d(3)
{
  start[1] = 0;
  start[2] = 0;
  start[3] = 0;
  size[1] = six;
  size[2] = siy;
  size[3] = siz;
  d[1] = 1.0;
  d[2] = 1.0;
  d[3] = 1.0;
  gt = t;
}



Volume::
Volume(int dimension,const Vector<int> &st,const Vector<int> &si,Vector<double> &dl) 
: dim(dimension), start(dimension), size(dimension), d(dimension)
{
  start=st;
  size=si;
  d=dl;
  gt = CUBE; // default is the cube case
};




Volume::
Volume(GeometryType t, int dimension ) : dim(dimension), start(dimension), size(dimension), d(dimension)
{
  start.SetZero();
  size.SetZero();
  d.SetZero();
  gt = t;
};
Volume::
Volume(int stx,int sty, int six, int siy, double dx, double dy)
  : dim(2), start(2), size(2), d(2)
{
  start[1]=stx;
  start[2]=sty;
  size[1]=six;
  size[2]=siy;
  d[1]=dx;
  d[2]=dy;
  gt = BALL;
};

Volume::
Volume(GeometryType t, int stx,int sty, int six, int siy, double dx, double dy)
  : dim(2), start(2), size(2), d(2)
{
  start[1]=stx;
  start[2]=sty;
  size[1]=six;
  size[2]=siy;
  d[1]=dx;
  d[2]=dy;
  gt = t;
};

Volume::
Volume(int stx,int sty, int stz,int six, int siy, int siz, double dx, double dy, double dz)
  : dim(3), start(3), size(3), d(3)
{
  start[1]=stx;
  start[2]=sty;
  start[3]=stz;
  size[1]=six;
  size[2]=siy;
  size[3]=siz;
  d[1]=dx;
  d[2]=dy;
  d[3]=dz;
  gt = BALL;
};

Volume::
Volume(GeometryType t, int stx,int sty, int stz,int six, int siy, int siz, double dx, double dy, double dz)
  : dim(3), start(3), size(3), d(3)
{
  start[1]=stx;
  start[2]=sty;
  start[3]=stz;
  size[1]=six;
  size[2]=siy;
  size[3]=siz;
  d[1]=dx;
  d[2]=dy;
  d[3]=dz;
  gt = t;
};






Volume::
~Volume()  
{

};   


Vector<double> 
Volume::
discreteToCont(Vector<int> posDiscrete)
{
  Vector<double> posCont(dim);
  posCont=ComponentMultiply(d,posDiscrete);
  return(posCont);   
}

int 
Volume::
VolumeCopy(Volume &vol)
{

  dim = vol.dim;
  start = vol.start;
  size = vol.size;
  d = vol.d;
  gt = vol.gt;
 
  return OK;
}


// transforms a volume specified as a ball into a cube
// !! works for dimensions 2 and 3 only !!
int 
Volume::
TransformToCube(Volume &vol)
{
  if ( ( vol.dim != 2 ) && ( vol.dim != 3 ) )
    return 1; // not working for other dimensions  
  if ( vol.gt == CUBE ) // is a cube already
    return 1;
  dim = vol.dim;
  start.Reset(vol.start.GetLength());
  size.Reset(vol.size.GetLength());
  d.Reset(vol.d.GetLength());
  start[1] = vol.start[1] - vol.size[1];
  start[2] = vol.start[2] - vol.size[2];
  size[1] = 2*vol.size[1]+1;
  size[2] = 2*vol.size[2]+1;
  d = vol.d;
  gt = BALL;
  if ( vol.dim == 3 )
    {
      size[3] = 2*vol.size[3]+1;
      start[3] = vol.start[3] - vol.size[3];
    }
 
  return OK;
}


void
Volume::
SetCubeSizeAndSpaceing(const Vector<int> &sl,Vector<double> &dl)
{
  dim = sl.GetLength();
  size=sl;
  d=dl;
}


// sets the start new
void 
Volume::
SetStart(int x, int y)
{
  start[1] = x; 
  start[2] = y;
}

// sets the start new
void 
Volume::
SetStart(int x, int y, int z)
{
  start[1] = x; 
  start[2] = y;
  start[3] = z;
}

// sets the start new
void
Volume::
SetStart(Vector<int>& s)
{
  start = s;
}

// sets the size new
void 
Volume::
SetSize(Vector<int> &s)
{
  dim = s.GetLength();
  size = s;
}

void 
Volume::
SetSpaceing(Vector<double> &dl)
{
    d=dl;
};

// returns the GeometryType
int 
Volume::
GetGeometryType()
{
  return gt;
}

int
Volume::
GetDim()
{
  return dim;
}

Vector<int> 
&Volume::
GetSize()
{
  return size;
}

Vector<double>
&Volume::
GetSpaceing()
{
  return d;
}


/*!
 * decides whether the point with specified ccordinates lied within this volume
 */
int 
Volume::
is_in(int x, int y, int z)
{
  if ( 
      ( x < 0 ) || ( x >= size[1] ) ||
      ( y < 0 ) || ( y >= size[2] ) ||
      ( z < 0 ) || ( z >= size[3] ) 
      )
    return 0;
  return 1;
}



/*!
 * calculates the corresponding node to specified position
 */
int 
Volume::
calc_node(Vector<int>& pos)
{
  if (dim==2) 
    return (pos[1]+start[1]) + 
      (pos[2]+start[2])* size[1];
  if (dim==3) 
    return (pos[1]+start[1]) + 
      (pos[2]+start[2])* size[1] + 
      (pos[3]+start[3]) * size[1]*size[2];
  return -1;
}



/*!
 * calculates corresponding position to specified node number
 */
int 
Volume::
calc_pos(int node, Vector<int>& pos)
{
  if ( dim == 2 )
    {
      pos[1] = ( node % size[1]) - start[1];
      pos[2] = ( node / size[1] ) - start[2];
    }

  if ( dim == 3 )
    {
      pos[1] = ( node % size[1]) - start[1];
      pos[2] = ( node / size[1] ) - start[2];
      pos[3] = ( node / ( size[1]*size[2] ) ) - start[3];
    }
  return OK;
}


ostream &operator<<(ostream &ostr, const Volume &V)
{
  
  ostr << "Volume" << endl;
  ostr << "Dimension" << V.dim << endl;
  ostr << "Start:" << endl << V.start << endl;
  ostr << "Size:" << endl << V.size << endl;
  ostr << "d:" << endl << V.d << endl;
  return (ostr);  
}  
