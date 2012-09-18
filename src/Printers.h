#ifndef PRINTERS_H
#define PRINTERS_H

/* printers.h - various utilities for printing results */

#include <vector>
#include <set>
#include "DataStructures.h"

using std::vector;
using std::set;

/* Basic printers */
void printIntVector(vector<int> intVector);
void printVector(vector<RXNID> intVector);
void printVector(vector<REV> intVector);
void printVector(vector<RXNDIRID> intVector);
void printVector(vector<METID> intVector);
void printDoubleVector(vector<double> doubleVector);
void printIntSet(const set<int> &mySet);
void printIntMap(map<int, int> intMap);

/* Reaction and metabolite printers */
void printGROWTHinputs(const GROWTH &growth);
void printRxnFormula(const REACTION &rxn, string stoichString, bool printStoichPart);

void printMetsFromIntVector(const vector<int> &intVector, const PROBLEM &ProblemSpace);
void printMETABOLITEinputs(const METABOLITE &metabolite);
void printIntMap_mets(const METSPACE &metspace, const map<int, int> &intMap);
void printDoubleVector_mets(const METSPACE &metspace, const vector<double> &doubleVec);

void printMetsFromStoich(vector<STOICH> a);
void printDoubleVector_rxns(const RXNSPACE &rxnspace, const vector<double> &doubleVec);
void printSTOICHIOMETRY_by_id(const vector<STOICH> &stoich, int num_met, int rev);
void printRxnsFromIntVector(const vector<RXNID> &intVector, const RXNSPACE &rxnspace);
void printRxnsFromIntVector(const vector<RXNDIRID> &intVector, const RXNSPACE &rxnspace);
void printRxnsFromIntSet(const set<int> &intSet, const RXNSPACE &rxnspace);
void printSynRxns(const RXNSPACE &synrxns, const RXNSPACE &fullrxns);
void printREACTIONinputs(const REACTION &reaction, int print_type);
void printREACTIONvector(const vector<REACTION> &reaction, int print_type);
void printREACTIONintermediates(const REACTION &reaction, int print_type);

/* Print ETCs (note - they come from fullrxns) */
void printNetReactionVector(const vector<NETREACTION> &netReactions, const PROBLEM &problemSpace);

/* Path related printers */
void PrintPathSummary_verbose(const PATHSUMMARY &psum);
void PrintPathSummary(const vector<vector<vector<PATHSUMMARY> > > &psum);
void PrintPathSummary2(const vector<vector<vector<PATHSUMMARY> > > &psum, RXNSPACE &rxnspace);
void PrintPathSummary2(const vector<vector<vector<PATHSUMMARY> > > &psum, PROBLEM &ProblemSpace);

void printPathResults(const vector<PATH> &path);
void printPathResults(const vector<PATH> &path, PROBLEM &ProblemSpace);
void printPathResults(const vector<PATH> &path, PROBLEM &ProblemSpace, RXNSPACE &rxnspace);

void PrintGapfillResult(const vector<GAPFILLRESULT> &res, const PROBLEM &problemSpace, const vector<int> &kToPrint);

/* Generate output files */
void MATLAB_out(const string fileName, const vector<REACTION> &InRxns);
void PATHS_rxns_out(const string filename, const vector<PATHSUMMARY> &psum, const PROBLEM &problem);
void PATHS_mets_out(const string filename, const vector<PATHSUMMARY> &psum, const PROBLEM &problem);
void ONEPATH_adj_list(const string fileName, const PATHSUMMARY &psum, const PROBLEM &problem);
void ANNOTATIONS_out(const string filename, const vector<REACTION> &annotated_reaction_list);

#endif
