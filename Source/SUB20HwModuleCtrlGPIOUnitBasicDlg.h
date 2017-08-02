#pragma once

#include "resource.h"
#include "ComboListCtrl.h"
#include "HwModuleCtrlGPIOUnitDrv.h"
#include "SUB20HwModuleCtrlGPIOUnit.h"
#include "afxwin.h"
#include "ResizeDlg\resize.h"
#include "BasicItemDlg.h"

#define GPIO_NAME_COL   2
#define GPIO_TYPE_COL   3
#define GPIO_STATE_COL  4

//For dlg resize size and position saving 
#define SECTION _T("CSUB20HwModuleCtrlGPIOUnitBasicDlg")

class CSUB20HwModuleCtrlGPIOUnit;
class CSUB20HwModuleCtrlGPIOUnitIOItem;



class CSUB20HwModuleCtrlGPIOUnitBasicDlg : 	public CBasicItemDlg, public CResize
{
	//DECLARE_DYNAMIC(CSUB20HwModuleCtrlGPIOUnitBasicDlg)
protected:
	HICON m_hIcon;

	CXTPButton					m_updateAllBtn;
	CXTPButton				    m_cancelBtn;
	CXTPButton				    m_updateSelectedBtn;
	CXTPButton   			    m_restoreDefaultsBtn;
	CComboListCtrl				m_GPIOListCtrl;
	CImageList				    m_imgList;
	CSUB20HwModuleCtrlGPIOUnit* m_pGPIOUnit;
	
	virtual int					StringTypeToInt(CString sType);
	virtual int					StringStateToInt(CString sState);
	virtual CString				TypeToString(int nType);
	virtual CString				StateToString(int nState);

public:
		// Dialog Data
	enum { IDD = IDD_SUB20_GPIO_UNIT_DLG };

	CSUB20HwModuleCtrlGPIOUnitBasicDlg(UINT nIDTemplate, CWnd* pParentWnd = NULL);
	virtual ~CSUB20HwModuleCtrlGPIOUnitBasicDlg(void);

protected:
	virtual BOOL				SetItem(int nItem,CSUB20HwModuleCtrlGPIOUnitIOItem* pGPIO);
	virtual void    			UpdateItem(int nItem);
	virtual BOOL				SetItemName(int nRow, CString sName);
	virtual BOOL    			SetItemState(int nRow, int nState);
	virtual BOOL    			SetItemType(int nRow, int nType);
	virtual void				DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL				OnInitDialog();
	virtual LRESULT 			PopulateComboList(WPARAM wParam, LPARAM lParam);
	virtual LRESULT 			OnComboSelectionEnd(WPARAM wParam, LPARAM lParam);
	virtual LRESULT 			OnEditNameEnd(WPARAM wParam, LPARAM lParam);
	virtual LRESULT 			OnComboStart(WPARAM wParam, LPARAM lParam);

	afx_msg void 				OnSize(UINT nType, int cx, int cy);
	afx_msg void 				OnGetMinMaxInfo(MINMAXINFO *lpMMI);

	afx_msg void 				OnBnClickedGpioUnitUpdateSelectedBtn();
	afx_msg void 				OnBnClickedGpioUnitUpdateAllBtn();
	afx_msg void 				OnBnClickedCancel();
	afx_msg void 				OnBnClickedGpioUnitRestoreDefaultsBtn();

	DECLARE_MESSAGE_MAP()


};
