#pragma once

#include "Stdafx.h"
#include "..\��Ϣ����\CMD_QuickRun.h"
#include "ScoreView.h"
#include "TimerControl.h"

//////////////////////////////////////////////////////////////////////////
//��Ϣ����
#define IDC_START						100								//��ʼ��ť
#define IDC_OUT_CARD					101								//���ư�ť
#define IDC_PASS_CARD					102								//PASS��ť

//////////////////////////////////////////////////////////////////////////

//��Ϸ��ͼ
class CGameClientView : public CGameFrameView
{
	//��Ԫ����
	friend class CGameClientDlg;

	//�������
protected:
	//����ͼƬ
	CSkinImage						m_ImageDisk;
	CSkinImage						m_ImageCenter;
	//��ǰ������
	CSkinImage						m_ImageFocus;
	//passͼƬ
	CSkinImage						m_ImagePass;
	//��ͼƬ
	CSkinImage						m_ImagePisa;
	//�ƵĴ�С
	CSize							m_szPisa;
	CSkinButton						m_btStart;							//��ʼ��ť
	CSkinButton						m_btOutCard;						//���ư�ť
	CSkinButton						m_btPassCard;						//Pass��ť
	CTimerControl					m_Timer;							//��ʱ��
	//����Լ�����
	CArrayTemplate<struct tagPisa,struct tagPisa&> m_PisaArray;
	//���˳�����
	CArrayTemplate<struct tagPisa,struct tagPisa&> m_OutPisaArray[GAME_PLAYER];
	//�������ʣ�µ�����,�����Լ�������ֵ��ʹ��
	int m_nPlayerLeftCount[GAME_PLAYER];
	//��������Ƿ�Pass
	BOOL m_bOutPass[GAME_PLAYER];
	int m_nOwnerIndex;
	int m_nFocusIndex;
	//�Ƿ�׼����
	BOOL m_bReady;
	//�Լ����Ƶľ���
	CRect m_rtPisa;
	//��ѡ�е���
	BOOL m_bOutSelect[PISACOUNT];
	//����˳��
	bool m_bDeasilOrder;
	//�Ƿ�տ�ʼ
	BOOL m_bStart;

	//�ؼ�����
public:
	CScoreView						m_ScoreView;						//������ͼ

	//��������
public:
	//���캯��
	CGameClientView();
	//��������
	virtual ~CGameClientView();

	//�û�˳��
	void SetUserOrder(bool bDeasilOrder);
	//��ȡ˳��
	bool IsDeasilOrder() { return m_bDeasilOrder; }
	//��ʾ��ʱ��
	void SetTimerUp(BOOL bUp);
	//���ü�ʱ����ͣ����
	void SetTimerPause(BOOL bPause);
	//���ú���3׼������
	int SetHeiTao3Out();

	//�̳к���
private:
	//���ý���
	virtual void ResetGameView();
	//�����ؼ�
	virtual void RectifyGameView(int nWidth, int nHeight);
	//�滭����
	virtual void DrawGameView(CDC * pDC, int nWidth, int nHeight);
	//��Ϣӳ��
protected:
	//��������
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//��ʼ��ť
	afx_msg void OnStart();
	//���ư�ť
	afx_msg void OnOutCard();
	//������ť
	afx_msg void OnPassCard();

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};

//////////////////////////////////////////////////////////////////////////
