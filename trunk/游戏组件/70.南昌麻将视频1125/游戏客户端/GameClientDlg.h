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

	//��������
protected:
	WORD							m_wHeapHand;						//����ͷ��
	WORD							m_wHeapTail;						//����β��
	BYTE							m_cbHeapCardInfo[4][2];				//������Ϣ


	//������ر���
	WORD							m_wKingHeapPos;						//���ƶ�λ
	WORD							m_wKingPos;							//����λ��
	BYTE							m_cbKingCardIndex[2];				//��������
	WORD							m_wSiceCount3;						//����ɸ��

	//���·�
	LONG							m_lUpDownScore[GAME_PLAYER];		//���·�
	BYTE							m_cbFloor;							//¥��
	BYTE							m_cbUpDownKingCardData;				//���·�����

	//�йܱ���
	bool							m_bStustee;							//�йܱ�־
	WORD							m_wTimeOutCount;					//��ʱ����

	//�˿˱���
protected:
	BYTE							m_cbLeftCardCount;					//ʣ����Ŀ
	BYTE							m_cbCardIndex[GAME_PLAYER][MAX_INDEX];//�����˿�
	BYTE							m_cbCardData[MAX_COUNT];			//�Լ����е��˿�

	//����˿�
protected:
	BYTE							m_cbWeaveCount[GAME_PLAYER];		//�����Ŀ
	tagWeaveItem					m_WeaveItemArray[GAME_PLAYER][4];	//����˿�

	//������Ϣ
protected:
	WORD							m_wOutCardUser;						//�����û�
	BYTE							m_cbOutCardData;					//�����˿�
	BYTE							m_cbDiscardCount[GAME_PLAYER];		//������Ŀ
	BYTE							m_cbDiscardCard[GAME_PLAYER][55];	//������¼
	bool                            m_bStart;                           //ɸ�Ӷ����ǲ��ǽ���
	WORD							m_wUserAction;						//��Ӧ����
	bool							m_bStart2;							//ɸ�Ӷ����ǲ��ǽ���

	//�ؼ�����
protected:
	CGameLogic						m_GameLogic;						//��Ϸ�߼�
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
	//��Ϸϴ��
	bool OnSubGameChip(const void *pBuffer,WORD wDataSize);
	//�û��й�
	bool OnSubTrustee(const void * pBuffer,WORD wDataSize);
	//ϴ����
	bool OnSubBatchResult(const void *pBuffer,WORD wDataSize);

	//��������
protected:
	//������Ϣ
	void SetHandCardData(BYTE cbCardData[], WORD wCardCount, BYTE cbCurrentCard);
	//����֮��ǰ��
	void SetHandCurrentCard(BYTE cbCurrentCard);
	//��������
	void PlayCardSound(WORD wChairID,BYTE cbCardData);
	//��������
	void PlayActionSound(WORD wChairID,WORD wAction);
	//��������
	void PlayHuSound(WORD wChairID,DWORD dwChiHuKind,DWORD dwCHiHuRight);

	//�۳��˿�
	void DeductionTableCard(bool bHeadCard);
	//��ʾ����
	bool ShowOperateControl(WORD wUserAction, BYTE cbActionCard);
	//����У��
	bool VerdictOutCard(BYTE cbPreCardData);
	//��Ϣӳ��
protected:
	//��ʼ��Ϣ
	LRESULT OnStart(WPARAM wParam, LPARAM lParam);
	//���Ʋ���
	LRESULT OnOutCard(WPARAM wParam, LPARAM lParam);
	//�˿˲���
	LRESULT OnCardOperate(WPARAM wParam, LPARAM lParam);
	//ɸ�Ӷ������
	LRESULT OnGameStart(WPARAM wParam, LPARAM lParam);
	//ɸ�Ӷ������
	LRESULT OnGameStart2(WPARAM wParam,LPARAM lParam);
	//ɸ�Ӷ������
	LRESULT OnSiceTwo(WPARAM wParam, LPARAM lParam);
	//��׷���
	LRESULT OnBatch(WPARAM wParam,LPARAM lParam);
	//�Ϲܿ���
	LRESULT OnStusteeControl(WPARAM wParam, LPARAM lParam);
	//ϴ�����
	LRESULT OnBatchEnd(WPARAM wParam,LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
