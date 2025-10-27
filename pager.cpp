#include "pager.h"

#include <cstdio>

#include "vaddr_tracereader.h"
#include "log_helpers.h"


Pager::Pager(PageTable *pt, FILE *fp, int availableFrames) { 
  this->pageTable = pt;
  this->filePtr = fp;
  this->nextFreeFrame = 0;
  this->availableFrames = availableFrames;
}

void Pager::run() {

  Map *mapEntry;
  int err;
  int c;
  p2AddrTr mtrace;
  
  while ( !feof(filePtr) ) {
    if ( NextAddress(filePtr, &mtrace) ) {
      currVA = mtrace.addr;
      numOfAddresses++;
    } else{
      continue;
    }
    print_num_inHex(currVA);

    // mapEntry = pageTable->searchMappedPfn(vAddr);
    if ( pageTable->searchMappedPfn(currVA) != nullptr ) {
      pageTableHits++;
    } else { // bc entry doesn't exist, must allocate new frame
      numOfFramesAllocated++;
    }

    pageTable->insertMapForVpn2Pfn(currVA, nextFreeFrame);
    nextFreeFrame++;

  }

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


