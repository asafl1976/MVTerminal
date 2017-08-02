#include "StdAfx.h"
#include "SUB20HwModuleCtrlI2CUnitDrv.h"
#include "SUB20HwModuleCtrlDrv.h"

CSUB20HwModuleCtrlI2CUnitDrv::CSUB20HwModuleCtrlI2CUnitDrv(CSUB20HwModuleCtrlDrv *pDrv)
{
	m_pDrv = pDrv;
}

CSUB20HwModuleCtrlI2CUnitDrv::~CSUB20HwModuleCtrlI2CUnitDrv(void)
{
}

BOOL  CSUB20HwModuleCtrlI2CUnitDrv::SetFrequency(int nFreq)
{
	//todo: get unit index
	return m_pDrv->I2CSetFrequency(&nFreq);
}

int  CSUB20HwModuleCtrlI2CUnitDrv::GetFrequency()
{
	return m_pDrv->I2CGetFrequency();
}

BOOL  CSUB20HwModuleCtrlI2CUnitDrv::HighSpeedRW(struct sub_i2c_hs_xfer* px, int nCount)
{
	return m_pDrv->I2CHighSpeedRW(px,nCount);
}

BOOL  CSUB20HwModuleCtrlI2CUnitDrv::WriteData(UCHAR nDevAddr, UCHAR nDevSubAddress, int nBytes, UCHAR *pWriteData)
{
	return m_pDrv->I2CWriteData(nDevAddr,nDevSubAddress,nBytes,pWriteData);
}

BOOL   CSUB20HwModuleCtrlI2CUnitDrv::ReadData(UCHAR  nDevAddr, UCHAR nDevSubAddress, int nBytes, UCHAR *pReadData)
{
	return m_pDrv->I2CReadData(nDevAddr,nDevSubAddress,nBytes,pReadData);
}

BOOL   CSUB20HwModuleCtrlI2CUnitDrv::WriteData(UCHAR nDevAddr, int nBytes, UCHAR *pWriteData, BOOL bSendStopBit)
{
	return m_pDrv->I2CWriteData(nDevAddr,nBytes,pWriteData,bSendStopBit);
}

BOOL   CSUB20HwModuleCtrlI2CUnitDrv::ReadData(UCHAR nDevAddr, int nBytes, UCHAR *pReadData, BOOL bSendStopBit)
{
	return m_pDrv->I2CReadData(nDevAddr,nBytes,pReadData,bSendStopBit);

}