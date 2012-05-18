/********************************************************************
	created:	2009/10/10
	filename: 	Path.h
	author:		lrq
	purpose:	
*********************************************************************/

//Redefinition protection
#ifndef _PATH_H_
#define _PATH_H_

//*********************************************************************
// INCLUDES
//*********************************************************************
#include <memory>				// Needed for singleton class


using namespace std;


//*********************************************************************
// CPath2
//*********************************************************************

class CPath2  
{
private:
	bool    Init();

public:
	// Functions
	static CPath2 * Instance();
	CString GetPathTemp();
	CString GetPathSystem();
	CString GetPathApp();
	CString GetPathCur();
	CString GetPathExecutable();
	CString ExtractFileName(CString sPath, CString sSplitter = _T("\\"));
	CString ExtractFilePath(CString sPath, CString sSplitter = _T("\\"));
	CString ExtractDrive(CString sPath, CString sSplitter = _T("\\"));
	CString ExtractFileExtension(CString sPath);
	CString StripFileExtension(CString sPath);
	CString ConvertRelativeToFullPath(CString sRelativePath);
	CString ConvertFullToRelativePath(CString sFullPath);
	CString GetFullFileName(CString sPath, CString sFileName);
	void CreatePath(LPCTSTR pszPath);
	~CPath2();
private:
	// Constructor & Destructor
	CPath2();			//Private constructor because singleton!
	
	// Variables
	static auto_ptr<CPath2> sm_inst;
	CString m_sFullName;						// full file name to executable
	CString m_sPathApp;							// Path to application
	CString m_sPathCur;							// Path to current
	CString m_sPathSystem;						// Path to system
	CString m_sPathTemp;						// Path to window temp
};

extern	CPath2*	g_pPath;					//路径操作类相关
#endif // _PATH_H_