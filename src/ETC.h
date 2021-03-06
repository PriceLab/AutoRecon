#ifndef ETC_H
#define ETC_H

#include "DataStructures.h"
#include "shortestPath.h"
#include "pathUtils.h"
#include <vector>

/* Main functions */
void ETC(const PROBLEM &ProblemSpace, vector<NETREACTION> &bigout, double cutoff);
void identifyETCrxns(const PROBLEM &ProblemSpace, vector<RXNID> &ETClist);
void identifyETCrxns(const PROBLEM &ProblemSpace, vector<RXNID> &ETClist, double cutoff);
vector<NETREACTION> ETC_dir_check(const RXNSPACE &fullrxnspace, const vector<NETREACTION> &bigout);

/* Utilities */
void decompose(const vector<RXNDIRID> &rxnDirIds, vector<RXNID> &rxnIds,  vector<REV> &rxnDirs);
void flip(vector<REV> &rxnDirs);
NETREACTION flip(const NETREACTION &one);
void convert(NETREACTION &net, REACTION add, int rxnDirId);
vector<vector<METID> > PossiblePairs(const vector<STOICH> &fromnet,  const METSPACE &cofactors);
int netrxn(NETREACTION &net, const PROBLEM &ProblemSpace, int rxnDirId);

#endif
