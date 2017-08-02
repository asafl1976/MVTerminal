#pragma once
#include "afxwin.h"
#include "IHwModuleItemDlg.h"
#include "HwModuleCtrlI2CUnit.h"


// CHwModuleCtrlUnitI2CSetFreqDlg dialog

class CHwModuleCtrlUnitI2CSetFreqDlg : public CDialog 
{
	DECLARE_DYNAMIC(CHwModuleCtrlUnitI2CSetFreqDlg)
	int                   m_nFreq;
	CHwModuleCtrlI2CUnit* m_pI2CUnit;

public:
	CHwModuleCtrlUnitI2CSetFreqDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CHwModuleCtrlUnitI2CSetFreqDlg();

// Dialog Data
	enum { IDD = IDD_HW_MODULE_CTRL_UNIT_SET_I2C_FREQ_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	int  GetFreq() {return m_nFreq;}
	void SetFreq(int nFreq) {m_nFreq = nFreq;}

	virtual void SetHwModuleCtrlI2CUnit(CHwModuleCtrlI2CUnit *pItem);

	CXTPButton m_okBtn;
	virtual BOOL OnInitDialog();
};
