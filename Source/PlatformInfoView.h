#pragma once

//#include "IPlatformInfoViewSubject.h"

class CPlatformInfo;
class CPlatformInfoCategory;

class CPlatformInfoView: public IPlatformInfoViewSubject
{
public:
	CPlatformInfoView(void);
	virtual ~CPlatformInfoView(void);

	//Triggered by presenter
	virtual void OnAddPlatformInfoCategory(CPlatformInfoCategory *pPlatformInfoCategory) = 0;

};
