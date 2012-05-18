/********************************************************************
created:	2009/10/10
filename: 	Path.h
author:		lrq
purpose:	
*********************************************************************/


//*********************************************************************
// INCLUDES
//*********************************************************************
#include "stdafx.h"
#include <direct.h>				// For getting path to current folder
#include <shlobj.h>				// For using SHGetSpecialFolderPath
#include <Shlwapi.h>			// For PathRelativePathTo function

#include "Path.h"

//*********************************************************************
// INITIALISATION
//*********************************************************************

std::auto_ptr<CPath2> CPath2::sm_inst;

CPath2*	g_pPath = CPath2::Instance(); //路径操作类相关

//*********************************************************************
// CONSTRUCTOR & DESTRUCTOR
//*********************************************************************

CPath2::CPath2()
{
	Init();
}


CPath2::~CPath2()
{
	TRACE("path2 Destructor");
}

//*********************************************************************
// PUBLIC FUNCTIONS
//*********************************************************************

CPath2 * CPath2::Instance()
{
	if (sm_inst.get() == 0)
		sm_inst.reset(new CPath2);

	return sm_inst.get();
	
}

//=====================================================================

bool CPath2::Init()
{
	TCHAR szTemp[_MAX_PATH] = {0};

	GetModuleFileName(NULL, szTemp, _MAX_PATH);
	m_sFullName = szTemp;

	// Application path
	m_sPathApp = ExtractFilePath(szTemp);
	m_sPathApp += _T("\\");
	
	// Current path
	GetCurrentDirectory(_MAX_PATH, szTemp);
	m_sPathCur = szTemp;
	m_sPathCur += _T("\\");

	// Temp path
	GetTempPath(_MAX_PATH, szTemp);
	m_sPathTemp = szTemp;
	m_sPathTemp += _T("\\");

	// System path
	GetSystemDirectory(szTemp, _MAX_PATH);
	m_sPathSystem = szTemp;
	m_sPathSystem += _T("\\");

	return true;
}

CString CPath2::GetPathTemp()
{
	return m_sPathTemp;
}


CString CPath2::GetPathSystem()
{
	return m_sPathSystem;
}


CString CPath2::GetPathApp()
{
	return m_sPathApp;
}

CString CPath2::GetPathCur()
{
	return m_sPathCur;
}


CString CPath2::ExtractFileName(CString sPath, CString sSplitter /* = _T("\\") */)
{
	// Find first
	int iPos = sPath.Find(_T('\\'), 0);

	// Search for backslash
	while (iPos != -1)
	{
		// Delete until backslash
		sPath.Delete(0, iPos+1);

		// Search again
		iPos = sPath.Find(_T('\\'), 0);
	}

	// Find first /
	iPos = sPath.Find(_T('/'), 0);

	// Search for slash
	while (iPos != -1)
	{
		// Delete until slash
		sPath.Delete(0, iPos+1);

		// Search again
		iPos = sPath.Find(_T('/'), 0);
	}

	// Return the path
	return sPath;
}

//=====================================================================

CString CPath2::ExtractFilePath(CString sPath, CString sSplitter /* = "\\" */)
{
	// Declare variables
	int iPos;

	// Now search for the last backslash
	for (int i = sPath.GetLength(); i > 0; i--)
	{
		iPos = sPath.Find(sSplitter, i);
		// If we found them, delete last piece
		if (iPos != -1)
		{
			sPath.Delete(iPos, sPath.GetLength() - i);
			return sPath;
		}
	}

	// If we not found, we are already dealing with path
	return _T("");
}

//=====================================================================

CString CPath2::ExtractDrive(CString sPath, CString sSplitter /* = "\\" */)
{
	// Declare variables
	int iPos;
	DWORD dwDriveType;

	// Search for first backslash
	iPos = sPath.Find(sSplitter);

	if (iPos != -1)
	{
		// Delete rest of path
		sPath.Delete(iPos, sPath.GetLength() - iPos);
	}

	// Check if the path is a valid drive
	sPath += _T("\\");
	dwDriveType = GetDriveType(sPath);
	if ((dwDriveType == DRIVE_UNKNOWN) || (dwDriveType == DRIVE_NO_ROOT_DIR))
	{
		return _T("");
	}

	// Return the drive
	return sPath;
}

//=====================================================================

CString CPath2::ExtractFileExtension(CString sPath)
{
	// Declare variables
	int iPos;

	// Now search for the last dot
	for (int i = sPath.GetLength(); i > 0; i--)
	{
		iPos = sPath.Find(_T("."), sPath.GetLength()-(sPath.GetLength()-i));
		// If we found them, delete last piece
		if (iPos != -1)
		{
			sPath.Delete(0, iPos);
			return sPath;
		}

		// If it is a slash, we should not use this extension anymore
		if (sPath.Find(_T("\\"), i) != -1)
			return _T("");
	}

	// If we not found, no extension is available
	return _T("");
}

//=====================================================================

CString CPath2::StripFileExtension(CString sPath)
{
	// Declare variables
	int iPos;

	// Now search for the last dot
	for (int i = sPath.GetLength(); i > 0; i--)
	{
		iPos = sPath.Find(_T("."), i);
		// If we found them, delete last piece
		if (iPos != -1)
		{
			sPath.Delete(iPos, sPath.GetLength() - i);
			return sPath;
		}

		// If it is a slash, we should not strip this extension anymore
		if (sPath.Find(_T("\\"), i) != -1)
			return sPath;
	}

	// If we not found, we are already dealing with path
	return sPath;
}

//=====================================================================

CString CPath2::ConvertRelativeToFullPath(CString sRelativePath)
{
	// Declare variables
	CString sFullPath, sBuffer;

	// Convert
	GetFullPathName(sRelativePath, _MAX_PATH, sBuffer.GetBuffer(_MAX_PATH), NULL);

	// Get value of buffer
	sFullPath = sBuffer.GetBuffer(sBuffer.GetLength());
	sBuffer.ReleaseBuffer();

	// Return result
	return sFullPath;
}

//=====================================================================

CString CPath2::ConvertFullToRelativePath(CString sFullPath)
{
	// Declare variables
	CString sRelativePath, sBuffer;
	DWORD dwPathType;

	// Check if path is pointing to file or folder
	if (PathIsDirectory(sFullPath))
	{
		dwPathType = FILE_ATTRIBUTE_DIRECTORY;
	}
	else
	{
		dwPathType = FILE_ATTRIBUTE_NORMAL;
	}

	// Convert
	PathRelativePathTo(sBuffer.GetBuffer(_MAX_PATH), m_sPathApp, FILE_ATTRIBUTE_DIRECTORY, 
		sFullPath, dwPathType);

	// Get value of buffer
	sRelativePath = sBuffer.GetBuffer(sBuffer.GetLength());
	sBuffer.ReleaseBuffer();

	// Return result
	return sRelativePath;
}

//创建如下形式的路径: "E:\\Dir1\SubDir1\"
void CPath2::CreatePath(LPCTSTR pszPath)
{
	TCHAR szDirectory[MAX_PATH]=TEXT("");
	lstrcpyn(szDirectory, pszPath, CountArray(szDirectory));

	//建立目录
	int iPos=0;

	do
	{
		if (szDirectory[iPos]==0) 
		{
			::CreateDirectory(szDirectory,NULL);
			break;
		}
		if (szDirectory[iPos]==TEXT('\\'))
		{
			szDirectory[iPos]=0;
			::CreateDirectory(szDirectory,NULL);
			szDirectory[iPos]=TEXT('\\');
		}
		iPos++;
	} while (true);
}

CString CPath2::GetFullFileName(CString sPath, CString sFileName)
{
	sFileName = sPath + "\\" + sFileName;
	sFileName.Replace(_T("\\\\"), _T("\\"));

	return ConvertRelativeToFullPath(sFileName);
}

CString CPath2::GetPathExecutable()
{
	return m_sFullName;
}

//*********************************************************************
// PRIVATE FUNCTIONS
//*********************************************************************