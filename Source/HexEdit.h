#pragma once


// CHexEdit

class CHexEdit : public CEdit
{
	DECLARE_DYNAMIC(CHexEdit)

public:
	CHexEdit();
	virtual ~CHexEdit();
	void    SetValue(CString sValue);
	BOOL    GetValue(UINT & nValue);
	BOOL    GetValue(CString& sValue);


protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);

};


