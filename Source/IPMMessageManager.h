#pragma once

#include "IPMMessage.h"
#include "IPMMessageSenderAPI.h"

class CMainFrame;

typedef  HWND  (* IPMGetProcessWindowHandleFunc)(char* pProcessName);
typedef  LRESULT  (* IPMSendMessageFunc)(HWND hWnd, CIPMMessage *pMsg);



class CIPMMessageManager
{
	IPMSendMessageFunc             m_IPMSendMessageFunc;
    IPMGetProcessWindowHandleFunc  m_IPMGetProcessWindowHandleFunc;
 
	HMODULE		                   m_hIPMDLLModule;
	CMainFrame                    *m_pMainframe;
	LPVOID						   m_retDataFileView;

public:
	CIPMMessageManager();
	virtual      ~CIPMMessageManager(void);
	
	virtual BOOL Init(CMainFrame *pMainframe);
	virtual BOOL HandleMessage(CIPMMessage *pMsg);

	//MVTerminal view messages
	virtual BOOL WriteComPortString(CIPMMessage *pMsg);
	virtual BOOL ReadComPortString(CIPMMessage *pMsg);
	virtual BOOL ClearComPortWindow(CIPMMessage *pMsg);
	virtual BOOL TransmitPlatformInfo(CIPMMessage *pMsg);

	//UFS
	virtual BOOL UFSOpenFile(CIPMMessage *pMsg);
	virtual BOOL UFSCloseFile(CIPMMessage *pMsg);
	virtual BOOL UFSReadLine(CIPMMessage *pMsg);
	virtual BOOL UFSWriteLine(CIPMMessage *pMsg);

	//HwModule model messages
	virtual BOOL MarvellDeviceHwModuleReadRegister(CIPMMessage *pMsg);
	virtual BOOL MarvellDeviceHwModuleWriteRegister(CIPMMessage *pMsg);
	virtual BOOL VoltageCtrlHwModuleSetActiveChannel(CIPMMessage *pMsg);
	virtual BOOL VoltageCtrlHwModuleGetActiveChannel(CIPMMessage *pMsg);
	virtual BOOL VoltageCtrlHwModuleSetVoltage(CIPMMessage *pMsg);
	virtual BOOL VoltageCtrlHwModuleGetVoltage(CIPMMessage *pMsg);
	virtual BOOL FrequencyCtrlHwModuleSetFrequency(CIPMMessage *pMsg);
	virtual BOOL ADCHwModuleCtrlUnitGetVoltage(CIPMMessage *pMsg);
	virtual BOOL ADCHwModuleCtrlUnitGetCurrent(CIPMMessage *pMsg);
	virtual BOOL GPIOHwModuleCtrlUnitSetResetLow(CIPMMessage *pMsg);
	virtual BOOL GPIOHwModuleCtrlUnitSetResetHigh(CIPMMessage *pMsg);
	virtual BOOL GPIOHwModuleCtrlUnitReset(CIPMMessage *pMsg);
	virtual BOOL GPIOHwModuleCtrlUnitSetPowerOn(CIPMMessage *pMsg);
	virtual BOOL GPIOHwModuleCtrlUnitSetPowerOff(CIPMMessage *pMsg);
	virtual BOOL GPIOHwModuleCtrlUnitGetGPIO(CIPMMessage *pMsg);
	virtual BOOL GPIOHwModuleCtrlUnitSetGPIO(CIPMMessage *pMsg);
	

};
