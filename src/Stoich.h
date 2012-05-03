#ifndef __STOICH_H__
#define __STOICH_H__

#include <map>
#include <string>
using namespace std;

class STOICH {
  public:
  int id;
  int met_id;
  int rxn_id;
  double rxn_coeff;
  char met_name[64];
  
  bool operator==(const STOICH &rhs) const;
  bool operator>(const STOICH &rhs) const;
  bool operator<(const STOICH &rhs) const;
  STOICH();
  ~STOICH();
  void reset();

  string as_json();
  string dump();

  static int nStoichs;

};

#endif
