#include "ModelCompartment.h"
#include <cstdlib>

ModelCompartment::ModelCompartment(Json::Value val)
{
	pH = val["pH"].asDouble();
	compartmentIndex = val["compartmentIndex"].asInt();
	potential = atof(val["potential"].asString().c_str());
	modDate = val["modDate"].asString();
	compartmentUuid = val["compartment_uuid"].asString();
	label = val["label"].asString();
	uuid = val["uuid"].asString();
}

std::ostream& operator<<(std::ostream& out, ModelCompartment& obj)
{
	out << "-ModelCompartment-" << endl;
	out << "uuid: " << obj.uuid << "; modDate: " << obj.modDate << "; label: " << obj.label;
	out << "; pH: " << obj.pH << "; potential: " << obj.potential;
	out << "; compartmentUuid: " << obj.compartmentUuid << "; compartmentIndex: " << obj.compartmentIndex;
	return out;
}
