#ifndef TIMER_CONTROL_HEAD_FILE
#define TIMER_CONTROL_HEAD_FILE

#pragma once

#include "Stdafx.h"

//ö�ٶ���

//��Ϣ����
#define	IDM_TIMER_OUT			(WM_USER+400)						//ʱ�ӵ�ʱ�¼�
//#define IDM_OUT_TIME			(WM_USER+401)
//�˿˿ؼ�
class CTimerControl : public CWnd
{
	//���ñ���
protected:
	bool							m_bDisplay;							//��ʾ��־

	//λ�ñ���
protected:
	CPoint							m_BenchmarkPos;						//��׼λ��

	//���б���
protected:
	DWORD							m_dwCurrentIndex;					//��ǰ����
	int								m_nTimer;							//��ǰʱ��
	BOOL							m_bNotify;							//�Ƿ񷢵�ʱ֪ͨ
	BOOL							m_bPause;							//�Ƿ���ͣ



	//�ڲ�����
protected:
	CWnd *							m_pSinkWindow;						//�ص�����

	//��Դ����
protected:
	CSkinImage				m_ImageTimeBack;						//ͼƬ��Դ
	CSkinImage				m_ImageArrow;						//��ͷ��Դ
	CSize					m_Size;								//��С
	BOOL					m_bDes;								//��ʱ����ʾ

	//��������
public:
	//���캯��
	CTimerControl();
	//��������
	virtual ~CTimerControl();

	//���ú���
public:
	void SetBitmap(UINT uIdBack,UINT uIdArrow);
	//���ô���
	void SetSinkWindow(CWnd * pSinkWindow);
	

	//�ؼ�����
public:
	void SetupRgn(CDC *pDC,CSkinImage bImage,COLORREF TransColor);	//͸��ɫ
	//������ʾ
	void ShowTimer(bool bDisplay);
	//���õ���ʱ��,directionΪ����,secordΪ����,bNotify�ǵ�ʱ�Ƿ�֪ͨ
	void SetTimerUp(int direction,int secord,BOOL bNotify);
	void SetPause(BOOL bPause);
	void SetBenchmarkPos(int nXPos, int nYPos);
	//��ʱ���Ƿ�Ϊ��ʱ����ʾ
	void SetDesOrder(bool bDes);

	//�ڲ�����
private:
	//����λ��
	void RectifyControl();

	//��Ϣ����
protected:
	//�ػ�����
	afx_msg void OnPaint();
	//������Ϣ
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//timer��Ϣ
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif