// Names: Kaylee Muckerman and Julian To
// REDID: 130526910 and 130405272

/*
 * Source code for Pager class; simulates MMU and demand paging from OS
 *
 */

#include "pager.h"

#include <cstdio>
#include <vector>

#include "vaddr_tracereader.h"
#include "log_helpers.h"
#include "pagereplacement.h"

using namespace std;

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
Pager::Pager(PageTable *pt, 
    FILE *fp, 
    int numOfAddresses, 
    int availableFrames, 
    int NFUInterval, 
    LogOptionsType logOptions) { 

  this->pageTable = pt;
  this->filePtr = fp;
  this->nextFreeFrame = 0;
  this->availableFrames = availableFrames;
  this->numOfAddresses = numOfAddresses;
  this->NFUInterval = NFUInterval;
  this->logOptions = logOptions;
}


/*
 * run()
 * 
 * @brief main loop of pager object; the main function called by user
 *
 * @return void
 *
 */
void Pager::run() {

  // non-member variables used exclusively in run()
  PageReplacer *pageReplacer = new PageReplacer(NFUInterval);
  vector<unsigned int> accessedVPNs;
  int freedPfn;
  unsigned int currVPN;
  Map *mapEntry;
  FrameTableEntry* victim;
  int addressCounter = 0;


  // vaddr_tracereadr variables
  p2AddrTr mtrace;

  // logging variables
  unsigned int pframe;
  int vpn_replaced;
  unsigned int victim_bitstring;
  bool pthit;
      
  while ( NextAddress(filePtr, &mtrace) ) { 
    if ( addressCounter == (int) numOfAddresses ) {
      break;
    }

    currVA = mtrace.addr;
    currVPN = pageTable->extractFullVPNFromVirtualAddress(currVA);
    addressCounter++; // update counter

    vpn_replaced = -1;


    mapEntry = pageTable->searchMappedPfn(currVA);
    if (mapEntry) {
      accessedVPNs.push_back(currVPN);
      pageReplacer->updateLoadedPages(addressCounter, currVPN, accessedVPNs );
      pageTableHits++;

      // logging
      pthit = true;
      vpn_replaced = -1;
      victim_bitstring = 0;
      pframe = mapEntry->frameNumber;

    }  

    // if vpn does not exist in pagetable!!!
    else {

      // update page replacer before anything
      accessedVPNs.push_back(currVPN);
      pageReplacer->updateLoadedPages(addressCounter, currVPN, accessedVPNs );

      // if there is available space
      if ( nextFreeFrame < availableFrames ) {

        // insert currVA
        pageTable->insertMapForVpn2Pfn(currVA, nextFreeFrame);

        // new entry added, so new frame added to loadedPages!!!
        pageReplacer->addFrameTableEntry(currVPN, nextFreeFrame);
        numOfFramesAllocated++;


        // logging
        pthit = false;
        vpn_replaced = -1;
        victim_bitstring = 0;
        pframe = nextFreeFrame;

        nextFreeFrame++;

      } else { // if there is no space, perform replacement
        // obtain victim and its pfn
        victim = pageReplacer->chooseVictimFrame();
        freedPfn = victim->pfn;


        // set victim's flag to false so OS knows its invalid
        mapEntry = pageTable->searchMappedPfn(victim->vpn << pageTable->offset);
        mapEntry->validFlag = false;

        // insert currVA along with newly freedPFN
        pageTable->insertMapForVpn2Pfn(currVA, freedPfn);

        // add to pagereplacer
        pageReplacer->addFrameTableEntry(currVPN, freedPfn);

        // so that we know there are still no avialbe frames
        nextFreeFrame = availableFrames + 1; 
        numOfPageReplaces++;


        //logging
        pthit = false;
        vpn_replaced = victim->vpn;
        victim_bitstring = victim->bitStr;
        pframe = freedPfn;
      }

    }

    // logging options to at every address!
    unsigned int offMask = ( 1 << pageTable->offset ) - 1;
    unsigned int offsetBits = currVA & offMask;
    unsigned int pa = pframe;

    if ( logOptions.addressTranslation ) {
      pa = (pframe << pageTable->offset) | offsetBits;

      log_va2pa(currVA, pa);
    }

    if ( logOptions.offset ) {
      print_num_inHex(offsetBits);
    }

    if ( logOptions.vpns_pfn ) {
      uint32_t vpns[pageTable->levelCount];
      for ( int i = 0; i < pageTable->levelCount; i ++ ) {
        vpns[i] = pageTable->extractVPNFromVirtualAddress(currVA, 
                                        pageTable->bitMaskAry[i],  
                                        pageTable->shiftAry[i] );
      }

      log_vpns_pfn(pageTable->levelCount, vpns, pa);
    }

    if (logOptions.vpn2pfn_with_pagereplace)
      log_mapping(currVPN, pframe, vpn_replaced, victim_bitstring, pthit);

  }

  numOfAddresses = addressCounter;

  pgtableEntries = pageTable->getTotalPgTableEntries( pageTable->rootLevel );
}

/*
 * log()
 * 
 * @brief short hand to to call log_summary from log_helpers.h w/o its arguments 
 *
 * @return void
 *
 */
void Pager::log() {
  log_summary( pageTable->pageSize,
      numOfPageReplaces,
      pageTableHits,
      numOfAddresses,
      numOfFramesAllocated,
      pgtableEntries
  );

}


