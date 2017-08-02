// BootLoaderParamsSettingsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MVTerminal.h"
#include "BootLoaderParamsSettingsDlg.h"
#include ".\BootLoaderParamssettingsdlg.h"
#include "Markup.h"


// CBootLoaderParamsSettingsDlg dialog

IMPLEMENT_DYNAMIC(CBootLoaderParamsSettingsDlg, CDialog)

CBootLoaderParamsSettingsDlg::CBootLoaderParamsSettingsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBootLoaderParamsSettingsDlg::IDD, pParent)
{
	OnBnClickedRestoreDefaultBtn();
}



CBootLoaderParamsSettingsDlg::~CBootLoaderParamsSettingsDlg()
{
}

void CBootLoaderParamsSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RESTORE_DEFAULT_BTN ,m_restoreDefaultBtn);

}


BEGIN_MESSAGE_MAP(CBootLoaderParamsSettingsDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedRestoreDefaultBtn)
END_MESSAGE_MAP()


// CBootLoaderParamsSettingsDlg message handlers

BOOL CBootLoaderParamsSettingsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	UpdateWindowData();

	m_restoreDefaultBtn.SetTheme(xtpButtonThemeOffice2000);

	return FALSE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CBootLoaderParamsSettingsDlg::OnOK()
{
	CString		sTmp;

	GetDlgItemText(IDC_LOAD_COMMAND_EDIT,sTmp);
	GetBootLoaderParams().SetTFTPLoadCommand(sTmp);

	GetDlgItemText(IDC_BURN_EXE_COMMAND_EDIT,sTmp);
	GetBootLoaderParams().SetTFTPBurnExeCommand(sTmp);

	GetDlgItemText(IDC_BURN_AS_IS_COMMAND_EDIT,sTmp);
	GetBootLoaderParams().SetTFTPBurnAsIsCommand(sTmp);

	GetDlgItemText(IDC_RUN_COMMAND_EDIT,sTmp);
	GetBootLoaderParams().SetRunCommand(sTmp);

	GetDlgItemText(IDC_LOAD_ADDRESS,sTmp);
	GetBootLoaderParams().SetTFTPLoadAddress(sTmp);
}

void   CBootLoaderParamsSettingsDlg::SetDlgType(int nType)
{
	m_nDlgType = nType;
}

int   CBootLoaderParamsSettingsDlg::GetDlgType()
{
	return m_nDlgType;
}

void CBootLoaderParamsSettingsDlg::OnBnClickedRestoreDefaultBtn()
{
    GetBootLoaderParams().SetTFTPLoadCommand("tftp");
	SetDlgItemText(IDC_LOAD_COMMAND_EDIT,GetBootLoaderParams().GetTFTPLoadCommand());
	GetBootLoaderParams().SetTFTPBurnExeCommand("FStftpe");
	SetDlgItemText(IDC_BURN_EXE_COMMAND_EDIT,GetBootLoaderParams().GetTFTPBurnExeCommand());
	GetBootLoaderParams().SetTFTPBurnAsIsCommand("FStftp");
	SetDlgItemText(IDC_BURN_AS_IS_COMMAND_EDIT,GetBootLoaderParams().GetTFTPBurnAsIsCommand());
	GetBootLoaderParams().SetRunCommand("go");
	SetDlgItemText(IDC_RUN_COMMAND_EDIT,GetBootLoaderParams().GetRunCommand());
	GetBootLoaderParams().SetTFTPLoadAddress("0x81000000");
	SetDlgItemText(IDC_LOAD_ADDRESS,GetBootLoaderParams().GetTFTPLoadAddress());
}



CString CBootLoaderParamsSettingsDlg::Serialize()
{
	CMarkup xmlWriter;

	xmlWriter.AddElem("BootLoaderParams");
	xmlWriter.AddAttrib("LoadCommand", m_bootLoaderParams.GetTFTPLoadCommand());
	xmlWriter.AddAttrib("BurnAsIsCommand", m_bootLoaderParams.GetTFTPBurnAsIsCommand());
	xmlWriter.AddAttrib("BurnExeCommand", m_bootLoaderParams.GetTFTPBurnExeCommand());
	xmlWriter.AddAttrib("RunCommand", m_bootLoaderParams.GetRunCommand());
	xmlWriter.AddAttrib("LoadAddress", m_bootLoaderParams.GetTFTPLoadAddress());

	return xmlWriter.GetDoc();
}

BOOL CBootLoaderParamsSettingsDlg::DeSerialize(CMarkup & xmlReader)
{
	if( xmlReader.FindElem("BootLoaderParams") == false)
		return FALSE;

	m_bootLoaderParams.SetTFTPLoadCommand( xmlReader.GetAttrib("LoadCommand") );
	m_bootLoaderParams.SetTFTPBurnAsIsCommand( xmlReader.GetAttrib("BurnAsIsCommand") );
	m_bootLoaderParams.SetTFTPBurnExeCommand( xmlReader.GetAttrib("BurnExeCommand") );
	m_bootLoaderParams.SetRunCommand( xmlReader.GetAttrib("RunCommand") );
	m_bootLoaderParams.SetTFTPLoadAddress( xmlReader.GetAttrib("LoadAddress") );

	return TRUE;
}

void CBootLoaderParamsSettingsDlg::UpdateWindowData()
{
	SetDlgItemText(IDC_LOAD_COMMAND_EDIT,GetBootLoaderParams().GetTFTPLoadCommand());
	SetDlgItemText(IDC_BURN_EXE_COMMAND_EDIT,GetBootLoaderParams().GetTFTPBurnExeCommand());
	SetDlgItemText(IDC_BURN_AS_IS_COMMAND_EDIT,GetBootLoaderParams().GetTFTPBurnAsIsCommand());
	SetDlgItemText(IDC_RUN_COMMAND_EDIT,GetBootLoaderParams().GetRunCommand());
	SetDlgItemText(IDC_LOAD_ADDRESS,GetBootLoaderParams().GetTFTPLoadAddress());
}



