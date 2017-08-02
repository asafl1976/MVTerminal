#pragma once

#include "Markup.h"
#include "Resource.h"

#include "ViewMessagingParams.h"

// CViewMessagingDlg dialog

class CViewMessagingDlg : public CDialog
{
	DECLARE_DYNAMIC(CViewMessagingDlg)

	CViewMessagingParams  m_viewMessagingParams;

public:
	CViewMessagingDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CViewMessagingDlg();

// Dialog Data
	enum { IDD = IDD_VIEW_MESSAGING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void		 UpdateWindowData();
	BOOL	     GetMessagingEn();
	void	     SetMessagingEn(BOOL bEnable);
	BOOL	     GetHideMessaging();
	BOOL	     GetAppendNewLine();

	CViewMessagingParams &  GetViewMessagingParams() { return m_viewMessagingParams;}
	void                    SetViewMessagingParams(CViewMessagingParams viewMessagingParams) { m_viewMessagingParams = viewMessagingParams;}

	CString		 Serialize();
	BOOL		 DeSerialize(CMarkup & xmlReader);

	afx_msg void OnBnClickedEnMessagingCheck();
	afx_msg void OnBnClickedAppendNewlineCheck();
	void		 OnOK();

};
