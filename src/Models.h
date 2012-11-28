#ifndef _MODELS_H
#define _MODELS_H

#include <string>
#include <map>
#include <vector>
#include <tr1/memory>
#include <jsoncpp/json/json.h>

using namespace std;

//! Alias set (do not understand this yet).

class AliasSet
{
public:

	//! \brief Constructor.
	//! \param[in] val Object represented in JSON format.

	AliasSet(Json::Value val);

	//! Name of alias set.
	string name;

	//! ??
	string attribute;

	//! Universally unique identifier.
	string uuid;

	//! ??
	string source;

	//! Last modification date.
	string modDate;

	//! ??
	string className;

	//! ??
	map<string,vector<string> > aliasList;

};

//! Smart pointer for AliasSet class.
typedef std::tr1::shared_ptr<AliasSet> AliasSetPtr;

//! Formatted output operator for AliasSet class.
std::ostream& operator<<(std::ostream& out, AliasSet& obj);

//! Cue (do not understand this yet).

class Cue
{
public:

	//! \brief Constructor.
	//! \param[in] val Object represented in JSON format.

	Cue(Json::Value val);

	//! Abbreviation for cue.
	string abbreviation;

	//! ??
	double deltaGErr;

	//! Universally unique identifier.
	string uuid;

	//! ??
	bool smallMolecule;

	//! Last modification date.
	string modDate;

	//! ??
	int priority;

	//! Checksum (unused).
	string cksum;

	//! ??
	double deltaG;

	//! ??
	string formula;

	//! ??
	string unchargedFormula;

	//! ??
	int defaultCharge;

	//! Name of cue.
	string name;

};

//! Smart pointer for Cue class.
typedef std::tr1::shared_ptr<Cue> CuePtr;

//! Formatted output operator for Cue class.
std::ostream& operator<<(std::ostream& out, Cue& obj);

//! Cell compartment in biochemistry domain.

class Compartment
{
public:

	//! \brief Constructor.
	//! \param[in] val Object represented in JSON format.

	Compartment(Json::Value val);

	//! Simple id string.
	string id;

	//! Relative position in hierarchy of cell compartments.
	int hierarchy;

	//! Universally unique identifier.
	string uuid;

	//! Name of compartment.
	string name;

	//! Last modification date.
	string modDate;

};

//! Smart pointer for Compartment class.
typedef std::tr1::shared_ptr<Compartment> CompartmentPtr;

//! Formatted output operator for Compartment class.
std::ostream& operator<<(std::ostream& out, Compartment& obj);

//! Compound in biochemistry domain.

class Compound
{
public:

	//! \brief Constructor.
	//! \param[in] val Object represented in JSON format.

	Compound(Json::Value val);

	//! Error bound on Gibbs free energy computation for compound.
	double deltaGErr;

	//! Hash of cue uuids with cue coefficients as values.
	map<string,int> cueList;

	//! Name of compound.
	string name;

	//! Hash of pKa values with atom numbers as values.
	map< string,vector<double> > pkaList;

	//! Last modification date.
	string modDate;

	//! Hash of pKb values with atom numbers as values.
	map< string,vector<double> > pkbList;

	//! Abbreviation for compound.
	string abbreviation;

	//! Atomic mass of the compound.
	double mass;

	//! True if universal cofactor (e.g. water/H+).
	bool isCofactor;

	//! Not currently implemented.
	string cksum;

	//! Computed Gibbs free energy value for compound at pH 7.
	double deltaG;

	//! Formula for the compound at pH 7.
	string formula;

	//! Formula for compound if it does not have a ionic charge.
	string unchargedFormula; // Unused

	//! Array of associated molecular structures.
	vector<string> structureUuidList; // What is this a reference to?

	//! Computed charge for compound at pH 7.
	int defaultCharge;

	//! Universally unique identifier.
	string uuid;

};

//! Smart pointer for Compound class.
typedef std::tr1::shared_ptr<Compound> CompoundPtr;

//! Formatted output operator for Compound class.
std::ostream& operator<<(std::ostream& out, Compound& obj);

//! Media compound in biochemistry domain.

class MediaCompound
{
public:

	//! \brief Constructor.
	//! \param[in] val Object represented in JSON format.

	MediaCompound(Json::Value val);

	//! ?? what units
	double concentration;

	//! ??
	double maxFlux;

	//! ??
	double minFlux;

	//! Universally unique identifier to compound.
	string compoundUuid;

	//! Pointer to the biochemistry compound.
	CompoundPtr compound;
};

//! Smart pointer for MediaCompound class.
typedef std::tr1::shared_ptr<MediaCompound> MediaCompoundPtr;

//! Formatted output operator for MediaCompound class.
std::ostream& operator<<(std::ostream& out, MediaCompound& obj);

//! Media in biochemistry domain.

class Media
{
public:

	//! \brief Constructor.
	//! \param[in] val Object represented in JSON format.

	Media(Json::Value val);

	//! Name of media.
	string name;

	//! Type of media.
	string type;

	//! ??
	bool isDefined;

	//! Last modification date.
	string modDate;

	//! ??
	bool isMinimal;

	//! List of media compounds.
	vector<MediaCompoundPtr> mediaCompoundList;

	//! ??
	string id;

	//! Universally unique identifier.
	string uuid;

};

//! Smart pointer for Media class.
typedef std::tr1::shared_ptr<Media> MediaPtr;

//! Formatted output operator for Media class.
std::ostream& operator<<(std::ostream& out, Media& obj);

//! Reagent in biochemistry domain.

class Reagent
{
public:

	//! \brief Constructor.
	//! \param[in] val Object represented in JSON format.

	Reagent(Json::Value val);

	//! ?? what units.
	int coefficient;

	//! ??
	bool isCofactor;

	//! Universally unique identifier to compound.
	string compoundUuid;

	//! Universally unique identifier to compartment.
	string compartmentUuid;

	//! Pointer to biochemistry compound.
	CompoundPtr compound;

	//! Pointer to biochemistry compartment.
	CompartmentPtr compartment;

};

//! Smart pointer for Reagent class.
typedef std::tr1::shared_ptr<Reagent> ReagentPtr;

//! Formatted output operator for Reagent class.
std::ostream& operator<<(std::ostream& out, Reagent& obj);

//! Reaction in biochemistry domain.

class Reaction
{
public:

	//! \brief Constructor.
	//! \param[in] val Object represented in JSON format.

	Reaction(Json::Value val);

	//! ??
	string status;

	//! ??
	string direction; // Need constants?

	//! ??
	double deltaGErr;

	//! List of cues for reaction.
	map<string,string> cueList;

	//! Name of reaction.
	string name;

	//! ??
	double probability;

	//! Last modification date.
	string modDate;

	//! Abbreviation for reaction.
	string abbreviation;

	//! Checksum (unused).
	string cksum;

	//! ??
	double deltaG;

	//! ??
	int defaultProtons;

	//! List of reagents in reaction.
	vector<ReagentPtr> reagentList;

	//! ??
	string complexInfo;

	//! ??
	string thermoReversibility; // Need constants?

	//! Gene-protein-reaction association.
	string GPR;

	//! Universally unique identifier.
	string uuid;

};

//! Smart pointer for Reaction class.
typedef std::tr1::shared_ptr<Reaction> ReactionPtr;

//! Formatted output operator for Reaction class.
std::ostream& operator<<(std::ostream& out, Reaction& obj);

typedef map<string,CompartmentPtr> CompartmentMap;
typedef pair<string,CompartmentPtr> CompartmentMapValue;
typedef map<string,CompartmentPtr>::iterator CompartmentMapIterator;

typedef map<string,CompoundPtr> CompoundMap;
typedef pair<string,CompoundPtr> CompoundMapValue;
typedef map<string,CompoundPtr>::iterator CompoundMapIterator;

typedef map<string,ReactionPtr> ReactionMap;
typedef pair<string,ReactionPtr> ReactionMapValue;
typedef map<string,ReactionPtr>::iterator ReactionMapIterator;

//! Biochemistry domain data.

class Biochemistry
{
public:

	//! \brief Default constructor.
	Biochemistry();

	//! Version of domain data.
	int version;

	//! List of alias sets in domain.
	vector<AliasSetPtr> aliasSetList;

	//! Universally unique identifier to biochemistry structures domain.
	string biochemistryStructures_uuid;

	//! List of cell compartments in domain.
	CompartmentMap compartmentList;

	//! List of compounds in domain.
	CompoundMap compoundList;

	//! Structural cues for parts of compound structures.
	vector<CuePtr> cueList;

	//! ??
	string defaultNameSpace;

	//! List of media in domain.
	vector<MediaPtr> mediaList;

	//! Last modification date.
	string modDate;

	//! Name of biochemistry domain.
	string name;

	//! List of reactions in domain.
	ReactionMap reactionList;

	//! Universally unique identifier.
	string uuid;

};

std::ostream& operator<<(std::ostream& out, Biochemistry& orig);

//! Compartment in metabolic model domain.

class ModelCompartment
{
public:

	//! \brief Constructor.
	//! \param[in] val Object represented in JSON format.

	ModelCompartment(Json::Value val);

	//! ?? pH value
	double pH;

	//! ??
	int compartmentIndex;

	//! ??
	double potential;

	//! Last modification date.
	string modDate;

	//! Universally unique identifier to biochemistry domain compartment.
	string compartmentUuid;

	//! ??
	string label;

	//! Universally unique identifier.
	string uuid;

	//! Pointer to biochemistry domain compartment.
	CompartmentPtr compartment;

};

//! Smart pointer for ModelCompartment class.
typedef std::tr1::shared_ptr<ModelCompartment> ModelCompartmentPtr;

//! Formatted output operator for ModelCompartment class.
std::ostream& operator<<(std::ostream& out, ModelCompartment& obj);

class ModelCompound; // Forward reference

//! Smart pointer for ModelCompound class.
typedef std::tr1::shared_ptr<ModelCompound> ModelCompoundPtr;

//! Compound in metabolic model domain.

class ModelCompound
{
public:

	//! \brief Constructor.
	//! \param[in] val Object represented in JSON format.

	ModelCompound(Json::Value val);

	// Universally unique identifier to model compartment.
	string modelCompartmentUuid;

	//! Universally unique identifier to biochemistry compound.
	string compoundUuid;

	//! ??
	int charge;

	//! ??
	string formula;

	//! Last modification date.
	string modDate;

	//! Universally unique identifier.
	string uuid;

	//! Pointer to model compartment.
	ModelCompartmentPtr modelCompartment;

	//! Pointer to the biochemistry compound.
	CompoundPtr compound;

	//! True when the compound is a transporter between compartments.
	bool isTransporter;

	//! Pointer to paired model compound when the compound is a transporter between compartments.
	ModelCompoundPtr transporterCompound;

};

//! Formatted output operator for ModelCompound class.
std::ostream& operator<<(std::ostream& out, ModelCompound& obj);

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

//! Biomass compound in metabolic model domain.

class BiomassCompound
{
public:

	//! \brief Constructor.
	//! \param[in] val Object represented in JSON format.

	BiomassCompound(Json::Value val);

	//! Universally unique identifier to model compound.
	string modelCompoundUuid;

	//! ?? what units
	double coefficient;

	//! Pointer to model compound.
	ModelCompoundPtr modelCompound;

};

//! Smart pointer for BiomassCompound class.
typedef std::tr1::shared_ptr<BiomassCompound> BiomassCompoundPtr;

//! Biomass in metabolic model domain.

class Biomass
{
public:

	//! \brief Constructor.
	//! \param[in] val Object represented in JSON format.

	Biomass(Json::Value val);

	//! ??
	double dna;

	//! ??
	double rna;

	//! ??
	double cofactor;

	//! ??
	int energy;

	//! ??
	bool locked; // bool?

	//! Name of biomass.
	string name;

	//! ??
	double cellwall;

	//! Universally unique identifier.
	string uuid;

	//! ??
	double protein;

	// List of compounds in biomass.
	vector<BiomassCompoundPtr> biomassCompoundList; // Should this be a map keyed by model compound uuid?

	//! Last modification date.
	string modDate;

	//! ??
	double lipid;

};

//! Smart pointer for Biomass class.
typedef std::tr1::shared_ptr<Biomass> BiomassPtr;

//! Formatted output operator for Biomass class.
std::ostream& operator<<(std::ostream& out, Biomass& obj);

typedef map<string,ModelCompartmentPtr> ModelCompartmentMap;
typedef pair<string,ModelCompartmentPtr> ModelCompartmentMapValue;
typedef map<string,ModelCompartmentPtr>::iterator ModelCompartmentMapIterator;

typedef map<string,ModelCompoundPtr> ModelCompoundMap;
typedef pair<string,ModelCompoundPtr> ModelCompoundMapValue;
typedef map<string,ModelCompoundPtr>::iterator ModelCompoundMapIterator;

//! Metabolic model domain data.

class MetabolicModel
{
public:

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

	//! Name of metabolic model.
	string name;

	//! ??
	string status;

	//! ??
	string type;

	//! Universally unique identifier.
	string uuid;

	//! Version number of metablic model.
	int version;

};

//! Formatted output operator for Metabolic model class.
std::ostream& operator<<(std::ostream& out, MetabolicModel& obj);

class Problem
{
public:

	Biochemistry biochem;

	MetabolicModel model;

};

typedef std::tr1::shared_ptr<Problem> ProblemPtr;

#endif // _MODELS_H
