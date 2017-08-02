#pragma once
#include "afxwin.h"
#include "ODPGHwModule.h"
#include "ODPGPattern.h"
#include "DragDropTreeCtrl.h"
#include "ODPGHwModuleRegAccessBasicDlg.h"
#include "HexEdit.h"
#include "BasicItemDlg.h"


class CODPGHwModuleBasicDlg : public CBasicItemDlg
{
protected:
	CODPGHwModule*          m_pODPGModule;
	CXTPButton			    m_regRWBtn;
	CXTPButton			    m_okBtn;
	CXTPButton				m_cancelBtn;
	CImageList		        m_cImageList;
	CODPGParams				m_params;
	CDragDropTreeCtrl		m_patternTreeCtrl;
	CComboBox				m_busWidthCombo;
	CODPGPattern*		    m_pSelectPattern;
	CString			        m_sTargetDev;
	CString			        m_sDefaultPattern;
	CHexEdit     		    m_addrEditCtrl;
	HTREEITEM				m_tiCurrSelPattern;
	CComboBox				m_targetDevCombo;

	void					UpdatePatterns();
	void					DeleteTreeItemChilds(HTREEITEM tItem);
	virtual void			InitPatternTree();
	HTREEITEM				GetMouseClickedItem(NMHDR *pNMHDR, LRESULT *pResult);
	virtual void			SetItemPattern(HTREEITEM tItem, CODPGPattern *pPattern);
	virtual void			UpdateItemPattern(HTREEITEM tItem, CODPGPattern *pPattern);

public:

	CODPGHwModuleBasicDlg(UINT nIDTemplate, CWnd* pParentWnd = NULL);
	virtual ~CODPGHwModuleBasicDlg();
// Dialog Data
	enum { IDD = IDD_ODPG_CTRL_DLG };

protected:
	virtual void			DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void			OnBnClickedOk();
	afx_msg void			OnBnClickedCancel();
	virtual BOOL			OnInitDialog();

	afx_msg void			OnBnClickedOdpgCtrlReadCompDataCheck();
	afx_msg void			OnBnClickedOdpgCtrlDataBusRadio();
	afx_msg void			OnBnClickedOdpgCtrlBusRadio();
	afx_msg void			OnBnClickedOdpgCtrlStopCondContRadio();
	afx_msg void			OnBnClickedOdpgCtrlStopCondSinglePatternRadio();
	afx_msg void			OnBnClickedOdpgCtrlStopCondPreDefAddrRadio();
	afx_msg void			OnBnClickedOdpgCtrlStopCondErrDataRadio();
	afx_msg void			OnBnClickedOdpgCtrlStopCondPreDefPhaseCountRadio();
	afx_msg void			OnNMDblclkOdpgCtrlPatternTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void			OnNMClickOdpgCtrlPatternTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void			OnCbnSelchangeOdpgCtrlTargetDeviceCombo();
	afx_msg void			OnCbnSelchangeOdpgCtrlBusWidthCombo();
	afx_msg void			OnBnClickedOdpgCtrlReadRadio();
	afx_msg void			OnBnClickedOdpgCtrlWriteRadio();

	virtual CODPGParams	    GetParams() 	              {return m_params;}
	virtual void            SetParams(CODPGParams params) {m_params = params;}

	DECLARE_MESSAGE_MAP()

};
