#pragma once
#include "resource.h"
#include "afxwin.h"
#include "BasicItemDlg.h"
#include "ICmdExeCmdItemDlg.h"
#include "CmdExeCmdItemExecuteCommand.h"

class CCmdExeCmdItemExecuteCommandDlg : public CBasicItemDlg, public ICmdExeCmdItemDlg
{
	CCmdExeCmdItemExecuteCommandParams	m_params;

	CXTPButton					        m_okBtn;
	CXTPButton					        m_cancelBtn;
	CComboBox					        m_targetWinCombo;
	CCmdExeCmdItemExecuteCommand       *m_pItem;

public:
	CCmdExeCmdItemExecuteCommandDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCmdExeCmdItemExecuteCommandDlg();

	virtual CBasicItemDlg*              Clone();
	virtual void						SetCmdItem(CCmdExeCmdItem* pItem);

	// Dialog Data
	enum { IDD = IDD_CE_EXE_CMD_DLG };

protected:
	virtual void					    DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void						OnBnClickedOk();
	virtual BOOL						OnInitDialog();

	CCmdExeCmdItemExecuteCommandParams  GetParams() {return m_params;}
	void                                SetParams(CCmdExeCmdItemExecuteCommandParams params) {m_params = params;}

	DECLARE_MESSAGE_MAP()
};
