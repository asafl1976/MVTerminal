#pragma once
#include "SUB20HwModuleCtrlADCUnitVCItemModelessDlg.h"

class CSUB20HwModuleCtrlADCUnitVoltageItem;

class CSUB20HwModuleCtrlADCUnitVolItemModelessDlg : public CSUB20HwModuleCtrlADCUnitVCItemModelessDlg
{
public: 

	CSUB20HwModuleCtrlADCUnitVoltageItem* m_pVolItem;

public:
	CSUB20HwModuleCtrlADCUnitVolItemModelessDlg(CWnd* pParent = NULL);
	virtual ~CSUB20HwModuleCtrlADCUnitVolItemModelessDlg(void);

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

	DECLARE_MESSAGE_MAP()
};
