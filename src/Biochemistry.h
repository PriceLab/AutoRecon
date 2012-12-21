#ifndef _BIOCHEMISTRY_H
#define _BIOCHEMISTRY_H

#include <string>
#include <tr1/memory>
#include <ostream>
#include <vector>
#include <map>
#include <set>
#include "Compartment.h"
#include "Compound.h"
#include "Reaction.h"
#include "AliasSet.h"
#include "Cue.h"
#include "Media.h"

using namespace std;

typedef map<string,CompartmentPtr> CompartmentMap;
typedef pair<string,CompartmentPtr> CompartmentMapValue;
typedef map<string,CompartmentPtr>::iterator CompartmentMapIterator;

typedef map<string,CompoundPtr> CompoundMap;
typedef pair<string,CompoundPtr> CompoundMapValue;
typedef map<string,CompoundPtr>::iterator CompoundMapIterator;

typedef map<string,ReactionPtr> ReactionMap;
typedef pair<string,ReactionPtr> ReactionMapValue;
typedef map<string,ReactionPtr>::iterator ReactionMapIterator;

typedef map<ReactionPtr, vector<ReactionPtr> > SynonymMap;
typedef pair<ReactionPtr, vector<ReactionPtr> > SynonymMapValue;
typedef map<ReactionPtr, vector<ReactionPtr> >::iterator SynonymMapIterator;

typedef vector<MediaPtr>::iterator MediaVectorIterator;
typedef vector<AliasSetPtr>::iterator AliasSetVectorIterator;

//! Biochemistry domain data.

class Biochemistry
{
public:

	//! \brief Default constructor.
	Biochemistry();

	//! \brief Load a biochemistry object from a file.
	//! \param[in] filename Path to file containing JSON representation.
	//! \return True when loaded successfully.

	bool loadFromFile(const char *filename);

	//! \brief Get the list of compounds that are identified as cofactors.
	//! \param[out] cofactorList List of compounds that are cofactors.
	//! \return Nothing.

	void getCofactorList(set<CompoundPtr>& cofactorList);

	//! \brief Find synonym reactions.
	//! \param[out] synonymList List of reactions and all of the reactions that are synonyms.
	// \return Nothing.

	void findSynonyms(SynonymMap& synonymList);

	//! \brief Find a media by name or uuid.
	//! \param[in] name Name or universally unique id of media.
	//! \param[out] media Pointer to media object if found.
	//! \return True when media is found.

	bool findMedia(const string name, MediaPtr& media);

	//! \brief Print compounds and reactions to files for flux balance analysis.
	//! \param[in] directory Path to directory to store files.
	//! \param[in] forcePrint When true print to the file even if it already exists.
	//! \return True when successful.

	bool printDBFiles(const string directory, bool forcePrint);

	AliasSetPtr queryAliasSet(string nameSpace, string type);

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

	//! Name of an AliasSet to use for reaction and compound ids.
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

//! Smart pointer for Biochemistry class.
typedef std::tr1::shared_ptr<Biochemistry> BiochemistryPtr;

//! Formatted output operator for Biochemistry class.
std::ostream& operator<<(std::ostream& out, Biochemistry& orig);

#endif
