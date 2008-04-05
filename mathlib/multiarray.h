/* MultiArray - Template data structure of type multidimensional array

Christoph Reisinger, 141401- ??*/

#ifndef MULTIARRAY_HPP
#define MULTIARRAY_HPP

#include <stream.h>

#include "../datalib/array.h"
#include "../datalib/vector.h"

template <typename T>
class MultiArray {

protected:
  Array<T> elements;
  Array<int> length;
  int dimension;
  int totalLength;

public:
  MultiArray(){elements.Reset(0);length.Reset(0);totalLength=0;}
  MultiArray(const Array<int>&);
  ~MultiArray(){Delete();}
  int Delete(){elements.Delete(); length.Delete();return 0;}

  int Reset(const Array<int>&);
  int Reset(const int);

  int Dimension() const {return(length.GetLength());}
  Array<int> Length() const {return(length);}
  int Length(int i) const {return(length[i]);}
  int TotalNumberOfElements() const {return(totalLength);}

  int InitPosition(Array<int>&);
  int InitPosition(Array<int>&, const int, const int);
  int NextPosition(Array<int>&);
  int NextPosition2(Array<int>&);
  int NextPosition(Array<int>&, Array<int>& step);
  int NextPosition(Array<int>&, const int); // index as in second argument is left constant
  //  int NextPositionOnLine(Array<int>&, const int); // index as in second argument is incremented
  int ToCoordinates(const Array<int>&, Vector<double>&);

  int Add(const int, const T&);
  int Multiply(const int, const T&);
  int Set(const Array<int>&, const T&);
  int Set(const int i, const T& element){return(elements.SetElement(i,element));}
  T& Get(const Array<int>&) const;
  T& Get(const int) const;
  int GetElement(const Array<int>&, T&) const;
  int Index(const Array<int>&) const;
  Array<int> Position(const int) const;
  int SetZero();
  int SetZero(const int, const int);

  MultiArray<T>& operator= (const MultiArray<T>&);
  MultiArray<T>& operator= (const T&);
  ostream& Print(ostream& o) const;
  void Print(int i);

  T& operator[] (const int) const;
  T& operator[] (const Array<int>&) const;
};



template <typename T>
MultiArray<T>::MultiArray(const Array<int>& newLength)
{
  Reset(newLength);
}



template <typename T>
int MultiArray<T>::Reset(const Array<int>& newLength)
{
  dimension = newLength.GetLength();
  length.Reset(dimension);
  length = newLength;

  totalLength = length[1];

  for(int i=2;i<=dimension;i++)
    {
      totalLength *= length[i];
    }

  elements.Reset(totalLength);
  return 0;
}



template <typename T>
int MultiArray<T>::Reset(const int newLength)
{
  dimension = 1;
  length.Reset(1);
  length.SetElement(1,newLength);
  totalLength = newLength;

  elements.Reset(newLength);
  return 0;
}



template <typename T>
int MultiArray<T>::InitPosition(Array<int>& position)
{
  for(int i=1;i<=position.Length();i++)
    {
      position.SetElement(i,1);
    }
    return 0;
}



template <typename T>
int MultiArray<T>::InitPosition(Array<int>& position, const int pos, const int index)
{
  for(int i=1;i<=position.Length();i++)
    {
      position.SetElement(i,1);
    }
#ifdef DEBUG
  if(pos<=0 || pos>position.Length() || index<=0 || index>length[pos])
    {
      cout << "InitPosition: Check indices!" << endl;
      exit(1);
    }
#endif

  position.SetElement(pos, index);
    return 0;
}



template <typename T>
int MultiArray<T>::NextPosition(Array<int>& position)
{
  int i, dim = position.Length();

  for(i=dim; i>0 && position[i]==length[i]; i--);

  if(i==0)
    {
      return(0);
    }
  else
    {
      position.SetElement(i,position[i]+1);
      for(int j=i+1; j<=dim;j++)
	{
	  position.SetElement(j,1);
	}
      return(1);
    }
  return 0;
}



template <typename T>
int MultiArray<T>::NextPosition(Array<int>& position, const int pos)
{
  int i, dim = position.Length();

  for(i=dim; i>0 && (position[i]==length[i] || i==pos); i--);

  if(i==0)
    {
      return(0);
    }
  else
    {
      position.SetElement(i,position[i]+1);
      for(int j=i+1; j<=dim;j++)
	{
	  if(j!=pos)
	    {
	      position.SetElement(j,1);
	    }
	}
      return(1);
    }

  return 0;
}



template <typename T>
int MultiArray<T>::NextPosition2(Array<int>& position)
{
  int i, dim = position.Length();

  for(i=1; i<=dim && position[i]==length[i]; i++);

  if(i>dim)
    {
      return(0);
    }
  else
    {
      if(position[i]<=length[i]-2)
	{
	  position.SetElement(i,position[i]+2);
	}
      else
	{
	  position.SetElement(i,position[i]+1);
	}

      for(int j=i-1; j>0;j--)
	{
	  position.SetElement(j,1);
	}
      return(1);
    }
  return 0;
}



template <typename T>
int MultiArray<T>::NextPosition(Array<int>& position, Array<int>& step)
{
  int i, dim = position.Length();

  for(i=1; i<=dim && position[i]==length[i]; i++);

  if(i>dim)
    {
      return(0);
    }
  else
    {
      if(position[i]+step[i]<=length[i])
	{
	  position.SetElement(i,position[i]+step[i]);
	}
      else
	{
	  position.SetElement(i,length[i]);
	}

      for(int j=i-1; j>0;j--)
	{
	  position.SetElement(j,1);
	}
      return(1);
    }

  return 0;
}



template <typename T>
int MultiArray<T>::ToCoordinates(const Array<int>& position, 
				 Vector<double>& coordinates)
{
  for(int i=1; i<=length.Length(); i++)
    {
      coordinates[i] = double((position[i]-1)/double(length[i]-1));
      //      coordinates[i] = double(position[i])/(length[i]+1.);
    }

    return 0;
}



template <typename T>
int MultiArray<T>::SetZero()
{
  for(int i=1; i<=totalLength; i++)
    {
      if(elements.SetElement(i,0))
	{
	  return(1);
	}      
    }

  return(0);
}



template <typename T>
int MultiArray<T>::SetZero(const int i, const int j)
{
  Array<int> pos(dimension);

  InitPosition(pos,i,j);

  do
    {
      elements.SetElement(Index(pos),0.);
    }
  while(NextPosition(pos,i));

  return(0);
}



template <typename T>
int MultiArray<T>::Set(const Array<int>& index, const T& value)
{
  int position = Index(index);
  return(elements.SetElement(position,value));
}



template <typename T>
T& MultiArray<T>::Get(const int position) const
{
  return(elements.GetElement(position));
}



template <typename T>
T& MultiArray<T>::Get(const Array<int>& index) const
{
  int position = Index(index);
  return(elements.GetElement(position));
}



template <typename T>
int MultiArray<T>::Add(const int i, const T& addend)
{
  return(elements.SetElement(i,elements[i]+addend));
}



template <typename T>
int MultiArray<T>::Multiply(const int i, const T& factor)
{
  return(elements.SetElement(i,factor*elements[i]));
}



template <typename T>
int MultiArray<T>::GetElement(const Array<int>& index, T& element) const
{
  int position = Index(index);
  return(elements.GetElement(position,this->value));
}



template <typename T>
int MultiArray<T>::Index(const Array<int>& index) const
{
  int position = index[dimension];

  for(int i=dimension-1;i>=1;i--)
    {
      position = (position-1)*length[i]+index[i];
    }

  return(position);
}



template <typename T>
Array<int> MultiArray<T>::Position(const int position) const
{
  Array<int> index(dimension);
  int i, coordinate, pos = position;
  //int n; unused

  for(i=1;i<=dimension-1;i++)
    {
      coordinate = (pos-1)%length[i] + 1;
      pos = (pos-coordinate)/length[i] + 1;
      index.SetElement(i,coordinate);
    }

  index.SetElement(i,pos);

  return(index);
}



template <typename T>
ostream & MultiArray<T>::Print(ostream &o) const
{
  o << length << endl;
  o << elements;

  return(o);
}


// prints the i-th entry of this multiarray
template <typename T>
void 
MultiArray<T>::
Print(int i)
{
  cout << (*this)[i];
}


template <typename T>
ostream & operator<< (ostream& o, const MultiArray<T>& multiArray)
{
  return(multiArray.Print(o));
}



template <typename T>
T & MultiArray<T>::operator[] (const int i) const
{
#ifdef DEBUG
  if(i<=0 || i>TotalNumberOfElements())
    {
      cout << "MultiArray::Operator []: Position outside valid range!" << endl;
      exit(1);
    }
  else
#endif
      return(elements[i]);
    
}



template <typename T>
T & MultiArray<T>::operator[] (const Array<int>& index) const
{
  return(elements[Index(index)]);
}



template <typename T>
MultiArray<T>& MultiArray<T>::operator= (const MultiArray<T>& multiArray)
{
  if(this == &multiArray)
    {
      return(*this);
    }

  Reset(multiArray.Length());

  for(int i=1; i<=multiArray.TotalNumberOfElements(); i++)
    {
      elements.SetElement(i,multiArray[i]);
    }

  return(*this);
}



template <typename T>
MultiArray<T>& MultiArray<T>::operator= (const T& value)
{
  for(int i=1; i<=totalLength; i++)
    {
      elements.SetElement(i,value);
    }

  return(*this);
}





#endif // MULTIARRAY_HPP
