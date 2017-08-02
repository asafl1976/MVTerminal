#pragma once
#include "HwModuleCtrlDrv.h"
#include "libsub.h"



class CSUB20HwModuleCtrlDrv : public CHwModuleCtrlDrv
{
	int			  m_nDelay;
	int			  m_nI2CFreq;
	sub_handle    m_sub20Handle;
	sub_device    m_subDevice;

	static CCriticalSection m_cs;
public:

	typedef enum __SUB20DrvErrorCode 
	{   
		SUB20_OK				         = 0x0, 
		SUB20_SUB_DEV_NOT_FOUND			 = 0x1,
		SUB20_CANT_OPEN_SUB_DEV			 = 0x2,
		SUB20_CANT_SET_CFG				 = 0x3,
		SUB20_CANT_CLAIM_IF				 = 0x4,
		SUB20_FAILED_TO_SETUP_ASYC_TRAN  = 0x5,
		SUB20_FAILED_TO_SUBMIT_ASYC_TRAN = 0x6,
		SUB20_BULK_WRITE_FAILED  		 = 0x7,
		SUB20_BULK_READ_FAILED  		 = 0x8,
		SUB20_BULK_READ_INCOMPLETE		 = 0x9,
		SUB20_OUT_BUF_OVERFLOW  	     = 0xA,
		SUB20_I2C_ERROR         	     = 0xB,
		SUB20_WRONG_TAG_CODE_IN_RES      = 0xC,
		SUB20_WRONG_TAG_SIZE_IN_RES      = 0xD,
		SUB20_WRONG_PARAMS               = 0xE,
		SUB20_SPI_DISABLED               = 0xF,
		SUB20_FEATURE_NOT_SUPPORTED      = 0x10

	}  SUB20_DRV_ERROR_CODE;

	SUB20_DRV_ERROR_CODE  m_eErrorCode;

	CSUB20HwModuleCtrlDrv(sub_handle handle);
	virtual ~CSUB20HwModuleCtrlDrv(void);

public:
	virtual int         GetDelay() {return m_nDelay;}
	virtual BOOL        Reset();
	virtual BOOL        Load();
	virtual void        Unload();
	virtual sub_handle	GetDriverHandle() {return m_sub20Handle;}

	// I2C 
	virtual BOOL I2CHighSpeedRW(struct sub_i2c_hs_xfer* px, int nCount);
	virtual BOOL I2CSetFrequency(int *pFreq );
	virtual int  I2CGetFrequency();
	virtual int	 I2CConfig(int nSlaveAddress, int nFlags ) {return sub_i2c_config(m_sub20Handle,nSlaveAddress,nFlags);}
	virtual int	 I2CSendStartBit() {return sub_i2c_start(m_sub20Handle);}
	virtual int	 I2CSendStopBit() {return sub_i2c_stop(m_sub20Handle);}
	virtual BOOL I2CWriteData(UCHAR nDevAddr, UCHAR nDevSubAddress, int nBytes, UCHAR *pWriteData);
    virtual BOOL I2CReadData(UCHAR nDevAddr, UCHAR nDevSubAddress, int nBytes, UCHAR *pReadData);
	virtual BOOL I2CWriteData(UCHAR nDevAddr, int nBytes, UCHAR *pWriteData, BOOL bSendStopBit = TRUE);
    virtual BOOL I2CReadData(UCHAR nDevAddr, int nBytes, UCHAR *pReadData, BOOL bSendStopBit = TRUE);

	// ADC
	virtual BOOL ADCInit();
	virtual BOOL GetVoltage(int nVoltage, int *pVoltage);
	virtual BOOL GetCurrent(int nCurrent, float nResistorVal, int *pCurrent);
	
	// GPIO
	virtual BOOL GetGPIOState(int nIndex,int& nState);
	virtual BOOL SetGPIOState(int nIndex,int  nState);

	virtual BOOL GetGPIOType(int nIndex,int& nType);
	virtual BOOL SetGPIOType(int nIndex,int nType);
};
