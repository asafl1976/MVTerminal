// SendMailDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MVTerminal.h"
#include "SendMailDlg.h"
#include ".\sendmaildlg.h"
#include "Markup.h"

#define SECURITY_WIN32
#include <windows.h>
#include <security.h>


// CSendMailDlg dialog




IMPLEMENT_DYNAMIC(CSendMailDlg, CDialog)
CSendMailDlg::CSendMailDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSendMailDlg::IDD, pParent), CPowerSmtp(theApp.m_hInstance)
{
	m_sendMailParams.SetEnSendMail(FALSE);
	m_sendMailParams.SetSubject("Status");


}
CSendMailDlg::~CSendMailDlg()
{

}

void CSendMailDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
		
	DDX_Control(pDX, IDC_MAIL_SETTINGS, m_MailSettingsBtn);
	DDX_Control(pDX, IDC_SEND_MAIL, m_SendMailBtn);

}


BEGIN_MESSAGE_MAP(CSendMailDlg, CDialog)
	ON_BN_CLICKED(IDC_SEND_MAIL_CHECK, OnBnClickedSendMailCheck)
	ON_BN_CLICKED(IDC_MAIL_SETTINGS, OnBnClickedMailSettings)
	ON_BN_CLICKED(IDC_SEND_MAIL, OnBnClickedSendMail)
END_MESSAGE_MAP()


// CSendMailDlg message handlers

void CSendMailDlg::OnBnClickedSendMailCheck()
{
	m_sendMailParams.SetEnSendMail(!m_sendMailParams.GetEnSendMail());

	GetDlgItem(IDC_MAIL_RECIPIENT)->EnableWindow(m_sendMailParams.GetEnSendMail());	
	GetDlgItem(IDC_MAIL_SUBJECT)->EnableWindow(m_sendMailParams.GetEnSendMail());	
	GetDlgItem(IDC_MAIL_DURATION_COMBO)->EnableWindow(m_sendMailParams.GetEnSendMail());	
	GetDlgItem(IDC_MAIL_MSG)->EnableWindow(m_sendMailParams.GetEnSendMail());	
	GetDlgItem(IDC_MAIL_SETTINGS)->EnableWindow(m_sendMailParams.GetEnSendMail());
	GetDlgItem(IDC_SEND_MAIL)->EnableWindow(m_sendMailParams.GetEnSendMail());

	InitControls();

}
BOOL    CSendMailDlg::SendMail(CString sAttachment)
{
	if(m_sendMailParams.GetEnSendMail() == FALSE)
		return FALSE;

	m_bFirstTime = TRUE;
	m_bLastTime = FALSE;

	SetAttachmentName(m_sendMailParams.GetAttachment());

	if(m_sendMailParams.GetAttachment().IsEmpty() ||
	   m_sendMailParams.GetRecipient().IsEmpty()  ||
	   m_mailSettingDlg.GetSender().IsEmpty()     ||
	   m_mailSettingDlg.GetMailServer().IsEmpty() ||
	   m_mailSettingDlg.GetMode().IsEmpty()       ||
	   (m_sendMailParams.GetEnSendMail() == FALSE))
	{
		return FALSE;
	}
	
	
	CPowerSmtp::Connect(NULL, PT_NOFLAGS, m_mailSettingDlg.GetMailServer(), 25, NULL, 0);

	return TRUE;
}


CString			CSendMailDlg::GetMailRecipient()
{
	return m_sendMailParams.GetRecipient();
}

CString			CSendMailDlg::GetMailSubject()
{
	return m_sendMailParams.GetSubject();
}

CString			CSendMailDlg::GetSendDuration()
{
	return m_sendMailParams.GetSendDuration();
}

unsigned int	CSendMailDlg::GetSendDurationInSeconds()
{
	
	if(m_sendMailParams.GetSendDuration() == "10 sec")
		return 10;

	if(m_sendMailParams.GetSendDuration() == "30 min")
		return 1800;
	if(m_sendMailParams.GetSendDuration() == "1 hour")
		return 3600;
	if(m_sendMailParams.GetSendDuration() == "2 hours")
		return 7200;
	if(m_sendMailParams.GetSendDuration() == "3 hours")
		return 10800;
	if(m_sendMailParams.GetSendDuration() == "4 hours")
		return 14400;
	if(m_sendMailParams.GetSendDuration() == "8 hours")
		return 28800;

	if(m_sendMailParams.GetSendDuration() == "1 day")
		return 86400;
	if(m_sendMailParams.GetSendDuration() == "2 days")
		return 172800;
	if(m_sendMailParams.GetSendDuration() == "3 days")
		return 259200;
	if(m_sendMailParams.GetSendDuration() == "4 days")
		return 345600;

	return 0;
}


BOOL      	    CSendMailDlg::GetSendMailEn()
{
	return m_sendMailParams.GetEnSendMail();
}
void      	    CSendMailDlg::SetSendMailEn(BOOL bEnable)
{
	m_sendMailParams.SetEnSendMail(bEnable);
}
void     CSendMailDlg::InitControls()
{
	SetDlgItemText(IDC_MAIL_RECIPIENT,NULL);
	SetDlgItemText(IDC_MAIL_SUBJECT,NULL);
	SetDlgItemText(IDC_MAIL_MSG,NULL);
	CheckDlgButton(IDC_SEND_MAIL_CHECK,FALSE);
	
	if(m_sendMailParams.GetSendDuration().IsEmpty() == TRUE)
		((CComboBox*)GetDlgItem(IDC_MAIL_DURATION_COMBO))->SelectString(-1,"2 Hours");

	if(m_sendMailParams.GetEnSendMail())
	{
		SetDlgItemText(IDC_MAIL_RECIPIENT,m_sendMailParams.GetRecipient());
		SetDlgItemText(IDC_MAIL_SUBJECT,m_sendMailParams.GetSubject());
		SetDlgItemText(IDC_MAIL_MSG,m_sendMailParams.GetMessage());	
		((CComboBox*)GetDlgItem(IDC_MAIL_DURATION_COMBO))->SelectString(-1,m_sendMailParams.GetSendDuration());
		CheckDlgButton(IDC_SEND_MAIL_CHECK,TRUE);
	}
}




BOOL CSendMailDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	GetDlgItem(IDC_MAIL_RECIPIENT)->EnableWindow(m_sendMailParams.GetEnSendMail());	
	GetDlgItem(IDC_MAIL_SUBJECT)->EnableWindow(m_sendMailParams.GetEnSendMail());	
	GetDlgItem(IDC_MAIL_DURATION_COMBO)->EnableWindow(m_sendMailParams.GetEnSendMail());	
	GetDlgItem(IDC_MAIL_MSG)->EnableWindow(m_sendMailParams.GetEnSendMail());
	GetDlgItem(IDC_MAIL_SETTINGS)->EnableWindow(m_sendMailParams.GetEnSendMail());
	GetDlgItem(IDC_SEND_MAIL)->EnableWindow(m_sendMailParams.GetEnSendMail());

	InitControls();

	m_MailSettingsBtn.SetTheme(xtpButtonThemeOffice2000);
	m_SendMailBtn.SetTheme(xtpButtonThemeOffice2000);

	TCHAR                buff[1024];
    unsigned long        buffSize = 1024;
    EXTENDED_NAME_FORMAT namefmt = NameUserPrincipal;
	::GetUserNameEx(namefmt, buff, &buffSize);
	m_sendMailParams.SetRecipient( buff);
	GetDlgItem(IDC_SEND_MAIL)->ShowWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSendMailDlg::OnBnClickedMailSettings()
{
	m_mailSettingDlg.DoModal();
}

CString  CSendMailDlg::GetAttachmentName()
{
	return m_sendMailParams.GetAttachment();
}

void     CSendMailDlg::SetAttachmentName(CString sAttachment)
{
	m_sendMailParams.SetAttachment(sAttachment);
}

void      CSendMailDlg::SetSender(CString sSender)
{
	m_mailSettingDlg.SetSender(sSender);
}
void      CSendMailDlg::SetMode(CString sMode)
{
	m_mailSettingDlg.SetMode(sMode);
}
void      CSendMailDlg::SetMailServer(CString sMailServer)
{
	m_mailSettingDlg.SetMailServer(sMailServer);
}

CString   CSendMailDlg::GetSender()
{
	return m_mailSettingDlg.GetSender();
}
CString   CSendMailDlg::GetMode()
{
	return m_mailSettingDlg.GetMode();
}
CString   CSendMailDlg::GetMailServer()
{
	return m_mailSettingDlg.GetMailServer();
}

CString	  CSendMailDlg::GetMailMessage()
{
	return m_sendMailParams.GetMessage();
}


void CSendMailDlg::SmtpEvent(SMTP_STATUS  nStatus, SMTP_COMMAND nLastCommand, 
							 int nReplyCode, LPCSTR lpszReplyDesc, BOOL bComplete) 
{ 
	switch (nStatus)
	{
        //
		// Notify the user of a closed mail connection
		//
		case SMTP_CLOSED:
			//m_edtLog.ReplaceSel("SMTP mail connection closed.");
			break;
		//
		// Oops! we have an error.  Most likely an RCPT (receiptient) is incorrect.
		// better trap it and close our connection.			
		//
		case SMTP_ERROR:
		case SMTP_FAILURE:
			::MessageBox(NULL, lpszReplyDesc, "SMTP ERROR", MB_OK + MB_ICONSTOP);
			CPowerSmtp::Close(FALSE);
			break;
		//
		// check for a status of either SUCCESS or WORKING in order to determine if it is OK for us to perform our next
		// logical step in sending a mail message.
		//
		case SMTP_SUCCESS:
		case SMTP_WORKING:
			switch (nLastCommand)
			{
				//
				// close the connection once we know the message has been completely sent
				//
				case SMTP_DATA:
		    		if (nStatus == SMTP_SUCCESS)
			    		CPowerSmtp::Close(FALSE);
					return;
				//
				// once we receive the greeting from the SMTP server, let actually send the mail message
				//
				case SMTP_HELO:
				{ 		
					CString strModes;

					strModes = GetMode().GetAt(0);					
			
					CPowerSmtp::Mail(SMTP_MAIL_TYPE, m_mailSettingDlg.GetSender(), 
									 GetMailRecipient(), TRUE, TRUE,
									 GetMailSubject(), 
									 TRUE, (LPVOID)GetMailMessage().GetBuffer(),
									 GetMailMessage().GetLength(), m_sendMailParams.GetAttachment(), 
									 strModes);

				}
		}
	}
}
void CSendMailDlg::ExceptionEvent(PT_EXCEPTION nErrorCode, LPCSTR lpszErrorDesc)
{	
 	
}

BOOL CSendMailDlg::UpdateDataMembers()
{
	CString sSubject, sRecipient,sMessage,sSendDuration;

	GetDlgItemText(IDC_MAIL_RECIPIENT,sRecipient);
	if((sRecipient.Find('@') == -1) || (sRecipient.Find('.') == -1))
	{
		MessageBox("Invalid recipient address!",NULL,MB_ICONERROR);
		return FALSE;
	}
	m_sendMailParams.SetRecipient(sRecipient);

	GetDlgItemText(IDC_MAIL_SUBJECT,sSubject);
	if(sSubject.IsEmpty() == TRUE)
	{
		if(MessageBox("Message contains no subject. Continue anyway?",NULL,MB_YESNO|MB_ICONQUESTION) == IDNO)
		{
			return FALSE;
		}
	}
	m_sendMailParams.SetRecipient(sSubject);

	GetDlgItemText(IDC_MAIL_MSG,sMessage);
	m_sendMailParams.SetMessage(sMessage);
	GetDlgItemText(IDC_MAIL_DURATION_COMBO,sSendDuration);
	if(sSendDuration.IsEmpty())
	{
		MessageBox("Must select send duration!",NULL,MB_ICONERROR);
		return FALSE;
	}
	m_sendMailParams.SetSendDuration(sSendDuration);
	return TRUE;
}



void CSendMailDlg::OnOK()
{
	if(m_sendMailParams.GetEnSendMail())
	{
		if(UpdateDataMembers() == FALSE)
		{
			MessageBox("Update data members failed!",NULL,MB_ICONERROR);
			return ;
		}
	}

	__super::OnOK();
}

void CSendMailDlg::OnBnClickedSendMail()
{
	if(UpdateDataMembers() == FALSE)
		return;

	SendMail(m_sendMailParams.GetAttachment());
}

CString CSendMailDlg::Serialize()
{
	CMarkup xmlWriter;

	xmlWriter.AddElem("SendMail");
	xmlWriter.AddAttrib("EnSendMail",m_sendMailParams.GetEnSendMail());
	xmlWriter.AddAttrib("Attachment",m_sendMailParams.GetAttachment());
	xmlWriter.AddAttrib("Message",m_sendMailParams.GetMessage());
	xmlWriter.AddAttrib("Recipient",m_sendMailParams.GetRecipient());
	xmlWriter.AddAttrib("Subject",m_sendMailParams.GetSubject());
	xmlWriter.AddAttrib("SendDuration",m_sendMailParams.GetSendDuration());

	return xmlWriter.GetDoc();
}

BOOL CSendMailDlg::DeSerialize(CMarkup & xmlReader)
{
	if(xmlReader.FindElem("SendMail") == false)
		return FALSE;

	m_sendMailParams.SetEnSendMail((xmlReader.GetAttrib("EnSendMail")=="0")? FALSE:TRUE);
	m_sendMailParams.SetAttachment( xmlReader.GetAttrib("Attachment"));
	m_sendMailParams.SetMessage(xmlReader.GetAttrib("Message"));
	m_sendMailParams.SetRecipient(xmlReader.GetAttrib("Recipient"));
	m_sendMailParams.SetSubject(xmlReader.GetAttrib("Subject"));
	m_sendMailParams.SetSendDuration(xmlReader.GetAttrib("SendDuration"));

	return TRUE;
}

void CSendMailDlg::UpdateWindowData()
{
	GetDlgItem(IDC_MAIL_RECIPIENT)->EnableWindow(m_sendMailParams.GetEnSendMail());	
	GetDlgItem(IDC_MAIL_SUBJECT)->EnableWindow(m_sendMailParams.GetEnSendMail());	
	GetDlgItem(IDC_MAIL_DURATION_COMBO)->EnableWindow(m_sendMailParams.GetEnSendMail());	
	GetDlgItem(IDC_MAIL_MSG)->EnableWindow(m_sendMailParams.GetEnSendMail());
	GetDlgItem(IDC_MAIL_SETTINGS)->EnableWindow(m_sendMailParams.GetEnSendMail());
	GetDlgItem(IDC_SEND_MAIL)->EnableWindow(m_sendMailParams.GetEnSendMail());

	if(m_sendMailParams.GetEnSendMail())
	{
		
		SetDlgItemText(IDC_MAIL_RECIPIENT,m_sendMailParams.GetRecipient());
		SetDlgItemText(IDC_MAIL_SUBJECT,m_sendMailParams.GetSubject());
		SetDlgItemText(IDC_MAIL_MSG,m_sendMailParams.GetMessage());	
		((CComboBox*)GetDlgItem(IDC_MAIL_DURATION_COMBO))->SelectString(-1,m_sendMailParams.GetSendDuration());
		CheckDlgButton(IDC_SEND_MAIL_CHECK,TRUE);
	}
}



