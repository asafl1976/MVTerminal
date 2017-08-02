#pragma once


// CCustomEditCtrl

class CCustomEditCtrl : public CEdit
{
	DECLARE_DYNAMIC(CCustomEditCtrl)

	COLORREF   m_color;
	int		   m_nBit;

public:
	CCustomEditCtrl();
	virtual ~CCustomEditCtrl();
	virtual void SetBkColor(COLORREF color) {m_color = color;}
	virtual void SetBitState(int nBit) {m_nBit = nBit;}

protected:
	HBRUSH CtlColor(CDC *pDC, UINT);
	DECLARE_MESSAGE_MAP()
};


