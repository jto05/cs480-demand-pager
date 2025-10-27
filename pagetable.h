#ifndef PAGETABLE_H
#define PAGETABLE_H

#include <iostream>
#include <map>
#include <vector>

using namespace std;

// forward declaration for compiler
class PageTable; 
struct Map;
struct Level;

/*
 * Map
 */
struct Map {
  int frameNumber;
  bool validFlag;
};

/*
 * The Level struct is the page tree node representation
 */
struct Level {
  PageTable* pageTablePtr;
  int currentDepth;

  Level** nextLevel;  // arr of Level objects that represent the next levels
  Map** mapArr;        // 
};

/*
 * PageTable
 */
class PageTable {
  private:
    int levelCount;             // number of levels
    unsigned int* bitMaskAry;   // array of bitmasks indexed by level
    int* shiftAry;              // array of num of bits to shift indexed by level
    int* entryCount;            // array of num of next level entries indexed by level
    int pageSize;               // size of each page
    int offset;
  public:
    Level* rootLevel;           // root node of pagte table's level tree
    /*
     * @brief constructor for page table
     *
     * @param BLAH!! IDK YET!!
     *
     */
    PageTable( int* bitsPerLevel, int levelCount, int addressSpaceSize );

    /* 
     * @brief Search page table at address and return result
     *
     * @param virtualAddress
     *
     * @return Map
     */
    Map* searchMappedPfn(unsigned int virtualAddress);

    /* 
     * @brief Insert VPN with VPN to PFN mapping OR updating an existing VPN to FPN mapping
     *
     * @param virtualAddress
     * @param frame
     *
     * @return errorCode 
     */
    int insertMapForVpn2Pfn(unsigned int virtualAddress, int frame);

    /* 
     * @brief Get VPN by applying bit mask and shift to given address
     *
     * @param virtualAddress
     * @param mask
     * @param shift
     *
     * @return errorCode 
     */
    unsigned int extractVPNFromVirtualAddress(unsigned int virtualAddress,
                                               unsigned int mask,
                                               unsigned int shift);

    unsigned long int getTotalPgTableEntries( Level* currLevel);

    int getPageSize();
};


#endif
