#pragma once

#include "Stdafx.h"
#include "GameClientView.h"
#include "GameLogic.h"
#include "ShowHand.h"

//////////////////////////////////////////////////////////////////////////

//��Ϸ�Ի���
class CGameClientDlg : public CGameFrameDlg
{
	//��Ϸ����
protected:
	WORD							m_wMeChairID;						//����λ��
	WORD							m_wDUser;							//D���
	WORD							m_wCurrentUser;						//��ǰ���
	bool							m_bOpenCard;						//����
	bool							m_bExitTag;							//ǿ�˱�־
	bool							m_bReset;							//��λ��־
	TCHAR							m_bUserName[GAME_PLAYER][32];		//�û�����			

	//��ע��Ϣ
protected:
	LONG							m_lCellScore;						//��Ԫ��ע
	LONG							m_lTurnLessScore;					//��С��ע
	LONG							m_lTurnMaxScore;					//�����ע
	LONG							m_lAddLessScore;					//����Сע
	LONG							m_lTableScore[GAME_PLAYER];			//��ע��Ŀ
	LONG							m_lTotalScore[GAME_PLAYER];			//�ۼ���ע
	LONG							m_lCenterScore;						//���ĳ���
	LONG							m_lBalanceScore;					//ƽ�����

	//״̬����
protected:
	BYTE							m_cbPlayStatus[GAME_PLAYER];			//��Ϸ״̬
	BOOL							m_bAutoStart;							//�Զ���ʼ
	LONG							m_dEndScore[GAME_PLAYER];				//��������

	//�˿���Ϣ
protected:
	BYTE							m_cbCenterCardData[MAX_CENTERCOUNT];	//�����˿�
	BYTE							m_cbHandCardData[GAME_PLAYER][MAX_COUNT];//�����˿�
	BYTE							m_cbOverCardData[GAME_PLAYER][MAX_CENTERCOUNT];//�����˿�

	//�ؼ�����
public:
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
	//�����Ϣ
	virtual bool OnFrameMessage(WORD wSubCmdID, const void * pBuffer, WORD wDataSize);

	//��Ϣ����
protected:
	//��Ϸ��ʼ
	bool OnSubGameStart(const void * pBuffer, WORD wDataSize);
	//�û���ע
	bool OnSubAddScore(const void * pBuffer, WORD wDataSize);
	//�û�����
	bool OnSubGiveUp(const void * pBuffer, WORD wDataSize);
	//������Ϣ
	bool OnSubSendCard(const void * pBuffer, WORD wDataSize);
	//��Ϸ����
	bool OnSubGameEnd(const void * pBuffer, WORD wDataSize);
	//������Ϣ
	bool OnSubOpenCard(const void * pBuffer, WORD wDataSize);

	//��������
protected:
	//���ؿ���
	void HideScoreControl();
	//���¿���
	void UpdateScoreControl();

	//��Ϣӳ��
protected:
	//��ʼ��ť
	LRESULT	OnStart(WPARAM wParam, LPARAM lParam);
	//�뿪��Ŧ
	LRESULT OnExit(WPARAM wParam,LPARAM lParam);
	//������Ŧ
	LRESULT OnGiveUp(WPARAM wParam,LPARAM lParam);
	//��ע��ť 
	LRESULT	OnAddScore(WPARAM wParam, LPARAM lParam);
	//���°�ť
	LRESULT	OnMinScore(WPARAM wParam, LPARAM lParam);
	//���ť
	LRESULT	OnMaxScore(WPARAM wParam, LPARAM lParam);
	//ȷ����ť
	LRESULT OnOKScore(WPARAM wParam,LPARAM lParam);
	//ȡ����ť
	LRESULT OnCancelScore(WPARAM wParam,LPARAM lParam);
	//���ư�ť
	LRESULT OnPassCard(WPARAM wParam,LPARAM lParam);
	//��ע��ť
	LRESULT OnFollow(WPARAM wParam,LPARAM lParam);
	//�����ť
	LRESULT OnShowHand(WPARAM wParam,LPARAM lParam);
	//��ʼ����
	LRESULT OnStartTimes(WPARAM wParam,LPARAM lParam);
	//�Զ���ʼ
	LRESULT OnAutoStart(WPARAM wParam,LPARAM lParam);
	//���°�ť
	LRESULT OnSitDown(WPARAM wParam,LPARAM lParam);
	//��Ϸ����
	LRESULT OnGameOver(WPARAM wParam,LPARAM lParam);
	//��Ϸ����
	LRESULT OnSendFinish(WPARAM wParam,LPARAM lParam);
	//������Ϣ
	LRESULT OnOpenCard(WPARAM wParam,LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
