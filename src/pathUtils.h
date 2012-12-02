#ifndef _PATHUTILS
#define _PATHUTILS

#include "Models.h"
#include <set>
#include <map>
#include <string>

bool isExternalCompound(ModelCompoundPtr compound, Problem& problem);

set<CompoundPtr> getCofactorList(Problem &problem);

map<ReactionPtr, vector<ReactionPtr> > computeSynonyms(Problem &problem);

MediaPtr findMedia(string name);

MetabolicModel findPaths(Problem &problem, map<ReactionPtr, vector<ReactionPtr> > synonymList, MediaPtr media, vector<BiomassPtr> biomasses );


#endif
