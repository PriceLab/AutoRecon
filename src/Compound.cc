#include "Compound.h"
#include <stdlib.h>

Compound::Compound(Json::Value val)
{
	defaultCharge = atoi(val["defaultCharge"].asString().c_str());
	isCofactor = atoi(val["isCofactor"].asString().c_str());
	deltaG = atof(val["deltaG"].asString().c_str());
	name = val["name"].asString();
	cksum = val["cksum"].asString();
	uuid = val["uuid"].asString();
	formula = val["formula"].asString();
	mass = atof(val["mass"].asString().c_str());
	modDate = val["modDate"].asString();
	abbreviation = val["abbreviation"].asString();
	unchargedFormula = val["unchargedFormula"].asString();
	deltaGErr = atof(val["deltaGErr"].asString().c_str());
	Json::Value cues = val["cues"];
	Json::Value::Members cueMembers = cues.getMemberNames();
	for (Json::Value::Members::iterator iter = cueMembers.begin(); iter != cueMembers.end(); iter++)
	{
		try
		{
			cueList.insert(pair<string,int>(*iter,cues[*iter].asInt()));
		}
		catch (std::exception& e)
		{
			string str = cues[*iter].asString();
			cueList.insert(pair<string,int>(*iter,atoi(str.c_str())));
		}
	}
	Json::Value pkas = val["pkas"];
	Json::Value::Members pkaMembers = pkas.getMemberNames();
	for (Json::Value::Members::iterator iter = pkaMembers.begin(); iter != pkaMembers.end(); iter++)
	{
		Json::Value pka = pkas[*iter];
		vector<double> pkaValues;
		for (int index = 0; index < pka.size(); index++)
		{
			pkaValues.push_back(atof(pka[index].asString().c_str()));
		}
		pkaList.insert(pair< string,vector<double> >(*iter,pkaValues));
	}
	Json::Value pkbs = val["pkbs"];
	Json::Value::Members pkbMembers = pkbs.getMemberNames();
	for (Json::Value::Members::iterator iter = pkbMembers.begin(); iter != pkbMembers.end(); iter++)
	{
		Json::Value pkb = pkbs[*iter];
		vector<double> pkbValues;
		for (int index = 0; index < pkb.size(); index++)
		{
			pkbValues.push_back(atof(pkb[index].asString().c_str()));
		}
		pkbList.insert(pair< string,vector<double> >(*iter,pkbValues));
	}

	Json::Value structureUuids = val["structure_uuids"];
	for (int index = 0; index < structureUuids.size(); index++)
	{
		structureUuidList.push_back(structureUuids[index].asString());
	}
}

std::ostream& operator<<(std::ostream& out, Compound& obj)
{
	out << "-Compound-" << endl;
	out << "name: " << obj.name << "; uuid: " << obj.uuid << "; modDate: " << obj.modDate;
	out << "; abbreviation: " << obj.abbreviation << "; defaultCharge: " << obj.defaultCharge;
	out << "; isCofactor: " << obj.isCofactor << "; deltaG: " << obj.deltaG << "; deltaGErr: " << obj.deltaGErr;
	out << "; formula: " << obj.formula << "; unchargedFormula: " << obj.unchargedFormula;
	out << "; mass: " << obj.mass << "; cksum: " << obj.cksum;
	out << "; cueList size: " << obj.cueList.size() << "; structureUuidList size: " << obj.structureUuidList.size();
	out << "; pkaList size: " << obj.pkaList.size() << "; pkbList size: " << obj.pkbList.size();
	return out;
}

