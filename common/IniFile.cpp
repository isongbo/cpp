#include "stdafx.h"
#include "IniFile.h"

#include <algorithm>
#include <fstream>

//using namespace std;
using std::string;
using std::ios;
using std::ifstream;
using std::ofstream;
using std::endl;

CIniFile::CIniFile(void)
{
}

CIniFile::~CIniFile(void)
{
}

int CIniFile::ReadFile(TAG_VAL &tagval, string strFile)
{
    tagval.clear();
    
    ifstream is;

    is.open(strFile.c_str(), ios::in);

    if (is.is_open())
    {
        char buf[256]={0};
        char *p = NULL;

        string first;
        string second;

        while(!is.eof())
        {
            is.getline(buf, 256);
            if (';' == buf[0])
                continue;
            
            p = strstr(buf, "=");
            if (p != NULL)
            {
                *p = '\0';
                p++;
                first = trim(buf);
                second = trim(p);
                
                transform(first.begin(), first.end(), first.begin(), tolower);

                tagval[first] = second;
            }
        }

        is.close();
    }

    return 0;
}

int CIniFile::WriteFile(const TAG_VAL &tagval, string strFile)
{
    if (tagval.empty())
        return 0;

    ofstream os;

    os.open(strFile.c_str(), ios::out);

    if (os.is_open())
    {
        TAG_VAL::const_iterator itr = tagval.begin();
        for (; itr != tagval.end(); itr++)
        {
            os << itr->first << " = " << itr->second << endl;
        }

        os.close();
    }

    return 0;
}

string CIniFile::ltrim(string str)
{
    int s=str.find_first_not_of(" \t");
    
    return str.substr(s,str.length());
}

string CIniFile::rtrim(string str)
{
    int e=str.find_last_not_of(" \t");

    return str.substr(0,e+1);
}

string CIniFile::trim(string str)
{
    int s=str.find_first_not_of(" \t");
    int e=str.find_last_not_of(" \t");

    return str.substr(s,e-s+1);
}