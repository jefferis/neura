/* Multigrid - Template structure governing the multigrid cycle

Christoph Reisinger, 080102 - ? */

#ifndef MULTIGRID_HPP
#define MULTIGRID_HPP

#include "smoother.h"
#include "prolongation.h"
#include "directsolver.h"

#include <math.h>

enum CycleTypes {V=1, W};

template <typename T>
class Multigrid : public BaseSolver<T>
{
private:
  Array<Smoother<T> > smoother;
  Array<Prolongation<T> > prolongation;
  DirectSolver<T> directSolver;

  CycleTypes cycleType;
  int preSmoothingSteps, postSmoothingSteps;
  int numberOfLevels;

  bool saveProlongation, saveRestriction;
  bool labelling;
  bool isProjected;
  bool isFull;
  bool isCascadic;

  Array<SparseMatrix<T> > matrixHierarchy;
  Array<List<int> > nodeHierarchy;

  Array<MultiVector<T> > rhsHierarchy;
  Array<MultiVector<T> > projectionHierarchy;

public:
  Multigrid(){saveProlongation=0; saveRestriction=0;  isProjected=0; isFull=0; isCascadic=0;}
  ~Multigrid();

  int SetCycleType(const CycleTypes type){cycleType = type; return(0);}
  int SetNumberOfLevels(const int levels){numberOfLevels = levels; return(0);}
  int SetSmoothingSteps(const int pre, const int post){preSmoothingSteps = pre; postSmoothingSteps = post; return(0);}
  int Initialise(const LabellingTypes labellingType, const InterpolationTypes
		 interpolationType, const RestrictionTypes restrictionType,
		 const double alpha, const SmoothingTypes smoothingType,
		 int pre, int post, const CycleTypes type, const SolverTypes solverType, 
		 const TypeOfUnknowns typeOfUnknowns, const int levels);
  int SetProjection(const MultiVector<T>&);

  int ResetInterpolation(const InterpolationTypes interpolationType);
  int ResetRestriction(const RestrictionTypes restrictionType);

  int SaveProlongation(){saveProlongation=1; return(0);}
  int SaveRestriction(){saveRestriction=1; return(0);}
  int FullMultigrid(){isFull=1; return(0);}
  int Cascadic(){isCascadic=1; return(0);}
  int Projected(){isProjected=1; return(0);}
  int BuildMatrixHierarchy(SparseMatrix<T> & mat);

  int MultigridCycle(MultiVector<T>&, MultiVector<T>&);
  int FullMultigridCycle(MultiVector<T>&, MultiVector<T>&);
  int FullProjectedMultigridCycle(MultiVector<T>&, const MultiVector<T>&, MultiVector<T>&);
  int CascadicMultigridCycle(MultiVector<T>&, MultiVector<T>&);
  int CascadicProjectedMultigridCycle(MultiVector<T>&, const MultiVector<T>&, MultiVector<T>&);
  int MultigridCycle(MultiVector<T>&, MultiVector<T>&, const int);
  int ProjectedMultigridCycle(MultiVector<T>&, MultiVector<T>&, MultiVector<T>&, const int);
  int Solve(MultiVector<T>&, MultiVector<T>&);

  int PrintHierarchy(ostream&);
};


template <typename T>
Multigrid<T>::~Multigrid()
{
 // int i;

  /*
  for(i=1;i<=matrixHierarchy.Length();i++)
    {
      matrixHierarchy[i].Delete();
      nodeHierarchy[i].Delete();
      prolongation[i].Delete();
      smoother[i].Delete();
    }
  */
  /*
  matrixHierarchy.Delete();
  nodeHierarchy.Delete();
  prolongation.Delete();
  smoother.Delete();
  */
  isProjected = 0;
  isFull = 0;
}



template <typename T>
int Multigrid<T>::Initialise(const LabellingTypes labellingType, const InterpolationTypes
			     interpolationType, const RestrictionTypes restrictionType,
			     const double alpha, const SmoothingTypes 
			     smoothingType, int pre, int post, const CycleTypes type, 
			     const SolverTypes solverType, const TypeOfUnknowns typeOfUnknowns,
			     const int levels)
{
  int level;

  prolongation.Reset(levels);

  for(level=1; level<=levels;level++)
    {
      prolongation[level].SetLabellingType(labellingType);
      prolongation[level].SetInterpolationType(interpolationType);
      prolongation[level].SetAlpha(alpha);
      prolongation[level].SetTypeOfUnknowns(typeOfUnknowns);

      if(RestrictionTypes(interpolationType) != restrictionType)
	{
	  prolongation[level].SetRestrictionType(restrictionType);
	}
    }

  if(typeOfUnknowns == NODES)
    {
      labelling = 1;
    }
  else
    {
      labelling = 0;  
    }

  smoother.Reset(levels);

  for(level=1; level<=levels;level++)
    {
      smoother[level].SetSmoothingType(smoothingType);
    }

  directSolver.SetSolverType(solverType);
  directSolver.SetPrecision(epsilon);
  directSolver.SetMaximumSteps(10000);

  SetCycleType(type);
  SetNumberOfLevels(levels);
  SetSmoothingSteps(pre, post);
  return 0;
}



template <typename T>
int Multigrid<T>::ResetInterpolation(const InterpolationTypes interpolationType)
{
  int level;

  prolongation.Reset(numberOfLevels);

  for(level=2; level<=numberOfLevels;level++)
    {
      prolongation[level].SetInterpolationType(interpolationType);

      if(saveProlongation)
	{
	  prolongation[level].CreateProlongation(matrixHierarchy[level], nodeHierarchy[level]);
	}
    }

  return(0);
}



template <typename T>
int Multigrid<T>::ResetRestriction(const RestrictionTypes restrictionType)
{
  int level;

  //  prolongation.Reset(numberOfLevels);

  for(level=2; level<=numberOfLevels;level++)
    { 
      prolongation[level].SetRestrictionType(restrictionType);

      if(saveRestriction)
	{
	  prolongation[level].CreateRestriction(matrixHierarchy[level], nodeHierarchy[level]);
	}
    }

  return(0);
}



template <typename T>
int Multigrid<T>::BuildMatrixHierarchy(SparseMatrix<T> & matrix)
{
  int level;

  matrixHierarchy.Reset(numberOfLevels);
  nodeHierarchy.Reset(numberOfLevels);

  matrixHierarchy.SetElement(numberOfLevels,matrix);

  for(level=numberOfLevels; level>1; level--)
    {
      if(labelling)
	{
	  prolongation[level].Label(matrixHierarchy[level], nodeHierarchy[level]);
	}

      if(saveProlongation)
	{
	  prolongation[level].CreateProlongation(matrixHierarchy[level], nodeHierarchy[level]);
	}

      if(saveRestriction)
	{
	  prolongation[level].CreateRestriction(matrixHierarchy[level], nodeHierarchy[level]);
	}

      prolongation[level].RestrictMatrix(matrixHierarchy[level],nodeHierarchy[level],
				  matrixHierarchy[level-1]);
    }

  if(isFull || isCascadic)
    {
      rhsHierarchy.Reset(numberOfLevels);

      for(level=1; level<=numberOfLevels; level++)
	{
	  rhsHierarchy[level].Reset(matrixHierarchy[level].Length());
	}

      if(isProjected)
	{
	  projectionHierarchy.Reset(numberOfLevels);

	  for(level=1; level<=numberOfLevels; level++)
	    {
	      projectionHierarchy[level].Reset(matrixHierarchy[level].Length());
	    }
	}
    }

  for(level=numberOfLevels; level>=1; level--)
    {
 #ifdef ModelP
      cout << me << ": ";
#endif
      cout << "Level " << level << ": " << matrixHierarchy[level].TotalNumberOfElements() 
	   << " nodes" << endl;
    }

  return(0);
}



template <typename T>
int Multigrid<T>::SetProjection(const MultiVector<T>& u)
{
  int level;

  isProjected = 1;
  smoother[numberOfLevels].SetProjection(u);
  smoother[numberOfLevels].SetSmoothingType(PROJECTED_GAUSS_SEIDEL);

  projection.Reset(u.Length());
  projection = u;

  for(level = 1; level<numberOfLevels; level++)
    {
      //      smoother[level].SetProjection();
      smoother[level].SetSmoothingType(PROJECTED_GAUSS_SEIDEL);
    } 

  //  directSolver.SetSolverType(PROJECTED_GAUSS_SEIDEL);
}



template <typename T>
int Multigrid<T>::MultigridCycle(MultiVector<T>& b, MultiVector<T>& u)
{
  if(isProjected)
    {
      return(ProjectedMultigridCycle(b,u,projection,numberOfLevels));
    }
  else
    {
      return(MultigridCycle(b,u,numberOfLevels));
    }
}



template <typename T>
int Multigrid<T>::FullMultigridCycle(MultiVector<T>& b, MultiVector<T>& u)
{
  int level, step, numberOfSteps = 2;
  MultiVector<T> coarseSolution, fineSolution;

  rhsHierarchy[numberOfLevels] = b;

  for(level=numberOfLevels; level>1; level--)
    {
      prolongation[level].Restrict(matrixHierarchy[level], nodeHierarchy[level], rhsHierarchy[level-1],
				   rhsHierarchy[level]);
    }

  coarseSolution.Reset(matrixHierarchy[1].Length());

  directSolver.Solve(matrixHierarchy[1],rhsHierarchy[1],coarseSolution);

  for(level=2;level<=numberOfLevels;level++)
    {
      fineSolution.Reset(matrixHierarchy[level].Length());
      prolongation[level].Prolong(matrixHierarchy[level], nodeHierarchy[level], coarseSolution,
				  fineSolution);

      for(step=1; step<=numberOfSteps; step++)
	{
	  MultigridCycle(rhsHierarchy[level],fineSolution,level);
	}

      coarseSolution.Reset(fineSolution.Length());
      coarseSolution = fineSolution;      
    }

  u = fineSolution;

  return(0);
}



template <typename T>
int Multigrid<T>::FullProjectedMultigridCycle(MultiVector<T>& b, const MultiVector<T>& c, MultiVector<T>& u)
{
  int level, step, numberOfSteps = 4;
  MultiVector<T> coarseSolution, fineSolution;

  rhsHierarchy[numberOfLevels] = b;

  projectionHierarchy[numberOfLevels] = c;

  for(level=numberOfLevels; level>1; level--)
    {
//       prolongation[level].Restrict(matrixHierarchy[level], nodeHierarchy[level], rhsHierarchy[level-1],
// 				   rhsHierarchy[level]);
//       prolongation[level].Restrict(matrixHierarchy[level], nodeHierarchy[level], projectionHierarchy[level-1],
// 				   projectionHierarchy[level]);

      prolongation[level].Inject(nodeHierarchy[level], rhsHierarchy[level-1], rhsHierarchy[level]);
      prolongation[level].Inject(nodeHierarchy[level], projectionHierarchy[level-1], projectionHierarchy[level]);
    }

  coarseSolution.Reset(matrixHierarchy[1].Length());

  directSolver.SetProjection(projectionHierarchy[1]);
  directSolver.SolveProjectedGaussSeidel(matrixHierarchy[1],rhsHierarchy[1],coarseSolution);

  for(level=2;level<=numberOfLevels;level++)
    {
      fineSolution.Reset(matrixHierarchy[level].Length());
      prolongation[level].Prolong(matrixHierarchy[level], nodeHierarchy[level], coarseSolution,
				  fineSolution);

      for(step=1; step<=numberOfSteps; step++)
	{
	  ProjectedMultigridCycle(rhsHierarchy[level],fineSolution,projectionHierarchy[level],level);
	}

      coarseSolution.Reset(fineSolution.Length());
      coarseSolution = fineSolution;      
    }

  u = fineSolution;

  return(0);
}



template <typename T>
int Multigrid<T>::CascadicMultigridCycle(MultiVector<T>& b, MultiVector<T>& u)
{
  const int factor = 3;
  int level, step, numberOfSteps;
  MultiVector<T> coarseSolution, fineSolution;

  numberOfSteps = postSmoothingSteps*pow(factor,numberOfLevels-1);

  rhsHierarchy[numberOfLevels] = b;

  for(level=numberOfLevels; level>1; level--)
    {
      prolongation[level].Restrict(matrixHierarchy[level], nodeHierarchy[level], rhsHierarchy[level-1],
				   rhsHierarchy[level]);
    }

  coarseSolution.Reset(matrixHierarchy[1].Length());

  directSolver.Solve(matrixHierarchy[1],rhsHierarchy[1],coarseSolution);

  for(level=2;level<=numberOfLevels;level++)
    {
      fineSolution.Reset(matrixHierarchy[level].Length());
      prolongation[level].Prolong(matrixHierarchy[level], nodeHierarchy[level], coarseSolution,
				  fineSolution);     

      for(step=1; step<=numberOfSteps; step++)
	{
	  smoother[level].Smoothen(matrixHierarchy[level],rhsHierarchy[level],fineSolution);
	}

      numberOfSteps = numberOfSteps/factor;

      coarseSolution.Reset(fineSolution.Length());
      coarseSolution = fineSolution;      
    }

  u = fineSolution;

  return(0);
}



template <typename T>
int Multigrid<T>::CascadicProjectedMultigridCycle(MultiVector<T>& b, const MultiVector<T>& c, MultiVector<T>& u)
{
  const int factor = 5;
  int level, step, numberOfSteps;
  MultiVector<T> coarseSolution, fineSolution;

  numberOfSteps = postSmoothingSteps*pow(factor,numberOfLevels-1);

  rhsHierarchy[numberOfLevels] = b;
  projectionHierarchy[numberOfLevels] = c;

  for(level=numberOfLevels; level>1; level--)
    {
      prolongation[level].Restrict(matrixHierarchy[level], nodeHierarchy[level], rhsHierarchy[level-1],
				   rhsHierarchy[level]);
      prolongation[level].Restrict(matrixHierarchy[level], nodeHierarchy[level], projectionHierarchy[level-1],
				   projectionHierarchy[level]);
      smoother[level].SetSmoothingType(PROJECTED_GAUSS_SEIDEL);
      smoother[level].SetProjection(projectionHierarchy[level]);
    }

  coarseSolution.Reset(matrixHierarchy[1].Length());

  directSolver.SetProjection(projectionHierarchy[1]);
  directSolver.SolveProjectedGaussSeidel(matrixHierarchy[1],rhsHierarchy[1],coarseSolution);

  for(level=2;level<=numberOfLevels;level++)
    {
      fineSolution.Reset(matrixHierarchy[level].Length());
      prolongation[level].Prolong(matrixHierarchy[level], nodeHierarchy[level], coarseSolution,
				  fineSolution);     

      for(step=1; step<=numberOfSteps; step++)
	{
	  smoother[level].Smoothen(matrixHierarchy[level],rhsHierarchy[level],fineSolution);
	}

      numberOfSteps = numberOfSteps/factor;

      coarseSolution.Reset(fineSolution.Length());
      coarseSolution = fineSolution;      
    }

  u = fineSolution;

  return(0);
}



template <typename T>
int Multigrid<T>::MultigridCycle(MultiVector<T>& b, MultiVector<T>& u, const int level)
{
  int step;
  MultiVector<T> coarseResidual, fineResidual, coarseDefect, fineDefect;

  if(level == 1)
    {
      if(directSolver.Solve(matrixHierarchy[1],b,u))
	{
	  cout << "Warning: No solution on coarse grid found!" << endl;
	  return(1);
	}
      else
	{
	  return(0);
	}
    }


  for(step=1;step<=preSmoothingSteps;step++)
    {
      smoother[level].Smoothen(matrixHierarchy[level],b,u);
    }

  fineResidual.Reset(u.Length());

  smoother[level].CalcResidual(matrixHierarchy[level],b,u,fineResidual);

  coarseResidual.Reset(nodeHierarchy[level].Length());

  prolongation[level].Restrict(matrixHierarchy[level], nodeHierarchy[level], coarseResidual,
			fineResidual);

  fineResidual.Delete();

  coarseDefect.Reset(coarseResidual.Length());
  coarseDefect.SetZero();

  for(step=1;step<=cycleType;step++)
    {
      MultigridCycle(coarseResidual, coarseDefect,level-1);
    }

  coarseResidual.Delete(); fineDefect.Reset(u.Length());

  prolongation[level].Prolong(matrixHierarchy[level], nodeHierarchy[level], coarseDefect,
			fineDefect);

  u += fineDefect;
  
  coarseDefect.Delete(); fineDefect.Delete();

  for(step=1;step<=postSmoothingSteps;step++)
    {
      smoother[level].Smoothen(matrixHierarchy[level],b,u);
    }

  return(0);
}



template <typename T>
int Multigrid<T>::ProjectedMultigridCycle(MultiVector<T>& b, MultiVector<T>& u, MultiVector<T>& c, const int level)
{
  int step;
  MultiVector<T> coarseResidual, fineResidual, coarseDefect, fineDefect, coarseProjection;

  if(level == 1)
    {
      directSolver.SetProjection(c);

      if(directSolver.SolveProjectedGaussSeidel(matrixHierarchy[1],b,u))
	{
	  cout << "Warning: No solution on coarse grid found!" << endl;
	  return(1);
	}
      else
	{
	  return(0);
	}
    }

  smoother[level].SetProjection(c);

  for(step=1;step<=preSmoothingSteps;step++)
    {
      smoother[level].Smoothen(matrixHierarchy[level],b,u);
    }

  fineResidual.Reset(u.Length());

  smoother[level].CalcResidual(matrixHierarchy[level],b,u,fineResidual);

  coarseResidual.Reset(nodeHierarchy[level].Length());
  coarseProjection.Reset(nodeHierarchy[level].Length());

  c -= u;

  //  prolongation[level].CreateRestrictionActive(matrixHierarchy[level], nodeHierarchy[level], c);

  prolongation[level].Inject(nodeHierarchy[level], coarseProjection, c);

  fineDefect.Reset(u.Length());
  fineDefect.SetZero();

  /* NEW
  prolongation[level].Restrict(matrixHierarchy[level], nodeHierarchy[level],
			       coarseResidual, fineResidual);
  */

  prolongation[level].Inject(nodeHierarchy[level], coarseResidual, fineResidual);

  double eps = 1e-10;
  int i, iMax;

  if(level==numberOfLevels)
    {
      for(i=1,iMax=0; i<=c.TotalNumberOfElements()&&fabs(c[i])<eps; i++){iMax = i;}
    }

  //  fineResidual.Delete();

  coarseDefect.Reset(coarseResidual.Length());
  coarseDefect.SetZero();

  for(step=1;step<=cycleType;step++)
    {
      ProjectedMultigridCycle(coarseResidual, coarseDefect, coarseProjection, level-1);
    }

  coarseResidual.Delete();

/* NEW */
  prolongation[level].Prolong(matrixHierarchy[level], nodeHierarchy[level], coarseDefect,
			fineDefect);

/*
  prolongation[level].ProlongActiveSets(matrixHierarchy[level], nodeHierarchy[level], c, fineResidual, coarseDefect,
					fineDefect);
*/
  fineResidual.Delete();

  if(level==numberOfLevels)
    {
      for(i=1,iMax=0; i<=fineDefect.TotalNumberOfElements()&&fabs(fineDefect[i])<eps; i++){iMax = i;}
    }

  u += fineDefect;
  
  coarseDefect.Delete(); fineDefect.Delete();

  for(step=1;step<=postSmoothingSteps;step++)
    {
      smoother[level].Smoothen(matrixHierarchy[level],b,u);
    }

  return(0);
}



template <typename T>
int Multigrid<T>::Solve(MultiVector<T>& b, MultiVector<T>& u)
{
  int n = 0;
  T newError = 1., error, iniError;
  const T epsilon2 = 1e-16;
  MultiVector<T> residual, defect;

  if(isFull)
    {
      residual.Reset(u.Length());
      defect.Reset(u.Length());
    }

 //   if(!isProjected)
    {
      newError = ErrorNorm(matrixHierarchy[numberOfLevels],b,u);
    }
   //else
   //  {
   //    newError = ProjectedErrorNorm(matrixHierarchy[numberOfLevels],b,u);
   //  }
#ifdef ModelP
  cout << me << ": ";
#endif
  cout << "Initial error: " << newError << endl;
  iniError = newError;

  while(!(newError<epsilon2 || newError<epsilon*iniError) && ++n<maxSteps)
    {
      if(isFull)
	{
	  if(isProjected)
	    {
	      CalcResidual(matrixHierarchy[numberOfLevels], b, u, residual);
	      FullProjectedMultigridCycle(residual, projection-u, defect);
	      u += defect;
	    }
	  else
	    {
	      CalcResidual(matrixHierarchy[numberOfLevels], b, u, residual);
	      FullMultigridCycle(residual, defect);
	      u += defect;
	    }
	}
      else if(isCascadic)
	{
	  if(isProjected)
	    {
	      CalcResidual(matrixHierarchy[numberOfLevels], b, u, residual);
	      //CascadicProjectedMultigridCycle(residual, projection-u, defect);
	      u += defect;
	    }
	  else
	    {
	      CalcResidual(matrixHierarchy[numberOfLevels], b, u, residual);
	      //CascadicMultigridCycle(residual, defect); CodeWarrior BUG
	      u += defect;
	    }
	}
      else
	{
	  MultigridCycle(b,u);
	}

      error = newError;
      
      if(isProjected)
	{
	  newError = ProjectedErrorNorm(matrixHierarchy[numberOfLevels],b,u);
	}
      else
	{
	  newError = ErrorNorm(matrixHierarchy[numberOfLevels],b,u);
	}

#ifdef ModelP
      cout << me << ": ";
#endif
      cout << "n = " << n << ": error: " << newError << ", multigrid rate: ";
      cout << newError/error << endl;
    }
  
  return(int(n==maxSteps));
}



template <typename T>
int Multigrid<T>::PrintHierarchy(ostream& o)
{
  o << matrixHierarchy[numberOfLevels].Length() << endl;
  for(int level=numberOfLevels; level>1;level--)
    {
      o << nodeHierarchy[level] << endl;
    }
}



#endif // MULTIGRID_HPP
