#pragma once

class CSendMailParams
{	
	
	CString			m_sAttachment;
	CString			m_sMessage;
	CString         m_sRecipient;
	CString         m_sSubject;
	CString         m_sSendDuration;
	BOOL      	    m_bEnSendMail;

public:
	CSendMailParams(void);
	virtual ~CSendMailParams(void);

	CString		 GetAttachment() {return m_sAttachment;}
	void		 SetAttachment(CString sAttachment) {m_sAttachment = sAttachment;}

	CString		 GetMessage() {return m_sMessage;}
	void		 SetMessage(CString sMessage) {m_sMessage = sMessage;}

	CString		 GetRecipient() {return m_sRecipient;}
	void		 SetRecipient(CString sRecipient) {m_sRecipient = sRecipient;}

	CString		 GetSubject() {return m_sSubject;}
	void		 SetSubject(CString sSubject) {m_sSubject = sSubject;}

	CString		 GetSendDuration() {return m_sSendDuration;}
	void		 SetSendDuration(CString sSendDuration) {m_sSendDuration = sSendDuration;}

	BOOL		 GetEnSendMail() {return m_bEnSendMail;}
	void		 SetEnSendMail(BOOL bEnSendMail) {m_bEnSendMail = bEnSendMail;}

};
