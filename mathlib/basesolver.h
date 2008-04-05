/* BaseSolver - Template structure including various utilities for solvers

Christoph Reisinger, 180102 - ? */

#ifndef BASESOLVER_HPP
#define BASESOLVER_HPP

#include "multivector.h"
#include "../datalib/sparsematrix.h"

enum SolverTypes {GAUSS_SEIDEL, BICGSTAB, RICHARDSON, ILU};

template <typename T>
class BaseSolver
{
protected:
  double epsilon;
  int maxSteps;
  bool isProjected;
  MultiVector<T> projection;

public:
  BaseSolver(){isProjected=0;}
  ~BaseSolver(){projection.Delete();}
  int SetPrecision(const double precision){epsilon = precision;return 0;}
  int SetMaximumSteps(const int steps){maxSteps = steps; return 0;}

  int SetProjection(const MultiVector<T>& w){isProjected = 1; projection.Reset(w.Length()); projection=w; return(0);}

  int CalcResidual(SparseMatrix<T>&, const MultiVector<T>&, const MultiVector<T>&, MultiVector<T>&);
  T ErrorNorm(SparseMatrix<T>&, const MultiVector<T>&, const MultiVector<T>&);
  T ProjectedErrorNorm(SparseMatrix<T>&, const MultiVector<T>&, const MultiVector<T>&);
};



template <typename T>
int BaseSolver<T>::CalcResidual(SparseMatrix<T>& mat, const MultiVector<T>& b,
				const MultiVector<T>& u, MultiVector<T>& residual)
{
  residual = b-(mat*u);

  return(0);
}



template <typename T>
T BaseSolver<T>::ErrorNorm(SparseMatrix<T>& mat, const MultiVector<T>& b, const MultiVector<T>& u)
{
  MultiVector<T> residual;
  T norm = 0;
  //double sum=0;
  residual.Reset(mat.TotalNumberOfElements());

  CalcResidual(mat, b, u, residual);
  
 
  //for (int i=1;i<residual.TotalNumberOfElements();i++)
  //  {
    
  //  sum+=residual[i]*residual[i];
  //  }
 
  //norm=sqrt(sum);
  norm = Norm(residual);
  
  return(norm);
}


template <typename T>
T BaseSolver<T>::ProjectedErrorNorm(SparseMatrix<T>& mat, const MultiVector<T>& b, const MultiVector<T>& u)
{
  int i, n = u.TotalNumberOfElements();
  T eps = 1e-12;

  MultiVector<T> residual;

  //  residual.Reset(mat.TotalNumberOfElements());
  residual.Reset(mat.Length());

  CalcResidual(mat, b, u, residual);

  //  cout << residual;

  for(i=1;i<=n;i++)
    {
      if(fabs(u[i]-projection[i])<eps)
	{
	  residual[i] = 0.;
	}
      else if(u[i]<projection[i]-eps)
	{
	  residual[i] = fabs(u[i]-projection[i]);
	  //	  cout << "Restriction failed: u[i]-projection[i] = " << u[i]-projection[i] << " at i = " << i << endl;
	}
    }

  return(Norm(residual));
}


#endif // BASESOLVER_HPP
