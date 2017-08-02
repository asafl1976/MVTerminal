#pragma once
#include "ClockETPHwModuleBasicDlg.h"
#include "IHwModuleItemDlg.h"

class CClockETPHwModuleModelessDlg : public CClockETPHwModuleBasicDlg, public IHwModuleItemDlg
{
	//For cascading modeless windows
	static	int				m_nWinPos;
	CClockETPHwModule*    	m_pClockETPHwModule;

public:
	CClockETPHwModuleModelessDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CClockETPHwModuleModelessDlg();

	virtual void			SetHwModuleItem(CHwModuleItem *pItem);
	virtual BOOL			Create(CWnd* pParentWnd);
	virtual CBasicItemDlg * Clone();

// Dialog Data
	enum { IDD = IDD_CLOCK_ETP_DLG };

protected:
	virtual void			DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void		    AlignWindow();
	virtual BOOL			OnInitDialog();
	virtual void			OnBnClickedOk();

	//Modeless 
	virtual void			PostNcDestroy();
	afx_msg void			OnDestroy();
	virtual void			OnCancel();
	
	DECLARE_MESSAGE_MAP()

};
