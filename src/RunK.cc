#include "MyConstants.h"
#include "pathUtils.h"
#include "RunK.h"
#include "FileLoader.h"

#include <algorithm>
#include <cassert>
#include <climits>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iterator>
#include <map>
#include <set>
#include <vector>
#include <string>

using std::vector;
using std::map;
using std::set;

/* Does all input parsing and hum-drum vector filling */
void InputSetup(int argc, char *argv[], Problem& problem) {

  if(_db.DEBUGSYN){printf("entering InputSetup\n");}
  if(argc < 4 || argc > 5){
    printf("Usage: ./a.out num_threads likelihoods.xml input.xml [outdir (optional)] \n");
    exit(0);}
  if(argc == 5) { 
    sprintf(_myoutputdir, "%s", argv[4]);
    printf("Directing output files to directory %s\n", _myoutputdir);
  } else {
    printf("No Output directory specified. Default directory is 'output'\n");
    sprintf(_myoutputdir, "%s", "output");
  }
  
  /* If the last character is a "/", strip it off (prevents invalid paths) */
  int len = strlen(_myoutputdir);
  if(len == 0) { printf("ERROR: User provided invalid output directory name \n"); assert(false); }
  if(_myoutputdir[len-1] == '/') {  _myoutputdir[len-1] = '\0';  }

  /* Create specified directory (if it does not already exist) */
  char cmdline[1028];
  sprintf(cmdline, "[ -d \"%s\" ] || mkdir -p \"%s\"", _myoutputdir, _myoutputdir);
  system(cmdline);

  loadBiochemistryFile(argv[2], problem.biochem);
  loadModelFile(argv[3], problem.model);
  validateProblem(problem);

  return;
}
