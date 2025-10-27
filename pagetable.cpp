#include "pagetable.h"

#include <iostream>
#include <cmath>

#include "vaddr_tracereader.h"

PageTable::PageTable(int* bitsPerLevel, int levelCount, int addressSpaceSize) {

  // get level count for future reference
  this->levelCount = levelCount;
  int idx;

  // allocate memory at runtime
  this->shiftAry = new int[this->levelCount];
  this->bitMaskAry = new unsigned int[this->levelCount];
  this->entryCount = new int[this->levelCount];

  this->offset = addressSpaceSize; // set to addressSpaceSize JUST for future calculations

  // initialize shiftAry, bitMaskAry, and offset values
  for (idx = 0; idx < this->levelCount; idx++ ) {
    this->offset -= bitsPerLevel[idx]; // (offset var != actual offset until after loop)
    this->shiftAry[idx] =  this->offset; // reusing this offset var for calculation 
    this->entryCount[idx] = 1 << bitsPerLevel[idx]; // effectively 2^bitsPerLevel[idx]  

      // (1u << bitsPerLevel) - 1:             sets number of bits at that level
      // ((1u << bitsPerLevel) - 1) << shift:  shifts set of bits to proper position
    this->bitMaskAry[idx] = (( 1u << bitsPerLevel[idx] ) - 1u) << shiftAry[idx];

    // initialize rootLevel to inside node if level count is not 1
    if (levelCount > 1) {
      this->rootLevel = new Level{
        this,                          // pageTablePtr
        0,                             // currentdepth
        new Level*[entryCount[0]],     // nextLevel
        NULL,                          // map
      };

    } else { // initialize rootLevel to leaf node if level count is 1
      this->rootLevel = new Level{
        this,                          // pageTablePtr
        0,                             // currentdepth
        NULL,                          // nextLevel
        new Map*[entryCount[0]],       // map
      };

    }
    
  }

  // initialize pageSize based on offset
  this->pageSize = pow(2, offset);
}


Map* PageTable::searchMappedPfn(unsigned int virtualAddress) {
  // walk through this page table!!1
  
  int entryIdx;
  Level* currLevel;
  Map* mapEntry;

  virtualAddress = virtualAddress << offset; // add offset to virtualAddress
  currLevel = rootLevel;

  for (int i = 0; i < levelCount; i++) { 
    entryIdx = (virtualAddress & bitMaskAry[i]) >> shiftAry[i];
    currLevel = currLevel->nextLevel[entryIdx];
    
    if ( currLevel == nullptr ) { // this means that address is unmapped
      return nullptr; 

    }

    if ( i == levelCount - 1 ) { // reaches leaf node
      mapEnty = currLevel->mapArr[entryIdx];

      if ( mapEntry->validFlag != false ) 
        return mapEntry;
      else
        return nullptr; // mapEntry is invalid so return a nullptr

    }

  }
  
  return nullptr; // kind of never runs into this statement
}


int PageTable::insertMapForVpn2Pfn(unsigned int virtualAddress, int frame) {

  return 0; // no error occurs
}


unsigned int PageTable::extractVPNFromVirtualAddress(unsigned int virtualAddress,
                                                     unsigned int mask,
                                                     unsigned int shift) {
  return 1u;
}
