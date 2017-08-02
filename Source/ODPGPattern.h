#pragma once
#include <vector>
using namespace std;
#include ".\odpgpatternentry.h"
#include "ODPGHwModule.h"


class CODPGPattern
{
protected:
	vector<CODPGPatternEntry>   m_pattern;
	int	 					    m_nODPGBufferLineByteCount;
	int	 					    m_nODPGBufferLineCount;
	int	 					    m_nWidth;
	CString						m_sDescription;
	CODPGHwModule				   *m_pODPGModule;
public:
	CODPGPattern(void);
	virtual ~CODPGPattern(void);
	virtual void			  Show();
	virtual void			  SetHwModule(CODPGHwModule* pODPGModule){m_pODPGModule = pODPGModule;}
	virtual void			  AddEntry(CODPGPatternEntry entry);
	virtual CODPGPatternEntry GetEntry(int nIndex);
	virtual int				  GetEntryCount();
	virtual CString			  GetDescription();
	virtual void			  SetWidth(int nWidth);
	virtual int				  GetWidth();


	virtual BOOL			OnSelect() = 0;
	virtual BOOL			Create()   = 0;
	virtual CODPGPattern*	Clone()    = 0;
};
//////////////////////////////////////////////////////////////
class CODPGKillerPattern : public CODPGPattern
{
	int	 	        m_nVictimBit;
	int	 	        m_nPhase;
	virtual void	ResetPhase(){m_nPhase = 0;}

public:
	CODPGKillerPattern()
	{
		m_nVictimBit   = 0;
		ResetPhase();
		m_sDescription = "Killer";
	}

	virtual void    SetVictimBit(int nVictimBit);
	virtual int     GetVictimBit();
	virtual BOOL			OnSelect();
	virtual BOOL			Create();
	virtual CODPGPattern*	Clone();
	CODPGPatternEntry		CreatePatternPhase();
};
//////////////////////////////////////////////////////////////
class CODPGSSOPattern : public CODPGPattern
{
public:
	CODPGSSOPattern()
	{
		m_sDescription = "SSO";
	}
	virtual BOOL			OnSelect(){return FALSE;}
	virtual BOOL	        Create();
	virtual CODPGPattern*	Clone();
	CODPGPatternEntry		CreatePatternPhase();

};
//////////////////////////////////////////////////////////////
class CODPGSSONotPattern : public CODPGPattern
{
public:
	CODPGSSONotPattern()
	{
		m_sDescription = "SSO Not";
	}
	virtual BOOL			OnSelect(){return FALSE;}
	virtual BOOL	        Create();
	virtual CODPGPattern*	Clone();
	CODPGPatternEntry		CreatePatternPhase();

};
//////////////////////////////////////////////////////////////
class CODPGAllOnePattern : public CODPGPattern
{
public:
	CODPGAllOnePattern()
	{
		m_sDescription = "All One";
	}
	virtual BOOL			OnSelect(){return FALSE;}
	virtual BOOL			Create();
	virtual CODPGPattern*	Clone();

};
//////////////////////////////////////////////////////////////
class CODPGAllZeroPattern : public CODPGPattern
{
public:
	CODPGAllZeroPattern()
	{
		m_sDescription = "All Zero";
	}	
	virtual BOOL			OnSelect(){return FALSE;}
	virtual BOOL			Create();
	virtual CODPGPattern*	Clone();
};

//////////////////////////////////////////////////////////////
class CODPGWalkingOnePattern : public CODPGPattern
{
	int				m_nBit;
	virtual void	ResetBit(){m_nBit = 0;}

public:
	CODPGWalkingOnePattern()
	{
		ResetBit();
		m_sDescription = "Walking One";
	}
	virtual BOOL			OnSelect(){return FALSE;}
	virtual BOOL			Create();
	virtual CODPGPattern*	Clone();
	CODPGPatternEntry		CreatePatternPhase();
};
//////////////////////////////////////////////////////////////
class CODPGWalkingZeroPattern : public CODPGPattern
{
	int				m_nBit;
	virtual void	ResetBit(){m_nBit = 0;}

public:
	CODPGWalkingZeroPattern()
	{
		ResetBit();
		m_sDescription = "Walking Zero";
	}
	virtual BOOL			OnSelect(){return FALSE;}
	virtual BOOL			Create();
	virtual CODPGPattern*	Clone();
	CODPGPatternEntry		CreatePatternPhase();
};
//////////////////////////////////////////////////////////////
class CODPGRandomDataPattern : public CODPGPattern
{

public:
	CODPGRandomDataPattern()
	{
		m_sDescription = "Random Data";
	}
	virtual BOOL			OnSelect(){return FALSE;}
	virtual BOOL			Create();
	virtual CODPGPattern*	Clone();
	CODPGPatternEntry		CreatePatternPhase();
};

//////////////////////////////////////////////////////////////
class CODPGQuietHighPattern : public CODPGPattern
{
	int				m_nBit;

public:
	CODPGQuietHighPattern()
	{
		m_nBit = 0;
		m_sDescription = "Quiet High";
	}
	virtual BOOL			OnSelect();
	virtual BOOL			Create();
	virtual CODPGPattern*	Clone();
	CODPGPatternEntry		CreatePatternPhase();
};

//////////////////////////////////////////////////////////////
class CODPGQuietLowPattern : public CODPGPattern
{
	int				m_nBit;

public:
	CODPGQuietLowPattern()
	{
		m_nBit = 0;
		m_sDescription = "Quiet Zero";
	}
	virtual BOOL			OnSelect();
	virtual BOOL			Create();
	virtual CODPGPattern*	Clone();
	CODPGPatternEntry		CreatePatternPhase();
};

#if 0
//////////////////////////////////////////////////////////////
class CODPGOppositeLowPattern : public CODPGPattern
{
	int				m_nBit;

public:
	CODPGQuietLowPattern()
	{
		m_nBit = 0;
		m_sDescription = "Opposite Low";
	}
	virtual BOOL			OnSelect();
	virtual BOOL			Create();
	virtual CODPGPattern*	Clone();
	CODPGPatternEntry		CreatePatternPhase();
};
//////////////////////////////////////////////////////////////
#endif
