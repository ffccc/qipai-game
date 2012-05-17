#pragma once

#include "Stdafx.h"
#include "GameLogic.h"
#include "GameClientView.h"

/*
���⣺
1.         û��ͣ�ơ�
//.���ơ���������

2.         û�л��ƣ�����ֻʣ��10�㣨����20���ƣ�ʱ��Ӧ���ֻ��ơ�
//ʣ������


3.         ɫ��ֻ��һ�飬���������


4.         �Լ����ȥ���ƣ�ֻ�����������˴������ʱ�򣬲����ٺ��ơ�

5.         ������ʾ���ƺ��ܼ������ơ�

1��ɫ�ӡ������� ��2�Ρ�����
ׯ�ҡ�������
6+7��������

17*
14*��������

1.ׯ��˦ɫ��-----�����ļ�����    12
2.��˦ɫ��------>�������١�����  12+12

2�����ơ���������

3����ô��ׯ��������
������ׯ������

1�����������һ����������
2������Ĳ��ܺ�����������
�Լ�����Ĳ��ܺ���������

3.���ơ�����

4.ʣ10���ƣ��ͻ��ơ�������

5.����3�̾��⡣����
*/
//////////////////////////////////////////////////////////////////////////

//��Ϸ�Ի���
class CGameClientDlg : public CGameFrameDlg
{
private:
	CMD_S_GameStart  myGameStart;
	BYTE cbSiceFirst;//��һ��
	//�Ƿ��һ���ƶ�
	int uiShowInt;
	BYTE cbSiceSecond;//�ڶ���

	BYTE cbSice2First;//��һ��
	BYTE cbSice2Second;//�ڶ���

public:
//��ʾ�û�MJ
	void ShowUserMJ();
	//������Ƶ����
	void ResetVideoWindowPostion(int wInt, int hInt);


	//�û�����
protected:
	WORD							m_wBankerUser;						//ׯ���û�
	WORD							m_wCurrentUser;						//��ǰ�û�

	//״̬����
protected:
	bool							m_bHearStatus;						//����״̬

	//��������
protected:
	WORD							m_wHeapHand;						//����ͷ��
	WORD							m_wHeapTail;						//����β��
	BYTE							m_cbHeapCardInfo[4][2];				//������Ϣ   ͷ����ȱ,β����ȱ

	//�齫����
protected:
	BYTE							m_cbLeftCardCount;					//ʣ����Ŀ
	BYTE							m_cbCardIndex[MAX_INDEX];			//�����齫

	//����齫
protected:
	BYTE							m_cbWeaveCount[GAME_PLAYER];		//�����Ŀ
	tagWeaveItem					m_WeaveItemArray[GAME_PLAYER][4];	//����齫

	//������Ϣ
protected:
	WORD							m_wOutCardUser;						//�����û�
	BYTE							m_cbOutCardData;					//�����齫
	BYTE							m_cbDiscardCount[GAME_PLAYER];		//������Ŀ
	BYTE							m_cbDiscardCard[GAME_PLAYER][55];	//������¼

	//�ؼ�����
protected:
	CGameLogic						m_GameLogic;						//��Ϸ�߼�
public:
	CGameClientView					m_GameClientView;					//��Ϸ��ͼ
	
	//��������
public:
	//���캯��
	CGameClientDlg();
	//��������
	virtual ~CGameClientDlg();

	//����̳�
private:
	//��ʼ����
	virtual bool InitGameFrame();
	//���ÿ��
	virtual void ResetGameFrame();
	//��Ϸ����
	virtual void OnGameOptionSet();
	//ʱ����Ϣ
	virtual bool OnTimerMessage(WORD wChairID, UINT nElapse, UINT nTimerID);
	//�Թ�״̬
	virtual void OnLookonChanged(bool bLookonUser, const void * pBuffer, WORD wDataSize);
	//������Ϣ
	virtual bool OnGameMessage(WORD wSubCmdID, const void * pBuffer, WORD wDataSize);
	//��Ϸ����
	virtual bool OnGameSceneMessage(BYTE cbGameStatus, bool bLookonOther, const void * pBuffer, WORD wDataSize);

	//��Ϣ����
protected:
	//��Ϸ��ʼ
	bool OnSubGameStart(const void * pBuffer, WORD wDataSize);
	//�û�����
	bool OnSubOutCard(const void * pBuffer, WORD wDataSize);
	//������Ϣ
	bool OnSubSendCard(const void * pBuffer, WORD wDataSize);
	//������ʾ
	bool OnSubOperateNotify(const void * pBuffer, WORD wDataSize);
	//�������
	bool OnSubOperateResult(const void * pBuffer, WORD wDataSize);
	//��Ϸ����
	bool OnSubGameEnd(const void * pBuffer, WORD wDataSize);
	//����
	bool OnSubTingCard(const void * pBuffer, WORD wDataSize);
	//��������
protected:
	//��������
	//Ĭ��Ϊ����
	void PlayCardSound(BYTE cbCardData, int sexInt = 1);
	//��������
	void PlayActionSound(BYTE cbAction , int sexInt = 1);
	//�۳��齫
	void DeductionTableCard(bool bHeadCard);
	//��ʾ����
	bool ShowOperateControl(BYTE cbUserAction, BYTE cbActionCard);

	//��Ϣӳ��
protected:
	//
	LRESULT OnShowSZ(WPARAM wParam, LPARAM lParam);
	//���ư�ť
	LRESULT OnTingCard(WPARAM wParam, LPARAM lParam);
	//��ʼ��Ϣ
	LRESULT OnStart(WPARAM wParam, LPARAM lParam);
	//���Ʋ���
	LRESULT OnOutCard(WPARAM wParam, LPARAM lParam);
	//�齫����
	LRESULT OnCardOperate(WPARAM wParam, LPARAM lParam);

	//RESET UI
	LRESULT OnResetUI(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
