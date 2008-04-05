/* Prolongation - Template structure performing prolongation/restriction

Christoph Reisinger, 080102 - ? */

#ifndef PROLONGATION_HPP
#define PROLONGATION_HPP

#include "../datalib/sparsematrix.h"
#include "sortedlistarray.h"

enum LabellingTypes {RUGE_STUEBEN, REGULAR, GEOMETRIC};
enum InterpolationTypes {MATRIX_DEPENDENT_INTERPOLATION = 1, BILINEAR_INTERPOLATION};
enum RestrictionTypes {INJECTION=  0, MATRIX_DEPENDENT_RESTRICTION = 1, BILINEAR_RESTRICTION, ACTIVE_RESTRICTION};
enum TypeOfUnknowns {NODES, ELEMENTS};


template <typename T>
class Prolongation
{
private:
  bool saveProlongation;
  SparseMatrix<T> prolongation;

  bool seperateRestriction;
  bool saveRestriction;
  SparseMatrix<T> transposedRestriction;

  TypeOfUnknowns typeOfUnknowns;

  double alpha;
  Array<bool> isCoarseningDirection;

  LabellingTypes labellingType;
  InterpolationTypes interpolationType;
  RestrictionTypes restrictionType;

  int LabelRugeStueben(SparseMatrix<T>&, List<int>&);
  int LabelRegular(SparseMatrix<T>&, List<int>&);
  int LabelGeometric(SparseMatrix<T>&, List<int>&);
  int CreateProlongationMatrixDependent(SparseMatrix<T>&, List<int>&);
  int CreateProlongationBilinear(SparseMatrix<T>&, List<int>&);
  int CreateNodeProlongationBilinear(SparseMatrix<T>&, List<int>&);
  int CreateElementProlongationBilinear(SparseMatrix<T>&);
  int CreateRestrictionInjection(SparseMatrix<T>&, List<int>&);
  int ProlongMatrixDependent(SparseMatrix<T>&, List<int>&, MultiVector<T>&, MultiVector<T>&);
  int RestrictMatrixDependent(SparseMatrix<T>&, List<int>&, MultiVector<T>&, MultiVector<T>&);

  int RestrictMatrixFromProlongation(SparseMatrix<T>&, SparseMatrix<T>&);
  int RestrictMatrixFromProlongationAndRestriction(SparseMatrix<T>&, SparseMatrix<T>&);
  int RestrictMatrixMatrixDependent(SparseMatrix<T>&, List<int>&, SparseMatrix<T>&);

  int SpreadOut(const Array<int>&, const Array<int>&, const Array<int>&);
  int RecursiveSpreadOut(int, int, const Array<int>&, Array<int>, const Array<int>&, const Array<int>&);

public:
  Prolongation(){saveProlongation=0;seperateRestriction=0,saveRestriction=0;}
  ~Prolongation(){Delete();}
  int Delete(){prolongation.Delete(); isCoarseningDirection.Delete();return 0;}

  int SetAlpha(const double value){alpha = value;return 0;}
  int SetLabellingType(const LabellingTypes& type){labellingType = type;return 0;}
  int SetInterpolationType(const InterpolationTypes& type){interpolationType = type; return 0;}
  int SetRestrictionType(const RestrictionTypes& type){restrictionType = type; seperateRestriction=1;return 0;}
  int SaveMatrix(){saveProlongation=1;return 0;}
  int SetTypeOfUnknowns(const TypeOfUnknowns unknowns){typeOfUnknowns = unknowns;return 0;}

  int Label(SparseMatrix<T>&, List<int>&);

  int Inject(List<int>&, MultiVector<T>&, MultiVector<T>&);

  int CreateProlongation(SparseMatrix<T>&, List<int>&);
  int CreateRestriction(SparseMatrix<T>&, List<int>&);
  int CreateRestrictionActive(SparseMatrix<T>&, List<int>&, MultiVector<T>&);
  int CreateProjectedRestriction(SparseMatrix<T>&, List<int>&, MultiVector<T>&, MultiVector<T>&);

  int Prolong(SparseMatrix<T>&, List<int>&, MultiVector<T>&, MultiVector<T>&);
  int Restrict(SparseMatrix<T>&, List<int>&, MultiVector<T>&, MultiVector<T>&);

  int ProlongActiveSets(SparseMatrix<T>&, List<int>&, const MultiVector<T>&, const MultiVector<T>&, MultiVector<T>&, MultiVector<T>&);
  int RestrictActiveSets(SparseMatrix<T>&, List<int>&, MultiVector<T>&, const MultiVector<T>&, const MultiVector<T>&, MultiVector<T>&);
  
  int RestrictMatrix(SparseMatrix<T>&, List<int>&, SparseMatrix<T>&);
};




/***
Switches to different types
***/



template <typename T>
int Prolongation<T>::Label(SparseMatrix<T>& mat, List<int>& coarseGridNodes)
{
  isCoarseningDirection.Reset(mat.Dimension());
  isCoarseningDirection.SetOne();

  for(int i=1; i<=mat.Dimension();i++)
    {
      if(mat.Length(i)<3)
	{
	  isCoarseningDirection.SetElement(i,0);
	}
    }

  switch(labellingType)
    {
    case RUGE_STUEBEN:
      return(LabelRugeStueben(mat,coarseGridNodes));
    case REGULAR:
      return(LabelRegular(mat,coarseGridNodes));
    case GEOMETRIC:
      return(LabelGeometric(mat,coarseGridNodes));
    default:
      cout << "Prolongation: Unknown labelling type!" << endl;
      return(1);
    }
}



template <typename T>
int Prolongation<T>::CreateRestriction(SparseMatrix<T>& mat, List<int>& coarseGridNodes)
{
  if(seperateRestriction)
    {
      saveRestriction = 1;

      transposedRestriction.Reset(mat.Length());

      switch(restrictionType)
	{
	case INJECTION:
	  return(CreateRestrictionInjection(mat,coarseGridNodes));
 	case ACTIVE_RESTRICTION:
// 	  return(CreateRestrictionActive(mat,coarseGridNodes)); 
	  transposedRestriction.Reset(prolongation.Length());
	  transposedRestriction = prolongation;
	default:
	  cout << "CreateRestriction: Unknown restriction type!" << endl;
	  return(1);
	}
    }

    return 0;
}



template <typename T>
int Prolongation<T>::CreateProlongation(SparseMatrix<T>& mat, List<int>& coarseGridNodes)
{
  saveProlongation = 1;

  prolongation.Reset(mat.Length());

  switch(interpolationType)
    {
    case MATRIX_DEPENDENT_INTERPOLATION:
      return(CreateProlongationMatrixDependent(mat,coarseGridNodes));
    case BILINEAR_INTERPOLATION:
      return(CreateProlongationBilinear(mat,coarseGridNodes));
    default:
      cout << "CreateProlongation: Unknown prolongation type!" << endl;
      return(1);
    }
}



template <typename T>
int Prolongation<T>::Inject(List<int>& coarseGridNodes, MultiVector<T>& coarseGridData, MultiVector<T>& fineGridData)
{
  int node, index;

  coarseGridNodes.GoToHead();
  
  index = 1;

  do
    {
      coarseGridNodes.GetElement(node);
      coarseGridData[index] = fineGridData[node];

      index++;
    }
  while(coarseGridNodes.Move());

  return(0);
}



template <typename T>
int Prolongation<T>::Prolong(SparseMatrix<T>& mat, List<int>& coarseGridNodes, 
			     MultiVector<T>& coarseGridData, MultiVector<T>& fineGridData)
{
  if(saveProlongation)
    {
      fineGridData = prolongation*coarseGridData;

      return(0);
    }
  else
    {
      switch(interpolationType)
	{
	case MATRIX_DEPENDENT_INTERPOLATION:
	  return(ProlongMatrixDependent(mat,coarseGridNodes,coarseGridData,fineGridData));
	default:
	  cout << "Prolongation: Unknown prolongation type!" << endl;
	  return(1);
	}
    }
}



template <typename T>
int Prolongation<T>::Restrict(SparseMatrix<T>& mat, List<int>& coarseGridNodes,
			     MultiVector<T>& coarseGridData, MultiVector<T>& fineGridData)
{
  if(seperateRestriction)
    {
      if(saveProlongation)
	{
	  //	  cout << "before" << endl;
	  coarseGridData = fineGridData*transposedRestriction;
	  //	  cout << "after" << endl;
	  return(0);	  
	}
      else
	{
	  cout << "Not yet implemented!" << endl;
	  return(1);
	}
    }
  else
    // restriction as transposed interpolation
    {
      if(saveProlongation)
	{
	  coarseGridData = fineGridData*prolongation;
	  return(0);
	}
      else
	{
	  switch(interpolationType)
	    {
	    case MATRIX_DEPENDENT_INTERPOLATION:
	      return(RestrictMatrixDependent(mat,coarseGridNodes,coarseGridData,fineGridData));
	    default:
	      cout << "Prolongation: Unknown restriction type!" << endl;
	      return(1);
	    }
	}
    }
}



template <typename T>
int Prolongation<T>::ProlongActiveSets(SparseMatrix<T>& mat, List<int>& coarseGridNodes,
				       const MultiVector<T>& obstacle, const MultiVector<T>& residual,
				       MultiVector<T>& coarseGridData, MultiVector<T>& fineGridData)
{
  int i, j;
  int n = fineGridData.TotalNumberOfElements();
  const double eps = 1e-10;
  T p;

  if(saveProlongation)
    {
      for(i=1;i<=n;i++)
	{
	  fineGridData[i] = 0;

	  if(fabs(obstacle[i])>eps)
	    //	  if(!(residual[i]<-eps))
	    {
	      prolongation.GoToDiagonal(i);

	      do
		{
		  prolongation.GetElement(i,j,p);
		  fineGridData[i] += p*coarseGridData[j];
		}
	      while(prolongation.Move(i));
	    }
	}
    }
  else
    {
      cout << "Interpolation type not yet implemented!" << endl;
      exit(1);
    }

  return(0);
}



template <typename T>
int Prolongation<T>::RestrictActiveSets(SparseMatrix<T>& mat, List<int>& coarseGridNodes,
					MultiVector<T>& coarseGridData, const MultiVector<T>& obstacle,
					const MultiVector<T>& solution, MultiVector<T>& fineGridData)
{
  if(seperateRestriction)
    {
//       if(saveProlongation)
// 	{
// 	  coarseGridData = fineGridData*transposedRestriction ;
// 	  return(0);	  
// 	}
//       else
	{
	  cout << "Not yet implemented!" << endl;
	  exit(1);
	}
    }
  else
    // restriction as transposed interpolation
    {
      if(saveProlongation)
	{
	  prolongation.Transpose();

	  coarseGridData.SetZero();

	  int i, j, index, node;
	  int n = coarseGridData.TotalNumberOfElements();
	  T r, sum, totalSum;
	  const double eps = 1e-10;
	  bool iOnObstacle, jOnObstacle;
	  Array<int> fineIndex(n);

	  coarseGridNodes.GoToHead();
	  index = 1;

	  do
	    {
	      coarseGridNodes.GetElement(node);
	      fineIndex[index] = node;
	      index++;
	    }
	  while(coarseGridNodes.Move());


	  for(i=1;i<=n;i++)
	    {
	      prolongation.GoToDiagonal(i);

	      if(fabs(solution[fineIndex[i]]-obstacle[fineIndex[i]])<eps)
		{
		  iOnObstacle = 1;
		}
	      else
		{
		  iOnObstacle = 0; 
		}

	      sum = 0;
	      totalSum = 0;

	      do
		{
		  prolongation.GetElement(i,j,r);

		  if(fabs(solution[j]-obstacle[j])<eps)
		    {
		      jOnObstacle = 1;
		    }
		  else
		    {
		      jOnObstacle = 0;
		    }

		  if(iOnObstacle == jOnObstacle)
		    {
		      coarseGridData[i] += r*fineGridData[j];
		      sum += r;
		    }

		  totalSum += r;
		}
	      while(prolongation.Move(i));

	      if(fabs(sum)<eps)
		{
		  cout << "No appropriate neighbour for restriction! Can't be!!!" << endl;
		  exit(1);
		}

	      coarseGridData[i] *= totalSum/sum;
	    }

	  //	  coarseGridData = fineGridData*prolongation;

	  prolongation.Transpose();

	  return(0);
	}
      else
	{
	  cout << "Restriction type not yet implemented!" << endl;
	  exit(1);

	  switch(interpolationType)
	    {
	    case MATRIX_DEPENDENT_INTERPOLATION:
	      return(RestrictMatrixDependent(mat,coarseGridNodes,coarseGridData,fineGridData));
	    default:
	      cout << "Prolongation: Unknown restriction type!" << endl;
	      return(1);
	    }
	}
    }
}



template <typename T>
int Prolongation<T>::CreateProjectedRestriction(SparseMatrix<T>& mat, List<int>& coarseGridNodes,
				       MultiVector<T>& projection, MultiVector<T>& fineGridData)
{
  int length = mat.TotalNumberOfElements();
  int j, node, index;
  Array<int> isProjectedCoarseGridNode(length);
  T eps = 1e-10, a, sum;

  int count;

  coarseGridNodes.GoToHead();
  isProjectedCoarseGridNode.SetZero();
  
  index = 1;

  do
    {
      coarseGridNodes.GetElement(node);
      if(fabs(fineGridData[node]-projection[node])<eps)
	{
	  isProjectedCoarseGridNode.SetElement(node,index);
	}
//       else
// 	{
// 	  // coarse grid node, but not active
// 	  isProjectedCoarseGridNode.SetElement(node,-index);
// 	}
      index++;
    }
  while(coarseGridNodes.Move());

  transposedRestriction = prolongation;

  transposedRestriction.Transpose();

  for(node=1;node<=length;node++)
    {
      index=isProjectedCoarseGridNode[node];
      if(index>0)
	{
	  transposedRestriction.DeleteRow(index);
	  transposedRestriction.Add(index,node,1.);
	  count = -1;
	}
//       else if(index<0)
// 	{
// 	  index = -index;
// 	  transposedRestriction.GoToDiagonal(index);
// 	  sum = 0;
// 	  while(transposedRestriction.GetElement(index,j,a))
// 	    {
// 	      sum += a;
// 	      transposedRestriction.Move(index);
// 	    }

// 	  if(fabs(sum)>eps)
// 	    {
// 	      transposedRestriction.MultiplyRow(index,1./sum);
// 	    }
// 	  index = -index;
// 	}

//       //      cout << "node: " << node << ", index: " << index << ", count: " << count << endl;

//       if(count>0 && index<0)
// 	{
// 	  index = - index;
// 	  transposedRestriction.DeleteRow(index);
// 	  transposedRestriction.Add(index,node,1.);
// 	  count--;
// 	}
    }

//   cout << "projection: " << endl << projection << endl;
//   cout << "fineGridData: " << endl << fineGridData << endl;
//  cout << "restriction: " << endl << transposedRestriction << endl;

  transposedRestriction.Transpose();

  return(0);
}



template <typename T>
int Prolongation<T>::RestrictMatrix(SparseMatrix<T>& mat, List<int>& coarseGridNodes,
				    SparseMatrix<T>& coarseMatrix)
{
  // restriction as transposed interpolation, coarseMatrix = R mat P

  if(seperateRestriction && saveRestriction && saveProlongation)
    {
      return(RestrictMatrixFromProlongationAndRestriction(mat,coarseMatrix));
    }
  else if(!seperateRestriction && saveProlongation)
    {
      return(RestrictMatrixFromProlongation(mat,coarseMatrix));
    }
  else
    {
      switch(interpolationType)
	{
	case MATRIX_DEPENDENT_INTERPOLATION:
	  return(RestrictMatrixMatrixDependent(mat,coarseGridNodes,coarseMatrix));
	default:
	  cout << "Prolongation: Unknown type for matrix restriction!" << endl;
	  return(1);
	}
    }
}



/***
Labelling types
***/



template <typename T>
int Prolongation<T>::LabelRegular(SparseMatrix<T>& mat, List<int>& coarseGridNodes)
{
  Array<int> position(mat.Dimension());

  mat.InitPosition(position);

  do
    {
      coarseGridNodes.Append(mat.Index(position));
    }
  while(mat.NextPosition2(position));

  return(0);
}



template <typename T>
int Prolongation<T>::LabelGeometric(SparseMatrix<T>& mat, List<int>& coarseGridNodes)
{
  int i, maxLength, dim = mat.Dimension();
  Array<int> position(dim), step(dim), length(dim);

  int span = 0;

  length = mat.Length();

  maxLength = 0;
  for(i=1;i<=dim;i++)
    {
      if(length[i]>maxLength)
	{
	  maxLength = length[i];
	}
    }

  for(i=1;i<=dim;i++)
    {
      if(length[i]>=maxLength-span && length[i]>2)
	{
	  step.SetElement(i,2);
	  isCoarseningDirection.SetElement(i,1);
	}
      else
	{
	  step.SetElement(i,1);
	  isCoarseningDirection.SetElement(i,0);
	}
    }


  mat.InitPosition(position);

  do
    {
      coarseGridNodes.Append(mat.Index(position));
    }
  while(mat.NextPosition(position, step));

  return(0);
}



template <typename T>
int Prolongation<T>::LabelRugeStueben(SparseMatrix<T>& mat, List<int>& coarseGridNodes)
{
  int dim = mat.Dimension(), length = mat.TotalNumberOfElements(),

  numberOfWeights = 2*(int(pow(3,dim))-1);

  int i, j, k, weight;
  T a, max;

  Array<bool> isCoarseGridNode(length), isFineGridNode(length);

  SparseMatrix<T> connectivity, transposedConnectivity;
  SortedListArray unusedNodes;

  connectivity.Reset(mat.Length());
  transposedConnectivity.Reset(mat.Length());

  // build strong connectivity graph and transposed graph

  for(i=1;i<=length;i++)
    {
      mat.GoToDiagonal(i);
      max = 0;

      while(mat.Move(i))
	{
	  mat.GetElement(i,j,a);

	  if(-a>max)
	    {
	      max = -a;
	    }
	}
      
      mat.GoToDiagonal(i);

      while(mat.Move(i))
	{
	  mat.GetElement(i,j,a);

	  if(-a/max>=alpha)
	    {
	      connectivity.Insert(i,j,-a/max);
	      transposedConnectivity.Insert(j,i,-a/max);
	    }
	}
    }

  // set up unused nodes and initialise weight function

  unusedNodes.Reset(length,numberOfWeights);

  for(i=1;i<=length;i++)
    {
      unusedNodes.InsertSorted(i, transposedConnectivity[i].Length()+1);
    }

  isCoarseGridNode.SetZero();
  isFineGridNode.SetZero();

  while(i = unusedNodes.Tail())
    {
      // choose node with maximum weight as coarse grid node...

      isCoarseGridNode.SetElement(i,1);

      // ... and remove it from list of available nodes

      unusedNodes.CutTail();
      
      // all (unused) strong neighbours are set fine grid nodes

      transposedConnectivity.GoToDiagonal(i);

      while(transposedConnectivity.GetElement(i,j,a))
      {
	if(unusedNodes.IsIn(j))
	  {
	    isFineGridNode.SetElement(j,1);
	  }
	
	transposedConnectivity.Move(i);	
      }

      // update weights of unused nodes
      
      transposedConnectivity.GoToDiagonal(i);
      
      while(transposedConnectivity.GetElement(i,j,a))
	{
	  if(unusedNodes.IsIn(j))
	    {
	      // remove new fine grid node from list of unused nodes
	      unusedNodes.Remove(j);
	      
	      connectivity.GoToDiagonal(j);
	      
	      while(connectivity.GetElement(j,k,a))
		{
		  if(unusedNodes.IsIn(k))
		    {
		      weight = unusedNodes.Weight(k);
		      unusedNodes.Remove(k);
		      unusedNodes.InsertSorted(k,weight+1);		     
		    }
		  connectivity.Move(j);
		}
	    }
	  
	  transposedConnectivity.Move(i);
	}
    }

  for(i=1;i<=length;i++)
    {
      if(isCoarseGridNode[i])
	{
	  coarseGridNodes.Append(i);
	}
    }

  return(0);
}



/***
Prolongation types
***/



template <typename T>
int Prolongation<T>::CreateProlongationMatrixDependent(SparseMatrix<T>& mat,
						       List<int>& coarseGridNodes)
{
  int i, j;
  int length = mat.TotalNumberOfElements();
  int node, index;
  Array<int> isCoarseGridNode(length);
  T denominator, a;

  coarseGridNodes.GoToHead();
  isCoarseGridNode.SetZero();
  
  index = 1;

  do
    {
      coarseGridNodes.GetElement(node);
      isCoarseGridNode.SetElement(node,index);
      index++;
    }
  while(coarseGridNodes.Move());

  for(i=1;i<=length;i++)
    {
      if(isCoarseGridNode[i])
	{
	  prolongation.Insert(i,isCoarseGridNode[i],1);
	}
      else
	{
	  denominator = 0;
	  mat.GoToDiagonal(i);

	  while(mat.Move(i))
	    {
	      mat.GetElement(i,j,a);

	      a = fabs(a);

	      if(isCoarseGridNode[j])
		{
		  prolongation.Insert(i,isCoarseGridNode[j],a);
		  denominator += a;
		}
	    }

	  if(fabs(denominator)<1e-16)
	    {
	      //cout << mat << endl;
	      cout << "node " << i <<": " << mat.Position(i);
	      cout << "Prolongation: Interpolation of node impossible!" << endl;
	      exit(1);
	    }

	  prolongation.MultiplyRow(i,1./denominator);
	}
    }

  return(0);
}



template <typename T>
int Prolongation<T>::SpreadOut(const Array<int>& coordinate, const Array<int>& dimensions,
			       const Array<int>& isCoarseGridNode)
{
  int one=1, zero=0;
  Array<int> start(coordinate);

  RecursiveSpreadOut(one, zero, coordinate, start, dimensions, isCoarseGridNode);
  return 0;
}



template <typename T>
int Prolongation<T>::RecursiveSpreadOut(int position, int counter, const Array<int>& coordinate, 
					Array<int> newCoordinate, const Array<int>& dimensions, 
					const Array<int>& isCoarseGridNode)
{
  int newPosition = position, dim = coordinate.Length();

  //  while(newPosition<=dim && newCoordinate[newPosition]%2)
  while(newPosition<=dim && (newCoordinate[newPosition]%2 || !isCoarseningDirection[newPosition]))
    {
      ++newPosition;
    }

  if(newPosition > dim)
    {
      if(!counter)
	// i. e. coarse grid node
	{
	  prolongation.Insert(prolongation.Index(coordinate),
			      isCoarseGridNode[prolongation.Index(coordinate)],1);
	}
      else
	{
	  prolongation.Insert(prolongation.Index(coordinate),
			      isCoarseGridNode[prolongation.Index(newCoordinate)],1.);
	}
    }
  else
    {
      if(newCoordinate[newPosition]<dimensions[newPosition])
	{
	  newCoordinate.Add(newPosition,1);
	  ++counter;	  
	  RecursiveSpreadOut(newPosition, counter, coordinate, newCoordinate, dimensions, 
			     isCoarseGridNode);
	  newCoordinate.Add(newPosition,-1);
	}
      
      if(newCoordinate[newPosition]>1)
	{
	  newCoordinate.Add(newPosition,-1);
	  ++counter;
	  RecursiveSpreadOut(newPosition, counter, coordinate, newCoordinate, dimensions, 
			     isCoarseGridNode);
	  newCoordinate.Add(newPosition,1);
	}
    }

  return(0);
}



template <typename T>
int Prolongation<T>::CreateProlongationBilinear(SparseMatrix<T>& mat,
						List<int>& coarseGridNodes)
{
  switch(typeOfUnknowns)
    {
    case NODES:
      return(CreateNodeProlongationBilinear(mat, coarseGridNodes));
    case ELEMENTS:
      return(CreateElementProlongationBilinear(mat));
    default:
      cout << "Prolongation: Unknown type of variables!" << endl;
      return(1);
    }
}



template <typename T>
int Prolongation<T>::CreateElementProlongationBilinear(SparseMatrix<T>& mat)
{
  // still missing

  return(0);
}



template <typename T>
int Prolongation<T>::CreateNodeProlongationBilinear(SparseMatrix<T>& mat,
						List<int>& coarseGridNodes)
{
  //int i, j unused
    int node, index;
  Array<int> position(mat.Dimension());
  Array<int> isCoarseGridNode(mat.TotalNumberOfElements());
  Array<int> length(mat.Length());

  coarseGridNodes.GoToHead();
  isCoarseGridNode.SetZero();
  
  index = 1;

  do
    {
      coarseGridNodes.GetElement(node);
      isCoarseGridNode.SetElement(node,index);
      index++;
    }
  while(coarseGridNodes.Move());

  mat.InitPosition(position);

  do
    {
      SpreadOut(position, length, isCoarseGridNode);
      
      index = prolongation.Index(position);
      prolongation.MultiplyRow(index,1./double(prolongation.Bandwidth(index)));
    }
  while(mat.NextPosition(position));

  return(0);
}



template <typename T>
int Prolongation<T>::ProlongMatrixDependent(SparseMatrix<T>& mat, List<int>& coarseGridNodes, 
			     MultiVector<T>& coarseGridData, MultiVector<T>& fineGridData)
  // WARNING: make sure that coarse grid data are numbered according to coarseGridNodes
{
  int i, j;
  int length = fineGridData.TotalNumberOfElements();
  int node, index;
  Array<int> isCoarseGridNode(length);
  T nominator, denominator, a;

  coarseGridNodes.GoToHead();
  isCoarseGridNode.SetZero();
  
  index = 1;

  do
    {
      coarseGridNodes.GetElement(node);
      isCoarseGridNode.SetElement(node,index);
      index++;
    }while(coarseGridNodes.Move());

  for(i=1;i<=length;i++)
    {
      if(isCoarseGridNode[i])
	{
	  fineGridData.Set(i,coarseGridData[isCoarseGridNode[i]]);
	}
      else
	{
	  nominator = denominator = 0;
	  mat.GoToDiagonal(i);

	  while(mat.Move(i))
	    {
	      mat.GetElement(i,j,a);

	      if(isCoarseGridNode[j])
		{
		  nominator += coarseGridData[isCoarseGridNode[j]]*a;
		  denominator += a;
		}
	    }

	  if(fabs(denominator)<1e-16)
	    {
	      cout << "Prolongation: Interpolation of node impossible!" << endl;
	      return(1);
	    }

	  fineGridData.Set(i,nominator/denominator);
	}
    }

  return(0);
}



/***
Restriction types
***/


template <typename T>
int Prolongation<T>::CreateRestrictionInjection(SparseMatrix<T>& mat,
						List<int>& coarseGridNodes)
{
  int node, index, n = mat.TotalNumberOfElements();
  Array<int> isCoarseGridNode(mat.TotalNumberOfElements());

  coarseGridNodes.GoToHead();
  isCoarseGridNode.SetZero();
  
  index = 1;

  do
    {
      coarseGridNodes.GetElement(node);
      isCoarseGridNode.SetElement(node,index);
      index++;
    }
  while(coarseGridNodes.Move());

  for(node=1; node<=n; node++)
    {
      if(isCoarseGridNode[node])
	{
	  transposedRestriction.Add(node,isCoarseGridNode[node],1.);
	}
    }

  return(0);
}



template <typename T>
int Prolongation<T>::CreateRestrictionActive(SparseMatrix<T>& mat, List<int>& coarseGridNodes, MultiVector<T>& projection)
{
  int node, index, n = mat.TotalNumberOfElements();
  Array<int> isCoarseGridNode(mat.TotalNumberOfElements());
  const double eps = 1e-10;

  coarseGridNodes.GoToHead();
  isCoarseGridNode.SetZero();
  
  index = 1;

  do
    {
      coarseGridNodes.GetElement(node);
      isCoarseGridNode.SetElement(node,index);
      index++;
    }
  while(coarseGridNodes.Move());

  transposedRestriction = prolongation;


  for(node=1; node<=n && fabs(projection[node])<eps; node++);

  if(isCoarseGridNode[node])
    {
      cout << "coarse grid node nr. " << isCoarseGridNode[node] << "(" << (mat.TotalNumberOfElements()-1)/2+1 << ")" << endl;
      transposedRestriction.DeleteRow(node-1);
      transposedRestriction.Add(node-1,isCoarseGridNode[node]-2,0.5);
      transposedRestriction.Add(node-1,isCoarseGridNode[node]-1,1.5);
    }
  else
    {
      cout << "next coarse grid node nr. " << isCoarseGridNode[node+1] << "(" << (mat.TotalNumberOfElements()-1)/2+1 << ")" << endl;
      transposedRestriction.DeleteRow(node);
      transposedRestriction.Add(node,isCoarseGridNode[node+1],1.5);
      transposedRestriction.Add(node,isCoarseGridNode[node+1]+1,-0.5);
    }
  
  //  transposedRestriction = prolongation;

  //  prolongation = transposedRestriction;

  return(0);
}





/* WHAT A MESS!!! */




template <typename T>
int Prolongation<T>::RestrictMatrixFromProlongation(SparseMatrix<T>& mat, 
						    SparseMatrix<T>& coarseMatrix)
{
  int length = mat.TotalNumberOfElements();
  //int node, index, unused!!!
    int i, j, k, l;	
  T a, alpha, beta;
  
  int dim = mat.Dimension();
  Array<int> dimensions(dim);

  if(labellingType == REGULAR || labellingType == GEOMETRIC)
    {
      for(i=1;i<=dim;i++)
	{
	  if(isCoarseningDirection[i])
	    {
	      dimensions.SetElement(i,(mat.Length(i)-1)/2+1);
	    }
	  else
	    {
	      dimensions.SetElement(i,mat.Length(i));
	    }
	}

      coarseMatrix.Reset(dimensions);
    }
  else
    {
      coarseMatrix.Reset(prolongation.NumberOfColumns());
    }

  //  cout << coarseMatrix.Length();


  for(k=1;k<=length;k++)
    {
      mat.GoToDiagonal(k);
      
      while(mat.GetElement(k,l,a))
	{
	  prolongation.GoToDiagonal(k);

	  while(prolongation.GetElement(k,i,alpha))
	    {
	      prolongation.GoToDiagonal(l);

	      while(prolongation.GetElement(l,j,beta))
		{
		  coarseMatrix.Add(i,j,alpha*a*beta);

		  prolongation.Move(l);
		}

	      if(l==k)
		// pointer has been moved to wrong position in last loop -> return it
		{
		  prolongation.GoToDiagonal(l);

		  prolongation.GetElement(l,j,beta);

		  while(j!=i)
		    {
		      prolongation.Move(l);
		      prolongation.GetElement(l,j,beta);
		    }
		}

	      prolongation.Move(k);
	    }
	  mat.Move(k);
	}
    }

  return(0);
}



template <typename T>
int Prolongation<T>::RestrictMatrixFromProlongationAndRestriction(SparseMatrix<T>& mat, 
								  SparseMatrix<T>& coarseMatrix)
{
  int length = mat.TotalNumberOfElements();
  //  int node, index; unused!
      int i, j, k, l;
  T a, alpha, beta;
  
  int dim = mat.Dimension();
  Array<int> dimensions(dim);

  if(labellingType == REGULAR || labellingType == GEOMETRIC)
    {
      for(i=1;i<=dim;i++)
	{
	  if(isCoarseningDirection[i])
	    {
	      dimensions.SetElement(i,(mat.Length(i)-1)/2+1);
	    }
	  else
	    {
	      dimensions.SetElement(i,mat.Length(i));
	    }
	}

      coarseMatrix.Reset(dimensions);
    }
  else
    {
      coarseMatrix.Reset(prolongation.NumberOfColumns());
    }

  //  cout << coarseMatrix.Length();


  for(k=1;k<=length;k++)
    {
      mat.GoToDiagonal(k);
      
      while(mat.GetElement(k,l,a))
	{
	  transposedRestriction.GoToDiagonal(k);

	  while(transposedRestriction.GetElement(k,i,alpha))
	    {
	      prolongation.GoToDiagonal(l);

	      while(prolongation.GetElement(l,j,beta))
		{
		  coarseMatrix.Add(i,j,alpha*a*beta);

		  prolongation.Move(l);
		}

	      transposedRestriction.Move(k);
	    }
	  mat.Move(k);
	}
    }

  return(0);
}



template <typename T>
int Prolongation<T>::RestrictMatrixDependent(SparseMatrix<T>& mat, List<int>& coarseGridNodes, 
					     MultiVector<T>& coarseGridData, MultiVector<T>& fineGridData)
  // IMPORTANT: coarse grid data are numbered according to coarseGridNodes
{
  int length = fineGridData.TotalNumberOfElements();
  int node, index, j;
  Array<int> isCoarseGridNode(length);
  T a, denominator;

  coarseGridNodes.GoToHead();

  index = 1;
  isCoarseGridNode.SetZero();
  coarseGridData.SetZero();

  do
    {
      coarseGridNodes.GetElement(node);
      isCoarseGridNode.SetElement(node,index);
      index++;
    }while(coarseGridNodes.Move());

  for(int i=1;i<=length;i++)
    {
      denominator = 0;

      if(isCoarseGridNode[i])
	{
	  coarseGridData.Add(isCoarseGridNode[i], fineGridData[i]);
	}
      else
	{
	  mat.GoToDiagonal(i);

	  while(mat.Move(i))
	    {
	      mat.GetElement(i,j,a);

	      if(isCoarseGridNode[j])
		{
		  denominator += a;
		}
	    }

	  if(isCoarseGridNode[i] && fabs(denominator)<1e-16)
	    {
	      cout << "Restriction: Restriction of node impossible!" << endl;
	      return(1);
	    }

	  mat.GoToDiagonal(i);

	  while(mat.Move(i))
	    {
	      mat.GetElement(i,j,a);

	      if(isCoarseGridNode[j])
		{
		  coarseGridData.Add(isCoarseGridNode[j],fineGridData[i]*a/denominator);
		}
	    }
	}
    }

  return(0);
}



/***
Types for matrix restriction
***/



template <typename T>
int Prolongation<T>::RestrictMatrixMatrixDependent(SparseMatrix<T>& mat,
						   List<int>& coarseGridNodes,
						   SparseMatrix<T>& coarseMatrix)
{
  int length = mat.TotalNumberOfElements();
  int i, j, k, l, index, node;
  Array<T> denominator(length);
  SparseMatrixEntry<T> entry, entryK, entryL;
  List<SparseMatrixEntry<T> > listK, listL;
  Array<int> isCoarseGridNode(length);
  T a, sum;

  coarseGridNodes.GoToHead();
  isCoarseGridNode.SetZero();
  index = 1;

  while(coarseGridNodes.GetElement(node))
    {
      isCoarseGridNode.SetElement(node,index);
      index++;
      coarseGridNodes.Move();
    }
  
  denominator.SetZero();

  for(i=1;i<=length;i++)
    {
      sum = 0;
      if(!isCoarseGridNode[i])
	{
	  mat.GoToDiagonal(i);

	  mat.Move(i);

	  while(mat.GetElement(i,j,a))
	    {
	      if(isCoarseGridNode[j])
		{
		  sum += a;
		}
	      mat.Move(i);
	    }
	  denominator.SetElement(i,sum);
	}
    }


  coarseMatrix.Reset(coarseGridNodes.Length());

  for(k=1; k<=length; k++)
    {
      // find k's neighbours

      listK.Delete();
      mat.GoToDiagonal(k);

      while(mat.GetElement(k,entry.id,entry.data))
	{
	  if(isCoarseGridNode[entry.id])
	    {
	      listK.Append(entry);
	    }

	  mat.Move(k);
	}

      mat.GoToDiagonal(k);

      while(mat.GetElement(k,l,a))
	{
	  // find l's neighbours

	  mat.GoToDiagonal(l);

	  listL.Delete();

	  while(mat.GetElement(l,entry.id,entry.data))
	    {
	      if(isCoarseGridNode[entry.id])
		{
		  listL.Append(entry);
		}

	      mat.Move(l);
	    }

	  if(l==k)
	    {
	      mat.GoToDiagonal(k);
	    }

	  listK.GoToHead();

	  while(listK.GetElement(entryK))
	    {
	      i = entryK.id;

	      listL.GoToHead();	     

	      while(listL.GetElement(entryL))
		{
		  j = entryL.id;

		  if(i==k && j==l)
		    {
		      coarseMatrix.Add(isCoarseGridNode[i], isCoarseGridNode[j], a);
		    }
		  else if(i==k)
		    {
		      coarseMatrix.Add(isCoarseGridNode[i], isCoarseGridNode[j], 
				       a*(entryL.data/denominator[l]));
		    }
		  else if(j==l)
		    {
		      coarseMatrix.Add(isCoarseGridNode[i], isCoarseGridNode[j], 
				       a*(entryK.data/denominator[k]));
		    }
		  else
		    {
		      coarseMatrix.Add(isCoarseGridNode[i], isCoarseGridNode[j], 
				       a*(entryK.data/denominator[k])*
				       (entryL.data/denominator[l]));
		    }

		  listL.Move();
		}

	      listK.Move();
	    }

	  mat.Move(k);
	}
    }

  return 0;
}





#endif // PROLONGATION_HPP
