#include "pagereplacement.h"


PageReplacer::PageReplacer(int interval) {
  this->interval = interval;
}

void PageReplacer::addNFUFrame(unsigned int vAddr, int pfn) {
  NFUFrame* f = new NFUFrame{
        vAddr,      // vpn
        pfn,        // pfn
        1u<<15,     // bitStr
        0};         // lastAccessTime
  loadedPages.push_back(f);
}

void PageReplacer::updateLoadedPages(int counter, 
    unsigned int currVAddr,
    vector<unsigned int> &accessedVPNs) {
  
  bool accessed;

  // update lastAccessTime for each loaded page
  for ( NFUFrame *fp : loadedPages ) {

    // if vAddr was the one currently accessed, js skip dat hoe and set it to 0
    if ( fp->vpn == currVAddr ) {
      fp->timeSinceLastAccess = 0;
      continue;
    }

    fp->timeSinceLastAccess++;
  }

  if ( counter % interval == 0 ) {
    // update bitstring for each loaded page
    for ( NFUFrame *fp : loadedPages ) {

      fp->bitStr = fp->bitStr >> 1; // first right shift all bitstr
      accessed = false;

      // check if current frame was accessed during interval
      for ( unsigned int addr : accessedVPNs ) {
        if ( addr == fp->vpn ) accessed = true;
      }
 
      // sets most significant bit of bitstr to 1 if accessed in interval
      if (accessed) 
        fp->bitStr = fp->bitStr | ( 1u << 15 ); 
    }

    accessedVPNs.clear(); // clear arr of accessed vpns for next interval

  } 
}


NFUFrame* PageReplacer::chooseVictimFrame() {
  NFUFrame* victimPage = nullptr;
  int idx;

  for ( int i = 0; i < loadedPages.size(); i++ ) {

    // initialize victimPage if null to curr frame
    if ( victimPage == nullptr ) {
      victimPage = loadedPages[i];
      idx = i;
      continue;
    }

    // compare bitstrs to determine currVictipage
    if ( victimPage->bitStr > loadedPages[i]->bitStr ) {
      victimPage = loadedPages[i];
      idx = i;
      continue;
    }

    // resolve tie 
    if ( victimPage->bitStr == loadedPages[i]->bitStr ) {
      if ( victimPage->timeSinceLastAccess < loadedPages[i]->timeSinceLastAccess ) {
        victimPage = loadedPages[i];
        idx = i;
        continue;
      }
    }

  }

  // remove victim frame from loaded pages
  loadedPages.erase(loadedPages.begin() + idx); 

  return victimPage;

}

