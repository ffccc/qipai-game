#pragma once

#include "Stdafx.h"
#include "GameLogic.h"
#include "GameClientView.h"

//////////////////////////////////////////////////////////////////////////

//��Ϸ�Ի���
class CGameClientDlg : public CGameFrameDlg
{
	//�û�����
protected:
	WORD							m_wBankerUser;						//ׯ���û�
	WORD							m_wCurrentUser;						//��ǰ�û�

	//״̬����
protected:
	bool							m_bAuto;
	WORD							m_wDoorDiceCount;					//�������Ӵ�С
	WORD							m_wGoldDiceCount;					//�������Ӵ�С
	BYTE							m_cbSubtractTailCount;				//β��ȡ�齫
	BYTE							m_cbSubtractHeadCount;				//ͷ��ȡ�齫
	
	//��������
protected:						
	WORD							m_wHeapHand;						//����ͷ��
	WORD							m_wHeapTail;						//����β��
	BYTE							m_cbHeapCardInfo[4][2];				//������Ϣ


	//�˿˱���
protected:
	BYTE							m_cbLeftCardCount;					//ʣ����Ŀ
	BYTE							m_cbCardIndex[MAX_INDEX];			//�����˿�
	BYTE							m_cbFlowerCard[8];					//�����齫

	//����˿�
protected:
	BYTE							m_cbWeaveCount[GAME_PLAYER];		//�����Ŀ
	tagWeaveItem					m_WeaveItemArray[GAME_PLAYER][5];	//����˿�

	//������Ϣ
protected:
	WORD							m_wOutCardUser;						//�����û�
	BYTE							m_cbOutCardData;					//�����˿�
	BYTE							m_cbDiscardCount[GAME_PLAYER];		//������Ŀ
	BYTE							m_cbDiscardCard[GAME_PLAYER][55];	//������¼
	BYTE							m_cbGoldCard[4];					//�����˿�

	//�ؼ�����
protected:
	CGameLogic						m_GameLogic;						//��Ϸ�߼�
	CGameClientView					m_GameClientView;					//��Ϸ��ͼ

//test
protected:
	//char							*m_cMsg;						//����Ϣ
	
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
	// ����״̬
	bool OnSubOpenDoor(const void * pBuffer, WORD wDataSize);
	// ����״̬
	bool OnSubOpenGold(const void * pBuffer, WORD wDataSize);
	// ��������
	bool OnSubEndFlower(const void * pBuffer, WORD wDataSize);
	// ����״̬
	bool OnSubGoldSwim(const void * pBuffer, WORD wDataSize);

	//��������
protected:
	//��������
	void PlayCardSound(BYTE cbCardData);
	//��������
	void PlayActionSound(BYTE cbAction);
	//�۳��˿�
	void DeductionTableCard(bool bHeadCard);
	//��ʾ����
	bool ShowOperateControl(BYTE cbUserAction, BYTE cbActionCard);

	//��Ϣӳ��
protected:
	//��ʼ��Ϣ
	LRESULT OnStart(WPARAM wParam, LPARAM lParam);
	//���Ʋ���
	LRESULT OnOutCard(WPARAM wParam, LPARAM lParam);
	//�˿˲���
	LRESULT OnCardOperate(WPARAM wParam, LPARAM lParam);
	//�������
	LRESULT OnAuto(WPARAM wParam, LPARAM lParam);
	//���Ų���
	LRESULT OnOpenDoor(WPARAM wParam,LPARAM lParam);
	//�������
	LRESULT OnOpenGold(WPARAM wParam,LPARAM lParam);	

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT nIDEvent);

	
	
	
};

//////////////////////////////////////////////////////////////////////////
