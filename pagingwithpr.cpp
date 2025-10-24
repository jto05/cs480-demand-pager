#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include <string>
#include <fstream>
#include <iostream>

/*
 * @brief Entry point of program; processes cl arguments, 
 *        reads file, and starts worker threads
 *
 * @arg int argc
 * @arg char** argv
 * @return int
 *
 */
int main(int argc, char **argv) {

  using namespace std;

  int option;                      // command line switch 
  int idx;                         // general index 
  string filepath;                 // filepath of traces


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
  ifstream file(filepath);  // open file at filepath
  string line;              // where file line is read to 
  
  if (!file.is_open()) { // return with error if unable to open file
    printf("Unable to open <<%s>>\n", filepath.c_str() );
    return 0;
  }

  while ( getline(file, line) ) { 
    // READ DAT FILE!!!!
  } 

  file.close();

}
