#include "StdAfx.h"
#include ".\mux9544hwmoduleaccessstartegy.h"

CMux9544HwModuleAccessStartegy::CMux9544HwModuleAccessStartegy(CHwModule *pHWModule, CHwModuleCtrlI2CUnitDrv *pI2CUnitDrv)
:CMuxHwModuleAccessStartegy(pHWModule)
{
	m_pI2CUnitDrv = pI2CUnitDrv;
}

CMux9544HwModuleAccessStartegy::~CMux9544HwModuleAccessStartegy(void)
{
}

BOOL  CMux9544HwModuleAccessStartegy::SelectChannel(int nChannel)
{
	UCHAR	data;
	UINT 	nTwsiAddress;
	CString sAddress;

	sAddress = m_pHwModule->GetID();
	_stscanf(sAddress,_T("%x"),&nTwsiAddress);
	data = nChannel + 4;
	return m_pI2CUnitDrv->WriteData((UCHAR)nTwsiAddress,1,&data);;
}

BOOL  CMux9544HwModuleAccessStartegy::DeSelectChannels()
{
	UCHAR	data;
	UINT  	nTwsiAddress;
	CString sAddress;

	sAddress = m_pHwModule->GetID();
	_stscanf(sAddress,_T("%x"),&nTwsiAddress);
	data = 0;
	return m_pI2CUnitDrv->WriteData((UCHAR)nTwsiAddress,1,&data);
}
