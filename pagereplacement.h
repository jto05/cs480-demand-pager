#ifndef PAGEREPLACEMENT_H
#define PAGEREPLACEMENT_H

#include <iostream>
#include <vector>

#include "pagetable.h"

using namespace std;

struct NFUFrame{
  unsigned int vpn;
  int pfn;
  unsigned int bitStr;
  int timeSinceLastAccess;
};

class PageReplacer{
public:
  vector<NFUFrame*> loadedPages;
  int interval;
  PageTable *pt;

  PageReplacer(int interval);

  void addNFUFrame(unsigned int vAddr, int pfn);
  void updateLoadedPages(int counter, unsigned int, vector<unsigned int> &accessedVPns);
  NFUFrame* chooseVictimFrame();

};
   

#endif
