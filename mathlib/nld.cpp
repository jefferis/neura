#include "nld.h"


NLD:: 
NLD(DataCube* datacube)
{
  dc = datacube;

  tau = 2.0;
  time_steps = 4;
  epsilon = 0.01;
  levels = 1;

  fixed_coeffs = YES;
  anicoeff1 = 1.0;
  anicoeff2 = 1.0;
  anicoeff3 = 0.0;
  dependence_type = PERONA_MALIK;
  lambda = 1;
  integration_size_x = 10;
  integration_size_y = 10;
  integration_size_z = 10;
  gt = CUBE;
  ip_flag = IP_USUAL;
  alpha = 0.5;
}


NLD::
~NLD()
{

}

/*! sets all FilterOptions
 */
void
NLD:: 
SetFilterOptions(FilterOptions fo)
{

  tau = fo.tau;
  time_steps = fo.time_steps;
  epsilon = fo.epsilon;
  levels = fo.levels;

  fixed_coeffs = fo.fixed_coeffs;
  anicoeff1 = fo.anicoeff1;
  anicoeff2 = fo.anicoeff2;
  anicoeff3 = fo.anicoeff3;
  dependence_type = fo.dependence_type; 
  lambda = fo.lambda;
  integration_size_x = fo.integration_size_x;
  integration_size_y = fo.integration_size_y;
  integration_size_z = fo.integration_size_z;
  gt = fo.gt;
  ip_flag = fo.ip_flag;

}

/*! sets the time step
 */
void
NLD::
SetTimeStep(NeuraDataType t)
{
  tau = t;
}

/*! sets the number of time steps
 */
void 
NLD::
SetNumberOfTimeSteps(int n)
{
  time_steps = n;
}

/*! sets the wanted reduction of initial defect
 */
void
NLD::
SetPrecision(NeuraDataType eps)
{
  epsilon = eps;
}

/*! sets the number of multigrid levels
 */
void 
NLD::
SetMultigridLevels(int l)
{
  levels = l;
}

/*! sets the parameter for the dependence function
 */
void 
NLD::
SetLambda(NeuraDataType l)
{
  lambda = l;
}

/*!
 * Sets the linear coefficients of anisotropy
 */
void 
NLD::
SetAniCoefficients(NeuraDataType ani1, NeuraDataType ani2, NeuraDataType ani3)
{
  anicoeff1 = ani1;
  anicoeff2 = ani2;
  anicoeff3 = ani3;
}


/*!
 * Sets the flag for fixed or variable coefficients of anisotropy
 */
void 
NLD::
SetFixedCoefficients(BOOL flag)
{
  fixed_coeffs = flag;
}


/*!
 * Sets the dependence function for coefficients of anisotropy
 */
void 
NLD::
SetDependenceType(DependenceType type)
{
  dependence_type = type;
}

void
NLD::
SetIntegrationSize(int s)
{
  integration_size_x = s;
  integration_size_y = s;
  integration_size_z = s;
}

void 
NLD::
SetIntegrationSizeX(int s)
{
  integration_size_x = s;
}

void 
NLD::
SetIntegrationSizeY(int s)
{
  integration_size_y = s;
}

void 
NLD::
SetIntegrationSizeZ(int s)
{
  integration_size_z = s;
}


void
NLD::
SetGeometryType(GeometryType type)
{
  gt = type;
}


void
NLD::
SetIpFlag(int flag)
{
  ip_flag = flag;
}


/*!
 * decides whether size is a power of two or not
 */
int 
NLD::
pow_of_two(int size)
{
  if ( size % 2 == 1 ) return 0;
  if ( ( size == 2 ) || ( size == 0 ) ) return 1;
  return pow_of_two(size/2);
} 



/*!
 * does the anisotropic filtering
 */
int 
NLD::
execute()
{  

  cout << "Filter options:" << endl;
  cout << "tau = " << tau << endl;
  cout << "time_steps = " << time_steps << endl;
  cout << "precision = " << epsilon << endl;
  cout << "multigrid levels = " << levels << endl;
  cout << "fixed_coeffs = " << fixed_coeffs << endl;
  cout << "anicoeff1 = " << anicoeff1 << endl;
  cout << "anicoeff2 = " << anicoeff2 << endl;
  cout << "anicoeff3 = " << anicoeff3 << endl;
  cout << "dependence_type = " << dependence_type << endl;
  cout << "lambda = " << lambda << endl;
  cout << "integration_size_x = " << integration_size_x << endl;
  cout << "integration_size_y = " << integration_size_y << endl;
  cout << "integration_size_z = " << integration_size_z << endl;
  cout << "geometry_type = " << gt << endl;
  cout << "ip_flag = " << ip_flag << endl;

  // initialise matrix
  Array<int> sizes(3);
  sizes.SetElement(1,dc->GetSize()[1]); 
  sizes.SetElement(2,dc->GetSize()[2]);
  sizes.SetElement(3,dc->GetSize()[3]);

  int code; // errorcode;

  time_t time1, time2, timediff;
  
  for ( int i = 1; i <= time_steps; i++ ){
    
    cout << "Time step " << i << " :" << endl; 
    
    SparseMatrix<NeuraDataType> matA(sizes);
    //delete marker1;
    
    // initialise discretation
    FV_3d27 disc(&matA,ip_flag);
    disc.SetLambda(lambda);
    disc.SetAniCoefficients(anicoeff1, anicoeff2, anicoeff3);
    disc.SetFixedCoefficients(fixed_coeffs);
    disc.SetDependenceType(dependence_type);
    disc.SetIntegrationSizeX(integration_size_x);
    disc.SetIntegrationSizeY(integration_size_y);
    disc.SetIntegrationSizeZ(integration_size_z);
    disc.SetGeometryType(gt);
   
    // initialise right hand side
    MultiVector<NeuraDataType> rhs;
    dc->CopyIntoMultiVector(rhs);
    //delete marker2;
    
    // initialise solution
    MultiVector<NeuraDataType> sol(rhs);
    sol.SetZero();
    
    // initialise multigrid
    Multigrid<NeuraDataType> multigrid;

    // important: precision has to be set before initializing multigrid!
    multigrid.SetPrecision(epsilon);

    // set multigrid properties
    // make sure that precision was setted before!
    multigrid.Initialise(REGULAR, MATRIX_DEPENDENT_INTERPOLATION,
			 MATRIX_DEPENDENT_RESTRICTION, alpha,
			 LEXICOGRAPHIC_GAUSS_SEIDEL, 2, 2, V, BICGSTAB, NODES, levels);

    multigrid.SaveProlongation();
    multigrid.SaveRestriction();
    multigrid.SetMaximumSteps(50);
    
    dc->CopyIntoMultiVector(rhs);
    dc->CopyIntoMultiVector(sol);
    
    cout << "discretize..." << endl;
    time1 = time(NULL);
    code = disc.discretize(*dc);
    if ( code ) return code;
	cout << "NLD: Ich komme hier an!" << endl;
    disc.SetTimeDependent(tau);
    time2 = time(NULL);
    cout << "...discretation finished" << endl;
    timediff = time2-time1;
    cout << "discretation took " << timediff << " seconds" << endl; 
   
    cout << "BuildMatrixHierarchy..." << endl; 
    if ( levels > 1 )
      {
	if ( 
	    ( !pow_of_two(dc->GetSize()[1]-1) ) ||
	    ( !pow_of_two(dc->GetSize()[2]-1) ) ||
	    ( !pow_of_two(dc->GetSize()[3]-1) ) 
	    )
	  {
	    return SIZE_NOT_POW_OF_TWO_PLUS_ONE;
	  }
      }
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
    
  }
  
  cout <<"quit nld now" << endl;
  
  return OK;
  
}



 

  
