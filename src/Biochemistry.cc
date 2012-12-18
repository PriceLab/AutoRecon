#include "Biochemistry.h"
#include "Utility.h"
#include <fstream>
#include <omp.h>

typedef map<ReactionPtr, vector<CompoundPtr> > NetReactionMap;
typedef pair<ReactionPtr, vector<CompoundPtr> > NetReactionMapValue;
typedef map<ReactionPtr, vector<CompoundPtr> >::iterator NetReactionMapIterator;


Biochemistry::Biochemistry()
{
	version = 0;
}

bool Biochemistry::loadFromFile(const char* filename)
{
	StopWatch timer;

	// Parse the biochemistry json file.
	cout << "Loading biochemistry from '" << filename << "' ... " << endl;
	ifstream myfile;
	myfile.open(filename);
	if (myfile.fail())
	{
		cout << "  FAILED Error opening biochemistry file" << endl;
		return false;
	}
	Json::Value root;
	Json::Reader reader;
	if (reader.parse(myfile, root) == false)
	{
		cout << "  FAILED Error parsing biochemistry file" << endl << reader.getFormatedErrorMessages();
		myfile.close();
		return false;
	}
	myfile.close();

	// Load root members.
#if 0
	Json::Value::Members members = root.getMemberNames();
	for (Json::Value::Members::iterator iter = members.begin(); iter != members.end(); iter++)
	{
		cout << "root member " << *iter << endl;
	}
#endif
	version = root["__VERSION__"].asInt();
	biochemistryStructures_uuid = root["biochemistryStructures_uuid"].asString();
	defaultNameSpace = root["defaultNameSpace"].asString();
	modDate = root["modDate"].asString();
	name = root["name"].asString();
	uuid = root["uuid"].asString();

	Json::Value compartments = root["compartments"];
	for (Json::ValueIterator iter = compartments.begin(); iter != compartments.end(); iter++)
	{
		CompartmentPtr compartment(new Compartment(*iter));
		compartmentList.insert(CompartmentMapValue(compartment->uuid, compartment));
	}
#if 0
	cout << "Added " << compartmentList.size() << " compartments" << endl;
	cout << "First compartment is " << endl << *compartmentList.begin()->second << endl;
#endif

	Json::Value compounds = root["compounds"];
	for (Json::ValueIterator iter = compounds.begin(); iter != compounds.end(); iter++)
	{
		CompoundPtr compound(new Compound(*iter));
		compoundList.insert(CompoundMapValue(compound->uuid, compound));
	}
#if 0
	cout << "Added " << compoundList.size() << " compounds" << endl;
	cout << "First compound is " << endl << *compoundList.begin()->second << endl;
	map<string, vector<double> >::iterator it = compoundList.begin()->second->pkaList.begin();
	cout << "pka " << it->second[0] << endl;
	it = compoundList.begin()->second->pkbList.begin();
	cout << "pkb " << it->second[0] << endl;
#endif

	Json::Value cues = root["cues"];
	for (Json::ValueIterator iter = cues.begin(); iter != cues.end(); iter++)
	{
		CuePtr cue(new Cue(*iter));
		cueList.push_back(cue);
	}
#if 0
	cout << "Added " << cueList.size() << " cues" << endl;
	cout << "First cue is " << endl << *cueList[0] << endl;
#endif

	Json::Value medias = root["media"];
	for (Json::ValueIterator iter = medias.begin(); iter != medias.end(); iter++)
	{
		MediaPtr media(new Media(*iter));
		mediaList.push_back(media);
	}
#if 0
	cout << "Added " << mediaList.size() << " media" << endl;
	cout << "First media is" << endl << *mediaList[0] << endl;
	cout << "First media compound is " << endl << *(mediaList[0]->mediaCompoundList[0]) << endl;
#endif

	Json::Value reactions = root["reactions"];
	for (Json::ValueIterator iter = reactions.begin(); iter != reactions.end(); iter++)
	{
		ReactionPtr reaction(new Reaction(*iter));
		reactionList.insert(ReactionMapValue(reaction->uuid, reaction));
	}
#if 0
	cout << "Added " << reactionList.size() << " reactions" << endl;
	cout << "First reaction is " << endl << *reactionList.begin()->second << endl;
	cout << "First reagent is " << endl << *(reactionList.begin()->second->reagentList[0]) << endl;
#endif

	Json::Value aliasSets = root["aliasSets"];
	for (Json::ValueIterator iter = aliasSets.begin(); iter != aliasSets.end(); iter++)
	{
		AliasSetPtr aliasSet(new AliasSet(*iter));
		aliasSetList.push_back(aliasSet);
	}
#if 0
	cout << "Added " << aliasSetList.size() << " alias sets" << endl;
	cout << "First alias set is " << endl << *aliasSetList[0] << endl;
#endif

	cout << "  done (" << timer.stop() << " seconds)"<< endl;
	return true;
}

void Biochemistry::getCofactorList(set<CompoundPtr>& cofactorList)
{
	cout << "Getting cofactor list from biochemistry ..." << endl;
	StopWatch timer;

	// Check each compound and if it is a cofactor then add it to the list.
	// \todo This is a candidate for parallelization but serial version is fast if no cofactors.
	for (CompoundMapIterator iter = compoundList.begin(); iter != compoundList.end(); iter++)
	{
		if (iter->second->isCofactor)
		{
			cofactorList.insert(iter->second);
		}
	}
	cout << "  Found " << cofactorList.size() << " compounds that are cofactors" << endl;
	cout << "  done (" << timer.stop() << " seconds)" << endl;
	return;

	// begin_block_range, end_block_range.
}

void Biochemistry::findSynonyms(SynonymMap& synonymList)
{
	//! Iterate over the biochemistry object,
	//! to identify sets of reactions with identical compounds except
	//! for those that are identified as cofactors either in the compound
	//! list or in the reaction itself.
	//!
	//! It is assumed that a compound that is declared as a cofactor
	//! is a cofactor regardless of what compartment it is found in.
	//!
	//! Returns a mapping from each reaction's pointer to a list of that reaction's
	//! synonyms (including itself)

	// Get the list of cofactor compounds.
	set<CompoundPtr> cofactorList;
	getCofactorList(cofactorList);

	cout << "Finding synonym reactions in biochemistry ..." << endl;
	StopWatch timer;

	// Mapping from each reaction to a list of non-cofactor compounds found within it.
	NetReactionMap netReactions;

	//! Iterate over reactions and identify their net reaction.
	//! NOTE - This would be more efficient though if we make a map<vector<CompoundPtr>, vector<ReactionPtr> >
	//! - this is only possible if the map data structure can correctly tell whether two vector<CompountPtr>
	//! are the same or not.
	//
	// for each ReactionPtr
	//    make a vector of CompoundPtr
	//    for each metabolite in that reaction
	//       if that metabolite is labled as a cofactor within that reaction:
	//          continue
	//       if that metabolite is labeled as a cofactor within the cofactor list:
	//          continue
	//       Add the metabolite to the vector of CompoundPtr
	//    Add cofactorList to netReactions with ReactionPtr as a pointer.

	// Iterate over reactions and identify their net reaction.
	for (ReactionMapIterator iter = reactionList.begin(); iter != reactionList.end(); iter++)
	{
		vector<CompoundPtr> nonCofactorCompounds;
		ReactionPtr reaction = iter->second;
		for (int index = 0; index < reaction->reagentList.size(); index++)
		{
			// Skip if compound is a cofactor within reaction.
			if (reaction->reagentList[index]->isCofactor) continue;

			// Skip if compound is a cofactor within cofactor list.
			CompoundPtr compound = reaction->reagentList[index]->compound;
			if (cofactorList.find(compound) != cofactorList.end()) continue;

			// Add the compound to the list for this reaction.
			nonCofactorCompounds.push_back(compound);
		}
		netReactions.insert(NetReactionMapValue(reaction, nonCofactorCompounds));
	}
	cout << "  Added " << netReactions.size() << " reactions to net reactions map" << endl;

	//! Identify lists of synonymous reactions.
	//! This part would be trivial if we can use the alternate implementation described above.
	//! As it is in this sketch it would be O(N^2)
	// For each ReactionPtr:
	//   Make a vector<ReactionPtr>
	//   For each ReactionPtr:
	//    If the vector<CompoundPtr> is the same as the reference:
	//      add the query reaction to the vector<ReactionPtr>
	//   Add ReactionPtr-->vector<ReactionPtr> to synList

	// Iterate over the reactions and identify the synonym reactions.
	//! \todo This is a candidate for parallelization.
	#pragma omp parallel shared(synonymList)
	{
		int threadId = omp_get_thread_num();
		int numThreads = omp_get_num_threads();
//		ReactionMapIterator beginIter = begin_block_range();
		for (ReactionMapIterator iter = reactionList.begin(); iter != reactionList.end(); iter++)
		{
			ReactionPtr reaction = iter->second;

			// Find this reaction in the net reactions list.
			NetReactionMapIterator found = netReactions.find(reaction);
			if (found == netReactions.end()) continue;

			// Check each net reaction and see if the compound list matches this reaction.  The synonyms
			// will at least include this reaction.
			vector<ReactionPtr> synonyms;
			for (NetReactionMapIterator nrIter = netReactions.begin(); nrIter != netReactions.end(); nrIter++)
			{
				if (found->second == nrIter->second) // This compares the vectors
				{
					synonyms.push_back(nrIter->first);
				}
			}
			#pragma omp critical
			synonymList.insert(SynonymMapValue(reaction,synonyms)); // This needs sync
		}

	}
	cout << "  Added " << synonymList.size() << " reactions to synonym reaction list" << endl;

	cout << "  done (" << timer.stop() << " seconds)" << endl;
	return;
}

bool Biochemistry::findMedia(const string value, MediaPtr& media)
{
	for (MediaVectorIterator iter = mediaList.begin(); iter != mediaList.end(); iter++)
	{
		media = *iter;
		if (media->name == value) return true;
		if (media->uuid == value) return true;
	}

	return false;
}

std::ostream& operator<<(std::ostream& out, Biochemistry& obj)
{
	out << "-Biochemistry-" << endl;
	out << "name: " << obj.name << "; uuid: " << obj.uuid << "; modDate: " << obj.modDate;
	out << "; version: " << obj.version << "; defaultNameSpace: " << obj.defaultNameSpace;
	out << "; biochemistryStructures_uuid: " << obj.biochemistryStructures_uuid;
	return out;
}

