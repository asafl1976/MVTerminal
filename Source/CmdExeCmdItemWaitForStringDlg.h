#pragma once
#include "resource.h"
#include "afxwin.h"
#include "CmdExeCmdItemWaitForString.h"
#include "BasicItemDlg.h"
#include "ICmdExeCmdItemDlg.h"



// CCmdExeCmdItemWaitForStringDlg dialog


class CCmdExeCmdItemWaitForStringDlg : public CBasicItemDlg, public ICmdExeCmdItemDlg
{
	CCmdExeCmdItemWaitForStringParams   m_params;
	CCmdExeCmdItemWaitForString*        m_pItem;

	CXTPButton  						    m_okBtn;
	CXTPButton  						    m_cancelBtn;
	CComboBox  						    m_targetWinCombo;

public:
	CCmdExeCmdItemWaitForStringDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCmdExeCmdItemWaitForStringDlg();

// Dialog Data
	enum { IDD = IDD_CE_WAIT_FOR_STR_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	afx_msg void						OnBnClickedOk();
	virtual BOOL						OnInitDialog();
	CCmdExeCmdItemWaitForStringParams   GetParams() {return m_params;}
	void                                SetParams(CCmdExeCmdItemWaitForStringParams params) {m_params = params;}
public:	
	virtual CBasicItemDlg*              Clone();
	virtual void					    SetCmdItem(CCmdExeCmdItem* pItem);

};
