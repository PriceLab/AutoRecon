#ifndef _MODELREACTION_H
#define _MODELREACTION_H

#include <string>
#include <tr1/memory>
#include <ostream>
#include <vector>
#include <jsoncpp/json/json.h>
#include "ModelCompound.h"
#include "ModelCompartment.h"
#include "Reaction.h"

using namespace std;

//! Reaction reagent in metabolic model domain.

class ModelReactionReagent
{
public:

	//! \brief Constructor.
	//! \param[in] val Object represented in JSON format.

	ModelReactionReagent(Json::Value val);

	//! Universally unique identifier to model compound.
	string modelCompoundUuid;

	//! ??
	double coefficient;

	//! Pointer to model compound.
	ModelCompoundPtr modelCompound;

};

//! Smart pointer for ModelReactionReagent class.
typedef std::tr1::shared_ptr<ModelReactionReagent> ModelReactionReagentPtr;

//! Reaction protein subunit in metabolic model domain.

class ModelReactionProteinSubunit
{
public:

	//! \brief Constructor.
	//! \param[in] val Object represented in JSON format.

	ModelReactionProteinSubunit(Json::Value val);

	//! ??
	string roleUuid;

	//! ??
	bool triggering;

	//! ??
	vector<string> geneList;

	//! ??
	bool optional;

};

//! Smart pointer for ModelReactionProteinSubunit class.
typedef std::tr1::shared_ptr<ModelReactionProteinSubunit> ModelReactionProteinSubunitPtr;

//! Reaction protein in metabolic model domain.

class ModelReactionProtein
{
public:

	//! \brief Constructor.
	//! \param[in] val Object represented in JSON format.

	ModelReactionProtein(Json::Value val);

	//! List of reaction protein subunits in reaction protein.
	vector<ModelReactionProteinSubunitPtr> reactionProteinSubunitList;

	//! ??
	string complexUuid;

	//! ??
	string note;

};

//! Smart pointer for ModelReactionProtein class.
typedef std::tr1::shared_ptr<ModelReactionProtein> ModelReactionProteinPtr;

//! Reaction in metabolic model domain.

class ModelReaction
{
public:

	//! \brief Constructor.
	//! \param[in] val Object represented in JSON format.

	ModelReaction(Json::Value val);

	//! ??
	string direction; // need a constant

	//! Universally unique identifier to model compartment.
	string modelCompartmentUuid;

	//! List of reagents in model reaction.
	vector<ModelReactionReagentPtr> modelReactionReagentList;

	//! Last modification date.
	string modDate;

	//! List of proteins in model reaction.
	vector<ModelReactionProteinPtr> modelReactionProteinList;

	//! Universally unique identifier.
	string uuid;

	//! ??
	int protons;

	// Universally unique identifier to biochemistry reaction.
	string reactionUuid;

	//! Pointer to model compartment.
	ModelCompartmentPtr modelCompartment;

	//! Pointer to biochemistry reaction.
	ReactionPtr reaction;

};

//! Smart pointer for ModelReaction class.
typedef std::tr1::shared_ptr<ModelReaction> ModelReactionPtr;

//! Formatted output operator for ModelReaction class.
std::ostream& operator<<(std::ostream& out, ModelReaction& obj);

#endif
