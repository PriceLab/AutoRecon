#include "Stoich.h"
#include "Reaction.h"

#include <sstream>
#include <map>
#include <string>
#include <stdlib.h>
#include <string.h>
using namespace std;

int STOICH::nStoichs=0;

STOICH::STOICH() {
  id=++nStoichs;
  met_id = -1;
  rxn_id = -1;
  strcpy(met_name, "");
  rxn_coeff = 999;
}

STOICH::~STOICH() {
  --nStoichs;
}

void STOICH::reset() {
  met_id = -1;
  rxn_id = -1;
  rxn_coeff = 999;
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


string STOICH::dump() {
  stringstream d;
  d << "Stoich: '" << met_name << ", id=" << id << ", met_id=" << met_id << ", coeff=" << rxn_coeff;
  return d.str();
}

string STOICH::as_json() {
  stringstream json;
  json << "{ id: " << id << ", met_id: " << met_id << ", rxn_id: " << rxn_id << 
    ", rxn_coeff: " << rxn_coeff << "}";
  return json.str();
}


