// MVTerminal.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "MVTerminal.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include "MVTerminalDoc.h"
#include "MVTerminalView.h"
#include ".\mvterminal.h"
#include "afxwin.h"
#include "IPAddressListDlg.h"
#include "ExecApp.h"
#include <direct.h>

#include "TFTPServerPane.h"
#include "SendScriptPane.h"
#include "DataAnalyzerPane.h"
#include "CmdExePane.h"
#include "HwModulePane.h"
#include "HwModuleModel.h"
#include "MVTerminalUpdateDlg.h"
#include "BoardPowerCtrlManager.h"
#include "PlatformInfoUpdateDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAboutDlg dialog used for App About
extern TCHAR szAppTitlString[];
extern TCHAR szAppVersion[];


// CMVTerminalApp

BEGIN_MESSAGE_MAP(CMVTerminalApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_HELP_CHECKFORUPDATES, OnCheckForUpdates)
	
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)

	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_UPDATE_COMMAND_UI(ID_FILE_NEW, OnFileNewUpdate)

	ON_COMMAND(ID_APP_SETTINGS, OnAppSettings)


	ON_COMMAND(ID_FILE_RESTORELASTCONFIGURATION, OnRestoreConfiguration)

	ON_COMMAND(ID_FILE_LOADCONFIGURATION, OnLoadConfiguration)
	ON_COMMAND(ID_FILE_SAVECONFIGURATION, OnSaveConfiguration)

	ON_COMMAND(ID_FILE_EXPORTSCRIPTS, OnExportScripts)
	ON_COMMAND(ID_FILE_IMPORTSCRIPTS, OnImportScripts)

	ON_COMMAND(ID_FILE_SENDTEXTFILE,OnSendTextFile)
	
	ON_COMMAND(ID_POWER_CTRL_POWER, OnPowerCtrlPower)
	ON_UPDATE_COMMAND_UI(ID_POWER_CTRL_POWER, OnPowerCtrlPowerUpdate)

	ON_COMMAND(ID_POWER_CTRL_RESET, OnPowerCtrlReset)
	ON_UPDATE_COMMAND_UI(ID_POWER_CTRL_RESET, OnPowerCtrlResetUpdate)


	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_COMMAND(ID_APP_EXIT, OnAppExit)
END_MESSAGE_MAP()


// CMVTerminalApp construction

CMVTerminalApp::CMVTerminalApp()
{
	m_sInstallPath = GetRegistryEntryValue("InstallPath");
	m_sAppDataPath = GetRegistryEntryValue("AppDataPath");

	// Create directories 
	CreateDirectory(m_sAppDataPath + "\\Default Scripts",NULL);
	CreateDirectory(m_sAppDataPath + "\\User Scripts",NULL);
	CreateDirectory(m_sAppDataPath + "\\AppConfig",NULL);
	

	InitLocalHostIPAddressList();
	m_bEnableNewSessionBtn = true;
	m_bCloseUponUpdate = FALSE;
	m_nPowerCtrlPowerCheck = 0;
	m_nPowerCtrlResetCheck = 0;
	m_bPowerCtrlPowerEn = FALSE;
	m_bPowerCtrlResetEn = FALSE;
}

CMVTerminalApp::~CMVTerminalApp()
{
}


void CMVTerminalApp::InitHwModulesModel(BOOL bDetHwModulesOnStartup)
{
	CHwModuleModel::GetInstance().Init(bDetHwModulesOnStartup);
}

void    CMVTerminalApp::OnDeleteObject() 
{
	
}

void CMVTerminalApp::CreateRestoreFile()
{
	if(GetConfigurationDataAvailability() == TRUE)
	{
		SaveConfiguration(m_sAppDataPath + "\\AppConfig\\mvtRestore.mvtc");
	}
}

// The one and only CMVTerminalApp object
CMVTerminalApp theApp;

// CMVTerminalApp initialization

CString  CMVTerminalApp::GetInstallationPath() 
{
	if(m_sInstallPath.IsEmpty() == TRUE)
	{
		m_sInstallPath = GetRegistryEntryValue("InstallPath");
	}
	return m_sInstallPath;
}

CString  CMVTerminalApp::GetAppDataPath() 
{
	if(m_sAppDataPath.IsEmpty() == TRUE)
	{
		m_sAppDataPath = GetRegistryEntryValue("AppDataPath");
	}
	return m_sAppDataPath;
}


void CMVTerminalApp::OnRestoreConfiguration()
{
	LoadConfiguration(m_sAppDataPath + "\\AppConfig\\mvtRestore.mvtc");
}

void CMVTerminalApp::OnLoadConfiguration()
{
	CString sFilePath;

	char szFilters[128] = "MVTerminal Configuration Files (*.mvtc)|*.mvtc|All Files (*.*)|*.*||";
 	CFileDialog		dlg(TRUE,"mvtc","",OFN_OVERWRITEPROMPT,szFilters,NULL);
	CString			sItem;
	
	if(dlg.DoModal() == IDOK)
	{
		sFilePath = dlg.GetPathName();
		LoadConfiguration(sFilePath);
	}
}

int CMVTerminalApp::SaveConfiguration()
{
	CString	    sFile;
	CMarkup  	xmlWriter;

	if(m_bCloseUponUpdate == TRUE)
	{
		return IDABORT;
	}
	if(GetAppSettingsDlg().GetGeneralSettingTab().GetPromptToSaveSettings() == FALSE)
	{
		return IDABORT;
	}
	if(GetConfigurationDataAvailability() == false)
	{
		return IDABORT;
	}

	m_nRC = OpenSaveConfigurationDlg();
	switch(m_nRC)
	{
	case IDYES:
		sFile = m_saveCfgDlg.GetPath();
		if(SaveConfiguration(sFile) == FALSE)
		{
			AfxMessageBox("Failed to save configuration!",NULL,MB_ICONERROR);
			return IDABORT;
		}
		break;
	default:
		break;
	}
	return m_nRC;
}

BOOL CMVTerminalApp::GetConfigurationDataAvailability()
{	
	BOOL	    bData = FALSE;

	CMainFrame *pMainFrame = (CMainFrame*)m_pMainWnd;

	CSendScriptPane		  *pSendScriptPane = pMainFrame->GetSendScriptPane();
	CTFTPServerPane       *pTFTPServerPane = pMainFrame->GetTFTPServerPane();
	CCmdExePane           *pCmdExePane     = pMainFrame->GetCommandExecuterPane();
	CHwModulesPane        *pHwModulesPane  = pMainFrame->GetHardwareModulesPane();
	
	m_saveCfgDlg.EnableSaveScriptCheck(FALSE);
	m_saveCfgDlg.EnableSaveTFTPCheck(FALSE);
	m_saveCfgDlg.EnableSaveCommandExeCheck(FALSE);
	m_saveCfgDlg.EnableSaveSessionsCheck(FALSE);
	m_saveCfgDlg.EnableSaveHardwareModulesCheck(FALSE);

	if(pSendScriptPane != NULL)
	{
		if(pSendScriptPane->GetScriptCount() > 0)
		{
			bData = TRUE;
			m_saveCfgDlg.EnableSaveScriptCheck(TRUE);
		}	
	}	

	if(pTFTPServerPane != NULL)
	{
		if(pTFTPServerPane->GetFileCount() > 0)
		{
			bData = TRUE;
			m_saveCfgDlg.EnableSaveTFTPCheck(TRUE);
		}	
	}

	if(pCmdExePane != NULL)
	{
		if(pCmdExePane->GetCommandCount() > 0)
		{
			bData = TRUE;
			m_saveCfgDlg.EnableSaveCommandExeCheck(TRUE);
		}	
	}

	if(pMainFrame->GetViewCount() > 0)
	{
		bData = TRUE;
		m_saveCfgDlg.EnableSaveSessionsCheck(TRUE);
	}
	if(pHwModulesPane != NULL)
	{
		if(pHwModulesPane->GetHwModuleCtrlsCount() > 0)
		{
			bData = TRUE;
			m_saveCfgDlg.EnableSaveHardwareModulesCheck(TRUE);
		}	
	}
	return bData;

}

int CMVTerminalApp::OpenSaveConfigurationDlg()
{
	CMainFrame *pMainFrame = (CMainFrame*)m_pMainWnd;

	if(GetConfigurationDataAvailability() == FALSE)
	{
		AfxMessageBox("No configuration to save!",NULL,MB_ICONHAND);
		return -1;
	}

	CString		sAppDataPath = GetAppDataPath() + "\\AppConfig\\";

	m_saveCfgDlg.SetPath(sAppDataPath);
	m_saveCfgDlg.DoModal();
	return m_saveCfgDlg.GetChoice();
}

void CMVTerminalApp::SetSaveConfigDlgCloseBtnText(CString sText)
{
	m_saveCfgDlg.SetCloseBtnText(sText);
}

void CMVTerminalApp::OnSaveConfiguration()
{
	CString	    sFile;
	CMarkup  	xmlWriter;

	SetSaveConfigDlgCloseBtnText("Close");
	m_nRC = OpenSaveConfigurationDlg();
	switch(m_nRC)
	{
	case IDYES:
		sFile = m_saveCfgDlg.GetPath();
		if(SaveConfiguration(sFile) == FALSE)
		{
			AfxMessageBox("Failed to save configuration!",NULL,MB_ICONERROR);
			return;
		}
		break;
	default:
		break;
	}
}

CString	CMVTerminalApp::Serialize()
{
	CMainFrame       *pMainFrame = ((CMainFrame*)m_pMainWnd);
	CMarkup			  xmlWriter;
	CTFTPServerPane  *pTFTPServerPane;
	CSendScriptPane	 *pSendScriptPane;
	CCmdExePane      *pCmdExePane    ;
	CHwModulesPane   *pHwModulesPane ;

	pTFTPServerPane = pMainFrame->GetTFTPServerPane();
	pSendScriptPane = pMainFrame->GetSendScriptPane();
	pCmdExePane     = pMainFrame->GetCommandExecuterPane();
	pHwModulesPane  = pMainFrame->GetHardwareModulesPane();


	xmlWriter.AddElem("MVTerminalConfiguration");
	xmlWriter.AddAttrib("CWD",GetCurrentWorkingDir());
	xmlWriter.AddAttrib("AppVersion",szAppVersion);
	xmlWriter.IntoElem();

	/////////////// Save TFTP Files ////////////////////
	if(pTFTPServerPane != NULL)
	{
		xmlWriter.AddSubDoc( pTFTPServerPane->Serialize() );
	}
	////////////////////////////////////////////////////

	/////////////// Save scripts ////////////////////////
	if(pSendScriptPane != NULL)
	{
		xmlWriter.AddSubDoc( pSendScriptPane->Serialize() );
	}
	////////////////////////////////////////////////////

	/////////////// Save sessions ////////////////////////
	xmlWriter.AddSubDoc( pMainFrame->Serialize() );
	////////////////////////////////////////////////////

	/////////////// Save HwModules  ////////
	if(pHwModulesPane != NULL)
	{
		xmlWriter.AddSubDoc( pHwModulesPane->Serialize() );
	}
	////////////////////////////////////////////////////

	/////////////// Save command executer files ////////
	if(pCmdExePane != NULL)
	{
		xmlWriter.AddSubDoc( pCmdExePane->Serialize() );
	}
	////////////////////////////////////////////////////

	xmlWriter.OutOfElem();
	return xmlWriter.GetDoc();
}

BOOL CMVTerminalApp::DeSerialize(CMarkup & xmlReader)
{
	CMainFrame       *pMainFrame = ((CMainFrame*)m_pMainWnd);

	CTFTPServerPane  *pTFTPServerPane;
	CSendScriptPane	 *pSendScriptPane;
	CCmdExePane      *pCmdExePane    ;
	CHwModulesPane   *pHwModulesPane ;
	CString			  sCWD,sAppVersion;

	pTFTPServerPane = pMainFrame->GetTFTPServerPane();
	pSendScriptPane = pMainFrame->GetSendScriptPane();
	pCmdExePane     = pMainFrame->GetCommandExecuterPane();
	pHwModulesPane  = pMainFrame->GetHardwareModulesPane();

	if(xmlReader.FindElem("MVTerminalConfiguration") == false)
		return FALSE;

	sCWD = xmlReader.GetAttrib("CWD");
	sAppVersion = xmlReader.GetAttrib("AppVersion");
	if(sAppVersion != szAppVersion)
	{
		int res = AfxMessageBox("Restore file was create by a different MVTerminal version ("+sAppVersion+"), continue?",
			MB_YESNO|MB_ICONWARNING);
		if(res == IDNO)
		{
			return FALSE;
		}
	}
	xmlReader.IntoElem();

	if(sCWD.IsEmpty() == TRUE)
		sCWD = "C:\\";
	SetCurrentWorkingDir(sCWD);

	/////////////// Load TFTP Files ////////////////////
	if(pTFTPServerPane != NULL)
	{
		if(pTFTPServerPane->DeSerialize(xmlReader) == FALSE)
		{
			AfxMessageBox("Failed to restore TFTP Server pane settings!",NULL,MB_ICONERROR);
			return FALSE;
		}
	}
	////////////////////////////////////////////////////

	/////////////// Load scripts ////////////////////////
	if(pSendScriptPane != NULL)
	{
		if(pSendScriptPane->DeSerialize(xmlReader) == FALSE)
		{
			AfxMessageBox("Failed to restore Send Script pane settings!",NULL,MB_ICONERROR);
			return FALSE;
		}
	}
	////////////////////////////////////////////////////
	
	/////////////// Load sessions ////////////////////////
	if(pMainFrame->DeSerialize(xmlReader) == FALSE)
	{
		AfxMessageBox("Failed to restore opened sessions and mainframe settings!",NULL,MB_ICONERROR);
		return FALSE;
	}
	if(pMainFrame->GetViewCount() == 0)
	{
		//pTFTPServerPane->EnableAllControlls(FALSE);
	}
	////////////////////////////////////////////////////

	/////////////// Load Hw Modules ////////
	if(pHwModulesPane != NULL)
	{
		if(pHwModulesPane->DeSerialize(xmlReader) == FALSE)
		{
			AfxMessageBox("Failed to restore Hardware Modules pane settings!",NULL,MB_ICONERROR);
			return FALSE;
		}
	}
	////////////////////////////////////////////////////

	/////////////// Load command executer files - MUST BE LAST ////////
	if(pCmdExePane != NULL)
	{
		if(pCmdExePane->DeSerialize(xmlReader) == FALSE)
		{
			AfxMessageBox("Failed to restore Command Executer pane settings!",NULL,MB_ICONERROR);
			return FALSE;
		}

	}
	////////////////////////////////////////////////////

	xmlReader.OutOfElem();
	return TRUE;
}


BOOL CMVTerminalApp::SaveConfiguration(CString sFile)
{
	CStdioFile  file;
	CMarkup  	xmlWriter;

	if(file.Open(sFile,CFile::modeCreate | CFile::modeWrite | CFile::typeText ) != TRUE) 
	{
		AfxMessageBox("Failed to open file "+sFile+". Unable to save configuration!",NULL,MB_ICONERROR);
		return FALSE;
	}

	xmlWriter.AddSubDoc( Serialize() );

	file.WriteString(xmlWriter.GetDoc());
	file.Close();

	return TRUE;
}

BOOL CMVTerminalApp::LoadConfiguration(CString sFile)
{
	CMarkup  	xmlReader;
	CMainFrame *pMainFrame = ((CMainFrame*)m_pMainWnd);

	if(xmlReader.Load(sFile) == false)
	{
		return FALSE;
	}
	return DeSerialize(xmlReader);
}

BOOL CMVTerminalApp::GetXmlPlatformInfoFiles(CString sPath, vector<CString> & xmlFilesVec)
{
	CFileFind   xmlCategoryFileFind;
	BOOL		bResult;
	CString     sCategoryFilePath,sDeviceFilesPath,sXmlFileName,sFilePath;

	CMarkup  	xmlReader;
   	if(bResult = xmlCategoryFileFind.FindFile(sPath + "\\*.xml"))
	{
		while(bResult)
		{
			bResult = xmlCategoryFileFind.FindNextFile();
			sCategoryFilePath = (LPCTSTR)xmlCategoryFileFind.GetFilePath();
			if(xmlReader.Load(sCategoryFilePath) == false)
			{
				AfxMessageBox("Failed to load xml file!",MB_ICONERROR);
				return FALSE;
			}	
			if(xmlReader.FindElem("PlatformInfoCategory") == true)
			{
				sDeviceFilesPath = xmlReader.GetAttrib("PlatformInfoFilesPath");
				sDeviceFilesPath.Replace("$PlatformInfoPath$",sPath);
				CFileFind   xmlPlatformInfoFileFind;
   				if(bResult = xmlPlatformInfoFileFind.FindFile(sDeviceFilesPath + "\\*.xml"))
				{
					while(bResult)
					{
						bResult = xmlPlatformInfoFileFind.FindNextFile();
						xmlFilesVec.push_back( (LPCTSTR)xmlPlatformInfoFileFind.GetFilePath() );
					}
				}
				xmlPlatformInfoFileFind.Close();
			}
		}
	}
	xmlCategoryFileFind.Close();
	return TRUE;
}

BOOL CMVTerminalApp::CheckForPlatformInfoNewFileUpdates(vector<CString> & remoteFilesToCopyVec)
{
	CStdioFile  file;
	CString     sSourcePath;

	// Check for new xml platform files
	sSourcePath = "\\\\marvell\\israel\\groups\\eecv\\Utilities\\LabTools\\MVTerminal\\PlatformInfo";
	
	//Search for xml files
	vector<CString> localXmlFilesVec;

	if(GetXmlPlatformInfoFiles(sSourcePath, remoteFilesToCopyVec) == FALSE)
	{
		AfxMessageBox("Failed to load remote xml platform files!",MB_ICONERROR);
		return FALSE;
	}
	
	// Check for new xml platform files
	if(GetXmlPlatformInfoFiles(m_sAppDataPath + "\\PlatformInfo",localXmlFilesVec) == FALSE)
	{
		AfxMessageBox("Failed to load local xml platform files!",MB_ICONERROR);
		return FALSE;
	}

	vector<CString>::iterator lIter, lEndIter = localXmlFilesVec.end();
	for(lIter = localXmlFilesVec.begin(); lIter != lEndIter; lIter++)
	{
		CString sFileName = (*lIter).Right( (*lIter).GetLength() -  (*lIter).ReverseFind('\\') - 1);
		vector<CString>::iterator rIter, rEndIter = remoteFilesToCopyVec.end();
		for(rIter = remoteFilesToCopyVec.begin(); rIter != rEndIter; rIter++)
		{
			if((*rIter).Find(sFileName) != -1)//Found
			{
				remoteFilesToCopyVec.erase(rIter);
				break;
			}
		}
	}

	//remoteFilesToCopyVec contains file to be copied to local folder
	if(remoteFilesToCopyVec.size() == 0)
		return FALSE;

	return TRUE;
}

BOOL CMVTerminalApp::CheckForApplicationUpdates()
{
	CStdioFile  file;
	CMainFrame *pMainFrame = ((CMainFrame*)m_pMainWnd);
	CString     sSourcePath,sCurrentVersion;
	float		fCurrVersion, fNewVersion;

	// Check for updates
	m_sAppUpdateExePath.Empty();
	sSourcePath = "\\\\marvell\\israel\\groups\\eecv\\Utilities\\LabTools\\MVTerminal\\SetupFiles\\Windows_xp_x86\\SoftwareUpdates\\SoftwareUpdates.txt";
					
	if(file.Open(sSourcePath, CFile::modeRead | CFile::typeText ) != TRUE) 
	{
		return FALSE;
	}

	sCurrentVersion = szAppVersion;
	file.ReadString(m_sAppUpdateVersion);
	file.ReadString(m_sAppUpdateExePath);
	file.Close();
	m_sAppUpdateExePath += m_sAppUpdateVersion;
	m_sAppUpdateExePath += ".exe";

	sscanf(sCurrentVersion,"%f",&fCurrVersion);
	sscanf(m_sAppUpdateVersion,"%f",&fNewVersion);

	if(fCurrVersion < fNewVersion)
	{
		return TRUE;
	}		
	m_sAppUpdateExePath.Empty();
	return FALSE;
}



BOOL CMVTerminalApp::ShowNewUpdateMessage()
{
	CMVTerminalUpdateDlg dlg;
	CStdioFile		     file;
	CString              sSourcePath, sChangeLog, sLine;

	// Check for updates
	
	sSourcePath = "\\\\marvell\\israel\\groups\\eecv\\Utilities\\LabTools\\MVTerminal\\SetupFiles\\Windows_xp_x86\\SoftwareUpdates\\ChangeLog.txt";
	if(file.Open(sSourcePath, CFile::modeRead | CFile::typeText ) != TRUE) 
	{
		return FALSE;
	}
	while(file.ReadString(sLine) == TRUE)
	{
		sChangeLog += sLine + "\n";
	}
	file.Close();

	CString sMsg;
	sMsg.Format("New version of MVTerminal is available (Version %s), would you like to update?",m_sAppUpdateVersion);
	dlg.SetNewVersionMsg(sMsg);
	dlg.SetChangeLog(sChangeLog);
	if(dlg.DoModal() == IDOK)
	{
		return TRUE;
	}
	return FALSE;
}
		
BOOL CMVTerminalApp::InitInstance()
{
   //Enable richEdit controls
	AfxInitRichEdit();

    //Enable richEdit2 controls
	AfxInitRichEdit2();

	// InitCommonControls() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	InitCommonControls();

	CWinApp::InitInstance();

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)
	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(IDR_MVTerminalTYPE,
		RUNTIME_CLASS(CMVTerminalDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CMVTerminalView));
	if (!pDocTemplate)
		return FALSE;
	pDocTemplate->SetContainerInfo(IDR_MVTerminalTYPE_CNTR_IP);
	AddDocTemplate(pDocTemplate);
	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	// Set theme
	pMainFrame->SetAppTheme(m_appSettingsDlg.GetAppearanceSettingsTab().GetAppThemeEnum());
	pMainFrame->SetPaneTheme(m_appSettingsDlg.GetAppearanceSettingsTab().GetPaneThemeEnum());


	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;


	// call DragAcceptFiles only if there's a suffix
	//  In an MDI app, this should occur immediately after setting m_pMainWnd
	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo dummyCmdInfo;

	ParseCommandLine(m_cmdInfo);
	
	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	// The main window has been initialized, so show and update it
	pMainFrame->ShowWindow(SW_SHOWMAXIMIZED);
	pMainFrame->UpdateWindow();
	pMainFrame->AddLogo();

	/* Set Application title */
	CString sTitle;
	sTitle  = szAppTitlString;
	sTitle += " v";
	sTitle += szAppVersion;
	pMainFrame->SetAppTitle(sTitle);
	m_pszAppName = szAppVersion;
	pMainFrame->SetWindowTextA(sTitle);

	if(GetAppSettingsDlg().GetGeneralSettingTab().GetCheckForUpdates() == TRUE)
	{
		if(CheckForApplicationUpdates() == TRUE)
		{
			if(ShowNewUpdateMessage() == TRUE)
			{
				ExecApp(m_sAppUpdateExePath);
				m_bCloseUponUpdate = TRUE;
				OnAppExit();
				return TRUE;
			}
		}
		vector<CString> remoteFilesToCopyVec;
		if(CheckForPlatformInfoNewFileUpdates(remoteFilesToCopyVec) == TRUE)
		{
			//Open update platform info files dialog
			CPlatformInfoUpdateDlg dlg;
			dlg.m_filesVec = remoteFilesToCopyVec;
			dlg.DoModal();
		}
	} 

	// Init Hw modules tree
	CHwModulesPane* pHwModulePane = pMainFrame->GetHardwareModulesPane();
	if(pHwModulePane != NULL)
	{
		InitHwModulesModel(FALSE);
	}

	if(InitBoardPowerCtrlManager() == TRUE)
	{
		CBoardPowerCtrlManager::GetInstance().PowerOn();
	}

	if(pHwModulePane->GetDetectHwModulesOnStartup() == TRUE)
	{
		CHwModuleModel::GetInstance().DetectHwModules();
	}

	if(m_cmdInfo.m_strFileName.IsEmpty() == FALSE)
	{
		/* Check if file is mvtc or mvts */
		CString sFileExt;

		sFileExt = m_cmdInfo.m_strFileName.Right(m_cmdInfo.m_strFileName.GetLength() - 
			m_cmdInfo.m_strFileName.ReverseFind('.') - 1);
		sFileExt.MakeLower();
		if(sFileExt == "mvtc")
		{
			return LoadConfiguration(m_cmdInfo.m_strFileName);
		}
	}

	if (!ProcessShellCommand(dummyCmdInfo))
		return FALSE;

	return TRUE;
}

BOOL  CMVTerminalApp::IsBoardResetStartegySet()
{
	if(CBoardPowerCtrlManager::GetInstance().GetResetStartegy() == NULL)
		return FALSE;
	return TRUE;
}

BOOL  CMVTerminalApp::IsBoardPowerStartegySet()
{
	if(CBoardPowerCtrlManager::GetInstance().GetPowerStartegy() == NULL)
		return FALSE;
	return TRUE;
}

VOID CMVTerminalApp::ResetBoardPowerCtrlHwModuleItem()
{
	CBoardPowerCtrlManager::GetInstance().ResetPowerStartegy();
	CBoardPowerCtrlManager::GetInstance().ResetResetStartegy();
}

BOOL CMVTerminalApp::SetBoardPowerCtrlHwModuleItem(CHwModuleItem *pItem)
{
	if(pItem != NULL)
	{
		if(CBoardPowerCtrlManager::GetInstance().SetPowerStartegy(pItem,13,"Off") == FALSE)
		{
			return FALSE;
		}
		if(CBoardPowerCtrlManager::GetInstance().SetResetStartegy(pItem,12,"Off") == FALSE)
		{
			return FALSE;
		}
		return TRUE;
	}			

	return FALSE;
}

BOOL CMVTerminalApp::InitBoardPowerCtrlManager()
{
	vector<CHwModuleCtrl*> hwModuleCtrlList;
	vector<CHwModuleCtrl*>::iterator iter, endIter;

	CHwModuleModel::GetInstance().GetHwModuleCtrlList(hwModuleCtrlList);

	iter    = hwModuleCtrlList.begin();
	endIter = hwModuleCtrlList.end();

	//Use the first SUB-20 found to initialize board power ctrl
	for(; iter != endIter; iter++)
	{
		if((*iter)->GetName() == "SUB-20")
		{
			CHwModuleItem *pItem = (CHwModuleItem*)(*iter)->GetUnit("SUB-20 GPIO");
			return SetBoardPowerCtrlHwModuleItem(pItem);
		}
	}
	return FALSE;
}


class CAboutDlg : public CDialog
{
public:
	CAboutDlg();
	BOOL OnInitDialog();
	CXTHyperLink			 m_wndLogo;
	CBitmap                  m_bmpLogo;


// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
private:
	CXTPButton m_okBtn;
public:
	afx_msg void OnBnClickedOk();
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_okBtn);
}

BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString  appVersionStr;
	appVersionStr = szAppTitlString;
	appVersionStr += " v";
	appVersionStr += szAppVersion;
	SetDlgItemText(IDC_VERSION_STR,appVersionStr); 
	m_okBtn.SetTheme(xtpButtonThemeOffice2000);
	return TRUE;
}


BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
//	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()

// App command to run the dialog
void CMVTerminalApp::OnAppAbout()
{
	CAboutDlg aboutDlg;

    aboutDlg.DoModal();
}

void CMVTerminalApp::OnCheckForUpdates()
{
	if(CheckForApplicationUpdates() == TRUE)
	{
		if(ShowNewUpdateMessage() == TRUE)
		{
			OnSaveConfiguration();
			ExecApp(m_sAppUpdateExePath);
			OnAppExit();
		}
	}
	else
	{
		AfxMessageBox("MVTerminal is up to date!");
	}
}

CMVTerminalView* CMVTerminalApp::GetActiveView()
{
	CChildFrame * pChildFrame = (CChildFrame *)((CMainFrame*)m_pMainWnd)->GetActiveFrame();
	if(pChildFrame == NULL)
		return NULL;

	return (CMVTerminalView *)pChildFrame->GetActiveView();
}

int CMVTerminalApp::GetComPortCount()
{
	vector<CString> portVec;
	EnumerateSerialPorts(portVec);
	return (int)portVec.size();
}


BOOL CMVTerminalApp::CreateNewSession(CViewPropertiesDlg*  pViewPropertiesDlg, BOOL bOpenViewPropertiesDlg)
{
	if(bOpenViewPropertiesDlg)
	{
		// Init ConnectionSettingsTab
		if(pViewPropertiesDlg->DoModal() != IDOK)
		{
			return FALSE;
		}
		if(pViewPropertiesDlg->GetConnectionSettingsTab().IsValid() == FALSE)
		{
			return FALSE;
		}
	}

	CConnectionParams  connectionParams = pViewPropertiesDlg->GetConnectionSettingsTab().GetConnectionParams();

	// Create the document and its frame
	CWinApp::OnFileNew();
	CMVTerminalView *pView = GetActiveView();
	if(pView == NULL)
	{
		AfxMessageBox("Error occured while attempting to retrieve active view!",NULL,MB_ICONERROR);
		return FALSE;
	}
	/* Link the propeties DLG */
	pView->SetPropertiesDlg(pViewPropertiesDlg);

	/* Activate the COM port */
	if(pView->CreateCommCtrl() == FALSE)
	{
		AfxMessageBox("Failed to create COM (RS-232) object",NULL,MB_ICONERROR);
		return FALSE;
	}

	if(pView->ActivateComPort(connectionParams) == FALSE)
	{
		AfxMessageBox("Failed to activate " + connectionParams.GetComPort(),NULL,MB_ICONERROR);
		return FALSE;
	}
	CString sWndName = connectionParams.GetWndName();
	pView->GetConnectionParams().SetWndName(sWndName);
	pView->SetWindowTitle(sWndName);
	
	// Update monitor params
	pView->SetBootLoaderParams(pViewPropertiesDlg->GetBootLoaderParams());

	// Update History settings
	pView->SetMaxHistoryCmdCount(pViewPropertiesDlg->GetCmdHistorySettingsTab().GetMaxHistoryCount());
	pView->SetUseTerminalHistory(pViewPropertiesDlg->GetCmdHistorySettingsTab().GetUseTerminalHistory());
	pView->SetCmdHistoryFilter(pViewPropertiesDlg->GetCmdHistorySettingsTab().GetFilter());
	((CMainFrame*)m_pMainWnd)->AddView(pView);
	return TRUE;

}

void CMVTerminalApp::OnFileNew()
{
	CViewPropertiesDlg*  pViewPropertiesDlg = new CViewPropertiesDlg();

	try 
	{
		if(CreateNewSession(pViewPropertiesDlg) == FALSE)
		{
			delete pViewPropertiesDlg;
		}
	}
	catch(...)
	{
	}
	
	//::PostMessage(((CMainFrame*)m_pMainWnd)->m_hWnd,WM_UPDATE_TAB_COLOR,0,0);
	//::PostMessage(((CMainFrame*)m_pMainWnd)->m_hWnd,WM_UPDATE_TFTP_FILE_LIST,0,0);
}



CString CMVTerminalApp::GetLocalHostIPAddress()
{
	return m_sHostIPAddress;
}

void CMVTerminalApp::SelectLocalHostIPAddress()
{
    CString				sIPAddress;
	CIPAddressListDlg   ipDlg;
	int				    nSize,i;

	nSize = (int)m_ipAddressArray.GetSize();
	if(nSize == 0)
	{
		m_sHostIPAddress = "127.0.0.1";
		return ;
	}
	if(nSize == 1)
	{
		m_sHostIPAddress = m_ipAddressArray[0];
		return;
	}
	for(i = 0; i < nSize; i++)
	{
		ipDlg.AddIPAddress(m_ipAddressArray[i]);
	}
	ipDlg.DoModal();
	m_sHostIPAddress = ipDlg.GetSelection();
}

BOOL CMVTerminalApp::InitLocalHostIPAddressList()
{
    char		  ac[80];

    WSAData wsaData;
    if (WSAStartup(MAKEWORD(1, 1), &wsaData) != 0) 
	{
		AfxMessageBox("Error during attempt to get host IP address",MB_ICONERROR);
	    WSACleanup();
		return FALSE;
    }

    if (gethostname(ac, sizeof(ac)) == SOCKET_ERROR) 
	{
        AfxMessageBox("Error during attempt to get host IP address",MB_ICONERROR);
	    WSACleanup();
        return FALSE;
    }

    struct hostent *phe = gethostbyname(ac);
    if (phe == 0) 
	{
		AfxMessageBox("Error during attempt to get host IP address",MB_ICONERROR);
	    WSACleanup();
        return FALSE;
    }

    for (int i = 0; phe->h_addr_list[i] != 0; ++i) 
	{
        struct   in_addr addr;
		CString  sIPAddress;
		int      nStr;

        memcpy(&addr, phe->h_addr_list[i], sizeof(struct in_addr));
		sIPAddress = inet_ntoa(addr);
		for(nStr = 0; nStr < m_ipAddressArray.GetSize(); nStr++)
		{
			if(sIPAddress == m_ipAddressArray[nStr])
				break;
		}			
		if(nStr == m_ipAddressArray.GetSize())
			m_ipAddressArray.Add(sIPAddress);
    }
	SelectLocalHostIPAddress();
    WSACleanup();
    return TRUE;
}

CString  CMVTerminalApp::GetRegistryEntryValue(CString sEntry)
{
	HKEY    hKey;
    char    szEntryValue[512];
    DWORD   dwBufLen=512;
    LONG	lRet;
	CString	sEntryValue;

	lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SOFTWARE\\Marvell\\Apps\\MVTerminal",
           0, REG_SZ, &hKey );
    if( lRet != ERROR_SUCCESS )
	{
		AfxMessageBox("Registery - Unable to find MVTerminal Installation.",MB_ICONERROR);
		return sEntryValue;
	}
	lRet = RegQueryValueEx( hKey, (LPCSTR)sEntry, NULL, NULL, (LPBYTE) szEntryValue, &dwBufLen);
    if( (lRet != ERROR_SUCCESS) || (dwBufLen > 512) )
	{
		AfxMessageBox("Registery - Unable to find MVTerminal registry entry.", MB_ICONERROR);
		return sEntryValue;
	}
	else
	{
		RegCloseKey( hKey );
		sEntryValue = szEntryValue;
	}
	return sEntryValue;
}


void CMVTerminalApp::OnAppSettings()
{
	if(m_appSettingsDlg.DoModal() != IDOK)
		return;
}


void	CMVTerminalApp::OnFileNewUpdate(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_bEnableNewSessionBtn);
}

void	CMVTerminalApp::SetPowerCtrlResetCheck(int nCheck)
{
	m_nPowerCtrlResetCheck = nCheck;
}

void	CMVTerminalApp::SetPowerCtrlPowerCheck(int nCheck)
{
	m_nPowerCtrlPowerCheck = nCheck;
} 

void	CMVTerminalApp::OnPowerCtrlPowerUpdate(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_nPowerCtrlPowerCheck);
	pCmdUI->Enable(m_bPowerCtrlPowerEn);
} 

void	CMVTerminalApp::EnablePowerCtrlPower(BOOL bEnable)
{
	m_bPowerCtrlPowerEn = bEnable;
}
void	CMVTerminalApp::EnablePowerCtrlReset(BOOL bEnable)
{
	m_bPowerCtrlResetEn = bEnable;
}

void	CMVTerminalApp::OnPowerCtrlResetUpdate(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_nPowerCtrlResetCheck);
	pCmdUI->Enable(m_bPowerCtrlResetEn);
} 

void	CMVTerminalApp::OnPowerCtrlPower()
{
	if(m_nPowerCtrlPowerCheck == 0)
	{
		if(CBoardPowerCtrlManager::GetInstance().PowerOn() == TRUE)
		{
			m_nPowerCtrlPowerCheck = 1;
		}
	}
	else
	{
		if(CBoardPowerCtrlManager::GetInstance().PowerOff() == TRUE)
		{
			m_nPowerCtrlPowerCheck = 0;
		}
	}
}

void	CMVTerminalApp::OnPowerCtrlReset()
{
	CBoardPowerCtrlManager::GetInstance().Reset(250);
}


void CMVTerminalApp::OnExportScripts()
{
	CString			 sDir;
	CString			 sInitialPath;
	CSendScriptPane* pSendScriptPane = ((CMainFrame*)m_pMainWnd)->GetSendScriptPane();

	if(pSendScriptPane == NULL)
		return;

	if(pSendScriptPane->GetScriptCount() == 0)
	{
		AfxMessageBox("You have no script to export",MB_ICONERROR);
		return;
	}
	sInitialPath = _T("C:\\");
	CPathDialog dlg(_T("Select Folder"), _T("Select folder to save your scripts"), sInitialPath);

	if (dlg.DoModal() == IDOK)
	{				
		CString		sFolderPath	= dlg.GetPathName();
		if(pSendScriptPane->ExportScripts(sFolderPath) == FALSE)
		{
			AfxMessageBox("Faild to export scripts",MB_ICONERROR);
		}	
	}
}
void CMVTerminalApp::OnImportScripts()
{
	CString	         sDir;
	CString			 sInitialPath;
	CSendScriptPane* pSendScriptPane = ((CMainFrame*)m_pMainWnd)->GetSendScriptPane();

	if(pSendScriptPane == NULL)
		return;

	sInitialPath = _T("C:\\");
	CPathDialog dlg(_T("Select Folder"), _T("Select folder to load scripts from"), sInitialPath);

	if (dlg.DoModal() == IDOK)
	{				
		CString		sFolderPath	= dlg.GetPathName();
		pSendScriptPane->ImportScripts(sFolderPath);
	}
}

void CMVTerminalApp::OnSendTextFile()
{
	CString	            sDir;
	CString		        sInitialPath;
	CMVTerminalView    *pView = GetActiveView();

	if(pView == NULL)
		return;

	CFileDialog fileDialog(true, NULL, NULL, (OFN_HIDEREADONLY | OFN_FILEMUSTEXIST |OFN_LONGNAMES| OFN_PATHMUSTEXIST), 	_T("Text Files (*.txt)|*.txt|All Files (*.*)|*.*||"));

	if(fileDialog.DoModal() == IDOK)
	{
		pView->LoadTextFile(fileDialog.GetPathName());
	}

}


BOOL CMVTerminalApp::PreTranslateMessage(MSG* pMsg)
{		

	return CWinApp::PreTranslateMessage(pMsg);
}

BOOL CMVTerminalApp::ProcessMessageFilter(int code, LPMSG lpMsg)
{

	return CWinApp::ProcessMessageFilter(code, lpMsg);
}

int CMVTerminalApp::EnumerateSerialPorts(vector<CString> &comPortVec)
{
    CUIntArray ports;
	char       buf[32];
	CString    portStr;

	//Make sure we clear out any elements which may already be in the array
	ports.RemoveAll();

	//Determine what OS we are running on
	OSVERSIONINFO osvi;
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	BOOL bGetVer = GetVersionEx(&osvi);

	//On NT use the QueryDosDevice API
	if (bGetVer && (osvi.dwPlatformId == VER_PLATFORM_WIN32_NT))
	{
		//Use QueryDosDevice to look for all devices of the form COMx. This is a better
		//solution as it means that no ports have to be opened at all.
		TCHAR szDevices[65535];
		DWORD dwChars = QueryDosDevice(NULL, szDevices, 65535);
		if (dwChars)
		{
		int i=0;

		for (;;)
		{
			//Get the current device name
			TCHAR* pszCurrentDevice = &szDevices[i];

			//If it looks like "COMX" then
			//add it to the array which will be returned
			int nLen = (int)_tcslen(pszCurrentDevice);
			if (nLen > 3 && _tcsnicmp(pszCurrentDevice, _T("COM"), 3) == 0)
			{
			//Work out the port number
			int nPort = _ttoi(&pszCurrentDevice[3]);
			ports.Add(nPort);
			}

			// Go to next NULL character
			while(szDevices[i] != _T('\0'))
			i++;

			// Bump pointer to the next string
			i++;

			// The list is double-NULL terminated, so if the character is
			// now NULL, we're at the end
			if (szDevices[i] == _T('\0'))
			break;
		}
		}
		else
		TRACE(_T("Failed in call to QueryDosDevice, GetLastError:%d\n"), GetLastError());
	}
	else
	{
		//On 95/98 open up each port to determine their existence

		//Up to 255 COM ports are supported so we iterate through all of them seeing
		//if we can open them or if we fail to open them, get an access denied or general error error.
		//Both of these cases indicate that there is a COM port at that number. 
		for (UINT i=1; i<256; i++)
		{
		//Form the Raw device name
		CString sPort;
		sPort.Format(_T("\\\\.\\COM%d"), i);

		//Try to open the port
		BOOL bSuccess = FALSE;
		HANDLE hPort = ::CreateFile(sPort, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0);
		if (hPort == INVALID_HANDLE_VALUE)
		{
			DWORD dwError = GetLastError();

			//Check to see if the error was because some other app had the port open or a general failure
			if (dwError == ERROR_ACCESS_DENIED || dwError == ERROR_GEN_FAILURE)
			bSuccess = TRUE;
		}
		else
		{
			//The port was opened successfully
			bSuccess = TRUE;

			//Don't forget to close the port, since we are going to do nothing with it anyway
			CloseHandle(hPort);
		}

		//Add the port number to the array which will be returned
		if (bSuccess)
			ports.Add(i);
		}
	}
	int i;
    for (i=0; i<ports.GetSize(); i++)
	{
		sprintf(buf,"%d",ports.ElementAt(i));
		portStr = buf;
		portStr.Insert(0,"COM");
		portStr.Remove(' ');
		if(portStr.IsEmpty() == FALSE)
			comPortVec.push_back(portStr);
	}
	return i;
}



void CMVTerminalApp::OnFileSave()
{

}

int CMVTerminalApp::ExitInstance()
{	
	return CWinApp::ExitInstance();
}

void CMVTerminalApp::OnAppExit()
{
	CWinApp::OnAppExit();
}

void CAboutDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

CString		CMVTerminalApp::GetCurrentWorkingDir()
{
	char    pathbuffer[512];
	CString	sCWD;

	_getcwd(pathbuffer,512);
	sCWD = pathbuffer;

	return sCWD;
}

void		CMVTerminalApp::SetCurrentWorkingDir(CString sDir)
{
	_chdir((LPCSTR)sDir);
}



	//This will put the current path into pathbuffer up to buflen chars.
