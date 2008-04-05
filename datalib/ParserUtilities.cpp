#include "ParserUtilities.h"

String::String()
{
  p = new srep;
  p->s = 0;
}



String::String(const String& x)
{
  x.p->n++;
  p = x.p;
}



String::String(const char* s)
{
  p = new srep;
  p->s = new char[strlen(s)+1];
  strcpy(p->s,s);
}



String::~String()
{
  if(--p->n == 0)
    {
      delete[] p->s;
      delete p;
    }
}


String& String::operator=(const char* s)
{
  if(p->n > 1)
    {
      p->n--;
      p = new srep;
    }
  else
    {
      delete[] p->s;
      p->s = new char[strlen(s)+1];
      strcpy(p->s,s);
      return *this;
    }
}



String& String::operator=(const String& x)
{
  x.p->n++;

  if(--p->n == 0)
    {
      delete[] p->s;
      delete p;
    }

  p = x.p;
  return *this;
}



ostream& operator<<(ostream& s, const String& x)
{
  return s << x.p->s << endl;
}



istream& operator>>(istream& s, String& x)
{
  char buf[256];

  s.get(buf, 256, '\n');

  char c;
  if(s.get(c) && c!='\n')
    {
      cout << "String operator >>: Input string too long!";
      exit(1);
    }

  x = buf;

  return s;
}



char* operator>>(const String& x, char* s)
{
  //  delete[] s;
  s = new char[strlen(x.p->s)+1];

  //  cout << "x.p->s: " << x.p->s << endl;

  strcpy(s,x.p->s);

  //  cout << "s: " << s << endl;  

  return s;
}


//Binary

Binary::Binary(const int& l)
{
  this->Reset(l);
}


int Binary::SetZero()
{
  for(int i=1;i<=GetLength();i++)
    {
      SetZero(i);
    }
}


int Binary::SetDigit(const int&i, const bool&b)
{
  if(int(b)!=0 && int(b)!=1)
    {
      cout << "Binary::SetDigit: Digit not in {0,1}!" << endl;
    }
  else
    {
      return(SetElement(i,b));
    }
}
