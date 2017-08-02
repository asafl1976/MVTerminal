#pragma once

#include "Resource.h"

// CMailSettings dialog

class CMailSettings : public CDialog
{
	DECLARE_DYNAMIC(CMailSettings)

	CString     m_sSender;
	CString     m_sMode;
	CString     m_sMailServer;

	CXTPButton   m_OkBtn;
	CXTPButton   m_CancelBtn;


public:
	CMailSettings(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMailSettings();

// Dialog Data
	enum { IDD = IDD_MAIL_SETTINGS };

	virtual void      	    SetSender(CString sSender);
	virtual void      	    SetMode(CString sMode);
	virtual void      	    SetMailServer(CString sMailServer);

	virtual CString      	GetSender();
	virtual CString      	GetMode();
	virtual CString      	GetMailServer();


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

protected:
public:
	afx_msg void OnBnClickedOk();
};
