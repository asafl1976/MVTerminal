#pragma once
#include "afxwin.h"
#include "resource.h"
#include "BasicItemDlg.h"
#include "SummitSMM205HwModule.h"

// CSummitSMM205HwModuleBasicDlg dialog

class CSummitSMM205HwModuleBasicDlg : public CBasicItemDlg
{
protected:
	CXTPButton			    	m_exeBtn;
	CXTPButton			    	m_cancelBtn;
	CXTPButton			    	m_settingBtn;
	CSummitSMM205Params     	m_params;

public:
	CSummitSMM205HwModuleBasicDlg(UINT nIDTemplate, CWnd* pParentWnd = NULL);   // standard constructor
	virtual ~CSummitSMM205HwModuleBasicDlg();

// Dialog Data
	enum { IDD = IDD_SUMMIT_SMM205_DLG };

protected:
	virtual void				DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual CSummitSMM205Params GetParams(){return m_params;}
	virtual void                SetParams(CSummitSMM205Params params) {m_params = params;}

	virtual BOOL				OnInitDialog();
	afx_msg void			 	OnBnClickedSummitSmm205CtrlWriteRadio();
	afx_msg void			 	OnBnClickedSummitSmm205CtrlReadRadio();
	afx_msg void			 	OnBnClickedOk();

};
