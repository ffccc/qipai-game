#ifndef GAME_CLIENT_ENGINE_HEAD_FILE
#define GAME_CLIENT_ENGINE_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "..\���������\GameLogic.h"
#include "HistoryRecord.h"
#include "GameClientView.h"
#include "GameOption.h"


//////////////////////////////////////////////////////////////////////////

//��Ϸ����
class CGameClientDlg : public CGameFrameDlg
{
	//��Ϸ����
protected:
	BYTE							m_cbMainValue;						//������ֵ
	WORD							m_wCurrentUser;						//��ǰ�û�
	BYTE							m_cbValueOrder[GAME_PLAYER];		//�ȼ���ֵ
	bool							m_bAutoStatus[GAME_PLAYER];			//�й�״̬
	int								m_TurnScore;						//���ַ���
	int								m_PlayerScore[GAME_PLAYER];			//�û�����

	BYTE							m_bTongHuaCount;
	BYTE							m_bTongHuaIndex;
	BYTE							m_bTongHuaCard[100][5];

	//ʤ����Ϣ
protected:
	WORD							m_wWinCount;						//ʤ������
	WORD							m_wWinOrder[GAME_PLAYER];			//ʤ���б�

	//�˿˱���
protected:
	BYTE							m_cbHandCardData[MAX_COUNT];		//�����˿�
	BYTE							m_cbHandCardCount[GAME_PLAYER];		//�˿���Ŀ

	//���Ʊ���
protected:
	BYTE							m_cbTurnCardType;					//�˿�����
	BYTE							m_cbTurnCardCount;					//������Ŀ
	BYTE							m_cbTurnCardData[MAX_COUNT];		//�����б�
	BYTE							m_cbMagicCardData[MAX_COUNT];		//����˿�

	//��������
protected:
	bool							m_bStustee;							//�йܱ�־
	bool							m_bLastTurn;						//���ֱ�־
	WORD							m_wMostUser;						//������
	BYTE							m_cbSortType;						//��������
	WORD							m_wTimeOutCount;					//��ʱ����

	//���Ʊ���
protected:
	BYTE							m_cbRemnantCardCount;				//ʣ����Ŀ
	BYTE							m_cbDispatchCardCount;				//�ɷ���Ŀ
	BYTE							m_cbDispatchCardData[MAX_COUNT];	//�ɷ��˿�

	//���ñ���
protected:
	DWORD							m_dwCardHSpace;						//�˿�����

	//�������
protected:
	CHistoryCard					m_HistoryCard;						//��ʷ�˿�

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
	bool OnSubGameStart(const void * pData, WORD wDataSize);
	//�û�����
	bool OnSubOutCard(const void * pData, WORD wDataSize);
	//�û�����
	bool OnSubPassCard(const void * pData, WORD wDataSize);
	//�˿���Ϣ
	bool OnSubCardInfo(const void * pData, WORD wDataSize);
	//��Ϸ����
	bool OnSubGameEnd(const void * pData, WORD wDataSize);
	//������
	bool OnSubRequestLook(const void * pData, WORD wDataSize);
	//�����뿪
	bool OnSubRequestLeave(const void * pData, WORD wDataSize);
	//������Ϸ
	bool OnSubContinueGame(const void * pData, WORD wDataSize);

	bool OnSubUserAutomatism(const void * pBuffer, WORD wDataSize);
	//���ܺ���
protected:
	//�����ж�
	bool VerdictOutCard();
	//�����ж�
	bool VerdictPassCard();

	//���ܺ���
protected:
	//�Զ���ʼ
	bool PerformAutoStart();
	//�Զ�����
	bool AutomatismOutCard();
	//��ǰ�˿�
	bool SwitchToCurrentCard();
	//ֹͣ����
	bool DoncludeDispatchCard();
	//�ɷ��˿�
	bool DispatchUserCard(BYTE cbCardData[], BYTE cbCardCount);

	//��Ϣӳ��
protected:
	//ʱ����Ϣ
	VOID OnTimer(UINT nIDEvent);
	//��ʼ��Ϣ
	LRESULT OnMessageStart(WPARAM wParam, LPARAM lParam);
	//������Ϣ
	LRESULT OnMessageOutCard(WPARAM wParam, LPARAM lParam);
	//�Ϲܿ���
	LRESULT OnMessageStustee(WPARAM wParam, LPARAM lParam);
	//�����˿�
	LRESULT OnMessageSortCard(WPARAM wParam, LPARAM lParam);
	//�鿴����
	LRESULT OnMessageLastTurn(WPARAM wParam, LPARAM lParam);
	//PASS��Ϣ
	LRESULT OnMessagePassCard(WPARAM wParam, LPARAM lParam);
	//��ʾ��Ϣ
	LRESULT OnMessageOutPrompt(WPARAM wParam, LPARAM lParam);
	//������Ϣ
	LRESULT OnMessageThrowCard(WPARAM wParam, LPARAM lParam);
	//�ع���Ϣ
	LRESULT OnMessageIncepCard(WPARAM wParam, LPARAM lParam);
	//����˿�
	LRESULT OnMessageLeftHitCard(WPARAM wParam, LPARAM lParam);
	//��������
	LRESULT OnMessageEnableHistory(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif