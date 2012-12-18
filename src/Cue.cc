#include "Cue.h"
#include <cstdlib>

Cue::Cue(Json::Value val)
{
	priority = val["priority"].asInt();
	defaultCharge = atoi(val["defaultCharge"].asString().c_str());
	deltaG = atof(val["deltaG"].asString().c_str());
	name = val["name"].asString();
	cksum = val["cksum"].asString();
	uuid = val["uuid"].asString();
	smallMolecule = atoi(val["smallMolecule"].asString().c_str());
	formula = val["formula"].asString();
	modDate = val["modDate"].asString();
	abbreviation = val["abbreviation"].asString();
	unchargedFormula = val["unchargedFormula"].asString();
	deltaGErr = atof(val["deltaGErr"].asString().c_str());
}

std::ostream& operator<<(std::ostream& out, Cue& obj)
{
	out << "-Cue-" << endl;
	out << "name: " << obj.name << "; uuid: " << obj.uuid << "; modDate: " << obj.modDate;
	out << "; abbreviation: " << obj.abbreviation << "; defaultCharge: " << obj.defaultCharge;
	out << "; smallMolecule: " << obj.smallMolecule << "; deltaG: " << obj.deltaG << "; deltaGErr: " << obj.deltaGErr;
	out << "; formula: " << obj.formula << "; unchargedFormula: " << obj.unchargedFormula;
	out << "; priority: " << obj.priority << "; cksum: " << obj.cksum;
	return out;
}
