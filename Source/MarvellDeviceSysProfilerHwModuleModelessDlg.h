#pragma once

#include "MarvellDeviceSysProfilerHwModule.h"
#include "MarvellDeviceSysProfilerHwModuleBasicDlg.h"
#include "IHwModuleItemDlg.h"
#include "SysProfilerObserver.h"

// 

class CMarvellDeviceSysProfilerHwModuleModelessDlg : public CMarvellDeviceSysProfilerHwModuleBasicDlg, public IHwModuleItemDlg, ISysProfilerObserver
{

	CWinThread*						    m_pThread;
protected:
	static	int							m_nWinPos;
public:
	CMarvellDeviceSysProfilerHwModuleModelessDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMarvellDeviceSysProfilerHwModuleModelessDlg();

	virtual void			SetHwModuleItem(CHwModuleItem *pItem);
	virtual BOOL			Create(CWnd* pParentWnd);
	virtual CBasicItemDlg * Clone();
	virtual void		    UpdateGraph(CSysProfilerEvent* pEvent, int nGraphIndex,double fEventSampleData, double fEventSampleRate);
	virtual BOOL		    SaveSettingsToFile();
	virtual BOOL			LoadSettingsFromFile();
	virtual void			OnPause();
	virtual void			OnStart();

	afx_msg BOOL			UpdateUIGraphData();
	CMarvellDeviceSysProfilerHwModule*  m_pMarvellDeviceSysProfilerHwModule;

// Dialog Data
	enum { IDD = IDD_MARVELL_DEVICE_SYSTEM_PROFILER_HWMODULE_DLG };
protected:

	virtual void			OnSysProfilerModelAddedEvent(CSysProfilerEvent *pEvent);
	virtual void			OnSysProfilerModelAddedLogLine(CString sLogLine);
	virtual void		    OnSysProfilerModelRemoveAllEvents();

	virtual void			DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void		    AlignWindow();
	virtual BOOL			OnInitDialog();
	virtual void			OnBnClickedOk();
	afx_msg void			OnBnClickedSummitDlgSettingsBtn();

	//Modeless 
	virtual void			PostNcDestroy();
	afx_msg void			OnDestroy();
	virtual void			OnCancel();
	
	DECLARE_MESSAGE_MAP()
};
