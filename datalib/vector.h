/* Vector - Template data structure of type vector = array (parent) + algebraic operations

Christoph Reisinger, 221001 - ?? */

#ifndef VECTOR_HPP
#define VECTOR_HPP

#include "array.h"
#include <math.h>


template <class T>
class Vector : public Array<T> {

public:
  Vector(const int&);
  ~Vector(){this->Delete();}

  Vector<T> operator+ (const Vector<T> &) const;
  Vector<T> operator- (const Vector<T> &) const;
  
  T operator* (const Vector<T>&) const;
  

 
  //friend T ScalarProduct <>(const Vector<T>&, const Vector<T>&);
  //friend double Norm <T>(const Vector<T>&);
  //friend T NormSquared <T>(const Vector<T>&);
  //friend Vector<T> ComponentMultiply<T>(const Vector<T>& A1, const Vector<T>& A2);
  //friend ostream  &operator<< <T>(ostream &ostr, const Vector<T> &v); 
  //friend Vector<T> operator* (const T & factor, const Vector<T> & vector);
};



template <class T, class S> 
Vector<T> ComponentMultiply(const Vector<T>& A1, const Vector<S>& A2)
{
  Vector<T> B(3);
  int length;
  length=A1.GetLength();
  if (A2.GetLength()<length) length=A2.GetLength();

  for(int i=1;i<=length;i++)
    {
      B[i] =(double) A1[i]*A2[i];
    }

  return(B);

}



template <class T>
Vector<T>::Vector(const int & l)
{
  this->Reset(l);
}



template <class T>
Vector<T> Vector<T>::operator+ (const Vector<T> & vector) const
{
  Vector <T> sum(this->length);
#ifdef DEBUG
  if(this->length != vector.GetLength())
    {
      cout << "Vector operator +: Dimensions mismatch!" << endl;
      exit(1);
    }
#endif

  for(int i=1;i<=this->length;i++)
    {
      sum[i] = this->elements[i]+vector[i];
    }

  return sum;
}

template <class T>
Vector<T> Vector<T>::operator- (const Vector<T> & vector) const
{
  Vector <T> diff(this->length);
#ifdef DEBUG
  if(this->length != vector.GetLength())
    {
      cout << "Vector operator +: Dimensions mismatch!" << endl;
      exit(1);
    }
#endif

  for(int i=1;i<=this->length;i++)
    {
      diff[i] = this->elements[i]-vector[i];
    }

  return diff;
}





template <class T>
T Vector<T>::operator* (const Vector<T> & vector) const
{
  T sum = 0;
#ifdef DEBUG
  if(this->length != vector.GetLength())
    {
      cout << "Vector operator *: Dimensions mismatch!" << endl;
      exit(1);
    }
#endif
  
  for(int i=1;i<=this->length;i++)
    {
      sum += this->elements[i]*vector.GetElement(i);
    }

  return sum;
}




// friends

template <class T>
T ScalarProduct(const Vector<T> & vector1, const Vector<T> & vector2)
{
  return vector1*vector2;
}



template <class T>
T NormSquared(const Vector<T> & vector)
{
  return vector*vector;
}



template <class T>
double Norm(const Vector<T> & vector)
{
  return (double) sqrt((double)  (vector*vector) );
}



template <class T>
Vector<T> operator* (const T & factor, const Vector<T> & vector)
{
  Vector <T> product(vector.GetLength());
  int i;
  
  for(i=1;i<=vector.GetLength();i++)
    {
      product.SetElement(i,factor*vector.GetElement(i));
      // product[i] = factor*vector[i];
    }
  
  return product;
}

template <class T> 
ostream &operator<<(ostream &ostr, const Vector<T> &v) 
{
  int i;
  
  for(i=1;i<=v.GetLength();i++)
    {
      ostr<< i << ":" << v.GetElement(i) << endl;
      
    }
  
  return (ostr);
}



#endif // VECTOR_HPP
