#ifndef __PATH_STUFF_H__
#define __PATH_STUFF_H__

#include <map>
#include <vector>
#include <string.h>
#include <string>

using namespace std;

class PATH{
 public:
  int outputId; /* The Path by definition is trying to reach a specific output. This lists that 
		   output */
  vector<int> inputIds; /* Inputs that were needed to reach the output */
  vector<int> rxnIds; /* Reactions in the path */
  vector<int> rxnDirection; /* Dijkstras will return this... which direction is the rxn going in 
			       the particular path? */
  vector<int> metsConsumedIds; /* Metabolites that are nodes on the path tree for this particular 
				  path */
  vector<int> deadEndIds; /* Dead end metabolites on the way to reach the specified output 
			     (EMPTY if no path is found) */
  vector<int> rxnPriority; /* Priority for particular reaction [higher priority means the reaction was closer to the beginning of the pathway] - useful for gapfind */
  double totalLikelihood; /* Sum of provided likelihoods for a given path */
  PATH();
};


class PATHSUMMARY{
 public:

  /* Member variables: *******************************************************/
  long int id;
  int outputId; /* Output (target component) for the given path - needed to run FBA / magic 
		   exits */
  vector<int> growthIdx; /* Which growth does it correspond to? - MB - did we decide on an Id or Idx for 
		    this? I guess since we'll number the growth ourselves and probably just 
		    enumerate it will be the same thing */
  int k_number; /* I assume this is whether this is the 1st, 2nd or kth path in a list */
  vector<int> rxnDirIds;
  vector<int> rxnPriority;
  vector<int> deadEndIds;
  vector<int> metsConsumed;
  double likelihood;

  /* Constructor & functions: ************************************************/
  PATHSUMMARY();
  bool metIn(int metId) const;
  bool operator==(const PATHSUMMARY &rhs1);
  PATHSUMMARY clear();
  PATHSUMMARY operator=(PATH &onepath);
  PATHSUMMARY operator/(PATH &onepath);

  string as_json();
};

#endif
