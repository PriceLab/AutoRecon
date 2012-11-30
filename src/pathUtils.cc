#include "pathUtils.h"
#include "Models.h"

using std::vector;
using std::map;

bool isExternalCompound(ModelCompoundPtr compound, Problem& problem)
{
	// Model compound points to model compartment ...
	ModelCompartmentMapIterator miter = problem.model.modelCompartmentList.find(compound->modelCompartmentUuid);

	// ... which points to compartment ...
	CompartmentMapIterator citer = problem.biochem.compartmentList.find(miter->second->compartmentUuid);

	// ... which has a name.
	if (citer->second->name == "Extracellular") return true;
	return false;
}
