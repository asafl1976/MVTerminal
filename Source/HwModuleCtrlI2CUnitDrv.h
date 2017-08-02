#pragma once

#include "HwModuleCtrlUnitDrv.h"

class CHwModuleCtrlI2CUnitDrv :public CHwModuleCtrlUnitDrv
{
public:
	CHwModuleCtrlI2CUnitDrv(void);
	virtual ~CHwModuleCtrlI2CUnitDrv(void);
	virtual BOOL	SetFrequency(int nFreq) = 0;
	virtual int  	GetFrequency() = 0;
	virtual BOOL	WriteData(UCHAR nAddress, UCHAR nSubAddress, int nBytes, UCHAR *pWriteData)        = 0;
    virtual BOOL    ReadData(UCHAR nAddress, UCHAR nSubAddress, int nBytes, UCHAR *pReadData)          = 0;
	virtual BOOL    WriteData(UCHAR nAddress, int nBytes, UCHAR *pWriteData, BOOL bSendStopBit = TRUE) = 0;
    virtual BOOL    ReadData(UCHAR nAddress, int nBytes, UCHAR *pReadData, BOOL bSendStopBit = TRUE)   = 0;
};
