#ifndef WIN_FILE_INFO_HEAD_FILE
#define WIN_FILE_INFO_HEAD_FILE

#pragma once

#include "ComService.h"

//////////////////////////////////////////////////////////////////////////

//�ļ��汾�Ա���
class COM_SERVICE_CLASS CWinFileInfo
{ 
	//��������
protected:
	DWORD							m_dwLangCharset;					//���Ա�ʶ
	DWORD							m_dwDataSize;						//���ݴ�С
	BYTE							m_cbInfoBuffer[4096];				//�ļ���Ϣ
	VS_FIXEDFILEINFO				* m_pFixedFileInfo;					//�ļ���Ϣ

	//��������
public: 
	//���캯��
    CWinFileInfo();
	//��������
    virtual ~CWinFileInfo(); 

	//���ܺ���
public:
	//���ļ�
	bool OpenWinFile(LPCTSTR lpszFileName);
	//��ѯ��Ϣ
    LPCTSTR QueryValue(LPCTSTR lpszValueName);

	//�汾����
public:
	//��ȡ�汾
    bool GetFileVersion(DWORD & dwFileVersionMS, DWORD & dwFileVersionLS);
	//��ȡ�汾
    bool GetProductVersion(DWORD & dwProductVersionMS, DWORD & dwProductVersionLS);

	//��չ��Ϣ
public:
	//�ļ���Ϣ
	VS_FIXEDFILEINFO * const GetFixedFileInfo() { return m_pFixedFileInfo; }
	//�ļ��汾
    LPCTSTR GetFileVersion() { return QueryValue(TEXT("FileVersion")); }
	//��Ʒ�汾
    LPCTSTR GetProductVersion() { return QueryValue(TEXT("ProductVersion")); }
	//�ڲ�����
    LPCTSTR GetInternalName() { return QueryValue(TEXT("InternalName")); }
	//��˾����
    LPCTSTR GetCompanyName() { return QueryValue(TEXT("CompanyName")); } 
	//��Ȩ˵��
    LPCTSTR GetLegalCopyright() { return QueryValue(TEXT("LegalCopyright")); }
	//ԭʼ����
    LPCTSTR GetOriginalFilename() { return QueryValue(TEXT("OriginalFilename")); }
	//��Ʒ����
    LPCTSTR GetProductName() { return QueryValue(TEXT("ProductName")); };
	//�ļ�����
    LPCTSTR GetFileDescription()  { return QueryValue(TEXT("FileDescription")); }
}; 

//////////////////////////////////////////////////////////////////////////

#endif