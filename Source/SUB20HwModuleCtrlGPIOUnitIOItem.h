#pragma once
#include "sub20hwmodulectrlgpiounititem.h"
#include "HwModuleCtrlGPIOUnitDrv.h"


class CGPIOConfig
{
public:

	CString m_sDisplayName;
	int     m_nState;
	int     m_nType;
	BOOL    m_bEnable;

	CGPIOConfig()
	{
		m_bEnable = TRUE;
	}
	CGPIOConfig(CString sDisplayName,int nState, int nType, BOOL bEnable = TRUE)
	{
		m_sDisplayName = sDisplayName;
		m_nState       = nState;
		m_nType        = nType;
		m_bEnable      = bEnable;

	}
};


class CSUB20HwModuleCtrlGPIOUnitIOItem : public CSUB20HwModuleCtrlGPIOUnitItem
{
	int	 m_nIndex;
	BOOL m_bEnable;

public:
	CSUB20HwModuleCtrlGPIOUnitIOItem(int nIndex, CSUB20HwModuleCtrlGPIOUnit *pUnit);
	virtual ~CSUB20HwModuleCtrlGPIOUnitIOItem(void);

	virtual int     GetIndex() {return m_nIndex;}
	
	virtual CString	Serialize();
	virtual BOOL	DeSerialize(CMarkup& xmlReader);

	virtual BOOL    SetConfig(CGPIOConfig cfg);
	virtual BOOL    GetConfig(CGPIOConfig& cfg);

	virtual BOOL    GetState(int& nState);
	virtual BOOL    SetState(int  nState);

	virtual BOOL    GetType(int& nType);
	virtual BOOL    SetType(int  nType);

	virtual BOOL    GetEnable() {return m_bEnable;}
};
