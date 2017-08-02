#include "StdAfx.h"
#include ".\sub20hwmodulectrlgpiounitbasicdlg.h"
#include "SUB20HwModuleCtrlGPIOUnit.h"
#include "SUB20HwModuleCtrlGPIOUnitIOItem.h"
#include "InPlaceCombo.h"
#include "InPlaceEdit.h"


//IMPLEMENT_DYNAMIC(CSUB20HwModuleCtrlGPIOUnitBasicDlg, CDialog)

CSUB20HwModuleCtrlGPIOUnitBasicDlg::CSUB20HwModuleCtrlGPIOUnitBasicDlg(UINT nIDTemplate, CWnd* pParentWnd) :
CBasicItemDlg(nIDTemplate,pParentWnd),
 CResize(this, SZ_NOHORISONTAL)

{
	m_GPIOListCtrl.SetParentDlg(this);
	m_hIcon = AfxGetApp()->LoadIcon(IDI_HW_MODULE_CTRL_UNIT_ICON);
 
}

CSUB20HwModuleCtrlGPIOUnitBasicDlg::~CSUB20HwModuleCtrlGPIOUnitBasicDlg(void)
{
}

void CSUB20HwModuleCtrlGPIOUnitBasicDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GPIO_UNIT_UPDATE_ALL_BTN, m_updateAllBtn);
	DDX_Control(pDX, IDCANCEL, m_cancelBtn);
	DDX_Control(pDX, IDC_SUB20_GPIO_UNIT_LIST_CTRL, m_GPIOListCtrl);
	DDX_Control(pDX, IDC_BUTTON1, m_updateSelectedBtn);
	DDX_Control(pDX, IDC_GPIO_UNIT_RESTORE_DEFAULTS_BTN, m_restoreDefaultsBtn);
}

BEGIN_MESSAGE_MAP(CSUB20HwModuleCtrlGPIOUnitBasicDlg, CDialog)
	ON_MESSAGE(WM_SET_ITEMS, PopulateComboList)
	ON_MESSAGE(WM_GPIO_BASIC_DLG_COMBO_SEL_END, OnComboSelectionEnd)
	ON_MESSAGE(WM_GPIO_BASIC_DLG_ON_COMBO_START, OnComboStart)
	ON_MESSAGE(WM_GPIO_BASIC_DLG_EDIT_END,OnEditNameEnd)

	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_BN_CLICKED(IDC_GPIO_UNIT_UPDATE_SELECTED_BTN, OnBnClickedGpioUnitUpdateSelectedBtn)
	ON_BN_CLICKED(IDC_GPIO_UNIT_UPDATE_ALL_BTN, OnBnClickedGpioUnitUpdateAllBtn)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_GPIO_UNIT_RESTORE_DEFAULTS_BTN, OnBnClickedGpioUnitRestoreDefaultsBtn)
//	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()



void CSUB20HwModuleCtrlGPIOUnitBasicDlg::OnGetMinMaxInfo(MINMAXINFO* pMMI)
{
	CResize::GetMinMaxInfo(pMMI);
}

void CSUB20HwModuleCtrlGPIOUnitBasicDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	CResize::Size();
}
BOOL CSUB20HwModuleCtrlGPIOUnitBasicDlg::OnInitDialog()
{
	ASSERT(GetStyle() & WS_THICKFRAME); // Needs resizable frame
	CDialog::OnInitDialog();
	CResize::Init();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// sizing set directly with functions
	SetResize(IDC_GPIO_UNIT_UPDATE_ALL_BTN, 0, 0, 0, 0);
	SetResize(IDC_GPIO_UNIT_UPDATE_SELECTED_BTN,0, 0, 0, 0);
	SetResize(IDC_GPIO_UNIT_RESTORE_DEFAULTS_BTN, 1, 1, 1, 1);
	SetResize(IDCANCEL, 1, 1, 1, 1);
	SetResize(IDC_SUB20_GPIO_UNIT_LIST_CTRL, 0, 0, 0, 1);

	LoadPlacement(SECTION);
	AlignWindow();

	m_updateAllBtn.SetTheme(xtpButtonThemeOffice2000);
	m_cancelBtn.SetTheme(xtpButtonThemeOffice2000);
	m_updateSelectedBtn.SetTheme(xtpButtonThemeOffice2000);
	m_restoreDefaultsBtn.SetTheme(xtpButtonThemeOffice2000);

	m_imgList.Create(16,16, ILC_COLOR32 , 1, 0);

	CBitmap bmpImage;
	bmpImage.LoadBitmap(IDB_IO_INPUT); 
	m_imgList.Add(&bmpImage, (COLORREF)NULL);
	bmpImage.DeleteObject();

	bmpImage.LoadBitmap(IDB_IO_OUTPUT); 
	m_imgList.Add(&bmpImage, (COLORREF)NULL);
	bmpImage.DeleteObject();

	bmpImage.LoadBitmap(IDB_IO_ERROR); 
	m_imgList.Add(&bmpImage, (COLORREF)NULL);
	bmpImage.DeleteObject();

	// Make the necessary initializations
	m_GPIOListCtrl.InsertColumn(0, ""     , LVCFMT_CENTER, 20);
	m_GPIOListCtrl.InsertColumn(1, "GPIO#", LVCFMT_CENTER, 50);
	m_GPIOListCtrl.InsertColumn(2, "Name" , LVCFMT_CENTER, 100);
	m_GPIOListCtrl.InsertColumn(3, "I/O"  , LVCFMT_CENTER, 90);
	m_GPIOListCtrl.InsertColumn(4, "State", LVCFMT_CENTER, 90);
	
	m_GPIOListCtrl.SetComboColumns(3);
	m_GPIOListCtrl.SetComboColumns(4);
	m_GPIOListCtrl.EnableVScroll(); 
	m_GPIOListCtrl.SetImageList(&m_imgList,LVSIL_SMALL);

	m_GPIOListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES); 

	int nIO,nIOCount, nItem = 0;
	nIOCount = m_pGPIOUnit->GetGPIOCount();
	for(nIO = 0; nIO < nIOCount; nIO++)
	{
		CString					            sIndex,sName;
		CSUB20HwModuleCtrlGPIOUnitIOItem*   pIO;

		pIO = m_pGPIOUnit->GetGPIO(nIO);
		if(pIO->GetEnable() == FALSE)
			continue;

		m_GPIOListCtrl.InsertItem(LVIF_TEXT,nItem,"",0,0,-1,NULL);

		sIndex.Format("%d",pIO->GetIndex());
		m_GPIOListCtrl.SetItemText(nItem,1,sIndex);
		if(SetItem(nItem,pIO) == FALSE)
		{
			CString sGPIO;
			sGPIO.Format("#%d, Name = %s",pIO->GetIndex(),pIO->GetDisplayName());
			MessageBox("Failed to update GPIO " +sGPIO+ " fields!",NULL,MB_ICONERROR);
			continue;
		}
		nItem++;
	}
	m_GPIOListCtrl.SetReadOnlyColumns(0);
	m_GPIOListCtrl.SetReadOnlyColumns(1);
	m_GPIOListCtrl.SetReadOnlyColumns(3);
	m_GPIOListCtrl.SetReadOnlyColumns(4);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CSUB20HwModuleCtrlGPIOUnitBasicDlg::SetItem(int nItem, CSUB20HwModuleCtrlGPIOUnitIOItem* pGPIO)
{
	int		nState, nType,nGPIO;
	CString	sName;

	nGPIO = pGPIO->GetIndex();

	if(pGPIO->GetState(nState) == FALSE)
		return FALSE;

	if(pGPIO->GetType(nType) == FALSE)
		return FALSE;

	sName = pGPIO->GetDisplayName();
	SetItemState(nItem,nState);
	SetItemType(nItem,nType);
	SetItemName(nItem,sName);
	m_GPIOListCtrl.SetItemData(nItem,(DWORD_PTR)pGPIO);

	return TRUE;
}

BOOL CSUB20HwModuleCtrlGPIOUnitBasicDlg::SetItemName(int nRow, CString sName)
{
	LVITEM	lvi;

	lvi.mask     =  LVIF_TEXT;
	lvi.iItem    = nRow;
	lvi.iSubItem = GPIO_NAME_COL;
	lvi.pszText  = (LPTSTR)(LPCTSTR)sName;
	m_GPIOListCtrl.SetItem(&lvi);
	return TRUE;
}

BOOL CSUB20HwModuleCtrlGPIOUnitBasicDlg::SetItemType(int nRow, int nType)
{
	LVITEM	lvi;
	CString	sType;
	int	    nImage;

	lvi.mask     =  LVIF_TEXT;
	lvi.iItem    = nRow;
	lvi.iSubItem = GPIO_TYPE_COL;

	switch(nType)
	{
        case 0:
			sType  = "Input";
			nImage = 0;
			break;
		case 1:
			sType  = "Output";
			nImage = 1;
			break;
		default:
			sType  = "Error";
			nImage = 3;
			break;
	}

	lvi.pszText = (LPTSTR)(LPCTSTR)sType;
	m_GPIOListCtrl.SetItem(&lvi);

	//Set image
	lvi.mask     = LVIF_IMAGE;
	lvi.iItem    = nRow;
	lvi.iSubItem = 0;
	lvi.iImage   = nImage;
    m_GPIOListCtrl.SetItem(&lvi);
	return TRUE;
}

BOOL CSUB20HwModuleCtrlGPIOUnitBasicDlg::SetItemState(int nRow, int nState)
{
	LVITEM	lvi;
	CString	sState;

	lvi.mask     =  LVIF_TEXT;
	lvi.iItem    = nRow;
	lvi.iSubItem = GPIO_STATE_COL;
	switch(nState)
	{
	case 1:
		sState  = "High";
		break;
	case 0:
		sState  = "Low";
		break;
	default:
		sState  = "Error";
		break;
	}

	lvi.pszText = (LPTSTR)(LPCTSTR)sState;
	m_GPIOListCtrl.SetItem(&lvi);

	return TRUE;
}


LRESULT CSUB20HwModuleCtrlGPIOUnitBasicDlg::OnComboStart(WPARAM wParam, LPARAM lParam)
{
	int nRow = (int)wParam, nCol =  (int)lParam;

	if( nCol == GPIO_STATE_COL)
	{
		if(m_GPIOListCtrl.GetItemText(nRow,GPIO_TYPE_COL) == "Input")
		{
			//Don't open combo --> read only
			return 0;
		}
	}
	return 1;
}

LRESULT CSUB20HwModuleCtrlGPIOUnitBasicDlg::OnEditNameEnd(WPARAM wParam, LPARAM lParam)
{	
	int								  nRow = (int)wParam;
	CSUB20HwModuleCtrlGPIOUnitIOItem *pItem;

	pItem = (CSUB20HwModuleCtrlGPIOUnitIOItem*)m_GPIOListCtrl.GetItemData(nRow);
	CString sName = m_GPIOListCtrl.GetItemText(nRow,GPIO_NAME_COL);
	pItem->SetDisplayName(sName);

	return 0;
}

LRESULT CSUB20HwModuleCtrlGPIOUnitBasicDlg::OnComboSelectionEnd(WPARAM wParam, LPARAM lParam)
{
	int								  nRow = (int)wParam;
	CSUB20HwModuleCtrlGPIOUnitIOItem *pItem;

	pItem = (CSUB20HwModuleCtrlGPIOUnitIOItem*)m_GPIOListCtrl.GetItemData(nRow);
	
	CString sType = m_GPIOListCtrl.GetItemText(nRow,GPIO_TYPE_COL);

	if(sType == "Input")
	{
		SetItemType(pItem->GetIndex(),GPIO_TYPE_INPUT);
	}
	if(sType == "Output")
	{
		SetItemType(pItem->GetIndex(),GPIO_TYPE_OUTPUT);
	}
	return 0;
}

LRESULT CSUB20HwModuleCtrlGPIOUnitBasicDlg::PopulateComboList(WPARAM wParam, LPARAM lParam)
{
	// Get the Combobox window pointer
	CComboBox* pInPlaceCombo = static_cast<CComboBox*> (GetFocus());

	// Get the inplace combbox top left
	CRect obWindowRect;
	pInPlaceCombo->GetWindowRect(&obWindowRect);
	CPoint obInPlaceComboTopLeft(obWindowRect.TopLeft()); 
	// Get the active list
	// Get the control window rect
	// If the inplace combobox top left is in the rect then
	// The control is the active control
	m_GPIOListCtrl.GetWindowRect(&obWindowRect);
	
	int iColIndex = (int)wParam;
	
	CStringList* pComboList = reinterpret_cast<CStringList*>(lParam);
	pComboList->RemoveAll(); 

	if (obWindowRect.PtInRect(obInPlaceComboTopLeft)) 
	{
		if (GPIO_TYPE_COL == iColIndex)
		{
			pComboList->AddTail("Input");
			pComboList->AddTail("Output");
		}
		if (GPIO_STATE_COL == iColIndex)
		{
			pComboList->AddTail("High");
			pComboList->AddTail("Low");
		}
	}
	return true;
}

void CSUB20HwModuleCtrlGPIOUnitBasicDlg::OnBnClickedGpioUnitUpdateSelectedBtn()
{
	int nItem = -1;

	while((nItem = m_GPIOListCtrl.GetNextItem( nItem, LVNI_SELECTED )) != -1)
	{
		UpdateItem(nItem);
	}
}

void CSUB20HwModuleCtrlGPIOUnitBasicDlg::UpdateItem(int nItem)
{
	CSUB20HwModuleCtrlGPIOUnitIOItem *pItem;

	CString sRequestedState, sRequestedType;
	CString sCurrentType;
	int		nRequestedState, nRequestedType;
	int		nCurrentState, nCurrentType;

	sRequestedState = m_GPIOListCtrl.GetItemText(nItem,GPIO_STATE_COL);
	sRequestedType   = m_GPIOListCtrl.GetItemText(nItem,GPIO_TYPE_COL);
	pItem            = (CSUB20HwModuleCtrlGPIOUnitIOItem*)m_GPIOListCtrl.GetItemData(nItem);

	if(pItem->GetType(nCurrentType) == FALSE)
	{
		MessageBox("Failed to get GPIO current type!",NULL,MB_ICONERROR);
		return;
	}
	sCurrentType = TypeToString(nCurrentType);
	//Set the type first
	if(sCurrentType != sRequestedType)
	{
		if((nRequestedType = StringTypeToInt(sRequestedType)) <= GPIO_TYPE_OUTPUT)
		{
			if(pItem->SetType(nRequestedType) == FALSE)
			{
				MessageBox("Failed to set GPIO type!",NULL,MB_ICONERROR);
				return;
			}
		}
	}
	if(sRequestedType == "Input")
	{
		if(pItem->GetState(nCurrentState) == FALSE)
		{
			MessageBox("Failed to set GPIO type!",NULL,MB_ICONERROR);
			return;
		}
		//Update GUI with read value
		SetItemState(nItem,nCurrentState);
	}
	if(sRequestedType == "Output")
	{
		nRequestedState = StringStateToInt(sRequestedState);
		//Update with read value
		if(pItem->SetState(nRequestedState) == FALSE)
		{
			MessageBox("Failed to get GPIO state!",NULL,MB_ICONERROR);
			return;
		}
	}
}

void CSUB20HwModuleCtrlGPIOUnitBasicDlg::OnBnClickedGpioUnitUpdateAllBtn()
{
	int nGPIO,nGPIOCount;

	nGPIOCount = m_pGPIOUnit->GetGPIOCount();
	for(nGPIO = 0; nGPIO < nGPIOCount; nGPIO++)
	{
		UpdateItem(nGPIO);
	}
}

void CSUB20HwModuleCtrlGPIOUnitBasicDlg::OnBnClickedCancel()
{
	SavePlacement(SECTION);
	CDialog::OnCancel();
}

int CSUB20HwModuleCtrlGPIOUnitBasicDlg::StringTypeToInt(CString sType)
{
	int nType = 100;

	if(sType == "Input")
	{
		nType = GPIO_TYPE_INPUT;
	}
	if(sType == "Output")
	{
		nType = GPIO_TYPE_OUTPUT;
	}
	return nType;
}

int CSUB20HwModuleCtrlGPIOUnitBasicDlg::StringStateToInt(CString sState)
{
	int nState = 100;

	if(sState == "Low")
	{
		nState = GPIO_STATE_LOW;
	}
	if(sState == "High")
	{
		nState = GPIO_STATE_HIGH;
	}
	return nState;
}
CString CSUB20HwModuleCtrlGPIOUnitBasicDlg::TypeToString(int nType)
{
	CString sType;
	switch(nType)
	{
	case GPIO_TYPE_INPUT:
		sType = "Input";
		break;
	case GPIO_TYPE_OUTPUT:
		sType = "Output";
		break;
	default:
		sType = "Error";
		break;
	}
	return sType;
}
CString CSUB20HwModuleCtrlGPIOUnitBasicDlg::StateToString(int nState)
{
	CString sState;

	switch(nState)
	{
	case GPIO_STATE_LOW:
		sState = "Low";
		break;
	case GPIO_STATE_HIGH:
		sState = "High";
		break;
	default:
		sState = "Error";
		break;
	}
	return sState;
}

void CSUB20HwModuleCtrlGPIOUnitBasicDlg::OnBnClickedGpioUnitRestoreDefaultsBtn()
{
	int nIO,nIOCount, nItem = 0;

	m_pGPIOUnit->SetDefaultGPIOMapping();

	nIOCount = m_pGPIOUnit->GetGPIOCount();
	for(nIO = 0; nIO < nIOCount; nIO++)
	{
		CString					            sIndex,sName;
		CSUB20HwModuleCtrlGPIOUnitIOItem*   pIO;

		pIO = m_pGPIOUnit->GetGPIO(nIO);
		if(pIO->GetEnable() == FALSE)
			continue;

		sIndex.Format("%d",pIO->GetIndex());
		m_GPIOListCtrl.SetItemText(nItem,1,sIndex);
		nItem++;

		if(SetItem(nItem,pIO) == FALSE)
		{
			CString sGPIO;
			sGPIO.Format("#%d, Name = %s",pIO->GetIndex(),pIO->GetDisplayName());
			MessageBox("Failed to update GPIO " +sGPIO+ " fields!",NULL,MB_ICONERROR);
			continue;
		}
	}
}

