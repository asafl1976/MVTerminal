#pragma once


#include "odpghwmodule.h"

class CIODPGConfigStrategy
{
protected:
	CODPGHwModule*    m_pODPG;
	unsigned int    m_nRegBase;
	CString			m_sDevName;
	CString			m_sDevBusWidth;
	int			    m_nBufLineByteCount;
	int			    m_nBufLineCount;
public:
	CIODPGConfigStrategy(CODPGHwModule *pODPG);
	virtual ~CIODPGConfigStrategy(void);
	virtual  unsigned int GetDeviceRegBase() {return m_nRegBase;}
	virtual  CString GetDeviceBusWidth() {return m_sDevBusWidth;}
	virtual  CString GetDeviceName()  {return m_sDevName;}
	virtual  int     GetBufLineByteCount() {return m_nBufLineByteCount;}
	virtual  int     GetBufLineCount() {return m_nBufLineCount;}

	virtual  BOOL    InitODPG()  = 0;
	virtual  BOOL    StartODPG() = 0;
	virtual  BOOL    StopODPG()  = 0;
};
