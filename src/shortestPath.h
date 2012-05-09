#ifndef shortestPath_h
#define shortestPath_h

#include "DataStructures.h"
#include "pathUtils.h"

#include <queue>
#include <map>
#include <set>
#include <vector>

using std::map;
using std::queue;
using std::set;
using std::vector;

PATH findShortestPath(const RXNSPACE &rxnspace, const METSPACE &metspace, const METSPACE &inputs, const METABOLITE &output);
void tracePath(const METSPACE &metspace, const RXNSPACE &rxnspace, map<int, int>  &precursorRxnIds, map<int, bool> &metsExplored, vector<int> &rxnIds, vector<METID> &inputIds,
               vector<int> &rxnDirections, queue<METID> &nodeList);
vector<METID> opposite(const REACTION* rxn, METID metId, int sgn);

#endif
