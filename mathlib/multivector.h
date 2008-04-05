/* MultiVector - Template data structure of type 
   multivector = multiarray (parent) + algebraic operations

Christoph Reisinger, 201201 - ?? */

#ifndef MULTIVECTOR_HPP
#define MULTIVECTOR_HPP

#include "multiarray.h"
#include <math.h>

template <typename T>
class MultiVector : public MultiArray<T> {

public:
  //  Vector(const int&);
  MultiVector(){}
  MultiVector(const Array<int>&);
  MultiVector(const int);

  MultiVector<T> operator+ (const MultiVector<T> &) const;
  MultiVector<T> operator- (const MultiVector<T> &) const;
  //  MultiVector<T> operator= (const MultiVector<T> &);
  MultiVector<T> operator+= (const MultiVector<T> &);
  MultiVector<T> operator-= (const MultiVector<T> &);
  T operator* (const MultiVector<T>&) const;

  //  friend Vector<T> operator* <> (const T &, const Vector<T> &);
  friend T ScalarProduct <>(const MultiVector<T>&, const MultiVector<T>&);
};

template <typename T>
MultiVector<T>::MultiVector(const Array<int>& newLength)
{
  Reset(newLength);
}



template <typename T>
MultiVector<T>::MultiVector(const int newLength)
{
  Reset(newLength);
}





// template <typename T>
// int MultiArray<T>::Reset(const Array<int>& newLength)
// {
//   dimension = newLength.GetLength();
//   length.Reset(dimension);
//   length = newLength;

//   totalLength = length[1];

//   for(int i=2;i<=dimension;i++)
//     {
//       totalLength *= length[i];
//     }

//   elements.Reset(totalLength);
// }



/*
template <typename T>
MultiVector<T>::MultiVector(const int & l)
{
  this->Reset(l);
}
*/



template <typename T>
MultiVector<T> MultiVector<T>::operator+ (const MultiVector<T> & vector) const
{
  MultiVector<T> sum(length);

#ifdef DEBUG	
  if(totalLength != vector.TotalNumberOfElements())
    {
      cout << "MultiVector operator +: Dimensions mismatch!" << endl;
      exit(1);
    }
#endif

  for(int i=1;i<=totalLength;i++)
    {
      sum.Set(i,elements[i]+vector[i]);
    }

  return sum;
}





template <typename T>
MultiVector<T> MultiVector<T>::operator- (const MultiVector<T> & vector) const
{
  MultiVector<T> difference(length);
#ifdef DEBUG
  if(totalLength != vector.TotalNumberOfElements())
    {
      cout << "MultiVector operator -: Dimensions mismatch!" << endl;
      exit(1);
    }
#endif
  
  for(int i=1;i<=totalLength;i++)
    {
      difference.Set(i,elements[i]-vector[i]);    
    }

  return difference;
}



// template <typename T>
// MultiVector<T> MultiVector<T>::operator= (const MultiVector<T> & vector)
// { 
//   if(totalLength != vector.TotalNumberOfElements())
//     {
//       cout << "MultiVector operator =: Dimensions mismatch!" << endl;
//       exit(1);
//     }

//   for(int i=1;i<=totalLength;i++)
//     {
//       Set(i,vector[i]);
//     }

//   return vector;
// }



template <typename T>
MultiVector<T> MultiVector<T>::operator+= (const MultiVector<T> & vector)
{ 
  MultiVector<T> sum(length);
    
#ifdef DEBUG
  if(totalLength != vector.TotalNumberOfElements())
    {
      cout << "MultiVector operator +=: Dimensions mismatch!" << endl;
      exit(1);
    }
#endif

  for(int i=1;i<=totalLength;i++)
    {
      Set(i,vector[i]+elements[i]);
    }

  return(*this);
}



template <typename T>
MultiVector<T> MultiVector<T>::operator-= (const MultiVector<T> & vector)
{ 
  MultiVector<T> sum(length);

#ifdef DEBUG
  if(totalLength != vector.TotalNumberOfElements())
    {
      cout << "MultiVector operator -=: Dimensions mismatch!" << endl;
      exit(1);
    }
#endif

  for(int i=1;i<=totalLength;i++)
    {
      Set(i,elements[i]-vector[i]);
    }

  return(*this);
}



template <typename T>
T MultiVector<T>::operator* (const MultiVector<T> & vector) const
{
  T sum = 0;

#ifdef DEBUG
  if(totalLength != vector.TotalNumberOfElements())
    {
      cout << "MultiVector operator *: Dimensions mismatch!" << endl;
      exit(1);
    }
#endif	

  for(int i=1;i<=totalLength;i++)
    {
      sum += elements[i]*vector.Get(i);
    }

  return sum;
}




// friends

template <typename T>
T ScalarProduct(const MultiVector<T> & vector1, const MultiVector<T> & vector2)
{
  return vector1*vector2;
}



template <typename T>
MultiVector<T> operator* (const T & factor, const MultiVector<T> & vector)
{
  MultiVector<T> product(vector.Length());
  int i;

  for(i=1;i<=vector.TotalNumberOfElements();i++)
    {
      product.Set(i,factor*vector.Get(i));
    }
  
  return product;
}



template <typename T>
T Norm(const MultiVector<T> & vector)
{
  int i, n = vector.TotalNumberOfElements();
  T sum = 0;
 
  for(i=1; i<=n; i++)
    {
      sum += vector[i]*vector[i];
    }
 
  return(sqrt(sum));
}



#endif // MULTIVECTOR_HPP
