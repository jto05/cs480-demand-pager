#include "pagetable.h"

#include <iostream>
#include <cmath>

#include "vaddr_tracereader.h"

PageTable::PageTable(int* bitsPerLevel, int levelCount, int addressSpaceSize) {
  using namespace std;

  Level levelValue;
  int offset;
  int idx;

  // get level count for future reference
  this->levelCount = levelCount;

  // allocate memory at runtime
  this->shiftAry = new int[this->levelCount];
  this->bitMaskAry = new unsigned int[this->levelCount];
  this->entryCount = new int[this->levelCount];

  offset = addressSpaceSize;

  // initialize shiftAry, bitMaskAry, and offset values
  for (idx = 0; idx < this->levelCount; idx++ ) {
    offset -= bitsPerLevel[idx];
    this->shiftAry[idx] =  offset;
    this->entryCount[idx] = pow(2, bitsPerLevel[idx]);

      // (1u << bitsPerLevel) - 1:             sets number of bits at that level
      // ((1u << bitsPerLevel) - 1) << shift:  shifts set of bits to proper position
    this->bitMaskAry[idx] = (( 1u << bitsPerLevel[idx] ) - 1u) << shiftAry[idx];

    // initialize rootLevel to inside node if level count is not 0
    if (levelCount > 1) {
      int rootLevelSize = entryCount[0];
      levelValue = {
        this,                          // pageTablePtr
        0,                             // currentdepth
        new Level*[rootLevelSize],     // nextLevel
        NULL,
      };
      this->rootLevel = &levelValue;

    } else { // initialize rootLevel to leaf node if level count is 0
      int rootLevelSize = entryCount[0];
      levelValue = {
        this,                          // pageTablePtr
        0,                             // currentdepth
        NULL,
        new Map[rootLevelSize],
      };
      this->rootLevel = &levelValue;

    }
    
  }

  // initialize pageSize based on offset
  this->pageSize = pow(2, offset);
}


Map* PageTable::searchMappedPfn(unsigned int virtualAddress) {
  return nullptr;
}


int PageTable::insertMapForVpn2Pfn(unsigned int virtualAddress, int frame) {
  return 0; // no error occurs
}


unsigned int PageTable::extractVPNFromVirtualAddress(unsigned int virtualAddress,
                                                     unsigned int mask,
                                                     unsigned int shift) {
  return 1u;
}
