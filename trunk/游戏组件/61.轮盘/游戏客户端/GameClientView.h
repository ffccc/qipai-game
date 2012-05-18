#pragma once

#include "Stdafx.h"
#include "ScoreView.h"
#include "dxgameview.h"

//////////////////////////////////////////////////////////////////////////
//��Ϣ����

#define IDM_START					(WM_USER+100)						//��ʼ��Ϣ

//////////////////////////////////////////////////////////////////////////

//��Ϸ��ͼ
class CGameClientView : public CGameFrameView
{
	//��Ϸ����
protected:


	//���ñ���
protected:
	bool							m_bDeasilOrder;						//����˳��

	//״̬����
public:


	//λ����Ϣ
public:

	//λͼ����
protected:


	//��ť�ؼ�
public:


	//�˿˿ؼ�
public:


	//�ؼ����
public:
	CScoreView						m_ScoreView;						//������ͼ
	CDXGameView						m_DxDialog;							//d3d����

	//��������
public:
	//���캯��
	CGameClientView();
	//��������
	virtual ~CGameClientView();

	//�̳к���
private:
	//���ý���
	virtual void ResetGameView();
	//�����ؼ�
	virtual void RectifyGameView(int nWidth, int nHeight);
	//�滭����
	virtual void DrawGameView(CDC * pDC, int nWidth, int nHeight);

	//���ú���
public:
	//�û�˳��
	void SetUserOrder(bool bDeasilOrder);
	//��ȡ˳��
	bool IsDeasilOrder() { return m_bDeasilOrder; }

	//���ܺ���
public:
	void SetUserCome(BYTE bHistoryInfo[HISTORY_NUMBER],long lMinBetScore,long lMaxBetWinScore,long lMaxUserScore,bool bLookOnUser=false);
	void SetGameStart(BYTE bTargetNumber);
	void SetUserChip(long lAllChip[256]);
	void SetRollStop(BYTE bWinNumber);
	void SetGameEnd(long lScore);
	void SetCellScore(long lCellScore);
	//��Ϣӳ��
protected:
	//��ʼ��ť
	afx_msg void OnStart();

	//��������
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	afx_msg void OnTimer(UINT_PTR nIDEvent);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
