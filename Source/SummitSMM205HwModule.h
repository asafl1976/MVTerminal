#pragma once
#include "VoltageCtrlHwModule.h"
#include ".\summitsmm205hwmoduleaccessstartegy.h"

class CSummitSMM205HwModuleAccessStartegy;

class CSummitSMM205Params
{
public:
	BOOL	m_bWrite;
	int  	m_nVoltage;
	int  	m_nExtReference;

	CSummitSMM205Params()
	{
		m_bWrite        = TRUE;
		m_nVoltage      = 901;
		m_nExtReference = 900;
	}
};

class CSummitSMM205HwModule : public CVoltageCtrlHwModule
{
	CSummitSMM205HwModuleAccessStartegy* m_pAccessStrategy;
	int					m_nMin;
	int					m_nMax;
	int					m_nExtReference;
public:
	CSummitSMM205HwModule(void);
	CSummitSMM205HwModule(CHwModuleInfo* pInfo);
	virtual ~CSummitSMM205HwModule(void);

	virtual void 		 SetActiveChannel(VOLTAGE_CTRL_CHANNEL eChannel);
	virtual VOLTAGE_CTRL_CHANNEL   GetActiveChannel();

	virtual int			 GetExtReferenceValue() {return m_nExtReference;}
	virtual int			 GetMinValue() {return m_nMin;}
	virtual int			 GetMaxValue() {return m_nMax;}

	virtual void		 SetExtReferenceValue(int nExtReference) {m_nExtReference = nExtReference;}
	virtual void		 SetMinValue(int nMin) {m_nMin = nMin;}
	virtual void		 SetMaxValue(int nMax) {m_nMax = nMax;}

	virtual BOOL  		 SetVoltage(int nVoltage);
	virtual BOOL		 GetVoltage(int* pVoltage);

	virtual  CString	 Serialize();
	virtual  BOOL   	 DeSerialize(CMarkup& xmlReader);

	virtual CHwModule*   Clone(CHwModuleInfo* pInfo);
};

