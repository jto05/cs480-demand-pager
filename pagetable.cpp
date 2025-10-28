// Names: Kaylee Muckerman and Julian To
// REDID: 130526910 and 130405272

/*
 * Page table source file; contains methods necessary for pageTable operations
 *
 */
#include "pagetable.h"

#include <cmath>

/*
 * @brief Constructor for page table
 *
 * @param int* bitsPerLevel is an array of amount of bits per level
 * @param int* levelCount is the number of array elements in bitsPerLevel
 * @param int addressSpaceSize ( 32 )
 *
 */
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

    this->rootLevel = new Level{
      this,                          // pageTablePtr
      0,                             // currentdepth
      levelCount == 1 ? nullptr : new Level*[entryCount[0]]{},
      levelCount == 1 ? new Map*[entryCount[0]]{} : nullptr,
    };

    
  }

  // initialize pageSize based on offset
  this->pageSize = pow(2, offset);
}


/* 
 * searchMappedPfn()
 *
 * @brief Search page table at address and return result
 *
 * @param unsigned int virtualAddress
 *
 * @return Map* ptr
 */
Map* PageTable::searchMappedPfn(unsigned int virtualAddress) {
  
  int entryIdx;
  Level* currLevel;
  Map* mapEntry;


  // walk through this page table!!1
  currLevel = rootLevel;
  for (int i = 0; i < levelCount; i++) { 
    if ( currLevel == nullptr )  // this means that address is unmapped
      return nullptr; 

    entryIdx = extractVPNFromVirtualAddress(virtualAddress, bitMaskAry[i], shiftAry[i]);

    if ( i == levelCount - 1 ) { // reaches leaf node
      mapEntry = currLevel->mapArr[entryIdx];

      if ( mapEntry == nullptr ) {
        return nullptr;
      }

      if ( mapEntry->validFlag ) 
        return mapEntry;
      else
        return nullptr; // mapEntry is invalid so return a nullptr
    }

    currLevel = currLevel->nextLevel[entryIdx];
  }
  
  return nullptr; // kind of never runs into this statement
}

/* 
 * insertMapForVpn2Pfn()
 *
 * @brief Insert VPN with VPN to PFN mapping OR update an existing VPN to FPN mapping
 *
 * @param virtualAddress
 * @param frame
 *
 * @return errorCode 
 */
int PageTable::insertMapForVpn2Pfn(unsigned int virtualAddress, int frame) {

  int entryIdx;
  Level* currLevel;
  Map* mapEntry;

  // walk through page table
  currLevel = rootLevel;
  for ( int i = 0; i < levelCount; i++ ) {

    entryIdx = extractVPNFromVirtualAddress(virtualAddress, bitMaskAry[i], shiftAry[i]);

    // if node is an inside node...
    if ( i != levelCount - 1 ) {
      // must initialize level if it is null
      if ( currLevel->nextLevel[entryIdx] == nullptr ) {
        currLevel->nextLevel[entryIdx] = new Level{
          this,                                   
          i + 1,                                       
          (i + 1) == levelCount - 1 ? nullptr : new Level*[entryCount[i+1]]{},
          (i + 1) != levelCount - 1 ? nullptr : new Map*[entryCount[i+1]]{},
        };
      }
    }

    // if we have reached the leaf node...
    if ( i == levelCount - 1 ) {
      mapEntry = currLevel->mapArr[entryIdx]; // get the map entry at entry idx

      // iniitalize entry if its null
      if ( mapEntry == nullptr ) {
        currLevel->mapArr[entryIdx] = new Map{
          frame,
          true,
        };
        return 0; // executed succesfully
      } 

      // else replace frame based on entry's validFlag
      else {
        mapEntry->frameNumber = frame;
        mapEntry->validFlag = true;
        return 0;
      }
    }

    // step into next entry at the new entryIdx
    currLevel = currLevel->nextLevel[entryIdx];

  }

  return 1; // impossible code reach, so return an error if i reach it
}

/* 
* extractVPNFromVirtualAddress()
*
* @brief Get VPN by applying bit mask and shift to given address
*
* @param virtualAddress
* @param mask
* @param shift
*
* @return virtual page number 
*/
unsigned int PageTable::extractVPNFromVirtualAddress(unsigned int virtualAddress,
                                                     unsigned int mask,
                                                     unsigned int shift) {
  return (virtualAddress & mask) >> shift;
}

/* 
* extractFullVPNFromVirtualAddress()
*
* @brief A wrapper to extractVPNFromVirtualAddress to easily obtain
*          the full VPN given a virtualAddress
*
* @param virtualAddress
*
* @return full virtual page numbeer 
*/
unsigned int PageTable::extractFullVPNFromVirtualAddress( unsigned int virtualAddress ) {
  unsigned int fullVPNBitMask = 0;
  for ( int i = 0; i < levelCount; i++ ) {
    fullVPNBitMask |= bitMaskAry[i];
  }

  return extractVPNFromVirtualAddress( virtualAddress, fullVPNBitMask, offset );
}

/* 
* getTotalPgTableEntries()
*
* @brief A recursive function that counts every entry in page table
*
* @param Level* to starting level 
*
* @return the total amount of page entries
*/
unsigned long int PageTable::getTotalPgTableEntries( Level* currLevel ) {
  unsigned long int sum;
  int depth;

  depth = currLevel->currentDepth;
  sum = entryCount[depth];
  
  if ( depth != levelCount - 1 
      && currLevel->nextLevel != nullptr ) {
    for ( int i = 0; i < entryCount[depth]; i++ ) {
      if ( currLevel->nextLevel[i] != nullptr ) {
        sum += getTotalPgTableEntries(currLevel->nextLevel[i]);
      }
    }
  }

  return sum;
}

