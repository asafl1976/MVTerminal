#include "StdAfx.h"
#include ".\sub20hwmodulectrlgpiounitmodelessdlg.h"
#include "SUB20HwModuleCtrlGPIOUnit.h"

//IMPLEMENT_DYNAMIC(CSUB20HwModuleCtrlGPIOUnitModelessDlg, CSUB20HwModuleCtrlGPIOUnitBasicDlg)

CSUB20HwModuleCtrlGPIOUnitModelessDlg::CSUB20HwModuleCtrlGPIOUnitModelessDlg(CWnd* pParent /*=NULL*/)
	: CSUB20HwModuleCtrlGPIOUnitBasicDlg(CSUB20HwModuleCtrlGPIOUnitModelessDlg::IDD, pParent)
{
}

CSUB20HwModuleCtrlGPIOUnitModelessDlg::~CSUB20HwModuleCtrlGPIOUnitModelessDlg()
{
}

void CSUB20HwModuleCtrlGPIOUnitModelessDlg::DoDataExchange(CDataExchange* pDX)
{
	CSUB20HwModuleCtrlGPIOUnitBasicDlg::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSUB20HwModuleCtrlGPIOUnitModelessDlg, CSUB20HwModuleCtrlGPIOUnitBasicDlg)
	ON_WM_DESTROY()	
	ON_MESSAGE(WM_SET_ITEMS, PopulateComboList)
	ON_BN_CLICKED(IDCANCEL, &CSUB20HwModuleCtrlGPIOUnitModelessDlg::OnBnClickedCancel)
END_MESSAGE_MAP()

// CSummitSMM205HwModuleModelessDlg message handlers

void CSUB20HwModuleCtrlGPIOUnitModelessDlg::OnDestroy()
{
	//Notify owner (the TreeView control) that dlg is destroyed
	::SendMessage(GetOwner()->m_hWnd,WM_HW_MODULE_ITEM_MODELESS_DLG_DESTROYED,(WPARAM)m_pGPIOUnit,NULL);

	CDialog::OnDestroy();
}



BOOL CSUB20HwModuleCtrlGPIOUnitModelessDlg::OnInitDialog()
{
	CSUB20HwModuleCtrlGPIOUnitBasicDlg::OnInitDialog();

	//LoadPlacement(SECTION);

	AlignWindow();
	SetWindowText(m_pGPIOUnit->GetDisplayName());
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSUB20HwModuleCtrlGPIOUnitModelessDlg::PostNcDestroy()
{
	delete this;
}

void  CSUB20HwModuleCtrlGPIOUnitModelessDlg::SetHwModuleItem(CHwModuleItem *pItem)
{
	m_pGPIOUnit = dynamic_cast<CSUB20HwModuleCtrlGPIOUnit*>(pItem);
}

BOOL  CSUB20HwModuleCtrlGPIOUnitModelessDlg::Create(CWnd* pParentWnd)
{
	return CDialog::Create(CSUB20HwModuleCtrlGPIOUnitModelessDlg::IDD,pParentWnd);
}

CBasicItemDlg *CSUB20HwModuleCtrlGPIOUnitModelessDlg::Clone()
{
	return new CSUB20HwModuleCtrlGPIOUnitModelessDlg();
}


void CSUB20HwModuleCtrlGPIOUnitModelessDlg::OnBnClickedCancel()
{
	DestroyWindow();
}
