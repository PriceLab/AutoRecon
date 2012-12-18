#ifndef _REACTION_H
#define _REACTION_H

#include <string>
#include <tr1/memory>
#include <ostream>
#include <map>
#include <vector>
#include "Reagent.h"

//! Reaction in biochemistry domain.

class Reaction
{
public:

	//! \brief Constructor.
	//! \param[in] val Object represented in JSON format.

	Reaction(Json::Value val);

	//! Additional information about reaction, generally indicating whether reaction is balanced and/or lumped.
	//! \note CI means charge imbalance, MI means mass imbalance, HI means hydrogen imbalance, SP means
	//!       spontaneous reactions, UN means unknown, FO means ?, RC means ?.
	string status;

	//! Direction of reaction (constants below).
	string direction;

	//! Error bound on Gibbs free energy computation for compound (kcal/mol).
	double deltaGErr;

	//! List of cues for reaction.
	map<string,string> cueList;

	//! Descriptive name of reaction.
	string name;

	//! ??
	double probability;

	//! Last modification date.
	string modDate;

	//! Abbreviated name for reaction.
	string abbreviation;

	//! Checksum (unused).
	string cksum;

	//! Computed Gibbs free energy of formation for compound at pH 7 (kcal/mol).
	double deltaG;

	//! Number of protons absorbed by this reaction at pH 7.
	double defaultProtons;

	//! List of reagents in reaction.
	vector<ReagentPtr> reagentList;

	//! ??
	string complexInfo;

	//! Computed reversibility of reaction at pH 7.
	string thermoReversibility;

	//! Gene-protein-reaction association.
	string GPR;

	//! Universally unique identifier.
	string uuid;

};

//! Smart pointer for Reaction class.
typedef std::tr1::shared_ptr<Reaction> ReactionPtr;

//! Formatted output operator for Reaction class.
std::ostream& operator<<(std::ostream& out, Reaction& obj);

#endif
