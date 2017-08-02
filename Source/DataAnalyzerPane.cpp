// DataAnalyzerPane.cpp : implementation file
//

#include "stdafx.h"
#include "MVTerminal.h"
#include "DataAnalyzerPane.h"
#include ".\dataanalyzerpane.h"


// CDataAnalyzerPane

IMPLEMENT_DYNCREATE(CDataAnalyzerPane, CFormView)

CDataAnalyzerPane::CDataAnalyzerPane()
	: CFormView(CDataAnalyzerPane::IDD)
{
	SetData(0);
	m_base = MVT_BASE_HEX; 
	m_bAnalyzeOnSelection = TRUE;
}

CDataAnalyzerPane::~CDataAnalyzerPane()
{
}

void CDataAnalyzerPane::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DATA_EDIT, m_dataEdit);

	DDX_Control(pDX, IDC_SWAP_BTN, m_swapBtn);
	DDX_Control(pDX, IDC_NOT_BTN, m_notBtn);
	DDX_Control(pDX, IDC_CLEAR_BTN, m_clearBtn);
	DDX_Control(pDX, IDC_SHIFT_LEFT_BTN, m_shiftLeftBtn);
	DDX_Control(pDX, IDC_SHIFT_RIGHT_BTN, m_shiftRightBtn);

	DDX_Control(pDX, IDC_BIT0,  m_bitEditCtrl[0]);
	DDX_Control(pDX, IDC_BIT1,  m_bitEditCtrl[1]);
	DDX_Control(pDX, IDC_BIT2,  m_bitEditCtrl[2]);
	DDX_Control(pDX, IDC_BIT3,  m_bitEditCtrl[3]);
	DDX_Control(pDX, IDC_BIT4,  m_bitEditCtrl[4]);
	DDX_Control(pDX, IDC_BIT5,  m_bitEditCtrl[5]);
	DDX_Control(pDX, IDC_BIT6,  m_bitEditCtrl[6]);
	DDX_Control(pDX, IDC_BIT7,  m_bitEditCtrl[7]);
	DDX_Control(pDX, IDC_BIT8,  m_bitEditCtrl[8]);
	DDX_Control(pDX, IDC_BIT9,  m_bitEditCtrl[9]);
	DDX_Control(pDX, IDC_BIT10, m_bitEditCtrl[10]);
	DDX_Control(pDX, IDC_BIT11, m_bitEditCtrl[11]);
	DDX_Control(pDX, IDC_BIT12, m_bitEditCtrl[12]);
	DDX_Control(pDX, IDC_BIT13, m_bitEditCtrl[13]);
	DDX_Control(pDX, IDC_BIT14, m_bitEditCtrl[14]);
	DDX_Control(pDX, IDC_BIT15, m_bitEditCtrl[15]);
	DDX_Control(pDX, IDC_BIT16, m_bitEditCtrl[16]);
	DDX_Control(pDX, IDC_BIT17, m_bitEditCtrl[17]);
	DDX_Control(pDX, IDC_BIT18, m_bitEditCtrl[18]);
	DDX_Control(pDX, IDC_BIT19, m_bitEditCtrl[19]);
	DDX_Control(pDX, IDC_BIT20, m_bitEditCtrl[20]);
	DDX_Control(pDX, IDC_BIT21, m_bitEditCtrl[21]);
	DDX_Control(pDX, IDC_BIT22, m_bitEditCtrl[22]);
	DDX_Control(pDX, IDC_BIT23, m_bitEditCtrl[23]);
	DDX_Control(pDX, IDC_BIT24, m_bitEditCtrl[24]);
	DDX_Control(pDX, IDC_BIT25, m_bitEditCtrl[25]);
	DDX_Control(pDX, IDC_BIT26, m_bitEditCtrl[26]);
	DDX_Control(pDX, IDC_BIT27, m_bitEditCtrl[27]);
	DDX_Control(pDX, IDC_BIT28, m_bitEditCtrl[28]);
	DDX_Control(pDX, IDC_BIT29, m_bitEditCtrl[29]);
	DDX_Control(pDX, IDC_BIT30, m_bitEditCtrl[30]);
	DDX_Control(pDX, IDC_BIT31, m_bitEditCtrl[31]);
	
}

BEGIN_MESSAGE_MAP(CDataAnalyzerPane, CFormView)
	ON_BN_CLICKED(IDC_CLEAR_BTN, OnBnClickedClearBtn)
	ON_BN_CLICKED(IDC_SWAP_BTN, OnBnClickedSwapBtn)
	ON_BN_CLICKED(IDC_NOT_BTN, OnBnClickedNotBtn)
	ON_BN_CLICKED(IDC_SHIFT_RIGHT_BTN, OnBnClickedShiftRightBtn)
	ON_BN_CLICKED(IDC_SHIFT_LEFT_BTN, OnBnClickedShiftLeftBtn)
	ON_EN_SETFOCUS(IDC_BIT0, OnEnSetfocusBit0)
	ON_EN_SETFOCUS(IDC_BIT1, OnEnSetfocusBit1)
	ON_EN_SETFOCUS(IDC_BIT2, OnEnSetfocusBit2)
	ON_EN_SETFOCUS(IDC_BIT3, OnEnSetfocusBit3)
	ON_EN_SETFOCUS(IDC_BIT4, OnEnSetfocusBit4)
	ON_EN_SETFOCUS(IDC_BIT5, OnEnSetfocusBit5)
	ON_EN_SETFOCUS(IDC_BIT6, OnEnSetfocusBit6)
	ON_EN_SETFOCUS(IDC_BIT7, OnEnSetfocusBit7)
	ON_EN_SETFOCUS(IDC_BIT8, OnEnSetfocusBit8)
	ON_EN_SETFOCUS(IDC_BIT9, OnEnSetfocusBit9)
	ON_EN_SETFOCUS(IDC_BIT10, OnEnSetfocusBit10)
	ON_EN_SETFOCUS(IDC_BIT11, OnEnSetfocusBit11)
	ON_EN_SETFOCUS(IDC_BIT12, OnEnSetfocusBit12)
	ON_EN_SETFOCUS(IDC_BIT13, OnEnSetfocusBit13)
	ON_EN_SETFOCUS(IDC_BIT14, OnEnSetfocusBit14)
	ON_EN_SETFOCUS(IDC_BIT15, OnEnSetfocusBit15)
	ON_EN_SETFOCUS(IDC_BIT17, OnEnSetfocusBit17)
	ON_EN_SETFOCUS(IDC_BIT18, OnEnSetfocusBit18)
	ON_EN_SETFOCUS(IDC_BIT19, OnEnSetfocusBit19)
	ON_EN_SETFOCUS(IDC_BIT20, OnEnSetfocusBit20)
	ON_EN_SETFOCUS(IDC_BIT21, OnEnSetfocusBit21)
	ON_EN_SETFOCUS(IDC_BIT22, OnEnSetfocusBit22)
	ON_EN_SETFOCUS(IDC_BIT23, OnEnSetfocusBit23)
	ON_EN_SETFOCUS(IDC_BIT24, OnEnSetfocusBit24)
	ON_EN_SETFOCUS(IDC_BIT25, OnEnSetfocusBit25)
	ON_EN_SETFOCUS(IDC_BIT26, OnEnSetfocusBit26)
	ON_EN_SETFOCUS(IDC_BIT27, OnEnSetfocusBit27)
	ON_EN_SETFOCUS(IDC_BIT28, OnEnSetfocusBit28)
	ON_EN_SETFOCUS(IDC_BIT29, OnEnSetfocusBit29)
	ON_EN_SETFOCUS(IDC_BIT30, OnEnSetfocusBit30)
	ON_EN_SETFOCUS(IDC_BIT31, OnEnSetfocusBit31)
	ON_BN_CLICKED(IDC_HEX_RADIO, OnBnClickedHexRadio)
	ON_BN_CLICKED(IDC_DEC_RADIO, OnBnClickedDecRadio)
	ON_EN_SETFOCUS(IDC_BIT16, OnEnSetfocusBit16)
	ON_EN_CHANGE(IDC_DATA_EDIT, OnEnChangeDataEdit)
	ON_EN_CHANGE(IDC_SHIFT_EDIT, OnEnChangeShiftEdit)
	ON_BN_CLICKED(IDC_ANALYZE_ON_SELECTION_CHECK, OnBnClickedAnalyzeOnSelectionCheck)
END_MESSAGE_MAP()


// CDataAnalyzerPane diagnostics

#ifdef _DEBUG
void CDataAnalyzerPane::AssertValid() const
{
	CFormView::AssertValid();
}

void CDataAnalyzerPane::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG


// CDataAnalyzerPane message handlers

void CDataAnalyzerPane::OnBnClickedClearBtn()
{
	SetData(0);
	UpdatePaneBits();
	UpdatePaneData();
}

void CDataAnalyzerPane::OnBnClickedSwapBtn()
{
	SetData(((((m_nData)&0xff)<<24)   |      
			 (((m_nData)&0xff00)<<8)   |     
			 (((m_nData)&0xff0000)>>8) |   
			 (((m_nData)&0xff000000)>>24))) ;

	UpdatePaneBits();
	UpdatePaneData();
}

void CDataAnalyzerPane::OnBnClickedNotBtn()
{
	SetData(~GetData()) ;

	UpdatePaneBits();
	UpdatePaneData();
}

void CDataAnalyzerPane::OnBnClickedShiftRightBtn()
{
	SetData(GetData() >> GetShift());
	UpdatePaneBits();
	UpdatePaneData();
}

void CDataAnalyzerPane::OnBnClickedShiftLeftBtn()
{
	SetData(GetData() << GetShift());
	UpdatePaneBits();
	UpdatePaneData();
}
void CDataAnalyzerPane::SetData(UINT nValue)
{	
	char	    *pFmt = NULL;

	if(m_base == MVT_BASE_HEX)
		pFmt = "%x";
	if(m_base == MVT_BASE_DEC)
		pFmt = "%ud";

	m_nData = nValue;
	m_sData.Format(pFmt,m_nData);
}
void CDataAnalyzerPane::SetData(CString sValue)
{
	char	    *pFmt = NULL;

	if(m_base == MVT_BASE_HEX)
		pFmt = "%x";
	if(m_base == MVT_BASE_DEC)
		pFmt = "%ud";

	m_sData = sValue;
	sscanf(m_sData,pFmt,&m_nData);
}

BOOL CDataAnalyzerPane::IsHex(CString sData) 
{
	int		 nLen, nCh;

	sData.MakeLower();
	nLen = sData.GetLength();

	for(nCh = 0 ; nCh < nLen; nCh++)
	{
		switch(sData.GetAt(nCh))
		{
			case 'a':
			case 'b':
			case 'c':
			case 'd':
			case 'e':
			case 'f':
				return TRUE;
			default:
				break;
		}
	}
	return FALSE;
}


BOOL CDataAnalyzerPane::ValidateData(CString sData) 
{
	int		 nLen, nCh;

	sData.MakeLower();
	nLen = sData.GetLength();

	if((m_base == MVT_BASE_HEX) && (nLen > 8))
		return FALSE;

	if((m_base == MVT_BASE_DEC) && (nLen > 10))
		return FALSE;

	for(nCh = 0 ; nCh < nLen; nCh++)
	{
		switch(sData.GetAt(nCh))
		{
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7': 
			case '8':
			case '9':
				break;
			case 'a':
			case 'b':
			case 'c':
			case 'd':
			case 'e':
			case 'f':
				if(m_base == MVT_BASE_DEC)
					return FALSE;
				break;
			default:
				return FALSE;
		}
	}
	return TRUE;
}


void CDataAnalyzerPane::OnEnSetfocusBit0()
{
	int      nShift = 0;

	SetBit(nShift,!GetBit(nShift));
	UpdatePaneBits();
	UpdatePaneData();
}

void CDataAnalyzerPane::OnEnSetfocusBit1()
{
	int      nShift = 1;

	SetBit(nShift,!GetBit(nShift));
	UpdatePaneBits();
	UpdatePaneData();
}

void CDataAnalyzerPane::OnEnSetfocusBit2()
{
	int      nShift = 2;

	SetBit(nShift,!GetBit(nShift));
	UpdatePaneBits();
	UpdatePaneData();
}

void CDataAnalyzerPane::OnEnSetfocusBit3()
{
	int      nShift = 3;

	SetBit(nShift,!GetBit(nShift));
	UpdatePaneBits();
	UpdatePaneData();
}

void CDataAnalyzerPane::OnEnSetfocusBit4()
{
	int      nShift = 4;

	SetBit(nShift,!GetBit(nShift));
	UpdatePaneBits();
	UpdatePaneData();
}

void CDataAnalyzerPane::OnEnSetfocusBit5()
{
	int      nShift = 5;

	SetBit(nShift,!GetBit(nShift));
	UpdatePaneBits();
	UpdatePaneData();
}

void CDataAnalyzerPane::OnEnSetfocusBit6()
{
	int       nShift = 6;

	SetBit(nShift,!GetBit(nShift));
	UpdatePaneBits();
	UpdatePaneData();
}

void CDataAnalyzerPane::OnEnSetfocusBit7()
{
	int      nShift = 7;

	SetBit(nShift,!GetBit(nShift));
	UpdatePaneBits();
	UpdatePaneData();
}

void CDataAnalyzerPane::OnEnSetfocusBit8()
{
	int      nShift = 8;

	SetBit(nShift,!GetBit(nShift));
	UpdatePaneBits();
	UpdatePaneData();
}

void CDataAnalyzerPane::OnEnSetfocusBit9()
{
	int      nShift = 9;

	SetBit(nShift,!GetBit(nShift));
	UpdatePaneBits();
	UpdatePaneData();
}

void CDataAnalyzerPane::OnEnSetfocusBit10()
{
	int      nShift = 10;

	SetBit(nShift,!GetBit(nShift));
	UpdatePaneBits();
	UpdatePaneData();
}

void CDataAnalyzerPane::OnEnSetfocusBit11()
{
	int      nShift = 11;

	SetBit(nShift,!GetBit(nShift));
	UpdatePaneBits();
	UpdatePaneData();
}

void CDataAnalyzerPane::OnEnSetfocusBit12()
{
	int       nShift = 12;

	SetBit(nShift,!GetBit(nShift));
	UpdatePaneBits();
	UpdatePaneData();
}

void CDataAnalyzerPane::OnEnSetfocusBit13()
{
	int      nShift = 13;

	SetBit(nShift,!GetBit(nShift));
	UpdatePaneBits();
	UpdatePaneData();
}

void CDataAnalyzerPane::OnEnSetfocusBit14()
{
	int      nShift = 14;

	SetBit(nShift,!GetBit(nShift));
	UpdatePaneBits();
	UpdatePaneData();
}

void CDataAnalyzerPane::OnEnSetfocusBit15()
{
	int      nShift = 15;

	SetBit(nShift,!GetBit(nShift));
	UpdatePaneBits();
	UpdatePaneData();
}

void CDataAnalyzerPane::OnEnSetfocusBit16()
{
	int       nShift = 16;

	SetBit(nShift,!GetBit(nShift));
	UpdatePaneBits();
	UpdatePaneData();
}

void CDataAnalyzerPane::OnEnSetfocusBit17()
{
	int       nShift = 17;

	SetBit(nShift,!GetBit(nShift));
	UpdatePaneBits();
	UpdatePaneData();
}

void CDataAnalyzerPane::OnEnSetfocusBit18()
{
	int      nShift = 18;

	SetBit(nShift,!GetBit(nShift));
	UpdatePaneBits();
	UpdatePaneData();
}

void CDataAnalyzerPane::OnEnSetfocusBit19()
{
	int      nShift = 19;

	SetBit(nShift,!GetBit(nShift));
	UpdatePaneBits();
	UpdatePaneData();
}

void CDataAnalyzerPane::OnEnSetfocusBit20()
{
	int      nShift = 20;

	SetBit(nShift,!GetBit(nShift));
	UpdatePaneBits();
	UpdatePaneData();
}

void CDataAnalyzerPane::OnEnSetfocusBit21()
{
	int      nShift = 21;

	SetBit(nShift,!GetBit(nShift));
	UpdatePaneBits();
	UpdatePaneData();
}

void CDataAnalyzerPane::OnEnSetfocusBit22()
{
	int       nShift = 22;

	SetBit(nShift,!GetBit(nShift));
	UpdatePaneBits();
	UpdatePaneData();
}

void CDataAnalyzerPane::OnEnSetfocusBit23()
{
	int      nShift = 23;

	SetBit(nShift,!GetBit(nShift));
	UpdatePaneBits();
	UpdatePaneData();
}

void CDataAnalyzerPane::OnEnSetfocusBit24()
{
	int       nShift = 24;

	SetBit(nShift,!GetBit(nShift));
	UpdatePaneBits();
	UpdatePaneData();
}

void CDataAnalyzerPane::OnEnSetfocusBit25()
{
	int       nShift = 25;

	SetBit(nShift,!GetBit(nShift));
	UpdatePaneBits();
	UpdatePaneData();
}

void CDataAnalyzerPane::OnEnSetfocusBit26()
{
	int      nShift = 26;

	SetBit(nShift,!GetBit(nShift));
	UpdatePaneBits();
	UpdatePaneData();
}

void CDataAnalyzerPane::OnEnSetfocusBit27()
{
	int       nShift = 27;

	SetBit(nShift,!GetBit(nShift));
	UpdatePaneBits();
	UpdatePaneData();
}

void CDataAnalyzerPane::OnEnSetfocusBit28()
{
	int       nShift = 28;

	SetBit(nShift,!GetBit(nShift));
	UpdatePaneBits();
	UpdatePaneData();
}

void CDataAnalyzerPane::OnEnSetfocusBit29()
{
	int       nShift = 29;

	SetBit(nShift,!GetBit(nShift));
	UpdatePaneBits();
	UpdatePaneData();
}

void CDataAnalyzerPane::OnEnSetfocusBit30()
{
	int       nShift = 30;

	SetBit(nShift,!GetBit(nShift));
	UpdatePaneBits();
	UpdatePaneData();
}

void CDataAnalyzerPane::OnEnSetfocusBit31()
{
	int    nShift = 31;

	SetBit(nShift,!GetBit(nShift));
	UpdatePaneBits();
	UpdatePaneData();
}

void CDataAnalyzerPane::SetBit(int nShift,int nBit)
{
	m_nData = m_nData & ~((UINT)(((UINT)1)<<nShift));
	m_nData |= (((UINT)nBit)<<nShift);

}
int CDataAnalyzerPane::GetBit(int nShift)
{
	return ((m_nData>>nShift)&0x1);
}

void CDataAnalyzerPane::OnBnClickedHexRadio()
{
	CheckDlgButton(IDC_HEX_RADIO,TRUE);
	CheckDlgButton(IDC_DEC_RADIO,FALSE);
	m_base = MVT_BASE_HEX;
	m_dataEdit.SetLimitText(8);
	UpdatePaneBits();
	UpdatePaneData();
}

void CDataAnalyzerPane::OnBnClickedDecRadio()
{
	CheckDlgButton(IDC_DEC_RADIO,TRUE);
	CheckDlgButton(IDC_HEX_RADIO,FALSE);
	m_base = MVT_BASE_DEC;
	m_dataEdit.SetLimitText(10);
	UpdatePaneBits();
	UpdatePaneData();
}




void CDataAnalyzerPane::UpdatePaneBit(UINT nId, int nData)
{
	if(nData == 0)
	{
		((CEdit*)GetDlgItem(nId))->SetReadOnly(FALSE);
		SetDlgItemText(nId,"0");
	}
	else
	{		
		((CEdit*)GetDlgItem(nId))->SetReadOnly(TRUE);
		SetDlgItemText(nId,"1");
	}

}
void CDataAnalyzerPane::UpdatePaneData()
{
	char	    *pFmt = NULL;

	if(m_base == MVT_BASE_HEX)
		pFmt = "%x";
	if(m_base == MVT_BASE_DEC)
		pFmt = "%ud";

	m_sData.Format(pFmt,m_nData);
	if(m_base == MVT_BASE_DEC)
		m_sData.Remove('d');
	SetDlgItemText(IDC_DATA_EDIT,m_sData);

	UpdateUnitValue();
}

void CDataAnalyzerPane::UpdateUnitValue()
{
	CString      sUnits,sValue;
	double        fValue;
	// Show M/K/G value
	if(m_nData < _1M) 
	{
		sUnits = "K";		
		fValue = (float)((float)m_nData/(float)_1K);
	}
	if((m_nData >= _1M) && (m_nData < _1G))
	{
		sUnits = "M";
		fValue = (float)((float)m_nData/(float)_1M);
	}
	if(m_nData >= _1G)
	{
		sUnits = "G";
		fValue = (float)((float)m_nData/(float)_1G);
	}	
	sValue.Format("%0.4f",fValue);
	SetDlgItemText(IDC_MB_VALUE,"= " + sValue + sUnits);
}

void CDataAnalyzerPane::UpdatePaneBits(BOOL bRemoveFocus)
{
	int			 nBit, nMaxBit = 32;

	for(nBit = 0; nBit < nMaxBit; nBit++)
	{
		UpdatePaneBit(m_bitEditCtrl[nBit].GetDlgCtrlID(),((m_nData >> nBit) & 0x1));
	}

	if(bRemoveFocus == TRUE)
		GetDlgItem(IDC_MSB_LSB_STATIC)->SetFocus();

}

void CDataAnalyzerPane::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	m_dataEdit.SetFocus();	

	// Start with Hex
	OnBnClickedHexRadio();

	CheckDlgButton(IDC_ANALYZE_ON_SELECTION_CHECK, m_bAnalyzeOnSelection);

	// Limit shift value
	((CEdit*)GetDlgItem(IDC_SHIFT_EDIT))->SetLimitText(2);

	// Set shift value
	SetShift(1);
	((CEdit*)GetDlgItem(IDC_SHIFT_EDIT))->SetWindowText("1");


	m_swapBtn.SetTheme(xtpButtonThemeOffice2000);
	m_notBtn.SetTheme(xtpButtonThemeOffice2000);
	m_clearBtn.SetTheme(xtpButtonThemeOffice2000);
	m_shiftLeftBtn.SetTheme(xtpButtonThemeOffice2000);
	m_shiftRightBtn.SetTheme(xtpButtonThemeOffice2000);
}

void CDataAnalyzerPane::OnEnChangeDataEdit()
{
	CString		sData;

	GetDlgItemText(IDC_DATA_EDIT,sData);
	if(sData.IsEmpty())
	{
		SetData(0);
		UpdatePaneBits();
		return;
	}

	if(ValidateData(sData) == FALSE)
	{		
		SetData(0);
		UpdatePaneBits();
		return;
	}

	SetData(sData);
	UpdatePaneBits(FALSE);
	UpdateUnitValue();
}

void CDataAnalyzerPane::OnEnChangeShiftEdit()
{
	CString sShift;

	GetDlgItemText(IDC_SHIFT_EDIT,sShift);
	if(sShift.IsEmpty() == TRUE)
	{
		SetShift(0);
		return;
	}
	SetShift(sShift);
}

void  CDataAnalyzerPane::Analyze(CString sData)
{
	if(sData.IsEmpty())
		return;

	// Remove spaces
	sData.Remove(' ');

	// Handle 0x prefix
	if((sData.Left(2) == "0x") || (sData.Left(2) == "0X"))
	{
		sData = sData.Right(sData.GetLength() - 2);
		OnBnClickedHexRadio();
	}

	if((ValidateData(sData) == TRUE) && (m_bAnalyzeOnSelection == TRUE))
	{
		SetData(sData);
		UpdatePaneBits(FALSE);
		UpdatePaneData();
	}
}


void CDataAnalyzerPane::OnBnClickedAnalyzeOnSelectionCheck()
{
	m_bAnalyzeOnSelection = !m_bAnalyzeOnSelection;
}

