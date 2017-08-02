#include "StdAfx.h"
#include ".\connectionparams.h"

CConnectionParams::CConnectionParams(void)
{
	/* Set default values */
	m_sComPort = "COM1";
	m_nComPort = 1;

	m_sBaudRate = "115200";
	m_nBaudRate = 115200;

	m_sDataBits = "8";
	m_nDataBits = 8;

	m_sParity   = "None";

	m_sStopBits = "1";
	m_nStopBits = 1;

	m_sWindowName = "";

}

CConnectionParams::~CConnectionParams(void)
{
}

void	CConnectionParams::SetBaudRate(CString  sBoudRate)
{
	m_sBaudRate = sBoudRate;
	if(m_sBaudRate.IsEmpty() == FALSE)
		sscanf_s((LPCTSTR)m_sBaudRate,"%d",&m_nBaudRate);
	else
		m_nBaudRate = 0;
}
CString	   CConnectionParams::GetBaudRate()
{
	return m_sBaudRate;
}

int		   CConnectionParams::GetBaudRateVal()
{
	return m_nBaudRate;
}

void	CConnectionParams::SetComPort(CString  sComPort)
{
	m_sComPort = sComPort;
	if(m_sComPort.IsEmpty() == FALSE)
	{
		CString tmp = m_sComPort; 
		tmp = tmp.Right(tmp.GetLength() - 3);
		sscanf_s((LPCTSTR)tmp,"%d",&m_nComPort);
	}
	else
		m_nComPort = 0;
}
CString    CConnectionParams::GetComPort()
{
	return m_sComPort;
}
int        CConnectionParams::GetComPortVal()
{
	return m_nComPort;
}

void	   CConnectionParams::SetWndName(CString  sWndName)
{
	m_sWindowName = sWndName;
}
CString		   CConnectionParams::GetWndName()
{
	return m_sWindowName;
}

void   CConnectionParams::Set(CString  sDataBits, CString  sStopBits, CString  sParity, CString  sBoudRate,CString  sComPort,CString  sWndName)
{
	SetDataBits(sDataBits);
	SetStopBits(sStopBits);
	SetParity(sParity);
	SetBaudRate(sBoudRate);
	SetComPort(sComPort);
	SetWndName(sWndName);
}

void	CConnectionParams::SetDataBits(CString  sDataBits)
{
	m_sDataBits = sDataBits;
	if(m_sDataBits.IsEmpty() == FALSE)
		sscanf_s((LPCTSTR)m_sDataBits,"%d",&m_nDataBits);
	else
		m_nDataBits = 0;
}

CString	   CConnectionParams::GetDataBits()
{
	return m_sDataBits;
}
void	CConnectionParams::SetStopBits(CString  sStopBits)
{
	m_sStopBits = sStopBits;
	if(m_sStopBits.IsEmpty() == FALSE)
		sscanf_s((LPCTSTR)m_sStopBits,"%d",&m_nStopBits);
	else
		m_nStopBits = 0;
}

CString	   CConnectionParams::GetStopBits()
{
	return m_sStopBits;
}

void	CConnectionParams::SetParity(CString  sParity)
{
	m_sParity = sParity;
}

CString	   CConnectionParams::GetParity()
{
	return m_sParity;
}

bool CConnectionParams::operator==(CConnectionParams & connectionParams)
{
	return ((connectionParams.GetBaudRate() == m_sBaudRate) && 
		    (connectionParams.GetComPort() == m_sComPort)   && 
		    (connectionParams.GetDataBits() == m_sDataBits) && 
		    (connectionParams.GetParity() == m_sParity)     && 
		    (connectionParams.GetStopBits() == m_sStopBits) &&
		    (connectionParams.GetWndName() == m_sWindowName)); 
}



