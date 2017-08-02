#pragma once
#include "BasicItemDlg.h"
#include "resource.h"
#include "ClockETPHwModule.h"


class CClockETPHwModuleBasicDlg : public CBasicItemDlg
{
protected:
	CXTPButton				m_exeBtn;
	CXTPButton				m_cancelBtn;
	CClockETPParams			m_params;

public:
	// Dialog Data
	enum { IDD = IDD_CLOCK_ETP_DLG };

	CClockETPHwModuleBasicDlg(UINT nIDTemplate, CWnd* pParentWnd = NULL);
	virtual ~CClockETPHwModuleBasicDlg(void);

protected:
	virtual void			DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL			OnInitDialog();
	virtual CClockETPParams GetParams(){return m_params;}
	virtual void            SetParams(CClockETPParams params) {m_params = params;}

	afx_msg void			OnBnClickedOk();

	DECLARE_MESSAGE_MAP()
};
