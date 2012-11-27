#include "FileLoader.h"
#include "pathUtils.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <jsoncpp/json/json.h>

bool loadModelFile(const char* filename, MetabolicModel& model)
{
	// Parse the experiment json file.
	ifstream myfile;
	myfile.open(filename);
	Json::Value root;
	Json::Reader reader;
	if (reader.parse(myfile, root) == false)
	{
		cout << "Failed to parse file " << filename << "\n" << reader.getFormatedErrorMessages();
		return false;
	}
	myfile.close();

#if 1
	Json::Value::Members members = root.getMemberNames();
	for (Json::Value::Members::iterator iter = members.begin(); iter != members.end(); iter++)
	{
		cout << "root member " << *iter << endl;
	}
#endif

	// Load
	model.annotationUuid = root["annotation_uuid"].asString();
	model.biochemistryUuid = root["biochemistry_uuid"].asString();
	model.current = root["current"].asInt();
	model.defaultNameSpace = root["defaultNameSpace"].asString();
	model.growth = root["growth"].asInt();
	model.id = root["id"].asString();
	model.mappingUuid = root["mapping_uuid"].asString();
	model.modDate = root["modDate"].asString();
	model.name = root["name"].asString();
	model.status = root["status"].asString();
	model.type = root["type"].asString();
	model.uuid = root["uuid"].asString();
	model.version = root["__VERSION__"].asInt();
	cout << "Parsing model file " << filename << " with version " << root["version"].asInt() << endl;

	Json::Value biomasses = root["biomasses"];
	for (Json::ValueIterator iter = biomasses.begin(); iter!= biomasses.end(); iter++)
	{
		BiomassPtr biomass(new Biomass(*iter));
		model.biomassList.push_back(biomass);
	}
	cout << "Added " << model.biomassList.size() << " biomasses" << endl;
	cout << "First biomass is " << endl << *model.biomassList[0] << endl;

	Json::Value modelCompounds = root["modelcompounds"];
	for (Json::ValueIterator iter = modelCompounds.begin(); iter!= modelCompounds.end(); iter++)
	{
		ModelCompoundPtr modelCompound(new ModelCompound(*iter));
		model.modelCompoundList.insert(ModelCompoundMapValue(modelCompound->uuid, modelCompound));
	}
	cout << "Added " << model.modelCompoundList.size() << " model compounds" << endl;
	cout << "First model compound is " << endl << *model.modelCompoundList.begin()->second << endl;

	Json::Value modelCompartments = root["modelcompartments"];
	for (Json::ValueIterator iter = modelCompartments.begin(); iter!= modelCompartments.end(); iter++)
	{
		ModelCompartmentPtr modelCompartment(new ModelCompartment(*iter));
		model.modelCompartmentList.insert(ModelCompartmentMapValue(modelCompartment->uuid, modelCompartment));
	}
	cout << "Added " << model.modelCompartmentList.size() << " model compartments" << endl;
	cout << "First model compartment is " << endl << *model.modelCompartmentList.begin()->second << endl;

	Json::Value modelReactions = root["modelreactions"];
	for (Json::ValueIterator iter = modelReactions.begin(); iter!= modelReactions.end(); iter++)
	{
		ModelReactionPtr modelReaction(new ModelReaction(*iter));
		model.modelReactionList.push_back(modelReaction);
	}
	cout << "Added " << model.modelReactionList.size() << " model reactions" << endl;
	cout << "First model reaction is " << endl << *model.modelReactionList[0] << endl;

	cout << endl << "Model is " << endl << model << endl;
	return true;
}

bool loadBiochemistryFile(const char* filename, Biochemistry& biochem)
{
	// Parse the biochemistry json file.
	ifstream myfile;
	myfile.open(filename);
	Json::Value root;
	Json::Reader reader;
	if (reader.parse(myfile, root) == false)
	{
		cout << "Failed to parse file " << filename << "\n" << reader.getFormatedErrorMessages();
		return false;
	}
	myfile.close();

	// Load
#if 1
	Json::Value::Members members = root.getMemberNames();
	for (Json::Value::Members::iterator iter = members.begin(); iter != members.end(); iter++)
	{
		cout << "root member " << *iter << endl;
	}
#endif
	biochem.version = root["__VERSION__"].asInt();
	biochem.biochemistryStructures_uuid = root["biochemistryStructures_uuid"].asString();
	biochem.defaultNameSpace = root["defaultNameSpace"].asString();
	biochem.modDate = root["modDate"].asString();
	biochem.name = root["name"].asString();
	biochem.uuid = root["uuid"].asString();
	cout << biochem << endl;
//	cout << "Parsing biochemistry file " << filename << " with version " << root["version"].asInt() << endl;

	Json::Value compartments = root["compartments"];
	for (Json::ValueIterator iter = compartments.begin(); iter != compartments.end(); iter++)
	{
		CompartmentPtr compartment(new Compartment(*iter));
		biochem.compartmentList.insert(CompartmentMapValue(compartment->uuid, compartment));
	}
	cout << "Added " << biochem.compartmentList.size() << " compartments" << endl;
	cout << "First compartment is " << endl << *biochem.compartmentList.begin()->second << endl;

	Json::Value compounds = root["compounds"];
	for (Json::ValueIterator iter = compounds.begin(); iter != compounds.end(); iter++)
	{
		CompoundPtr compound(new Compound(*iter));
		biochem.compoundList.insert(CompoundMapValue(compound->uuid, compound));
	}
	cout << "Added " << biochem.compoundList.size() << " compounds" << endl;
	cout << "First compound is " << endl << *biochem.compoundList.begin()->second << endl;
	map<string, vector<double> >::iterator it = biochem.compoundList.begin()->second->pkaList.begin();
	cout << "pka " << it->second[0] << endl;
	it = biochem.compoundList.begin()->second->pkbList.begin();
	cout << "pkb " << it->second[0] << endl;

	Json::Value cues = root["cues"];
	for (Json::ValueIterator iter = cues.begin(); iter != cues.end(); iter++)
	{
		CuePtr cue(new Cue(*iter));
		biochem.cueList.push_back(cue);
	}
	cout << "Added " << biochem.cueList.size() << " cues" << endl;
	cout << "First cue is " << endl << *biochem.cueList[0] << endl;

	Json::Value medias = root["media"];
	for (Json::ValueIterator iter = medias.begin(); iter != medias.end(); iter++)
	{
		MediaPtr media(new Media(*iter));
		biochem.mediaList.push_back(media);
	}
	cout << "Added " << biochem.mediaList.size() << " media" << endl;
	cout << "First media is" << endl << *biochem.mediaList[0] << endl;
	cout << "First media compound is " << endl << *(biochem.mediaList[0]->mediaCompoundList[0]) << endl;

	Json::Value reactions = root["reactions"];
	for (Json::ValueIterator iter = reactions.begin(); iter != reactions.end(); iter++)
	{
		ReactionPtr reaction(new Reaction(*iter));
		biochem.reactionList.insert(ReactionMapValue(reaction->uuid, reaction));
	}
	cout << "Added " << biochem.reactionList.size() << " reactions" << endl;
	cout << "First reaction is " << endl << *biochem.reactionList.begin()->second << endl;
	cout << "First reagent is " << endl << *(biochem.reactionList.begin()->second->reagentList[0]) << endl;

	Json::Value aliasSets = root["aliasSets"];
	for (Json::ValueIterator iter = aliasSets.begin(); iter != aliasSets.end(); iter++)
	{
		AliasSetPtr aliasSet(new AliasSet(*iter));
		biochem.aliasSetList.push_back(aliasSet);
	}
	cout << "Added " << biochem.aliasSetList.size() << " alias sets" << endl;
	cout << "First alias set is " << endl << *biochem.aliasSetList[0] << endl;

	cout << endl << "Biochemistry is" << endl << biochem << endl;
	return true;
}

bool validateProblem(Problem& problem)
{
	MetabolicModel& model  = problem.model;
	Biochemistry& biochem = problem.biochem;

	// Make sure model points at a valid biochemistry.
	if (model.biochemistryUuid != biochem.uuid)
	{
		cout << "Experiment biochemistry uuid " << model.biochemistryUuid << " does not match " << biochem.uuid << endl;
		return false;
	}

	// Make sure each biochemistry media compound points to a valid compound.
	for (int index = 0; index < biochem.mediaList.size(); index++)
	{
		for (int cindex = 0; cindex < biochem.mediaList[index]->mediaCompoundList.size(); cindex++)
		{
			MediaCompoundPtr mediaCompound = biochem.mediaList[index]->mediaCompoundList[cindex];
			CompoundMapIterator found = biochem.compoundList.find(mediaCompound->compoundUuid);
			if (found == biochem.compoundList.end())
			{
				cout << mediaCompound->compoundUuid << " compound UUID was not found for media compound " <<
						cindex << " in media " << biochem.mediaList[index]->uuid << endl;
				return false;
			}
			mediaCompound->compound = found->second;
		}
	}
	cout << "validated media compounds in " << biochem.mediaList.size() << " biochemistry media" << endl;

	// Make sure each biochemistry reagent points to a valid compound and compartment.
	for (ReactionMapIterator iter = biochem.reactionList.begin(); iter != biochem.reactionList.end(); iter++)
	{
		for (int index = 0; index < iter->second->reagentList.size(); index++)
		{
			ReagentPtr reagent = iter->second->reagentList[index];
			CompoundMapIterator foundCompound = biochem.compoundList.find(reagent->compoundUuid);
			if (foundCompound == biochem.compoundList.end())
			{
				cout << reagent->compoundUuid << " compound UUID was not found for reagent " << index <<
						"in reaction " << iter->second->uuid << endl;
			}
			reagent->compound = foundCompound->second;

			CompartmentMapIterator foundCompartment = biochem.compartmentList.find(reagent->compartmentUuid);
			if (foundCompartment == biochem.compartmentList.end())
			{
				cout << reagent->compartmentUuid << " compartment UUID was not found for reagent " << index <<
						"in reaction " << iter->second->uuid << endl;
			}
			reagent->compartment = foundCompartment->second;
		}
	}
	cout << "validated reagents in " << biochem.reactionList.size() << " biochemistry reactions" << endl;

	// Make sure each model compartment points to a valid compartment.
	for (ModelCompartmentMapIterator iter = model.modelCompartmentList.begin(); iter != model.modelCompartmentList.end(); iter++)
	{
		CompartmentMapIterator found = biochem.compartmentList.find(iter->second->compartmentUuid);
		if (found == biochem.compartmentList.end())
		{
			cout << iter->second->compartmentUuid << " compartment UUID was not found for model compartment " <<
					iter->second->uuid << endl;
			return false;
		}
		iter->second->compartment = found->second;
	}
	cout << "validated " << model.modelCompartmentList.size() << " model compartments" << endl;

	// Make sure each model reaction points to a valid reaction and model compartment and also that model reagents point
	// to valid model compounds.
	for (int index = 0; index < model.modelReactionList.size(); index++)
	{
		ModelReactionPtr modelReaction = model.modelReactionList[index];
		ReactionMapIterator foundReaction = biochem.reactionList.find(modelReaction->reactionUuid);
		if (foundReaction == biochem.reactionList.end())
		{
			cout << modelReaction->reactionUuid << " reaction UUID was not found for model reaction " <<
					modelReaction->uuid << endl;
			return false;
		}
		modelReaction->reaction = foundReaction->second;

		ModelCompartmentMapIterator foundCompartment = model.modelCompartmentList.find(modelReaction->modelCompartmentUuid);
		if (foundCompartment == model.modelCompartmentList.end())
		{
			cout << modelReaction->modelCompartmentUuid << " model compartment UUID was not found for model reaction " <<
					modelReaction->uuid << endl;
			return false;
		}
		modelReaction->modelCompartment = foundCompartment->second;

		for (int rindex = 0; rindex < modelReaction->modelReactionReagentList.size(); rindex++)
		{
			ModelReactionReagentPtr reagent = modelReaction->modelReactionReagentList[rindex];
			ModelCompoundMapIterator foundCompound = model.modelCompoundList.find(reagent->modelCompoundUuid);
			if (foundCompound == model.modelCompoundList.end())
			{
				cout << reagent->modelCompoundUuid << " model compound UUID was not found for model reagent " << rindex <<
						" in model reaction " << modelReaction->uuid << endl;
				return false;
			}
			reagent->modelCompound = foundCompound->second;
		}
	}
	cout << "validated " << model.modelReactionList.size() << " model reactions" << endl;

	// Make sure each model compound points to a valid compound and points to a valid model compartment.
	for (ModelCompoundMapIterator iter = model.modelCompoundList.begin(); iter != model.modelCompoundList.end(); iter++)
	{
		ModelCompoundPtr modelCompound = iter->second;
		CompoundMapIterator foundCompound = biochem.compoundList.find(modelCompound->compoundUuid);
		if (foundCompound == biochem.compoundList.end())
		{
			cout << modelCompound->compoundUuid << " compound UUID was not found for model compound " << modelCompound->uuid << endl;
			return false;
		}
		modelCompound->compound = foundCompound->second;

		ModelCompartmentMapIterator foundCompartment = model.modelCompartmentList.find(modelCompound->modelCompartmentUuid);
		if (foundCompartment == model.modelCompartmentList.end())
		{
			cout << modelCompound->modelCompartmentUuid << " model compartment UUID was not found for model compound " << modelCompound->uuid << endl;
			return false;
		}
		modelCompound->modelCompartment = foundCompartment->second;
	}
	cout << "validated " << model.modelCompoundList.size() << " model compounds" << endl;

	// Make sure each biomass compound points to a valid model compound (and by extension to a valid compound).
	for (int bindex = 0; bindex < model.biomassList.size(); bindex++)
	{
		for (int cindex = 0; cindex < model.biomassList[bindex]->biomassCompoundList.size(); cindex++)
		{
			BiomassCompoundPtr biomassCompound = model.biomassList[bindex]->biomassCompoundList[cindex];
			ModelCompoundMapIterator found = model.modelCompoundList.find(biomassCompound->modelCompoundUuid);
			if (found == model.modelCompoundList.end())
			{
				cout << biomassCompound->modelCompoundUuid << " model compound UUID was not found for biomass compound " <<
						cindex << " in biomass " << model.biomassList[bindex]->uuid << endl;
				return false;
			}
			biomassCompound->modelCompound = found->second;
		}
	}
	cout << "validated " << model.biomassList.size() << " biomasses" << endl;

	bool external = isExternalCompound(model.modelCompoundList.begin()->second, problem);

	// Find transporter compounds (defined as a compound in more than one compartment).
	for (ModelCompoundMapIterator iter = model.modelCompoundList.begin(); iter != model.modelCompoundList.end(); iter++)
	{
		// Run the list looking for the same compound in a different compartment.  If so, the model compound is a transporter.
		if (iter->second->isTransporter == false)
		{
			for (ModelCompoundMapIterator checkIter = model.modelCompoundList.begin(); checkIter != model.modelCompoundList.end(); checkIter++)
			{
				if ((checkIter->second->compoundUuid == iter->second->compoundUuid) &&
					(checkIter->second->modelCompartmentUuid != iter->second->modelCompartmentUuid))
				{
					cout << "Model compound " << iter->second->compound->name << " is a transporter between compartments " <<
							iter->second->modelCompartment->compartment->name << " and " << checkIter->second->modelCompartment->compartment->name << endl;
					iter->second->isTransporter = checkIter->second->isTransporter = true;
					iter->second->transporterCompound = checkIter->second;
					checkIter->second->transporterCompound = iter->second;
					if (iter->second->formula != iter->second->compound->formula || iter->second->charge != iter->second->compound->defaultCharge)
					{
						cout << "  model formula " << iter->second->formula << " with charge " << iter->second->charge <<
								" compared to formula at pH7 " << iter->second->compound->formula << " with default charge " <<
								iter->second->compound->defaultCharge << endl;
					}
				}
			}
		}
	}

	return true;
}
