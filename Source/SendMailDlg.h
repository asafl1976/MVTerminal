#pragma once


#include "MailSettings.h"
#include "SMTPClientFiles\include\powersmt.hpp"
#include "Markup.h"
#include "Resource.h"
#include "SendMailParams.h"

// CSendMailDlg dialog

class CSendMailDlg : public CDialog, public CPowerSmtp
{
	DECLARE_DYNAMIC(CSendMailDlg)	
	
public:
	CMailSettings   m_mailSettingDlg;
	CXTPButton      m_MailSettingsBtn;
	CXTPButton      m_SendMailBtn;

	CSendMailParams m_sendMailParams;

	CSendMailDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSendMailDlg();

	virtual BOOL			UpdateDataMembers();

	virtual BOOL      	    SendMail(CString sAttachment);
	void					InitControls();
	CString		            Serialize();
	BOOL			        DeSerialize(CMarkup & xmlReader);
	CSendMailParams &       GetMailParams() { return m_sendMailParams;}
	void                    SetMailParams(CSendMailParams sendMailParams) { m_sendMailParams = sendMailParams;}


	virtual CString			GetMailMessage();
	virtual CString			GetMailRecipient();
	virtual CString			GetMailSubject();
	virtual CString			GetSendDuration();
	virtual unsigned int	GetSendDurationInSeconds();
	virtual BOOL      	    GetSendMailEn();

	virtual void      	    SetSendMailEn(BOOL bEnable);
	virtual void      	    SetAttachmentName(CString sAttachment);
	virtual void      	    SetSender(CString sSender);
	virtual void      	    SetMode(CString sMode);
	virtual void      	    SetMailServer(CString sMailServer);

	virtual CString      	GetAttachmentName();
	virtual CString      	GetSender();
	virtual CString      	GetMode();
	virtual CString      	GetMailServer();
	void				    UpdateWindowData();

	BOOL m_bFirstTime;
	BOOL m_bLastTime;

	virtual void SmtpEvent(SMTP_STATUS  nStatus, SMTP_COMMAND nLastCommand, 
						   int nReplyCode, LPCSTR lpszReplyDesc, BOOL bComplete);
	virtual void ExceptionEvent(PT_EXCEPTION nErrorCode, LPCSTR lpszErrorDesc);

	
// Dialog Data
	enum { IDD = IDD_SEND_EMAIL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support	


	DECLARE_MESSAGE_MAP()
public:	
	virtual void OnOK();
	
	afx_msg void OnBnClickedSendMailCheck();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedMailSettings();

protected:
public:
	afx_msg void OnBnClickedSendMail();
};
