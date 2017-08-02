// CECheckResultDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MVTerminal.h"
#include "CmdExeCmdItemCheckResultDlg.h"


// CCmdExeCmdItemCheckResultDlg dialog

CCmdExeCmdItemCheckResultDlg::CCmdExeCmdItemCheckResultDlg(CWnd* pParent /*=NULL*/)
: CBasicItemDlg(CCmdExeCmdItemCheckResultDlg::IDD, pParent)
{
}
CCmdExeCmdItemCheckResultDlg::~CCmdExeCmdItemCheckResultDlg()
{
}

void CCmdExeCmdItemCheckResultDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_okBtn);
	DDX_Control(pDX, IDCANCEL, m_cancelBtn);
	DDX_Control(pDX, IDC_CE_CHECK_RESULT_BROWSE_BTN, m_cmpFileBrowseBtn);
	DDX_Control(pDX, IDC_TARGET_WIN_COMBO, m_targetWinCombo);
}


BEGIN_MESSAGE_MAP(CCmdExeCmdItemCheckResultDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_CE_COMPARE_STR_RADIO, OnBnClickedCeCompareStrRadio)
	ON_BN_CLICKED(IDC_COMPARE_TEXT_FILE_RADIO, OnBnClickedCompareTextFileRadio)
	ON_BN_CLICKED(IDC_CE_CHECK_RESULT_BROWSE_BTN, OnBnClickedCeCheckResultBrowseBtn)
	ON_BN_CLICKED(IDC_CE_CHECK_RES_FAIL_IF_FOUND_CHECK, OnBnClickedCeCheckResFailIfFoundCheck)
END_MESSAGE_MAP()


// CCmdExeCmdItemCheckResultDlg message handlers

void CCmdExeCmdItemCheckResultDlg::OnBnClickedOk()
{
	GetDlgItemText(IDC_CE_CHECK_RES_EDIT,m_params.m_sStr);
	if(m_params.m_sStr.IsEmpty() == TRUE)
	{
		MessageBox("Please enter a valid string!",NULL,MB_ICONERROR);
		return;
	}
	GetDlgItemText(IDC_TARGET_WIN_COMBO,m_params.m_sComPort);
	m_pItem->SetParams(GetParams());


	CString sDisplayName;
	switch(m_params.m_type)
	{
	case CE_CHECK_RESULT_COMPARE_STRING_TYPE:
		if(m_params.m_bFailIfFound)
			sDisplayName = "Check Result: Fail if \"" +m_params.m_sStr+"\" is found";
		else
			sDisplayName = "Check Result: Pass if \"" +m_params.m_sStr+"\" is found";
		break;
	case CE_CHECK_RESULT_COMPARE_FILE_TYPE:
		sDisplayName = "Check result: Compare \"" +m_params.m_sStr+"\" with window content";
		break;
	default:
		break;
	}
	sDisplayName += " (" + m_params.m_sComPort + ")";

	m_pItem->SetDisplayName(sDisplayName);

	OnOK();
}

void CCmdExeCmdItemCheckResultDlg::OnBnClickedCeCompareStrRadio()
{
	CheckDlgButton(IDC_CE_COMPARE_STR_RADIO, TRUE);
	CheckDlgButton(IDC_COMPARE_TEXT_FILE_RADIO, FALSE);
	GetDlgItem(IDC_CE_CHECK_RES_FAIL_IF_FOUND_CHECK)->EnableWindow(TRUE);
	GetDlgItem(IDC_CE_CHECK_RESULT_BROWSE_BTN)->ShowWindow(FALSE);
	SetDlgItemText(IDC_CE_CHECK_RESULT_TEXT,"String:");
	m_params.m_type = CE_CHECK_RESULT_COMPARE_STRING_TYPE;
}

void CCmdExeCmdItemCheckResultDlg::OnBnClickedCompareTextFileRadio()
{
	CheckDlgButton(IDC_COMPARE_TEXT_FILE_RADIO, TRUE);
	CheckDlgButton(IDC_CE_COMPARE_STR_RADIO, FALSE);
	GetDlgItem(IDC_CE_CHECK_RES_FAIL_IF_FOUND_CHECK)->EnableWindow(FALSE);
	GetDlgItem(IDC_CE_CHECK_RESULT_BROWSE_BTN)->ShowWindow(TRUE);
	SetDlgItemText(IDC_CE_CHECK_RESULT_TEXT,"Text File:");
	m_params.m_type = CE_CHECK_RESULT_COMPARE_FILE_TYPE;
}

void CCmdExeCmdItemCheckResultDlg::OnBnClickedCeCheckResultBrowseBtn()
{
	char szFilters[128] = "Text Files (*.txt)|*.txt|All Files (*.*)|*.*||";
 	CFileDialog		dlg(TRUE,"txt","*.txt",OFN_FILEMUSTEXIST,szFilters,this);

	if(dlg.DoModal() == IDOK)
	{
		CString	 sFilePath;
		sFilePath = dlg.GetPathName();
		SetDlgItemText(IDC_CE_CHECK_RES_EDIT,sFilePath);
	}
}

void CCmdExeCmdItemCheckResultDlg::OnBnClickedCeCheckResFailIfFoundCheck()
{
	m_params.m_bFailIfFound = !m_params.m_bFailIfFound;
}

BOOL CCmdExeCmdItemCheckResultDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	if(m_pItem != NULL)
	{
		m_params = m_pItem->GetParams();
	}

	m_okBtn.SetTheme(xtpButtonThemeOffice2000);
	m_cancelBtn.SetTheme(xtpButtonThemeOffice2000);
	m_cmpFileBrowseBtn.SetTheme(xtpButtonThemeOffice2000);
	
	/* Intialize com ports combo*/
	vector<CString>           comPortVec;
	vector<CString>::iterator iter,endIter;
	((CMVTerminalApp*)AfxGetApp())->EnumerateSerialPorts(comPortVec);
	iter    = comPortVec.begin();
	endIter = comPortVec.end();
	for(; iter != endIter; iter++)
		m_targetWinCombo.AddString(*iter);

	if(	m_targetWinCombo.SelectString(-1,m_params.m_sComPort) == CB_ERR)
	{
		CMVTerminalView *p = ((CMVTerminalApp*)AfxGetApp())->GetActiveView();
		if(p != NULL)
		{
			// Select current open window
			m_params.m_sComPort = p->GetConnectionParams().GetComPort();
		}
		m_targetWinCombo.SelectString(-1,m_params.m_sComPort);
	}

	switch(m_params.m_type)
	{
	case CE_CHECK_RESULT_COMPARE_STRING_TYPE:
		GetDlgItem(IDC_CE_CHECK_RES_FAIL_IF_FOUND_CHECK)->EnableWindow(TRUE);
		CheckDlgButton(IDC_CE_CHECK_RES_FAIL_IF_FOUND_CHECK, m_params.m_bFailIfFound);
		CheckDlgButton(IDC_COMPARE_TEXT_FILE_RADIO, FALSE);
		CheckDlgButton(IDC_CE_COMPARE_STR_RADIO, TRUE);
		SetDlgItemText(IDC_CE_CHECK_RESULT_TEXT,"String:");
		GetDlgItem(IDC_CE_CHECK_RESULT_BROWSE_BTN)->ShowWindow(FALSE);
		break;
	case CE_CHECK_RESULT_COMPARE_FILE_TYPE:
		GetDlgItem(IDC_CE_CHECK_RES_FAIL_IF_FOUND_CHECK)->EnableWindow(FALSE);
		CheckDlgButton(IDC_COMPARE_TEXT_FILE_RADIO, TRUE);
		CheckDlgButton(IDC_CE_COMPARE_STR_RADIO, FALSE);
		GetDlgItem(IDC_CE_CHECK_RES_FAIL_IF_FOUND_CHECK)->EnableWindow(FALSE);
		GetDlgItem(IDC_CE_CHECK_RESULT_BROWSE_BTN)->ShowWindow(TRUE);
		SetDlgItemText(IDC_CE_CHECK_RESULT_TEXT,"Text File:");
		break;
	default:
		break;
	}
	SetDlgItemText(IDC_CE_CHECK_RES_EDIT,m_params.m_sStr);


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
void CCmdExeCmdItemCheckResultDlg::SetCmdItem(CCmdExeCmdItem* pItem)
{
	m_pItem = dynamic_cast<CCmdExeCmdItemCheckResult*>(pItem);
}


CBasicItemDlg *CCmdExeCmdItemCheckResultDlg::Clone()
{
	return new CCmdExeCmdItemCheckResultDlg();
}





