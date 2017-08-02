#include "StdAfx.h"
#include "SUB20HwModuleCtrlDrv.h"
#include <vector>
using namespace std;

CCriticalSection CSUB20HwModuleCtrlDrv::m_cs;

CSUB20HwModuleCtrlDrv::CSUB20HwModuleCtrlDrv(sub_handle handle)
{
	m_nDelay	  = 20; 
	m_sub20Handle = handle;
	m_nI2CFreq    = 55000;
}

CSUB20HwModuleCtrlDrv::~CSUB20HwModuleCtrlDrv(void)
{
}

void  CSUB20HwModuleCtrlDrv::Unload()
{

}

BOOL  CSUB20HwModuleCtrlDrv::Reset()
{
	m_eErrorCode = (SUB20_DRV_ERROR_CODE)sub_reset(m_sub20Handle);
    if(m_eErrorCode != SUB20_OK)
	{
		return FALSE;
	}
	return TRUE;
}

BOOL  CSUB20HwModuleCtrlDrv::Load()
{
	return TRUE;
}

BOOL  CSUB20HwModuleCtrlDrv::I2CSetFrequency(int *pFreq ) 
{
	m_eErrorCode = (SUB20_DRV_ERROR_CODE)sub_i2c_freq(m_sub20Handle,pFreq);
    if(m_eErrorCode != SUB20_OK)
	{
		return FALSE;
	}
	m_nI2CFreq = *pFreq;
    return TRUE;
}

int  CSUB20HwModuleCtrlDrv::I2CGetFrequency() 
{
    return m_nI2CFreq;
}

BOOL CSUB20HwModuleCtrlDrv::I2CHighSpeedRW(struct sub_i2c_hs_xfer* px, int nCount)
{
	m_cs.Lock();
	m_eErrorCode = (SUB20_DRV_ERROR_CODE)sub_i2c_hs_rw(m_sub20Handle, 0x4, nCount , px);
	m_cs.Unlock();

	CString s;
	s.Format("0 - actual size = %d, stat = %x | 1 - actual size = %d, stat = %x",px[0].act_sz,px[0].status,px[1].act_sz,px[1].status);
	AfxMessageBox(s);

    if(m_eErrorCode != SUB20_OK)
	{
		return FALSE;
	}
    return TRUE;
}


BOOL  CSUB20HwModuleCtrlDrv::I2CWriteData(UCHAR nDevAddr, UCHAR nDevSubAddress, int nBytes, UCHAR *pWriteData)
{
	m_cs.Lock();
	m_eErrorCode = (SUB20_DRV_ERROR_CODE)sub_i2c_write(m_sub20Handle, nDevAddr, nDevSubAddress, 1 ,( char*)pWriteData, nBytes);
	m_cs.Unlock();

    if(m_eErrorCode != SUB20_OK)
	{
		return FALSE;
	}
    return TRUE;
}

BOOL   CSUB20HwModuleCtrlDrv::I2CReadData(UCHAR  nDevAddr, UCHAR nDevSubAddress, int nBytes, UCHAR *pReadData)
{
	m_cs.Lock();
	m_eErrorCode = (SUB20_DRV_ERROR_CODE)sub_i2c_read(m_sub20Handle, nDevAddr, nDevSubAddress, 1 ,( char*)pReadData, nBytes);
	m_cs.Unlock();
    if(m_eErrorCode != SUB20_OK)
	{
		return FALSE;
	}
    return TRUE;
}

BOOL   CSUB20HwModuleCtrlDrv::I2CWriteData(UCHAR nDevAddr, int nBytes, UCHAR *pWriteData, BOOL bSendStopBit)
{
	m_cs.Lock();
    m_eErrorCode = (SUB20_DRV_ERROR_CODE)sub_i2c_write(m_sub20Handle, nDevAddr, 0, 0 ,( char*)pWriteData, nBytes);
	m_cs.Unlock();
    if(m_eErrorCode != SUB20_OK)
	{
		return FALSE;
	}
    return TRUE;
}

BOOL   CSUB20HwModuleCtrlDrv::I2CReadData(UCHAR nDevAddr, int nBytes, UCHAR *pReadData, BOOL bSendStopBit)
{
	m_cs.Lock();
	m_eErrorCode = (SUB20_DRV_ERROR_CODE)sub_i2c_read(m_sub20Handle, nDevAddr, 0, 0 ,( char*)pReadData, nBytes);
	m_cs.Unlock();

    if(m_eErrorCode != SUB20_OK)
	{
		return FALSE;
	}
    return TRUE;
}

/////////////////////// ADC ///////////////////////////////////////////////////////////////

BOOL	CSUB20HwModuleCtrlDrv::ADCInit()
{
	m_cs.Lock();
	if(sub_adc_config(m_sub20Handle, ADC_ENABLE+ADC_REF_VCC) == 0)
	{
		m_cs.Unlock();
		return TRUE;
	}
	m_cs.Unlock();
	return FALSE;
}

BOOL	CSUB20HwModuleCtrlDrv::GetVoltage(int nVoltage, int *pVoltage)
{
	int nData;

	m_cs.Lock();
	if(sub_adc_single(m_sub20Handle,&nData, nVoltage) == 0)
	{
		*pVoltage = nData*3300/1023;
		m_cs.Unlock();
		return TRUE;
	}
	m_cs.Unlock();
	return FALSE;
}

BOOL	CSUB20HwModuleCtrlDrv::GetCurrent(int nCurrent, float nResistorVal, int *pCurrent)
{
	int mux[3],nCurrentVal ;
	int measurement[3];

	if(nCurrent == 0)
	{
		mux[0]=ADC_D01;
		mux[1]=ADC_D10_10X;
		mux[2]=ADC_D10_200X;
	}
	else
	{
        mux[0]=ADC_D32;
		mux[1]=ADC_D32_10X;
		mux[2]=ADC_D32_200X;
	}

	m_cs.Lock();
	if(sub_adc_read(m_sub20Handle, measurement, mux,3) != 0)
	{
		m_cs.Unlock();
		return FALSE;
	}	
	m_cs.Unlock();

	if (measurement[2] < 450)
	{
		nCurrentVal = (int)((float)measurement[2]*(float)3300/((float)512*(float)nResistorVal/(float)5));
	}
	else if(measurement[1] < 500)
	{
		nCurrentVal = (int)((float)measurement[1]*(float)3300/((float)512*(float)nResistorVal/(float)100));
	}
	else
	{
		nCurrentVal = (int)(-((float)measurement[0]*(float)3300/((float)512*(float)nResistorVal/(float)1000)));
	}
	*pCurrent = nCurrentVal;

	return TRUE;
}

//////////////////////////////// GPIO///////////////////////////////////
BOOL	CSUB20HwModuleCtrlDrv::GetGPIOState(int nIndex,int& nState)
{
	sub_int32_t nStat;

	nState = 5;//Set invalid value
	/* read gpio statue */
	m_cs.Lock();
	if( sub_gpio_read( m_sub20Handle, &nStat ) != 0)
	{
		m_cs.Unlock();
		return FALSE;
	}
	m_cs.Unlock();
	nState = ((nStat>>nIndex) & 0x1);
	return TRUE;
}

BOOL	CSUB20HwModuleCtrlDrv::SetGPIOState(int nIndex,int  nState)
{
	sub_int32_t nSet = nState, nGet, nMask = 0x1;

	nSet  = nSet  << nIndex;
	nMask = nMask << nIndex; 

	m_cs.Lock();
	if( sub_gpio_write( m_sub20Handle, nSet, &nGet, nMask ) != 0)
	{
		m_cs.Unlock();
		return FALSE;
	}
	m_cs.Unlock();
	return TRUE;
}

BOOL	CSUB20HwModuleCtrlDrv::GetGPIOType(int nIndex,int& nType)
{
	sub_int32_t nGPIO = 0x1, nGPIOType = 0x1, nConfig;
	
	nType = 5;//Set invalid value
	/* read gpio configuration */
	m_cs.Lock();
	if( sub_gpio_config( m_sub20Handle, 0, &nConfig, 0 ) != 0)
	{
		m_cs.Unlock();
		return FALSE;
	}
	m_cs.Unlock();

	//Set type, 0 = input, 1 = output
	nType = (nConfig >> nIndex) & 0x1;

	return TRUE;
}
BOOL	CSUB20HwModuleCtrlDrv::SetGPIOType(int nIndex,int nType)
{
	sub_int32_t nGPIO = 0x1, nGPIOType = 0x1, nConfig;
	
	//Set type, 0 = input, 1 = output
	nGPIOType = nGPIO << nIndex;
	if(nType == 0)
	{
		nGPIOType = 0;
	}
	m_cs.Lock();
	if( sub_gpio_config( m_sub20Handle, nGPIOType, &nConfig, nGPIO << nIndex ) != 0)
	{
		m_cs.Unlock();
		return FALSE;
	}
	m_cs.Unlock();

	return TRUE;
}

