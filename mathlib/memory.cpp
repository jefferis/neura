#include "memory.h"

int MemoryManagement::nrOfUser=0;
int MemoryManagement::usedByte=0;
int MemoryManagement::maxByte=0;
int MemoryManagement::maxUsedBytes = 0; 
char* MemoryManagement::startPointer=NULL;
char* MemoryManagement::present=NULL;
int MemoryManagement::nrofpages=0;
char* MemoryManagement::page[MAXNROFPAGES];

void 
MemoryManagement::
operator=(const MemoryManagement &m)
{  
  MemoryManagement(PageSizeMegaByte);
}



MemoryManagement::
MemoryManagement(MemoryManagement &m)
{
  MemoryManagement(PageSizeMegaByte);
}



MemoryManagement::
MemoryManagement(const MemoryManagement &m)
{
  MemoryManagement(PageSizeMegaByte);
}

  


MemoryManagement::
MemoryManagement()
{
  MemoryManagement(PageSizeMegaByte);
}




int 
MemoryManagement::
freeAllPages()
{
for(int i=0;i<nrofpages;i++)
	{
	cout << "Free page " << i << endl;
	free(page[i]);
	}

return 0;
}

int
MemoryManagement::
createNewPage(int pagesizeMB)
{
if (nrofpages>=MAXNROFPAGES) {cerr << "MemoryManagement::Cant Malloc, max nr of pages reached, quit"<<endl;exit(1);};
cout << "MemoryManagement: Malloc " << pagesizeMB << "MB." << endl;
page[nrofpages]=(char *) malloc(pagesizeMB*MegaByte);
if (page[nrofpages]==NULL) {cerr << "MemoryManagement::Cant Malloc, quit"<<endl;exit(1);};
cout << "MemoryManagement:: Created Page " << nrofpages << endl;

startPointer=page[nrofpages];
present=startPointer;
maxByte=pagesizeMB*MegaByte;
usedByte=0;

nrofpages++;
return 0;
}



MemoryManagement::
MemoryManagement(int pSizeInMB)
{
    //cout << "Constructor!!!" << endl; dont care what in function header!
    irregular_initialised = 0;
    if (nrOfUser==0)
    {
   	  nrofpages=0;
        nrOfUser=0;
        usedByte=0;
        maxByte=0;
        maxUsedBytes = 0;
        startPointer=NULL;
        present=NULL;
		  createNewPage(PageSizeMegaByte);
        nrOfUser++;
    }
    else
    {
        nrOfUser++;
    }
}



MemoryManagement::
~MemoryManagement()
{
  if ( !irregular_initialised ){
    //cout << "Destructor - Nr of User." << nrOfUser << endl;
    nrOfUser--; 
    if (nrOfUser==0)
      {
		freeAllPages();
		cout << "MemoryManagement: Freed memory!!!" << endl;
	//cout << (*this);
      }
  }
  else {
    //cout << "Destruktor of not regular initialised memory object " << endl;
  }
}

char*  
MemoryManagement:: 
GetMemory(int sizeInByte)
{
  if (nrOfUser==0)
  {
  cout << "request for memory but no User!!!, quit" << endl;
  exit(1);
  
  }
  
  char *help;
  if (sizeInByte+usedByte>=maxByte) 
    {
      cerr << "Out of memory in Page " << nrofpages-1 << endl;
      if (createNewPage(PageSizeMegaByte)) {cerr << "Error, cant create new page" << endl;exit(1);};
      if (sizeInByte+usedByte>=maxByte) 
      {
      cerr << "Page to small try to alloc " << sizeInByte << " quit."<<endl;	
      exit(1);
      }
    }
  help=present;
  present= &(present[sizeInByte]);
  usedByte=usedByte+sizeInByte;
  if ( usedByte > maxUsedBytes ) maxUsedBytes = usedByte;
  return help;
}

ostream &operator<<(ostream &ostr, const MemoryManagement &MM)
{
  ostr << "------MemoryManagement----" << endl;
  ostr << "NrOfUser:" << MM.nrOfUser << endl;
  ostr << "UsedByte:" << MM.usedByte << endl;
  ostr << "maxByte:" << MM.maxByte << endl;
  ostr << "maxUsedBytes:" << MM.maxUsedBytes << endl;
  //ostr << "startPointer:" << MM.startPointer << endl;
  //ostr << "presentPointer:" << MM.present << endl;
  ostr << "--------------------------" << endl;
  return ostr;
} 
