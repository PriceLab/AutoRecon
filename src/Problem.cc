#include "Problem.h"
#include "Utility.h"

typedef pair<CompoundPtr,CompartmentPtr> BiomassTarget;

Problem::Problem()
{
	biochem = BiochemistryPtr(new Biochemistry());
	model = MetabolicModelPtr(new MetabolicModel());
}

bool Problem::validate(void)
{
	cout << "Validating problem ... " << endl;
	StopWatch timer;

	// Make sure model points at a valid biochemistry.
	if (model->biochemistryUuid != biochem->uuid)
	{
		cout << "  FAILED Experiment biochemistry uuid " << model->biochemistryUuid << " does not match " << biochem->uuid << endl;
		return false;
	}

	// Make sure each biochemistry media compound points to a valid compound.
	for (int index = 0; index < biochem->mediaList.size(); index++)
	{
		for (int cindex = 0; cindex < biochem->mediaList[index]->mediaCompoundList.size(); cindex++)
		{
			MediaCompoundPtr mediaCompound = biochem->mediaList[index]->mediaCompoundList[cindex];
			CompoundMapIterator found = biochem->compoundList.find(mediaCompound->compoundUuid);
			if (found == biochem->compoundList.end())
			{
				cout << "  FAILED " << mediaCompound->compoundUuid << " compound UUID was not found for media compound " <<
						cindex << " in media " << biochem->mediaList[index]->uuid << endl;
				return false;
			}
			mediaCompound->compound = found->second;
		}
	}
	cout << "  Validated media compounds in " << biochem->mediaList.size() << " biochemistry media" << endl;

	// Make sure each biochemistry reagent points to a valid compound and compartment.
	for (ReactionMapIterator iter = biochem->reactionList.begin(); iter != biochem->reactionList.end(); iter++)
	{
		for (int index = 0; index < iter->second->reagentList.size(); index++)
		{
			ReagentPtr reagent = iter->second->reagentList[index];
			CompoundMapIterator foundCompound = biochem->compoundList.find(reagent->compoundUuid);
			if (foundCompound == biochem->compoundList.end())
			{
				cout << " FAILED " << reagent->compoundUuid << " compound UUID was not found for reagent " << index <<
						"in reaction " << iter->second->uuid << endl;
			}
			reagent->compound = foundCompound->second;

			CompartmentMapIterator foundCompartment = biochem->compartmentList.find(reagent->compartmentUuid);
			if (foundCompartment == biochem->compartmentList.end())
			{
				cout << "  FAILED " << reagent->compartmentUuid << " compartment UUID was not found for reagent " << index <<
						"in reaction " << iter->second->uuid << endl;
			}
			reagent->compartment = foundCompartment->second;
		}
	}
	cout << "  Validated reagents in " << biochem->reactionList.size() << " biochemistry reactions" << endl;

	// Make sure each model compartment points to a valid compartment.
	for (ModelCompartmentMapIterator iter = model->modelCompartmentList.begin(); iter != model->modelCompartmentList.end(); iter++)
	{
		CompartmentMapIterator found = biochem->compartmentList.find(iter->second->compartmentUuid);
		if (found == biochem->compartmentList.end())
		{
			cout << "  FAILED " << iter->second->compartmentUuid << " compartment UUID was not found for model compartment " <<
					iter->second->uuid << endl;
			return false;
		}
		iter->second->compartment = found->second;
	}
	cout << "  Validated " << model->modelCompartmentList.size() << " model compartments" << endl;

	// Make sure each model reaction points to a valid reaction and model compartment and also that model reagents point
	// to valid model compounds.
	for (int index = 0; index < model->modelReactionList.size(); index++)
	{
		ModelReactionPtr modelReaction = model->modelReactionList[index];
		ReactionMapIterator foundReaction = biochem->reactionList.find(modelReaction->reactionUuid);
		if (foundReaction == biochem->reactionList.end())
		{
			cout << "  FAILED " << modelReaction->reactionUuid << " reaction UUID was not found for model reaction " <<
					modelReaction->uuid << endl;
			return false;
		}
		modelReaction->reaction = foundReaction->second;

		ModelCompartmentMapIterator foundCompartment = model->modelCompartmentList.find(modelReaction->modelCompartmentUuid);
		if (foundCompartment == model->modelCompartmentList.end())
		{
			cout << "  FAILED " << modelReaction->modelCompartmentUuid << " model compartment UUID was not found for model reaction " <<
					modelReaction->uuid << endl;
			return false;
		}
		modelReaction->modelCompartment = foundCompartment->second;

		for (int rindex = 0; rindex < modelReaction->modelReactionReagentList.size(); rindex++)
		{
			ModelReactionReagentPtr reagent = modelReaction->modelReactionReagentList[rindex];
			ModelCompoundMapIterator foundCompound = model->modelCompoundList.find(reagent->modelCompoundUuid);
			if (foundCompound == model->modelCompoundList.end())
			{
				cout << "  FAILED " << reagent->modelCompoundUuid << " model compound UUID was not found for model reagent " << rindex <<
						" in model reaction " << modelReaction->uuid << endl;
				return false;
			}
			reagent->modelCompound = foundCompound->second;
		}
	}
	cout << "  Validated " << model->modelReactionList.size() << " model reactions" << endl;

	// Make sure each model compound points to a valid compound and points to a valid model compartment.
	for (ModelCompoundMapIterator iter = model->modelCompoundList.begin(); iter != model->modelCompoundList.end(); iter++)
	{
		ModelCompoundPtr modelCompound = iter->second;
		CompoundMapIterator foundCompound = biochem->compoundList.find(modelCompound->compoundUuid);
		if (foundCompound == biochem->compoundList.end())
		{
			cout << "  FAILED " << modelCompound->compoundUuid << " compound UUID was not found for model compound " << modelCompound->uuid << endl;
			return false;
		}
		modelCompound->compound = foundCompound->second;

		ModelCompartmentMapIterator foundCompartment = model->modelCompartmentList.find(modelCompound->modelCompartmentUuid);
		if (foundCompartment == model->modelCompartmentList.end())
		{
			cout << "  FAILED " << modelCompound->modelCompartmentUuid << " model compartment UUID was not found for model compound " << modelCompound->uuid << endl;
			return false;
		}
		modelCompound->modelCompartment = foundCompartment->second;
	}
	cout << "  Validated " << model->modelCompoundList.size() << " model compounds" << endl;

	// Make sure each biomass compound points to a valid model compound (and by extension to a valid compound).
	for (int bindex = 0; bindex < model->biomassList.size(); bindex++)
	{
		for (int cindex = 0; cindex < model->biomassList[bindex]->biomassCompoundList.size(); cindex++)
		{
			BiomassCompoundPtr biomassCompound = model->biomassList[bindex]->biomassCompoundList[cindex];
			ModelCompoundMapIterator found = model->modelCompoundList.find(biomassCompound->modelCompoundUuid);
			if (found == model->modelCompoundList.end())
			{
				cout << "  FAILED " << biomassCompound->modelCompoundUuid << " model compound UUID was not found for biomass compound " <<
						cindex << " in biomass " << model->biomassList[bindex]->uuid << endl;
				return false;
			}
			biomassCompound->modelCompound = found->second;
		}
	}
	cout << "  Validated " << model->biomassList.size() << " biomasses" << endl;

	// Find transporter compounds (defined as a compound in more than one compartment).
	for (ModelCompoundMapIterator iter = model->modelCompoundList.begin(); iter != model->modelCompoundList.end(); iter++)
	{
		// Run the list looking for the same compound in a different compartment.  If so, the model compound is a transporter.
		if (iter->second->isTransporter == false)
		{
			for (ModelCompoundMapIterator checkIter = model->modelCompoundList.begin(); checkIter != model->modelCompoundList.end(); checkIter++)
			{
				if ((checkIter->second->compoundUuid == iter->second->compoundUuid) &&
					(checkIter->second->modelCompartmentUuid != iter->second->modelCompartmentUuid))
				{
//					cout << "Model compound " << iter->second->compound->name << " is a transporter between compartments " <<
//							iter->second->modelCompartment->compartment->name << " and " << checkIter->second->modelCompartment->compartment->name << endl;
					iter->second->isTransporter = checkIter->second->isTransporter = true;
					iter->second->transporterCompound = checkIter->second;
					checkIter->second->transporterCompound = iter->second;
					if (iter->second->formula != iter->second->compound->formula || iter->second->charge != iter->second->compound->defaultCharge)
					{
//						cout << "  model formula " << iter->second->formula << " with charge " << iter->second->charge <<
//								" compared to formula at pH7 " << iter->second->compound->formula << " with default charge " <<
//								iter->second->compound->defaultCharge << endl;
					}
				}
			}
		}
	}

	cout << "  done (" << timer.stop() << " seconds)" << endl;
	return true;
}

bool Problem::isExternalCompound(ModelCompoundPtr compound)
{
  // Model compound points to model compartment ...
  ModelCompartmentMapIterator miter = model->modelCompartmentList.find(compound->modelCompartmentUuid);

  // ... which points to compartment ...
  CompartmentMapIterator citer = biochem->compartmentList.find(miter->second->compartmentUuid);

  // ... which has a hierarchy and hierarchy 0 is external.
  if (citer->second->hierarchy == 0) return true;
  return false;
}

MetabolicModelPtr Problem::findPaths(SynonymMap synonymList, MediaPtr media, int numSolutions)
{
	//! In this function we use the MFA toolkit to construct a maximum-likelihood
	//! FBA problem from the biochemsitry within the input Problem structure
	//! and solve it.
	//!
	//! biomassComponent is a compound for which we want to require production and
	//! biomassCompartment is the compartment of that compound.

	// Note - The following things need to be done here.
	//
	// 1: Turn the biomassPtr into a list of target metabolites.
	// 2: Process probabilities from the JSON file into costs.
	// 3: Hook up the problem to the MFA toolkit. Processing to do in translation:
	//       Add exchange reactions for everything in the provided media
	//       Add irreversible (+ only) exchanges for everything (anything that needs to leave can).
	//             Give them a non-zero probability so that the model is penalized somewhat for using them with a high flux.
	//       (Optionally?) don't include unbalanced reactions.
	// 4: Use the MFA toolkit to run K iterations of the path-finding algorithm [this should be in a separate function, called in a loop]
	//     to identify compounds and reactions to add to the model file.
	//   - IMPORTANT: After the first iteration the synonymList is used to exclude any synonyms of reactions already found
	//    in subsequent iterations (except for the reaction itself).
	//      - It will be worth-while to keep track of the rates we get from our FBA problem as well as whether or not they are 0.
	//        The rates can be used to identify preferred directions and minimize the chances of pathological answers like infinite
	//        ATP loops
	// 5: Expand these back out into a new ModelPtr to spit out as a model JSON file.

	//! Use the biomass objective identified in the model JSON object
	//! passed into this function.
	//! This ensures that we are using the same target as the ModelSEED
	//! and thus that we can directly compare the results of our algorithms.

	//! Step 1 - compute a list of target metabolite\compartment pairs from the biomass equations.
	vector<BiomassTarget> targetList;

	// There can be multiple biomass equations...
	for (BiomassVectorIterator bm = model->biomassList.begin(); bm != model->biomassList.end(); bm++)
	{
		BiomassPtr biomass = (*bm);

		// ... Each of which has a list of compounds in it
		for ( vector<BiomassCompoundPtr>::iterator bmc = biomass->biomassCompoundList.begin(); bmc != biomass->biomassCompoundList.end(); bmc++)
		{
			// Here is the compound (translating from model compound to biochemistry compound).
			ModelCompoundPtr modelCompound  = model->modelCompoundList[(*bmc)->modelCompoundUuid];
			CompoundPtr compound = modelCompound->compound;

			// And here is its compartment (translating from model compartment to biochemistry compartment.
			ModelCompartmentPtr modelCompartment = model->modelCompartmentList[modelCompound->modelCompartmentUuid];
			CompartmentPtr compartment = modelCompartment->compartment;

			targetList.push_back(BiomassTarget(compound,compartment));
		}
	}

	//! Step 2 - Convert probabilities into costs
	//! Lets talk about this next week.

	//! Step 3 - Hook up to MFA toolkit

	//! Step 4 - Find K solutions (K isn't a parameter yet but it will be soon)

	//! Step 5 - Expand into a model object. Maybe the seed people already have
	// some better ideas on how to iterate over and find optimal fit to experiment.

	MetabolicModelPtr newModel = MetabolicModelPtr(new MetabolicModel());

	return newModel;

}



