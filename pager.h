// Names: Kaylee Muckerman and Julian To
// REDID: 130526910 and 130405272

/*
 * Header for Pager class; simulates MMU and demand paging from OS
 *
 */

#ifndef PAGER_H
#define PAGER_H

#include <cstdio>

#include "pagetable.h"
#include "log_helpers.h"


/*
 * Pager class
 *
 * @brief The main user function is in this class; simulates MMU and demand paging processes
 */
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

  /*
   * Pager()
   * 
   * @brief Constructor for pager object
   *
   * @param FILE* file pointer to current working file
   * @param int numOfAddresses
   * @param int availableFrames
   * @param int updateInterval for page replacement
   * @param LogOptionsType logOptions
   */
  Pager(PageTable *pt, 
      FILE *fp,
      int numOfAddresses, 
      int availableFrames, 
      int interval, 
      LogOptionsType logOptions);

  /*
   * run()
   * 
   * @brief main loop of pager object; the main function called by user
   *
   * @return void
   *
   */
  void run();

  /*
   * log()
   * 
   * @brief short hand to to call log_summary from log_helpers.h w/o its arguments 
   *
   * @return void
   *
   */
  void log();

};

#endif
