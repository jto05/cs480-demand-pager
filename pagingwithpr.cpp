#include <stdio.h>
#include <unistd.h>
#include <cstdio>
#include <string.h>

#include <string>
#include <fstream>
#include <iostream>

#include "pagetable.h"
#include "pager.h"
#include "vaddr_tracereader.h"
#include "log_helpers.h"

/*
 * @brief TESTING!!!
 *
 */
int _main() {
  int bitsPerLevel[3] = {8, 8, 8};
  PageTable* pt = new PageTable(bitsPerLevel, 3, 32);
  unsigned int vaddr = 4278189762;
  pt->insertMapForVpn2Pfn(vaddr, 3);

  int n = pt->searchMappedPfn(vaddr)->frameNumber;
  std::cout << n << std::endl;
  return 0;
}

/*
 * @brief Entry point of program; processes cl arguments, 
 *        reads file, and starts worker threads
 *
 * @arg int argc * @arg char** argv
 * @return int
 *
 */
int main(int argc, char **argv) {

  using namespace std;

  int option;                      // command line switch 
  int idx;                         // general index 
  char* filepath;                 // filepath of traces


  // Get any optional flag arguments
  while ( (option = getopt(argc, argv, "s:q:")) != -1 ) {

    switch ( option )  {
    case 'n': // Process only the first N memory accesses / references
      break;

    case 'f': // Number of available physical frames
      break;

    case 'b': // Number of memory accesses between bitstring updates
      break;

    }
  }

  
  filepath = argv[optind];


  // Open and process file
  FILE* fp = fopen( filepath, "r" );
  Pager* pager;
  PageTable* pt;
  
  if (!fp) { // return with error if unable to open file
    printf("Unable to open <<%s>>\n", filepath );
    return 0;
  }

  int bitsPerLevel[3] = {4, 8, 8};

  pt = new PageTable( bitsPerLevel, 3, 32  );

  pager = new Pager(pt, fp, 999999);
  pager->run();
  pager->log(  );


  fclose(fp);



  return 0;
}
