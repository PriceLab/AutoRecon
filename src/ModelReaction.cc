#include "ModelReaction.h"
#include <cstdlib>

ModelReactionReagent::ModelReactionReagent(Json::Value val)
{
	modelCompoundUuid = val["modelcompound_uuid"].asString();
	coefficient = val["coefficient"].asDouble();
}

ModelReactionProteinSubunit::ModelReactionProteinSubunit(Json::Value val)
{
	roleUuid = val["role_uuid"].asString();
	triggering = atoi(val["triggering"].asString().c_str());
	optional = atoi(val["optional"].asString().c_str());
	Json::Value genes = val["modelReactionProteinSubunitGenes"];
	for (Json::ValueIterator iter = genes.begin(); iter != genes.end(); iter++)
	{
		Json::Value gene = *iter;
		geneList.push_back(gene["feature_uuid"].asString());
	}
}

ModelReactionProtein::ModelReactionProtein(Json::Value val)
{
	complexUuid = val["complex_uuid"].asString();
	note = val["note"].asString();
	Json::Value subunits = val["modelReactionProteinSubunits"];
	for (Json::ValueIterator iter = subunits.begin(); iter != subunits.end(); iter++)
	{
		ModelReactionProteinSubunitPtr subunit(new ModelReactionProteinSubunit(*iter));
		reactionProteinSubunitList.push_back(subunit);
	}
}

ModelReaction::ModelReaction(Json::Value val)
{
	direction = val["direction"].asString();
	modelCompartmentUuid = val["modelcompartment_uuid"].asString();
	modDate = val["modDate"].asString();
	uuid = val["uuid"].asString();
	protons = atoi(val["protons"].asString().c_str());
	reactionUuid = val["reaction_uuid"].asString();
	Json::Value reagents = val["modelReactionReagents"];
	for (Json::ValueIterator iter = reagents.begin(); iter != reagents.end(); iter++)
	{
		ModelReactionReagentPtr reagent(new ModelReactionReagent(*iter));
		modelReactionReagentList.push_back(reagent);
	}
	Json::Value proteins = val["modelReactionProteins"];
	for (Json::ValueIterator iter = proteins.begin(); iter != proteins.end(); iter++)
	{
		ModelReactionProteinPtr protein(new ModelReactionProtein(*iter));
		modelReactionProteinList.push_back(protein);
	}
}

std::ostream& operator<<(std::ostream& out, ModelReaction& obj)
{
	out << "-ModelReaction-" << endl;
	out << "uuid: " << obj.uuid << "; modDate: " << obj.modDate << "; direction: " << obj.direction;
	out << "; modelCompartmentUuid: " << obj.modelCompartmentUuid << "; reactionUuid: " << obj.reactionUuid;
	out << "; protons: " << obj.protons << "; reagentList size: " << obj.modelReactionReagentList.size();
	out << "; proteinList size: " << obj.modelReactionProteinList.size();
	return out;
}
