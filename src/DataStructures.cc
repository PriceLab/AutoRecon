#include "DataStructures.h"
#include "pathUtils.h"

#include <iostream>
#include <cstdlib>
#include <assert.h>
#include <cstring>
#include <cstdio>
#include <map>
#include <vector>

using std::map;
using std::vector;
using std::strcpy;
using std::sprintf;

vector<NETREACTION> ANSWER::etc;
vector<vector<vector<PATHSUMMARY> > > ANSWER::pList;
map<string, vector<VALUESTORE> > ANSWER::annoteToRxns;

template <class T, class U> 
U intTimes(const T& x, const U& y){
  int tempx = x;
  int tempy = y;
  return (U) (x*y);
}

REV convertRXNDIRID2REV(RXNDIRID x){
  if(x > 0){return (REV)1;}
  else{return (REV)-1;}
}

/*
RXNID operator=(const RXNDIRID x){
  int tempx = x;
  return (RXNID) abs(x);
}
*/

METABOLITE::METABOLITE() {
  //name = {0};
  input = 0;
  output = 0;
  biomass = 0;
  secondary_lone = 0;
  noncentral = 0;
  modifier = 1.0f;
}

void METABOLITE::reset() {
  rxnsInvolved_nosec.clear();
  input = 0;
  output = 0;
  biomass = 0;
  secondary_lone = 0;
  noncentral = 0;
  modifier = 1.0f;
  secondary_pair.clear();
}

int METABOLITE::isSecondary() const {
  if(1 == this->secondary_lone) return 1;
  if(this->secondary_pair.size() > 0) return 1;
  return 0;
}

// OUTPUT:
// ID: - ; Name: -
std::ostream& operator<<(std::ostream &out, METABOLITE &orig){
  out<<"ID: "<<orig.id<<"; Name: "<<orig.name;
  return out;
}

bool STOICH::operator==(const STOICH &rhs) const{
  return (this[0].rxn_coeff == rhs.rxn_coeff);
}

bool STOICH::operator>(const STOICH &rhs) const{
  return (this[0].rxn_coeff > rhs.rxn_coeff);
}

bool STOICH::operator<(const STOICH &rhs) const{
  return (this[0].rxn_coeff < rhs.rxn_coeff);
}

std::ostream& operator<<(std::ostream &out, STOICH & orig){
  //out<<"ID: "<<orig.met_id<<"; Name: "<<orig.met_name<<"; Coeff: "
  //   <<orig.rxn_coeff<<"; Compartment: "<<orig.compartment;
  out<<orig.rxn_coeff<<"*"<<orig.met_name<<"("<<orig.met_id<<")";
  return out;
}

bool ANNOTATION::operator<(const ANNOTATION &rhs)const {
  return this->probability > rhs.probability;
}

STOICH::STOICH() {
  met_id = -1;
  //met_name;
  rxn_coeff = 999;
}

void STOICH::reset() {
  met_id = -1;
  rxn_coeff = 999;
}

bool REACTION::operator==(const REACTION &rhs) const{
  return (this[0].id == rhs.id);
}
bool REACTION::operator>(const REACTION &rhs) const{
  return (this[0].id > rhs.id);
}
bool REACTION::operator<(const REACTION &rhs) const{
  return (this[0].id < rhs.id);
}


REACTION::REACTION(){
  freeMakeFlag = false;
  transporter = false;
  transportedMetabolite = -1;
  /* Default likelihood is -5 [NOT FOUND] */
  init_likelihood = -5;
  current_likelihood = init_likelihood;
  init_reversible = 0;
  net_reversible = init_reversible; /*Note - Dijkstras uses this one */
  if(net_reversible == 0) { lb = -1000.0f; ub = 1000.0f; }
  else if(net_reversible < 0) { lb = -1000.0f; ub = 0.0f; }
  else { lb = 0.0f; ub = 1000.0f; }
}

/* Exchange reaction is just defined as a reaction with exactly one metabolite (regardless of direction)*/
bool REACTION::isExchange() const {
  if(this->stoich.size() == 1 && this->id != _db.BIOMASS) { return true; }
  else { return false; }
}

void REACTION::reset() {
  freeMakeFlag = false;
  transporter = false;
  init_likelihood = -5;
  current_likelihood = init_likelihood;
  init_reversible = 0;
  net_reversible = init_reversible; /*Note - Dijkstras uses this one */
  if(net_reversible == 0) { lb = -1000.0f; ub = 1000.0f; }
  else if(net_reversible < 0) { lb = -1000.0f; ub = 0.0f; }
  else { lb = 0.0f; ub = 1000.0f; }

  stoich.clear();
  stoich_part.clear();
  syn.clear();
  annote.clear();
}

// OUTPUT:
// ID: -; [Transporter ; ]Name: -
// iLikelihood: -; Bounds l: - u: -
// REACTION + REACTION + REACTION + ...
std::ostream& operator<<(std::ostream &out, REACTION & orig){
  out<<"ID: "<<orig.id<<"; Name: "<<orig.name;
  if(orig.transporter == 1){
    out<<"; Transporter ";
  }
  if(orig.init_reversible == 0){
    out<<"; Reversible ";
  }
  else if(orig.init_reversible == 1){
    out<<"; Backwards only ";
  }
  else if(orig.init_reversible == -1){
    out<<"; Forward only ";
  }
  out<<std::endl<<" iLikelihood: "<<orig.init_likelihood
     <<"; Bounds l: "<<orig.lb<<" u: "<<orig.ub<<std::endl;
  for(int i=0; i<orig.stoich.size(); i++){
    if(i != 0){
      out<<" + "; 
    }
    out<<orig.stoich[i];
  }
  out<<std::endl;
  return out;
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

std::ostream& operator<<(std::ostream &out, MEDIA & orig){
  out<<"ID: "<<orig.id<<"; Name: "<<orig.name<<"; Uptake Rate: "<<orig.rate
     <<" mmol/gDw/hr";
  return out;
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

std::ostream& operator<<(std::ostream &out, GROWTH & orig){
  out<<"--GROWTH DATA--"<<std::endl
     <<"Growth Rate: "<<orig.growth_rate<<std::endl
     <<"-MEDIA-"<<std::endl;
  for(int i=0; i<orig.media.size(); i++)
    out<<orig.media[i]<<std::endl;
  out<<"-BYPRODUCTS-"<<std::endl;
  for(int i=0; i<orig.byproduct.size(); i++)
    out<<orig.byproduct[i]<<std::endl;
  out<<"-BIOMASS-"<<std::endl;
  for(int i=0; i<orig.biomass.size(); i++)
    out<<orig.biomass[i]<<std::endl;
  return out;
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

//Constructor
RXNSPACE::RXNSPACE(const RXNSPACE& existingSpace, const vector<RXNID> &idSubset) {
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

REACTION & RXNSPACE::operator[](RXNIDX idx) {
  assert(idx < rxns.size());
  //  return this->rxns[idx];
  return rxns[idx];
}

REACTION & RXNSPACE::operator[](RXNID id) {
  assert(idIn(id));
  RXNIDX idx = idxFromId(id);
  REACTION & rRXN = rxns[idx];
  return rRXN;
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

// OUTPUT:
// -RXNSPACE-
// REACTION
// REACTION
// ...
std::ostream& operator<<(std::ostream &out, RXNSPACE & orig){
  out<<"-RXNSPACE-"<<std::endl;
  for(int i=0; i<orig.rxns.size(); i++)
    out<<orig.rxns[i]<<std::endl;
  return out;
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
  RXNID  id  = rxns.back().id;
  RXNIDX idx = Ids2Idx[id];
  rxns.pop_back();
  Ids2Idx.erase(id);
  numRxns--;
}

void RXNSPACE::changeId(RXNID oldId, RXNID newId) {
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
void RXNSPACE::changeReversibility(RXNID id, REV new_rev) {
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
void RXNSPACE::change_Lb(RXNID id, double new_lb) {
  REACTION* ptr = rxnPtrFromId(id);
  assert(new_lb <= ptr->ub - 1E-8);

  ptr->lb = new_lb;

  if(new_lb >= 1E-5f) { ptr->net_reversible = 1; }
  if(new_lb <= 1E-5f & ptr->ub >= -1E-5f) { ptr->net_reversible = 0; }
}

/* Change the ub of the reaction to new_ub
   Change the net_reversible to -1 if lb is < 0 and 0 if the reaction can now go in either direction */
void RXNSPACE::change_Ub(RXNID id, double new_ub) {
  REACTION* ptr = rxnPtrFromId(id);
  assert(new_ub >= ptr->lb + 1E-8);

  ptr->ub = new_ub;

  if(new_ub <= -1E-5f) { ptr->net_reversible = -1; }
  if(new_ub >= -1E-5f & ptr->lb <= 1E-5f) { ptr->net_reversible = 0; }
}

/* If you know you want to change both the LB AND the UB... you can pass both here to avoid obnoxious assertions.
   I suggest doing this INSTEAD of modifying net_reversible... although I have written functions to let you do it either way.
*/
void RXNSPACE::change_Lb_and_Ub(RXNID id, double new_lb, double new_ub) {
  assert(new_lb <= new_ub + 1E-8);

  REACTION* ptr = rxnPtrFromId(id);
  ptr->lb = new_lb;
  ptr->ub = new_ub;

  if(new_lb > 1E-5) { ptr->net_reversible = 1; }
  else if(new_ub < -1E-5) { ptr->net_reversible = -1; }
  else { ptr->net_reversible = 0; }

}

/* Uses "find" function from map to allow us to declare constant RXNSPACE's
   Return a reaction with a given ID*/ 
REACTION RXNSPACE::rxnFromId(RXNID id) const {
  int idx = this->idxFromId(id);
  return rxns[idx];
}

REACTION* RXNSPACE::rxnPtrFromId(RXNID id) {
  int idx = this->idxFromId(id);
  return &rxns[idx];
}

const REACTION* RXNSPACE::rxnPtrFromId(RXNID id) const {
  int idx = this->idxFromId(id);
  return &rxns[idx];
}

/* Returns a reaction index from an Id (does bounds-checking) */
RXNIDX RXNSPACE::idxFromId(RXNID id) const {
  map<RXNID,RXNIDX>::const_iterator it = Ids2Idx.find(id);
  if(it == Ids2Idx.end()) {
    printf("FAILURE: Attempt to get an index for ID %d that is not in the RXNSPACE(%d)!\n", 
	   (int)id,(int)this->rxns.size());
    assert(it != Ids2Idx.end() );
  }
  return (it -> second);
}

bool RXNSPACE::idIn(RXNID id) const {
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

// returns vector of lower bounds for reactions
vector<double> & RXNSPACE::lbVector()
{
  static vector<double> STATIC_VAR_lbs;
  STATIC_VAR_lbs.clear();
  for(int i=0; i<rxns.size(); i++)
    STATIC_VAR_lbs.push_back(rxns[i].lb);
  return STATIC_VAR_lbs;
}

// returns vector of upper bounds for reactions
vector<double> & RXNSPACE::ubVector()
{
  static vector<double> STATIC_VAR_ubs;
  STATIC_VAR_ubs.clear();
  for(int i=0; i<rxns.size(); i++)
    STATIC_VAR_ubs.push_back(rxns[i].ub);
  return STATIC_VAR_ubs;
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
METSPACE::METSPACE(const METSPACE& existingSpace, const vector<METID> &idSubset) {
  for(int i=0; i<idSubset.size(); i++) {
    mets.push_back(existingSpace[idSubset[i]]);
    Ids2Idx[idSubset[i]] = i;
  }
  numMets = mets.size();
}

/* Initializes a list of mets based on stoich (NOT stoich_part) for a given RXNSPACE */
METSPACE::METSPACE(const RXNSPACE &rxnspace, const METSPACE &largeMetSpace) {
  vector<METID> metIds;
  for(int i=0; i<rxnspace.rxns.size(); i++) {
    for(int j=0; j<rxnspace.rxns[i].stoich.size(); j++) {
      metIds.push_back(rxnspace.rxns[i].stoich[j].met_id);
    }
  }
  custom_unique(metIds);
  for(int i=0; i<metIds.size(); i++) {
    mets.push_back(largeMetSpace[metIds[i]]);
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
  if(isIn(met.id)) {
    //printf("WARNING: Attempted to pass a metabolite with id %d that was already in the METSPACE!\n", met.id);
    return;
  }
  mets.push_back(met);
  Ids2Idx[met.id] = mets.size()-1;
  numMets++;
}

void METSPACE::removeMetFromBack() {
  assert(mets.size() > 0);
  METID id = mets.back().id;
  int idx = Ids2Idx[id];
  mets.pop_back();
  Ids2Idx.erase(id);
  numMets--;
}

METABOLITE METSPACE::getMetObj(METID id) const {
  return mets[idxFromId(id)];
}

METABOLITE* METSPACE::metPtrFromId(METID id) {
  METIDX idx = this->idxFromId(id);
  return &mets[idx];
}


METIDX METSPACE::idxFromId(METID id) const {
  map<METID,METIDX>::const_iterator it = Ids2Idx.find(id);
  if(it == Ids2Idx.end()) {
    printf("FAIL: Attempted to access metabolite %d that is not present in the metabolite struct...\n", (int)id);
    assert(it != Ids2Idx.end() );
  }
  return (it -> second);
}

bool METSPACE::isIn(METID id) const {
  if(Ids2Idx.count(id) > 0) { return true; }
  else { return false; }
}

bool METSPACE::isIn(METIDX idx) const {
  if(idx < mets.size()) { return true; }
  else { return false; }
}

/*
METABOLITE  & METSPACE::operator[](int idx) {
  printf("Warning: int instead of METIDX\n");
  assert(0);
  return this->mets[idx];
}
*/

METABOLITE & METSPACE::operator[](METIDX idx) {
  assert(idx < mets.size());
  return this->mets[idx];
}

METABOLITE & METSPACE::operator[](METID id) {
  assert(this->isIn(id));
  METIDX idx = this->idxFromId(id);
  return this->mets[idx];
}

const METABOLITE & METSPACE::operator[](METID id) const{
  assert(this->isIn(id));
  METIDX idx = this->idxFromId(id);
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

// OUTPUT:
// -METSPACE-
// METABOLITE
// METABOLITE
// ...
std::ostream& operator<<(std::ostream &out, METSPACE & orig){
  out<<"-METSPACE-"<<std::endl;
  for(int i=0; i<orig.mets.size(); i++)
    out<<orig.mets[i]<<std::endl;
  return out;
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

  vector<METID> mets;
  for(int i=0;i<fullrxns.rxns.size();i++){
    for(int j=0;j<fullrxns.rxns[i].stoich.size();j++){
      mets.push_back(fullrxns.rxns[i].stoich[j].met_id);
    }
  }
  custom_unique(mets);

  for(int i=0;i<mets.size();i++){
    metabolites.addMetabolite(ProblemSpace.metabolites[mets[i]]);
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

/* WARNING: When I implement the new shortest-path algorithm
   this class will go away */
PATH::PATH() {
  /* This flag is how you can tell if there was actually a path found or not - total likelihood
     will be > 0 if a path is found */
  totalLikelihood = INT_MAX;
  outputId = -1;
}


PATHSUMMARY PATHSUMMARY::clear(){
  this->rxnDirIds.clear();
  this->deadEndIds.clear();
  this->metsConsumed.clear();
  this->outputId = -1;
  this->likelihood = -1;
  this->growthIdx.clear();
  this->k_number = -1;
  return *this;
}

PATHSUMMARY::PATHSUMMARY() {
  outputId = -1;
  likelihood = -1;
  k_number = -1;
};

bool PATHSUMMARY::metIn(METID metId) const{
  for(int i=0;i<this->deadEndIds.size();i++){
    if(this->deadEndIds[i]==metId){ return 1;}
  }
  for(int i=0;i<this->metsConsumed.size();i++){
    if(this->metsConsumed[i]==metId){ return 1;}
  }
  return 0;
};


bool PATHSUMMARY::operator==(const PATHSUMMARY &rhs1){
  PATHSUMMARY rhs = rhs1;
  PATHSUMMARY lhs = *this;
  if(this->outputId!=rhs.outputId){ return false;}
  if(this->rxnDirIds.size()!=rhs.rxnDirIds.size()){ return false;}
  sort(lhs.rxnDirIds.begin(), lhs.rxnDirIds.end());
  sort(rhs.rxnDirIds.begin(), rhs.rxnDirIds.end());
  for(int i=0;i<lhs.rxnDirIds.size();i++){
    if(lhs.rxnDirIds[i]!=rhs.rxnDirIds[i]){ return false; }
  }
  return true;
}

PATHSUMMARY PATHSUMMARY::operator=(PATH &onepath){
  this->rxnDirIds.clear();
  this->deadEndIds.clear();
  this->metsConsumed.clear();
  for(int i=0;i<onepath.rxnIds.size();i++){
    this->rxnDirIds.push_back(onepath.rxnIds[i] * onepath.rxnDirection[i]);}
  for(int i=0;i<onepath.deadEndIds.size();i++){
    this->deadEndIds.push_back(onepath.deadEndIds[i]);}
  for(int i=0;i<onepath.metsConsumedIds.size();i++){
    this->metsConsumed.push_back(onepath.metsConsumedIds[i]);}

  this->likelihood = onepath.totalLikelihood;
  this->outputId = onepath.outputId;
  return *this;
}

PATHSUMMARY PATHSUMMARY::operator/(PATH &onepath){
  this->rxnDirIds.clear();
  this->deadEndIds.clear();
  this->metsConsumed.clear();
  for(int i=0;i<onepath.rxnIds.size();i++){
    this->rxnDirIds.push_back(onepath.rxnIds[i] * onepath.rxnDirection[i] * (REV) -1);}
  for(int i=0;i<onepath.deadEndIds.size();i++){
    this->deadEndIds.push_back(onepath.deadEndIds[i]);}
  for(int i=0;i<onepath.metsConsumedIds.size();i++){
    this->metsConsumed.push_back(onepath.metsConsumedIds[i]);}
  this->likelihood = onepath.totalLikelihood;
  this->outputId = onepath.outputId;
  return *this;
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

void initializeAnswer(const vector<NETREACTION> &myEtc, const vector<vector<vector<PATHSUMMARY> > > &myPlist, const map<string, vector<VALUESTORE> > &annoteToRxn) {
  ANSWER::etc = myEtc;
  ANSWER::pList = myPlist;
  ANSWER::annoteToRxns = annoteToRxn;
}


// initializes the STOICHMATRIX with a REACTION vector
void STOICHMATRIX::buildStoich(const vector<REACTION> & rxns){
  if(!matrix.empty()){
    std::cout<<"Stoichmatrix not empty. Original left unchanged."<<std::endl;
    return;
  }
  map<int, double> temp;
  for(int i=0; i<rxns.size(); i++){
    yIDs.push_back(rxns[i].id);
    yIdx[rxns[i].id] = i;
    for(int j=0; j<rxns[i].stoich.size(); j++){
      if(!xIdx.count(rxns[i].stoich[j].met_id)){
        xIDs.push_back(rxns[i].stoich[j].met_id);
        xIdx[rxns[i].stoich[j].met_id] = (xIDs.size()-1);
        matrix.push_back(temp);
      }
      matrix[xIdx[rxns[i].stoich[j].met_id]][i] = rxns[i].stoich[j].rxn_coeff;
    }
  }
  sizeX = xIDs.size();
  sizeY = yIDs.size();
  return;
}

// initializes the STOICHMATRIX with a matrix of STOICHs
void STOICHMATRIX::buildStoich(const vector<vector<STOICH> > & rxns){
  if(!matrix.empty()){
    std::cout<<"Stoichmatrix not empty. Original left unchanged."<<std::endl;
    return;
  }
  map<int, double> temp;
  for(int i=0; i<rxns.size(); i++){
    yIDs.push_back(RXNID(i));
    yIdx[RXNID(i)] = i;
    for(int j=0; j<rxns[i].size(); j++){
      if(!xIdx.count(rxns[i][j].met_id)){
        xIDs.push_back(rxns[i][j].met_id);
        xIdx[rxns[i][j].met_id] = (xIDs.size()-1);
        matrix.push_back(temp);
      }
      matrix[xIdx[rxns[i][j].met_id]][i] = rxns[i][j].rxn_coeff;
    }

  }
  sizeX = xIDs.size();
  sizeY = yIDs.size();
  return;
}

// initializes the STOICHMATRIX with a swapped-axes sparse matrix
// FIXME the iterator is not working
void STOICHMATRIX::buildStoich(const vector<map<int,double> > & rxns){
  if(!matrix.empty()){
    std::cout<<"Stoichmatrix not empty. Original left unchanged."<<std::endl;
    return;
  }
/* FIXME
  map<int, double> temp;
  map<int, double>::iterator it;
  for(int i=0; i<rxns.size(); i++){
    yIDs.push_back(RXNID(i));
    yIdx[RXNID(i)] = i;
    temp.clear();
    for( it = rxns[i].begin(); it != rxns[i].end(); it++){
      if(!xIdx.count(METID(it->first))){
        xIDs.push_back(METID(it->first));
        xIdx[METID(it->first)] = (xIDs.size()-1);
      }
      temp[xIdx[METID(it->first)]] = it->second;
    }
    matrix.push_back(temp);
  }
  sizeX = xIDs.size();
  sizeY = yIDs.size();
*/
  return;
}

// adds reaction to the stoichmatrix by REACTION input. Returns 0 if failure
bool STOICHMATRIX::addReaction(REACTION &rxn)
{
  if(yIdx.count(rxn.id))
    return 0;

  yIDs.push_back(rxn.id);
  yIdx[rxn.id] = sizeY++; // sets index to rxnsize then increments rxnsize
  for(int j=0; j<rxn.stoich.size(); j++)
  {
    if(!xIdx.count(rxn.stoich[j].met_id))
    {
      xIDs.push_back(rxn.stoich[j].met_id);
      xIdx[rxn.stoich[j].met_id] = sizeX++;
      matrix.push_back(map<int,double>());
    }
    matrix[xIdx[rxn.stoich[j].met_id]][sizeY-1] = rxn.stoich[j].rxn_coeff;
  }
  return 1;
}

// adds reaction to the STOICHMATRIX by RXNID input. Returns 0 if failure
bool STOICHMATRIX::addEmptyReaction(RXNID id)
{
  if(yIdx.count(id))
    return 0;
  
  yIDs.push_back(id);
  yIdx[id] = sizeY++;
  return 1;
}

bool STOICHMATRIX::addEmptyMetabolite(METID id)
{
  if(xIdx.count(id))
    return 0;
  
  xIDs.push_back(id);
  xIdx[id] = sizeX++;
  matrix.push_back(map<int,double>());
  return 1;
}

// STOICHMATRIX matrix get access function
double STOICHMATRIX::getCell(int x, int y)
{
  if(matrix[x].count(y))
    return matrix[x][y];
  else return 0.0;
}

// STOICHMATRIX matrix set access function
double& STOICHMATRIX::setCell(int x, int y, double val)
{
  matrix[x][y] = val;
  return matrix[x][y];
}

// Reaction ID to index. Returns -1 if unsuccessful
int STOICHMATRIX::rxnIDtoIDX(RXNID id)
{
  if(yIdx.count(id))
    return yIdx[id];
  else return -1;
}

// Reaction index to ID. Returns -1 if unsuccessful
RXNID STOICHMATRIX::rxnIDXtoID(int idx)
{
  if(idx<sizeY && idx>-1)
    return yIDs[idx];
  else return RXNID(-1);
}

// Metabolite ID to index. Returns -1 if unsuccessful
int STOICHMATRIX::metIDtoIDX(METID id)
{
  if(xIdx.count(id))
    return xIdx[id];
  else return -1;
}

// Metabolite index to ID. Returns -1 if unsuccessful
METID STOICHMATRIX::metIDXtoID(int idx)
{
  if(idx<sizeX && idx>-1)
    return xIDs[idx];
  else return METID(-1);
}

// OUTPUT:
// (x1, y1): double1
// (x2, y2): double2
// ...
std::ostream& operator<<(std::ostream &out, STOICHMATRIX & orig){
  for(int i=0; i<orig.sizeX; i++){
    for(map<int,double>::iterator it=orig[i].begin(); it!=orig[i].end(); it++)
      out<<"("<<i<<","<<(it->first)<<"): "<<(it->second)<<std::endl;
  }
  return out;
}
 
 
vector<double> & STOICHMATRIX::getObjFromMetID(const METID & id)
{ 
  static vector<double> STATIC_VAR_objMet;
  STATIC_VAR_objMet.clear();
  
  if(!xIdx.count(id)){
    std::cout<<"getObjFromMetID error: METID does not exist"<<std::endl;
    return STATIC_VAR_objMet;
  }

  int met = xIdx[id];
  map<int, double>::iterator it;
  int i = 0;
  for(it=matrix[met].begin(); it!=matrix[met].end(); it++)
  {
    for(;i<it->first; i++)
      STATIC_VAR_objMet.push_back(0.0);

    STATIC_VAR_objMet.push_back(it->second);
    i++;
  }

  for(;i<sizeY; i++)
    STATIC_VAR_objMet.push_back(0.0);

  return STATIC_VAR_objMet;
}


vector<double> & STOICHMATRIX::getObjFromRxnID(const RXNID & id)
{
  static vector<double> STATIC_VAR_objRxn;
  STATIC_VAR_objRxn.clear();
  if(!yIdx.count(id)){
    std::cout<<"getObjFromRxnID error: RXNID does not exist"<<std::endl;
    return STATIC_VAR_objRxn;
  }

  int rxn = yIdx[id];
  for(int i=0; i<rxn; i++)
    STATIC_VAR_objRxn.push_back(0.0);
  STATIC_VAR_objRxn.push_back(1.0);
  for(int i=rxn+1; i<sizeY; i++)
    STATIC_VAR_objRxn.push_back(0.0);
  return STATIC_VAR_objRxn;
}

vector<int> & STOICHMATRIX::equalLEG()
{
  static vector<int> STATIC_VAR_eqleg;
  STATIC_VAR_eqleg.clear();
  for(int i=0; i<sizeX; i++)
    STATIC_VAR_eqleg.push_back(0);
  return STATIC_VAR_eqleg;
}

vector<double> & STOICHMATRIX::zeroRHS()
{
  static vector<double> STATIC_VAR_zerorhs;
  STATIC_VAR_zerorhs.clear();
  for(int i=0; i<sizeX; i++)
    STATIC_VAR_zerorhs.push_back(0.0);
  return STATIC_VAR_zerorhs;
}



