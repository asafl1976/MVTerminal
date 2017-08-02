#include "StdAfx.h"
#include "HwModuleCtrlI2CUnit.h"
#include "HwModuleCtrl.h"
#include "UnknownI2CHWModule.h"
#include "HwModule.h"
#include "HwModuleFactory.h"
#include "HwModuleModel.h"


CHwModuleCtrlI2CUnit::CHwModuleCtrlI2CUnit(CHwModuleCtrl *pHwModuleCtrl,
										   CHwModuleCtrlI2CUnitDrv* pI2CUnitDrv):
CHwModuleCtrlUnit(pHwModuleCtrl)
{
	m_pI2CUnitDrv  = pI2CUnitDrv;
}

CHwModuleCtrlI2CUnit::~CHwModuleCtrlI2CUnit(void)
{
	//m_pI2CUnitDrv already deleted in CxxxxxxHwModuleCtrl 

}

void   CHwModuleCtrlI2CUnit::NotifyCreateHwModule(CHwModule* pHwModule)
{
	CHwModuleModel::GetInstance().NotifyCreateHwModuleItem(pHwModule,this);
}

BOOL   CHwModuleCtrlI2CUnit::SetFrequency(int nFreq)
{
	return m_pI2CUnitDrv->SetFrequency(nFreq);
}

int   CHwModuleCtrlI2CUnit::GetFrequency()
{
	return m_pI2CUnitDrv->GetFrequency();
}


BOOL  CHwModuleCtrlI2CUnit::DetectHwModules()
{
	vector<CHwModuleInfo*> HwModuleInfo;

	if( DetectHwModules(HwModuleInfo,this) == FALSE )
	{
		return FALSE;
	}
	if( CreateHwModules(HwModuleInfo) == FALSE)
	{
		return FALSE;
	}
	//Detect Hw Modules under the modules (mux) that were just found
	return IHwModuleContainer::DetectHwModules();
}

BOOL  CHwModuleCtrlI2CUnit::DetectHwModules(vector<CHwModuleInfo*>& HwModuleInfo,CHwModuleItem* pOwner)
{
	m_pParent->EnableDriverErrorMessages(FALSE);
	//Detect local hw module only
	for(UCHAR nTwsiAddress = 5; nTwsiAddress < 128; nTwsiAddress++)
	{
		UCHAR   nByteData;	
		CString	sAddress;

		if(m_pI2CUnitDrv->ReadData(nTwsiAddress,1,&nByteData) == TRUE)
		{
			sAddress.Format("%x",nTwsiAddress);
			CHwModuleInfo *pHWInfo = new CHwModuleInfo("Unknown (I2C)",sAddress,HWM_IF_I2C,this,pOwner);
			HwModuleInfo.push_back(pHWInfo);
		}
		else
		{
			//Some devices response to write...
			nByteData = 0;
			if(m_pI2CUnitDrv->WriteData(nTwsiAddress,1,&nByteData) == TRUE)
			{
				sAddress.Format("%x",nTwsiAddress);
				CHwModuleInfo *pHWInfo = new CHwModuleInfo("Unknown (I2C)",sAddress,HWM_IF_I2C,this,pOwner);
				HwModuleInfo.push_back(pHWInfo);
			}
	
		}
	}
	m_pParent->EnableDriverErrorMessages(TRUE);
	return TRUE;
}

CString	CHwModuleCtrlI2CUnit::Serialize()
{
	CMarkup xmlWriter;

	xmlWriter.AddElem( "HwModuleCtrlUnit");
	xmlWriter.AddAttrib( "Name",GetName());
	xmlWriter.AddAttrib( "Freq",m_pI2CUnitDrv->GetFrequency());
	xmlWriter.IntoElem();

	xmlWriter.AddSubDoc( IHwModuleContainer::Serialize() );

	xmlWriter.OutOfElem();
	return xmlWriter.GetDoc();

}

BOOL  CHwModuleCtrlI2CUnit::DeSerialize(CMarkup& xmlReader)
{
	CString sFreq;
	sFreq = xmlReader.GetAttrib( "Freq");
	if(sFreq.IsEmpty() == FALSE)
	{
		int nFreq;
		sscanf(sFreq,"%d",&nFreq);
		m_pI2CUnitDrv->SetFrequency(nFreq);
	}

	if(CHwModuleModel::GetInstance().GetDetectHwModulesOnStartup() == TRUE)
	{
		if(DetectHwModules() == FALSE)
		{
			return FALSE;
		}
	}

	xmlReader.IntoElem();

	if(IHwModuleContainer::DeSerialize(xmlReader) == FALSE)
		return FALSE;

	xmlReader.OutOfElem();
	return TRUE;
}

CHwModuleItem*  CHwModuleCtrlI2CUnit::GetSubHwModuleItem(list<CHwModuleItem>& ownerList) 
{
	return IHwModuleContainer::GetSubHwModuleItem(ownerList);
}




