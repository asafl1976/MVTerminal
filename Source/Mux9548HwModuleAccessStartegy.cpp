#include "StdAfx.h"
#include ".\mux9548hwmoduleaccessstartegy.h"

CMux9548HwModuleAccessStartegy::CMux9548HwModuleAccessStartegy(CHwModule *pHWModule, CHwModuleCtrlI2CUnitDrv *pI2CUnitDrv)
:CMuxHwModuleAccessStartegy(pHWModule)
{
	m_pI2CUnitDrv = pI2CUnitDrv;
}

CMux9548HwModuleAccessStartegy::~CMux9548HwModuleAccessStartegy(void)
{
}

BOOL  CMux9548HwModuleAccessStartegy::SelectChannel(int nChannel)
{
	UCHAR	data;
	UINT 	nTwsiAddress;
	CString sAddress;

	sAddress = m_pHwModule->GetID();
	_stscanf(sAddress,_T("%x"),&nTwsiAddress);
	data = 0x1 << nChannel;
	return m_pI2CUnitDrv->WriteData((UCHAR)nTwsiAddress,1,&data);;
}

BOOL  CMux9548HwModuleAccessStartegy::DeSelectChannels()
{
	UCHAR	data;
	UINT  	nTwsiAddress;
	CString sAddress;

	sAddress = m_pHwModule->GetID();
	_stscanf(sAddress,_T("%x"),&nTwsiAddress);
	data = 0;
	return m_pI2CUnitDrv->WriteData((UCHAR)nTwsiAddress,1,&data);
}
