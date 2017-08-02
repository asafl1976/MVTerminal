#include "StdAfx.h"
#include "LPT2I2CHwModuleCtrlI2CUnitDrv.h"
#include "LPT2I2CHwModuleCtrlDrv.h"

CLPT2I2CHwModuleCtrlI2CUnitDrv::CLPT2I2CHwModuleCtrlI2CUnitDrv(CLPT2I2CHwModuleCtrlDrv *pDrv)
{
	m_pDrv = pDrv;
}

CLPT2I2CHwModuleCtrlI2CUnitDrv::~CLPT2I2CHwModuleCtrlI2CUnitDrv(void)
{
}
BOOL	CLPT2I2CHwModuleCtrlI2CUnitDrv::WriteData(UCHAR nAddress, UCHAR nSubAddress, int nBytes, UCHAR *pWriteData)
{
	return m_pDrv->WriteData(nAddress,nSubAddress,nBytes,pWriteData);
}
BOOL    CLPT2I2CHwModuleCtrlI2CUnitDrv::ReadData(UCHAR nAddress, UCHAR nSubAddress, int nBytes, UCHAR *pReadData)
{
	return m_pDrv->ReadData(nAddress,nSubAddress,nBytes,pReadData);
}
BOOL    CLPT2I2CHwModuleCtrlI2CUnitDrv::WriteData(UCHAR nAddress, int nBytes, UCHAR *pWriteData, BOOL bSendStopBit)
{
	return m_pDrv->WriteData(nAddress,nBytes,pWriteData,bSendStopBit);
}
BOOL    CLPT2I2CHwModuleCtrlI2CUnitDrv::ReadData(UCHAR nAddress, int nBytes, UCHAR *pReadData, BOOL bSendStopBit)
{
	return m_pDrv->ReadData(nAddress,nBytes,pReadData,bSendStopBit);
}
