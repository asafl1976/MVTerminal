// HexEdit.cpp : implementation file
//

#include "stdafx.h"
#include "MVTerminal.h"
#include "HexEdit.h"
#include ".\hexedit.h"

#define  VK_CTRL_C		TCHAR(3)
#define  VK_CTRL_V		TCHAR(22)


// CHexEdit

IMPLEMENT_DYNAMIC(CHexEdit, CEdit)
CHexEdit::CHexEdit()
{
	SetLimitText(8);
}

CHexEdit::~CHexEdit()
{
}


BEGIN_MESSAGE_MAP(CHexEdit, CEdit)
	ON_WM_CHAR()
//	ON_WM_CREATE()
END_MESSAGE_MAP()

void CHexEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CString sCH((TCHAR)nChar),sWinText;

	sCH.MakeLower();
	switch(sCH.GetAt(0))
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
		case 'a':
		case 'b':
		case 'c':
		case 'd':
		case 'e':
		case 'f':
		case VK_CTRL_C:
		case VK_CTRL_V:
		case VK_DELETE:
		case VK_BACK:
			break;
		default:
			return;
	}

	CEdit::OnChar(nChar, nRepCnt, nFlags);
}

BOOL CHexEdit::GetValue(CString& sValue)
{
	GetWindowText(sValue);
	if(sValue.IsEmpty() == TRUE)
		return FALSE;
	return TRUE; 
}

void    CHexEdit::SetValue(CString sValue)
{
	SetWindowText(sValue);
}


BOOL    CHexEdit::GetValue(UINT & nValue)
{
	CString sWinText;

	if(GetValue(sWinText) == TRUE)
	{
		sscanf(sWinText,"%x",&nValue);
		return TRUE;
	}
	return FALSE;
}


