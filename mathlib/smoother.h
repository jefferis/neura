/* Smoother - Template structure including various smoothers

Christoph Reisinger, 191201 - ? */

#ifndef SMOOTHER_HPP
#define SMOOTHER_HPP

#include "list.h"
#include "multivector.h"
#include "../datalib/sparsematrix.h"
#include "sortedlistarray.h"
#include "basesolver.h"

enum SmoothingTypes {LEXICOGRAPHIC_GAUSS_SEIDEL, CHEQUERED_GAUSS_SEIDEL, ORDERED_GAUSS_SEIDEL, 
		     PROJECTED_GAUSS_SEIDEL, POINT_ILU};

template <typename T>
class Smoother : public BaseSolver<T>
{
private:
  SmoothingTypes type;

  // for Gauss-Seidel-ordering
  bool isOrdered;

  Array<int> order;

  // for ILU-decomposition:
  bool isDecomposition;

  SparseMatrix<T> lower;
  SparseMatrix<T> upper;

  MultiVector<T> v, residual, defect;

public:
  Smoother(){isDecomposition=0; isOrdered=0;}
  ~Smoother(){Delete();}
  int Delete(){lower.Delete(); upper.Delete(); isDecomposition=0; return(0);}

  int SetSmoothingType(const SmoothingTypes t){type=t;return 0;}
  SmoothingTypes GetSmoothingType() const {return(type);}

  int Smoothen(SparseMatrix<T>&, MultiVector<T>&, MultiVector<T>&);

  int OrderLayered(MultiVector<T>&);

  int LexicographicGaussSeidelStep(SparseMatrix<T>&, MultiVector<T>&, MultiVector<T>&);
  int ChequeredGaussSeidelStep(SparseMatrix<T>&, MultiVector<T>&, MultiVector<T>&, List<int>&);
  int OrderedGaussSeidelStep(SparseMatrix<T>&, MultiVector<T>&, MultiVector<T>&);
  int ProjectedGaussSeidelStep(SparseMatrix<T>&, MultiVector<T>&, MultiVector<T>&);

  int BuildILU(SparseMatrix<T>&);
  int ILUStep(SparseMatrix<T>&, MultiVector<T>&, MultiVector<T>&);
  int SolveLower(SparseMatrix<T>&, MultiVector<T>&, MultiVector<T>&);
  int SolveUpper(SparseMatrix<T>&, MultiVector<T>&, MultiVector<T>&);
};



template <typename T>
int Smoother<T>::Smoothen(SparseMatrix<T>& mat, MultiVector<T>& b, MultiVector<T>& u)
{
  switch(type)
    {
    case LEXICOGRAPHIC_GAUSS_SEIDEL:
      return(LexicographicGaussSeidelStep(mat,b,u));
    case CHEQUERED_GAUSS_SEIDEL:
      return(1);
      //      return(ChequeredGaussSeidelStep(mat,b,u));
    case ORDERED_GAUSS_SEIDEL:
      return(OrderedGaussSeidelStep(mat,b,u));
    case PROJECTED_GAUSS_SEIDEL:
      return(ProjectedGaussSeidelStep(mat,b,u));
    case POINT_ILU:
      return(ILUStep(mat,b,u));
    default:
      cout << "Smoothen: Unknown smoother type!" << endl;
      return(1);
    }
}




/*************************************************************************
Gauss-Seidel-smoother
*************************************************************************/


template <typename T>
int Smoother<T>::LexicographicGaussSeidelStep(SparseMatrix<T>& mat, MultiVector<T>& b, 
					      MultiVector<T>& u)
{
  int row, column;
  T sum, diag, element;

  for(row=1;row<=u.TotalNumberOfElements();row++)
    {
      mat.GoToDiagonal(row);
      mat.GetElement(row,column,diag);
      sum = 0;

      while(mat.Move(row))
	{
	  mat.GetElement(row,column,element);
	  sum += element*u[column];
	}

      if(diag)
	{
	  u.Set(row,(b[row]-sum)/diag);
	}
      else
	{
	  cout << "Gauss-Seidel-Error: Diagonal has zero element!" << endl; cout << row << endl;
	}
    }

  return(0);
}



template <typename T>
int Smoother<T>::OrderLayered(MultiVector<T>& u)
{
  int dim = u.Dimension();
  Array<int> position(dim), center(dim);

  int i, next, weight, maxWeight, n = u.TotalNumberOfElements();

  SortedListArray sortedList;

  maxWeight = 0;

  for(i=1;i<=dim;i++)
    {
      center[i] = int(u.Length(i)/2) + 1;
      maxWeight += center[i]*center[i];
    }

  sortedList.Reset(n, maxWeight);
  order.Reset(n);

  u.InitPosition(position);

  do
    {
      weight = 0;

      for(i=1;i<=dim;i++)
	{
	  weight += (position[i]-center[i])*(position[i]-center[i]);
	}

      weight++;

      sortedList.InsertSorted(u.Index(position),weight);
    } 
  while(u.NextPosition(position));


  for(i=n;i>=1;i--)
    {
      next = sortedList.Tail();
      
      order[i] = next;

      sortedList.CutTail();
    }

  isOrdered = 1;

  return(0);
}



template <typename T>
int Smoother<T>::OrderedGaussSeidelStep(SparseMatrix<T>& mat, MultiVector<T>& b, 
					      MultiVector<T>& u)
{
  int i, row, column;
  T sum, diag, element;

  if(!isOrdered)
    {
      OrderLayered(u);
    }

  for(i=1;i<=u.TotalNumberOfElements();i++)
    {
      row = order[i];

      mat.GoToDiagonal(row);
      mat.GetElement(row,column,diag);
      sum = 0;

      while(mat.Move(row))
	{
	  mat.GetElement(row,column,element);
	  sum += element*u[column];
	}

      if(diag)
	{
	  u.Set(row,(b[row]-sum)/diag);
	}
      else
	{
	  cout << "Gauss-Seidel-Error: Diagonal has zero element!" << endl; cout << row << endl;
	}
    }

  return(0);
}



template <typename T>
int Smoother<T>::ChequeredGaussSeidelStep(SparseMatrix<T>& mat, MultiVector<T>& b, 
					  MultiVector<T>& u, List<int>& nodes)
{
  int row, column;
  T sum, diag, element;
  Array<bool> isInList(u.TotalNumberOfElements());

  isInList.SetZero();
  nodes.GoToHead();
  
  while(nodes.GetElement(row))
    {
      mat.GoToDiagonal(row);
      mat.GetElement(row,column,diag);
      sum = 0;

      while(mat.Move(row))
	{
	  mat.GetElement(row,column,element);
	  sum += element*u[column];
	}

      if(diag)
	{
	  u.Set(row,(b[row]-sum)/diag);
	}
      else
	{
	  cout << "Gauss-Seidel-Error: Diagonal has zero element!" << endl;
	}

      isInList.SetElement(row,1);
      nodes.Move();
    }


  for(row=1;row<=u.TotalNumberOfElements();row++)
    {
      if(!isInList[row])
	{
	  mat.GoToDiagonal(row);
	  mat.GetElement(row,column,diag);
	  sum = 0;
	  
	  while(mat.Move(row))
	    {
	      mat.GetElement(row,column,element);
	      sum += element*u[column];
	    }
	  
	  if(diag)
	    {
	      u.Set(row,(b[row]-sum)/diag);
	    }
	  else
	    {
	      cout << "Gauss-Seidel-Error: Diagonal has zero element!" << endl;
	    }
	}
    }

  return(0);
}



template <typename T>
int Smoother<T>::ProjectedGaussSeidelStep(SparseMatrix<T>& mat, MultiVector<T>& b, MultiVector<T>& u)
{
  int row, column;
  T sum, diag, element;

  MultiVector<T> w;
  T omega = 1.;

  w.Reset(u.Length());
  w=u;

  for(row=1;row<=u.TotalNumberOfElements();row++)
    {
      mat.GoToDiagonal(row);
      mat.GetElement(row,column,diag);
      sum = 0;

      while(mat.Move(row))
	{
	  mat.GetElement(row,column,element);
	  sum += element*u[column];
	}

      if(diag)
	{
	  u.Set(row,(b[row]-sum)/diag);

	  if(u[row]<this->projection[row])
	    {
	      //	      cout << "correction at " << row << endl;
	      u[row] = this->projection[row];
	    }
	}
      else
	{
	  cout << "Gauss-Seidel-Error: Diagonal has zero element!" << endl; cout << row << endl;
	}
    }

  u = ((T)omega) *u + ((T) 1.-omega)*w;

  return(0);







//   int i, n = u.TotalNumberOfElements();
//   MultiVector<T> w;
//   T omega = 1.5;

//   w.Reset(u.Length());
//   w=u;


//   LexicographicGaussSeidelStep(mat, b, u);
//   u = omega*u + (1.-omega)*w;

//   for(i=1;i<=n;i++)
//     {
//       if(u[i]<v[i])
// 	{
// 	  u[i] = v[i];
// 	}
//     }

//   return(0);
}



/*************************************************************************
ILU-smoother
*************************************************************************/


template <typename T>
int Smoother<T>::BuildILU(SparseMatrix<T>& mat)
{
  //  cout << "build.." << endl;
  //  cout << mat << endl;

  int i, j, k, index, dim = mat.Dimension(), n = mat.TotalNumberOfElements();
  T a, d, diag;
  Array<int> posI(dim), posJ(dim);
  Array<T> product(dim);

  lower.Reset(mat.Length());
  upper.Reset(mat.Length());

  for(i=1; i<=n; i++)
    {
      mat.GoToDiagonal(i);
      mat.GetElement(i,j,d);

      posI = mat.Position(i);
      product.SetOne();

      while(mat.Move(i))
	{
	  mat.GetElement(i,j,a);

	  posJ = mat.Position(j);
	  
	  for(k=1;k<=dim;k++)
	    {
	      if(posI[k] == posJ[k])
		{
		  product[k] *= a;
		}
	    }

	  if(i>j)
	    {
	      lower.Insert(i,j,a);
	    }
	  else
	    {
	      upper.Insert(i,j,a);
	    }
	}
      
      for(k=1;k<=dim;k++)
	{
	  if(posI[k]>1)
	    {
	      posI[k]--;
	      index = mat.Index(posI);
	      upper.GoToDiagonal(index);
	      upper.GetElement(index,j,diag);
	      d -= product[k]/diag;

	      posI[k]++;
	    }
	}

      upper.Insert(i,i,d);
    }

  for(i=1;i<=n;i++)
    {
      upper.GoToDiagonal(i);
      upper.GetElement(i,j,d);
      lower.MultiplyRow(i,1./d);
      lower.Insert(i,i,1);
    }

  isDecomposition = 1;

  return(0);
}



template <typename T>
int Smoother<T>::ILUStep(SparseMatrix<T>& mat, MultiVector<T>& b, MultiVector<T>& u)
{
  if(!isDecomposition)
    {
      BuildILU(mat);
      v.Reset(mat.Length());
      residual.Reset(mat.Length());
      defect.Reset(mat.Length());
    }

  CalcResidual(mat,b,u,residual);

  SolveLower(lower,residual,v);
  SolveUpper(upper,v,defect);

  u += defect;

  return(0);
}



template <typename T>
int Smoother<T>::SolveLower(SparseMatrix<T>& lower, MultiVector<T>& b, MultiVector<T>& u)
{
  int i, j, n = lower.TotalNumberOfElements();
  T l;

  for(i=1;i<=n;i++)
    {
      u[i] = b[i];

      lower.GoToDiagonal(i);

      while(lower.Move(i))
	{
	  lower.GetElement(i,j,l);
	  u[i] -= l*u[j];
	}
    }

  return(0);
}



template <typename T>
int Smoother<T>::SolveUpper(SparseMatrix<T>& upper, MultiVector<T>& b, MultiVector<T>& u)
{
  int i, j, n = upper.TotalNumberOfElements();
  T r, d;

  for(i=n;i>=1;i--)
    {
      u[i] = b[i];

      upper.GoToDiagonal(i);
      upper.GetElement(i,j,d);

      while(upper.Move(i))
	{
	  upper.GetElement(i,j,r);
	  u[i] -= r*u[j];
	}

      u[i] = u[i]/d;
    }

  return(0);
}


#endif // SMOOTHER_HPP
