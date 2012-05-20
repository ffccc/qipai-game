#pragma once

#include "Stdafx.h"
#include "GameLogic.h"
#include "GameClientView.h"

//////////////////////////////////////////////////////////////////////////

//��Ϸ�Ի���
class CGameClientDlg : public CGameFrameDlg
{
	//��Ϸ����
protected:
	WORD							m_wBankerUser;						//ׯ���û�

	//��Ϸ����
protected:
	WORD							m_wCurrentUser;						//��ǰ�û�

	//ʤ����Ϣ
protected:
	WORD							m_wWinCount;						//ʤ������
	WORD							m_wWinOrder[GAME_PLAYER];			//ʤ���б�

	//�˿˱���
protected:
	BYTE							m_cbHandCardData[MAX_COUNT];		//�����˿�
	BYTE							m_cbHandCardCount[GAME_PLAYER];		//�˿���Ŀ
	BYTE							m_cbPartnerCardData[MAX_COUNT];		//�ϻ����˿�

	//���Ʊ���
protected:
	BYTE							m_cbTurnCardCount;					//������Ŀ
	BYTE							m_cbTurnCardData[MAX_COUNT];		//�����б�
	BYTE							m_cbMagicCardData[MAX_COUNT];		//����˿�

	//���ñ���
protected:
	bool							m_bStustee;							//�йܱ�־
	bool							m_bDeasilOrder;						//����˳��
	DWORD							m_dwCardHSpace;						//�˿�����

	//��������
protected:
	BYTE							m_cbRemnantCardCount;				//ʣ����Ŀ
	BYTE							m_cbDispatchCardCount;				//�ɷ���Ŀ
	BYTE							m_cbDispatchCardData[MAX_COUNT];	//�ɷ��˿�
	WORD							m_wMostUser;						//������
	WORD							m_wTimeOutCount;					//��ʱ����

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
	//��������
	bool OnSubPassCard(const void * pBuffer, WORD wDataSize);
	//��Ϸ����
	bool OnSubGameEnd(const void * pBuffer, WORD wDataSize);
	//�û��й�
	bool OnSubTrustee(const void * pBuffer,WORD wDataSize);

	//��������
protected:
	//�����ж�
	bool VerdictOutCard();
	//�����ж�
	bool VerdictPassCard();
	//�Զ�����
	bool AutomatismOutCard();
	//ֹͣ����
	bool DoncludeDispatchCard();
	//�ɷ��˿�
	bool DispatchUserCard(BYTE cbCardData[], BYTE cbCardCount);
	//ը������
	void PlayBombSound(WORD wChairID, BYTE cbStarLevel);

	//��Ϣӳ��
protected:
	//��ʱ����Ϣ
	afx_msg void OnTimer(UINT nIDEvent);
	//��ʼ��Ϣ
	LRESULT OnStart(WPARAM wParam, LPARAM lParam);
	//������Ϣ
	LRESULT OnOutCard(WPARAM wParam, LPARAM lParam);
	//��������
	LRESULT OnPassCard(WPARAM wParam, LPARAM lParam);
	//������ʾ
	LRESULT OnOutPrompt(WPARAM wParam, LPARAM lParam);
	//��ɫ����
	LRESULT OnSortByColor(WPARAM wParam, LPARAM lParam);
	//��С����
	LRESULT OnSortByValue(WPARAM wParam, LPARAM lParam);
	//�Ҽ��˿�
	LRESULT OnLeftHitCard(WPARAM wParam, LPARAM lParam);
	//����˿�
	LRESULT OnRightHitCard(WPARAM wParam, LPARAM lParam);
	//�Ϲܿ���
	LRESULT OnStusteeControl(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
