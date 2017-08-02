#include "StdAfx.h"
#include "PlatformInfoPresenter.h"
#include "PlatformInfoModel.h"
#include "PlatformInfoView.h"
#include "PlatformInfoCategory.h"

CPlatformInfoPresenter::CPlatformInfoPresenter(CPlatformInfoModel* pModel, CPlatformInfoView* pView)
{
	m_pModel = pModel;
	m_pView  = pView;

	m_pModel->RegisterObserver(this);
	m_pView->RegisterObserver(this);

}

CPlatformInfoPresenter::~CPlatformInfoPresenter(void)
{
}

void  CPlatformInfoPresenter::OnModelAddPlatformInfoCategory(CPlatformInfoCategory *pPlatformInfoCategory)
{
	m_pView->OnAddPlatformInfoCategory(pPlatformInfoCategory);
}
