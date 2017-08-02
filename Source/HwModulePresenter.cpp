#include "StdAfx.h"
#include "HwModulePresenter.h"
#include "HwModuleTreeView.h"
#include "HwModuleModel.h"

CHwModulePresenter::CHwModulePresenter(CHwModuleModel *pModel, CHwModuleTreeView* pView)
{
	m_pHWMModel = pModel;
	m_pHWMView  = pView;

	m_pHWMModel->RegisterObserver(this);
	m_pHWMView->RegisterObserver(this);
}

CHwModulePresenter::~CHwModulePresenter(void)
{

}
	
BOOL  CHwModulePresenter::OnViewSetHwModuleCtrlUnitItemDisplayName(CHwModuleCtrlUnitItem* pItem,CString sDisplayName)
{
	return m_pHWMModel->SetHwModuleCtrlUnitItemDisplayName(pItem,sDisplayName);
}

BOOL  CHwModulePresenter::OnViewRefreshHwModuleCtrl(CHwModuleCtrl *pCtrl)
{
	return m_pHWMModel->RefreshHwModuleCtrl(pCtrl);
}

BOOL  CHwModulePresenter::OnViewResetHwModuleCtrl(CHwModuleCtrl *pCtrl)
{
	return m_pHWMModel->ResetHwModuleCtrl(pCtrl);
}

BOOL  CHwModulePresenter::OnViewSetHwModuleDisplayName(CHwModule *pHwModule, CString sDisplayName)
{
	return m_pHWMModel->SetHwModuleDisplayName(pHwModule,sDisplayName);
}

BOOL  CHwModulePresenter::OnViewLoadHwModuleModelSettings(CString sFile,CHwModuleCtrl *pCtrl)
{
	return m_pHWMModel->LoadHwModuleModelSettings(sFile,pCtrl);
}

BOOL  CHwModulePresenter::OnViewSaveHwModuleModelSettings(CString sFile,CHwModuleCtrl *pCtrl)
{
	return m_pHWMModel->SaveHwModuleModelSettings(sFile,pCtrl);
}


BOOL  CHwModulePresenter::OnViewReplaceHwModule(CHwModule *pHwModule, CString sNewHwModuleName)
{
	return m_pHWMModel->ReplaceHwModule(pHwModule,sNewHwModuleName);
}

BOOL  CHwModulePresenter::OnViewRemoveHwModule(CHwModule *pObj)
{
	return m_pHWMModel->RemoveHwModule(pObj);
}
BOOL  CHwModulePresenter::OnViewSelectHwModule(CHwModule *pObj)
{
	return m_pHWMModel->SelectHwModule(pObj);
}
BOOL  CHwModulePresenter::OnViewDeSelectHwModule(CHwModule *pObj)
{
	return m_pHWMModel->DeSelectHwModule(pObj);
}

BOOL  CHwModulePresenter::OnViewAddUsbDeviceMsg(CString sDevID)
{
	return m_pHWMModel->AddUsbDevice(sDevID);
}

BOOL  CHwModulePresenter::OnViewRemoveUsbDeviceMsg(CString sDevID)
{
	return m_pHWMModel->RemoveUsbDevice(sDevID);
}

void CHwModulePresenter::OnModelCreateHwModuleItem(CHwModuleCtrlUnitItem *pObj, CHwModuleItem *pParentObj)
{
	m_pHWMView->OnAddHwModuleItem(pObj, pParentObj);
}

void CHwModulePresenter::OnModelCreateHwModuleItem(CHwModule *pObj, CHwModuleItem *pParentObj)
{
	m_pHWMView->OnAddHwModuleItem(pObj, pParentObj);
}
void CHwModulePresenter::OnModelCreateHwModuleItem(CHwModuleCtrl *pObj, CHwModuleItem *pParentObj)
{
	m_pHWMView->OnAddHwModuleItem(pObj, pParentObj);
}

void CHwModulePresenter::OnModelCreateHwModuleItem(CHwModuleCtrlUnit *pObj, CHwModuleItem *pParentObj)
{
	m_pHWMView->OnAddHwModuleItem(pObj, pParentObj);
}

BOOL CHwModulePresenter::OnModelReplaceHwModule(CHwModule *pObj, CHwModule *pNewObj)
{
	return m_pHWMView->OnReplaceHwModule(pObj,pNewObj);
}

BOOL CHwModulePresenter::OnModelRemoveHwModule(CHwModule *pObj)
{
	return m_pHWMView->OnRemoveHwModule(pObj);
}

BOOL CHwModulePresenter::OnModelHwModuleCtrlUnitItemDisplayNameChange(CHwModuleCtrlUnitItem *pObj)
{
	return m_pHWMView->OnHwModuleCtrlUnitItemDisplayNameChange(pObj);
}

BOOL CHwModulePresenter::OnModelHwModuleDisplayNameChange(CHwModule *pObj)
{
	return m_pHWMView->OnHwModuleDisplayNameChange(pObj);
}

BOOL CHwModulePresenter::OnModelRemoveHwModuleCtrl(CHwModuleCtrl *pObj)
{
	return m_pHWMView->OnRemoveHwModuleCtrl(pObj);
}

BOOL CHwModulePresenter::OnModelSelectHwModule(CHwModule *pObj)
{
	return m_pHWMView->OnSelectHwModule(pObj);
}
BOOL CHwModulePresenter::OnModelDeSelectHwModule(CHwModule *pObj)
{
	return m_pHWMView->OnDeSelectHwModule(pObj);
}



