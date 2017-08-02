#pragma once
#include "resource.h"
#include "afxwin.h"

// CODPGPatternSelectBitDlg dialog

class CODPGPatternSelectBitDlg : public CDialog
{
	DECLARE_DYNAMIC(CODPGPatternSelectBitDlg)

	int		  m_nMaxBit;
	int		  m_nSelBit;
	CXTPButton m_okBtn;
	CComboBox m_vicBitCombo;
public:
	CODPGPatternSelectBitDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CODPGPatternSelectBitDlg();

// Dialog Data
	enum { IDD = IDD_ODPG_SELECT_PATTERN_BIT_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	void    SetMaxBit(int nMaxBit) {m_nMaxBit = nMaxBit;}
	int     GetBit() {return m_nSelBit;}
	void    SetBit(int nBit) {m_nSelBit = nBit ;}
	afx_msg void    OnBnClickedOk();
	virtual BOOL	OnInitDialog();

	afx_msg void OnCbnSelchangePatternBitCombo();
};
