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


  //! FIXME - Use user input to
  //! identify which media condition from the biomchemistry object
  //! to use with the path-finding algortihm.
  MediaPtr media = findMedia(argv[3]);

  //! Use the biomass objective identified in the model JSON object
  //! passed into this function.
  //! This ensures that we are using the same target as the ModelSEED
  //! and thus that we can directly compare the results of our algorithms.

  vector<BiomassPtr> biomassList = problem.model.biomassList;

  //! This is the function I have set up for you to call the MFA toolkit
  //! and get solutions back
  //!
  //! I'd suggest we find out what data structure they're using to store
  //! results in the MFA toolkit and use the same one.
  //!
  MetabolicModel newModel = findPaths(problem, rxnSynonyms, media, biomassList);

  return 0;
}
