#include "FileLoader.h"
#include "Models.h"
#include "pathUtils.h"

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <vector>
#include <map>

using namespace std;

int main(int argc, char *argv[]) {
  if (argc != 4) {
    cout << "Usage: ./a.out [biochem_json] [model_json] [Desired_media_name]" << endl;
    cout << "Desired_media_name is the name of one of the biochemistry medias or its UUID." << endl;
    exit(EXIT_FAILURE);
  }

  //! Parse JSON files.
  Problem problem;

  loadBiochemistryFile(argv[1], problem.biochem);
  loadModelFile(argv[2], problem.model);
  validateProblem(problem);

  //! Identify synonymous reactions
  map<ReactionPtr, vector<ReactionPtr> > rxnSynonyms = computeSynonyms(problem);

  //! Use the ModelSEED model file to identify the biomass equation to use.
  //! This ensures that we are using the same target as the ModelSEED
  //! and thus that we can directly compare the results of our algorithms.

  vector<BiomassPtr> biomassList = problem.model.biomassList;

  // These should be filled up such that the index lines up.
  // There has to be a better way to store pairs of objects so that they have
  // to be unique together...
  //
  // Maybe we should just define an object with a compound and a compartment UUID
  // and stick with that to make life much easier down the road. Thoughts?
  vector<CompoundPtr> biomassTargetList;
  vector<CompartmentPtr> biomassCompartmentList;

  // There can be more than one biomass equation...
  for (vector<BiomassPtr>::iterator bm = biomassList.begin(); bm != biomassList.end(); bm++) {
    BiomassPtr currentBm = (*bm);

    // ... Each of which has a list of compounds in it
    for ( vector<BiomassCompoundPtr>::iterator bmc = currentBm->biomassCompoundList.begin(); bmc != currentBm->biomassCompoundList.end(); bmc++) {

      string bmcUuid = (*bmc)->modelCompoundUuid;
      ModelCompoundPtr modelCompoundPtr  = problem.model.modelCompoundList[bmcUuid];

      // ... the model compound contains a few important things:
      // a model compartment
      string modelCompartmentUuid = modelCompoundPtr->modelCompartmentUuid;

      // Technically this isn't safe unless the validator correctly checkes that all model compartment uuids
      // are in the mocelCompartmentList
      ModelCompartmentPtr modelCompartmentPtr = problem.model.modelCompartmentList[modelCompartmentUuid];

      // A biochemistry compartment
      CompoundPtr compoundPtr = modelCompoundPtr->compound;
      CompartmentPtr compartmentPtr = modelCompartmentPtr->compartment;

      // Add that compound to a list of desired targets.
      biomassTargetList.push_back(compoundPtr);
      biomassCompartmentList.push_back(compartmentPtr);
    }
  }

  //! This is the function I have set up for you to call the MFA toolkit
  //! and get solutions back
  //!
  //! I'd suggest we find out what data structure they're using to store
  //! results in the MFA toolkit and use the same one.
  //!
  //! FIXME - How do we make these unique so we don't keep doign the same thing
  //! over and over?
  for ( int ii = 0; ii < biomassTargetList.size(); ii++ ) {
    findPaths(problem, rxnSynonyms, biomassTargetList.at(ii) );
  }

  return 0;
}
