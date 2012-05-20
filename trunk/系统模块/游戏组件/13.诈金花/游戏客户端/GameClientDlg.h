#pragma once

#include "Stdafx.h"
#include "GameLogic.h"
#include "GameClientView.h"

//////////////////////////////////////////////////////////////////////////

//����˵��
typedef CArrayTemplate<CMD_Buffer,CMD_Buffer &> CSocketPacketArray;

//////////////////////////////////////////////////////////////////////////

//��Ϸ�Ի���
class CGameClientDlg : public CGameFrameDlg
{
	//��Ϸ����
protected:
	WORD							m_wCurrentUser;							//��ǰ�û�
	WORD							m_wBankerUser;							//ׯ���û�

	//�û�״̬
protected:
	TCHAR							m_szAccounts[GAME_PLAYER][NAME_LEN];	//�������
	BYTE							m_cbPlayStatus[GAME_PLAYER];			//��Ϸ״̬
	LONG							m_lTableScore[GAME_PLAYER];				//��ע��Ŀ

	//�˿˱���
protected:
	BYTE							m_cbHandCardData[GAME_PLAYER][MAX_COUNT];//�����˿�

	//��ע��Ϣ
protected:
	LONG							m_lMaxScore;							//�ⶥ��Ŀ
	LONG							m_lMaxCellScore;						//��Ԫ����
	LONG							m_lCellScore;							//��Ԫ��ע
	LONG							m_lCurrentTimes;						//��ǰ����
	LONG							m_lUserMaxScore;						//������
	bool							m_bMingZhu[GAME_PLAYER];				//���ƶ���

	//�����û�
protected:
	WORD							m_wLostUser;							//����ʧ��
	WORD							m_wWinnerUser;							//ʤ���û�

	//������Ϣ
protected:
	WORD							m_wLostUserID[GAME_PLAYER];				//�Ȱ��û�

	//������Ϣ
protected:
	WORD							m_wViewChairID[GAME_PLAYER];			//�������

	//���ñ���
protected:
	DWORD							m_dwCardHSpace;							//�˿�����

	//�������
protected:
	CSocketPacketArray				m_SocketPacketArray;					//���ݻ���

	//�ؼ�����
public:
	CGameLogic						m_GameLogic;							//��Ϸ�߼�
	CGameClientView					m_GameClientView;						//��Ϸ��ͼ

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
	bool OnSubGiveUp(const void * pBuffer, WORD wDataSize);
	//�û���ע
	bool OnSubAddScore(const void * pBuffer, WORD wDataSize);
	//�û�����
	bool OnSubLookCard(const void * pBuffer, WORD wDataSize);
	//�û�����
	bool OnSubCompareCard(const void * pBuffer, WORD wDataSize);
	//�û�����
	bool OnSubOpenCard(const void * pBuffer, WORD wDataSize);
	//�û�ǿ��
	bool OnSubPlayerExit(const void * pBuffer, WORD wDataSize);
	//��Ϸ����
	bool OnSubGameEnd(const void * pBuffer, WORD wDataSize);

	//��������
protected:
	//�������
	void ScoreControl(BOOL bShow);
	//��ע����
	void AddScoreControl(BOOL bShow);
	//�������
	void UpdataControl();

	//��Ϣӳ��
protected:
	//��ʼ��ť
	LRESULT	OnStart(WPARAM wParam, LPARAM lParam);
	//��ע��ť 
	LRESULT	OnAddScore(WPARAM wParam, LPARAM lParam);
	//���ټ�ע
	LRESULT	OnMinScore(WPARAM wParam, LPARAM lParam);
	//����ע
	LRESULT	OnMaxScore(WPARAM wParam, LPARAM lParam);
	//ȷ����Ϣ
	LRESULT OnConfirmScore(WPARAM wParam, LPARAM lParam);
	//ȡ����Ϣ
	LRESULT OnCancelAdd(WPARAM wParam, LPARAM lParam);
	//������Ϣ
	LRESULT	OnLookCard(WPARAM wParam, LPARAM lParam);
	//������Ϣ
	LRESULT	OnCompareCard(WPARAM wParam, LPARAM lParam);
	//��ѡ�����û�
	LRESULT OnChooseCUser(WPARAM wParam, LPARAM lParam);
	//������Ϣ
	LRESULT OnOpenCard(WPARAM wParam, LPARAM lParam);
	//������Ϣ
	LRESULT	OnGiveUp(WPARAM wParam, LPARAM lParam);
	//�������
	LRESULT OnSendCardFinish(WPARAM wParam, LPARAM lParam);
	//�������
	LRESULT OnFlashCardFinish(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
