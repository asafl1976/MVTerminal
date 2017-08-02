// HListBox.cpp : implementation file
//

#include "stdafx.h"
#include "HListBox.h"
#include ".\hlistbox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHListBox

CHListBox::CHListBox()
{
 width = 0;
}

CHListBox::~CHListBox()
{
}


BEGIN_MESSAGE_MAP(CHListBox, CListBox)
	//{{AFX_MSG_MAP(CHListBox)
	//}}AFX_MSG_MAP
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipText)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipText)	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHListBox message handlers
void CHListBox::updateWidth(LPCTSTR s)
    {
     CClientDC dc(this);
     CFont * f = CListBox::GetFont();
     dc.SelectObject(f);
     CSize sz = dc.GetTextExtent(s, (int)_tcslen(s));
     sz.cx += 3 * ::GetSystemMetrics(SM_CXBORDER);
     if(sz.cx > width)
	 { /* extend */
	  width = sz.cx;
	  CListBox::SetHorizontalExtent(width);
	 } /* extend */
    }

int CHListBox::AddString(LPCTSTR s)
    {
     int result = CListBox::AddString(s);
     if(result < 0)
	 return result;
     updateWidth(s);
     return result;
    }

int CHListBox::InsertString(int i, LPCTSTR s)
    {
     int result = CListBox::InsertString(i, s);
     if(result < 0)
	 return result;
     updateWidth(s);
     return result;
    }

void CHListBox::ResetContent()
    {
     CListBox::ResetContent();
     width = 0;
    }

int CHListBox::DeleteString(int n)
    {
     int result = CListBox::DeleteString(n);
     if(result < 0)
	 return result;
     CClientDC dc(this);

     CFont * f = CListBox::GetFont();
     dc.SelectObject(f);

     width = 0;
     for(int i = 0; i < CListBox::GetCount(); i++)
	 { /* scan strings */
	  CString s;
	  CListBox::GetText(i, s);
	  CSize sz = dc.GetTextExtent(s);
          sz.cx += 3 * ::GetSystemMetrics(SM_CXBORDER);
	  if(sz.cx > width)
	      width = sz.cx;
	 } /* scan strings */
     CListBox::SetHorizontalExtent(width);
     return result;
    }

	void CHListBox::PreSubclassWindow()
	{
		CListBox::PreSubclassWindow();
		EnableToolTips(TRUE);
	}

	INT_PTR CHListBox::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
	{
		int row;

		RECT cellrect; // cellrect - to hold the bounding rect
		BOOL tmp = FALSE;
		row = ItemFromPoint(point,tmp); //we call the ItemFromPoint function to determine the row,
		//note that in NT this function may fail use the ItemFromPointNT member function
		if ( row == -1 ) 
			return -1;

		//set up the TOOLINFO structure. GetItemRect(row,&cellrect);
		GetItemRect(row,&cellrect);
		pTI->rect = cellrect;
		pTI->hwnd = m_hWnd;
		pTI->uId = (UINT)((row)); //The ‘uId’ is assigned a value according to the row value.
		pTI->lpszText = LPSTR_TEXTCALLBACK;  //Send a TTN_NEEDTEXT messages 
		return pTI->uId;
		//return CListBox::OnToolHitTest(point, pTI);
	} 

	BOOL CHListBox::OnToolTipText( UINT id, NMHDR * pNMHDR, LRESULT * pResult )
	{
		// need to handle both ANSI and UNICODE versions of the message
			TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
			TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
			CString strTipText;
			UINT nID = (UINT)pNMHDR->idFrom;  //list box item index 
			GetText( nID ,strTipText); //get item text 

		//display item text as tool tip 
		#ifndef _UNICODE
			if (pNMHDR->code == TTN_NEEDTEXTA)
			lstrcpyn(pTTTA->szText, strTipText, 128);
		else
			_mbstowcsz(pTTTW->szText, strTipText, 128);
		#else
			if (pNMHDR->code == TTN_NEEDTEXTA)
			_wcstombsz(pTTTA->szText, strTipText, 128);
			else
			lstrcpyn(pTTTW->szText, strTipText, 128);
			#endif
			*pResult = 0;

		return TRUE; 
	}
