#pragma once

#include "ConnectionSettingsDlg.h"
#include "BootLoaderParamsSettingsDlg.h"
#include "SendMailDlg.h"
#include "ViewMessagingDlg.h"
#include ".\BootLoaderParams.h"
#include ".\cmdhistorysettings.h"
#include "ViewPropertiesParams.h"
#include "Resource.h"

#include "afxcmn.h"
#include "afxwin.h"

#define   NEW_SESSION_DLG_TYPE   0
#define   PROPERTIES_DLG_TYPE    1


// CViewPropertiesDlg dialog

class CViewPropertiesDlg : public CDialog
{
	DECLARE_DYNAMIC(CViewPropertiesDlg)

	CConnectionSettingsDlg        m_connectionSettingsTab;
	CBootLoaderParamsSettingsDlg  m_bootLoaderParamsSettingsTab;
	CCmdHistorySettingsDlg        m_cmdHistorySettingsTab;
	CSendMailDlg                  m_sendMailTab;
	CViewMessagingDlg             m_viewMessagingTab;

	CViewPropertiesParams         m_viewPropertiesParams;
	CTabCtrl		              m_tabCtrl;
	CRect						  m_tabItemRect;

	int					          m_nDlgType;

public:
	CViewPropertiesDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CViewPropertiesDlg();

	void						   ShowWindowNumber(int number);
	afx_msg void				   OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void				   OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);

	CViewPropertiesParams          GetViewPropertiesParams()        {return m_viewPropertiesParams;}
	void                           SetViewPropertiesParams(CViewPropertiesParams viewPropertiesParams)  {m_viewPropertiesParams = viewPropertiesParams;}

	CConnectionSettingsDlg       & GetConnectionSettingsTab()       {return m_connectionSettingsTab;}
	CBootLoaderParamsSettingsDlg & GetBootLoaderParamsSettingsTab() {return m_bootLoaderParamsSettingsTab;}
	CCmdHistorySettingsDlg       & GetCmdHistorySettingsTab()       {return m_cmdHistorySettingsTab;}
	CSendMailDlg                 & GetSendMailTab()                 {return m_sendMailTab;}
	CViewMessagingDlg            & GetViewMessagingDlg()            {return m_viewMessagingTab;}

	virtual CString				   Serialize();
	BOOL					       DeSerialize(CMarkup& xmlReader);
	void					       UpdateWindowData();
	CBootLoaderParams              GetBootLoaderParams()             {return m_bootLoaderParamsSettingsTab.GetBootLoaderParams();}
	void		                   SetDlgType(int nType);

// Dialog Data
	enum { IDD = IDD_VIEW_PROPERTIES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	//afx_msg void OnTcnSelchangeViewTab(NMHDR *pNMHDR, LRESULT *pResult);
private:
	CXTPButton m_okBtn;
	CXTPButton m_cancelBtn;

public:
	afx_msg void OnBnClickedOk();
};
