#pragma once

class CHwModuleItem;

class CHwModuleBoardPowerCtrlStartegy
{
protected:

	CHwModuleItem*							m_pHwModuleItem;
	int									    m_nIO;
	CString									m_sIOInitialState;
	CString									m_sName;

public:
	CHwModuleBoardPowerCtrlStartegy();
	virtual ~CHwModuleBoardPowerCtrlStartegy(void);

	virtual CString GetName() {return m_sName;}
	virtual void SetIO(int nIO);
	virtual void SetIOInitialState(CString sState);
	virtual void SetHwModuleItem(CHwModuleItem *pItem);
};
