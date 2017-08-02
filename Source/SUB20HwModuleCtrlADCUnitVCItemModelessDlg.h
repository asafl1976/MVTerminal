#pragma once
#include "SUB20HwModuleCtrlADCUnitVCItemBasicDlg.h"
#include "resource.h"
#include "2DPushGraph.h"
#include "afxcmn.h"
#include "afxwin.h"
#include "IHwModuleItemDlg.h"

//ADC Voltage/Current dlg
class CSUB20HwModuleCtrlADCUnitVCItemModelessDlg : 	public CSUB20HwModuleCtrlADCUnitVCItemBasicDlg, public IHwModuleItemDlg
{
	//For cascading modeless windows
	static	int				 m_nWinPos;

protected:
	UINT   GetSampleSum(int* pValArray,int nSampleCount);


public:
	enum { IDD = IDD_SUB20_HW_MODULE_CTRL_ADC_UNIT_CUR_VOL_ITEM_MODLS_DLG };

	CSUB20HwModuleCtrlADCUnitVCItemModelessDlg(CWnd* pParent = NULL);
	virtual ~CSUB20HwModuleCtrlADCUnitVCItemModelessDlg(void);

protected:
	virtual void			DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void		    AlignWindow();
	virtual BOOL			OnInitDialog();

	//Modeless 
	virtual void			PostNcDestroy();
	virtual void			OnDestroy() = 0;
	virtual void			OnCancel();
	
	DECLARE_MESSAGE_MAP()

public:
};
