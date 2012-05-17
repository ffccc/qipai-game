#ifndef PLAT_FORM_EVENT_HEAD_FILE
#define PLAT_FORM_EVENT_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////////////
//�¼�����

//�û��¼�
#define EVENT_USER_LOGON			1									//��¼�ɹ�
#define EVENT_USER_LOGOUT			2									//ע���ɹ�
#define EVENT_USER_INFO_UPDATE		3									//���ϸ���
#define EVENT_USER_MOOR_MACHINE		4									//�󶨻���

//ϵͳ��Ϣ
#define EVENT_DOWN_LOAD_FINISH		20									//���سɹ�
#define EVENT_SKIN_CONFIG_UPDATE	21									//��Դ����

//////////////////////////////////////////////////////////////////////////////////

//֪ͨ��Ϣ
#define WM_PLATFORM_EVENT			(WM_USER+1000)						//ƽ̨�¼�

//////////////////////////////////////////////////////////////////////////////////

//�¼�����
class CPlatformEvent
{
	//��������
protected:
	CWHArray<HWND>					m_EventWndArray;					//�¼�����

	//��̬����
protected:
	static CPlatformEvent *			m_pPlatformEvent;					//����ָ��

	//��������
public:
	//���캯��
	CPlatformEvent();
	//��������
	virtual ~CPlatformEvent();

	//ע�ᴰ��
public:
	//ע�ᴰ��
	bool RegisterEventWnd(HWND hEventWnd);
	//ע������
	bool UnRegisterEventWnd(HWND hEventWnd);

	//�����¼�
public:
	//�����¼�
	VOID SendPlatformEvent(WORD wEventID, LPARAM lParam);
	//�����¼�
	VOID PostPlatformEvent(WORD wEventID, LPARAM lParam);

	//��̬����
public:
	//��ȡ����
	static CPlatformEvent * GetInstance() { return m_pPlatformEvent; }
};

//////////////////////////////////////////////////////////////////////////////////

#endif