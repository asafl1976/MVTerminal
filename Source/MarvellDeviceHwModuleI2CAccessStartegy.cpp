#include "StdAfx.h"
#include ".\MarvellDeviceHwModuleI2CAccessStartegy.h"
#include "HwModuleCtrlI2CUnitDrv.h"

CMarvellDeviceHwModuleI2CAccessStartegy::CMarvellDeviceHwModuleI2CAccessStartegy(CHwModule *pHwModule,CHwModuleCtrlI2CUnitDrv *pI2CUnitDrv)
:CHwModuleAccessStartegy(pHwModule)
{
	m_pI2CUnitDrv  = pI2CUnitDrv;

}

CMarvellDeviceHwModuleI2CAccessStartegy::~CMarvellDeviceHwModuleI2CAccessStartegy(void)
{
}

BOOL	CMarvellDeviceHwModuleI2CAccessStartegy::WriteRegister(UINT32 nRegAddress,UINT32 nRegValue)
{
    BOOL			bRetVal = FALSE;
	UINT32			nVal = 0;
    int				nRetry, i = 0, iOffset = 0, iCurMask = 0, iByteMask = 0xFF;
	UCHAR			data[8];
	UINT32          nRegOffset;

    //Send the registers' base.
    data[0] = (nRegAddress & 0xff000000) >> 24;

	nRegOffset = (nRegAddress & 0x00ffffff);

    //Send the register number as appears in a spec.
    for (i = 1; i < 4; i++)
    {
        iOffset = 8 * (3 - i);
        iCurMask = iByteMask << iOffset;
        data[i] = (byte)((nRegOffset & iCurMask) >> iOffset);
    }

	nVal |= nRegValue;

    //Send the required register's value.
    for (; i < 8; i++)
    {
        iOffset = 8 * (7 - i);
        iCurMask = iByteMask << iOffset;
        data[i] = (byte)((nVal & iCurMask) >> iOffset);
    }
	for(nRetry = 0; nRetry < 60 ; nRetry++)
	{
	    bRetVal = m_pI2CUnitDrv->WriteData(m_nHwModuleID,8, data);
		if(bRetVal == TRUE)
			break;
	}
	if(bRetVal == FALSE)
	{
		CString s;
		s.Format("%d",nRetry);
		AfxMessageBox("Write register retry timeout ("+s+" write retries)!",NULL,MB_ICONERROR);
	}
    return bRetVal;
}

BOOL CMarvellDeviceHwModuleI2CAccessStartegy::ReadRegister(UINT32 nRegAddress, UINT32 *pRegValue)
{
	BOOL		bRetVal = FALSE;
	UCHAR		address[4],data[8];
    int			nRetry,iOffset = 0;
	CString		sLog;
	//UINT32      nRegOffset;
	UINT32      iByteMask = 0xFF,iCurMask;

	*pRegValue = 0;

    //Send the registers' base.
    address[0] = (nRegAddress & 0xff000000) >> 24;

	//nRegOffset = (nRegAddress & 0x00ffffff);

    //Send the register number as appears in a spec.
    for (int i = 1; i < 4; i++)
    {
        iOffset = 8 * (3 - i);
        iCurMask = iByteMask << iOffset;
        address[i] = (UCHAR)((nRegAddress & iCurMask) >> iOffset); 
    }

	bRetVal = m_pI2CUnitDrv->WriteData(m_nHwModuleID,4, address);
    if (bRetVal == TRUE)
    {
		for(nRetry = 0; nRetry < 40 ; nRetry++)
		{
		    bRetVal = m_pI2CUnitDrv->ReadData(m_nHwModuleID,4, data);
			if(bRetVal == TRUE)
				break;
		}
		
		if(bRetVal == FALSE)
		{
			AfxMessageBox("Write register retry timeout!",NULL,MB_ICONERROR);
		    return bRetVal;
		}
        for (int i = 0; i < 4; i++)
            (*pRegValue) |= (UINT)(data[i] << (8 * (3 - i)));
    }
    return bRetVal;
}
