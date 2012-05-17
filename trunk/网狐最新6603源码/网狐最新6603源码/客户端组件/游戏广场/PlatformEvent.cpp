#include "StdAfx.h"
#include "PlatformEvent.h"

//////////////////////////////////////////////////////////////////////////////////
//��̬����

CPlatformEvent * CPlatformEvent::m_pPlatformEvent=NULL;					//����ָ��

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CPlatformEvent::CPlatformEvent()
{
	//���ö���
	ASSERT(m_pPlatformEvent==NULL);
	if (m_pPlatformEvent==NULL) m_pPlatformEvent=this;

	return;
}

//��������
CPlatformEvent::~CPlatformEvent()
{
	//�ͷŶ���
	ASSERT(m_pPlatformEvent==this);
	if (m_pPlatformEvent==this) m_pPlatformEvent=NULL;

	return;
}

//ע�ᴰ��
bool CPlatformEvent::RegisterEventWnd(HWND hEventWnd)
{
	//Ч�����
	ASSERT(IsWindow(hEventWnd));
	if (IsWindow(hEventWnd)==FALSE) return false;

	//�����ж�
	for (INT_PTR i=0;i<m_EventWndArray.GetCount();i++)
	{
		if (m_EventWndArray[i]==hEventWnd)
		{
			ASSERT(FALSE);
			return false;
		}
	}

	//��������
	m_EventWndArray.Add(hEventWnd);

	return true;
}

//ע������
bool CPlatformEvent::UnRegisterEventWnd(HWND hEventWnd)
{
	//Ч�����
	ASSERT(IsWindow(hEventWnd));
	if (IsWindow(hEventWnd)==FALSE) return false;

	//ɾ���ж�
	for (INT_PTR i=0;i<m_EventWndArray.GetCount();i++)
	{
		if (m_EventWndArray[i]==hEventWnd)
		{
			m_EventWndArray.RemoveAt(i);
			return true;
		}
	}

	//�������
	ASSERT(FALSE);

	return false;
}

//�����¼�
VOID CPlatformEvent::SendPlatformEvent(WORD wEventID, LPARAM lParam)
{
	//�����¼�
	for (INT_PTR i=0;i<m_EventWndArray.GetCount();i++)
	{
		HWND hWnd=m_EventWndArray[i];
		if (IsWindow(hWnd)) SendMessage(hWnd,WM_PLATFORM_EVENT,wEventID,lParam);
	}

	return;
}

//�����¼�
VOID CPlatformEvent::PostPlatformEvent(WORD wEventID, LPARAM lParam)
{
	//�����¼�
	for (INT_PTR i=0;i<m_EventWndArray.GetCount();i++)
	{
		HWND hWnd=m_EventWndArray[i];
		if (IsWindow(hWnd)) PostMessage(hWnd,WM_PLATFORM_EVENT,wEventID,lParam);
	}

	return;
}

//////////////////////////////////////////////////////////////////////////////////
