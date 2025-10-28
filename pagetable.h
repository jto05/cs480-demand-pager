// Names: Kaylee Muckerman and Julian To
// REDID: 130526910 and 130405272

/*
 * Page table header; defines necessary structs for page table implementation
 *
 */

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
 * Map struct
 *
 * @brief Contains the mapping of vpn at leaf node to physical frame
 *         number and validity bit
 *
 */
struct Map {
  int frameNumber;
  bool validFlag;
};

/*
 * Level struct
 *
 * @brief The node representation of the page table. Inside nodes contain
 *         Level ptr arrays while the leaf nodes contain map ptr arrays
 *
 */
struct Level {
  PageTable* pageTablePtr;
  int currentDepth;

  Level** nextLevel;  // arr of Level objects that represent the next levels
  Map** mapArr;        // 
};

/*
 * PageTable class
 *
 * @brief The object that encapsulates Level and Map; contains methods
 *          for necessary page table operations
 *
 */
class PageTable {
  public:
    int levelCount;             // number of levels
    unsigned int* bitMaskAry;   // array of bitmasks indexed by level
    int* shiftAry;              // array of num of bits to shift indexed by level
    int* entryCount;            // array of num of next level entries indexed by level
    int pageSize;               // size of each page
    int offset;
    Level* rootLevel;           // root node of pagte table's level tree

    /*
     * PageTable()
     *
     * @brief Constructor for page table
     *
     * @param int* bitsPerLevel is an array of amount of bits per level
     * @param int* levelCount is the number of array elements in bitsPerLevel
     * @param int addressSpaceSize ( 32 )
     *
     */
    PageTable( int* bitsPerLevel, int levelCount, int addressSpaceSize );

    /* 
     * searchMappedPfn()
     *
     * @brief Search page table at address and return result
     *
     * @param unsigned int virtualAddress
     *
     * @return Map* ptr
     */
    Map* searchMappedPfn(unsigned int virtualAddress);

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
    int insertMapForVpn2Pfn(unsigned int virtualAddress, int frame);

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
    unsigned int extractVPNFromVirtualAddress(unsigned int virtualAddress,
                                               unsigned int mask,
                                               unsigned int shift);

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
    unsigned int extractFullVPNFromVirtualAddress( unsigned int virtualAddress ); 

    /* 
     * getTotalPgTableEntries()
     *
     * @brief A recursive function that counts every entry in page table
     *
     * @param Level* to starting level 
     *
     * @return the total amount of page entries
     */
    unsigned long int getTotalPgTableEntries( Level* currLevel);
};


#endif
