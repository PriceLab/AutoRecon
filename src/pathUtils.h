#ifndef _PATHUTILS
#define _PATHUTILS

#include "Models.h"
#include <set>
#include <map>

bool isExternalCompound(ModelCompoundPtr compound, Problem& problem);

set<CompoundPtr> getCofactorList(Problem &problem);

map<ReactionPtr, vector<ReactionPtr> > computeSynonyms(Problem &problem);

void findPaths(Problem &problem, map<ReactionPtr, vector<ReactionPtr> > synonymList, CompoundPtr biomassComponent );

#endif
