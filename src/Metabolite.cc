#include "Metabolite.h"
#include <iostream>
#include <sstream>
#include <vector>
using namespace std;

METABOLITE::METABOLITE() {
  id = 0;
  input = 0;
  output = 0;
  biomass = 0;
  secondary_lone = 0;
  noncentral = 0;
  modifier = 1.0f;
  name[0]=(char)0;
  charge=0;
  chemform[0]=(char)0;
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

string METABOLITE::as_json() {
  stringstream json;
  json << "{ id: " << id << "," << endl;
  json << "  name: '" << name << "'," << endl;
  json << "  charge: " << charge << "," << endl;
  json << "  input: " << input << "," << endl;
  json << "  output: " << output << "," << endl;
  json << "  biomass: " << biomass << "," << endl;
  json << "  secondary_lone: " << secondary_lone << "," << endl;
  json << "  noncentral: " << noncentral << "," << endl;
  json << "  chemform: '" << chemform << "'," << endl;
  json << "  modifier: " << modifier << "," << endl;
  
  json << "  secondary_pair: [ ";
  vector<int>::iterator it=this->secondary_pair.begin();
while (it != secondary_pair.end()) {
    json << *it++;
    if (it != secondary_pair.end()) { json << ", "; }
  }
  json << "]," << endl;
  
json << "  rxnsInvolved_nosec: [ ";
it=this->rxnsInvolved_nosec.begin();
  while (it != rxnsInvolved_nosec.end()) {
    json << *it++;
    if (it != rxnsInvolved_nosec.end()) { json << ", "; }
  }
  json << "]," << endl;

 
json << "}";
return json.str();  
}

string METABOLITE::dump() {
  stringstream d;
  d << "Metabolite " << id << ": name=" << name << endl <<
    "  input: " << input << ", output: " << output << ", biomass: " << biomass <<
    ", sec_lone: " << secondary_lone << ", noncentral: " << noncentral << 
    ", modifier: " <<  modifier << endl;
  return d.str();
}

