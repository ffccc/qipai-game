#ifndef THREAD_DRAW_HEAD_FILE
#define THREAD_DRAW_HEAD_FILE

#pragma once

#include "GameFrame.h"

//�궨��
#define THREAD_TIME_ID					1								//��ʱ�� ID

//////////////////////////////////////////////////////////////////////////

//�滭�߳�ģ��
class GAME_FRAME_CLASS CThreadDraw
{
	//�̱߳���
private:
	bool								m_bExit;						//�˳���־
	UINT								m_uSpeed;						//��ʾ֡��
	UINT								m_uTimeSpace;					//���ʱ��
	UINT								m_uDrawSpace;					//���ʱ��
	HANDLE								m_hDrawThreadHandle;			//�߳̾��
	HANDLE								m_hTimeThreadHandle;			//�߳̾��

	//״̬��Ϣ
protected:
	int									m_nClientWidth;					//���ڿ��
	int									m_nClientHeight;				//���ڸ߶�
	CWnd								* const m_pGameView;			//��Ϸ��ͼ

	//��������
public:
	//���캯��
	CThreadDraw(CWnd * pGameView);
	//��������
	virtual ~CThreadDraw();

	//���غ���
public:
	//��ʼ���滭
	virtual bool InitDraw(CDC * pDrawDC);
	//���ٻ滭
	virtual bool UnInitDraw();
	//�滭����
	virtual void ThreadDrawView(CDC * pDC, int nClinetWith, int nClinetHeight)=NULL;

	//���ܺ���
public:
	//��ȡ֡��
	UINT GetSpeed() { return m_uSpeed; }
	//���÷�Χ
	void SetClientRange(int nClientWidth, int nClientHeight);
	//���ö�ʱ�����
	void SetTimeSpace(UINT uTimeSpace) { m_uTimeSpace=uTimeSpace; };
	//���� GDI �̼߳��
	void SetDrawSpace(UINT uDrawSpace) { m_uDrawSpace=uDrawSpace; };

	//�̺߳���
public:
	//��ʼ�߳�
	bool BeginThread();
	//�����߳�
	bool EndThread();
	//ֹͣ�߳�
	void SuspendDrawThread();
	//�����߳�
	void ResumeDrawThread();
	//��ͼ�߳�
	static unsigned __stdcall ThreadDraw(LPVOID pThreadData);
	//��ʱ���߳�
	static unsigned __stdcall ThreadTimer(LPVOID pThreadData);
};

//////////////////////////////////////////////////////////////////////////

#endif