#include "StdAfx.h"
#include ".\sub20hwmodulectrladcunitvcitemmodelessdlg.h"


int CSUB20HwModuleCtrlADCUnitVCItemModelessDlg::m_nWinPos = 0;

//IMPLEMENT_DYNAMIC(CSUB20HwModuleCtrlADCUnitVCItemModelessDlg, CDialog)

CSUB20HwModuleCtrlADCUnitVCItemModelessDlg::CSUB20HwModuleCtrlADCUnitVCItemModelessDlg(CWnd* pParentWnd)
: CSUB20HwModuleCtrlADCUnitVCItemBasicDlg(CSUB20HwModuleCtrlADCUnitVCItemModelessDlg::IDD, pParentWnd)
{
}

CSUB20HwModuleCtrlADCUnitVCItemModelessDlg::~CSUB20HwModuleCtrlADCUnitVCItemModelessDlg(void)
{
}

void CSUB20HwModuleCtrlADCUnitVCItemModelessDlg::DoDataExchange(CDataExchange* pDX)
{
	CSUB20HwModuleCtrlADCUnitVCItemBasicDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SUB20_HWMODULE_ADC_CUR_VOL_OFFSET_SLIDER, m_offsetSlider);
}

BEGIN_MESSAGE_MAP(CSUB20HwModuleCtrlADCUnitVCItemModelessDlg, CDialog)
END_MESSAGE_MAP()


void  CSUB20HwModuleCtrlADCUnitVCItemModelessDlg::AlignWindow()
{
    CRect rectFrame, rectDlg;
    CWnd* pMainWnd = AfxGetMainWnd();
    if(pMainWnd != NULL)
    {
        pMainWnd->GetClientRect(rectFrame);
        pMainWnd->ClientToScreen(rectFrame);
        GetWindowRect(rectDlg);

        int nXPos = rectFrame.left + (rectFrame.Width() / 2) 
                                   - (rectDlg.Width() / 2);
        int nYPos = rectFrame.top + (rectFrame.Height() / 3) 
                                  - (rectDlg.Height() / 3);

		//Cascade windows
		nXPos += CSUB20HwModuleCtrlADCUnitVCItemModelessDlg::m_nWinPos;
        nYPos += CSUB20HwModuleCtrlADCUnitVCItemModelessDlg::m_nWinPos;
		CSUB20HwModuleCtrlADCUnitVCItemModelessDlg::m_nWinPos += 20;
		if(CSUB20HwModuleCtrlADCUnitVCItemModelessDlg::m_nWinPos == 220)
			CSUB20HwModuleCtrlADCUnitVCItemModelessDlg::m_nWinPos = 0;

	    ::SetWindowPos(m_hWnd, HWND_TOP, nXPos, nYPos, 
          rectDlg.Width(), rectDlg.Height(), SWP_NOCOPYBITS);
    }
}

void CSUB20HwModuleCtrlADCUnitVCItemModelessDlg::PostNcDestroy()
{
	CSUB20HwModuleCtrlADCUnitVCItemModelessDlg::m_nWinPos -= 20;
	if(CSUB20HwModuleCtrlADCUnitVCItemModelessDlg::m_nWinPos < 0)
		CSUB20HwModuleCtrlADCUnitVCItemModelessDlg::m_nWinPos = 0;
	
	delete this;
}

void CSUB20HwModuleCtrlADCUnitVCItemModelessDlg::OnCancel()
{
	DestroyWindow();
}

BOOL CSUB20HwModuleCtrlADCUnitVCItemModelessDlg::OnInitDialog()
{
	CSUB20HwModuleCtrlADCUnitVCItemBasicDlg::OnInitDialog();

	CSUB20HwModuleCtrlADCUnitVCItemModelessDlg::AlignWindow();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

UINT CSUB20HwModuleCtrlADCUnitVCItemModelessDlg::GetSampleSum(int* pValArray,int nSampleCount)
{
	UINT nSum = 0;

	for(int nSumCount = 0; nSumCount < nSampleCount; nSumCount++)
	{
		nSum += pValArray[nSumCount];
	}
	return nSum;
}


