#include "StdAfx.h"
#include "ODPGPatternFactory.h"

CODPGPatternFactory::CODPGPatternFactory(void)
{
	CODPGPattern *pPattern;

	pPattern = new CODPGSSOPattern();
	m_patterns[pPattern->GetDescription()]= pPattern;

	pPattern = new CODPGSSONotPattern();
	m_patterns[pPattern->GetDescription()]= pPattern;

	pPattern = new CODPGAllOnePattern();
	m_patterns[pPattern->GetDescription()]= pPattern;

	pPattern = new CODPGAllZeroPattern();
	m_patterns[pPattern->GetDescription()]= pPattern;

	pPattern = new CODPGKillerPattern();
	m_patterns[pPattern->GetDescription()]= pPattern;

	pPattern = new CODPGWalkingOnePattern();
	m_patterns[pPattern->GetDescription()]= pPattern;

	pPattern = new CODPGWalkingZeroPattern();
	m_patterns[pPattern->GetDescription()]= pPattern;

	pPattern = new CODPGRandomDataPattern();
	m_patterns[pPattern->GetDescription()]= pPattern;

	pPattern = new CODPGQuietLowPattern();
	m_patterns[pPattern->GetDescription()]= pPattern;

	pPattern = new CODPGQuietHighPattern();
	m_patterns[pPattern->GetDescription()]= pPattern;
}

CODPGPatternFactory::~CODPGPatternFactory(void)
{
}
CODPGPatternFactory& CODPGPatternFactory::GetInstance()
{
	static CODPGPatternFactory obj;
	return obj;
}

void CODPGPatternFactory::GetPatternTypeList(vector<CString>& patternList)
{
	map<CString,CODPGPattern*>::iterator iter,endIter;
	
	iter    = m_patterns.begin();
	endIter = m_patterns.end();

	for(;iter != endIter;iter++)
	{
		patternList.push_back(iter->first);
	}
}

CODPGPattern* CODPGPatternFactory::Create(CString sPatternName)
{
	map<CString,CODPGPattern*>::iterator iter;
	
	iter = m_patterns.find(sPatternName);
	if(iter != m_patterns.end())
	{
		return iter->second->Clone();
	}
	return NULL;
}

