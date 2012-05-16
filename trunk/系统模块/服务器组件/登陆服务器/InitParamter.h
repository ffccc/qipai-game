#ifndef INIT_PARAMTER_HEAD_FILE
#define INIT_PARAMTER_HEAD_FILE

#pragma once

#include "Commonality.h"

//////////////////////////////////////////////////////////////////////////

//���ò���
class CInitParamter
{
	//��������
public:
	WORD							m_wListenPort;						//�����˿�
	WORD							m_wMaxConnect;						//�������
	TCHAR							m_szMainPage[32];					//վ��ҳ��
	TCHAR							m_szCenterServerAddr[16];			//���ķ�����

	//��¼���ݿ�
public:
	WORD							m_wUserDataBasePort;				//���ݿ�˿�
	TCHAR							m_szUserDataBaseAddr[16];			//���ݿ��ַ
	TCHAR							m_szUserDataBaseUser[32];			//���ݿ��û�
	TCHAR							m_szUserDataBasePass[32];			//���ݿ�����
	TCHAR							m_szUserDataBaseName[32];			//���ݿ�����

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