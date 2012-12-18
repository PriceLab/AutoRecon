#ifndef _MODELS_H
#define _MODELS_H

#include <string>
#include <tr1/memory>
#include <ostream>
#include <map>
#include <vector>
#include "Biomass.h"
#include "ModelCompound.h"
#include "ModelCompartment.h"
#include "ModelReaction.h"

using namespace std;

//! Forward-only reaction direction.
const string Forward = ">";

//! Backward-only reaction direction.
const string Backward = "<";

//! Bidirectional reaction.
const string Bidirectional = "=";

typedef map<string,ModelCompartmentPtr> ModelCompartmentMap;
typedef pair<string,ModelCompartmentPtr> ModelCompartmentMapValue;
typedef map<string,ModelCompartmentPtr>::iterator ModelCompartmentMapIterator;

typedef map<string,ModelCompoundPtr> ModelCompoundMap;
typedef pair<string,ModelCompoundPtr> ModelCompoundMapValue;
typedef map<string,ModelCompoundPtr>::iterator ModelCompoundMapIterator;

typedef vector<BiomassPtr>::iterator BiomassVectorIterator;

//! Metabolic model domain data.

class MetabolicModel
{
public:

	//! \brief Default constructor.
	MetabolicModel();

	//! \brief Load a model object from a file.
	//! \param[in] filename Path to file containing JSON representation.
	//! \return True when loaded successfully.

	bool loadFromFile(const char *filename);

	//! Universally unique identifier to annotation domain data.
	string annotationUuid;

	//! Universally unique identifier to biochemistry domain data.
	string biochemistryUuid;

	//! List of biomasses in metabolic model.
	vector<BiomassPtr> biomassList;

	//! ??
	int current;

	//! ??
	string defaultNameSpace;

	//! ??
	double growth;

	//! ??
	string id;

	//! Universally unique identifier to mapping domain data.
	string mappingUuid;

	//! Last modification date.
	string modDate;

	// Map of model compartments where key is model compartment uuid.
	ModelCompartmentMap modelCompartmentList;

	//! Map of model compounds where key is model compound uuid.
	ModelCompoundMap modelCompoundList;

	//! List of reactions.
	vector<ModelReactionPtr> modelReactionList;

	//! Descriptive name of metabolic model.
	string name;

	//! Indication of whether model is stable, under construction, or under reconstruction.
	string status;

	//! Indication of where model came from (e.g. single genome, multiple genome, or community model).
	string type;

	//! Universally unique identifier.
	string uuid;

	//! Version number of metabolic model.
	int version;

};

//! Smart pointer for MetabolicModel class.
typedef std::tr1::shared_ptr<MetabolicModel> MetabolicModelPtr;

//! Formatted output operator for Metabolic model class.
std::ostream& operator<<(std::ostream& out, MetabolicModel& obj);

#endif // _MODELS_H
