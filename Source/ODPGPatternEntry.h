#pragma once
#include <vector>
using namespace std;


class CODPGPatternEntry
{	
	vector<UCHAR>   m_dataVec;

public:
	CODPGPatternEntry(void);
	virtual ~CODPGPatternEntry(void);

public:
	void Empty()
	{
		m_dataVec.clear();
	}
	void AddByte(UCHAR nByte)
	{
		m_dataVec.push_back(nByte);
		//m_dataVec.insert(m_dataVec.end(),nByte);

	}
	int	 GetByteCount()
	{
		return (int)m_dataVec.size();
	}
	UCHAR	 GetByte(int nIndex)
	{
		if(nIndex < GetByteCount())
		{
			return m_dataVec[nIndex];
		}
		return -1;
	}
	void	 SetByte(int nIndex, UCHAR nData)
	{
		if(nIndex < GetByteCount())
		{
			m_dataVec[nIndex] = nData;
		}
	}
	CString  ToString()
	{
		CString sValue,sByte;

		vector<UCHAR>::iterator iter, endIter;
		iter    = m_dataVec.begin();
		endIter = m_dataVec.end();
		for(; iter != endIter; iter++)
		{
			sByte.Format("%02x",(*iter));
			sValue += sByte;
		}
		return sValue;
	}

};