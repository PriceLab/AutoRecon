#include "Reaction.h"
#include <cstdlib>
#include <sstream>

Reaction::Reaction(Json::Value val)
{
	status = val["status"].asString();
	direction = val["direction"].asString();
	if (val["deltaGErr"] != Json::Value::null)
	{
		deltaGErr = val["deltaGErr"].asString();
	}
	Json::Value cues = val["cues"];
	Json::Value::Members members = cues.getMemberNames();
	try
	{
		for (Json::Value::Members::iterator mbrIter = members.begin(); mbrIter != members.end(); mbrIter++)
		{
			cueList.insert(pair<string,string>(*mbrIter,cues[*mbrIter].asString()));
		}
	}
	catch (std::exception& e)
	{
		cerr << "invalid cue for reaction " << name << " with uuid " << uuid << endl;
	}
	name = val["name"].asString();
	try
	{
		probability = val["probability"].asDouble();
	}
	catch (std::exception& e)
	{
		probability = atof(val["probability"].asString().c_str());
	}
	modDate = val["modDate"].asString();
	abbreviation = val["abbreviation"].asString();
	cksum = val["cksum"].asString();
	if (val["deltaG"] != Json::Value::null)
	{
		deltaG = val["deltaG"].asString();
	}
	defaultProtons = atoi(val["defaultProtons"].asString().c_str());
	Json::Value reagents = val["reagents"];
	for (Json::ValueIterator iter = reagents.begin(); iter != reagents.end(); iter++)
	{
		ReagentPtr reagent(new Reagent(*iter));
		reagentList.push_back(reagent);
	}
	complexInfo = val["complexinfo"].asString();
	thermoReversibility = val["thermoReversibility"].asString();
	GPR = val["GPR"].asString();
	uuid = val["uuid"].asString();
}

string Reaction::createEquation(void)
{
	if (!equation.empty())
	{
		return equation;
	}
	return equation;
}

string Reaction::toDBString(AliasSetPtr aliasSet)
{
	string id;
	UuidMapIterator found = aliasSet->uuidList.find(uuid);
	if (found != aliasSet->uuidList.end())
	{
		id = found->second;
	}
	else
	{
		id = uuid;
	}
	ostringstream line;
	line << abbreviation << "\t" << deltaG << "\t" << deltaGErr << "\t" << createEquation() << "\t" <<
			id << "\t" << name << "\t" << direction << "\t" << status << "\t" << thermoReversibility;
	return line.str();
}

std::ostream& operator<<(std::ostream& out, Reaction& obj)
{
	out << "-Reaction-" << endl;
	out << "name: " << obj.name << "; uuid: " << obj.uuid << "; modDate: " << obj.modDate;
	out << "; abbreviation: " << obj.abbreviation << "; status: " << obj.status;
	out << "; deltaG: " << obj.deltaG << "; deltaGErr: " << obj.deltaGErr;
	out << "; direction: " << obj.direction << "; thermoReversibility: " << obj.thermoReversibility;
	out << "; defaultProtons: " << obj.defaultProtons << "; probability: " << obj.probability;
	out << "; cksum: " << obj.cksum << "; complexInfo: " << obj.complexInfo << "; GPR: " << obj.GPR;
	out << "; reagentList size: " << obj.reagentList.size() << "; cueList size: " << obj.cueList.size();
	return out;
}
