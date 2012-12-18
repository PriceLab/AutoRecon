#include "Models.h"
#include "Utility.h"
#include <fstream>

MetabolicModel::MetabolicModel()
{
	version = 0;
}

bool MetabolicModel::loadFromFile(const char* filename)
{
	StopWatch timer;

	// Parse the model json file.
	cout << "Loading model from '" << filename << "' ... " << endl;
	ifstream myfile;
	myfile.open(filename);
	if (myfile.fail())
	{
		cout << "  FAILED Error opening model file" << endl;
		return false;
	}
	Json::Value root;
	Json::Reader reader;
	if (reader.parse(myfile, root) == false)
	{
		cout << "  FAILED Error parsing model file" << endl << reader.getFormatedErrorMessages();
		myfile.close();
		return false;
	}
	myfile.close();

#if 0
	Json::Value::Members members = root.getMemberNames();
	for (Json::Value::Members::iterator iter = members.begin(); iter != members.end(); iter++)
	{
		cout << "root member " << *iter << endl;
	}
#endif

	// Load root members.
	annotationUuid = root["annotation_uuid"].asString();
	biochemistryUuid = root["biochemistry_uuid"].asString();
	current = root["current"].asInt();
	defaultNameSpace = root["defaultNameSpace"].asString();
	growth = root["growth"].asInt();
	id = root["id"].asString();
	mappingUuid = root["mapping_uuid"].asString();
	modDate = root["modDate"].asString();
	name = root["name"].asString();
	status = root["status"].asString();
	type = root["type"].asString();
	uuid = root["uuid"].asString();
	version = root["__VERSION__"].asInt();

	Json::Value biomasses = root["biomasses"];
	for (Json::ValueIterator iter = biomasses.begin(); iter!= biomasses.end(); iter++)
	{
		BiomassPtr biomass(new Biomass(*iter));
		biomassList.push_back(biomass);
	}
#if 0
	cout << "Added " << biomassList.size() << " biomasses" << endl;
	cout << "First biomass is " << endl << *biomassList[0] << endl;
#endif

	Json::Value modelCompounds = root["modelcompounds"];
	for (Json::ValueIterator iter = modelCompounds.begin(); iter!= modelCompounds.end(); iter++)
	{
		ModelCompoundPtr modelCompound(new ModelCompound(*iter));
		modelCompoundList.insert(ModelCompoundMapValue(modelCompound->uuid, modelCompound));
	}
#if 0
	cout << "Added " << modelCompoundList.size() << " model compounds" << endl;
	cout << "First model compound is " << endl << *modelCompoundList.begin()->second << endl;
#endif

	Json::Value modelCompartments = root["modelcompartments"];
	for (Json::ValueIterator iter = modelCompartments.begin(); iter!= modelCompartments.end(); iter++)
	{
		ModelCompartmentPtr modelCompartment(new ModelCompartment(*iter));
		modelCompartmentList.insert(ModelCompartmentMapValue(modelCompartment->uuid, modelCompartment));
	}
#if 0
	cout << "Added " << modelCompartmentList.size() << " model compartments" << endl;
	cout << "First model compartment is " << endl << *modelCompartmentList.begin()->second << endl;
#endif

	Json::Value modelReactions = root["modelreactions"];
	for (Json::ValueIterator iter = modelReactions.begin(); iter!= modelReactions.end(); iter++)
	{
		ModelReactionPtr modelReaction(new ModelReaction(*iter));
		modelReactionList.push_back(modelReaction);
	}
#if 0
	cout << "Added " << modelReactionList.size() << " model reactions" << endl;
	cout << "First model reaction is " << endl << *modelReactionList[0] << endl;
#endif

	cout << "  done (" << timer.stop() << " seconds)" << endl;
	return true;
}

std::ostream& operator<<(std::ostream& out, MetabolicModel& obj)
{
	out << "-MetabolicModel-" << endl;
	out << "name: " << obj.name << "; uuid: " << obj.uuid << "; modDate: " << obj.modDate;
	out << "; version: " << obj.version << "; id: " << obj.id << "; status: " << obj.status;
	out << "; type: " << obj.type << "; defaultNameSpace: " << obj.defaultNameSpace;
	out << "; current: " << obj.current << "; growth: " << obj.growth;
	out << "; annotationUuid; " << obj.annotationUuid;
	out << "; biochemistryUuid: " << obj.biochemistryUuid << "; mappingUuid: " << obj.mappingUuid;
	out << "; biomassList size: " << obj.biomassList.size() << "; modelCompartmentList size: " << obj.modelCompartmentList.size();
	out << "; modelCompoundList size: " << obj.modelCompoundList.size() << "; modelReactionList size: " << obj.modelReactionList.size();
	return out;
}

