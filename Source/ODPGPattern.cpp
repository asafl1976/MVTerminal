#include "StdAfx.h"
#include ".\odpgpattern.h"
#include "ODPGPatternSelectBitDlg.h"

CODPGPattern::CODPGPattern(void)
{
	m_nODPGBufferLineByteCount = 0;
	m_nODPGBufferLineCount     = 0;

}

CODPGPattern::~CODPGPattern(void)
{

}

void	CODPGPattern::AddEntry(CODPGPatternEntry entry)
{
	m_pattern.push_back(entry);
}


int  CODPGPattern::GetEntryCount()
{
	return  (int)m_pattern.size();
}

CODPGPatternEntry CODPGPattern::GetEntry(int nIndex)
{
	if(nIndex < (int)m_pattern.size())
		return m_pattern[nIndex];
	return CODPGPatternEntry();
}
CString CODPGPattern::GetDescription()
{
	return m_sDescription;
}

void    CODPGPattern::SetWidth(int nWidth)
{
	m_nWidth = nWidth;
}

int     CODPGPattern::GetWidth()
{
	return m_nWidth;
}

void    CODPGPattern::Show()
{
	vector<CODPGPatternEntry>::iterator iter, endIter;

	iter    = m_pattern.begin();
	endIter = m_pattern.end();
	for(;iter != endIter; iter++)
	{
		AfxMessageBox( (*iter).ToString() );
	}
}

//////////////////////////////////////////////////////////////////////////////////////
CODPGPatternEntry    CODPGSSOPattern::CreatePatternPhase()
{
	CODPGPatternEntry	entry;
	int					nByte;

	// Create pattern entry according to width 
	for(nByte = 0; nByte < m_nWidth/8; nByte++)
	{
		entry.AddByte(0xFF);  
	}
	for(nByte = 0; nByte < m_nWidth/8; nByte++)
	{
		entry.AddByte(0x00);  
	}
	return entry;
}

BOOL    CODPGSSOPattern::Create()
{
	int				  nLine,nByte,nByteIndex = 0,nByteCount;
	CODPGPatternEntry entry, tempEntry;

	if(m_pODPGModule == NULL)
		return FALSE;

	m_pattern.clear();

	m_nODPGBufferLineByteCount = m_pODPGModule->GetBufferLineByteCount();
	m_nODPGBufferLineCount     = m_pODPGModule->GetBufferLineCount();

	tempEntry  = CreatePatternPhase();
	nByteCount = tempEntry.GetByteCount();
	nByteIndex = 0;
	for(nLine = 0; nLine < m_nODPGBufferLineCount; nLine++)
	{
		entry.Empty();
		for(nByte = 0; nByte < m_nODPGBufferLineByteCount; nByte++)
		{
			entry.AddByte( tempEntry.GetByte(nByteIndex) );
			
			nByteIndex++;

			if(m_nODPGBufferLineByteCount > nByteCount)
			{
				nByteIndex = nByteIndex % nByteCount;
			}
			else
			{
				if(nByteIndex == nByteCount)
					nByteIndex = 0;
			}
		}
		m_pattern.push_back(entry);
	}
	return TRUE;
}

CODPGPattern* CODPGSSOPattern::Clone()
{
	return new CODPGSSOPattern();
}



//////////////////////////////////////////////////////////////////////////////////////
CODPGPatternEntry    CODPGSSONotPattern::CreatePatternPhase()
{
	CODPGPatternEntry	entry;
	int					nByte;

	// Create pattern entry according to width 
	for(nByte = 0; nByte < m_nWidth/8; nByte++)
	{
		entry.AddByte(0x00);  
	}
	for(nByte = 0; nByte < m_nWidth/8; nByte++)
	{
		entry.AddByte(0xFF);  
	}
	return entry;
}

BOOL    CODPGSSONotPattern::Create()
{
	int				  nLine,nByte,nByteIndex = 0,nByteCount;
	CODPGPatternEntry entry, tempEntry;

	if(m_pODPGModule == NULL)
		return FALSE;

	m_pattern.clear();

	m_nODPGBufferLineByteCount = m_pODPGModule->GetBufferLineByteCount();
	m_nODPGBufferLineCount     = m_pODPGModule->GetBufferLineCount();

	tempEntry  = CreatePatternPhase();
	nByteCount = tempEntry.GetByteCount();
	nByteIndex = 0;
	for(nLine = 0; nLine < m_nODPGBufferLineCount; nLine++)
	{
		entry.Empty();
		for(nByte = 0; nByte < m_nODPGBufferLineByteCount; nByte++)
		{
			entry.AddByte( tempEntry.GetByte(nByteIndex) );
			
			nByteIndex++;

			if(m_nODPGBufferLineByteCount > nByteCount)
			{
				nByteIndex = nByteIndex % nByteCount;
			}
			else
			{
				if(nByteIndex == nByteCount)
					nByteIndex = 0;
			}
		}
		m_pattern.push_back(entry);
	}
	return TRUE;
}

CODPGPattern* CODPGSSONotPattern::Clone()
{
	return new CODPGSSONotPattern();
}

//////////////////////////////////////////////////////////////////////////////////////

BOOL    CODPGAllOnePattern::Create()
{
	int				  nLine,nByte;
	CODPGPatternEntry entry;

	if(m_pODPGModule == NULL)
		return FALSE;

	m_pattern.clear();

	m_nODPGBufferLineByteCount = m_pODPGModule->GetBufferLineByteCount();
	m_nODPGBufferLineCount     = m_pODPGModule->GetBufferLineCount();
	for(nByte = 0; nByte < m_nODPGBufferLineByteCount; nByte++)
	{
		entry.AddByte( 0xFF );
	}
	for(nLine = 0; nLine < m_nODPGBufferLineCount; nLine++)
	{
		m_pattern.push_back(entry);
	}
	return TRUE;
}

CODPGPattern* CODPGAllOnePattern::Clone()
{
	return new CODPGAllOnePattern();
}
//////////////////////////////////////////////////////////////////////////////////////

BOOL    CODPGAllZeroPattern::Create()
{
	int				  nLine,nByte;
	CODPGPatternEntry entry;

	if(m_pODPGModule == NULL)
		return FALSE;

	m_pattern.clear();

	m_nODPGBufferLineByteCount = m_pODPGModule->GetBufferLineByteCount();
	m_nODPGBufferLineCount     = m_pODPGModule->GetBufferLineCount();
	for(nByte = 0; nByte < m_nODPGBufferLineByteCount; nByte++)
	{
		entry.AddByte( 0x00 );
	}
	for(nLine = 0; nLine < m_nODPGBufferLineCount; nLine++)
	{
		m_pattern.push_back(entry);
	}
	return TRUE;
}

CODPGPattern* CODPGAllZeroPattern::Clone()
{
	return new CODPGAllZeroPattern();
}

//////////////////////////////////////////////////////////////////////////////////////
CODPGPatternEntry    CODPGWalkingOnePattern::CreatePatternPhase()
{
	CODPGPatternEntry	entry;
	int					nByte;

	// Create pattern entry according to width 
	for(nByte = 0; nByte < m_nWidth/8; nByte++)
	{
		entry.AddByte(0x00);  
	}
	entry.SetByte(entry.GetByteCount() - (m_nBit/8) - 1,0x1<<(m_nBit%8));
	
	m_nBit++;
	if(m_nBit == m_nWidth)
		m_nBit = 0;

	return entry;
}

BOOL    CODPGWalkingOnePattern::Create()
{
	int				  nLine,nByte,nByteIndex = 0,nByteCount;
	CODPGPatternEntry entry,tempEntry;

	if(m_pODPGModule == NULL)
		return FALSE;

	m_pattern.clear();
	ResetBit();

	m_nODPGBufferLineByteCount = m_pODPGModule->GetBufferLineByteCount();
	m_nODPGBufferLineCount     = m_pODPGModule->GetBufferLineCount();

	nByteIndex = 0;

	for(nLine = 0; nLine < m_nODPGBufferLineCount; nLine++)
	{
		tempEntry = CreatePatternPhase();
		nByteCount = tempEntry.GetByteCount();
		entry.Empty();
		for(nByte = 0; nByte < m_nODPGBufferLineByteCount; nByte++)
		{
			entry.AddByte( tempEntry.GetByte(nByteIndex) );
			
			nByteIndex++;

			if(m_nODPGBufferLineByteCount > nByteCount)
			{
				nByteIndex = nByteIndex % nByteCount;
			}
			else
			{
				if(nByteIndex == nByteCount)
					nByteIndex = 0;
			}
		}
		m_pattern.push_back(entry);
	}
	return TRUE;
}

CODPGPattern* CODPGWalkingOnePattern::Clone()
{
	return new CODPGWalkingOnePattern();
}

/////////////////////////////////////////////////////////////////////////////////////
CODPGPatternEntry    CODPGWalkingZeroPattern::CreatePatternPhase()
{
	CODPGPatternEntry	entry;
	int					nByte;

	// Create pattern entry according to width 
	for(nByte = 0; nByte < m_nWidth/8; nByte++)
	{
		entry.AddByte(0xFF);  
	}
	entry.SetByte(entry.GetByteCount() - (m_nBit/8) - 1,0xFF & ~( 0x1<<(m_nBit%8) ));
	
	m_nBit++;
	if(m_nBit == m_nWidth)
		m_nBit = 0;

	return entry;
}

BOOL    CODPGWalkingZeroPattern::Create()
{
	int				  nLine,nByte,nByteIndex = 0,nByteCount;
	CODPGPatternEntry entry,tempEntry;

	if(m_pODPGModule == NULL)
		return FALSE;

	m_pattern.clear();
	ResetBit();

	m_nODPGBufferLineByteCount = m_pODPGModule->GetBufferLineByteCount();
	m_nODPGBufferLineCount     = m_pODPGModule->GetBufferLineCount();

	nByteIndex = 0;

	for(nLine = 0; nLine < m_nODPGBufferLineCount; nLine++)
	{
		tempEntry = CreatePatternPhase();
		nByteCount = tempEntry.GetByteCount();
		entry.Empty();
		for(nByte = 0; nByte < m_nODPGBufferLineByteCount; nByte++)
		{
			entry.AddByte( tempEntry.GetByte(nByteIndex) );
			
			nByteIndex++;

			if(m_nODPGBufferLineByteCount > nByteCount)
			{
				nByteIndex = nByteIndex % nByteCount;
			}
			else
			{
				if(nByteIndex == nByteCount)
					nByteIndex = 0;
			}
		}
		m_pattern.push_back(entry);
	}
	return TRUE;
}

CODPGPattern* CODPGWalkingZeroPattern::Clone()
{
	return new CODPGWalkingZeroPattern();
}

//////////////////////////////////////////////////////////////////////////////////////
BOOL	CODPGKillerPattern::OnSelect()
{
	CODPGPatternSelectBitDlg dlg;

	dlg.SetMaxBit(m_nWidth);
	dlg.SetBit(m_nVictimBit);
	dlg.DoModal();
	m_nVictimBit = dlg.GetBit();

	return TRUE;
}

#define ODPG_KILLER_PATTERN_ARR_LEN  54
CODPGPatternEntry    CODPGKillerPattern::CreatePatternPhase()
{
	CODPGPatternEntry	entry;
	int					nByte;
	UINT				nData[2];

    UCHAR     killerPatterBusState[ODPG_KILLER_PATTERN_ARR_LEN] = {0,0,0,1,1,1,0,1,0,1,
																   0,0,1,0,1,0,1,0,1,0,
																   0,1,0,1,0,0,0,1,1,1,
																   0,0,1,1,0,1,0,1,1,0,
																   0,1,0,1,1,0,1,1,0,1,
																   1,0,0,1};

    UCHAR     killerPatterBitState[ODPG_KILLER_PATTERN_ARR_LEN] = {1,0,1,1,0,0,1,0,1,0,
																   1,1,0,1,0,0,1,0,1,1,
																   0,1,0,1,0,0,0,0,0,1,
																   0,0,1,1,0,1,0,1,1,0,
																   0,1,0,1,0,0,0,0,0,1,
																   1,1,1,1};

    switch(m_nWidth)
    {
    case 16:
		if((killerPatterBusState[m_nPhase%ODPG_KILLER_PATTERN_ARR_LEN] == 1) &&
		   (killerPatterBitState[m_nPhase%ODPG_KILLER_PATTERN_ARR_LEN] == 0))
		{
			nData[0] = (~((0x1 << (m_nVictimBit%m_nWidth)))) & 0xffff;
		}
		else
		{
			nData[0] = 0xffff * killerPatterBusState[m_nPhase%ODPG_KILLER_PATTERN_ARR_LEN] |
					(0x1 << (m_nVictimBit%m_nWidth)) * killerPatterBitState[m_nPhase%ODPG_KILLER_PATTERN_ARR_LEN];
		}
		for(nByte = 0; nByte < 2; nByte++)
		{
			entry.AddByte((nData[0]>>(8 - (nByte*8))) & 0xFF);  
		}        
		break;

    case 32:
        if((killerPatterBusState[m_nPhase%ODPG_KILLER_PATTERN_ARR_LEN] == 1) &&
           (killerPatterBitState[m_nPhase%ODPG_KILLER_PATTERN_ARR_LEN] == 0))
        {
	       nData[0] = ~((0x1 << (m_nVictimBit%m_nWidth)));
        }
        else
        {
		   nData[0] = 0xffffffff * killerPatterBusState[m_nPhase%ODPG_KILLER_PATTERN_ARR_LEN] |
                   (0x1 << (m_nVictimBit%m_nWidth)) * killerPatterBitState[m_nPhase%ODPG_KILLER_PATTERN_ARR_LEN];
        }
		for(nByte = 0; nByte < 4; nByte++)
		{
			entry.AddByte((nData[0]>>(24 - (nByte*8))) & 0xFF);  
		}        
        break;
    case 64:
        if((killerPatterBusState[m_nPhase%ODPG_KILLER_PATTERN_ARR_LEN] == 1) &&
           (killerPatterBitState[m_nPhase%ODPG_KILLER_PATTERN_ARR_LEN] == 0))
        {
            nData[0] = 0xffffffff;
            if((m_nVictimBit%m_nWidth) <= 31)
                nData[0] = ~((0x1 << (m_nVictimBit%m_nWidth)));

			nData[1] = 0xffffffff;
            if((m_nVictimBit%m_nWidth) > 31)
                nData[1] = ~((0x1 << (m_nVictimBit%m_nWidth)));
        }
        else
        {
            nData[0] = 0xffffffff * killerPatterBusState[m_nPhase%ODPG_KILLER_PATTERN_ARR_LEN];
            if((m_nVictimBit%m_nWidth) <= 31)
                nData[0] |= (0x1 << ((m_nVictimBit%m_nWidth)%32)) * killerPatterBitState[m_nPhase%ODPG_KILLER_PATTERN_ARR_LEN];

            nData[1] = 0xffffffff * killerPatterBusState[m_nPhase%ODPG_KILLER_PATTERN_ARR_LEN];
            if((m_nVictimBit%m_nWidth) > 31)
                nData[1] |= (0x1 << ((m_nVictimBit%m_nWidth)%32)) * killerPatterBitState[m_nPhase%ODPG_KILLER_PATTERN_ARR_LEN];
        }
        for(nByte = 0; nByte < 4; nByte++)
		{
			entry.AddByte((nData[1]>>(24 - (nByte*8))) & 0xFF);  
		}   
        for(nByte = 0; nByte < 4; nByte++)
		{
			entry.AddByte((nData[0]>>(24 - (nByte*8))) & 0xFF);  
		}        
 
		break;
	default:
		break;
	}

	//m_nPhase += 64/m_nWidth;
	m_nPhase ++;

	return entry;
}


BOOL    CODPGKillerPattern::Create()
{
	int				  nLine,nByte,nByteIndex = 0,nByteCount;
	CODPGPatternEntry entry,tempEntry;

	if(m_pODPGModule == NULL)
		return FALSE;

	m_pattern.clear();
	ResetPhase();
	
	m_nODPGBufferLineByteCount = m_pODPGModule->GetBufferLineByteCount();
	m_nODPGBufferLineCount     = m_pODPGModule->GetBufferLineCount();

	//nByteIndex = 0;

	for(nLine = 0; nLine < m_nODPGBufferLineCount; nLine++)
	{
		//Create line
		entry.Empty();
		for(nByte = 0; nByte < m_nODPGBufferLineByteCount; nByte+=nByteCount)
		{
			tempEntry = CreatePatternPhase();
			nByteCount = tempEntry.GetByteCount();
			if((m_nODPGBufferLineByteCount % nByteCount) != 0)
			{
				AfxMessageBox("Error while creating killer pattern, invalid byte count!",NULL,MB_ICONERROR);
				return FALSE;
			}
			for(int i = 0; i < nByteCount; i++)
			{
				entry.AddByte( tempEntry.GetByte(i) );
			}

		}
		m_pattern.push_back(entry);
	}
	return TRUE;
}

CODPGPattern* CODPGKillerPattern::Clone()
{
	return new CODPGKillerPattern();
}

void    CODPGKillerPattern::SetVictimBit(int nVictimBit)
{
	m_nVictimBit = nVictimBit;
}
int     CODPGKillerPattern::GetVictimBit()
{
	return m_nVictimBit;
}
//////////////////////////////////////////////////////////////////////////////////////

CODPGPatternEntry    CODPGRandomDataPattern::CreatePatternPhase()
{
	CODPGPatternEntry	entry;
	int					nByte;

	// Create pattern entry according to width 
	for(nByte = 0; nByte < 8; nByte++)
	{
		entry.AddByte(rand()&0xff);  
	}

	return entry;
}

BOOL    CODPGRandomDataPattern::Create()
{
	int				  nLine,nByte,nByteIndex = 0,nByteCount;
	CODPGPatternEntry entry,tempEntry;

	if(m_pODPGModule == NULL)
		return FALSE;

	m_pattern.clear();

	m_nODPGBufferLineByteCount = m_pODPGModule->GetBufferLineByteCount();
	m_nODPGBufferLineCount     = m_pODPGModule->GetBufferLineCount();

	nByteIndex = 0;

	for(nLine = 0; nLine < m_nODPGBufferLineCount; nLine++)
	{
		tempEntry = CreatePatternPhase();
		nByteCount = tempEntry.GetByteCount();
		entry.Empty();
		for(nByte = 0; nByte < m_nODPGBufferLineByteCount; nByte++)
		{
			entry.AddByte( tempEntry.GetByte(nByteIndex) );
			
			nByteIndex++;

			if(m_nODPGBufferLineByteCount > nByteCount)
			{
				nByteIndex = nByteIndex % nByteCount;
			}
			else
			{
				if(nByteIndex == nByteCount)
					nByteIndex = 0;
			}
		}
		m_pattern.push_back(entry);
	}
	return TRUE;
}

CODPGPattern* CODPGRandomDataPattern::Clone()
{
	return new CODPGRandomDataPattern();
}

//////////////////////////////////////////////////////////////////////////////////////
CODPGPatternEntry    CODPGQuietHighPattern::CreatePatternPhase()
{
	CODPGPatternEntry	entry;
	int					nByte;

	// Create pattern entry according to width 
	for(nByte = 0; nByte < m_nWidth/8; nByte++)
	{
		entry.AddByte(0x00);  
	}
	entry.SetByte(entry.GetByteCount() - (m_nBit/8) - 1,0x1<<(m_nBit%8));
	
	for(nByte = 0; nByte < m_nWidth/8; nByte++)
	{
		entry.AddByte(0xFF);  
	}
	//entry.SetByte(entry.GetByteCount() - (m_nBit/8) - 1,0xFF & ~(0x1<<(m_nBit%8)));

	return entry;
}

BOOL	CODPGQuietHighPattern::OnSelect()
{
	CODPGPatternSelectBitDlg dlg;

	dlg.SetMaxBit(m_nWidth);
	dlg.SetBit(m_nBit);
	dlg.DoModal();
	m_nBit = dlg.GetBit();
	return TRUE;
}



BOOL    CODPGQuietHighPattern::Create()
{
	int				  nLine,nByte,nByteIndex = 0,nByteCount;
	CODPGPatternEntry entry,tempEntry;

	if(m_pODPGModule == NULL)
		return FALSE;

	m_pattern.clear();

	m_nODPGBufferLineByteCount = m_pODPGModule->GetBufferLineByteCount();
	m_nODPGBufferLineCount     = m_pODPGModule->GetBufferLineCount();

	nByteIndex = 0;

	for(nLine = 0; nLine < m_nODPGBufferLineCount; nLine++)
	{
		tempEntry = CreatePatternPhase();
		nByteCount = tempEntry.GetByteCount();
		entry.Empty();
		for(nByte = 0; nByte < m_nODPGBufferLineByteCount; nByte++)
		{
			entry.AddByte( tempEntry.GetByte(nByteIndex) );
			
			nByteIndex++;

			if(m_nODPGBufferLineByteCount > nByteCount)
			{
				nByteIndex = nByteIndex % nByteCount;
			}
			else
			{
				if(nByteIndex == nByteCount)
					nByteIndex = 0;
			}
		}
		m_pattern.push_back(entry);
	}
	return TRUE;
}

CODPGPattern* CODPGQuietHighPattern::Clone()
{
	return new CODPGQuietHighPattern();
}


//////////////////////////////////////////////////////////////////////////////////////
CODPGPatternEntry    CODPGQuietLowPattern::CreatePatternPhase()
{
	CODPGPatternEntry	entry;
	int					nByte;

	// Create pattern entry according to width 
	for(nByte = 0; nByte < m_nWidth/8; nByte++)
	{
		entry.AddByte(0xFF);  
	}
	entry.SetByte(entry.GetByteCount() - (m_nBit/8) - 1,0xFF & ~(0x1<<(m_nBit%8)));
	
	for(nByte = 0; nByte < m_nWidth/8; nByte++)
	{
		entry.AddByte(0x00);  
	}

	return entry;
}

BOOL	CODPGQuietLowPattern::OnSelect()
{
	CODPGPatternSelectBitDlg dlg;

	dlg.SetMaxBit(m_nWidth);
	dlg.SetBit(m_nBit);
	dlg.DoModal();
	m_nBit = dlg.GetBit();
	return TRUE;
}



BOOL    CODPGQuietLowPattern::Create()
{
	int				  nLine,nByte,nByteIndex = 0,nByteCount;
	CODPGPatternEntry entry,tempEntry;

	if(m_pODPGModule == NULL)
		return FALSE;

	m_pattern.clear();

	m_nODPGBufferLineByteCount = m_pODPGModule->GetBufferLineByteCount();
	m_nODPGBufferLineCount     = m_pODPGModule->GetBufferLineCount();

	nByteIndex = 0;

	for(nLine = 0; nLine < m_nODPGBufferLineCount; nLine++)
	{
		tempEntry = CreatePatternPhase();
		nByteCount = tempEntry.GetByteCount();
		entry.Empty();
		for(nByte = 0; nByte < m_nODPGBufferLineByteCount; nByte++)
		{
			entry.AddByte( tempEntry.GetByte(nByteIndex) );
			
			nByteIndex++;

			if(m_nODPGBufferLineByteCount > nByteCount)
			{
				nByteIndex = nByteIndex % nByteCount;
			}
			else
			{
				if(nByteIndex == nByteCount)
					nByteIndex = 0;
			}
		}
		m_pattern.push_back(entry);
	}
	return TRUE;
}

CODPGPattern* CODPGQuietLowPattern::Clone()
{
	return new CODPGQuietLowPattern();
}

/////////////////////////////////////////////////////////////////////////////////////