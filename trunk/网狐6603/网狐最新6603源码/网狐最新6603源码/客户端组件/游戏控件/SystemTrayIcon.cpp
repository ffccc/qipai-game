#include "StdAfx.h"
#include "SystemTrayIcon.h"

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CSystemTrayIcon::CSystemTrayIcon()
{
	//���Ա���
	m_uTrayID=0L;
	m_bIconVisible=false;

	//���ñ���
	m_hWndSend=NULL;
	m_hIconTray=NULL;
	m_uMessage=WM_TRAY_ICON;

	return;
}

//��������
CSystemTrayIcon::~CSystemTrayIcon()
{
	//����ͼ��
	HideTrayIcon();

	return;
}

//����ͼ��
bool CSystemTrayIcon::InitTrayIcon(HWND hWndSend, UINT uTrayID, UINT uMessage)
{
	//���ñ���
	m_uTrayID=uTrayID;
	m_hWndSend=hWndSend;
	m_uMessage=uMessage;

	return true;
}

//����ͼ��
bool CSystemTrayIcon::HideTrayIcon()
{
	//ɾ��ͼ��
	if (m_bIconVisible==true)
	{
		//֪ͨ����
		NOTIFYICONDATA NotifyIconData;
		ZeroMemory(&NotifyIconData,sizeof(NotifyIconData));

		//���ñ���
		NotifyIconData.cbSize=sizeof(NOTIFYICONDATA);
		NotifyIconData.uID=m_uTrayID;
		NotifyIconData.hWnd=m_hWndSend;
		NotifyIconData.uFlags=NIF_ICON|NIF_MESSAGE;

		//����ͼ��
		m_bIconVisible=false;
		Shell_NotifyIcon(NIM_DELETE,&NotifyIconData);
	}

	return true;
}

//��ʾͼ��
bool CSystemTrayIcon::ShowTrayIcon(HICON hIcon)
{
	//���ñ���
	m_hIconTray=hIcon;

	//֪ͨ����
	NOTIFYICONDATA NotifyIconData;
	ZeroMemory(&NotifyIconData,sizeof(NotifyIconData));

	//��������
	NotifyIconData.cbSize=sizeof(NOTIFYICONDATA);
	NotifyIconData.uID=m_uTrayID;
	NotifyIconData.hWnd=m_hWndSend;
	NotifyIconData.hIcon=m_hIconTray;
	NotifyIconData.uFlags=NIF_ICON;

	//����ͼ��
	if (m_bIconVisible==false)
	{
		m_bIconVisible=true;
		Shell_NotifyIcon(NIM_ADD,&NotifyIconData);
	}
	else
	{
		Shell_NotifyIcon(NIM_MODIFY,&NotifyIconData);
	}

	return true;
}

//��ʾͼ��
bool CSystemTrayIcon::ShowTrayIcon(HICON hIcon, LPCTSTR pszDescribe)
{
	//���ñ���
	m_hIconTray=hIcon;

	//֪ͨ����
	NOTIFYICONDATA NotifyIconData;
	ZeroMemory(&NotifyIconData,sizeof(NotifyIconData));

	//��������
	NotifyIconData.cbSize=sizeof(NOTIFYICONDATA);

	//ͼ������
	NotifyIconData.uID=m_uTrayID;
	NotifyIconData.hWnd=m_hWndSend;
	NotifyIconData.hIcon=m_hIconTray;
	NotifyIconData.uCallbackMessage=m_uMessage;
	NotifyIconData.uFlags=NIF_ICON|NIF_MESSAGE|NIF_TIP;
	lstrcpyn(NotifyIconData.szTip,pszDescribe,CountArray(NotifyIconData.szTip));

	//����ͼ��
	if (m_bIconVisible==false)
	{
		m_bIconVisible=true;
		Shell_NotifyIcon(NIM_ADD,&NotifyIconData);
	}
	else
	{
		Shell_NotifyIcon(NIM_MODIFY,&NotifyIconData);
	}

	return true;
}

//��ʾ��ʾ
bool CSystemTrayIcon::ShowBalloonTip(LPCTSTR szMesssage, LPCTSTR szTitle)
{
	return true;
}

//��ʾ��ʾ
bool CSystemTrayIcon::ShowBalloonTip(LPCTSTR szMesssage, LPCTSTR szTitle, DWORD dwInfoFlags, UINT uTimeOut)
{
	return true;
}

//////////////////////////////////////////////////////////////////////////////////
