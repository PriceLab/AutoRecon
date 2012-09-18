#ifndef MYCONST_H
#define MYCONST_H

#include <cstdio>
#include <cstdlib>
#include <string>
//#include "DataStructures.h"

#define AR_MAXNAMELENGTH 256

using namespace std;

extern char _myoutputdir[AR_MAXNAMELENGTH];

/* See MyConstants.cc for definitions and values for all of these switches.
   If you want to change a value you must re-compile for it to take effect */
class DEBUGFLAGS{
 public:

  bool TEST_SYNTHESIS;

  int GAPFILL_K;
  int INITIAL_K;
  double ANNOTE_CUTOFF_1;
  double ANNOTE_CUTOFF_2;

  bool DEBUGETC;
  bool DEBUGSYN;
  bool DEBUGPATHS;
  bool DEBUGRUNK;
  bool DEBUGGAPFILL;
  bool DEBUGBRIDGES;
  bool DEBUGFVA;
  bool DEBUGFBA;
  bool PARSIMONY;
  bool OUTPUTPATHRESULTS;
  bool VISUALIZEPATHS;
  bool PRINTGAPFILLRESULTS;
  bool PRINTETCRESULTS;
  bool PRINTANNOTATIONS;
  bool PRINTSHOULDGROW;
  bool SAVEANNOTATIONS;
  int SYNFACTOR;
  int REVFACTOR;
  int MAGICBRIDGEFACTOR;
  int BLACKMAGICFACTOR;
  int MISSINGEXCHANGEFACTOR;
  int MISSINGTRANSPORTFACTOR;
  int BRIDGEFIXFACTOR;
  int ETCFACTOR;
  int MINFACTORSPACING;
  int BRIDGEMETFACTOR;
  int BIOMASS;
  /* Database conventions */
  string E_tag;
  string H_name;
  string Na_name;
  string Na_plus_E;
  string H_plus_E;
  string ATPM_name;
  string ATP_name;
  string ADP_name;
  string H2O_name;
  string PI_name;

  double FLUX_CUTOFF;
  double GROWTH_CUTOFF;

  DEBUGFLAGS();
};

/* Global debugflags variable - should be used in all files in the project instead of each one defning its own copy  */
const DEBUGFLAGS _db;

#endif
