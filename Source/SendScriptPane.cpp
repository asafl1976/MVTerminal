// SendScriptPane.cpp : implementation file
//

#include "stdafx.h"
#include "MVTerminal.h"
#include "SendScriptPane.h"
#include ".\sendscriptpane.h"



#define  VK_CTRL_C						TCHAR(3)
#define  VK_CTRL_V						TCHAR(22)
#define  CHAR_CR						TCHAR(13) 
#define  CHAR_LF						TCHAR(10)




static LPCTSTR szKeywords = "boot bootd bootext2 bootm bootp bubt chpart cmp cmpm cp cpumap crc32 date dclk	dhcp diskboot echo eeprom erase ext2load "
"ext2ls fatinfo fatload fatls fi flinfo fsinfo fsload g go help icrc32 ide iloop imd iminfo imw inm iprobe ir loop ls map "
"md me mm mmcinit mp mtdparts mtest mw nm pci phyRead pciePhyWrite phyRead phyWrite ping printenv protect rarpboot rcvr	"
"reset resetenv run saveenv se setenv sflash sg sp switchRegRead switchRegWrite tftpboot usb usbboot version SatR whoAmI";

static LPCTSTR szConstants = "";


// CSendScriptPane
IMPLEMENT_DYNCREATE(CSendScriptPane, CFormView)

CSendScriptPane::CSendScriptPane()
	: CFormView(CSendScriptPane::IDD)
{
	m_bLoadDefaultScripts = TRUE;
	// Get installation dir
	m_sAppDataPath = ((CMVTerminalApp*)AfxGetApp())->GetAppDataPath();
	m_sDefaultScriptFolder = GetDefaultScriptsFolder();
	m_bLoadDefaultScripts =  GetLoadDefaultScript();
}

CSendScriptPane::~CSendScriptPane()
{
}

void CSendScriptPane::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SCRIPT_LIST, m_scriptListCtrl);
	DDX_Control(pDX, IDC_NEW_SCRIPT_BTN, m_newScriptBtn);
	DDX_Control(pDX, IDC_REMOVE_SCRIPT_BTN, m_removeScriptBtn);
	DDX_Control(pDX, IDC_LOAD_SCRIPT_BTN, m_loadScriptBtn);
	//DDX_Control(pDX, IDC_SCRIPT_VIEW, m_scriptEditCtrl);
	DDX_Control(pDX, IDC_LINE_DELAY_EDIT, m_lineDelayEditCtrl);
	DDX_Control(pDX, IDC_SCRIPT_NAME_EDIT, m_scriptNameEditCtrl);
	DDX_Control(pDX, IDC_BROWSE_BTN, m_browseBtn);
}

BEGIN_MESSAGE_MAP(CSendScriptPane, CFormView)
	ON_BN_CLICKED(IDC_NEW_SCRIPT_BTN, OnBnClickedNewScriptBtn)
	ON_BN_CLICKED(IDC_REMOVE_SCRIPT_BTN, OnBnClickedRemoveScriptBtn)
	ON_BN_CLICKED(IDC_LOAD_SCRIPT_BTN, OnBnClickedLoadScriptBtn)
	ON_EN_KILLFOCUS(IDC_LINE_DELAY_EDIT, OnEnKillfocusLineDelayEdit)
	ON_EN_KILLFOCUS(IDC_SCRIPT_NAME_EDIT, OnEnKillfocusScriptNameEdit)
	ON_EN_KILLFOCUS(IDC_SCRIPT_VIEW, OnEnKillfocusScriptView)
	ON_LBN_SELCHANGE(IDC_SCRIPT_LIST, OnLbnSelchangeScriptList)
	ON_MESSAGE(WM_LOAD_SCRIPT_THREAD_ENDED,OnLoadScriptEnd)
	ON_LBN_DBLCLK(IDC_SCRIPT_LIST, OnLbnDblclkScriptList)
	ON_BN_CLICKED(IDC_BROWSE_BTN, OnBnClickedBrowseBtn)
END_MESSAGE_MAP()


// CSendScriptPane diagnostics

#ifdef _DEBUG
void CSendScriptPane::AssertValid() const
{
	CFormView::AssertValid();
}

void CSendScriptPane::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG


// CSendScriptPane message handlers

void CSendScriptPane::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	m_newScriptBtn.SetTheme(xtpButtonThemeOffice2000);
	m_removeScriptBtn.SetTheme(xtpButtonThemeOffice2000);
	m_loadScriptBtn.SetTheme(xtpButtonThemeOffice2000);
	m_browseBtn.SetTheme(xtpButtonThemeOffice2000);
	EnableButtons(FALSE);

	m_scriptEditCtrl.SubclassDlgItem(IDC_SCRIPT_VIEW, this);
	m_scriptEditCtrl.Initialize();

	m_scriptEditCtrl.SetCaseSensitive(FALSE);
	m_scriptEditCtrl.SetStringQuotes(_T("\""));
	m_scriptEditCtrl.SetSLComment(_T('/'));
	m_scriptEditCtrl.SetNumberColor( RGB(0,0,0),FALSE);
	

	m_scriptEditCtrl.AddKeywords(szKeywords);
	m_scriptEditCtrl.AddConstants(szConstants);

	// Set plain text so copy/paste with differnt font is always the default font
	//m_scriptEditCtrl.SetTextMode(TM_PLAINTEXT);

    RemoveAllScripts();
	// Create default script
	CreateDefaultScripts();

	// Load scripts from defualt folder
	if(m_bLoadDefaultScripts == TRUE)
	{
		m_importScriptParams.SetParams(this,m_sDefaultScriptFolder);
		AfxBeginThread(ImportScriptsThread,&m_importScriptParams);
	}
}




CString CSendScriptPane::GetDefaultScriptsFolder()
{
	return ((CMVTerminalApp*)AfxGetApp())->GetDefaultScriptsFolder();
}
BOOL CSendScriptPane::GetLoadDefaultScript()
{
	return ((CMVTerminalApp*)AfxGetApp())->GetLoadDefaultScript();
}


void CSendScriptPane::CreateDefaultScripts()
{
	CTextFile   script;
	CString   sLastByte, sLine, sClientIPAddress, sHostIPAddress = ((CMVTerminalApp*)AfxGetApp())->GetLocalHostIPAddress();
	int		  nLastByte;

	/*
		setenv bootdelay -1
		setenv serverip 10.4.53.64
		setenv ipaddr 10.4.53.154
		setenv enaMonExt yes		
		setenv disaMvPnp yes
		saveenv
	*/

	sLine = "setenv bootdelay -1";
	script.AddLine(sLine,TRUE);

	sLine = "setenv enaMonExt yes";
	script.AddLine(sLine,TRUE);

	sLine = "setenv disaMvPnp yes";
	script.AddLine(sLine,TRUE);

	sLine = "setenv serverip ";
	sLine += sHostIPAddress;
	script.AddLine(sLine,TRUE);


	sClientIPAddress = sHostIPAddress.Left(sHostIPAddress.ReverseFind('.') );
	sLastByte = sHostIPAddress.Right(sHostIPAddress.GetLength() - sHostIPAddress.ReverseFind('.') - 1);
	sscanf(sLastByte,"%d",&nLastByte);

	nLastByte += 90;
	if(nLastByte > 240)
		nLastByte = 236;

	sLastByte.Format("%d",nLastByte);
	sClientIPAddress += "." + sLastByte;

	sLine = "setenv ipaddr ";
	sLine += sClientIPAddress;
	script.AddLine(sLine,TRUE);

	sLine = "saveenv";
	script.AddLine(sLine,TRUE);

	script.SetName("Set UBOOT Env.txt");
	script.SetDelay("200");

	ExportScript(script,m_sAppDataPath + "\\Default Scripts");
}

CString CSendScriptPane::GenerateScriptName()
{
	CString sName;
	int		nCount = 0;

	sName.Format("Script%d",nCount);
	while(GetScript(sName) != NULL)
	{
		nCount++;
		sName.Empty();

		sName.Format("Script%d",nCount);
	}
	return sName;
}

void CSendScriptPane::OnBnClickedNewScriptBtn()
{
	CString   sName = GenerateScriptName();
	CTextFile   script(sName,this,"200");

	// Add script to lists
	AddScript(script);
	m_scriptListCtrl.SelectString(-1,script.GetName());
	// show script content
	ShowScript(&script);

	m_scriptEditCtrl.EnableWindow(TRUE);
	m_scriptNameEditCtrl.EnableWindow(TRUE);
	m_lineDelayEditCtrl.EnableWindow(TRUE);

}

void CSendScriptPane::OnBnClickedRemoveScriptBtn()
{
	CTextFile   *pScript = GetSelectedScript();

	if(pScript == NULL)
		return;
	RemoveScript(pScript);
	EnableButtons(FALSE);	
}

void CSendScriptPane::OnBnClickedLoadScriptBtn()
{
	CTextFile			 *pScript = GetSelectedScript();
	CMVTerminalView  *pActiveView = GetActiveView();

	if((pScript == NULL) || (pActiveView == NULL))
		return;

	m_loadScriptBtn.EnableWindow(FALSE);
	m_scriptListCtrl.EnableWindow(FALSE);
	pActiveView->LoadScript(pScript);

	// Set Focus back to view
	pActiveView->SetFocus();

}

void CSendScriptPane::AddScript(CTextFile  script)
{
	m_scriptList.push_back(script);
	m_scriptListCtrl.AddString(script.GetName());
	EnableButtons(FALSE);	
}

void CSendScriptPane::RemoveScript(CTextFile  *pScript)
{
	// Remove from list
	list<CTextFile>::iterator iter = m_scriptList.begin();
	for(; iter != m_scriptList.end(); iter++)
	{
		if((*iter).GetName() == pScript->GetName())
		{
			m_scriptList.erase(iter);
			SetDlgItemText(IDC_SCRIPT_VIEW,"");
			SetDlgItemText(IDC_SCRIPT_NAME_EDIT,"");
			SetDlgItemText(IDC_LINE_DELAY_EDIT,"");
			break;
		}
	}

	// Remove from list ctrl
	int		index = m_scriptListCtrl.GetCurSel();
	if(index == LB_ERR)
		return;
	m_scriptListCtrl.DeleteString(index);
}

CTextFile* CSendScriptPane::GetSelectedScript()
{
	int			index = m_scriptListCtrl.GetCurSel();
	CString		sScriptName;

	if(index == LB_ERR)
		return NULL;
	m_scriptListCtrl.GetText(index,sScriptName);
	return GetScript(sScriptName);
}

CTextFile* CSendScriptPane::GetScript(CString sScriptName)
{
	list<CTextFile>::iterator iter,endIter = m_scriptList.end();
	for(iter = m_scriptList.begin(); iter != endIter; iter++)
	{
		if((*iter).GetName() == sScriptName)
		{
			return  &(*iter);
		}
	}
	return NULL;
}


void CSendScriptPane::OnEnKillfocusLineDelayEdit()
{	
	CTextFile   *pScript = GetSelectedScript();
	CString    sDelay;

	if(pScript == NULL)
		return;

	GetDlgItemText(IDC_LINE_DELAY_EDIT,sDelay);
    pScript->SetDelay(sDelay);
}

void CSendScriptPane::OnEnKillfocusScriptNameEdit()
{
	CTextFile   *pScript = GetSelectedScript();
	CString    sName;

	if(pScript == NULL)
		return;

	GetDlgItemText(IDC_SCRIPT_NAME_EDIT,sName);

	// No change in current name
	if(pScript->GetName() == sName)
		return;

	if(GetScript(sName) == NULL)
	{
        pScript->SetName(sName);
		// Remove from list ctrl
		int			index = m_scriptListCtrl.GetCurSel();
		if(index == LB_ERR)
			return;
		m_scriptListCtrl.DeleteString(index);
		m_scriptListCtrl.AddString(sName);
		m_scriptListCtrl.SelectString(-1,sName);
		return;
	}
	MessageBox("Script name already exist. Please set a different name.",NULL,MB_ICONERROR);
	SetDlgItemText(IDC_SCRIPT_NAME_EDIT,pScript->GetName());
}


void CSendScriptPane::OnEnKillfocusScriptView()
{
	int				 nLine, nLineCount = m_scriptEditCtrl.GetLineCount();
	CTextFile       *pScript = GetSelectedScript();
	CString			 sLine;
	TCHAR			 buf[1024];

	if(pScript == NULL)
	    return;

	if(nLineCount == 0)
		return;

	pScript->Clear();
	for(nLine = 0; nLine < nLineCount; nLine++)
	{
		int nLen  = m_scriptEditCtrl.GetLine(nLine,buf,1024);
		buf[nLen] = NULL;
		sLine     = buf;
		// In Richedit ctrl, remove VK_RETURN char
		sLine.Remove(VK_RETURN);
		// If next line exist --> this line has newline 
		if((nLine + 1) < nLineCount)
			pScript->AddLine(sLine,TRUE);
		else
			pScript->AddLine(sLine,FALSE);
	}
}

void CSendScriptPane::ShowScript(CTextFile   *pScript) 
{
	CString		  sContent, sNewLine;

	if(pScript == NULL)
		return;
	list<CTextLine>::iterator  iter = pScript->GetLineList().begin();
	for(; iter != pScript->GetLineList().end(); iter++)
	{
		sContent += (*iter).GetLine(); 
		if((*iter).HasNewLine() == TRUE)
		{
			// CEdit needs CR+LF for newline
			sContent += CHAR_CR; 
			sContent += CHAR_LF;
		}
	}

	SetDlgItemText(IDC_SCRIPT_VIEW,sContent);
	SetDlgItemText(IDC_SCRIPT_NAME_EDIT,pScript->GetName());
	SetDlgItemText(IDC_LINE_DELAY_EDIT,pScript->GetDelay());
}

void CSendScriptPane::OnLbnSelchangeScriptList()
{
	CTextFile   *pScript = GetSelectedScript();

	if(pScript == NULL)
		return;
	ShowScript(pScript);
	EnableButtons(TRUE);
}

void CSendScriptPane::EnableButtons(BOOL bEnable)
{
    m_removeScriptBtn.EnableWindow(bEnable);
	m_loadScriptBtn.EnableWindow(bEnable);
	m_scriptEditCtrl.EnableWindow(bEnable);
	m_lineDelayEditCtrl.EnableWindow(bEnable);
	m_scriptNameEditCtrl.EnableWindow(bEnable);
}

CMVTerminalView*  CSendScriptPane::GetActiveView()
{
	CChildFrame     *pActiveMDIFrame;
	CMainFrame		*pMainFrame;
	
	pMainFrame = reinterpret_cast<CMainFrame *>(AfxGetApp()->m_pMainWnd);
	if(pMainFrame == NULL)
		return NULL;
	pActiveMDIFrame = (CChildFrame *)pMainFrame->GetActiveFrame();
	if(pActiveMDIFrame == NULL)
		return NULL;
	return (CMVTerminalView*)pActiveMDIFrame->GetActiveView();
}

LRESULT CSendScriptPane::OnLoadScriptEnd(WPARAM a, LPARAM b) 
{
	m_loadScriptBtn.EnableWindow(TRUE);
	m_scriptListCtrl.EnableWindow(TRUE);
	return 0;
}

void CSendScriptPane::OnLbnDblclkScriptList()
{
	OnBnClickedLoadScriptBtn();
}


void CSendScriptPane::OnBnClickedBrowseBtn()
{
	CFileDialog fileDialog(true, NULL, NULL, (OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT | OFN_FILEMUSTEXIST |OFN_LONGNAMES							| OFN_PATHMUSTEXIST), 	_T("Text Files (*.txt)|*.txt|All Files (*.*)|*.*||"));

    fileDialog.m_ofn.lpstrTitle = _T("Choose Scripts");

    // Need larger filename buffer to support multiselect
	TCHAR filenameBuffer[4096] = { 0 } ;
	fileDialog.m_ofn.lpstrFile = filenameBuffer;
	fileDialog.m_ofn.nMaxFile = sizeof(filenameBuffer) / sizeof(filenameBuffer[0]);

	if (IDOK == fileDialog.DoModal())
	{
		CString   sFileName;
		CTextFile script;
		POSITION  pos = fileDialog.GetStartPosition();
		while(pos != NULL)
		{
			sFileName = fileDialog.GetNextPathName( pos );
			script = ImportScript(sFileName);
			if(GetScript(script.GetName()) == NULL)
			{
				AddScript(script);
				ShowScript(&script);
				m_scriptListCtrl.SelectString(0,script.GetName());
			}
			else
			{
				MessageBox("Script name " + script.GetName() + " already exist",NULL,MB_ICONERROR);
			}
		}
	}
	else
	{
		DWORD errCode = CommDlgExtendedError();
		switch (errCode)
		{
		case 0:
			//TRACE(_T("User cancelled Extract dialog\n"));
			break;
		case 0x3003: // buffer to small
			MessageBox(_T("Too many files selected. Try again with fewer files."));
			break;
		default:
			MessageBox(_T("An error occurred"));
			break;
		}
	}
	EnableButtons(TRUE);
}

CTextFile  CSendScriptPane::ImportScript(CString sPath)
{
	CTextFile		script;
	CString     sLine;
	FILE	   *pScriptFile;
    TCHAR   	cLine[1024];

	// Use this method of open file since we need newline char on read
    if((pScriptFile = fopen( sPath, "rt" )) == NULL )
    {		
		MessageBox("Can't open file "+sPath,NULL,MB_ICONERROR);
		return script;
	}

	// Read lines INCLUDING newline char
    while(fgets( cLine, 1024, pScriptFile ) != NULL)
	{
		sLine = cLine;
		// Remove newline char - indication for newline
		script.AddLine(sLine,sLine.Remove(CHAR_LF));
	}
	fclose( pScriptFile );
	script.SetDelay("200");
	script.SetName(sPath.Right(sPath.GetLength() - sPath.ReverseFind('\\') - 1));
	script.SetWnd(this);
	return script;

}
BOOL CSendScriptPane::ExportScripts(CString sDirPath)
{
	list<CTextFile>::iterator iter = m_scriptList.begin(),endIter = m_scriptList.end();

	for(; iter != endIter; iter++)
	{
		if(ExportScript(*iter,sDirPath) == FALSE)
			MessageBox("Can't save script " + (*iter).GetName() ,NULL,MB_ICONEXCLAMATION);
	}
	return TRUE;
}
void CSendScriptPane::ImportScripts(CString sDirPath)
{
	CString     sLine,sFileName;
	CFileFind   scriptFileFind;
	BOOL		bResult;
	CTextFile     script;

   	if(bResult = scriptFileFind.FindFile(sDirPath + "\\*.txt"))
	{
		while(bResult)
		{
			bResult = scriptFileFind.FindNextFile();
			sFileName = (LPCTSTR)scriptFileFind.GetFilePath();
			script = ImportScript(sFileName);
			if(GetScript(script.GetName()) == NULL)
			{
				AddScript(script);
			}
			else
			{
				MessageBox("Script name " + script.GetName() + " already exist",NULL,MB_ICONERROR);
			}
		}
	}
	scriptFileFind.Close();
}

UINT ImportScriptsThread(LPVOID pParam)
{
	CSendScriptPane::CImportScriptsParams  *pImportScriptsParams = (CSendScriptPane::CImportScriptsParams*)pParam;
	pImportScriptsParams->GetScriptPane()->ImportScripts(pImportScriptsParams->GetDirPath());
	return 0;
}

BOOL  CSendScriptPane::ExportScript(CTextFile script, CString sDirPath)
{
	CStdioFile scriptFile;
	CString		sScriptFileName,*spLine;
	int			nLineCount;

	sScriptFileName = sDirPath + "\\";
	sScriptFileName += script.GetName();

	// Append txt extension
	if(sScriptFileName.Find(".txt",0) == -1)
		sScriptFileName += ".txt";

    if(!scriptFile.Open(sScriptFileName, CFile::modeCreate | CFile::modeWrite | CFile::typeText ) ) 
	{
		return FALSE;
	}	
	nLineCount = script.GetLineCount();
	for(int nLine = 0; nLine < nLineCount; nLine++)
	{
		if((spLine = script.GetLine(nLine)) != NULL)
		{
			scriptFile.WriteString(*spLine);
			if(script.GetTextLine(nLine)->HasNewLine() == TRUE)
				scriptFile.WriteString("\n");
		}
		else
		{
			scriptFile.Close();
			return FALSE;
		}
	}
	scriptFile.Close();			
	return TRUE;
}

CString CSendScriptPane::Serialize()
{
	CMarkup xmlWriter;

	list<CTextFile>::iterator iter = m_scriptList.begin(), endIter = m_scriptList.end();

	xmlWriter.AddElem("SendScriptPaneFiles");
	xmlWriter.IntoElem();

	for(; iter != endIter; iter++)
	{
		xmlWriter.AddSubDoc( (*iter).Serialize() );
	}

	xmlWriter.OutOfElem();
	return xmlWriter.GetDoc();
}


BOOL CSendScriptPane::DeSerialize(CMarkup & xmlReader)
{
	if( xmlReader.FindElem("SendScriptPaneFiles") == false)
		return FALSE;

	//First, remove all current scripts
	RemoveAllScripts();

	xmlReader.IntoElem(); 
	while(xmlReader.FindElem("Script"))
	{
		CTextFile  script;
		if(script.DeSerialize(xmlReader) == false)
			return FALSE;

		if(GetScript(script.GetName()) == NULL)
		{
			script.SetWnd(this);
			AddScript(script);
		}
	}
	xmlReader.OutOfElem(); 
	return TRUE;	
}

void CSendScriptPane::RemoveAllScripts()
{
	m_scriptList.clear();

	SetDlgItemText(IDC_SCRIPT_VIEW,"");
	SetDlgItemText(IDC_SCRIPT_NAME_EDIT,"");
	SetDlgItemText(IDC_LINE_DELAY_EDIT,"");

	m_scriptListCtrl.ResetContent();
	EnableButtons(FALSE);	
}


