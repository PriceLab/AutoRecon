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
void tracePath(const METSPACE &metspace, const RXNSPACE &rxnspace, 
	       map<METID, RXNID>  &precursorRxnIds, map<METID, bool> &metsExplored, 
	       vector<RXNID> &rxnIds, vector<METID> &inputIds,
               vector<REV> &rxnDirections, queue<METID> &nodeList);
vector<METID> opposite(const REACTION* rxn, METID metId, REV sgn);

#endif
