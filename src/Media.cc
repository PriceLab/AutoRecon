#include "Media.h"
#include <cstdlib>

MediaCompound::MediaCompound(Json::Value val)
{
	compoundUuid = val["compound_uuid"].asString();
	concentration = atof(val["concentration"].asString().c_str());
	maxFlux = atof(val["maxFlux"].asString().c_str());
	minFlux = atof(val["minFlux"].asString().c_str());
}

std::ostream& operator<<(std::ostream& out, MediaCompound& obj)
{
	out << "-MediaCompound-" << endl;
	out << "compoundUuid: " << obj.compoundUuid << "; concentration: " << obj.concentration;
	out << "; minFlux: " << obj.minFlux << "; maxFlux: " << obj.maxFlux;
	return out;
}

Media::Media(Json::Value val)
{
	isMinimal = atoi(val["isMinimal"].asString().c_str());
	name = val["name"].asString();
	modDate = val["modDate"].asString();
	id = val["id"].asString();
	type = val["type"].asString();
	uuid = val["uuid"].asString();
	isDefined = atoi(val["isDefined"].asString().c_str());
	Json::Value mediaCompounds = val["mediacompounds"];
	for (Json::ValueIterator iter = mediaCompounds.begin(); iter != mediaCompounds.end(); iter++)
	{
		MediaCompoundPtr mc(new MediaCompound(*iter));
		mediaCompoundList.push_back(mc);
	}
}

std::ostream& operator<<(std::ostream& out, Media& obj)
{
	out << "-Media-" << endl;
	out << "name: " << obj.name << "; uuid: " << obj.uuid << "; modDate: " << obj.modDate;
	out << "; id: " << obj.id << "; type: " << obj.type;
	out << "; isMinimal: " << obj.isMinimal << "; isDefined: " << obj.isDefined;
	out << "; mediaCompoundList size: " << obj.mediaCompoundList.size();
	return out;
}
