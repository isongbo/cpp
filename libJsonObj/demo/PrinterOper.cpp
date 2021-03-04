#include "StdAfx.h"
#include "PrinterOper.h"
#include "Utils.h"
void parseFormate(list<map<string, string>>& listValue)
{

	FILE* fp = fopen("formate.txt","r");

	if (fp == NULL) return;

	fseek( fp, 0, SEEK_END);
	int fileSize = ftell(fp );
	fseek( fp, 0, SEEK_SET);

	char* pFilebuf = NULL;
	pFilebuf = new char[fileSize];
	memset(pFilebuf,0,fileSize);
	fread(pFilebuf,1,fileSize,fp);
	fclose(fp);

	cJSON* root = NULL;
	cJSON* item = NULL;

	root = cJSON_Parse(pFilebuf);
	if (root == cJSON_False)    return ;

	item = cJSON_GetObjectItem(root, "items");
	if (item == cJSON_False)    return ;

	std::map<std::string,std::string> mapValue;

	for(int i=0; i<cJSON_GetArraySize(item); i++)  
	{
		cJSON * key = cJSON_GetArrayItem(item, i);        
		if(cJSON_Object == key->type) 
		{
			for (int j = 0; j < cJSON_GetArraySize(key); j++)
			{
				cJSON* value = cJSON_GetArrayItem(key,j);
				string jsonkey;
				string jsonvalue;
				if(cJSON_Object != value->type)
				{
					if (cJSON_Int == value->type)
					{
						mapValue.insert(pair<std::string,std::string>(value->string,cJSON_Print(value)));
					}
					else if (cJSON_False == value->type)
					{
						if (strcmp(cJSON_Print(value), "true") == 0)
						{
							mapValue.insert(pair<std::string,std::string>(value->string,"1"));
						}
						else
						{
							mapValue.insert(pair<std::string,std::string>(value->string,"0"));
						}
					}
					else if (cJSON_String == value->type)
					{
						jsonkey = value->string;
						jsonvalue = ToString(cJSON_Print(value));
						mapValue.insert(pair<std::string,std::string>(jsonkey,jsonvalue));
					}
					else
					{	
						mapValue.insert(pair<std::string,std::string>(value->string,cJSON_Print(value)));
					}
				}
			}
			listValue.push_back(mapValue);
			mapValue.clear();
		}
	}
}
void AddNextLineY(list<map<string, string>>& listValue)
{
	list<map<string, string>>::iterator itor;
	map<string, string>::iterator mapitor;
	list<string> listY;
	string str;
	int index = 0;
	int listYSize = 0;

	for (itor = listValue.begin(); itor != listValue.end(); itor++)
	{
		mapitor = itor->find("y");
		if (mapitor != itor->end())
		{
			listY.push_back(mapitor->second);
		}
	}

	listY.pop_front();
	listYSize = listY.size();
	for (itor = listValue.begin(); itor != listValue.end(); itor++)
	{
		if (index < listYSize)
		{
			index++;
			str = listY.front();
			listY.pop_front();
			if (str.empty() == false)
			{
				itor->insert(pair<std::string,std::string>("NextLineY",str));
			}
		}
		else
		{
			itor->insert(pair<std::string,std::string>("NextLineY",""));
		}
	}
}

std::string ParsePrintData(map<string,string>& mapValueFormat, map<string,string>& mapValue)
{
	CJsonObject cJson;
	cJson.AddEmptySubArray("format");

	CJsonObject cJsonData;

	vector<string> vctstr;
	map<string, string>::iterator itor;
	map<string, string>::iterator SpecItor;

	string str;


	//解析数据
	itor = mapValueFormat.find("type");
	if (itor != mapValueFormat.end())
	{
		//logo
		if (itor->second.compare("logo") == 0)    
		{	
			string path = GetConfig("server","logo","","bank.ini"); 

			cJsonData.Add("type", PRINT_TYPE_IMAGE);
			cJsonData.Add("horizontal", PRINT_HORIZONTAL_LEFT);
			cJsonData.Add("data", path);

			cJson["format"].Add(cJsonData);
			std::string str = cJson.ToString();
			return str;
		}

		//二维码
		else if (itor->second.compare("qrCode") == 0)    
		{
			cJsonData.Add("type", PRINT_TYPE_IMAGE);
			cJsonData.Add("horizontal", PRINT_HORIZONTAL_CENTER);
			cJsonData.Add("data", "./ecode.bmp");

			cJson["format"].Add(cJsonData);
			std::string str = cJson.ToString();
			return str;
		}

		//机构
		else if (itor->second.compare("orgName") == 0)     
		{
			SpecItor = mapValue.find("instName");
			if (SpecItor != mapValue.end())
			{
				str = SpecItor->second;
			}

			SpecItor = mapValueFormat.find("x");
			if (SpecItor != mapValueFormat.end())    SpecItor->second = "130";

			SpecItor = mapValueFormat.find("size");
			if (SpecItor != mapValueFormat.end())    SpecItor->second = "30";
		}

		//业务
		else if (itor->second.compare("buzCnname") == 0)    
		{
			SpecItor = mapValue.find("bussinessType");
			if (SpecItor != mapValue.end())    str = SpecItor->second;
		}

		//票号
		else if (itor->second.compare("ticketNo") == 0)   
		{
			SpecItor = mapValue.find("lineUpNo");
			if (SpecItor != mapValue.end())    str = SpecItor->second;

			SpecItor = mapValueFormat.find("size");
			if (SpecItor != mapValueFormat.end())    SpecItor->second = "40";

			SpecItor = mapValueFormat.find("style");
			if (SpecItor != mapValueFormat.end())    SpecItor->second = "bold";
		}

		//柜员
		else if (itor->second.compare("counterNo") == 0)     
		{
			SpecItor = mapValue.find("counterNo");
			if (SpecItor != mapValue.end())    str = SpecItor->second;
		}

		//等待人数
		else if (itor->second.compare("buzWaitingCount") == 0)    
		{
			SpecItor = mapValue.find("waitNum");
			if (SpecItor != mapValue.end())    str = SpecItor->second;
		}

		//时间
		else if (itor->second.compare("fetchedDate") == 0)    
		{
			SpecItor = mapValue.find("produceTime");
			if (SpecItor != mapValue.end())    str = SpecItor->second;
		}

		//广告
		else if (itor->second.compare("ticketAdv") == 0)    
		{
			SpecItor = mapValue.find("recomProduct");
			if (SpecItor != mapValue.end())    str = SpecItor->second;

			vctstr = split(str,"\"");
			if (vctstr.empty())    return "";
		}

		//提示
		else if(itor->second.compare("tips") == 0)   
		{
			SpecItor = mapValue.find("tips");
			if (SpecItor != mapValue.end())
			{
				str = "温馨提示：";
				str += SpecItor->second;
			}
		}

		//文本
		else if(itor->second.compare("text") == 0)    
		{
			itor = mapValueFormat.find("text");
			if (itor != mapValueFormat.end())    str += itor->second;
		}
	}
	//是否加粗
	itor = mapValueFormat.find("style");
	if (itor != mapValueFormat.end())
	{
		if (itor->second.compare("bold") == 0)
		{

			cJsonData.Add("blod", PRINT_STYLE_BOLD);
		}
		else  
		{
			cJsonData.Add("blod", PRINT_STYLE_NONE);
		}
	}
	//字体
	itor = mapValueFormat.find("size");
	if (itor != mapValueFormat.end())
	{
		if (atoi(itor->second.c_str()) >= 25 && atoi(itor->second.c_str()) < 40)
		{
			cJsonData.Add("double", 3);
		}
		else if (atoi(itor->second.c_str()) >= 40)
		{
			cJsonData.Add("double", 1);
		}
		else
		{
			cJsonData.Add("double", 0);
		}
	}

	//左右位置
	itor = mapValueFormat.find("x");
	if (itor != mapValueFormat.end())
	{
		if (atoi(itor->second.c_str()) > 200)
		{
			cJsonData.Add("horizontal", PRINT_HORIZONTAL_RIGHT);
		}
		else if (atoi(itor->second.c_str()) > 120)
		{
			cJsonData.Add("horizontal", PRINT_HORIZONTAL_CENTER);
		}
		else 
		{
			cJsonData.Add("horizontal", PRINT_HORIZONTAL_LEFT);
		}
	}

	//是否换行
	itor = mapValueFormat.find("y");
	if (itor != mapValueFormat.end())
	{
		string curLineY = itor->second;
		itor = mapValueFormat.find("NextLineY");
		if (itor != mapValueFormat.end())
		{
			if (curLineY.compare(itor->second) == 0)
			{
				cJsonData.Add("line", 0);
			}
			else
			{
				cJsonData.Add("line", 1);
			}
		}
		else
		{
			cJsonData.Add("line", 1);
		}
	}

	//查找ticketAdv标签内容
	if (vctstr.size() > 0)
	{
		string strAdv,strPrintAdv;
		CJsonObject cJsonAdvData;

		for (int i = 0; i < vctstr.size(); i++)
		{
			cJsonAdvData = cJsonData;

			if (vctstr.at(i).compare(", ") != 0 && vctstr.at(i).empty() == false && vctstr.at(i).length() != 1)
			{
				cJsonAdvData.Add("data", vctstr.at(i));
				cJsonAdvData.Add("type", PRINT_TYPE_TEXT);
				cJson["format"].Add(cJsonAdvData);
			}
		}
	}
	else
	{
		cJsonData.Add("data", str);
		cJsonData.Add("type", PRINT_TYPE_TEXT);
		cJson["format"].Add(cJsonData);
	}

	str = cJson.ToString();
	return str;
}