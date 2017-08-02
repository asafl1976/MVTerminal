#include "StdAfx.h"
#include ".\basicitemdlg.h"

CBasicItemDlg::CBasicItemDlg(UINT nIDTemplate, CWnd* pParentWnd):
CDialog(nIDTemplate,pParentWnd)
{
}

CBasicItemDlg::~CBasicItemDlg(void)
{
}

void  CBasicItemDlg::AlignWindow()
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
        
	    ::SetWindowPos(m_hWnd, HWND_TOP, nXPos, nYPos, 
          rectDlg.Width(), rectDlg.Height(), SWP_NOCOPYBITS);
    }
}
