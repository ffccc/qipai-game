#ifndef THREADDRAW_HEAD_FILE
#define THREADDRAW_HEAD_FILE

#pragma once

#include "StdAfx.h"
#include "ChessView.h"
#include "BreviaryChessView.h"
#define   MSCREENPOSY      80   

class CGameClientView;

struct DrawInfoStruct
{
	//��־����
	bool						bChangeData;			//�Ƿ��������

	//��������
	RECT						ClientRect;				//�ͻ���λ��

	//һ���Գ�ʼ������ 
	CChessView					*m_ChessView;			//������ͼ
	CGameClientView				*pPlayView;				//��Ϸ��ͼָ��

	DBall						*m_Chess;			//������
	BreviaryChessView			*m_BreviaryView[3];	//��������

	CDC							memDC;				//�߿򻺳�
	bool						ismemup;			//�����Ƿ����
	bool						isbomb;				//�Ƿ�ը��
	bool						isstart;			//��ʾ��Ϸ��ʼ

	bool						start_do;			//��ҫ����

	//�߳�ʹ�ò���
	long int					dwSpeed;				//��ʾ֡��

	int							m_XBoard;				//�߿�x
	int							m_YBoard;				//�߿�y
	int							m_ChessBoardX;			//���̱߿�
	int							m_ChessBoardY;
	int							m_sChessBoardX;			//�������̱߿�
	int							m_sChessBoardY;			//�������̱߿�
	int							m_ChessPos_X;
	int							m_ChessPos_Y;
	int							kx;						//�����϶
	int 						m_ViewWidth;
	int 						m_ViewHeight;

	int							m_start;

	BYTE						m_Result;				//
	DBall						m_ResultColor;
	DBall						*m_nextballcolor;

	int							m_TimerCount;			//��ʱ��

	CPoint						m_nextballpos;
	CPoint						m_PropPos;
	CPoint						m_BombPos;
	CPoint						m_SetStayPos;

	int							m_succ;
	bool						isHitToo;
	CPoint						m_TextScorePos;
	BYTE						m_TextScorePosKx;
    TCHAR						m_TextScore[40];
	TCHAR						m_TextHitToo[40];
	bool						m_isGetSelect;
	bool						m_isGetBomb;
	bool						m_isGetMagic;
	bool						m_isGetStay;
	bool						m_GameTimeOut;
	bool						m_isSetStay;
	TCHAR						m_TextGetSelect[40];
	TCHAR						m_TextGetBomb[40];
	TCHAR						m_TextGetMagic[40];
	TCHAR						m_TextGetStay[40];

	bool						m_TimeStay;
	TCHAR						m_TimeStayString[40];    //ʱ��ħ��

	//�ٷֱ�
	bool						m_HundredDisplayed;

	//����ͼƬ����
	int						m_succ_up;

	//��ϰģʽ
	bool						m_Exered;

	//�Ƿ���Ϸ������ʾӮ
	bool						m_isGameOver;
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
	 void DrawViewFrameAndBg(CDC *pDC);
	void GDIModeDraw(CDC * pDC, DrawInfoStruct * pDrawInfo);
	void GDIModeInitThread(CDC * pDrawDC,DrawInfoStruct * pDrawInfo);
private:
	CSkinImage										m_ImageBkg;								//����
	CSkinImage										m_ImageChessboard;						//����
	CSkinImage										m_ImageFrame;

	CSkinImage										m_ImageTalk;						//����	

	CSkinImage										m_ImageChessman;						//����
	CSkinImage										m_ImageChessman2;						//��ҫ����

	CSkinImage										m_ImageMARK;							//��־
	CSkinImage										m_ImageProp;							//����
	CSkinImage										m_ImageBomb;							//ը��

	CSkinImage										m_ImageTimer1;							//��ʱ��
	CSkinImage										m_ImageTimer2;							//

	CSkinImage										m_sImageChessboard;						//��������
	CSkinImage										m_sImageChessman;						//��������

	CSkinImage										m_ImageMemA;							//�Ŷ���Ϣ
	CSkinImage										m_ImageMemB;							//

	CSkinImage										m_ImageBallImplement;
	CSkinImage										m_ImageBallImplementPick;

	CSkinImage										m_ImageLose;
	CSkinImage										m_ImageWin;

	CSkinImage										m_ImageRate;
	CSkinImage										m_ImageRate_B;

	CSkinImage										m_ImageDeadBall1;
	CSkinImage										m_ImageDeadBall2;

	CFont LittleFont;
	CFont BigFont;
	CFont MidFont;
	long int dwSpeed;
	long int dwBeginTime;
	long int dwEndTime;
public:
	void DrawBlog(CDC * pDC, int type, int x, int y);
};

#endif