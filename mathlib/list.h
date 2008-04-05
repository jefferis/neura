/* List - Template data structure of type single linked list

Christoph Reisinger, 221001 - ? */

#ifndef LIST_HPP
#define LIST_HPP

#include "memory.h"

template <typename T>
struct Element{
  T data;
  Element<T>* next;
};


template <typename T>
class List : public MemoryManagement {

protected:
  Element<T>* head;
  Element<T>* tail;

  Element<T>* bookmark;
  int bookmarkPosition;

  int length;

public:
  List();
  List(const List<T>&);
  int Reset(const List<T>&);
  // allocates and sets a copy of a given list
  ~List();
  int Delete();

  int Length() const;

  int Append(const T&);
  int Prepend(const T&);
  int Insert(const int, const T&);
  int Insert(const T&);
  int SetElement(const T&);

  T Get() const;
  int GetElement(T&) const;                // returns: 1... bookmark set, 0... otherwise
  int GetElement(int, T&) const;
  //  T Get(const int) const;

  T Next();
  T Previous();

  int DeleteNext();
  int DeleteFirst();
  int DeleteLast();

  int GoToHead();
  int SetBookmark(const int i);
  int BookmarkPosition() const;
  int Move();                      // returns: 0... end of list has been reached, 1... otherwise
  int MoveBack();

  istream & Read(istream&);
  ostream & Print(ostream&) const;
  
  int Append(List<T>&);

  int Join(List<T>&);
  int Add(const T&);

  List<T>& operator=(const List<T>&);
};





template <typename T>
List<T>::List() : MemoryManagement(PageSizeMegaByte) //there is a reason why  MemoryManagement has to be initalized with an integer!
{
  length = bookmarkPosition = 0;
  head = tail = bookmark = NULL;
}



template <typename T>
List<T>::List(const List<T>& list)
{
  Reset(list);
}



template <typename T>
int List<T>::Reset(const List<T>& list)
{
  Element<T>* nextElement;
  T element;
  int newLength;

  if(this == &list)
    {
      return(0);
    }

  Delete();

  newLength = list.Length();

  if(newLength == 0)
    {
      head = tail = bookmark = NULL;
      bookmarkPosition = 0;
    }
  else
    {
      for(int i=1;i<=newLength;i++)
	{
	  list.GetElement(i,element);
	  Append(element);
	}
    }
}



template <typename T>
List<T>& List<T>::operator=(const List<T>& list)
{
  if(this == &list)
    {
      return(*this);
    }

  Reset(list);

  return(*this);
}



template <typename T>
int List<T>::Length() const
{
  return(length);
}



template <typename T>
int List<T>::Prepend(const T& element)
{
  Element<T>* newElement;
  int ElementSize=sizeof(Element<T>);

  newElement = (Element<T>*)   GetMemory(ElementSize);
  newElement->data = element;
  newElement->next = head;

  head = newElement;

  if(!tail)
    {
      tail = head;
    }

  length++;
}



template <typename T>
int List<T>::Append(const T& element)
{
  Element<T>* newElement;

  int ElementSize=sizeof(Element<T>);
  newElement = (Element<T>*) GetMemory(ElementSize);
  newElement->data = element;
  newElement->next = NULL;

  if(tail)
    {
      tail->next = newElement;
    }
  
  tail = newElement;

  if(!head)
    {
      head = tail;
    }

  length++;
  return 0;
}



template <typename T>
int List<T>::GetElement(int position, T& element) const
{
  Element<T>* newElement;

#ifdef DEBUG
  if(position>length || position<1)
    {
      cout << "List: Invalid index!" << endl;
      exit(1);
    };
#endif

  newElement = head;

  for(int i=2;i<=position;i++)
    {
      newElement = newElement->next;
    }

  element = newElement->data;

  return(0);
}



template <typename T>
T List<T>::Get() const
{
#ifdef DEBUG
  if(!bookmark)
    {
      cout << "List::Get: State position!" << endl;
      exit(1);
    };
#endif	
  
      return(bookmark->data);
    
}



template <typename T>
int List<T>:: GetElement(T& element) const
{
  if(!bookmark)
    {
      return(0);
    }
  else
    {
      element = bookmark->data;
      return(1);
    }
}



template <typename T>
int List<T>::DeleteFirst()
{
  Element<T>* oldHead;

  if(!head)
	// empty list
	{
	  return(1);
	}

  oldHead = head;
  head = head->next;

  if(bookmark == oldHead)
	{
	  bookmark = head;
	}

  length--;
  //delete oldHead;

  return(0);
}



template <typename T>
int List<T>::DeleteLast()
{
  if(!head)
	// empty list
	{
	  return(1);
	}

  bookmark = head;

  if(!(bookmark->next))
	{
	  //delete head;
	  length = 0;
	  head = tail = bookmark = NULL;
	  return(0);
	}

  while((bookmark->next)->next)
	{
	  bookmark = bookmark->next;
	}

  //  delete tail;
  tail = bookmark;

  //delete tail->next;
  tail->next = NULL;

  length--;
  return(0);
}



template <typename T>
int List<T>::DeleteNext()
{
  Element<T>* aux;

  if(!bookmark)
    {
      cout << "List::DeleteNext: State position!" << endl;
      return(1); 
    }

  aux = bookmark->next;

  if(!aux)
    {
      cout << "List::DeleteNext: No next element!" << endl;
      return(1);
    }

  bookmark->next = aux->next;
  //delete aux;
  length--;

  if(!bookmark->next)
    {
      tail = bookmark;
    }

  return(0);
}



template <typename T>
int List<T>::GoToHead()
{
  bookmark = head;
    return 0;	
}



template <typename T>
int List<T>::SetBookmark(const int i)
{
  this->bookmark=head;
  for(int j=1; j<i; j++)
    {
      if(!bookmark=bookmark->next)
	{
	  cout << "List::SetBookmark: Desired position greater than length!" << endl;
	  return(1);
	}
    }

  bookmarkPosition = i;
}



template <typename T>
int List<T>::BookmarkPosition() const
{
  return(bookmarkPosition);
}



template <typename T>
int List<T>::Move()
{
#ifdef DEBUG
  if(!bookmark)
    {
      cout << "List::Move: State position!" << endl;
      exit(1);
    }
#endif

    if(bookmark = bookmark-> next)
    {
      return(1);
    }
    else
    {
      return(0);
    }
}



template <typename T>
T List<T>::Next()
{
#ifdef DEBUG	
  if(!bookmark)
    {
      cout << "List::Next: State position!" << endl;
      exit(1);
    }
#endif
  if(bookmark = bookmark-> next)
    {
      return(bookmark->data);
    }
  else
    {
      return(T(0));
    }
}



template <typename T>
ostream &  List<T>::Print(ostream &o) const
{
  Element<T>* current;

  o << length << endl;

  current = head;

  for(current=head; current; current = current->next)
    {
      o << current->data << " ";
    }

  o << endl;

  return(o);
}



template <typename T>
istream &  List<T>::Read(istream &i)
{
  Element<T>* current;

  i >> length;

  if(length)
    {
      int ElementSize=sizeof( Element<T>);
      head = (Element<T>*) GetMemory(ElementSize);
      i >> head->data;
      current = head;

      for(int j=2;j<=length;j++)
	{
	  current->next = (Element<T>*) GetMemory(ElementSize);
	  current = current->next;
	  i >> current->data;
	}
      
      current->next = NULL;
      tail = current;
      
    }
  
  return(i);
}



template <typename T>
ostream & operator<< (ostream& o, const List<T>& list)
{
  return(list.Print(o));
}


template <typename T>
istream & operator>> (istream& i, List<T>& list)
{
  return(list.Read(i));
}



template <typename T>
int operator++ (List<T>& list)
{
  return(list.Move());
}



template <typename T>
List<T>::~List() //: ~MemoryManagement()
{
Element<T>* current;

  if(head)
    {
      while(current=head->next)
        {
          head->data.~T();
          head = current;
        }
    }

  head = tail = bookmark = NULL;
  bookmarkPosition = length = 0;
 
}



template <typename T>
int List<T>::Delete()
{
  return(0);
}



template <typename T>
int List<T>::Append(List<T>& list)
{
  T element;

  list.GoToHead();

  while(list.GetElement(element))
    {
      Append(element);
      list.Move();
    }

  return(0);
}



template <typename T>
int List<T>::Join(List<T>& list)
{
  T element;

  list.GoToHead();

  while(list.GetElement(element))
    {
      Add(element);
      list.Move();
    }

  return(0);
}



template <typename T>
int List<T>::Add(const T& element)
{
  T elem;

  GoToHead();

  while(GetElement(elem))
    {
      if(element.id == elem.id)
	{
	  bookmark->data.data = element.data;
	  return(0);
	}
      Move();
    }

  return(Append(element));
}



template <typename T>
int List<T>::SetElement(const T& element)
{
  if(!bookmark)
    {
      cout << "List::Get: State position!" << endl;
      return(1);
    }
  else
    {
      bookmark->data = element;
      return(0);
    }
}



#endif // LIST_HPP
