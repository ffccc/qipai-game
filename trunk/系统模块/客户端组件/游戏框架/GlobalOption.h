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
	CPlatformResourceHelper			m_PlatformResourceModule;			//��Դ���

	//��Դ����
public:
	tagChatExpViewImage				m_ChatExpViewImage;					//������Դ
	tagGameFrameImage				m_GameFrameImage;					//�����Դ
	tagPropertyViewImage			m_PropertyViewImage;				//������Դ
	tagVideoFrameImage				m_VideoFrameImage;					//��Ƶ��Դ
	tagPropertyMenuImage			m_PropertyMenuImage;				//�˵���Դ
	tagGiftFlashImage				m_GiftFlashImage;					//������Դ

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
	//������Դ
	bool LoadGameFrameRes();
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