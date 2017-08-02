#pragma once

#include "resource.h"
#include "UnknownI2CHwModuleBasicDlg.h"
#include "CmdExeCmdItemUnknownI2CReadWrite.h"
#include "BasicItemDlg.h"
#include "ICmdExeCmdItemDlg.h"


class CCmdExeCmdItemUnknownI2CReadWriteDlg : public CUnknownI2CHwModuleBasicDlg, public ICmdExeCmdItemDlg
{
	CCmdExeCmdItemUnknownI2CReadWrite    *m_pItem;

public:
	CCmdExeCmdItemUnknownI2CReadWriteDlg(CWnd* pParent = NULL);   // standard constructor
	virtual				   ~CCmdExeCmdItemUnknownI2CReadWriteDlg();
	virtual				    CBasicItemDlg* Clone();
	virtual void			SetCmdItem(CCmdExeCmdItem* pItem);
// Dialog Data
	enum { IDD = IDD_UNKNOWN_I2C_DEV_CTRL_DLG };

protected:
	virtual void			DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	virtual BOOL            OnInitDialog();
	afx_msg void            OnBnClickedOk();

	DECLARE_MESSAGE_MAP()
};
