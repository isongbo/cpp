#pragma once
#include "cJSON.h"
#include <map>
#include <string>
using namespace std;
class jsonfomat
{
public:
	jsonfomat(void);
	~jsonfomat(void);
public:
	bool Parse(string strjson, map<string, string> &mapvalue);
	string Returnjson(map<string, string> &mapvalue);
	string ToJson(std::string str);
	string Returnform(map<string, string> &mapvalue);
	string ToString(std::string str);
	void ParseJson(cJSON * root, std::map<std::string, std::string> &mapValue);
	std::string addMac(std::string strjson, std::string mac);

};
