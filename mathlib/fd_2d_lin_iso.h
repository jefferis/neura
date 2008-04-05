/****************************************************************************/
/*                                                                          */
/* File:      fd_2d_lin_iso.h                                               */
/*                                                                          */
/* Purpose:   routines to discretisize linear isotropic diffusion           */
/*            by finite differences in 2d                                   */
/*                                                                          */
/* Author:    Roland Schulte                                                */
/*            Interdisziplinaeres Zentrum fuer Wissenschaftliches Rechnen   */
/*            Universitaet Heidelberg                                       */
/*            Im Neuenheimer Feld 368                                       */
/*            D-69120 Heidelberg                                            */
/*                                                                          */
/* EMail:     roland.schulte@iwr.uni-heidelberg.de                          */
/*                                                                          */
/* History:   04.12.02 begin                                                */
/*                                                                          */
/* Remarks:                                                                 */
/*                                                                          */
/****************************************************************************/
#ifndef FD_2D_LIN_ISO
#define FD_2D_LIN_ISO

#include "../datalib/stencil.h"
#include "../datalib/stencil2d5.h"
#include "../datalib/algequation.h"
#include "../datalib/sparsematrix.h"

class FD_2d_lin_iso
{
  
 public:
  
  FD_2d_lin_iso(SparseMatrix<Stencil2d5> *matrix, DataCube *rhs);
  ~FD_2d_lin_iso();
  
  int discretisize(DataCube &dc);

  int dirichlet(DataCube &dc, int val);

  int neumann(DataCube &dc, int val);

 private:
  
  SparseMatrix<Stencil2d5> *a; // pointer to matrix
  DataCube *b; // pointer to rhs

};

FD_2d_lin_iso::
FD_2d_lin_iso(SparseMatrix<Stencil2d5> *matrix, DataCube *rhs)
{
  a = matrix;
  b = rhs;
}


FD_2d_lin_iso::
~FD_2d_lin_iso()
{

}



// discretisizes 2d linear isotropic diffusion 
// by finite difference scheme
int 
FD_2d_lin_iso::
discretisize(DataCube &dc)
{
  // treatment of inner points
  int i,j;
  Stencil2d5 *tmp;
  for ( i = 1; i < dc.GetSize()[1]-1; i++ )
    {
      for ( j = 1; j < dc.GetSize()[2]-1; j++ )
	{
	  tmp = a->GetStencilOnGrid(i,j);
	  tmp->SetElem(0,0,-4);
	  tmp->SetElem(-1,0,1);
	  tmp->SetElem(1,0,1);
	  tmp->SetElem(0,-1,1);
	  tmp->SetElem(0,1,1);
	}
    }
  // treatment of boundary points
  return OK;
}


// dirichlet boundary condition
int 
FD_2d_lin_iso::
dirichlet(DataCube &dc, int val)
{
  int i,j;
  int n = dc.GetSize()[1];
  int m = dc.GetSize()[2];
  Stencil2d5 *tmp;
  // upper boundary
  for ( i = 0; i < n; i++ )
    {
      tmp = a->GetStencilOnGrid(i,0);
      tmp->SetElem(0,0,1);
      (*b)(i,0) = val; 
    }
  // lower boundary
 for ( i = 0; i < n; i++ )
    {
      tmp = a->GetStencilOnGrid(i,m-1);
      tmp->SetElem(0,0,1);
      (*b)(i,m-1) = val; 
    }
 //left boundary
 for ( j = 1; j < m-1; j++ )
   {
     tmp = a->GetStencilOnGrid(0,j);
     tmp->SetElem(0,0,1);
     (*b)(0,j) = val; 
   }
  //right boundary
 for ( j = 1; j < m-1; j++ )
   {
     tmp = a->GetStencilOnGrid(n-1,j);
     tmp->SetElem(0,0,1);
     (*b)(n-1,j) = val; 
   }
 return OK;
}


// neumann boundary condition
int 
FD_2d_lin_iso::
neumann(DataCube &dc, int val)
{
  int i,j;
  int n = dc.GetSize()[1];
  int m = dc.GetSize()[2];
  Stencil2d5 *tmp;
  // upper boundary
  for ( i = 0; i < n; i++ )
    {
      tmp = a->GetStencilOnGrid(i,0);
      tmp->SetElem(0,0,1);
      tmp->SetElem(0,1,-1);
      (*b)(i,0) = val; 
    }
  // lower boundary
 for ( i = 0; i < n; i++ )
    {
      tmp = a->GetStencilOnGrid(i,m-1);
      tmp->SetElem(0,0,1);
      tmp->SetElem(0,-1,-1); 
      (*b)(i,m-1) = val; 
    }
 //left boundary
 for ( j = 1; j < m-1; j++ )
   {
     tmp = a->GetStencilOnGrid(0,j);
     tmp->SetElem(0,0,1);
     tmp->SetElem(1,0,-1);
     (*b)(0,j) = val;
   }
  //right boundary
 for ( j = 1; j < m-1; j++ )
   {
     tmp = a->GetStencilOnGrid(n-1,j);
     tmp->SetElem(0,0,1);
     tmp->SetElem(-1,0,-1); 
     (*b)(n-1,j) = val; 
   }
 return OK;
}

#endif
