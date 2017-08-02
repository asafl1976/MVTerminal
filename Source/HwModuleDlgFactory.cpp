#include "StdAfx.h"
#include "HwModuleDlgFactory.h"
#include "BasicItemDlg.h"
#include "IHwModuleItemDlg.h"
#include "ODPGHwModuleModelessDlg.h"
#include "SummitSMM205HwModuleModelessDlg.h"
#include "UnknownI2CHwModuleModelessDlg.h"
#include "SUB20HwModuleCtrlADCUnitVolItemModelessDlg.h"
#include "SUB20HwModuleCtrlADCUnitCurrItemModelessDlg.h"
#include "ClockETPHwModuleModelessDlg.h"
#include "SUB20HwModuleCtrlGPIOUnitModelessDlg.h"
#include "MarvellDeviceSysProfilerHwModuleModelessDlg.h"
#include "Max6699TempSensorHwModuleModelessDlg.h"
#include "MarvellDeviceHwModuleModelessDlg.h"

CHwModuleItemDlgFactory::CHwModuleItemDlgFactory(void)
{
	m_hwModuleItemDlg["ODPG"]           = new CODPGHwModuleModelessDlg();
	m_hwModuleItemDlg["Summit SMM205"]  = new CSummitSMM205HwModuleModelessDlg();
	m_hwModuleItemDlg["Clock ETP"]      = new CClockETPHwModuleModelessDlg();
	m_hwModuleItemDlg["Unknown (I2C)"]  = new CUnknownI2CHwModuleModelessDlg();
	m_hwModuleItemDlg["Marvell Device"] = new CMarvellDeviceHwModuleModelessDlg();
	m_hwModuleItemDlg["System Profiler"] = new CMarvellDeviceSysProfilerHwModuleModelessDlg();
	m_hwModuleItemDlg["Temperature Sensor (MAX6699)"] = new CMax6699TempSensorHwModuleModelessDlg();
	m_hwModuleItemDlg["ADC Voltage"]    = new CSUB20HwModuleCtrlADCUnitVolItemModelessDlg();
	m_hwModuleItemDlg["ADC Current"]    = new CSUB20HwModuleCtrlADCUnitCurrItemModelessDlg();
	m_hwModuleItemDlg["SUB-20 GPIO"]    = new CSUB20HwModuleCtrlGPIOUnitModelessDlg();
}

CHwModuleItemDlgFactory::~CHwModuleItemDlgFactory(void)
{
	map<CString,CBasicItemDlg*>::iterator iter, endIter;

	iter    = m_hwModuleItemDlg.begin();
	endIter = m_hwModuleItemDlg.end();

	for(;iter != endIter; iter++)
	{
		delete (*iter).second;
	}
}

CHwModuleItemDlgFactory& CHwModuleItemDlgFactory::GetInstance()
{
	static CHwModuleItemDlgFactory obj;
	return obj;
}

CBasicItemDlg* CHwModuleItemDlgFactory::Create(CHwModuleItem* pItem)
{
	map<CString,CBasicItemDlg*>::iterator iter;
	CBasicItemDlg						 *pDlg;
	IHwModuleItemDlg					 *pHwModuleItemDlg;
	
	iter = m_hwModuleItemDlg.find(pItem->GetName());
	if(iter != m_hwModuleItemDlg.end())
	{
		pDlg = (iter->second)->Clone();
		pHwModuleItemDlg = dynamic_cast<IHwModuleItemDlg*>(pDlg);
		if(pHwModuleItemDlg != NULL)
		{
			pHwModuleItemDlg->SetHwModuleItem(pItem);
			return pDlg;
		}
		delete pDlg;
	}
	return NULL;
}



