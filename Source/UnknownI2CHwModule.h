#pragma once
#include "Unknownhwmodule.h"

class CUnknownI2CHwModuleParams
{
public:

	UCHAR	 m_data[64];
	BOOL	 m_bStopBit;
	BOOL	 m_bRead;
	int      m_nByteCount;

	CUnknownI2CHwModuleParams()
	{
		m_data[0]  = NULL;
		m_bRead	   = FALSE;
		m_bStopBit = TRUE;
		m_nByteCount = 0;
	}
	CUnknownI2CHwModuleParams(const CUnknownI2CHwModuleParams& obj)
	{
    	memcpy(m_data,obj.m_data,64);
		m_bRead	   = obj.m_bRead;
		m_bStopBit = obj.m_bStopBit;
		m_nByteCount = obj.m_nByteCount;
	}
	void operator=(CUnknownI2CHwModuleParams& obj)
	{
		memcpy(m_data,obj.m_data,64);
		m_bRead	   = obj.m_bRead;
		m_bStopBit = obj.m_bStopBit;
		m_nByteCount = obj.m_nByteCount;
	}
};

class CUnknownI2CHwModule : public CUnknownHwModule
{
	int			m_nHwModuleID;
public:
	CUnknownI2CHwModule();
	CUnknownI2CHwModule(CHwModuleInfo* pInfo);
    virtual ~CUnknownI2CHwModule(); 

	virtual BOOL		 WriteData(UCHAR nSubAddress, int nBytes, UCHAR *pWriteData);
    virtual BOOL		 ReadData(UCHAR nSubAddress, int nBytes, UCHAR *pReadData);
	virtual BOOL   		 WriteData(int nBytes, UCHAR *pWriteData, BOOL bSendStopBit = TRUE);
    virtual BOOL		 ReadData(int nBytes, UCHAR *pReadData, BOOL bSendStopBit = TRUE);
	virtual CHwModule*   Clone(CHwModuleInfo* pInfo);

};

