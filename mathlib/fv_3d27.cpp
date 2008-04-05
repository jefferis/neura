#include "fv_3d27.h"

FV_3d27::
FV_3d27(SparseMatrix<NeuraDataType> *matrix, int ip_flag)
{
  a = matrix;
  // fill integration points
  fill_integration_points(ip_flag);

  // default parameter
  anicoeff1 = 1;
  anicoeff2 = 1;
  anicoeff3 = 0;
  fixed_coeffs = YES;
  dependence_type = PERONA_MALIK; 
  lambda = 0.1;
  gt = CUBE;
  integration_size_x =  2;
  integration_size_y =  2;
  integration_size_z =  2;
  gt = CUBE;
}


FV_3d27::
~FV_3d27()
{

}


/*!
 * Sets the parameter for nonlinearity
 */
void
FV_3d27::
SetLambda(NeuraDataType l)
{
  lambda = l;
}


/*!
 * Sets the linear coefficients of anisotropy
 */
void 
FV_3d27::
SetAniCoefficients(NeuraDataType c1, NeuraDataType c2, NeuraDataType c3)
{
  anicoeff1 = c1;
  anicoeff2 = c2;
  anicoeff3 = c3;

}


/*!
 * the Perona-Malik diffusivity
 */
NeuraDataType
FV_3d27::
g_pm(NeuraDataType s)
{
  return 1/(1+s*s/(lambda*lambda));
}


/*!
 * the Weikert diffusivity
 */
NeuraDataType 
FV_3d27::
g_w(NeuraDataType s)
{
  if ( s <= lambda ) return 1;
  return 1 - exp(-2.33667/(pow(s/lambda,4)));
}

/*!
 * the Black-Sapiro diffusivity
 */
NeuraDataType 
FV_3d27::
g_bs(NeuraDataType s)
{
  if ( s <= lambda*2.236068 )
    return 0.5*(1-(s/(lambda*2.236068))*(s/(lambda*2.236068)))*
      (1-(s/(lambda*2.236068))*(s/(lambda*2.236068)));
  return 0;
}


/*!
 * Sets the flag for nonlinearity
 */
void 
FV_3d27::
SetFixedCoefficients(BOOL flag)
{
  fixed_coeffs = flag;
}


/*!
 * Sets the nonlinear function
 */
void 
FV_3d27::
SetDependenceType(DependenceType type)
{
  dependence_type = type;
}


/*!
 * Sets the integration size in x-direction for moment calculation 
 */
void
FV_3d27::
SetIntegrationSizeX(int s)
{
  integration_size_x = s;
}

/*!
 * Sets the integration size in y-direction for moment calculation 
 */
void
FV_3d27::
SetIntegrationSizeY(int s)
{
  integration_size_y = s;
}

/*!
 * Sets the integration size in z-direction for moment calculation 
 */
void
FV_3d27::
SetIntegrationSizeZ(int s)
{
  integration_size_z = s;
}


 
/*!
 * Sets the geometry type of integration domain
 */
void 
FV_3d27::
SetGeometryType(GeometryType type)
{
  gt = type;
}



/*!
 * fills the arrays of integration points and the correspondending normals
 */
void
FV_3d27::
fill_integration_points(int ip_flag)
{ 
  switch (ip_flag)
    {
    case IP_USUAL:
      ip[0][0][0] = 0.5;   ip[0][0][1] = 0.25;  ip[0][0][2] = 0.25;
      ip[0][1][0] = 0.25;  ip[0][1][1] = 0.25;  ip[0][1][2] = 0.5;
      
      ip[1][0][0] = 0.75;  ip[1][0][1] = 0.5;   ip[1][0][2] = 0.25;
      ip[1][1][0] = 0.75;  ip[1][1][1] = 0.25;  ip[1][1][2] = 0.5;
      
      ip[2][0][0] = 0.25;  ip[2][0][1] = 0.5;   ip[2][0][2] = 0.25;
      ip[2][1][0] = 0.25;  ip[2][1][1] = 0.75;  ip[2][1][2] = 0.5;
      
      ip[3][0][0] = 0.5;   ip[3][0][1] = 0.75;  ip[3][0][2] = 0.25;
      ip[3][1][0] = 0.75;  ip[3][1][1] = 0.75;  ip[3][1][2] = 0.5;
      
      ip[4][0][0] = 0.5; ip[4][0][1] = 0.25; ip[4][0][2] = 0.75;
      ip[4][1][0] = -1; ip[4][1][1] = -1;  ip[4][1][2] = -1;
      
      ip[5][0][0] = 0.75; ip[5][0][1] = 0.5; ip[5][0][2] = 0.75;
      ip[5][1][0] = -1; ip[5][1][1] = -1;  ip[5][1][2] = -1;
      
      ip[6][0][0] = 0.25; ip[6][0][1] = 0.5; ip[6][0][2] = 0.75;
      ip[6][1][0] = -1; ip[6][1][1] = -1;  ip[6][1][2] = -1;
      
      ip[7][0][0] = 0.5; ip[7][0][1] = 0.75; ip[7][0][2] = 0.75;
      ip[7][1][0] = -1; ip[7][1][1] = -1;  ip[7][1][2] = -1;

      break;

    case IP_BND:
      ip[0][0][0] = 0.5;   ip[0][0][1] = 0.0;  ip[0][0][2] = 0.0;
      ip[0][1][0] = 0.0;  ip[0][1][1] = 0.0;  ip[0][1][2] = 0.5;
      
      ip[1][0][0] = 1.0;  ip[1][0][1] = 0.5;   ip[1][0][2] = 0.0;
      ip[1][1][0] = 1.0;  ip[1][1][1] = 0.0;  ip[1][1][2] = 0.5;
      
      ip[2][0][0] = 0.0;  ip[2][0][1] = 0.5;   ip[2][0][2] = 0.0;
      ip[2][1][0] = 0.0;  ip[2][1][1] = 1.0;  ip[2][1][2] = 0.5;
      
      ip[3][0][0] = 0.5;   ip[3][0][1] = 1.0;  ip[3][0][2] = 0.0;
      ip[3][1][0] = 1.0;  ip[3][1][1] = 1.0;  ip[3][1][2] = 0.5;
      
      ip[4][0][0] = 0.5; ip[4][0][1] = 0.0; ip[4][0][2] = 1.0;
      ip[4][1][0] = -1; ip[4][1][1] = -1;  ip[4][1][2] = -1;
      
      ip[5][0][0] = 1.0; ip[5][0][1] = 0.5; ip[5][0][2] = 1.0;
      ip[5][1][0] = -1; ip[5][1][1] = -1;  ip[5][1][2] = -1;
      
      ip[6][0][0] = 0.0; ip[6][0][1] = 0.5; ip[6][0][2] = 1.0;
      ip[6][1][0] = -1; ip[6][1][1] = -1;  ip[6][1][2] = -1;
      
      ip[7][0][0] = 0.5; ip[7][0][1] = 1.0; ip[7][0][2] = 1.0;
      ip[7][1][0] = -1; ip[7][1][1] = -1;  ip[7][1][2] = -1;
      break;
    default: 
      cout << "FV_3d27::Unknown kind of integration point flag specified!\n" << endl; 
    }

  nip[0][0][0] = 1.0;  nip[0][0][1] = 0.0;  nip[0][0][2] = 0.0;
  nip[0][1][0] = 0.0;  nip[0][1][1] = 0.0;  nip[0][1][2] = 1.0;

  nip[1][0][0] = 0.0;  nip[1][0][1] = 1.0;  nip[1][0][2] = 0.0;
  nip[1][1][0] = 0.0;  nip[1][1][1] = 0.0;  nip[1][1][2] = 1.0;

  nip[2][0][0] = 0.0;  nip[2][0][1] = -1.0; nip[2][0][2] = 0.0;
  nip[2][1][0] = 0.0;  nip[2][1][1] = 0.0;  nip[2][1][2] = 1.0;

  nip[3][0][0] = -1.0; nip[3][0][1] = 0.0;  nip[3][0][2] = 0.0;
  nip[3][1][0] = 0.0;  nip[3][1][1] = 0.0;  nip[3][1][2] = 1.0;

  nip[4][0][0] = 1.0; nip[4][0][1] = 0.0; nip[4][0][2] = 0.0;
  nip[4][1][0] = 0; nip[4][1][1] = 0;  nip[4][1][2] = 0;

  nip[5][0][0] = 0.0; nip[5][0][1] = 1.0; nip[5][0][2] = 0.0;
  nip[5][1][0] = 0; nip[5][1][1] = 0;  nip[5][1][2] = 0;

  nip[6][0][0] = 0.0; nip[6][0][1] = -1.0; nip[6][0][2] = 0.0;
  nip[6][1][0] = 0; nip[6][1][1] = 0;  nip[6][1][2] = 0;

  nip[7][0][0] = -1.0; nip[7][0][1] = 0.0; nip[7][0][2] = 0.0;
  nip[7][1][0] = 0; nip[7][1][1] = 0;  nip[7][1][2] = 0;
  
  intact[0][0] = 1; 
  intact[0][1] = 4; 
  
  intact[1][0] = 3; 
  intact[1][1] = 5; 
  
  intact[2][0] = 0; 
  intact[2][1] = 6; 
  
  intact[3][0] = 2; 
  intact[3][1] = 7; 
   
  intact[4][0] = 5; 
  intact[4][1] = -1; 
 
  intact[5][0] = 7; 
  intact[5][1] = -1; 
 
  intact[6][0] = 4; 
  intact[6][1] = -1; 
  
  intact[7][0] = 6; 
  intact[7][1] = -1; 

}

/*!
 * calculates for a given element number e 
 * the respectively global numbers to the nodes
 */ 
void 
FV_3d27::
elements_nodes(int e, int n, int m, int en[8])
{

  int help = e + ((e/(n-1))%(m-1)) + (n+m-1)*(e/((n-1)*(m-1)));

  // done because enumeration of elements and nodes begins with 0,
  // but enumeration of rows in the matrix with 1
  help += 1;

  en[0] = help        ;
  en[1] = help+1      ; 
  en[2] = help+n      ; 
  en[3] = help+n+1    ; 
  en[4] = help+n*m    ;
  en[5] = help+n*m+1  ; 
  en[6] = help+n*m+n  ;
  en[7] = help+n*m+n+1; 

}


NeuraDataType 
FV_3d27::
phi(int i, NeuraDataType xi, NeuraDataType eta, NeuraDataType theta )
{
  switch ( i )
    {
    case 0: return (1-xi)*(1-eta)*(1-theta);
    case 1: return xi*(1-eta)*(1-theta);
    case 2: return (1-xi)*eta*(1-theta);
    case 3: return xi*eta*(1-theta);
    case 4: return (1-xi)*(1-eta)*theta;
    case 5: return xi*(1-eta)*theta;
    case 6: return (1-xi)*eta*theta;
    case 7: return xi*eta*theta;
    default: 
      cout << "Tried to use ansatzfuntion " << i << " where only 0 ... 7 are possible!" << endl;
      exit(1);
    }
}


NeuraDataType 
FV_3d27::
grad_phi_x(int j, NeuraDataType xi, NeuraDataType eta, NeuraDataType theta )
{
  switch ( j )
    {
    case 0: return (eta-1)*(1-theta);
    case 1: return (1-eta)*(1-theta);
    case 2: return eta*(theta-1);
    case 3: return eta*(1-theta);
    case 4: return (eta-1)*theta;
    case 5: return (1-eta)*theta;
    case 6: return -eta*theta;
    case 7: return eta*theta;
    default: 
      cout << "Tried to use ansatzfuntion " << j << " where only 0 ... 7 are possible!" << endl;
      exit(1);
    }
}


NeuraDataType 
FV_3d27::
grad_phi_y(int j, NeuraDataType xi, NeuraDataType eta, NeuraDataType theta )
{
 switch ( j )
    {
    case 0: return (xi-1)*(1-theta); 
    case 1: return xi*(theta-1);
    case 2: return (1-xi)*(1-theta);
    case 3: return xi*(1-theta);
    case 4: return (xi-1)*theta;
    case 5: return -xi*theta;
    case 6: return (1-xi)*theta;
    case 7: return xi*theta;
    default: 
      cout << "Tried to use ansatzfuntion " << j << " where only 0 ... 7 are possible!" << endl;
      exit(1);
    }
}


NeuraDataType
FV_3d27::
grad_phi_z(int j, NeuraDataType xi, NeuraDataType eta, NeuraDataType theta )
{
 switch ( j )
    {
    case 0: return (xi-1)*(1-eta);
    case 1: return xi*(eta-1);
    case 2: return (xi-1)*eta;
    case 3: return -xi*eta;
    case 4: return (1-xi)*(1-eta);
    case 5: return xi*(1-eta);
    case 6: return (1-xi)*eta;
    case 7: return xi*eta;
    default: 
      cout << "Tried to use ansatzfuntion " << j << " where only 0 ... 7 are possible!" << endl;
      exit(1);
    }
}




/*!
 * discretizes 3d nonlinear anisotropic diffusion 
 * by finite volume method using a 27-point stencil
 */
int 
FV_3d27::
discretize(DataCube &dc)
{ 
 
  int code; // errorcode
  int e,i,j,k,ewc,evc; // loop variables
  NeuraDataType insert_diff; // help variable
  NeuraDataType diff_x, diff_y, diff_z; // help variables

  int n = a->Length(1);//=dc.GetSize()[1];
  int m = a->Length(2);//=dc.GetSize()[2];
  int l = a->Length(3);//=dc.GetSize()[3];
 
  int number_of_elements = (n-1)*(m-1)*(l-1);
  int number_of_nodes = n*m*l;

  // eigenvalues and eigenvectors of inertian tensor 
  // within current integration point
  Vector<NeuraDataType> ew(3);
  Vector<NeuraDataType> ev(9); 

  Vector<NeuraDataType> tmp(3); // help vector
  Vector<NeuraDataType> t(3); // coefficients of anisotropy

  NeuraDataType cl, cp, ci; // characteristic structure coefficients
  NeuraDataType sum_ew; // sum over eigenvalues of inertian tensor
  NeuraDataType swap; // help variable 

  a->SetZero();

  // prepare moment calculation

  // for to use fastest method
  if ( gt == CUBE ){
    int warn = 0;
    if ( integration_size_x % 2 == 1 ){ integration_size_x ++; warn = 1;}
    if ( integration_size_y % 2 == 1 ){ integration_size_y ++; warn = 1;}
    if ( integration_size_z % 2 == 1 ){ integration_size_z ++; warn = 1;}
    if ( warn ) cout << "Warning: You should use even integration sizes in all directions at CUBE integration! Given values were adapted." << endl;
  }

  Volume vol(gt, 3, integration_size_x, integration_size_y, integration_size_z );

  Moments mom(&dc);
  Vector<NeuraDataType> *vals; 
  Vector<NeuraDataType> *vects;
  
  time_t time1, time2, timediff;
  // calculate moments
  cout << "calculate moments..." << endl;
  time1 = time(NULL);

  if ( 
      ( integration_size_x > n ) || 
      ( integration_size_y > m ) || 
      ( integration_size_z > l ) 
      ) 
    {
      return INTEGRATION_DOMAIN_TOO_BIG;
    } 
  
  switch ( gt )
    {
    case CUBE: // fast moments calculation and elementwise kindOfMoments
      kind_of_moments = ELEMENTWISE;
      vals = new ( Vector<NeuraDataType>[number_of_elements])(3);
      vects = new (Vector<NeuraDataType>[number_of_elements])(9);
      code = mom.elemwise_fast2_all_inertian_values(vals,vects,vol);
      if ( code ) return code;
      break;
    case BALL: // fourier moments calculation and nodewise kindOfMoments
      kind_of_moments = NODEWISE;
      vals = new ( Vector<NeuraDataType>[number_of_nodes])(3);
      vects = new (Vector<NeuraDataType>[number_of_nodes])(9);
      code = mom.fourier_all_inertian_values(vals,vects,vol,n,m,l);
      code = mom.all_inertian_values(vals,vects,vol);
      if ( code ) return code;
      break;
    default:
      return UNKNOWN_GEOMETRY_TYPE;
    }
  
  if ( code ) return code;
  time2 = time(NULL);
  cout << "...moments calculated" << endl;
  timediff = time2-time1;
  cout << "calculation of moments took " << timediff << " seconds" << endl;
 
  // discretise
  for ( e = 0; e < number_of_elements; e++ ) // elements 
    {  
      elements_nodes(e,n,m,en);
      
      for ( i = 0; i < 8; i++ ) // nodes
	{   
	  // determine moments
	  switch ( kind_of_moments )
	    {
	    case ELEMENTWISE:
	      // eigenvalues
	      for ( ewc = 1; ewc <= 3; ewc++ )
		ew[ewc] = vals[e][ewc];
	      // eigenvectors
	      for ( evc = 1; evc <= 9; evc++ )
		ev[evc] = vects[e][evc];
	      break;
	    case NODEWISE:
	      // eigenvalues
	      for ( ewc = 1; ewc <= 3; ewc++ )
		ew[ewc] = vals[en[i]-1][ewc];
	      // eigenvectors
	      for ( evc = 1; evc <= 9; evc++ )
		ev[evc] = vects[en[i]-1][evc];
	      break;
	    default: 
	      cout << "FV_3d27::RunTimeError: Unknown kindOfMoments! \n Abort " << endl;
	      exit(1);
	    }
	  
	  k = 0;
	  while ( ( ip[i][k][0] != -1 ) && ( k < 2 ) ) // ips
	    {   
	      for ( j = 0; j < 8; j++ ) // ansatzfunctions
		{  
		  diff_x = grad_phi_x(j,ip[i][k][0],ip[i][k][1],ip[i][k][2]);
		  diff_y = grad_phi_y(j,ip[i][k][0],ip[i][k][1],ip[i][k][2]);
		  diff_z = grad_phi_z(j,ip[i][k][0],ip[i][k][1],ip[i][k][2]);
		  
		  // sort eigenvalues
		  if ( ew[1] < ew[2] ){ 
		    swap = ew[1]; ew[1] = ew[2]; ew[2] = swap;
		    swap = ev[1]; ev[1] = ev[4]; ev[4] = swap;
		    swap = ev[2]; ev[2] = ev[5]; ev[5] = swap;
		    swap = ev[3]; ev[3] = ev[6]; ev[6] = swap;
		    
		  }
		  if ( ew[1] < ew[3] ){ 
		    swap = ew[1]; ew[1] = ew[3]; ew[3] = swap;
		    swap = ev[1]; ev[1] = ev[7]; ev[7] = swap;
		    swap = ev[2]; ev[2] = ev[8]; ev[8] = swap;
		    swap = ev[3]; ev[3] = ev[9]; ev[9] = swap;
		  }
		  if ( ew[2] < ew[3] ){ 
		    swap = ew[2]; ew[2] = ew[3]; ew[3] = swap;
		    swap = ev[4]; ev[4] = ev[7]; ev[7] = swap;
		    swap = ev[5]; ev[5] = ev[8]; ev[8] = swap;
		    swap = ev[6]; ev[6] = ev[9]; ev[9] = swap;
		  }
		  
		  
		  // determine coefficients of anisotropy
		 	  
		  if ( fixed_coeffs )
		    {
		      t[1] = anicoeff1;
		      t[2] = anicoeff2;
		      t[3] = anicoeff3;
		    }
		  else
		    {
		      sum_ew = ew[1]+ew[2]+ew[3];
		      if ( fabs(sum_ew) <= 0.000001 )
			{ cl = 0; cp = 0; ci = 1; }
		      else {
			cl = (ew[1]-ew[2])/sum_ew;
			cp = 2*(ew[2]-ew[3])/sum_ew;
			ci = 3*ew[3]/sum_ew;
		      }
		      
		      t[1] = 1.0;
		      switch (dependence_type)
			{
			case PERONA_MALIK:
			  t[2] = g_pm(cl);
			  t[3] = g_pm(1-ci);
			  break;
			case WEIKERT:
			  t[2] = g_w(cl);
			  t[3] = g_w(1-ci);
			  break;
			case BLACK_SAPIRO: 
			  t[2] = g_bs(cl);
			  t[3] = g_bs(1-ci);
			  break;
			default:
			  cout << "Unknown nonlinear function specified!\n Abort" << endl;
			  exit(1);	
			}
		    }

		  // multiplication with the anisotropy tensor

		  tmp[1] = ev[1]*diff_x + ev[2]*diff_y + ev[3]*diff_z;
		  tmp[2] = ev[4]*diff_x + ev[5]*diff_y + ev[6]*diff_z;
		  tmp[3] = ev[7]*diff_x + ev[8]*diff_y + ev[9]*diff_z;

		  tmp[1] *= t[1]; tmp[2] *= t[2]; tmp[3] *= t[3];

		  diff_x = ev[1]*tmp[1] + ev[4]*tmp[2] + ev[7]*tmp[3];
		  diff_y = ev[2]*tmp[1] + ev[5]*tmp[2] + ev[8]*tmp[3];
		  diff_z = ev[3]*tmp[1] + ev[6]*tmp[2] + ev[9]*tmp[3];


		  // multiplication with normal
		  insert_diff = diff_x * nip[i][k][0] +
		    diff_y * nip[i][k][1] +
		    diff_z * nip[i][k][2];
 
		
		  insert_diff *= 0.25; // surface of subcontrolvolume
		 
		      a->Add(en[i], en[j], insert_diff);
		    
		      a->Add( en[intact[i][k]], en[j], -insert_diff); 
		     	  
		}//for j
	    
	      k++;
       
	    }// while
	 
	}// for i
     	  	      
    }// for e

  delete [] vals;
  delete [] vects;


  // treatment of boundary

  int position; 
  
  // lower and upper boundary
  for ( i = 0; i < n; i++ )
    {
      for ( j = 0; j < m; j++ )
	{
	  k = 0;
	  position = 1 + i + j*n + k*n*m;
	  a->MultiplyRow(position,2);
	  k = l-1;
	  position = 1 + i + j*n + k*n*m;
	  a->MultiplyRow(position,2);
	} 
    }
  //left and right boundary
  for ( j = 0; j < m; j++ )
    {
      for ( k = 0; k < l; k++ )
	{ 
	  i = 0;
	  position = 1 + i + j*n + k*n*m;
	  a->MultiplyRow(position,2);
	  i = n-1;
	  position = 1 + i + j*n + k*n*m;
	  a->MultiplyRow(position,2);
	} 
    }
  
  // front and back boundary
  for ( i = 0; i < n; i++ )
    {
      for ( k = 0; k < l; k++ )
	{
	  j = 0;
	  position = 1 + i + j*n + k*n*m;
	  a->MultiplyRow(position,2);
	  j = m-1;
	  position = 1 + i + j*n + k*n*m;
	  a->MultiplyRow(position,2);
	}
    }


return OK;
}



/*!
  * sets discretize matrix A to I-tau*A/Vi
  */
int
FV_3d27::
SetTimeDependent(NeuraDataType tau)
{ 
 
  a->Multiply(-tau);
  a->AddIdentity(); 
 
 return OK;

}




/*!
  * sets dirichlet values at the boundary
  */
int
FV_3d27::
SetDirichlet(NeuraDataType val, MultiVector<NeuraDataType>& b)
{
  int i,j,k;
  int position; 
  Array<int> place(3);
  int n = a->Length(1);
  int m = a->Length(2);
  int l = a->Length(3);

  // upper boundary
  place[3] = 1;
  for ( i = 0; i < n; i++ )
    {
      place[1] = i+1; 

      for ( j = 0; j < m; j++ )
	{
	  place[2] = j+1;
	  k = 0;
	  position = 1 + i + j*n + k*n*m;
	  a->MultiplyRow( position, 0 );
	  a->Add(position,position,1);
	  b[place] = val;
	} 
    }
  // lower boundary  
  place[3] = l;
  for ( i = 0; i < n; i++ )
    { 
      place[1] = i+1;
      
      for ( j = 0; j < m; j++ )
	{   
	  place [2] = j+1;
	  k = l-1;
	  position = 1 + i + j*n + k*n*m;
	  a->MultiplyRow( position, 0 );
	  a->Add(position,position,1);
	  b[place] = val;
	} 
    }
  //right boundary
  place[1] = 1;
  for ( j = 0; j < m; j++ )
    { 
      place [2] = j+1;
      
      for ( k = 1; k < l-1; k++ )
	{
	  place[3] = k+1;
	  i = 0;
	  position = 1 + i + j*n + k*n*m;
	  a->MultiplyRow( position, 0 );
	  a->Add(position,position,1);
	  b[place] = val;
	} 
    }
  //left boundary
  place[1] = n;
  for ( j = 0; j < m; j++ )
    {
      place[2] = j+1;
      for ( k = 1; k < l-1; k++ )
       {
	 place[3] = k+1;
	 i = n-1;
	 position = 1 + i + j*n + k*n*m;
	 a->MultiplyRow( position, 0 );
	 a->Add(position,position,1);
	 b[place] = val;
       } 
    }
 // back boundary
  place[2] = 1;
  for ( i = 1; i < n-1; i++ )
    {
      place[1] = i+1;
      for ( k = 1; k < l-1; k++ )
	{
	  place[3] = k+1;
	  j = 0;
	  position = 1 + i + j*n + k*n*m;
	  a->MultiplyRow( position, 0 );
	  a->Add(position,position,1);
	  b[place] = val;
	}
    }
  // front boundary
  place[2] = m;
  for ( i = 1; i < n-1; i++ )
   {
     place[1] = i+1;
     for ( k = 1; k < l-1; k++ )
       {
	 place[3] = k+1;
	 j = m-1;
	 position = 1 + i + j*n + k*n*m;
	 a->MultiplyRow( position, 0 );
	 a->Add(position,position,1);
	 b[place] = val;
       }
   }
 return OK;
}


