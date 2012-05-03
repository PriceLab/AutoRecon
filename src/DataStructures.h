// Data structures used throughout the project

#ifndef _DATASTRUCTURES_H
#define _DATASTRUCTURES_H

#include <algorithm>
#include <climits>
#include <map>
#include <string>
#include <vector>

using std::vector;
using std::map;
using std::string;

#include "Metabolite.h"
#include "Reaction.h"
#include "Stoich.h"
#include "Annotation.h"
#include "PathStuff.h"

/* Input data classes */
struct MEDIA;
struct KNOCKOUT;
struct ANNOTATION;
class STOICH;
class REACTION;
class GROWTH;
class RXNSPACE;
class METSPACE;

class PATH;
class PATHSUMMARY;

class VALUESTORE;
class GRAPHSTORE;
class BADIDSTORE;

class GAPFILLRESULT;
class ANSWER;
class SIMULATIONRESULT;
class KORESULT;
class SCORE;

class RXNSPACE{
 public:
  vector<REACTION> rxns;

  RXNSPACE();
  RXNSPACE(const vector<REACTION> &rxnVec);
  RXNSPACE(const RXNSPACE& existingSpace, const vector<int> &idSubset);

  void clear();

  void changeReversibility(int id, int new_rev);
  void change_Lb(int id, double new_lb);
  void change_Ub(int id, double new_ub);
  void change_Lb_and_Ub(int id, double new_lb, double new_ub);

  void changeId(int oldId, int newId);

  void addReactionVector(const vector<REACTION> &rxnVec);
  void addReaction(const REACTION &rxn);
  void removeRxnFromBack();
  REACTION rxnFromId(int id) const;
  REACTION* rxnPtrFromId(int id);
  const REACTION* rxnPtrFromId(int id) const;
  int idxFromId(int id) const;
  bool idIn(int id) const;
  void rxnMap();

  RXNSPACE operator=(const RXNSPACE& init);
  REACTION & operator[](int idx);
  bool operator==(const RXNSPACE &rhs);

 private:
  /* Note - this is just a REFERENCE POINT - it always starts at 0 and all changes to ATPM are relative to whatever the user inputs */
  double currentAtpm;
  map<int,int> Ids2Idx;
  int numRxns;

};

class METSPACE{
 public:
  vector<METABOLITE> mets;

  METSPACE();
  METSPACE(const vector<METABOLITE> &metVec);
  METSPACE(const METSPACE &existingSpace, const vector<int> &idSubset);
  METSPACE(const RXNSPACE &rxnspace, const METSPACE &largeMetSpace);

  void clear();
  void removeMetFromBack();
  void addMetabolite(const METABOLITE &met);
  METABOLITE metFromId(int id) const;
  METABOLITE* metPtrFromId(int id);
  int idxFromId(int id) const;
  bool idIn(int id) const;
  void metMap();

  METSPACE operator=(const METSPACE& init);
  METABOLITE & operator[](int idx);
  map<int, int> Ids2Idx;

 private:
  int numMets;
};

class PROBLEM{
 public:
  RXNSPACE fullrxns;
  RXNSPACE synrxns;
  RXNSPACE synrxnsR;
  RXNSPACE exchanges;

  METSPACE secondaryLones;
  METSPACE metabolites;
  METSPACE cofactors;

  vector<GROWTH> growth;

  PROBLEM(const RXNSPACE &x, const PROBLEM &ProblemSpace);
  PROBLEM();
  void clear();

  void to_jsonfile(const char* filename);
};

class GROWTH{
 public:
  /* Note that these should be ID's not Indexes */
  vector<MEDIA> media;
  vector<MEDIA> byproduct;
  /* FIXME: Needs secretion rates here (default would be 1000) */
  vector<STOICH> biomass;

  vector<KNOCKOUT> mutation;
  double growth_rate;

  GROWTH();
  void reset();
};

struct MEDIA{
  int id; /* Metabolite id */
  char name[64]; /* Metabolie name */
  double rate; /* mmol/gDW/hr 
		  (uptake rate for media, secretion rate for byproducts */
  bool operator==(const MEDIA &rhs) const;
  bool operator>(const MEDIA &rhs) const;
  bool operator<(const MEDIA &rhs) const;
  
  MEDIA();
};

struct KNOCKOUT{
  int id;
  string genename;
  double act_coef; /* percentage of maximum growth rate achieved with knockout (0 = lethal knockout, 1 = wild type growth rate) */
};





class NETREACTION{
 public:
  /* Reaction IDs that create the NETREACTION (negative numbers are used in reverse) */
  vector<int> rxnDirIds;
  /* The NETREACTION itself */
  REACTION rxn;
  bool operator==(const NETREACTION &rhs) const;
};

/* This is to be used only for the purposes of the priority queue. I put it this way so that
   we can keep track of index but still use the priority queue to make minimization more efficient. */
class VALUESTORE{
 public:
  int id;
  double value;
  /* The > is not a typo - we are looking for the MINIMUM and not the MAXIMUM so we need to switch the sign
     to make it work with priority_queue... 
   Incidentally this also makes it so that a vector<VALUESTORE> sorts max to min instead of min to max ... */
  bool operator<(const VALUESTORE &rhs) const { return this[0].value > rhs.value; }
};

/* Setup priority queue for K-shortest (again, we want the minimum and not the maximum)
   We exclude specific reactions from a particular PATH and then pass those onto the next iteration */
class GRAPHSTORE{
 public:
  vector<int> excludedRxnIds;
  PATH path;  
  bool operator<(const GRAPHSTORE &rhs) const { return this[0].path.totalLikelihood > rhs.path.totalLikelihood; }
};

class GAPFILLRESULT{
 public:  
  int deadMetId; /* ID of any essential magic exits given the specified combination of PATHSUMMARY */
  vector< vector<int> > deadEndSolutions; /* Dijkstras solutions for metabolite deadMetId - deadEndSolutions[i] is the i'th shortest gapfill solution */
};

/* Setup for storage of ID's of reactions that cannot be used in Dijkstras algorithm and which metablites block them.
   Since we use a SET to store them we need a "<" operator - I order them first by reaction ID and then by length of the met ID vectors,
   and finally by the values in the met ID vector themselves (if they are the same)

   To remove the values I also need an == operator. */
class BADIDSTORE {
 public:
  int badRxnId;
  vector<int> badMetIds;
  BADIDSTORE();
  bool operator<(const BADIDSTORE &rhs) const;
  bool operator==(const BADIDSTORE &rhs) const;
};

class INNERPOPSTORE {
 public:
  double score;
  vector<int> whichK;
  vector<int> essentialExits;
  INNERPOPSTORE();
  bool operator<(const INNERPOPSTORE &rhs) const;
  bool operator==(const INNERPOPSTORE &rhs) const;
};

class SCORE1{
 public:
  double score;

  //parameter
  double gam;
  double ngam;
  vector<REACTION> ETC_adjusted;
  map<int,double> growthRate;
  map<int,double> growthScore;
};

class ANSWER {
 public:
  /*  Reaction IDs in the model
      Path numbers
      Bottlenecks fixed
       Parameters (ETC/ATPM)
       Growth rates (Experimental)
       Growth rates (Predicted by algorithm)
       Secretion rates (Experimental) for each media in the PROBLEM
       Secretion rates (Predicted by algorithm) for each media in the PROBLEM
       Total network likelihood */

  static map<string, vector<VALUESTORE> > annoteToRxns;
  static vector<NETREACTION> etc;
  static vector<vector<vector<PATHSUMMARY> > > pList; /* indexed by growth cond, metabolite, k */

  RXNSPACE reactions;
  METSPACE metabolites;
  /* Set of path IDs used in model (the genetic algorithm will shuffle these) */
  vector<const PATHSUMMARY*> passedPaths;

  vector<const METABOLITE*> fixedGaps;
  vector<vector<const REACTION*> > fixes; //indexed by the fixedGaps they solve 

  vector<const REACTION*> essentialMagicExits;

  /* ETC IDs (not sure how I'll do this) for ETCs connected to our network via ETC_CONNECT */
  vector<const REACTION*> etcIds; 
  vector<const REACTION*> etcConnect;
  vector<KORESULT> knockoutResults;

  //optimum scoring answer
  SCORE1 optLocalScore;

  void to_jsonfile(const char* filename);


  ANSWER();
  ANSWER(const ANSWER &source);

};

class KORESULT{

};

/* Initialize static members of the ANSWER class */
void initializeAnswer(const vector<NETREACTION> &myEtc, const vector<vector<vector<PATHSUMMARY> > > &myPlist, const map<string, vector<VALUESTORE> > &annoteToRxn);

#endif // _DATASTRUCTURES_H
