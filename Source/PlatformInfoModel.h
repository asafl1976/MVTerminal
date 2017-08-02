#pragma once

#include "IPlatformInfoModelSubject.h"
#include <vector>
using namespace std;

class CPlatformInfoCategory;

class CPlatformInfoModel: public IPlatformInfoModelSubject
{
	vector<CPlatformInfoCategory*>	 m_platformInfoCategoryVec;

	CPlatformInfoModel(void);
	void    ReleaseAllPlatformInfoCategories();
public:
	static  CPlatformInfoModel&  GetInstance();
	virtual ~CPlatformInfoModel(void);

	virtual void DeSerialize(CString sPlatformInfoCategoriesPath);
	virtual void Serialize();

	virtual void AddPlatformInfoCategory(CPlatformInfoCategory *pPlatformInfoCategory);
};
