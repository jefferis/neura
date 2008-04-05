#include "nld_auto.h"


NLD_Auto::
NLD_Auto(DataCube* datacube)
{
  dc = datacube;
  tau = 0.0;
  epsilon = 0.001;
  levels = 1;
  lambda = 1;
  anicoeff1 = 1;
  anicoeff2 = 0;
  anicoeff3 = 0;
  nonlinear = 0;
  /* FIXME!!! Alexander
  nonlinear_type = PERONA_MALIK;
  */
  kind_of_moments = ELEMENTWISE;
  integration_size = 3;
  small_boundary_pixels = 0;
  ip_flag = 0;
  alpha = 0.5;
}


NLD_Auto::
~NLD_Auto()
{

}

void
NLD_Auto:: 
SetFilterOptions(FilterOptions fo)
{

  tau = fo.tau;
  time_steps = fo.time_steps;
  epsilon = fo.epsilon;
  levels = fo.levels;
  
  /* FIXME!!! Alexander
  nonlinear = fo.nonlinear;
  */
  anicoeff1 = fo.anicoeff1;
  anicoeff2 = fo.anicoeff2;
  anicoeff3 = fo.anicoeff3;
  /* FIXME!!!!! Alexander
  nonlinear_type = fo.nonlinear_type;
  */ 
  lambda = fo.lambda;
  /*FIXME!!! Alexander
  kind_of_moments = fo.kind_of_moments;
  */
  /* FIXME!!! Alexander
  integration_size = fo.integration_size;
  */
  ip_flag = fo.ip_flag;

}

void
NLD_Auto::
SetTimeStep(double t)
{
  tau = t;
}


void 
NLD_Auto::
SetNumberOfTimeSteps(int n)
{
  time_steps = n;
}


void
NLD_Auto::
SetPrecision(double eps)
{
  epsilon = eps;
}


void 
NLD_Auto::
SetMultigridLevels(int l)
{
  levels = l;
}


void 
NLD_Auto::
SetLambda(double l)
{
  lambda = l;
}

/*!
 * Sets the linear coefficients of anisotropy
 */
void 
NLD_Auto::
SetAniCoefficients(double ani1, double ani2, double ani3)
{
  anicoeff1 = ani1;
  anicoeff2 = ani2;
  anicoeff3 = ani3;
}


/*!
 * Sets the flag for nonlinearity
 */
void 
NLD_Auto::
SetNonlinear(int non)
{
  nonlinear = non;
}


/*!
 * Sets the nonlinear function
 */
 
/* FIXME!!! Alexander
void 
NLD_Auto::
SetNonlinearType(NonlinearType type)
{
  nonlinear_type = type;
}
*/

/*!
 * Sets the kind of moment calculation within integration points
 */
void
NLD_Auto::
SetKindOfMoments(KindOfMoments kind)
{
  kind_of_moments = kind;
}


void 
NLD_Auto::
SetIntegrationSize(int s)
{
  integration_size = s;
}

/*!
 * Sets the flag for amount of boundary pixels
 */
void 
NLD_Auto::
SetSmallBoundaryPixels(int flag)
{
  small_boundary_pixels = flag;
}


void
NLD_Auto::
SetIpFlag(int flag)
{
  ip_flag = flag;
}


/*!
 * does the anisotropic filtering
 * after each time step slices can be written
 * into file with modified name filename
 */
int
NLD_Auto::
execute(int number)
{

  cout << "Filter options:" << endl;
  cout << "tau = " << tau << endl;
  cout << "time_steps = " << time_steps << endl;
  cout << "precision = " << epsilon << endl;
  cout << "multigrid levels = " << levels << endl;
  cout << "nonlinear = " << nonlinear << endl;
  cout << "anicoeff1 = " << anicoeff1 << endl;
  cout << "anicoeff2 = " << anicoeff2 << endl;
  cout << "anicoeff3 = " << anicoeff3 << endl;
  /*FIXME!!! Alexander
  cout << "nonlinear_type = " << nonlinear_type << endl;
  */
  cout << "lambda = " << lambda << endl;
  cout << "KindOfMoments = " << kind_of_moments << endl;
  cout << "integration_size = " << integration_size << endl;
  cout << "small_boundary_pixels = " << small_boundary_pixels << endl;
  cout << "ip_flag = " << ip_flag << endl;


  // initialise matrix
  Array<int> sizes(3);
  sizes.SetElement(1,dc->GetSize()[1]);
  sizes.SetElement(2,dc->GetSize()[2]);
  sizes.SetElement(3,dc->GetSize()[3]);

 int code; // errorcode;


  time_t time1, time2, timediff;

  char filename[60]; // to store slices

   for ( int i = 1; i <= time_steps; i++ ){

     cout << "Time step " << i << " :" << endl;
     
   //   Moments mom(dc);
     
//      for ( int jj = 0; jj < 9; jj++ )
//        for ( int ii = 0; ii < 9; ii++ )
//        {
// 	 int intsize = 3;
// 	 //define volume
// 	 Volume vol(1, ii, jj, 4, intsize,intsize,intsize, 1.0, 1.0, 1.0 );
// 	 cout << "Integration auf Kugel" << endl;
// 	 cout << "Radius = " << intsize << endl;
     
// 	 // calculate inertian values
     
// 	 cout << "calculate inertian values" << endl;
     
// 	 Vector<double> ew(3);
// 	 Vector<double> ev(9);
     
// 	 int code = mom.inertian_values(ew,ev,vol);
// 	 if ( code ) cout << errorCode(code);
     
// 	 cout << "Traegheitswerte am Punkt ( " << ii << ", " << jj << ", 4 ): " << endl;
// 	 cout << "DataCube-value: " << (*dc)(ii,jj,4) << endl;
// 	 cout << "eigenvektor: ( " << ev[1] << " " << ev[2] << " " << ev[3] << " )" << endl;
// 	 cout << "dazu eigenwert: " << ew[1] << endl;
// 	 cout << "eigenvektor: ( " << ev[4] << " " << ev[5] << " " << ev[6] << " )" << endl;
// 	 cout << "dazu eigenwert: " << ew[2] << endl;
// 	 cout << "eigenvektor: ( " << ev[7] << " " << ev[8] << " " << ev[9] << " )" << endl;
// 	 cout << "dazu eigenwert: " << ew[3] << endl;
     
//        }
     
     
     SparseMatrix<double> matA(sizes);
     //matA=new SparseMatrix<double>(sizes);
     
     // initialise discretation
     FV_3d27 disc(&matA,ip_flag);
     //disc.SetLambda(lambda);
     //disc.SetAniCoefficients(anicoeff1, anicoeff2, anicoeff3);
     //disc.SetNonlinear(nonlinear);
     //disc.SetNonlinearType(nonlinear_type);
     //disc.SetIntegrationSize(integration_size);
     //disc.SetKindOfMoments(kind_of_moments);
     //disc.SetSmallBoundaryPixels(small_boundary_pixels);
     
// initialise right hand side
     MultiVector<double> rhs;
     dc->CopyIntoMultiVector(rhs);
     
     // initialise solution
     MultiVector<double> sol(rhs);
     sol.SetZero();
     
     // initialise multigrid
     Multigrid<double> multigrid;
     
     // set multigrid properties
     multigrid.Initialise(REGULAR, BILINEAR_INTERPOLATION,
			  BILINEAR_RESTRICTION, alpha,
			  POINT_ILU, 2, 2, V, BICGSTAB, NODES, levels);
     
     multigrid.SaveProlongation();
     multigrid.SaveRestriction();
     multigrid.SetPrecision(epsilon);
     multigrid.SetMaximumSteps(10);
     
     dc->CopyIntoMultiVector(rhs);
     sol.SetZero();
     
     cout << "discretize..." << endl;
     time1 = time(NULL);
     code = disc.discretize(*dc);
     if ( code ) return code;
     disc.SetTimeDependent(tau);
     time2 = time(NULL);
     cout << "...discretation finished" << endl;
     timediff = time2-time1;
     cout << "discretation took " << timediff << " seconds" << endl; 
     
     cout << "BuildMatrixHierarchy..." << endl; 
     time1 = time(NULL);
     multigrid.BuildMatrixHierarchy(matA); 
     time2 = time(NULL);
     cout << "...done" << endl; 
     timediff = time2-time1; 
     cout << "building matrix hierarchy took " << timediff << " seconds" << endl; 
     
     cout << "solve by multigrid method..." << endl; 
     time1 = time(NULL);
     multigrid.Solve(rhs,sol);
     time2 = time(NULL);
     cout << "......done" << endl;
     timediff = time2-time1; 
     cout << "soluting took " << timediff << " seconds" << endl; 
     
     dc->CopyFromMultiVector(sol);
     
     //print datacube
     cout << "Diagonal Luecke: i:[21...27], j:[21...27]" << endl;
     cout << "k:15" << endl;
     for ( int j = 21; j <= 27; j++ ){
       for ( int i = 21; i <= 27; i++ ){
	 cout << (*dc)(i,j,15) << " ";
       }
       cout << endl;
     }
     cout << endl;

     cout << "k:16" << endl;
     for ( int j = 21; j <= 27; j++ ){
       for ( int i = 21; i <= 27; i++ ){
	 cout << (*dc)(i,j,16) << " ";
       }
       cout << endl;
     }
     cout << endl;

     cout << "k:17" << endl;
     for ( int j = 21; j <= 27; j++ ){
       for ( int i = 21; i <= 27; i++ ){
	 cout << (*dc)(i,j,17) << " ";
       }
       cout << endl;
     }
     cout << endl;
     
     cout << "Mittel Luecke: i:[13...19], j:[13...19]" << endl;
     cout << "k:15" << endl;
     for ( int j = 13; j <= 19; j++ ){
       for ( int i = 13; i <= 19; i++ ){
	 cout << (*dc)(i,j,15) << " ";
       }
       cout << endl;
     }
     cout << endl;

     cout << "k:16" << endl;
     for ( int j = 13; j <= 19; j++ ){
       for ( int i = 13; i <= 19; i++ ){
	 cout << (*dc)(i,j,16) << " ";
       }
       cout << endl;
     }
     cout << endl;

     cout << "k:17" << endl;
     for ( int j = 13; j <= 19; j++ ){
       for ( int i = 13; i <= 19; i++ ){
	 cout << (*dc)(i,j,17) << " ";
       }
       cout << endl;
     }
     cout << endl;

     cout << "Gerade Luecke: i:[14...18], j:[6...10]" << endl;
     cout << "k:15" << endl;
     for ( int j = 6; j <= 10; j++ ){
       for ( int i = 14; i <= 18; i++ ){
	 cout << (*dc)(i,j,15) << " ";
       }
       cout << endl;
     }
     cout << endl;

     cout << "k:16" << endl;
     for ( int j = 6; j <= 10; j++ ){
       for ( int i = 14; i <= 18; i++ ){
	 cout << (*dc)(i,j,16) << " ";
       }
       cout << endl;
     }
     cout << endl;

     cout << "k:17" << endl;
     for ( int j = 6; j <= 10; j++ ){
       for ( int i = 14; i <= 18; i++ ){
	 cout << (*dc)(i,j,17) << " ";
       }
       cout << endl;
     }
     cout << endl;

     sprintf(filename,"testseries/Y3/slice%d_timestep%d_k15",number,i);
     dc->writeSlicePPM(filename,15,Z_DIR);

     sprintf(filename,"testseries/Y3/slice%d_timestep%d_k16",number,i);
     dc->writeSlicePPM(filename,16,Z_DIR);

     sprintf(filename,"testseries/Y3/slice%d_timestep%d_k17",number,i);
     dc->writeSlicePPM(filename,17,Z_DIR);

     // luecken gefuellt? Ueberpruefe Lueckenmittelpunkte
     if ( 
	 ( (*dc)(16,8,16) > 0.3 ) && 
	 ( (*dc)(16,16,16) > 0.3 ) &&
	 ( (*dc)(24,24,16) > 0.3 ) 
  )
       break;
   }
   
   cout << "quit nld now" << endl;
   
   return OK;
   
}

 



 

  
