#include "moments.h"
#include "momentcalc.h"

Moments::
Moments(DataCube *datacube) : i_t(), f_p(3)
{
  dc = datacube;
  MassIntegrant ms;
  i_t.SetIntegrant(&ms);
  i_t.SetDatacube(datacube);
}

Moments::
~Moments(){

}

/*! computes the focal point
 */
int
Moments::
focal_point(Vector<NeuraDataType> &fp, Volume &vol, NeuraDataType &mass_res )
{

  // check correctness of dimensions
  if ( dc->dim != vol.dim ) return DCDIM_NEQ_VOLDIM;

  switch ( vol.dim )
    {
    case 2: 
      if ( fp.GetLength() != 2 ) return WRONG_RESVECTOR_LENGTH;
      return focal_point2d(fp,vol,mass_res);
      break;
    case 3:  
      if ( fp.GetLength() != 3 ) return WRONG_RESVECTOR_LENGTH;
      return focal_point3d(fp,vol,mass_res);
      break;
    default: return UNIMPLEMENTED_VOL_DIM;
    }

}


/*! computes the 2d-focal point, also called moment zero
 */
int
Moments::
focal_point2d(Vector<NeuraDataType> &fp, Volume &vol, NeuraDataType &mass_res )
{

  MassIntegrant mass;
  i_t.SetIntegrant(&mass);
  int code = i_t.execute2d(mass_res, vol);

  if ( code ){ cout << errorCode(code); exit(1); }

  fp[1] = 0.0;
  fp[2] = 0.0;

  Focal_point_x focal_point_x;
  i_t.SetIntegrant(&focal_point_x);
  i_t.execute2d(fp[1],vol);

  Focal_point_y focal_point_y;
  i_t.SetIntegrant(&focal_point_y);
  i_t.execute2d(fp[2],vol);

  if ( mass_res != 0 ){
    fp[1] /= mass_res;
    fp[2] /= mass_res;
  }

  return OK;

} 

/*! computes the 3d-focal point
 */
int 
Moments::
focal_point3d(Vector<NeuraDataType> &fp, Volume &vol, NeuraDataType &mass_res )
{

  MassIntegrant mass;
  i_t.SetIntegrant(&mass);
  int code = i_t.execute3d(mass_res,vol);

  if ( code ) return code;

  fp[1] = 0.0;
  fp[2] = 0.0;
  fp[3] = 0.0;

  Focal_point_x focal_point_x;
  i_t.SetIntegrant(&focal_point_x);
  i_t.execute3d(fp[1],vol);

  Focal_point_y focal_point_y; 
  i_t.SetIntegrant(&focal_point_y);
  i_t.execute3d(fp[2],vol);

  Focal_point_z focal_point_z;
  i_t.SetIntegrant(&focal_point_z);
  i_t.execute3d(fp[3],vol);

  if ( mass_res != 0 ){
    fp[1] /= mass_res;
    fp[2] /= mass_res;
    fp[3] /= mass_res;
  }

  return OK;

} 

/*! computes the 2d inertian tensor, also called moment one
 */
int 
Moments::
inertian_tensor2d(Vector<NeuraDataType> &it, Volume &vol )
{

   NeuraDataType mass_res = 0.0;


  it[1] = 0.0;
  it[2] = 0.0;
  it[3] = 0.0;
  it[4] = 0.0;

  focal_point2d(f_p,vol, mass_res);

  Inertian_tensor11 inertian_tensor11;
  i_t.SetIntegrant(&inertian_tensor11);
  i_t.execute2d(it[1],vol);
  it[1] += (mass_res-2)*f_p[1]*f_p[1];

  Inertian_tensor12 inertian_tensor12; 
  i_t.SetIntegrant(&inertian_tensor12);
  i_t.execute2d(it[2],vol);
  it[2] += (mass_res-2)*f_p[1]*f_p[2];

  Inertian_tensor22 inertian_tensor22; 
  i_t.SetIntegrant(&inertian_tensor22);
  i_t.execute2d(it[4],vol);
  it[4] += (mass_res-2)*f_p[2]*f_p[2];


  it[3] = it[2]; // tensor symmetric 

  return OK;

  } 


/*! computes the 3d inertian tensor, also called moment one
 */
int
Moments::
inertian_tensor3d(Vector<NeuraDataType> &it, Volume &vol )
{
   NeuraDataType mass_res;

  it[1] = 0.0;
  it[2] = 0.0;
  it[3] = 0.0;
  it[4] = 0.0;
  it[5] = 0.0;
  it[6] = 0.0;
  it[7] = 0.0;
  it[8] = 0.0;
  it[9] = 0.0;

  focal_point3d(f_p,vol,mass_res);
  
  Inertian_tensor11 inertian_tensor11;
  i_t.SetIntegrant(&inertian_tensor11);
  i_t.execute3d(it[1],vol);
  it[1] -= f_p[1]*f_p[1]*mass_res;

  Inertian_tensor12 inertian_tensor12;
  i_t.SetIntegrant(&inertian_tensor12);
  i_t.execute3d(it[2],vol);
  it[2] -= f_p[1]*f_p[2]*mass_res;

  Inertian_tensor13 inertian_tensor13;
  i_t.SetIntegrant(&inertian_tensor13);
  i_t.execute3d(it[3],vol);
  it[3] -= f_p[1]*f_p[3]*mass_res;

  Inertian_tensor22 inertian_tensor22; 
  i_t.SetIntegrant(&inertian_tensor22);
  i_t.execute3d(it[5],vol);
  it[5] -= f_p[2]*f_p[2]*mass_res;

  Inertian_tensor23 inertian_tensor23;
  i_t.SetIntegrant(&inertian_tensor23);
  i_t.execute3d(it[6],vol);
  it[6] -= f_p[2]*f_p[3]*mass_res;

  Inertian_tensor33 inertian_tensor33; 
  i_t.SetIntegrant(&inertian_tensor33);
  i_t.execute3d(it[9],vol);
  it[9] -= f_p[3]*f_p[3]*mass_res;


  it[4] = it[2]; // tensor symmetric 
  it[7] = it[3]; // tensor symmetric
  it[8] = it[6]; // tensor symmetric

  return OK;

  } 


/*! computes the inertian tensor
 */
int
Moments::
inertian_tensor(Vector<NeuraDataType> &v,  Volume &vol )
{

  // check correctness of dimensions
  if ( dc->dim != vol.dim ) return DCDIM_NEQ_VOLDIM;

  switch (vol.dim){

  case 2: 
    if ( v.GetLength() != 4 ) return WRONG_RESVECTOR_LENGTH;
    return inertian_tensor2d(v,vol);
    break;
  case 3: 
    if ( v.GetLength() != 9 ) return WRONG_RESVECTOR_LENGTH;
    return inertian_tensor3d(v,vol);
    break;
  default: return UNIMPLEMENTED_VOL_DIM;

  }

}



/*! computes eigenvalues and eigenvevtors of the inertian_tensor
 */
int
Moments::
inertian_values(Vector<NeuraDataType> &vals, Vector<NeuraDataType> &vects, Volume &vol )
{
  // check correctness of dimensions
  if ( dc->dim != vol.dim ) return DCDIM_NEQ_VOLDIM;

  switch (vol.dim){

  case 2: 
    if ( 
	( vals.GetLength() != 2 ) ||
	( vects.GetLength() != 4 )
	)
      return WRONG_RESVECTOR_LENGTH;
    else{
      int code = inertian_tensor2d(vects,vol);
      if ( code ) return code;
     
      double **m = dmatrix(1,2,1,2);
    
      m[1][1] = vects[1]; m[1][2] = vects[2];
      m[2][1] = vects[3]; m[2][2] = vects[4];
     
      double d[3];
      double e[3];
     
      tred2(m,2,d,e);
     
      tqli(d,e,2,m);
     

      vals[1] = (NeuraDataType)d[1];
      vals[2] = (NeuraDataType)d[2];
      vects[1] = (NeuraDataType)m[1][1];
      vects[3] = (NeuraDataType)m[1][2];
      vects[2] = (NeuraDataType)m[2][1];
      vects[4] = (NeuraDataType)m[2][2];

      free_dmatrix(m,1,2,1,2);
    }
    return OK;
    break;
  case 3: 
    if ( 
	( vals.GetLength() != 3 ) ||
	( vects.GetLength() != 9 )
	)
      return WRONG_RESVECTOR_LENGTH;
    else{
      int code = inertian_tensor3d(vects,vol);
      if ( code ) return code;
         
      double **m = dmatrix(1,3,1,3);
    
      m[1][1] = vects[1]; m[1][2] = vects[2]; m[1][3] = vects[3];
      m[2][1] = vects[4]; m[2][2] = vects[5]; m[2][3] = vects[6];
      m[3][1] = vects[7]; m[3][2] = vects[8]; m[3][3] = vects[9];
     
      double d[4];
      double e[4];
     
      tred2(m,3,d,e);
     
      tqli(d,e,3,m);
     
      vals[1] = (NeuraDataType)d[1];
      vals[2] = (NeuraDataType)d[2];
      vals[3] = (NeuraDataType)d[3];
      vects[1] = (NeuraDataType)m[1][1];
      vects[4] = (NeuraDataType)m[1][2];
      vects[7] = (NeuraDataType)m[1][3];
      vects[2] = (NeuraDataType)m[2][1];
      vects[5] = (NeuraDataType)m[2][2];
      vects[8] = (NeuraDataType)m[2][3];
      vects[3] = (NeuraDataType)m[3][1];
      vects[6] = (NeuraDataType)m[3][2];
      vects[9] = (NeuraDataType)m[3][3];

      free_dmatrix(m,1,3,1,3);
    
    }

    return OK;
    break;

 default: return UNIMPLEMENTED_VOL_DIM;

  }

}


/*! calculates the inertian values for all points in datacube
 */
int 
Moments::
all_inertian_values( Vector<NeuraDataType> *vals, Vector<NeuraDataType> *vects, Volume &vol)
{
  // check correctness of dimensions
  if ( dc->dim != vol.dim ) return DCDIM_NEQ_VOLDIM;

  if ( !vol.GetGeometryType() ) // case cube
    {
      return all_cube_inertian_values(vals, vects, vol);
    } 

  int size_X = dc->GetSize()[1];
  int size_Y = dc->GetSize()[2];
  int size_Z;

  int counter = 0;

  switch (vol.dim){
    
  case 2:
    
    for ( int count_y = 0; count_y < size_Y; count_y++ )
      {
	for (  int count_x = 0; count_x < size_X; count_x++)
	  {
	    vol.SetStart(count_x, count_y);
	    
	    inertian_values( vals[counter], vects[counter], vol );
	    
	    counter++;    
	    
	  }
      }
  
  return OK;
  break;
  
 case 3:
   
   size_Z = dc->GetSize()[3];
    
   for ( int count_z = 0; count_z < size_Z; count_z++ )
     {
	for ( int count_y = 0; count_y < size_Y; count_y++)
	  {
	    for ( int count_x = 0; count_x < size_X; count_x++ )
 
	      {
		vol.SetStart(count_x, count_y, count_z);

		inertian_values( vals[counter], vects[counter], vol );
		
		counter++;
		
	      }
	  }
     }
   
   return OK;
   break;
   
 default: return UNIMPLEMENTED_VOL_DIM;
  }

}

/*! calculates the inertian values by cube integration 
 * for all points in datacube 
 */
int 
Moments::
all_cube_inertian_values(Vector<NeuraDataType> *vals, Vector<NeuraDataType> *vects, Volume &vol)
{
  int size_X = dc->GetSize()[1];
  int size_Y = dc->GetSize()[2];
  int size_Z;
  
  int move_X = vol.GetSize()[1];
  int move_Y = vol.GetSize()[2];
  int move_Z;
  
  int counter = 0;
  
  switch (vol.dim){
    
  case 2:
    
    if ( ( move_X % 2 == 0 ) || ( move_Y % 2 == 0 ) )
      {
	return CUBE_WITHOUT_MIDPOINT;
      }

    move_X /= 2;
    move_Y /= 2;
      
    for (  int count_y = 0; count_y < size_Y; count_y++ )
      {
	for ( int count_x = 0; count_x < size_X; count_x++ )
	  {

	    vol.SetStart(count_x-move_X, count_y-move_Y);
	    
	    inertian_values( vals[counter], vects[counter], vol );
	    
	    counter++;
	    
	  }
      }
    
    return OK;
    break;
    
  case 3:
    
    size_Z = dc->GetSize()[3];
    
    move_Z = vol.GetSize()[3];
    
    if ( ( move_X % 2 == 0 ) || ( move_Y % 2 == 0 ) || ( move_Z % 2 == 0 ) )
      {
	return CUBE_WITHOUT_MIDPOINT;
      }
    
    move_X /=2;
    move_Y /=2;
    move_Z /= 2;
       
    for ( int count_z = 0; count_z < size_Z; count_z++  )
      {
	for ( int count_y = 0; count_y < size_Y; count_y++)
	  {
	    for ( int count_x = 0; count_x < size_X; count_x++ )
	      {
		vol.SetStart(count_x-move_X, count_y-move_Y, count_z-move_Z);
		
		inertian_values( vals[counter], vects[counter], vol );
		
		counter++;
		
	      }
	  }
      }
    
   return OK;
   break;
   
  default: return UNIMPLEMENTED_VOL_DIM;
  }
  
}

/*! calculates the inertian values for all points in datacube by combining 
 *  points to discs
 *  only for 3d cubes usable
 */
int 
Moments::
fast_all_inertian_values( Vector<NeuraDataType> *vals, Vector<NeuraDataType> *vects, Volume &vol)
{
  // check correctness of dimensions
  if ( dc->dim != vol.dim ) return DCDIM_NEQ_VOLDIM;

  int i,j,k;
  int i_lay, j_lay;
  int dis, disbndl, disbndu;
  NeuraDataType res;
  NeuraDataType mass_help;

  NeuraDataType* data;
  data = dc->GetDataPointer();

  NeuraDataType* current_mom; // points to the current moment field

  NeuraDataType* massptr; // pointer to mass field for to be able to divide calculated
                   // current moment by mass
  NeuraDataType* fpptr_a;
  NeuraDataType* fpptr_b;

  // define MomentCube to store moments
  MomentCube mc(dc->GetDim()); // must be 3
  mc.SetCubeSizeAndSpaceing(dc->GetSize(),dc->GetSpaceing());
  mc.MallocMomentCube();

  // define needed integrant classes
  Integrant* inte;
  MassIntegrant mass;
  Focal_point_x focal_point_x;
  Focal_point_y focal_point_y; 
  Focal_point_z focal_point_z;
  Inertian_tensor11 inertian_tensor11;
  Inertian_tensor12 inertian_tensor12;
  Inertian_tensor13 inertian_tensor13;
  Inertian_tensor22 inertian_tensor22;
  Inertian_tensor23 inertian_tensor23;
  Inertian_tensor33 inertian_tensor33;

  int bxl, bxu, byl, byu;

  int n = dc->GetSize()[1];
  int m = dc->GetSize()[2];
  int l = dc->GetSize()[3]; 

  int vol_n = vol.GetSize()[1];
  int vol_m = vol.GetSize()[2];
  int vol_l = vol.GetSize()[3];

  if ( ( vol_n % 2 == 0 ) || ( vol_m % 2 == 0 ) || ( vol_l % 2 == 0 ) )
      {
	return CUBE_WITHOUT_MIDPOINT;
      }

  Vector<int> coord(3); // contents the current coordinates

  massptr = mc.GetMassPointer();

  // choose moment to be treat
  for ( int momchoose = 0; momchoose < 10; momchoose++ )
    { 
      switch ( momchoose )
	{
	case 0:
	  current_mom = mc.GetMassPointer();
	  inte = &mass;
	  break;
	case 1:
	  current_mom = mc.GetFP1Pointer();
	  inte = &focal_point_x;
	  break;
	case 2:
	  current_mom = mc.GetFP2Pointer();
	  inte = &focal_point_y;
	  break;
	case 3:
	  current_mom = mc.GetFP3Pointer();
	  inte = &focal_point_z;
	  break;
	case 4:
	  current_mom = mc.GetIT11Pointer();
	  inte = &inertian_tensor11;
	  break;
	case 5:
	  current_mom = mc.GetIT12Pointer();
	  inte = &inertian_tensor12;
	  break;
	case 6:
	  current_mom = mc.GetIT13Pointer();
	  inte = &inertian_tensor13;
	  break;
	case 7:
	  current_mom = mc.GetIT22Pointer();
	  inte = &inertian_tensor22;
	  break;
	case 8:
	  current_mom = mc.GetIT23Pointer();
	  inte = &inertian_tensor23;
	  break;
	case 9:
	  current_mom = mc.GetIT33Pointer();
	  inte = &inertian_tensor33;
	  break;
	default : 
	  cout << " Error at choose of moment " << endl;
	  exit(1);
	}

 
      for ( j = 0; j < m; j++ )
	{
	  for ( i = 0; i < n; i++ )
	    {
	      for ( k = 0; k < l; k++ )
		{
		  vol.SetStart( i-vol_n/2, j-vol_m/2, k-vol_l/2 );
		  
		  res = 0.0;
		  
		  // lower boundary of integration domain in x-direction
		  bxl = vol.start[1];
		  if ( bxl < 0 ) bxl = 0;
		  
		  // upper boundary of integration domain in x-direction
		  bxu = vol.start[1] + vol_n - 1; 
		  if ( bxu >= n ) bxu = n-1;
		  
		  // lower boundary of integration domain in y-direction
		  byl = vol.start[2];
		  if ( byl < 0 ) byl = 0;
		  
		  // upper boundary of integration domain in y-direction
		  byu = vol.start[2] + vol_m - 1;
		  if ( byu >= m ) byu = m-1;
		  
		  
		  coord[3] = k;
		  
		  for ( j_lay = byl; j_lay <= byu; j_lay++ ){
		    
		    coord[2] = j_lay;
		    
		    for ( i_lay = bxl; i_lay <= bxu; i_lay++ ){
		      
		      coord[1] = i_lay;
		      
		      res += inte->g(data[i_lay+j_lay*n+k*n*m],coord);
		      
		    }
		  }
		  
		  // calculate boundarys of layer distribution
		  disbndl = k-vol_l/2;
		  if ( disbndl < 0 ) disbndl = 0;
		  
		  disbndu = k+vol_l/2;
		  if ( disbndu >= l ) disbndu = l-1;
		  
		  // distribute result
		  for ( dis = disbndl; dis <= disbndu; dis++ )
		    {
		      current_mom[i+j*n+dis*n*m] += res;
		    }
		  
		  
		  //  }// layers
		  
		  
		} // DataCube z direction
	      
	    } // DataCube x direction
	  
	} // DataCube y direction
      
      // divide by mass
      if ( ( momchoose > 0 ) && ( momchoose < 4 ) )
	{	  
	  for ( k = 0; k < l; k++ )
	    { 
	      for ( j = 0; j < m; j++ )
		{
		  for ( i = 0; i < n; i++ )
		    {	  
		      mass_help = massptr[i+j*n+k*n*m];
		      if ( mass_help )
			current_mom[i+j*n+k*n*m] /= mass_help;
		    }
		}
	    }
	  
	}// divide by mass 
      
      // after treatment inertian tensor
      switch ( momchoose )
	{
	case 0: break;
	case 1: break;
	case 2: break;
	case 3: break;
	case 4:
	  fpptr_a = mc.GetFP1Pointer();
	  fpptr_b = mc.GetFP1Pointer();	 
	  break;
	case 5:
	  fpptr_a = mc.GetFP1Pointer();
	  fpptr_b = mc.GetFP2Pointer();	 
	  break;
	case 6:
	  fpptr_a = mc.GetFP1Pointer();
	  fpptr_b = mc.GetFP3Pointer();	 
	  break;
	case 7:
	  fpptr_a = mc.GetFP2Pointer();
	  fpptr_b = mc.GetFP2Pointer();	 
	  break;
	case 8:
	  fpptr_a = mc.GetFP2Pointer();
	  fpptr_b = mc.GetFP3Pointer();	 	 
	  break;
	case 9:
	  fpptr_a = mc.GetFP3Pointer();
	  fpptr_b = mc.GetFP3Pointer();	 
	  break;
	default : 
	  cout << " Error at choose of moment " << endl;
	  exit(1);
	}

      if ( momchoose >= 4 )
	{
	  
	  for ( k = 0; k < l; k++ )
	    { 
	      for ( j = 0; j < m; j++ )
		{
		  for ( i = 0; i < n; i++ )
		    {	  
		      current_mom[i+j*n+k*n*m] -= fpptr_a[i+j*n+k*n*m] * 
		                                  fpptr_b[i+j*n+k*n*m] * 
			                          massptr[i+j*n+k*n*m];
		    }
		}
	    }
	  
	}  // after treatment inertian tensor

      
    } // choose of moment


//calculate inertian values from inertian tensor
  
  double **dm = dmatrix(1,3,1,3);
  double d[4];
  double e[4];
  
  NeuraDataType* it11ptr = mc.GetIT11Pointer();
  NeuraDataType* it12ptr = mc.GetIT12Pointer();
  NeuraDataType* it13ptr = mc.GetIT13Pointer();
  NeuraDataType* it22ptr = mc.GetIT22Pointer();
  NeuraDataType* it23ptr = mc.GetIT23Pointer();
  NeuraDataType* it33ptr = mc.GetIT33Pointer();

  int counter = 0;
  
  for ( k = 0; k < l; k++ )
    { 
      for ( j = 0; j < m; j++ )
	{
	  for ( i = 0; i < n; i++ )
	    {	  
	      dm[1][1] = it11ptr[i+j*n+k*n*m]; 
	      dm[1][2] = it12ptr[i+j*n+k*n*m]; 
	      dm[1][3] = it13ptr[i+j*n+k*n*m]; 
	      dm[2][1] = it12ptr[i+j*n+k*n*m]; 
	      dm[2][2] = it22ptr[i+j*n+k*n*m]; 
	      dm[2][3] = it23ptr[i+j*n+k*n*m];
	      dm[3][1] = it13ptr[i+j*n+k*n*m];
	      dm[3][2] = it23ptr[i+j*n+k*n*m];
	      dm[3][3] = it33ptr[i+j*n+k*n*m];
	      
	      tred2(dm,3,d,e);
	      tqli(d,e,3,dm);

	      (vals[counter])[1] = (NeuraDataType)d[1];
	      vals[counter][2] = (NeuraDataType)d[2];
	      vals[counter][3] = (NeuraDataType)d[3];
	      vects[counter][1] = (NeuraDataType)dm[1][1];
	      vects[counter][2] = (NeuraDataType)dm[2][1];
	      vects[counter][3] = (NeuraDataType)dm[3][1];
	      vects[counter][4] = (NeuraDataType)dm[1][2];
	      vects[counter][5] = (NeuraDataType)dm[2][2];
	      vects[counter][6] = (NeuraDataType)dm[3][2];
	      vects[counter][7] = (NeuraDataType)dm[1][3];
	      vects[counter][8] = (NeuraDataType)dm[2][3];
	      vects[counter][9] = (NeuraDataType)dm[3][3];

	      counter++;

	    }
	}
    }

  free_dmatrix(dm,1,3,1,3);
 
  return OK;
}



/*! calculates the inertian values for all elements in datacube by combining
 * points to discs 
 * only for 3d usable
 */
int 
Moments::
elemwise_fast_all_inertian_values( Vector<NeuraDataType> *vals, Vector<NeuraDataType> *vects, Volume &vol)
{
  // check correctness of dimensions
  if ( dc->dim != vol.dim ) return DCDIM_NEQ_VOLDIM;

  int i,j,k;
  int i_lay, j_lay;
  int dis, disbndl, disbndu;
  NeuraDataType res;
  NeuraDataType mass_help;

  NeuraDataType* data;
  data = dc->GetDataPointer();

  NeuraDataType* current_mom; // points to the current moment field

  NeuraDataType* massptr; // pointer to mass field for to be able to divide calculated
                   // current moment by mass
  NeuraDataType* fpptr_a;
  NeuraDataType* fpptr_b;

  // define MomentCube to store moments
  MomentCube mc(3);
  Vector<int> mc_size(3);
  mc_size.SetElement(1,dc->GetSize()[1]-1); 
  mc_size.SetElement(2,dc->GetSize()[2]-1);
  mc_size.SetElement(3,dc->GetSize()[3]-1);
  mc.SetCubeSizeAndSpaceing(mc_size,dc->GetSpaceing());
  mc.MallocMomentCube();

  // define needed integrant classes
  Integrant* inte;
  MassIntegrant mass;
  Focal_point_x focal_point_x;
  Focal_point_y focal_point_y; 
  Focal_point_z focal_point_z;
  Inertian_tensor11 inertian_tensor11;
  Inertian_tensor12 inertian_tensor12;
  Inertian_tensor13 inertian_tensor13;
  Inertian_tensor22 inertian_tensor22;
  Inertian_tensor23 inertian_tensor23;
  Inertian_tensor33 inertian_tensor33;

  int bxl, bxu, byl, byu;

  int n = dc->GetSize()[1];
  int m = dc->GetSize()[2];
  int l = dc->GetSize()[3]; 

  int nelem = dc->GetSize()[1]-1;
  int melem = dc->GetSize()[2]-1;
  int lelem = dc->GetSize()[3]-1; 

  int vol_n = vol.GetSize()[1];
  int vol_m = vol.GetSize()[2];
  int vol_l = vol.GetSize()[3];

  if ( ( vol_n % 2 != 0 ) || ( vol_m % 2 != 0 ) || ( vol_l % 2 != 0 ) )
      {
	return CUBE_WITHOUT_MIDELEM;
      }

  Vector<int> coord(3); // contents the current coordinates

  massptr = mc.GetMassPointer();

  // choose moment to be treat
  for ( int momchoose = 0; momchoose < 10; momchoose++ )
    { 
      switch ( momchoose )
	{
	case 0:
	  current_mom = mc.GetMassPointer();
	  inte = &mass;
	  break;
	case 1:
	  current_mom = mc.GetFP1Pointer();
	  inte = &focal_point_x;
	  break;
	case 2:
	  current_mom = mc.GetFP2Pointer();
	  inte = &focal_point_y;
	  break;
	case 3:
	  current_mom = mc.GetFP3Pointer();
	  inte = &focal_point_z;
	  break;
	case 4:
	  current_mom = mc.GetIT11Pointer();
	  inte = &inertian_tensor11;
	  break;
	case 5:
	  current_mom = mc.GetIT12Pointer();
	  inte = &inertian_tensor12;
	  break;
	case 6:
	  current_mom = mc.GetIT13Pointer();
	  inte = &inertian_tensor13;
	  break;
	case 7:
	  current_mom = mc.GetIT22Pointer();
	  inte = &inertian_tensor22;
	  break;
	case 8:
	  current_mom = mc.GetIT23Pointer();
	  inte = &inertian_tensor23;
	  break;
	case 9:
	  current_mom = mc.GetIT33Pointer();
	  inte = &inertian_tensor33;
	  break;
	default : 
	  cout << " Error at choose of moment " << endl;
	  exit(1);
	}

     
      for ( j = 0; j < melem; j++ )
	{
	  for ( i = 0; i < nelem; i++ )
	    {
	      for ( k = 0; k < l; k++ )
		{
		  vol.SetStart( i-(vol_n/2-1), j-(vol_m/2-1), k-(vol_l/2-1) );

		      res = 0.0;
		      
		      // lower boundary of integration domain in x-direction
		      bxl = vol.start[1];
		      if ( bxl < 0 ) bxl = 0;
		      
		      // upper boundary of integration domain in x-direction
		      bxu = vol.start[1] + vol_n - 1; 
		      if ( bxu >= n ) bxu = n-1;
		      
		      // lower boundary of integration domain in y-direction
		      byl = vol.start[2];
		      if ( byl < 0 ) byl = 0;
		      
		      // upper boundary of integration domain in y-direction
		      byu = vol.start[2] + vol_m - 1;
		      if ( byu >= m ) byu = m-1;
		      
		      coord[3] = k;
		      
		      for ( j_lay = byl; j_lay <= byu; j_lay++ ){
			
			coord[2] = j_lay;
			
			for ( i_lay = bxl; i_lay <= bxu; i_lay++ ){
			  
			  coord[1] = i_lay;
			  	  
			  res += inte->g(data[i_lay+j_lay*n+k*n*m],coord);
			  
			}
		      }
		      
		      // calculate boundarys of layer distribution
		      disbndl = k-vol_l/2;
		      if ( disbndl < 0 ) disbndl = 0;
		      
		      disbndu = k+vol_l/2-1;
		      if ( disbndu >= lelem ) disbndu = lelem-1;		      
		    
		      // distribute result
		      for ( dis = disbndl; dis <= disbndu; dis++ )
			{
			  current_mom[i+j*nelem+dis*nelem*melem] += res;
			}
		      
	     
		} // DataCube z direction
	      
	    } // MomentCube x direction
	  
	} // MomentCube y direction
      
      // divide focal point by mass
      if ( ( momchoose > 0 ) && ( momchoose < 4 ) )
	{	
	  for ( k = 0; k < lelem; k++ )
	    { 
	      for ( j = 0; j < melem; j++ )
		{
		  for ( i = 0; i < nelem; i++ )
		    {	  
		      mass_help = massptr[i+j*nelem+k*nelem*melem];
		      if ( mass_help )
			current_mom[i+j*nelem+k*nelem*melem] /= mass_help;

		    }
		}
	    }
	  
	}// divide by mass 
      
      // after treatment inertian tensor
      switch ( momchoose )
	{
	case 0: break;
	case 1: break;
	case 2: break;
	case 3: break;
	case 4:
	  fpptr_a = mc.GetFP1Pointer();
	  fpptr_b = mc.GetFP1Pointer();	 
	  break;
	case 5:
	  fpptr_a = mc.GetFP1Pointer();
	  fpptr_b = mc.GetFP2Pointer();	 
	  break;
	case 6:
	  fpptr_a = mc.GetFP1Pointer();
	  fpptr_b = mc.GetFP3Pointer();	 
	  break;
	case 7:
	  fpptr_a = mc.GetFP2Pointer();
	  fpptr_b = mc.GetFP2Pointer();	 
	  break;
	case 8:
	  fpptr_a = mc.GetFP2Pointer();
	  fpptr_b = mc.GetFP3Pointer();	 	 
	  break;
	case 9:
	  fpptr_a = mc.GetFP3Pointer();
	  fpptr_b = mc.GetFP3Pointer();	 
	  break;
	default : 
	  cout << " Error at choose of moment " << endl;
	  exit(1);
	}

      if ( momchoose >= 4 )
	{ 
	  for ( k = 0; k < lelem; k++ )
	    { 
	      for ( j = 0; j < melem; j++ )
		{
		  for ( i = 0; i < nelem; i++ )
		    {	  
		      current_mom[i+j*nelem+k*nelem*melem] -= fpptr_a[i+j*nelem+k*nelem*melem] * 
		                                  fpptr_b[i+j*nelem+k*nelem*melem] * 
			                          massptr[i+j*nelem+k*nelem*melem];
		    }
		}
	    }
	  
	}  // after treatment inertian tensor

      
    } // choose of moment


//calculate inertian values from inertian tensor
  
  double **dm = dmatrix(1,3,1,3);
  double d[4];
  double e[4];
  
  NeuraDataType* it11ptr = mc.GetIT11Pointer();
  NeuraDataType* it12ptr = mc.GetIT12Pointer();
  NeuraDataType* it13ptr = mc.GetIT13Pointer();
  NeuraDataType* it22ptr = mc.GetIT22Pointer();
  NeuraDataType* it23ptr = mc.GetIT23Pointer();
  NeuraDataType* it33ptr = mc.GetIT33Pointer();

  int counter = 0;
  
  for ( k = 0; k < lelem; k++ )
    { 
      for ( j = 0; j < melem; j++ )
	{
	  for ( i = 0; i < nelem; i++ )
	    {	  
	      dm[1][1] = it11ptr[i+j*nelem+k*nelem*melem]; 
	      dm[1][2] = it12ptr[i+j*nelem+k*nelem*melem]; 
	      dm[1][3] = it13ptr[i+j*nelem+k*nelem*melem]; 
	      dm[2][1] = it12ptr[i+j*nelem+k*nelem*melem]; 
	      dm[2][2] = it22ptr[i+j*nelem+k*nelem*melem]; 
	      dm[2][3] = it23ptr[i+j*nelem+k*nelem*melem];
	      dm[3][1] = it13ptr[i+j*nelem+k*nelem*melem];
	      dm[3][2] = it23ptr[i+j*nelem+k*nelem*melem];
	      dm[3][3] = it33ptr[i+j*nelem+k*nelem*melem];
	      
	      tred2(dm,3,d,e);
	      tqli(d,e,3,dm);

	      vals[counter][1] = (NeuraDataType)d[1];
	      vals[counter][2] = (NeuraDataType)d[2];
	      vals[counter][3] = (NeuraDataType)d[3];
	      vects[counter][1] = (NeuraDataType)dm[1][1];
	      vects[counter][2] = (NeuraDataType)dm[2][1];
	      vects[counter][3] = (NeuraDataType)dm[3][1];
	      vects[counter][4] = (NeuraDataType)dm[1][2];
	      vects[counter][5] = (NeuraDataType)dm[2][2];
	      vects[counter][6] = (NeuraDataType)dm[3][2];
	      vects[counter][7] = (NeuraDataType)dm[1][3];
	      vects[counter][8] = (NeuraDataType)dm[2][3];
	      vects[counter][9] = (NeuraDataType)dm[3][3];

	      counter++;

	    }
	}
    }

  free_dmatrix(dm,1,3,1,3);
 
  return OK;
}


/*!calculates the inertian values for all elements in datacube by comining 
 * points to discs 
 * sums on discs are calculated by moving old disc
 *only for 3d usable
 */
int 
Moments::
elemwise_fast2_all_inertian_values( Vector<NeuraDataType> *vals, Vector<NeuraDataType> *vects, Volume &vol)
{
  // check correctness of dimensions
  if ( dc->dim != vol.dim ) return DCDIM_NEQ_VOLDIM;

  int i,j,k;
  int i_lay, j_lay;
  int dis, disbndl, disbndu;
  NeuraDataType res;
  NeuraDataType disc_i = 0;  // for to store discs that are to be moved
  NeuraDataType disc_ij = 0; // for to store discs that are to be moved 
  NeuraDataType mass_help;

  NeuraDataType* data;
  data = dc->GetDataPointer();

  NeuraDataType* current_mom; // points to the current moment field

  NeuraDataType* massptr; // pointer to mass field for to be able to divide calculated
                   // current moment by mass
  NeuraDataType* fpptr_a;
  NeuraDataType* fpptr_b;

  // define MomentCube to store moments
  MomentCube mc(3);
  Vector<int> mc_size(3);
  mc_size.SetElement(1,dc->GetSize()[1]-1); 
  mc_size.SetElement(2,dc->GetSize()[2]-1);
  mc_size.SetElement(3,dc->GetSize()[3]-1);
  mc.SetCubeSizeAndSpaceing(mc_size,dc->GetSpaceing());
  mc.MallocMomentCube();

  // define needed integrant classes
  Integrant* inte;
  MassIntegrant mass;
  Focal_point_x focal_point_x;
  Focal_point_y focal_point_y; 
  Focal_point_z focal_point_z;
  Inertian_tensor11 inertian_tensor11;
  Inertian_tensor12 inertian_tensor12;
  Inertian_tensor13 inertian_tensor13;
  Inertian_tensor22 inertian_tensor22;
  Inertian_tensor23 inertian_tensor23;
  Inertian_tensor33 inertian_tensor33;

  int bxl, bxu, byl, byu;

  int n = dc->GetSize()[1];
  int m = dc->GetSize()[2];
  int l = dc->GetSize()[3]; 

  int nelem = dc->GetSize()[1]-1;
  int melem = dc->GetSize()[2]-1;
  int lelem = dc->GetSize()[3]-1; 

  int vol_n = vol.GetSize()[1];
  int vol_m = vol.GetSize()[2];
  int vol_l = vol.GetSize()[3];

  if ( ( vol_n % 2 != 0 ) || ( vol_m % 2 != 0 ) || ( vol_l % 2 != 0 ) )
      {
	return CUBE_WITHOUT_MIDELEM;
      }

  Vector<int> coord(3); // contents the current coordinates

  massptr = mc.GetMassPointer();

  // choose moment to be treat
  for ( int momchoose = 0; momchoose < 10; momchoose++ )
    { 
      switch ( momchoose )
	{
	case 0:
	  current_mom = mc.GetMassPointer();
	  inte = &mass;
	  break;
	case 1:
	  current_mom = mc.GetFP1Pointer();
	  inte = &focal_point_x;
	  break;
	case 2:
	  current_mom = mc.GetFP2Pointer();
	  inte = &focal_point_y;
	  break;
	case 3:
	  current_mom = mc.GetFP3Pointer();
	  inte = &focal_point_z;
	  break;
	case 4:
	  current_mom = mc.GetIT11Pointer();
	  inte = &inertian_tensor11;
	  break;
	case 5:
	  current_mom = mc.GetIT12Pointer();
	  inte = &inertian_tensor12;
	  break;
	case 6:
	  current_mom = mc.GetIT13Pointer();
	  inte = &inertian_tensor13;
	  break;
	case 7:
	  current_mom = mc.GetIT22Pointer();
	  inte = &inertian_tensor22;
	  break;
	case 8:
	  current_mom = mc.GetIT23Pointer();
	  inte = &inertian_tensor23;
	  break;
	case 9:
	  current_mom = mc.GetIT33Pointer();
	  inte = &inertian_tensor33;
	  break;
	default : 
	  cout << " Error at choose of moment " << endl;
	  exit(1);
	}
      
      
      for ( k = 0; k < l; k++ )
	{
	  vol.SetStart( -(vol_n/2-1), -(vol_m/2-1), vol.start[3] );

	  for ( i = 0; i < nelem; i++ )     
	    { 
	      vol.SetStart( vol.start[1], -(vol_m/2-1), vol.start[3] );

	      for ( j = 0; j < melem; j++ )
		{
		  
		  res = 0.0;

		  if ( ( i == 0 ) && ( j == 0 ) ) // build disc
		    {
		      
		      // lower boundary of integration domain in x-direction
		      bxl = 0;
		      
		      // upper boundary of integration domain in x-direction
		      bxu = vol_n/2; 
		      if ( bxu >= n ) bxu = n-1;
		      
		      // lower boundary of integration domain in y-direction
		      byl = 0;
		      
		      // upper boundary of integration domain in y-direction
		      byu = vol_m/2;
		      if ( byu >= m ) byu = m-1;
		      
		      coord[3] = k;
		      
		      for ( j_lay = byl; j_lay <= byu; j_lay++ ){
			
			coord[2] = j_lay;
			
			for ( i_lay = bxl; i_lay <= bxu; i_lay++ ){
			  
			  coord[1] = i_lay;
			  
			  res += inte->g(data[i_lay+j_lay*n+k*n*m],coord);
			  
			}
		      }

		      disc_i = res; // store result
		      disc_ij = res; // store result
		      
		    }// builded disc

		  else // move disc
		    {
		      if ( j == 0 ) // move disc_i in x-direction
			{
			  // get result of old disc
			  res = disc_i; 
				 
			  bxl = vol.start[1]; 
			  bxu = vol.start[1] + vol_n - 1;
			  byl = vol.start[2]; if ( byl < 0 ) byl = 0;
			  byu = vol.start[2] + vol_m - 1; if ( byu >= m ) byu = m-1;
			  
			  if ( bxl >= 0 ) // erase left boundary
			    {
			      coord[3] = k;
			      coord[1] = bxl;

			      for ( j_lay = byl; j_lay <= byu; j_lay++ )
				{
				  coord[2] = j_lay;
				  res -= inte->g(data[bxl+j_lay*n+k*n*m],coord);

				}
			    }
			  if ( bxu < n-1 ) // add new right boundary
			    {
			      coord[3] = k;
			      coord[1] = bxu+1;

			      for ( j_lay = byl; j_lay <= byu; j_lay++ )
				{
				  coord[2] = j_lay;
				  res += inte->g(data[bxu+1+j_lay*n+k*n*m],coord);

				}
			    }
			  
			  vol.SetStart( vol.start[1]+1, vol.start[2], vol.start[3]);
			    disc_i = res;
			    disc_ij = res;

			}

		      else // move disc_ij in y-direction
			{
			  res = disc_ij; // get result of old disc

			  byl = vol.start[2]; 
			  byu = vol.start[2] + vol_m - 1;
			  bxl = vol.start[1]; if ( bxl < 0 ) bxl = 0;
			  bxu = vol.start[1] + vol_n - 1; if ( bxu >= n ) bxu = n-1;
			  
			  if ( byl >= 0 ) // erase front boundary
			    {
			      coord[3] = k;
			      coord[2] = byl;

			      for ( i_lay = bxl; i_lay <= bxu; i_lay++ )
				{
				  coord[1] = i_lay;
				  res -= inte->g(data[i_lay+byl*n+k*n*m],coord);

				}
			    }
			  if ( byu < m-1 ) // add new back boundary
			    {
			      coord[3] = k;
			      coord[2] = byu+1;

			      for ( i_lay = bxl; i_lay <= bxu; i_lay++ )
				{
				  coord[1] = i_lay;
				  res += inte->g(data[i_lay+(byu+1)*n+k*n*m],coord);

				}
			    }
			  
			  vol.SetStart( vol.start[1], vol.start[2]+1, vol.start[3]);
			    disc_ij = res;

			}
		      
		    } // moved disc
		  
		  // calculate boundarys for disc distribution
		  disbndl = k-vol_l/2;
		  if ( disbndl < 0 ) disbndl = 0;
		  
		  disbndu = k+vol_l/2-1;
		  if ( disbndu >= lelem ) disbndu = lelem-1;		      
		  
		  // distribute result
		  for ( dis = disbndl; dis <= disbndu; dis++ )
		    {
		      current_mom[i+j*nelem+dis*nelem*melem] += res;
		      
		    }
		  
		  
		} // DataCube y direction
	      
	    } // DataCube x direction
	  
	} // DataCube z direction
      
      // divide by mass
      if ( ( momchoose > 0 ) && ( momchoose < 4 ) ) 
	{	
	  for ( k = 0; k < lelem; k++ )
	    { 
	      for ( j = 0; j < melem; j++ )
		{
		  for ( i = 0; i < nelem; i++ )
		    {	  
		      mass_help = massptr[i+j*nelem+k*nelem*melem];
		      if ( mass_help )
			current_mom[i+j*nelem+k*nelem*melem] /= mass_help;

		    }
		}
	    }
	  
	}// divide by mass 
      
      // after treatment inertian tensor
      switch ( momchoose )
	{
	case 0: break;
	case 1: break;
	case 2: break;
	case 3: break;
	case 4:
	  fpptr_a = mc.GetFP1Pointer();
	  fpptr_b = mc.GetFP1Pointer();	 
	  break;
	case 5:
	  fpptr_a = mc.GetFP1Pointer();
	  fpptr_b = mc.GetFP2Pointer();	 
	  break;
	case 6:
	  fpptr_a = mc.GetFP1Pointer();
	  fpptr_b = mc.GetFP3Pointer();	 
	  break;
	case 7:
	  fpptr_a = mc.GetFP2Pointer();
	  fpptr_b = mc.GetFP2Pointer();	 
	  break;
	case 8:
	  fpptr_a = mc.GetFP2Pointer();
	  fpptr_b = mc.GetFP3Pointer();	 	 
	  break;
	case 9:
	  fpptr_a = mc.GetFP3Pointer();
	  fpptr_b = mc.GetFP3Pointer();	 
	  break;
	default : 
	  cout << " Error at choose of moment " << endl;
	  exit(1);
	}

      if ( momchoose >= 4 )
	{ 
	  for ( k = 0; k < lelem; k++ )
	    { 
	      for ( j = 0; j < melem; j++ )
		{
		  for ( i = 0; i < nelem; i++ )
		    {	  
		      current_mom[i+j*nelem+k*nelem*melem] -= fpptr_a[i+j*nelem+k*nelem*melem] * 
		                                              fpptr_b[i+j*nelem+k*nelem*melem]* 
			                                      massptr[i+j*nelem+k*nelem*melem];;
		    }
		}
	    }
	  
	}  // after treatment inertian tensor

      
    } // choose of moment


//calculate inertian values from inertian tensor
  
  double **dm = dmatrix(1,3,1,3);
  double d[4];
  double e[4];
  
  NeuraDataType* it11ptr = mc.GetIT11Pointer();
  NeuraDataType* it12ptr = mc.GetIT12Pointer();
  NeuraDataType* it13ptr = mc.GetIT13Pointer();
  NeuraDataType* it22ptr = mc.GetIT22Pointer();
  NeuraDataType* it23ptr = mc.GetIT23Pointer();
  NeuraDataType* it33ptr = mc.GetIT33Pointer();

  int counter = 0;
  
  for ( k = 0; k < lelem; k++ )
    { 
      for ( j = 0; j < melem; j++ )
	{
	  for ( i = 0; i < nelem; i++ )
	    {	  
	      dm[1][1] = it11ptr[i+j*nelem+k*nelem*melem]; 
	      dm[1][2] = it12ptr[i+j*nelem+k*nelem*melem]; 
	      dm[1][3] = it13ptr[i+j*nelem+k*nelem*melem]; 
	      dm[2][1] = it12ptr[i+j*nelem+k*nelem*melem]; 
	      dm[2][2] = it22ptr[i+j*nelem+k*nelem*melem]; 
	      dm[2][3] = it23ptr[i+j*nelem+k*nelem*melem];
	      dm[3][1] = it13ptr[i+j*nelem+k*nelem*melem];
	      dm[3][2] = it23ptr[i+j*nelem+k*nelem*melem];
	      dm[3][3] = it33ptr[i+j*nelem+k*nelem*melem];
	      
	      tred2(dm,3,d,e);
	      tqli(d,e,3,dm);

	      vals[counter][1] = (NeuraDataType)d[1];
	      vals[counter][2] = (NeuraDataType)d[2];
	      vals[counter][3] = (NeuraDataType)d[3];
	      vects[counter][1] = (NeuraDataType)dm[1][1];
	      vects[counter][2] = (NeuraDataType)dm[2][1];
	      vects[counter][3] = (NeuraDataType)dm[3][1];
	      vects[counter][4] = (NeuraDataType)dm[1][2];
	      vects[counter][5] = (NeuraDataType)dm[2][2];
	      vects[counter][6] = (NeuraDataType)dm[3][2];
	      vects[counter][7] = (NeuraDataType)dm[1][3];
	      vects[counter][8] = (NeuraDataType)dm[2][3];
	      vects[counter][9] = (NeuraDataType)dm[3][3];

	      counter++;

	    }
	}
    }

  free_dmatrix(dm,1,3,1,3);
 
  return OK;
}


/*!
 * decides whether size is a power of two or not
 */
int 
Moments::
pow_of_two(int size)
{
  if ( size % 2 == 1 ) return 0;
  if ( ( size == 2 ) || ( size == 0 ) ) return 1;
  return pow_of_two(size/2);
}


/*! calculates the inertian values for all points in datacube by using 
 * fast fourier transform
 * builded on a example of NumericalRecipes
 */
int 
Moments::
fourier_all_inertian_values(Vector<NeuraDataType> *vals, Vector<NeuraDataType> *vects, Volume &vol, int N, int M, int L)
{
 
  int i,j,k;
  NeuraDataType mass_help;

  // example from NumericalRecipes
  float fac,re,im,***data1,***data2,**speq1,**speq2,*sp1,*sp2;

  data1=f3tensor(1,N,1,M,1,L);
  data2=f3tensor(1,N,1,M,1,L);
  speq1=matrix(1,N,1,2*M);
  speq2=matrix(1,N,1,2*M);

  // end example from NumericalRecipes

  NeuraDataType* data;
  data = dc->GetDataPointer();
  
  NeuraDataType* current_mom; // points to the current moment field
  
  NeuraDataType* massptr; // pointer to mass field for to be able to divide calculated
                   // current moment by mass
  NeuraDataType* fpptr_a;
  NeuraDataType* fpptr_b;

  int iw,jw,kw; // only for ball integration
  NeuraDataType* weights; // only for ball-integration
  Integration inte; // only for ball-integration

  int n = dc->GetSize()[1];
  int m = dc->GetSize()[2];
  int l = dc->GetSize()[3];

  int vol_n = vol.GetSize()[1];
  int vol_m = vol.GetSize()[2];
  int vol_l = vol.GetSize()[3];

  if ( 
      ( !pow_of_two(n) ) ||
      ( !pow_of_two(m) ) ||
      ( !pow_of_two(l) ) 
      )
    {
      return SIZE_NOT_POW_OF_TWO;
    }

  if ( vol.GetGeometryType() == BALL )
    {
      vol_n = vol_n*2 + 1;
      vol_m = vol_m*2 + 1;
      vol_l = vol_l*2 + 1;
      
      if ( 
	  ( inte.GetBallRadX() != vol.size[1] ) || 
	  ( inte.GetBallRadY() != vol.size[2] ) ||
	  ( inte.GetBallRadZ() != vol.size[3] ) ||
	  ( !inte.GetBallRadSet() ) 
	  )
	{ 
	  inte.computeBallWeights3d(vol);
	}
      weights = inte.BallWeights.GetDataPointer();      
    }
  else{
    
    if ( ( vol_n%2 == 0 ) || ( vol_m%2 == 0 ) || ( vol_l%2 == 0 ) )
      {
	return CUBE_WITHOUT_MIDPOINT;
      }
  } 

 //  // print weights
//   cout << "KugelGewichte: " << endl;
//   for ( k = 0; k < vol_l; k++ ){
//     for ( j = 0; j < vol_m; j++ ){
//       for ( i = 0; i < vol_n; i++ ){  
// 	cout << weights[i+j*vol_n+k*vol_n*vol_m] << " "; 
//       }
//       cout << endl;
//     }
//     cout << endl;
//   }
  
  
  // define MomentCube to store moments
  MomentCube mc(3);
  Vector<int> mc_size(3);
  mc_size.SetElement(1,n); 
  mc_size.SetElement(2,m);
  mc_size.SetElement(3,l);
  mc.SetCubeSizeAndSpaceing(mc_size,dc->GetSpaceing());
  mc.MallocMomentCube();
  
  massptr = mc.GetMassPointer();

 // choose moment to be treat
  for ( int momchoose = 0; momchoose < 10; momchoose++ )
    {   
      // define datacube and clear convolution kernel
      for ( k = 0; k < l; k++ ){
	for ( j = 0; j < m; j++ ){
	  for ( i = 0; i < n; i++ ){
	    data1[i+1][j+1][k+1] = (float)data[i+j*n+k*n*m];
	    data2[i+1][j+1][k+1] = 0;
	  }
	}
      }

      switch ( momchoose )
	{
	case 0:
	  current_mom = mc.GetMassPointer();
	  // define convolution kernel
	  for ( k = l-vol_l/2; k <= l+vol_l/2; k++ ){ 
	    for ( j = m-vol_m/2; j <= m+vol_m/2; j++ ){
	      for ( i = n-vol_n/2; i <= n+vol_n/2; i++ ){
		data2[i%n+1][j%m+1][k%l+1] = 1.0;
	      }
	    }
	  }
	  break;
	case 1:
	  current_mom = mc.GetFP1Pointer(); 
	  // define convolution kernel
	  for ( k = l-vol_l/2; k <= l+vol_l/2; k++ ){ 
	    for ( j = m-vol_m/2; j <= m+vol_m/2; j++ ){
	      for ( i = n-vol_n/2; i <= n+vol_n/2; i++ ){
		data2[i%n+1][j%m+1][k%l+1] = n-i;
	      }
	    }
	  }
	  break;
	case 2:
	  current_mom = mc.GetFP2Pointer(); 
	  // define convolution kernel
	  for ( k = l-vol_l/2; k <= l+vol_l/2; k++ ){ 
	    for ( j = m-vol_m/2; j <= m+vol_m/2; j++ ){
	      for ( i = n-vol_n/2; i <= n+vol_n/2; i++ ){
		data2[i%n+1][j%m+1][k%l+1] = m-j;
	      }
	    }
	  }
	  break;
	case 3:
	  current_mom = mc.GetFP3Pointer();
	  // define convolution kernel
	  for ( k = l-vol_l/2; k <= l+vol_l/2; k++ ){ 
	    for ( j = m-vol_m/2; j <= m+vol_m/2; j++ ){
	      for ( i = n-vol_n/2; i <= n+vol_n/2; i++ ){
		data2[i%n+1][j%m+1][k%l+1] = l-k;
	      }
	    }
	  }
	  break;
	case 4:
	  current_mom = mc.GetIT11Pointer();
	  // define convolution kernel
	  for ( k = l-vol_l/2; k <= l+vol_l/2; k++ ){ 
	    for ( j = m-vol_m/2; j <= m+vol_m/2; j++ ){
	      for ( i = n-vol_n/2; i <= n+vol_n/2; i++ ){
		data2[i%n+1][j%m+1][k%l+1] = (n-i)*(n-i);
	      }
	    }
	  }
	  break;
	case 5:
	  current_mom = mc.GetIT12Pointer();
	  // define convolution kernel
	  for ( k = l-vol_l/2; k <= l+vol_l/2; k++ ){ 
	    for ( j = m-vol_m/2; j <= m+vol_m/2; j++ ){
	      for ( i = n-vol_n/2; i <= n+vol_n/2; i++ ){
		data2[i%n+1][j%m+1][k%l+1] = (n-i)*(m-j);
	      }
	    }
	  }
	  break;
	case 6:
	  current_mom = mc.GetIT13Pointer();
	  // define convolution kernel
	  for ( k = l-vol_l/2; k <= l+vol_l/2; k++ ){ 
	    for ( j = m-vol_m/2; j <= m+vol_m/2; j++ ){
	      for ( i = n-vol_n/2; i <= n+vol_n/2; i++ ){
		data2[i%n+1][j%m+1][k%l+1] = (n-i)*(l-k);
	      }
	    }
	  }
	  break;
	case 7:
	  current_mom = mc.GetIT22Pointer();
	  // define convolution kernel
	  for ( k = l-vol_l/2; k <= l+vol_l/2; k++ ){ 
	    for ( j = m-vol_m/2; j <= m+vol_m/2; j++ ){
	      for ( i = n-vol_n/2; i <= n+vol_n/2; i++ ){
		data2[i%n+1][j%m+1][k%l+1] = (m-j)*(m-j);
	      }
	    }
	  }
	  break;
	case 8:
	  current_mom = mc.GetIT23Pointer();
	   // define convolution kernel
	  for ( k = l-vol_l/2; k <= l+vol_l/2; k++ ){ 
	    for ( j = m-vol_m/2; j <= m+vol_m/2; j++ ){
	      for ( i = n-vol_n/2; i <= n+vol_n/2; i++ ){
		data2[i%n+1][j%m+1][k%l+1] = (m-j)*(l-k);
	      }
	    }
	  }	   
	  break;
	case 9:
	  current_mom = mc.GetIT33Pointer();
	  // define convolution kernel
	  for ( k = l-vol_l/2; k <= l+vol_l/2; k++ ){ 
	    for ( j = m-vol_m/2; j <= m+vol_m/2; j++ ){
	      for ( i = n-vol_n/2; i <= n+vol_n/2; i++ ){
		data2[i%n+1][j%m+1][k%l+1] = (l-k)*(l-k);
	      }
	    }
	  }
	  break;
	default : 
	  cout << " Error at choose of moment " << endl;
	  exit(1);
	}
      
      // weight kernel
      if ( vol.GetGeometryType() == BALL ) 
	{ 
	  iw = 0; jw = 0; kw = 0;
	  for ( k = l-vol_l/2; k <= l+vol_l/2; k++ ){ 
	    for ( j = m-vol_m/2; j <= m+vol_m/2; j++ ){
	      for ( i = n-vol_n/2; i <= n+vol_n/2; i++ ){
		data2[i%n+1][j%m+1][k%l+1] *= (float)weights[iw+jw*vol_n+kw*vol_n*vol_m];
		iw++;
	      }
	      jw++;iw=0;
	    }
	    kw++;jw=0;iw=0;
	  }
	}
      
      // example from NumericalRecipes
      
      rlft3(data1,speq1,N,M,L,1);
      rlft3(data2,speq2,N,M,L,1);
      
      fac=2.0/(N*M*L);
      sp1 = &data1[1][1][1];
      sp2 = &data2[1][1][1];
      for (j=1;j<=N*M*L/2;j++){
	re = sp1[0]*sp2[0] - sp1[1]*sp2[1];
	im = sp1[0]*sp2[1] + sp1[1]*sp2[0];
	sp1[0] = fac*re;
	sp1[1] = fac*im;
	sp1 += 2;
	sp2 += 2;
      }
      sp1 = &speq1[1][1];
      sp2 = &speq2[1][1];
      for (j=1;j<=N*M;j++){
	re = sp1[0]*sp2[0] - sp1[1]*sp2[1];
	im = sp1[0]*sp2[1] + sp1[1]*sp2[0];
	sp1[0] = fac*re;
	sp1[1] = fac*im;
	sp1 += 2;
	sp2 += 2;
      }
      rlft3(data1,speq1,N,M,L,-1);
      
      // end example from NumericalRecipes

      // write data     
      for ( k = 0; k < l; k++ ){
	for ( j = 0; j < m; j++ ){
	  for ( i = 0; i < n; i++ ){     
	    current_mom[i+j*n+k*n*m] = data1[i+1][j+1][k+1]; 
	  }
	}
      }
      
      // divide by mass
      if ( ( momchoose > 0 ) && ( momchoose < 4 ) )
	{	  
	  for ( k = 0; k < l; k++ )
	    { 
	      for ( j = 0; j < m; j++ )
		{
		  for ( i = 0; i < n; i++ )
		    {	  
		      mass_help = massptr[i+j*n+k*n*m];
		      if ( mass_help )
			current_mom[i+j*n+k*n*m] /= mass_help;
		    }
		}
	    }
	  
	}// divide by mass 
      
      // after treatment inertian tensor
      switch ( momchoose )
	{
	case 0: break;
	case 1: break;
	case 2: break;
	case 3: break;
	case 4:
	  fpptr_a = mc.GetFP1Pointer();
	  fpptr_b = mc.GetFP1Pointer();	 
	  break;
	case 5:
	  fpptr_a = mc.GetFP1Pointer();
	  fpptr_b = mc.GetFP2Pointer();	 
	  break;
	case 6:
	  fpptr_a = mc.GetFP1Pointer();
	  fpptr_b = mc.GetFP3Pointer();	 
	  break;
	case 7:
	  fpptr_a = mc.GetFP2Pointer();
	  fpptr_b = mc.GetFP2Pointer();	 
	  break;
	case 8:
	  fpptr_a = mc.GetFP2Pointer();
	  fpptr_b = mc.GetFP3Pointer();	 	 
	  break;
	case 9:
	  fpptr_a = mc.GetFP3Pointer();
	  fpptr_b = mc.GetFP3Pointer();	 
	  break;
	default : 
	  cout << " Error at choose of moment " << endl;
	  exit(1);
	}

      if ( momchoose >= 4 )
	{
	  
	  for ( k = 0; k < l; k++ )
	    { 
	      for ( j = 0; j < m; j++ )
		{
		  for ( i = 0; i < n; i++ )
		    {	  
		      current_mom[i+j*n+k*n*m] -= fpptr_a[i+j*n+k*n*m] * 
		                                  fpptr_b[i+j*n+k*n*m] * 
			                          massptr[i+j*n+k*n*m];
		    }
		}
	    }
	  
	}  // after treatment inertian tensor



    }// momchoose
  
  free_matrix(speq2,1,N,1,2*M);
  free_matrix(speq1,1,N,1,2*M);
  free_f3tensor(data2,1,N,1,M,1,L);
  free_f3tensor(data1,1,N,1,M,1,L);
  
  //calculate inertian values from inertian tensor
  
  double **dm = dmatrix(1,3,1,3);
  double d[4];
  double e[4];
  
  NeuraDataType* it11ptr = mc.GetIT11Pointer();
  NeuraDataType* it12ptr = mc.GetIT12Pointer();
  NeuraDataType* it13ptr = mc.GetIT13Pointer();
  NeuraDataType* it22ptr = mc.GetIT22Pointer();
  NeuraDataType* it23ptr = mc.GetIT23Pointer();
  NeuraDataType* it33ptr = mc.GetIT33Pointer();

  int counter = 0;
  
  for ( k = 0; k < l; k++ )
    { 
      for ( j = 0; j < m; j++ )
	{
	  for ( i = 0; i < n; i++ )
	    {	  
	      dm[1][1] = it11ptr[i+j*n+k*n*m]; 
	      dm[1][2] = it12ptr[i+j*n+k*n*m]; 
	      dm[1][3] = it13ptr[i+j*n+k*n*m]; 
	      dm[2][1] = it12ptr[i+j*n+k*n*m]; 
	      dm[2][2] = it22ptr[i+j*n+k*n*m]; 
	      dm[2][3] = it23ptr[i+j*n+k*n*m];
	      dm[3][1] = it13ptr[i+j*n+k*n*m];
	      dm[3][2] = it23ptr[i+j*n+k*n*m];
	      dm[3][3] = it33ptr[i+j*n+k*n*m];
	      
	      tred2(dm,3,d,e);
	      tqli(d,e,3,dm);

	      (vals[counter])[1] = (NeuraDataType)d[1];
	      vals[counter][2] = (NeuraDataType)d[2];
	      vals[counter][3] = (NeuraDataType)d[3];
	      vects[counter][1] = (NeuraDataType)dm[1][1];
	      vects[counter][2] = (NeuraDataType)dm[2][1];
	      vects[counter][3] = (NeuraDataType)dm[3][1];
	      vects[counter][4] = (NeuraDataType)dm[1][2];
	      vects[counter][5] = (NeuraDataType)dm[2][2];
	      vects[counter][6] = (NeuraDataType)dm[3][2];
	      vects[counter][7] = (NeuraDataType)dm[1][3];
	      vects[counter][8] = (NeuraDataType)dm[2][3];
	      vects[counter][9] = (NeuraDataType)dm[3][3];

	      counter++;

	    }
	}
    }

  free_dmatrix(dm,1,3,1,3);
 
  
 
  return OK;

}



//********************************************************************************
//
// NUMERICAL RECIPES CODE
//
//********************************************************************************

// ---copied from numerical recipes--- 
void 
Moments::
nrerror(char error_text[])
  // Numerical Recipes standard error handler 
{
  fprintf(stderr,"Numerical Recipes run-time error...\n");
  fprintf(stderr,"%s\n",error_text);
  fprintf(stderr,"...now exiting to system...\n");
  exit(1);
}


// ---copied from numerical recipes---
#define NR_END 1
float **
Moments::
matrix(long nrl, long nrh, long ncl, long nch)
/* allocate a float matrix with subscript range m[nrl..nrh][ncl..nch] */
{
	long i, nrow=nrh-nrl+1,ncol=nch-ncl+1;
	float **m;

	/* allocate pointers to rows */
	m=(float **) malloc((size_t)((nrow+NR_END)*sizeof(float*)));
	if (!m) nrerror("allocation failure 1 in matrix()");
	m += NR_END;
	m -= nrl;

	/* allocate rows and set pointers to them */
	m[nrl]=(float *) malloc((size_t)((nrow*ncol+NR_END)*sizeof(float)));
	if (!m[nrl]) nrerror("allocation failure 2 in matrix()");
	m[nrl] += NR_END;
	m[nrl] -= ncl;

	for(i=nrl+1;i<=nrh;i++) m[i]=m[i-1]+ncol;

	/* return pointer to array of pointers to rows */
	return m;
}


// ---copied from numerical recipes---
double **
Moments::
dmatrix(long nrl, long nrh, long ncl, long nch)
  // allocate a double matrix with subscript range m[nrl..nrh][ncl..nch]/
{
  long i, nrow=nrh-nrl+1,ncol=nch-ncl+1;
  double **m;
  
  // allocate pointers to rows 
  m=(double **) malloc((size_t)((nrow+NR_END)*sizeof(double*)));
  if (!m) nrerror("allocation failure 1 in matrix()");
  m += NR_END;
  m -= nrl;
  
  // allocate rows and set pointers to them 
  m[nrl]=(double *) malloc((size_t)((nrow*ncol+NR_END)*sizeof(double)));
  if (!m[nrl]) nrerror("allocation failure 2 in matrix()");
  m[nrl] += NR_END;
  m[nrl] -= ncl;
  
  for(i=nrl+1;i<=nrh;i++) m[i]=m[i-1]+ncol;
  
  // return pointer to array of pointers to rows 
  return m;
}


// ---copied from numerical recipes---
float *** 
Moments::
f3tensor(long nrl, long nrh, long ncl, long nch, long ndl, long ndh)
/* allocate a float 3tensor with range t[nrl..nrh][ncl..nch][ndl..ndh] */
{
	long i,j,nrow=nrh-nrl+1,ncol=nch-ncl+1,ndep=ndh-ndl+1;
	float ***t;

	/* allocate pointers to pointers to rows */
	t=(float ***) malloc((size_t)((nrow+NR_END)*sizeof(float**)));
	if (!t) nrerror("allocation failure 1 in f3tensor()");
	t += NR_END;
	t -= nrl;

	/* allocate pointers to rows and set pointers to them */
	t[nrl]=(float **) malloc((size_t)((nrow*ncol+NR_END)*sizeof(float*)));
	if (!t[nrl]) nrerror("allocation failure 2 in f3tensor()");
	t[nrl] += NR_END;
	t[nrl] -= ncl;

	/* allocate rows and set pointers to them */
	t[nrl][ncl]=(float *) malloc((size_t)((nrow*ncol*ndep+NR_END)*sizeof(float)));
	if (!t[nrl][ncl]) nrerror("allocation failure 3 in f3tensor()");
	t[nrl][ncl] += NR_END;
	t[nrl][ncl] -= ndl;

	for(j=ncl+1;j<=nch;j++) t[nrl][j]=t[nrl][j-1]+ndep;
	for(i=nrl+1;i<=nrh;i++) {
		t[i]=t[i-1]+ncol;
		t[i][ncl]=t[i-1][ncl]+ncol*ndep;
		for(j=ncl+1;j<=nch;j++) t[i][j]=t[i][j-1]+ndep;
	}

	/* return pointer to array of pointers to rows */
	return t;
}


// ---copied from numerical recipes---
#define FREE_ARG char*
void 
Moments::
free_matrix(float **m, long nrl, long nrh, long ncl, long nch)
/* free a float matrix allocated by matrix() */
{
	free((FREE_ARG) (m[nrl]+ncl-NR_END));
	free((FREE_ARG) (m+nrl-NR_END));
}


// ---copied from numerical recipes---
void 
Moments::
free_dmatrix(double **m, long nrl, long nrh, long ncl, long nch)
/* free a double matrix allocated by dmatrix() */
{
	free((FREE_ARG) (m[nrl]+ncl-NR_END));
	free((FREE_ARG) (m+nrl-NR_END));
}


// ---copied from numerical recipes---
void 
Moments::
free_f3tensor(float ***t, long nrl, long nrh, long ncl, long nch,
	long ndl, long ndh)
/* free a float f3tensor allocated by f3tensor() */
{
	free((FREE_ARG) (t[nrl][ncl]+ndl-NR_END));
	free((FREE_ARG) (t[nrl]+ncl-NR_END));
	free((FREE_ARG) (t+nrl-NR_END));
}


// ---copied from numerical recipes---
static float sqrarg;
#define SQR(a) ((sqrarg=(a)) == 0.0 ? 0.0 : sqrarg*sqrarg)
float 
Moments::
pythag(float a, float b)
{
  float absa,absb;
  absa=fabs(a);
  absb=fabs(b);
  if (absa > absb) return absa*sqrt(1.0+SQR(absb/absa));
  else return (absb == 0.0 ? 0.0 : absb*sqrt(1.0+SQR(absa/absb)));
}


// ---copied from numerical recipes---
// reduces a matrix to tridiagonal form 
void
Moments::
tred2(double **a, int n, double d[], double e[])
{
  int l, k, j, i;
  double scale, hh, h, g, f;
  
  for ( i=n;i>=2;i--)
    {
      l=i-1;
      h=scale=0.0;
      if (l > 1){
	for ( k=1;k<=l;k++)
	  scale += fabs(a[i][k]);
	if(scale == 0.0)
	  e[i]=a[i][l];
	else{
	  for (k=1;k<=l;k++){
	    a[i][k] /= scale;
	    h += a[i][k]*a[i][k];
	  }
	  f=a[i][l];
	  g=(f >= 0.0 ? -sqrt(h) : sqrt(h));
	  e[i]=scale*g;
	  h-= f*g;
	  a[i][l]=f-g;
	  f=0.0;
	  for(j=1;j<=l;j++){
	    a[j][i]=a[i][j]/h;
	    g=0.0;
	    for (k=1;k<=j;k++)
	      g += a[j][k]*a[i][k];
	    for (k=j+1;k<=l;k++)
	      g += a[k][j]*a[i][k];
	    e[j]=g/h;
	    f += e[j]*a[i][j];
	  }
	  hh=f/(h+h);
	  for (j=1;j<=l;j++){
	    f=a[i][j];
	    e[j]=g=e[j]-hh*f;
	    for (k=1;k<=j;k++)
	      a[j][k] -= (f*e[k]+g*a[i][k]);
	  }
	}
      } else
	e[i]=a[i][l];
      d[i]=h;
    }
  
  d[1] = 0.0;
  e[1] = 0.0;
  
  for (i=1;i<=n;i++){
    l=i-1;
    if (d[i]){
      for (j=1;j<=l;j++){
	g=0.0;
	for(k=1;k<=l;k++)
	  g += a[i][k]*a[k][j];
	for (k=1;k<=l;k++)
	  a[k][j] -= g*a[k][i];
      }
    }
    d[i]=a[i][i];
    a[i][i]=1.0;
    for(j=1;j<=l;j++) a[j][i]=a[i][j]=0.0;
  }
}

// ---copied from numerical recipes---
// ---return value was changed by roland schulte from void to int---
#define SIGN(a,b) ((b) >= 0.0 ? fabs(a) : -fabs(a))
int
Moments::
tqli(double d[], double e[], int n, double **z)
{
  //float pythag(float a, float b);
  int u,v; // added by Roland Schulte
  int m,l,iter,i,k;
  double s,r,p,g,f,dd,c,b;
  
  for (i=2;i<=n;i++) e[i-1]=e[i];
  e[n]=0.0;
  for (l=1;l<=n;l++) {
    iter=0;
    do {
      for (m=l;m<=n-1;m++) {
	dd=fabs(d[m])+fabs(d[m+1]);
	if ((double)(fabs(e[m])+dd) == dd) break;
      }
      if (m != l) {
	if (iter++ == 30) 
	  { // **** changed by roland schulte here ! ****
	    //nrerror("Too many iterations in tqli");
	    cout << "too may iterations in tqli!" << endl;
	    for (v=1;v<=n;v++) d[v] = 1.0;
	    for (v=1;v<=n;v++){
	      for (u=1;u<=n;u++){
		if ( u == v ) z[u][v] = 1.0;
		else z[u][v] = 0.0;
	      }
	    }
	    return 0;
	  }
	g=(d[l+1]-d[l])/(2.0*e[l]);
	r=pythag(g,1.0);
	g=d[m]-d[l]+e[l]/(g+SIGN(r,g));
	s=c=1.0;
	p=0.0;
	for (i=m-1;i>=l;i--) {
	  f=s*e[i];
	  b=c*e[i];
	  e[i+1]=(r=pythag(f,g));
	  if (r == 0.0) {
	    d[i+1] -= p;
	    e[m]=0.0;
	    break;
	  }
	  s=f/r;
	  c=g/r;
	  g=d[i+1]-p;
	  r=(d[i]-g)*s+2.0*c*b;
	  d[i+1]=g+(p=s*r);
	  g=c*r-b;
	  for (k=1;k<=n;k++) {
	    f=z[k][i+1];
	    z[k][i+1]=s*z[k][i]+c*f;
	    z[k][i]=c*z[k][i]-s*f;
	  }
	}
	if (r == 0.0 && i >= l) continue;
	d[l] -= p;
	e[l]=g;
	e[m]=0.0;
      }
    } while (m != l);
  }
  return 0;
}


// -- copied from numerical recipes -- 
// -- code for n-dimensional complex fourier transform
#define SWAP(a,b) tempr=(a);(a)=(b);(b)=tempr

void 
Moments::
fourn(float data[], unsigned long nn[], int ndim, int isign)
{
	int idim;
	unsigned long i1,i2,i3,i2rev,i3rev,ip1,ip2,ip3,ifp1,ifp2;
	unsigned long ibit,k1,k2,n,nprev,nrem,ntot;
	float tempi,tempr;
	double theta,wi,wpi,wpr,wr,wtemp;

	for (ntot=1,idim=1;idim<=ndim;idim++)
		ntot *= nn[idim];
	nprev=1;
	for (idim=ndim;idim>=1;idim--) {
		n=nn[idim];
		nrem=ntot/(n*nprev);
		ip1=nprev << 1;
		ip2=ip1*n;
		ip3=ip2*nrem;
		i2rev=1;
		for (i2=1;i2<=ip2;i2+=ip1) {
			if (i2 < i2rev) {
				for (i1=i2;i1<=i2+ip1-2;i1+=2) {
					for (i3=i1;i3<=ip3;i3+=ip2) {
						i3rev=i2rev+i3-i2;
						SWAP(data[i3],data[i3rev]);
						SWAP(data[i3+1],data[i3rev+1]);
					}
				}
			}
			ibit=ip2 >> 1;
			while (ibit >= ip1 && i2rev > ibit) {
				i2rev -= ibit;
				ibit >>= 1;
			}
			i2rev += ibit;
		}
		ifp1=ip1;
		while (ifp1 < ip2) {
			ifp2=ifp1 << 1;
			theta=isign*6.28318530717959/(ifp2/ip1);
			wtemp=sin(0.5*theta);
			wpr = -2.0*wtemp*wtemp;
			wpi=sin(theta);
			wr=1.0;
			wi=0.0;
			for (i3=1;i3<=ifp1;i3+=ip1) {
				for (i1=i3;i1<=i3+ip1-2;i1+=2) {
					for (i2=i1;i2<=ip3;i2+=ifp2) {
						k1=i2;
						k2=k1+ifp1;
						tempr=(float)wr*data[k2]-(float)wi*data[k2+1];
						tempi=(float)wr*data[k2+1]+(float)wi*data[k2];
						data[k2]=data[k1]-tempr;
						data[k2+1]=data[k1+1]-tempi;
						data[k1] += tempr;
						data[k1+1] += tempi;
					}
				}
				wr=(wtemp=wr)*wpr-wi*wpi+wr;
				wi=wi*wpr+wtemp*wpi+wi;
			}
			ifp1=ifp2;
		}
		nprev *= n;
	}
}
#undef SWAP



// -- copied from numerical recipes --
// code for threedimensional real fourier transform
void 
Moments::
rlft3(float ***data, float **speq, unsigned long nn1, unsigned long nn2,
	unsigned long nn3, int isign)
{
  //void fourn(float data[], unsigned long nn[], int ndim, int isign);
  //void nrerror(char error_text[]);
	unsigned long i1,i2,i3,j1,j2,j3,nn[4],ii3;
	double theta,wi,wpi,wpr,wr,wtemp;
	float c1,c2,h1r,h1i,h2r,h2i;

 

	if (1+&data[nn1][nn2][nn3]-&data[1][1][1] != nn1*nn2*nn3)
		nrerror("rlft3: problem with dimensions or contiguity of data array\n");
	c1=0.5;
	c2 = -0.5*isign;
	theta=isign*(6.28318530717959/nn3);
	wtemp=sin(0.5*theta);
	wpr = -2.0*wtemp*wtemp;
	wpi=sin(theta);
	nn[1]=nn1;
	nn[2]=nn2;
	nn[3]=nn3 >> 1;
	if (isign == 1) {
		fourn(&data[1][1][1]-1,nn,3,isign);
		for (i1=1;i1<=nn1;i1++)
			for (i2=1,j2=0;i2<=nn2;i2++) {
				speq[i1][++j2]=data[i1][i2][1];
				speq[i1][++j2]=data[i1][i2][2];
			}
	}
	for (i1=1;i1<=nn1;i1++) {
		j1=(i1 != 1 ? nn1-i1+2 : 1);
		wr=1.0;
		wi=0.0;
		for (ii3=1,i3=1;i3<=(nn3>>2)+1;i3++,ii3+=2) {
			for (i2=1;i2<=nn2;i2++) {
				if (i3 == 1) {
					j2=(i2 != 1 ? ((nn2-i2)<<1)+3 : 1);
					h1r=c1*(data[i1][i2][1]+speq[j1][j2]);
					h1i=c1*(data[i1][i2][2]-speq[j1][j2+1]);
					h2i=c2*(data[i1][i2][1]-speq[j1][j2]);
					h2r= -c2*(data[i1][i2][2]+speq[j1][j2+1]);
					data[i1][i2][1]=h1r+h2r;
					data[i1][i2][2]=h1i+h2i;
					speq[j1][j2]=h1r-h2r;
					speq[j1][j2+1]=h2i-h1i;
				} else {
					j2=(i2 != 1 ? nn2-i2+2 : 1);
					j3=nn3+3-(i3<<1);
					h1r=c1*(data[i1][i2][ii3]+data[j1][j2][j3]);
					h1i=c1*(data[i1][i2][ii3+1]-data[j1][j2][j3+1]);
					h2i=c2*(data[i1][i2][ii3]-data[j1][j2][j3]);
					h2r= -c2*(data[i1][i2][ii3+1]+data[j1][j2][j3+1]);
					data[i1][i2][ii3]=h1r+wr*h2r-wi*h2i;
					data[i1][i2][ii3+1]=h1i+wr*h2i+wi*h2r;
					data[j1][j2][j3]=h1r-wr*h2r+wi*h2i;
					data[j1][j2][j3+1]= -h1i+wr*h2i+wi*h2r;
				}
			}
			wr=(wtemp=wr)*wpr-wi*wpi+wr;
			wi=wi*wpr+wtemp*wpi+wi;
		}
	}
	if (isign == -1)
		fourn(&data[1][1][1]-1,nn,3,isign);
}


