// Names: Kaylee Muckerman and Julian To
// REDID: 130526910 and 130405272

/*
 * PageReplacer source file; contains methods of page replacer object 
 *    that calculates and keeps track of pages for replacmeent
 *
 */
#include "pagereplacement.h"


/*
* PageReplacer()
*
* @brief constructor for PageReplacer
*
* @param updateInterval
*
*/
PageReplacer::PageReplacer(int interval) {
  this->interval = interval;
}


/*
* addFrameTableEntry
*
* @brief virtualAddress of loaded page and its corresponding pfn
*
* @return void
*
*/
void PageReplacer::addFrameTableEntry(unsigned int vAddr, int pfn) {
  FrameTableEntry* f = new FrameTableEntry{
        vAddr,      // vpn
        pfn,        // pfn
        1u<<15,     // bitStr
        0};         // lastAccessTime
  loadedPages.push_back(f);
}

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
void PageReplacer::updateLoadedPages(int counter, 
    unsigned int currVAddr,
    vector<unsigned int> &accessedVPNs) {
  
  bool accessed;

  // update lastAccessTime for each loaded page
  for ( FrameTableEntry *fp : loadedPages ) {

    // if vAddr was the one currently accessed, js skip dat hoe and set it to 0
    if ( fp->vpn == currVAddr ) {
      fp->timeSinceLastAccess = 0;
      continue;
    }

    fp->timeSinceLastAccess++;
  }

  if ( counter % interval == 0 ) {
    // update bitstring for each loaded page
    for ( FrameTableEntry *fp : loadedPages ) {

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

/*
 * chooseVictimPage()
 *
 * @brief returns a frame entry most optimal to replace 
 *
 * @return FrameTableEntry ptr
 *
 */
FrameTableEntry* PageReplacer::chooseVictimFrame() {
  FrameTableEntry* victimPage = nullptr;
  int idx;

  for ( size_t i = 0; i < loadedPages.size(); i++ ) {

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

