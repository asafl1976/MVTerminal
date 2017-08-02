#pragma once
#include "SUB20HwModuleCtrlADCUnitVCItemModelessDlg.h"

class CSUB20HwModuleCtrlADCUnitCurrentItem;

class CSUB20HwModuleCtrlADCUnitCurrItemModelessDlg : public CSUB20HwModuleCtrlADCUnitVCItemModelessDlg
{
public:
	CSUB20HwModuleCtrlADCUnitCurrentItem* m_pCurrentItem;
	float							      m_nResistorVal;

public:
	CSUB20HwModuleCtrlADCUnitCurrItemModelessDlg(CWnd* pParent = NULL);
	virtual ~CSUB20HwModuleCtrlADCUnitCurrItemModelessDlg(void);

	virtual void			   SetHwModuleItem(CHwModuleItem *pItem);
	virtual BOOL			   Create(CWnd* pParentWnd);
	virtual CBasicItemDlg *    Clone();

	afx_msg void			   OnDestroy();
	virtual BOOL			   OnInitDialog();

	virtual void		       UpdateUIGraphData( );
	virtual void		       StopUpdateThread();
	virtual void		       StartUpdateThread();

	afx_msg void		       OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void               OnEnChangeHwModuleAdcModDlgVolNameEdit();
	afx_msg void               OnEnChangeHwModuleAdcModDlgVolResistorEdit();
	DECLARE_MESSAGE_MAP()
};
