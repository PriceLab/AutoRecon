#include "Reaction.h"
#include "MyConstants.h"
#include <sstream>
#include <map>
#include <vector>
#include <string>
using namespace std;

bool REACTION::operator==(const REACTION &rhs) const{
  return (this[0].id == rhs.id);
}
bool REACTION::operator>(const REACTION &rhs) const{
  return (this[0].id > rhs.id);
}
bool REACTION::operator<(const REACTION &rhs) const{
  return (this[0].id < rhs.id);
}

bool REACTION::isExchange() const {
  if(this->stoich.size() == 1 && this->id != _db.BIOMASS) { return true; }
  else { return false; }
}


REACTION::REACTION(){
  id = 0;
  freeMakeFlag = 0;
  transporter = 0;
  synthesis = -1;
  /* Default likelihood is -5 [NOT FOUND] */
  init_likelihood = -5;
  current_likelihood = init_likelihood;
  init_reversible = 0;
  net_reversible = init_reversible; /*Note - Dijkstras uses this one */
  if(net_reversible == 0) { lb = -1000.0f; ub = 1000.0f; }
  else if(net_reversible < 0) { lb = -1000.0f; ub = 0.0f; }
  else { lb = 0.0f; ub = 1000.0f; }
  revPair=0;
}

void REACTION::reset() {
  freeMakeFlag = 0;
  transporter = 0;
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

string REACTION::dump() {
  stringstream d;
  d << "Reaction " << id << ": name=" << name << endl <<
    "  synthesis (id): " << synthesis << ", transporter: " << transporter << endl <<
    ", freeMakeFlag: " << freeMakeFlag << endl << 
    "  init_likelihood: " << init_likelihood << ", current_likelihood: " << current_likelihood << endl <<
    "  init_reversible: " << init_reversible << ", net_reversible: " << net_reversible << endl <<
    "  lb: " << lb << ", ub: " << ub << ", revPair: " << revPair << endl <<
    stoich.size() << " total metabolites, " << stoich_part.size() << " primary metabolites" << endl;
  return d.str();
}

string REACTION::as_json() {
  stringstream json;
  json << "{ id: " << id << ", " << endl;
  json << "  name: '" << name << "'," << endl;
  json << "  synthesis: " << synthesis << "," << endl;
  json << "  transporter: " << transporter << "," << endl;
  json << "  freeMakeFlag: " << freeMakeFlag << "," << endl;
  json << "  init_likelihood: " << init_likelihood << "," << endl;
  json << "  current_likelihood: " << current_likelihood << "," << endl;
  json << "  init_reversible: " << init_reversible << "," << endl;
  json << "  net_reversible: " << net_reversible << "," << endl;
  json << "  lb: " << lb << "," << endl;
  json << "  ub: " << ub << "," << endl;
  json << "  refPair: " << revPair << "," << endl;
  
  json << "  stoich_ids: [";
  vector<STOICH>::iterator it=this->stoich.begin();
  while (it != stoich.end()) {
    json << it->id;
    it++;
    if (it != stoich.end()) { json << ", "; }
  }
  json << "]" << endl;
  // Also maybe need stoich_part, syn, annote (don't forget ','s) above

  json << "}";
  return json.str();
}



