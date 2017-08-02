#pragma once
#include <map>
using namespace std;
#include "HwModuleBoardPowerCtrlStartegy.h"

class CHwModuleBoardPowerCtrlPowerStartegy;
class CHwModuleBoardPowerCtrlResetStartegy;


class CBoardPowerCtrlManager
{
	map<CString,CHwModuleBoardPowerCtrlPowerStartegy*> m_powerStartegyMap;
	map<CString,CHwModuleBoardPowerCtrlResetStartegy*> m_resetStartegyMap;
	
	CString m_sPowerStartegy;
	CString m_sResetStartegy;

	CHwModuleBoardPowerCtrlPowerStartegy *m_pPowerStartegy;
	CHwModuleBoardPowerCtrlResetStartegy *m_pResetStartegy;

	CBoardPowerCtrlManager(void);
public:
	static CBoardPowerCtrlManager& GetInstance();
	virtual ~CBoardPowerCtrlManager(void);

	virtual BOOL SetPowerStartegy(CHwModuleItem* pItem, int nIO, CString sIOInitialState);
	virtual BOOL SetResetStartegy(CHwModuleItem* pItem, int nIO, CString sIOInitialState);

	virtual VOID ResetPowerStartegy();
	virtual VOID ResetResetStartegy();

	virtual CHwModuleBoardPowerCtrlPowerStartegy* GetPowerStartegy() {return m_pPowerStartegy;}
	virtual CHwModuleBoardPowerCtrlResetStartegy* GetResetStartegy() {return m_pResetStartegy;}

	virtual BOOL PowerOn();
	virtual BOOL PowerOff();
	virtual BOOL Reset(int nPulseLen);
};
