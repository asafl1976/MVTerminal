#include "StdAfx.h"
#include ".\mv88f6601odpgconfigstrategy.h"
#include "ODPGPattern.h"

#define ODPG_CTRL_REG			       0x1600
#define ODPG_RAM_PD_REG 		       0x1520
#define ODPG_DDR_CFG_REG	           0x1400
#define ODPG_DDR_CTRL_HIGH_REG         0x1424
#define ODPG_READ_BUFF_SEL_REG         0x14a4
#define ODPG_DATA_CTRL_REG			   0x1630
#define ODPG_DRAM_TARGET_ADDR_REG	   0x1638
#define ODPG_DATA_WR_ADDR_REG          0x16B0
#define ODPG_DATA_WR_DATA_HIGH_REG     0x16B4
#define ODPG_DATA_WR_DATA_LOW_REG      0x16B8
#define ODPG_DATA_WR_ERR_REG           0x16CC
#define ODPG_DATA_RX_WORD_ERR_ADDR_REG 0x16BC
#define ODPG_ENABLE_REG			       0x186d4

CMV88F6601ODPGConfigStrategy::CMV88F6601ODPGConfigStrategy(CODPGHwModule *pODPG):CIODPGConfigStrategy(pODPG)
{
	m_sDevName			= "MV88F6601";
	m_sDevBusWidth      = "16";
	m_nBufLineByteCount = 8;
	m_nBufLineCount     = 64;
	m_nRegBase          = 0xd0;
}

CMV88F6601ODPGConfigStrategy::~CMV88F6601ODPGConfigStrategy(void)
{

}



BOOL    CMV88F6601ODPGConfigStrategy::InitODPG()
{
	UINT	nVal, nStopCond = 0, nCS = 0;
	int		nIndex = 0;

	if(m_pODPG == NULL)
		return FALSE;

	if(m_pODPG->WriteRegister32(ODPG_RAM_PD_REG,0) == FALSE)
	{
		AfxMessageBox("1 Error while writing register!",NULL,MB_ICONERROR);
		return FALSE;
	}
	if(m_pODPG->WriteRegister32(ODPG_CTRL_REG,0) == FALSE)
	{
		AfxMessageBox("2 Error while writing register!",NULL,MB_ICONERROR);
		return FALSE;
	}

	if(m_pODPG->WriteRegister32(ODPG_DDR_CFG_REG,0x80000,TRUE) == FALSE)
	{
		AfxMessageBox("3 Error while writing register!",NULL,MB_ICONERROR);
		return FALSE;
	}
	if(m_pODPG->WriteRegister32(ODPG_READ_BUFF_SEL_REG,0xff) == FALSE)
	{
		AfxMessageBox("4 Error while writing register!",NULL,MB_ICONERROR);
		return FALSE;
	}

	if(m_pODPG->WriteRegister32(ODPG_DATA_CTRL_REG,0x1) == FALSE)
	{
		AfxMessageBox("5 Error while writing register!",NULL,MB_ICONERROR);
		return FALSE;
	}

	for(int nLine = 0; nLine < m_nBufLineCount; nLine++)
	{
		CODPGPatternEntry patternEntry;
		UINT		      nByte,nShift = 0;
		volatile UINT	  nPatternLow = 0,nPatternHigh = 0,nTLow,nTHigh;

		patternEntry = m_pODPG->GetParams().m_pPattern->GetEntry(nLine);
		nPatternLow  = 0;
		nPatternHigh = 0;

		//Sleep(50);//Solves i2c write failures
		for(nByte = 0; nByte < 4; nByte++)// Pattern byte count is 8 in KW
		{
			nVal = patternEntry.GetByte(nByte);
			nPatternLow  |= (nVal<<(8*nShift));

			nVal = patternEntry.GetByte(nByte + 4);
			nPatternHigh |= (nVal<<(8*nShift));

			nShift++;
		}

		nTHigh   = ( ((nPatternHigh>>8)&0x00FF0000) | ((nPatternHigh<<8)&0xFF000000) | 
			       ((nPatternHigh>>8)&0x000000FF) | ((nPatternHigh<<8)&0x0000FF00) );

		nTLow = ( ((nPatternLow>>8)&0x00FF0000) | ((nPatternLow<<8)&0xFF000000) | 
			      ((nPatternLow>>8)&0x000000FF) | ((nPatternLow<<8)&0x0000FF00) );

		if(m_pODPG->WriteRegister32(ODPG_DATA_WR_DATA_LOW_REG,nTLow) == FALSE)
		{
			AfxMessageBox("6 Error while writing register!",NULL,MB_ICONERROR);
			return FALSE;
		}
		if(m_pODPG->WriteRegister32(ODPG_DATA_WR_DATA_HIGH_REG,nTHigh) == FALSE)
		{
			AfxMessageBox("6 Error while writing register!",NULL,MB_ICONERROR);
			return FALSE;
		}
		if(m_pODPG->WriteRegister32(ODPG_DATA_WR_ADDR_REG,nLine) == FALSE)
		{
			AfxMessageBox("8 Error while writing register!",NULL,MB_ICONERROR);
			return FALSE;
		}
	}

	// Set DRAM target address at register 0x1638
	nVal = m_pODPG->GetParams().m_nAddress;
	if(m_pODPG->WriteRegister32(ODPG_DRAM_TARGET_ADDR_REG,nVal>>3) == FALSE)
	{
		AfxMessageBox("9 Error while writing register!",NULL,MB_ICONERROR);
		return FALSE;
	}

	switch(m_pODPG->GetParams().m_eStopCond)
	{
	case ODPG_STOP_COND_SINGLE_PATTERN:
		nStopCond = 0;
		break;
	case ODPG_STOP_COND_CONTINUOUS:
		nStopCond = 4;
		break;
	case ODPG_STOP_COND_PRE_DEF_ADDR:
		nStopCond = 2;
		break;
	case ODPG_STOP_COND_ERR_DATA_ADDR:

		nStopCond = 1;
		break;
	case ODPG_STOP_COND_PHASE_COUNT:
		nStopCond = 3;
		break;
	}

	nVal  = 0x1;
	nVal |= nStopCond<<2;
	nVal |= 0x1f<<5;
	nVal |= 0xf<<11;
	nVal |= 2<<15;

	if(m_pODPG->GetParams().m_bWrite == TRUE)
	{
		nVal |= 1<<25;
	}
	else // Read
	{
		nVal |= 1<<1;
	}

	if(m_pODPG->WriteRegister32(ODPG_DATA_CTRL_REG,nVal) == FALSE)
	{
		AfxMessageBox("3 Error while writing register!",NULL,MB_ICONERROR);
		return FALSE;
	}


	return TRUE;
}

BOOL    CMV88F6601ODPGConfigStrategy::StartODPG()
{	
	UINT	nVal;

	if(m_pODPG == NULL)
		return FALSE;

	// Write ODPG start register (R-Unit 0x100d4)
	nVal = 0x1;
	if(m_pODPG->WriteRegister32(ODPG_ENABLE_REG,nVal) == FALSE)
	{
		AfxMessageBox("12 Error while writing register!",NULL,MB_ICONERROR);
		return FALSE;
	}

	if(m_pODPG->GetParams().m_eStopCond == ODPG_STOP_COND_SINGLE_PATTERN)
	{
	
		if(m_pODPG->ReadRegister32(ODPG_ENABLE_REG,&nVal) == FALSE)
		{
			AfxMessageBox("13 Error while reading register!",NULL,MB_ICONERROR);
			return FALSE;
		}

		while((nVal & 0x1) == 0x1)
		{
			if(m_pODPG->ReadRegister32(ODPG_ENABLE_REG,&nVal) == FALSE)
			{
				AfxMessageBox("Polling error - Error while reading register!",NULL,MB_ICONERROR);
				return FALSE;
			}
		}

		if((m_pODPG->GetParams().m_bRead == TRUE) && (m_pODPG->GetParams().m_bCompareData == TRUE))
		{
			if(m_pODPG->ReadRegister32(ODPG_DATA_RX_WORD_ERR_ADDR_REG,&nVal) == FALSE)
			{
				AfxMessageBox("14 Error while reading register!",NULL,MB_ICONERROR);
				return FALSE;
			}
			if(nVal != 0)
			{
				nVal = (nVal << 3) - 8;
				CString sErrAddr;
				sErrAddr.Format("Data mismatch error found at address range 0x%x --> 0x%x",nVal,nVal + 4);
				AfxMessageBox(sErrAddr,NULL,MB_ICONERROR);
				return FALSE;
			}
		}
	}

	return TRUE;
}

BOOL    CMV88F6601ODPGConfigStrategy::StopODPG()
{	
	UINT	nVal;

	if(m_pODPG == NULL)
		return FALSE;

	// Disable OPDG 
	nVal = 0x2;
	if(m_pODPG->WriteRegister32(ODPG_ENABLE_REG,nVal) == FALSE)
		return FALSE;

	return TRUE;
}
