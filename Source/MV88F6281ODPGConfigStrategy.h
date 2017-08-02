#pragma once
#include "iodpgconfigstrategy.h"



class CMV88F6281ODPGConfigStrategy : public CIODPGConfigStrategy
{

public:
	CMV88F6281ODPGConfigStrategy(CODPGHwModule *pODPG);
	virtual ~CMV88F6281ODPGConfigStrategy(void);
	virtual  BOOL    InitODPG();
	virtual  BOOL    StartODPG();
	virtual  BOOL    StopODPG();

};
