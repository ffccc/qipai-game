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
	//���ñ���
protected:
	DWORD							m_dwCardHSpace;							//�˿�����
	WORD							m_wViewChairID[GAME_PLAYER];			//��ͼλ��

	//��ע��Ϣ
protected:
	LONG							m_lTurnMaxScore;						//�����ע
	LONG							m_lTableScore[GAME_PLAYER];				//��ע��Ŀ

	//״̬����
protected:
	WORD							m_wBankerUser;							//ׯ���û�
	BYTE							m_cbPlayStatus[GAME_PLAYER];			//��Ϸ״̬
	TCHAR							m_szAccounts[GAME_PLAYER][NAME_LEN];	//�������

	//�û��˿�
protected:
	BYTE							m_cbHandCardData[GAME_PLAYER][MAX_COUNT];//�û�����
	BYTE							m_bUserOxCard[GAME_PLAYER];				//ţţ����								

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
	//�û���ׯ
	bool OnSubCallBanker(const void * pBuffer, WORD wDataSize);
	//��Ϸ��ʼ
	bool OnSubGameStart(const void * pBuffer, WORD wDataSize);
	//�û���ע
	bool OnSubAddScore(const void * pBuffer, WORD wDataSize);
	//������Ϣ
	bool OnSubSendCard(const void * pBuffer, WORD wDataSize);
	//�û�̯��
	bool OnSubOpenCard(const void * pBuffer, WORD wDataSize);
	//�û�ǿ��
	bool OnSubPlayerExit(const void * pBuffer, WORD wDataSize);
	//��Ϸ����
	bool OnSubGameEnd(const void * pBuffer, WORD wDataSize);

	//��������
protected:
	//���¿���
	void UpdateScoreControl(LONG lScore[],BOOL bShow);

	//��Ϣӳ��
protected:
	//��ʾ��ť
	LRESULT	OnHintOx(WPARAM wParam, LPARAM lParam);
	//��ʼ��ť
	LRESULT	OnStart(WPARAM wParam, LPARAM lParam);
	//��ע��ť 
	LRESULT	OnAddScore(WPARAM wParam, LPARAM lParam);
	//ţţ��ť
	LRESULT	OnOx(WPARAM wParam, LPARAM lParam);
	//��ݼ���ť 
	LRESULT OnShortcut(WPARAM wParam, LPARAM lParam);
	//̯�ư�ť
	LRESULT	OnOpenCard(WPARAM wParam, LPARAM lParam);
	//�������
	LRESULT OnSendCardFinish(WPARAM wParam, LPARAM lParam);
	//��ʾ����
	LRESULT OnSetUserOxValue(WPARAM wParam, LPARAM lParam);
	//��ʾ��Ϣ
	LRESULT OnShowInfo(WPARAM wParam, LPARAM lParam);
	//ţBTΪ��
	LRESULT OnOxEnable(WPARAM wParam, LPARAM lParam);
	//�˿˷���
	LRESULT OnSortCard(WPARAM wParam, LPARAM lParam);
	//�˿˷���
	LRESULT OnReSortCard(WPARAM wParam, LPARAM lParam);
	//��ׯ��Ϣ
	LRESULT OnBanker(WPARAM wParam, LPARAM lParam);

public:
	afx_msg void OnTimer(UINT nIDEvent);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
