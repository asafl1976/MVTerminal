#pragma once
#include "afxwin.h"
#include "HListBox.h"
#include "TFTPFileDescriptor.h"
#include "MacProgressCtrl.h"
#include "FontStatic.h"
#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include "Resource.h"
#include "Markup.h"



#include <list>
using namespace std;



// CTFTPServerPane form view

#include <stdlib.h>
#include <string.h> 
#include "TFTPServerFiles\common.h"
#include "TFTPServerFiles\powertft.hpp"  
#include "TFTPServerFiles\tftpp.hh"
#include "afxcmn.h"
#include "hexedit.h"

class CTFTPServer : public CPowerTftp
{
public:
   CTFTPServer(HINSTANCE hinst, HWND hwnd);
	DWORD LastTransferId; // last dwTransferId seen
protected:
   void ConnectEvent(LPCSTR lpszLocalDotAddr, WORD wLocalPort, 
         LPCSTR lpszLocalName, WORD wMaxByteCnt);
   void TftpEvent(TFTP_OPERATION opCode, LPCSTR lpszLocalFileSpec, 
      WORD wBlock, UINT nByteCnt, DWORD dwTransferId, WORD wActiveCnt, 
      LPCSTR lpszRemotedotAddr, WORD wRemotePort, PT_EXCEPTION codeError,
      LPCSTR lpszErrorDesc);

   void ExceptionEvent(PT_EXCEPTION codeError, LPCSTR lpszErrorDesc);
private:
   HWND m_hwndMain;    // identifies the main window
   WORD wLastBlock;  // last block sent/received
};

#define GWL_LPAPPVARS DLGWINDOWEXTRA       
#define MAINWNDEXTRA DLGWINDOWEXTRA + sizeof(LPAPPVARS)  
// Port 69 is the well-known service for TFTP as defined
// by RFC 1060 (Assigned Numbers).
#define IPPORT_TFTP 69               
// Function prototypes for CTL3D.DLL
typedef BOOL (CALLBACK* LPFNCTL3DREGISTER)(HINSTANCE);
typedef BOOL (CALLBACK* LPFNCTL3DAUTOSUBCLASS)(HINSTANCE);
typedef BOOL (CALLBACK* LPFNCTL3DSUBCLASSDLGEX)(HWND, DWORD);  



class CMainFrame;
class CChildFrame;
class CMVTerminalView;



class CTFTPServerPane : public CFormView
{
	DECLARE_DYNCREATE(CTFTPServerPane)
	HDROP             m_hDropInfo;
	BOOL		      m_bTftpServerConnected;
	BOOL		      m_bConnectOnStartup;
	BOOL		      m_bAlreadyConnected;
	BOOL		      m_bTransferEnd;
	list<CTFTPFileDescriptor>  m_fileDescList;



	CMVTerminalView*      m_pTargetView; 
    CString		          m_sLoadAddress;

	CString				  m_sServerIP;
	CXTPButton   		  m_loadBtn;
	CXTPButton   		  m_removeBtn;
	CXTPButton   		  m_removeAllFileBtn;
	CXTPButton   		  m_addBtn;	
	CXTPButton   		  m_connectBtn;
	CXTPButton             m_goBtn;

	CButton     		  m_loadNGoCheckBtn;
	CButton				  m_burnToFSCheckBtn;

	int					  GetFileCount() {return (int)m_fileDescList.size();}
	int					  KillProcessByName(const char *szToTerminate);
	void				  ResetLoadNGoState();
	void				  ResetCtrlStates();
	void			      EnableFileControlls(BOOL bEnable);
	void			      UpdateFileDescriptor(CTFTPFileDescriptor *pFd);
	void			      AllowLoadControlls(BOOL bAllow);
	void				  EnableAllControlls(BOOL bEnable);
	void				  EnableAutoDetectLoadAddressCtrl(BOOL bEnable);
	CTFTPFileDescriptor*  GetSelectedFileDesc();
	CTFTPFileDescriptor*  GetFileDesc(CString sFileName);
	CMVTerminalView*	  GetActiveView();
	LRESULT				  OnFileTransferInProgress(WPARAM a, LPARAM b);
	LRESULT				  OnFileTransferEnd(WPARAM a, LPARAM b); 
	CTFTPFileDescriptor   CreateFileDescriptor(CString sFileName);
	void				  AddFileDescriptor(CTFTPFileDescriptor fd);
	CString				  DetectTftpFileLoadAddress(CString sTftpFilePath);
	CString				  ExtractTftpLoadAddrFromBin(CString sTftpFilePath);
	void			      RemoveTftpFileDesc(CString   sFileName);
	void			      RemoveAllTftpFileDesc();
	bool				  DisconnectTftpServer();
	bool				  ConnectTftpServer();
protected:
	CTFTPServerPane();           // protected constructor used by dynamic creation
	virtual ~CTFTPServerPane();

public:
	enum { IDD = IDD_TFTP_SERVER_PANE };
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:

	void         UpdateChanges();
	afx_msg void OnBnClickedAddTftpFile();
	afx_msg void OnBnClickedRemoveTftpFile();
	afx_msg void OnBnClickedLoadTftpFile();
	afx_msg void OnBnClickedTftpConnectBtn();
	afx_msg void OnBnClickedBurnToFs();
	afx_msg void OnBnClickedLoadNGo();
	void		 AddAndLoadFile(CString sFileName);
	int		     GetProgressBarPos() {return m_TFTPLoadProgressCtrl.GetPos();}
	
	CString		 Serialize();
	BOOL		 DeSerialize(CMarkup & xmlReader);


private:
	CHListBox	 m_fileListCtrl;
	
	// TFTP Server instance
    CTFTPServer* m_pTftpServer;

public:
	virtual void OnInitialUpdate();
	afx_msg void OnBnClickedAsIs();
	afx_msg void OnBnClickedExecutable();
	afx_msg void OnLbnDblclkList();
	afx_msg void OnEnKillfocusProgramArgs();
	afx_msg void OnLbnSelchangeList1();
	CMacProgressCtrl m_TFTPLoadProgressCtrl;
	CFontStatic      m_statusTextCtrl;
	afx_msg void OnEnChangeTftpLoadAddress();
	afx_msg void OnBnClickedRemoveAllTftpFileBtn2();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnPaint();
	afx_msg void OnBnClickedAutoDetectLoadAddr();
	CHexEdit m_loadAddrEdit;
	afx_msg void OnBnClickedTftpFileGoBtn();
	afx_msg void OnBnClickedForceCacheableLoadAddr();
};


/////////////////////////////////////////////////////////////////////////////////////////


