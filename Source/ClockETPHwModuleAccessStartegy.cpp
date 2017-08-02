#include "StdAfx.h"
#include ".\clocketphwmoduleaccessstartegy.h"
#include "HwModuleCtrlI2CUnitDrv.h"
#include "HwModule.h"


CClockETPHwModuleAccessStartegy::CClockETPHwModuleAccessStartegy(CHwModule *pHwModule, CHwModuleCtrlI2CUnitDrv *pI2CUnitDrv)
:CFrequencyCtrlHwModuleAccessStartegy(pHwModule)
{
	m_pI2CUnitDrv = pI2CUnitDrv;
}

CClockETPHwModuleAccessStartegy::~CClockETPHwModuleAccessStartegy(void)
{
}

BOOL	CClockETPHwModuleAccessStartegy::SetFrequency(long nFreq, long nRef, long nRange)
{
	unsigned long   Range2Od[8] = { 6, 2, 8, 4, 5, 7, 1, 3 };
	unsigned long	rdwMax; // maximal value of RDW
	unsigned long	rdw, vdw, od;
	unsigned long	bestRdw, bestVdw;
	long			curFreq, bestFreq = 0;
	unsigned long	temp;

	// check input parameters
	if(nRange > 7)
	{
		return FALSE;
	}

	od = Range2Od[nRange];
	// Calculate range of RDW
	rdwMax = nRef/200 - 2;
	
	// find the best couple rdw, vdw
	for(rdw = 0; rdw < rdwMax; rdw++)
	{
		for(vdw = 0; vdw < 512; vdw++)
		{
			// calculate frequency with given parameters 
			curFreq = nRef * 2 * (vdw + 8) / (rdw + 2) / od;

			// test ranges
			temp = nRef * 2 * (vdw + 8) / (rdw + 2);
			if((temp < 12000) || (temp > 240000))
			{
				continue;
			}

			// compare with best value
			if(abs(curFreq - nFreq) < abs(bestFreq - nFreq))
			{
				bestRdw = rdw;
				bestVdw = vdw;
				bestFreq = curFreq;
			}
		}
	}
	if(abs(bestFreq - nFreq) > 100)
	{
		return FALSE;
	}
	// program synthesizer
	UCHAR    data;	    // data to write to IO

	// set ports state to output
	data=0;

	if(m_pI2CUnitDrv->WriteData(m_nHwModuleID,6,1,&data) == FALSE)
	{
		return FALSE;
	}
	if(m_pI2CUnitDrv->WriteData(m_nHwModuleID,7,1,&data) == FALSE)
	{
		return FALSE;
	}  
	// set RDW
	data = 0;
	data = (bestVdw & 0x1) << 7; 
	data |= (bestRdw & 0x7F);

	if(m_pI2CUnitDrv->WriteData(m_nHwModuleID,3,1,&data) == FALSE)
	{
		return FALSE;
	} 
	// set VDW	
	data = 0; 
	data |= (bestVdw & 0x1E) >> 1; // bits1-4
	data |= (bestVdw & 0x20) << 2; // bit5
	data |= (bestVdw & 0x40);      // bit6
	data |= (bestVdw & 0x80) >> 2; // bit7
	data |= (bestVdw & 0x100) >> 4; // bit8

	if(m_pI2CUnitDrv->WriteData(m_nHwModuleID,2,1,&data) == FALSE)
	{
		return FALSE;
	}
	return TRUE;
}
