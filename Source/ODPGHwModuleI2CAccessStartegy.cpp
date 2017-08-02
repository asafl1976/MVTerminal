#include "StdAfx.h"
#include ".\odpghwmodulei2caccessstartegy.h"
#include "HwModule.h"
#include "HwModuleCtrlI2CUnitDrv.h"

CODPGHwModuleI2CAccessStartegy::CODPGHwModuleI2CAccessStartegy(CHwModule *pHwModule, CHwModuleCtrlI2CUnitDrv *pI2CUnitDrv)
:CODPGHwModuleAccessStartegy(pHwModule)
{
	m_pI2CUnitDrv = pI2CUnitDrv;
}

CODPGHwModuleI2CAccessStartegy::~CODPGHwModuleI2CAccessStartegy(void)
{
}

BOOL  CODPGHwModuleI2CAccessStartegy::WriteRegister32(UINT nRegBase, UINT nRegOffset, UINT nRegValue, BOOL bReadModifyWrite)
{
    BOOL			bRetVal = FALSE;
	UINT			nVal = 0;
    int				nRetry, i = 0, iOffset = 0, iCurMask = 0, iByteMask = 0xFF;
	UCHAR			data[8];

	if(bReadModifyWrite == TRUE)
	{
		if(ReadRegister32(nRegBase,nRegOffset,&nVal) == FALSE)
			return FALSE;
	}
    //Send the registers' base.
    data[0] = nRegBase;

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
		AfxMessageBox("Write register retry timeout (" + s + " write retries)!",NULL,MB_ICONERROR);
	}
    return bRetVal;
}

BOOL  CODPGHwModuleI2CAccessStartegy::ReadRegister32(UINT nRegBase, UINT nRegOffset,UINT *pRegValue)
{	
	BOOL		bRetVal = FALSE;
	UCHAR		address[4],data[8];
    int			nRetry,iOffset = 0, iCurMask = 0, iByteMask = 0xFF;
	CString		sLog;

	*pRegValue = 0;

    //Send the registers' base.
    address[0] = nRegBase;

    //Send the register number as appears in a spec.
    for (int i = 1; i < 4; i++)
    {
        iOffset = 8 * (3 - i);
        iCurMask = iByteMask << iOffset;
        address[i] = (UCHAR)((nRegOffset & iCurMask) >> iOffset);
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

