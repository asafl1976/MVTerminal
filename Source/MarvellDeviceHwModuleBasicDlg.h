#pragma once
#include "afxwin.h"
#include "BasicItemDlg.h"
#include "MarvellDeviceHwModule.h"

// CMarvellDeviceHwModuleBasicDlg dialog

class CMarvellDeviceHwModuleBasicDlg : public CBasicItemDlg
{
protected:

	CXTPButton					 m_okBtn;
	CXTPButton					 m_cancelBtn;
	CMarvellDeviceHwModuleParams m_params;

public:
	CMarvellDeviceHwModuleBasicDlg(UINT nIDTemplate, CWnd* pParentWnd = NULL);
	virtual ~CMarvellDeviceHwModuleBasicDlg();

	CMarvellDeviceHwModuleParams      GetParams() {return m_params;}
	void                              SetParams(CMarvellDeviceHwModuleParams params) {m_params = params;}

// Dialog Data
	enum { IDD = IDD_MARVELL_DEVICE_HWMODULE_DLG };

protected:
	virtual void 			DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL 			OnInitDialog();
	afx_msg void 			OnBnClickedCancel();
	afx_msg void 			OnBnClickedReadRadio();
	afx_msg void 			OnBnClickedWriteRadio();
    afx_msg void		    OnBnClickedOk();


	DECLARE_MESSAGE_MAP()
};
