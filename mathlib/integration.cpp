#include "integration.h"

Integration::
Integration( )
{
  inte = NULL;
  dc = NULL;
  BallRadSet = 0;
  BallItDepht = 3;
  BallRefinement = 2; 
}


Integration::
Integration( DataCube *datacube, Integrant *integrant_class )
{
  inte = integrant_class;
  dc = datacube;
  BallRadSet = 0;
  BallItDepht = 3;
  BallRefinement = 2; 
}

Integration::
~Integration()
{
 ;
}

/*! sets the integrant class 
 */
void
Integration::
SetIntegrant( Integrant *integrant_class )
{
  inte = integrant_class;
}

/*! sets the datacube
 */
void
Integration::
SetDatacube( DataCube *datacube )
{
  dc = datacube;
}


/*! sets iteration depht for ball integration
 */
void 
Integration::
SetBallItDepht(int depht)
{
  BallItDepht = depht;
}

/*! sets refinement number for ball integration
 */
void
Integration::
SetBallRefinement(int refinement)
{
  BallRefinement = refinement;
}


int
Integration::
GetBallRadX()
{
  return BallRad_x;
}

int
Integration::
GetBallRadY()
{
  return BallRad_y;
}

int
Integration::
GetBallRadZ()
{
  return BallRad_z;
}

/*! returns whether BallRad is set
 */
int 
Integration::
GetBallRadSet()
{
  return BallRadSet;
}


/*! decides, whether the point (x,y) lies in the ball
 * with radius rad round the midpoint ( mxp, myp ) or not
 */
int 
Integration::
point_in_ball(NeuraDataType mxp, NeuraDataType myp, NeuraDataType radx, NeuraDataType rady, NeuraDataType x, NeuraDataType y)
{
  return  (x-mxp)*(x-mxp)/(radx*radx) + (y-myp)*(y-myp)/(rady*rady) <= 1; 

}

/*! decides, whether the point (x,y,z) lies in the ball
 * with radius rad round the midpoint ( mxp, myp, mzp ) or not
 */
int 
Integration::
point_in_ball(NeuraDataType mxp, NeuraDataType myp, NeuraDataType mzp, NeuraDataType radx, NeuraDataType rady, NeuraDataType radz, NeuraDataType x, NeuraDataType y, NeuraDataType z)
{
  return  (x-mxp)*(x-mxp)/(radx*radx) + 
    (y-myp)*(y-myp)/(rady*rady) + 
    (z-mzp)*(z-mzp)/(radz*radz) <= 1;
} 



/*! calucluates the volume of the cube with 
 * boundarys bxl, bxu, byl, byu that
 * lies in the ball with radius rad
 */
NeuraDataType 
Integration::
calculate_locale_volume(NeuraDataType mpx, NeuraDataType mpy, 
			NeuraDataType radx, NeuraDataType rady,
			NeuraDataType volume, 
			NeuraDataType bxl, NeuraDataType bxu, 
			NeuraDataType byl, NeuraDataType byu, 
			int itdepht, int refinement ){

  int i,j;
  int volume_in = 1; // decides whether the volume is completly in ball
  int point_in = 0; // decides whether any point is in ball
  NeuraDataType loc_volume = 0.0;
  NeuraDataType xref, yref;

    if ( point_in_ball(mpx,mpy,radx,rady,bxl,byl) ) point_in = 1;
    else volume_in = 0;
    if ( point_in_ball(mpx,mpy,radx,rady,bxl,byu) ) point_in = 1; 
    else volume_in = 0;
    if ( point_in_ball(mpx,mpy,radx,rady,bxu,byl) ) point_in = 1;
    else volume_in = 0;
    if ( point_in_ball(mpx,mpy,radx,rady,bxu,byu) ) point_in = 1;
    else volume_in = 0;

    if ( volume_in ) return volume;
    if ( !point_in ) return 0.0;
    else 
      {
	if ( itdepht == 0 ) // volume not completly in ball
	  { // but maximum number of refinements done
	    if ( point_in )
	    return 0.5*volume; 
	    else return 0; 
	  }
                                         
	xref = fabs(bxu-bxl)/refinement;
	yref = fabs(byu-byl)/refinement;

	NeuraDataType bxli = bxl-xref;
	NeuraDataType bylj = byl-yref;

	for ( i = 1; i <= refinement; i++){
	  
	  bxli += xref;
	  bylj = byl-yref;

	  for ( j = 1; j <= refinement; j++){

	    bylj += yref;

	    loc_volume += calculate_locale_volume(mpx, mpy, 
						  radx, rady,
						  volume/(refinement*refinement),
						  bxli, bxli+xref, 
						  bylj, bylj+yref, 
						  itdepht-1, refinement);
	  }
	}
      }
    return loc_volume;
  
}


/*! calucluates the volume of the cube with 
 * boundarys bxl, bxu, byl, byu, bzl, bzu that
 * lies in the ball with radius rad
 */
NeuraDataType 
Integration::
calculate_locale_volume(NeuraDataType mpx, NeuraDataType mpy, NeuraDataType mpz, 
			NeuraDataType radx, NeuraDataType rady, NeuraDataType radz,
			NeuraDataType volume, 
			NeuraDataType bxl, NeuraDataType bxu, 
			NeuraDataType byl, NeuraDataType byu,
			NeuraDataType bzl, NeuraDataType bzu,
			int itdepht, int refinement ){

  int i,j,l;
  int volume_in = 1; // decides whether the volume is completly in ball
  int point_in = 0; // decides whether any point is in ball
  NeuraDataType loc_volume = 0.0;
  NeuraDataType xref, yref, zref;

  if ( point_in_ball(mpx,mpy,mpz,radx,rady,radz,bxl,byl,bzl) ) point_in = 1;
  else volume_in = 0;
  if ( point_in_ball(mpx,mpy,mpz,radx,rady,radz,bxl,byl,bzu) ) point_in = 1;
  else volume_in = 0;
  if ( point_in_ball(mpx,mpy,mpz,radx,rady,radz,bxl,byu,bzl) ) point_in = 1;
  else volume_in = 0;
  if ( point_in_ball(mpx,mpy,mpz,radx,rady,radz,bxl,byu,bzu) ) point_in = 1;
  else volume_in = 0;
  if ( point_in_ball(mpx,mpy,mpz,radx,rady,radz,bxu,byl,bzl) ) point_in = 1;
  else volume_in = 0;
  if ( point_in_ball(mpx,mpy,mpz,radx,rady,radz,bxu,byl,bzu) ) point_in = 1;
  else volume_in = 0;
  if ( point_in_ball(mpx,mpy,mpz,radx,rady,radz,bxu,byu,bzl) ) point_in = 1;
  else volume_in = 0;
  if ( point_in_ball(mpx,mpy,mpz,radx,rady,radz,bxu,byu,bzu) ) point_in = 1;
  else volume_in = 0;

  if ( volume_in ) return volume;
  if ( !point_in ) return 0.0;
  else 
    {
      if ( itdepht == 0 ) // volume not completly in ball
	{ // but maximum number of refinements done
	  if ( point_in )
	    return 0.5*volume; 
	  else return 0; 
	}
      
      xref = fabs(bxu-bxl)/refinement;
      yref = fabs(byu-byl)/refinement;
      zref = fabs(bzu-bzl)/refinement;
      
      NeuraDataType bxli = bxl-xref;
      NeuraDataType bylj = byl-yref;
      NeuraDataType bzll = bzl-zref;
      
      for ( i = 1; i <= refinement; i++){
	
	bxli += xref;
	bylj = byl-yref;
	bzll = bzl-zref;

	for ( j = 1; j <= refinement; j++){
	  
	  bylj += yref;
	  bzll = bzl-zref;

	  for ( l = 1; l <= refinement; l++){

	    bzll += zref;
	  
	    loc_volume += calculate_locale_volume(mpx, mpy, mpz,
						  radx, rady, radz,
						  volume/pow(refinement,3), 
						  bxli, bxli+xref, 
						  bylj, bylj+yref, 
						  bzll, bzll+zref, 
						  itdepht-1, refinement);
	   
	  }
	}
      }
    }
  
  return loc_volume;
  
}


/*! computes for all pixels the corresponding volume lying within 2d ball
 * only useable for sphere volumes
 */
int
Integration::
computeBallWeights2d(Volume &vol){

  cout << "calculate weights for circle integration" << endl;

  BallRad_x = vol.size[1];
  BallRad_y = vol.size[2];
  int BallMPx = vol.start[1];
  int BallMPy = vol.start[2];

  if ( BallRadSet ) BallWeights.freeMemory();
 
  BallWeights.TransformToCube(vol);

  BallWeights.MallocCube();
  
  // lower boundary of integration domain in x-direction
  int bxl = vol.start[1] - vol.size[1]; 

  // upper boundary of integration domain in x-direction
  int bxu = vol.start[1] + vol.size[1];

  // lower boundary of integration domain in y-direction
  int byl = vol.start[2] - vol.size[2]; 

  // upper boundary of integration domain in y-direction
  int byu = vol.start[2] + vol.size[2];
 
  for ( int i = bxl; i <= bxu; i++ ){ 

    for ( int j = byl; j <= byu; j++ ){
     
      BallWeights(i-bxl,j-byl) = calculate_locale_volume(BallMPx, BallMPy, 
						    BallRad_x, BallRad_y, 
						    1.0, i-0.5, i+0.5, 
						    j-0.5, j+0.5, BallItDepht, BallRefinement);
      
     }

  } 

  BallRadSet = 1;  

  return OK;

}


/*!
 * computes for all pixels the corresponding volume lying within 3d ball
 * only useable for sphere volumes
 */
int
Integration::
computeBallWeights3d(Volume &vol){

  cout << "calculate weights for sphere integration...";

  BallRad_x = vol.size[1];
  BallRad_y = vol.size[2];
  BallRad_z = vol.size[3];
  int BallMPx = vol.start[1];
  int BallMPy = vol.start[2];
  int BallMPz = vol.start[3];

  if ( BallRadSet ) BallWeights.freeMemory();
 
  BallWeights.TransformToCube(vol);

  BallWeights.MallocCube();

  // lower boundary of integration domain in x-direction
  int bxl = vol.start[1] - vol.size[1];

  // upper boundary of integration domain in x-direction
  int bxu = vol.start[1] + vol.size[1];

  // lower boundary of integration domain in y-direction
  int byl = vol.start[2] - vol.size[2];

  // upper boundary of integration domain in y-direction
  int byu = vol.start[2] + vol.size[2];

  // lower boundary of integration domain in z-direction
  int bzl = vol.start[3] - vol.size[3];
      
  // upper boundary of integration domain in z-direction
  int bzu = vol.start[3] + vol.size[3];

  for ( int i = bxl; i <= bxu; i++ ){ 

    for ( int j = byl; j <= byu; j++ ){

      for ( int l = bzl; l <= bzu; l++ ){

	BallWeights(i-bxl,j-byl,l-bzl) = calculate_locale_volume(BallMPx, BallMPy, BallMPz,
						    BallRad_x, BallRad_y, BallRad_z,
						    1.0, i-0.5, i+0.5, 
						    j-0.5, j+0.5, l-0.5, l+0.5, 
						     BallItDepht, BallRefinement);
 
      }
    }
  }

  BallRadSet = 1;

  cout << "done" << endl;

  return OK;

}


// // decides, whether the point (x,y) lies within
// // the datacube or not
// int 
// Integration::
// within_dc(int x, int y, int n, int m)
// {
//   if (
//       ( x < 0 ) ||
//       ( x > n-1 ) ||
//       ( y < 0 ) ||
//       ( y > m-1 ) 
//       )
//     return 0;
//   return 1; 
    
// }

// // decides, wheter the point (x,y,z) lies within
// // the datacube or not
// int 
// Integration::
// within_dc(int x, int y, int z, int n, int m, int k)
// {
//   if (
//       ( x < 0 ) ||
//       ( x > n-1 ) ||
//       ( y < 0 ) ||
//       ( y > m-1 ) ||
//       ( z < 0 ) ||
//       ( z > k-1 ) 
//       )
//     return 0;
//   return 1; 
    
// }



/*! does the integration on domain vol
 */
int
Integration::
execute(NeuraDataType &res, Volume &vol )
{

  // check correctness of dimensions

  if ( dc->dim != vol.dim ) return DCDIM_NEQ_VOLDIM;

  res = 0.0;

  switch (vol.dim)
    {
    case 2:
      return execute2d(res,vol);
      break;
    case 3: 
      return execute3d(res,vol);
      break;
    default: return UNIMPLEMENTED_VOL_DIM;
    }

}

/*! does the integration for 2d case on domain vol
 */
int
Integration::
execute2d(NeuraDataType &res, Volume &vol )
{  

  // check correctness of dimensions 
  if ( dc->dim != vol.dim ) return DCDIM_NEQ_VOLDIM;

  res = 0.0;

  // prepare integrant
  inte->prepare();

  int n = dc->GetSize()[1]; // get size of data in x-direction
  int m = dc->GetSize()[2]; // get size of data in y-direction

  int bn; // size of BallWeightsCube in x direction

  int bxl, bxu, byl, byu; // boundarys for intergration

  int bwx = 0; // boundarys for DataCube BallWeights  
  int bwy = 0;

  int shift_x = 0;
  int shift_y = 0;

  Vector<int> coord(2); // contents the current coordinates
    
  NeuraDataType* data = dc->GetDataPointer();
  NeuraDataType* weights;
 
  switch (vol.GetGeometryType()) 
    {

    case BALL:

	if ( 
	    ( BallRad_x != vol.size[1] ) || 
	    ( BallRad_y != vol.size[2] ) || 
	     ( !BallRadSet ) 
	    )
	  {
	    computeBallWeights2d(vol);
	  }
	
	bn = BallWeights.GetSize()[1];  // get size of BallWeights in x-direction
	weights = BallWeights.GetDataPointer();

	// lower boundary of integration domain in x-direction
        bxl = vol.start[1] - vol.size[1]; 
	if ( bxl < 0 ) { shift_x = -bxl; bxl = 0; }  

	// upper boundary of integration domain in x-direction
        bxu = vol.start[1] + vol.size[1];
	if ( bxu >= n ) bxu = n-1;

	// lower boundary of integration domain in y-direction
        byl = vol.start[2] - vol.size[2]; 
	if ( byl < 0 ) { shift_y = -byl; byl = 0; }

	// upper boundary of integration domain in y-direction
        byu = vol.start[2] + vol.size[2];
	if ( byu >= m ) byu = m-1;
	
	bwx = shift_x;
	bwy = shift_y;

	for ( int j = byl; j <= byu; j++ ){
	  
	  coord[2] = j; // y-coordinate
	  
	  for ( int i = bxl; i <= bxu; i++ ){ 
	    
	    coord[1] = i; // x-coordinate
	    
	    res += inte->g(data[i+j*n],coord) * 
	      weights[bwx+bwy*n]; 

	    bwx++;  
	  }
	  bwx = shift_x;
	  bwy++;
	}
	
	return OK;
	break;

    case CUBE:

      // lower boundary of integration domain in x-direction
      bxl = vol.start[1];

      // upper boundary of integration domain in x-direction
      bxu = vol.start[1] + vol.size[1] - 1; 
      if ( bxu >= n ) bxu = n-1;

      // lower boundary of integration domain in y-direction
      byl = vol.start[2];

      // upper boundary of integration domain in y-direction
      byu = vol.start[2] + vol.size[2] - 1;
      if ( byu >= m ) byu = m-1;
      
      for ( int j = byl; j <= byu; j++ ){
	
	coord[2] = j;
	
	for ( int i = bxl; i <= bxu; i++ ){
	  
	  coord[1] = i;
	  
	  res += inte->g(data[i+j*n],coord);
	  
	}
      }
      
      return OK;
      break;

    default: return UNDEFINED_GEOMETRIE;
      
    }
  
}


/*! does the integration for 3d case on the domain vol
 */
int
Integration::
execute3d(NeuraDataType &res, Volume &vol )
{  

  // check correctness of dimensions
  if ( dc->dim != vol.dim ) return DCDIM_NEQ_VOLDIM;

  res = 0.0;

  // prepare integrant
  inte->prepare();

  int n = dc->GetSize()[1]; // get size of data in x-direction
  int m = dc->GetSize()[2]; // get size of data in y-direction
  int k = dc->GetSize()[3]; // get size of data in z-direction

 
  int bn, bm; // size of BallWeightsCube in x and y direction

  int bxl, bxu, byl, byu, bzl, bzu;

  int bwx = 0; // boundarys for DataCube BallWeights  
  int bwy = 0;
  int bwz = 0;

  int shift_x = 0;
  int shift_y = 0;
  int shift_z = 0;

  //NeuraDataType vol_loc = 1.0;
  Vector<int> coord(3); // contents the current coordinates

  NeuraDataType* data = dc->GetDataPointer();
  NeuraDataType* weights;
     
  switch (vol.GetGeometryType()) 
    {

    case BALL: 
   
      if  ( 
	   ( BallRad_x != vol.size[1] ) || 
	   ( BallRad_y != vol.size[2] ) ||
	   ( BallRad_z != vol.size[3] ) ||
	   ( !BallRadSet ) ) 
	{
	  computeBallWeights3d(vol);
	}

      bn = BallWeights.GetSize()[1];  // get size of BallWeights in x-direction
      bm = BallWeights.GetSize()[2];  // get size of BallWeights in y-direction

      weights = BallWeights.GetDataPointer();

      // lower boundary of integration domain in x-direction
      bxl = vol.start[1] - vol.size[1];
      if ( bxl < 0 ) { shift_x = -bxl; bxl = 0; } 

      // upper boundary of integration domain in x-direction
      bxu = vol.start[1] + vol.size[1];
      if ( bxu >= n ) bxu = n-1;

      // lower boundary of integration domain in y-direction
      byl = vol.start[2] - vol.size[2];
      if ( byl < 0 ) { shift_y = -byl; byl = 0; }

      // upper boundary of integration domain in y-direction
      byu = vol.start[2] + vol.size[2];
      if ( byu >= m ) byu = m-1;

      // lower boundary of integration domain in z-direction
      bzl = vol.start[3] - vol.size[3];
      if ( bzl < 0 ) { shift_z = -bzl; bzl = 0; }
      
      // upper boundary of integration domain in z-direction
      bzu = vol.start[3] + vol.size[3];
      if ( bzu >= k ) bzu = k-1;

      bwx = shift_x;
      bwy = shift_y;
      bwz = shift_z;
	
      for ( int l = bzl; l <= bzu; l++ ){
	
	coord[3] = l; // z-coordinate
	
	for ( int j = byl; j <= byu; j++ ){
	  
	  coord[2] = j; // y-coordinate
	  
	  for ( int i = bxl; i <= bxu; i++ ){ 
	    
	    coord[1] = i; // x-coordinate
	    
	    res += inte->g(data[i+j*n+l*n*m],coord) * 
	      weights[bwx+bwy*bn+bwz*bn*bm];
	   
	    bwx++;			    
	  }
	  bwx = shift_x;
	  bwy++;
	}
	bwx = shift_x;
	bwy = shift_y;
	bwz++;
      }
      
      return OK;
      break;
      
    case CUBE:
   
      // lower boundary of integration domain in x-direction
      bxl = vol.start[1];
      if ( bxl < 0 ) bxl = 0;
      
      // upper boundary of integration domain in x-direction
      bxu = vol.start[1] + vol.size[1] - 1;
      if ( bxu >= n ) bxu = n-1;

      // lower boundary of integration domain in y-direction
      byl = vol.start[2];
      if ( byl < 0 ) byl = 0;

      // upper boundary of integration domain in y-direction
      byu = vol.start[2] + vol.size[2] - 1;
      if ( byu >= m ) byu = m-1;
 
      // lower boundary of integration domain in z-direction
      bzl = vol.start[3];
      if ( bzl < 0 ) bzl = 0; 
      
      // upper boundary of integration domain in z-direction
      bzu = vol.start[3] + vol.size[3] - 1;
      if ( bzu >= k ) bzu = k-1;

  
      for ( int l = bzl; l <= bzu; l++ ){
	
	coord[3] = l;
	
	for ( int j = byl; j <= byu; j++ ){
	  
	  coord[2] = j;
	  
	  for ( int i = bxl; i <= bxu; i++ ){
	    
	    coord[1] = i;
	    
	    res += inte->g(data[i+j*n+l*n*m],coord);
	    
	  }  
	}
      }

      return OK;
      break;
      
    default: return UNDEFINED_GEOMETRIE;
      
    }
}
