/* ParameterTree - Ordered binary tree for fast access

Christoph Reisinger, 230402 - ? */
/* BinTree - Template data structure of type binary tree

Christoph Reisinger, 041201 - ? */


/* String - Definition of a string class as in Stroustrup, p. 248 ff

Christoph Reisinger, 280402- ??*/

#ifndef STRING_HPP
#define STRING_HPP

#include <stream.h>
#include <string.h>

class String
{
  struct srep
  {
    char *s;
    int n;
    srep() {n=1;}
  };
  srep *p;

public:
  String(const char*);
  String();
  String(const String&);
  ~String();

  String& operator=(const char*);
  String& operator=(const String&);

  char& operator[](int);
  const char& operator[](int) const;

  friend ostream& operator<<(ostream&, const String&);
  friend istream& operator>>(istream&, String&);

  friend char* operator>>(const String&, char*);

  friend int operator==(const String &x, const char *s)
  {return strcmp(x.p->s,s)==0;}

  friend int operator==(const String &x, const String &y)
  {return strcmp(x.p->s,y.p->s)==0;}

  friend int operator!=(const String &x, const char *s)
  {return strcmp(x.p->s,s)!=0;}
  
  operator char*() {return p->s;};

  friend int operator!=(const String &x, const String &y)
  {return strcmp(x.p->s,y.p->s)!=0;}
};




#endif // STRING_HPP


#ifndef BINARY_NUMBER_HPP
#define BINARY_NUMBER_HPP

#include "array.h"

class Binary : public Array <bool>{

public:
  Binary(){}
  Binary(const int&);
  int GetDigit(const int&i, bool&b) const{return(GetElement(i,b));}
  bool& GetDigit(const int&i) const{return(GetElement(i));}
  int SetDigit(const int&i, const bool&b);
  int SetZero();
  int SetZero(const int&i){return(SetElement(i,0));}
  int SetOne(const int&i){return(SetElement(i,1));}
};


#endif // BINARY_NUMBER_HPP


#ifndef BINTREE_HPP
#define BINTREE_HPP
template <typename T>
struct TreeElement{
  T data;
  bool isempty;
  TreeElement<T>* left;
  TreeElement<T>* right;
};


template <typename T>
class BinTree {

protected:
  TreeElement<T>* root;

  TreeElement<T>* position;
  int level;

public:
  BinTree();
  BinTree(const BinTree <T> &);
  // allocates and sets a copy of a given tree
  ~BinTree();

  int IsRoot();
  int GoToRoot();

  int InsertRoot ();
  int InsertRoot (const T&);
  int InsertLeft (const T&);
  int InsertRight (const T&);

  int ReplaceElement (const T&);
  int ReplaceLeftElement (const T&);
  int ReplaceRightElement (const T&);

  int GetElement (T&) const;
  int GetLeftElement (T&) const;
  int GetRightElement (T&) const;
  T & GetElement() const;
  T & GetLeftElement() const;
  T & GetRightElement() const;


  int GoTo(const Binary&);
  int GoToRelative(const Binary&);
  int InsertElement(const Binary&, const T&);
  int InsertRelative(const Binary&, const T&);
  int InsertRelativeTo(const Binary&, const T&, TreeElement<T>* const);
  int GetElement(const Binary&, T&);
  T & GetElement(const Binary&);

  int IsLeftBranch() const;
  int IsRightBranch() const;
  int TurnLeft();
  int TurnRight();

  int GetLevel() const;
  
  void RecursivePrint (ostream &);

  int ChopLeftBranch();
  int ChopRightBranch();
  int ChopDown();
};



template <typename T>
BinTree<T>::BinTree()
{
  root = position = NULL;
  level = 0;
}



template <typename T>
int BinTree<T>::InsertRoot()
{
  if(root)
    {
      cout << "BinTree::InsertRoot: Tree already exists!" << endl;
      return(1);
    }

  root = new TreeElement<T>;
  root->isempty = 1;
  root->left = NULL;
  root->right = NULL;

  level = 1;
  position = root;

  return(0);
}



template <typename T>
int BinTree<T>::InsertRoot(const T& element)
{
  if(root)
    {
      cout << "BinTree::InsertRoot: Tree already exists!" << endl;
      return(1);
    }

  root = new TreeElement<T>;
  root->data = element;
  root->isempty = 0;
  root->left = NULL;
  root->right = NULL;

  level = 1;
  position = root;

  return(0);
}



template <typename T>
int BinTree<T>::GoToRoot()
{
  if(root)
    {
      position = root;
      level = 1;
      return(0);
    }
  else
    {
      //      cout << "BinTree::GoToRoot: No root!" << endl;
      return(1);
    }
}



template <typename T>
int BinTree<T>::IsRoot()
{
  if(root)
    {
      return(1);
    }
  else
    {
      return(0);
    }
}



template <typename T>
T & BinTree<T>::GetElement() const
{
  if(position)
    {
      if(position->isempty)
	{
	  cout << "BinTree::GetElement():Warning:Element not set!" << endl;
	}

      return(position->data);
    }
  else
    {
      cout << "BinTree::GetElement: State position!" << endl;
    }
}



template <typename T>
int BinTree<T>::GetElement(T& element) const
{
  if(position)
    {
      if(position->isempty)
	{
	  return(1);
	}
      else
	{
	  element = position->data;
	  return(0);
	}
    }
  else
    {
      cout << "BinTree::GetElement: State position!" << endl;
    }
}



template <typename T>
int BinTree<T>::ReplaceElement (const T& element)
{
  if(position)
    {
      if(position->isempty)
	{
	  cout << "BinTree::ReplaceElement: No element to replace!" << endl;
	  exit(1);
	}
      else
	{
	  position->data = element;
	  return(0);
	}
    }
  else
    {
      cout << "BinTree::ReplaceElement: State position!" << endl;
      exit(1);
    }
}



template <typename T>
int BinTree<T>::InsertLeft (const T& element)
{
  if(position)
    {
      if(IsLeftBranch())
	{
	  cout << "BinTree::InsertLeft: Warning: Left branch already exists!" << endl;
	  position->left->data = element;
	}
      else
	{
	  position->left = new TreeElement<T>;
	  position->left->data = element;
	  position->left->isempty = 0;
	  position->left->left = NULL;
	  position->left->right = NULL;
	}
    }
  else
    {
      if(root)
	{
	  cout << "BinTree::InsertLeft: I'm lost! Specify position!" << endl;
	  return(1);
	}
      else
	{
	  InsertRoot(element);
	}
    }
}



template <typename T>
int BinTree<T>::InsertRight (const T& element)
{
  if(position)
    {
      if(IsRightBranch())
	{
	  cout << "BinTree::InsertRight: Warning: Right branch already exists!" << endl;
	  position->right->data = element;
	}
      else
	{
	  position->right = new TreeElement<T>;
	  position->right->data = element;
	  position->right->isempty = 0;
	  position->right->left = NULL;
	  position->right->right = NULL;
	}
    }
  else
    {
      if(root)
	{
	  cout << "BinTree::InsertRight: I'm lost! Specify position!" << endl;
	  return(1);
	}
      else
	{
	  InsertRoot(element);
	}
    }
}



template <typename T>
int BinTree<T>::GoTo(const Binary& bin)
{
  GoToRoot();
  GoToRelative(bin);
}



template <typename T>
int BinTree<T>::GoToRelative(const Binary& bin)
{
  for(int i=1;i<=bin.GetLength();i++)
    {
      if(bin.GetDigit(i))
	{
	  TurnRight();
	}
      else
	{
	  TurnLeft();
	}
    }
}



template <typename T>
int BinTree<T>::InsertElement(const Binary& bin, const T& element)
{
  TreeElement<T>* current;

  if(!root)
    {
      InsertRoot();
    }

  return(InsertRelativeTo(bin,element,root));
}


template <typename T>
int BinTree<T>::InsertRelative(const Binary& bin, const T& element)
{
  if(!position)
    {
      cout << "BinTree::InsertRelative:State position!" << endl;
      return(1);
    }
  else
    {
      return(InsertRelativeTo(bin,element,position));
    }
}




template <typename T>
int BinTree<T>::InsertRelativeTo(const Binary& bin, const T& element, 
TreeElement<T>* const start)
{
  TreeElement<T>* current;

  current = start;

  for(int i=1;i<=bin.GetLength();i++)
    {
      if(bin.GetDigit(i))
	{
	  if(!current->right)
	    {
	      current->right=new TreeElement<T>;
	      current->right->isempty = 1;
	      current->right->left = NULL;
	      current->right->right = NULL;
	    }

	  current = current->right;
	}
      else
	{
	  if(!current->left)
	    {
	      current->left=new TreeElement<T>;
	      current->left->isempty = 1;
	      current->left->left = NULL;
	      current->left->right = NULL;
	    }

	  current = current->left;
	}
    }
 
  current->data = element;
  current->isempty = 0;

  return(0);
}




template <typename T>
int BinTree<T>::GetElement(const Binary& bin, T& element)
{
  TreeElement<T>* current;

  if(!root)
    {
      InsertRoot();
    }

  current = root;
  
  for(int i=1;i<=bin.GetLength();i++)
    {
      if(bin.GetDigit(i))
	{
	  if(!current->right)
	    {
	      cout << "BinTree::GetElement:Branch does not exist!" << endl;
	      return(1);
	    }
	  else
	    {
	      current = current->right;
	    }
	}
      else
	{
	  if(!current->left)
	    {
	      cout << "BinTree::GetElement:Branch does not exist!" << endl;
	      return(1);
	    }
	  else
	    {
	      current = current->left;
	    }
	}
    }

  if(current->isempty)
    {
      cout << "BinTree::GetElement: No element given!" << endl;
      return(1);
    }
  else
    {
      element = current->data;
    }

  return(0);
}


  
template <typename T>
T & BinTree<T>::GetElement(const Binary&bin)
{
  T elem;
  
  GetElement(bin,elem);
  return(elem);
}



template <typename T>
int BinTree<T>::GetLevel() const
{
  return(level);
}

template <typename T>
int BinTree<T>::IsLeftBranch() const
{
  if(position->left)
    {
      return(1);
    }
  else
    {
      return(0);
    }
}


template <typename T>
int BinTree<T>::IsRightBranch() const
{
  if(position->right)
    {
      return(1);
    }
  else
    {
      return(0);
    }
}


template <typename T>
int BinTree<T>::TurnLeft()
{
  if(IsLeftBranch())
    {
      position = position->left;
      level++;
      return(0);
    }
  else
    {
      cout << "BinTree::TurnLeft: Error: No left branch!" << endl;
      return(1);
    }
}


template <typename T>
int BinTree<T>::TurnRight()
{
  if(IsRightBranch())
    {
      position = position->right;
      level++;
      return(0);
    }
  else
    {
      cout << "BinTree::TurnRight: Error: No right branch!" << endl;
      return(1);
    }
}


template <typename T>
int BinTree<T>::ChopLeftBranch()
{
  TreeElement<T>* aux;

  if(IsLeftBranch())
    {
      aux = position;

      position = position->left;
      ChopLeftBranch();
      ChopRightBranch();

      position = aux;
      delete position->left;
      position->left = NULL;
    }
  return 0;
}



template <typename T>
int BinTree<T>::ChopRightBranch()
{
  TreeElement<T>* aux;

  if(IsRightBranch())
    {
      aux = position;

      position = position->right;
      ChopLeftBranch();
      ChopRightBranch();

      position = aux;
      delete position->right;
      position->right = NULL;
    }
return 0;
}



template <typename T>
BinTree<T>::~BinTree()
{
  if(IsRoot())
    {
      GoToRoot();
      ChopLeftBranch();
      ChopRightBranch();
      delete root;

      root = NULL;
      position = NULL;
      level = 0;
    }
}



/*****************************************************************************************/




template <typename T>
ostream & operator<< (ostream &o, BinTree<T> & tree)
{
  cout << "BinTree::Operator<<: Warning: Internal label is being moved!" << endl;

  tree.GoToRoot();

  tree.RecursivePrint(o);

  return o;
}


template <typename T>
void BinTree<T>::RecursivePrint (ostream &o)
{ 
  TreeElement<T>* aux;
  int auxlevel;
  T elem;

  for(int i=1; i<=GetLevel(); i++)
    {
      o << "   ";
    }

  if(GetElement(elem))
    {
      o << ". " << endl;
    }
  else
    {
      o << elem << " " << endl;
    }

  aux = position;
  auxlevel = level;
  if(IsLeftBranch())
    {
      TurnLeft();
      RecursivePrint(o);
    }

  position = aux;
  level = auxlevel;

  if(IsRightBranch())
    {
      TurnRight(); 
      RecursivePrint(o);
    }
  position = aux;
  level = auxlevel;
}
#endif // BINTREE_HPP





#ifndef PARAMETERTREE_HPP
#define PARAMETERTREE_HPP

#include <string.h>



template <typename T>
struct Parameter
{
  char* name;
  T value;
  bool isSet;
};




template <typename T>
class ParameterTree : public BinTree<Parameter<T> >
{
public:
  ParameterTree();
  ~ParameterTree();

  int CreateSorted(char* name);
  int CreateSortedRecursively(char* name);
  int InsertSorted(char* name, const T& value);
  int InsertSortedRecursively(char* name, const T& value);
  int GoTo(char* name);
  int GoToRecursively(char* name);
  int Insert(const T& value);
  int Get(T& value);
  int GetSorted(char* name, T& value);
  int GetSortedRecursively(char* name, T& value);
};



template <typename T>
ParameterTree<T>::ParameterTree()
{
  this->root = this->position = NULL;
  this->level = 0;
}



template <typename T>
ParameterTree<T>::~ParameterTree()
{
  if(this->IsRoot())
    {
      this->GoToRoot();
      this->ChopLeftBranch();
      this->ChopRightBranch();
      delete this->root;

      this->root = NULL;
      this->position = NULL;
      this->level = 0;
    }
}



template <typename T>
int ParameterTree<T>::CreateSorted(char* name)
{
  Parameter<T> parameter;

  parameter.name = name;
  parameter.isSet = 0;

  if(this->GoToRoot())
    {
      //      cout << "insert root *" << endl;
      return(InsertRoot(parameter));
    }
  else
    {
      return(CreateSortedRecursively(name));
    }
}



template <typename T>
int ParameterTree<T>::CreateSortedRecursively(char* name)
{
  int status;
  Parameter<T> current, parameter;

  GetElement(current);

  status = strcmp(name, current.name);

  parameter.name = name;
  parameter.isSet = 0;

  if(status == 0)
    // parameter exists already
    {
      cout << "Warning: Parameter exists already!" << endl;
      return(0);
    }
  else if(status<0)
    // name < current name (lexicographically)
    {
      if(this->IsLeftBranch())
	{
	  //	  cout << "turn left -> ";
	  this->TurnLeft();
	  CreateSortedRecursively(name);
	}
      else
	{
	  //	  cout << "insert left *" << endl;
	  return(InsertLeft(parameter));
	}
    }
  else
    // name > current name (lexicographically)
    {
      if(this->IsRightBranch())
	{
	  //	  cout << "turn right -> ";
	  this->TurnRight();
	  CreateSortedRecursively(name);
	}
      else
	{
	  //	  cout << "insert right *" << endl;
	  return(InsertRight(parameter));
	}
    }
}



template <typename T>
int ParameterTree<T>::Insert(const T& value)
{
  Parameter<T> parameter;

  parameter.name = this->position->data.name;
  //  cout << "Parameter " << position->data.name << " set to ";
  parameter.value = value;
  //  cout << value << endl;
  parameter.isSet = 1;

  ReplaceElement(parameter);

  return(0);
}



template <typename T>
int ParameterTree<T>::Get(T& value)
{
  Parameter<T> parameter;

  GetElement(parameter);

  if(parameter.isSet)
    {
      //      cout << "Get: " << parameter.name << ": " << parameter.value << endl << endl;
      value = parameter.value;
    }
  else
    {
      cout << "Parameter:" << parameter.name << "not set!" << endl;
      return 1;
    }

  return(0);
}



template <typename T>
int ParameterTree<T>::InsertSorted(char* name, const T& value)
{
  Parameter<T> parameter;

  parameter.name = name;
  parameter.value = value;
  parameter.isSet = 1;

  if(this->GoToRoot())
    {
      //      cout << "insert root *" << endl;
      return(InsertRoot(parameter));
    }
  else
    {
      return(InsertSortedRecursively(name, value));
    }
}



template <typename T>
int ParameterTree<T>::InsertSortedRecursively(char* name, const T& value)
{
  int status;
  Parameter<T> current, parameter;

  GetElement(current);

  status = strcmp(name, current.name);

  parameter.name = name;
  parameter.value = value;

  if(status == 0)
    // parameter exists already
    {
      current.value = value;
      current.isSet = 1;
      ReplaceElement(current);
      return(0);
    }
  else if(status<0)
    // name < current name (lexicographically)
    {
      if(this->IsLeftBranch())
	{
	  //	  cout << "turn left -> ";
	  this->TurnLeft();
	 
	  InsertSortedRecursively(name, value);
	}
      else
	{
	  //	  cout << "insert left *" << endl;
	  return(InsertLeft(parameter));
	}
    }
  else
    // name > current name (lexicographically)
    {
      if(this->IsRightBranch())
	{
	  //	  cout << "turn right -> ";
	  this->TurnRight();
	  InsertSortedRecursively(name, value);
	}
      else
	{
	  //	  cout << "insert right *" << endl;
	  return(InsertRight(parameter));
	}
    }
}



template <typename T>
int ParameterTree<T>::GoTo(char* name)
{
  if(this->GoToRoot())
    {
      //      cout << "Empty tree!" << endl;
      return(1);
    }
  else
    {
      return(GoToRecursively(name));
    }
}



template <typename T>
int ParameterTree<T>::GoToRecursively(char* name)
{
  int status;
  Parameter<T> current;

  GetElement(current);

  status = strcmp(name, current.name);

  //  cout << name << " " << current.name << " " << status << endl;

  if(status == 0)
    // parameter exists
    {
      //      cout << "Parameter found!" << endl;
      return(0);
    }
  else if(status<0)
    // name < current name (lexicographically)
    {
      if(this->IsLeftBranch())
	{
	  //	  cout << "turn left -> ";
	  this->TurnLeft();
	  return(GoToRecursively(name));
	}
      else
	{
	  cout << "Parameter " << name << " does not exist in tree!" << endl;
	  return(1);
	}
    }
  else
    // name > current name (lexicographically)
    {
      if(this->IsRightBranch())
	{
	  //	  cout << "turn right -> ";
	  this->TurnRight();
	  return(GoToRecursively(name));
	}
      else
	{
	  cout << "Parameter does not exist in tree!" << endl;
	  return(1);
	}
    }
}



template <typename T>
int ParameterTree<T>::GetSorted(char* name, T& value)
{
  if(this->GoToRoot())
    {
      cout << "Empty tree!" << endl;
      return(1);
    }
  else
    {
      return(GetSortedRecursively(name, value));
    }
}



template <typename T>
int ParameterTree<T>::GetSortedRecursively(char* name, T& value)
{
  int status;
  Parameter<T> current;

  GetElement(current);

  status = strcmp(name, current.name);

  if(status == 0)
    // parameter exists
    {
      //      cout << "Parameter found!" << endl;

      if(current.isSet)
	{	  
	  value = current.value;
	  return(0);
	}
      else
	{
	  cout << "Parameter has not been set!" << endl;
	  exit(1);
	}
    }
  else if(status<0)
    // name < current name (lexicographically)
    {
      if(this->IsLeftBranch())
	{
	  //	  cout << "turn left -> ";
	  this->TurnLeft();
	  GetSortedRecursively(name, value);
	}
      else
	{
	  cout << "Parameter does not exist in tree!" << endl;
	  return(1);
	}
    }
  else
    // name > current name (lexicographically)
    {
      if(this->IsRightBranch())
	{
	  //	  cout << "turn right -> ";
	  this->TurnRight();
	  GetSortedRecursively(name, value);
	}
      else
	{
	  cout << "Parameter does not exist in tree!" << endl;
	  return(1);
	}
    }
}





#endif // PARAMETERTREE_HPP
