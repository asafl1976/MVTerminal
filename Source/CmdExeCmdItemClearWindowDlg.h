#pragma once
#include "afxwin.h"
#include "resource.h"
#include "CmdExeCmdItemClearWindow.h"
#include "BasicItemDlg.h"
#include "ICmdExeCmdItemDlg.h"

class CCmdExeCmdItemClearWindowDlg : public CBasicItemDlg, public ICmdExeCmdItemDlg 
{
	CComboBox				        m_targetWinCombo;
	CCmdExeCmdItemClearWindowParams m_params;
	CCmdExeCmdItemClearWindow      *m_pItem;

public:
	CCmdExeCmdItemClearWindowDlg(CWnd* pParentWnd = NULL);   // standard constructor
	virtual ~CCmdExeCmdItemClearWindowDlg();

// Dialog Data
	enum { IDD = IDD_CE_CLEAR_WIN_DLG };

protected:
	virtual void                      DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	afx_msg void                      OnBnClickedOk();
	virtual BOOL                      OnInitDialog();

	CCmdExeCmdItemClearWindowParams   GetParams() {return m_params;}
	void                              SetParams(CCmdExeCmdItemClearWindowParams params) {m_params = params;}

public:	
	virtual CBasicItemDlg*            Clone();
	virtual void					  SetCmdItem(CCmdExeCmdItem* pItem);
};
