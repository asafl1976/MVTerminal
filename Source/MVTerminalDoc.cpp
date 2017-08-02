// MVTerminalDoc.cpp : implementation of the CMVTerminalDoc class
//

#include "stdafx.h"
#include "MVTerminal.h"

#include "MVTerminalDoc.h"
#include "CntrItem.h"
#include ".\mvterminaldoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMVTerminalDoc

IMPLEMENT_DYNCREATE(CMVTerminalDoc, CRichEditDoc)

BEGIN_MESSAGE_MAP(CMVTerminalDoc, CRichEditDoc)
	// Enable default OLE container implementation
	ON_UPDATE_COMMAND_UI(ID_OLE_EDIT_LINKS, CRichEditDoc::OnUpdateEditLinksMenu)
	ON_COMMAND(ID_OLE_EDIT_LINKS, CRichEditDoc::OnEditLinks)
	ON_UPDATE_COMMAND_UI_RANGE(ID_OLE_VERB_FIRST, ID_OLE_VERB_LAST, CRichEditDoc::OnUpdateObjectVerbMenu)
	ON_COMMAND(ID_FILE_CLOSE, OnFileClose)
END_MESSAGE_MAP()


// CMVTerminalDoc construction/destruction

CMVTerminalDoc::CMVTerminalDoc()
{

}

CMVTerminalDoc::~CMVTerminalDoc()
{
}

BOOL CMVTerminalDoc::OnNewDocument()
{
	if (!CRichEditDoc::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}
CRichEditCntrItem* CMVTerminalDoc::CreateClientItem(REOBJECT* preo) const
{
	return new CMVTerminalCntrItem(preo, const_cast<CMVTerminalDoc*>(this));
}




// CMVTerminalDoc serialization

void CMVTerminalDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}

	// Calling the base class CRichEditDoc enables serialization
	//  of the container document's COleClientItem objects.
	// TODO: set CRichEditDoc::m_bRTF = FALSE if you are serializing as text
	CRichEditDoc::Serialize(ar);
}


// CMVTerminalDoc diagnostics

#ifdef _DEBUG
void CMVTerminalDoc::AssertValid() const
{
	CRichEditDoc::AssertValid();
}

void CMVTerminalDoc::Dump(CDumpContext& dc) const
{
	CRichEditDoc::Dump(dc);
}
#endif //_DEBUG


// CMVTerminalDoc commands


void CMVTerminalDoc::OnFileClose()
{
	OnCloseDocument();
}

BOOL CMVTerminalDoc::SaveModified()
{
	// Disable the "Save changes to COM .." MessageBox
	return TRUE;//CRichEditDoc::SaveModified();
}
