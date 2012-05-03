#include "PathStuff.h"

#include <string>
#include <vector>
#include <map>
#include <assert.h>
#include <cstring>
#include <cstdio>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <climits>

using namespace std;

/*
  PATH
 */


PATH::PATH() {
  /* This flag is how you can tell if there was actually a path found or not - total likelihood
     will be > 0 if a path is found */
  totalLikelihood = INT_MAX;
  outputId = -1;
  //  path_id=n_paths++;
}


/***********************************************************************
   MysqlObject requirements for PATH: 
***********************************************************************/

/***********************************************************************
  PATHSUMMARY
 ***********************************************************************/

PATHSUMMARY PATHSUMMARY::clear(){
  this->rxnDirIds.clear();
  this->deadEndIds.clear();
  this->metsConsumed.clear();
  this->outputId = -1;
  this->likelihood = -1;
  this->growthIdx.clear();
  this->k_number = -1;
  this->rxnPriority.clear();
  return *this;
}

PATHSUMMARY::PATHSUMMARY() {
  id=0;
  outputId = -1;
  likelihood = -1;
  k_number = -1;
};

bool PATHSUMMARY::metIn(int metId) const{
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
  this->rxnPriority.clear();
  for(int i=0;i<onepath.rxnIds.size();i++){
    this->rxnDirIds.push_back(onepath.rxnIds[i] * onepath.rxnDirection[i]);}
  for(int i=0;i<onepath.deadEndIds.size();i++){
    this->deadEndIds.push_back(onepath.deadEndIds[i]);}
  for(int i=0;i<onepath.metsConsumedIds.size();i++){
    this->metsConsumed.push_back(onepath.metsConsumedIds[i]);}
  for(int i=0; i<onepath.rxnPriority.size(); i++) {
    this->rxnPriority.push_back(onepath.rxnPriority[i]);  }

  this->likelihood = onepath.totalLikelihood;
  this->outputId = onepath.outputId;
  return *this;
}

PATHSUMMARY PATHSUMMARY::operator/(PATH &onepath){
  this->rxnDirIds.clear();
  this->deadEndIds.clear();
  this->metsConsumed.clear();
  this->rxnPriority.clear();
  for(int i=0;i<onepath.rxnIds.size();i++){
    this->rxnDirIds.push_back(-onepath.rxnIds[i] * onepath.rxnDirection[i]);}
  for(int i=0;i<onepath.deadEndIds.size();i++){
    this->deadEndIds.push_back(onepath.deadEndIds[i]);}
  for(int i=0;i<onepath.metsConsumedIds.size();i++){
    this->metsConsumed.push_back(onepath.metsConsumedIds[i]);}
  for(int i=0; i<onepath.rxnPriority.size(); i++) {
    this->rxnPriority.push_back(onepath.rxnPriority[i]);  }  

  this->likelihood = onepath.totalLikelihood;
  this->outputId = onepath.outputId;
  return *this;
}

string PATHSUMMARY::as_json() {
  stringstream json;
  json << "{ id: " << id << "," << endl;
  json << "  outputId: " << outputId << "," << endl;
  json << "  k_number: " << k_number << "," << endl;
  json << "  likelihood: " << likelihood << "," << endl;

  
  json << "  growthIdx: [";
  vector<int>::iterator it=this->growthIdx.begin();
  while (it != growthIdx.end()) {
    json << *it++;
    if (it != growthIdx.end()) { json << ", "; }
  }
  json << "]," << endl;
  
  json << "  rxnDirIds: [";
  it=this->rxnDirIds.begin();
  while (it != rxnDirIds.end()) {
    json << *it++;
    if (it != rxnDirIds.end()) { json << ", "; }
  }
  json << "]," << endl;
  
  json << "  rxnPriority: [";
  it=this->rxnPriority.begin();
  while (it != rxnPriority.end()) {
    json << *it++;
    if (it != rxnPriority.end()) { json << ", "; }
  }
  json << "]," << endl;
  
  json << "  deadEndIds: [";
  it=this->deadEndIds.begin();
  while (it != deadEndIds.end()) {
    json << *it++;
    if (it != deadEndIds.end()) { json << ", "; }
  }
  json << "]," << endl;
  
  json << "  metsConsumed: [";
  it=this->metsConsumed.begin();
  while (it != metsConsumed.end()) {
    json << *it++;
    if (it != metsConsumed.end()) { json << ", "; }
  }
  json << "]," << endl;
  

  json << "}";
  return json.str();
}
