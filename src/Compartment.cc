#include "Compartment.h"

Compartment::Compartment(Json::Value val)
{
	name = val["name"].asString();
	modDate = val["modDate"].asString();
	id = val["id"].asString();
	uuid = val["uuid"].asString();
	hierarchy = val["hierarchy"].asInt();
}

std::ostream& operator<<(std::ostream& out, Compartment& obj)
{
	out << "-Compartment-" << endl;
	out << "name: " << obj.name << "; uuid: " << obj.uuid << "; modDate: " << obj.modDate;
	out << "; id: " << obj.id << "; hierarchy: " << obj.hierarchy;
	return out;
}
