#include "pathUtils.h"
#include "Models.h"

#include <map>
#include <vector>
#include <set>

using std::vector;
using std::map;

MetabolicModel findPaths(Problem &problem, map<ReactionPtr, vector<ReactionPtr> > synonymList, MediaPtr media, vector<BiomassPtr> biomasses ) {
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

  //! Step 1 - compute a list of target metabolite\compartment pairs from the biomass equations.
  vector<BiomassPtr> biomassList = problem.model->biomassList;

  // These should be filled up such that the index lines up.
  // There has to be a better way to store pairs of objects so that they have
  // to be unique together...
  //
  // Maybe we should just define an object with a compound and a compartment UUID
  // and stick with that to make life much easier down the road. Thoughts?
  vector<CompoundPtr> biomassTargetList;
  vector<CompartmentPtr> biomassCompartmentList;

  // There can be multiple biomass equations...
  for (vector<BiomassPtr>::iterator bm = biomasses.begin(); bm != biomasses.end(); bm++) {
    BiomassPtr currentBm = (*bm);

    // ... Each of which has a list of compounds in it
    for ( vector<BiomassCompoundPtr>::iterator bmc = currentBm->biomassCompoundList.begin(); bmc != currentBm->biomassCompoundList.end(); bmc++) {

      // Here is the compound (translating from model compound to biochemistry compound)
      string bmcUuid = (*bmc)->modelCompoundUuid;
      ModelCompoundPtr modelCompoundPtr  = problem.model->modelCompoundList[bmcUuid];
      CompoundPtr compoundPtr = modelCompoundPtr->compound;

      // And here is its compartment.
      string modelCompartmentUuid = modelCompoundPtr->modelCompartmentUuid;
      ModelCompartmentPtr modelCompartmentPtr = problem.model->modelCompartmentList[modelCompartmentUuid];
      CompartmentPtr compartmentPtr = modelCompartmentPtr->compartment;

      biomassTargetList.push_back(compoundPtr);
      biomassCompartmentList.push_back(compartmentPtr);
    }
  }

  //! Step 2 - Convert probabilities into costs
  //! Lets talk about this next week.

  //! Step 3 - Hook up to MFA toolkit

  //! Step 4 - Find K solutions (K isn't a parameter yet but it will be soon)

  //! Step 5 - Expand into a model object. Maybe the seed people already have
  // some better ideas on how to iterate over and find optimal fit to experiment.

  MetabolicModel newModel;

  return newModel;
}
