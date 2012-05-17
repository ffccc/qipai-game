#ifndef SYSTEM_OPTION_HEAD_FILE
#define SYSTEM_OPTION_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////

//ȫ��������
class CGlobalOption
{
	//��Ϣ����
public:
	bool							m_bShowInOutMessage;				//������Ϣ

	//��ɫ����
public:
	COLORREF						m_crChatTX;							//��������
	COLORREF						m_crMsgName;						//������Ϣ
	COLORREF						m_crMsgNormal;						//��ͨ��Ϣ
	COLORREF						m_crMsgSysHead;						//ϵͳ��Ϣ
	COLORREF						m_crMsgSysString;					//ϵͳ��Ϣ

	//��������
public:
	//���캯��
	CGlobalOption();
	//��������
	virtual ~CGlobalOption();

	//���ܺ���
public:
	//��������
	void LoadOptionParameter();
	//��������
	void SaveOptionParameter();
};

//////////////////////////////////////////////////////////////////////////

//ȫ����Ϣ 
extern	CGlobalOption					g_GlobalOption;					//ϵͳ����

//////////////////////////////////////////////////////////////////////////

#endif