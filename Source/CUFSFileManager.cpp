#include "StdAfx.h"
#include "CUFSFileManager.h"

CUFSFileManager::CUFSFileManager()
{
}


CUFSFileManager::~CUFSFileManager()
{
}

CUFSFileManager& CUFSFileManager::GetInstance()
{
    static CUFSFileManager manager;
    return manager;
}


UFS_FILE_HANDLER  CUFSFileManager::OpenFile(CString sFile, UFS_OPEN_OPTION eOpenOption)
{
	CStdioFile* pFile = new CStdioFile();
	UINT nOpenFlags;
	switch(eOpenOption)
	{
	case UFS_READ:
		nOpenFlags = CFile::modeRead | CFile::typeText;
		break;
	case UFS_READ_WRITE:
		nOpenFlags = CFile::modeReadWrite | CFile::typeText;
		break;
	case UFS_CREATE:
		nOpenFlags = CFile::modeCreate | CFile::modeReadWrite | CFile::typeText;
		break;
	default:
		nOpenFlags = CFile::modeRead | CFile::typeText;
		break;
	}
	
	if(pFile->Open(sFile, nOpenFlags ) == FALSE)
	{
		AfxMessageBox("Error in opening file " + sFile,NULL,MB_ICONERROR);
		return -1;
	}
	//Handler == ptr
	m_fileMap[(UFS_FILE_HANDLER)pFile] = pFile;
	return (UFS_FILE_HANDLER)pFile;
}

BOOL    CUFSFileManager::CloseFile(UFS_FILE_HANDLER nHandle)
{
	map<UFS_FILE_HANDLER,CStdioFile*>::iterator iter = m_fileMap.find(nHandle);
	if(iter == m_fileMap.end())
	{
		AfxMessageBox("Error in read line from file!",NULL,MB_ICONERROR);
		return FALSE;
	}
	CStdioFile* pFile = iter->second;
	if(pFile == NULL)
	{
		AfxMessageBox("Error in read line from file!",NULL,MB_ICONERROR);
		return FALSE;
	}
	pFile->Close();
	return FALSE;
}

BOOL CUFSFileManager::ReadLine(UFS_FILE_HANDLER nHandle, CString & sLine)
{
	map<UFS_FILE_HANDLER,CStdioFile*>::iterator iter = m_fileMap.find(nHandle);
	if(iter == m_fileMap.end())
	{
		AfxMessageBox("Error in read line from file!",NULL,MB_ICONERROR);
		return FALSE;
	}
	CStdioFile* pFile = iter->second;
	if(pFile == NULL)
	{
		AfxMessageBox("Error in read line from file!",NULL,MB_ICONERROR);
		return FALSE;
	}
	return pFile->ReadString(sLine);
}

BOOL	CUFSFileManager::WriteLine(UFS_FILE_HANDLER nHandle,CString sLine)
{
	map<UFS_FILE_HANDLER,CStdioFile*>::iterator iter = m_fileMap.find(nHandle);
	if(iter == m_fileMap.end())
	{
		AfxMessageBox("Error in write line of file!",NULL,MB_ICONERROR);
		return FALSE;
	}
	CStdioFile* pFile = iter->second;
	if(pFile == NULL)
	{
		AfxMessageBox("Error in write line of file!",NULL,MB_ICONERROR);
		return FALSE;
	}
	pFile->WriteString(sLine);
	return TRUE;
}
