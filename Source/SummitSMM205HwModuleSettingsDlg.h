#pragma once
#include "afxwin.h"
#include ".\voltagectrlhwmodule.h"


// CSummitSMM205HwModuleSettingsDlg dialog

class CSummitSMM205HwModuleSettingsDlg : public CDialog
{
	DECLARE_DYNAMIC(CSummitSMM205HwModuleSettingsDlg)
	CXTPButton m_okBtn;
	CXTPButton m_cancelBtn;

	CString   m_sName;
	CString   m_sMin;
	CString   m_sMax;
	CString   m_sExtRef;
	VOLTAGE_CTRL_CHANNEL   m_eActiveChannel;

public:
	CSummitSMM205HwModuleSettingsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSummitSMM205HwModuleSettingsDlg();

// Dialog Data
	enum { IDD = IDD_SUMMIT_SMM205_SETTINGS_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	
	virtual void    SetName(CString sName) {m_sName = sName;}
	virtual void    SetExtRef(CString sExtRef)  {m_sExtRef = sExtRef;}
	virtual void    SetMin(CString sMin)  {m_sMin = sMin;}
	virtual void    SetMax(CString sMax)  {m_sMax = sMax;}
	virtual void    SetActiveChannel(VOLTAGE_CTRL_CHANNEL eActiveChannel)  {m_eActiveChannel = eActiveChannel;}

	virtual CString GetName() {return m_sName;}
	virtual CString GetMin()  {return m_sMin;}
	virtual CString GetMax()  {return m_sMax;}
	virtual CString GetExtRef()  {return m_sExtRef;}
	virtual VOLTAGE_CTRL_CHANNEL  GetActiveChannel()  {return m_eActiveChannel;}
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedSummitChannelARadio();
	afx_msg void OnBnClickedSummitChannelBRadio();
};
