// PlatformInfoUpdateDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MVTerminal.h"
#include "PlatformInfoUpdateDlg.h"
#include "FileOperations.h"

// CPlatformInfoUpdateDlg dialog

IMPLEMENT_DYNAMIC(CPlatformInfoUpdateDlg, CDialog)

CPlatformInfoUpdateDlg::CPlatformInfoUpdateDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPlatformInfoUpdateDlg::IDD, pParent)
{

}

CPlatformInfoUpdateDlg::~CPlatformInfoUpdateDlg()
{
}

void CPlatformInfoUpdateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, m_fileList);
	DDX_Control(pDX, IDC_NEW_FILES_STATIC, m_newFilesMsgStatic);

}


BEGIN_MESSAGE_MAP(CPlatformInfoUpdateDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CPlatformInfoUpdateDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CPlatformInfoUpdateDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CPlatformInfoUpdateDlg message handlers

void CPlatformInfoUpdateDlg::OnBnClickedOk()
{
	CString sAppDataPath = ((CMVTerminalApp*)AfxGetApp())->GetAppDataPath();
	CString sTargetPath = sAppDataPath;
	CFileOperation fo;      

	fo.SetOverwriteMode(true); 

	vector<CString>::iterator iter, endIter = m_filesVec.end();
	for(iter = m_filesVec.begin(); iter != endIter; iter++)
	{
		CString sFileName = (*iter).Right( (*iter).GetLength() -  (*iter).Find("\\PlatformInfo") - 1);
		sTargetPath = sTargetPath + "\\" + sFileName;
		fo.DoFileCopy((*iter), sTargetPath );
	}

	OnOK();
}

void CPlatformInfoUpdateDlg::OnBnClickedCancel()
{
	OnCancel();
}

BOOL CPlatformInfoUpdateDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetDlgItemText(IDC_NEW_FILES_STATIC,"New Platform Info XML files were found, would you like to update your local folder?");
	m_newFilesMsgStatic.SetFontStyle(FS_BOLD);

	vector<CString>::iterator iter, endIter = m_filesVec.end();
	for(iter = m_filesVec.begin(); iter != endIter; iter++)
	{
		CString sFileName = (*iter).Right( (*iter).GetLength() -  (*iter).ReverseFind('\\') - 1);
		m_fileList.AddString(sFileName);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
