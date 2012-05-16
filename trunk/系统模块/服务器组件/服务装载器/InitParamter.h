#ifndef INIT_PARAMTER_HEAD_FILE
#define INIT_PARAMTER_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////

//���ò���
class CInitParamter
{
	//��������
public:
	TCHAR							m_szAccreditID[39];					//��Ȩ����
	TCHAR							m_szCenterServerAddr[16];			//���ķ�����

	//�û����ݿ�
public:
	WORD							m_wGameUserDBPort;					//���ݿ�˿�
	TCHAR							m_szGameUserDBAddr[16];				//���ݿ��ַ
	TCHAR							m_szGameUserDBUser[32];				//���ݿ��û�
	TCHAR							m_szGameUserDBPass[32];				//���ݿ�����
	TCHAR							m_szGameUserDBName[32];				//���ݿ�����

	//��Ϸ���ݿ�
public:
	WORD							m_wServerInfoDBPort;				//���ݿ�˿�
	TCHAR							m_szServerInfoDBAddr[16];			//���ݿ��ַ
	TCHAR							m_szServerInfoDBUser[32];			//���ݿ��û�
	TCHAR							m_szServerInfoDBPass[32];			//���ݿ�����
	TCHAR							m_szServerInfoDBName[32];			//���ݿ�����

	//��������
public:
	//���캯��
	CInitParamter();
	//��������
	virtual ~CInitParamter();

	//���ܺ���
public:
	//��ʼ��
	void InitParamter();
	//��������
	void LoadInitParamter();
	//��������
	void SaveInitParamter(bool bSaveAll);
};

//////////////////////////////////////////////////////////////////////////

#endif