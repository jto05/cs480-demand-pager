#include "pager.h"

#include <cstdio>
#include <vector>

#include "vaddr_tracereader.h"
#include "log_helpers.h"
#include "pagereplacement.h"

using namespace std;

Pager::Pager(PageTable *pt, FILE *fp, int numOfAddresses, int availableFrames, int NFUInterval) { 
  this->pageTable = pt;
  this->filePtr = fp;
  this->nextFreeFrame = 0;
  this->availableFrames = availableFrames;
  this->numOfAddresses = numOfAddresses;
  this->NFUInterval = NFUInterval;
}

void Pager::run() {

  PageReplacer *pageReplacer = new PageReplacer(NFUInterval);
  vector<unsigned int> accessedVPNs;
  int freedPfn;
  unsigned int currVPN;

  Map *mapEntry;
  NFUFrame* victim;
  int err;
  int addressCounter;

  p2AddrTr mtrace;
  int c;

  // logging
  unsigned int pframe;
  int vpn_replaced;
  unsigned int victim_bitstring;
  bool pthit;
      

  while ( !feof(filePtr) ) {
    vpn_replaced = -1;

    if ( addressCounter == numOfAddresses ) {
      break;
    }

    if ( NextAddress(filePtr, &mtrace) ) {
      currVA = mtrace.addr;
      currVPN = pageTable->extractFullVPNFromVirtualAddress(currVA);
      addressCounter++; // update counter
    } else{
      continue; // skip when no address is found
    }



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
        pageReplacer->addNFUFrame(currVPN, nextFreeFrame);

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
        pageReplacer->addNFUFrame(currVPN, freedPfn);

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

    log_mapping(currVPN, pframe, vpn_replaced, victim_bitstring, pthit);

  }

  numOfAddresses = addressCounter;

  pgtableEntries = pageTable->getTotalPgTableEntries( pageTable->rootLevel );

}

void Pager::log() {
  log_summary( pageTable->getPageSize(),
      numOfPageReplaces,
      pageTableHits,
      numOfAddresses,
      numOfFramesAllocated,
      pgtableEntries
  );

}


