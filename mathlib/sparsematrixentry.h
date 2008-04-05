/* SparseMatrixEntry  */

// this class extends struct SparseMatrixEntry 
// from Christoph Reisinger to class structure 

// Roland Schulte, 210103 - 

template <typename T>
class SparseMatrixEntry
{
 public:

  T data;
  int id;

  SparseMatrixEntry();

  int operator= (const SparseMatrixEntry<T>&);
  int operator= (T);
  SparseMatrixEntry<T>& operator+ (const SparseMatrixEntry<T>&);
  SparseMatrixEntry<T>& operator* (const SparseMatrixEntry<T>&);
  double operator+ ( const double );
  double operator* ( const double );


};


template <typename T>
SparseMatrixEntry<T>::SparseMatrixEntry()
{
  data = 0;
  id = 0;
}



template <typename T>
int SparseMatrixEntry<T>::operator=(const SparseMatrixEntry<T>& entry)
{
  data = entry.data;
  id = entry.id;

  return 0;
}

template <typename T>
int SparseMatrixEntry<T>::operator=(T entry)
{
  data = entry;
  //id = entry;

  return 0;
}

template <typename T>
SparseMatrixEntry<T>& SparseMatrixEntry<T>::operator+(const SparseMatrixEntry<T>& entry)
{
  data += entry.data;
  id = entry.id;
  
  return (*this);
}


template <typename T>
SparseMatrixEntry<T>& SparseMatrixEntry<T>::operator*(const SparseMatrixEntry<T>& factor)
{
  data *= factor.data;
  
  return (*this);

}


template <typename T>
double SparseMatrixEntry<T>::operator*(const double factor)
{
  return data * factor;
  
}
  
template <typename T>
double SparseMatrixEntry<T>::operator+(const double sum)
{
  return data + sum;
  
}
  
template <typename T>
ostream & operator<< (ostream &o, const SparseMatrixEntry<T> & spme)
{
 
  o << spme.id << " " << spme.data << " ";

   return o;
}
