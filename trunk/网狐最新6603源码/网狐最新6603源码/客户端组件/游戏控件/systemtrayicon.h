#ifndef SYSTEM_TRAY_ICON_HEAD_FILE
#define SYSTEM_TRAY_ICON_HEAD_FILE

#pragma once

#include "ShareControlHead.h"

//////////////////////////////////////////////////////////////////////////////////
//�궨��

#define WM_TRAY_ICON				(WM_USER+100)						//Ĭ����Ϣ

//////////////////////////////////////////////////////////////////////////////////

//�¼�ͼ��
class SHARE_CONTROL_CLASS CSystemTrayIcon
{
	//���ö���
protected:
	UINT							m_uMessage;							//������Ϣ
	HWND							m_hWndSend;							//���ʹ���
	HICON							m_hIconTray;						//����ͼ��

	//���Ա���
protected:
	UINT							m_uTrayID;							//���̱�ʶ
	bool							m_bIconVisible;						//���ӱ�־

	//��������
public:
	//���캯��
	CSystemTrayIcon();
	//��������
	virtual ~CSystemTrayIcon();

	//���ú���
public:
	//����ͼ��
	bool InitTrayIcon(HWND hWndSend, UINT uTrayID, UINT uMessage=WM_TRAY_ICON);

	//ͼ�꺯��
public:
	//����ͼ��
	bool HideTrayIcon();
	//��ʾͼ��
	bool ShowTrayIcon(HICON hIcon);
	//��ʾͼ��
	bool ShowTrayIcon(HICON hIcon, LPCTSTR pszDescribe);

	//��ʾ����
public:
	//��ʾ��ʾ
	bool ShowBalloonTip(LPCTSTR szMesssage, LPCTSTR szTitle);
	//��ʾ��ʾ
	bool ShowBalloonTip(LPCTSTR szMesssage, LPCTSTR szTitle, DWORD dwInfoFlags, UINT uTimeOut);
};

//////////////////////////////////////////////////////////////////////////////////

#endif