/****************************************************************************/
/*                                                                          */
/* File:      fd_3d_lin_iso.h                                               */
/*                                                                          */
/* Purpose:   routines to discretisize linear isotropic diffusion           */
/*            by finite differences in 3d                                   */
/*                                                                          */
/* Author:    Roland Schulte                                                */
/*            Interdisziplinaeres Zentrum fuer Wissenschaftliches Rechnen   */
/*            Universitaet Heidelberg                                       */
/*            Im Neuenheimer Feld 368                                       */
/*            D-69120 Heidelberg                                            */
/*                                                                          */
/* EMail:     roland.schulte@iwr.uni-heidelberg.de                          */
/*                                                                          */
/* History:   11.12.02 begin                                                */
/*                                                                          */
/* Remarks:                                                                 */
/*                                                                          */
/****************************************************************************/
#ifndef FD_3D_LIN_ISO
#define FD_3D_LIN_ISO

#include "../datalib/stencil.h"
#include "../datalib/stencil3d7.h"
#include "../datalib/algequation.h"
#include "../datalib/sparsematrix.h"

class FD_3d_lin_iso
{
  
 public:
  
  FD_3d_lin_iso(SparseMatrix<Stencil3d7> *matrix, DataCube *rhs);
  ~FD_3d_lin_iso();
  
  int discretisize(DataCube &dc);

  int dirichlet(DataCube &dc, int val);

  int neumann(DataCube &dc, int val);

 private:
  
  SparseMatrix<Stencil3d7> *a; // pointer to matrix
  DataCube *b; // pointer to rhs

};

FD_3d_lin_iso::
FD_3d_lin_iso(SparseMatrix<Stencil3d7> *matrix, DataCube *rhs)
{
  a = matrix;
  b = rhs;
}


FD_3d_lin_iso::
~FD_3d_lin_iso()
{

}



// discretisizes 3d linear isotropic diffusion 
// by finite difference scheme
int 
FD_3d_lin_iso::
discretisize(DataCube &dc)
{
  // treatment of inner points
  int i,j,k; 
  int n = dc.GetSize()[1];
  int m = dc.GetSize()[2];
  int l = dc.GetSize()[3];
  Stencil3d7 *tmp;
  for ( i = 1; i < n-1; i++ )
    {
      for ( j = 1; j < m-1; j++ )
	{
	  for ( k = 1; k < l-1; k++)
	    {
	      tmp = a->GetStencilOnGrid(i,j,k); 
	      tmp->SetElem(-1,0,0,1);
	      tmp->SetElem(0,-1,0,1);
	      tmp->SetElem(0,0,-1,1);
	      tmp->SetElem(0,0,0,-6);
	      tmp->SetElem(0,0,1,1);
	      tmp->SetElem(0,1,0,1); 
	      tmp->SetElem(1,0,0,1);
	    }
	}
    }
  // treatment of boundary points
  return OK;
}


// dirichlet boundary condition
int 
FD_3d_lin_iso::
dirichlet(DataCube &dc, int val)
{
  int i,j,k;
  int n = dc.GetSize()[1];
  int m = dc.GetSize()[2];
  int l = dc.GetSize()[3];
  Stencil3d7 *tmp;
  // upper boundary
  for ( i = 0; i < n; i++ )
    {
      for ( j = 0; j < m; j++ )
	{
	  tmp = a->GetStencilOnGrid(i,j,0);
	  tmp->SetElem(0,0,0,1);
	  (*b)(i,j,0) = val;
	} 
    }
  // lower boundary
  for ( i = 0; i < n; i++ )
    {
      for ( j = 0; j < m; j++ )
	{  
	  tmp = a->GetStencilOnGrid(i,j,l-1);
	  tmp->SetElem(0,0,0,1);
	  (*b)(i,j,l-1) = val;
	} 
    }
  //right boundary
 for ( j = 0; j < m; j++ )
   {
     for ( k = 1; k < l-1; k++ )
       { 
	 tmp = a->GetStencilOnGrid(0,j,k);
	 tmp->SetElem(0,0,0,1);
	 (*b)(0,j,k) = val;
       } 
   }
  //left boundary
 for ( j = 0; j < m; j++ )
   {
     for ( k = 1; k < l-1; k++ )
       {
	 tmp = a->GetStencilOnGrid(n-1,j,k);
	 tmp->SetElem(0,0,0,1);
	 (*b)(n-1,j,k) = val;
       } 
   }
 // back boundary
 for ( i = 1; i < n-1; i++ )
   {
     for ( k = 1; k < l-1; k++ )
       {
	 tmp = a->GetStencilOnGrid(i,0,k);
	 tmp->SetElem(0,0,0,1);
	 (*b)(i,0,k) = val;
       }
   }
 // front boundary
 for ( i = 1; i < n-1; i++ )
   {
     for ( k = 1; k < l-1; k++ )
       {
	 tmp = a->GetStencilOnGrid(i,m-1,k);
	 tmp->SetElem(0,0,0,1);
	 (*b)(i,m-1,k) = val;
       }
   }

 return OK;
}


// neumann boundary condition
int 
FD_3d_lin_iso::
neumann(DataCube &dc, int val)
{
  int i,j,k;
  int n = dc.GetSize()[1];
  int m = dc.GetSize()[2];
  int l = dc.GetSize()[3];
  Stencil3d7 *tmp;

  // upper boundary
  for ( i = 0; i < n; i++ )
    {
      for ( j = 0; j < m; j++ )
	{
	  tmp = a->GetStencilOnGrid(i,j,0);
	  tmp->SetElem(0,0,0,1);
	  tmp->SetElem(0,0,1,-1);
	  (*b)(i,j,0) = val;
	} 
    }
  // lower boundary
  for ( i = 0; i < n; i++ )
    {
      for ( j = 0; j < m; j++ )
	{  
	  tmp = a->GetStencilOnGrid(i,j,l-1);
	  tmp->SetElem(0,0,0,1);
	  tmp->SetElem(0,0,-1,-1);
	  (*b)(i,j,l-1) = val;
	} 
    }
  //right boundary
 for ( j = 0; j < m; j++ )
   {
     for ( k = 1; k < l-1; k++ )
       { 
	 tmp = a->GetStencilOnGrid(0,j,k);
	 tmp->SetElem(0,0,0,1);
	 tmp->SetElem(1,0,0,-1);
	 (*b)(0,j,k) = val;
       } 
   }
  //left boundary
 for ( j = 0; j < m; j++ )
   {
     for ( k = 1; k < l-1; k++ )
       {
	 tmp = a->GetStencilOnGrid(n-1,j,k);
	 tmp->SetElem(0,0,0,1);
	 tmp->SetElem(-1,0,0,-1);
	 (*b)(n-1,j,k) = val;
       } 
   }
 // back boundary
 for ( i = 1; i < n-1; i++ )
   {
     for ( k = 1; k < l-1; k++ )
       {
	 tmp = a->GetStencilOnGrid(i,0,k);
	 tmp->SetElem(0,0,0,1);
	 tmp->SetElem(0,1,0,-1);
	 (*b)(i,0,k) = val;
       }
   }
 // front boundary
 for ( i = 1; i < n-1; i++ )
   {
     for ( k = 1; k < l-1; k++ )
       {
	 tmp = a->GetStencilOnGrid(i,m-1,k);
	 tmp->SetElem(0,0,0,1);
	 tmp->SetElem(0,-1,0,-1);
	 (*b)(i,m-1,k) = val;
       }
   }
 return OK;
}

#endif
