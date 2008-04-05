
#ifndef MEMORYMANAGEMENT
#define MEMORYMANAGEMENT 

#include <stdio.h>
#include <iostream.h>
#include <stream.h>


#define MegaByte 1024*1024
#define PageSizeMegaByte 50
#define MAXNROFPAGES 200
//4GB should be enough
class MemoryManagement
{
 public:
  MemoryManagement(const MemoryManagement &m); 
  MemoryManagement(MemoryManagement &m);
  MemoryManagement(int pageSizeinMB);
  MemoryManagement();
  void operator=(const MemoryManagement &m);
  ~MemoryManagement();
  char *GetMemory(int sizeInByte);
  friend ostream &operator<<(ostream &ostr, const MemoryManagement &MM);  
 
private:
  int createNewPage(int pagesize);
  int freeAllPages();
  
  static int nrOfUser;
  static int usedByte;
  static int maxByte;
  static int maxUsedBytes;
  static char* startPointer;
  static char* present;

  int irregular_initialised;

  static char* page[MAXNROFPAGES];
  static int nrofpages;

};


#endif
