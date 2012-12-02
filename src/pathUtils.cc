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

void findPaths(Problem &problem, map<ReactionPtr, vector<ReactionPtr> > synonymList, CompoundPtr biomassComponent ) {
  //! In this function we use the MFA toolkit to construct a maximum-likelihood
  //! FBA problem from the biochemsitry within the input Problem structure
  //! and solve it.
  //!
  //! biomassComponent is a compound for which we want to require production.

  return;
}
