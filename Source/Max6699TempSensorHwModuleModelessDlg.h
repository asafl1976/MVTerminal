#pragma once
#include "max6699tempsensorhwmodulebasicdlg.h"
#include "IHwModuleItemDlg.h"
#include "Max6699TempSensorHwModule.h"

class CMax6699TempSensorHwModuleModelessDlg : public CMax6699TempSensorHwModuleBasicDlg, public IHwModuleItemDlg
{
	static	int				     m_nWinPos;
	CMax6699TempSensorHwModule*  m_pTempSensorHwModule;

public:
	CMax6699TempSensorHwModuleModelessDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMax6699TempSensorHwModuleModelessDlg();

	virtual void			SetHwModuleItem(CHwModuleItem *pItem);
	virtual BOOL			Create(CWnd* pParentWnd);
	virtual CBasicItemDlg * Clone();
	virtual void            UpdateUIGraphData();

// Dialog Data
	enum { IDD = IDD_TEMPERATURE_MONITOR_DLG };

	virtual void	StopUpdateThread();
	virtual void	StartUpdateThread();

	protected:
	virtual void			DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void		    AlignWindow();
	virtual BOOL			OnInitDialog();
	//Modeless 
	virtual void			PostNcDestroy();
	afx_msg void			OnDestroy();
	virtual void			OnCancel();

	DECLARE_MESSAGE_MAP()

};
