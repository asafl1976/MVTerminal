#pragma once

#include "resource.h"
#include "CmdExeCmdItemDelay.h"
#include "CmdExeCmdItem.h"
#include "BasicItemDlg.h"
#include "ICmdExeCmdItemDlg.h"

// CCmdExeCmdItemDelayDlg dialog


class CCmdExeCmdItemDelayDlg : public CBasicItemDlg, public ICmdExeCmdItemDlg 
{
	CCmdExeCmdItemDelayParams        m_params;
	CCmdExeCmdItemDelay*             m_pItem;			

	CXTPButton				         m_okBtn;
	CXTPButton				         m_cancelBtn;
public:
	CCmdExeCmdItemDelayDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCmdExeCmdItemDelayDlg();

// Dialog Data 
	enum { IDD = IDD_CE_SET_DELAY_DLG };

protected:

	DECLARE_MESSAGE_MAP()

	virtual void                     DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void                     OnBnClickedOk();
	afx_msg void                     OnBnClickedCeSetDelayFixedRadio();
	afx_msg void                     OnBnClickedCeSetDelayRandRadio();
	virtual BOOL                     OnInitDialog();

	CCmdExeCmdItemDelayParams        GetParams() {return m_params;}
	void                             SetParams(CCmdExeCmdItemDelayParams params) {m_params = params;}

public:	
	virtual CBasicItemDlg*           Clone();
	virtual void					 SetCmdItem(CCmdExeCmdItem* pItem);

};
