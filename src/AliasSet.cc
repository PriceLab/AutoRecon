#include "AliasSet.h"

AliasSet::AliasSet(Json::Value val)
{
	attribute = val["attribute"].asString();
	source = val["source"].asString();
	name = val["name"].asString();
	modDate = val["modDate"].asString();
	className = val["class"].asString();
	uuid = val["uuid"].asString();
	Json::Value aliases = val["aliases"];
	Json::Value::Members members = aliases.getMemberNames();
	for (Json::Value::Members::iterator iter = members.begin(); iter != members.end(); iter++)
	{
		vector<string> uuids;
		Json::Value alias = aliases[*iter];
		for (int index = 0; index < alias.size(); index++)
		{
			uuids.push_back(alias[index].asString());
		}
		aliasList.insert(pair<string,vector<string> >(*iter, uuids));
		for (int index = 0; index < uuids.size(); index++)
		{
			uuidList.insert(pair<string,string>(uuids[index], *iter));
		}
	}
}

std::ostream& operator<<(std::ostream& out, AliasSet& obj)
{
	out << "-AliasSet-" << endl;
	out << "name: " << obj.name << "; uuid: " << obj.uuid << "; modDate: " << obj.modDate;
	out << "; attribute: " << obj.attribute << "; source: " << obj.source;
	out << "; class: " << obj.className << "; aliasList size: " << obj.aliasList.size();
	return out;
}

