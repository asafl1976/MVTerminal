#include "StdAfx.h"
#include ".\boardpowerctrlsettingdlg.h"
#include "stdafx.h"
#include "MVTerminal.h"
#include "HwModuleModel.h"
#include "IHwModuleContainer.h"
#include "HwModuleCtrlUnit.h"
#include "HwModuleCtrl.h"
#include "SUB20HwModuleCtrlGPIOUnit.h"
#include "SUB20GPIOHwModuleBoardPowerCtrlPowerStartegy.h"
#include "BoardPowerCtrlManager.h"

#include <vector>
using namespace std;

// CAppGeneralSettingDlg dialog

IMPLEMENT_DYNAMIC(CBoardPowerCtrlSettingDlg, CDialog)
CBoardPowerCtrlSettingDlg::CBoardPowerCtrlSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBoardPowerCtrlSettingDlg::IDD, pParent)
{

	// Set installation dir
	m_sAppDataPath = ((CMVTerminalApp*)AfxGetApp())->GetAppDataPath();
	// Load settings from file
	if(ImportSettingsFromFile() == FALSE) 
	{		
		//m_bPromptToSaveSettings = TRUE;
		//m_bCheckForUpdates  = TRUE;
	}
	m_pPowerHwModuleItem = NULL;
	m_pResetHwModuleItem = NULL;
}

void CBoardPowerCtrlSettingDlg::CreateImageList()
{
	//CImageList
	
	m_cImageList.Create(16,16, ILC_COLOR32 , 1, 0);
	CBitmap bmpImage;
	bmpImage.LoadBitmap(IDB_TITLE_BMP); 
	m_cImageList.Add(&bmpImage, (COLORREF)NULL);
	bmpImage.DeleteObject();

	bmpImage.LoadBitmap(IDB_CE_CMD_STATE_BMP);
	m_cImageList.Add(&bmpImage, (COLORREF)NULL);
	bmpImage.DeleteObject();

	m_boardPowerCtrlTree.SetImageList(&m_cImageList, TVSIL_NORMAL);	
	m_boardResetCtrlTree.SetImageList(&m_cImageList, TVSIL_NORMAL);	
}

CBoardPowerCtrlSettingDlg::~CBoardPowerCtrlSettingDlg()
{
	ExportSettingsToFile();
}

void CBoardPowerCtrlSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_POWER_CTRL_TOGGLE_POWER_IO_STATE_COMBO, m_powerIOInitialStateCombo);
	DDX_Control(pDX, IDC_POWER_CTRL_RESET_IO_STATE_COMBO, m_resetIOInitialStateCombo);
	DDX_Control(pDX, IDC_BOARD_POWER_HWMODULE_TREE, m_boardPowerCtrlTree);
	DDX_Control(pDX, IDC_BOARD_RESET_HWMODULE_TREE, m_boardResetCtrlTree);
	DDX_Control(pDX, IDC_POWER_CTRL_TOGGLE_POWER_IO_NUMBER_COMBO, m_powerIONumCombo);
	DDX_Control(pDX, IDC_POWER_CTRL_RESET_IO_NUMBER_COMBO, m_resetIONumCombo);
}

void CBoardPowerCtrlSettingDlg::OnOk()
{
	//Power
	CString	 sIO, sIOInitialState;
	int		 nIO;

	if(m_pPowerHwModuleItem != NULL)
	{
		m_powerIONumCombo.GetWindowText(sIO);
		_stscanf(sIO,"%d",&nIO);

		m_powerIOInitialStateCombo.GetWindowText(sIOInitialState);
		if(sIOInitialState == "Off")
		{
			sIOInitialState = "Off";
		}
		if(CBoardPowerCtrlManager::GetInstance().SetPowerStartegy(m_pPowerHwModuleItem,nIO,sIOInitialState) == FALSE)
		{
			MessageBox("Failed to initlize board power settings!",NULL,MB_ICONERROR);
		}
	}

	//Reset
	if(m_pResetHwModuleItem != NULL)
	{
		m_resetIONumCombo.GetWindowText(sIO);
		_stscanf(sIO,"%d",&nIO);

		m_resetIOInitialStateCombo.GetWindowText(sIOInitialState);
		if(sIOInitialState == "Off")
		{
			sIOInitialState = "Off";
		}
		if(CBoardPowerCtrlManager::GetInstance().SetResetStartegy(m_pResetHwModuleItem,nIO,sIOInitialState) == FALSE)
		{
			MessageBox("Failed to initlize board reset settings!",NULL,MB_ICONERROR);
		}
	}	
}


BEGIN_MESSAGE_MAP(CBoardPowerCtrlSettingDlg, CDialog)
	ON_NOTIFY(NM_CLICK, IDC_BOARD_POWER_HWMODULE_TREE, OnNMClickBoardPowerHwmoduleTree)
	ON_NOTIFY(NM_CLICK, IDC_BOARD_RESET_HWMODULE_TREE, OnNMClickBoardResetHwmoduleTree)
END_MESSAGE_MAP()


// CAppGeneralSettingDlg message handlers

BOOL CBoardPowerCtrlSettingDlg::ImportSettingsFromFile()
{
	CString			sLine;
	CStdioFile		cfgFile;

	if(cfgFile.Open(m_sAppDataPath +  "\\boardPowerCtrlSettingDlg.ini", 
		              CFile::modeRead | CFile::typeText ) == FALSE) 
		return FALSE;

	cfgFile.ReadString(sLine);
	if(sLine == "[START]")
	{	
		// Zero all members
				

		//[END]
		cfgFile.ReadString(sLine);
		if(sLine != "[END]")
		{
			cfgFile.Close();
			return FALSE;
		}
		cfgFile.Close();
		return TRUE;
	}
	else
	{
		cfgFile.Close();
		return FALSE;
	}
}
BOOL CBoardPowerCtrlSettingDlg::ExportSettingsToFile()
{
	CStdioFile  cfgFile;

	// Load settings from file
	if(cfgFile.Open(m_sAppDataPath +  "\\boardPowerCtrlSettingDlg.ini", 
		              CFile::modeCreate | CFile::modeWrite | CFile::typeText ) != TRUE) 
	{
		return FALSE;
	}


	CString sLine;
	/*
	[START]
	m_bPromptToSaveSettings=
	m_bPromptToSaveSession=
	m_bCheckForUpdates=
	[END]
	*/
	sLine = "[START]";
	cfgFile.WriteString(sLine + "\n");

	cfgFile.WriteString("[END]\n");

	cfgFile.Close();
	return TRUE; 
}

BOOL CBoardPowerCtrlSettingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CreateImageList();

	m_pPowerHwModuleItem = NULL;
	m_pResetHwModuleItem = NULL;

	EnableResetControls(FALSE);
	EnablePowerControls(FALSE);

	InitHwModuleTreeView();


	return FALSE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CBoardPowerCtrlSettingDlg::EnableResetControls(BOOL bEnable)
{
	m_resetIOInitialStateCombo.EnableWindow(bEnable);
	m_resetIONumCombo.EnableWindow(bEnable);
}

void CBoardPowerCtrlSettingDlg::EnablePowerControls(BOOL bEnable)
{
	m_powerIOInitialStateCombo.EnableWindow(bEnable);
	m_powerIONumCombo.EnableWindow(bEnable);
}

void CBoardPowerCtrlSettingDlg::AddHwModuleContainer(CTreeCtrl* pTreeCtrl, IHwModuleContainer* pHwModuleContainer,CString sName,HTREEITEM	hParent)
{
	HTREEITEM           hParentContainer;
	CHwModule*          pHwModule;
	int				    nHwModule, nHwModuleCount;
	IHwModuleContainer* pSubContainer;

	//Add container

	hParentContainer = pTreeCtrl->InsertItem(sName,0,0,hParent); 
	nHwModuleCount = pHwModuleContainer->GetHwModuleCount();
	for(nHwModule = 0; nHwModule < nHwModuleCount ; nHwModule++)
	{
		pHwModule = pHwModuleContainer->GetHwModule(nHwModule);
		pSubContainer = dynamic_cast<IHwModuleContainer*>(pHwModule);
		if(pSubContainer != NULL)
		{
			//Recursivelly add all hw modules
			AddHwModuleContainer(pTreeCtrl,pSubContainer,pHwModule->GetDisplayName(),hParentContainer);
		}
		else
		{
			if(pHwModule->GetName() == "IO Card")
			{
				HTREEITEM  hItem;
				hItem = pTreeCtrl->InsertItem(pHwModule->GetDisplayName(),1,1,hParent); 
				pTreeCtrl->SetItemData(hItem,(DWORD_PTR)pHwModule);
			}
		}
	}
}
void CBoardPowerCtrlSettingDlg::AddHwModuleCtrlUnit(CTreeCtrl* pTreeCtrl, CHwModuleCtrlUnit* pUnit,HTREEITEM	hParent)
{
	IHwModuleContainer* pHwModuleContainer;

	if(pUnit->GetDisplayName() == "GPIO")
	{
		HTREEITEM  hItem;
		hItem = pTreeCtrl->InsertItem(pUnit->GetDisplayName(),1,1,hParent); 
		pTreeCtrl->SetItemData(hItem,(DWORD_PTR)pUnit);
	}
	pHwModuleContainer = dynamic_cast<IHwModuleContainer*>(pUnit);
	if(pHwModuleContainer != NULL)
	{
		//Recursivelly add all hw modules
		AddHwModuleContainer(pTreeCtrl,pHwModuleContainer,pUnit->GetDisplayName(),hParent);
	}
}

void CBoardPowerCtrlSettingDlg::AddHwModuleCtrl(CTreeCtrl* pTreeCtrl, CHwModuleCtrl* pCtrl)
{
	HTREEITEM			hParent;
	int				    nUnit, nUnitCount = pCtrl->GetUnitCount();
	CHwModuleCtrlUnit*  pUnit;

	hParent = pTreeCtrl->InsertItem(pCtrl->GetDisplayName() + " (" + pCtrl->GetUniqueID() + ")",0,0,TVI_ROOT); 
	for(nUnit = 0; nUnit < nUnitCount ; nUnit++)
	{
		pUnit = pCtrl->GetUnit(nUnit);
		AddHwModuleCtrlUnit(pTreeCtrl,pUnit,hParent);
	}
	pTreeCtrl->Expand(hParent,TVE_EXPAND);
}

void CBoardPowerCtrlSettingDlg::InitHwModuleTreeView()
{
	//Add HwModules containers 
	vector<CHwModuleCtrl*>            hwModuleCtrlList;
	vector<CHwModuleCtrl*>::iterator  iter, endIter;

	CHwModuleModel::GetInstance().GetHwModuleCtrlList(hwModuleCtrlList);

	iter    = hwModuleCtrlList.begin();
	endIter = hwModuleCtrlList.end();

	for(;iter != endIter; iter++)
	{
		AddHwModuleCtrl(&m_boardPowerCtrlTree,(*iter));
		AddHwModuleCtrl(&m_boardResetCtrlTree,(*iter));
	}
}

HTREEITEM CBoardPowerCtrlSettingDlg::GetMouseClickedItem(CTreeCtrl *pTreeCtrl, NMHDR *pNMHDR, LRESULT *pResult)
{
     CPoint     point;
	 UINT       uFlags;
     HTREEITEM  item = NULL;

	 GetCursorPos(&point);   
	 pTreeCtrl->ScreenToClient(&point);
	 item = pTreeCtrl->HitTest(point, &uFlags);
	 if(item)
	 { 
         // There is an item under the cursor. 
		 // See what exactly was under the cursor:
		 switch(uFlags)
		 {
		 case TVHT_ONITEMSTATEICON:
	 		break;
	  	 case TVHT_ONITEMBUTTON:
			break;
		 case TVHT_ONITEMLABEL:
			return item;
		 default:
			break;
	   	}
	 }	
	 *pResult = 0;
	 return NULL;
}



void CBoardPowerCtrlSettingDlg::OnNMClickBoardPowerHwmoduleTree(NMHDR *pNMHDR, LRESULT *pResult)
{
     HTREEITEM			  hItem = NULL;

 	 EnablePowerControls(FALSE);

	 m_pPowerHwModuleItem = NULL;

	 if((hItem = GetMouseClickedItem(&m_boardPowerCtrlTree, pNMHDR,pResult)) == NULL)
		 return;

	 CHwModuleItem* pHwModuleItem = (CHwModuleItem*)m_boardPowerCtrlTree.GetItemData(hItem);
	 if(pHwModuleItem == NULL)
	 {
		 return;
	 }
	 CSUB20HwModuleCtrlGPIOUnit* pGPIOUnit = dynamic_cast<CSUB20HwModuleCtrlGPIOUnit*>(pHwModuleItem);
	 if(pGPIOUnit != NULL)
	 {
		 m_pPowerHwModuleItem = pGPIOUnit;
		 m_powerIONumCombo.ResetContent();

		 int nIOCount = pGPIOUnit->GetGPIOCount();
		 for(int nIO = 0; nIO < nIOCount; nIO++)
		 {
			 CString sIO;
			 sIO.Format("%d",nIO);
			 m_powerIONumCombo.AddString(sIO);
		 }

		 //Select defaults
		 m_powerIOInitialStateCombo.SelectString(-1,"Off");
		 m_powerIONumCombo.SelectString(-1,"13");

	 	 EnablePowerControls(TRUE);
		 m_powerIOInitialStateCombo.EnableWindow(FALSE);

	 }
	

	*pResult = 0;
}

void CBoardPowerCtrlSettingDlg::OnNMClickBoardResetHwmoduleTree(NMHDR *pNMHDR, LRESULT *pResult)
{
     HTREEITEM			  hItem = NULL;

	 EnableResetControls(FALSE);

	 m_pResetHwModuleItem = NULL;

	 if((hItem = GetMouseClickedItem(&m_boardResetCtrlTree, pNMHDR,pResult)) == NULL)
		 return;

	 CHwModuleItem* pHwModuleItem = (CHwModuleItem*)m_boardResetCtrlTree.GetItemData(hItem);
	 if(pHwModuleItem == NULL)
	 {
		 return;
	 }
	 CSUB20HwModuleCtrlGPIOUnit* pGPIOUnit = dynamic_cast<CSUB20HwModuleCtrlGPIOUnit*>(pHwModuleItem);
	 if(pGPIOUnit != NULL)
	 {
		 m_pResetHwModuleItem = pGPIOUnit;

		 m_resetIONumCombo.ResetContent();
		 m_resetIOInitialStateCombo.ResetContent();

		 int nIOCount = pGPIOUnit->GetGPIOCount();
		 for(int nIO = 0; nIO < nIOCount; nIO++)
		 {
			 CString sIO;
			 sIO.Format("%d",nIO);
			 m_resetIONumCombo.AddString(sIO);
		 }

		 //Select defaults
		 m_resetIONumCombo.SelectString(-1,"12");
	 	 EnableResetControls(TRUE);
		 m_resetIOInitialStateCombo.EnableWindow(FALSE);

	 }
	 

	*pResult = 0;
}
