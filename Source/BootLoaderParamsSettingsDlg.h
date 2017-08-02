#pragma once
#include ".\BootLoaderParams.h"
#include "Markup.h"
#include "Resource.h"

// CBootLoaderParamsSettingsDlg dialog

class CBootLoaderParamsSettingsDlg : public CDialog
{
private:

	DECLARE_DYNAMIC(CBootLoaderParamsSettingsDlg)

	CBootLoaderParams	m_bootLoaderParams;
	int			        m_nDlgType;
	CXTPButton		    m_restoreDefaultBtn;

public:
	CBootLoaderParamsSettingsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBootLoaderParamsSettingsDlg();
	
	CString		     Serialize();
	BOOL			 DeSerialize(CMarkup & xmlReader);

	void			 UpdateWindowData();
	CBootLoaderParams & GetBootLoaderParams() { return m_bootLoaderParams;}
	void             SetBootLoaderParams(CBootLoaderParams bootLoaderParams) { m_bootLoaderParams = bootLoaderParams;}
	void             SetDlgType(int  nType);
	int		         GetDlgType();

// Dialog Data
	enum { IDD = IDD_MONITOR_PARAMS_SETTINGS_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();

protected:
public:
	afx_msg void OnBnClickedRestoreDefaultBtn();
};
