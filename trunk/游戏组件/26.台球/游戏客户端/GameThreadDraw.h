#ifndef THREADDRAW_HEAD_FILE
#define THREADDRAW_HEAD_FILE

#pragma once

#include "StdAfx.h"
#include "Ball.h"
#include "Station.h"

struct DrawInfoStruct
{

	//��������
	double						iPower;					//����
	bool						bOkSet;					//�Ƿ��������
	BYTE						bStation;				//״̬
	POINT						CursorPos;				//���λ��
	RECT						ClientRect;				//�ͻ���λ��
	CBall						* pBallList;			//������
	POINT						OtherPolePos;			//�����������

	//һ���Գ�ʼ������ 
	double						dwTimes;				//ͼ������ϵ��
	CBallDesk					* pDesk;				//����ָ��
	CGameClientView				* pPlayView;			//��ͼָ��

	//�߳�ʹ�ò���
	long int					dwSpeed;				//��ʾ֡��
	CFont						BigFont;				//������
	CFont						LittleFont;				//С����
	BITMAP						BackBmpInfo;			//��ͼ��Ϣ
	BYTE                        m_StarPos;              //�ر��������ǵ�λ�ã�
	BYTE                        m_StarPoint;             //��������
	int 					    Degree;					//�Ƕ�
	double                      m_iPowerPos;
};
class CGameThreadDraw : public CThreadDraw
{
public:
	//���캯��
	CGameThreadDraw(CWnd * pGameView);
	virtual ~CGameThreadDraw();

	//���غ���
public:
	//��ʼ���滭
	virtual bool InitDraw(CDC * pDrawDC);
	//���ٻ滭
	virtual bool UnInitDraw();
	//�滭����
	virtual void ThreadDrawView(CDC * pDC, int nClinetWith, int nClinetHeight);
	void GDIModeDraw(CDC * pDC);
	void GDIModeInitThread();
private:
	CSkinImage    m_DeskImage;
	CSkinImage	  m_BallImage;
	CSkinImage	  m_BallAImage;
	CSkinImage	  m_BallInBagImage;
	CSkinImage	  m_PoleImage;
	CSkinImage    m_BagImage;
	CSkinImage    m_AppendImage;
	CSkinImage	  m_BackImage;
	BYTE    m_StarFlash;          //�������ǵ���˸
	double						m_iPower;					//����
	BITMAP						BallBmpInfo;			//��ͼ��Ϣ
	BITMAP						BallABmpInfo;			//��ͼ��Ϣ
	BITMAP						BallInBagBmpInfo;			//��ͼ��Ϣ
	BITMAP						BagBmpInfo;				//��ͼ��Ϣ
	BITMAP						PoleBmpInfo;			//��ͼ��Ϣ
};

#endif