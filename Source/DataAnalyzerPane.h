#pragma once
#include "afxwin.h"
#include "hexedit.h"

#define    _1K     0x400
#define    _1M     0x100000
#define    _1G     0x40000000


// CDataAnalyzerPane form view

class CDataAnalyzerPane : public CFormView
{
	DECLARE_DYNCREATE(CDataAnalyzerPane)

	typedef enum _mvtBase { MVT_BASE_DEC, MVT_BASE_HEX} MVT_BASE;

	UINT		m_nData;
	int			m_nShift;
	CString	    m_sData;
	MVT_BASE    m_base;
	BOOL	    m_bAnalyzeOnSelection;

	BOOL	    ValidateData(CString sData);
	BOOL		IsHex(CString sData);
	void	    SetShift(UINT nShift){m_nShift = nShift;}
	void	    SetShift(CString sShift){sscanf(sShift,"%d", &m_nShift);}
	int 	    GetShift() {return m_nShift;}
	void	    SetData(UINT nData);
	void	    SetData(CString sData);
	UINT	    GetData() {return m_nData;}
	void		UpdatePaneBit(UINT nId, int nData);
	void		SetBit(int nShift,int nBit);
	int  		GetBit(int nShift);
	void		UpdatePaneData();
	void	    UpdatePaneBits(BOOL bRemoveFocus = TRUE);
	void		UpdateUnitValue();


protected:
	CDataAnalyzerPane();           // protected constructor used by dynamic creation
	virtual ~CDataAnalyzerPane();

public:
	enum { IDD = IDD_DATA_ANALYZER_PANE };
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	void	    Analyze(CString sData);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedClearBtn();
	afx_msg void OnBnClickedSwapBtn();
	afx_msg void OnBnClickedNotBtn();
	afx_msg void OnBnClickedShiftRightBtn();
	afx_msg void OnBnClickedShiftLeftBtn();
	afx_msg void OnEnSetfocusBit0();
	afx_msg void OnEnSetfocusBit1();
	afx_msg void OnEnSetfocusBit2();
	afx_msg void OnEnSetfocusBit3();
	afx_msg void OnEnSetfocusBit4();
	afx_msg void OnEnSetfocusBit5();
	afx_msg void OnEnSetfocusBit6();
	afx_msg void OnEnSetfocusBit7();
	afx_msg void OnEnSetfocusBit8();
	afx_msg void OnEnSetfocusBit9();
	afx_msg void OnEnSetfocusBit10();
	afx_msg void OnEnSetfocusBit11();
	afx_msg void OnEnSetfocusBit12();
	afx_msg void OnEnSetfocusBit13();
	afx_msg void OnEnSetfocusBit14();
	afx_msg void OnEnSetfocusBit15();
	afx_msg void OnEnSetfocusBit17();
	afx_msg void OnEnSetfocusBit18();
	afx_msg void OnEnSetfocusBit19();
	afx_msg void OnEnSetfocusBit20();
	afx_msg void OnEnSetfocusBit21();
	afx_msg void OnEnSetfocusBit22();
	afx_msg void OnEnSetfocusBit23();
	afx_msg void OnEnSetfocusBit24();
	afx_msg void OnEnSetfocusBit25();
	afx_msg void OnEnSetfocusBit26();
	afx_msg void OnEnSetfocusBit27();
	afx_msg void OnEnSetfocusBit28();
	afx_msg void OnEnSetfocusBit29();
	afx_msg void OnEnSetfocusBit30();
	afx_msg void OnEnSetfocusBit31();
private:

	CXTPButton     m_swapBtn;
	CXTPButton     m_notBtn;
	CXTPButton     m_clearBtn;	
	CXTPButton     m_shiftLeftBtn;	
	CXTPButton     m_shiftRightBtn;	

public:
	CHexEdit     m_dataEdit;
	CEdit     	 m_bitEditCtrl[32];

	afx_msg void OnBnClickedHexRadio();
	afx_msg void OnBnClickedDecRadio();
	afx_msg void OnEnSetfocusBit16();
	virtual void OnInitialUpdate();
	afx_msg void OnEnChangeDataEdit();
	afx_msg void OnEnChangeShiftEdit();
	afx_msg void OnBnClickedAnalyzeOnSelectionCheck();
};


