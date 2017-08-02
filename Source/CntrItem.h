// CntrItem.h : interface of the CMVTerminalCntrItem class
//

#pragma once

class CMVTerminalDoc;
class CMVTerminalView;

class CMVTerminalCntrItem : public CRichEditCntrItem
{
	DECLARE_SERIAL(CMVTerminalCntrItem)

// Constructors
public:
	CMVTerminalCntrItem(REOBJECT* preo = NULL, CMVTerminalDoc* pContainer = NULL);
		// Note: pContainer is allowed to be NULL to enable IMPLEMENT_SERIALIZE
		//  IMPLEMENT_SERIALIZE requires the class have a constructor with
		//  zero arguments.  Normally, OLE items are constructed with a
		//  non-NULL document pointer

// Attributes
public:
	CMVTerminalDoc* GetDocument()
		{ return reinterpret_cast<CMVTerminalDoc*>(CRichEditCntrItem::GetDocument()); }
	CMVTerminalView* GetActiveView()
		{ return reinterpret_cast<CMVTerminalView*>(CRichEditCntrItem::GetActiveView()); }

	public:
	protected:

// Implementation
public:
	~CMVTerminalCntrItem();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

