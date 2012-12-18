#include "Biomass.h"
#include <cstdlib>

BiomassCompound::BiomassCompound(Json::Value val)
{
	modelCompoundUuid = val["modelcompound_uuid"].asString();
	coefficient = val["coefficient"].asDouble();
}

Biomass::Biomass(Json::Value val)
{
	dna = atof(val["dna"].asString().c_str());
	rna = atof(val["rna"].asString().c_str());
	cofactor = atof(val["cofactor"].asString().c_str());
	energy = val["energy"].asInt();
	locked = atoi(val["locked"].asString().c_str());
	name = val["name"].asString();
	cellwall = atof(val["cellwall"].asString().c_str());
	uuid = val["uuid"].asString();
	protein = atof(val["protein"].asString().c_str());
	modDate = val["modDate"].asString();
	lipid = atof(val["lipid"].asString().c_str());
	Json::Value biomasscompounds = val["biomasscompounds"];
	for (Json::ValueIterator iter = biomasscompounds.begin(); iter != biomasscompounds.end(); iter++)
	{
		BiomassCompoundPtr biomassCompound(new BiomassCompound(*iter));
		biomassCompoundList.push_back(biomassCompound);
	}
}

std::ostream& operator<<(std::ostream& out, Biomass& obj)
{
	out << "-Biomass-" << endl;
	out << "name: " << obj.name << "; uuid: " << obj.uuid << "; modDate: " << obj.modDate;
	out << "; dna: " << obj.dna << "; rna: " << obj.rna << "; cofactor: " << obj.cofactor;
	out << "; energy: " << obj.energy << "; locked: " << obj.locked << "; protein: " << obj.protein;
	out << "; lipid: " << obj.lipid << "; biomassCompoundList size: " << obj.biomassCompoundList.size();
	return out;
}
