#pragma once

#include "afxcmn.h"
#include "resource.h"
#include "afxwin.h"
#include "HwModuleFactory.h"
#include <map>
using namespace std;

#include "HwModuleModel.h"
#include "HwModuleTreeView.h"
#include "HwModulePresenter.h"


// CHwModulesPane form view

class CHwModulesPane : public CFormView
{
	DECLARE_DYNCREATE(CHwModulesPane)

protected:
	CHwModulesPane();           // protected constructor used by dynamic creation
	virtual ~CHwModulesPane();

	CString				   m_sAppDataPath;
	CHwModuleTreeView      m_HwModuleTreeCtrl;
	CHwModuleModel		  *m_pHWMModel;
    CHwModulePresenter    *m_pHWMPresenter;

	HTREEITEM		       m_tiSelectedItem;
	map<int,CString>       m_menuMap;

	virtual void           HandleMenuSelection(UINT nMenuIndex);
	virtual BOOL           PreTranslateMessage(MSG* pMsg);
	virtual CString        GetMenuItem(int nIndex);
public:
	enum { IDD = IDD_HARDWARE_MODULES_PANE };
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	LRESULT				   OnDeviceChange(WPARAM wParam, LPARAM lParam);
	void				   UpdateDevice(PDEV_BROADCAST_DEVICEINTERFACE pDevInf, WPARAM wParam);
protected:
	virtual void           DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	HTREEITEM	           GetMouseClickedItem();

	virtual BOOL		   SaveSettings(CString sFile);
	virtual BOOL		   LoadSettings(CString sFile);

public:

	virtual BOOL	       DeSerialize(CMarkup& xmlReader);
	virtual CString	       Serialize();

	virtual BOOL		   GetDetectHwModulesOnStartup() {return CHwModuleModel::GetInstance().GetDetectHwModulesOnStartup();}
	virtual void		   RemoveAllHwModuleCtrls();
	virtual int		       GetHwModuleCtrlsCount();
	virtual void		   OnInitialUpdate();
	afx_msg void		   OnNMDblclkHwModuleTreeCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void		   OnNMRclickHwModuleTreeCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void		   OnTvnSelchangedHwModuleTreeCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void		   OnBnClickedHwModulePaneDetOnStartup();

	afx_msg void		   OnMenuClickedHwModulePaneRefreshTreeBtn(CHwModuleCtrl *pCtrl);
	afx_msg void		   OnMenuClickedHwModulePaneResetSub20Btn(CHwModuleCtrl *pCtrl);
	afx_msg void		   OnMenuClickedHwModulePaneLoadConTableBtn(CHwModuleCtrl *pCtrl);
	afx_msg void		   OnMenuClickedHwModulePaneSaveConTableBtn(CHwModuleCtrl *pCtrl);
	afx_msg void		   OnMenuClickedHwModulePaneSetI2CFreq(CHwModuleCtrlUnit *pUnit);
};


