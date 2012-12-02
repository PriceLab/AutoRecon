#include "pathUtils.h"
#include "Models.h"

#include <map>
#include <vector>
#include <set>

using std::vector;
using std::map;

bool isExternalCompound(ModelCompoundPtr compound, Problem& problem)
{
  //! Based on the name of the compartment in which a compound is found,
  //! identify whether or not it is extra-cellular.
  //!
  //! Returns TRUE if the compound is extra-cellular and FALSE otherwise.

  // Model compound points to model compartment ...
  ModelCompartmentMapIterator miter = problem.model.modelCompartmentList.find(compound->modelCompartmentUuid);
  
  // ... which points to compartment ...
  CompartmentMapIterator citer = problem.biochem.compartmentList.find(miter->second->compartmentUuid);
  
  // ... which has a name.
  if (citer->second->name == "Extracellular") return true;
  return false;
}

////INCOMPLETE functions
set<CompoundPtr> getCofactorList(Problem &problem) {
  //! Given a problem structure, iterate over the biochemistry
  //! object to obtain a list of metabolites (compounds) that are
  //! identified as cofactors.
  //!
  //! Returns a list of pointers to compounds that are labeled as "cofactors"
  //! in the biochemistry object. This uses the metabolite attribute, not the
  //! reaction-specific attribute.

  set<CompoundPtr> cofactorList;

  return cofactorList;
}

map<ReactionPtr, vector<ReactionPtr> > computeSynonyms(Problem &problem) {
  //! Given a problem structure, iterate over the biochemistry object
  //! to identify sets of reactions with identical metabolites except
  //! for those that are identified as cofactors either in the metabolite
  //! list or in the reaction itself.
  //!
  //! It is assumed that a metabolite that is declared as a cofactor
  //! is a cofactor regardless of what compartment it is found in.
  //!
  //! Returns a mapping from each reaction's pointer to a list of that reaction's
  //! synonyms (including itself)

  map<ReactionPtr, vector<ReactionPtr> > synList;

  // List of metabolites labeled as cofactors in the biochemistry object
  set<CompoundPtr> cofactorList;

  // Mapping from each reaction to a list of non-cofactor metabolites found within it
  map<ReactionPtr, vector<CompoundPtr> > netReactions;

  //! Get list of cofactor metabolites
  cofactorList = getCofactorList(problem);

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


  //! Itentify lists of synonymous reactions.
  //! This part would be trivial if we can use the alternate implementation described above.
  //! As it is in this sketch it would be O(N^2)
  // For each ReactionPtr:
  //   Make a vector<ReactionPtr>
  //   For each ReactionPtr:
  //    If the vector<CompoundPtr> is the same as the reference:
  //      add the query reaction to the vector<ReactionPtr>
  //   Add ReactionPtr-->vector<ReactionPtr> to synList

  return synList;
}

MediaPtr findMedia(string medianame) {
  //! This function will identify a media based on the provided name
  //! It should be able to accept a common name (in the 'name' field)
  //! or a UUID (in the 'uuid' field) as input.
  //!
  //! The return valie is a pointer to the media conditions specified.
  //! Returns NULL on failure.
  MediaPtr mediaPtr;
  return mediaPtr;
}

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
  vector<BiomassPtr> biomassList = problem.model.biomassList;

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
      ModelCompoundPtr modelCompoundPtr  = problem.model.modelCompoundList[bmcUuid];
      CompoundPtr compoundPtr = modelCompoundPtr->compound;

      // And here is its compartment.
      string modelCompartmentUuid = modelCompoundPtr->modelCompartmentUuid;
      ModelCompartmentPtr modelCompartmentPtr = problem.model.modelCompartmentList[modelCompartmentUuid];
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
