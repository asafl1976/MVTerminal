#pragma once
#include "iodpgconfigstrategy.h"



class CMV88F6601ODPGConfigStrategy :
	public CIODPGConfigStrategy
{

public:
	CMV88F6601ODPGConfigStrategy(CODPGHwModule *pODPG);
	virtual ~CMV88F6601ODPGConfigStrategy(void);
	virtual  BOOL    InitODPG();
	virtual  BOOL    StartODPG();
	virtual  BOOL    StopODPG();

};
