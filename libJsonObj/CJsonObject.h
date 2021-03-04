/*******************************************************************************
 * Project:  neb
 * @file     CJsonObject.hpp
 * @brief    Json
 * @author   bwarliao
 * @date:    2014-7-16
 * @note
 * Modify history:
 ******************************************************************************/

#ifndef CJSONOBJECT_HPP_
#define CJSONOBJECT_HPP_

#include <stdio.h>
#include <stddef.h>
#include <malloc.h>
#include <errno.h>
#include <limits.h>
#include <math.h>
#include <string>
#include <map>
#include <list>
#ifdef __cplusplus
extern "C" {
#endif
#include "cJSON.h"
#ifdef __cplusplus
}
#endif


class CJsonObject
{
public: // method of ordinary json object or json array
    CJsonObject();
    CJsonObject(const std::string& strJson);		// 通过Json字符串构造
    CJsonObject(const CJsonObject* pJsonObject);	// 通过对象指针构造
    CJsonObject(const CJsonObject& oJsonObject);
    virtual ~CJsonObject();

    CJsonObject& operator=(const CJsonObject& oJsonObject);
    bool operator==(const CJsonObject& oJsonObject) const;

    bool Parse(const std::string& strJson); // 解析json字符串
    void Clear(); // 清除内存
    bool IsEmpty() const; // 判断解析对象是否为空
    bool IsArray() const; // 判断解析对象是否为JSON数组

    std::string ToString() const; // 将解析对象转换为字符串
    std::string ToFormattedString() const;
    const std::string& GetErrMsg() const
    {
        return(m_strErrMsg);
    }

public: // method of ordinary json object
    bool AddEmptySubObject(const std::string& strKey);	// 在当前的原始解析对象上添加空的子对象 "111":{}
    bool AddEmptySubArray(const std::string& strKey);	// 在当前的原始解析对象上添加空的子数组 "data":[]
    bool GetKey(std::string& strKey);
    void ResetTraversing();
    CJsonObject& operator[](const std::string& strKey);
    std::string operator()(const std::string& strKey) const;
    bool Get(const std::string& strKey, CJsonObject& oJsonObject) const;	// 得到与key值相对应的对象键值
    bool Get(const std::string& strKey, std::string& strValue) const;		// 得到与key值相对应的字符串键值

	// 以下的只是根据key值获取不同数据类型的键值
    bool Get(const std::string& strKey, int32& iValue) const;
    bool Get(const std::string& strKey, uint32& uiValue) const;
    bool Get(const std::string& strKey, int64& llValue) const;
    bool Get(const std::string& strKey, uint64& ullValue) const;
    bool Get(const std::string& strKey, bool& bValue) const;
    bool Get(const std::string& strKey, float& fValue) const;
    bool Get(const std::string& strKey, double& dValue) const;

    bool IsNull(const std::string& strKey) const;

	// 在相应的key值上添加不同数据类型的键值
    bool Add(const std::string& strKey, const CJsonObject& oJsonObject);
    bool Add(const std::string& strKey, const std::string& strValue);
    bool Add(const std::string& strKey, int32 iValue);
    bool Add(const std::string& strKey, uint32 uiValue);
    bool Add(const std::string& strKey, int64 llValue);
    bool Add(const std::string& strKey, uint64 ullValue);
    bool Add(const std::string& strKey, bool bValue, bool bValueAgain);
    bool Add(const std::string& strKey, float fValue);
    bool Add(const std::string& strKey, double dValue);

    bool AddNull(const std::string& strKey); // add null like this: "key":null

    bool Delete(const std::string& strKey); // 删除指定的key值

	// 根据key值替代指定的键值
    bool Replace(const std::string& strKey, const CJsonObject& oJsonObject);
    bool Replace(const std::string& strKey, const std::string& strValue);
    bool Replace(const std::string& strKey, int32 iValue);
    bool Replace(const std::string& strKey, uint32 uiValue);
    bool Replace(const std::string& strKey, int64 llValue);
    bool Replace(const std::string& strKey, uint64 ullValue);
    bool Replace(const std::string& strKey, bool bValue, bool bValueAgain);
    bool Replace(const std::string& strKey, float fValue);
    bool Replace(const std::string& strKey, double dValue);

    bool ReplaceWithNull(const std::string& strKey); // replace value with null

public: // method of json array
    int GetArraySize(); // 得到json数组的元素数量
    CJsonObject& operator[](unsigned int uiWhich);
    std::string operator()(unsigned int uiWhich) const;

    bool Get(int iWhich, CJsonObject& oJsonObject) const; // 得到数组iWhich索引的解析对象

	// 根据索引iWhich取不同数据类型的数据
    bool Get(int iWhich, std::string& strValue) const;
    bool Get(int iWhich, int32& iValue) const;
    bool Get(int iWhich, uint32& uiValue) const;
    bool Get(int iWhich, int64& llValue) const;
    bool Get(int iWhich, uint64& ullValue) const;
    bool Get(int iWhich, bool& bValue) const;
    bool Get(int iWhich, float& fValue) const;
    bool Get(int iWhich, double& dValue) const;
    bool IsNull(int iWhich) const;

	// 往数组添加不同数据类型的键值
    bool Add(const CJsonObject& oJsonObject);
    bool Add(const std::string& strValue);
    bool Add(int32 iValue);
    bool Add(uint32 uiValue);
    bool Add(int64 llValue);
    bool Add(uint64 ullValue);
    bool Add(int iAnywhere, bool bValue);
    bool Add(float fValue);
    bool Add(double dValue);
    bool AddNull(); // add a null value

	// 往数组添加不同数据类型的键值作为第一个元素
    bool AddAsFirst(const CJsonObject& oJsonObject);
    bool AddAsFirst(const std::string& strValue);
    bool AddAsFirst(int32 iValue);
    bool AddAsFirst(uint32 uiValue);
    bool AddAsFirst(int64 llValue);
    bool AddAsFirst(uint64 ullValue);
    bool AddAsFirst(int iAnywhere, bool bValue);
    bool AddAsFirst(float fValue);
    bool AddAsFirst(double dValue);
    bool AddNullAsFirst();  // add a null value

    bool Delete(int iWhich); // 删除指定索引iWhich的元素

	// 替代指定索引iWhich的元素
    bool Replace(int iWhich, const CJsonObject& oJsonObject);
    bool Replace(int iWhich, const std::string& strValue);
    bool Replace(int iWhich, int32 iValue);
    bool Replace(int iWhich, uint32 uiValue);
    bool Replace(int iWhich, int64 llValue);
    bool Replace(int iWhich, uint64 ullValue);
    bool Replace(int iWhich, bool bValue, bool bValueAgain);
    bool Replace(int iWhich, float fValue);
    bool Replace(int iWhich, double dValue);
    bool ReplaceWithNull(int iWhich); // replace with a null value

private:
    CJsonObject(cJSON* pJsonData);

private:
    cJSON* m_pJsonData;
    cJSON* m_pExternJsonDataRef;
    cJSON* m_pKeyTravers;
    std::string m_strErrMsg;
    std::map<unsigned int, CJsonObject*> m_mapJsonArrayRef;
    std::map<std::string, CJsonObject*> m_mapJsonObjectRef;
};

#endif /* CJSONHELPER_HPP_ */
