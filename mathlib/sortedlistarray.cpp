#include "sortedlistarray.h"




SortedListArray::SortedListArray(const int l, const int n)
{
  Reset(l,n);
}


int SortedListArray::Reset(const int l, const int num)
{
  //  Reset(l);

  if(l<0)
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

  next.SetZero();
  previous.SetZero();
  
  numberOfWeights = num;
  limit.Reset(num);
  limit.SetZero();

  return(0);
}



int SortedListArray::InsertSorted(const int position, int const weight)
{
  int k;

  elements.SetElement(position, weight);
  
  if(limit[weight])
    {
      if(previous[limit[weight]])
	{
	  previous.SetElement(position,previous[limit[weight]]);
	  next.SetElement(previous[limit[weight]],position);
	}
      else
	{
	  previous.SetElement(position,0);
	}

      next.SetElement(position,limit[weight]);      
      previous.SetElement(limit[weight],position);
    }
  else
    {
      k = weight;

      while(k<numberOfWeights && !limit[k])
	{
	  k++;
	}

      if(limit[k])
	{
	  if(previous[limit[k]])
	    {
	      previous.SetElement(position,previous[limit[k]]);
	      next.SetElement(previous[limit[k]],position);
	    }
	  else
	    {
	      previous.SetElement(position,0);
	    }

	  previous.SetElement(limit[k],position);
	  next.SetElement(position,limit[k]);	  
	}
      else
	{
	  if(tail)
	    {
	      previous.SetElement(position,tail);
	      next.SetElement(tail,position);
	    }
	  else
	    {
	      previous.SetElement(position,0);
	      next.SetElement(position,0);
	    }
	  
	  tail = position;
	}
    }

  limit.SetElement(weight,position);
  return 0;
}



int SortedListArray::Tail() const
{
  return(tail);
}



int SortedListArray::CutTail()
{
  int i;

  if(previous[tail])
    {
      next.SetElement(previous[tail],0);
    }

  elements.SetElement(tail,0);

  for(i=1;i<=numberOfWeights;i++)
    {
      if(limit[i]==tail)
	{
	  limit.SetElement(i,0);
	}
    }

  tail = previous[tail];

  return(0);
}



bool SortedListArray::IsIn(const int i) const
{
  return(bool(elements[i]));
}



int SortedListArray::Weight(const int i) const
{
  return(elements[i]);
}



int SortedListArray::Remove(const int position)
{
  if(position<=0 || position > maxLength || !elements[position])
    {
      cout << "Remove: Invalid position!" << endl;
      return(1);
    }

  if(previous[position])
    {
      next.SetElement(previous[position],next[position]);
    }

  if(next[position])
    {
      previous.SetElement(next[position],previous[position]);
    }
  else
    {
      tail = previous[position];
    }

  if(position == limit[elements[position]])
    {
      if(next[position] && elements[position] == elements[next[position]])
	{
	  limit.SetElement(elements[position],next[position]);
	}
      else
	{
	  limit.SetElement(elements[position],0);
	}
    }

  elements.SetElement(position,0);
  previous.SetElement(position,0);
  next.SetElement(position,0);

  return(0);
}
