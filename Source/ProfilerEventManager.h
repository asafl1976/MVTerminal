#pragma once
#include "SysProfilerEvent.h"
#include "SysProfilerTimeStampEvent.h"
#include "SysProfilerBeforeEvents.h"
#include "SysProfilerAfterEvents.h"
#include "MarvellDeviceSysProfilerHwModule.h"
#include "SysProfilerSubject.h"
#include "SysProfilerObserver.h"
#include "SysProfilerInit.h"
#include <string>
#include <vector>
using namespace std;

class CProfilerEventManager : public ISysProfilerSubject
{
	BOOL							    m_bEnableLogging;
	BOOL								m_bInitialized;
	CString								m_sConfigFilePath;
	CSysProfilerInit*                   m_pSystemInit;
	CSysProfilerBeforeEvents*			m_pBeforeEvents;
	CSysProfilerAfterEvents*			m_pAfterEvents;
	CSysProfilerTimeStampEvent*			m_pTimeStampEvent;
	vector<CSysProfilerBasicEvent*>		m_events;
	vector<ISysProfilerObserver*>		m_observers;
	double								m_globalDataArray[128];
	CMarvellDeviceSysProfilerHwModule*  m_pMarvellDeviceSysProfilerHwModule;
	virtual void					    NotifySysProfilerModelAddedEvent(CSysProfilerEvent *pEvent);
	virtual void						NotifySysProfilerModelAddedLogLine(CString sLogLine);
	virtual void						NotifySysProfilerModelRemoveAllEvents();
	virtual void						AddEvent(CSysProfilerEvent* pEvent);
	virtual void						DeleteAllEvents();
	CProfilerEventManager(void);
public:

	static CProfilerEventManager& GetInstance();
	~CProfilerEventManager(void);
	
	virtual void					UnRegisterObserver(ISysProfilerObserver *pObs);
	virtual void					RegisterObserver(ISysProfilerObserver *pObs);
	virtual BOOL					ReadMarvellDeviceRegister(UINT32 nRegAddress, UINT32 *pRegVal);
	virtual BOOL					WriteMarvellDeviceRegister(UINT32 nRegAddress,UINT32 nVal);
	virtual void				    SetGlobalDataArrayValue(int nIndex, double nData);
	virtual double					GetGlobalDataArrayValue(int nIndex);
	virtual void                    RemoveAllEvents();

	virtual BOOL				    SystemInit();
	virtual void				    SetHwModuleItem(CMarvellDeviceSysProfilerHwModule *pItem);
	virtual BOOL					LoadConfigFile(CString sConfigFilePath);
	virtual int						GetEventCount();
	virtual CSysProfilerBasicEvent* GetEvent(int nIndex);
	virtual BOOL                    InitTimeStamp();
	virtual double                  GetTimeStamp();
	virtual void                    OnEventsBegin();
	virtual void                    OnEventsEnd();
	virtual void                    AddLogLine(CString sLine);
	virtual void                    SetEnableLogging(BOOL bEnable) {m_bEnableLogging = bEnable;}
	virtual BOOL                    GetEnableLogging()			   {return m_bEnableLogging;}


};
