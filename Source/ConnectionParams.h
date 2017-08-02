#pragma once

class CConnectionParams
{

	CString         m_sDataBits;
	int 		    m_nDataBits;

	CString         m_sStopBits;
	int 		    m_nStopBits;

	CString         m_sParity;

	CString         m_sBaudRate;
	int 		    m_nBaudRate;

	CString         m_sComPort;
	int 		    m_nComPort;

	CString         m_sWindowName;

public:
	CConnectionParams(void);
	~CConnectionParams(void);

	void           Set(CString  sDataBits, CString  sStopBits, CString  sParity, CString  sBoudRate, CString  sComPort, CString  sWndName);
	
	void		   SetDataBits(CString  sDataBits);
	CString		   GetDataBits();

	void		   SetStopBits(CString  sStopBits);
	CString		   GetStopBits();

	void		   SetParity(CString  sParity);
	CString		   GetParity();

	void		   SetBaudRate(CString  sBoudRate);
	CString		   GetBaudRate();
	int  		   GetBaudRateVal();

	void		   SetComPort(CString  sComPort);
	CString        GetComPort();
	int            GetComPortVal();

	void		   SetWndName(CString  sWndName);
	CString		   GetWndName();

	bool operator==(CConnectionParams & connectionParams);

};
