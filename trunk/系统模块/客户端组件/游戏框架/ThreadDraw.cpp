#include "StdAfx.h"
#include "ThreadDraw.h"

//////////////////////////////////////////////////////////////////////////

//���캯��
CThreadDraw::CThreadDraw(CWnd * pGameView) : m_pGameView(pGameView)
{
	m_uSpeed=0L;
	m_bExit=false;
	m_uTimeSpace=10;
	m_uDrawSpace=10;
	m_nClientWidth=0;
	m_nClientHeight=0;
	m_hDrawThreadHandle=NULL;
	m_hTimeThreadHandle=NULL;
}

//��������
CThreadDraw::~CThreadDraw()
{
	EndThread();
}

//���÷�Χ
void CThreadDraw::SetClientRange(int nClientWidth, int nClientHeight) 
{ 
	m_nClientWidth=nClientWidth;
	m_nClientHeight=nClientHeight;
	return;
}

//��ʼ����ͼ����
bool CThreadDraw::InitDraw(CDC * pDrawDC)
{
	//���ñ���
	m_uSpeed=0L;
	m_uTimeSpace=10;
	m_uDrawSpace=10;

	return true;
}

//ȡ����ʼ������
bool CThreadDraw::UnInitDraw()
{
	//���ñ���
	m_uSpeed=0L;
	m_uTimeSpace=10L;
	m_uDrawSpace=10L;
	m_nClientWidth=0L;
	m_nClientHeight=0L;

	return true;
}

//ֹͣ�߳�
void CThreadDraw::SuspendDrawThread()
{
	if (m_hDrawThreadHandle!=NULL) ::SuspendThread(m_hDrawThreadHandle);
}
	
//�����߳�
void CThreadDraw::ResumeDrawThread()
{
	if (m_hDrawThreadHandle!=NULL) ::ResumeThread(m_hDrawThreadHandle);
}

//��ʼ�߳�
bool CThreadDraw::BeginThread()
{
	try
	{
		//�����߳�
		m_bExit=false;
		UINT dwThreadID=0;
		m_hDrawThreadHandle=(HANDLE)_beginthreadex(NULL,0,ThreadDraw,this,CREATE_SUSPENDED,&dwThreadID);
		if (m_hDrawThreadHandle==NULL) throw TEXT("�滭�߳���������");
		SetThreadPriority(m_hDrawThreadHandle,THREAD_PRIORITY_IDLE);
		m_hTimeThreadHandle=(HANDLE)_beginthreadex(NULL,0,ThreadTimer,this,CREATE_SUSPENDED,&dwThreadID);
		if (m_hTimeThreadHandle==NULL) throw TEXT("��ʱ���߳���������");
		::ResumeThread(m_hDrawThreadHandle);
		::ResumeThread(m_hTimeThreadHandle);
	}
	catch (...)
	{
		EndThread();
		return false;
	}

	return true;
}

//�����߳�
bool CThreadDraw::EndThread()
{
	//�����߳�
	m_bExit=true;
	if (m_hDrawThreadHandle!=NULL)
	{
		if (WaitForSingleObject(m_hDrawThreadHandle,1000)==WAIT_TIMEOUT)
		{
			ASSERT(FALSE);
			TerminateThread(m_hDrawThreadHandle,1);
		}
		CloseHandle(m_hDrawThreadHandle);
		m_hDrawThreadHandle=NULL;
	}

	//�����߳�
	if (m_hTimeThreadHandle!=NULL)
	{
		if (WaitForSingleObject(m_hTimeThreadHandle,1000)==WAIT_TIMEOUT) 
		{
			ASSERT(FALSE);
			TerminateThread(m_hTimeThreadHandle,1);
		}
		CloseHandle(m_hTimeThreadHandle);
		m_hTimeThreadHandle=NULL;
	}

	return true;
}

//��ͼ�߳�
unsigned __stdcall CThreadDraw::ThreadDraw(LPVOID pThreadData)
{
	//��ʼ������
	CDC	DCBuffer;
	CBitmap	BufferBmp;
	CThreadDraw * pThreadDraw=(CThreadDraw *)pThreadData;
	CDC * pDrawDC=pThreadDraw->m_pGameView->GetDC();

	try
	{
		//λ����Ϣ
		int nClientWidth=0;
		int nClientHeight=0;

		//����˫����
		DCBuffer.CreateCompatibleDC(pDrawDC);
		DCBuffer.SetBkMode(TRANSPARENT);
		if (pThreadDraw->InitDraw(&DCBuffer)==false) throw TEXT("��ʼ������");

		//֡������
		int uSpeed=0;
		DWORD dwBeginTime=GetTickCount();
		DWORD dwEndTime=dwBeginTime;

		//ѭ���滭
		while (true)
		{
			//Ч��λͼ��С
			if ((pThreadDraw->m_nClientWidth>nClientWidth)||(pThreadDraw->m_nClientHeight>nClientHeight))
			{
				nClientWidth=__max(nClientWidth,pThreadDraw->m_nClientWidth);
				nClientHeight=__max(nClientHeight,pThreadDraw->m_nClientHeight);
				BufferBmp.Detach();
				BufferBmp.CreateCompatibleBitmap(pDrawDC,nClientWidth,nClientHeight);
				DeleteObject(DCBuffer.SelectObject(&BufferBmp));
			}

			//�滭����
			pThreadDraw->ThreadDrawView(&DCBuffer,nClientWidth,nClientHeight);
			pDrawDC->BitBlt(0,0,nClientWidth,nClientHeight,&DCBuffer,0,0,SRCCOPY);

			//����֡��
			uSpeed++;
			dwEndTime=GetTickCount();
			if ((dwEndTime-dwBeginTime)>=1000L)
			{
				pThreadDraw->m_uSpeed=uSpeed;
				dwBeginTime=dwEndTime;
				uSpeed=0;
			}

			//���ֹͣ
			if (pThreadDraw->m_bExit==true) break;
			Sleep(pThreadDraw->m_uDrawSpace);
		}
	}
	catch (...) {}

	//��������
	pThreadDraw->m_pGameView->ReleaseDC(pDrawDC);
	DCBuffer.DeleteDC();
	BufferBmp.DeleteObject();
	pThreadDraw->UnInitDraw();

	return 0;
}

//��ʱ���߳�
unsigned __stdcall CThreadDraw::ThreadTimer(LPVOID pThreadData)
{
	//�������
	CThreadDraw * pThreadDraw=(CThreadDraw *)pThreadData;
	CWnd * pWndHandle=pThreadDraw->m_pGameView->GetParent();

	//ѭ����ʱ��
	try
	{
		while (true)
		{
			//������Ϣ
			Sleep(pThreadDraw->m_uTimeSpace);
			pWndHandle->PostMessage(WM_TIMER,THREAD_TIME_ID,0);

			//���ֹͣ
			if (pThreadDraw->m_bExit==true) break;
		}
	}
	catch (...) {}

	return 0;
}

//////////////////////////////////////////////////////////////////////////
