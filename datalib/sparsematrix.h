/* SparseMatrix2 - Template data structure of type sparse matrix using arrays

Christoph Reisinger, 270303 - ? */

#ifndef SPARSEMATRIX_HPP
#define SPARSEMATRIX_HPP

#include "../mathlib/multiarray.h"
#include "../mathlib/list.h"
#include "../mathlib/sparsematrixentry.h"

template <typename T>
struct Line{
    Array<T> data;
    Array<int> id;
    int length;
    int pos;

    int Length(){return(length);}
    int Reset(const int len){data.Reset(len);id.Reset(len);length=0;return(0);}
    int Insert(const int col, const T entry){data[length+1]=entry;id[length+1]=col;length++;return(0);}
//    int InsertDiag(const int col, const T entry){data[1]=entry;id[1]=col;length=1;}
    inline int Add(const int col, const T entry);
    inline int Prepend(const int col, const T entry);
    inline int PreAdd(const int col, const T entry);
    inline T Multiply(const MultiVector<T>& u);
    inline int GetElement(int& col, T& entry);
    inline int Move(){if(++pos>length){return(0);}else{return(1);}}
    inline int GoToHead(){pos=1; return(0);}
    inline int Multiply(const T factor);
    ostream & Print(ostream &o) const;
    inline int SetZero();
    int Delete(){data.Delete();id.Delete();length=0;pos=0;return(0);}
};



template <typename T>
ostream & operator<< (ostream& o, const Line<T>& line)
{
    return(line.Print(o));
}



template <typename T>
ostream & Line<T>::Print(ostream &o) const
{
  o << length << endl;
  o << id;
  o << data << endl;

  return(o);
}



template <typename T>
inline int Line<T>::GetElement(int& col, T& entry)
{
    if(pos>0 && pos<=length)
    {
	col = id[pos];
	entry = data[pos];
	return(1);
    }
    else
    {
	return(0);
    }
}



template <typename T>
inline int Line<T>::Add(const int col, const T entry)
{
    int i;

    for(i=1;i<=length;i++)
    {
	if(col==id[i])
	{
	    data[i] += entry;
	    return(0);
	}
    }

    length++;
    data[length] = entry;
    id[length] = col;

    return(0);
}



template <typename T>
inline int Line<T>::Prepend(const int col, const T entry)
{
    int i;

    if(length)
    {
	for(i=2;i<=length;i++)
	{
	    if(id[1]==id[i])
	    {
		data[i] += data[1];

		data[1] = entry;
		id[1] = col;
		return(0);
	    }
	}

	length++;
	data[length] = data[1];
	id[length] = id[1];
    }
    else
    {
	length = 1;
    }

    data[1] = entry;
    id[1] = col;

    return(0);
}



template <typename T>
inline int Line<T>::PreAdd(const int col, const T entry)
{
    int i;

    if(length)
    {
	if(id[1]==col)
	{
	    data[1] += entry;
	}
	else
	{
	    for(i=2;i<=length;i++)
	    {
		if(id[1]==id[i])
		{
		    data[i] += data[1];

		    data[1] = entry;
		    id[1] = col;
		    return(0);
		}
	    }

	    length++;
	    data[length] = data[1];
	    id[length] = id[1];
	    data[1] = entry;
	    id[1] = col;
	}
    }
    else
    {
	length = 1;
	data[1] = entry;
	id[1] = col;
    }

    return(0);
}



template <typename T>
inline T Line<T>::Multiply(const MultiVector<T>& u)
{
    int i;
    T p=0;

    for(i=1;i<=length;i++)
    {
	p += data[i]*u[id[i]];
    }
    return(p);
}



template <typename T>
inline int Line<T>::Multiply(const T factor)
{
    int i;
    for(i=1;i<=length;i++)
    {
	data[i] *= factor;
    }

    return(0);
}



template <typename T>
inline int Line<T>::SetZero()
{
    length = 0;

    return(0);
}





template <typename T>
class SparseMatrix : public MultiArray<Line<T> >
{
private:
  int numberOfColumns;

public:
  SparseMatrix();
  SparseMatrix(const Array<int>&);
  SparseMatrix(const Array<int>&, const int);
  int Reset(const Array<int>&);
  int Reset(const Array<int>&, const int);
//  ~SparseMatrix(){Delete();}
  int Delete();
  int Bandwidth(const int i) const {return(elements[i].length);return(0);}
  int NumberOfColumns(){return(numberOfColumns);return(0);}
  inline int Insert(const Array<int>&, const Array<int>&, const T&);
  inline int Insert(const int, const int, const T&);
  inline int Add(const int, const int, const T&);
  inline int Add(const Array<int>& row, const Array<int>& column, const T& value);
  inline int AddIdentity();
  inline int AddMultipleIdentity(const T factor);
  inline int GetElement(const int, int&, T&);
  inline int GetElement(const Array<int>&, Array<int>&, T&);
  inline int Move(const int);
  inline int Move(const Array<int>&);
  inline int GoToDiagonal(const int);
  inline int GoToDiagonal(const Array<int>&);

  inline  int Multiply(const MultiVector<T>&, MultiVector<T>&) const;
  inline int MultiplyTransposed(const MultiVector<T>&, MultiVector<T>&) const;

  inline int MultiplyRow(const int, const T&);
  inline int Multiply(const T&);
  inline int DeleteRow(const Array<int>&);
  inline int DeleteRow(const int);
  inline int SetZero();

  inline int Transpose();

  //  SparseMatrix<T> operator= (SparseMatrix<T>&);
    inline SparseMatrix<T> operator+ (SparseMatrix<T>&);
    inline SparseMatrix<T> operator* (const T&);
  //  T & operator[] (const int&, const int&) const;
};





template <typename T>
SparseMatrix<T>::SparseMatrix()
{
  elements.Reset(0);
  length.Reset(0);
  totalLength=0;
  numberOfColumns=0;
}



template <typename T>
SparseMatrix<T>::SparseMatrix(const Array<int>& newLength)
{
    Reset(newLength);
}



template <typename T>
SparseMatrix<T>::SparseMatrix(const Array<int>& newLength, const int len)
{
    Reset(newLength, len);
}



template <typename T>
int SparseMatrix<T>::Reset(const Array<int>& newLength)
{
    int i, len, d = newLength.Length();

    len = 1;

    for(i=1;i<=d;i++)
    {
	len *= 3;
    }

    Reset(newLength, len);
    return 0;
}



template <typename T>
int SparseMatrix<T>::Reset(const Array<int>& newLength, const int len)
{
    int i;
    dimension = newLength.Length();
    length.Reset(dimension);
    length = newLength;

    totalLength = length[1];

    for(i=2;i<=dimension;i++)
    {
	totalLength *= length[i];
    }

    elements.Reset(totalLength);

    for(i=1;i<=totalLength;i++)
    {
	elements[i].Reset(len);
    }

//    cout << elements << endl;

    numberOfColumns = 0;
    return 0;
}



template <typename T>
int SparseMatrix<T>::Delete()
{
  int i;

  for(i=1;i<=totalLength;i++)
  {
	elements[i].Delete();
  }

//  elements.Delete();
  return 0;
}



template <typename T>
inline int SparseMatrix<T>::Insert(const Array<int>& row, const Array<int>& column, const T& value)
{
    int col = Index(column), r = Index(row);
    if(col>numberOfColumns)
    {
	numberOfColumns = col;
    }

    if(col==r)
    {
	return(elements[r].Prepend(col,value));
    }
    else
    {
	return(elements[r].Insert(col,value));
    }
}



template <typename T>
inline int SparseMatrix<T>::Insert(const int row, const int column, const T& value)
{
    if(column>numberOfColumns)
    {
	numberOfColumns = column;
    }

    if(column==row)
    {
	return(elements[row].Prepend(column,value));
    }
    else
    {
	return(elements[row].Insert(column,value));
    }
}



template <typename T>
inline int SparseMatrix<T>::Add(const int row, const int column, const T& value)
{
    if(column>numberOfColumns)
    {
	numberOfColumns = column;
    }

    if(column==row)
    {
	return(elements[row].PreAdd(column,value));
    }
    else
    {
	return(elements[row].Add(column,value));
    }
}



template <typename T>
inline int SparseMatrix<T>::Add(const Array<int>& row, const Array<int>& column, const T& value)
{
    int col = Index(column), r=Index(row);
    if(col>numberOfColumns)
    {
	numberOfColumns = col;
    }

    if(col==r)
    {
	return(elements[r].PreAdd(col,value));
    }
    else
    {
	return(elements[r].Add(col,value));
    }
}



template <typename T>
inline int SparseMatrix<T>::AddIdentity()
{
  int i;

  for(i=1; i<=totalLength; i++)
    {
	elements[i].Add(i,1);
    }

  return(0);
}



template <typename T>
inline int SparseMatrix<T>::AddMultipleIdentity(const T factor)
{
  int i;

  for(i=1; i<=totalLength; i++)
    {
	elements[i].Add(i,factor);
    }

  return(0);
}



/*
template <typename T>
ostream & operator<< (ostream &o, const SparseMatrixEntry<T> & entry)
{
  o << entry.id << " " << entry.data;
}
*/


template <typename T>
inline int SparseMatrix<T>::Multiply(const MultiVector<T> & u, MultiVector<T> & v) const
{
    //int p,
    int i;

    for(i=1;i<=totalLength;i++)
    {
	v[i] = elements[i].Multiply(u);
    }
    return(0);
}



template <typename T>
inline int SparseMatrix<T>::MultiplyTransposed(const MultiVector<T> & u,
					MultiVector<T> & v) const
{
    int i, id;
    T data;
    v.SetZero();

    for(i=1;i<=totalLength;i++)
    {
	elements[i].GoToHead();

	while(elements[i].GetElement(id,data))
	{
	    v.Add(id,data*u[i]);
	    elements[i].Move();
	}
    }
    return(0);
}



template <typename T>
inline int SparseMatrix<T>::GetElement(const int row, int & column, T& element)
{
    if(elements[row].pos>elements[row].length)
    {
	return(0);
    }
    else
    {
	column = elements[row].id[elements[row].pos];
	element = elements[row].data[elements[row].pos];

	return(1);
    }

//    return(elements[row].GetElement(column,element));
}



template <typename T>
inline int SparseMatrix<T>::GetElement(const Array<int>& row, Array<int>& column, T& element)
{
    int srow = Index(row);

    if(elements[srow].pos>elements[srow].length)
    {
	return(0);
    }
    else
    {
	column = Position(elements[srow].id[elements[srow].pos]);
	element = elements[srow].data[elements[srow].pos];

	return(1);
    }

//    return(Position(elements[Index(row)].GetElement(Index(column), element)));
}




template <typename T>
inline int SparseMatrix<T>::Move(const int row)
{
    if(++elements[row].pos>elements[row].length){return(0);}else{return(1);}
//  return(elements[row].Move());
}



template <typename T>
inline int SparseMatrix<T>::Move(const Array<int>& row)
{
    int r = Index(row);
    if(++elements[r].pos>elements[r].length){return(0);}else{return(1);}
//  return(elements[Index(row)].Move());
}



template <typename T>
inline int SparseMatrix<T>::GoToDiagonal(const int row)
{
    elements[row].pos = 1;
//  return(elements[row].GoToHead());
    return 0;
}



template <typename T>
inline int SparseMatrix<T>::GoToDiagonal(const Array<int>& row)
{
    elements[Index(row)].pos = 1;
//  return(elements[Index(row)].GoToHead());
}



template <typename T>
inline int SparseMatrix<T>::MultiplyRow(const int row, const T& factor)
{
    elements[row].Multiply(factor);
    return 0;
}



template <typename T>
inline int SparseMatrix<T>::Multiply(const T& factor)
{
  int i;

  for(i=1; i<=totalLength; i++)
    {
	elements[i].Multiply(factor);
    }

  return(0);
}



template <typename T>
inline int SparseMatrix<T>::DeleteRow(const int row)
{
    return(elements[row].SetZero());
//  return(elements[row].Delete());
}



template <typename T>
inline int SparseMatrix<T>::DeleteRow(const Array<int>& row)
{
    return(elements[Index(row)].SetZero());
//  return(elements[Index(row)].Delete());
}



template <typename T>
inline int SparseMatrix<T>::SetZero()
{
  int row;

  for(row=1; row<=totalLength; row++)
  {
      elements[row].SetZero();
  }
  numberOfColumns = 0;

  return(0);
}



template <typename T>
inline int SparseMatrix<T>::Transpose()
{
  SparseMatrix<T> mat(length);
  int i, j;
  T a;

  for(i=1;i<=totalLength;i++)
    {
      GoToDiagonal(i);

      while(GetElement(i,j,a))
	{
	  mat.Add(j,i,a);
	  Move(i);
	}
    }

  *this = mat;

  return(0);
}



template <typename T>
inline SparseMatrix<T> SparseMatrix<T>::operator+ (SparseMatrix<T>& matrix)
{
  SparseMatrix<T> mat(length);
  int i, j;
  T a;

  mat = matrix;

  for(i=1;i<=totalLength;i++)
    {
      GoToDiagonal(i);

      while(GetElement(i,j,a))
	{
	  mat.Add(i,j,a);
	  matrix.Move(i);
	}
    }

  return(mat);
}




/***
    related non-member functions
***/



template <typename T>
inline SparseMatrix<T> IdentityMatrix(const Array<int>& length)
{
  SparseMatrix<T> matrix(length);
  int i;

  for(i=1; i<=totalLength; i++)
  {
      matrix.Add(i,i,1);
  }

  return(matrix);
}



template <typename T>
inline MultiVector<T> operator* (SparseMatrix<T>& mat, const MultiVector<T>& u)
{
  MultiVector<T> v;

  v.Reset(mat.Length());

  mat.Multiply(u,v);
  return(v);
}



template <typename T>
inline MultiVector<T> operator* (const MultiVector<T>& u, SparseMatrix<T>& mat)
{
  MultiVector<T> v(mat.NumberOfColumns());

  mat.MultiplyTransposed(u,v);
  return(v);
}



template <typename T>
inline SparseMatrix<T> operator* (const T factor, SparseMatrix<T>& matrix)
{
  SparseMatrix<T> mat(matrix.Length());

  mat = matrix;
  mat.Multiply(factor);

  return(mat);
}





/* template <typename T> */
/* struct SparseMatrixEntry{ */
/*   T data; */
/*   int id; */
/* }; */




#endif // SPARSEMATRIX_HPP
