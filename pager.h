#ifndef PAGER_H
#define PAGER_H

#include <cstdio>

#include "pagetable.h"
#include "log_helpers.h"

class Pager {
private:
  PageTable *pageTable;
  FILE* filePtr;
  unsigned int currVA;
  int nextFreeFrame;
  int availableFrames;
  int NFUInterval;
  LogOptionsType logOptions;
  unsigned int numOfPageReplaces;
  unsigned int pageTableHits;
  unsigned int numOfAddresses;
  unsigned int numOfFramesAllocated;
  unsigned long int pgtableEntries;

public:
  Pager(PageTable *pt, 
      FILE *fp,
      int numOfAddresses, 
      int availableFrames, 
      int interval, 
      LogOptionsType logOptions);

  void log_vpns_to_pfns(int,unsigned int, int);

  void run();
  void log();
  void pageReplacement();

};

#endif
