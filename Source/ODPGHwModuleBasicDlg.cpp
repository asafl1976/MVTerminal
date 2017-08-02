// ODPGControlDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MVTerminal.h"
#include "ODPGHwModuleBasicDlg.h"
#include "odpghwmodule.h"
#include "ODPGPatternFactory.h"
#include "ODPGHwModuleRegAccessBasicDlg.h"

// CODPGHwModuleBasicDlg dialog

CODPGHwModuleBasicDlg::CODPGHwModuleBasicDlg(UINT nIDTemplate, CWnd* pParentWnd)
: CBasicItemDlg(nIDTemplate,pParentWnd)
{ 
	m_sTargetDev	   = "MV88F6601";//Defult device
	m_tiCurrSelPattern = NULL;
	m_sDefaultPattern  = "SSO";
}

CODPGHwModuleBasicDlg::~CODPGHwModuleBasicDlg()
{
}




void CODPGHwModuleBasicDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_okBtn);
	DDX_Control(pDX, IDCANCEL, m_cancelBtn);
	DDX_Control(pDX, IDC_ODPG_CTRL_RW_REG_BTN, m_regRWBtn);
	DDX_Control(pDX, IDC_ODPG_CTRL_PATTERN_TREE, m_patternTreeCtrl);

	DDX_Control(pDX, IDC_ODPG_CTRL_TARGET_DEVICE_COMBO, m_targetDevCombo);
	DDX_Control(pDX, IDC_ODPG_CTRL_BUS_WIDTH_COMBO, m_busWidthCombo);
	DDX_Control(pDX, IDC_ODPG_CTRL_DATA_BUS_ADDR_EDIT, m_addrEditCtrl);
}


BEGIN_MESSAGE_MAP(CODPGHwModuleBasicDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_ODPG_CTRL_READ_COMP_DATA_CHECK, OnBnClickedOdpgCtrlReadCompDataCheck)
	ON_BN_CLICKED(IDC_ODPG_CTRL_DATA_BUS_RADIO, OnBnClickedOdpgCtrlDataBusRadio)
	ON_BN_CLICKED(IDC_ODPG_CTRL_BUS_RADIO, OnBnClickedOdpgCtrlBusRadio)
	ON_BN_CLICKED(IDC_ODPG_CTRL_STOP_COND_CONT_RADIO, OnBnClickedOdpgCtrlStopCondContRadio)
	ON_BN_CLICKED(IDC_ODPG_CTRL_STOP_COND_SINGLE_PATTERN_RADIO, OnBnClickedOdpgCtrlStopCondSinglePatternRadio)
	ON_BN_CLICKED(IDC_ODPG_CTRL_STOP_COND_PRE_DEF_ADDR_RADIO, OnBnClickedOdpgCtrlStopCondPreDefAddrRadio)
	ON_BN_CLICKED(IDC_ODPG_CTRL_STOP_COND_ERR_DATA_RADIO, OnBnClickedOdpgCtrlStopCondErrDataRadio)
	ON_BN_CLICKED(IDC_ODPG_CTRL_STOP_COND_PRE_DEF_PHASE_COUNT_RADIO, OnBnClickedOdpgCtrlStopCondPreDefPhaseCountRadio)
	ON_NOTIFY(NM_DBLCLK, IDC_ODPG_CTRL_PATTERN_TREE, OnNMDblclkOdpgCtrlPatternTree)
	ON_NOTIFY(NM_CLICK, IDC_ODPG_CTRL_PATTERN_TREE, OnNMClickOdpgCtrlPatternTree)
	ON_CBN_SELCHANGE(IDC_ODPG_CTRL_TARGET_DEVICE_COMBO, OnCbnSelchangeOdpgCtrlTargetDeviceCombo)
	ON_CBN_SELCHANGE(IDC_ODPG_CTRL_BUS_WIDTH_COMBO, OnCbnSelchangeOdpgCtrlBusWidthCombo)
	ON_BN_CLICKED(IDC_ODPG_CTRL_READ_RADIO, OnBnClickedOdpgCtrlReadRadio)
	ON_BN_CLICKED(IDC_ODPG_CTRL_WRITE_RADIO, OnBnClickedOdpgCtrlWriteRadio)
END_MESSAGE_MAP()



void CODPGHwModuleBasicDlg::OnBnClickedOk()
{
	CDialog::OnOK();
}

void CODPGHwModuleBasicDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialog::OnCancel();
}


BOOL CODPGHwModuleBasicDlg::OnInitDialog()
{
	CString str;

	CDialog::OnInitDialog();

	if(m_pODPGModule == NULL)
	{
		MessageBox("ODPG module not set!",NULL,MB_ICONERROR);
		return FALSE;
	}

	m_pODPGModule->SetDevice(m_sTargetDev);

	m_okBtn.SetTheme(xtpButtonThemeOffice2000);
	m_cancelBtn.SetTheme(xtpButtonThemeOffice2000);
	m_regRWBtn.SetTheme(xtpButtonThemeOffice2000);
	m_cImageList.Create(16,16, ILC_COLOR32 , 1, 0);
	CBitmap bmpImage;
	bmpImage.LoadBitmap(IDB_PATTERN_BMP); 
	m_cImageList.Add(&bmpImage, (COLORREF)NULL);
	bmpImage.DeleteObject();

	bmpImage.LoadBitmap(IDB_PATTERN_ENTRY_BMP); 
	m_cImageList.Add(&bmpImage, (COLORREF)NULL);
	bmpImage.DeleteObject();

	m_busWidthCombo.AddString("16");
	m_busWidthCombo.AddString("32");
	m_busWidthCombo.AddString("64");

	vector<CString>			  devList;
	vector<CString>::iterator iter,endIter;

	m_pODPGModule->GetDeviceList(devList);
	iter    = devList.begin();
	endIter = devList.end();
	for(;iter != endIter;iter++)
	{
		m_targetDevCombo.AddString(*iter);
	}

	GetDlgItem(IDC_ODPG_CTRL_DATA_BUS_ADDR_EDIT)->EnableWindow(FALSE);
	SetDlgItemText(IDC_ODPG_CTRL_DATA_BUS_ADDR_EDIT,"");
	switch(m_params.m_eBusType)
	{
	case ODPG_DATA_BUS:
		CheckDlgButton(IDC_ODPG_CTRL_DATA_BUS_RADIO,TRUE);
		GetDlgItem(IDC_ODPG_CTRL_DATA_BUS_ADDR_EDIT)->EnableWindow(TRUE);
		break;
	case ODPG_CTRL_BUS:
		CheckDlgButton(IDC_ODPG_CTRL_BUS_RADIO,TRUE);
		break;
	default:
		CheckDlgButton(IDC_ODPG_CTRL_DATA_BUS_RADIO,TRUE);
		break;
	}


	// Disable temporary unsupported stop modes
	GetDlgItem(IDC_ODPG_CTRL_STOP_COND_ERR_DATA_RADIO)->EnableWindow(FALSE);
	GetDlgItem(IDC_ODPG_CTRL_STOP_COND_PRE_DEF_ADDR_RADIO)->EnableWindow(FALSE);
	GetDlgItem(IDC_ODPG_CTRL_STOP_COND_PRE_DEF_PHASE_COUNT_RADIO)->EnableWindow(FALSE);

	// Disable control bus mode
	GetDlgItem(IDC_ODPG_CTRL_BUS_RADIO)->EnableWindow(FALSE);

	GetDlgItem(IDC_ODPG_CTRL_STOP_COND_PRE_DEF_PHASE_COUNT_EDIT)->EnableWindow(FALSE);
	SetDlgItemText(IDC_ODPG_CTRL_STOP_COND_PRE_DEF_PHASE_COUNT_EDIT,"");
	switch(m_params.m_eStopCond)
	{
	case ODPG_STOP_COND_CONTINUOUS:
		CheckDlgButton(IDC_ODPG_CTRL_STOP_COND_CONT_RADIO,TRUE);
		break;
	case ODPG_STOP_COND_SINGLE_PATTERN:
		CheckDlgButton(IDC_ODPG_CTRL_STOP_COND_SINGLE_PATTERN_RADIO,TRUE);
		break;
	case ODPG_STOP_COND_PRE_DEF_ADDR:
		CheckDlgButton(IDC_ODPG_CTRL_STOP_COND_PRE_DEF_ADDR_RADIO,TRUE);
		break;
	case ODPG_STOP_COND_ERR_DATA_ADDR:
		CheckDlgButton(IDC_ODPG_CTRL_STOP_COND_ERR_DATA_RADIO,TRUE);
		break;
	case ODPG_STOP_COND_PHASE_COUNT:
		CheckDlgButton(IDC_ODPG_CTRL_STOP_COND_PRE_DEF_PHASE_COUNT_RADIO,TRUE);
		GetDlgItem(IDC_ODPG_CTRL_STOP_COND_PRE_DEF_PHASE_COUNT_EDIT)->EnableWindow(TRUE);
		str.Format("%d", m_params.m_nStopCondPhaseCount);
		SetDlgItemText(IDC_ODPG_CTRL_STOP_COND_PRE_DEF_PHASE_COUNT_EDIT,str);
		break;
	default:
		CheckDlgButton(IDC_ODPG_CTRL_STOP_COND_SINGLE_PATTERN_RADIO,TRUE);
		break;
	}

	CheckDlgButton(IDC_ODPG_CTRL_READ_RADIO,m_params.m_bRead);
	CheckDlgButton(IDC_ODPG_CTRL_WRITE_RADIO,m_params.m_bWrite);
	CheckDlgButton(IDC_ODPG_CTRL_READ_COMP_DATA_CHECK,m_params.m_bCompareData);
	GetDlgItem(IDC_ODPG_CTRL_READ_COMP_DATA_CHECK)->EnableWindow(m_params.m_bRead);

	str.Format("%x",m_params.m_nAddress);
	SetDlgItemText(IDC_ODPG_CTRL_DATA_BUS_ADDR_EDIT,str);

	m_targetDevCombo.SelectString(-1,m_sTargetDev);	

	CString sWidth = m_pODPGModule->GetDeviceBusWidth(m_sTargetDev);
	sscanf(sWidth,"%d",&m_params.m_nBusWidth);
	m_busWidthCombo.SelectString(-1,sWidth);

	InitPatternTree();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}



void CODPGHwModuleBasicDlg::UpdateItemPattern(HTREEITEM tItem, CODPGPattern *pPattern)
{
     HTREEITEM  tNewItem;
	 CString	sItem;
	
	 if(pPattern == NULL)
		return;

     sItem = m_patternTreeCtrl.GetItemText(tItem);
     m_patternTreeCtrl.DeleteItem(tItem);
     tNewItem = m_patternTreeCtrl.InsertItem(sItem,0,0);
	 SetItemPattern(tNewItem,pPattern);
}

void CODPGHwModuleBasicDlg::DeleteTreeItemChilds(HTREEITEM tItem)
{
	m_patternTreeCtrl.DeleteItemChilds(tItem);


}

void CODPGHwModuleBasicDlg::SetItemPattern(HTREEITEM tItem, CODPGPattern *pPattern)
{
	int			nEntry,nEntryCount;
	HTREEITEM   hItem;

	if(pPattern == NULL)
		return;

	pPattern->Create();
	DeleteTreeItemChilds(tItem);
	m_patternTreeCtrl.SetItemData(tItem,(DWORD_PTR)pPattern);
    nEntryCount = pPattern->GetEntryCount();
	for(nEntry = 0; nEntry < nEntryCount; nEntry++)
	{
		CString sLine;
		if(nEntry < 10)
			sLine.Format("Phase#0%d - ",nEntry);
		else
			sLine.Format("Phase#%d - ",nEntry);

		sLine += ("0x" + pPattern->GetEntry(nEntry).ToString().MakeUpper());

		hItem = m_patternTreeCtrl.InsertItem(sLine,1,1,tItem);
		m_patternTreeCtrl.RemoveItemCheckBox(hItem);
	}
}

void CODPGHwModuleBasicDlg::UpdatePatterns()
{
	HTREEITEM	    hItem;
	CODPGPattern   *pPattern; 
	
	hItem = m_patternTreeCtrl.GetRootItem();
	pPattern = (CODPGPattern*)m_patternTreeCtrl.GetItemData(hItem);
	if(pPattern != NULL)
	{
		pPattern->SetWidth(m_params.m_nBusWidth);
		SetItemPattern(hItem,pPattern);
	}
    hItem = m_patternTreeCtrl.GetNextSiblingItem( hItem );
    while( hItem != NULL)
    {
		pPattern = (CODPGPattern*)m_patternTreeCtrl.GetItemData(hItem);
		if(pPattern != NULL)
		{
			pPattern->SetWidth(m_params.m_nBusWidth);
			SetItemPattern(hItem,pPattern);
		}
        hItem = m_patternTreeCtrl.GetNextSiblingItem( hItem );
    }
	m_patternTreeCtrl.RedrawWindow();
}

void CODPGHwModuleBasicDlg::InitPatternTree()
{
	vector<CString> patternVec;
	vector<CString>::iterator iter, endIter;

	m_patternTreeCtrl.SetImageList(&m_cImageList, TVSIL_NORMAL);	
	m_patternTreeCtrl.RegisterDropTarget();
	m_patternTreeCtrl.SetIsDroppable(FALSE);
	m_patternTreeCtrl.SetCanMoveItems(FALSE);

	CODPGPatternFactory::GetInstance().GetPatternTypeList(patternVec);
	iter    = patternVec.begin();
	endIter = patternVec.end();

	for(; iter != endIter; iter++)
	{
		CODPGPattern *pPattern = CODPGPatternFactory::GetInstance().Create((*iter));
		if(pPattern != NULL)
		{
			HTREEITEM htItem = m_patternTreeCtrl.InsertItem((*iter), 0,0);
			m_patternTreeCtrl.SetItemBold(htItem);
			m_patternTreeCtrl.AddNonDragableItem((*iter));
			pPattern->SetHwModule(m_pODPGModule);
			pPattern->SetWidth(m_params.m_nBusWidth);
			SetItemPattern(htItem,pPattern);

			if(pPattern->GetDescription() == m_sDefaultPattern)
			{
				m_params.m_pPattern = pPattern;
				m_tiCurrSelPattern = htItem;
				m_patternTreeCtrl.SetCheck(m_tiCurrSelPattern,TRUE);
				m_patternTreeCtrl.SelectItem(htItem);
			}
		}
	}
}




void CODPGHwModuleBasicDlg::OnBnClickedOdpgCtrlReadCompDataCheck()
{
	m_params.m_bCompareData = !m_params.m_bCompareData;
}

void CODPGHwModuleBasicDlg::OnBnClickedOdpgCtrlDataBusRadio()
{
	m_params.m_eBusType = ODPG_DATA_BUS;

	CheckDlgButton(IDC_ODPG_CTRL_DATA_BUS_RADIO,TRUE);
	CheckDlgButton(IDC_ODPG_CTRL_BUS_RADIO,FALSE);
}

void CODPGHwModuleBasicDlg::OnBnClickedOdpgCtrlBusRadio()
{
	m_params.m_eBusType = ODPG_CTRL_BUS;

	CheckDlgButton(IDC_ODPG_CTRL_DATA_BUS_RADIO,FALSE);
	CheckDlgButton(IDC_ODPG_CTRL_BUS_RADIO,TRUE);

}

void CODPGHwModuleBasicDlg::OnBnClickedOdpgCtrlStopCondContRadio()
{
	m_params.m_eStopCond = ODPG_STOP_COND_CONTINUOUS;

	CheckDlgButton(IDC_ODPG_CTRL_STOP_COND_CONT_RADIO,TRUE);
	CheckDlgButton(IDC_ODPG_CTRL_STOP_COND_ERR_DATA_RADIO,FALSE);
	CheckDlgButton(IDC_ODPG_CTRL_STOP_COND_PRE_DEF_ADDR_RADIO,FALSE);
	CheckDlgButton(IDC_ODPG_CTRL_STOP_COND_PRE_DEF_PHASE_COUNT_RADIO,FALSE);
	CheckDlgButton(IDC_ODPG_CTRL_STOP_COND_SINGLE_PATTERN_RADIO,FALSE);
}

void CODPGHwModuleBasicDlg::OnBnClickedOdpgCtrlStopCondSinglePatternRadio()
{
	m_params.m_eStopCond = ODPG_STOP_COND_SINGLE_PATTERN;

	CheckDlgButton(IDC_ODPG_CTRL_STOP_COND_CONT_RADIO,FALSE);
	CheckDlgButton(IDC_ODPG_CTRL_STOP_COND_ERR_DATA_RADIO,FALSE);
	CheckDlgButton(IDC_ODPG_CTRL_STOP_COND_PRE_DEF_ADDR_RADIO,FALSE);
	CheckDlgButton(IDC_ODPG_CTRL_STOP_COND_PRE_DEF_PHASE_COUNT_RADIO,FALSE);
	CheckDlgButton(IDC_ODPG_CTRL_STOP_COND_SINGLE_PATTERN_RADIO,TRUE);
}

void CODPGHwModuleBasicDlg::OnBnClickedOdpgCtrlStopCondPreDefAddrRadio()
{
	m_params.m_eStopCond = ODPG_STOP_COND_PRE_DEF_ADDR;

	CheckDlgButton(IDC_ODPG_CTRL_STOP_COND_CONT_RADIO,FALSE);
	CheckDlgButton(IDC_ODPG_CTRL_STOP_COND_ERR_DATA_RADIO,FALSE);
	CheckDlgButton(IDC_ODPG_CTRL_STOP_COND_PRE_DEF_ADDR_RADIO,TRUE);
	CheckDlgButton(IDC_ODPG_CTRL_STOP_COND_PRE_DEF_PHASE_COUNT_RADIO,FALSE);
	CheckDlgButton(IDC_ODPG_CTRL_STOP_COND_SINGLE_PATTERN_RADIO,FALSE);
}

void CODPGHwModuleBasicDlg::OnBnClickedOdpgCtrlStopCondErrDataRadio()
{
	m_params.m_eStopCond = ODPG_STOP_COND_ERR_DATA_ADDR;

	CheckDlgButton(IDC_ODPG_CTRL_STOP_COND_CONT_RADIO,FALSE);
	CheckDlgButton(IDC_ODPG_CTRL_STOP_COND_ERR_DATA_RADIO,TRUE);
	CheckDlgButton(IDC_ODPG_CTRL_STOP_COND_PRE_DEF_ADDR_RADIO,FALSE);
	CheckDlgButton(IDC_ODPG_CTRL_STOP_COND_PRE_DEF_PHASE_COUNT_RADIO,FALSE);
	CheckDlgButton(IDC_ODPG_CTRL_STOP_COND_SINGLE_PATTERN_RADIO,FALSE);
}

void CODPGHwModuleBasicDlg::OnBnClickedOdpgCtrlStopCondPreDefPhaseCountRadio()
{
	m_params.m_eStopCond = ODPG_STOP_COND_PHASE_COUNT;

	CheckDlgButton(IDC_ODPG_CTRL_STOP_COND_CONT_RADIO,FALSE);
	CheckDlgButton(IDC_ODPG_CTRL_STOP_COND_ERR_DATA_RADIO,FALSE);
	CheckDlgButton(IDC_ODPG_CTRL_STOP_COND_PRE_DEF_ADDR_RADIO,FALSE);
	CheckDlgButton(IDC_ODPG_CTRL_STOP_COND_PRE_DEF_PHASE_COUNT_RADIO,TRUE);
	CheckDlgButton(IDC_ODPG_CTRL_STOP_COND_SINGLE_PATTERN_RADIO,FALSE);

}

void CODPGHwModuleBasicDlg::OnNMDblclkOdpgCtrlPatternTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	
	*pResult = 0;
}

HTREEITEM CODPGHwModuleBasicDlg::GetMouseClickedItem(NMHDR *pNMHDR, LRESULT *pResult)
{
     CPoint     point;
	 UINT       uFlags;
     HTREEITEM  item = NULL;

	 GetCursorPos(&point);   
	 m_patternTreeCtrl.ScreenToClient(&point);
	 item = m_patternTreeCtrl.HitTest(point, &uFlags);
	 if(item)
	 { 
         // There is an item under the cursor. 
		 // See what exactly was under the cursor:
		 switch(uFlags)
		 {
		 case TVHT_ONITEMSTATEICON:
	 		break;
	  	 case TVHT_ONITEMBUTTON:
			break;
		 case TVHT_ONITEMLABEL:
			return item;
		 default:
			break;
	   	}
	 }	
	 *pResult = 0;
	 return NULL;
}


void CODPGHwModuleBasicDlg::OnNMClickOdpgCtrlPatternTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	CPoint     point;
	UINT       uFlags;
    HTREEITEM  item = NULL;

	GetCursorPos(&point);   
	m_patternTreeCtrl.ScreenToClient(&point);
	item = m_patternTreeCtrl.HitTest(point, &uFlags);
	if(item)
	{ 
        m_patternTreeCtrl.SelectItem(item);
        // There is an item under the cursor. 
		// See what exactly was under the cursor:
		switch(uFlags)
		{
		case TVHT_ONITEMSTATEICON://Check box
            if(m_tiCurrSelPattern != NULL)
			{
				m_patternTreeCtrl.SetCheck(m_tiCurrSelPattern,FALSE);
			}
			m_params.m_pPattern = (CODPGPattern*)m_patternTreeCtrl.GetItemData( item );
			m_pSelectPattern = m_params.m_pPattern;
			m_sDefaultPattern = m_params.m_pPattern->GetDescription();
			m_tiCurrSelPattern  = item;
			m_patternTreeCtrl.SetCheck(m_tiCurrSelPattern,TRUE);
			if(m_params.m_pPattern->OnSelect() == TRUE)
			{
				CString sWidth;

				GetDlgItemText(IDC_ODPG_CTRL_BUS_WIDTH_COMBO,sWidth);
				sscanf(sWidth,"%d",&m_params.m_nBusWidth);
				UpdatePatterns();
			}
			*pResult = 1;
	 		break;
		 default:
			*pResult = 0;
			break;
	   	}
	}	
}


void CODPGHwModuleBasicDlg::OnCbnSelchangeOdpgCtrlTargetDeviceCombo()
{
	GetDlgItemText(IDC_ODPG_CTRL_TARGET_DEVICE_COMBO,m_sTargetDev);
	m_pODPGModule->SetDevice(m_sTargetDev);
}

void CODPGHwModuleBasicDlg::OnCbnSelchangeOdpgCtrlBusWidthCombo()
{
	CString sWidth;

	GetDlgItemText(IDC_ODPG_CTRL_BUS_WIDTH_COMBO,sWidth);
	sscanf(sWidth,"%d",&m_params.m_nBusWidth);
	UpdatePatterns();
}


void CODPGHwModuleBasicDlg::OnBnClickedOdpgCtrlReadRadio()
{
	m_params.m_bRead  = TRUE;
	m_params.m_bWrite = FALSE;
	GetDlgItem(IDC_ODPG_CTRL_READ_COMP_DATA_CHECK)->EnableWindow(m_params.m_bRead);
	CheckDlgButton(IDC_ODPG_CTRL_READ_COMP_DATA_CHECK,m_params.m_bCompareData);

	CheckDlgButton(IDC_ODPG_CTRL_READ_RADIO,TRUE);
	CheckDlgButton(IDC_ODPG_CTRL_WRITE_RADIO,FALSE);

	GetDlgItem(IDC_ODPG_CTRL_PATTERN_TREE)->EnableWindow(FALSE);
}

void CODPGHwModuleBasicDlg::OnBnClickedOdpgCtrlWriteRadio()
{
	m_params.m_bRead  = FALSE;
	m_params.m_bWrite = TRUE;
	GetDlgItem(IDC_ODPG_CTRL_READ_COMP_DATA_CHECK)->EnableWindow(m_params.m_bRead);
	CheckDlgButton(IDC_ODPG_CTRL_READ_COMP_DATA_CHECK,FALSE);

	CheckDlgButton(IDC_ODPG_CTRL_READ_RADIO,FALSE);
	CheckDlgButton(IDC_ODPG_CTRL_WRITE_RADIO,TRUE);

	GetDlgItem(IDC_ODPG_CTRL_PATTERN_TREE)->EnableWindow(TRUE);

}
