#ifndef PATHS2MODEL_H
#define PATHS2MODEL_H

#include "shortestPath.h"
#include "Grow.h"
#include "genericLinprog.h"
#include "pathUtils.h"
#include "visual01.h"
#include "DataStructures.h"
#include "RunK.h"

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <map>

/* Unsynonymize */
void addR(PATHSUMMARY &psum, PROBLEM &ProblemSpace);
void addR(vector<vector<vector<PATHSUMMARY> > > &psum, PROBLEM &ProblemSpace);

vector<RXNID> findRxnsToIncludeFromSyn(const REACTION &TMPSYN, const RXNSPACE &rxnspace, REV dir);
PATHSUMMARY replaceWithRealRxnIds(const PATHSUMMARY &psum, const vector<PATHSUMMARY> &pList, PROBLEM &ProblemSpace);
vector<RXNID> fillMagicBridges(const PATHSUMMARY &psum, const vector<PATHSUMMARY> &pList, 
			     const PROBLEM &ProblemSpace, RXNDIRID id, 
			     vector<RXNID> &filledMagicBridges);

/* Random utility functions */
void checkExchangesAndTransporters(PROBLEM &working, const PROBLEM &ProblemSpace, const vector<METID> &metIds, const vector<int> &dirs);
void makeSimulatableModel(const vector<PATHSUMMARY> &pList, const PROBLEM &ProblemSpace, const REACTION &biomass, const vector<METID> &magicIds, 
			  const vector<REV> &magicDirs,
                          PROBLEM &working, int &baseNum);


#endif
