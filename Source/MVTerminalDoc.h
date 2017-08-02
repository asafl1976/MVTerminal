// MVTerminalDoc.h : interface of the CMVTerminalDoc class
//


#pragma once

#include "MainFrm.h"
#include "ChildFrm.h"

class CMVTerminalDoc : public CRichEditDoc
{
protected: // create from serialization only
	CMVTerminalDoc();
	DECLARE_DYNCREATE(CMVTerminalDoc)

// Attributes
public:

// Operations
public:

// Overrides
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual CRichEditCntrItem* CreateClientItem(REOBJECT* preo) const;

// Implementation
public:
	virtual ~CMVTerminalDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnFileClose();
	afx_msg void OnFileClose();
protected:
	virtual BOOL SaveModified();
};


