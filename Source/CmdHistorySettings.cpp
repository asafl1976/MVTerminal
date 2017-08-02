// CmdHistorySettings.cpp : implementation file
//

#include "stdafx.h"
#include "MVTerminal.h"
#include "CmdHistorySettings.h"
#include ".\cmdhistorysettings.h"
#include "Markup.h"


// CCmdHistorySettingsDlg dialog

IMPLEMENT_DYNAMIC(CCmdHistorySettingsDlg, CDialog)
CCmdHistorySettingsDlg::CCmdHistorySettingsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCmdHistorySettingsDlg::IDD, pParent)
{
	m_cmdHistoryParams.SetMaxCmdCount(30);
	m_cmdHistoryParams.SetCmdMinLenThreshold(2);
	m_cmdHistoryParams.SetCmdMaxLenThreshold(64);
	m_cmdHistoryParams.SetUseTerminalHistory(TRUE);
	m_cmdHistoryParams.SetRemoveDuplicatesOnMenu(TRUE);
	m_cmdHistoryParams.SetFilter("");
} 

CCmdHistorySettingsDlg::~CCmdHistorySettingsDlg()
{
}

void CCmdHistorySettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CLEAR_HISTORY_BTN, m_clearHistoryBtn);
}


BEGIN_MESSAGE_MAP(CCmdHistorySettingsDlg, CDialog)
	ON_BN_CLICKED(IDC_CLEAR_HISTORY_BTN, OnBnClickedClearHistoryBtn)
	ON_BN_CLICKED(IDC_SEND_KEY_CODES_AS_IS, OnBnClickedSendKeyCodesAsIs)
	ON_BN_CLICKED(IDC_ACT_AS_HISTORY, OnBnClickedActAsHistory)
	ON_BN_CLICKED(IDC_REM_DUP_STRINGS_MENU_CHECK, OnBnClickedRemDupStringsMenuCheck)
END_MESSAGE_MAP()


// CCmdHistorySettingsDlg message handlers

void CCmdHistorySettingsDlg::OnBnClickedClearHistoryBtn()
{
	CChildFrame     *pActiveMDIFrame;
	CMainFrame		*pMainFrame;
	
	pMainFrame = reinterpret_cast<CMainFrame *>(AfxGetApp()->m_pMainWnd);
	if(pMainFrame == NULL)
		return ;
	pActiveMDIFrame = (CChildFrame *)pMainFrame->GetActiveFrame();
	if(pActiveMDIFrame == NULL)
		return ;
	CMVTerminalView* pView = (CMVTerminalView*)pActiveMDIFrame->GetActiveView();

	if(pView == NULL)
		return;

	pView->GetCmdHistoryLogger().ClearHistoryList();
}

void CCmdHistorySettingsDlg::OnBnClickedSendKeyCodesAsIs()
{
	m_clearHistoryBtn.EnableWindow(FALSE);
	GetDlgItem(IDC_CMD_MAX_LEN_EDIT)->EnableWindow(FALSE);

	CheckDlgButton(IDC_ACT_AS_HISTORY,FALSE);
	CheckDlgButton(IDC_SEND_KEY_CODES_AS_IS,TRUE);

	GetDlgItem(IDC_CMD_FILTER_EDIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_CMD_LEN_THRS_EDIT)->EnableWindow(FALSE);

	m_cmdHistoryParams.SetUseTerminalHistory(FALSE);

}

BOOL CCmdHistorySettingsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString  sMaxHisCount,sMinLenThrshold, sMaxLenThrshold;

	UpdateWindowData();

	m_clearHistoryBtn.SetTheme(xtpButtonThemeOffice2000);

	return FALSE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CCmdHistorySettingsDlg::OnOK()
{
	CString  sMaxHisCount,sFilter,sCmdMinLenThreshold,sCmdMaxLenThreshold;
	int    nCmdMinLenThreshold,nCmdMaxLenThreshold,nMaxCmdCount;

	GetDlgItemText(IDC_CMD_MAX_LEN_EDIT,sMaxHisCount);
	sscanf(sMaxHisCount,"%d",&nMaxCmdCount);
	m_cmdHistoryParams.SetMaxCmdCount(nMaxCmdCount);

	GetDlgItemText(IDC_CMD_FILTER_EDIT,sFilter);
	if((sFilter.Right(1) != ";") && (sFilter.IsEmpty() == FALSE))
		sFilter += ";";
	m_cmdHistoryParams.SetFilter(sFilter);

	GetDlgItemText(IDC_CMD_MIN_LEN_THRS_EDIT,sCmdMinLenThreshold);
	GetDlgItemText(IDC_CMD_MAX_LEN_THRS_EDIT,sCmdMaxLenThreshold);
	sscanf(sCmdMinLenThreshold,"%d",&nCmdMinLenThreshold);
	sscanf(sCmdMaxLenThreshold,"%d",&nCmdMaxLenThreshold);
	m_cmdHistoryParams.SetCmdMinLenThreshold(nCmdMinLenThreshold);
	m_cmdHistoryParams.SetCmdMaxLenThreshold(nCmdMaxLenThreshold);

}

void CCmdHistorySettingsDlg::OnBnClickedActAsHistory()
{
	m_clearHistoryBtn.EnableWindow(TRUE);
	GetDlgItem(IDC_CMD_MAX_LEN_EDIT)->EnableWindow(TRUE);
	CheckDlgButton(IDC_ACT_AS_HISTORY,TRUE);
	CheckDlgButton(IDC_SEND_KEY_CODES_AS_IS,FALSE);
	m_cmdHistoryParams.SetUseTerminalHistory(TRUE);

	GetDlgItem(IDC_CMD_FILTER_EDIT)->EnableWindow(TRUE);
	GetDlgItem(IDC_CMD_LEN_THRS_EDIT)->EnableWindow(TRUE);
}






CString CCmdHistorySettingsDlg::Serialize()
{
	CMarkup xmlWriter;

	xmlWriter.AddElem("CommandHistorySettings");
	xmlWriter.AddAttrib("QueueDepth",m_cmdHistoryParams.GetMaxCmdCount());
	xmlWriter.AddAttrib("CmdMinLenThreshold",m_cmdHistoryParams.GetCmdMinLenThreshold());
	xmlWriter.AddAttrib("CmdMaxLenThreshold",m_cmdHistoryParams.GetCmdMaxLenThreshold());
	xmlWriter.AddAttrib("UseTerminalHistory",m_cmdHistoryParams.GetUseTerminalHistory());
	xmlWriter.AddAttrib("Filter",m_cmdHistoryParams.GetFilter());
	xmlWriter.AddAttrib("RemoveDuplicatesOnMenu",m_cmdHistoryParams.GetRemoveDuplicatesOnMenu());

	return xmlWriter.GetDoc();
}

BOOL CCmdHistorySettingsDlg::DeSerialize(CMarkup & xmlReader)
{
	CString sVal;
	int nMaxCmdCount,nCmdMinLenThreshold,nCmdMaxLenThreshold;

	if( xmlReader.FindElem("CommandHistorySettings") == false )
		return FALSE;

	sVal = xmlReader.GetAttrib("QueueDepth");
	sscanf(sVal,"%d",&nMaxCmdCount);

	sVal = xmlReader.GetAttrib("CmdMinLenThreshold");
	sscanf(sVal,"%d",&nCmdMinLenThreshold);

	sVal = xmlReader.GetAttrib("CmdMaxLenThreshold");
	sscanf(sVal,"%d",&nCmdMaxLenThreshold);

	m_cmdHistoryParams.SetMaxCmdCount(nMaxCmdCount);
	m_cmdHistoryParams.SetCmdMinLenThreshold(nCmdMinLenThreshold);
	m_cmdHistoryParams.SetCmdMaxLenThreshold(nCmdMaxLenThreshold);

	m_cmdHistoryParams.SetUseTerminalHistory( (xmlReader.GetAttrib("UseTerminalHistory") == "0")? FALSE:TRUE);
	m_cmdHistoryParams.SetFilter(xmlReader.GetAttrib("Filter"));
	m_cmdHistoryParams.SetRemoveDuplicatesOnMenu((xmlReader.GetAttrib("RemoveDuplicatesOnMenu") == "0")? FALSE:TRUE);

	return TRUE;
}

void CCmdHistorySettingsDlg::UpdateWindowData()
{
	CString  sMaxHisCount,sMinLenThrshold,sMaxLenThrshold;

	sMaxHisCount.Format("%d",m_cmdHistoryParams.GetMaxCmdCount());
	SetDlgItemText(IDC_CMD_MAX_LEN_EDIT,sMaxHisCount);
	sMinLenThrshold.Format("%d",m_cmdHistoryParams.GetCmdMinLenThreshold());
	sMaxLenThrshold.Format("%d",m_cmdHistoryParams.GetCmdMaxLenThreshold());
	SetDlgItemText(IDC_CMD_MIN_LEN_THRS_EDIT,sMinLenThrshold);
	SetDlgItemText(IDC_CMD_MAX_LEN_THRS_EDIT,sMaxLenThrshold);
	if(m_cmdHistoryParams.GetUseTerminalHistory() == TRUE)
		OnBnClickedActAsHistory();
	else
		OnBnClickedSendKeyCodesAsIs();
	SetDlgItemText(IDC_CMD_FILTER_EDIT,m_cmdHistoryParams.GetFilter());

}


void CCmdHistorySettingsDlg::OnBnClickedRemDupStringsMenuCheck()
{
	m_cmdHistoryParams.SetRemoveDuplicatesOnMenu(!m_cmdHistoryParams.GetRemoveDuplicatesOnMenu());
}
