// Data structures used throughout the project

#ifndef _DATASTRUCTURES_H
#define _DATASTRUCTURES_H

#include <algorithm>
#include <climits>
#include <map>
#include <string>
#include <vector>
#include "MyConstants.h"
//#include <boost/serialization/strong_typedef.hpp>

using std::vector;
using std::map;
using std::string;

/*Basic typedefs*/
//BOOST_STRONG_TYPEDEF(int,METID);
//BOOST_STRONG_TYPEDEF(int,METIDX);
//BOOST_STRONG_TYPEDEF(int,RXNID);

class METID {
 public:
  METID(int i) { this->i = i; }
  int i;
  operator int() { return i; }
  operator int() const { return i; }
  METID(){};
};
class METIDX {
 public:
  METIDX(int i) { this->i = i; }
  int i;
  operator int() { return i; }
  operator int() const { return i; }
  METIDX(){};
};
class RXNID {
 public:
  RXNID(int i) { this->i = i; }
  int i;
  operator int() { return i; }
  operator int() const { return i; }
  RXNID(){};
};
class REV {
 public:
  REV(int i) { this->i = i; }
  int i;
  operator int() { return i; }
  operator int() const { return i; }
  REV(){};
};
class RXNDIRID {
 public:
  RXNDIRID(int i) { this->i = i; }
  int i;
  operator int() { return i; }
  operator int() const { return i; }
  RXNDIRID(){};
};
class RXNIDX {
 public:
  RXNIDX(int i) { this->i = i; }
  int i;
  operator int() { return i; }
  operator int() const { return i; }
  RXNIDX(){};
};
class GROWTHIDX {
 public:
  GROWTHIDX(int i) { this->i = i; }
  int i;
  operator int() { return i; }
  operator int() const { return i; }
  GROWTHIDX(){};
};

//BOOST_STRONG_TYPEDEF(int,RXNDIRID);
//BOOST_STRONG_TYPEDEF(int,RXNIDX);
//BOOST_STRONG_TYPEDEF(int,GROWID);
//BOOST_STRONG_TYPEDEF(int,GROWTHIDX);

REV convertRXNDIRID2REV(RXNDIRID x);


/* Input data classes */
struct MEDIA;
struct KNOCKOUT;
struct ANNOTATION;
class STOICH;
class REACTION;
class GROWTH;
class METABOLITE;
class RXNSPACE;
class METSPACE;

class PATH;
class PATHSUMMARY;

class VALUESTORE;
class GRAPHSTORE;

class GAPFILLRESULT;
class ANSWER;
class SIMULATIONRESULT;
class KORESULT;
class SCORE;
class STOICHMATRIX;

class RXNSPACE{
 public:
  vector<REACTION> rxns;

  RXNSPACE();
  RXNSPACE(const vector<REACTION> &rxnVec);
  RXNSPACE(const RXNSPACE& existingSpace, const vector<RXNID> &idSubset);

  void clear();

  void changeReversibility(RXNID id, REV new_rev);
  void change_Lb(RXNID id, double new_lb);
  void change_Ub(RXNID id, double new_ub);
  void change_Lb_and_Ub(RXNID id, double new_lb, double new_ub);

  void changeId(RXNID oldId, RXNID newId);

  void addReactionVector(const vector<REACTION> &rxnVec);
  void addReaction(const REACTION &rxn);
  void removeRxnFromBack();
  REACTION rxnFromId(RXNID id) const;
  REACTION* rxnPtrFromId(RXNID id);
  const REACTION* rxnPtrFromId(RXNID id) const;
  RXNIDX idxFromId(RXNID id) const;
  bool idIn(RXNID id) const;
  void rxnMap();

  vector<double> & lbVector();
  vector<double> & ubVector();

  RXNSPACE operator=(const RXNSPACE& init);
  REACTION & operator[](RXNIDX idx);
  REACTION & operator[](RXNID idx);
  bool operator==(const RXNSPACE &rhs);

  friend std::ostream& operator<<(std::ostream &out, RXNSPACE & orig);

 private:
  /* Note - this is just a REFERENCE POINT - it always starts at 0 and all changes to ATPM are relative to whatever the user inputs */
  double currentAtpm;
  map<RXNID,RXNIDX> Ids2Idx;
  int numRxns;

};

std::ostream& operator<<(std::ostream &out, RXNSPACE & orig);

class METSPACE{
 public:
  vector<METABOLITE> mets;

  METSPACE();
  METSPACE(const vector<METABOLITE> &metVec);
  METSPACE(const METSPACE &existingSpace, const vector<METID> &idSubset);
  METSPACE(const RXNSPACE &rxnspace, const METSPACE &largeMetSpace);

  void clear();
  void removeMetFromBack();
  void addMetabolite(const METABOLITE &met);
  METABOLITE getMetObj(METID id) const;
  METABOLITE* metPtrFromId(METID id);
  METIDX idxFromId(METID id) const;
  bool isIn(METID id) const;
  bool isIn(METIDX idx) const;
  void metMap();

  METSPACE operator=(const METSPACE& init);
  //METABOLITE & operator[](int idx);
  METABOLITE & operator[](METIDX idx);
  METABOLITE & operator[](METID id);
  const METABOLITE & operator[](METID id) const;
  map<METID, METIDX> Ids2Idx;

  friend std::ostream& operator<<(std::ostream &out, METSPACE & orig);

 private:
  int numMets;
};

std::ostream& operator<<(std::ostream &out, METSPACE & orig);

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

  friend std::ostream& operator<<(std::ostream &out, GROWTH & orig);
};

std::ostream& operator<<(std::ostream &out, GROWTH & orig);

struct MEDIA{
  METID id; /* Metabolite id */
  char name[AR_MAXNAMELENGTH]; /* Metabolie name */
  double rate; /* mmol/gDW/hr 
		  (uptake rate for media, secretion rate for byproducts */
  bool operator==(const MEDIA &rhs) const;
  bool operator>(const MEDIA &rhs) const;
  bool operator<(const MEDIA &rhs) const;
  friend std::ostream& operator<<(std::ostream &out, MEDIA & orig); 
 
  MEDIA() : id(-1), rate(1000.0f) {}
};

std::ostream& operator<<(std::ostream &out, MEDIA & orig);

struct KNOCKOUT{
  int id;
  string genename;
  double act_coef; /* percentage of maximum growth rate achieved with knockout (0 = lethal knockout, 1 = wild type growth rate) */
};

class METABOLITE{
 public:
  /* Externally (XML/User) defined parameters */
  METID id; /* Has to be big matrix row index */
  char name[AR_MAXNAMELENGTH];

  /* All of these are for visualization only */
  int input; /* Is it an input?: 0 for no, 1 for yes */
  int output; /* Is it an output? 0 for no, 1 for yes */
  int biomass; /* Is it a biomass component? 0 for no, 1 for yes */

  /* Used to identify synrxns (will soon be obsolete in favor of reaction-specific removals) */
  int secondary_lone; /* 0 for no, 1 for yes */
  vector<METID> secondary_pair; /* Int of the ID for each possible secondary pair */

  /* ETC */
  int noncentral; /* 0 = central (not used for ETC), 1 = noncentral (used for ETC), -1 = undefined */

  double modifier; /* Reserved for things that can be used to modify metabolite cost in Dijkstras algorithm, such as metabolomics data or other thigns we calculate */
  
  /* Connected reactions (computed) */
  vector<RXNID> rxnsInvolved_nosec;
  
  METABOLITE();
  void reset();
  int isSecondary() const;

  friend std::ostream& operator<<(std::ostream &out, METABOLITE & orig);

};

std::ostream& operator<<(std::ostream &out, METABOLITE & orig);

class STOICH{
  public:
  METID met_id;
  double rxn_coeff;
  char met_name[AR_MAXNAMELENGTH];
  /* Will this be a char or a char []? */
  char compartment;
  
  bool operator==(const STOICH &rhs) const;
  bool operator>(const STOICH &rhs) const;
  bool operator<(const STOICH &rhs) const;
  STOICH();
  STOICH(METID id, double coef) : met_id(id), rxn_coeff(coef) {}

  void reset();
  
  friend std::ostream& operator<<(std::ostream &out, STOICH & orig);
};

std::ostream& operator<<(std::ostream &out, STOICH & orig);


class REACTION{
 public:
  
  RXNID id; /* ID */
  bool transporter; /* TRUE if it is a transport reaction */
  METID transportedMetabolite; /* Metabolite ID for transported metabolite */
  char name[AR_MAXNAMELENGTH];
  vector<STOICH> stoich; /* Full chemical reaction */

  /* stoich, but without the secondary metabolties (Load_Stoic_Part deals with this) */
  /* This is the one used by Dijkstras - if you want to use fullrxns with dijkstras you need to copy it over first */
  vector<STOICH> stoich_part;

  double init_likelihood; /* based on user input:
			     1>x>0 PROBABILITY INCLUSION
			     -1 DO NOT INCLUDE (100% sure)
			     -2 HARD INCLUDE (100% sure)
			     -3 BLACK MAGIC
			     -4 SPONTANEOUS REACTION
			     -5 NOT ON HIT LIST
			  */

  REV init_reversible; /* -1 backwards only, 0 reversible, 1 forward only */
  vector<ANNOTATION> annote; /* List of gene annotations */

  /***** Calculated quantities ******/
  bool freeMakeFlag;
  vector<RXNID> syn;    /* Special vector for storing reactions in the same reaction-class */
  RXNID revPair; /* The iD of the reaction gonig the other way in the network (if there is one) and -1 if none */
  double current_likelihood;   /* Likelihood used by Dijkstras - after modifying them, put them here */
  double old_likelihood; /* Temporary storage used by K-shortest to save old likelihood when setting new one to -1 */
  REV net_reversible;   /* Reversibility used by Dijkstras and Simplex */
  double lb;  double ub; /* LB and UB (should be consistent with net_reversible - net_reversible < 0 means lb < 0 and ub = 0) */
  
  bool operator==(const REACTION &rhs) const;
  bool operator>(const REACTION &rhs) const;
  bool operator<(const REACTION &rhs) const;
  REACTION();
  void reset();

  /* Trivially calculatable properties */
  bool isExchange() const;

  friend std::ostream& operator<<(std::ostream &out, REACTION & orig);

};

std::ostream& operator<<(std::ostream &out, REACTION & orig);

struct ANNOTATION{
  double probability;
  string genename;
  /* THis compares > because we want to go in opposite order.. when sorting these. */
  bool operator<(const ANNOTATION &rhs) const;
};

class NETREACTION{
 public:
  /* Reaction IDs that create the NETREACTION (negative numbers are used in reverse) */
  vector<RXNDIRID> rxnDirIds;
  /* The NETREACTION itself */
  REACTION rxn;
  bool operator==(const NETREACTION &rhs) const;
};


/* WARNING: 
   When the new (LP-based) shortest path algorithm is implemented,
   this class will go away!

   The new algorithm will just output a PATHSUMMARY directly.
*/
class PATH{
 public:
  METID outputId; /* The Path by definition is trying to reach a specific output. This lists that 
		   output */
  vector<METID> inputIds; /* Inputs that were needed to reach the output */
  vector<RXNID> rxnIds; /* Reactions in the path */
  vector<REV> rxnDirection; /* Dijkstras will return this... which direction is the rxn going in 
			       the particular path? */
  vector<METID> metsConsumedIds; /* Metabolites that are nodes on the path tree for this particular 
				  path */
  vector<METID> deadEndIds; /* Dead end metabolites on the way to reach the specified output 
			     (EMPTY if no path is found) */
  double totalLikelihood; /* Sum of provided likelihoods for a given path */
  PATH();
};

class PATHSUMMARY{
 public:
  long int id;
  METID outputId; /* Output (target component) for the given path - needed to run FBA / magic 
		   exits */
  vector<int> growthIdx; /* Which growth does it correspond to? - MB - did we decide on an Id or Idx for 
		    this? I guess since we'll number the growth ourselves and probably just 
		    enumerate it will be the same thing */
  int k_number; /* I assume this is whether this is the 1st, 2nd or kth path in a list */
  vector<RXNDIRID> rxnDirIds;
  vector<RXNID> rxnPriority; /*not even sure what this does   just making it work*/
  vector<METID> deadEndIds;
  vector<METID> metsConsumed;
  double likelihood;
  PATHSUMMARY();
  bool metIn(METID metId) const;
  bool operator==(const PATHSUMMARY &rhs1);
  PATHSUMMARY clear();
  PATHSUMMARY operator=(PATH &onepath);
  PATHSUMMARY operator/(PATH &onepath);
};

/* This is to be used only for the purposes of the priority queue. I put it this way so that
   we can keep track of index but still use the priority queue to make minimization more efficient. */
class VALUESTORE{
 public:
  METID id;
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
  vector<RXNID> excludedRxnIds;
  PATH path;  
  bool operator<(const GRAPHSTORE &rhs) const { return this[0].path.totalLikelihood > rhs.path.totalLikelihood; }
};

class GAPFILLRESULT{
 public:  
  METID deadMetId; /* ID of any essential magic exits given the specified combination of PATHSUMMARY */
  vector< vector<RXNID> > deadEndSolutions; /* Dijkstras solutions for metabolite deadMetId - deadEndSolutions[i] is the i'th shortest gapfill solution */
};

class INNERPOPSTORE {
 public:
  double score;
  vector<int> whichK;
  vector<RXNID> essentialExits;
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
  map<GROWTHIDX,double> growthRate;
  map<GROWTHIDX,double> growthScore;
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
  static vector<vector<vector<PATHSUMMARY> > > pList;

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

  ANSWER();

  //copy constructor
  ANSWER(const ANSWER &source){
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

};


// A sparse matrix primarily used in FBA. Sparse matrix used to reduce the
//   space used by a matrix of mostly (99%) zeros.
class STOICHMATRIX{
public:
  // vector of map<int, double> because it allows for a call like a normal
  //   matrix. Ex: matrix[int][int] gives you a double if the value isn't zero.
  // IMPORTANT NOTE: do not ask for a value that is zero. It will create an
  //   empty cell, since the cell itself does not exist in a sparse matrix
  //   format. If you must iterate through the matrix, iterate through the
  //   vector normally, then use a map iterator to iterate through the map.
  //   If you wish to access the matrix, and you are unfamiliar with how to
  //   handle a vector of maps, use the access functions at a cost of slightly
  //   increased run time.
  // TIP: to test if a map element contains a value, use 
  //   int map<int,double>::count(int index). Empty returns 0.
  vector<map<int, double> > matrix; // sparse matrix format

  // converts from index to reaction/metabolite ID
  vector<RXNID> yIDs;
  vector<METID> xIDs;

  // Converts from reaction/metabolite ID to index
  // The same applies to these maps. If you are unsure how to handle a map,
  //   use the access functions. These are left public for use by non-member
  //   functions.
  map<RXNID, int> yIdx;
  map<METID, int> xIdx;
  int sizeX; // number of metabolites
  int sizeY; // number of reactions



  // constructors
  STOICHMATRIX() : sizeX(0), sizeY(0) {}
  STOICHMATRIX(const RXNSPACE & rxns) : sizeX(0), sizeY(0)
               { buildStoich(rxns.rxns); }
  STOICHMATRIX(const vector<REACTION> & rxns) : sizeX(0), sizeY(0)
               { buildStoich(rxns); }
  STOICHMATRIX(const vector<vector<STOICH> > & rxns) : sizeX(0), sizeY(0)
               { buildStoich(rxns); }
  // this last constructor does not work yet, due to iterator problems.
  STOICHMATRIX(const vector<map<int,double> > rxns)
               { buildStoich(rxns); }

  // the initialization functions for the constructors. Left public for
  //   manual initialization functionality.
  void buildStoich(const vector<REACTION> & rxns);
  void buildStoich(const vector<vector<STOICH> > & rxns);
  // this last buildStoich function does not work yet, due to iterator problems
  void buildStoich(const vector<map<int, double> > & rxns); 

  // functions to manually build the STOICHMATRIX. Returns 0 if unsuccessful
  // Not yet tested. If problems, email: colin.hoffman48@gmail.com
  bool addReaction(REACTION &rxn); // Not yet tested
  bool addEmptyReaction(RXNID id);      // Not yet tested
  bool addEmptyMetabolite(METID id);    // Not yet tested

  // calling the stoich matrix by [index] will result in the row map being
  //   returned. This allows for the calling of a cell by
  //   STOICHMATRIX[int][int].
  // Just remember that calling an empty cell will create an empty cell.
  map<int, double> & operator[](int i) { return matrix[i]; }

  // access functions
  // matrix get access by index. Returns unchangeable value
  double getCell(int x, int y);
  // matrix set access by index. Returns changeable value. This means the value
  //   for the cell can be set in the val parameter or by altering the return
  //   value outside of the function.
  double& setCell(int x, int y, double val=0.0);
  // matrix get access by IDs. Returns unchangeable value
  double getCell(METID x, RXNID y);
  // matrix set access by index. Returns changeable value. This means the value
  //   for the cell can be set in the val parameter or by altering the return
  //   value outside of the function.
  double& setCell(METID x, RXNID y, double val=0.0);
  // Reaction ID to index. Returns -1 if unsuccessful
  int rxnIDtoIDX(RXNID id);
  // Reaction index to ID. Returns -1 if unsuccessful
  RXNID rxnIDXtoID(int idx);
  // Metabolite ID to index. Returns -1 if unsuccessful
  int metIDtoIDX(METID id);
  // Metabolite index to ID. Returns -1 if unsuccessful
  METID metIDXtoID(int idx);

  // Allows for the creation of an objective function to maximize or minimize
  //   a reaction or metabolite.
  vector<double> & getObjFromMetID(const METID & id);
  vector<double> & getObjFromRxnID(const RXNID & id);

  // These functions create vectors to be used in manual runs of FBA
  // creates a proper size Lesser Equal Greater vector {-1,0,1} of all equal {0}
  vector<int> & equalLEG();
  // creates a proper size Right Hand Size value vector of all 0
  vector<double> & zeroRHS();

  // output operator.
  // OUTPUT:
  // (x,y): z        for all x,y that contain a value z.
  // (x1,y1): z1     example line
  // (x2,y2): z2     example line
  //   [etc.]
  friend std::ostream& operator<<(std::ostream &out, STOICHMATRIX & orig);

};

std::ostream& operator<<(std::ostream &out, STOICHMATRIX & orig);

class KORESULT{

};

/* Initialize static members of the ANSWER class */
void initializeAnswer(const vector<NETREACTION> &myEtc, const vector<vector<vector<PATHSUMMARY> > > &myPlist, const map<string, vector<VALUESTORE> > &annoteToRxn);

#endif // _DATASTRUCTURES_H
