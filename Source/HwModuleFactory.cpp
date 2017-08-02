#include "StdAfx.h"
#include ".\hardwaremodulefactory.h"
#include "Mux9544Module.h"
#include "Mux9548Module.h"
#include "ODPGHwModule.h"
#include "UnknownI2CHwModule.h"
#include "SummitSMM205HwModule.h"
#include "ClockETPHwModule.h"
#include "MarvellDeviceHwModule.h"
#include "MarvellDeviceSysProfilerHwModule.h"
#include "Max6699TempSensorHwModule.h"


CHwModuleFactory::CHwModuleFactory(void)
{
	CHwModule   *pModule;

	/////////////////// Non mux items ///////////////////////
	pModule = new CMux9544Module();
	m_hwModules[pModule->GetName()] = pModule;

	pModule = new CMux9548Module();
	m_hwModules[pModule->GetName()] = pModule;

	pModule = new CODPGHwModule();
	m_hwModules[pModule->GetName()] = pModule;

	pModule = new CSummitSMM205HwModule();
	m_hwModules[pModule->GetName()] = pModule;

	pModule = new CMax6699TempSensorHwModule();
	m_hwModules[pModule->GetName()] = pModule;

	pModule = new CUnknownI2CHwModule();
	m_hwModules[pModule->GetName()] = pModule;

	pModule = new CClockETPHwModule();
	m_hwModules[pModule->GetName()] = pModule;

	pModule = new CMarvellDeviceHwModule();
	m_hwModules[pModule->GetName()] = pModule;

	pModule = new CMarvellDeviceSysProfilerHwModule();
	m_hwModules[pModule->GetName()] = pModule;

}

CHwModuleFactory::~CHwModuleFactory(void)
{
	map<CString,CHwModule*>::iterator iter, endIter;

	iter    = m_hwModules.begin();
	endIter = m_hwModules.end();

	for(;iter != endIter; iter++)
	{
		delete (*iter).second;
	}
}

CHwModuleFactory& CHwModuleFactory::GetInstance()
{
	static CHwModuleFactory obj;
	return obj;
}

CHwModule* CHwModuleFactory::Create(CHwModuleInfo* pInfo)
{
	map<CString,CHwModule*>::iterator iter;
	
	if(pInfo == NULL)
		return NULL;

	iter = m_hwModules.find(pInfo->GetName());
	if(iter != m_hwModules.end())
	{
		CHwModule* pModule = (CHwModule*)(iter->second->Clone(pInfo));
		return pModule;
	}
	return NULL;
}

int	CHwModuleFactory::GetPrototypeCount()
{
	return (int)m_hwModules.size();
}

CString  	CHwModuleFactory::GetPrototypeName(int i)
{
	int										 nCount = 0;
	map<CString,CHwModule*>::iterator iter, endIter;  m_hwModules;

	iter    = m_hwModules.begin();
	endIter = m_hwModules.end();

	for(;iter != endIter; iter++)
	{
		if(nCount == i)
			return (*iter).second->GetName();
		nCount++;
	}
	return CString("");
}



