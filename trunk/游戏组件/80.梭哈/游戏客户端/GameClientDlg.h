#pragma once

#include "Stdafx.h"
#include "GameClientView.h"

//////////////////////////////////////////////////////////////////////////

//��Ϸ�Ի���
class CGameClientDlg : public CGameFrameDlg
{
	//��Ϸ����
protected:
	LONG							m_lTurnMaxGold;						//�����ע
	LONG							m_lTurnBasicGold;					//��ע��Ŀ
	LONG							m_lBasicGold;						//��Ԫ��Ŀ
	BYTE							m_bPlayStatus[4];					//��Ϸ״̬
	WORD							m_wCurrentUser;						//

	TCHAR							m_szAccounts[GAME_PLAYER][NAME_LEN];

	//��������
protected:
	bool							m_bShowHand;						//�Ƿ����
	bool							m_bMeAddGold;						//�Ƿ���ע
	bool							m_bAddGold;							//�Ƿ�����ע
	LONG							m_lGoldShow;						//��ע����
	TCHAR							m_szName[GAME_PLAYER][NAME_LEN];	//�������

	//�ؼ�����
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
	//�û���ע
	bool OnSubAddGold(const void * pBuffer, WORD wDataSize);
	//�û�����
	bool OnSubGiveUp(const void * pBuffer, WORD wDataSize);
	//������Ϣ
	bool OnSubSendCard(const void * pBuffer, WORD wDataSize);
	//��Ϸ����
	bool OnSubGameEnd(const void * pBuffer, WORD wDataSize);

	//��Ϣӳ��
protected:
	//��ʼ��ť
	LRESULT	OnStart(WPARAM wParam, LPARAM lParam);
	//������ť
	LRESULT OnGiveUp(WPARAM wParam, LPARAM lParam);
	//��ע��ť
	LRESULT OnFollow(WPARAM wParam, LPARAM lParam);
	//�����ť
	LRESULT OnShowHand(WPARAM wParam, LPARAM lParam);
	//��ע��Ϣ
	LRESULT OnAddGold(WPARAM wParam, LPARAM lParam);
	//�������
	LRESULT OnSendCardFinish(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
