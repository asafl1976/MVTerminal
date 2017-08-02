#include "StdAfx.h"
#include "PlatformInfoManager.h"

CPlatformInfoManager::CPlatformInfoManager(void)
{
	m_bInitialized = FALSE;
}

CPlatformInfoManager::~CPlatformInfoManager(void)
{
}

CPlatformInfoManager& CPlatformInfoManager::GetInstance()
{
    /* Allocate system object once on the heap */
    static CPlatformInfoManager *pObj = new CPlatformInfoManager();
    return *pObj; 
}

void CPlatformInfoManager::Reset()
{
	m_bInitialized = FALSE;
}

BOOL CPlatformInfoManager::ReloadData()
{
	Reset();
	return Init(m_sPlatformInfoPath);
}

BOOL CPlatformInfoManager::Init(CString sPlatformInfoPath)
{
	if(m_bInitialized == FALSE)
	{
		m_sPlatformInfoPath = sPlatformInfoPath;
		VARIANT_BOOL retVal;

		//Init and load platformInfoLib dll
		HRESULT hr = CoInitialize(NULL);
	
		IPlatformInfoLibPtr pPlatformInfo(__uuidof(PlatformInfoModel));
		m_pModel = pPlatformInfo;

		CoUninitialize();

		_bstr_t _bstr( sPlatformInfoPath );
		BSTR bstr = _bstr.copy();
		
		m_pModel->LoadPlatformInfoFiles(bstr,&retVal);
		::SysFreeString( bstr );

		m_bInitialized = TRUE;
	}

	return TRUE;
}

int CPlatformInfoManager::GetCategoryCount()
{
	long categoryCount = 0;
	m_pModel->GetCategoryCount(&categoryCount);

	return (int)categoryCount;
}

IPlatformInfoItem* CPlatformInfoManager::GetCategoryWithDisplayName(CString sDisplayName)
{
	int categoryCount = GetCategoryCount();

	for(long category = 0; category < categoryCount; category++)
	{
		IPlatformInfoItem* pCategory;
		m_pModel->GetCategoryAtIndex(category,&pCategory);

		string sDisName;
		BSTR  bstrDisplayName = NULL;
		pCategory->GetDisplayName(&bstrDisplayName); 
		if(bstrDisplayName != NULL)
		{
			_bstr_t displayName(bstrDisplayName, FALSE);   
			sDisName = displayName;
			::SysFreeString( displayName );
		}

		if(sDisName.c_str() == sDisName)
		{
			return pCategory;
		}		
	}
	return NULL;
}

IPlatformInfoItem* CPlatformInfoManager::GetCategoryAtIndex(int nIndex)
{
	int categoryCount = GetCategoryCount();

	for(long category = 0; category < categoryCount; category++)
	{
		if(nIndex == (int)category)
		{
			IPlatformInfoItem* pCategory;
			m_pModel->GetCategoryAtIndex(category,&pCategory);
			return pCategory;
		}
	}
	return NULL;
}

string CPlatformInfoManager::GetItemAttributeDescription(IPlatformInfoItemAttribute* pAttribute)
{
	string sDesc;
	BSTR  bstrDesc = NULL;
	pAttribute->GetDescription(&bstrDesc); 
	if(bstrDesc != NULL)
	{
		_bstr_t attributeDesc(bstrDesc, FALSE);   
		sDesc = attributeDesc;
		::SysFreeString( bstrDesc );
	}

	return sDesc;
}

string CPlatformInfoManager::GetItemAttributeName(IPlatformInfoItemAttribute* pAttribute)
{
	BSTR  bstrName = NULL;
	pAttribute->GetName(&bstrName); 
	_bstr_t attributeName(bstrName, FALSE);  
	string sName = attributeName;
	::SysFreeString( bstrName );

	return sName;
}

BOOL CPlatformInfoManager::IsSubitemFiltered(IPlatformInfoItem* pItem)
{
	VARIANT_BOOL retVal;
	m_pModel->IsSubitemFiltered(pItem,&retVal);
	if(retVal == 0)
		return FALSE;
	return TRUE;
}

string CPlatformInfoManager::GetItemAttributeValue(IPlatformInfoItem* pItem, string sAttributeName)
{
	IPlatformInfoItemAttribute* pAttribute;

	_bstr_t _bstr( sAttributeName.c_str() );
	BSTR bstr = _bstr.copy();
	pItem->GetAttributeWithName(bstr,&pAttribute); 
	::SysFreeString( bstr );

	BSTR  bstrValue = NULL;
	pAttribute->GetValue(&bstrValue); 
	_bstr_t attributeValue(bstrValue, FALSE);  
	string sValue = attributeValue;
	::SysFreeString( bstrValue );

	return sValue;
}

string CPlatformInfoManager::GetItemAttributeType(IPlatformInfoItemAttribute* pAttribute)
{
	BSTR  bstrType = NULL;
	pAttribute->GetFieldType(&bstrType); 
	_bstr_t attributeType(bstrType, FALSE);  
	string sType = attributeType;
	::SysFreeString( bstrType );
	return sType;
}

int CPlatformInfoManager::GetItemAttributeValueCount(IPlatformInfoItemAttribute* pAttribute)
{
	long nValuesCount = 0;
	pAttribute->GetValuesCount(&nValuesCount);
	return nValuesCount;
}

string CPlatformInfoManager::GetItemAttributeValueAtIndex(IPlatformInfoItemAttribute* pAttribute, int nIndex)
{
	BSTR  bstrValue = NULL;
	pAttribute->GetValueAtIndex(nIndex,&bstrValue); 
	_bstr_t attributeValue(bstrValue, FALSE);   
	string sValue = attributeValue;
	::SysFreeString( bstrValue );

	return sValue;
}

BOOL   CPlatformInfoManager::SerializePlaformInfoToFile(string sPlatformInfoName, string sFile)
{
	BSTR  bstrXmlData = NULL;

	_bstr_t _bstr( sPlatformInfoName.c_str() );
	BSTR bstr = _bstr.copy();
	m_pModel->GetPlatformInfoXmlRepresentation(bstr,&bstrXmlData);
	_bstr_t xmlData(bstrXmlData, FALSE);   
	string sXmlData = xmlData;
	::SysFreeString( bstrXmlData );

	CStdioFile  file;
	if(file.Open(sFile.c_str(),CFile::typeText | CFile::modeCreate|CFile::modeWrite) == FALSE)
	{
		AfxMessageBox(("Failed to open file " + sFile).c_str(),NULL,MB_ICONERROR);
		return FALSE;
	}

	file.WriteString( sXmlData.c_str() );
	file.Close();

	return TRUE;
}

string   CPlatformInfoManager::GetPlaformInfoXmlRepresentation(string sPlatformInfoName)
{
	BSTR  bstrXmlData = NULL;
	string sXmlData;

	_bstr_t _bstr( sPlatformInfoName.c_str() );
	BSTR bstr = _bstr.copy();
	m_pModel->GetPlatformInfoXmlRepresentation(bstr,&bstrXmlData);
	if(bstrXmlData != NULL)
	{
		_bstr_t xmlData(bstrXmlData, FALSE);   
		sXmlData = xmlData;
		::SysFreeString( bstrXmlData );
	}

	return sXmlData;
}


void   CPlatformInfoManager::SavePlaformInfoXmlRepresentation(string sPlatformInfoName)
{
	_bstr_t _bstr( sPlatformInfoName.c_str() );
	BSTR bstr = _bstr.copy();
	m_pModel->SavePlatformInfoXmlRepresentation(bstr);
}

string   CPlatformInfoManager::GetPlaformInfoXmlFilePath(string sPlatformInfoName)
{
	BSTR  bstrXmlFilePath = NULL;
	string sXmlFilePath;

	_bstr_t _bstr( sPlatformInfoName.c_str() );
	BSTR bstr = _bstr.copy();
	m_pModel->GetPlatformInfoXmlFilePath(bstr,&bstrXmlFilePath);

	if(bstrXmlFilePath != NULL)
	{
		_bstr_t xmlFilePath(bstrXmlFilePath, FALSE);   
		sXmlFilePath = xmlFilePath;
		::SysFreeString( bstrXmlFilePath );
	}
	return sXmlFilePath;

}





