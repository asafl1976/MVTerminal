#pragma once
#include "afxwin.h"
#include "resource.h"
#include "hexedit.h"
#include "CmdExeCmdItemToggleComConnection.h"
#include "ICmdExeCmdItemDlg.h"
#include "BasicItemDlg.h"
#include "CmdExeCmdItemExecuteCommand.h"

class CCmdExeCmdItemToggleComConnectionDlg : public CBasicItemDlg, public ICmdExeCmdItemDlg
{
	CCmdExeCmdItemToggleComConnectionParams m_params;
	CCmdExeCmdItemToggleComConnection*      m_pItem;
	CXTPButton								m_okBtn;
	CXTPButton								m_cancelBtn;
	CComboBox						        m_targetWinCombo;
	CEdit								    m_delayEditCtrl;

public:
	CCmdExeCmdItemToggleComConnectionDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCmdExeCmdItemToggleComConnectionDlg();

// Dialog Data
	enum { IDD = IDD_CE_TOGGLE_COM_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void							 OnBnClickedCeConRadio();
	afx_msg void							 OnBnClickedCeDiscRadio();
	virtual BOOL							 OnInitDialog();
	afx_msg void							 OnBnClickedOk();
	afx_msg void							 OnBnClickedCeComToggleDisCheck();
	afx_msg void							 OnBnClickedCeComToggleConCheck();

	CCmdExeCmdItemToggleComConnectionParams  GetParams() {return m_params;}
	void                                     SetParams(CCmdExeCmdItemToggleComConnectionParams params) {m_params = params;}

public:
	virtual CBasicItemDlg*                   Clone();
	virtual void						     SetCmdItem(CCmdExeCmdItem* pItem);
};
