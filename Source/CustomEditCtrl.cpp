// CustomEditCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "MVTerminal.h"
#include "CustomEditCtrl.h"


// CCustomEditCtrl

IMPLEMENT_DYNAMIC(CCustomEditCtrl, CEdit)
CCustomEditCtrl::CCustomEditCtrl()
{
	m_color = RGB(255,255,255);
	m_nBit  = 0;
}

CCustomEditCtrl::~CCustomEditCtrl()
{
}


BEGIN_MESSAGE_MAP(CCustomEditCtrl, CEdit)
	ON_WM_CTLCOLOR_REFLECT()
END_MESSAGE_MAP()


HBRUSH CCustomEditCtrl::CtlColor(CDC *pDC, UINT)
{
	
	if(m_nBit == 0)
	{
		pDC->SetBkColor(RGB(255,255,255));
		return CreateSolidBrush(RGB(255,255,255));
	}

	pDC->SetBkColor(m_color);
	return CreateSolidBrush(m_color);
}
// CCustomEditCtrl message handlers

