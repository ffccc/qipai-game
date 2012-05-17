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

	//��ע��Ϣ
protected:
	LONG							m_lMaxScore;							//�����ע
	LONG							m_lCellScore;							//��Ԫ��ע
	LONG							m_lTurnMaxScore;						//�����ע
	LONG							m_lTurnLessScore;						//��С��ע
	LONG							m_lTableScore[GAME_PLAYER*2];			//��ע��Ŀ

	//״̬����
protected:
	bool							m_bAddScore;							//��ע��־
	bool							m_bShowHand;							//�����־
	WORD							m_wCurrentUser;							//��ǰ�û�
	BYTE							m_cbPlayStatus[GAME_PLAYER];			//��Ϸ״̬
	TCHAR							m_szAccounts[GAME_PLAYER][NAME_LEN];	//�������

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
	//�û���ע
	bool OnSubAddScore(const void * pBuffer, WORD wDataSize);
	//�û�����
	bool OnSubGiveUp(const void * pBuffer, WORD wDataSize);
	//������Ϣ
	bool OnSubSendCard(const void * pBuffer, WORD wDataSize);
	//��Ϸ����
	bool OnSubGameEnd(const void * pBuffer, WORD wDataSize);

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
	//������ť
	LRESULT OnGiveUp(WPARAM wParam, LPARAM lParam);
	//��ע��ť
	LRESULT OnFollow(WPARAM wParam, LPARAM lParam);
	//��ע��ť 
	LRESULT	OnAddScore(WPARAM wParam, LPARAM lParam);
	//�����ť
	LRESULT OnShowHand(WPARAM wParam, LPARAM lParam);
	//�������
	LRESULT OnSendCardFinish(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
