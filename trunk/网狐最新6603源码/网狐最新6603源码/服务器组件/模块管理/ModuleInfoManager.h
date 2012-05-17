#ifndef MODULE_INFO_MANAGER_HEAD_FILE
#define MODULE_INFO_MANAGER_HEAD_FILE

#pragma once

#include "AfxTempl.h"
#include "ModuleManagerHead.h"

//////////////////////////////////////////////////////////////////////////////////

//���鶨��
typedef CWHArray<tagGameModuleInfo *> CGameModuleInfoArray;

//��������
typedef CMap<WORD,WORD,tagGameModuleInfo *,tagGameModuleInfo *> CGameModuleInfoMap;

//////////////////////////////////////////////////////////////////////////////////

//ģ������
class MODULE_MANAGER_CLASS CModuleInfoBuffer
{
	//��������
public:
	CGameModuleInfoMap				m_GameModuleInfoMap;				//ģ������
	CGameModuleInfoArray			m_GameModuleInfoArray;				//ģ������

	//��������
public:
	//���캯��
	CModuleInfoBuffer();
	//��������
	virtual ~CModuleInfoBuffer();

	//������
public:
	//��������
	bool ResetModuleInfo();
	//ɾ������
	bool DeleteModuleInfo(WORD wModuleID);
	//��������
	bool InsertModuleInfo(tagGameModuleInfo * pGameModuleInfo);

	//��Ϣ����
public:
	//��ȡ��Ŀ
	DWORD GetModuleInfoCount();
	//��������
	tagGameModuleInfo * SearchModuleInfo(WORD wModuleID);

	//�ڲ�����
private:
	//��������
	tagGameModuleInfo * CreateModuleInfo();
};

//////////////////////////////////////////////////////////////////////////////////

//ģ����Ϣ
class MODULE_MANAGER_CLASS CModuleInfoManager
{
	//��������
public:
	//���캯��
	CModuleInfoManager();
	//��������
	virtual ~CModuleInfoManager();

	//ע�����
public:
	//����ģ��
	bool RegisterGameModule(LPCTSTR pszModuleName);
	//ע��ģ��
	bool UnRegisterGameModule(LPCTSTR pszModuleName);

	//���ݹ���
public:
	//����ģ��
	bool LoadGameModuleInfo(CModuleInfoBuffer & ModuleInfoBuffer);

	//ģ����Ϣ
public:
	//ģ������
	bool GetGameServiceAttrib(LPCTSTR pszModuleName, tagGameServiceAttrib & GameServiceAttrib);
};

//////////////////////////////////////////////////////////////////////////////////

#endif