// CommandExecuterPane.cpp : implementation file
//

#include "stdafx.h"
#include "MVTerminal.h"
#include "CmdExePane.h"
#include ".\CmdExePane.h"
#include "CmdExeCmdItem.h"
#include "CmdExeItemDlgFactory.h"
#include "CmdExeCmdItem.h"


#define  _WM_USER              		 2500
#define  _ADD_STATE_MENU_START 		(_WM_USER+12)
#define  ID_CE_COPY_ITEM	   		(_WM_USER+13)
#define  ID_CE_PASTE_ITEM	   		(_WM_USER+14)
#define  ID_CE_DUPLICATE_ITEM  		(_WM_USER+15)
#define  ID_CE_REMOVE_ITEM     		(_WM_USER+16)
#define  ID_CE_SAVE_CMD     		(_WM_USER+17)
#define  ID_CE_LOAD_CMD        		(_WM_USER+18)
#define  ID_CE_SET_CMD_EXE_SEQ      (_WM_USER+20)
#define  ID_CE_CLR_LOG_WIN          (_WM_USER+21)
#define  ID_CE_OPEN_LOG_WIN_NOTEPAD (_WM_USER+22)
#define  ID_CE_LOG_WIN_TO_FILE	    (_WM_USER+23)

   

// CCmdExePane

IMPLEMENT_DYNCREATE(CCmdExePane, CFormView)

CCmdExePane::CCmdExePane(): CFormView(CCmdExePane::IDD)
{
	m_bInfiniteExe     = FALSE;
	m_nLoopCount       = 1;
	m_pCmdExeModel     = &CCmdExeModel::GetInstance();
	m_pCmdExePresenter = new CCmdExePresenter(m_pCmdExeModel, &m_cmdTreeView, &m_cmdLogView,this);

}

CCmdExePane::~CCmdExePane()
{
	delete m_pCmdExePresenter;
}

void CCmdExePane::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CE_NEW_CMD_BTN, m_newCmdBtn);
	DDX_Control(pDX, IDC_CE_REMOVE_CMD_BTN, m_removeCmdBtn);
	DDX_Control(pDX, IDC_CE_REMOVE_ALL_CMD_BTN, m_removeAllBtn);
	DDX_Control(pDX, IDC_CE_FILE_CMD_BTN, m_fileBtn);
	DDX_Control(pDX, IDC_CE_EXECUTE_BTN, m_executeBtn);
	DDX_Control(pDX, IDC_CE_STATUS_WINDOW_EDIT, m_cmdLogView);
	DDX_Control(pDX, IDC_CE_CMD_TREE, m_cmdTreeView);
	DDX_Control(pDX, IDC_CE_MORE_OPTIONS_BTN,m_moreOptionsBtn);
}

BEGIN_MESSAGE_MAP(CCmdExePane, CFormView)
	ON_BN_CLICKED(IDC_CE_NEW_CMD_BTN, OnBnClickedCeNewCmdBtn)
	ON_BN_CLICKED(IDC_CE_REMOVE_CMD_BTN, OnBnClickedCeRemoveBtn)
	ON_BN_CLICKED(IDC_CE_FILE_CMD_BTN, OnBnClickedCeFileCmdBtn)
	ON_BN_CLICKED(IDC_CE_REMOVE_ALL_CMD_BTN, OnBnClickedCeRemoveAllCmdBtn)
	ON_BN_CLICKED(IDC_CE_EXECUTE_BTN, OnBnClickedCeExecuteBtn)
	ON_BN_CLICKED(IDC_CE_INFINITE_LOOP_CHECK, OnBnClickedCeInfiniteLoopCheck)
	ON_EN_CHANGE(IDC_CE_LOOP_COUNT_EDIT, OnEnChangeCeLoopCountEdit)
	ON_BN_CLICKED(IDC_CE_MORE_OPTIONS_BTN, OnBnClickedCeMoreOptionsBtn)
	ON_NOTIFY(NM_DBLCLK, IDC_CE_CMD_TREE, OnNMDblclkCeCmdTree)
	ON_NOTIFY(NM_CLICK, IDC_CE_CMD_TREE, OnNMClickCeCmdTree)
	ON_NOTIFY(NM_RCLICK, IDC_CE_CMD_TREE, OnNMRclickCeCmdTree)
END_MESSAGE_MAP()


// CCmdExePane diagnostics

#ifdef _DEBUG
void CCmdExePane::AssertValid() const
{
	CFormView::AssertValid();
}

void CCmdExePane::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG


CString	CCmdExePane::Serialize()
{
	CMarkup		xmlWriter;

	xmlWriter.AddElem("CmdExePaneSettings");//root
	xmlWriter.AddAttrib("InfiniteExe",m_bInfiniteExe);
	xmlWriter.AddAttrib("LoopCount",m_nLoopCount);
	xmlWriter.IntoElem();

	if(m_cmdTreeView.SaveCommands(xmlWriter) == FALSE)
	{
		return NULL;
	}
	xmlWriter.OutOfElem();
	return xmlWriter.GetDoc();
}

BOOL	CCmdExePane::DeSerialize(CMarkup& xmlReader)
{
	if(xmlReader.FindElem("CmdExePaneSettings") == false)
	{
		return FALSE;
	}
	//First, remove all current commands
	RemoveAllCommands();

	CString sLoopCount;
	m_bInfiniteExe = (xmlReader.GetAttrib("InfiniteExe") == "0")? FALSE:TRUE;
	sLoopCount = xmlReader.GetAttrib("LoopCount");
	sscanf(sLoopCount,"%d",&m_nLoopCount);
	xmlReader.IntoElem();

	if(m_cmdTreeView.LoadCommands(xmlReader) == FALSE)
	{
		return FALSE;
	}
	
	if(GetCommandCount() > 0)
	{
		m_executeBtn.EnableWindow(TRUE);
	}
	
	xmlReader.OutOfElem();
	return TRUE;
}

void CCmdExePane::SaveCommands()
{
	char strFilter[] = { "XML Files (*.xml)|*.xml|All Files (*.*)|*.*||" };
	CFileDialog fileDlg(FALSE, ".xml", "CmdExeCommands.xml", OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, strFilter);

	if(fileDlg.DoModal() == IDOK)
	{
		CString	 sFilePath;
		sFilePath = fileDlg.GetPathName();
		if(sFilePath.IsEmpty() == FALSE)
		{
			CStdioFile  file;
			CMarkup		xmlWriter;

			if(file.Open(sFilePath,CFile::typeText | CFile::modeCreate|CFile::modeWrite) == FALSE)
			{
				AfxMessageBox("Failed to open file " + sFilePath + "!",NULL,MB_ICONERROR);
				return;
			}

			if(m_cmdTreeView.SaveCommands(xmlWriter) == FALSE)
			{
				AfxMessageBox("Error while saving commands!",NULL,MB_ICONERROR);
				return;
			}

			file.WriteString( xmlWriter.GetDoc() );
			file.Close();
		}
	}
}

void CCmdExePane::LoadCommands()
{
	char strFilter[] = { "XML Files (*.xml)|*.xml|All Files (*.*)|*.*||" };
	CFileDialog fileDlg(TRUE, ".xml", "CmdExeCommands.xml", OFN_HIDEREADONLY, strFilter);

	if(fileDlg.DoModal() == IDOK)
	{
		CString	 sFilePath;
		sFilePath = fileDlg.GetPathName();
		if(sFilePath.IsEmpty() == FALSE)
		{
			CMarkup xmlReader;

			//Open and load the file
			if(xmlReader.Load(sFilePath) == false)
			{
				AfxMessageBox("Error while loading file!",NULL,MB_ICONERROR);
				return;
			}
    		if(m_cmdTreeView.LoadCommands(xmlReader) == FALSE)
			{
				AfxMessageBox("Error while loading settings!",NULL,MB_ICONERROR);
				return;
			}
			if(GetCommandCount() != 0)
			{
				EnableControls(TRUE);
			}
		}
	}
}

BOOL CCmdExePane::IsCommandNameExists(CString sName)
{
	vector<CString>::iterator iter, endIter;

	iter    = m_cmdNameVec.begin();
	endIter = m_cmdNameVec.end();
	for(; iter != endIter; iter++)
	{
		if(sName == (*iter))
		{
			return TRUE;
		}
	}
	return FALSE;
}


// CCmdExePane message handlers

void CCmdExePane::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	
	m_newCmdBtn.SetTheme(xtpButtonThemeOffice2000);
	m_removeCmdBtn.SetTheme(xtpButtonThemeOffice2000);
	m_removeAllBtn.SetTheme(xtpButtonThemeOffice2000);
	m_fileBtn.SetTheme(xtpButtonThemeOffice2000);
	m_moreOptionsBtn.SetTheme(xtpButtonThemeOffice2000);
	m_executeBtn.SetTheme(xtpButtonThemeOffice2000);
	m_executeBtn.SetBitmap(CSize(24,24),IDB_CE_EXE );

	m_cmdLogView.SetReadOnly();

	CString sLC;
	sLC.Format("%d",m_nLoopCount);
	SetDlgItemText(IDC_CE_LOOP_COUNT_EDIT,sLC);
	
	m_cmdTreeView.CreateImageList();

	EnableControls(FALSE);

	if(GetCommandCount() > 0)
	{
		EnableControls(TRUE);
	}
}




CString CCmdExePane::GetNewCommandName()
{
	CString sName;
	int		nRetry = 0;

	while(1) 
	{
		sName.Format("Cmd#%d",nRetry);
		if(IsCommandNameExists(sName) == FALSE)
			break;
		nRetry++;
	}
	return sName;
}

CCmdExeCmd* CCmdExePane::CreateNewCommand()
{	
	CCmdExeCmd *pCmd = new CCmdExeCmd();
	CString		sCmdName;

	sCmdName = GetNewCommandName();
	pCmd->SetDisplayName(sCmdName);
	return pCmd;
}

void CCmdExePane::OnBnClickedCeNewCmdBtn()
{
	CCmdExeCmd *pCmd;
 
	if((pCmd = CreateNewCommand()) == NULL)
	{
		return;
	}

	if(ShowCmdDlg(pCmd) == TRUE)
	{			
		if(pCmd->GetCmdItemCount() != 0)
		{
			m_cmdTreeView.AddCommand(pCmd);
			AddCmdName(pCmd->GetDisplayName());
			EnableControls(TRUE);
			return;
		}
		delete pCmd;
	}
}


HTREEITEM CCmdExePane::GetMouseClickedItem(NMHDR *pNMHDR, LRESULT *pResult)
{
     CPoint     point;
	 UINT       uFlags;
     HTREEITEM  item = NULL;

	 GetCursorPos(&point);   
	 m_cmdTreeView.ScreenToClient(&point);
	 item = m_cmdTreeView.HitTest(point, &uFlags);
	 if(item)
	 { 
		 // Select the item
 		 m_cmdTreeView.SelectItem(item);
         // There is an item under the cursor. 
		 // See what exactly was under the cursor:
		 switch(uFlags)
		 {
		 case TVHT_ONITEMSTATEICON:
			 break;
	  	 case TVHT_ONITEMBUTTON:
			break;
		 case TVHT_ONITEMLABEL:
			 //Return item only if the click was on the Text
			 return item;
		 default:
			break;
	   	}
	 }	
	 return NULL;
}

void CCmdExePane::AddCmdName(CString sName)
{
	if(IsCommandNameExists(sName) == FALSE)
	{
		m_cmdNameVec.push_back(sName);
	}
}

void CCmdExePane::RemoveCmdName(CString sName)
{
	vector<CString>::iterator iter, endIter;

	iter    = m_cmdNameVec.begin();
	endIter = m_cmdNameVec.end();
	for(; iter != endIter; iter++)
	{
		if(sName == (*iter))
		{
			m_cmdNameVec.erase(iter);
			break;
		}
	}
}

void CCmdExePane::EnableControls(BOOL bEn)
{
	m_removeCmdBtn.EnableWindow(bEn);
	m_removeAllBtn.EnableWindow(bEn);
	m_executeBtn.EnableWindow(bEn);
}

void CCmdExePane::RemoveAllCommands()
{
	OnBnClickedCeRemoveAllCmdBtn();
	m_cmdNameVec.clear();
}

void CCmdExePane::OnBnClickedCeRemoveAllCmdBtn()
{
	if(m_cmdTreeView.RemoveAllCommands() == FALSE)
	{
		MessageBox("Failed to remove all commands!",NULL,MB_ICONERROR);
		return;
	}
	EnableControls(FALSE);
}

void CCmdExePane::OnBnClickedCeRemoveBtn()
{
	HTREEITEM	  hItem = NULL;
	CCmdExeItem *pCmdExeItem;

	hItem = m_cmdTreeView.GetSelectedItem();
	if(hItem != NULL)
	{			 
		pCmdExeItem = (CCmdExeItem*)m_cmdTreeView.GetItemData(hItem);
		CCmdExeCmd* pCmd = dynamic_cast<CCmdExeCmd*>(pCmdExeItem);
		if(pCmd != NULL)
		{
			CString sCmdName;
			sCmdName = pCmd->GetDisplayName();
			if(m_cmdTreeView.RemoveCommand(pCmd) == FALSE)
			{
				MessageBox("Failed to remove commands!",NULL,MB_ICONERROR);
				return;
			} 
			RemoveCmdName(sCmdName);
			if(m_cmdTreeView.GetCommandCount() == 0)
			{
				EnableControls(FALSE);

			}
		}
	}
}



void CCmdExePane::OnBnClickedCeFileCmdBtn()
{

	HTREEITEM	 hItem = NULL;
	CPoint		 point;
	CNewMenu	 itemMenu;
	int			 mMenuItem =0;
	CRect		 rect;
	BOOL		 bSaveCmdEn = 0;

	m_fileBtn.GetWindowRect(rect);

	int x = rect.right ;
	int y = rect.bottom - 27;
    itemMenu.CreatePopupMenu();
	if(m_cmdTreeView.GetCommandCount() == 0)
	{
		bSaveCmdEn = MF_GRAYED|MF_ENABLED;
	}
    itemMenu.InsertMenu(mMenuItem++, MF_BYPOSITION|bSaveCmdEn, ID_CE_SAVE_CMD ,  "Save Commands...");
	itemMenu.InsertMenu(mMenuItem++, MF_BYPOSITION, ID_CE_LOAD_CMD ,  "Load Commands...");
	itemMenu.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, x , y , this);
}

void CCmdExePane::OnEndExecution()
{
	if(CCmdExeModel::GetInstance().StopExecution() == FALSE)
	{
		MessageBox("Failed to stop execution!!",NULL,MB_ICONERROR);
		return;
	}
	m_executeBtn.SetWindowText("Execute");
	m_newCmdBtn.EnableWindow(TRUE);
	m_fileBtn.EnableWindow(TRUE);

	if(m_cmdTreeView.GetSelectedItem() != NULL)
	{
		m_removeCmdBtn.EnableWindow(TRUE);
	}
	m_removeAllBtn.EnableWindow(TRUE);
}



void CCmdExePane::OnStartExecution()
{
	CString  sLoopCount;

	m_executeBtn.SetWindowText("Stop");
	m_newCmdBtn.EnableWindow(FALSE);
	m_removeAllBtn.EnableWindow(FALSE);
	m_removeCmdBtn.EnableWindow(FALSE);
	m_fileBtn.EnableWindow(FALSE);

	SetDlgItemText(IDC_CE_LOOP_COUNT_STATIC,"0");
	GetDlgItemText(IDC_CE_LOOP_COUNT_EDIT,sLoopCount);
	if(sLoopCount.IsEmpty())
	{
		sLoopCount = "1";	
   		SetDlgItemText(IDC_CE_LOOP_COUNT_EDIT,sLoopCount);
		m_nLoopCount = 1;
	}
}

BOOL CCmdExePane::ExecuteCommands()
{
	return CCmdExeModel::GetInstance().ExecuteCommands();
}

UINT ExecuteCommandsThread(LPVOID pParam)
{
	CCmdExePane  *pPane = (CCmdExePane*)pParam;
	pPane->ExecuteCommands();
	pPane->OnEndExecution();
	return 0;
}

void CCmdExePane::SetLoopIndex(int nLoopIndex)
{
	CString sLoopIndex;

	sLoopIndex.Format("%d",nLoopIndex);
	SetDlgItemText(IDC_CE_LOOP_COUNT_STATIC,sLoopIndex);

}
void CCmdExePane::SetLoopCount(int nLoopCount)
{
	CString sLoopCount;

	sLoopCount.Format("%d",nLoopCount);
	SetDlgItemText(IDC_CE_LOOP_COUNT_EDIT,sLoopCount);
}

void CCmdExePane::OnBnClickedCeExecuteBtn()
{
	CString sWinText;
	m_executeBtn.GetWindowText(sWinText);
	if(sWinText == "Execute")
	{
		OnStartExecution();
		AfxBeginThread(ExecuteCommandsThread,this);
	}
	else
	{
		OnEndExecution();
	}
}



void CCmdExePane::HandleMenuSelection(UINT nMenuIndex)
{	

	switch(nMenuIndex)
	{

	case ID_CE_SET_CMD_EXE_SEQ:
//		SetExeSeq();
		return;
	case ID_CE_CLR_LOG_WIN:
		m_cmdLogView.ClearLogWindow();
		return;
	case ID_CE_OPEN_LOG_WIN_NOTEPAD:
		m_cmdLogView.OpenLogWinNotepad();
		return;
	case ID_CE_LOG_WIN_TO_FILE:
		if(m_cmdLogView.GetCaptureToLogFile() == FALSE)
		{
			m_cmdLogView.OpenLogWinBrowseDlg();
		}
		else
		{
			m_cmdLogView.SetCaptureToLogFile(FALSE);
		}
		return;
		//Save/Load commands
	case ID_CE_SAVE_CMD:
		SaveCommands();
		return;
	case ID_CE_LOAD_CMD:
		LoadCommands();
		return;
	default:
		break;
	}

	HTREEITEM item = m_cmdTreeView.GetSelectedItem();
	if(item == NULL)
	{
        AfxMessageBox("Unable to get command!",NULL,MB_ICONERROR);
		return;
	}

	switch(nMenuIndex)
	{
	case ID_CE_COPY_ITEM:
		CopyCmd(item);
		break;
	case ID_CE_PASTE_ITEM:
		PasteCmd(item);
		break;
	case ID_CE_DUPLICATE_ITEM:
		CopyCmd(item);
		PasteCmd(item);
    	break;
	case ID_CE_REMOVE_ITEM:
		OnBnClickedCeRemoveBtn();
		break;
	default:
		break;
	}

}

BOOL CCmdExePane::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_COMMAND)
	{			
		//The high-order word specifies the notification code if the message is from a control. 
		//If the message is from an accelerator, this value is 1. If the message is from a menu, 
		//this value is zero. 
		//The low-order word specifies the identifier of the menu item, control, or accelerator.
		if(((((UINT)pMsg->wParam)>>16) & 0xFFFF) == 0)
		{
			HandleMenuSelection((UINT)pMsg->wParam & 0xFFFF);
		}
	}
	return CFormView::PreTranslateMessage(pMsg);
}

void CCmdExePane::OnBnClickedCeInfiniteLoopCheck()
{
	CString sLoopCount = "1";

	m_bInfiniteExe = !m_bInfiniteExe;
	if(m_bInfiniteExe == TRUE)
	{
		sLoopCount = "-1";	
	}
	SetDlgItemText(IDC_CE_LOOP_COUNT_EDIT,sLoopCount);
	GetDlgItem(IDC_CE_LOOP_COUNT_EDIT)->EnableWindow(!m_bInfiniteExe);

	CCmdExeModel::GetInstance().SetInfiniteExecution(m_bInfiniteExe);
}

void CCmdExePane::OnEnChangeCeLoopCountEdit()
{
	CString sLoopCount;	

    GetDlgItemText(IDC_CE_LOOP_COUNT_EDIT,sLoopCount);
	if(sLoopCount.IsEmpty() == FALSE)
	{
		sscanf(sLoopCount,"%d",&m_nLoopCount);
	}
	CCmdExeModel::GetInstance().SetExecutionLoopCount(m_nLoopCount);
}



int  CCmdExePane::GetCommandCount()
{
	return CCmdExeModel::GetInstance().GetCommandCount();
	//return m_cmdTreeView.GetCommandCount();
}


void CCmdExePane::OnBnClickedCeMoreOptionsBtn()
{
	HTREEITEM	 hItem = NULL;
	CPoint		 point;
	CNewMenu	 itemMenu;
	int			 mMenuItem =0, bCmdSeqExeCheck = 0, bCaptureToLogFileCheck = 0;
	CRect		 rect;

	m_moreOptionsBtn.GetWindowRect(rect);

	int x = rect.right ;
	int y = rect.bottom - 30;
    itemMenu.CreatePopupMenu();

	//TODO: On demand, add support for sequance exe
	//if(m_seqExeDlg.GetEnable() == TRUE)
	//	bCmdSeqExeCheck = MF_CHECKED;

    //itemMenu.InsertMenu(mMenuItem++, MF_BYPOSITION|bCmdSeqExeCheck, ID_CE_SET_CMD_EXE_SEQ ,  "Custom command execution...");
	//itemMenu.InsertMenu(mMenuItem++, MF_BYPOSITION|MF_SEPARATOR);
	itemMenu.InsertMenu(mMenuItem++, MF_BYPOSITION, ID_CE_CLR_LOG_WIN ,  "Clear log window");
	itemMenu.InsertMenu(mMenuItem++, MF_BYPOSITION, ID_CE_OPEN_LOG_WIN_NOTEPAD ,  "Dump log window to notepad");
	
	if(m_cmdLogView.GetCaptureToLogFile() == TRUE)
		bCaptureToLogFileCheck = MF_CHECKED;
	itemMenu.InsertMenu(mMenuItem++, MF_BYPOSITION|bCaptureToLogFileCheck, ID_CE_LOG_WIN_TO_FILE ,  "Capture output to file...");
	
	itemMenu.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, x , y , this);
}

BOOL CCmdExePane::ShowCmdDlg(CCmdExeCmd *pCmd)
{
	CCmdExeCmdSettingsDlg dlg(pCmd,this);

	if(dlg.DoModal() == IDOK)
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CCmdExePane::ShowCmdItemDlg(CCmdExeCmdItem *pCmdItem)
{
	CBasicItemDlg  *pDlg;

	pDlg = CCmdExeItemDlgFactory::GetInstance().Create(pCmdItem);
	if(pDlg == NULL)
	{
		return FALSE;
	}
	if(pDlg->DoModal() == IDOK)
	{
		return TRUE;
	}
	return FALSE;
}

void CCmdExePane::OnNMDblclkCeCmdTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	 HTREEITEM	  hItem = NULL;
	 CCmdExeItem *pCmdExeItem;

	 if((hItem = GetMouseClickedItem(pNMHDR,pResult)) == NULL)
		 return;

	 pCmdExeItem = (CCmdExeItem*)m_cmdTreeView.GetItemData(hItem);
	 if(pCmdExeItem != NULL)
	 {			
		CCmdExeCmdItem* pCmdExeCmdItem = dynamic_cast<CCmdExeCmdItem*>(pCmdExeItem);
		//Command item
		if(pCmdExeCmdItem != NULL)
		{
			if(ShowCmdItemDlg(pCmdExeCmdItem) == TRUE)
			{
				m_cmdTreeView.SetItemText(hItem,pCmdExeCmdItem->GetDisplayName());
			}
		    *pResult = 0;
			return;
		}
		CCmdExeCmd* pCmdExeCmd = dynamic_cast<CCmdExeCmd*>(pCmdExeItem);
		if(pCmdExeCmd != NULL)
		{
			if(ShowCmdDlg(pCmdExeCmd) == TRUE)
			{
				m_cmdTreeView.UpdateCommand(pCmdExeCmd);
				AddCmdName(pCmdExeCmd->GetName());
			}
		    *pResult = 1;
			return;
		}
	 }
    *pResult = 0;
}


void CCmdExePane::OnNMClickCeCmdTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	 HTREEITEM	  hItem = NULL;
	 CCmdExeItem *pCmdExeItem;
	 CPoint       point;
	 UINT         uFlags;

	 GetCursorPos(&point);   
	 m_cmdTreeView.ScreenToClient(&point);
	 hItem = m_cmdTreeView.HitTest(point, &uFlags);

	 if(hItem)
	 { 
		 pCmdExeItem = (CCmdExeItem*)m_cmdTreeView.GetItemData(hItem);
		 if(pCmdExeItem != NULL)
		 {	
			// Select the item
 			m_cmdTreeView.SelectItem(hItem);
			// There is an item under the cursor. 
			// See what exactly was under the cursor:
			switch(uFlags)
			{
			case TVHT_ONITEMSTATEICON:
				pCmdExeItem->SetEnable(!pCmdExeItem->GetEnable());
				break;
			default:
				break;
	   		}
			CCmdExeCmd* pCmd = dynamic_cast<CCmdExeCmd*>(pCmdExeItem);
			if(pCmd != NULL)
			{
				m_removeCmdBtn.EnableWindow(TRUE);
				m_removeAllBtn.EnableWindow(TRUE);
			}
			else
			{
				m_removeCmdBtn.EnableWindow(FALSE);
				m_removeAllBtn.EnableWindow(TRUE);
			}
		 }
	 }	
	*pResult = 0;
}

void CCmdExePane::OnNMRclickCeCmdTree(NMHDR *pNMHDR, LRESULT *pResult)
{
    HTREEITEM		hItem = NULL;
	CPoint			point;
	CNewMenu		itemMenu;
	int			    mMenuItem =0, bGrayed = 0;

	if((hItem = GetMouseClickedItem(pNMHDR,pResult)) == NULL)
	 return;

	CCmdExeCmd* pCmd = dynamic_cast<CCmdExeCmd*>((CCmdExeCmdItem*)m_cmdTreeView.GetItemData(hItem));
	if(pCmd == NULL)
	{
		return;
	}

    GetCursorPos(&point);   
	itemMenu.CreatePopupMenu();

	CBitmap bmpImage;
	bmpImage.LoadBitmap(IDB_MENU_COPY_BITMAP); 
	itemMenu.InsertMenu(mMenuItem++, MF_BYPOSITION, ID_CE_COPY_ITEM ,  "Copy",&bmpImage);
	if(m_CmdClipboard.m_bEmpty == TRUE)
	{
		bGrayed = MF_GRAYED;
	}
	bmpImage.DeleteObject();

	bmpImage.LoadBitmap(IDB_MENU_PASTE_BITMAP); 
	itemMenu.InsertMenu(mMenuItem++, MF_BYPOSITION|bGrayed, ID_CE_PASTE_ITEM ,  "Paste",&bmpImage);
	bmpImage.DeleteObject();

	bmpImage.LoadBitmap(IDB_MENU_DUPLICATE_BITMAP); 
	itemMenu.InsertMenu(mMenuItem++, MF_BYPOSITION, ID_CE_DUPLICATE_ITEM ,  "Duplicate",&bmpImage);
	itemMenu.InsertMenu(mMenuItem++, MF_BYPOSITION|MF_SEPARATOR);
 	bmpImage.DeleteObject();
 
	bmpImage.LoadBitmap(IDB_REMOVE_BMP); 
	itemMenu.InsertMenu(mMenuItem++,MF_BYPOSITION, ID_CE_REMOVE_ITEM ,  "Remove",&bmpImage);
	bmpImage.DeleteObject();

	itemMenu.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, point.x, point.y, this);

	*pResult = 0;
}

void CCmdExePane::ClearCmdClipboard()
{
	m_CmdClipboard.m_bEmpty   = TRUE;
	m_CmdClipboard.m_pCmd     = NULL;
}

void CCmdExePane::PasteCmd(HTREEITEM hInsAfterItem)
{
	if(m_CmdClipboard.m_bEmpty == TRUE)
	{
		return;
	}
	CCmdExeCmd   *pNewCmd = (CCmdExeCmd*)m_CmdClipboard.m_pCmd->Clone(); 
	*pNewCmd = *m_CmdClipboard.m_pCmd;
	pNewCmd->SetDisplayName(pNewCmd->GetDisplayName() + " - Copy");
	m_cmdTreeView.AddCommand(pNewCmd);
}

void CCmdExePane::CopyCmd(HTREEITEM hItem)
{
	CCmdExeCmd* pCmd = dynamic_cast<CCmdExeCmd*>((CCmdExeCmdItem*)m_cmdTreeView.GetItemData(hItem));
	if(pCmd == NULL)
	{
		return;
	}
	//Save to command to copy
	m_CmdClipboard.m_pCmd = pCmd;
	m_CmdClipboard.m_bEmpty = FALSE;
}


