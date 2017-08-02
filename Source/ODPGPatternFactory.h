#pragma once
#include "ODPGPattern.h"
#include <map>
using namespace std;



class CODPGPatternFactory
{	
	CODPGPatternFactory(void);
	map<CString,CODPGPattern*>  m_patterns;

public:
	static   CODPGPatternFactory& GetInstance();
	virtual ~CODPGPatternFactory(void);
	virtual  void GetPatternTypeList(vector<CString>& patternList);
	virtual  CODPGPattern* Create(CString sPatternName);
};
