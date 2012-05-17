#ifndef STATION_HEAD_FILE
#define STATION_HEAD_FILE

#pragma once

#include "StdAfx.h"
#include "Resource.h"

#define STATION_WND_HIGHT			80			//״̬���ڸ߶� 
#define PIC_BALL_RADII				21		//ͼ��뾶
#define ID_TIMER					10			//��ʱ�� 

class CGameClientView;

//�滭��Ϣ
//״̬������
class CStation : public CWnd
{
public:
	//��������
	BYTE				m_iTargetBall[2];		//Ŀ����
	bool				m_bPowerTimer;			//�Ƿ�������ʱ��
	bool				m_bAddMode;				//�Ƿ�����ģʽ
	int					m_iPower;				//�����ٷֱ�
	int					m_iPoint[2];			//�÷�
	double				m_iXBallPos;			//X ��׼��
	double				m_iYBallPos;			//Y ��׼��
	long int			m_iBaseMoney;			//�������
	BOOL                m_IsTrain;
	double				m_iXAnglePos;			//X ��׼��
	double				m_iYAnglePos;			//Y ��׼��
	int					iPos;
	//�ؼ�����
public:
	HCURSOR				m_Cursor;				//���
	HCURSOR				m_NormalCursor;			//���
	//CGameClientView		* m_pPlayView;			//��ͼָ��

	//�ؼ�����
public:
	
	//��������
public:
	//���캯��
	CStation();
	//CGameClientView * pPlayView
	//���û����
	void SetBallPos(double iXBallPos, double iYBallPos,bool bIsAngle=false);
	//��ȡ����� X
	inline double GetBallXPos() { return m_iYBallPos/PIC_BALL_RADII; };
	//��ȡ����� Y
	inline double GetBallYPos() { return m_iXBallPos/PIC_BALL_RADII; };
	//��������
	void SetPower(int iPower);
	//��ȡ����
	inline int GetPower() { return __max(0,__min(m_iPower,100)); };
	//��ʼ����
	void BeginPower();
	//ֹͣ����
	void EndPower();
	//���÷���
	void SetUserPoint(int iViewStation, int Point, bool bAdd);
	//����Ŀ����
	void SetTargertBall(int iViewStation, BYTE bColor);
	//�滭ͼƬ
	void DrawBmp(CDC * pDC, int x, int y, UINT uBmpID, UINT uType, CRect * pRect, int xSrc=0, int ySrc=0, int iWidth=0, COLORREF rcTranColor=RGB(0,0,0));

	//��Ϣ����
protected:
	//�ػ�����
	afx_msg void OnPaint();
	//�����Ϣ
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//����Ҽ���Ϣ
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//��ʱ����Ϣ
	afx_msg void OnTimer(UINT nIDEvent);

	DECLARE_MESSAGE_MAP()

};

#endif