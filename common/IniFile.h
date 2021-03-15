#pragma once

#include <string>
#include <map>

using std::map;
using std::string;
using std::wstring;

typedef map<string, string>  TAG_VAL;

class CIniFile
{
public:
    CIniFile(void);
    ~CIniFile(void);
    
    
    static int ReadFile(TAG_VAL &tagval, string strFile);
    static int WriteFile(const TAG_VAL &tagval, string strFile);

    static string ltrim(string str);
    static string rtrim(string str);
    static string trim(string str);
};
