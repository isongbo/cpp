#include "StdAfx.h"
#include "jsonfomat.h"

jsonfomat::jsonfomat(void)
{
}

jsonfomat::~jsonfomat(void)
{
}
void jsonfomat::ParseJson(cJSON * root, std::map<std::string,std::string> &mapValue)
{
	for(int i=0; i<cJSON_GetArraySize(root); i++)   
	{
		cJSON * item = cJSON_GetArrayItem(root, i);        
		if(cJSON_Object == item->type)      
			ParseJson(item,mapValue);
		else                            
		{
			if (cJSON_Int == item->type)
			{
				mapValue.insert(pair<std::string,std::string>(item->string,cJSON_Print(item)));
			}
			else if (cJSON_False == item->type)
			{
				if (strcmp(cJSON_Print(item), "true") == 0)
				{
					mapValue.insert(pair<std::string,std::string>(item->string,"1"));
				}
				else
				{
					mapValue.insert(pair<std::string,std::string>(item->string,"0"));

				}
			}
			else
			{
				mapValue.insert(pair<std::string,std::string>(item->string,ToJson(cJSON_Print(item))));
			}
			
		}
	}
}


string jsonfomat::Returnjson(map<string, string> &mapvalue)
{
	cJSON* root = NULL, *object = NULL;
	root = cJSON_CreateObject();
	map<string, string>::iterator itor;
	string ret;

	for (itor = mapvalue.begin() ; itor != mapvalue.end(); itor++)
	{

		string temp = itor->second;
		cJSON *object = cJSON_Parse(temp.c_str());
		if (object != cJSON_False)
		{
			if (object->type == cJSON_Array || object->type == cJSON_Object)
			{
				cJSON_AddItemToObject(root, itor->first.c_str(), object);
			}
			else
			{
				cJSON_AddStringToObject(root, itor->first.c_str(), itor->second.c_str());
			}
		}
		else
		{
			cJSON_AddStringToObject(root, itor->first.c_str(), itor->second.c_str());
		}
	}

	ret = cJSON_Print(root);
	cJSON_Delete(root);

	return ret;
}

std::string jsonfomat::Returnform(map<string, string> &mapvalue)
{
	map<string, string>::iterator itor;
	string str;

	for (itor = mapvalue.begin() ; itor != mapvalue.end(); itor++)
	{
		str +=itor->first.c_str();
		str += "=";
		str +=itor->second.c_str();
		str += "&";
	}
	return str.substr(0,str.length()-1);
}

std::string jsonfomat::ToJson(std::string str)
{
	string temp;
	if (str.length() > 2)
	{
		temp = str.substr(1, str.length()-2);

	}
	return temp;
}

bool jsonfomat::Parse(string strjson, map<string, string> &mapvalue)
{
	if (strjson.empty() == true)    return false;
	cJSON* root = NULL;
	root = cJSON_Parse(strjson.c_str());
	if (root == cJSON_False)    return false;
	ParseJson(root, mapvalue);
	if (mapvalue.empty())    return false;
	cJSON_Delete(root);

	return true;
}


std::string jsonfomat::ToString(std::string str)
{
	return str.substr(1, str.length() - 2);
}

std::string jsonfomat::addMac(std::string strjson, std::string mac)
{
	if (strjson.empty() == true)    return false;

	cJSON* root = NULL;
	cJSON* pItem = NULL;
	string ret;

	root = cJSON_Parse(strjson.c_str());
	if (root == cJSON_False)    return false;

	cJSON_DeleteItemFromObject(root, "deviceMac");
	
	cJSON_AddItemToObject(root, "deviceMac", cJSON_CreateString(mac.c_str()));

	ret = cJSON_Print(root);

	cJSON_Delete(root);

	return ret;
}