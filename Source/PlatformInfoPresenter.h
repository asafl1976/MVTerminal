#pragma once
#include "IPlatformInfoModelObserver.h"
#include "IPlatformInfoViewObserver.h"

class CPlatformInfoModel;
class CPlatformInfoView;
class CPlatformInfoCPU;
class CPlatformInfoCategory;

class CPlatformInfoPresenter: public IPlatformInfoModelObserver, public IPlatformInfoViewObserver
{
	CPlatformInfoModel  *m_pModel;
	CPlatformInfoView   *m_pView;

public:
	CPlatformInfoPresenter(CPlatformInfoModel* pModel, CPlatformInfoView* pView);
	virtual ~CPlatformInfoPresenter(void);

	//View observer
	//virtual void	OnViewAddCPU(CString sFile,CHwModuleCtrl *pCtrl);
	
	//Model observer
	virtual void    OnModelAddPlatformInfoCategory(CPlatformInfoCategory *pPlatformInfoCategory);  

};
