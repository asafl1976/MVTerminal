#include "StdAfx.h"
#include "SUB20HwModuleCtrlDrv.h"
#include "SUB20HwModuleCtrlGPIOUnit.h"
#include "HwModuleModel.h"
#include "SUB20HwModuleCtrlGPIOUnitIOItem.h"

CSUB20HwModuleCtrlGPIOUnit::CSUB20HwModuleCtrlGPIOUnit(CHwModuleCtrl *pHwModuleCtrl,CHwModuleCtrlGPIOUnitDrv* pGPIOUnitDrv)
:CHwModuleCtrlGPIOUnit(pHwModuleCtrl,pGPIOUnitDrv)
{
	m_sName		   = "SUB-20 GPIO";
	m_sDisplayName = "GPIO";

	for(int i = 0; i < 32; i++)
	{
		m_GPIOs.push_back(new CSUB20HwModuleCtrlGPIOUnitIOItem(i,this));
	}
	CHwModuleModel::GetInstance().NotifyCreateHwModuleItem(this,pHwModuleCtrl);

	CString sDisplayName;

	// Create Default GPIO setting map
	for(int nLcdD = 0; nLcdD < 4; nLcdD++)
	{
		sDisplayName.Format("LCD_D%d",nLcdD);
		m_defaultGPIOConfigMap[nLcdD] = new CGPIOConfig(sDisplayName,0,GPIO_TYPE_OUTPUT);
	}
	m_defaultGPIOConfigMap[4] = new CGPIOConfig(NULL,0,GPIO_TYPE_INPUT);
	m_defaultGPIOConfigMap[5] = new CGPIOConfig(NULL,0,GPIO_TYPE_INPUT);
	m_defaultGPIOConfigMap[6] = new CGPIOConfig("LCD_RS",0,GPIO_TYPE_OUTPUT);
	m_defaultGPIOConfigMap[7] = new CGPIOConfig("LCD_En",0,GPIO_TYPE_OUTPUT);
	m_defaultGPIOConfigMap[8] = new CGPIOConfig("I2C_SCL",0,GPIO_TYPE_INPUT);
	m_defaultGPIOConfigMap[9] = new CGPIOConfig("I2C_SDA",0,GPIO_TYPE_INPUT);
	m_defaultGPIOConfigMap[10] = new CGPIOConfig(NULL,0,GPIO_TYPE_INPUT);
	m_defaultGPIOConfigMap[11] = new CGPIOConfig(NULL,0,GPIO_TYPE_INPUT);
	m_defaultGPIOConfigMap[12] = new CGPIOConfig("Reset",0,GPIO_TYPE_INPUT);
	m_defaultGPIOConfigMap[13] = new CGPIOConfig("Power_On",0,GPIO_TYPE_INPUT);
	m_defaultGPIOConfigMap[14] = new CGPIOConfig("MDC",0,GPIO_TYPE_OUTPUT);
	m_defaultGPIOConfigMap[15] = new CGPIOConfig("MDIO",0,GPIO_TYPE_OUTPUT);
	for(int nADC = 0; nADC < 8; nADC++)
	{
		// Remove ADC lines from list (requested by motyf) == FALSE
		sDisplayName.Format("ADC%d",nADC);
		m_defaultGPIOConfigMap[16 + nADC] = new CGPIOConfig(sDisplayName,0,GPIO_TYPE_INPUT, FALSE);
	}
	for(int nPWM = 0; nPWM < 8; nPWM++)
	{
		sDisplayName.Format("PWM_%d",nPWM);
		m_defaultGPIOConfigMap[24 + nPWM] = new CGPIOConfig(sDisplayName,0,GPIO_TYPE_OUTPUT);
	}
}

CSUB20HwModuleCtrlGPIOUnit::~CSUB20HwModuleCtrlGPIOUnit(void)
{
	vector<CSUB20HwModuleCtrlGPIOUnitIOItem*>::iterator iter, endIter;

	iter    = m_GPIOs.begin();
	endIter = m_GPIOs.end();

	//Search for the item
	for(;iter != endIter;iter++)
	{
		delete (*iter);
	}
	m_GPIOs.clear();

	map<int,CGPIOConfig*>::iterator mapIter, mapEndIter;   

	mapIter    = m_defaultGPIOConfigMap.begin();
	mapEndIter = m_defaultGPIOConfigMap.end();

	for(;mapIter != mapEndIter;mapIter++)
	{
		delete (mapIter->second);
	}
	m_defaultGPIOConfigMap.clear();

}

int CSUB20HwModuleCtrlGPIOUnit::GetGPIOCount()
{
	return (int)m_GPIOs.size();
}

CSUB20HwModuleCtrlGPIOUnitIOItem* CSUB20HwModuleCtrlGPIOUnit::GetGPIO(int nIndex)
{
	if(nIndex >= (int)m_GPIOs.size())
	{
		return NULL;
	}
	return m_GPIOs[nIndex];
}

BOOL CSUB20HwModuleCtrlGPIOUnit::Init()
{
	return SetDefaultGPIOMapping();
}

BOOL CSUB20HwModuleCtrlGPIOUnit::SetDefaultGPIOMapping()
{
	int			nIO,nIOCount;

	//Init all GPIOs with default settings
	nIOCount = GetGPIOCount();
	for(nIO = 0; nIO < nIOCount; nIO++)
	{
		CSUB20HwModuleCtrlGPIOUnitIOItem*   pGPIO;

		pGPIO = GetGPIO(nIO);
		if(pGPIO->SetConfig(*m_defaultGPIOConfigMap[nIO]) == FALSE)
		{
			return FALSE;
		}
	}
	return TRUE;
}

CString	CSUB20HwModuleCtrlGPIOUnit::Serialize()
{
	CMarkup xmlWriter;

	xmlWriter.AddElem( "HwModuleCtrlUnit");
	xmlWriter.AddAttrib( "Name",GetName());
	xmlWriter.IntoElem();

	xmlWriter.AddElem( "GPIOS");
	xmlWriter.IntoElem();

	int nIO,nIOCount;
	nIOCount = GetGPIOCount();
	for(nIO = 0; nIO < nIOCount; nIO++)
	{
		CSUB20HwModuleCtrlGPIOUnitIOItem*   pGPIO;
		pGPIO = GetGPIO(nIO);
		xmlWriter.AddSubDoc(pGPIO->Serialize());
	}
	xmlWriter.OutOfElem();
	xmlWriter.OutOfElem();
	return xmlWriter.GetDoc();
}

BOOL  CSUB20HwModuleCtrlGPIOUnit::DeSerialize(CMarkup& xmlReader)
{
	int							      nIndex;
	CSUB20HwModuleCtrlGPIOUnitIOItem* pGPIO;

	xmlReader.IntoElem();

	if(xmlReader.FindElem("GPIOS") == false)
		return FALSE;

	xmlReader.IntoElem();
    while ( xmlReader.FindElem("GPIO") )
	{
		CString		sIndex;

		sIndex       = xmlReader.GetAttrib("Index");
		sscanf((LPCSTR)sIndex,"%d",&nIndex);
		if((pGPIO = GetGPIO(nIndex)) == NULL)
		{
			//Notify user of replace failure
			AfxMessageBox("Error while loading GPIO item!",NULL,MB_ICONWARNING);
			continue;
		}
		if(pGPIO->DeSerialize(xmlReader) == FALSE)
		{
			return FALSE;
		}
	}
	xmlReader.OutOfElem();
	xmlReader.OutOfElem();
	return TRUE;
}

CHwModuleItem*   CSUB20HwModuleCtrlGPIOUnit::GetSubHwModuleItem(list<CHwModuleItem>& ownerList) 
{
	vector<CSUB20HwModuleCtrlGPIOUnitIOItem*>::iterator iter, endIter;
	list<CHwModuleItem>::iterator        item;

	//Get the first element
	item = ownerList.begin();
	if(item == ownerList.end())
		return NULL;

	iter    = m_GPIOs.begin();
	endIter = m_GPIOs.end();

	//Search for the item
	for(;iter != endIter;iter++)
	{
		if(((*iter)->GetID() == (*item).GetID()) && ((*iter)->GetName() == (*item).GetName()))
		{
			//Remove the item from list
			ownerList.erase(item);
			
			if(ownerList.empty() == true)
				return (*iter);
		}
	}
	return NULL;
}

CHwModuleCtrlUnitItem* CSUB20HwModuleCtrlGPIOUnit::GetItem(CString sName)
{
	vector<CSUB20HwModuleCtrlGPIOUnitIOItem*>::iterator iter, endIter;
	
	iter    = m_GPIOs.begin();
	endIter = m_GPIOs.end();

	for(; iter != endIter; iter++)
	{
		if((*iter)->GetDisplayName() == sName)
		{
			return (*iter);
		}
	}

	return NULL;
}


