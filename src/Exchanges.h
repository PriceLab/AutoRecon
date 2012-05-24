#ifndef EXCHANGES_H_
#define EXCHANGES_H_

#include <vector>
#include "DataStructures.h"

void LoadAllExchanges(PROBLEM &ProblemSpace);
void FeedTheBeast(RXNSPACE &inModel,GROWTH &growth);
void FeedEnergy(PROBLEM &Model,double flux_bound);

REACTION MagicTransport(const vector<REACTION> &reaction, const METSPACE &metspace, METID met_id, char *name, int R);
REACTION MagicTransport(const vector<REACTION> &reaction, const METSPACE &metspace, METID met_id, char *name, int R,double bound);
REACTION MagicExchange(METABOLITE met,double flux_bound,int dir);

void GetExchangeReactions(vector<METID> metIdList, vector<int> directions, const PROBLEM &ProblemSpace, RXNSPACE &exchanges);
void GetExchangeReactions_byproducts(const GROWTH &growth, const PROBLEM &ProblemSpace, RXNSPACE &eList);
void GetExchangeReactions_oneMedia(const GROWTH &growth, const PROBLEM &ProblemSpace, RXNSPACE &eList);
void GetTransportReactions(vector<METID> metIds, vector<int> rxnDirection, const PROBLEM &ProblemSpace, 
			   vector<REACTION> &Transporters);
void GetTransportReactionsFromGrowth(const GROWTH &growth, vector<int> rxnDirection, const PROBLEM &ProblemSpace, 
				     vector<REACTION> &Transporters);

RXNID FindExchange4Metabolite(const vector<REACTION> &reaction, METID met_id);
RXNID FindTransport4Metabolite(const vector<REACTION> &reaction, METID met_id, REV rxnDirection);


#endif
