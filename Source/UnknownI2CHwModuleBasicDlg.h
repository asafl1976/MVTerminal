#pragma once
#include "resource.h"
#include "afxwin.h"
#include "UnknownI2CHWModule.h"
#include "BasicItemDlg.h"
// CUnknownI2CHwModuleBasicDlg dialog

class CUnknownI2CHwModuleBasicDlg : public CBasicItemDlg
{ 
protected:

	CXTPButton       			m_exeBtn;
	CXTPButton       			m_cancelBtn;
	CXTHexEdit					m_dataHexEdit;
	CXTMaskEdit					m_dataHexEditBaseAddress;
	CUnknownI2CHwModuleParams   m_params;

public:
	CUnknownI2CHwModuleBasicDlg(UINT nIDTemplate, CWnd* pParentWnd = NULL);
	virtual ~CUnknownI2CHwModuleBasicDlg();

// Dialog Data
	enum { IDD = IDD_UNKNOWN_I2C_DEV_CTRL_DLG };

protected:
	virtual void		 DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void		 ResetData();
	afx_msg void 		 OnBnClickedOk();
	afx_msg void 		 OnBnClickedCancel();
	virtual BOOL 		 OnInitDialog();
	afx_msg void 		 OnEnSetfocusGenI2cDevCtrlHexEdit();
	afx_msg void 		 OnBnClickedGenI2cDevCtrlReadRadio();
	afx_msg void 		 OnBnClickedGenI2cDevCtrlWriteRadio();
	afx_msg void 		 OnBnClickedGenI2cDevCtrlSendStopCheck();

	virtual CUnknownI2CHwModuleParams GetParams(){return m_params;}
	virtual void         SetParams(CUnknownI2CHwModuleParams params) {m_params = params;}

	DECLARE_MESSAGE_MAP()
};
