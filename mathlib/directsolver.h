/* DirectSolver - Template structure including various solvers

Christoph Reisinger, 191201 - ? */

#ifndef DIRECTSOLVER_HPP
#define DIRECTSOLVER_HPP

#include "multivector.h"
#include "../datalib/sparsematrix.h"
#include "basesolver.h"
//#include "Smoother.hpp"
#include "multigrid.h"

template <typename T>
class DirectSolver : public BaseSolver<T>
{
private:
  SolverTypes type;

public:
    int SetSolverType(const SolverTypes t){type = t;return 0;}
  int Solve(SparseMatrix<T>&, MultiVector<T>&, MultiVector<T>&);

  int SolveGaussSeidel(SparseMatrix<T>&, MultiVector<T>&, MultiVector<T>&);
  int SolveBiCGstab(SparseMatrix<T>&, MultiVector<T>&, MultiVector<T>&);
  int SolveILU(SparseMatrix<T>&, MultiVector<T>&, MultiVector<T>&);
  int SolveProjectedGaussSeidel(SparseMatrix<T>&, MultiVector<T>&, MultiVector<T>&);
};



template <typename T>
int DirectSolver<T>::Solve(SparseMatrix<T>& mat, MultiVector<T>& b, MultiVector<T>& u)
{
  switch(type)
    {
    case GAUSS_SEIDEL:
      return(SolveGaussSeidel(mat, b, u));
    case BICGSTAB:
      return(SolveBiCGstab(mat, b, u));
    case ILU:
      return(SolveILU(mat, b, u));
    default:
      cout << "DirectSolver::Solve: Unknown solver type!" << endl;
      return(1);
    }
    return 0;
}



template <typename T>
int DirectSolver<T>::SolveGaussSeidel(SparseMatrix<T>& mat, MultiVector<T>& b, MultiVector<T>& u)
{
  Smoother<T> gaussSeidel;
  int step = 0;
  const T epsilon2 = 1e-10;

  gaussSeidel.SetSmoothingType(LEXICOGRAPHIC_GAUSS_SEIDEL);
  //  gaussSeidel.SetSmoothingType(ORDERED_GAUSS_SEIDEL);

  T iniErr = ErrorNorm(mat,b,u);
  T err = iniErr;
  while( 
	( ErrorNorm(mat,b,u)>epsilon2 ) &&
	( err>this->epsilon*iniErr) &&
	( ++step<this->maxSteps )
	)
    {
      //cout << "step: " << step << ", error: " << ErrorNorm(mat,b,u) << endl;
      //cout << u;
      gaussSeidel.Smoothen(mat,b,u);
      err = ErrorNorm(mat,b,u);

    }
  cout << "GaussSeidel steps: " << step << endl;
  return(int(step==this->maxSteps));
}



template <typename T>
int DirectSolver<T>::SolveProjectedGaussSeidel(SparseMatrix<T>& mat, MultiVector<T>& b, MultiVector<T>& u)
{
  Smoother<T> gaussSeidel;
  int step = 0;

  gaussSeidel.SetSmoothingType(PROJECTED_GAUSS_SEIDEL);
  gaussSeidel.SetProjection(this->projection);

  while((ProjectedErrorNorm(mat,b,u)>this->epsilon && step<this->maxSteps) || step<5)
    {
      //      cout << "step: " << step << ", error: " << ProjectedErrorNorm(mat,b,u) << endl;
      gaussSeidel.Smoothen(mat,b,u);
      ++step;
    }

  return(int(step==this->maxSteps));
}



template <typename T>
int DirectSolver<T>::SolveILU(SparseMatrix<T>& mat, MultiVector<T>& b, MultiVector<T>& u)
{
  Smoother<T> ilu;
  int step = 0;

  ilu.SetSmoothingType(POINT_ILU);

  while(ErrorNorm(mat,b,u)>this->epsilon && ++step<this->maxSteps)
    {
      //      cout << "step: " << step << ", error: " << ErrorNorm(mat,b,u) << endl;
      ilu.Smoothen(mat,b,u);
    }

  return(int(step==this->maxSteps));
}



template <typename T>
int DirectSolver<T>::SolveBiCGstab(SparseMatrix<T>& mat, MultiVector<T>& b, MultiVector<T>& u)
{
  MultiVector<T> r, rBar, p, pHat, v, s, sHat, t;
  T rho, rhoOld, alpha, beta, omega;
  T err, oldErr = 1, iniErr;
  const T epsilon2 = 1e-10;
  int step = 0;

  r.Reset(mat.TotalNumberOfElements());
  rBar.Reset(mat.TotalNumberOfElements());
  p.Reset(mat.TotalNumberOfElements());
  pHat.Reset(mat.TotalNumberOfElements());
  v.Reset(mat.TotalNumberOfElements());
  s.Reset(mat.TotalNumberOfElements());
  sHat.Reset(mat.TotalNumberOfElements());
  t.Reset(mat.TotalNumberOfElements());

  r = b-mat*u;
  rBar = r;

  err = ErrorNorm(mat,b,u);
  iniErr = err;
  //  cout << "bicgstab begin: err = " << err << endl;

  while( 
	( err>epsilon2 ) 
	&& 
	( err>this->epsilon*iniErr) 
	&& 
	( ++step<this->maxSteps )
	)
    {
     
      rho = rBar*r;

      if(step==1)
	{
	  p = r;
	}
      else
	{
	  beta = (rho/rhoOld)*(alpha/omega);
	  p = r + beta*(p-omega*v);
	}

      pHat = p;
      v = mat*pHat;
      alpha = rho/(rBar*v);
      s = r-alpha*v;

      if(Norm(s)<epsilon2)
	{
	  u = u + alpha*pHat;
	  cout << "1. criterion: BiCGstab steps: " << step << endl; 
	  //	  cout << "error:" << ErrorNorm(mat,b,u) << endl;
	  return(0);
	}

      sHat = s;
      t = mat*sHat;
      omega = (t*s)/(t*t);
      u = u + alpha*pHat + omega*sHat;
      r = s - omega*t;


      rhoOld = rho;
      oldErr = err;

      err = ErrorNorm(mat,b,u);
      //      cout << "BiCGstab: error = " <<  err << ", rate: " << err/oldErr << endl;
    }
  
  cout << "BiCGstab: error = " <<  err << ", rel.: " << err/iniErr << endl;
  cout << "BiCGstab steps: " << step << endl;
  return(int(step==this->maxSteps));
}





#endif // DIRECTSOLVER_HPP
