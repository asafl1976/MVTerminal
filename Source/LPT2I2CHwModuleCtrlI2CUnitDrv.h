#pragma once
#include "HwModuleCtrlI2CUnitDrv.h"

class CLPT2I2CHwModuleCtrlDrv;

class CLPT2I2CHwModuleCtrlI2CUnitDrv : public CHwModuleCtrlI2CUnitDrv
{
	CLPT2I2CHwModuleCtrlDrv *m_pDrv;
public:
	CLPT2I2CHwModuleCtrlI2CUnitDrv(CLPT2I2CHwModuleCtrlDrv *pDrv);
	virtual ~CLPT2I2CHwModuleCtrlI2CUnitDrv(void);
	virtual BOOL	WriteData(UCHAR nAddress, UCHAR nSubAddress, int nBytes, UCHAR *pWriteData);
    virtual BOOL    ReadData(UCHAR nAddress, UCHAR nSubAddress, int nBytes, UCHAR *pReadData);
	virtual BOOL    WriteData(UCHAR nAddress, int nBytes, UCHAR *pWriteData, BOOL bSendStopBit = TRUE);
    virtual BOOL    ReadData(UCHAR nAddress, int nBytes, UCHAR *pReadData, BOOL bSendStopBit = TRUE);
	virtual BOOL	SetFrequency(int nFreq) 
	{
		return FALSE;
	}
	int  GetFrequency()
	{
		return 100000;
	}
};
