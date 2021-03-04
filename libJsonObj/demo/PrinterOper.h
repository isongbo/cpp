#pragma once

#include <list>
#include <map>
#include <vector>
#include <string>

using namespace std;

void AddNextLineY(list<map<string, string>>& listValue);
void parseFormate(list<map<string, string>>& listValue);
string ParsePrintData(map<string,string>& mapValueFormat, map<string,string>& mapValue);