#include "Problem.h"
#include "pathUtils.h"

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <vector>
#include <map>

using namespace std;

int main(int argc, char *argv[])
{
  if (argc != 4) {
    cout << "Usage: ./a.out [biochem_json] [model_json] [Desired_media_name]" << endl;
    cout << "Desired_media_name is the name of one of the biochemistry medias or its UUID." << endl;
    exit(EXIT_FAILURE);
  }

  //! Parse JSON files.
  Problem problem;
  if (problem.biochem->loadFromFile(argv[1]) == false)
  {
	  exit(EXIT_FAILURE);
  }
  if (problem.model->loadFromFile(argv[2]) == false)
  {
	  exit(EXIT_FAILURE);
  }
  if (problem.validate() == false)
  {
	  exit(EXIT_FAILURE);
  }

  //! Identify synonymous reactions in the biochemistry.
  SynonymMap rxnSynonyms;
  problem.biochem->findSynonyms(rxnSynonyms);

  //! Identify which media condition from the biomchemistry object
  //! to use with the path-finding algorithm.
  MediaPtr media;
  bool found = problem.biochem->findMedia(argv[3], media);
  if (!found)
  {
	  cout << "FAILED media " << argv[3] << " was not found in biochemistry" << endl;
	  exit(EXIT_FAILURE);
  }
  cout << "Desired media " << *media << endl;

  //! This is the function I have set up for you to call the MFA toolkit
  //! and get solutions back
  //!
  //! I'd suggest we find out what data structure they're using to store
  //! results in the MFA toolkit and use the same one.
  //!
  MetabolicModelPtr newModel = problem.findPaths(rxnSynonyms, media, 1);

  return 0;
}
