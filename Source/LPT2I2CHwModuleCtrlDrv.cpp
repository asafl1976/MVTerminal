#include "StdAfx.h"
#include ".\lpt2i2CHwModuleCtrlDrv.h"

CLPT2I2CHwModuleCtrlDrv::CLPT2I2CHwModuleCtrlDrv(void)
{
	m_hDLLModule     = NULL;
	m_nFrequency     = 90;
	m_nDelay         = 20; 
	m_bNormalMode    = TRUE;
}

CLPT2I2CHwModuleCtrlDrv::~CLPT2I2CHwModuleCtrlDrv(void)
{
	Unload();
}

void   CLPT2I2CHwModuleCtrlDrv::ShowErrorMessageLow(LPT2I2C_LOWDRV_ERROR_CODE errCode)
{
	if(m_bErrMsgEnable == FALSE)
		return;

	switch(errCode)
	{
	case LPT2I2C_LOWDRV_ADDR_NOT_ACK:
		AfxMessageBox("LPT-2-I2C: I2C Address not Acknowledged!",NULL,MB_ICONERROR);
		break;
	case LPT2I2C_LOWDRV_ACK_NOT_REC:
		AfxMessageBox("LPT-2-I2C: Ack not recieved!",NULL,MB_ICONERROR);
		break;
	case LPT2I2C_LOWDRV_READ_ACK_CORRUPTED:
		AfxMessageBox("LPT-2-I2C: Read ack corrupted!",NULL,MB_ICONERROR);
		break;
	case LPT2I2C_LOWDRV_SCL_SDA_STUCK_LOW:
		AfxMessageBox("LPT-2-I2C: SCL / SDA stuck low!",NULL,MB_ICONERROR);
		break;
	case LPT2I2C_LOWDRV_SDA_STUCK_LOW:
		AfxMessageBox("LPT-2-I2C: SDA stuck low!",NULL,MB_ICONERROR);
		break;
	case LPT2I2C_LOWDRV_SDA_STUCK_HIGH:
		AfxMessageBox("LPT-2-I2C: SDA stuck high!",NULL,MB_ICONERROR);
		break;
	case LPT2I2C_LOWDRV_SCL_STUCK_HIGH:
		AfxMessageBox("LPT-2-I2C: SCL stuck high!",NULL,MB_ICONERROR);
		break;
	case LPT2I2C_LOWDRV_SCL_SDA_STUCK_HIGH:
		AfxMessageBox("LPT-2-I2C: SCL / SDA stuck high!",NULL,MB_ICONERROR);
		break;
	case LPT2I2C_LOWDRV_SCL_STUCK_LOW:
		AfxMessageBox("LPT-2-I2C: SCL stuck low!",NULL,MB_ICONERROR);
		break;
	case LPT2I2C_LOWDRV_HW_NOT_DETECTED:
		AfxMessageBox("LPT-2-I2C: Hardware not detected!",NULL,MB_ICONERROR);
		break;
	default:
		AfxMessageBox("LPT-2-I2C: Unknown error code!",NULL,MB_ICONERROR);
		break;
	}
}

void   CLPT2I2CHwModuleCtrlDrv::ShowErrorMessageHigh(LPT2I2C_HIGHDRV_ERROR_CODE errCode)
{
	if(m_bErrMsgEnable == FALSE)
		return;

	switch(errCode)
	{
	case LPT2I2C_HIGHDRV_DLL_LOAD_ERROR:
		AfxMessageBox("LPT-2-I2C: DLL Load Error!",NULL,MB_ICONERROR);
		break;
	case LPT2I2C_HIGHDRV_REQ_DEV_IS_BUSY:
		AfxMessageBox("LPT-2-I2C: Requested device is busy!",NULL,MB_ICONERROR);
		break;
	case LPT2I2C_HIGHDRV_WRONG_DRV_STATUS:
		AfxMessageBox("LPT-2-I2C: Wrong driver status!",NULL,MB_ICONERROR);
		break;
	case LPT2I2C_HIGHDRV_REQ_LPT_IS_BUSY:
		AfxMessageBox("LPT-2-I2C: Requested LPT is busy!",NULL,MB_ICONERROR);
		break;
	case LPT2I2C_HIGHDRV_HW_PROBLEM:
		AfxMessageBox("LPT-2-I2C: Hardware problem!",NULL,MB_ICONERROR);
		break;
	case LPT2I2C_HIGHDRV_NORMAL_MODE_PROBLEM:
		AfxMessageBox("LPT-2-I2C: Normal mode problem!",NULL,MB_ICONERROR);
		break;
	case LPT2I2C_HIGHDRV_SLOW_MODE_PROBLEM:
		AfxMessageBox("LPT-2-I2C: Slow mode problem!",NULL,MB_ICONERROR);
		break;
	case LPT2I2C_HIGHDRV_FREQ_PROBLEM:
		AfxMessageBox("LPT-2-I2C: Frequancy problem!",NULL,MB_ICONERROR);
		break;
	case LPT2I2C_HIGHDRV_NO_DRV_PROBLEM:
		break;
	default:
		AfxMessageBox("LPT-2-I2C: Unknown error code!",NULL,MB_ICONERROR);
		break;
	}
}
void  CLPT2I2CHwModuleCtrlDrv::Unload()
{
	if(m_hDLLModule != NULL)
		FreeLibrary(m_hDLLModule);

}


BOOL  CLPT2I2CHwModuleCtrlDrv::Load()
{
	/*Load LPTToI2C dll */
	m_hDLLModule = LoadLibrary("C:\\Windows\\system32\\LPTtoI2Cpro.dll");
	if (m_hDLLModule != NULL)
	{
		if((m_StartI2CDriverFunc    = (EntryPointfuncPtr1)GetProcAddress(m_hDLLModule,"StartI2CDriver")) == NULL)
			return FALSE;
		if((m_SetLPTNumberFunc      = (EntryPointfuncPtr8)GetProcAddress(m_hDLLModule,"SetLPTNumber")) == NULL)
			return FALSE;
		if((m_SetI2CFrequencyFunc   = (EntryPointfuncPtr3)GetProcAddress(m_hDLLModule,"SetI2CFrequency")) == NULL)       
			return FALSE;
		if((m_GetI2CFrequencyFunc   = (EntryPointfuncPtr9)GetProcAddress(m_hDLLModule,"GetI2CFrequency")) == NULL)       
			return FALSE;
		if((m_MillisecondsFunc      = (EntryPointfuncPtr4)GetProcAddress(m_hDLLModule,"milliseconds")) == NULL)
			return FALSE;
		if((m_StopI2CDriverFunc     = (EntryPointfuncPtr5)GetProcAddress(m_hDLLModule,"StopI2CDriver")) == NULL)
			return FALSE;
		if((m_CheckDriverStatusFunc = (EntryPointfuncPtr2)GetProcAddress(m_hDLLModule,"CheckDriverStatus")) == NULL)
			return FALSE;
		if((m_HardwareDetectFunc    = (EntryPointfuncPtr2)GetProcAddress(m_hDLLModule,"HardwareDetect")) == NULL)
			return FALSE;
		if((m_SetNormalModeFunc     = (EntryPointfuncPtr2)GetProcAddress(m_hDLLModule,"SetNormalMode")) == NULL)
			return FALSE;
		if((m_SetSlowModeFunc       = (EntryPointfuncPtr2)GetProcAddress(m_hDLLModule,"SetSlowMode")) == NULL)
			return FALSE;
		if((m_I2CWriteArrayFunc     = (EntryPointfuncPtr6)GetProcAddress(m_hDLLModule,"I2CWriteArray")) == NULL)
			return FALSE;
		if((m_I2CReadArrayNSFunc    = (EntryPointfuncPtr7)GetProcAddress(m_hDLLModule,"I2CReadArrayNS")) == NULL)
			return FALSE;
		if((m_I2CReadArrayFunc     = (EntryPointfuncPtr6)GetProcAddress(m_hDLLModule,"I2CReadArray")) == NULL)
			return FALSE;
		if((m_I2CStopFunc          = (EntryPointfuncPtr9)GetProcAddress(m_hDLLModule,"I2CStop")) == NULL)
			return FALSE;
		return TRUE;
	}

	return FALSE;
}

BOOL  CLPT2I2CHwModuleCtrlDrv::WriteData(UCHAR nDevAddr, UCHAR nDevSubAddress, int nBytes, UCHAR *pWriteData)
{
    LPT2I2C_LOWDRV_ERROR_CODE eEc;

    nDevAddr <<= 1;//Shift the address --> read/write bit.
 
    eEc = (LPT2I2C_LOWDRV_ERROR_CODE)m_I2CWriteArrayFunc(nDevAddr, nDevSubAddress, nBytes, pWriteData);
    if ((eEc != LPT2I2C_LOWDRV_NO_ERROR) && (eEc != LPT2I2C_LOWDRV_ACK_NOT_REC))
    {
		ShowErrorMessageLow(eEc);
        return FALSE;
    }
	m_MillisecondsFunc(m_nDelay);
    return TRUE;
}



BOOL   CLPT2I2CHwModuleCtrlDrv::ReadData(UCHAR  nDevAddr, UCHAR nDevSubAddress, int nBytes, UCHAR *pReadData)
{
	LPT2I2C_LOWDRV_ERROR_CODE eEc;

    nDevAddr <<= 1;//Shift the address to make a room for read/write bit.

    eEc = (LPT2I2C_LOWDRV_ERROR_CODE)m_I2CReadArrayFunc(nDevAddr, nDevSubAddress, nBytes, pReadData);

    if (eEc != LPT2I2C_LOWDRV_NO_ERROR)
    {
		ShowErrorMessageLow(eEc);
        return FALSE;
    }	
    m_MillisecondsFunc(m_nDelay);
    return TRUE;
}

BOOL   CLPT2I2CHwModuleCtrlDrv::WriteData(UCHAR nDevAddr, int nBytes, UCHAR *pWriteData, BOOL bSendStopBit)
{
	//Workaround for sending a data without subaddress.
    //The first data's byte is sent as a subaddress, and the 
    //sent data is the original without the first byte.
	return WriteData(nDevAddr,pWriteData[0],nBytes - 1,&pWriteData[1]);

}
BOOL   CLPT2I2CHwModuleCtrlDrv::ReadData(UCHAR nDevAddr, int nBytes, UCHAR *pReadData, BOOL bSendStopBit)
{
	LPT2I2C_LOWDRV_ERROR_CODE eEc;

    nDevAddr <<= 1;//Shift the address to make a room for read/write bit.

    eEc = (LPT2I2C_LOWDRV_ERROR_CODE)m_I2CReadArrayNSFunc(nDevAddr, nBytes, pReadData);

    if (eEc != LPT2I2C_LOWDRV_NO_ERROR)
    {
		ShowErrorMessageLow(eEc);
        return FALSE;
    }	
	if(bSendStopBit == TRUE)
	{
		eEc = (LPT2I2C_LOWDRV_ERROR_CODE)m_I2CStopFunc();
		if (eEc != LPT2I2C_LOWDRV_NO_ERROR)
		{
			ShowErrorMessageLow(eEc);
			return FALSE;
		}	
	}
    m_MillisecondsFunc(m_nDelay);
	return TRUE;
}
