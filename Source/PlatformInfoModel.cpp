#include "StdAfx.h"
#include "PlatformInfoModel.h"
#include "PlatformInfoCategory.h"
#include "MVTerminal.h"

CPlatformInfoModel::CPlatformInfoModel(void)
{
}


CPlatformInfoModel::~CPlatformInfoModel(void)
{
	ReleaseAllPlatformInfoCategories();
}

CPlatformInfoModel& CPlatformInfoModel::GetInstance()
{
    /* Allocate system object once on the heap */
    static CPlatformInfoModel *pObj = new CPlatformInfoModel();
    return *pObj;
}


void CPlatformInfoModel::AddPlatformInfoCategory(CPlatformInfoCategory *pPlatformInfoCategory)
{
	m_platformInfoCategoryVec.push_back(pPlatformInfoCategory);
	NotifyAddPlatformInfoCategory(pPlatformInfoCategory);
}

void   CPlatformInfoModel::ReleaseAllPlatformInfoCategories()
{
	vector<CPlatformInfoCategory*>::iterator iter,endIter;
	iter    = m_platformInfoCategoryVec.begin();
	endIter = m_platformInfoCategoryVec.end();
	for(;iter != endIter;iter++)
	{
		if((*iter) != NULL)
		{
			delete (*iter);
		}
	}
	m_platformInfoCategoryVec.clear();
}

void CPlatformInfoModel::DeSerialize(CString sPlatformInfoCategoriesPath)
{
	//Search xml files and add them as CPU roots
	CFileFind    xmlFileFind;
	CStdioFile   file;

	// Look for *.xpf (xml platform file) files
	if(xmlFileFind.FindFile(sPlatformInfoCategoriesPath+"\\*.xml") == TRUE)
	{
		xmlFileFind.FindNextFile();

		CPlatformInfoCategory *pPlatformInfoCategory = new CPlatformInfoCategory();
		if(pPlatformInfoCategory->DeSerialize((LPCTSTR)xmlFileFind.GetFilePath()) == TRUE)
		{
			AddPlatformInfoCategory(pPlatformInfoCategory);
		}

        while(xmlFileFind.FindNextFile() == TRUE)
		{
			CPlatformInfoCategory *pPlatformInfoCategory = new CPlatformInfoCategory();
			if(pPlatformInfoCategory->DeSerialize((LPCTSTR)xmlFileFind.GetFilePath()) == TRUE)
			{
				AddPlatformInfoCategory(pPlatformInfoCategory);
			}
		}
	}
	xmlFileFind.Close();
}

void CPlatformInfoModel::Serialize()
{

}

