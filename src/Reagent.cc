#include "Reagent.h"
#include <cstdlib>

Reagent::Reagent(Json::Value val)
{
	isCofactor = atoi(val["isCofactor"].asString().c_str());
	compoundUuid = val["compound_uuid"].asString();
	compartmentUuid = val["compartment_uuid"].asString();
	coefficient = val["coefficient"].asInt();
}

std::ostream& operator<<(std::ostream& out, Reagent& obj)
{
	out << "-Reagent-" << endl;
	out << "compoundUuid: " << obj.compoundUuid << "; compartmentUuid: " << obj.compartmentUuid;
	out << "; isCofactor: " << obj.isCofactor << "; coefficient: " << obj.coefficient;
	return out;
}
