// CntrItem.cpp : implementation of the CMVTerminalCntrItem class
//

#include "stdafx.h"
#include "MVTerminal.h"

#include "MVTerminalDoc.h"
#include "MVTerminalView.h"
#include "CntrItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMVTerminalCntrItem implementation

IMPLEMENT_SERIAL(CMVTerminalCntrItem, CRichEditCntrItem, 0)

CMVTerminalCntrItem::CMVTerminalCntrItem(REOBJECT* preo, CMVTerminalDoc* pContainer)
	: CRichEditCntrItem(preo, pContainer)
{
	// TODO: add one-time construction code here
}

CMVTerminalCntrItem::~CMVTerminalCntrItem()
{
	// TODO: add cleanup code here
}


// CMVTerminalCntrItem diagnostics

#ifdef _DEBUG
void CMVTerminalCntrItem::AssertValid() const
{
	CRichEditCntrItem::AssertValid();
}

void CMVTerminalCntrItem::Dump(CDumpContext& dc) const
{
	CRichEditCntrItem::Dump(dc);
}
#endif

