// Names: Kaylee Muckerman and Julian To
// REDID: 130526910 and 130405272

/*
 * PageReplacer header; defines a page replacer object used to calculate and keep track of
 * of pages for replacement
 *
 */
#ifndef PAGEREPLACEMENT_H
#define PAGEREPLACEMENT_H

#include <iostream>
#include <vector>

#include "pagetable.h"

using namespace std;

/*
 * FrameTableEntry struct
 *
 * @brief a simple collection used to keep track of values in each loaded page of
 *        pager
 */
struct FrameTableEntry{
  unsigned int vpn;
  int pfn;
  unsigned int bitStr;
  int timeSinceLastAccess;
};

/*
 * PageReplacer class
 *
 * @brief A functional object used to keep track of frames to be replaced 
 *           in page replacement process
 */
class PageReplacer{
public:
  vector<FrameTableEntry*> loadedPages;
  int interval;
  PageTable *pt;

  /*
   * PageReplacer()
   *
   * @brief constructor for PageReplacer
   *
   * @param updateInterval
   *
   */
  PageReplacer(int interval);

  /*
   * addFrameTableEntry
   *
   * @brief virtualAddress of loaded page and its corresponding pfn
   *
   * @return void
   *
   */
  void addFrameTableEntry(unsigned int vAddr, int pfn);

  /*
   * updateLoadedPages()
   *
   * @brief process the loaded pages by updating their bitstring every update interval and
   *          updating the lastAccesedTime of each page
   *
   * @param counter the num of addressed processed
   * @param current virtual address 
   * @param vector of accessedVPNs during update interval
   *
   * @return void
   *
   */
  void updateLoadedPages(int counter, unsigned int, vector<unsigned int> &accessedVPns);

  /*
   * chooseVictimPage()
   *
   * @brief returns a frame entry most optimal to replace 
   *
   * @return FrameTableEntry ptr
   *
   */
  FrameTableEntry* chooseVictimFrame();

};
   

#endif
