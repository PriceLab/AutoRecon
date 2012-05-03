#ifndef __METABOLITE_H__
#define __METABOLITE_H__


#include <vector>
#include <map>
#include <string>
using namespace std;

class METABOLITE  {
 public:
  /* Externally (XML/User) defined parameters */
  int id; /* Has to be big matrix row index */
  char name[64];
  int charge;
  int input; /* Is it an input?: 0 for no, 1 for yes */
  int output; /* Is it an output? 0 for no, 1 for yes */
  int biomass; /* Is it a biomass component? 0 for no, 1 for yes */
  int secondary_lone; /* 0 for no, 1 for yes */
  vector<int> secondary_pair; /* Int of the ID for each possible secondary pair */
  int noncentral; /* 0 = central (not used for ETC), 1 = noncentral (used for ETC), -1 = undefined */
  char chemform[64];
  double modifier; /* Reserved for things that can be used to modify metabolite cost in Dijkstras algorithm, such as metabolomics data or other thigns we calculate */
  
  /* Connected reactions */
  vector<int> rxnsInvolved_nosec;
  
  METABOLITE();
  void reset();
  int isSecondary() const;
  string as_json();
  string dump();


};

#endif
