#pragma once
#include "HwModuleCtrlI2CUnit.h"

class CSUB20HwModuleCtrlDrv;

class CSUB20HwModuleCtrlI2CUnitDrv : public CHwModuleCtrlI2CUnitDrv
{
	CSUB20HwModuleCtrlDrv *m_pDrv;

public:
	CSUB20HwModuleCtrlI2CUnitDrv(CSUB20HwModuleCtrlDrv *pDrv);
	virtual ~CSUB20HwModuleCtrlI2CUnitDrv(void);

	virtual BOOL	SetFrequency(int nFreq);
	virtual int 	GetFrequency();
	
	virtual BOOL	HighSpeedRW(struct sub_i2c_hs_xfer* px, int nCount);
	virtual BOOL	WriteData(UCHAR nAddress, UCHAR nSubAddress, int nBytes, UCHAR *pWriteData);
    virtual BOOL    ReadData(UCHAR nAddress, UCHAR nSubAddress, int nBytes, UCHAR *pReadData);
	virtual BOOL    WriteData(UCHAR nAddress, int nBytes, UCHAR *pWriteData, BOOL bSendStopBit = TRUE);
    virtual BOOL    ReadData(UCHAR nAddress, int nBytes, UCHAR *pReadData, BOOL bSendStopBit = TRUE);

};
