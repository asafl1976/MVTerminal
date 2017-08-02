#include "StdAfx.h"
#include "USB2I2CHwModuleCtrlI2CUnitDrv.h"

CUSB2I2CHwModuleCtrlI2CUnitDrv::CUSB2I2CHwModuleCtrlI2CUnitDrv(CUSB2I2CHwModuleCtrlDrv *pDrv)
{
	m_pDrv = pDrv;
}

CUSB2I2CHwModuleCtrlI2CUnitDrv::~CUSB2I2CHwModuleCtrlI2CUnitDrv(void)
{
}
BOOL	CUSB2I2CHwModuleCtrlI2CUnitDrv::WriteData(UCHAR nAddress, UCHAR nSubAddress, int nBytes, UCHAR *pWriteData)
{
	return m_pDrv->WriteData(nAddress,nSubAddress,nBytes,pWriteData);
}
BOOL    CUSB2I2CHwModuleCtrlI2CUnitDrv::ReadData(UCHAR nAddress, UCHAR nSubAddress, int nBytes, UCHAR *pReadData)
{
	return m_pDrv->ReadData(nAddress,nSubAddress,nBytes,pReadData);
}
BOOL    CUSB2I2CHwModuleCtrlI2CUnitDrv::WriteData(UCHAR nAddress, int nBytes, UCHAR *pWriteData, BOOL bSendStopBit)
{
	return m_pDrv->WriteData(nAddress,nBytes,pWriteData,bSendStopBit);
}
BOOL    CUSB2I2CHwModuleCtrlI2CUnitDrv::ReadData(UCHAR nAddress, int nBytes, UCHAR *pReadData, BOOL bSendStopBit)
{
	return m_pDrv->ReadData(nAddress,nBytes,pReadData,bSendStopBit);
}
