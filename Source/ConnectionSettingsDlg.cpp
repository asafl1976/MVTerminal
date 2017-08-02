// ConnectionSettingsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MVTerminal.h"
#include "ConnectionSettingsDlg.h"
#include ".\connectionsettingsdlg.h"
#include "Markup.h"

#define  _COM_PORT_ID (WM_USER+11)

// CPortPropetiesDlg dialog

IMPLEMENT_DYNAMIC(CConnectionSettingsDlg, CDialog)

CConnectionSettingsDlg::CConnectionSettingsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConnectionSettingsDlg::IDD)
{
	m_nDlgType = NEW_SESSION_DLG_TYPE;
	m_bValid   = TRUE;
}

CConnectionSettingsDlg::~CConnectionSettingsDlg()
{
}

void CConnectionSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BAUD_RATE_COMBO, m_baudRateCombo);
	DDX_Control(pDX, IDC_COM_PORT_COMBO, m_comPortCombo);

	DDX_Control(pDX, IDC_DATA_BITS_COMBO, m_dataBitsCombo);
	DDX_Control(pDX, IDC_PARITY_COMBO, m_parityCombo);
	DDX_Control(pDX, IDC_STOP_BITS_COMBO, m_stopBitsCombo);
	DDX_Control(pDX, IDC_RESTORE_DEFAULT_BTN ,m_restoreDefaultBtn);
}


BEGIN_MESSAGE_MAP(CConnectionSettingsDlg, CDialog)
	ON_CBN_SELCHANGE(IDC_BAUD_RATE_COMBO, OnCbnSelchangeBaudRateCombo)
//	ON_WM_CLOSE()
ON_BN_CLICKED(IDC_RESTORE_DEFAULT_BTN, OnBnClickedRestoreDefaultBtn)
END_MESSAGE_MAP()


// CPortPropetiesDlg message handlers

void CConnectionSettingsDlg::OnOK()
{
	CString			  sComPortTemp, sWindowNameTemp, sBaudRateTemp, sDataBits, sParity, sStopBits; 
	int				  nComPort;
	CString	          comPortStr;
	CString			  sComSettings;

	m_bPortAlreadyOpen = FALSE;
	// Get current data

	GetDlgItemText(IDC_BAUD_RATE_COMBO,sBaudRateTemp);
	m_connectionParams.SetBaudRate(sBaudRateTemp);

	GetDlgItemText(IDC_WND_NAME_COMBO,sWindowNameTemp);
	m_connectionParams.SetWndName(sWindowNameTemp);

	GetDlgItemText(IDC_DATA_BITS_COMBO,sDataBits);
	m_connectionParams.SetDataBits(sDataBits);

	GetDlgItemText(IDC_PARITY_COMBO,sParity);
	m_connectionParams.SetParity(sParity);

	GetDlgItemText(IDC_STOP_BITS_COMBO,sStopBits);
	m_connectionParams.SetStopBits(sStopBits);

	GetDlgItemText(IDC_COM_PORT_COMBO,sComPortTemp);
	if(sComPortTemp == m_connectionParams.GetComPort())
		return;

	switch(m_nDlgType)
	{
	case PROPERTIES_DLG_TYPE:	
		// Get the COM num and baudrate
		comPortStr = sComPortTemp;
		comPortStr = sComPortTemp.Right(m_connectionParams.GetComPort().GetLength() - 3);
 		sscanf((LPCTSTR)comPortStr,"%d",&nComPort);

		/* Just for test - n = parity , 8 = number of data-bits, 1 = number of stop bits */
		sComSettings  = m_connectionParams.GetBaudRate();
		sComSettings += "," + sParity.Left(1);//Get the first char only
		sComSettings += "," + sDataBits;
		sComSettings += "," + sStopBits;
		if(m_SPCommCtrl.Create(NULL, WS_VISIBLE, CRect(0,0,0,0),
			this,_COM_PORT_ID, NULL, FALSE) != TRUE)
		{			
			MessageBox("Failed to create test COM port!",NULL,MB_ICONERROR);
			return ;
		}
		m_SPCommCtrl.InitString(sComSettings);
		if(m_SPCommCtrl.Open(sComPortTemp) == FALSE)
		{		
			MessageBox(sComPortTemp + " is already open!",NULL,MB_ICONERROR);
			m_bPortAlreadyOpen = TRUE;
			return;
		}
		// Close port since its just for test..
		m_SPCommCtrl.Close();
		break;
	case NEW_SESSION_DLG_TYPE:	
		break;
	default:
		break;
	}
	m_connectionParams.SetComPort(sComPortTemp);		
	return ;
}



void   CConnectionSettingsDlg::SetDlgType(int  nType)
{
	m_nDlgType = nType;
}

int   CConnectionSettingsDlg::GetDlgType()
{
	return m_nDlgType;
}


CString CConnectionSettingsDlg::GetFirstCOMPort(vector<CString>& ComPortVec)
{
	vector<CString>::iterator iter,endIter;
	CString  sStr, sLowestCom; 
	int		 nComNum, nLowestComNum;
	
	if(ComPortVec.size() == 0)
		return sLowestCom;

	iter    = ComPortVec.begin();
	endIter = ComPortVec.end();

	sLowestCom = sStr = *iter;
	sStr = sStr.Right(sStr.GetLength() - 3);
	sscanf(sStr,"%d",&nComNum);
	nLowestComNum = nComNum;
	iter++;

	for(; iter != endIter; iter++)
	{
		sStr = *iter;
		sStr = sStr.Right(sStr.GetLength() - 3);
		sscanf(sStr,"%d",&nComNum);
		if(nComNum < nLowestComNum)
		{
			nLowestComNum = nComNum;
			sLowestCom = *iter;
		}
	}
	return sLowestCom;
}

BOOL CConnectionSettingsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString sCom;
	int		nCom; 
	vector<CString>::iterator iter,endIter;
	vector<CString>			  UnusedComPortVec;

	m_restoreDefaultBtn.SetTheme(xtpButtonThemeOffice2000);

	m_comPortVec.clear();
 	if(((CMVTerminalApp*)AfxGetApp())->EnumerateSerialPorts(m_comPortVec) == 0)
	{	
		/* Add defualt ports incase enumaration failed... */
		for(nCom = 1; nCom < 15; nCom++)
		{
			sCom.Format("COM%d",nCom);
			m_comPortVec.push_back(sCom);
		}
	}
	

	switch(m_nDlgType)
	{
	case PROPERTIES_DLG_TYPE:

		iter    = m_comPortVec.begin();
		endIter = m_comPortVec.end();
		for(; iter != endIter; iter++)
		{
			m_comPortCombo.AddString(*iter);
		}
		break;
	case NEW_SESSION_DLG_TYPE:
		if(m_SPCommCtrl.Create(NULL, WS_VISIBLE, CRect(0,0,0,0),
						this,_COM_PORT_ID, NULL, FALSE) != TRUE)
		{
			return FALSE;
		}
		iter    = m_comPortVec.begin();
		endIter = m_comPortVec.end();
		for(; iter != endIter; iter++)
		{
			// List only un-used ports
			if(m_SPCommCtrl.Open(*iter) == TRUE)
			{
				m_SPCommCtrl.Close();
				m_comPortCombo.AddString(*iter);
				UnusedComPortVec.push_back(*iter);
			}
		}
		/* Select first COM port */
		m_connectionParams.SetComPort(GetFirstCOMPort(UnusedComPortVec));
		m_bValid = TRUE;
		if(m_comPortCombo.GetCount() == 0)
		{	
			m_comPortCombo.AddString("No Active Ports");
			m_comPortCombo.SelectString(-1,"No Active Ports");
			m_bValid = FALSE;
			return FALSE;
		}

		break;
	default:
		break;
	}

	UpdateWindowData();

    return FALSE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CConnectionSettingsDlg::OnCbnSelchangeBaudRateCombo()
{
	// TODO: Add your control notification handler code here
}

void CConnectionSettingsDlg::OnBnClickedRestoreDefaultBtn()
{
	CString		sComPort;
	int			nIndex = 0;

	// Search for the first availiable port
	sComPort.Format("COM%d",nIndex);
	while(m_comPortCombo.SelectString(-1,sComPort) == CB_ERR)
	{
		nIndex++;
		sComPort.Format("COM%d",nIndex);
	}

    m_baudRateCombo.SelectString(-1,"115200");
	m_comPortCombo.SelectString(-1,sComPort);
	m_dataBitsCombo.SelectString(-1,"8");
	m_parityCombo.SelectString(-1,"None");
	m_stopBitsCombo.SelectString(-1,"1");
}


CString CConnectionSettingsDlg::Serialize()
{
	CMarkup xmlWriter;

	xmlWriter.AddElem("ConnectionSettings");
	xmlWriter.AddAttrib("COMPort", m_connectionParams.GetComPort());
	xmlWriter.AddAttrib("BaudRate", m_connectionParams.GetBaudRate());
	xmlWriter.AddAttrib("DataBits", m_connectionParams.GetDataBits());
	xmlWriter.AddAttrib("Parity", m_connectionParams.GetParity());
	xmlWriter.AddAttrib("Name", m_connectionParams.GetWndName());

	return xmlWriter.GetDoc();
}

BOOL CConnectionSettingsDlg::DeSerialize(CMarkup & xmlReader)
{
	if( xmlReader.FindElem("ConnectionSettings") == false)
		return FALSE;

	m_connectionParams.SetComPort( xmlReader.GetAttrib("COMPort") );
	m_connectionParams.SetBaudRate( xmlReader.GetAttrib("BaudRate") );
	m_connectionParams.SetDataBits( xmlReader.GetAttrib("DataBits") );
	m_connectionParams.SetParity( xmlReader.GetAttrib("Parity") );
	m_connectionParams.SetWndName( xmlReader.GetAttrib("Name") );

	return TRUE;
}



void CConnectionSettingsDlg::UpdateWindowData()
{
	CString					  sComPort;
	vector<CString>::iterator iter,endIter;
	int						  nIndex = 0;

	iter    = m_comPortVec.begin();
	endIter = m_comPortVec.end();
	// Incase restore was done and COM port do not exist 
	for(; iter != endIter; iter++)
	{
		// List only un-used ports
		if(GetConnectionParams().GetComPort() == (*iter))
		{
			sComPort = GetConnectionParams().GetComPort();
			break;
		}
	}
	// If restored com port was not found... use the first in list
	if(iter == endIter)
		sComPort =  *m_comPortVec[m_comPortVec.size() - 1];

	m_comPortCombo.SelectString(-1,sComPort);
	m_baudRateCombo.SelectString(-1,GetConnectionParams().GetBaudRate());
	m_dataBitsCombo.SelectString(-1,GetConnectionParams().GetDataBits());
	m_parityCombo.SelectString(-1,GetConnectionParams().GetParity());
	m_stopBitsCombo.SelectString(-1,GetConnectionParams().GetStopBits());
	SetDlgItemText(IDC_WND_NAME_COMBO,GetConnectionParams().GetWndName());
}

