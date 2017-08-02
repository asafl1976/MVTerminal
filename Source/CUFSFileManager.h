#pragma once
#include <map>
#include "IPMMessageType.h"

using namespace std;

class CUFSFileManager
{
	map<UFS_FILE_HANDLER,CStdioFile*>  m_fileMap;

    // Constructor
    CUFSFileManager();

public:

    static CUFSFileManager& GetInstance();
    // Destructor
    virtual ~CUFSFileManager();

	virtual UFS_FILE_HANDLER OpenFile(CString sFile, UFS_OPEN_OPTION eOpenOption);
	virtual BOOL			 CloseFile(UFS_FILE_HANDLER nHandle);
	virtual BOOL			 ReadLine(UFS_FILE_HANDLER nHandle,CString& sLine);
	virtual BOOL			 WriteLine(UFS_FILE_HANDLER nHandle,CString sLine);

};


