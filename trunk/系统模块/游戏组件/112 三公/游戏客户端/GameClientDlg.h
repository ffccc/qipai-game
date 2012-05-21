#pragma once

#include "Stdafx.h"
#include "GameLogic.h"
#include "GameClientView.h"

//////////////////////////////////////////////////////////////////////////

//��Ϸ�Ի���
class CGameClientDlg : public CGameFrameDlg
{
	//��Ԫ����
	friend class CGameClientView;

	//��Ϸ����
protected:
	WORD							m_wBankerUser;						//ׯ��λ��
	BYTE							m_cbPlayStatus[GAME_PLAYER];		//��Ϸ״̬

	//��ʷ��¼
protected:
	WORD							m_wDrawCount;						//��Ϸ����
	LONG							m_lGameScore;						//��Ϸ�ɼ�

	//��ע��Ϣ
protected:
	LONG							m_lMaxScore;						//�����ע
	LONG							m_lLessScore;						//������ע

	//��������
protected:
	bool							m_bGameScene;						//������־
	LONG							m_lChipInCount[5];					//������Ŀ
	TCHAR							m_szAccounts[GAME_PLAYER][NAME_LEN];//�������

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

	//���غ���
protected:
	//ȡ����Ϣ
	virtual void OnCancel();

	//��Ϣ����
protected:
	//ʱ����Ϣ
	bool OnSubTimeInfo(const void * pBuffer, WORD wDataSize);
	//��ע��Ϣ
	bool OnSubScoreInfo(const void * pBuffer, WORD wDataSize);
	//ׯ����Ϣ
	bool OnSubBankerUser(const void * pBuffer, WORD wDataSize);
	//��ׯ��Ϣ
	bool OnSubChangeInfo(const void * pBuffer, WORD wDataSize);
	//��ע��Ϣ
	bool OnSubUserChipin(const void * pBuffer, WORD wDataSize);
	//�����˿�
	bool OnSubSendCard(const void * pBuffer, WORD wDataSize);
	//��Ϸ��ʼ
	bool OnSubGameStart(const void * pBuffer, WORD wDataSize);
	//��Ϸ����
	bool OnSubGameEnd(const void * pBuffer, WORD wDataSize);

	//���ܺ���
public:
	//���³���
	void UpdateClipinText(LONG lMaxScore);
	//���³���
	void UpdateClipinStatus(LONG lLeaveScore);

	//��Ϣӳ��
protected:
	//��ʼ��Ϣ
	LRESULT OnStart(WPARAM wParam, LPARAM lParam);
	//��ע��Ϣ
	LRESULT OnScore(WPARAM wParam, LPARAM lParam);
	//�����Ϣ
	LRESULT OnClean(WPARAM wParam, LPARAM lParam);
	//��ע��Ϣ
	LRESULT OnClipin(WPARAM wParam, LPARAM lParam);
	//��ׯ��Ϣ
	LRESULT OnBanker(WPARAM wParam, LPARAM lParam);
	//������Ϣ
	LRESULT OnContinue(WPARAM wParam, LPARAM lParam);
	//�������
	LRESULT OnSendCardFinish(WPARAM wParam, LPARAM lParam);
	//�������
	LRESULT OnReclaimCardFinish(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
