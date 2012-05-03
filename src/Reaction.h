#ifndef __REACTION_H_
#define __REACTION_H_

#include <vector>
#include <map>
#include <string>
#include <stdlib.h>

#include "Stoich.h"
#include "Annotation.h"

class REACTION {
 public:
  
  int id;
  int synthesis; /* ID for metabolite that the REACTION synthesizes - if any (-1 otherwise) */
  char name[64];
  vector<STOICH> stoich; /* Full chemical reaction */

  /* stoich, but without the secondary metabolties (Load_Stoic_Part deals with this) */
  /* This is the one used by Dijkstras - if you want to use fullrxns with dijkstras you need to copy it over first */
  vector<STOICH> stoich_part;

  int transporter; /* 0 for no, 1 for yes */

  /* Special flag for reactions that go from secondaries ONLY to a metabolite
     that is not a secondary.   ex. R00005 2 CO2 + 2 NH3 <=> 	Urea-1-carboxylate + H2O */
  int freeMakeFlag;

  double init_likelihood; /* based on user input:
			     1>x>0 PROBABILITY INCLUSION
			     -1 DO NOT INCLUDE (100% sure)
			     -2 HARD INCLUDE (100% sure)
			     -3 BLACK MAGIC
			     -4 SPONTANEOUS REACTION
			     -5 NOT ON HIT LIST
			  */

  double current_likelihood;   /* Likelihood used by Dijkstras - after modifying them, put them here */
  double old_likelihood; /* Temporary storage used by K-shortest to save old likelihood when setting new one to -1 */
  int init_reversible; /* -1 backwards only, 0 reversible, 1 forward only */
  int net_reversible;   /* Reversibility used by Dijkstras and Simplex */
  double lb;  double ub; /* LB and UB (should be consistent with net_reversible - net_reversible < 0 means lb < 0 and ub = 0) */

  int revPair; /* The iD of the reaction gonig the other way in the network (if there is one) and -1 if none */
  vector<int> syn;    /* Special vector for storing reactions in the same reaction-class */
  vector<ANNOTATION> annote; /* List of gene annotations */
  
  bool operator==(const REACTION &rhs) const;
  bool operator>(const REACTION &rhs) const;
  bool operator<(const REACTION &rhs) const;
  REACTION();
  void reset();

  /* Trivially calculatable properties */
  bool isExchange() const;

  string as_json();
  string dump();
};

#endif
