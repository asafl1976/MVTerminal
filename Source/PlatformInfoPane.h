#pragma once

#include "exctrls.h"
#include "PlatformInfoTreeView.h"
#include "PlatformInfoSettingsDlg.h"
#import  "C:\Projects\C# Projects\PlatformInfoLib\PlatformInfoLib\bin\release\PlatformInfoLib.tlb" named_guids raw_interfaces_only
using namespace PlatformInfoLib;

class CPlatformInfoModel;
class CPlatformInfoPresenter;
// CPlatformInfoPane form view

class CPlatformInfoPane : public CFormView, public IPlatformInfoSettingsDlgDelegate
{
	DECLARE_DYNCREATE(CPlatformInfoPane)

	HTREEITEM CPlatformInfoPane::GetMouseClickedItem();

protected:
	CPlatformInfoPane();           // protected constructor used by dynamic creation
	virtual ~CPlatformInfoPane();

	CPlatformInfoTreeView   m_platformTree;
	//CXTTreeCtrl             m_binFilesTree;
	CImageList				m_cImageList;
	BOOL					m_bInitialized;
	HTREEITEM		        m_tiSelectedItem;

public: 
	enum { IDD = IDD_PLATFORM_INFO_PANE };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif 
#endif
	virtual void           HandleMenuSelection(UINT nMenuIndex);
	virtual void           LoadTree();
	virtual BOOL           PreTranslateMessage(MSG* pMsg);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	virtual void		  OnInitialUpdate();

public:
	void RestoreDefaultsClicked();
	afx_msg void OnNMDblclkPlatformInfoTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickPlatformInfoTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedRestorePlatformInfoDefaultsBtn();
};


