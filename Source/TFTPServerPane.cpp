// TFTPServerPane.cpp : implementation file
//

#include "stdafx.h"
#include "MVTerminal.h"
#include "TFTPServerPane.h"
#include ".\tftpserverpane.h"


#define  WM_FILE_TRANSFER_ENDED        (WM_USER + 8)
#define  WM_FILE_TRANSFER_IN_PROGRESS  (WM_USER + 9)
#define  CHAR_CR  				       TCHAR(13)

UINT MonitorTFTPServerActivity(LPVOID pParam);



// CTFTPServerPane

IMPLEMENT_DYNCREATE(CTFTPServerPane, CFormView)


CTFTPServerPane::CTFTPServerPane()
	: CFormView(CTFTPServerPane::IDD)
{
    // Create our TFTP server.
	m_pTargetView  = NULL;
    m_pTftpServer = NULL;
	m_bAlreadyConnected = FALSE;
	m_bTftpServerConnected = FALSE;
	m_bTransferEnd         = TRUE;
	m_bConnectOnStartup = ((CMVTerminalApp*)AfxGetApp())->GetTFTPServerAutoConnect();
}

CTFTPServerPane::~CTFTPServerPane()
{
	if(m_pTftpServer != NULL)
		delete m_pTftpServer;
}

void CTFTPServerPane::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TFTP_FILE_LIST, m_fileListCtrl);
	DDX_Control(pDX, IDC_LOAD_TFTP_FILE_BTN, m_loadBtn);
	DDX_Control(pDX, IDC_REMOVE_TFTP_FILE_BTN, m_removeBtn);
	DDX_Control(pDX, IDC_ADD_TFTP_FILE_BTN, m_addBtn);
	DDX_Control(pDX, IDC_TFTP_CONNECT_BTN, m_connectBtn);
	DDX_Control(pDX, IDC_LOAD_N_GO, m_loadNGoCheckBtn);
	DDX_Control(pDX, IDC_BURN_TO_FS, m_burnToFSCheckBtn);
	DDX_Control(pDX, IDC_TFTP_LOAD_PROGRESS, m_TFTPLoadProgressCtrl);
	DDX_Control(pDX, IDC_TFTP_CONNECTION_STATUS, m_statusTextCtrl);
	DDX_Control(pDX, IDC_REMOVE_ALL_TFTP_FILE_BTN2, m_removeAllFileBtn);
	DDX_Control(pDX, IDC_TFTP_LOAD_ADDRESS, m_loadAddrEdit);
	DDX_Control(pDX, IDC_TFTP_FILE_GO_BTN, m_goBtn);
}

BEGIN_MESSAGE_MAP(CTFTPServerPane, CFormView)
	
	ON_BN_CLICKED(IDC_ADD_TFTP_FILE_BTN, OnBnClickedAddTftpFile)
	ON_BN_CLICKED(IDC_REMOVE_TFTP_FILE_BTN, OnBnClickedRemoveTftpFile)
	ON_BN_CLICKED(IDC_LOAD_TFTP_FILE_BTN, OnBnClickedLoadTftpFile)
	ON_BN_CLICKED(IDC_TFTP_CONNECT_BTN, OnBnClickedTftpConnectBtn)
	ON_BN_CLICKED(IDC_BURN_TO_FS, OnBnClickedBurnToFs)
	ON_BN_CLICKED(IDC_LOAD_N_GO, OnBnClickedLoadNGo)
	ON_BN_CLICKED(IDC_AS_IS, OnBnClickedAsIs)
	ON_BN_CLICKED(IDC_EXECUTABLE, OnBnClickedExecutable)
	ON_LBN_DBLCLK(IDC_TFTP_FILE_LIST, OnLbnDblclkList)
	ON_EN_KILLFOCUS(IDC_PROGRAM_ARGS, OnEnKillfocusProgramArgs)
	ON_LBN_SELCHANGE(IDC_TFTP_FILE_LIST, OnLbnSelchangeList1)

	ON_MESSAGE(WM_FILE_TRANSFER_ENDED, OnFileTransferEnd)
	ON_MESSAGE(WM_FILE_TRANSFER_IN_PROGRESS, OnFileTransferInProgress)

	ON_EN_CHANGE(IDC_TFTP_LOAD_ADDRESS, OnEnChangeTftpLoadAddress)
	ON_BN_CLICKED(IDC_REMOVE_ALL_TFTP_FILE_BTN2, OnBnClickedRemoveAllTftpFileBtn2)
	ON_WM_CREATE()
	ON_WM_DROPFILES()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_AUTO_DETECT_LOAD_ADDR, OnBnClickedAutoDetectLoadAddr)
	ON_BN_CLICKED(IDC_TFTP_FILE_GO_BTN, OnBnClickedTftpFileGoBtn)
	ON_BN_CLICKED(IDC_FORCE_CACHEABLE_LOAD_ADDR, &CTFTPServerPane::OnBnClickedForceCacheableLoadAddr)
END_MESSAGE_MAP()


// CTFTPServerPane diagnostics

#ifdef _DEBUG
void CTFTPServerPane::AssertValid() const
{
	CFormView::AssertValid();
}

void CTFTPServerPane::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG



void  CTFTPServerPane::AddAndLoadFile(CString sFileName)
{	
	CTFTPFileDescriptor fd;
	
	// Add file only if it's not exist
	if(GetFileDesc(fd.GetFileName()) == NULL)
	{
		EnableFileControlls(TRUE);
		fd = CreateFileDescriptor(sFileName);
		AddFileDescriptor(fd);
		OnBnClickedLoadTftpFile();
	}
}


// CTFTPServerPane message handlers

void  CTFTPServerPane::UpdateFileDescriptor(CTFTPFileDescriptor *pFd)
{
	EnableFileControlls(TRUE);

	SetDlgItemText(IDC_TFTP_LOAD_ADDRESS,pFd->GetLoadAddress());
	SetDlgItemText(IDC_PROGRAM_ARGS,pFd->GetExecuteArguments());
	CheckDlgButton(IDC_LOAD_N_GO,pFd->GetLoadAndGo());
	CheckDlgButton(IDC_FORCE_CACHEABLE_LOAD_ADDR,pFd->GetForceCacheableLoadAddr());
	m_goBtn.EnableWindow(!pFd->GetLoadAndGo());
	CheckDlgButton(IDC_AUTO_DETECT_LOAD_ADDR,FALSE);
	GetDlgItem(IDC_TFTP_LOAD_ADDRESS)->EnableWindow(TRUE);
	if(pFd->GetAutoDetectLoadAddress() == TRUE)
	{
		GetDlgItem(IDC_TFTP_LOAD_ADDRESS)->EnableWindow(FALSE);
		CheckDlgButton(IDC_AUTO_DETECT_LOAD_ADDR,TRUE);
	}
	GetDlgItem(IDC_AUTO_DETECT_LOAD_ADDR)->EnableWindow(pFd->GetIsLoadAddressDetectable());
}

void  CTFTPServerPane::AddFileDescriptor(CTFTPFileDescriptor fd)
{
	if(GetFileDesc(fd.GetFileName()) != NULL)
	{
		m_fileListCtrl.SelectString(0,fd.GetFileName());
		return;
	}

	m_fileDescList.push_back(fd);
	// Update list
	m_fileListCtrl.AddString(fd.GetFileName());
	m_fileListCtrl.SelectString(0,fd.GetFileName());	
	UpdateFileDescriptor(&fd);

}

CTFTPFileDescriptor  CTFTPServerPane::CreateFileDescriptor(CString sFileName)
{				
	CTFTPFileDescriptor  fd;
	CString				 sLoadAddress;

	CMVTerminalView*  pActiveView = GetActiveView();
	if(pActiveView == NULL)
	{
		MessageBox("Cannot add file since there is no active session opened. Please open a session and try again!",NULL,MB_ICONSTOP);
		return fd;
	}
	// Set file name
	fd.SetFileName(sFileName);
	fd.SetExecuteArguments("");
	fd.SetAutoDetectLoadAddress(FALSE);

	sLoadAddress = DetectTftpFileLoadAddress(sFileName);
	if(sLoadAddress.IsEmpty() == TRUE)
	{
		fd.SetIsLoadAddressDetectable(FALSE);
        fd.SetLoadAddress(pActiveView->GetBootLoaderParams().GetTFTPLoadAddress());
	}
	else
	{
		fd.SetIsLoadAddressDetectable(TRUE);
        fd.SetLoadAddress(sLoadAddress);
		fd.SetAutoDetectLoadAddress(TRUE);
	}
	fd.SetLoadAndGo(TRUE);
	fd.SetForceCacheableLoadAddr(FALSE);

	return fd;
}

CString CTFTPServerPane::ExtractTftpLoadAddrFromBin(CString sTftpFilePath)
{
    CStdioFile   refFile;
    CFile        binFile;
	CString		 sLine,sRefFile,sLoadAddress,sSig;	
	BYTE 		 buf[12];
	UINT         nSig,nLoadAddress, nByte;

	if(binFile.Open(sTftpFilePath,CFile::typeBinary | CFile::modeRead) == FALSE)
	{
		return sLoadAddress;
	}

	binFile.Read(buf,12);
	binFile.Close();
	nSig = buf[4];
	nByte = buf[5];
	nSig = (nSig | (nByte << 8));
	nByte = buf[6];
	nSig = (nSig | (nByte << 16));
	nByte = buf[7];
	nSig = (nSig | (nByte << 24));
	if(nSig == 0xacacacac)
	{
		nLoadAddress = buf[8];
		nByte = buf[9];
		nLoadAddress = (nLoadAddress | (nByte << 8));
		nByte = buf[10];
		nLoadAddress = (nLoadAddress | (nByte << 16));
		nByte = buf[11];
		nLoadAddress = (nLoadAddress | (nByte << 24));
		sLoadAddress.Format("%x",nLoadAddress);
	}
	return sLoadAddress;
}

CString CTFTPServerPane::DetectTftpFileLoadAddress(CString sTftpFilePath)
{
    CStdioFile   refFile;
	CString		 sLine,sRefFile,sLoadAddress;	
	
	sLoadAddress = ExtractTftpLoadAddrFromBin(sTftpFilePath);
	if(sLoadAddress.IsEmpty() == FALSE)
	{
		return sLoadAddress;
	}

	//If load address signature not found, continue with the old way..
	sRefFile = sTftpFilePath.Left(sTftpFilePath.ReverseFind('\\') + 1);
	sRefFile += "mfGenSettings.ini";

	if(refFile.Open(sRefFile,CFile::typeText | CFile::modeRead) == FALSE)
		return sLoadAddress;
	
	while(refFile.ReadString(sLine) != FALSE)
	{
		if(sLine == "[ENTRY POINT]")
			break;
	}
	if(refFile.ReadString(sLine) != FALSE)
	{
		sLine.Remove(' ');
		sLine.MakeLower();
		if(sLine.Left(2) == "0x")
			sLoadAddress = sLine.Right(sLine.GetLength() - 2);	
	}
	refFile.Close();
	return sLoadAddress;
}

void CTFTPServerPane::OnBnClickedAddTftpFile()
{
	char szFilters[128] = "Binary Files (*.bin)|*.bin|All Files (*.*)|*.*||";
 	CFileDialog		dlg(TRUE,"bin","*.bin",OFN_FILEMUSTEXIST,szFilters,this);
	CTFTPFileDescriptor  fd;


	if(dlg.DoModal() == IDOK)
	{
		CString				 sFilePath;

		sFilePath = dlg.GetPathName();
		if(sFilePath.GetLength() >= 127)
		{
			MessageBox("File path:\n" + sFilePath+ "\nExceeds maximum byte length (126 bytes). Please specify a shorter path!",NULL,MB_ICONSTOP);
			return;
		}
		if(GetFileDesc(sFilePath) == NULL)
		{
			EnableFileControlls(TRUE);	
			fd = CreateFileDescriptor(sFilePath);
			// Update list ctrl
			AddFileDescriptor(fd);
		}
	}
}


void CTFTPServerPane::RemoveAllTftpFileDesc()
{
	m_fileListCtrl.ResetContent();
	m_fileDescList.clear();
}

void CTFTPServerPane::RemoveTftpFileDesc(CString   sFileName)
{
	/* Remove from view list*/
	//GetActiveView()->RemoveTFTPFileDesc(sFileName);
	list<CTFTPFileDescriptor>::iterator iter,endIter = m_fileDescList.end();
	for(iter = m_fileDescList.begin(); iter != endIter; iter++)
	{
		if((*iter).GetFileName() == sFileName)
		{
			m_fileDescList.erase(iter);
			break;
		}
	}
}

void CTFTPServerPane::OnBnClickedRemoveTftpFile()
{
	CString          sCount,sFileName;
	int	             index = m_fileListCtrl.GetCurSel();
	
	if(index == LB_ERR)
		return;

	m_fileListCtrl.GetText(index,sFileName);	
	RemoveTftpFileDesc(sFileName);

	m_fileListCtrl.DeleteString(index);

	// Enable/Disable buttons if empty
	if(m_fileListCtrl.GetCount() == 0)
	{
		ResetLoadNGoState();
		m_goBtn.EnableWindow(FALSE);
	}
	if(m_fileListCtrl.GetCurSel() == LB_ERR)
	{
		m_goBtn.EnableWindow(FALSE);
	}
	
	// Clear edit boxes
	SetDlgItemText(IDC_TFTP_LOAD_ADDRESS,"");
	SetDlgItemText(IDC_PROGRAM_ARGS,"");		
	EnableFileControlls(FALSE);
	EnableAutoDetectLoadAddressCtrl(FALSE);
	CheckDlgButton(IDC_AUTO_DETECT_LOAD_ADDR,false);
	CheckDlgButton(IDC_FORCE_CACHEABLE_LOAD_ADDR,false);
}


void CTFTPServerPane::OnBnClickedLoadTftpFile()
{
	m_pTargetView = GetActiveView();

	if(m_pTargetView == NULL)
	{
		MessageBox("No active view avaliable!",NULL,MB_ICONERROR);
		return;
	}

	if(m_bTftpServerConnected == FALSE)
	{
		MessageBox("TFTP Server not connected. Please connect and try again!",NULL,MB_ICONSTOP);
		return;
	}

	CTFTPFileDescriptor *pFd = GetSelectedFileDesc();
	if(pFd == NULL)
	{
		MessageBox("No file selected!",NULL,MB_ICONSTOP);
		return;
	}
	UpdateFileDescriptor(pFd);

	/* Disconnect and connect incase of prior load failure */
	if(m_bTransferEnd == FALSE)
	{
		if(DisconnectTftpServer() == false)
			return;
		if(ConnectTftpServer() == false)
			return;
	}

	// Set progress bar parameters
	CStdioFile   loadFile;
	ULONGLONG    fileSize;
	CString		 sFilePath, sLine, sFile = pFd->GetFileName();	
		
	UINT		 nOpenFlags = CFile::modeRead;

	if((pFd->GetFileName()).Right(4) == ".bin")
		nOpenFlags |= CFile::typeBinary;

	if(loadFile.Open(pFd->GetFileName(),nOpenFlags) == FALSE)
	{
		MessageBox("The file '"+pFd->GetFileName()+"' could not be found!",NULL,MB_ICONSTOP);
		return;
	}

	fileSize = loadFile.GetLength();

	m_TFTPLoadProgressCtrl.SetPos(0);
	m_TFTPLoadProgressCtrl.SetRange(0,(short)((fileSize)/512));
	m_TFTPLoadProgressCtrl.SetStep(1);
	loadFile.Close();

	// Set load command
	if(IsDlgButtonChecked(IDC_BURN_TO_FS) == FALSE)
		
	// Set load command
	sLine  = m_pTargetView->GetBootLoaderParams().GetTFTPLoadCommand();
	if(IsDlgButtonChecked(IDC_BURN_TO_FS) == TRUE)
	{
		if(IsDlgButtonChecked(IDC_EXECUTABLE) == TRUE)
			// Burn exe command
			sLine = m_pTargetView->GetBootLoaderParams().GetTFTPBurnExeCommand();
		if(IsDlgButtonChecked(IDC_AS_IS) == TRUE)
		   // Burn as is command
		   sLine = m_pTargetView->GetBootLoaderParams().GetTFTPBurnAsIsCommand();
	}

	// Append load address if not as is
	if(IsDlgButtonChecked(IDC_AS_IS) == FALSE)
	{	
		CString sNewLoadAddress, sLoadAddress;

		sLine += " ";
		// Always re-extract load address incase it was modified 		
		sNewLoadAddress = DetectTftpFileLoadAddress(pFd->GetFileName());
		GetDlgItemText(IDC_TFTP_LOAD_ADDRESS,sLoadAddress);
		if(sNewLoadAddress.IsEmpty() == FALSE)
		{				
			if(pFd->GetAutoDetectLoadAddress() == TRUE)
			{	
				// Update new load address in terminal view
				SetDlgItemText(IDC_TFTP_LOAD_ADDRESS,sNewLoadAddress);
				sLoadAddress = sNewLoadAddress;
				pFd->SetLoadAddress(sNewLoadAddress);
			}
		}

		if(pFd->GetForceCacheableLoadAddr() == TRUE)
		{
			UINT32 nLoadAddress = 0;
			sscanf(sLoadAddress,"%x",&nLoadAddress);
			nLoadAddress = nLoadAddress | 0x80000000;
			sLoadAddress = "";
			sLoadAddress.Format("%x",nLoadAddress);
		}
	
		sLine += sLoadAddress;
	}

	sLine += " ";
	sFile.Replace("\\","\\\\");
	sFile.Replace("\\\\\\\\","\\\\");

	sFilePath = sFile.Left(sFile.ReverseFind('\\') + 1);
			
	if(::SetCurrentDirectory(sFilePath) == FALSE)
	{
		MessageBox("Error setting current directory to " + sFilePath,NULL,MB_ICONSTOP);
		return;
	}

	sFile = sFile.Right( sFile.GetLength() - sFile.ReverseFind('\\') - 1);

	// Fix replacement incase its a network folder i.e. \\PC29 
	sLine += "\"" + sFile +"\"";
	CTextLine  txtLine(sLine,0,100,TRUE);

	m_bTransferEnd = FALSE;
	m_pTargetView->GetCmdHistoryLogger().SetEnable(FALSE);
	m_pTargetView->SendTextLine(txtLine);
	m_pTargetView->GetCmdHistoryLogger().SetEnable(TRUE);

	m_loadBtn.EnableWindow(FALSE);
	// Set Focus back to view
	m_pTargetView->SetFocus();
	if(((CMVTerminalApp*)AfxGetApp())->GetWarnTFTPFailure() == TRUE)
	{
		AfxBeginThread(MonitorTFTPServerActivity,this);
	}
}

UINT MonitorTFTPServerActivity(LPVOID pParam)
{
	CTFTPServerPane  *pTFTPServerPane = (CTFTPServerPane*)pParam;

	Sleep(4000);
	if(pTFTPServerPane->GetProgressBarPos() == 0)
	{
		AfxMessageBox("Warning!!! TFTP load file request could not be started. Please check client/server IP settings or local network connection and try again.",NULL,MB_ICONWARNING);
	}
	return 0;
}

bool CTFTPServerPane::ConnectTftpServer()
{
	int		  status;
	PT_STATE  state;

	state = m_pTftpServer->State();
	if ((PT_LISTENING == state) || (PT_CONNECTED == state)) 
	{	
		m_bTftpServerConnected = TRUE;
		return true;
	}		
	if((status = m_pTftpServer->Listen(NULL, PT_TCPNODELAY, NULL, IPPORT_TFTP, 1000, 3)) == 0)
	{
		MessageBox("TFTP Server could not be connected, port 69 is probably open by another application!",NULL,MB_ICONERROR);
		return false;
	}		
	m_bTftpServerConnected = TRUE;
	/* Kill the DART dialog box */
	//KillProcessByName("PTlic32.exe");
	return true;
}

bool CTFTPServerPane::DisconnectTftpServer()
{
    if (PT_CLOSED == m_pTftpServer->State()) 
	{			
		m_bTftpServerConnected = FALSE;
		return true;
	}
	m_pTftpServer->Close(TRUE);
    if (PT_CLOSED != m_pTftpServer->State()) 
	{
		MessageBox("TFTP Server could not be disconnected!",NULL,MB_ICONERROR);
		return false;
	}
	m_bTftpServerConnected = FALSE;
	return true;
}


void CTFTPServerPane::OnBnClickedTftpConnectBtn()
{
	CString   sWinTxt;

	// Request for connect
	m_connectBtn.GetWindowText(sWinTxt);	
	if(sWinTxt == "Connect")
	{
		if(ConnectTftpServer() == false)
			return;
		
		m_connectBtn.SetWindowText("Disconnect");	
		SetDlgItemText(IDC_TFTP_CONNECTION_STATUS,"TFTP Server Connected (Port 69, " + m_sServerIP +")");
		m_statusTextCtrl.SetFontStyle(FS_BOLD);
	}	
	// Request for disconnect
	if(sWinTxt == "Disconnect")
	{
		if(DisconnectTftpServer() == false)
			return;

		m_connectBtn.SetWindowText("Connect");	

		SetDlgItemText(IDC_TFTP_CONNECTION_STATUS,"TFTP Server Disconnected.");
		m_statusTextCtrl.SetFontStyle(FS_NORMAL);

		// Enable load btn incase TFTP was reset
		if(m_fileListCtrl.GetCount() > 0)
			m_loadBtn.EnableWindow(TRUE);

		// Reset progress bar incase TFTP was reset
		m_TFTPLoadProgressCtrl.SetPos(0);
	}
	RedrawWindow();
}

void CTFTPServerPane::OnBnClickedBurnToFs()
{
	BOOL nChecked = IsDlgButtonChecked(IDC_BURN_TO_FS);
	
	GetDlgItem(IDC_AS_IS)->ShowWindow(nChecked);
	GetDlgItem(IDC_EXECUTABLE)->ShowWindow(nChecked);

	CheckDlgButton(IDC_LOAD_N_GO,false);
	OnBnClickedExecutable();
}


void CTFTPServerPane::ResetLoadNGoState()
{
	CheckDlgButton(IDC_BURN_TO_FS,false);
	CheckDlgButton(IDC_AS_IS,false);
	CheckDlgButton(IDC_EXECUTABLE,false);
	GetDlgItem(IDC_AS_IS)->ShowWindow(FALSE);
	GetDlgItem(IDC_EXECUTABLE)->ShowWindow(FALSE);
}

void CTFTPServerPane::OnBnClickedLoadNGo()
{
	CTFTPFileDescriptor *pFd = GetSelectedFileDesc();

	if(pFd != NULL)
		pFd->SetLoadAndGo(!pFd->GetLoadAndGo());

	m_goBtn.EnableWindow(!pFd->GetLoadAndGo());

	ResetLoadNGoState();

}

////////////////////////////  TFTP Server class /////////////////////////////////////////

CTFTPServer::CTFTPServer(HINSTANCE hinst, HWND hwnd) :
      CPowerTftp(hinst), m_hwndMain(hwnd)
{

}

// A UDP port has been successfully allocated for our TFTP server.
void CTFTPServer::ConnectEvent(LPCSTR lpszLocalDotAddr, WORD wLocalPort, 
      LPCSTR lpszLocalName, WORD wMaxByteCnt)
{

}

// Monitor our TFTP server file transfers.
void CTFTPServer::TftpEvent(TFTP_OPERATION opCode, 
      LPCSTR lpszLocalFileSpec, WORD wBlock, UINT nByteCnt, 
      DWORD dwTransferId, WORD wActiveCnt, LPCSTR lpszRemotedotAddr, 
      WORD wRemotePort, PT_EXCEPTION codeError, LPCSTR lpszErrorDesc)

{
    // save TransferId in case use presses Abort button
    LastTransferId = dwTransferId;
    if (codeError)
    {
        // Display a description of the error or failure and remove the
	    // file name from the transfer list.
	    return;
    }
	if (wBlock && wBlock == wLastBlock)
    {
      // Block has been resent.
    }                                                         

    wLastBlock = wBlock;
      
    if (TFTP_CLOSED == opCode)
    {
        if (!dwTransferId)
        {
         // Our only positive indication of a succesful close.
        }
    } 
    else if (!wBlock)
    {
        // Start of new transfer.
    }                        
    else if (nByteCnt < 516)
    {
        // Send Message that transfer is complete
		::PostMessage(m_hwndMain,WM_FILE_TRANSFER_ENDED,0,0);
		
    }
    else
    {
        // Send Message that transfer is in progress.
   		::PostMessage(m_hwndMain,WM_FILE_TRANSFER_IN_PROGRESS,0,0);
    }           
}

// Display a description of the error or failure.
void CTFTPServer::ExceptionEvent(PT_EXCEPTION codeError, 
      LPCSTR lpszErrorDesc)
{                 

} 
////////////////////////////////////////////////////////////////////////////////////////////

void CTFTPServerPane::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	//Buttons
	m_goBtn.SetTheme(xtpButtonThemeOffice2000);
	m_goBtn.EnableWindow(FALSE);
	m_loadBtn.SetTheme(xtpButtonThemeOffice2000);
	m_removeBtn.SetTheme(xtpButtonThemeOffice2000);
	m_addBtn.SetTheme(xtpButtonThemeOffice2000);
	m_connectBtn.SetTheme(xtpButtonThemeOffice2000);
	m_removeAllFileBtn.SetTheme(xtpButtonThemeOffice2000);

	m_loadAddrEdit.SetLimitText(8);


	// Set load address
	CMVTerminalView *pView = GetActiveView();
	if(pView != NULL)
		m_sLoadAddress = pView->GetBootLoaderParams().GetTFTPLoadAddress();
	else
		m_sLoadAddress = "";

	SetDlgItemText(IDC_TFTP_LOAD_ADDRESS,m_sLoadAddress);

	// Reset load and go state
	ResetLoadNGoState();
	EnableFileControlls(FALSE);

	// Get Local host IP address
	m_sServerIP = ((CMVTerminalApp*)AfxGetApp())->GetLocalHostIPAddress();
	
	// Create the TFTP server
	if(m_pTftpServer == NULL)
	{ 
		HINSTANCE  hinst = (HINSTANCE)GetWindowInstance(m_hWnd);
		m_pTftpServer = new CTFTPServer(hinst, m_hWnd);
	}

	// Auto connect (if true)
	if((m_bConnectOnStartup == TRUE) && (m_bAlreadyConnected == FALSE))
	{
		m_bAlreadyConnected = TRUE; 
		OnBnClickedTftpConnectBtn();
	}

	EnableAutoDetectLoadAddressCtrl(FALSE);
	CheckDlgButton(IDC_AUTO_DETECT_LOAD_ADDR,false);
	EnableAllControlls(FALSE);


}
void CTFTPServerPane::EnableAutoDetectLoadAddressCtrl(BOOL bEnable)
{
	GetDlgItem(IDC_AUTO_DETECT_LOAD_ADDR)->EnableWindow(bEnable);
}

void CTFTPServerPane::EnableFileControlls(BOOL bEnable)
{	
	// Disable buttons
	m_addBtn.EnableWindow(TRUE);
	m_loadBtn.EnableWindow(bEnable);
	m_removeBtn.EnableWindow(bEnable);
	m_removeAllFileBtn.EnableWindow(bEnable);
	m_loadNGoCheckBtn.EnableWindow(bEnable);
	m_burnToFSCheckBtn.EnableWindow(bEnable);
	GetDlgItem(IDC_TFTP_LOAD_ADDRESS)->EnableWindow(bEnable);
	GetDlgItem(IDC_AUTO_DETECT_LOAD_ADDR)->EnableWindow(bEnable);
	GetDlgItem(IDC_PROGRAM_ARGS)->EnableWindow(bEnable);
	GetDlgItem(IDC_AS_IS)->EnableWindow(bEnable);
	GetDlgItem(IDC_EXECUTABLE)->EnableWindow(bEnable);
	GetDlgItem(IDC_FORCE_CACHEABLE_LOAD_ADDR)->EnableWindow(bEnable);
}

void CTFTPServerPane::AllowLoadControlls(BOOL bAllow)
{	
	m_addBtn.EnableWindow(bAllow);
	m_fileListCtrl.EnableWindow(bAllow);
	if(m_fileListCtrl.GetCount() > 0)
	{
		CString str;
		m_fileListCtrl.GetText(0,str);
		m_fileListCtrl.SelectString(-1,str);
		UpdateFileDescriptor( GetFileDesc(str) );
	}
}


void CTFTPServerPane::EnableAllControlls(BOOL bEnable)
{	
	// Disable buttons
	m_addBtn.EnableWindow(bEnable);
	m_loadBtn.EnableWindow(bEnable);
	m_removeBtn.EnableWindow(bEnable);
	m_removeAllFileBtn.EnableWindow(bEnable);
	m_burnToFSCheckBtn.EnableWindow(bEnable);
	m_fileListCtrl.EnableWindow(bEnable);

	GetDlgItem(IDC_TFTP_LOAD_ADDRESS)->EnableWindow(bEnable);
	GetDlgItem(IDC_PROGRAM_ARGS)->EnableWindow(bEnable);
	GetDlgItem(IDC_AUTO_DETECT_LOAD_ADDR)->EnableWindow(bEnable);

	GetDlgItem(IDC_LOAD_N_GO)->EnableWindow(bEnable);
	GetDlgItem(IDC_AS_IS)->EnableWindow(bEnable);
	GetDlgItem(IDC_EXECUTABLE)->EnableWindow(bEnable);
	GetDlgItem(IDC_FORCE_CACHEABLE_LOAD_ADDR)->EnableWindow(bEnable);

}


void CTFTPServerPane::OnBnClickedAsIs()
{	
	CheckDlgButton(IDC_AS_IS,true);
	CheckDlgButton(IDC_EXECUTABLE,false);
}

void CTFTPServerPane::OnBnClickedExecutable()
{	
	CheckDlgButton(IDC_EXECUTABLE,true);
	CheckDlgButton(IDC_AS_IS,false);
}

void CTFTPServerPane::OnLbnDblclkList()
{
	OnBnClickedLoadTftpFile();
}

void CTFTPServerPane::OnEnKillfocusProgramArgs()
{
	CTFTPFileDescriptor *pFd = GetSelectedFileDesc();

	if(pFd == NULL)
	{
		MessageBox("Cannot file selected file descriptor",NULL,MB_ICONERROR);
		return;
	}
	CString tmp;
	GetDlgItemText(IDC_PROGRAM_ARGS,tmp);
	pFd->SetExecuteArguments(tmp);
}

CTFTPFileDescriptor* CTFTPServerPane::GetSelectedFileDesc()
{
	int			index = m_fileListCtrl.GetCurSel();
	CString		sFileName;

	if(index == LB_ERR)
		return NULL;

	m_fileListCtrl.GetText(index,sFileName);

	return GetFileDesc(sFileName);
}

void CTFTPServerPane::UpdateChanges()
{
	CTFTPFileDescriptor *pFd = GetSelectedFileDesc();

	if(pFd == NULL)
	{
		return;
	}
	OnLbnSelchangeList1();
}

void CTFTPServerPane::OnLbnSelchangeList1()
{
	CTFTPFileDescriptor *pFd = GetSelectedFileDesc();

	if(pFd == NULL)
	{
		MessageBox("Cannot find selected file's descriptor",NULL,MB_ICONERROR);
		return;
	}
	UpdateFileDescriptor(pFd);
}

void CTFTPServerPane::OnBnClickedAutoDetectLoadAddr()
{
	CTFTPFileDescriptor *pFd = GetSelectedFileDesc();

	pFd->SetAutoDetectLoadAddress(!pFd->GetAutoDetectLoadAddress());
	GetDlgItem(IDC_TFTP_LOAD_ADDRESS)->EnableWindow(!pFd->GetAutoDetectLoadAddress());
}


CTFTPFileDescriptor* CTFTPServerPane::GetFileDesc(CString sFileName)
{
	list<CTFTPFileDescriptor>::iterator iter,endIter = m_fileDescList.end();

	for(iter = m_fileDescList.begin(); iter != endIter; iter++)
	{
		if((*iter).GetFileName() == sFileName)
		{
			return  &(*iter);
		}
	}
	return NULL;
}

CMVTerminalView*  CTFTPServerPane::GetActiveView()
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



LRESULT CTFTPServerPane::OnFileTransferEnd(WPARAM a, LPARAM b) 
{	
	int				 nLowerRange, nUperRange;

	if(m_pTargetView == NULL)
		return 0;

	m_bTransferEnd = TRUE;
	// Move progress bar to end
	m_TFTPLoadProgressCtrl.GetRange(nLowerRange,nUperRange);
	m_TFTPLoadProgressCtrl.SetPos(nUperRange);

	if(IsDlgButtonChecked(IDC_LOAD_N_GO) == TRUE)
	{
		CString		sRunCommand, sExecuteArgs, sLoadAddress;
		
		// Get run command
		sRunCommand = m_pTargetView->GetBootLoaderParams().GetRunCommand();

		// Append load address
		GetDlgItemText(IDC_TFTP_LOAD_ADDRESS,sLoadAddress);
		sRunCommand += " " + sLoadAddress;

		// Get Args
		GetDlgItemText(IDC_PROGRAM_ARGS,sExecuteArgs);

		// Append args
		if(sExecuteArgs.IsEmpty() == FALSE)
		{
			GetDlgItemText(IDC_TFTP_LOAD_ADDRESS,sLoadAddress);
			sRunCommand += " " + sExecuteArgs;
		}

		CTextLine    txtLine(sRunCommand,1000,150,TRUE);

		m_pTargetView->GetCmdHistoryLogger().SetEnable(FALSE);
		m_pTargetView->SendTextLine(txtLine);
		m_pTargetView->GetCmdHistoryLogger().SetEnable(TRUE);
	}

	// Enable back the load button
	if(m_fileListCtrl.GetCount() > 0)
		m_loadBtn.EnableWindow(TRUE);

	return 0;
}

LRESULT CTFTPServerPane::OnFileTransferInProgress(WPARAM a, LPARAM b) 
{
	m_TFTPLoadProgressCtrl.StepIt();
	return 0;
}

void CTFTPServerPane::OnEnChangeTftpLoadAddress()
{
	CTFTPFileDescriptor *pFd = GetSelectedFileDesc();

	if(pFd == NULL)
	{
		return;
	}
	CString sLoadAddress;
	GetDlgItemText(IDC_TFTP_LOAD_ADDRESS,sLoadAddress);
	pFd->SetLoadAddress(sLoadAddress);
}


void CTFTPServerPane::ResetCtrlStates()
{
	ResetLoadNGoState();
	CheckDlgButton(IDC_LOAD_N_GO,false);


	// Clear edit boxes
	SetDlgItemText(IDC_TFTP_LOAD_ADDRESS,"");
	SetDlgItemText(IDC_PROGRAM_ARGS,"");		
	EnableFileControlls(FALSE);

	EnableAutoDetectLoadAddressCtrl(FALSE);
	CheckDlgButton(IDC_AUTO_DETECT_LOAD_ADDR,false);
	CheckDlgButton(IDC_FORCE_CACHEABLE_LOAD_ADDR,false);

}

void CTFTPServerPane::OnBnClickedRemoveAllTftpFileBtn2()
{
	m_fileDescList.clear();
	m_fileListCtrl.ResetContent();
	ResetCtrlStates();
	m_goBtn.EnableWindow(FALSE);
}

int CTFTPServerPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;
	return 0;
}

void CTFTPServerPane::OnDropFiles(HDROP hDropInfo)
{
	CTFTPFileDescriptor  fd;
    char*  pFilename = new char[_MAX_PATH];

	CMVTerminalView*  pActiveView = GetActiveView();
	if(pActiveView == NULL)
	{
		MessageBox("Cannot add file since there is no active session opened. Please open a session and try again!",NULL,MB_ICONSTOP);
		return;
	}

    m_hDropInfo = hDropInfo;
    if (m_hDropInfo) 
	{
        int nFiles = DragQueryFile(m_hDropInfo, (UINT)-1, NULL, 0);
		for(int i = 0; i < nFiles ; i++) 
		{
            DragQueryFile(m_hDropInfo, i, pFilename, _MAX_PATH);

			if((GetFileAttributes(pFilename) & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY)
			{
				fd = CreateFileDescriptor(pFilename);
				AddFileDescriptor(fd);
			}
		}
    }    
	delete pFilename;
    DragFinish(m_hDropInfo);
    m_hDropInfo = 0;
	CFormView::OnDropFiles(hDropInfo);
}

int CTFTPServerPane::KillProcessByName(const char *szToTerminate)
{
    BOOL bResult,bResultm;
    DWORD aiPID[1000],iCb=1000,iNumProc,iV2000=0;
    DWORD iCbneeded,i,iFound=0;
    char szName[MAX_PATH],szToTermUpper[MAX_PATH];
    HANDLE hProc,hSnapShot,hSnapShotm;
    OSVERSIONINFO osvi;
    HINSTANCE hInstLib;
    int iLen,indx;
	size_t  iLenP;
    HMODULE hMod;
    PROCESSENTRY32 procentry;      
    MODULEENTRY32 modentry;

    // Transfer Process name into "szToTermUpper" and
    // convert it to upper case
    iLenP=strlen(szToTerminate);
    if (iLenP<1 || iLenP>MAX_PATH) return 632;
    for (indx=0;indx<(int)iLenP;indx++)
        szToTermUpper[indx]=toupper(szToTerminate[indx]);
    szToTermUpper[iLenP]=0;

    // PSAPI Function Pointers.
    BOOL (WINAPI *lpfEnumProcesses)( DWORD *, DWORD cb, DWORD * );
    BOOL (WINAPI *lpfEnumProcessModules)( HANDLE, HMODULE *,
                                          DWORD, LPDWORD );
    DWORD (WINAPI *lpfGetModuleBaseName)( HANDLE, HMODULE,
                                          LPTSTR, DWORD );

    // ToolHelp Function Pointers.
    HANDLE (WINAPI *lpfCreateToolhelp32Snapshot)(DWORD,DWORD) ;
    BOOL (WINAPI *lpfProcess32First)(HANDLE,LPPROCESSENTRY32) ;
    BOOL (WINAPI *lpfProcess32Next)(HANDLE,LPPROCESSENTRY32) ;
    BOOL (WINAPI *lpfModule32First)(HANDLE,LPMODULEENTRY32) ;
    BOOL (WINAPI *lpfModule32Next)(HANDLE,LPMODULEENTRY32) ;

    // First check what version of Windows we're in
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    bResult=GetVersionEx(&osvi);
    if (!bResult)     // Unable to identify system version
        return 606;

    // At Present we only support Win/NT/2000/XP or Win/9x/ME
    if ((osvi.dwPlatformId != VER_PLATFORM_WIN32_NT) &&
        (osvi.dwPlatformId != VER_PLATFORM_WIN32_WINDOWS))
        return 607;

    if (osvi.dwPlatformId==VER_PLATFORM_WIN32_NT)
    {
        // Win/NT or 2000 or XP
        // Load library and get the procedures explicitly. We do
        // this so that we don't have to worry about modules using
        // this code failing to load under Windows 9x, because
        // it can't resolve references to the PSAPI.DLL.
        hInstLib = LoadLibraryA("PSAPI.DLL");
        if (hInstLib == NULL)
            return 605;

        // Get procedure addresses.
        lpfEnumProcesses = (BOOL(WINAPI *)(DWORD *,DWORD,DWORD*))
                           GetProcAddress( hInstLib, "EnumProcesses" ) ;
        lpfEnumProcessModules = (BOOL(WINAPI *)(HANDLE, HMODULE *,
                                                DWORD, LPDWORD)) GetProcAddress( hInstLib,
                                                                                 "EnumProcessModules" ) ;
        lpfGetModuleBaseName =(DWORD (WINAPI *)(HANDLE, HMODULE,
                                                LPTSTR, DWORD )) GetProcAddress( hInstLib,
                                                                                 "GetModuleBaseNameA" ) ;

        if (lpfEnumProcesses == NULL ||
            lpfEnumProcessModules == NULL ||
            lpfGetModuleBaseName == NULL)
        {
            FreeLibrary(hInstLib);
            return 700;
        }

        bResult=lpfEnumProcesses(aiPID,iCb,&iCbneeded);
        if (!bResult)
        {
            // Unable to get process list, EnumProcesses failed
            FreeLibrary(hInstLib);
            return 701;
        }

        // How many processes are there?
        iNumProc=iCbneeded/sizeof(DWORD);

        // Get and match the name of each process
        for (i=0;i<iNumProc;i++)
        {
            // Get the (module) name for this process

			
            strcpy(szName,"Unknown");
            // First, get a handle to the process
            hProc=OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_READ,FALSE,
                              aiPID[i]);
            // Now, get the process name
            if (hProc)
            {
                if (lpfEnumProcessModules(hProc,&hMod,sizeof(hMod),&iCbneeded) )
                {
                    iLen=lpfGetModuleBaseName(hProc,hMod,szName,MAX_PATH);
                }
            }
            CloseHandle(hProc);
            // We will match regardless of lower or upper case
            if (strcmp(_strupr(szName),szToTermUpper)==0)
            {
                // Process found, now terminate it
                iFound=1;
                // First open for termination
                hProc=OpenProcess(PROCESS_TERMINATE,FALSE,aiPID[i]);
                if (hProc)
                {
                    if (TerminateProcess(hProc,0))
                    {
                        // process terminated
                        CloseHandle(hProc);
                        FreeLibrary(hInstLib);
                        return 0;
                    }
                    else
                    {
                        // Unable to terminate process
                        CloseHandle(hProc);
                        FreeLibrary(hInstLib);
                        return 602;
                    }
                }
                else
                {
                    // Unable to open process for termination
                    FreeLibrary(hInstLib);
                    return 604;
                }
            }
        }
    }

    if (osvi.dwPlatformId==VER_PLATFORM_WIN32_WINDOWS)
    {
        // Win/95 or 98 or ME

        hInstLib = LoadLibraryA("Kernel32.DLL");
        if ( hInstLib == NULL )
            return 702;

        // Get procedure addresses.
        // We are linking to these functions of Kernel32
        // explicitly, because otherwise a module using
        // this code would fail to load under Windows NT,
        // which does not have the Toolhelp32
        // functions in the Kernel 32.
        lpfCreateToolhelp32Snapshot=
        (HANDLE(WINAPI *)(DWORD,DWORD))
        GetProcAddress( hInstLib,
                        "CreateToolhelp32Snapshot" ) ;
        lpfProcess32First=
        (BOOL(WINAPI *)(HANDLE,LPPROCESSENTRY32))
        GetProcAddress( hInstLib, "Process32First" ) ;
        lpfProcess32Next=
        (BOOL(WINAPI *)(HANDLE,LPPROCESSENTRY32))
        GetProcAddress( hInstLib, "Process32Next" ) ;
        lpfModule32First=
        (BOOL(WINAPI *)(HANDLE,LPMODULEENTRY32))
        GetProcAddress( hInstLib, "Module32First" ) ;
        lpfModule32Next=
        (BOOL(WINAPI *)(HANDLE,LPMODULEENTRY32))
        GetProcAddress( hInstLib, "Module32Next" ) ;
        if ( lpfProcess32Next == NULL ||
             lpfProcess32First == NULL ||
             lpfModule32Next == NULL ||
             lpfModule32First == NULL ||
             lpfCreateToolhelp32Snapshot == NULL )
        {
            FreeLibrary(hInstLib);
            return 703;
        }

        // The Process32.. and Module32.. routines return names in all uppercase

        // Get a handle to a Toolhelp snapshot of all the systems processes.

        hSnapShot = lpfCreateToolhelp32Snapshot(
                                               TH32CS_SNAPPROCESS, 0 ) ;
        if ( hSnapShot == INVALID_HANDLE_VALUE )
        {
            FreeLibrary(hInstLib);
            return 704;
        }

        // Get the first process' information.
        procentry.dwSize = sizeof(PROCESSENTRY32);
        bResult=lpfProcess32First(hSnapShot,&procentry);

        // While there are processes, keep looping and checking.
        while (bResult)
        {
            // Get a handle to a Toolhelp snapshot of this process.
            hSnapShotm = lpfCreateToolhelp32Snapshot(
                                                    TH32CS_SNAPMODULE, procentry.th32ProcessID) ;
            if ( hSnapShotm == INVALID_HANDLE_VALUE )
            {
                CloseHandle(hSnapShot);
                FreeLibrary(hInstLib);
                return 704;
            }
            // Get the module list for this process
            modentry.dwSize=sizeof(MODULEENTRY32);
            bResultm=lpfModule32First(hSnapShotm,&modentry);

            // While there are modules, keep looping and checking
            while (bResultm)
            {
                if (strcmp(modentry.szModule,szToTermUpper)==0)
                {
                    // Process found, now terminate it
                    iFound=1;
                    // First open for termination
                    hProc=OpenProcess(PROCESS_TERMINATE,FALSE,procentry.th32ProcessID);
                    if (hProc)
                    {
                        if (TerminateProcess(hProc,0))
                        {
                            // process terminated
                            CloseHandle(hSnapShotm);
                            CloseHandle(hSnapShot);
                            CloseHandle(hProc);
                            FreeLibrary(hInstLib);
                            return 0;
                        }
                        else
                        {
                            // Unable to terminate process
                            CloseHandle(hSnapShotm);
                            CloseHandle(hSnapShot);
                            CloseHandle(hProc);
                            FreeLibrary(hInstLib);
                            return 602;
                        }
                    }
                    else
                    {
                        // Unable to open process for termination
                        CloseHandle(hSnapShotm);
                        CloseHandle(hSnapShot);
                        FreeLibrary(hInstLib);
                        return 604;
                    }
                }
                else
                {  // Look for next modules for this process
                    modentry.dwSize=sizeof(MODULEENTRY32);
                    bResultm=lpfModule32Next(hSnapShotm,&modentry);
                }
            }

            //Keep looking
            CloseHandle(hSnapShotm);
            procentry.dwSize = sizeof(PROCESSENTRY32);
            bResult = lpfProcess32Next(hSnapShot,&procentry);
        }
        CloseHandle(hSnapShot);
    }
    if (iFound==0)
    {
        FreeLibrary(hInstLib);
        return 603;
    }
    FreeLibrary(hInstLib);
    return 0;
}


void CTFTPServerPane::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CFormView::OnPaint() for painting messages
}

CString CTFTPServerPane::Serialize()
{
	CMarkup xmlWriter;

	list<CTFTPFileDescriptor>::iterator iter = m_fileDescList.begin(), endIter = m_fileDescList.end();

	xmlWriter.AddElem("TFTPServerPaneFiles");
	xmlWriter.IntoElem();

	for(; iter != endIter; iter++)
	{
		xmlWriter.AddSubDoc( (*iter).Serialize() );
	}

	xmlWriter.OutOfElem();
	return xmlWriter.GetDoc();

}

BOOL CTFTPServerPane::DeSerialize(CMarkup & xmlReader)
{
	if( xmlReader.FindElem("TFTPServerPaneFiles") == false)
		return FALSE;

	//First, remove all current scripts
	RemoveAllTftpFileDesc();

	xmlReader.IntoElem(); 
	while(xmlReader.FindElem("TFTPServerFileDesc"))
	{
		CTFTPFileDescriptor  fd;
		fd.DeSerialize(xmlReader);
		AddFileDescriptor(fd);	
	}
	xmlReader.OutOfElem(); 
	return TRUE;
}


void CTFTPServerPane::OnBnClickedTftpFileGoBtn()
{
	CString		sRunCommand, sExecuteArgs, sLoadAddress;
	int			index = m_fileListCtrl.GetCurSel();

	if(index == LB_ERR)
		return;

	if(m_pTargetView == NULL)
		return;

	// Get run command
	sRunCommand = m_pTargetView->GetBootLoaderParams().GetRunCommand();

	// Append load address
	GetDlgItemText(IDC_TFTP_LOAD_ADDRESS,sLoadAddress);

	CTFTPFileDescriptor *pFd = GetSelectedFileDesc();
	if(pFd->GetForceCacheableLoadAddr() == TRUE)
	{
		UINT32 nLoadAddress = 0;
		sscanf(sLoadAddress,"%x",&nLoadAddress);
		nLoadAddress = nLoadAddress | 0x80000000;
		sLoadAddress = "";
		sLoadAddress.Format("%x",nLoadAddress);
	}

	sRunCommand += " " + sLoadAddress;

	// Get Args
	GetDlgItemText(IDC_PROGRAM_ARGS,sExecuteArgs);

	// Append args
	if(sExecuteArgs.IsEmpty() == FALSE)
	{
		GetDlgItemText(IDC_TFTP_LOAD_ADDRESS,sLoadAddress);
		sRunCommand += " " + sExecuteArgs;
	}

	CTextLine    txtLine(sRunCommand,500,150,TRUE);

	m_pTargetView->GetCmdHistoryLogger().SetEnable(FALSE);
	m_pTargetView->SendTextLine(txtLine);
	m_pTargetView->GetCmdHistoryLogger().SetEnable(TRUE);

}

void CTFTPServerPane::OnBnClickedForceCacheableLoadAddr()
{
	CTFTPFileDescriptor *pFd = GetSelectedFileDesc();
	pFd->SetForceCacheableLoadAddr(!pFd->GetForceCacheableLoadAddr());
}
