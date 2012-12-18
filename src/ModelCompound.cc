#include "ModelCompound.h"
#include <cstdlib>

ModelCompound::ModelCompound(Json::Value val)
{
	modelCompartmentUuid = val["modelcompartment_uuid"].asString();
	compoundUuid = val["compound_uuid"].asString();
	charge = atoi(val["charge"].asString().c_str());
	formula = val["formula"].asString();
	modDate = val["modDate"].asString();
	uuid = val["uuid"].asString();
	isTransporter = false;
}

std::ostream& operator<<(std::ostream& out, ModelCompound& obj)
{
	out << "-ModelCompound-" << endl;
	out << "uuid: " << obj.uuid << "; modDate: " << obj.modDate;
	out << "; compoundUuid: " << obj.compoundUuid << "; modelCompartmentUuid: " << obj.modelCompartmentUuid;
	out << "; charge: " << obj.charge << "; formula: " << obj.formula;
	return out;
}
