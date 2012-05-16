#ifndef INIR_PARAMTER_HEAD_FILE
#define INIR_PARAMTER_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////

//���ò���
class CInitParamter
{
	//��������
public:
	WORD							m_wListenPort;						//�����˿�
	WORD							m_wMaxConnect;						//�������

	//��Ϣ���ݿ�
public:
	WORD							m_wServerDataBasePort;				//���ݿ�˿�
	TCHAR							m_szServerDataBaseAddr[16];			//���ݿ��ַ
	TCHAR							m_szServerDataBaseUser[32];			//���ݿ��û�
	TCHAR							m_szServerDataBasePass[32];			//���ݿ�����
	TCHAR							m_szServerDataBaseName[32];			//���ݿ�����

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
