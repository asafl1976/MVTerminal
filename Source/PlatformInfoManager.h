#pragma once
#include <string>
using namespace std;

#import  "C:\Projects\C# Projects\PlatformInfoLib\PlatformInfoLib\bin\release\PlatformInfoLib.tlb" named_guids raw_interfaces_only
using namespace PlatformInfoLib;

class CPlatformInfoManager
{
	BOOL                m_bInitialized;
	CString             m_sPlatformInfoPath;

	CPlatformInfoManager(void);
	IPlatformInfoLibPtr m_pModel;
public:
	static CPlatformInfoManager& GetInstance();
	~CPlatformInfoManager(void);

	virtual void Reset();
	virtual BOOL ReloadData();
	virtual BOOL Init(CString sPlatformInfoPath);
	virtual BOOL IsSubitemFiltered(IPlatformInfoItem* pItem);
	virtual IPlatformInfoItem* GetCategoryAtIndex(int nIndex);
	virtual IPlatformInfoItem* GetCategoryWithDisplayName(CString sDisplayName);
	virtual int GetCategoryCount();
	virtual string GetItemAttributeValue(IPlatformInfoItem* pItem, string attrName);
	virtual string GetItemAttributeName(IPlatformInfoItemAttribute* pAttribute);
	virtual string GetItemAttributeType(IPlatformInfoItemAttribute* pAttribute);
	virtual int    GetItemAttributeValueCount(IPlatformInfoItemAttribute* pAttribute);
	virtual string GetItemAttributeValueAtIndex(IPlatformInfoItemAttribute* pAttribute, int nIndex);
	virtual string GetItemAttributeDescription(IPlatformInfoItemAttribute* pAttribute);

	virtual string   GetPlaformInfoXmlRepresentation(string sPlatformInfoName);
	virtual void     SavePlaformInfoXmlRepresentation(string sPlatformInfoName);
	virtual string   GetPlaformInfoXmlFilePath(string sPlatformInfoName);
	virtual BOOL     SerializePlaformInfoToFile(string sPlatformInfoName, string sFile);

};
