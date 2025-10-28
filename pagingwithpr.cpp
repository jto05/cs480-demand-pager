// Names: Kaylee Muckerman and Julian To
// REDID: 130526910 and 130405272

/*
 * Main entry point of program; processes CLI arguments and 
 *  initializes pager object and page table according to arguments
 *
 */

#include <stdio.h>
#include <unistd.h>
#include <cstdio>
#include <string.h>

#include <string>
#include <fstream>
#include <iostream>
#include <cstring>

#include "pagetable.h"
#include "pager.h"
#include "vaddr_tracereader.h"
#include "log_helpers.h"

/*
 * @brief Entry point of program; processes cl arguments, 
 *        begins pager simulation
 *
 @arg int argc * @arg char** argv
 * @return int
 *
 */
int main(int argc, char **argv) {

  using namespace std;

  int option;                      // command line switch 
  int idx;                         // general index 
  char* filepath;                 // filepath of traces
  int numOfAddresses = -1;  
  int numOfPhysicalFrames = 999999;  
  int updateInterval = 10;

  LogOptionsType logOptions;
  logOptions.pagetable_bitmasks = false;
  logOptions.addressTranslation = false;
  logOptions.vpns_pfn = false;
  logOptions.vpn2pfn_with_pagereplace = false;
  logOptions.offset = false;
  logOptions.summary = true;


  // Get any optional flag arguments
  while ( (option = getopt(argc, argv, "n:f:b:l:")) != -1 ) {

    switch ( option )  {
    case 'n': // Process only the first N memory accesses / references
      numOfAddresses = atoi(optarg);

      if ( numOfAddresses < 1 ) {
        printf("Number of memory accesses must be a number and greater than 0\n");
        return 0;
      }

      break;

    case 'f': // Number of available physical frames
      numOfPhysicalFrames = atoi(optarg);

      if ( numOfPhysicalFrames < 1 ) {
        printf("Number of available frames must be a number and greater than 0\n");
        return 0;
      }

      break;

    case 'b': // Number of memory accesses between bitstring updates
      updateInterval = atoi(optarg);

      if ( updateInterval < 1 ) {
        printf("Bit string update interval must be a number and greater than 0\n");
        return 0;
      }

      break;

    case 'l': // Log option type

      if ( strcmp(optarg, "bitmasks") == 0 ) {
        logOptions.pagetable_bitmasks = true;
        logOptions.summary = false;

      } else if (  strcmp(optarg, "va2pa") == 0 ) {
        logOptions.addressTranslation = true;
        logOptions.summary = false;

      } else if (  strcmp(optarg, "vpns_pfn")  == 0) {
        logOptions.vpns_pfn = true;
        logOptions.summary = false;

      } else if (  strcmp(optarg, "vpn2pfn_pr") == 0 ) {
        logOptions.vpn2pfn_with_pagereplace = true;
        logOptions.summary = false;

      } else if ( strcmp(optarg, "offset") == 0) {
        logOptions.offset = true;
        logOptions.summary = false;

      } else {

      }

      break;
    }
  }

  filepath = argv[optind];

  // get bits per level
  vector<int> bitsPerLevel;
  idx = optind + 1;
  int count = 0;
  do {
    if ( atoi(argv[idx]) < 1 )  {
      printf("Level %d page table must be at least 1 bit\n", count);
      return 0;
    }

    bitsPerLevel.push_back( atoi(argv[idx]) );
    idx++;
    count++;
  } while ( idx != argc );

  // sum total bits
  int totalBits = 0;
  for ( int bits : bitsPerLevel ) {
    totalBits += bits;
  }

  if ( totalBits > 28 ) {
    printf("Too many bits used in page tables\n");
    return 0;
  }


  // Open and process file
  FILE* fp = fopen( filepath, "r" );
  Pager* pager;
  PageTable* pt;
  
  if (!fp) { // return with error if unable to open file
    printf("Unable to open <<%s>>\n", filepath );
    return 0;
  }


// ./pagingwithpr -n 100000 -f 40 -l summary trace.tr 4 4 10
// ./pagingwithpr -n 50 -f 20 -b 10 -l vpn2pfn_pr trace.tr 6 6 8

  pt = new PageTable( bitsPerLevel.data(), bitsPerLevel.size(), 32 );

  pager = new Pager(pt, fp, numOfAddresses, numOfPhysicalFrames, updateInterval, logOptions);

  // bitmasks
  if ( logOptions.pagetable_bitmasks ) {
    log_bitmasks( pt->levelCount, pt->bitMaskAry );
  } 

  // all other log options
  else {
    pager->run();
  }


  if ( logOptions.summary )
    pager->log();

  fclose(fp);



  return 0;
}
