#include "DataStructures.h"
#include "pathUtils.h"
#include "Metabolite.h"
#include "Reaction.h"
#include "Stoich.h"

#include <assert.h>
#include <cstring>
#include <cstdio>
#include <map>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>		// for perror()

using std::map;
using std::vector;
using std::strcpy;
using std::sprintf;
using namespace std;

vector<NETREACTION> ANSWER::etc;
vector<vector<vector<PATHSUMMARY> > > ANSWER::pList;
map<string, vector<VALUESTORE> > ANSWER::annoteToRxns;

bool ANNOTATION::operator<(const ANNOTATION &rhs)const {
  return this->probability > rhs.probability;
}



bool NETREACTION::operator==(const NETREACTION &rhs) const{
  unsigned int i;
  if(this[0].rxnDirIds.size()!=rhs.rxnDirIds.size()){
    return false;}
  if(this[0].rxn.stoich_part.size()!=rhs.rxn.stoich_part.size()){
    return false;}
  for(i=0;i<rhs.rxnDirIds.size();i++){
    if(this[0].rxnDirIds[i]!=rhs.rxnDirIds[i]){return false;}}
  return true;
}

bool MEDIA::operator==(const MEDIA &rhs) const{
  return (this[0].id == rhs.id);
}
bool MEDIA::operator>(const MEDIA &rhs) const{
  return (this[0].id > rhs.id);
}
bool MEDIA::operator<(const MEDIA &rhs) const{
  return (this[0].id < rhs.id);
}

MEDIA::MEDIA() {
  id = -1;
  rate = 1000.0f;
}

GROWTH::GROWTH() {
  growth_rate = -1;
}
void GROWTH::reset() {
  media.clear();
  byproduct.clear();
  biomass.clear();
  mutation.clear();
  growth_rate = -1;
}

/* I think I'm required to put this here...even if it does nothing*/
RXNSPACE::RXNSPACE() {
  numRxns = 0;
  currentAtpm = 0.0f;
}

RXNSPACE::RXNSPACE(const vector<REACTION> &rxnVec) {
  
  for(int i=0;i<rxnVec.size();i++){
    rxns.push_back(rxnVec[i]);
  }
  numRxns = rxns.size();
  for(int i=0; i<rxns.size(); i++) {
    Ids2Idx[rxns[i].id] = i;
  }
}

RXNSPACE::RXNSPACE(const RXNSPACE& existingSpace, const vector<int> &idSubset) {
  for(int i=0; i<idSubset.size(); i++) {
    REACTION rxn = existingSpace.rxnFromId(idSubset[i]);
    rxns.push_back(rxn);
    Ids2Idx[rxn.id] = i;
  }
  numRxns = rxns.size();
}

void RXNSPACE::clear() {
  rxns.clear();
  Ids2Idx.clear();
  numRxns = 0;
}

REACTION & RXNSPACE::operator[](int idx) {
  assert(idx < rxns.size());
  return this->rxns[idx];
}

/* Note - I implemented this myself so that I automatically reserve the capacity... lets see if it helps make this more efficient */
RXNSPACE RXNSPACE::operator=(const RXNSPACE &orig) {
  if(&orig != this) {
    clear();
    rxns.reserve(orig.rxns.capacity());
    for(int i=0; i<orig.rxns.size(); i++) {
      addReaction(orig.rxns[i]);
    }
  }
  return *this;
}

/* Note - the two RXNSPACE must have all the reactions in the same ORDER in order to count */
bool RXNSPACE::operator==(const RXNSPACE &rhs) {
  if(this->rxns.size() != rhs.rxns.size()) { return false; }
  if(this->Ids2Idx.size() != rhs.Ids2Idx.size()) { return false; }

  for(int i=0; i<this->rxns.size(); i++) {
    if(!(this->rxns[i] == rhs.rxns[i])) { return false; }
  }

  return true;
}

void RXNSPACE::addReactionVector(const vector<REACTION> &rxnVec) {
  for(int i=0; i<rxnVec.size(); i++) {
    addReaction(rxnVec[i]);
  }
}

void RXNSPACE::addReaction(const REACTION &rxn) {
  if(idIn(rxn.id)) {
    //printf("WARNING: Attempted to pass a reaction with id %d that was already in the RXNSPACE!\n", rxn.id);
    return;
  }
  rxns.push_back(rxn);
  Ids2Idx[rxn.id] = rxns.size()-1;
  numRxns++;
}

void RXNSPACE::removeRxnFromBack() {
  assert(rxns.size() > 0);
  int id = rxns.back().id;
  int idx = Ids2Idx[id];
  rxns.pop_back();
  Ids2Idx.erase(id);
  numRxns--;
}

void RXNSPACE::changeId(int oldId, int newId) {
  if(idIn(newId)) { printf("ERROR: Request to change ID to an ID already taken by a reaction in the RXNSPACE (in RXNSPACE::changeID)\n"); assert(false); }
  int idx = idxFromId(oldId);
  rxns[idx].id = newId;
  Ids2Idx.erase(oldId);
  Ids2Idx[newId] = idx;
}

/* NOTE (IMPORTANT): For reverse compatibility, :

   1: I DEFINE negative reversibility to be
   the SAME thing as "ub <= 0" and positive reversibility to
   be the SAME thing as "lb >= 0". If lb < 0 and ub > 0 the net_reversble is 0

   Also, 2: I KEEP THE S MATRIX THE SAME ALL THE TIME. This makes it much
   easier to keep track of the large number of RXNSPACE running around.
   
*/
void RXNSPACE::changeReversibility(int id, int new_rev) {
  REACTION* ptr = rxnPtrFromId(id);
  /* new_rev = -1: set ub = 0 */
  if(new_rev == -1) { ptr->lb = -1000.0f; ptr->ub = 0.0f;  }
  if(new_rev == 0)  { ptr->lb = -1000.0f; ptr->ub = 1000.0f; }
  if(new_rev == 1)  { ptr->lb = 0.0f    ; ptr->ub = 1000.0f; }
  ptr->net_reversible = new_rev;
}

/* Change the lb of the reaction to new_lb
   Change net_reversible to +1 if the lb is > 0 and
   0 if the lb is < 0 and the ub is > 0 */
void RXNSPACE::change_Lb(int id, double new_lb) {
  REACTION* ptr = rxnPtrFromId(id);
  assert(new_lb <= ptr->ub - 1E-8);

  ptr->lb = new_lb;

  if(new_lb >= 1E-5f) { ptr->net_reversible = 1; }
  if(new_lb <= 1E-5f & ptr->ub >= -1E-5f) { ptr->net_reversible = 0; }
}

/* Change the ub of the reaction to new_ub
   Change the net_reversible to -1 if lb is < 0 and 0 if the reaction can now go in either direction */
void RXNSPACE::change_Ub(int id, double new_ub) {
  REACTION* ptr = rxnPtrFromId(id);
  assert(new_ub >= ptr->lb + 1E-8);

  ptr->ub = new_ub;

  if(new_ub <= -1E-5f) { ptr->net_reversible = -1; }
  if(new_ub >= -1E-5f & ptr->lb <= 1E-5f) { ptr->net_reversible = 0; }
}

/* If you know you want to change both the LB AND the UB... you can pass both here to avoid obnoxious assertions.
   I suggest doing this INSTEAD of modifying net_reversible... although I have written functions to let you do it either way.
*/
void RXNSPACE::change_Lb_and_Ub(int id, double new_lb, double new_ub) {
  assert(new_lb <= new_ub + 1E-8);

  REACTION* ptr = rxnPtrFromId(id);
  ptr->lb = new_lb;
  ptr->ub = new_ub;

  if(new_lb > 1E-5) { ptr->net_reversible = 1; }
  else if(new_ub < -1E-5) { ptr->net_reversible = -1; }
  else { ptr->net_reversible = 0; }

}

/* Uses "find" function from map to allow us to declare constant RXNSPACE's
Return a reaction with a given ID */
REACTION RXNSPACE::rxnFromId(int id) const {
  int idx = this->idxFromId(id);
  return rxns[idx];
}

REACTION* RXNSPACE::rxnPtrFromId(int id) {
  int idx = this->idxFromId(id);
  return &rxns[idx];
}

const REACTION* RXNSPACE::rxnPtrFromId(int id) const {
  int idx = this->idxFromId(id);
  return &rxns[idx];
}

/* Returns a reaction index from an Id (does bounds-checking) */
int RXNSPACE::idxFromId(int id) const {
  map<int,int>::const_iterator it = Ids2Idx.find(id);
  if(it == Ids2Idx.end()) {
    printf("FAILURE: Attempt to get an index for ID %d that is not in the RXNSPACE(%d)!\n", 
	   id,(int)this->rxns.size());
    assert(it != Ids2Idx.end() );
  }
  return (it -> second);
}

bool RXNSPACE::idIn(int id) const {
  if(Ids2Idx.count(id) > 0) { return true; }
  else { return false; }
}

/* It is my hope that we won't actually need this now that it's part of the constructor for RXNSPACE.
   You won't have to use this if you always make and expand RXNSPACEs with the member class functions*/
void RXNSPACE::rxnMap() {
  assert(this->rxns.size()>0);
  this->Ids2Idx.clear();
  for(int i=0;i<this->rxns.size();i++) {
    this->Ids2Idx[this->rxns[i].id] = i;
  }
  return;
}


/* I think I'm required to put this here...even if it does nothing*/
METSPACE::METSPACE() {
}

METSPACE::METSPACE(const vector<METABOLITE> &metVec) {
  mets = metVec;
  for(int i=0; i<mets.size();i++) {
    Ids2Idx[mets[i].id] = i;
  }
  numMets = mets.size();
}

/* Initialize metabolite list to be a subset of a larger metabolite list */
METSPACE::METSPACE(const METSPACE& existingSpace, const vector<int> &idSubset) {
  for(int i=0; i<idSubset.size(); i++) {
    mets.push_back(existingSpace.metFromId(idSubset[i]));
    Ids2Idx[idSubset[i]] = i;
  }
  numMets = mets.size();
}

/* Initializes a list of mets based on stoich (NOT stoich_part) for a given RXNSPACE */
METSPACE::METSPACE(const RXNSPACE &rxnspace, const METSPACE &largeMetSpace) {
  vector<int> metIds;
  for(int i=0; i<rxnspace.rxns.size(); i++) {
    for(int j=0; j<rxnspace.rxns[i].stoich.size(); j++) {
      metIds.push_back(rxnspace.rxns[i].stoich[j].met_id);
    }
  }
  custom_unique(metIds);
  for(int i=0; i<metIds.size(); i++) {
    mets.push_back(largeMetSpace.metFromId(metIds[i]));
    Ids2Idx[metIds[i]] = i;
  }
  numMets = metIds.size();
}

void METSPACE::clear() {
  mets.clear();
  Ids2Idx.clear();
  numMets++;
}

void METSPACE::addMetabolite(const METABOLITE &met) {
  if(idIn(met.id)) {
    //printf("WARNING: Attempted to pass a metabolite with id %d that was already in the METSPACE!\n", met.id);
    return;
  }
  mets.push_back(met);
  Ids2Idx[met.id] = mets.size()-1;
  numMets++;
}

void METSPACE::removeMetFromBack() {
  assert(mets.size() > 0);
  int id = mets.back().id;
  int idx = Ids2Idx[id];
  mets.pop_back();
  Ids2Idx.erase(id);
  numMets--;
}

METABOLITE METSPACE::metFromId(int id) const {
  return mets[idxFromId(id)];
}

METABOLITE* METSPACE::metPtrFromId(int id) {
  int idx = this->idxFromId(id);
  return &mets[idx];
}


int METSPACE::idxFromId(int id) const {
  map<int,int>::const_iterator it = Ids2Idx.find(id);
  if(it == Ids2Idx.end()) {
    printf("FAIL: Attempted to access metabolite %d that is not present in the metabolite struct...\n", id);
    assert(it != Ids2Idx.end() );
  }
  return (it -> second);
}

bool METSPACE::idIn(int id) const {
  if(Ids2Idx.count(id) > 0) { return true; }
  else { return false; }
}

METABOLITE & METSPACE::operator[](int idx) {
  assert(idx < mets.size());
  return this->mets[idx];
}

/* Convert metabolite ID's to metabolite indicies and vice versa */
/* You won't have to use this functino if you always make and expand/contract METSPACEs with member class functions */
void METSPACE::metMap() {
  assert(this->mets.size() > 0);
  this->Ids2Idx.clear();
  for(int i=0;i<this->mets.size();i++) {
    this->Ids2Idx[this->mets[i].id] = i;
  }
  return;
}

/* Note - I implemented this myself so that I automatically reserve the capacity... lets see if it helps make this more efficient */
METSPACE METSPACE::operator=(const METSPACE &orig) {
  if(&orig != this) {
    clear();
    mets.reserve(orig.mets.capacity());
    for(int i=0; i<orig.mets.size(); i++) {
      addMetabolite(orig.mets[i]);
    }
  }
  return *this;
}


PROBLEM::PROBLEM(){

}

/* Initializes a problem to ONLY contain (full) reactions
   identified in reaction space x and whatever metabolites are present in them (intended to be used after synrxns is no longer needed)

   Does not fill up synrxns or the reversibility-shifted structs and adds only noncentrals to "cofactors" (used by ETC chain)

   Copies the GROWTH conditions */
PROBLEM::PROBLEM(const RXNSPACE &x, const PROBLEM &ProblemSpace){
  for(int i=0;i<x.rxns.size();i++){
    fullrxns.addReaction(x.rxns[i]);
    if(x.rxns[i].isExchange()) {
      exchanges.addReaction(x.rxns[i]);
    }
  }

  vector<int> mets;
  for(int i=0;i<fullrxns.rxns.size();i++){
    for(int j=0;j<fullrxns.rxns[i].stoich.size();j++){
      mets.push_back(fullrxns.rxns[i].stoich[j].met_id);
    }
  }
  custom_unique(mets);

  for(int i=0;i<mets.size();i++){
    metabolites.addMetabolite(ProblemSpace.metabolites.metFromId(mets[i]));
    if(metabolites.mets.back().noncentral == 1) {
      cofactors.addMetabolite(metabolites.mets.back());
    }
  }

  growth = ProblemSpace.growth;
}

/* Empty out all that junk */
void PROBLEM::clear() {
  fullrxns.clear();
  synrxns.clear();
  synrxnsR.clear();
  exchanges.clear();

  metabolites.clear();
  cofactors.clear();
  
  growth.clear();
}

// Write out metabolites, reactions, and stoich values in json format:
void PROBLEM::to_jsonfile(const char* filename) {
  ofstream jsonfile;

  // Metabolites:
  stringstream met_fname;
  met_fname << filename << ".mets.json";
  jsonfile.open(met_fname.str().c_str());	// what if can't???
  if (!jsonfile.is_open()) {
    cerr << "unable to open " << met_fname << " for some reason." << endl;
    perror("Is this the error? :");
    exit(1);
  }

  vector<METABOLITE> mets=this->metabolites.mets;
  vector<METABOLITE>::iterator mets_it=mets.begin();
  jsonfile << "[" << endl;
  while (mets_it != mets.end()) {
    string json=mets_it->as_json();
    jsonfile << json;
    mets_it++;
    if (mets_it != mets.end()) {
      jsonfile << ",";
    }
    jsonfile << endl;
  }
  jsonfile << "]" << endl;
  jsonfile.close();
  cerr << met_fname.str() << " written" <<endl;

  // Reactions & stoich values:
  stringstream rxn_fname;
  rxn_fname << filename << ".rxns.json";
  ofstream rxnfile;
  rxnfile.open(rxn_fname.str().c_str());

  stringstream stoich_fname;
  stoich_fname << filename << ".stoich.json";
  ofstream stoichfile;
  stoichfile.open(stoich_fname.str().c_str());
  stoichfile << "[" << endl;

  vector<REACTION> rxns=this->fullrxns.rxns;
  vector<REACTION>::iterator rxn_it=rxns.begin();
  rxnfile << "[" << endl;
  while (rxn_it != rxns.end()) {
    rxnfile << rxn_it->as_json();
    
    // write out stoich values:
    vector<STOICH>::iterator stoich_it=rxn_it->stoich.begin();
    while (stoich_it != rxn_it->stoich.end()) {
      if (stoich_it->rxn_id != rxn_it->id) {
	cerr << "mismatched rxn_ids: stoich->rxn_id=" << stoich_it->rxn_id <<
	  ", rxn.id=" << rxn_it->id << endl;
      }
      stoich_it->rxn_id=rxn_it->id; // just to be sure
      stoichfile << stoich_it->as_json() << "," << endl;
      stoich_it++;
    }

    rxn_it++;
    if (rxn_it != rxns.end()) {
      rxnfile << "," << endl;
    }
  }

  rxnfile << "[" << endl;
  rxnfile.close();

  stoichfile << "]" << endl;
  stoichfile.close();
  cerr << rxn_fname.str() << " written" <<endl;
  cerr << stoich_fname.str() << " written" <<endl;
}

/***********************************************************************/

BADIDSTORE::BADIDSTORE() {  badRxnId = -1; }

bool BADIDSTORE::operator<(const BADIDSTORE &rhs) const {
  BADIDSTORE lhs = *this;
  /* order first by the reaction ID, then by the size of badMetIds, and finally by their values. */
  if(lhs.badRxnId < rhs.badRxnId) { return true; }
  if(lhs.badRxnId > rhs.badRxnId) { return false; }
  /* badRxnId is equal */
  if(lhs.badMetIds.size() < rhs.badMetIds.size() ) { return true; }
  if(lhs.badMetIds.size() > rhs.badMetIds.size() ) { return false; }
  /* size is equal */
  for(int i=0; i<lhs.badMetIds.size(); i++) { 
    if(lhs.badMetIds[i] < rhs.badMetIds[i]) { return true; }
    if(lhs.badMetIds[i] > rhs.badMetIds[i]) { return false; }
  }
  /* Everything is equal (return false) */
  return false;
}

bool BADIDSTORE::operator==(const BADIDSTORE &rhs) const {
  BADIDSTORE lhs = *this;
  if(lhs.badRxnId != rhs.badRxnId) { return false; }
  if(lhs.badMetIds.size() != rhs.badMetIds.size()) { return false; }
  for(int i=0; i<lhs.badMetIds.size(); i++) {
    if(lhs.badMetIds[i] != rhs.badMetIds[i]) { return false; }
  }
  return true;
}

INNERPOPSTORE::INNERPOPSTORE() { score = 0.0f; }

bool INNERPOPSTORE::operator<(const INNERPOPSTORE &rhs) const {
  INNERPOPSTORE lhs = *this;
  if(lhs.whichK.size() != rhs.whichK.size() ) { printf("INTERNAL ERROR: INNERPOPSTORE.whichK should always be the same size. \n"); assert(false); }
  /* order first by the reaction ID, then by the size of badMetIds, and finally by their values. */
  if(lhs.score < rhs.score) { return true; }
  if(lhs.score > rhs.score) { return false; }
  for(int i=0; i<lhs.whichK.size(); i++) { 
    if(lhs.whichK[i] < rhs.whichK[i]) { return true; }
    if(lhs.whichK[i] > rhs.whichK[i]) { return false; }
  }
  /* Everything is equal (return false) */
  return false;
}

bool INNERPOPSTORE::operator==(const INNERPOPSTORE &rhs) const {
  INNERPOPSTORE lhs = *this;
  if(lhs.whichK.size() != rhs.whichK.size() ) { printf("INTERNAL ERROR: INNERPOPSTORE.whichK should always be the same size. \n"); assert(false); }
  if(lhs.score != rhs.score) { return false; }
  for(int i=0; i<lhs.whichK.size(); i++) {
    if(lhs.whichK[i] != rhs.whichK[i]) { return false; }
  }
  return true;
}

ANSWER::ANSWER(){

}


//copy constructor
ANSWER::ANSWER(const ANSWER &source){
    /* These aren't necessary because these variables are declared as STATIC so they have the same value automatically
       as any other ANSWER */
    /*    this->annoteToRxns = source.annoteToRxns;
    this->etc = source.etc;
    this->pList = source.pList; */

    this->reactions = source.reactions;
    this->metabolites = source.metabolites;
    this->knockoutResults = source.knockoutResults;
    this->optLocalScore = source.optLocalScore;

    /* This should be OK since passedPaths is a STATIC it should always be in the same memory location... */
    this->passedPaths = source.passedPaths;

    /* Since the reactions and metabolites are NOT static (by design)
       we need to move the pointers to the new answer */
    for(int i=0; i<source.fixedGaps.size(); i++) { this->fixedGaps.push_back( this->metabolites.metPtrFromId( source.fixedGaps[i]->id )); }
    for(int i=0; i<source.fixes.size(); i++) {
      vector<const REACTION*> tmp;
      for(int j=0; j<source.fixes[i].size(); j++) {
	tmp.push_back(this->reactions.rxnPtrFromId( source.fixes[i][j]->id ));
      }
      this->fixes.push_back(tmp);
    }
    for(int i=0; i<source.essentialMagicExits.size(); i++) { this->essentialMagicExits.push_back( this->reactions.rxnPtrFromId( source.essentialMagicExits[i]->id )); }
    for(int i=0; i<source.etcIds.size(); i++) { this->etcIds.push_back( this->reactions.rxnPtrFromId( source.etcIds[i]->id )); }
    for(int i=0; i<source.etcConnect.size(); i++) { this->etcConnect.push_back( this->reactions.rxnPtrFromId( source.etcConnect[i]->id )); }
  }


// Write out pathsummaries, other stuff?
void ANSWER::to_jsonfile(const char* filename) {
  stringstream fname;
  fname << filename << ".psum.json";
  ofstream jsonfile;
  jsonfile.open(fname.str().c_str());

  jsonfile << "[" << endl;
  int i, j, k;		// growth, metabolite, k
  for (i=0; i < pList.size(); i++) {
    for (j=0; j < pList[i].size(); j++) {
      for (k=0; k < pList[i][j].size(); k++) {
	jsonfile << pList[i][j][k].as_json();
	if (i!=pList.size()-1 && j!=pList[i].size()-1 && k!=pList[i][j].size()-1) {
	  jsonfile << ",";
	}
	jsonfile << endl;
      }
    }
  }
  jsonfile << "]" << endl;

  jsonfile.close();
  cerr << fname.str() << " written" << endl;
}

void initializeAnswer(const vector<NETREACTION> &myEtc, const vector<vector<vector<PATHSUMMARY> > > &myPlist, const map<string, vector<VALUESTORE> > &annoteToRxn) {
  ANSWER::etc = myEtc;
  ANSWER::pList = myPlist;
  ANSWER::annoteToRxns = annoteToRxn;
}
