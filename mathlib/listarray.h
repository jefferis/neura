/* Array - Template data structure of type array

Christoph Reisinger, 140102- ??*/

#ifndef LISTARRAY_HPP
#define LISTARRAY_HPP

#include "../datalib/array.h"
#include <stream.h>

template <typename T>
class ListArray {

protected:
  Array<T> elements;
  Array<int> next;
  Array<int> previous;

  int head;
  int tail;

  int length;
  int maxLength;

public:
  ListArray(){length = maxLength = 0;}
  ListArray(const int);
  //  ~ListArray();

  int Reset(const int);
  int SetZero();
  int GetLength () const {return(length);}
  int Length () const {return(length);}
  int MaxLength () const {return(maxLength);}

  int SetElement(const int, const T&);
  int Add(const int, const T&);
  int Multiply(const int, const T&);

  int GetElement (const int, T&) const;
  T & GetElement(const int) const;
  T &  Get(const int) const;

  ostream & Print(ostream & o) const;
  T & operator[] (const int) const;
};



template <typename T>
ListArray<T>::ListArray(const int l)
{
  Reset(l);
}



template <typename T>
int ListArray<T>::Reset(const int newlength)
{
  
  if(newlength<0)
    {
      cout << "ListArray::Reset: Cannot reset to negative length!" << endl;
      return(1);
    }

  head = tail = 0;

  length = 0;
  maxLength = l;
  elements.Reset(l);
  next.Reset(l);
  previous.Reset(l);

  return(0);
}



template <typename T>
int ListArray<T>::SetZero()
{
  return(elements.SetZero());
}



template <typename T>
int ListArray<T>::SetElement(const int position, const T & element)
{
  return(elements.SetElement(position,element));
}



template <typename T>
int ListArray<T>::Add(const int i, const T& addend)
{
  return(SetElement(i,elements[i]+addend));
}



template <typename T>
int ListArray<T>::Multiply(const int i, const T& factor)
{
  return(SetElement(i,factor*elements[i]));
}



template <typename T>
int ListArray<T>::GetElement(const int position, T & element) const
{
#ifdef DEBUG
  if(position<=0 || position>length)
    {
      cout << "GetElement: ListArray position outside valid range!" << endl;
      return(1);
    };
#endif
    
      element = elements[position];
    

  return(0);
}



template <typename T>
T & ListArray<T>::GetElement(const int position) const
{
  return(Get(position));
}



template <typename T>
T & ListArray<T>::Get(const int position) const
{
#ifdef DEBUG	
  if(position<=0 || position>length)
    {
      cout << "GetElement: ListArray position outside valid range!" << endl;
      exit(1);
    };
#endif

    return(elements[position]);
    
}



template <typename T>
T & ListArray<T>::operator[] (const int i) const
{
#ifdef DEBUG
  if(i<=0 || i>length)
    {
      cout << "Operator []: ListArray position outside valid range!" << endl;
      exit(1);
    };
#endif
    return(elements[i]);
    
}



template <typename T>
ostream & ListArray<T>::Print(ostream & o) const
{
  int k;

  o << length << endl;

  o << elements << endl << next << endl << previous << endl;

  k = tail;

  while(k!=0)
    {
      o << k << ":" << elements[k] << " -> ";
      k = previous[k];
    }

  o << "*" << endl;

  return(o);
}



template <typename T>
ostream & operator<< (ostream &o, const ListArray<T> & listArray)
{
  return(listArray.Print(o));
}







#endif // LISTARRAY_HPP
