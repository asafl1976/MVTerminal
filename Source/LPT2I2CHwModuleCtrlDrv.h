#pragma once
#include "HwModuleCtrlDrv.h"

typedef short int (CALLBACK* EntryPointfuncPtr1)(int nArg);
typedef short int (CALLBACK* EntryPointfuncPtr2)();
typedef int       (CALLBACK* EntryPointfuncPtr3)(int nArg);
typedef void      (CALLBACK* EntryPointfuncPtr4)(int nArg);
typedef void      (CALLBACK* EntryPointfuncPtr5)();
typedef UCHAR     (CALLBACK* EntryPointfuncPtr6)(UCHAR nAddr,UCHAR nSubAddr, int nBytes, UCHAR *pData);
typedef UCHAR     (CALLBACK* EntryPointfuncPtr7)(UCHAR nAddr,int nBytes, UCHAR *pData);
typedef short int (CALLBACK* EntryPointfuncPtr8)(UCHAR nArg);
typedef int       (CALLBACK* EntryPointfuncPtr9)();

class CLPT2I2CHwModuleCtrlDrv : public CHwModuleCtrlDrv
{
	int           m_nFrequency;
	BOOL          m_bNormalMode;
	int			  m_nDelay;
	HMODULE	      m_hDLLModule;

public:
	CLPT2I2CHwModuleCtrlDrv(void);
	virtual ~CLPT2I2CHwModuleCtrlDrv(void);

	typedef  enum __lptToI2cHighDrvErrorCode 
	{   
		LPT2I2C_HIGHDRV_DLL_LOAD_ERROR,
		LPT2I2C_HIGHDRV_REQ_DEV_IS_BUSY,
		LPT2I2C_HIGHDRV_WRONG_DRV_STATUS,
		LPT2I2C_HIGHDRV_REQ_LPT_IS_BUSY,
		LPT2I2C_HIGHDRV_HW_PROBLEM,
		LPT2I2C_HIGHDRV_NORMAL_MODE_PROBLEM,
		LPT2I2C_HIGHDRV_SLOW_MODE_PROBLEM,
		LPT2I2C_HIGHDRV_FREQ_PROBLEM,
		LPT2I2C_HIGHDRV_NO_DRV_PROBLEM
	}  LPT2I2C_HIGHDRV_ERROR_CODE;

	typedef  enum __lptToI2cLowDrvErrorCode 
	{   
		LPT2I2C_LOWDRV_NO_ERROR           = 0x0,
		LPT2I2C_LOWDRV_ADDR_NOT_ACK       = 0x1,
		LPT2I2C_LOWDRV_ACK_NOT_REC        = 0x2,
		LPT2I2C_LOWDRV_READ_ACK_CORRUPTED = 0x3,
		LPT2I2C_LOWDRV_SCL_SDA_STUCK_LOW  = 0x4,
		LPT2I2C_LOWDRV_SDA_STUCK_LOW      = 0x8 ,
		LPT2I2C_LOWDRV_SDA_STUCK_HIGH     = 0x9,
		LPT2I2C_LOWDRV_SCL_STUCK_HIGH     = 0xa,
		LPT2I2C_LOWDRV_SCL_SDA_STUCK_HIGH = 0xb,
		LPT2I2C_LOWDRV_SCL_STUCK_LOW      = 0x80,
		LPT2I2C_LOWDRV_HW_NOT_DETECTED    = 0xff,
	}  LPT2I2C_LOWDRV_ERROR_CODE;

	EntryPointfuncPtr1      m_StartI2CDriverFunc;
	EntryPointfuncPtr8      m_SetLPTNumberFunc;
	EntryPointfuncPtr3      m_SetI2CFrequencyFunc;       
	EntryPointfuncPtr9      m_GetI2CFrequencyFunc;       
	EntryPointfuncPtr4      m_MillisecondsFunc;
	EntryPointfuncPtr2      m_CheckDriverStatusFunc;
	EntryPointfuncPtr5      m_StopI2CDriverFunc;
	EntryPointfuncPtr2      m_HardwareDetectFunc;
	EntryPointfuncPtr2      m_SetNormalModeFunc;
	EntryPointfuncPtr2      m_SetSlowModeFunc; 
	EntryPointfuncPtr6      m_I2CWriteArrayFunc;
	EntryPointfuncPtr7      m_I2CReadArrayNSFunc;
	EntryPointfuncPtr6      m_I2CReadArrayFunc;
	EntryPointfuncPtr9      m_I2CStopFunc;

public:
	virtual int    GetFrequency() {return m_nFrequency;}
	virtual BOOL   GetNormalMode(){return m_bNormalMode;}
	virtual int    GetDelay(){return m_nDelay;}
	virtual BOOL   Load();
	virtual void   Unload();
	virtual void   ShowErrorMessageLow(LPT2I2C_LOWDRV_ERROR_CODE errCode);
	virtual void   ShowErrorMessageHigh(LPT2I2C_HIGHDRV_ERROR_CODE errCode);

	virtual BOOL   WriteData(UCHAR nDevAddr, UCHAR nDevSubAddress, int nBytes, UCHAR *pWriteData);
    virtual BOOL   ReadData(UCHAR nDevAddr, UCHAR nDevSubAddress, int nBytes, UCHAR *pReadData);
	virtual BOOL   WriteData(UCHAR nDevAddr, int nBytes, UCHAR *pWriteData, BOOL bSendStopBit = TRUE);
    virtual BOOL   ReadData(UCHAR nDevAddr, int nBytes, UCHAR *pReadData, BOOL bSendStopBit = TRUE);

};
