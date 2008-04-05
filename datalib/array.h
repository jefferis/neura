/* Array - Template data structure of type array

Christoph Reisinger, 161001- ??
expanded by Roland Schulte 210103 */

#ifndef ARRAY_HPP
#define ARRAY_HPP

#include <stream.h>

template <class T>
class Array {

protected:
  T* elements;
  int length;
  int maxlength;

public:
  Array(){length=0; maxlength=0; elements = NULL;}
  Array(const int);
  Array(const Array <T>&);
  int Reset(const Array <T>&);
  ~Array(){Delete();}
  int Delete();

  int Reset(const int );
  int SetZero();
  int SetOne();
  int ExtendTo(const int );
  int GetLength () const {return(length);}
  int Length () const {return(length);}
  int SetElement(const int, const T&);
  int Set(const T&);
  int Add(const int, const T&);
  int Add(const T&);
  int Multiply(const int, const T&);
  int Multiply(const T&);

  int GetElement (const int, T&) const;
  T& GetElement(const int) const;
  T& Get(const int) const;
  
  
  T& operator[] (const int) const;
  Array<T>& operator= (const Array<T>&);
  Array<T>& operator= (const T&);
  Array<T>& operator- (const T&);
};



template <class T>
Array<T>::Array(const int l)
{
  length = l;
  maxlength = l;
  elements = new T[maxlength+1];
}



template <class T>
Array<T>::Array(const Array<T> & array)
{
  length = array.GetLength();
  maxlength = length;
  elements = new T[maxlength+1];
  
  for(int i=1; i<=length; i++)
    {
      elements[i] = array.GetElement(i);
    }
}



template <class T>
int Array<T>::Reset(const Array<T> & array)
{
  if(this == &array)
    {
      return(0);
    }

  length = array.GetLength();
  maxlength = length;
  delete [] elements;
  elements = new T[maxlength+1];

  for(int i=1; i<=length; i++)
    {
      elements[i] = array.GetElement(i);
    }
}



template <class T>
int Array<T>::Reset(const int newlength)
{

  if(newlength<0)
    {
      cout << "Array::Reset: Cannot reset to negative length!" << endl;
      exit(1);
    }

  delete [] elements;

  length = newlength;
  maxlength = length;
  elements = new T[newlength + 1];

  return(0);
}



template <class T>
int Array<T>::SetZero()
{
  for(int i=1; i<=length; i++)
    {
      elements[i] = 0;
    }

  return(0);
}



template <class T>
int Array<T>::SetOne()
{
  for(int i=1; i<=length; i++)
    {
      elements[i] = 1;
    }

  return(0);
}



template <class T>
int Array<T>::ExtendTo(const int newlength)
{
  T* newelements;

  if(newlength<maxlength)
    {
      cout << "Array::ExtendTo: Cannot extend to smaller length! Use ShrinkTo(...)!" << endl;
      exit(1);
    }

  newelements = new T[newlength + 1];

  for(int i=1;i<=length;i++)
    {
      newelements[i] = elements[i];
    }

  maxlength = newlength;
  delete [] elements;
  elements = newelements;

  return(0);
}



template <class T>
int Array<T>::SetElement(const int position, const T& element)
{
#ifdef DEBUG
  if(position<=0)
    {
      cout << "Array::SetElement:Give positive position!" << endl;
      exit(1);
    };
#endif
    
  if(position>maxlength)
    {
      maxlength = int(position*1.41421);
      ExtendTo(maxlength);
    }
  
  if(position>length)
    {
      length = position;
    }

  //  cout << "element: " << element << ", ";

  elements[position] = element;

  //  cout << elements[position] << endl;

  return(0);
}


// added by Roland Schulte
template <class T>
int Array<T>::Set(const T& entry)
{
  for(int i=1; i<=length; i++)
    {
      elements[i] = entry;   
    }

  return(0);
}



template <class T>
int Array<T>::Add(const int i, const T& addend)
{
  return(SetElement(i,elements[i]+addend));
}


// added by Roland Schulte
template <class T>
int Array<T>::Add(const T& addend)
{
  for(int i=1; i<=length; i++)
    {
      elements[i] += addend;   
    }

  return(0);
}


template <class T>
int Array<T>::Multiply(const int i, const T& factor)
{
  return(SetElement(i,factor*elements[i]));
}


// added by Roland Schulte
template <class T>
int Array<T>::Multiply(const T& factor)
{
  for(int i=1; i<=length; i++)
    {
     elements[i] = elements[i] * factor;   
    }

  return(0);
}


template <class T>
int Array<T>::GetElement(const int position, T& element) const
{
#ifdef DEBUG
  if(position<=0 || position>length)
    {
      cout << "GetElement: Array position outside valid range!" << endl;
      exit(1);
    };
#endif

    element = elements[position];

  return(0);
}



template <class T>
T& Array<T>::GetElement(const int position) const
{
  return(elements[position]);
}



template <class T>
T& Array<T>::Get(const int position) const
{
#ifdef DEBUG
  if(position<=0 || position>length)
    {
      cout << "GetElement: Array position outside valid range!" << endl;
      exit(1);
    };
#endif
      return(elements[position]);
    
}



template <class T>
T& Array<T>::operator[] (const int i) const
{
#ifdef DEBUG
  if(i<=0 || i>length)
    {
      cout << "Operator []: Array position outside valid range!" << endl;
      cout << "Tried to access position " << i << " where only 1 ... " << length << " are possible!" << endl; 
      exit(1);
    };
#endif
      return(elements[i]);
    
}







template <class T>
Array<T>& Array<T>::operator= (const Array<T> & a)
{
  if(this == &a)
    {
      return(*this);
    }

  int lengthofa = a.GetLength();

  if(length != lengthofa)
    {
      this->Reset(lengthofa);
    }

  for(int i=1;i<=length;i++)
    {
      elements[i] = a[i];
    }

  return(*this);
}



template <class T>
Array<T>& Array<T>::operator= (const T& a)
{
  for(int i=1;i<=length;i++)
    {
      elements[i] = a;
    }

  return(*this);
}


// added by Roland Schulte
template <class T>
Array<T>& Array<T>::operator- (const T&)
{
 int lengthofa = this->a.GetLength();

#ifdef DEBUG
  if(length != lengthofa)
    {
      cout << "Array::operator- : Arrays must have the same length!" << endl;
      return NULL;
    };
#endif	

  for(int i=1;i<=length;i++)
    {
      elements[i] -= this->a[i];
    }

  return(*this);

}


template <class T>
ostream & operator<< (ostream &o, const Array<T> & a)
{
 
  o << a.GetLength() << endl;

  for(int i=1; i<=a.GetLength(); i++) 
    {
      o << a.GetElement(i) << " ";
    }

   o <<  endl;
   return o;
}


template <class T>
istream & operator>> (istream & i, Array<T> & a)
{
  int l;
  T element;

  i >> l;
  a.Reset(l);

  for(int j=1; j<=l; j++) 
    {
      i >> element;
      a.SetElement(j,element);
    }

  return i;
}



template <class T>
int Array<T>::Delete()
{
  delete [] elements;
  elements = NULL;
  length = 0;
  maxlength = 0;
  return 0;
}





#endif // ARRAY_HPP
