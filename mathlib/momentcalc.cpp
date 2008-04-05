#include "momentcalc.h"

/*! using this class for integration calculates 
 * the volume of the specified domain
 */
NeuraDataType 
VolumeIntegrant::
g(NeuraDataType pixel, const Vector<int> &coord)
{
  return 1;
}

/*! using this class for integration calculates 
 * the mass of the specified domain
 */
NeuraDataType 
MassIntegrant::
g(NeuraDataType pixel, const Vector<int> &coord)
{
  return pixel;
}


/*! using this class for integration calculates the x-coordinate 
 * of the focal point of the specified domain,
 * if the result is divided by the mass 
 */
NeuraDataType 
Focal_point_x::
g(NeuraDataType pixel, const Vector<int> &coord)
{ 
  return pixel*coord[1];
}


/*! using this class for integration calculates the y-coordinate 
 * of the focal point of the specified domain,
 * if the result is divided by the mass 
 */
NeuraDataType 
Focal_point_y::
g(NeuraDataType pixel, const Vector<int> &coord)
{ 
  return pixel*coord[2];
}


/*! using this class for integration calculates the z-coordinate 
 * of the focal point of the specified domain,
 * if the result is divided by the mass 
 */
NeuraDataType 
Focal_point_z::
g(NeuraDataType pixel, const Vector<int> &coord)
{ 
  return pixel*coord[3];
}


Vector<NeuraDataType> Inertian_tensor:: focal_point(3);
 

void 
Inertian_tensor::
prepare(){

}

/*! sets the focal point needed to calculate inertian tensor
 */
void 
Inertian_tensor::
SetFocalPoint(Vector<NeuraDataType> fp)
{
  focal_point[1] = fp[1];
  focal_point[2] = fp[2];
  if ( fp.GetLength() == 3 )
    focal_point[3] = fp[3];
}


NeuraDataType 
Inertian_tensor11::
g(NeuraDataType pixel, const Vector<int> &coord)
{   
  return pixel*coord[1]*coord[1];
 //return pixel*(coord[1]-focal_point[1])*(coord[1]-focal_point[1]);
}

 
NeuraDataType 
Inertian_tensor12::
g(NeuraDataType pixel, const Vector<int> &coord)
{
  return pixel*coord[1]*coord[2];
  //return pixel*(coord[1]-focal_point[1])*(coord[2]-focal_point[2]); 
}


NeuraDataType 
Inertian_tensor22::
g(NeuraDataType pixel, const Vector<int> &coord)
{
  return pixel*coord[2]*coord[2];
  //return pixel*(coord[2]-focal_point[2])*(coord[2]-focal_point[2]); 
}

NeuraDataType
Inertian_tensor13::
g(NeuraDataType pixel, const Vector<int> &coord)
{
  return pixel*coord[1]*coord[3];
  //return pixel*(coord[1]-focal_point[1])*(coord[3]-focal_point[3]);
}

NeuraDataType
Inertian_tensor23::
g(NeuraDataType pixel, const Vector<int> &coord)
{
  return pixel*coord[2]*coord[3];
  //return pixel*(coord[2]-focal_point[2])*(coord[3]-focal_point[3]);
}

NeuraDataType 
Inertian_tensor33::
g(NeuraDataType pixel, const Vector<int> &coord)
{
  return pixel*coord[3]*coord[3];
  //return pixel*(coord[3]-focal_point[3])*(coord[3]-focal_point[3]);
}
