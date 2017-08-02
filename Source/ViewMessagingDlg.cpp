// ViewMessagingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MVTerminal.h"
#include "ViewMessagingDlg.h"
#include ".\viewmessagingdlg.h"
#include "Markup.h"


// CViewMessagingDlg dialog

IMPLEMENT_DYNAMIC(CViewMessagingDlg, CDialog)
CViewMessagingDlg::CViewMessagingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CViewMessagingDlg::IDD)
{
	m_viewMessagingParams.SetMessagingEn(TRUE);
	m_viewMessagingParams.SetAppendNewLine(TRUE);
}

CViewMessagingDlg::~CViewMessagingDlg()
{
}

void CViewMessagingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CViewMessagingDlg, CDialog)
	ON_BN_CLICKED(IDC_EN_MESSAGING_CHECK, OnBnClickedEnMessagingCheck)
	ON_BN_CLICKED(IDC_APPEND_NEWLINE_CHECK, OnBnClickedAppendNewlineCheck)
END_MESSAGE_MAP()


// CViewMessagingDlg message handlers

BOOL CViewMessagingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	UpdateWindowData();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CViewMessagingDlg::OnBnClickedEnMessagingCheck()
{
	m_viewMessagingParams.SetMessagingEn(!m_viewMessagingParams.GetMessagingEn());
	GetDlgItem(IDC_APPEND_NEWLINE_CHECK)->EnableWindow(m_viewMessagingParams.GetMessagingEn());

}

BOOL CViewMessagingDlg::GetMessagingEn()
{
	return m_viewMessagingParams.GetMessagingEn();
}

void CViewMessagingDlg::SetMessagingEn(BOOL bEnable)
{
	m_viewMessagingParams.SetMessagingEn(bEnable);

}

BOOL CViewMessagingDlg::GetAppendNewLine()
{
	return m_viewMessagingParams.GetAppendNewLine();
}

void CViewMessagingDlg::OnBnClickedAppendNewlineCheck()
{
	m_viewMessagingParams.SetAppendNewLine(!m_viewMessagingParams.GetAppendNewLine()); 
}

void CViewMessagingDlg::OnOK()
{
}



CString CViewMessagingDlg::Serialize()
{
	CMarkup xmlWriter;

	xmlWriter.AddElem("WindowMessaging");
	xmlWriter.AddAttrib("MessagingEn",m_viewMessagingParams.GetMessagingEn());
	xmlWriter.AddAttrib("AppendNewLine",m_viewMessagingParams.GetAppendNewLine());

	return xmlWriter.GetDoc();
}

BOOL CViewMessagingDlg::DeSerialize(CMarkup & xmlReader)
{
	if(xmlReader.FindElem("WindowMessaging") == false)
		return FALSE;
	m_viewMessagingParams.SetMessagingEn((xmlReader.GetAttrib("MessagingEn") == "0")? FALSE:TRUE);
	m_viewMessagingParams.SetAppendNewLine((xmlReader.GetAttrib("AppendNewLine") == "0")? FALSE:TRUE);

	return TRUE;
}

void CViewMessagingDlg::UpdateWindowData()
{
	CheckDlgButton(IDC_EN_MESSAGING_CHECK,m_viewMessagingParams.GetMessagingEn());
	CheckDlgButton(IDC_APPEND_NEWLINE_CHECK,m_viewMessagingParams.GetAppendNewLine());
	GetDlgItem(IDC_APPEND_NEWLINE_CHECK)->EnableWindow(m_viewMessagingParams.GetMessagingEn());
}


