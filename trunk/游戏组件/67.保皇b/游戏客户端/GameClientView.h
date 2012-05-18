#pragma once

#include "Stdafx.h"
#include "ScoreView.h"
#include "CardControl.h"

//#define DEBUG_GAME
//////////////////////////////////////////////////////////////////////////
//��Ϣ����

#define IDM_START					(WM_USER+100)						//��ʼ��Ϣ
#define IDM_OUT_CARD				(WM_USER+101)						//������Ϣ
#define IDM_PASS_CARD				(WM_USER+102)						//��������
#define IDM_AUTO_OUTCARD			(WM_USER+104)                       //��ʾ��Ϣ
#define IDM_EMPEROR					(WM_USER+105)						//�ʵ�
#define IDM_HOUSECARL				(WM_USER+106)						//����

//////////////////////////////////////////////////////////////////////////

//��Ϸ��ͼ
class CGameClientView : public CGameFrameView
{
	//��Ϸ����
protected:
	WORD							m_wChairID;							//������
	bool							m_bDif;								//�ʵۣ�����һͬ

	//���ñ���
protected:
	bool							m_bDeasilOrder;						//����˳��

	//״̬����
public:
	bool							m_bPass[GAME_PLAYER];				//��������
	BYTE							m_bCardCount[GAME_PLAYER];			//�˿���Ŀ
	BYTE							m_wCurrentEnthronement;				//��ǰ��λ
	BYTE							m_wCurrentHousecarl;				//��ǰ����
	
	//λ����Ϣ
public:
	CPoint							m_ptHeart[GAME_PLAYER];				//��սλ��
	CPoint							m_ptEmp_Hou[2];							//�ʣ�����λ��
	bool							m_bGamePlaying;
	bool							m_bOutHousecarl;
	//λͼ����
protected:
	CSkinImage						m_ImageBack;						//������Դ
	CSkinImage						m_ImageCenter;						//������Դ
	CSkinImage						m_ImageHousecarl;
	CSkinImage						m_ImageEmperor;
	CSkinImage						m_ImageFarmer;

	//��ť�ؼ�
public:
	CSkinButton						m_btStart;							//��ʼ��ť
	CSkinButton						m_btOutCard;						//���ư�ť
	CSkinButton						m_btPassCard;						//������ť
	CSkinButton						m_btAutoOutCard;					//��ʾ��ť
	CSkinButton						m_btEnthronement;					//��λ 
	CSkinButton						m_btDemise;							//��λ 
	CSkinButton						m_btByOneself;						//���� 
	CSkinButton						m_btNotByOneself;					//������ 
	CSkinButton						m_btRebelled;						//�췴
	CSkinButton						m_btNotRebelled;					//���췴
	CSkinButton						m_btKeep;							//����
	CSkinButton						m_btHideKeep;						//����
	//�˿˿ؼ�
public:
	CCardControl					m_HandCardControl;					//�����˿�
	CCardControl					m_UserCardControl[GAME_PLAYER];				//�˿���ͼ
//	CCardControl					m_LeaveCardControl[2];				//�����˿�

	//�ؼ����
public:
	CScoreView						m_ScoreView;						//������ͼ

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
	//���÷���
	void SetPassFlag(WORD wChairID, bool bPass);
	//�˿���Ŀ
	void SetCardCount(WORD wChairID, BYTE bCardCount);
	//���ܰ�ť����
	void SetBT(WORD wChairID,BYTE bIndex);
	
	//��Ϣӳ��
protected:
	//��λ
	afx_msg void OnEnthronement();
	//��λ
	afx_msg void OnDemise();
	//����
	afx_msg void OnByOneself();
	//������
	afx_msg void OnNotByOneself();
	//�췴
	afx_msg void OnReballed();
	//���췴
	afx_msg void OnNotReballed();
	//����
	afx_msg void OnKeep();
	//����
	afx_msg void OnHideKeep();
	//��ʼ��ť
	afx_msg void OnStart();
	//���ư�ť
	afx_msg void OnOutCard();
	//������ť
	afx_msg void OnPassCard();
	//������ʾ
	afx_msg void OnAutoOutCard();
	//��������
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//�����Ϣ
	afx_msg void OnRButtonUp(UINT nFlags, CPoint Point);
	//���˫��
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
